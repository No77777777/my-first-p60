/*!
   @file
   rfwcdma_mc.c

   @brief
   RF Driver's WCDMA interface file.

   @details
   This file contains all declarations and definitions necessary to use
   the WCDMA Main Control. The client for this file is RFM.
*/

/*===========================================================================
Copyright (c) 1999 - 2017 by QUALCOMM Technologies Inc.  All Rights Reserve

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_wcdma.mpss/3.11/rf/mc/src/rfwcdma_mc.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/27/23   hak     Changes to handle DAC all-combo registers in Tx
06/04/20   nik     Changes to pass sub_id as param to WTR APIs to handle MSIM
01/24/20   nik     Changes for Minidump
01/03/20   nik     Reverting tq alloc/release changes made in CR 2596387
01/03/20   nik     Moving tq alloc and release to wakeup/sleep from enter/exit
05/03/18   nik     Fix for RF leakage issue
03/07/18   nik     QAT3522 Offset loss compensation using Char Data
03/02/18   nik     Adding QAT3522 XSW support for WCDMA
02/22/18   nik     KW error fix
02/07/18   sk      API for WL1 to query NV band mask for a given device.
01/29/18   sk      KW fix to check the Tx handle passed
10/26/17   sk      Enable LTE B71 support
02/21/17   ag      Re-allocate notch cfg in wake-up
01/18/17   sk      Send Tx handle to XPT api's for LTE
01/06/17   ag      Fix for KW errors
12/22/16   bg      Fix for KW errors
09/09/16   ag      DC Loop BW gain changes
07/26/16   vs      Disable ref log captures by setting 3k flag to FALSE if FBRx is disabled
07/21/16   ak      Dynamic notch filter implementation
07/20/16   sk      Addressing KW error for number of channels.
07/01/16   sk      GRFC pin in W Rx path swing during idle if GPS is ON. Updated with correct band
06/10/16   vs      De-init FBRx at Tx deconfig
06/03/16   ag      Fix KW errors
05/19/16   sub     Select the RFFE script buffer for WTR LNA writes instead of GRFC in Rx Tune
                   since using GRFC buffer for RFFE writes would corrupt the GRFC buffer.
05/19/16   sub     Put the systm gain state of eLNA and LNA to default CGAGC gain state after Rx Tune
05/16/16   ak      Change to avoid diversity lna swicth point corruption
05/09/16   sub     RF F3 cleanup
04/28/16   vs      Initialize the MTPL every Tx Config
04/07/16   sd      Reset rflm_fbrx_iqmc during Tx enable 
04/19/16   sub     RF F3 cleanup
04/19/16   sub     Check for AsDiv support in RFC before building scripts
04/04/16   sd      TxAGC log packet 0x1842 v2 changes
04/05/16   ak      FR 32751:Correct the ADIV forced antenna logic when both positions are disabled
04/05/16   ak      FR 32751:SAR based AsDiv Disable changes 
03/24/16   sbo     Move Txagc to Static Scripts in PDMEM.
01/29/16   sd      For Diveristy only mode, get RxAGC info from PATH_1
02/16/16   ag      Remove deallocate static event for autopin in Jacala
02/09/16   vs      Move autopin event to static PDMEM for fast enqueue
02/02/16   nv      Keeping WTR2965 based autopin under feature flag.  
01/28/16   sd      Feature differentiation for WTRs TA2.3
01/18/16   ak      Check for number of valid tasks to identify LNA script type 
12/11/15   ak      Changes to do the Tx config correctly for SC<->DC transition
12/08/15   ak      Moving the WTR LPM feature to NV control
12/03/15   ak      Changes to support Rx/Tx Split 
11/15/15   zhh     remove rfcommon_fbrx_mc_deinit_tx() during tx disable
11/11/15   ag      Changes to check whether WTR supports Autopin
10/28/15   asn     Add critical section for AGC logging when W is in both subs
10/28/15   asn     Add critical section between AOL update and Tx disable
04/11/15   dw      Fix for DCHSUPA FBRx issue
11/03/15   sd      Fix KW error 
09/29/15   ak      Changes to Fix the Calibration failure with WTR LPM changes
10/20/15   asn     Add critical section in RFLM handle cleanup to support W+W
10/08/15   dw      Asdiv support for GRFC switch
10/5/15    sd      FR27546 Update MTPL based on SAR after AsDiv switch
10/06/15   rmb     Fix to build Tune scripts for Primary device followed by Diversity in case of Combined Tune
10/06/15   rmb     Add check for Power Mask between standalone and NULL2W measurement.
09/29/15   sd      FR30081 AsDiv with eLNA gain compensation 
09/24/15   sd      FR30081 add support for AsDiv alt gain param in device APIs
09/28/15   vbh     Use nv container id for Rx V2 NVs
09/21/15   aa      Save TX/FBRx LM buffer info in FBRx state machine
09/22/15   ak      Changes to fetch the BTF value correctly per band
09/16/15   ak      Changes to update the LNA switch points correctly for HS
09/14/15   asn     Ensure dynamic notching happens only in DBDC
09/10/15   rmb     Fix to avoid race condition between Power On and Power off APIs.
08/25/15   sd      Update source band in prep tune before setting current band
03/04/15   ac      grfc support changes
05/27/15   zhh    FBRX ILPC optimize
08/18/15   vbh     In case of SC<->DC transitions skip Tx config chan
08/17/15   ac      for now call it in calc till fw fixes the seq after rach where txagc is called after tx_on
08/13/15   asn     Fix memory exhaustion in prep wakeup and prep tune
08/13/15   ak      Changes to fix the memory leak
08/10/15   ac      timeline opt change, prefetch to be called in tx_enable
08/07/15   ag      Fix KW warning
08/06/15   ag      Changes to fix the issue of secondary carrier and diversity dev not sending disable AGC cmd before enable
08/06/15   ag      Clean up code and make it more robust
08/03/15   ak      Changes to associate the tune scripts to the correct RFLM handle
07/28/15   yb      FBRx VSWR support
07/29/15   ak      Changes to update the RF state correctly 
07/20/15   nv      [TA]Use different setpoints for SC/DC case for TA 
07/27/15  asn      New Power ON OFF API
07/27/15   cdb     Add runtime check for Autopin enabled
07/27/15   rmb     Add antenna index in RxAGC enable parameters
                   and wrap it around a structure.
07/24/15  asn      Fix bug in TQ pair deallocation in exit mode
07/21/15  asn      Fix error in power on/off api
07/20/15   vbh     Add pre-RFC call checks in MC, move carrier mask update to before create scripts
07/20/15   joz     Add call to rfcommon_fbrx_mc_deinit_tx() during TX disable
07/14/15   rc      Add power logging support
07/14/15    aa     Added support for FBRx C1 NV container
07/10/15   ag      Fix to support GRFC eLNA sleep 
07/10/15   ag      Fix band info during lna wakeup in create device scripts.
07/07/15   ac      fix the rx_freq for the tx_rx_ratio
07/22/15   asn     New RxAGC read API
07/01/15   ag      Release critical section in fail scenario in exit mode
6/29/15   vbh      Added PRx device param for enter mode
06/26/15   ac      fix to remove all the handles being deleted at exit
06/25/15   cdb     Autopin fix to process tx handle updates
06/24/15   vs      Fix Tx logging issue in DCUPA configured mode
06/23/15   joz     FR 25072 - Dynamic enable/disable of one of two WTRs on 9x45 based on band needed
06/22/15   vbh     State validation update. 
06/19/15   nv      Added support to dynamically pickup Rxd device based on Prx instead 
                   of harcoded PATH1 for GET_RXD_SUPPORT API
06/17/15   ag      Include parameter for device in set lpm API in all cases
06/17/15   rmb     Add Handle mask for measurement and Critical section for TQ and MDSP
                   allocation/deallocation that happens in standalone and measurement.
06/17/15   vs      Pass PRx device info to vreg manager instead of Tx dev
06/16/15   vbh     Update the mc validate state routine 
06/15/15   ac      extract the carrier_idx for the device for sleep
06/16/15   vs      Pass Tx config mode to HAL layer
06/15/15   aa      autopin feature
06/15/15   vs      Init params of RxAGC cmd for all RFM devices
06/11/15   sub     Update channel info before fbrx tune
06/11/15   sub     Fix to tune fbrx before Tx channel config
06/10/15   rmb     Pass meas device type as MAX in standalone tune.
06/09/15   ag      Fix klocwork warnings
06/10/15   ag      Include W+W feature changes in TA
06/09/15   ag      Fix klocwork warnings
06/04/15   sub     Bug fixes for MTPL and apt only param passed on to FBRx module
06/04/15   ag      Fix comilation error on Tabasco
06/03/15   dw      Fix RxAGC cmd issue with new RFM devices
06/03/15   dw      Fix RxAGC cmd issue with new RFM devices
06/01/15   vbh     Tx Init always use the channel passed by Wl1
06/01/15   sub     Pass max power value to CMN FBRx module based on MTPL NV
06/01/15   sub     Pass APT only configuration info to CMN FBRx module based on DPD config NV 
05/28/15   ac      compiler error fix with w_plus_w feature defined
05/28/15   ac      add band as a paramter for rxagc 
05/23/15   kr      [Rx-Tx Split] Update device type before TQ pair allocation
05/23/15   kr      Fix compilation error
05/20/15   joz     Tx power backoff in DSDA/DSDS conflicts
05/18/15   ak      Changes to Fix the direct write based tune routine
04/23/15   ka      Remove hardcoded device 0 for fbrx
05/14/15   ac      lpm changes for w plus w
05/12/15   vbh     Antenna tuner interface updates
05/01/15   ak      [Rx-Tx Split]Changes to Support Cal with the new Split Design
04/24/15   vbh     [Rx/Tx split] Add device type parameter for rfwcdma_core_rxctl_config_band
04/24/15   ak      Changes to reset the channel loop variable to update the SM correctly
04/24/15   dw      Send TxLM update cmd to FW in SC<->DC transition
04/23/15   vbh     [Rx/Tx split] Remove device 4 checks, logic updates in prep using new state machine
04/22/15   ak      [Rx/Tx Split] Changes for the new CRAT design   
04/21/15   ac      tx/rx ratio helper function
04/14/15   ak      [Rx-Tx Split]Changes to Support Cal with the new Split Design
04/13/15   ak      [Rx/Tx Split] Changes for the new CRAT design
04/14/15   vbh     Migration to wcdma data module.
04/13/15   ak      Rx/Tx Split Changes for the new CRAT design
4/10/15    vbh     [Rx/Tx split] Updates for WCDMA data routine
04/10/15   vbh     Provide additional parameters to driver interface apis
04/09/15   vbh     [1] Update the power ON/OFF implementation with the Rx/Tx split logic
                   [2] Remove calls to hal_bus/ power_vote from all the APIs
04/09/15   vbh     [Rx/Tx split] Tx api remove references to mc state and add calls to wcdma data   routine
4/8/15     vbh     [Rx/Tx split] Move to the wcdma data module
4/2/15     vbh     Added mc tx_wakeup routine, maintain the wakeup_tx routine for WL1
3/31/15    vbh     [Rx/Tx split]Cleanup enter_mode/ exit_mode to separate the HW operations. 
04/28/15   rmb     Remove feature flag RF_HAS_MEAS_INTF_TH_2_0 but retain code
                   added under this flag. 
04/23/15   rmb     Add a new api for handle based cleanup of WCDMA mdsp data.
04/13/15   sd      Reprogram cal codes for Tx DAC and ET DAC
04/06/15   rmb     Merge back meas interface changes onto Mainline with feature flag 
03/19/15   rmb     Hook up the new apis for GRFC script generation.
03/25/15   sd      Remove release tq pair reserve in Rx sleep 
03/19/15   rmb     Hook up the new apis for GRFC script generation.
03/13/15   dw      Fix sleep state update for the carrier index 
03/13/15   dw      Fix sleep state update for the carrier index 
03/20/15   rmb     Use correct enum for static notch tune.
03/16/15   sd      Fix KW error 
03/02/15   rmb    Add channel type as UL channel to Tx params for WTR Tx Tune.
03/03/14   rmb     Pass Alternate path info to Config band and chan to choose correct NV container.
02/18/15   kr      replace FEATURE_JOLOKIA_MODEM by FEATURE_TABASCO_MODEM
02/13/15   dw      Temporialy add the primariy device check because exit mode is not yet called on all devices
02/11/15   dw      [1] Add device voting for common enter/exit routines
                   [2] Add power_on_off API to do device based power control
                   [3] Code cleanup on and state machine update
01/19/15   dw/rmb  Restoring asdiv position when building ASM script
12/16/14   ag      Calling rfwcdma_asdiv_abort in sleep to abort the pending AsDiv events
12/16/14   vs      Pass SC as carrier mode to update RxLM during W2W meas
12/03/14   ac      Skip update tune code if already under progress
10/07/14   kr      Update GRFC Tx scripts for W2G tune back scripts in tx mode.
11/25/14   sd      call update rxlm buffer with carrier mask param
12/02/14   ac      Workaround to ABORT all pending Snums during disable Tx
03/09/14   ag      Skip Hdet reading and temp comp in W2G gap
11/17/14   rmb     Added Dynamic spur mitigation feature related changes.
11/17/14   vbh     Update the call to txplim_get_mtpl_nv_item with band param
11/12/14   sd      Cleanup of deprecated sleep/wakeup routines
11/11/14   sd      Put GPIOs in LPM after all devices are disabled
11/03/14   vbh     Deallocate FBRx DM buffers for given FBRxLM handle before reset state machine
11/05/14   sd      Main line feature FEATURE_WCDMA_DC_HSUPA_FRAMEWORK
09/09/14   ak      Check for TQ IDLE before initiating a TQ release.
09/11/14   kr      Update rfwcdma_core_txplim_enabled flag properly in SC <-> DC. 
10/29/14   kai     Fix FBRx temp comp sequence issue during init Tx 
10/27/14   dw      Move MSM TxFE BW update from retune bw to init Tx
10/20/14   ac      CW fix
10/20/14   ac      fix oft errors
10/14/14   ac      xsw script to be appended to tune, featurised for now
10/13/14   vbh     Add support for NV container
10/09/14   kai     Reload LMEM in wakeup_tx
09/26/14   kai     Added support for WCDMA Rx V2 NVs
09/25/14   vbh     Fixed the AGC disble issue on all carriers during HO scenarios.
09/19/14   tks     Added support for db-dc spur mitigation
09/18/14   joz     put back some changes removed by Bolt
09/15/14   joz     replace FEATURE_DIMEPM_MODEM by FEATURE_JOLOKIA_MODEM
09/15/14   vbh     [1] Added device as a parameter to support Tx sharing
                   [2] Removed hardcoding on device 0
09/15/14   dw      WXE based Tx tune support for Single Tx
09/02/14   dw      Disable TxAGC during Tx bw change
08/28/14   vbh     [1] Call config ccs with the correct device
                   [2] Call release TQ pair only in exit mode
                   [3] Add time profiling in exit_mode
08/27/14   gh      Add check before configuring FBRx ILPC in init_tx and enable_tx
08/25/14   rmb     Fix KW Error and CW on Bolt.
08/25/14   dw      Fix warning
08/22/14   vbh     Update the doxygen documentation and parameter info to used the passed device
08/21/14   rmb     Add alternate support in the WCDMA MSM layer.
08/15/14   aro/gh  Do FBRX ILPC config capture in enable_tx
08/14/14   vbh     Fix return value in enable_tx    
08/12/14   vs      Set FR2001 flat by default to FALSE
08/11/14   rmb     Add alternate path support for W IRAT scenario. 
08/11/14   joz     added hardcoding wcdma_device_vote = TRUE for RUMI.
08/11/14   ak      Doing an Explicit PA ON from SW in retune as PA(QFGE2320) is OFF after config 
08/08/14   zhh     RF warmup profiling
08/07/14   kai     mutex the entire temp comp in decfg Tx              
08/05/14   vbh     Send AGC disable only if requested by WL1
08/06/14   dw      Disable TxAGC in Tx init when Tx BW change
08/04/14   rmb     Change in name in the usage of APIs.
08/01/14   dw      Initial support for single Tx DSDA
08/01/14   vbh     [1] Fix reset state machine sequnce in rx_sleep
                   [2] Added carrier mask to exec_tune to fix the state update
                   [3] Corrected messages and indentation 
07/21/14   vbh     SET and RESET mdsp flag indicating on going AGC config to avoid race condition
04/01/14   kr      Free DPD table in EPT_CFG
07/08/14   ak      Aborting the Tuner AOL operation as a part of Disable Tx
07/10/14   vbh     Fix DPM Warnings, add state machine update for all carriers in prep_tune
07/08/14   vbh     Reinitialize last_device with reset state machine
06/30/14   aro     Added support to handle FET+NBR+DSDA
06/30/14   vbh     [DR-DS] [1] Clean-up device 0 hardcode 
                   [2]Use MC state variable to reference primary Rx device
06/27/14   gh      Add check in sleep to see if device/path is already in sleep state
06/25/14   nv      Temporarily comment out script allocation for FBRX on WCDMA
                   DPM as it causing memory leak.
06/23/14   zhh     implement ASM sleep during retune or any ifreq/irat 
06/18/14   aa      DCHSUPA: DC config always in SC mode, update mdsp PDA during retune_bw
06/06/14   dw      Update alternate path info to RFC
06/17/14   aa      Release TQ pair in sleep only for device 0
06/12/14   aa      Get TQ pair in rx_wakup() for BOLT 2.0 call flow
06/05/14   dw      Put all ASM to sleep in primary device sleep
06/10/14   gh      Add the check on path info in rfm_wcdma_get_rxagc() for Diversity call.
06/06/14   ac      nbr support
06/04/14   sn      Enable ASM disable/sleep api
06/04/14   vws     Support script based interface rfwcdma_core_antenna_tuner_program_tx_mode 
06/04/14   vws     port change about tuner tx disable during Tx Disable 
06/04/14   vws     WCDMA IRAT tuner_disable in X2W
06/02/14   aa      compiler warning fix
05/28/14   rmb     Temporary fix to remove ASM disable tx from mc disable tx.
05/21/14   aa      TQ alloc/release/reserve in enter/exit and wakeup/sleep
05/15/14   aro     Added cmd processing module initialization
05/15/14   aro     Deprecated old command to perform MTPL update
05/19/14   ak      Updating the sleep_rx properly for early shutdown
05/15/15   ak      Fixed Compiler Warnings.
05/15/14   ak      Add parameter to sleep_rx API check for early shutdown
05/15/14   rmb     Added proper callflow of ASM/PA Sleep Wakeup.
05/12/14   sn      Backout PA/ASM sleep callflow in disable_tx
05/09/14   kai     Block Tx_decfg if temp comp not done
05/08/14   sn      Added PA sleep callflow
05/05/14   sn      Added ASM sleep wakeup callflow
05/05/14   vbh     KW warnings clean up
05/01/14   rmb     Add support for new version of AGC log packet 0x4176 v2.
04/20/14   bn      WCDMA 8 PA state support and related cleanup
04/24/14   kai     Skip temp comp if not in RxTx state
04/18/14   rmb     Add home meas enum info to config_band and config_chan APIs.
04/16/14   rmb     Fix KW Errors.
04/15/14   vbh     [FR2001] Clean LPM script building from prep_wakeup
04/15/14   vbh     [FR17754] Added support for script based tuning
04/12/14   zhh     WCDMA related Array boundary check
04/11/14   dw      Add Tx ON logging from RFLM
04/10/14   yb      Added support for coupler
04/09/14   dw      [1]Fix OFT error
                   [2]Fix Tuner CL Tx handle
04/04/14   vbh     Fix the FGAGC crash in registration due to AGC disable command
04/03/14   dw      Add dynamic RxLM update support
04/02/14   ac      fix compiler error for asdiv feature
03/28/14   vbh     Added prep/exec wakeup and sleep routines to support concurrent FR2001
03/26/14   kg      Added support for CL LM handle
03/25/14   kai     swap back order of tx band config and tx channel config
03/24/14   kai     Add ET DAC disble
03/21/14   aa      Read fbrx NV items during reload nv
03/25/14   dw      Add multi_carrier idx and TxLM handle in config mc
03/25/14   rmb     Add a new get rxagc API during X2W measurements.
03/20/14   ac      porting the asd changes from dime
03/18/14   vbh     Fix RF cal issues seen during W XO Cal
03/14/14   kai     swap order of tx band config and tx channel config
03/13/14   rmb     Pass the IRAT flag to config chan and band API.
03/13/14   zhh     check device ID in rfwcdma_mc_exec_wakeup_rx()
03/13/14   vbh     Update the FR2001 routines to support concurrent wakeup routines
03/11/14   dw      [1] DCHSUPA support
                   [2] New API for Tx retune BW
                   [3] Code cleanup, fix some hard-coding, magic number and other improper coding
                   [4] Mainlining DC feature
03/10/14   vbh     [1]Reinitialize the command action array before enable RxAGC in tune
                   [2]Add config band, config chan support in prep_wakeup
03/03/14   aa      Fix off-target error
03/03/14   dw      Remove AGC cmd from sleep_rx
02/27/14   aa      Update rfcommon_fbrx_mc_tune_to_chan
02/28/14   vbh     Crash fix to support DC calV4
02/27/14   rmb     Add fix to tune to channel for multi-carrier scenario.
02/26/14   vbh     Warning fixes
02/25/14   zhh     fix KW warning
02/25/14   vbh     Remove Err_fatal condition in wakeup APIs
02/24/14   rmb     Fix OFT Compilation Error.
02/24/14   aa      Added support for fbrx control via nv
02/24/14   aa      Added support for rsb cal
02/24/14   vbh     Add sleep_Rx api to keep consistent naming convention
02/21/14   rmb     Add global NV pointers for each of band specific static NVs.
02/13/14   dw      [1]Change AGC command function to support multiple device
                   [2]Code cleanup
02/19/14   ac      remove release taskQ for sleep(), do it only for exit()
02/18/14   vbh     DBDC- Update MC state for device 2/3
02/08/14   zhh     Add param validation to check whether the device id is valid or not
02/07/14   vbh     Wakeup callflow - fixes
02/04/14   aa      Fix DimePM llvm compiler error
02/04/14   vbh     Updated the wakeup callflow for PrepExec changes
01/30/14   aa      Update input params for rfcommon_fbrx_mc_init_tx
01/14/14   aa      Read all WCDMA specific FBRx NV's during bootup
01/09/14   vbh     Added a routine to return a copy of the MC state structure
01/08/14   kai     Remove ET chain config/deconfig
01/02/14   dw      [1]Bolt callflow cleanup
                   [2]Added support for run-time LNA switchpoint updates with FED
12/26/13   rp      Code changes to replace get critical section pointer with get common device
                   pointer for device comparison.
12/19/13   vbh     Init_tx timing optimization
12/14/13   tks     Updated calls to rfdevice rx tune api
12/13/13   tks     Updated calls to rfdevice tx tune api
12/10/dw   dw      Fix warning and remove bringup hack
12/04/13   dw      Consolidate all get RxAGC core functions. Access RxAGC value and status through single API
11/14/13   ka      Added fbrx tune to chan
11/13/13   ka      Added missing parameter to fbrx exit
11/13/13   aa      Add FBRx LM call flow for wcdma driver
10/28/13   dw      Move rxlm_buf_idx out of car_path_state in sm
11/08/13   ac      moving the ccs init to enter_mode() for ftm->ns crash
11/07/13   dw      Added back flag to enable fixed Tx power
10/31/13   ac      bringup flag for tx_on/Pa_on
10/29/13   dw      Disable temp comp for bolt bringup
10/26/13   shb     Updated IRAT APIs to support preload
10/23/13   kai     Add wrapper API config/deconfig rflm wcdma ccs taskQ
                   Removed rx_lna_setup device calling from pretune_to_chan
10/23/13   dw      Always return TRUE in init Tx for RUMI
10/20/13   dw      Fix warning
10/16/13   dw      Allocate/Release CCS TQ in sleep/wakeup
10/15/13   dw      Add rfwcdma_mdsp_exit()
10/10/13   kai     Add PA wake up before PA config
10/10/13   dw      Temporarily disable wakeup optimizatino for Bolt bring up
10/08/13   vs      Add API to get 3C RF tune status
09/24/13   dw      D3925 updates
09/20/13   dw      Turn on/off Tx and PA from MC directly for D3925
09/20/13   ac      hardcoding the lna state for the D3925 bringup
09/16/13   hm      hooking up WCDMA C++ device driver interface for WTR
09/17/13   vbh     Warning fixes
09/16/13   dw      Bolt update, initial changes for tx/rx config
09/13/13   kai     Add mdsp_init_once_flag
09/10/13   dw      Remove DBDC featurization and code clean up.
08/30/13   vs      Modifying MDSP interface to make common across DI/BO/TR
08/30/13   vs      Add rfwcdma_mc_get_rxagc
08/29/13   vs      Add RFM API to return RxAGC for all carriers
08/28/13   vs      Removal of DBDC featurization
08/27/13   dj      Added state variable use_fb_path_flag and the corresponding
                   get and set functions
08/23/13   vs      Added support for 3 carrier
08/20/13   dw      Remove FEATURE_BOLT_MODEM featurization
08/20/13   ac     add the declaration in the Qtuner feature for Triton
08/19/13   ac     compiler warning fix
08/19/13   ac     DBDC->SC fix
08/19/13  ska     Correcting the Tx init sequence to fix a Tx performance issue
08/17/13   kr      fix to pass proper parameters to rfwcdma_core_antenna_tuner_program_rx_mode()
08/16/13   ndb     Clean-up the SPI based antenna tuner support
08/14/13   ak      phone enter to "offline" with QRD RP4 RF card on TR1.1 CRM build
08/13/13   ac     dynrxlm crash due to rxlmbuf idx corruption in state machine
08/12/13   ac     compiler warning fix
08/12/13   dw/ac   DBDC fixes : sleep DBDC -> SC,enter_mode,lpm,dc->sc,tech_dependent_nv -> band
08/06/13   ac      crash due to invalid band being passed in
08/01/13   kr      replace memcpy with memscpy
08/05/13   vs      Modified RF interface to query DBDC bands supported
08/02/13   dw      Move RF bus Hal disable to rfwcdma_mc_sleep for device 0
08/01/13   st       Remove Un-used LM Includes
08/01/13   dw      Add support for L1 to get the W2W neighbor physical device index
07/31/13   aro     Added AGC log init
07/24/13   ac      compiler error fix for TR
07/24/13   ac      DBDC bringup changes
07/24/13   kai     Add temp comp data init
07/24/13   vs      Pass Closed loop NV info to Tuner
07/19/13   aa      Featurized mdsp init command for RXAGC for BOLT
07/26/13   vs      Add RF interface to query DBDC bands supported
07/16/13   aa      Multiple LNA transaction support
07/16/13   kcj     Return the result of NV read success for mc_init.
07/16/13   vws     idle timeline optimization: removed rfdevice_wcdma_tx_init()
                   from rfwcdma_mc_select_band_rx()
07/12/13   aro     Compiler Warning fix
07/09/13   kcj     Check that device init succeeded during rfwcdma_mc_init.
07/03/13   kcj     Dynamically allocated NV struct memory.
07/03/13   aa      Passed the buffer to tuner init to build the scripts
06/27/13   ac      Unified RF API to support query of RxAGC for SC and DC case
06/24/13   ac      tx freezeaccum api change for both car and txlm state for dc cfg
06/28/13   vs      Add WCDMA support for FTM_PROGRAM_QFE_SCENARIO
06/20/13   aa      Tuner init optimization
06/18/13   kai     Select Tx band before load the linearizer data
06/17/13   ry      Made changes to make Rx PLL settling time Device specific
06/14/13   kai     Added free DPD tables in deconfig Tx
06/14/13   aa      Updated Tuner init with correct device info
06/05/13   rmb     Fix to avoid loading linearizer data to MDSP pertaining
                   to the current Tx chan in FTM Cal Mode to resolve ET Cal Crash.
05/29/13   dw      Add HDET temp comp in the enable_tx
05/30/13   ac      DC HSUPA SC->DC crash fix
05/28/13   kai     Added DPD enable flag as setting WTR for Tx
05/23/13   ac      fix compiler warnings
05/22/13  ka/ac    dc hsupa changes
05/22/13   kcj     Enable multi-TX SAR limit
05/21/13   rmb     Added API to get Mutli synth lock.
05/20/13   jmf     xPT IQ Capture ET Path Delay (+API change to return samp_rate)
05/20/13   rmb     Using Memcopy to copy over the structure in rfwcdma_mc_get_car_state_info_from_device().
05/13/13   jl      Port IQ capture code
05/10/13   bn      Support EPT_ET_CFG configuration
05/09/13   aa      support for W2W FTM IRAT with new FTM framework
05/09/13   jj      RF API to get UL frequency from DL UARFCN
05/07/13   sbm     Remove papm init(QPOET bypass)
05/06/13   tks     Added support for tracking rxlm buf idx in rfwcdma_mc
                   state machine
05/03/13   rmb/aa  Replaced rfwcdma_msm_update_rxlm_buffer
05/02/13   vb      Fix wrong tuner "SPI tuner availability" check in Wakeup
05/02/13   vb      Fix possible wrong band being used during WCDMA sleep
05/02/13   vb      When initializing the tuners for both Path-0 and Path-1
                   use the active band on Path-0
05/02/13   aa      replaced rfwcdma_msm_update_rxlm_buffer
05/01/13   rmb     Added enable_qtf_tuner_support for AOL feature and rfwcdma_mc_get_car_state_info_from_device()
                   API to get state information for a particular device and removed hard coded extra warmup time in wakeup().
04/29/13   jj      Add support to get default band
04/26/13   jj      fix KW errors
04/23/13   ka      Initial DBDC support
04/17/13   sma     Fix false alarm due to incorrect parameter call
04/13/13   gh      Localize rf_buffer usage
04/11/13   dw      Initialize power tracker to APT mode in enable Tx
04/10/13   ac      removing the modulator setting function call for dc hsupa
04/08/13   sma     Fix memory leak in rfwcdma_mc_enter_mode and rfwcdma_mc_exit_mode
04/05/13   aa      Fixed the wake up sequence
04/04/13   ka      WCDMA state machine updates
04/05/13   rmb     Added rfdevice_wcdma_asm_config_rx() in RF Wakeup and rfwcdma_core_rxctl_freq_comp(),
                   rfwcdma_mdsp_update_rx_events() in pretune_to_chan() for having good RxAGC in Inter Freq Idle.
04/03/13   vb      Disable antenna tuners part of WCDMA sleep
04/03/13   kcj     Added rfwcdma_mc_wakeup_tx.
04/02/13   ac      off target compiler error fix
04/01/13   ac      avoid race condition for multiple interrupts and clean up of script type for static events
03/26/13   jyu     Added the change to allow override trigger type
03/21/13   kai     Removed asm_config in retune
03/13/13   jj      add empty function rfwcdma_mc_set_multi_tx_sar_limit()
03/12/13   rmb     Used rxAgcEnable flag in rfwcdma_mc_tune_to_multi_chan() and rfwcdma_mc_execute_rxlm()
                   when Start/Stop AGC command is sent from SW to FW.
03/12/13   kai     Put disable ET into rfwcdma_mc_decfg_tx
03/11/13   kcj     Utilize PAPM enable/disable when enabling/disabling TX.
03/08/13   ac      event script and init cmd cleanup
03/07/13   sr      changed the "pwr_tracker" name to "papm".
03/06/13   kai     Added rfwcdma_mc_decfg_tx
03/05/13   ka      Update state machine during pretune
03/05/13   kai     Put disable ET into disable_tx
03/04/13   aa      Pass the right band info in rfwcdma_mc_power_clock_on
03/01/13   kai     [1]Removed rfwcdma_mc_enable_et
                   [2]Added calling ET relative functions from rfwcdma_core_xpt
02/19/13   ac      fix OFT compile error
02/17/13   dw      Fix memory leak in rfwcdma_mc_script_buffer_ptr when diversity is enabled
02/14/13   vb      Deleted the Nikel implemenation of rfwcdma_mc_set_tuner_tune_code_override()
02/14/13   vb      Removed support for old antenna tuner scenario programming
02/15/13   aka     Reverting previous check-in for WCDMA AGC cmd check
02/14/13   ac      common api for hdet,feature cleanup
02/14/13   vb      Compilation fix for Triton
02/14/13   aka     Added check for AGC cmd
02/14/13   vb      Support for Tuner disable
02/13/13   vb      Support for new tuner manager in WCDMA
02/07/13   ndb/vb  Added rfwcdma_mc_set_tuner_tune_code_override() implementation support
02/07/13   rmb     Fix Compiler Warning.
02/07/13   rmb     Fix Compiler Errors on Dime 1.1, TR.DSDA and Triton 1.0 related to Antenna Tuner.
02/06/13   rmb     Added optimized wakeup function to send SSBI writes as a script to FW.
02/06/13   ac      HS rxlm state support
02/06/13   dw      Add inclusion of rfcommon_msg.h
01/31/13   aa      W FTM IRAT support
01/28/13   kai     Fix rfwcdma_mc_state initial error
01/28/13   kai     Removed rfwcdma_mc_set_sapt_state
01/21/13   rmb     Using rfwcdma_mdsp_get_rxagc_enable_status() for RF Wakeup/Sleep.
01/18/13   ac      removing the QTUNER feature for hdet event init
01/14/12   tks     Changed the sleep function to call the disable interface
                   of transceiver driver
01/08/12   dw      Fix crash in RxD only call
12/19/12   ka      Corrected multi_carrier_index in rfwcdma_mc_tune_to_multi_chan()
12/12/12   dw      Remove the workaround for stop all AGC cmomand in the beginning of tune
12/12/12   ac      pa_on/off update to go through helper
12/11/12   dw      Add support for DTx and DRx/LPM.
12/11/12   ac      pa_ range event builder support, pa q current as a txagc task
12/03/12   dw      Fix L2W redirection wakeup failure
11/20/12   aa      Send RF init command for RX_AGC_MEAS upon wakeup
11/16/12    vb     Tuner init and Tuner rx mode at the same time
11/16/12   aa      Enable RX AGC Measurement init command for IRAT
11/16/12   ac      hdet event builder function
11/15/12   ka      Initial support for DCHSUPA
11/15/12   kai     Added disable ET and config ET TXLM command to MDSP
11/14/12   dw      Add function to get RF buffer pointer from MC
11/09/12   kai     Added enable ET
11/02/12   dw      [1] Add support for Dual Carrier on Dime
                   [2] Remove RxAGC enable flag
10/17/12   vb      Creating tuner managers during MC init
11/02/12   jfc     Replace rf_hal_enable_rfcmd_app with rf_hal_bus_enable
11/01/12   ac      tx_on/off event builder
10/31/12   aa      Backing out the RX AGC MEAS event
10/10/12   kai     Changed interface rfwcdma_core_antenna_program_scenario_standalone
09/21/12   kai     Added new antenna_tuner support, removed old one
10/30/12   ac      pa_on/off event builder function
10/29/12   aa      Issue init RF command to FW for RX AGC MEAS event
10/25/12   dw      Vote to disable/enable RFCMD app in sleep/wakeup
10/19/12   dw      Sleep bringup and code clean up
10/17/12   dw      Fixed bug that turns on PA in disable Tx.
10/11/12   dw      SVVZ bringup updates
10/8/12    ac      drx crash fix during rf cal
10/05/12   aa      Removed obsolete wtow interface
10/04/12   sr      Added IRAT APIs.
10/2/12    ac      fix for the wcdma crash in online mode, disabling the bringup flag to set the vregs
09/28/12   ac      device interface to pass band to rfc_wcdma_get_device
09/22/12   ac      temp workaround for the rfwcdma_mc_init_tx crash
09/21/12   ka      Enable tx agc by default
09/20/12   ka      Updated start agc for FTM
09/19/12   ka      Check status before sending AGC commands
09/19/12   ka      Add PA and ASM to select tx band
09/19/12   vbh     Added request for RFCMD app in enter and exit mode
09/18/12   ac      rfc to take device as parameter
09/17/12   ka      Temporarily set constant tx power
09/17/12   vbh     Enable time profiling
09/12/12   ac      diversity rxagc bringup
09/12/12   ka      Added tx mem init calls
09/10/12   aa      Updated interface to rxlm buffer to logcial device
09/07/12   vbh     Fixed compiler warnings
09/06/12   tks     Fixed compiler warnings
09/05/12   ka      Restore tx lin init
09/05/12   vbh     Fix on target compiler warnings
08/31/12   ac      fixed script and ran rfcal successfully
08/28/12   tks     Added PA ON events in enable tx
08/27/12   tks     Added PA OFF events in disable tx
08/27/12   tks     Added ASM config event in select band rx
08/27/12   tks     Added support for Rx AGC init event in select band rx
08/24/12   ac      wait till commit is done before the start of next
08/21/12   ac      1st tune issue fix
08/18/12   tks     Converted diversity related device calls from write
                   immediate to prepare script
08/17/12   tks     Removed pre-silicon by pass flag
08/16/12   tks     Added tx enter mode and rx enter mode device calls to be part
                   of the respective tune events
08/16/12   tks     Added rx_agc_enable flag to mc state variable
08/16/12   aa      Move setting current band state machine to invalid from end
                   of enter mode to the beginning of enter mode.
08/12/12   vbh     updated the calls to ASM and PA with WCDMA interfaces
08/07/12   ac      rxagc_init change
07/29/12   tks     Fix compile errors in rfwcdma_mc_init
07/29/12   ac      for now commenting the mdsp_sync_send_rf_cfg_cmd until wl1
                   checks in the changes in mdspsync.c/.h files
07/29/12   ac      wcdma rumi bringup changes
07/21/12   tks     Call flow updates for PA & ASM
07/20/12   tks     Call flow updates
07/19/12   ac      enum updates to be compatible with the fw
07/17/12   ka      Updates to event building
07/11/12   ka      call flow updates
07/11/12   shb     Updated script_buffer to be of rf_buffer_intf* type
                   instead of void ptr
07/10/12   Saul    WCDMA. Added tune_with_lte_settings API.
07/06/12   ka      Event call flow updates
07/05/12   tks     Modified exisiting call flow to accommodate ASM & PA
07/02/12   ac      static event script writes for wtr device
07/17/12   shb     Added rfc_wcdma_init() call in rfwcdma_mc_init()
07/10/12   rmb     Removed critical section enter/exit for wakeup/sleep function,
                   moved common and wcdma wakeup just after switching on power
                   rails and made sure only power rail on/off is present
                   in power clock on/off.
07/09/12   aak     Move Quiet mode API from device to RFC
07/02/12   sn      PA/ASM device interface and scripting support
06/26/12   dw      Update rfwcdma_mc_path_car_state in pretune function
06/21/12   vb      Added support for QFE1510
06/13/12   swb     Add rxlm buffer index to rfm_pretune_to_chan function
06/12/12   cri     Fixed remaining issues for W EPT online mode support
06/12/12   kai     Added critical section lock for wakeup and sleep
06/07/12   shb     Added DPD support in WCDMA TX tune API - disabled for now
05/24/12   kai     Bypass Wakeup from Non-Sleep state
05/15/12   jl/dyc  Omit vote for DRx vreg.  Shared with PRx
05/08/12   tks     Call CPC DRx & CPC DTx SBI update in select_band function
05/08/12   kai     Add API for CPC register setting and check the CPC register
                   before issue Stop-AGC to FW
05/07/12   dw      Put all GPIO to input state in sleep
04/17/12   kai     Move update_txlm_buffer from enable_tx to init_tx,
                   remove txlm_buf_idx from disable_tx
04/13/12   kai     Revert the following update_txlm_buffer change since gsm
                   layer1 dependence not ready
04/13/12   kai     Move update_txlm_buffer from enable_tx to init_tx,
                   remove txlm_buf_idx from disable_tx
03/30/12   ems     For tune_to_multi_chan only transition to RX if not in RXTX
03/01/12   dw      HS APT support
03/27/12   dw      Fix max power issue that NV limit was not initalized
                   properly until temp comp kicks in
03/20/12   kai     remove tx_init from rfwcdma_mc_wakeup
02/27/12   dw      Stop Chain 0 AGC in sleep if it's still on
03/13/12   dw      Check AGC status of chain 1 before send agc1 cmd in sleep
03/22/12   ac      adding changes for msgr for wcdma
03/27/12   jfc     Remove pm_paon_cntrl.h / temporarily revert the MSGR changes
03/17/12   jfc     Remove clkrgm* header files
03/07/12   cri     Added initial DPD support
03/01/12   tks     Fix W2G issues caused due to missing vote in sleep function
02/27/12   swb     Return indication of missing cal data within init_tx function
02/27/12   dw      Stop Chain 0 AGC in sleep if it's still on
02/22/12   swb     Inter-freq tune optimization: add rfwcdma_mc_pretune_to_chan
                                                                                function
02/22/12   swb     Mainline FEATURE_WCDMA_RXD_PCH in wakeup function
02/21/12   av/jl   Added turning on the tcxo buffer during rfm_enter_mode().
                                                                                With GP_CLK removal, rfm_power_clock_on/off() functions are
                                                                                obsolete, which take care of tcxo buffer management.
02/17/12   swb     Removed redundant tx enter mode calls
02/16/12   kai     Added API for return PA state in func rfwcdma_mc_cmd_dispatch
02/07/12   tks     Add support RF device manager voting for power up
                                                                                and power down
01/24/11   dw      Only configure DC related SBIs in SC<->DC transitions.
12/20/11   sn      Added support for QFE1320
12/13/11   aak     Path 1 turned off in power clock off ()
12/08/11   ac      rxd idle feature
12/01/11   dw      Add support for WCDMA low power mode (DSR)
11/30/11   aak     Remove call to turn on vregs for path 1 in WCDMA
11/22/11   vb      Bug fix in programming RFC signals during WCDMA Wakeup
11/17/11   aak/dw  Changes to enable Quiet mode
11/11/11   dw      Remove GP clock programming and deprecate API
11/09/11   ac      tx band config added in tx enable as a part of handover fix
11/04/11   gh      Add ant_tuner state check. Send shutdown CW only
                                                                                when sleeping PATH0
11/03/11   ac      rfc tx_disable
10/31/11   ac      rfc wakeup
10/26/11   dw      Support for closed loop AGC in FTM
10/25/11   ac/ka   Update temp comp after reload RF NV
10/13/11   gh      Add additional checks for ant_tuner
10/13/11   swb     Added API for freezing Rx AGC accumulator
10/05/11   ac/sr      rfc sleep
09/30/11   dw      Temp comp updates
09/28/11   dw      Swap path in MC command dispatch for RxD only mode
09/28/11   sb      Mainline FEATURE_RF_COMMON_LM_RFM_INTERFACE and
                                                                                FEATURE_RF_WCDMA_LM_RFM_INTERFACE.
09/25/11   vb      Fix p4 Merge issue
09/25/11   vb      Compiler error fix
09/25/11   vb      Support for DAC cal using different Irefs
09/20/11   ac      dual carrier drx rf cal fix
09/19/11   dw      clean up rfwcdma_core_txctl_set_tx_modulator()
09/16/11   dw      In sleep, do not stop chain 1 AGC in RxD only mode
09/08/11   gh      Add Ant Tuner support
09/08/11   ac      SAR support
09/06/11   dw      APT cleanup
08/25/11   sar     Removed obsolete file bbrx.h.
08/24/11   dw      Added Support for polling based AGC cmd response handling
08/18/11   vb      Added support for splitting rfwcdma_mc_enable_tx &
                                                                                added rfwcdma_mc_init_tx
08/18/11   dw      Fix missing break in the MC command dispatch
08/13/11   vb      Need to call rfwcdma_core_txlin_load_nv_data even when
                                                                                retriving of some non critical NVs fails
08/11/11   vb      Support for new multi-chan linearizer/APT
08/08/11   dw      RxD only mode support on Nikel and optimze some callflows
08/08/11   dw      Temporary back out the AGC settling time change for wakeup
08/02/11   ckl     Remove RFWCDMA_MC_GET_CGAGC_SETTLING_TIME from
                                                                                command dispatch
08/02/11   dw      Add support to populate multi-linearizer data strucures
07/18/11   dw      legacy feature cleanup
                                                                                handle return status of lm buffer updates
07/12/11   vb      rfc cleanup and rfc interface changes to suit the new
                                                                                auto-gen of rf card files
06/29/11   vb      Updates based on the new RFC AG design
06/23/11   dw      Optimize dual carrier enable callflow
06/01/11   dw      Stop AGC only for path 0
05/28/11   dw      Enable stop AGC
05/23/11   dw      Remove rfwcdmalib_msm.h
05/20/10   dw      Remove rfwcdmalib_msm_init_config()
05/10/11   dw      Temporary change not to stop AGC when tuning
05/10/11   sty     Work-around for 8960 bringup
05/05/11   dw      Add RFWCDMA_MC_GET_LO_FREQ_FROM_UARFCN
05/05/11   dw      Remove some obsolete function calls.
04/20/11   jhe     band type correction
04/18/11   dw      KW fixes
04/12/11   dw      dynamic RxLM updates
04/10/11   dw      Power management support for Nikel
04/01/10   dw      Fix WCDMA band type mismatch in tune device API
03/25/11   dw      Remove BBRx access for Nikel
03/22/11   dw      RUMI updates
03/22/11   dw      cleanup common RFC interface access and other obsolete
                                                                                functions
03/09/11   sty     Changed return value in rfwcdma_mc_enter_mode
03/09/11   sar     Replaced clk.h with DALSys.h and updated clk_busy_wait api.
03/08/11   pl      Update mock_fw_configure_chain()
03/08/11   dw      Rename rfwcdma_msm_enable_tx_agc()
03/04/11   dw      Do not implicitly tune RxD based on internal state machine
03/11/11   dw      Nikel updates
03/01/11   dw      Fix warning
02/25/11   dw/ka   Merge support for thermal mitigation
02/23/10   dw/ka   Changes to enable RxD by NV_RF_BC_CONFIG_DIV_I
02/23/11   dw      Fix compiler warnings
02/22/11   dw      Include RxLM buffer index in AGC cmd
02/09/11   dw      MC callflow cleanup, add support for AGC cmd control,
                                                                                add support for new RF CB event
01/27/11   dw      WCDMA RFC cleanup
01/31/11   dw      Featurization for Genesis LM code for temporary mock-fw
                                                                                code access.
01/28/11   dw      Initial TxLM support for WCDMA
01/31/10   dw      Linker error fix for 8960
01/26/10   dw      updates for 8960 build
01/05/10   dw      Initial support for RxLM
01/18/11   pl      Remove obsolete interfaces
01/04/11   pl      Fix compile error -- use rfm_path_enum_type
11/17/10   kguo    Populated fix for CR 264265 to main\latest
11/05/10   dw      Program band-dependent default RxF setting in enter mode
11/05/10   kguo    Updated some function calls with band parameter to be in sync
                                                                                with function prototype changes
10/29/10   ka      Updates for initial WCDMA APT functionality
10/29/10   kguo    Fixed retval of rfwcdma_mc_disable_tx to the right value for
                                                                                success case
10/26/10   ap      Turn off Vregs for Rx_Div case in rfm_power_clock_off()
10/19/10   vb      All device functions are now accessed thru device interface
                                                                                functions
10/14/10   mkv/whc Integrating Reload RFNV from SCMM into MDM9x00 (CL 1315259)
10/04/10   ka      Reset HDET loop to acq mode after handover to
                                                                                speed convergence
09/28/10   ap      Added GP_CLK enable/disable API
09/22/10   dw      move rfwcdma_core_rxctl_init_cgagc_params to rfwcdma_mc_init
09/17/10   av      GPS notc filter update
09/02/10   ka      Complete support for common GRFC architecture
08/31/10   ac      low band drx rxfe fix
08/18/10   ac      lna phase offset support for dual carrier and sc
08/19/10   ka      Port over exit mode fixes
08/18/10   dw      Enable BBRx sleep
08/17/10   ka      Back out hdet change
08/12/10   dw      Workaround for the issue that RxF sample clock not turned
                                                                                back on after power collapse
08/11/10   ka      Update tx temp comp when enabling tx
08/09/10   dw      Add channel specific RxFE programming
08/05/10   ka      Add missing parameter
08/04/10   ac      Improve exp_hdet_vs_agc interpolation to reduce overall
                                                                                enable_tx execution time
07/19/10   aro     Fixed BBRx Bug
07/30/10   dw      Change the function prototype of
                                                                                rfwcdma_mc_force_tune_to_multi_chan
07/30/10   dw      Add rfwcdma_mc_tune_to_multi_chan_noband_check() also add
                                                                                GRFC programming in wakeup
07/28/10   ac      compiler warning fix
07/26/10   ap      Added back power collapse debug variable
07/26/10   dw      reset RxFE FIFO for every tune to channel
07/23/10   ka      Change dual carrier interface to accept channel
07/23/10   ap      Added support for rfwcdma_power_clock_on/off
07/20/10   ka      Added API to determine if dual carrier is supported
07/20/10   ap      Provided support for Vreg management during Power Collapse
07/19/10   ka      Removed warnings
06/30/10   dw      Fix for DC->FACH AGC issue
06/11/10   ac      B1 DC Low channel fix and freq comp fix for DC
06/10/10   dw      initialize Tx in wakeup for PRx only
06/10/10   kguo    Added utility function for ltow flag
06/08/10   lcl     Added an API to get the carrier type
06/08/10   ka      Klocwork fixes.
06/01/10   dw      Fix wrong function call in get tx agc
05/25/10   dw      changes to support MDM9k Div only mode call
05/21/10   dw      Sleep support for WCDMA Dual Carrier
05/20/10   tws     Add API to read/set gtow flag.
05/13/10   dw      Only init PA control in PRx enter mode
05/12/10   dw      Support for Dual Carrier to Single Carrier transition
05/10/10   dw      Added Dual Carrier state machine check in
                                                                                rfwcdma_mc_write_tech_dependent_nv_items_mdsp
05/06/10   ka      Enable HDET and temp comp for MDM9K.
05/05/10   ka      Do not send start agc command during gtow
04/23/10   ad      Fix compiler errors in WPLT build
04/16/10   kguo    Called msm_config_multimode in enter_mode to fix
                                                                                L->W rxagc problem
03/24/10   ka      Added support for DC wtow.
03/19/10   lcl     Swap the order of argument 1 and 2 for rfwcdma_mc_init
03/18/10   kguo    Recover changes on 3/17/10
03/18/10   kguo    Fixed compiler error
03/17/10   ad      Fix RxAGC logs on diversity chain
03/16/10   kguo    Merged from scmm_final_merge branch
03/08/10   ka      Restore wtow
02/26/10   ad      Fix missing RxAGC logs on diversity chain
02/25/10   ka      Temporarily remove wtow
02/18/10   ad      Support for SC-DC transition for both antennas
02/18/10   ka      Fix Tx PLL tuning
02/18/10   aak     Eliminate compile error from UMTS-only build
02/16/10   ad      Fix B5 merge changes
02/11/10   ac      merge the dual carrier changes
02/10/12   vb      Klockwork error fixes
02/08/10    vb     Removed Vreg turn-off from the function rfwcdma_mc_sleep()
02/03/10   kma     Fixed compilation issue for CDMA only build
02/02/10    vb     Added missing command dispatch functions
02/02/10   kma     Added WCDMA specific power collapse functions
02/01/10   kguo    Called init_pa_ctl after mdsp is initialized.
01/29/10   ad      Merge SC-DRX, DC-DRX changes from mdm9k dev branch
29/01/10   sr      Changed the <tech>_mc_init() signature to fix warnings in
                                                                                <tech> specific builds
01/28/10   kma     Added WCDMA specific power collapse functions
01/21/10   bn      Lint Fixes
01/21/10   bn      Removed unused rfwcdma_core_txctl_set_pa function
01/19/10   kguo    Fixed compiler error in tune_to_muitl_chan for merged build
01/18/10   ckl     Removed med level lint error.
01/18/10   ckl     Fixed BBRx power control during sleep and wakeup.
01/08/10   ckl     Removed high level lint error.
01/06/10   kguo    Merged from mdm9k dev branch to main\latest
12/23/09   ckl     Fixed PCH SLEEP issue for diversity only call
12/23/09   lp      Removed XO_OUT buffer support
12/20/09   bn      Fixed compiler warning.
12/19/09   lp      Added XO_OUT buffer support
12/09/09    bn     Adding rfwcdma_msm_set_web_misc()
12/08/09   dyc     Setup PMIC TCXO buffer prior to transmit
12/08/09    bn     Separate Tx and Rx PLL for current savings
12/02/09   ckl     Added W2W function dispatch.
12/01/09   ckl     Return always TRUE for rfwcdma_mc_init( ).
12/1/09    ac      dual carrier in ftm which was broken in plf7
11/18/09   ad      Move RFD matrix config for PA_ONx to HAL
11/11/09   ka      Updates for dual carrier.
11/30/09   ckl     Added rfm_get_freq_from_dl_arfcn( ) for W2W measurement
11/30/09   bmg     Added common_init_successful parameter to initialization
                                                                                Skip initialization if the common NV could not be loaded
                                                                                Changed init function return to boolean
11/18/09   dyc     Use correct PMIC switcher PM_VREG_RF2_ID vs PM_VREG_RF_ID
11/18/09   ad      Move RFD matrix config for PA_ONx to HAL
11/11/09   ka      Updates for dual carrier.
11/05/09   vb      Fixed the bug in rfwcdma_mc_wakeup(); rtr8600 init function
                                                                                should not be called when PRx is active
10/29/09   kguo    Reverted a feature to compile
10/29/09   kguo    Featured code that has dependency on L1
10/23/09   kguo    Merged changes for 9k bringup
10/22/09   ckl     Fixed missing Tx power on diversity only call
10/16/09   jhe     _sleep/_wakeup changes
10/01/09   ckl     Removed all warnings and extern function definitions
10/01/09   ckl     Added code to check for BBRX freeze and send F3 msg
10/01/09    bn     One-time HDET autocalibration
09/30/09   ckl     Support Diversity only call
09/22/09   dyc     Support for SMPS freq setting + some warnings cleanup.
09/17/09   ckl     Moved Tx functions to core_txctl file.
09/17/09   sar     Removed deprecated file, adie.h for CMI conformance.
09/15/09   jhe     Initial W sleep enable
08/24/09   ckl     Changed enter mode operation for diversity path.
08/20/09   ckl     Initialize primary and secondary BBRx setting at the
                                                                                same time
08/19/09    ka     Added additional header file
08/14/09    ka     Added include file required for MDM9x00
08/13/09   jhe     freq comp change
08/13/09   ckl     Support diversity and callback functions.
08/11/09   bn      Added get HDET threshold API
08/11/09   bn      Enabled Temp Comp API
07/08/09   av      Removed configuring PMIC PA Controller in MC. Moved it to RFC
07/07/09   bn      TxAGC dB10 changes
06/30/09   ckl     Replace temporary BBRx init funtion with bbrx_set_mode.
06/11/09   ckl     Call BBRx initialization in mc_enter_mode.
06/04/09   lp      Changed pm_paon_cntrl.h to pm.h
06/04/09   ckl     Support rfm_get_freq_from_uarfcn, rfm_convert_chan_to_band,
                                                                                rfm_get_hdet_tx_agc_value.
06/01/09   ckl     Fixed Rx AGC frozen issue when L1 stops and starts acquisition.
05/28/09   ckl     Modified to have one return statement in rfwcdma_mc_cmd_dispatch( )
05/28/09   ckl     Add PMIC PA_ON control initialization.
05/27/09   bn      Correct ARM register settings
05/19/09   bn      Bringup hack - Setting WCDMA mode register to retain PLL lock status
05/14/09   ckl     Added rfwcdma_nv_retrive_items() in rfwcdma_mc_init()
03/23/09   ckl     Cleanup Feature SUPPORT_MDM8200_PALTFORM
03/22/09   ckl     Added new rfm functions, rfwcdma_enable_rx, rfwcdma_disable_rx,
                                                                                rfwcdma_mc_ioctl, and rfwcdma_mc_init
03/20/09   ckl     Change rf_get_rf_hw_config() to rfcommon_get_rf_hw_config()
03/19/09   ckl     Feature SUPPORT_MDM8200_PALTFORM to support Poseidon Modem
03/18/09   ckl     Fix duplicate enum names between rfwcdma_mc_state_enum_type
                                                                                and rf_state_enum_type
03/17/09   ckl     Include clkrgm_msm.h instead of clkrgm_modem.h for SCMM build
03/15/09   ckl     Include rfnv_wcdma.h ahead of other wcdma core header files
03/10/09   ckl     Use new common function name, rfcommon_core_device_to_path
03/09/09   ckl     Code cleanup
01/30/09   av      Need to have AGC in tune_to_chan. Or has trouble acq. Needs
                                                                                further investigation
01/30/09   av      Changes and 1st round of cleanup for WCDMA support
10/14/08   sr      Initial version of wcdma Main Control.

============================================================================*/

#include "rxlm_intf.h"
#include "rfa_variation.h"
#include "rfcom.h"
#include "msg.h"
#include "rfwcdma_mc.h"

/*! @cond vbhide */
#include "rfcommon_core.h"
#include "rfcommon_core_cb.h"
#include "rfcommon_nv_mm.h"
#include "rfcommon_nv.h"
#include "rfmeas_mc.h"
#include "rfnv_wcdma.h"
#include "rfwcdma_core.h"
#include "rfwcdma_core_rxctl.h"
#include "rfwcdma_core_temp_comp.h"
#include "rfwcdma_core_txctl.h"
#include "rfwcdma_core_txlin.h"
#include "rfwcdma_core_txplim.h"
#include "rfwcdma_core_util.h"
#include "rfdevice_wcdma_intf.h"
#include "rfdevice_wcdma_lna_intf.h"
#include "rfwcdma_mdsp_types.h"
#include "rfc_card.h"
#include "rfc_card_wcdma.h"
#include "rfumts.h"
#include "ftm.h"
#include "rfwcdma_mdsp.h"
#include "wl1api.h"
#include "rfwcdma_msm.h"
#include "pm.h"
#include "DALSys.h"
#include "rfcommon_core_device_manager.h"
#include "timetick.h"
#if 0
#ifdef FEATURE_QDSP6
#include "pm_vreg_modem.h"
#else
#include "pm_vreg.h"
#endif
#endif

#include "rf_hal_common.h"
#include "rfnv_wcdma.h"
#include "rfm_types.h"
#include "rfm_common_sar.h"
#include "rfm_internal.h"
#include "rfdevice_wcdma_asm_intf.h"
#include "rfdevice_wcdma_pa_intf.h"
#include "rfdevice_wcdma_coupler_intf.h"
#include "rf_hal_bus_types.h"
#include "rf_buffer_intf.h"
#include "rf_hal_buffer.h"
#include "rfm_internal.h" // For RUMI Bring up
#include "ftm_wcdma_ctl.h"
#include "rfdevice_papm.h"
#include "rfdevice_papm_intf.h"
#include "wfw_sw_intf.h"
#include "rfmeas_mc.h"
#include "rfwcdma_mc_meas.h"
#include "rfcommon_msg.h"
#include "rfmeas_mdsp.h"
#include "rfc_card_meas.h"
#include "rfwcdma_core_util.h"
#include "rfwcdma_core_log.h"
#include "rfdevice_trx_wcdma_rx.h"
#include "rfwcdma_mdsp_async.h"
#include "rfwcdma_mdsp_sync.h"
#include "rfwcdma_mdsp_data.h"
#include "rflm_dm_api.h"
#include "rfcommon_fbrx_api.h"
#include "ftm_common_selftest.h"
#include "rfwcdma_data.h"
#include "rfcommon_math.h"
#include "rfwcdma_cmd_proc.h"
#include "rfcommon_time_profile.h"
#include "rfc_wcdma_data.h"
#include "rfwcdma_core_auto_pin.h"
#ifdef FEATURE_RF_ASDIV  
#include "rfwcdma_core_asdiv.h"
#endif
#include "rfdevice_wcdma_type_defs.h"

#include "rfcommon_core_xpt.h"
#include "rfcommon_fbrx_ilpc.h"

#ifdef FEATURE_RF_HAS_QTUNER
#include "rfwcdma_core_antenna_tuner.h"
#endif

#include "rfdevice_hdet_wcdma_intf.h"
#include "rfwcdma_core_xpt.h"
/* memscpy */
#include "stringl.h"

#include "rfcommon_autopin_api.h"
#ifdef FEATURE_RF_POWER_REPORTING
#include "rf_power_reporting.h"
#endif
#include "rfcommon_core_utils.h"

#ifdef FEATURE_RF_ASDIV
#include "rfcommon_asdiv_manager.h"
#endif
#include "rfcommon_core_sar.h"
#include "rfdevice_wcdma_xsw_intf.h"

#ifdef FEATURE_QSH_MDUMP
#include "rflm_wcdma_ccs.h"
#include "rfm_wcdma.h"
#include "ftm_wcdma_ctl.h"
#include "qsh.h" 
#include "rfcommon_qsh_client.h"
extern rflm_wcdma_data_t* rflm_wcdma_get_rflm_data();
#endif

boolean rflm_check_dac_cal_machine_status();
void rflm_dtr_tx_read_dac_status_tx_all_done(txlm_chain_type chain,uint32* data);

/*----------------------------------------------------------------------------*/
/*                          Global Declaration                                */
/*----------------------------------------------------------------------------*/

/* 200 us delay is based on the RF1/RF2 rise time with safe margin */
#define RFCOMMON_MC_RF_VREG_DELAY_TIME 200

#define RFWCDMA_HANDLE_COUNTS 24

extern WfwSwIntfStruct *rf_fwsw_intf_addr_ptr;

/*global variable to store the iq value so the updates only happen when there is a change*/
extern uint32 rflm_fbrx_iqmc;

rf_wcdma_bup_flags_t rf_wcdma_debug_flags = { 0 };  // Struct that holds wcdma debug flags

extern void rfwcdma_core_txctl_init_pa_ctl(void);
extern void rfwcdma_mdsp_init_agc_ctl(void);
extern void rfwcdma_core_update_min_power_from_nv(void);
extern void rfwcdma_mdsp_set_pa(boolean state, rfcom_wcdma_band_type band);

extern rfcommon_nv_status_type rfwcdma_nv_retrive_items
(
  rex_tcb_type *task_ptr,
  rex_sigs_type task_wait_sig,
  void (*task_wait_handler)(rex_sigs_type)
);

extern uint32 combodac_all_done[TXLM_CHAIN_MAX];

extern boolean rf_disable_powercollapse;

extern void rflm_wcdma_ccs_allocate_static_mem_block(void);

extern void rflm_wcdma_ccs_deallocate_static_mem_block(void);


#ifdef FEATURE_RF_HAS_QTUNER
extern boolean enable_qtf_tuner_support;
#endif

extern const rf_hal_bus_resource_script_type* rfwcdma_core_get_resource_script_from_buffer
(
  rf_buffer_intf *buffer_ptr
);

/*! @todo (tks) PA & Device object should be obtained from RFC */
void *pa_device;
void *asm_device;

/* flag to indicate whether wakeup optimization should be performed*/
boolean enable_wakeup_optimization_flag = FALSE;

/* MC State variable to hold the state, band on each path.
                Also stores rx and tx frequency. And diversity only call
                in that order*/
rfwcdma_mc_state_type rfwcdma_mc_state;

rfcom_band_type_u tx_band;
rfcom_band_type_u rx_band;

/* The following structure is obsolete. */
/* Retained so components shared with old targets can compile */
rfwcdma_mc_path_car_state_type rfwcdma_mc_path_car_state =
{
  RF_PATH_0,
  RFCOM_SINGLE_CARRIER,
  { 10700, 10700 },
};

rfwcdma_mc_rxlm_buf_type rfwcdma_mc_rxlm_buf[2] = { { 0, FALSE }, { 0, FALSE } };

static boolean ftm_irat_testing = FALSE;

static boolean rfwcdma_mc_enable_time_profiling = TRUE;

/*for bringup added this flag for immediate and script based write*/
boolean tx_on_pa_on_immediate_write_flag = FALSE;

rf_hal_bus_client_handle *rf_wcdma_hal_bus_client_handle;

static boolean rfwcdma_ftm_use_fb_path = FALSE;

/*< Temporary Variable to store the status of RF 3C tune per path */
boolean wcdma_3c_active[RF_PATH_2] = {FALSE};

static lm_handle_type rfwcdma_mc_fbrx_lm_handle;

/* Narrow Band Rejection global vars */
boolean enable_nbr_flag = FALSE; /* flag to indicate if NBR is enabled or not */
uint32  nbr_rxlm_buf_idx = 0; /* NBR modG RxLM index */

static rfm_device_enum_type last_device = RFM_INVALID_DEVICE; 

/*----------------------------------------------------------------------------*/
/*                          Function Prototypes                               */
/*----------------------------------------------------------------------------*/
void rfwcdma_mc_create_rf_buff(rf_buffer_intf **buffer_ptr);
void rfwcdma_mc_destroy_rf_buff(rf_buffer_intf *buffer_ptr);
void rfwcdma_mc_create_device_scripts
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  uint16 channel_list[4],
  rf_buffer_intf *rx_device_buffer[],
  rfcom_multi_carrier_hspa_id_type num_chan,
  rfcom_wcdma_band_type source_band,
  rfwcdma_core_rxctl_dis_first_ind_type disable_ind,
  rfdevice_wcdma_spur_param_type* dyn_notch_filter_settings
);
/*----------------------------------------------------------------------------*/
/*                                Functions                                   */
/*----------------------------------------------------------------------------*/

/*!
        @brief
        Called to enter WCDMA mode: Doesn't place device to RX mode as we don't assume
        band. Init once flag handles programming that needs to be done only once for
        every power up

        @details
        This function should be called after MDSP image is downloaded and when
        entering WCDMA mode.

        @param device

        @param user_data_ptr

        @param cb_handler

        @param rxlm_buf_idx

        @return
        1 on success and 0 on failure
*/
uint32 rfwcdma_mc_enter_mode
(
  rfcom_device_enum_type device,
  const void *user_data_ptr,
  rfm_cb_handler_type cb_handler,
  uint32 rxlm_buf_idx
)
{
  rf_path_enum_type path;
  boolean wcdma_device_vote;
  timetick_type prof_cal_func;
  rfcommon_fbrx_error_type fbrx_status = RFCOMMON_FBRX_SUCCESS;
  boolean device_map_status = FALSE; 
  uint32 dev_mask =0;
  rfm_device_enum_type pair_rx_dev, pair_tx_dev;

  const rfm_wcdma_enter_mode_param* enter_param = (const rfm_wcdma_enter_mode_param*)user_data_ptr;
  rf_lock_data_type* tq_mdsp_mc_meas_crit_sec = NULL;
#ifdef FEATURE_RF_ASDIV
  rfcommon_asdiv_cb_handler_type asdiv_cb_handle; 
#endif

  RF_MSG_1( RF_HIGH, "rfwcdma_mc_enter_mode: START. device %d ",
            device );

  if (device >= RFM_MAX_WAN_DEVICES)
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_enter_mode: Error api called with invalid device %d. success %d. Cannot enter mode.",
              device, 0 );
    return 0;
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  /*Parameter check*/
  if (enter_param->device_type == RFM_WCDMA_INVALID_DEVICE) 
  {
    RF_MSG_1(RF_ERROR,"rfwcdma_mc_enter_mode: Device%d passed with invalid type", device);
    return 0; // Failure
  }

  if ((enter_param->device_type == RFM_WCDMA_TX_DEVICE) && 
      (enter_param->prx_device >= RFM_MAX_WAN_DEVICES)) 
  {
    RF_MSG_2(RF_ERROR,"rfwcdma_mc_enter_mode: Tx Device%d passed with invalid paired device",
                                                                   device,enter_param->prx_device );
    return 0; // Failure
  }

  /* Enter Critical Section for Meas MC TQ MDSP */
  tq_mdsp_mc_meas_crit_sec = rfwcdma_data_get_meas_mc_mdsp_crit_sec_data();

  if (tq_mdsp_mc_meas_crit_sec == NULL) 
  {
    RF_MSG(RF_ERROR,"rfwcdma_mc_enter_mode: MC_MEAS Critical section data is NULL in RFWCDMA data");
    return 0; // Failure
  }

  else
  {
    rf_common_enter_critical_section(tq_mdsp_mc_meas_crit_sec);
  }

  dev_mask = rfwcdma_data_get_enabled_device_mask();

  if (dev_mask == 0)
  {
    (void)rfwcdma_mc_common_enter(device);

	/*Register an AsDiv Callback on first device enter mode*/
#ifdef FEATURE_RF_ASDIV
    if(rfwcdma_data_get_asdiv_cb_status() == FALSE)
    {
      asdiv_cb_handle = rfcommon_asdiv_register_cb((void*) rfwcdma_mc_asdiv_common_callback,
                                                   NULL,
                                                   rfwcdma_data_get_asdiv_cb_pos_ptr(),
                                                   RFM_IMT_MODE,
                                                   ASDIV_CB_REPEAT);	  
      rfwcdma_data_set_asdiv_cb_handle(asdiv_cb_handle);
      rfwcdma_data_set_asdiv_cb_status(TRUE);
    }
#endif
  }
  
  /* Convert rfcom_device_enum_type to rf_path_enum_type */
  path = rfcommon_core_device_to_path(device);

  rfwcdma_data_update_device_type(device,enter_param->device_type);
  
 if (rfwcdma_data_get_device_type(device) != RFM_WCDMA_TX_DEVICE)  
 {
  /* Allocate TQ pair */
  rfwcdma_mdsp_ccs_allocate_tq_pair(device);
  }
  
  /*Data module update*/ 
  rfwcdma_data_update_rf_state(device,RFWCDMA_STATE_ENTER_MODE);

  /*This should be generic. Enter mode should be called even for Tx device with CRAT design. Don't confuse with name rxlm_buf_idx*/
  rfwcdma_data_set_rflm_buf_idx(device,rxlm_buf_idx);

  /*If the device is PRX device, update the associated Tx device info from RFC*/
  if(enter_param->device_type == RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
    pair_tx_dev = rfc_common_get_preferred_associated_tx_device(device);
    rfwcdma_data_set_associated_device_tx(device, pair_tx_dev);
  }

  if(enter_param->device_type == RFM_WCDMA_TX_DEVICE) 
  {
   /*If the device is Tx device, make sure its paired PRx device now points to the right Tx device */
    pair_rx_dev = enter_param->prx_device;
    rfwcdma_data_set_associated_device_tx(pair_rx_dev,device);
    //Allocate pdmem in static region
    rflm_wcdma_ccs_allocate_static_mem_block();
  }
  else
  {
   /* if it is not a Tx device, the paired Rx device info should come from RFC*/
    pair_rx_dev = rfc_common_get_preferred_associated_rx_device(device);
  }

  rfwcdma_data_set_associated_device_rx(device,pair_rx_dev);

  /* Register callback function */
  rfwcdma_core_register_cb(cb_handler, user_data_ptr,RFWCDMA_CORE_CB_DELAY_EXPIRED);

  if (rfcommon_autopin_is_enabled(RFM_IMT_MODE))
  {
  if(RFCOMMON_AUTOPIN_ERROR == rfcommon_autopin_mc_enter_mode(RFM_IMT_MODE))
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "WCDMA AutoPin enter mode failed!");
  }
  else
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_HIGH, "WCDMA AutoPin enter mode successful!");
  }
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Enter Mode - %d", prof_cal_func);
  }

  /* Leave Critical Section for Meas MC TQ MDSP */
  rf_common_leave_critical_section(tq_mdsp_mc_meas_crit_sec);

  RF_MSG_1( RF_HIGH, "rfwcdma_mc_enter_mode: END. device %d status 1",
            device );

  return 1;   /* success */
}
/*------------------------------------------------------------------------------------------------*/

boolean rfwcdma_mc_reset_radio(rfm_device_enum_type device)
{
  boolean ret_val = FALSE;

  uint8 sub_id = rfwcdma_data_get_subscription_type(device);

  if (device < RFM_WCDMA_MAX_DEVICE) 
  {
    ret_val = rfwcdma_data_init_radio_status(device);
    RF_MSG_2(RF_HIGH, "rfwcdma_mc_reset_radio: Resetting the WCDMA radio module on device %d, sub_id %d", device, sub_id);
  }
  else 
  {
    ret_val = FALSE;
  }

  return ret_val;
}
/*----------------------------------------------------------------------------*/

boolean rfwcdma_mc_reset_data()
{
  boolean ret_val = FALSE;

  RF_MSG_1(RF_HIGH, "rfwcdma_mc_reset_data: Resetting the WCDMA data module",0);
  ret_val = rfwcdma_data_init();

  return ret_val;
}
/*----------------------------------------------------------------------------*/
/*!
        @brief
        Called to exit WCDMA mode.

        @details

        @param device

        @param to_mode

        @return
        0 on success and 1 on error
*/
int32 rfwcdma_mc_exit_mode
(
  rfcom_device_enum_type device,
  rfcom_mode_enum_type to_mode
)
{

  rfcommon_fbrx_error_type fbrx_status = RFCOMMON_FBRX_SUCCESS;
  rf_path_enum_type path ;
  rf_time_type profile_time; /* Profile Time */
  rf_time_tick_type profile_tick; /* Profile Tick */
  uint32 dev_mask =0, temp_dev_mask =0;
  rf_lock_data_type* tq_mdsp_mc_meas_crit_sec = NULL;
  rflm_dm_handle_id_t lm_handle;
  boolean api_status = TRUE;

  /* Start Profiling of API */
  profile_tick = rf_time_get_tick();

  /* Enter Critical Section for Meas MC TQ MDSP */
  tq_mdsp_mc_meas_crit_sec = rfwcdma_data_get_meas_mc_mdsp_crit_sec_data();

  if (tq_mdsp_mc_meas_crit_sec == NULL) 
  {
    RF_MSG(RF_ERROR,"rfwcdma_mc_exit_mode: MC_MEAS Critical section data is NULL in RFWCDMA data");
    return 1; // Failure
  }

  else
  {
    rf_common_enter_critical_section(tq_mdsp_mc_meas_crit_sec);
  }
  
  dev_mask = rfwcdma_data_get_enabled_device_mask();

  /* For the purpose of state checks, convert the device to device mask */

  temp_dev_mask = 1<<device;

  if (RFM_WCDMA_HEALTHY_STATE != rfwcdma_mc_validate_state(temp_dev_mask, RFM_WCDMA_EXIT))
  {
     RF_MSG_1(RF_ERROR, "rfwcdma_mc_exit_mode: State validation failed on dev mask %d", temp_dev_mask);

     /* Leave Critical Section for Meas MC TQ MDSP */
     rf_common_leave_critical_section(tq_mdsp_mc_meas_crit_sec);

     /* Return the API Wait time */
     return 0;
  }
  
  dev_mask &= ~(1 << device);

/* RxAGC data cleanup */
  lm_handle = rfwcdma_data_get_rflm_buf_idx(device);
  if (rfwcdma_data_get_device_type(device)!= RFM_WCDMA_TX_DEVICE) 
  {
    api_status &= rfwcdma_mdsp_async_deconfig_rx((rflm_dm_handle_id_t)lm_handle);
    if (api_status == FALSE) 
    {
       RF_MSG_3(RF_ERROR,"rfwcdma_mc_exit_mode: Exit failed for device: %d and  RFLM handle: %d", 
                device, lm_handle,0); 
       /* Leave Critical Section for Meas MC TQ MDSP */
       rf_common_leave_critical_section(tq_mdsp_mc_meas_crit_sec);
       return -1;
    }
    else
    {
      RF_MSG_4(RF_HIGH,"rfwcdma_mc_exit_mode: Device idx %d, RFLM handle: %d", 
               device,lm_handle,0,0); 
    }
  
    rfwcdma_mdsp_ccs_release_tq_pair(device);
  }

  else  
  {
    //Deallocate pdmem in static region
    rflm_wcdma_ccs_deallocate_static_mem_block();
  }

  /* All devices have exited. Proceed to common cleanup */
  if (dev_mask == 0)
  {
  	/*De-register the AsDiv Callback on last device exit*/
#ifdef FEATURE_RF_ASDIV
    if(rfwcdma_data_get_asdiv_cb_status() == TRUE)
    {      
      rfcommon_asdiv_deregister_cb(rfwcdma_data_get_asdiv_cb_handle());
      rfwcdma_data_set_asdiv_cb_status(FALSE);
    }
#endif  

     /* Common exit routine */
    (void)rfwcdma_mc_common_exit(device);
  }
  else 
  {
    rfwcdma_mc_reset_radio(device);
  }

  if (rfcommon_autopin_is_enabled(RFM_IMT_MODE))
  {
  if(RFCOMMON_AUTOPIN_ERROR == rfcommon_autopin_mc_exit_mode(RFM_IMT_MODE))
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "WCDMA AutoPin exit mode failed!");
  }
  else
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_HIGH, "WCDMA AutoPin exit mode successful!");
  }
  }

  /* get time spent in API */
  profile_time = rf_time_get_elapsed( profile_tick, RF_USEC );

  RF_MSG_2(RF_HIGH,"rfwcdma_mc_exit_mode:Dev %d, Time Spent:%d",device, profile_time);

  /* Leave Critical Section for Meas MC TQ MDSP */
  rf_common_leave_critical_section(tq_mdsp_mc_meas_crit_sec);

  return 0;   /* no error */
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        Called to enable Tx chain

        @details
        The function enables the transmit chain for WCDMA uplink operation.

        @param device

        @param user_data_ptr

        @param cb_handler
*/
uint32 rfwcdma_mc_enable_tx
(
  rfcom_device_enum_type device,
  uint16 tx_channel[RFM_WCDMA_MAX_CARRIERS]
)
{
  rf_path_enum_type path;
  timetick_type prof_cal_func;
  rfcommon_fbrx_error_type fbrx_status = RFCOMMON_FBRX_SUCCESS;
  uint16 channels[RFCOM_MAX_CARRIERS] = {0};
  uint32 wtr_tune_time = 0;
  uint8 ret_val =1; 
  boolean buffer_valid = FALSE;
  uint8 txagc_ccs_mem_write_counter;
  rfm_device_enum_type prx_device = RFM_INVALID_DEVICE;
  uint32 wtr_instance = 0;
  uint8 fbrx_modem_chain = 3;
  uint8 tx_handle = 255;

  if (device >= RFM_MAX_WAN_DEVICES)
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_enable_tx: Error api called with invalid device %d. success %d. Cannot enable TX.",
              device, 0 );
    return 0;
  }

  RF_MSG_2(RF_ERROR, "rfwcdma_mc_enable_tx: Tx Enable params device: %d  channel %d", device, tx_channel[0]);
  
  rfcom_wcdma_band_type band = rfwcdma_data_get_curr_band(device);
  
  if ( band >= RFCOM_NUM_WCDMA_BANDS )
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_mc_enable_tx ERROR invalid band %d. Cannot enable TX.",
              band );
    return 0;
  }

  if ( rfc_common_check_band_supported( rfcommon_core_convert_rfcom_band_to_sys_band(RFM_IMT_MODE, (rfcom_band_type_u)band), device ) == FALSE )
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_enable_tx ERROR device %d does not support WCDMA RF band %d. Cannot enable TX.",
              device, band );
    return 0;
  }

  tx_handle = rfwcdma_data_get_rflm_buf_idx(device);
  if(tx_handle >= RFWCDMA_HANDLE_COUNTS)
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_enable_tx: ERROR invalid tx handle for device %d. success %d. Cannot enable TX.",
                         device,0 );
    return 0;
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  path = rfcommon_core_device_to_path(device);

  fbrx_modem_chain = (uint8)(rfc_wcdma_get_fb_chain(device,band));

  fbrx_status = rfcommon_fbrx_enable(rfwcdma_mc_fbrx_lm_handle, fbrx_modem_chain);
  
  /*Common fbrx enable re writes default iqmc to HW. Reset the global so correct iqmc overwrite happens during fbrx get exp gain  */
  rflm_fbrx_iqmc = 0xFF;

  if(fbrx_status == RFCOMMON_FBRX_ERROR)
  {
    RF_MSG_1(RF_FATAL, "rfcommon_fbrx_enable FBRx HW enable failed: handle %d", rfwcdma_mc_fbrx_lm_handle);
  }

  #ifndef FEATURE_MODEM_TX_SHARING

  /* Band specific settings. Including device config and mdsp config */
  if( rfwcdma_core_txctl_config_tune_device(device,
                                            band,
                                            tx_channel,
                                            tx_handle,
                                            NULL,
                                            RFDEVICE_EXECUTE_IMMEDIATE,
                                            &wtr_tune_time ) == FALSE 
    )
    {
    RF_MSG_1(RF_ERROR, "rfwcdma_mc_enable_tx: Config Tune Device Tx failed for device %d", device);
    ret_val=0; 
    }
  #endif

  /* Configure ILPC sample capture if ILPC is enabled */
  if(rfcommon_fbrx_mc_is_ilpc_enabled(RFLM_TECH_WCDMA))
  {
    /*check the wtr_instance from RFC before enable ILPC sample capture*/
    wtr_instance = rfcommon_core_get_wtr_instance_from_device(device, band, RFCOM_WCDMA_MODE);
    RF_MSG_1(RF_ERROR, "rfwcdma_mc_enable_tx: wtr_instance %d", wtr_instance);
    if((wtr_instance < RFM_DEVICE_0) || (wtr_instance >= RFM_DEVICE_4))
    {
      wtr_instance = 0;
    }
    rfcommon_fbrx_ilpc_config_sample_capture( wtr_instance,
                                              tx_handle,
                                              rfwcdma_mc_fbrx_lm_handle );
  }

  /* Enable the Tx power limiting loop */
  rfwcdma_core_txplim_enable(TRUE);

  /* Update WCDMA state to RxTx since the Tx is enabled */
  rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_TX);

  #ifdef FEATURE_RF_HAS_WTR2965
  rfwcdma_auto_pin_supported(device,band);
  #endif


  if (rfcommon_autopin_is_enabled(RFM_IMT_MODE))
  {
  rfcommon_autopin_mc_enable_tx(tx_handle,RFM_IMT_MODE,NULL);
  }
  rfwcdma_core_tx_set_common_selftest_ind(ftm_common_get_selftest_capture_flag());

  RF_MSG_1(RF_HIGH, "rfwcdma_mc_enable_tx: rflm_check_dac_cal_machine_status API Start with return value: %d [expected 0]", rflm_check_dac_cal_machine_status());

  /*Re-programme DAC if found to be in reset state in Enable Tx*/
  if (rflm_check_dac_cal_machine_status() == FALSE)
  {
	
    rf_time_tick_type prof_cal_func; /* Function Profiling Variable */
    rf_time_type prof_cal_func_val = 0; /* Function Profiling Value Variable */
	  
    prof_cal_func = rf_time_get_tick();
   
    rfcommon_msm_dac_mission_mode_enter();
   
    prof_cal_func_val = rf_time_get_elapsed( prof_cal_func, RF_USEC );
   
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,
          "rfwcdma_mc_enable_tx : rflm_check_dac_cal_machine_status during WCDMA TX wakeup is %d [expected 1]",
           rflm_check_dac_cal_machine_status());
  }
  
  RF_MSG_1(RF_HIGH, "rfwcdma_mc_enable_tx: rflm_check_dac_cal_machine_status API End with with value: %d [expected 1]", rflm_check_dac_cal_machine_status());

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Enable Tx - %d", prof_cal_func);
  }

  RF_MSG_1(RF_HIGH, "RF Tx is enabled", 0);

  return ret_val;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        Called to disable Tx chain

        @details
        The function is used to disable tx chain previously assigned for WCDMA
        uplink operation

        @param device

        @return
*/
int32 rfwcdma_mc_disable_tx(rfcom_device_enum_type device)
{
  rf_path_enum_type path;
  rfdevice_coupler_tech_cfg_type coupler_tech_cfg;
  timetick_type prof_cal_func;
  rf_lock_data_type* update_tuner_crit_sec = NULL;
  uint8 tx_handle = 255;

  if (device >= RFM_MAX_WAN_DEVICES)
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_disable_tx: Error api called with invalid device %d. success %d. Cannot disable TX.",
			 device, 0 ); 
    return 0;
  }

  tx_handle = rfwcdma_data_get_rflm_buf_idx(device);
  if(tx_handle >= RFWCDMA_HANDLE_COUNTS)
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_disable_tx: ERROR invalid tx handle for device %d. success %d. Cannot disable TX.",
                         device,0 );
    return 0;
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }
  update_tuner_crit_sec = rfwcdma_get_tuner_update_lock_data();

  /* Enter critical section to ensure that AOL doesn't happen during Tx disable */
  rf_common_enter_critical_section(update_tuner_crit_sec);

  path = rfcommon_core_device_to_path(device);

  /* Disable power limiting loop */
  rfwcdma_core_txplim_enable(FALSE);

  /* Disable Transceiever */
  rfdevice_wcdma_tx_disable(device,
                            rfwcdma_data_get_curr_band(device),
                            RFDEVICE_EXECUTE_IMMEDIATE,
                            NULL);

  rfc_wcdma_tx_disable(device, rfwcdma_data_get_curr_band(device));

  #ifdef FEATURE_RF_HAS_QTUNER

   /* Lock the ASDiv manager interface to avoid concurrency of ASDiv and Tuner configuration*/
    #ifdef FEATURE_RF_ASDIV
    rfcommon_asdiv_manager_enter_lock(); 
    #endif
    /*Disable Tx */
    (void)rfwcdma_core_antenna_tuner_tx_disable ( device, 
                                                  rfwcdma_data_get_curr_band(device),
                                                  rfwcdma_data_get_curr_chan(device,0),
                                                  RFDEVICE_EXECUTE_IMMEDIATE,
                                                  NULL,
                                                  RFCOM_INVALID_TIMING_OFFSET);

   /* Release the ASDiv manager interface */
   #ifdef FEATURE_RF_ASDIV
   rfcommon_asdiv_manager_release_lock();
   #endif
  #endif /* FEATURE_RF_HAS_QTUNER */

  /* Disable the TX ASM before sleep */
  rfdevice_wcdma_asm_disable_tx(device,
                                rfwcdma_data_get_curr_band(device),
                                NULL,
                                RFDEVICE_EXECUTE_IMMEDIATE,
                                RFCOM_INVALID_TIMING_OFFSET );

  /* Put the ASM to sleep */
  rfdevice_wcdma_asm_sleep_tx(device,
                              rfwcdma_data_get_curr_band(device),
                              NULL,
                              RFDEVICE_EXECUTE_IMMEDIATE,
                              RFCOM_INVALID_TIMING_OFFSET );

  /* Disable QPOET. Passing NULL for the script buffer instructs */
  /* the PA class to perform the operation directly. */
  if (!rfdevice_wcdma_papm_disable(device, rfwcdma_data_get_curr_band(device), NULL))
  {
    RF_MSG_2(RF_ERROR, "Power tracker disable failed on device %d, band %d", 
                                 device, rfwcdma_data_get_curr_band(device));
  }

//#ifdef FEATURE_RFA_D3925

  if (tx_on_pa_on_immediate_write_flag == TRUE)
  {
    RF_MSG(RF_HIGH, "D3925: Turning off Tx On/PA On from SW ");

    rfdevice_wcdma_set_rf_on(device,
                             rfwcdma_data_get_curr_band(device),
                             FALSE,
                             RFDEVICE_EXECUTE_IMMEDIATE,
                             NULL);

    (void)rfdevice_wcdma_pa_on_off(device,
                                   rfwcdma_data_get_curr_band(device),
                                   FALSE,
                                   NULL,
                                   RFDEVICE_EXECUTE_IMMEDIATE,
                                   RFCOM_INVALID_TIMING_OFFSET);
  }

  (void)rfdevice_wcdma_pa_sleep(device,
                                rfwcdma_data_get_curr_band(device),
                                NULL,
                                RFDEVICE_EXECUTE_IMMEDIATE,
                                RFCOM_INVALID_TIMING_OFFSET);

  coupler_tech_cfg.mode = RFM_IMT_MODE;
  coupler_tech_cfg.band.wcdma_band = rfwcdma_data_get_curr_band(device);
  coupler_tech_cfg.port = 0;
  coupler_tech_cfg.direction = RFDEVICE_COUPLER_DIRECTION_OPEN;

  rfdevice_wcdma_coupler_config(device,
                                coupler_tech_cfg,
                                NULL,
                                RFDEVICE_EXECUTE_IMMEDIATE,
                                RFCOM_INVALID_TIMING_OFFSET);

//#endif
  if (rfwcdma_core_xpt_is_enabled(rfwcdma_data_get_curr_band(device)))
  {
    //Disable ET DAC
    rfwcdma_msm_disable_et_dac(tx_handle);
  }
  
  rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_TX_SLEEP);

  rfwcdma_mdsp_async_abort_tuner_open_loop_update(tx_handle);

  rfwcdma_mdsp_sync_abort_open_loop_pending_snums();

  if (rfcommon_autopin_is_enabled(RFM_IMT_MODE))
  {
  rfcommon_autopin_mc_disable_tx(tx_handle,
                                 RFM_IMT_MODE);
  }

  rf_common_leave_critical_section(update_tuner_crit_sec);

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Tx Disable - %d", prof_cal_func);
  }
  RF_MSG_1(RF_HIGH, "RF Tx is disabled", 0);

  return 1;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief

        @details
*/
uint32 rfwcdma_mc_enable_rx
(
  rfcom_device_enum_type device,
  const void *user_data_ptr,
  rfm_cb_handler_type cb_handler
)
{
  RF_MSG_1(RF_HIGH, "rfwcdma_mc_enable_rx (device = %d)", device);
  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief

        @details
*/
int32 rfwcdma_mc_disable_rx(rfcom_device_enum_type device)
{
  RF_MSG_1(RF_HIGH, "rfwcdma_mc_disable_rx (device = %d)", device);
  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief

        @details

*/
rfcom_ioctl_return_type rfwcdma_mc_ioctl
(
  rfcom_device_enum_type device,
  rfcom_ioctl_param_type request,
  void *pbuf, int32 length
)
{
  return RFCOM_IOCTL_NO_ERROR;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        Called to initialize Rx band specific settings when switching between
        WCDMA bands

        @details

        @param path

        @param band

        @return
*/
uint32 rfwcdma_mc_select_band_rx(rfcom_device_enum_type device,
                                 rfcom_wcdma_band_type band,
                                 uint32 multi_carrier_idx,
                                 uint32 rxlm_buf_idx,
                                 rfcom_wcdma_band_type source_band)
{

  
  if((device < RFM_DEVICE_0) || (device >=RFM_MAX_WAN_DEVICES))
	 {
	   RF_MSG_1(RF_ERROR, "rfwcdma_mc_select_band_rx: Invalid device, returning from here %d", device);
	   return 0;
	 }
  
  uint8 i;
  rf_path_enum_type path = rfcommon_core_device_to_path(device);
  rf_buffer_intf *buffer_ptr = NULL;

  rfm_device_enum_type paired_dev;
  rfdevice_rx_mode_type rx_mode;

  if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
    rx_mode = RFDEVICE_PRX_OPER_MODE;
  }
  else if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_DIVERSITY_RX_DEVICE) 
  {
    rx_mode = RFDEVICE_DRX_OPER_MODE;
  }
  else
  {
    rx_mode = RFDEVICE_INVALID_OPER_MODE;
  }

  paired_dev = rfwcdma_data_get_associated_device_rx(device);

  /* Create rf_buff */
  rfwcdma_mc_create_rf_buff(&buffer_ptr);

  /* No band change implies no need to repogram settings
                  for the same band */
  if ((band == rfwcdma_data_get_curr_band(device)) && (ftm_irat_testing == FALSE))
  {
    /* Destroy rf_buff */
    rfwcdma_mc_destroy_rf_buff(buffer_ptr);
    return 0;
  }

  /* Program the GPIO/GRFC/RF_CTRL signals for the specified band*/
  rfc_wcdma_rx_band_config(device, band);

  /* Clear MC buffer */
  (void)rf_buffer_clear(buffer_ptr);

  /* If a different band is tuned on the device that was tuned before, then disable WTR on the previous band
  for that device */
  if ( (source_band != RFCOM_BAND_INVALID) && (source_band != band) ) 
  {
    uint8 sub_id = rfwcdma_data_get_subscription_type(device);
    RF_MSG_2(RF_HIGH, "rfwcdma_mc_select_band_rx: Calling rfdevice_wcdma_rx_disable: device %d, sub_id %d", device, sub_id);

    rfdevice_wcdma_rx_disable(device,
                              source_band,
                              RFDEVICE_EXECUTE_IMMEDIATE,
                              NULL, paired_dev, rx_mode, (rfdevice_trx_sub_type)sub_id);

    rfwcdma_core_rxctl_disable_sleep_asm(device,
                                         source_band,
                                         NULL,
                                         RFDEVICE_EXECUTE_IMMEDIATE,
                                         RFCOM_INVALID_TIMING_OFFSET,
                                         RFWCDMA_CORE_ASM_TUNER_DIS_IND_BAND);
  }
  /* Configure ASM Rx */
  rfwcdma_core_rxctl_enable_asm(device,
                                band,
                                NULL,
                                RFDEVICE_EXECUTE_IMMEDIATE,
                                RFCOM_INVALID_TIMING_OFFSET);

  /* Destroy rf_buff */
  rfwcdma_mc_destroy_rf_buff(buffer_ptr);

  return 0;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        Called to initialize Tx band specific settings when switching between
        WCDMA bands

        @details

        @param path

        @param band

        @return
*/
uint32 rfwcdma_mc_select_band_tx(rfm_device_enum_type device,
                                 rfcom_wcdma_band_type band,
                                 uint32 txlm_handle)
{

  /* Update WCDMA Tx band information */
  tx_band.wcdma_band = band;

  rfdevice_wcdma_pa_init(device,
                         band,
                         NULL,
                         NULL,
                         RFDEVICE_EXECUTE_IMMEDIATE);

  /*! If same RF device, device library should take care that it
                  shouldn't init it again esp when in RX mode */
  rfdevice_wcdma_tx_init(device,
                         RFCOM_WCDMA_MODE,
                         band,
                         RFDEVICE_EXECUTE_IMMEDIATE,
                         NULL);

  /* Wakeup PA for specific WCDMA band */
  rfdevice_wcdma_pa_wakeup(device,
                           band,
                           NULL,
                           RFDEVICE_EXECUTE_IMMEDIATE,
                           RFCOM_INVALID_TIMING_OFFSET);

  /* Configure the PA for specific WCDMA band */
  rfdevice_wcdma_pa_config(device,
                           band,
                           NULL,
                           RFDEVICE_EXECUTE_IMMEDIATE,
                           RFCOM_INVALID_TIMING_OFFSET,
                           RFDEVICE_PA_BW_5MHZ);
  /* Wakeup TX ASM for specific WCDMA band */
  rfdevice_wcdma_asm_wakeup_tx(device,
                               band,
                               NULL,
                               RFDEVICE_EXECUTE_IMMEDIATE,
                               RFCOM_INVALID_TIMING_OFFSET);

  /* Configure the ASM for specific WCDMA band */
  rfdevice_wcdma_asm_enable_tx(device,
                               band,
                               NULL,
                               RFDEVICE_EXECUTE_IMMEDIATE,
                               RFCOM_INVALID_TIMING_OFFSET);

  return 0;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief

        @details

        @return

*/
boolean rfwcdma_mc_init
(
  rfcom_device_enum_type device,
  boolean common_init_successful,
  rex_tcb_type *caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (*task_wait_func_ptr)(rex_sigs_type)
)
{
  rfcommon_nv_status_type nv_status = RFCOMMON_NV_READ_FAILED;
  static boolean nv_items_read = FALSE;
  static boolean nv_init_success = FALSE;
  rfcom_wcdma_band_type band_ctr;
  rf_lock_data_type* tq_mdsp_mc_meas_crit_sec = NULL;

  RF_MSG_2( RF_HIGH, "rfwcdma_mc_init: START. dev %d cmn success %d", device, common_init_successful );

  /* Reset callback data for all devices */
  rfwcdma_core_rxctl_reset_dynamic_notch_cb_data();

  /* Initialize WCDMA Data module */
  rfwcdma_mc_reset_data();

  /* Initialise WCDMA Meas Radio data */
  rfwcdma_data_init_meas_data();

#ifdef FEATURE_QSH_MDUMP
  boolean ret_val = FALSE;
  ret_val = rf_qsh_cb_register(RFM_IMT_MODE,&rf_wcdma_QSH_mdump_cb);
#endif

  if (common_init_successful)
  {
    if (!nv_items_read)
    {
      nv_status = rfwcdma_nv_retrive_items(caller_tcb_ptr, task_nv_wait_sig,
                                           task_wait_func_ptr);

      if (nv_status == RFCOMMON_NV_READ_SUCCESS)
      {
        /* Not missing critical NV items */
        /* Assign NV table for each band to the global pointers. Do this whenever we retrieve NV */
        /* Loop through all of the possible NV bands. */
        for (band_ctr=RFCOM_BAND_IMT; band_ctr<RFCOM_NUM_WCDMA_BANDS; band_ctr++)
        {
          rfnv_wcdma_tbl_ptr[band_ctr] = rfwcdma_core_init_nv_table(band_ctr);
        }

        nv_init_success = TRUE;
      }

      rfwcdma_mdsp_init_critical_section();

      /* Initialize WCDMA Locks as the FIRST step in MC init for WCDMA. This is
      to prevent cases where rfm_init may fail, but other modules call RFM APIs.
      */
      rfwcdma_set_lock_data(rfm_get_lock_data());

      /* multi-lin data NV read is handled seperately*/
      rfwcdma_core_txlin_init();

      /* FBRX Cal data NV read is handled seperately */
      rfcommon_fbrx_nv_retrieve_wcdma_data( caller_tcb_ptr,
                                            task_nv_wait_sig,
                                            task_wait_func_ptr);

      rf_common_init_critical_section(rfwcdma_core_temp_comp_get_lock_data());

      /* Remember that we've read the NV. */
      nv_items_read = TRUE;
    }
  }   /* if ( common_init_successful ) */
  else
  {
    RF_MSG( RF_ERROR, "rfwcdma_mc_init: main tech init skipped due to cmn init failure. Cannot operate in WCDMA." );
  }

  /* Perform command processing initalization */
  rfwcdma_cmd_proc_init();

  /* register the IRAT APIs with meas module */
  rfwcdma_mc_meas_init();

  /* Initialise the MDSP TQ critical section common to standalone and measurement */
  tq_mdsp_mc_meas_crit_sec = rfwcdma_data_get_meas_mc_mdsp_crit_sec_data();

  if (tq_mdsp_mc_meas_crit_sec == NULL) 
  {
    RF_MSG(RF_ERROR,"rfwcdma_mc_init: MC_MEAS Critical section data is NULL in RFWCDMA data");
  }
  else
  {
    rf_common_init_critical_section(tq_mdsp_mc_meas_crit_sec);
  }

  /* Initialise the Power on/off Critical section*/
  rf_common_init_critical_section(rfwcdma_get_power_on_off_lock_data());
  
  /* Initialise the critical section for AGC logging where W is in both subs */
  rf_common_init_critical_section(rfwcdma_get_agc_log_lock_data());  

  /* Initialise the critical section between AOL updates and Tx disable */
  rf_common_init_critical_section(rfwcdma_get_tuner_update_lock_data());   

  /* Initialize WCDMA log */
  rfwcdma_log_init();

  rfwcdma_core_create_npa_client();
  
  RF_MSG_2( RF_HIGH, "rfwcdma_mc_init: END. dev %d success %d", device, nv_init_success );
  
  /*! Return success based on NV read success. */
  return nv_init_success;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
                This function initializes the transmit chain for WCDMA uplink operation.

        @details
        It initializes Tx parameter table, mDSP variables, and MODEM Tx registers.
        All function calls are come from rfwcdma_mc_enable_tx( ) to reduce
        enable_tx( ) timing. Long enable_tx() timing may cause RACH No-Ack and
        DCH channel setup failure due to missing WL1 timeline.
        This function should be called before rfm_enable_tx( ) is called.

        @return
        TRUE if all RFcal data is present
        FALSE if missing RFcal data
*/
boolean rfwcdma_mc_init_tx(rfcom_device_enum_type device, 
                           uint32 txlm_buf_idx, 
                           uint32 multi_carrier_idx, 
                           uint16 tx_channel[RFM_WCDMA_MAX_CARRIERS])
{
  uint8 i,num_channels;
  uint32 fbrx_channels[RFCOM_MAX_CARRIERS];
  uint16 channels[RFCOM_MAX_CARRIERS];
  uint32 prev_carr_mask = 0;
  boolean status = TRUE;
  rfcommon_fbrx_error_type fbrx_status = RFCOMMON_FBRX_SUCCESS;
  rfcom_band_type_u fbrx_band = {0};
  rfcommon_fbrx_tx_cfg_type fbrx_cfg = RFCOMMON_FBRX_UMTS_SC;
  rfcommon_fbrx_tx_param_type fbrx_tx_params;
  timetick_type prof_cal_func;
  rfcom_tx_multi_carrier_cfg_type tx_cfg_mode = RFCOM_TX_SINGLE_CAR_CFG;
  boolean tx_bw_change = FALSE;
  rfwcdma_mdsp_set_data_param_type tx_param;
  uint8 fbrx_modem_chain = 3;
  rfwcdma_mdsp_data_type *data = rfwcdma_mdsp_get_data();

  prev_carr_mask = rfwcdma_data_get_carrier_mask(device);
  tx_bw_change = IS_BW_CHANGE(prev_carr_mask, multi_carrier_idx);

  if (device >= RFM_MAX_WAN_DEVICES)
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_init_tx: Error api called with invalid device %d. success %d. Cannot init TX.",
              device, 0 );
    return 0;
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "RF Tx initialization started on device %d", device);

  if ( rfwcdma_core_convert_ul_chan_to_band(tx_channel[0]) >= RFCOM_NUM_WCDMA_BANDS )
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_mc_init_tx ERROR invalid band %d. Cannot init TX.",
              rfwcdma_core_convert_ul_chan_to_band( tx_channel[0]) );
    return 0;
  }

  if ( rfc_common_check_band_supported( rfcommon_core_convert_rfcom_band_to_sys_band(RFM_IMT_MODE, (rfcom_band_type_u)rfwcdma_core_convert_ul_chan_to_band( tx_channel[0] )), device ) == FALSE )
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_init_tx ERROR device %d does not support WCDMA RF band %d.",
              device, rfwcdma_core_convert_ul_chan_to_band( tx_channel[0]));
    return 0;
  }

    /* Prepare for Multi-Carrier config */
  fbrx_channels[0] = (uint32)(tx_channel[0]);
  fbrx_channels[1] = (uint32)(tx_channel[1]);    
  channels[0] = tx_channel[0];
  channels[1] = tx_channel[1];

  /* FBRx common enter mode */
  fbrx_status = rfcommon_fbrx_mc_enter_mode( &rfwcdma_mc_fbrx_lm_handle, RFCOM_WCDMA_MODE, 0);
  if(fbrx_status == RFCOMMON_FBRX_ERROR)
  {
    RF_MSG(RF_FATAL, "rfcommon_fbrx_mc_enter_mode FBRx DM buffer allocation failed");
    //api_status &= FALSE;
  }

  /* Enable temp comp - should be moved to Tx init */
  rfwcdma_core_temp_comp_enable(TRUE,device);

  /*Init MTPL as part of Tx Config*/
  rfwcdma_core_txplim_reset_mtpl();

  /* This need to be moved to MDSP Tx cfg */
  rfwcdma_core_update_min_power_from_nv();

  
  /* Disable TxAGC in case its running. Only applies to SC<->DC transitions */
  if (tx_bw_change)
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "RF Tx BW change from %d to %d, disable TxAGC.",
        rfwcdma_data_get_carrier_mask(device), multi_carrier_idx);
	
  (void)rfwcdma_core_tx_disable_txagc((rflm_dm_handle_id_t)txlm_buf_idx,TRUE);
  }

  /* Allocate Tx DM buffers for the given TxLM handle */
  (void)rfwcdma_mdsp_allocate_dm_tx_buf((rflm_dm_handle_id_t)txlm_buf_idx);

  /* Update state machine */
  rfwcdma_data_set_carrier_mask(device,multi_carrier_idx);

  rfwcdma_data_set_rflm_buf_idx(device,txlm_buf_idx);

  rfwcdma_data_set_num_carriers(device,rfwcdma_core_count_carriers(multi_carrier_idx));

  /*Update Tx freq to tx dm*/
  tx_param.rflm_handle = txlm_buf_idx;
  tx_param.set_data_cmd = RFWCDMA_MDSP_SET_TX_FREQ;
  tx_param.data.gen_dw = rfwcdma_core_get_freq_from_ul_arfcn(tx_channel[0]);
  rfwcdma_mdsp_async_set_data(&tx_param);

  /*Update CAL mode to tx dm*/
  if(IS_FTM_CAL_MODE())
  {
    tx_param.rflm_handle = txlm_buf_idx;
    tx_param.set_data_cmd = RFWCDMA_MDSP_SET_CAL_MODE;
    tx_param.data.gen_b = (uint8)TRUE;
    rfwcdma_mdsp_async_set_data(&tx_param);
  }
  /*This is to make sure that RF state is TX while doing transition SC->DC config and DC config ->SC*/
  if(rfwcdma_data_get_rf_state(device)!= RFWCDMA_STATE_TX)
  {
  rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_TX_PREP);
  }

  for (i = 0; i < RFCOM_MAX_CARRIERS; i++)
  {
    if ((multi_carrier_idx & (1 << i)) != 0)
    {
      rfwcdma_data_set_curr_band(device, rfwcdma_core_convert_ul_chan_to_band(tx_channel[0]));
      rfwcdma_data_set_curr_chan(device,i,tx_channel[i]);
    }
  }

  if(multi_carrier_idx == RFWCDMA_MC_DC)
  {
    tx_cfg_mode = RFCOM_TX_SC_RF_DC_BB_CFG ;
  }
  else if(multi_carrier_idx == RFWCDMA_MC_SC)
  {
    tx_cfg_mode = RFCOM_TX_SINGLE_CAR_CFG ; 
  }
  
  /* Init tx power limiting loop */
  rfwcdma_core_txplim_init(rfwcdma_data_get_curr_band(device),
                           rfwcdma_data_get_curr_chan(device,RFCOM_SINGLE_CARRIER),
                           multi_carrier_idx);

  /* Update TxLM buffer */
  status &= rfwcdma_msm_update_txlm_buffer(txlm_buf_idx, 
                                           device,
                                           rfwcdma_data_get_curr_band(device),
                                           RFWCDMA_MC_SC, 
                                           tx_cfg_mode);

/* Do FBRx initialization if FBRx ILPC is enabled in NV */
/* For WTR3k , FBRx init has to be happen irrespective of FBRx being enabled.  THe  reason is REF log captures happen for WTR3k  irrespective of FBRx enabled or not.  Hence dont remove this. Removing this will cause crash in C-FW as REF log settings will be empty */
  #ifdef FEATURE_RF_HAS_WTR2965 
  if( rfcommon_fbrx_mc_is_ilpc_enabled(RFLM_TECH_WCDMA) )
  #endif
  {
  /* FBRx config */
  if (multi_carrier_idx == RFWCDMA_MC_DC)
  {
    fbrx_cfg = RFCOMMON_FBRX_UMTS_DC;
  }
  else if (multi_carrier_idx == RFWCDMA_MC_SC)
  {
    fbrx_cfg = RFCOMMON_FBRX_UMTS_SC;
  }
  else
  {
    RF_MSG_1(RF_FATAL, "rfwcdma_mc_init_tx: unsupported multi_carrier_idx:%d", multi_carrier_idx);
  }

#ifdef FEATURE_RF_HAS_WTR3925
if( rfcommon_fbrx_mc_is_ilpc_enabled(RFLM_TECH_WCDMA) )
{
  data->intf_data.wfw_intf_ptr->txAsyncWriteBuf.agc.wtr3kFlag = 1;
}
else
{	
  data->intf_data.wfw_intf_ptr->txAsyncWriteBuf.agc.wtr3kFlag = 0;
}
 #endif

 fbrx_band.wcdma_band = rfwcdma_data_get_curr_band(device);

  /* Allocate Tx DM buffers for the given FBRxLM handle */
  fbrx_status = rfcommon_fbrx_mc_init_tx(rfwcdma_mc_fbrx_lm_handle, 
                                         fbrx_cfg, 
                                         txlm_buf_idx, 
                                         RFCOM_WCDMA_MODE, 
                                         0);

  fbrx_tx_params.apt_only_mode = FALSE;
  if (!(rfwcdma_core_xpt_is_enabled(fbrx_band.wcdma_band)))
  {
    fbrx_tx_params.apt_only_mode = TRUE;
  }
  
  fbrx_tx_params.mtpl_db10 = rfwcdma_core_txplim_get_mtpl_nv_item( RFCOM_TXPLIM_DCH, fbrx_band.wcdma_band );
  fbrx_tx_params.mtpl_db10 = fbrx_tx_params.mtpl_db10 + (RF_TXAGC_RESOLUTION * RF_MIN_TX_POWER_DBM);

  fbrx_modem_chain = (uint8)(rfc_wcdma_get_fb_chain(device,fbrx_band.wcdma_band));
  num_channels = (uint8)rfwcdma_core_count_carriers(multi_carrier_idx)+1;
  if(num_channels < RFCOM_MAX_CARRIERS)
  {
     fbrx_status &= rfcommon_fbrx_mc_tune_to_chan(device,
                 txlm_buf_idx,
                 RFCOM_WCDMA_MODE,
                 fbrx_band,
                 fbrx_channels,
                 num_channels,
                 rfwcdma_mc_fbrx_lm_handle,
                 fbrx_modem_chain,
                 fbrx_tx_params,
                 RFCOMMON_FBRX_NV_CONTAINER_C0);
  }
  else
  {
     RF_MSG_1(RF_FATAL, "rfwcdma_mc_init_tx: unsupported num_channels:%u",num_channels);
  }
  
#ifdef FEATURE_RF_HAS_WTR3925
  if (tx_bw_change)
  {
    /* Send TxLM update command to FW for SC/DC reconfiguration of FBRx */
    fbrx_status &= rfwcdma_mdsp_sync_send_cmd(RFWCDMA_MDSP_TXLM_UPDATE_CMD_ID,&txlm_buf_idx);    
  }
#endif

    /* Error fatal if there are any errors */
  if(fbrx_status == RFCOMMON_FBRX_ERROR)
  {
    RF_MSG(RF_FATAL, "rfcommon_fbrx_mc_init_tx Tx DM buffer allocation failed");
  }


  } /* if( ( rfcommon_fbrx_mc_is_ilpc_enabled(RFLM_TECH_WCDMA) ) ||
      ( rfcommon_fbrx_nv_get_control_val(RFLM_TECH_WCDMA) != RFCOMMON_FBRX_DISABLE_NO_UPDATES) ) */

  /*Regsiter the VSWR post processing call back function to FBRX module*/
  fbrx_status &= rfcommon_fbrx_mc_register_vswr_meas_call_back(rflm_wcdma_post_process_fbrx_vswr_measurements,
                                                               txlm_buf_idx);


  
	 /*Update input parameters for autopin captures */										
#ifdef FEATURE_RF_HAS_WTR2965 
  if (rfcommon_autopin_is_enabled(RFM_IMT_MODE))
  {
   rfwcdma_core_autopin_init(rfwcdma_data_get_rflm_buf_idx(device));
  }
#endif


  /* Configure MDSP and FED modules */
  rfwcdma_core_txctl_config_band( device,
                                  rfwcdma_data_get_curr_band(device), 
                                  txlm_buf_idx, 
                                  tx_bw_change);


  /* Tx channel config of MDSP/FED layer */
  rfwcdma_core_txctl_config_chan(device,
                                 rfwcdma_data_get_curr_band(device),
                                 rfwcdma_data_get_curr_chan(device,RFCOM_SINGLE_CARRIER),
                                 txlm_buf_idx,
                                 tx_bw_change);
  

  /* Set sar limit */
  rfwcdma_core_txplim_set_sar_limit(rfm_common_sar_get_state());

  /* Only initialized PDA, will be updated to actuall setting in retune_bw */
  rfwcdma_core_txctl_config_mc(rfwcdma_data_get_curr_band(device), RFWCDMA_MC_SC , channels, txlm_buf_idx,tx_cfg_mode);

  rfwcdma_core_rxctl_enable_tuner(device,
                                  rfwcdma_data_get_curr_band(device),
                                  rfwcdma_data_get_num_carriers(device),
                                  rfwcdma_data_get_curr_chan(device,0),
                                  RFDEVICE_EXECUTE_IMMEDIATE,
                                  NULL);
  if (tx_bw_change)
  {
    /* Update MSM Tx bandwidth settings */
    (void)rfwcdma_msm_update_tx_bw(rfwcdma_core_count_carriers(multi_carrier_idx), txlm_buf_idx);

    if (!IS_FTM_CAL_MODE())
  {
    /* Reenable TxAGC in case its running. Should only happen in SC<->DC transitions */
    (void)rfwcdma_core_tx_disable_txagc((rflm_dm_handle_id_t)txlm_buf_idx,FALSE);
  }
  }

  /* Check status */
  if (status == FALSE)
  {
    RF_MSG_1(RF_HIGH, "RF Tx initialization failed for band:%d",
             rfwcdma_data_get_curr_band(device));
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Init Tx - %d", prof_cal_func);
  }

  if (tx_bw_change)
  {  
    rfwcdma_core_txplim_enable(TRUE);
  }
  
  rflm_dtr_tx_read_dac_status_tx_all_done(0,&combodac_all_done[0]);
/*Checking combodac_all_done if found to be in reset state in Tx init*/
  if (combodac_all_done[0] == 0)
  {
  
    rf_time_tick_type prof_cal_func; /* Function Profiling Variable */
    rf_time_type prof_cal_func_val = 0; /* Function Profiling Value Variable */
	
    prof_cal_func = rf_time_get_tick();

    rfcommon_msm_dac_mission_mode_enter();
 
    prof_cal_func_val = rf_time_get_elapsed( prof_cal_func, RF_USEC );
 
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,
             "rfwcdma_mc_init_tx : rflm_check_dac_cal_machine_status during WCDMA TX init is %d [expected 0]",
              rflm_check_dac_cal_machine_status());
  }
  
  RF_MSG_1(RF_HIGH, "RF Tx initialization is complete", 0);

  /* For RUMI, it is expected to fail without QCN and valid Cal data */
#ifdef T_RUMI_EMULATION
  return TRUE;
#else
  return status;
#endif
}

/*!
        * @brief Prepare CCS memory after a power collapse.
        *
        * @details After a power collapse we lose any contents in CCS memory.
        * This function will re-populate the TX information. This allows for
        * a shorter transition to TX then going through
        * rfwcdma_mc_init_tx.
        *
        * \author kylej (3/18/2013)
        */
boolean rfwcdma_mc_tx_wakeup( rfm_device_enum_type device, 
                              uint32 txlm_buf_idx, 
                              uint32 multi_carrier_idx,
                              uint16 tx_channel)
{
  timetick_type prof_cal_func;
  rfcom_wcdma_band_type tx_band;

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  tx_band = rfwcdma_core_convert_ul_chan_to_band(tx_channel);
  
  /* Populate Tx AGC configuration to RFLM through common Tx AGC configuration */
  rfwcdma_core_txctl_txagc_config(device,
                                  tx_band, 
                                  txlm_buf_idx);

  /* Reload DPD tables */
  rfwcdma_core_txlin_reload_dpd(device,
                                tx_band,
                                tx_channel,
                                txlm_buf_idx);

  rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_TX_PREP);

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_4(RF_HIGH, "RFWCDMA Execution Time: rfwcdma_mc_wakeup_tx - %dus", prof_cal_func, device, tx_band, txlm_buf_idx);
  }

  return TRUE;
}/* rfwcdma_mc_tx_wakeup */

void rfwcdma_mc_wakeup_tx(void)
{
  rfm_device_enum_type device = RFM_INVALID_DEVICE;
  uint8 handle;
  timetick_type prof_cal_func;

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  handle = rfwcdma_data_get_rflm_buf_idx(device);
  
  if ( IS_VALID_TXLM_HANDLE(handle) )
  {
  /* Populate Tx AGC configuration to RFLM 
     through common Tx AGC configuration */
  rfwcdma_core_txctl_txagc_config(device,
                                  rfwcdma_data_get_curr_band(device), 
                                  handle );

  /* Reload DPD tables */
  rfwcdma_core_txlin_reload_dpd(device,
                            rfwcdma_mc_state.car_path_state[RF_PATH_0][RFCOM_MULTI_CARRIER_ID__0].curr_band_tx,
                            rfwcdma_mc_state.car_path_state[RF_PATH_0][RFCOM_MULTI_CARRIER_ID__0].curr_chan_tx,
                            handle );
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: rfwcdma_mc_wakeup_tx - %dus", prof_cal_func);
}
}/* rfwcdma_mc_wakeup_tx */

/*----------------------------------------------------------------------------*/
/*!
        @brief
        This is used to set an RF physical path (antenna -> demod -> RxFE, etc.) to
        the RFCOM_TRANSCEIVER_0 logical device.  Typically, the RFCOM_TRANSCEIVER_0
        is mapped with the physical RF path 0 (intuitive!).
        However, for certain test cases, we might want to exercise the secondary (diversity)
        physical RF path, and therefore map it to the logical device RFCOM_TRANSCEIVER_0.

        This also has the effect of mapping the "other" RF physical path to the "other"
                logical device.  For example calling rfm_set_primary_path( RF_PATH_0)
                maps RFCOM_TRANSCEIVER_0 logical device to RF_PATH_0 physical device AND
                maps RFCOM_RECEIVER_DIV logical device to RF_PATH_1 physical device.

        @details
*/
void rfwcdma_mc_set_primary_path(rf_path_enum_type primary_path)
{
  rfwcdma_mc_state.primary_path = primary_path;
}

void rfwcdma_mc_set_prx_path(rf_path_enum_type primary_path)
{
  (void)rfwcdma_data_set_prx_path(primary_path);
}
/*----------------------------------------------------------------------------*/
/*!
        @brief
        This function is used to get the current primary path and return to L1

        @details
*/
rf_path_enum_type rfwcdma_mc_get_primary_path(void)
{
  return rfwcdma_mc_state.primary_path;
}


/*----------------------------------------------------------------------------*/
/*!
        @brief
        This function writes tech dependent (R99 or HS) lna nv items to mdsp

        @details
*/
void rfwcdma_mc_write_tech_dependent_nv_items_mdsp(rfm_device_enum_type device, rfcom_wcdma_band_type band, rfcom_tech_type tech)
{
  rfwcdma_mdsp_set_data_param_type lna_swicthpoints = {0};
  uint8 lna_state = 0;
  rfwcdma_nv_rx_static_element_type rise_element, fall_element = 0;
  boolean api_status = TRUE;
  uint32 nv_container =0;
  rfc_device_info_type *device_info_ptr;
  rfc_cfg_params_type cfg;
  boolean is_consolidated = TRUE;
  rfnv_wcdma_nv_tbl_type *nv_tbl = rfnv_wcdma_get_nv_tbl_ptr(rfnv_wcdma_convert_band_rfcom_to_rfnv(band));

  if(nv_tbl != NULL)
  {
    is_consolidated = nv_tbl->rx_static_data.payload.rx_stat_v2.is_consolidated;
  }
  else
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_mc_write_tech_dependent_nv_items_mdsp: NV ptr is NULL for band %d",
		         band);
    api_status = FALSE;
  }

  /*No need to update diversity LNA swicthpoints when consolidated RF NVs are not present*/
  if((is_consolidated == FALSE) && (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE))
  {
    
     RF_MSG_2( RF_HIGH, "rfwcdma_mc_write_tech_dependent_nv_items_mdsp: Early Return for device:%d, is_cons %d",
				  device, is_consolidated);
     return;
  }
  
  RF_MSG_3( RF_HIGH, "rfwcdma_mc_write_tech_dependent_nv_items_mdsp: For Device:%d, band: %d tech is: %d",
              device, band, tech);

  cfg.alternate_path = 0;
  cfg.band = band;
  cfg.logical_device = device;
  cfg.rx_tx = RFC_CONFIG_RX;
  cfg.req = RFC_REQ_DEFAULT_GET_DATA;


  if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == TRUE)
  {
    RF_MSG_3( RF_HIGH, "rfwcdma_mc_write_tech_dependent_nv_items_mdsp: For Device:%d, band: %d NV container is: %d",
              device, band, device_info_ptr->nv_container);
  }
  else
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_write_tech_dependent_nv_items_mdsp - Cannot get NV container for"
                         "device %d Div config, band %d",device,band);

  return;
  }

  if (device_info_ptr->nv_container < RFM_MAX_WAN_DEVICES)
  {
    nv_container = device_info_ptr->nv_container; 
  } 

  /* Check R99/HS mode and update the NV element before query NV module */
  if (tech == RFCOM_REL99_MODE)
  {
    rise_element = LNA_RISE;
    fall_element = LNA_FALL;
  }
  else if (tech == RFCOM_HS_MODE)
  {
    rise_element = LNA_HS_RISE;
    fall_element = LNA_HS_FALL;
  }
  else
  {
    RF_MSG_1(RF_ERROR,"rfwcdma_mc_write_tech_dependent_nv_items_mdsp, invalid tech:%d",
             tech);
    return;
  }

  if(nv_tbl != NULL)
  {
    /* Get FGAGC LNA switchpoints */
    for (lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES - 1; lna_state++)
    {
      /* HS/R99 switchpoint change only applicable to device 0 (PRx) in single carrier mode  */
      lna_swicthpoints.data.set_lna_switchpoint.lnarise[lna_state] =
       rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type)nv_container,
                                                RFCOM_SINGLE_CARRIER,
                                                rise_element,
                                                (lna_state),
                                                &nv_tbl->rx_static_data);

      lna_swicthpoints.data.set_lna_switchpoint.lnafall[lna_state] =
       rfnv_wcdma_get_info_from_rx_static_data((rfnv_wcdma_container_enum_type)nv_container,
                                                RFCOM_SINGLE_CARRIER,
                                                fall_element,
                                                (lna_state),
                                                &nv_tbl->rx_static_data);
     }
   }

  /* Update set data command */
  lna_swicthpoints.set_data_cmd = RFWCDMA_MDSP_SET_LNA_SWITHPOINT;

  /* Update RFLM handle */
  lna_swicthpoints.rflm_handle = rfwcdma_data_get_rflm_buf_idx(device);

  /* Call mdsp function to update switchpoints */
  api_status &= rfwcdma_mdsp_async_set_data(&lna_swicthpoints);

  if (!api_status)
  {
    RF_MSG_2(RF_ERROR,"rfwcdma_mc_write_tech_dependent_nv_items_mdsp,failed! band:%d, tech:%d ",
             band,tech);
  }
  else
  {
    RF_MSG_2(RF_MED,"rfwcdma_mc_write_tech_dependent_nv_items_mdsp,success.. band:%d, tech:%d ",
             band,tech);
  }
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        This function writes tech dependent (R99 or HS) ici coeffs from the rxlm

        @details
*/
boolean rfwcdma_mc_write_tech_dependent_dynamic_update(rfcom_tech_type curr_tech, uint8 rxlm_buf_idx[], rfcom_device_enum_type device)
{
  RF_MSG(RF_MED,"rfwcdma_mc_write_tech_dependent_dynamic_update: nothing to be done");
  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        Called by L1 every 10sec to update temp comp data.

        @details
*/
void rfwcdma_mc_do_temp_comp(rfm_device_enum_type device)
{

  if (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_TX)
  {
    rfwcdma_core_temp_comp_update(device);
  }

}


/*----------------------------------------------------------------------------*/
/*!
        @brief
        Dispatch WCDMA command.

        @details

        @param device devices supported by RF driver.
        @param cmd Specifies the command.
        @param *data Input and/or Output parameter for the command

        @retval value generic return value which depends on the 'cmd' parameter.

*/
int32 rfwcdma_mc_cmd_dispatch(rfcom_device_enum_type device, int32 cmd,
                              void *data)
{
  int32 value = 0; 
  uint8 alt_path = 0;
  rf_path_enum_type path;
  rfc_wcdma_dc_supported_type dc_support;
  rfcom_wcdma_band_type band;
  sys_band_mask_type sys_band_mask;
  rfagc_receive_chain_type rx_chain;
  int16 rxagc_val = 0;
  rfcmn_status_type alt_path_status = RFCMN_PATH_SEL_SUCCESS;
  uint32 multi_carrier_idx = 1;  /* Pass multi carrier index always as 1 for W2W meas RxLM update*/
  rfcom_device_enum_type device_id = RFM_DEVICE_0;
  rfwcdma_mc_rxagc_input_params rxagc_input_info;
  rfwcdma_mc_rxagc_output_params rxagc_output_info;


  /*! Convert rfcom_device_enum_type to rf_path_enum_type */
  path = rfcommon_core_device_to_path(device);

  if (path > RF_PATH_1)
  {
    RF_MSG_1(RF_ERROR, "Invalid Rx path :%d", path);
    return (value);
  }

  /*! If it is diversity only call, change the path to diversity */
  if ((path == RF_PATH_0) && (rfwcdma_data_get_prx_path() == RF_PATH_1))
  {
    path = RF_PATH_1;
  }

  rx_chain = rfcommon_core_path_to_receive_chain(path);

  switch (cmd)
  {
  case RFWCDMA_MC_IS_SLEEPING:
    value = (int32)rfwcdma_core_util_is_sleeping(device);
    break;

  case RFWCDMA_MC_GET_RX_AGC:
    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_MED, "rfwcdma_mc_cmd_dispatch: Invalid device %d", device);
    }
    else
    {
      rxagc_input_info.ant_idx = rfwcdma_core_util_device_to_antenna(device);
      rxagc_input_info.band = rfwcdma_data_get_curr_band(device);
      rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__0;
      rxagc_input_info.device = device;
      rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
      rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
      (void)rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
      value = (int32)rxagc_output_info.rxagc_val;
    }	  
    break;

  case RFWCDMA_MC_GET_RAW_RX_AGC:
    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_MED, "rfwcdma_mc_cmd_dispatch: Invalid device %d", device);
    }
    else
    {
      rxagc_input_info.ant_idx = rfwcdma_core_util_device_to_antenna(device);
      rxagc_input_info.band = rfwcdma_data_get_curr_band(device);
      rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__0;
      rxagc_input_info.device = device;
      rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC_RAW;
      rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;
      (void)rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
      value = (int32)rxagc_output_info.rxagc_val;
    }
    break;

  case RFWCDMA_MC_GET_TX_RX_RATIO:
    value= rfwcdma_mc_tx_rx_freq_ratio((uint32)((rfwcdma_mc_channel_type *)data)->chan[0]);
    break;

  case RFWCDMA_MC_GET_RAW_RX_AGC_X2W_MEAS:
    value = (int32)rfwcdma_core_util_get_rxagc_xtow_meas( (rfagc_receive_chain_type)path,RFCOM_INGAP_RXAGC_RAW, RFCOM_MULTI_CARRIER_ID__0,RFCOM_RXAGC_IN_DB10, ( (rfwcdma_mc_meas_rxagc_xtow*)data)-> wcdma_band,  ( (rfwcdma_mc_meas_rxagc_xtow*)data)-> bypass_rxagcon_flag );
    break;

  case RFWCDMA_MC_GET_RX_AGC_X2W_MEAS:
    value = (int32)rfwcdma_core_util_get_rxagc_xtow_meas( (rfagc_receive_chain_type)path,RFCOM_INGAP_RXAGC, RFCOM_MULTI_CARRIER_ID__0,RFCOM_RXAGC_IN_DBM, ( (rfwcdma_mc_meas_rxagc_xtow*)data)-> wcdma_band,  ( (rfwcdma_mc_meas_rxagc_xtow*)data)-> bypass_rxagcon_flag );
    break;

  case RFWCDMA_MC_GET_TX_AGC_VAL:
    value = (int32)rfwcdma_core_util_get_rflm_txagc_dBm();
    break;

  case RFWCDMA_MC_GET_RX_AGC_VAL_FOR_RSCP:
    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_MED, "rfwcdma_mc_cmd_dispatch: Invalid device %d", device);
    }
    else
    {  	
      rxagc_input_info.ant_idx = rfwcdma_core_util_device_to_antenna(device);
      rxagc_input_info.band = rfwcdma_data_get_curr_band(device);
      rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__0;
      rxagc_input_info.device = device;
      rxagc_input_info.rxagc_type = (*(rfcom_rxagc_type *)data);
      rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
      (void)rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
      value = (int32)rxagc_output_info.rxagc_val;
    }	  
    break;

  case RFWCDMA_MC_GET_RX_AGC_C0_VAL_FOR_RSCP:
    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_MED, "rfwcdma_mc_cmd_dispatch: Invalid device %d", device);
    }
    else
    { 	
      rxagc_input_info.ant_idx = rfwcdma_core_util_device_to_antenna(device);
      rxagc_input_info.band = rfwcdma_data_get_curr_band(device);
      rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__0;
      rxagc_input_info.device = device;
      rxagc_input_info.rxagc_type = (*(rfcom_rxagc_type *)data);
      rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
      (void)rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
      value = (int32)rxagc_output_info.rxagc_val;
    }
    break;

  case RFWCDMA_MC_GET_RX_AGC_C1_VAL_FOR_RSCP:
    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_MED, "rfwcdma_mc_cmd_dispatch: Invalid device %d", device);
    }
    else
    { 
      rxagc_input_info.ant_idx = rfwcdma_core_util_device_to_antenna(device);
      rxagc_input_info.band = rfwcdma_data_get_curr_band(device);
      rxagc_input_info.car_id = RFCOM_MULTI_CARRIER_ID__1;
      rxagc_input_info.device = device;
      rxagc_input_info.rxagc_type = (*(rfcom_rxagc_type *)data);
      rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
      (void)rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
      value = (int32)rxagc_output_info.rxagc_val;
    }
    break;

  case RFWCDMA_MC_GET_MIN_TX_PWR_VAL:
    value = (int32)rfwcdma_core_util_get_ue_min_tx_power();
    break;

  case RFWCDMA_MC_GET_HDET_THRESHOLD_AGC:
    value = (int32)rfwcdma_core_txplim_get_hdet_threshold();
    break;

  case RFWCDMA_MC_GET_TX_AGC_TABLES:
    value = (int32)rfwcdma_core_util_get_txagctablestore((*(rfcom_multi_carrier_id_type *)data));
    break;

  case RFWCDMA_MC_TX_AGC_TO_DBM:
    value = (int32)rfwcdma_core_util_convert_tx_agc_to_dbm_unit(*(int16 *)(data));
    break;

  case RFWCDMA_MC_TX_DBM_TO_AGC:
    value = (int32)rfwcdma_core_util_convert_tx_dbm_to_agc_unit(*(int16 *)(data));
    break;

  case RFWCDMA_MC_GET_UE_MAX_TX_PWR:
    value = (int32)rfwcdma_core_util_get_ue_max_tx_power(*(rfcom_txplim_channel_type *)(data));
    break;

  case RFWCDMA_SET_TX_FREEZE_ACUUM:
    value = (int32)rfwcdma_core_txctl_set_tx_freezeaccum(((rfwcdma_mc_tx_freezeaccum_type *)data)->val, ((rfwcdma_mc_tx_freezeaccum_type *)data)->car_idx);
    break;

  case RFWCDMA_MC_ENABLE_PWR_CTRL:
    value = (int32)rfwcdma_core_txctl_enable_pwr_ctrl();
    break;

  case RFWCDMA_MC_SET_TX_PWR_LIMIT:
    rfwcdma_core_txplim_set_mtpl(((rfwcdma_mc_tx_pwr_limit_type *)data)->tx_limit_dbm, ((rfwcdma_mc_tx_pwr_limit_type *)data)->chan_type);
    break;

  case RFWCDMA_MC_SET_MCS_TX_PWR_LIMIT:
    rfwcdma_core_txplim_set_mcs_mtpl(((rfwcdma_mc_tx_pwr_limit_type *)data)->tx_limit_dbm);
    break;

  case RFWCDMA_MC_GET_TX_ROTATOR_ANGLE:
    rfwcdma_core_util_get_tx_rotator_angle(device,data);
    break;

  case RFWCDMA_MC_PA_IS_ON:
    value = (int32)rfwcdma_core_util_get_rflm_tx_on();
    break;

  case RFWCDMA_MC_GET_ENC_BTF_VAL:
    if(data != NULL)
    {
    value = (int32)rfwcdma_core_get_enc_btf_val(*((rfcom_wcdma_band_type*)data));
    }
    else
    {/*Need to have this hack till WL1 changes are in place.*/
      value = (int32)rfwcdma_core_get_enc_btf_val(RFCOM_BAND_IMT);
    }
    break;

  case RFWCDMA_MC_GET_MAX_TX_PWR_LIMIT:
    rfwcdma_core_txplim_get_mtpl_backoff((rf_max_tx_pwr_limit_type *)data);
    break;

  case RFWCDMA_MC_TURN_ON_RF_LDO:
    RF_MSG_1(RF_ERROR, "rfwcdma_mc_turn_on_rf_ldo() - Device - %d", device);
    break;

  case RFWCDMA_MC_GET_CURR_BAND:
    value = (int32)rfwcdma_core_convert_band_rfcom_to_rfi(rfwcdma_data_get_curr_band(device));
    break;

  case RFWCDMA_MC_GET_MAX_TX_PWR_CONFIG_ITEM:
    band = rfwcdma_data_get_curr_band(device);// Use Rx band as Tx band has not been initialised
    value = (int32)rfwcdma_core_txplim_get_mtpl_nv_item(*(rfcom_txplim_channel_type *)data, band);
    value = (value / RF_TXAGC_RESOLUTION) + RF_MIN_TX_POWER_DBM;
    break;

  case RFWCDMA_MC_GET_RF_WARMUP_TIME:
    value = (int32)rfwcdma_core_util_get_rf_warmup_time();
    break;

  case RFWCDMA_MC_GET_RF_CAPABILITY:
    value = rfwcdma_core_util_get_rf_capability(*(rfcom_rf_cap_type *)data);
    break;

  case RFWCDMA_MC_GET_MDSP_TX_AGC:
    value = (int32)rfwcdma_core_util_get_rflm_txagc_dB10();
    break;

  case RFWCDMA_MC_GET_MDSP_TX_PWR_LIMIT:
    value = (int32)rfwcdma_core_util_get_mdsp_tx_power_limit();
    break;

  case RFWCDMA_MC_BAND_SUPPORTS_RXD:
    if (rfwcdma_core_convert_band_rfi_to_rfcom(*(rfi_band_type *)data) >= RFCOM_NUM_WCDMA_BANDS)
    {
      value = FALSE;
    }
    else
    {
      device_id = rfwcdma_data_get_first_prx_device();
     if(device_id != RFM_MAX_WAN_DEVICES)
     {
      sys_band_mask = rfwcdma_core_get_sys_band_mask_from_band_rfi(*(rfi_band_type *)data);
      if (rfcommon_nv_get_rf_bc_config((rf_path_enum_type)rfwcdma_data_get_associated_device_rx(device_id)) & sys_band_mask)
      {
        value = TRUE;
      }
      else
      {
        value = FALSE;
      }
    }
    }
    break;

  case RFWCDMA_MC_BAND_SUPPORTS_DC:
    dc_support.band = rfwcdma_core_convert_chan_to_band((*(uint16 *)data));
    (void)rfc_wcdma_command_dispatch(RFM_DEVICE_0, RFC_WCDMA_IS_DUAL_CARRIER_SUPPORTED, (void *)(&dc_support));
    value = dc_support.dc_is_supported;
    break;

  case RFWCDMA_MC_GET_TX_PWR_LIMIT:
    value = (int32)rfwcdma_core_txplim_get_mtpl();
    break;

  case RFWCDMA_MC_GET_MCS_TX_PWR_LIMIT:
    value = (int32)rfwcdma_core_txplim_get_mcs_mtpl();
    break;

  case RFWCDMA_MC_GET_RX_AGC_VAL:
  case RFWCDMA_MC_GET_RX_AGC_C0_VAL:
  case RFWCDMA_MC_GET_RX_AGC_C1_VAL:

    /* Get the RxAGC value from MDSP */
    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_MED, "rfwcdma_mc_cmd_dispatch: Invalid device %d", device);
    }
    else
    { 
      rxagc_input_info.ant_idx = rfwcdma_core_util_device_to_antenna(device);
      rxagc_input_info.band = rfwcdma_data_get_curr_band(device);
      rxagc_input_info.car_id = ((rfwcdma_mc_rxagc_type *)data)->carr_id;
      rxagc_input_info.device = device;
      rxagc_input_info.rxagc_type = ((rfwcdma_mc_rxagc_type *)data)->type;
      rxagc_input_info.rxagc_unit = ((rfwcdma_mc_rxagc_type *)data)->unit;
      (void)rfwcdma_core_util_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
      value = (int32)rxagc_output_info.rxagc_val;
    }
    break;

  case RFWCDMA_MC_TURN_ON_PA:
    rfwcdma_mdsp_set_pa(TRUE, rfwcdma_data_get_curr_band(device));
    break;

  case RFWCDMA_MC_TURN_OFF_PA:
    rfwcdma_mdsp_set_pa(FALSE, rfwcdma_data_get_curr_band(device));
    break;

  case RFWCDMA_MC_GET_FREQ_FROM_UARFCN:
    band = rfwcdma_core_convert_chan_to_band(((rfwcdma_mc_channel_type *)data)->chan[0]);
    value = (int32)rfwcdma_core_get_freq_from_uarfcn(band, ((rfwcdma_mc_channel_type *)data)->chan[0], RFWCDMA_CORE_UARFCN_DL);
    break;

  case RFWCDMA_MC_GET_UL_FREQ_FROM_DL_UARFCN:
    value = (int32)rfwcdma_core_get_freq_from_uarfcn(((rfwcdma_mc_channel_type *)data)->band,
            (word)rfwcdma_core_map_rx_to_tx_chan((uint16)(((rfwcdma_mc_channel_type *)data)->chan[0]), ((rfwcdma_mc_channel_type *)data)->band),
            RFWCDMA_CORE_UARFCN_UL
                                                    );
    break;


  case RFWCDMA_MC_GET_LO_FREQ_FROM_UARFCN:
    band = rfwcdma_core_convert_chan_to_band(((rfwcdma_mc_channel_type *)data)->chan[0]);
    value = (int32)((rfwcdma_core_get_freq_from_uarfcn_no_limit(band, ((rfwcdma_mc_channel_type *)data)->chan[0], RFWCDMA_CORE_UARFCN_DL)\
                     + rfwcdma_core_get_freq_from_uarfcn_no_limit(band, ((rfwcdma_mc_channel_type *)data)->chan[1], RFWCDMA_CORE_UARFCN_DL)) / 2);
    break;

  case RFWCDMA_MC_CONVERT_CHAN_TO_BAND:
    value = (int32)rfwcdma_core_convert_chan_to_band(*(uint16 *)data);
    break;

  case RFWCDMA_MC_GET_HDET_TX_AGC_VALUE:
    value = (int16)rfwcdma_mdsp_get_hdet_tx_agc_value();
    break;

  case RFWCDMA_DO_TEMP_COMP:
    rfwcdma_mc_do_temp_comp(device);
    break;

  case RFWCDMA_MC_GET_MDSP_BUF_SIZE_W16:
    value = (int32)RFWCDMACM_MDSP_WTOW_SETUP_SIZE;
    break;

  case RFWCDMA_MC_GET_WTOW_RF_CTL_BUF:
#ifndef FEATURE_WCDMA_PLT
    /*rfmeas_mc_wtow_get_rf_ctl_buf(((rfwcdma_mc_wtow_ctl_type *)data)->arfcn,
                                                                                                                            ((rfwcdma_mc_wtow_ctl_type *)data)->buf_ptr);*/
#endif
    break;

  case RFWCDMA_MC_WRITE_TECH_DEPENDENT_CONFIG_TO_MDSP:
    band = rfwcdma_data_get_curr_band(device);
    rfwcdma_mc_write_tech_dependent_nv_items_mdsp(device, band, *(rfcom_tech_type *)data);
    break;

  case RFWCDMA_MC_GET_MULTI_CARRIER_WTOW_RF_CTL_BUF:
#ifndef FEATURE_WCDMA_PLT
#endif
    break;

  case RFWCDMA_MC_GP_CLOCK_ON:
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "RFWCDMA_MC_GP_CLOCK_ON: deprecated API", 0);
    break;

  case RFWCDMA_MC_GP_CLOCK_OFF:
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "RFWCDMA_MC_GP_CLOCK_OFF: deprecated API", 0);
    break;

  case RFWCDMA_REDUCE_MTPL_FOR_THERM:
    rfwcdma_core_txplim_reduce_mptl_db_for_therm(*((uint8 *)data));
    break;

  case RFWCDMA_MC_UPDATE_RXLM_BUFFER:
    
    /* Get Alternate path info from RFC */
    alt_path_status = rfcmn_concurrency_mgr_update_rx_path( device,
                                                            RFCOM_WCDMA_MODE,
                                                            (int)rfwcdma_core_convert_chan_to_band(((rfwcdma_mc_rxlm_param_type *)data)->channel[0]),
                                                            &alt_path);

    if (alt_path_status == RFCMN_PATH_SEL_FAILED) 
    {
       MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "MC cmd dispath update rxlm buffer: Alternate path read failed");
    }

    /* This API here is used only for neighbor RxLM update which is single carrier, so Pass multi carrier index always as 1*/
    value = (int32)rfwcdma_msm_update_rxlm_buffer(device,
            ((rfwcdma_mc_rxlm_param_type *)data)->channel,
           	multi_carrier_idx,
            ((rfwcdma_mc_rxlm_param_type *)data)->rxlm_buf_idx,
            (uint32)alt_path,
             enable_nbr_flag,
             NULL, RFWCDMA_MSM_STATIC_NOTCH_TUNE);
    break;

  case RFWCDMA_MC_GET_PRIMARY_PATH:
    value = (int32)rfwcdma_mc_get_primary_path();
    break;

  case RFWCDMA_MC_TRIGGER_SAR_UPDATE:
    rfwcdma_core_txplim_set_sar_limit(rfm_common_sar_get_state());
    break;

  case RFWCDMA_MC_SET_RX_FREEZE_ACCUM:
    rfwcdma_core_rxctl_set_rx_freeze_accum(device,
                                           *(boolean *)data);
    break;

  case RFWCDMA_MC_SET_QUIET_MODE:
    value = (int32)rfwcdma_mc_set_pmic_quiet_mode(*(boolean *)data);

    break;

  case RFWCDMA_MC_SET_LOWER_POWER_MODE:
    
    /* Get Alternate path info from RFC for Primary chain only as LPM is for SC PRx only*/
    alt_path_status = rfcmn_concurrency_mgr_update_rx_path( device,
                                                            RFCOM_WCDMA_MODE,
                                                            (int)rfwcdma_data_get_curr_band(device),
                                                            &alt_path);

    if (alt_path_status == RFCMN_PATH_SEL_FAILED) 
    {
       MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "MC cmd dispath set low power mode: Alternate path read failed");
    }

    value = (int32)rfwcdma_mc_set_lower_power_mode(device,((rfwcdma_mc_lpm_param_type *)data)->lpm_config,
            ((rfwcdma_mc_lpm_param_type *)data)->c0_rxlm_buf_idx,
            ((rfwcdma_mc_lpm_param_type *)data)->c1_rxlm_buf_idx,
            (uint32)alt_path);
    break;

  case RFWCDMA_MC_GET_PA_STATE:
    value = (int32)rfwcdma_core_util_get_rflm_pa_range();
    break;

  case RFWCDMA_MC_SET_CPC_STATE:
    rfwcdma_mc_state.cpc_state_flag = *(boolean *)data;
    break;

  case RFWCDMA_MC_SET_WAKEUP_OPTIMIZATION:
    // Temporarily disable wakeup optimizatino for Bolt bring up
    enable_wakeup_optimization_flag = *(boolean *)data;
    break;

  case RFWCDMA_MC_CONFIG_NBR:
    enable_nbr_flag = ((rfwcdma_mc_nbr_config_param_type *)data)->enable_nbr;
    nbr_rxlm_buf_idx = ((rfwcdma_mc_nbr_config_param_type *)data)->nbr_rxlm_idx;
    break;

  default:
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "Invalid Command Dispatch=%d", cmd);
    break;

  }

  return value;
}


/*----------------------------------------------------------------------------*/
/*!
        @brief

        rfdevice pointer must have been initialized before call this function

        @details
*/
void rfwcdma_mc_power_clock_on(rfcom_device_enum_type device)
{

  if (rfwcdma_mc_state.car_path_state[RF_PATH_0][0].curr_band_rx == RFCOM_BAND_INVALID)
  {
    rfc_wcdma_manage_vregs(RFM_DEVICE_0, RFCOM_BAND_IMT, RF_PATH_RX_STATE,RFM_IMT_MODE);

    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfwcdma_mc_power_clock_on (device=%d) (band = %d)", device, RFCOM_BAND_IMT);

  }
  else
  {
    rfc_wcdma_manage_vregs(RFM_DEVICE_0, rfwcdma_mc_state.car_path_state[RF_PATH_0][0].curr_band_rx, RF_PATH_RX_STATE, RFM_IMT_MODE);

    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfwcdma_mc_power_clock_on (device=%d) (band = %d)", device, rfwcdma_mc_state.car_path_state[RF_PATH_0][0].curr_band_rx);

  }

  /* vote to enable RFCMD app */
  rf_hal_bus_enable(TRUE, &rf_wcdma_hal_bus_client_handle, RF_HAL_RFCMD_APP_CLIENT(WCDMA));

}

/*----------------------------------------------------------------------------*/
/*!
        @brief

        rfdevice pointer must have been initialized before call this function

        @details
*/
void rfwcdma_mc_power_clock_off(rfcom_device_enum_type device)
{

  if (!rf_disable_powercollapse)
  {

    rfc_wcdma_manage_vregs(device, rfwcdma_mc_state.car_path_state[RF_PATH_0][0].curr_band_rx, RF_PATH_SLEEP_STATE,RFM_IMT_MODE);

    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfwcdma_mc_power_clock_off(device=%d) (band=%d)", device, rfwcdma_mc_state.car_path_state[RF_PATH_0][0].curr_band_rx);

  }

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set RF 3C tune status

  @details
  This function will check if RF is tuned to 3C or not

  @param device
   RFM Device to be used

  @param num_chan
   Carrier mode to which RF is tuned to. SC/DC/3C

*/
static void
rfwcdma_set_tri_carrier_status
(
  rfcom_device_enum_type device,
  rfcom_multi_carrier_hspa_id_type num_chan
)
{
  rf_path_enum_type path;

  path = rfcommon_core_device_to_path(device);

  if(num_chan == RFCOM_3_CARRIER)
  {
    wcdma_3c_active[path] = TRUE ;
  }
  else
  {
    wcdma_3c_active[path] = FALSE ;
  }

} /* rfwcdma_set_tri_carrier_status */

/*----------------------------------------------------------------------------*/
/*!
        @brief
        This function tunes the synthesizer to the specified channel.

        @details
        Once
        rf_tune_to_chan has been called, a delay in microseconds must be performed
        by the application to allow the synthesizer to settle.
        The channel is the UTRA Absolute Radio Frequency Channel Number (UARFCN).

        @param device
                RFM device to be tuned

        @param channel
                List of channels

        @param multi_carrier_idx
                Carrier ID combo (0/1/2)

        @param  user_data_ptr
                Used for CB

        @param cb_handler
                Call back handler

        @param rxlm_buf_idx
                RxLM buffer Index

*/
uint32 rfwcdma_mc_tune_to_multi_chan(rfcom_device_enum_type device,
                                     uint16 channel[],
                                     uint32 multi_carrier_idx,
                                     const void *user_data_ptr,
                                     rfm_cb_handler_type cb_handler,
                                     uint32 rxlm_buf_idx
                                    )
{

  
  if((device < RFM_DEVICE_0) || (device >=RFM_MAX_WAN_DEVICES))
	 {
	   RF_MSG_1(RF_ERROR, "rfwcdma_mc_tune_to_multi_chan: Invalid device, returning from here %d", device);
	   return 0;
	 }
  
  rf_path_enum_type path=0;
  rfcom_wcdma_band_type band;
  rfcom_wcdma_band_type source_band = RFCOM_BAND_INVALID;
  rfc_wcdma_core_config_type rfc_core_config;
  uint8 i, c;
  rfcom_multi_carrier_hspa_id_type num_chan;
  timetick_type prof_cal_func;
  timetick_type pll_wait_time;
  uint32 alt_path = 0;
  rfdevice_wcdma_spur_param_type *dyn_notch_filter_settings = NULL;
  rfdevice_wcdma_rx_tune_param_type rx_params;
  rfm_device_enum_type paired_dev;
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_enable_params = {0};
  boolean asdiv_alt_gain = FALSE;

#ifdef FEATURE_RF_ASDIV
  if(rfcommon_asdiv_get_current_position(device)==ASDIV_POSITION_1)
  {
    asdiv_alt_gain = TRUE;
  }
  else
  {
    asdiv_alt_gain = FALSE;
  }
#endif
  
  if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
    rx_params.rx_mode = RFDEVICE_PRX_OPER_MODE;
  }
  else if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE) 
  {
    rx_params.rx_mode = RFDEVICE_DRX_OPER_MODE;
  }
  else
  {
    rx_params.rx_mode = RFDEVICE_INVALID_OPER_MODE;
  }

  paired_dev = rfwcdma_data_get_associated_device_rx(device);

  if((paired_dev < RFM_DEVICE_0) || (paired_dev >= RFM_WCDMA_MAX_DEVICE))
	 {
	    RF_MSG_2(RF_HIGH, "rfwcdma_mc_tune_to_multi_chan: Invalid paired_dev %d for device %d ",paired_dev,device);
        return 0;
	 }
  

  rx_params.paired_device = paired_dev;
  rx_params.bw   = RFDEV_WCDMA_BW_INVALID;
  rx_params.band = RFCOM_BAND_INVALID;

  if (multi_carrier_idx == 0) 
  {
     RF_MSG_2(RF_ERROR, "rfwcdma_mc_tune_to_multi_chan: Multi carrier index=%d, must be non zero for device=%d", multi_carrier_idx, device);
     return 0;
  }

  /* Invalid # of WCDMA Carriers check, multi_carrier_idx is bit mask, rfwcdma_mc_count_carriers returns the carrir #*/
  if (rfwcdma_core_count_carriers(multi_carrier_idx) >= RFCOM_MAX_CARRIERS)
  {
    return 0;
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  path = rfcommon_core_device_to_path(device);

  band = rfwcdma_core_convert_chan_to_band(channel[0]);

  source_band = rfwcdma_mc_get_source_band(device);
  
  (void)rfc_wcdma_command_dispatch(device, RFC_WCDMA_GET_CORE_CONFIG, (void *)(&rfc_core_config));

  if (rfwcdma_data_get_rf_state(device)== RFWCDMA_MC_STATE_POWERUP_INIT)
  {
    return 0;
  }

  if ( rfc_common_check_band_supported( rfcommon_core_convert_rfcom_band_to_sys_band(RFM_IMT_MODE, (rfcom_band_type_u)band), device ) == FALSE )
  {
    ERR_FATAL(" Check with Wl1, rfwcdma_mc_tune_to_multi_chan: device %d does not support WCDMA RF band %d. Cannot tune RX ",
              device, band,0 );
  }

  /* Update Alt path to RFC*/
  rfwcdma_core_rxctl_update_alt_path(device, band, FALSE, &alt_path);

  /* Update state machine */
  rfwcdma_data_set_alt_path(device,alt_path);
  
  /* Maintain a shadow of the multi_carrier index for future use */

  if (rfwcdma_data_get_carrier_mask(device)!= 0)
  {
    rfwcdma_data_set_carrier_mask_shadow(device, rfwcdma_data_get_carrier_mask(device));
    //rfwcdma_mc_state.rx_multi_carrier_idx_shadow[device] = rfwcdma_mc_state.rx_multi_carrier_idx[device]; 
  }
  else 
  {
    rfwcdma_data_set_carrier_mask_shadow(device, multi_carrier_idx);
    //rfwcdma_mc_state.rx_multi_carrier_idx_shadow[device] = multi_carrier_idx; 
  }


  rfwcdma_data_set_carrier_mask(device, multi_carrier_idx);
  rfwcdma_data_set_num_carriers(device,rfwcdma_core_count_carriers(multi_carrier_idx));
  rfwcdma_data_set_rflm_buf_idx(device, rxlm_buf_idx);

#if 0
  rfwcdma_mc_state.rx_multi_carrier_idx[device] = multi_carrier_idx;
  rfwcdma_mc_state.rx_carriers[device] = rfwcdma_core_count_carriers(multi_carrier_idx);
  rfwcdma_mc_state.rxlm_buf_idx[device] = rxlm_buf_idx;
#endif

  num_chan = rfwcdma_data_get_num_carriers(device);

  /* Set the RxAGC enable params */
  memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));
  rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
  rxagc_enable_params.rxlm_buf_idx[device] = rxlm_buf_idx;
  rxagc_enable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);
  rxagc_enable_params.multi_carrier_idx = rfwcdma_data_get_carrier_mask_shadow(device);
  rxagc_enable_params.rf_warmup_en = FALSE;
  rxagc_enable_params.nbr_en_flag = FALSE;
  rxagc_enable_params.nbr_rxlm_buf_idx = 0;

  if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE)
  {
    /* For Primary Rx device, disable diversity on secondary too*/
    rxagc_enable_params.cmd_action[paired_dev] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
    rxagc_enable_params.ant_idx[paired_dev] = rfwcdma_core_util_device_to_antenna(paired_dev);

    /* Always disable AGC on all carriers */
    rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
  }
  else
  {
    rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
  }

  /* Allocate Rx DM buffers for the given RxLM handle */
  (void)rfwcdma_mdsp_allocate_dm_rx_buf((rflm_dm_handle_id_t)rxlm_buf_idx);

  /*! This function executes if the band changes. Band specific initialization occurs here */
  rfwcdma_mc_select_band_rx(device, band,multi_carrier_idx, rxlm_buf_idx, source_band);

  rfwcdma_set_tri_carrier_status(device,num_chan);

#if 0
  for (i = 0; i < RFCOM_MAX_CARRIERS; i++)
  {
    if ((multi_carrier_idx & (1 << i)) != 0)
    {
      rfwcdma_mc_state.car_path_state[path][i].curr_band_rx =
        rfwcdma_core_convert_chan_to_band(channel[rfwcdma_mc_state.rx_carriers[device]]);
      rfwcdma_mc_state.car_path_state[path][i].rx_rfm_device = device;
    }
  }
#endif  
  
  rfwcdma_data_set_curr_band(device,rfwcdma_core_convert_chan_to_band(channel[0]));

#ifdef FEATURE_RF_HAS_QTUNER
  if ( (source_band != RFCOM_BAND_INVALID) && (source_band != band) ) 
  {
    rfwcdma_core_rxctl_disable_tuner(device,
                                     source_band,
                                     RFDEVICE_EXECUTE_IMMEDIATE,
                                     NULL,
                                     RFCOM_INVALID_TIMING_OFFSET,
                                     RFWCDMA_CORE_ASM_TUNER_DIS_IND_BAND);
  }

  /* Program the tuner in Rx mode*/
  rfwcdma_core_rxctl_enable_tuner(device,
                                  band,
                                  rfwcdma_data_get_carrier_mask(device),
                                  channel[0],
                                  RFDEVICE_EXECUTE_IMMEDIATE,
                                  NULL);
#endif

  /* Get the global callback data here before calling Set channel */
  dyn_notch_filter_settings = rfwcdma_core_rxctl_get_dynamic_notch_cb_data(device);

  /*! Tune RX PLL */
  #ifdef FEATURE_RF_POWER_REPORTING
  /* associate handle with device */
  rflm_dm_handle_meta_s handle_meta;
  rflm_dm_get_handle_info(  rxlm_buf_idx , &handle_meta);
  handle_meta.devide_id = device;
  handle_meta.device_id = device;
  rflm_dm_set_handle_info( rxlm_buf_idx , &handle_meta );
  /* log channel number */
  rf_power_reporting_update_channel( device, channel[0] );
  #endif

  uint8 sub_id = rfwcdma_data_get_subscription_type(device);

  RF_MSG_2(RF_HIGH, "rfwcdma_mc_tune_to_multi_chan: device %d, sub_id %d", device, sub_id);
  
  /* Single Carrier */
  if (num_chan == RFCOM_SINGLE_CARRIER)
  {
    /* Set rx tune params */
    rx_params.band       = band;
    rx_params.bw         = RFDEV_WCDMA_BW_1X;
    rx_params.channel[0] = channel[0];

    /* Tune Rx PLL to SC mode */
    rfdevice_wcdma_rx_set_channel( device, RFCOM_WCDMA_MODE, rx_params.band,
                                   rx_params, RFDEVICE_EXECUTE_IMMEDIATE, 
                                   NULL, rfwcdma_core_rxctl_update_dynamic_notch_cb, 
                                   dyn_notch_filter_settings, TRUE, asdiv_alt_gain, (rfdevice_trx_sub_type)sub_id );

    RFGNSS_MC_V(wg_chan)(channel[0]);

    /* Update Notch filter settings */
    RFGNSS_MC_V(program_notch_filter)(RFGNSS_NOTCH_CFG_WCDMA);
  }
  /* Dual carrier */
  else if (num_chan == RFCOM_DUAL_CARRIER)
  {
    /* Set rx tune params */
    rx_params.band       = band;
    rx_params.bw         = RFDEV_WCDMA_BW_2X;
    rx_params.channel[0] = channel[0];
    rx_params.channel[1] = channel[1];

    /* Tune Rx PLL to DC mode */
    rfdevice_wcdma_rx_set_channel( device, RFCOM_WCDMA_MODE,
                                   rx_params.band, rx_params,
                                   RFDEVICE_EXECUTE_IMMEDIATE, 
                                   NULL, rfwcdma_core_rxctl_update_dynamic_notch_cb,
                                   dyn_notch_filter_settings, TRUE, asdiv_alt_gain, (rfdevice_trx_sub_type)sub_id );
  }
  /* 3-Carrier */
  else if (num_chan == RFCOM_3_CARRIER)
  {
    /* Set rx tune params */
    rx_params.band       = band;
    rx_params.bw         = RFDEV_WCDMA_BW_3X;
    rx_params.channel[0] = channel[0];
    rx_params.channel[1] = channel[1];
    rx_params.channel[2] = channel[2];

    /* Tune Rx PLL to 3-C mode */
    rfdevice_wcdma_rx_set_channel( device, RFCOM_WCDMA_MODE,
                                   rx_params.band, rx_params,
                                   RFDEVICE_EXECUTE_IMMEDIATE, 
                                   NULL, rfwcdma_core_rxctl_update_dynamic_notch_cb, 
                                   dyn_notch_filter_settings, TRUE, asdiv_alt_gain, (rfdevice_trx_sub_type)sub_id );
  }
  else
  {
    RF_MSG_1( RF_ERROR,
              " Invalid carrier specification in rfwcdma_mc_tune_to_multi_chan - %d",
              num_chan );
    return 0;
  }

  pll_wait_time = 0;
  pll_wait_time = timetick_get();

  /* Apply dynamic notches for DBDC during regular tune for SCELL */
  if ( (dyn_notch_filter_settings->apply_notch_immediate == FALSE) && (dyn_notch_filter_settings->current_cell_spur_count > 0) && (rfwcdma_data_get_carrier_mask(device) == 2) )
  {
    (void)rfwcdma_msm_update_rxlm_buffer(device, channel, multi_carrier_idx, rxlm_buf_idx, alt_path, enable_nbr_flag, dyn_notch_filter_settings, RFWCDMA_MSM_STATIC_PLUS_DYNAMIC_NOTCH_TUNE);
  }

  /* Applying static notches during regular tune */
  else
  {
  (void)rfwcdma_msm_update_rxlm_buffer(device, channel, multi_carrier_idx, rxlm_buf_idx, alt_path, enable_nbr_flag, dyn_notch_filter_settings, RFWCDMA_MSM_STATIC_NOTCH_TUNE);
  }

  if(enable_nbr_flag && (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE)) 
  {
    /* print dbg msg */
    RF_MSG_1( RF_HIGH,
              "W_NBR DBG: Loading up modG on RxLM index %d for PRx",
              nbr_rxlm_buf_idx );
    (void)rfwcdma_msm_update_nbr_rxlm_buffer( device,
                                              channel, 
                                              num_chan,
                                              nbr_rxlm_buf_idx,
                                              alt_path );
  }

  rfwcdma_core_rxctl_config_band(rxlm_buf_idx, 
                                 device, 
                                 band, 
                                 multi_carrier_idx, 
                                 RFLM_WCDMA_HOME_MEAS, 
                                 alt_path,
                                 MAX_RX_DEV_HANDLE_ITEM);

  rfwcdma_core_rxctl_config_chan(rxlm_buf_idx, 
                                 device, 
                                 band, 
                                 channel, 
                                 multi_carrier_idx, 
                                 RFLM_WCDMA_HOME_MEAS, 
                                 alt_path,
                                 MAX_RX_DEV_HANDLE_ITEM);

  /*
                  Following conditions need to be met to send start AGC command to avoid sending back to back AGC start command
                  1. FW RxAGC ON status flag need to be OFF for the given carrier and chain
                  2. RF internal RxAGC enable flag need to be FALSE for the given carrier and chain
                  For DC, both carrier AGC will be enbaled
  */

  /* Set the RxAGC enable params */
  memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));
  rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
  rxagc_enable_params.rxlm_buf_idx[device] = rxlm_buf_idx;
  rxagc_enable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);
  rxagc_enable_params.multi_carrier_idx = multi_carrier_idx;
  rxagc_enable_params.rf_warmup_en = FALSE;
  rxagc_enable_params.nbr_rxlm_buf_idx = (uint8)nbr_rxlm_buf_idx;

  pll_wait_time = timetick_get_elapsed(pll_wait_time, T_USEC);
  if(pll_wait_time < rfc_core_config.tune_time)
  {
    /*! Wait for PLL settle after device tune */
    if ((!IS_FTM_IN_TEST_MODE()) || (rfm_get_calibration_state() == FALSE))
    {
      /* Wait for PLL to settling before starting AGC */
      DALSYS_BusyWait(MIN(rfc_core_config.tune_time, (rfc_core_config.tune_time - pll_wait_time)));
    }
  }
   if( enable_nbr_flag &&(rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE))
   {
     /* print dbg msg */
     RF_MSG( RF_HIGH, "W_NBR DBG: Enabling RxAGC");
   
     rxagc_enable_params.nbr_en_flag = TRUE;
   
     /* Send enable AGC cmd to FW */
     rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
   }
   else
   {
     rxagc_enable_params.nbr_en_flag = FALSE;

     /* Send enable AGC cmd to FW */
     rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
   }

  /*! Update the channel information. */
  for (i = 0,c = 0; i < RFCOM_MAX_CARRIERS; i++)
  {
    if ((multi_carrier_idx & (1 << i)) != 0)
    {
      //rfwcdma_mc_state.car_path_state[path][i].curr_chan_rx = channel[c++];
	  rfwcdma_data_set_curr_chan(device,i,channel[c++]);

      MSG_4(MSG_SSID_RF, MSG_LEGACY_HIGH, "Tune to multi-Chan: Path:%d,Multi-Carrier:%d, i %d channel:%d ",
            path, multi_carrier_idx, i, rfwcdma_mc_state.car_path_state[path][i].curr_chan_rx);
      rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_RX);
	  
#if 0
      /* In case of SC<->DC config, L1 will not stop Tx and we are still in RxTx state after retune Rx*/
      if (rfwcdma_mc_state.car_path_state[path][i].rf_state != RFWCDMA_MC_STATE_RXTX)
      {
        /* Update the rf_state here */
        rfwcdma_mc_state.car_path_state[path][i].rf_state  = RFWCDMA_MC_STATE_RX;
      }
#endif	  
    }
  }

#if 0
  /* Reset MTPL to ACQ mode */
  if (device == RFM_DEVICE_0)
  {
    rfwcdma_core_txplim_reset_to_acq_mode();
  }
#endif
  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_7(RF_HIGH, "Tune to multi-Chan, band:%d,Path:%d, Alt path:%d,Multi-Carrier:%d, Channel1:%d, Channel2:%d Execution Time: %d",
          band, path, alt_path, num_chan, channel[0], channel[1], prof_cal_func);
  }
  else
  {
  //Need to update the F3 to print 3rd channel info
    RF_MSG_6(RF_HIGH, "Tune to multi-Chan, band:%d,Path:%d, Alt path:%d, Multi-Carrier:%d, Channel1:%d, Channel2:%d ",
        band, path, alt_path, num_chan, channel[0], channel[1]);
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
/*!
        This function returns a flag to indicate whether it is SC/DC/3C.

*/

rfcom_multi_carrier_hspa_id_type rfwcdma_mc_get_carr_state(rfcom_device_enum_type device)
{
  return (rfwcdma_data_get_num_carriers(device));
}


/*----------------------------------------------------------------------------*/
/*!
        @brief
        Reading NV for WCDMA.

        @details
        Retrieves all the NV from WCDMA mode. Reads common and mode specific RF
        configuration parameters from NV. This is called as part of the
        LOAD_RF_NV function case where we need to read the calibration NV and write
        them without having to reset.

*/
boolean rfwcdma_mc_reload_nv
(
  rfcom_device_enum_type device,
  rex_tcb_type *caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (*task_wait_func_ptr)(rex_sigs_type)
)
{
  rfcommon_nv_status_type nv_status = RFCOMMON_NV_READ_FAILED;
  boolean nv_init_success = FALSE;

  nv_status = rfwcdma_nv_retrive_items(caller_tcb_ptr, task_nv_wait_sig,
                                       task_wait_func_ptr);

  /* For certain factory verification sequence, NS testing is performed right after RF Cal and reload NV.
                  Updating temp comp NVs because essential in this sequence */
  rfwcdma_core_temp_comp_update(device);

  if (nv_status == RFCOMMON_NV_READ_SUCCESS)
  {
    /* Not missing critical NV items */
    nv_init_success = TRUE;
  }
  /* multi-lin data NV read is handled seperately*/
  rfwcdma_core_txlin_init();

  /* FBRX Cal data NV read is handled seperately */
  rfcommon_fbrx_nv_retrieve_wcdma_data( caller_tcb_ptr,
                                        task_nv_wait_sig,
                                        task_wait_func_ptr);

  /*! @todo Need to return actual nv_init_success status
  Returning TRUE is a temporary solution to avoid going to offline for other
  technologies which don't activate WCDMA critical NV items. */
  return nv_init_success;
}
/*----------------------------------------------------------------------------*/
/*!
        @brief
        Set PMIC quiet mode

        @details
*/
boolean rfwcdma_mc_set_pmic_quiet_mode(boolean en)
{
  const rfc_common_logical_device_params_type *cmn_logical_dev = NULL;

  /*Hard coding the logical device to RFM_DEVICE_0 as device information is not passed
          for WCDMA functions and WCDMA is always supported on RFM_DEVICE_0 only for now*/
  cmn_logical_dev = rfc_common_get_logical_device_params(RFM_DEVICE_0);

  if (cmn_logical_dev == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "RFWCDMA set PMIC quiet mode error: common device not initialized", 0);
    return FALSE;
  }

  rfc_set_quiet_mode(RFCOM_WCDMA_MODE, en);

  return (TRUE);
}


boolean rfwcdma_mc_set_lower_power_mode(rfm_device_enum_type device,rfcom_lpm_config_enum_type lpm_config,
                                        uint32 c0_rxlm_buf_idx,
                                        uint32 c1_rxlm_buf_idx,
                                        uint32 alt_path)
{
  boolean status = FALSE, rxd_enabled = FALSE;
  rfm_device_enum_type paired_dev;

  
  if((device < RFM_DEVICE_0) || (device >=RFM_MAX_WAN_DEVICES))
	 {
	   RF_MSG_1(RF_ERROR, "rfwcdma_mc_set_lower_power_mode: Invalid device %d", device);
	   return FALSE ;
	 }
  

  if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
     paired_dev = rfwcdma_data_get_associated_device_rx(device);

     if (paired_dev < RFM_MAX_WAN_DEVICES)
     {
      if ((rfwcdma_data_get_rf_state(paired_dev) == RFWCDMA_STATE_RX) && 
          (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX ))
       {
        rxd_enabled == TRUE;
       }
     }

  }
  else if( rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE)
  {
   if (rfwcdma_data_get_rf_state(device)== RFWCDMA_STATE_RX) 
   {
     rxd_enabled = TRUE;
   }
  }
   /* Low power/High power mode switch can only be invoked in single carrier mode and no RxD support for now */
  if ((rfwcdma_data_get_num_carriers(device) != RFCOM_SINGLE_CARRIER) || (rxd_enabled == TRUE))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfwcdma_set_low_power_mode, not in single carrier mode or"
           "RxD is enabled! current carrier mode:%d",rfwcdma_data_get_num_carriers(device));
    return FALSE;
  }
  /* Check state machine for primary chain*/
  if (rfwcdma_data_get_rf_state(device) != RFWCDMA_STATE_RX)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfwcdma_set_low_power_mode,PRx is not enabled! current state:%d, return...",
          rfwcdma_data_get_rf_state(device));
    return FALSE;
  }
  if ((lpm_config == RFCOM_LPM_MODEM) || (lpm_config == RFCOM_LPM_MODEM_RF))
  {
    /* Update RxLM */
    status = rfwcdma_msm_set_low_power_mode(TRUE, 
                                            device, 
                                            rfwcdma_data_get_curr_chan(device, 0),
                                            alt_path,
                                            c0_rxlm_buf_idx);
    /* Enable when we support RxD with DSR*/

  }
  else if (lpm_config == RFCOM_LPM_DISABLE)
  {
    /* Update RxLM */
    status = rfwcdma_msm_set_low_power_mode(FALSE, 
                                            device, 
                                            rfwcdma_data_get_curr_chan(device, 0), 
                                            alt_path,
                                            c0_rxlm_buf_idx);

  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Invalid LPM config:%d !", lpm_config);
    status = FALSE;
  }

  if (status == TRUE)
  {
    /* Send command to FW */
    rfwcdma_mdsp_send_lpm_agc_cmd(device,lpm_config, c0_rxlm_buf_idx, c1_rxlm_buf_idx);
    MSG_3(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfwcdma_set_low_power_mode sucess, lpm_config:%d,c0 rxlm buffer index:%d, c1 rxlm_buffer index:%d",
          lpm_config, c0_rxlm_buf_idx, c1_rxlm_buf_idx);
    return TRUE;
  }
  else
  {
    MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfwcdma_set_low_power_mode fail, en:%d,c0 rxlm buffer index:%d, c1 rxlm_buffer index:%d",
          lpm_config, c0_rxlm_buf_idx, c1_rxlm_buf_idx);

    return FALSE;
  }

}
/*----------------------------------------------------------------------------*/
/*!
        @brief
                This function deconfig the transmit chain for WCDMA uplink operation.

        @details
                It deconfig some settings made in init_tx, and make disable Tx thinner.
                This function should be called before WL1 send txlm_cfg(disable) to FW.

        @return
                1 on success and 0 on failure
*/
boolean rfwcdma_mc_decfg_tx(rfcom_device_enum_type device)
{
  boolean status = TRUE;
  xpt_cfg_type xptcfg = XPT_CFG_TYPE_MAX;
  rfmeas_event_infor_type *autopin_event;

  /* Wait for the temp comp done */
  rf_common_enter_critical_section(rfwcdma_core_temp_comp_get_lock_data());

  /* Disable ET path if it's ET supported */
  xptcfg = (xpt_cfg_type)rfwcdma_core_xpt_get_cfg_param\
           (rfwcdma_data_get_curr_band(device), XPT_CFG);
  if ((xptcfg == ET_CFG) || (xptcfg == EPT_ET_CFG))
  {
    status &= rf_common_xpt_init_dpd(device, NULL);
  }
  else if(xptcfg == EPT_CFG)
  {
    status &= rf_common_xpt_init_dpd(device, NULL);
  }
  
  if( rfcommon_fbrx_mc_is_ilpc_enabled(RFLM_TECH_WCDMA) )
  {
    rfcommon_fbrx_mc_deinit_tx(rfwcdma_data_get_rflm_buf_idx(device));
  }
  
  rfwcdma_core_temp_comp_enable(FALSE,device);

  rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_ENTER_MODE);
  /*------------- Exit mode is needed after decfg---------------*/

  rf_common_leave_critical_section(rfwcdma_core_temp_comp_get_lock_data());

  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, "rfwcdma_mc_decfg_tx status %d", status);
  return status;
}

/*----------------------------------------------------------------------------*/
/*!
        @details
                This function apply SAR limit passed from MCS

        @param device
        @param
*/
void rfwcdma_mc_set_multi_tx_sar_limit(rfcom_device_enum_type device, int16 sar_limit)
{
  rfwcdma_core_txplim_set_multi_tx_sar_limit(device, sar_limit);

  MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, "set_multi_tx_sar_limit status: device %d,limit %d",
        device, sar_limit);
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
                This creates an rf_buffer
*/
void  rfwcdma_mc_create_rf_buff(rf_buffer_intf **buffer_ptr)
{
  *buffer_ptr = rf_buffer_create(RFWCDMA_MAX_SSBI_SCRIPT_SIZE,
                                 RFWCDMA_MAX_RFFE_SCRIPT_SIZE,
                                 RFWCDMA_MAX_GRFC_SCRIPT_SIZE);

  /* Check rf_buff has been properly created. No point proceeding if error */
  if (*buffer_ptr == NULL)
  {
    RF_MSG_1(RF_ERROR, "Buffer creation failed %d", buffer_ptr);
    ASSERT(0);
  }

} /* rfwcdma_mc_create_rf_buff */

/*----------------------------------------------------------------------------*/
/*!
        @brief
                This destroy
*/
void rfwcdma_mc_destroy_rf_buff(rf_buffer_intf *buffer_ptr)
{
  (void)rf_buffer_destroy(buffer_ptr);
} /* rfwcdma_mc_destroy_rf_buff */




/*----------------------------------------------------------------------------*/
/*!
        @brief
        Get default band
        This will return the default band in rfcom_wcdma_band_type

        @details
        This will return the default band during set_model for both RF and WL1.
        default band is not necessarily band 1.

===========================================================================*/
rfcom_wcdma_band_type rfwcdma_mc_get_default_rfcom_band(void)
{
  rfcom_wcdma_band_type band = RFCOM_BAND_IMT;
  while (!rfwcdma_core_is_band_supported(band) && band < RFCOM_NUM_WCDMA_BANDS)
  {
    band++;
  }

  /* check if there is no wcdma band supported */
  if (band == RFCOM_NUM_WCDMA_BANDS)
  {
    /* Removing the ERR fatal to support those cards that doesn't support WCDMA.
            ERR_FATAL("No WCDMA band is supported in Band config NV",0,0,0);
            */
    band = RFCOM_BAND_INVALID;
  }

  return band;
}



/*----------------------------------------------------------------------------*/
/*!
        @brief
        Get default band and middle DL chan for WL1 to call in rfm layer

        @details
        This will return the default band and its middle DL chan, during set_model for WL1.
        default band is not necessarily band 1.


===========================================================================*/
void rfwcdma_mc_get_default_rfi_band_and_chan(rfi_band_type *band, uint16 *chan)
{
  /* call rfwcdma_core_convert_band_rfcom_to_rfi to get chan and convert rfcom_wcdma_band_type to rfi_band_type */
  rfcom_wcdma_band_type rfcom_band = rfwcdma_mc_get_default_rfcom_band();
  *band = rfwcdma_core_convert_band_rfcom_to_rfi(rfcom_band);

  /*Get middle DL chan*/
  *chan = rfwcdma_core_get_middle_DL_chan_from_band(rfcom_band);

}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        Get DBDC band combinations supported

        @details
        This will have to return the number of DBDC combinations supported on the RFC and those band combos.
        For RFCs which does not support any DBDC bands , by deafult it will be zero.

        @param num_band_comb_supported
        This will return the total number of DBDC band combinations supported by the RFC

        @param band_comb_list
        This would give the DB-DC band combinations as to which band is supported on which carrier

        @return
                Flag indicating the validity of the DBDC band combinations

*/

boolean rfwcdma_mc_get_dbdc_band_support(uint16 *num_band_comb_supported, const rfm_wcdma_dual_band_type **band_comb_list)
{
  boolean status = TRUE;
  rfc_wcdma_properties_type *band_list_ptr = NULL;
  uint16 num = 0;

  band_list_ptr = (rfc_wcdma_properties_type *)rfc_wcdma_get_dbdc_band_support();

  if ((band_list_ptr == NULL) || (band_comb_list == NULL))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "DBDC: Null pointer returned for DBDC band support", 0);
    status = FALSE;
    return (status);
  }

  *band_comb_list = band_list_ptr->dbdc_band_combo;

  while (((uint32)(band_list_ptr->dbdc_band_combo[num].prim_band)) != ((uint32)SYS_BAND_CLASS_NONE))
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_HIGH, " DBDC: Primary band : %d Secondary car band : %d", band_list_ptr->dbdc_band_combo[num].prim_band, band_list_ptr->dbdc_band_combo[num].sec_band);

    num++;
  }

  *num_band_comb_supported = num;

  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH, " DBDC: Number of DBDC combinations supported : %d", *num_band_comb_supported);

  return (status);

}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        This function gives RFWCDMA MC state info plus Path and Number of Carriers supported by the RF Device

        @details
        This function gives rfwcdma_mc_state.car_path_state info and Number of Carriers supported by the RF Device

        @param device Devices supported by RF driver

        @param car_path_state[] Array giving RFWCDMA MC state info for all the supported carriers

        @return
        valid_num_of_carrier Number of Carriers supported by Device

*/
uint8 rfwcdma_mc_get_car_state_info_from_device(rfcom_device_enum_type device, rfwcdma_mc_car_path_state_type car_path_state[])
{
  uint8 i;
  uint8 valid_num_of_carrier = 0;
  rf_path_enum_type path;

  path = rfcommon_core_device_to_path(device);

  if (path == RF_PATH_MAX)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Invalid path for device: %d", device);
    return valid_num_of_carrier;
  }

  for (i = 0; i < RFCOM_MAX_CARRIERS; i++)
  {
    if (device == rfwcdma_mc_state.car_path_state[path][i].rx_rfm_device)
    {
      memscpy(&car_path_state[valid_num_of_carrier], sizeof(rfwcdma_mc_car_path_state_type), &rfwcdma_mc_state.car_path_state[path][i], sizeof(rfwcdma_mc_car_path_state_type));
      valid_num_of_carrier++;
    }
  }
  return valid_num_of_carrier;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
        Returns MC state for a given path

        @details
                        This indicates WCDMA RF state machince

        @return
        Returns MC state for a given path
*/

rfwcdma_mc_state_enum_type rfwcdma_mc_get_path_state(rf_path_enum_type path)
{
  rfcom_device_enum_type device;
  device= rfcommon_core_path_to_device(path );
  uint8 first_valid_carrier = 0;
  if(device ==RFM_INVALID_DEVICE || device >= RFM_MAX_WAN_DEVICES )
  {
	 RF_MSG_1(RF_ERROR, "rfwcdma_mc_get_path_state: Invalid device %d", device);
     return 0;
  }

  first_valid_carrier = (rfcom_multi_carrier_hspa_id_type)rf_extract_first_one_bit_lsb_location((uint64)rfwcdma_mc_state.rx_multi_carrier_idx[device]);
  if(first_valid_carrier >= RFCOM_SINGLE_CARRIER && first_valid_carrier < RFCOM_MAX_CARRIERS)
  {
     return (rfwcdma_mc_state.car_path_state[path][first_valid_carrier].rf_state);
  }
  else
  {
     RF_MSG_1(RF_ERROR, "rfwcdma_mc_get_path_state: Invalid multi carrier %d", first_valid_carrier);
  	 return 0;
  }
  	
}


/*----------------------------------------------------------------------------*/
/*!
        @brief
        Returns Sampling rate for a given Rx LM

        @details
        Used for xPT IQ Capture ET Path delay

        @param
        Rx LM Buffer Index for which to return sampling rate

        @return
        Returns Sampling rate in Hz
*/

uint32 rfwcdma_mc_get_samp_rate(uint8 rx_lm_buf_idx)
{
  return (rfwcdma_msm_get_samp_rate(rx_lm_buf_idx));
}
/*----------------------------------------------------------------------------*/
/*!
        @brief
        Returns MC state for a given device

        @details
                        This indicates WCDMA RF state machince

        @return
        Returns MC state for a given device
*/

rfwcdma_mc_state_enum_type rfwcdma_mc_get_path_state_of_device(rfcom_device_enum_type device)
{
  rf_path_enum_type path;
  path = rfcommon_core_device_to_path(device);
  return (rfwcdma_mc_state.car_path_state[path][0].rf_state);
}


/*
        @brief
This API will return the synth status for a given Tech.Path and synthesizer

        @details

        @param
        device : RF device
        rf_mode : RF mode
        synth_type: where TX PLL is rqequired or RX PLL
        is_synth_locked: PLL status.
*/

void rfwcdma_mc_get_synth_lock_status
(
  rfcom_device_enum_type device,
  rfcom_mode_enum_type rf_mode,
  rfcom_synth_type synth_type,
  void *is_synth_locked
)
{

  if((device < RFM_DEVICE_0) || (device >=RFM_MAX_WAN_DEVICES))
	 {
	   RF_MSG_1(RF_ERROR, "rfwcdma_mc_get_synth_lock_status: Invalid device %d", device);
	   return ;
	 }



  if (synth_type == RF_SYNTH_RX)
  {
    if ((rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE) ||
        (rfwcdma_data_get_device_type(device)== RFM_WCDMA_DIVERSITY_RX_DEVICE))
    {
      rfdevice_wcdma_rx_pll_lock_detect(device,
                                        rfwcdma_data_get_curr_band(device),
                                        is_synth_locked,
                                        RFDEVICE_EXECUTE_IMMEDIATE,
                                        NULL
                                       );
    }
    else
     RF_MSG_2(RF_ERROR,"rfwcdma_mc_get_synth_lock_status: Device %d passed with RF_SYNTH_RX is of type %d",
                        device, rfwcdma_data_get_device_type(device));
  }
  
  else if (synth_type == RF_SYNTH_TX)
  {
    if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_TX_DEVICE) 
    {
      rfdevice_wcdma_tx_pll_lock_detect(device,
                                   rfwcdma_data_get_curr_band(device),
                                   is_synth_locked,
                                   RFDEVICE_EXECUTE_IMMEDIATE,
                                   NULL
                                  );
    }
    else
     RF_MSG_2(RF_ERROR,"rfwcdma_mc_get_synth_lock_status: Device %d passed with RF_SYNTH_TX is of type %d",
                        device, rfwcdma_data_get_device_type(device));
  }
  else
  {
    RF_MSG_1(RF_ERROR, "Invalid SYNTH query(%d) for SYNTH in for rfm_get_synth_lock_status",
             synth_type);
  }
}


/*
        @brief
This API will return if any of the WCDMA bands are supported

        @details

        @param

*/


boolean rfwcdma_mc_is_wcdma_supported(void)
{

  rfcom_wcdma_band_type wcdma_band = RFCOM_BAND_IMT;
  boolean is_any_of_the_bands_supported = FALSE;

  for (wcdma_band = RFCOM_BAND_IMT; wcdma_band < RFCOM_NUM_WCDMA_BANDS; wcdma_band++)
  {
    /*Check if any of the bands is supported.  */
    is_any_of_the_bands_supported |= rfwcdma_core_is_band_supported(wcdma_band);
  }

  return (is_any_of_the_bands_supported);

}



#ifdef FEATURE_RF_HAS_QTUNER
/*============================================================================

FUNCTION
rfwcdma_mc_program_qfe_scenario

DESCRIPTION
This API will be called to program the tuner immediately and no script or FW
        action is required. Inside this API check all the active paths then give a call to
        tuner manager associated with appropriate tuners

DEPENDENCIES
        None

RETURN VALUE
        Flag indicating if the api operation was successful (TRUE) or failed (FALSE)

SIDE EFFECTS
        None
=============================================================================*/

boolean rfwcdma_mc_program_qfe_scenario(void)
{
  boolean status;

  status = rfwcdma_core_antenna_tuner_program_qfe_scenario();

  if (status)
  {
    RF_MSG_1(RF_HIGH, "ANT_TUNER: rfwcdma_mc_program_qfe_scenario done !", 0);
  }

  return status;
}
#endif /* FEATURE_RF_HAS_QTUNER */
/*----------------------------------------------------------------------------*/
/*!
        @brief
        Get the W2W neighbor physical device index to support DBDC

        @details
        This function will return the physical device index that map to the device index in the LNA/LPM FW interface
        which is tuned for W2W measurement home/neighbor
        Example
        DBDC Primary band: B1 -> LNA/LPM device 0
        DBDC Secondary band: B8 -> LNA/LPM device 1
        Neighbor Band: B5 -> B5 will be tune on the same physical device as B8, return index 1
        Neighbor Band: B2 -> B2 will be tune on the same physical device as B1, return index 0


        @param chan
        neighbor cell ARFCN

        @param device
        neighbor cell RFM device

        @return
                physical device index for the home/neighbor pair
                return 0xFF if no valid device index can be found

*/
uint8 rfwcdma_mc_get_nbr_phy_dev_ind(uint16 chan, rfm_device_enum_type device)
{
  uint8 carrier_idx, dev_idx = 0;
  rfdevice_trx_wcdma_rx * *home_wcdma_device_ptr,**nbr_wcdma_device_ptr = NULL;
  rfdevice_rxtx_common_class *home_device_instance_ptr, *nbr_device_instance_ptr = NULL;
  rfwcdma_mc_car_path_state_type *temp_state_ptr = NULL;
  rfcom_wcdma_band_type nbr_band = rfwcdma_core_convert_chan_to_band(chan);

  RF_MSG_2(RF_HIGH, " rfwcdma_mc_get_nbr_phy_dev_ind for arfcn:%d, device:%d", chan, device);

  if (rfwcdma_mc_state.rx_multi_carrier_idx[RFM_DEVICE_2] == 0)
  {
    /* If DBDC is not enabled, neighbor device index is always 0 */
    RF_MSG_1(RF_HIGH, " Secondary band carrier not enabled, neighbor device index is 0", 0);
    return 0;
  }


  /* Get the device instance of neighbor */
  nbr_wcdma_device_ptr = (rfdevice_trx_wcdma_rx **)rfc_wcdma_get_device(device,
                         RFC_CONFIG_RX,
                         nbr_band,
                         RFDEVICE_TRANSCEIVER);

  /* Null pointer check */
  if ((nbr_wcdma_device_ptr != NULL) && (nbr_wcdma_device_ptr[0] != NULL))
  {
    /* Get critical section lock from the device instance */
    nbr_device_instance_ptr =
      rfdevice_wcdma_rx_get_common_device_pointer(nbr_wcdma_device_ptr[0]);
  }
  else
  {
    RF_MSG_1(RF_ERROR, "failed to get valid device instance for the neighbor", 0);
    return 0xFF;
  }


  for (carrier_idx = 0; carrier_idx < RFCOM_MAX_CARRIERS; carrier_idx++)
  {
    temp_state_ptr = &rfwcdma_mc_state.car_path_state[RF_PATH_0][carrier_idx];

    /* Check if the given carrier is in the right state */
    if ((temp_state_ptr->rf_state == RFWCDMA_MC_STATE_RX) || (temp_state_ptr->rf_state == RFWCDMA_MC_STATE_RXTX))
    {
      /* Get the device instance of home */
      home_wcdma_device_ptr = (rfdevice_trx_wcdma_rx **)rfc_wcdma_get_device(temp_state_ptr->rx_rfm_device,
                              RFC_CONFIG_RX,
                              temp_state_ptr->curr_band_rx,
                              RFDEVICE_TRANSCEIVER);
      /* Null pointer check */
      if ((home_wcdma_device_ptr != NULL) && (home_wcdma_device_ptr[0] != NULL))
      {
        /* Get critical section lock from the device instance */
        home_device_instance_ptr =
          rfdevice_wcdma_rx_get_common_device_pointer(home_wcdma_device_ptr[0]);

        if ((rfdevice_compare_common_rxtx_devices(home_device_instance_ptr, nbr_device_instance_ptr))) //*(home_device_instance_ptr) == *(nbr_device_instance_ptr))
        {
          /* Matching device instance for home and neighbor found */
          dev_idx = (uint8)rfwcdma_core_util_device_to_lna_id(temp_state_ptr->rx_rfm_device, RFLM_WCDMA_HOME_MEAS,1);
          RF_MSG_2(RF_HIGH, "physical device index:%d returned for rfm device:%d", dev_idx, temp_state_ptr->rx_rfm_device);
          return dev_idx;
        }
      }
    }
  }

  RF_MSG_1(RF_ERROR, " w2w failed to found the matching device index from the state machine!", 0);
  return 0xFF;
}

/*----------------------------------------------------------------------------*/
/*!
        This function sets the wcdma use fb path flag

        @params - val
*/
void rfwcdma_mc_set_use_fb_path_flag
(
  boolean val
)
{
  rfwcdma_ftm_use_fb_path = val;
}

/*----------------------------------------------------------------------------*/
/*!
This function returns the wcdma use fb path flag

*/
boolean rfwcdma_mc_get_use_fb_path_flag
(
  void
)
{
  return (rfwcdma_ftm_use_fb_path);
}
/*----------------------------------------------------------------------------*/
/*!
        @brief
                Get the Rx AGC value for the specified carrier.

        @details
                This function will return the RxAGC value from FW for the specified carrier ID and in desired TYPE and UNIT.
                We directly query the MDSP layer and get the value.

        @param device
                RFM device

        @param Pointer to data
                This would contain the Carrier ID , unit and type of the value

        @return rxagc_val
                RxAGC value for the specified carrier ID in desired UNIT and TYPE
        */

int16 rfwcdma_mc_get_rxagc
(
  rfcom_device_enum_type device,
  rfwcdma_mc_rxagc_type *data
)
{
  int16 rxagc_val;
  rf_path_enum_type path;
  rfagc_receive_chain_type rx_chain;
  rf_path_enum_type primary_path;

  /*! Convert rfcom_device_enum_type to rf_path_enum_type */
  path = rfcommon_core_device_to_path(device);

  rx_chain = rfcommon_core_path_to_receive_chain(path);

  /* Get the RxAGC value from MDSP */
  (void)rfwcdma_core_util_get_rxagc(device,data->type, data->carr_id,data->unit, &rxagc_val);

  return rxagc_val;
}

/*----------------------------------------------------------------------------*/
/*!
        @brief
                Get the Rx AGC value for the specified carrier.

        @details
                This function will return the RxAGC value from FW for the specified carrier ID and in desired TYPE and UNIT.
                We directly query the MDSP layer and get the value.

        @param device
                RFM device

        @param Pointer to data
                This would contain the Carrier ID , unit and type of the value
 
        @param band
                rfcom_wcdma_band_type band

        @return rxagc_val
                RxAGC value for the specified carrier ID in desired UNIT and TYPE
        */

boolean rfwcdma_mc_get_rxagc_info
(
  rfwcdma_mc_rxagc_input_params *rxagc_input_info,
  rfwcdma_mc_rxagc_output_params *rxagc_output_info
)
{
  /*For Diveristy only mode, Primaty path is PATH 1*/
  if(rfwcdma_data_get_prx_path()==RF_PATH_1)
  {
    rxagc_input_info->ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
  }
  
  /* Get the RxAGC value from MDSP */
  if(TRUE == rfwcdma_core_util_get_rxagc_info(rxagc_input_info, rxagc_output_info))
  {
    return TRUE;
  }

  return FALSE;
}
/*============================================================================*/
/*!
  @name WCDMA Multi carrier

  @brief
  This section includes interfaces pertaining to Multi carrier WCDMA
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get the 3C RF status

  @details
  This function will get the status of RF 3C tune

  @param device
   RFM Device to be used

  @return wcdma_3c_active
   Status of the 3C tune per RF path

*/
boolean
rfwcdma_get_tri_carrier_status
(
  rfcom_device_enum_type device
)
{
  rf_path_enum_type path;

  path = rfcommon_core_device_to_path(device);

  return(wcdma_3c_active[path]);

} /* rfwcdma_get_tri_carrier_status */

/*! @} */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get RF MC state
  @details
  This function will return a copy of the MC state structure
  @param

  @return rfwcdma_mc_state_copy
  Current MC state variable
*/

rfwcdma_mc_state_type* rfwcdma_mc_get_state()
{
  rfwcdma_mc_state_type *rfwcdma_mc_state_copy =NULL;

  rfwcdma_mc_state_copy = &rfwcdma_mc_state;

  return rfwcdma_mc_state_copy;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Map device as primary rx or diversity rx device
  @details
  This function will find out if the passed device is primary rx or diversity rx 
  device and update th mc state machine accordingly 

  @param device
  Current rfm_device enum type device on which enter mode is called 
  @return 
  
*/
boolean rfwcdma_mc_map_rx_device(rfm_device_enum_type rx_device)
{
  rf_path_enum_type path =0; 
  rfcom_multi_carrier_hspa_id_type carrier =0; 
  boolean is_primary = TRUE; 
  uint8 enabled_devices =0; 
  boolean map_status = TRUE; 



  for (path  =RF_PATH_0; path < RF_PATH_MAX; path ++)
  {
    for (carrier = RFCOM_SINGLE_CARRIER; carrier < RFCOM_MAX_CARRIERS; carrier ++ )
    {
      if (rfwcdma_mc_state.car_path_state[path][carrier].rf_state == RFWCDMA_MC_STATE_POWERUP_INIT)
      {
        is_primary &= TRUE; 
      }
      else 
      {
        is_primary &= FALSE;
        enabled_devices += 1;
      }
    }
  }

  if (last_device == rx_device)
  {
    RF_MSG_1(RF_HIGH,"rfwcdma_mc_map_rx_device: device %d already mapped ", rx_device );
    map_status = FALSE; 
  }

  else
  {
      if (is_primary == TRUE && enabled_devices == 0)
      {
        rfwcdma_mc_state.primary_rx_device = rx_device;

        if (rfwcdma_mc_state.primary_rx_device == RFM_DEVICE_0)
        {
          rfwcdma_mc_state.secondary_rx_device = RFM_DEVICE_1; 
        }
        else if (rfwcdma_mc_state.primary_rx_device == RFM_DEVICE_2)
        {
          rfwcdma_mc_state.secondary_rx_device = RFM_DEVICE_3;
        }

        RF_MSG_2(RF_HIGH, "rfwcdma_mc_map_rx_device: Identified PRx device: %d, DRx device %d", rx_device,rfwcdma_mc_state.secondary_rx_device ); 
      }
      else 
      {
        RF_MSG_2(RF_HIGH, "rfwcdma_mc_map_rx_device: %d devices enabled, passed device %d not primary or secondary ",enabled_devices, rx_device); 
      }
  }
  
  last_device = rx_device; 
  return map_status; 
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Validate the RF WCDMA State

  @details
  This function compares the WCDMA internal variables against the expected state
  to indicate the calling RFM function if it is safe to proceed with execution
  of the RFM functionality.

  The return value of this API is not a boolean, as this API can provide more
  info on the cause of the failure (invalid mode, state, etc).

  @param device
  The RF device on which the validation is performed.

  @param rfa_api
  Enum indicating the aPI to be validated

  @return
  Status of RF state. Negative value refers to bad state.
*/
rfm_api_status_t
rfwcdma_mc_validate_state
(
  rfwcdma_device_mask_type device_mask,
  rfm_wcdma_api_type rf_api
)
{
  rfm_mode_enum_type curr_mode;
  rfm_api_status_t return_val = RFM_WCDMA_HEALTHY_STATE;
  rf_path_enum_type path =0;
  uint32 device_mask_copy = 0;
  rfm_device_enum_type device =0;
  boolean device_state_valid = TRUE;
  rfcom_multi_carrier_hspa_id_type carrier_index =0;
  device_mask_copy = device_mask;

  while (device_mask != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

    if((device < RFM_DEVICE_0) || (device >= RFM_MAX_WAN_DEVICES))
    {
      RF_MSG_1(RF_HIGH, "rfwcdma_mc_validate_state: Invalid device %d", device);
      return RFM_WCDMA_ERROR_FAILURE;
    }
    /* Get Current Mode */
    curr_mode =  rfm_get_current_mode(device);

    /* Validate the current Mode */
    if ( curr_mode != RFM_IMT_MODE )
    {
      RF_MSG_2( RF_ERROR, "rfwcdma_mc_validate_state: Device %d - Not in IMT mode, current mode %d ",
                device, curr_mode );
      return RFM_WCDMA_ERROR_INVALID_MODE;
    } /* if ( curr_mode != RFM_IMT_MODE ) */

    switch (rf_api) 
    {
    case RFM_WCDMA_ENTER_MODE:
      if (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_INIT) 
      {
        return_val = RFM_WCDMA_HEALTHY_STATE;
      }
      else
      {
       device_state_valid = FALSE;
       return_val = RFM_WCDMA_ERROR_BAD_STATE;
      }
      break;

    case RFM_WCDMA_PREP_TUNE:
     if ((rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_ENTER_MODE)|| 
         (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_SLEEP)||
         (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX))
     {
       return_val = RFM_WCDMA_HEALTHY_STATE;
     }
     else
     {
       device_state_valid = FALSE;
       return_val = RFM_WCDMA_ERROR_BAD_STATE;
     }
     break;

     case RFM_WCDMA_EXEC_TUNE:
     if ((rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_PREP)|| 
         (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_PREP_WAKEUP))
     {
       return_val = RFM_WCDMA_HEALTHY_STATE;
     }
     else
     {
       device_state_valid = FALSE;
       return_val = RFM_WCDMA_ERROR_BAD_STATE;
     }
     break;

     case RFM_WCDMA_PREP_WAKEUP:
     if (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_SLEEP)
     {
       return_val = RFM_WCDMA_HEALTHY_STATE;
     }
     else
     {
       device_state_valid = FALSE;
       return_val = RFM_WCDMA_ERROR_BAD_STATE;
     }
     break;

     case RFM_WCDMA_EXEC_WAKEUP:
     if ((rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_PREP_WAKEUP)|| 
         (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_PREP)|| 
         (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX)||
         (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_SLEEP))
     {
       return_val = RFM_WCDMA_HEALTHY_STATE;
     }
     else
    {
      device_state_valid = FALSE;
      return_val = RFM_WCDMA_ERROR_BAD_STATE;
     }
     break;

     case RFM_WCDMA_SLEEP:
     if ((rfwcdma_data_get_rf_state(device)!= RFWCDMA_STATE_ENTER_MODE)&& 
         (rfwcdma_data_get_rf_state(device) != RFWCDMA_STATE_RX_SLEEP)&&
         (rfwcdma_data_get_rf_state(device) != RFWCDMA_STATE_INIT))
     {
       return_val = RFM_WCDMA_HEALTHY_STATE;
     }
    else
    {
       device_state_valid = FALSE;
       return_val = RFM_WCDMA_ERROR_BAD_STATE;
     }
     break;

     case RFM_WCDMA_EXIT:
     if ((rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_RX_SLEEP) ||
          (rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_ENTER_MODE))
     {
      return_val = RFM_WCDMA_HEALTHY_STATE;
     }
     else
     {
       device_state_valid = FALSE;
       return_val = RFM_WCDMA_ERROR_BAD_STATE;
     }
     break;

    default:
     RF_MSG_2(RF_ERROR, "rfwcdma_mc_validate_state:Invalid api name %d on dev_mask %d", rf_api, device_mask_copy);
     return_val = RFM_WCDMA_ERROR_BAD_STATE;

    }
    /* if!( rfwcdma_mc_state.car_path_state[path][carrier_index].rf_state != exp_state ) */

    if (return_val == RFM_WCDMA_HEALTHY_STATE)
    {
      device_state_valid &= TRUE;
    }

    device_mask &= (~rf_extract_first_one_bit_mask(device_mask));
  }

  RF_MSG_3(RF_HIGH, "rfwcdma_mc_validate_state: [Status %d] on dev mask %d for API %d",return_val, 
                                                                               device_mask_copy, rf_api);
return return_val;

} /* rfwcdma_mc_validate_state */

/*! @endcond */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Create device scripts for ASM, GRFC and WTR

  @details
  based on the device information passed this routine would create scripts for ASM/
  Tuners/ GRFC and WTR. These routine would be called for FR2001 scenario.

  @param device
  Device for which device scripts need to be created

  @param band
  WCDMA operational band on the device

  @param channel_list
  List of channels for the tune

  @param rx_device_buffer
  RF buffer array passed my the caller for populating the device scripts.

  @param num_chan
  Parameter defining the number of channels for the tune.

  @param source_band
  Parameter to indicate the current band tuned

  @param disable_ind
  Parameter defining the ASM disable operation type

 */
void rfwcdma_mc_create_device_scripts
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  uint16 channel_list[4],
  rf_buffer_intf *rx_device_buffer[],
  rfcom_multi_carrier_hspa_id_type carr_id,
  rfcom_wcdma_band_type source_band,
  rfwcdma_core_rxctl_dis_first_ind_type disable_ind,
  rfdevice_wcdma_spur_param_type* dyn_notch_filter_settings
)
{
  rfdevice_wcdma_lna_param_type lna_param = {0};
  
  if((device < RFM_DEVICE_0) || (device >=RFM_MAX_WAN_DEVICES))
	 {
	   RF_MSG_1(RF_ERROR, "rfwcdma_mc_create_device_scripts: Invalid device, returning from here %d", device);
	   return;
	 }

  uint32 path =0;
  rfdevice_wcdma_rx_tune_param_type rx_params = {0};
  rfwcdma_mc_wakeup_scripts_enum_type lna_script_type = RFWCDMA_MC_ASM_TUNER_SCRIPTS; /*default script type is RFWCDMA_MC_ASM_TUNER_SCRIPTS*/
  const rf_hal_bus_resource_script_type* script_ptr = NULL;
  
  /* Creating a dummy buffer to populate scripts and query LNA type (GRFC/RFFE) */
  rf_buffer_intf *lna_query_buffer = NULL;
  rfm_device_enum_type paired_dev;
  boolean asdiv_alt_gain = FALSE;
  
  lna_param.band = band;
  lna_param.device = device;
  lna_param.carrier = carr_id;
  
#ifdef FEATURE_RF_ASDIV 
  rfwcdma_asdiv_asm_script_data xsw_script_data;
  rfc_cfg_params_type asdiv_cfg; 
  rfc_device_info_type *device_info_ptr; 
  /* Populate params */
  xsw_script_data.device = device;
  xsw_script_data.band= band;
  xsw_script_data.immediate=FALSE;
  
  if(rfcommon_asdiv_get_current_position(device)==ASDIV_POSITION_1)
  {
    asdiv_alt_gain = TRUE;
  }
  else
  {
    asdiv_alt_gain = FALSE;
  }

  rfcommon_asdiv_position_type asdiv_position = 0;
#endif

  if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
    rx_params.rx_mode = RFDEVICE_PRX_OPER_MODE;
    lna_param.rx_mode = RFDEVICE_PRX_OPER_MODE;
  }
  else if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_DIVERSITY_RX_DEVICE) 
  {
    rx_params.rx_mode = RFDEVICE_DRX_OPER_MODE;
    lna_param.rx_mode = RFDEVICE_DRX_OPER_MODE;
  }
  else
  {
    rx_params.rx_mode = RFDEVICE_INVALID_OPER_MODE;
    lna_param.rx_mode = RFDEVICE_INVALID_OPER_MODE;
  }

  paired_dev = rfwcdma_data_get_associated_device_rx(device);

  rx_params.paired_device = paired_dev;
  lna_param.paired_device = paired_dev;

  /* Perform Device to Path mapping */
  path = rfcommon_core_device_to_path(device);

  rx_params.band = band;

  if (carr_id == 0)
  {
    rx_params.bw   = RFDEV_WCDMA_BW_INVALID;
    rx_params.channel[0] = channel_list[0];
  }
  else if (carr_id == 1)
  {
    rx_params.bw   = RFDEV_WCDMA_BW_2X;
    rx_params.channel[0] = channel_list[0];
    rx_params.channel[1] = channel_list[1];

  }
  else if (carr_id ==2 )
  {
    rx_params.bw   = RFDEV_WCDMA_BW_3X;
    rx_params.channel[0] = channel_list[0];
    rx_params.channel[1] = channel_list[1];
    rx_params.channel[2] = channel_list[2];

  }

  RF_MSG_4(RF_HIGH, "rfwcdma_mc_create_device_scripts: Parameters - device: %d path: %d, band: %d,"
           "Channel 0 %d",device,path, band, rx_params.channel[0]);


  /* Path validation */
  if ( path > RF_PATH_1 )
  {
    RF_MSG_2( RF_ERROR, "rfwcdma_mc_create_device_scripts: Device %d, "
              "Invalid Path %d mapping",
              device,
              path );
    return;
  } /* if ( path > RF_PATH_1 ) */
   /* Create dummy RF buffer*/  
    rfwcdma_mc_create_rf_buff(&lna_query_buffer);

    if (lna_query_buffer == NULL)
    {
      RF_MSG(RF_ERROR,"rfwcdma_mc_create_device_scripts: rf buffer allocation for lna_query_buffer failed!");
      /*Destroy local RF dummy buffers*/  
      rfwcdma_mc_destroy_rf_buff(lna_query_buffer);
	  return;
    }  

    /* Clear MC buffer */  
    rf_buffer_clear(lna_query_buffer);  

    /*Create dummy sleep script and query for resource buffer*/  
    rfdevice_wcdma_lna_sleep(device,
                             source_band,
                             lna_query_buffer,
                             RFDEVICE_CREATE_SCRIPT,
                             RFCOM_INVALID_TIMING_OFFSET);  

    /* Query the script for GRFC resource type; default script type is RFWCDMA_MC_ASM_TUNER_SCRIPTS*/
	script_ptr = rf_buffer_get_script_ptr(lna_query_buffer, RF_HAL_BUS_RESOURCE_GRFC);
	
	if(script_ptr != NULL && script_ptr->num_trans > 0)
	{
	   lna_script_type =  RFWCDMA_MC_GRFC_SCRIPTS;
	   RF_MSG_2(RF_HIGH, "rfwcdma_mc_create_device_scripts: eLNA is GRFC type lna_script_type = %d, num_trans = %d",lna_script_type,script_ptr->num_trans);
	}

  /* If a different band is tuned on the device that was tuned before, then disable WTR on the previous band
  for that device */
  if ( (source_band != RFCOM_BAND_INVALID) && (source_band != band) ) 
  {
    uint8 sub_id = rfwcdma_data_get_subscription_type(device);

    RF_MSG_2(RF_HIGH, "Calling rfdevice_wcdma_rx_disable: device %d, sub_id %d", device, sub_id);
	
    rfdevice_wcdma_rx_disable(device,
                              source_band,
                              RFDEVICE_CREATE_SCRIPT,
                              rx_device_buffer[RFWCDMA_MC_WTR_SCRIPTS], paired_dev, rx_params.rx_mode, (rfdevice_trx_sub_type)sub_id );

/* Put the eLNA to low power mode */
    rfdevice_wcdma_lna_sleep(device,
                             source_band,
                             rx_device_buffer[lna_script_type],
                             RFDEVICE_CREATE_SCRIPT,
                             RFCOM_INVALID_TIMING_OFFSET);
  }

  /* Configure ASM Rx */
  rfwcdma_core_rxctl_disable_sleep_asm(device,
                                       source_band,
                                       rx_device_buffer[RFWCDMA_MC_ASM_TUNER_SCRIPTS],
                                       RFDEVICE_CREATE_SCRIPT,
                                       RFCOM_INVALID_TIMING_OFFSET,
                                       disable_ind);

  rfwcdma_core_rxctl_enable_asm_grfc(device,
                                band,
                                rx_device_buffer[RFWCDMA_MC_ASM_TUNER_SCRIPTS],
                                rx_device_buffer[RFWCDMA_MC_GRFC_SCRIPTS],
                                RFDEVICE_CREATE_SCRIPT,
                                RFCOM_INVALID_TIMING_OFFSET);

  /* Build GRFC script */
  rfwcdma_core_rxctl_generate_grfc_script( device, 
                                           band,
                                           rx_device_buffer[RFWCDMA_MC_GRFC_SCRIPTS] );
  #ifdef FEATURE_RF_ASDIV 

  asdiv_position = rfcommon_asdiv_get_current_position(xsw_script_data.device);


  /*Check if the RFC has support for AsDiv before building scripts*/ 
  asdiv_cfg.alternate_path = rfwcdma_data_get_alt_path(device); 
  asdiv_cfg.band = band; 
  asdiv_cfg.logical_device = device;   
  asdiv_cfg.req = RFC_REQ_ENABLE_ANT_SWAP_1; 
   
  if(rfc_wcdma_data_get_device_info(&asdiv_cfg,&device_info_ptr)==TRUE) 
  { 
      
    RF_MSG_2(RF_HIGH, "rfwcdma_mc_create_device_scripts: RF card supports AsDiv device: %d, band: %d",device,band);
    /* First build GRFC switch script from RFC (if any) */
    (void)rfwcdma_asdiv_create_grfc_script(asdiv_position,rx_device_buffer[RFWCDMA_MC_GRFC_SCRIPTS],(void*)&(xsw_script_data));
  
  
    /* Second build device switch script from xsw device, which can be either RFFE or GRFC.  */
    if (rfwcdma_core_asdiv_get_script_type(xsw_script_data) == RF_HAL_BUS_RESOURCE_GRFC)
    {  
    RF_MSG_1( RF_HIGH, "Asdiv script type:%d ", RF_HAL_BUS_RESOURCE_GRFC);
      (void)rfc_wcdma_get_asdiv_xsw_script(asdiv_position,rx_device_buffer[RFWCDMA_MC_GRFC_SCRIPTS],(void*)&(xsw_script_data));
    }
    else if (rfwcdma_core_asdiv_get_script_type(xsw_script_data) == RF_HAL_BUS_RESOURCE_RFFE)
    {
    RF_MSG_1( RF_HIGH, "Asdiv script type:%d ", RF_HAL_BUS_RESOURCE_RFFE);
      (void)rfc_wcdma_get_asdiv_xsw_script(asdiv_position,rx_device_buffer[RFWCDMA_MC_ASM_TUNER_SCRIPTS],(void*)&(xsw_script_data));
    }
  }
#endif

#ifdef FEATURE_RF_HAS_QTUNER

  /* Enable both PRx and DRx tuners during wakeup so that when
  doing IRAT/meas the init scripts for DRx need not be generated and executed.
  This is to avoid adding burden to IRAT timeline when enabling
  DRx for IRAT/meas (e.g. X2L).
  During initialization using the same band for Path-0 and Path-1,
  as Path-1 state machine may not be initialized in this context */

  /* Program the tuner in Rx mode*/
  rfwcdma_core_rxctl_disable_tuner(device,
                                   source_band,
                                   RFDEVICE_CREATE_SCRIPT,
                                   rx_device_buffer[RFWCDMA_MC_ASM_TUNER_SCRIPTS],
                                   RFCOM_INVALID_TIMING_OFFSET,
                                   disable_ind);

  rfwcdma_core_rxctl_enable_tuner(device,
                                  band,
                                  carr_id,
                                  rx_params.channel[0],
                                  RFDEVICE_CREATE_SCRIPT,
                                  rx_device_buffer[RFWCDMA_MC_ASM_TUNER_SCRIPTS]);

#endif /* FEATURE_RF_HAS_QTUNER */

  rfwcdma_set_tri_carrier_status(device,carr_id);

  uint8 sub_id = rfwcdma_data_get_subscription_type(device);
  RF_MSG_2(RF_HIGH, "Calling rfdevice_wcdma_rx_set_channel: device %d, sub_id %d", device, sub_id);

  /* If dynamic spur update is not needed, then this param will be NULL */
  if (dyn_notch_filter_settings == NULL) 
  {  
  rfdevice_wcdma_rx_set_channel( device,
                                 RFCOM_WCDMA_MODE,
                                 band,
                                 rx_params,
                                 RFDEVICE_CREATE_SCRIPT,
                                 rx_device_buffer[RFWCDMA_MC_WTR_SCRIPTS], 
                                 NULL, NULL, FALSE, asdiv_alt_gain, (rfdevice_trx_sub_type)sub_id );
  }

  else
  {
    rfdevice_wcdma_rx_set_channel( device,
                                   RFCOM_WCDMA_MODE,
                                   band,
                                   rx_params,
                                   RFDEVICE_CREATE_SCRIPT,
                                   rx_device_buffer[RFWCDMA_MC_WTR_SCRIPTS], 
                                   rfwcdma_core_rxctl_update_dynamic_notch_cb, 
                                   dyn_notch_filter_settings, TRUE, asdiv_alt_gain, (rfdevice_trx_sub_type)sub_id );
  }
  
  RF_MSG_3(RF_HIGH, "rfdevice_wcdma_lna_wakeup called on device %d,band %d, sub_id %d", device,source_band, sub_id); 

  /* Wakeup the eLNA */
  rfdevice_wcdma_lna_wakeup(device,
                             band,
                             rx_device_buffer[lna_script_type],
                             RFDEVICE_CREATE_SCRIPT,
                             RFCOM_INVALID_TIMING_OFFSET);
 
  /* Put the systm gain state of eLNA and LNA to default CGAGC gain state, after tune script 
        - Setup and Set Gain APIs */

  lna_param.current_state = RF_WCDMA_DEFAULT_LNA_STATE;

  /* Set the gain state of WTR LNA */
  rfdevice_wcdma_rx_lna_setup(device,
                              band,
                              &lna_param,
                              RFDEVICE_CREATE_SCRIPT,
                              rx_device_buffer[RFWCDMA_MC_WTR_SCRIPTS]);

  /* Set the gain state of eLNA */
  rfdevice_wcdma_lna_set_gain(device,
                              band,
                              rx_device_buffer[lna_script_type],
                              RFDEVICE_CREATE_SCRIPT,
                              RFCOM_INVALID_TIMING_OFFSET,
                              RF_WCDMA_DEFAULT_LNA_STATE,
                              asdiv_alt_gain);

  /*Destroy local RF dummy buffers*/  
  rfwcdma_mc_destroy_rf_buff(lna_query_buffer);

  

}/*rfwcdma_mc_create_scripts*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Execute Wakeup Rx sequence with immediate write to turn on the radio and tune
  to the given band and channel.

  @details
  This function will send a command to firmware to indicate that Rx needs to be
  started. The command will indicate FW to activate the Script.

  @param device
  The receive path to wakeup.

  @param rxlm_buf_idx
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @return
  Variable indicating the status of API execution. The error codes are
  defined in #rfm_wcdma_error_code_type enumeration.
*/
rfm_api_status_t
rfwcdma_mc_exec_wakeup_rx_imm
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_buf_idx
)
{
  rf_path_enum_type path = rfcommon_core_device_to_path(device);
  boolean wcdma_device_vote;
  timetick_type prof_cal_func;

  uint16 pll_settling_time = 200;
  rf_buffer_intf *buffer_ptr = NULL;
  rfdevice_wcdma_rx_tune_param_type rx_params;
  uint32 alt_path = 0;
  rfm_device_enum_type paired_dev;
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_enable_params = {0};
  boolean asdiv_alt_gain = FALSE;

#ifdef FEATURE_RF_ASDIV
  if(rfcommon_asdiv_get_current_position(device)==ASDIV_POSITION_1)
  {
    asdiv_alt_gain = TRUE;
  }
  else
  {
    asdiv_alt_gain = FALSE;
  }
#endif

  if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE) 
  {
    rx_params.rx_mode = RFDEVICE_PRX_OPER_MODE;
  }
  else if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE) 
  {
    rx_params.rx_mode = RFDEVICE_DRX_OPER_MODE;
  }
  else
  {
    rx_params.rx_mode = RFDEVICE_INVALID_OPER_MODE;
  }

  paired_dev = rfwcdma_data_get_associated_device_rx(device);

  rx_params.paired_device = paired_dev;

  rx_params.bw   = RFDEV_WCDMA_BW_INVALID;
  rx_params.band = RFCOM_BAND_INVALID;

  if(device >= RFM_WCDMA_MAX_DEVICE)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_mc_wakeup:Invalid device %d",device);
    return 0;
  }

  if ( enable_wakeup_optimization_flag == TRUE )
  {
    //ERR_FATAL( "********* DISABLED rfwcdma_mc_wakeup *********", 0, 0, 0 );
    RF_MSG(RF_HIGH," rfwcdma_mc_wakeup_rx_imm: WL1 still using the non-optimized wakeup sequence");
  }

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  /* Update Alt path to RFC*/
  rfwcdma_core_rxctl_update_alt_path(device, 
                                     rfwcdma_data_get_curr_band(device), 
                                     FALSE,
                                     &alt_path);
  
  rfwcdma_data_set_alt_path(device,alt_path);

  /* Create rf_buf */
  rfwcdma_mc_create_rf_buff(&buffer_ptr);

  if (rfwcdma_data_get_rf_state(device) !=RFWCDMA_STATE_RX_SLEEP)
  {
    RF_MSG_2(RF_HIGH, "Wakeup from Non-Sleep state - Rx path: %d state: %d",
             path, rfwcdma_data_get_rf_state(device));

    /*
                    If RF is not in sleep state, only send start AGC if needed
                    Following conditions need to be met to send start AGC command to avoid sending back to back AGC start command
                    1. FW RxAGC ON status flag need to be OFF
                    2. RF internal RxAGC enable flag need to be FALSE
    */

    if (rfwcdma_core_util_is_rxagc_enabled(device, RFCOM_MULTI_CARRIER_ID__0) == FALSE)
    {
      /* Set the RxAGC enable params */
      memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));
      rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxagc_enable_params.rxlm_buf_idx[device] = rxlm_buf_idx;
      rxagc_enable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);
      rxagc_enable_params.multi_carrier_idx = rfwcdma_data_get_carrier_mask(device);
      rxagc_enable_params.rf_warmup_en = FALSE;
      rxagc_enable_params.nbr_en_flag = FALSE;
      rxagc_enable_params.nbr_rxlm_buf_idx = 0;

      /* Send wakeup AGC cmd to FW */
      rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);

    }

    /* Destroy rf_buff */
    rfwcdma_mc_destroy_rf_buff(buffer_ptr);

    return RFM_WCDMA_HEALTHY_STATE;
  }

  /* If wakeup optimization is not enabled, write SSBI registers directly to device and wait for
  PLL to settle */
  if (enable_wakeup_optimization_flag == FALSE)
  {
    /* Clear MC buffer */
    (void)rf_buffer_clear(buffer_ptr);

    /* Configure ASM Rx */
    rfwcdma_core_rxctl_enable_asm(device,
                                  rfwcdma_data_get_curr_band(device),
                                  NULL,
                                  RFDEVICE_EXECUTE_IMMEDIATE,
                                  RFCOM_INVALID_TIMING_OFFSET);

    rfc_wcdma_wakeup(device);

    /* Step2 for RFC: Configure RFC signals for the band in which
                    RF wakeup needs to be done */
    rfc_wcdma_rx_band_config(device, rfwcdma_data_get_curr_band(device));

#ifdef FEATURE_RF_HAS_QTUNER

    /* Enable both PRx and DRx tuners in at the time of Rx init so that when
                    doing IRAT/meas the init scripts for DRx need not be generated and executed.
                    This is to avoid adding burden to IRAT timeline when enabling
                    DRx for IRAT/meas (e.g. X2L).
                    During initialization using the same band for Path-0 and Path-1, as Path-1 state
                    machine may not be initialized in this context
    */
    /* Program the tuner in Rx mode*/
    rfwcdma_core_rxctl_enable_tuner(device,
        rfwcdma_data_get_curr_band(device),
                                    rfwcdma_data_get_carrier_mask(device),
        rfwcdma_data_get_curr_chan(device,0),
        RFDEVICE_EXECUTE_IMMEDIATE,
        NULL);

#endif /* FEATURE_RF_HAS_QTUNER */


    /*! Tune tranceiver device */

    /* Set rx tune params */
    rx_params.band       = rfwcdma_data_get_curr_band(device);
    rx_params.bw         = RFDEV_WCDMA_BW_1X;
    rx_params.channel[0] = rfwcdma_data_get_curr_chan(device,0);

    uint8 sub_id = rfwcdma_data_get_subscription_type(device);
    RF_MSG_2(RF_HIGH, "Calling rfdevice_wcdma_rx_set_channel: device %d, sub_id %d", device, sub_id);

    /* Tune Rx PLL */
    rfdevice_wcdma_rx_set_channel( device, RFCOM_WCDMA_MODE, rx_params.band,
                                   rx_params, RFDEVICE_EXECUTE_IMMEDIATE, 
                                   NULL, NULL, NULL, FALSE, asdiv_alt_gain, (rfdevice_trx_sub_type)sub_id );


    #ifdef FEATURE_RF_POWER_REPORTING
    /* associate handle with device */
    rflm_dm_handle_meta_s handle_meta;
    rflm_dm_get_handle_info(  rxlm_buf_idx , &handle_meta);
    handle_meta.devide_id = device;
    handle_meta.device_id = device;
    rflm_dm_set_handle_info( rxlm_buf_idx , &handle_meta );
    /* log channel number */
    rf_power_reporting_update_channel( device, rx_params.channel[0] );
    #endif

    /* Wait for PLL to settle after tune*/
    DALSYS_BusyWait(pll_settling_time);

    /* Start RxAGC */
    if (rfwcdma_core_util_is_rxagc_enabled(device, RFCOM_MULTI_CARRIER_ID__0) == FALSE)
    {
      /* Set the RxAGC enable params */
      memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));
      rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
      rxagc_enable_params.rxlm_buf_idx[device] = rxlm_buf_idx;
      rxagc_enable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);
      rxagc_enable_params.multi_carrier_idx = rfwcdma_data_get_carrier_mask(device);
      rxagc_enable_params.rf_warmup_en = FALSE;
      rxagc_enable_params.nbr_en_flag = FALSE;
      rxagc_enable_params.nbr_rxlm_buf_idx = 0;

      /* Send wakeup AGC cmd to FW */
      rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
    }

  }


  rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_RX);
   RF_MSG_1(RF_HIGH, "rfwcdma_mc_wakeup(device=%d)", device);

  /* Destroy rf_buff */
  rfwcdma_mc_destroy_rf_buff(buffer_ptr);
  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Wakeup -%d", prof_cal_func);
  }
  return RFM_WCDMA_HEALTHY_STATE;

}



/*----------------------------------------------------------------------------*/
/*!
@brief
 Enable WCDMA Rx AGC

 @details
 This function will send a command to firmware to indicate that Rx needs to be
 started. The command will indicate FW to activate the Script.

 @param device_mask
 Parameter indicating the devices on which RxAGC needs to be enabled 

 @param rxlm_handle
 The Rx Link Manager buffer that shall be used to configure the
 modem hardware.

 @return
 Variable indicating the status of API execution. The error codes are
 defined in #rfm_wcdma_error_code_type enumeration.
*/
rfm_api_status_t
rfwcdma_mc_enable_rxagc
(
  rfwcdma_device_mask_type device_mask,
  lm_handle_type rxlm_handle[RFM_MAX_WAN_DEVICES]
)
{
  rfm_api_status_t ret_val = RFM_WCDMA_HEALTHY_STATE; /* Return value */
  rf_path_enum_type path;
  rfwcdma_data_debug_flags_type *dbg_flag;
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_enable_params = {0};
  uint8 num_devices =device_mask;
  rfm_device_enum_type device =0;
  uint32 multicarrier_idx_enable =0;

  memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));
 
  /* Get debug flags */
  dbg_flag = rfwcdma_get_debug_flags();

  if ( dbg_flag->enable_opt_wakeup == FALSE )
  {
    RF_MSG( RF_HIGH, "rfwcdma_mc_enable_rxagc: enable_opt_wakeup = FALSE " );

    //ERR_FATAL( "********* DISABLED rfwcdma_mc_enable_rxagc *********",
    //           0, 0, 0 );
  }

  if(device_mask == 0)
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_mc_enable_rxagc: No device passed, mask %d", device_mask );
    return RFM_WCDMA_ERROR_FAILURE;
  }

  while (num_devices != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

    if((device >= RFM_DEVICE_0) && (device < RFM_WCDMA_MAX_DEVICE))
    {
      /* Perform Device to Path mapping */
      path = rfcommon_core_device_to_path(device);

      multicarrier_idx_enable|=rfwcdma_data_get_carrier_mask(device);

      /* Path validation */
      if ( path > RF_PATH_1 )
      {
        RF_MSG_3( RF_ERROR, "rfwcdma_mc_enable_rxagc: Device %d, RxLM %d - "
                  "Invalid Path %d mapping",
                  device,
                  rxlm_handle[device],
                  path  );
        return RFM_WCDMA_ERROR_BAD_PATH_MAPPING;
      } /* if ( path > RF_PATH_1 ) */

      /* Enable RxAGC only if the AGC is not enabled */
      if (rfwcdma_mdsp_get_rxagc_enable_status(
            (rfagc_receive_chain_type)path,
            RFCOM_MULTI_CARRIER_ID__0) == TRUE )
      {
        RF_MSG_3( RF_ERROR, "rfwcdma_mc_enable_rxagc: Device %d, RxLM %d, Path %d - "
                  "AGC is already enabled!",
                  device,
                  rxlm_handle[device],
                  path );
        return RFM_WCDMA_ERROR_FAILURE;
      } /* if(rfwcdma_mdsp_get_rxagc_enable_status == TRUE)*/

      else
      {
        /* Set the RxAGC enable params */
        rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
        rxagc_enable_params.rxlm_buf_idx[device] = rxlm_handle[device];
        rxagc_enable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);
      }/* if!(rfwcdma_mdsp_get_rxagc_enable_status == TRUE) */

      num_devices &= (~rf_extract_first_one_bit_mask(num_devices));
    }

    else
    {
      RF_MSG_1( RF_ERROR, "rfwcdma_mc_enable_rxagc: invalid device %d", device );
      return RFM_WCDMA_ERROR_FAILURE;
    }
  }

  /* Set the remaining RxAGC enable params */
  rxagc_enable_params.multi_carrier_idx = multicarrier_idx_enable;
  rxagc_enable_params.rf_warmup_en = FALSE;
  rxagc_enable_params.nbr_en_flag = FALSE;
  rxagc_enable_params.nbr_rxlm_buf_idx = 0;

  /* Send wakeup AGC cmd to FW */
  rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);

  /* Return the API Wait time */
  return ret_val;
}


/*! @cond vbhide */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Return RF Tx bandwidth for uplink multi-carrier/single-carrier fast switching

  @details
  This function will reprogram Tx devices for Tx bandwith change, Tx has to
  be already enabled (rfm_init_wcdma_tx and rfm_enable_tx called). It can be
  called for fast bandwidth switching.

  Scenarios                                RF API Call flows
  Bringup Tx in SC mode                    rfm_init_wcdma_tx(SC)+rfm_enable_tx()
  Configure DC from SC                     rfm_init_wcdma_tx(DC) (non-critcal timeline, lin+txlm)
  Activate DC from DC configured           rfm_wcdma_tx_retune_bw(DC)- move PDA control here
  Deactivate DC to DC configured           rfm_wcdma_tx_retune_bw(SC)
  Re-activate DC with DC configured        rfm_wcdma_tx_retun_bw(DC)
  Deconfigure DC after deactivation        rfm_init_wcdma_tx(SC) (non-critical timeline, lin+txlm)
  Seamless reconfig (direct DC configured) rfm_init_wcdma_tx(DC) + rfm_enable_tx()

  @param device
  RFM device type

  @param multi_carrier_idx
  Tx Multi Carrier Index Bitmask

*/
rfm_api_status_t
rfwcdma_mc_tx_retune_bw
(
  const rfm_device_enum_type device,
  const uint32 multi_carrier_idx
)
{
  uint16 channels[RFCOM_MAX_CARRIERS];
  rfcom_tx_multi_carrier_cfg_type tx_cfg_mode = RFCOM_TX_SINGLE_CAR_CFG;
  /* Convert Path from Device */
  rf_path_enum_type path = rfcommon_core_device_to_path(device);

  /* Device current band and channel from state machine. */
  rfcom_wcdma_band_type band = rfwcdma_data_get_curr_band(device);
  uint16 chan0 = rfwcdma_data_get_curr_chan(device,RFCOM_SINGLE_CARRIER);
  uint16 chan1 = rfwcdma_data_get_curr_chan(device,RFCOM_DUAL_CARRIER);
   
  /* Device Tx Tune params*/
  rfdevice_wcdma_tx_tune_param_type tx_params = {0};

  /* Multi Carrier Type (bandwith) converted from Multi Carrier Index */
  rfcom_multi_carrier_hspa_id_type multi_carrier_type = rfwcdma_core_count_carriers(multi_carrier_idx);

  /* Status return value */
  rfm_api_status_t ret_val = RFM_WCDMA_HEALTHY_STATE;

  /* PA bandwidth parameter */
  rfdevice_pa_bw_enum_type pa_bw = RFDEVICE_PA_BW_NOT_APPLICABLE;

  boolean api_status = TRUE;


  if(multi_carrier_idx == RFWCDMA_MC_DC)
  {
    tx_cfg_mode = RFCOM_TX_DUAL_CAR_CFG ;
  }
  else if(multi_carrier_idx == RFWCDMA_MC_SC)
  {
    tx_cfg_mode = RFCOM_TX_SC_RF_DC_BB_CFG ; 
  }

  /* Parameter valid range check, Tx supports up to 2 carriers for now */
  if ((band >= RFCOM_NUM_WCDMA_BANDS) || (multi_carrier_type > RFCOM_DUAL_CARRIER) || (path > RF_PATH_1))
  {

    RF_MSG_4(RF_ERROR, "rfm_wcdma_tx_retune_bw, invalid parameters: band:%d, multi_carrier_idx:%d multi_carrier_type:%d, path:%d",
             band,
             multi_carrier_idx,
             multi_carrier_type,
             path);

    ret_val = RFM_WCDMA_ERROR_INVALID_PARAMS;
  }
  else
  {
    if ( rfc_common_check_band_supported( rfcommon_core_convert_rfcom_band_to_sys_band(RFM_IMT_MODE, (rfcom_band_type_u)band), device ) == FALSE )
    {
      RF_MSG_2( RF_ERROR, "rfwcdma_mc_tx_retune_bw ERROR device %d does not support WCDMA RF band %d.",
                device, band );
      return RFM_WCDMA_ERROR_INVALID_PARAMS;
    }

  /* Prepare for Multi-Carrier config */
  channels[0] = rfwcdma_data_get_curr_chan(device,RFCOM_SINGLE_CARRIER); 
  channels[1] = rfwcdma_data_get_curr_chan(device,RFCOM_DUAL_CARRIER);  
      /* Config Multi-Carrier */
    rfwcdma_core_txctl_config_mc(rfwcdma_data_get_curr_band(device), multi_carrier_idx,
                                 channels,
                                 rfwcdma_data_get_rflm_buf_idx(device),
                                 tx_cfg_mode);
    

    /* Prepare for WTR Tx PLL tune Poluate Device Tx params  */
    /* Get DPD state */
    if (rfwcdma_core_xpt_is_enabled(band))
    {
      tx_params.dpd_state = RFDEVICE_DPD_ENABLED;
    }
    else
    {
      tx_params.dpd_state = RFDEVICE_DPD_DISABLED;
    }

    tx_params.band = band;
    tx_params.ul_dl = RFWCDMA_CORE_UARFCN_UL;

    /* Set tx tune params based on dual carrier or single carrier mode */
    if (multi_carrier_type == RFCOM_SINGLE_CARRIER )
    {
      /* Update BW parameter and channels for SC */
      tx_params.bw         = RFDEV_WCDMA_BW_1X;
      tx_params.channel[0] = chan0;
      tx_params.channel[1] = chan0;

      /* Update PA BW */
      pa_bw = RFDEVICE_PA_BW_5MHZ;
    }
    else
    {
      /* Update BW parameter and channels for DC*/
      tx_params.bw         = RFDEV_WCDMA_BW_2X;
      tx_params.channel[0] = chan0;
      tx_params.channel[1] = chan1;

      /* Update PA BW */
      pa_bw = RFDEVICE_PA_BW_10MHZ;
    }

    /* Tune Tx PLL */
    rfdevice_wcdma_tx_set_channel(device, RFCOM_WCDMA_MODE,
                                  tx_params.band, tx_params,
                                  RFDEVICE_EXECUTE_IMMEDIATE, NULL);

    #ifdef FEATURE_RF_POWER_REPORTING
    /* associate handle with device */
    rflm_dm_handle_meta_s handle_meta;
    rflm_dm_get_handle_info(   rfwcdma_data_get_rflm_buf_idx(device) , &handle_meta);
    handle_meta.devide_id = device;
    handle_meta.device_id = device;
    rflm_dm_set_handle_info(  rfwcdma_data_get_rflm_buf_idx(device) , &handle_meta );
    /* log channel number */
    rf_power_reporting_update_channel( device, tx_params.channel[0]  );
    #endif

    /* Confg PA BW */
    rfdevice_wcdma_pa_config(device,
                             band,
                             NULL,
                             RFDEVICE_EXECUTE_IMMEDIATE,
                             RFCOM_INVALID_TIMING_OFFSET,
                             pa_bw);

#ifdef FEATURE_TABASCO_MODEM 
   (void)rfdevice_wcdma_pa_on_off(device,
                                 band,
                                 TRUE,
                                 NULL,
                                 RFDEVICE_EXECUTE_IMMEDIATE,
                                 RFCOM_INVALID_TIMING_OFFSET);
#endif

  if (api_status == FALSE)
  {
    ret_val = RFM_WCDMA_ERROR_FAILURE;
  }

  }/*if !((band >= RFCOM_NUM_WCDMA_BANDS) || (multi_carrier_type > RFCOM_DUAL_CARRIER) || (path > RF_PATH_1))*/

  return ret_val;

}
/*! @endcond */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Prepare to turn on the receive radio and tune to the active band and channel
  on the device(s) specified.

  @details
  Prepares the software to move the WCDMA reciever from the Sleep state to the
  Rx state. This function will prepare scripts for the device(s) specified
  to turn on the receiver.The scripts will be created for the band and channel which
  was tuned to earlier during tuning operation or pre-tuning operation. The
  function will not perform any operation related to direct access to the
  hardware, as it is not guaranteed that all the necessary clock are enabled
  while calling this function.

  @param device_mask
  This is a bitmask passed indicating Rx devices which need to be turned on.

  @param rxlm_buf_idx
  The Rx Link Manager buffer array that shall be used to configure the
  modem hardware. the array will hold the rxlm handles for only the specified devices

  @return
  Variable indicating the status of API execution. The error codes are
  defined in #rfm_wcdma_error_code_type enumeration.
*/
rfm_api_status_t
rfwcdma_mc_rx_prep_wakeup
(
  uint32 device_mask,
  uint32 rxlm_buf_idx[RFM_MAX_WAN_DEVICES]
)
{
  rfm_api_status_t ret_val = RFM_WCDMA_HEALTHY_STATE; /* Return value */
  rf_path_enum_type path;
  uint32 execution_time =0;
  uint16 pll_settling_time = 200, channel[RFM_MAX_WAN_DEVICES];
  uint32 warmup_time_us = 0, script_type=0;
  uint32 num_devices =0, valid_scripts =0;
  rfm_device_enum_type device =0, paired_dev = 0;
  uint32 carrier_mask =0, alt_path =0;
  uint8 num_chan =0;
  rfwcdma_data_debug_flags_type *dbg_flag;
  uint8 num_valid_dev  = 0;

  rf_buffer_intf *rf_wakeup_script_buffer[RFWCDMA_MC_WAKEUP_SCRIPTS_MAX] = { NULL };

  /* Data structures needed for populating the wakeup scripts in DM */
  rfwcdma_mdsp_rx_wakeup_cfg_type cfg;

  memset(&cfg, 0, sizeof(rfwcdma_mdsp_rx_wakeup_cfg_type));

  /* Get debug flags */
  dbg_flag = rfwcdma_get_debug_flags();

  if ( dbg_flag->enable_opt_wakeup == FALSE )
  {
    RF_MSG( RF_HIGH, "rfwcdma_mc_prep_wakeup_rx: enable_opt_wakeup = FALSE " );
  }

  /* Create RF buffer to populate the script. This buffer will be maintained
  until we populate the DM */
  for ( script_type=0; script_type < RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++)
  {
    rfwcdma_mc_create_rf_buff(&rf_wakeup_script_buffer[script_type]);

    if (rf_wakeup_script_buffer[script_type]== NULL)
    {
      RF_MSG(RF_ERROR,
             "rfwcdma_mc_rx_prep_wakeup: rf buffer allocation failed!");
      break;
    }
  }

  /* Clear MC buffer */
  for ( script_type =0; script_type < RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++)
  {
    rf_buffer_clear(rf_wakeup_script_buffer[script_type]);
  }

  /* Create GRFC for mode config */
  (void)rfwcdma_core_generate_mode_config_grfc_script(rf_wakeup_script_buffer[RFWCDMA_MC_GRFC_SCRIPTS]);

  num_devices= device_mask;

  while (num_devices != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

    if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))
    {
      /* For PRx+ DRx tune, build scripts for Primary first for only PonD and DonP configuration
         but for regular PRx + DRx tune, Primary will be built first automatically */
      paired_dev = rfwcdma_data_get_associated_device_rx(device);

      if( (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE) &&
          (paired_dev >= RFM_DEVICE_0)&& (paired_dev < RFM_WCDMA_MAX_DEVICE) &&
          ( ((1 << paired_dev) & num_devices) != 0 ) 
         )
      {
        device = paired_dev; // Build scripts for Primary first
      }

       path = rfcommon_core_device_to_path(device);

       carrier_mask =  rfwcdma_data_get_carrier_mask(device);

       channel[0]= rfwcdma_data_get_curr_chan(device,0);

       num_chan = rfwcdma_data_get_num_carriers(device);

       RF_MSG_5(RF_HIGH, "rfwcdma_mc_prep_wakeup: Parameters - num_devices %d device: %d"
                " path: %d,carrier_index: %d, channel: %d",
                num_devices,device,path, carrier_mask,
                channel[0]);

       /* Update Alt path to RFC*/
       rfwcdma_core_rxctl_update_alt_path(device, 
                                          rfwcdma_data_get_curr_band(device), 
                                          FALSE,
                                          &alt_path);
	   
       rfwcdma_data_set_alt_path(device,alt_path);
	   
       rfwcdma_mc_create_device_scripts(device,
                                        rfwcdma_data_get_curr_band(device),
                                        channel,
                                        rf_wakeup_script_buffer,
                                        num_chan,
                                        RFCOM_BAND_INVALID,
                                        RFWCDMA_CORE_ASM_TUNER_NO_DISABLE_OPS,
                                        NULL);

       /* Reallocate any Common Notches deallocated*/
       (void)rfwcdma_msm_cmn_notch_filter_reallocate(device, rxlm_buf_idx[device], FALSE);

       #ifdef FEATURE_RF_POWER_REPORTING
       /* associate handle with device */
       rflm_dm_handle_meta_s handle_meta;
       rflm_dm_get_handle_info(  rxlm_buf_idx[device] , &handle_meta);
       handle_meta.devide_id = device;
       handle_meta.device_id = device;
       rflm_dm_set_handle_info( rxlm_buf_idx[device] , &handle_meta );
       /* log channel number */
       rf_power_reporting_update_channel( device, channel[0] );
       #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
       RF_MSG_2(RF_MED, "rfwcdma_mc.c rfwcdma_mc_rx_prep_wakeup device %d handle %d" , device, rxlm_buf_idx[device]);
       #endif
       #endif

       num_devices &=  (~(1 << device));

       num_valid_dev++;
    }

    else
    {
       RF_MSG_1(RF_ERROR, "rfwcdma_mc_rx_prep_wakeup: Invalid Device = %d", device);
       ret_val = RFM_WCDMA_ERROR_FAILURE;
    }
  }

  if (RFM_WCDMA_HEALTHY_STATE == ret_val) 
  {
     /* Get the WTR script execution time from Device */
     rf_buffer_get_script_execution_time(rf_wakeup_script_buffer[RFWCDMA_MC_WTR_SCRIPTS],
                                         RF_BUFFER_RFFE,
                                         &execution_time);

     for (script_type=0; script_type < RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++ )
     {
       if (rfwcdma_core_get_resource_script_from_buffer(rf_wakeup_script_buffer[script_type])!= NULL)
       {
         cfg.device_cfg.wakeup[valid_scripts].script= rf_wakeup_script_buffer[script_type];
         cfg.device_cfg.wakeup[valid_scripts].delay= 0;
         valid_scripts ++;
       }
     }

     warmup_time_us = (execution_time/10) + pll_settling_time;
     cfg.device_cfg.rf_warmup_time = WCDMA_CONV_US_TO_USTMR(warmup_time_us);

     RF_MSG_4(RF_HIGH, "rfwcdma_mc_rx_prep_wakeup: [Time Profile] Dev Mask %d:"
              "WTR script execution %d, "
              "PLL %d, Warmup - %d",
              device_mask,
              (execution_time/10),
              pll_settling_time,
              cfg.device_cfg.rf_warmup_time );


     /*Always send rflm handle of the first primary device*/
     if (0 != device_mask)
     {
       device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

       if((RFM_DEVICE_0 <= device)&& (device < RFM_MAX_WAN_DEVICES))
       {
         paired_dev = rfwcdma_data_get_associated_device_rx(device);

         if (num_valid_dev == 2 && (paired_dev < RFM_WCDMA_MAX_DEVICE)) 
         {/* This means Dual Wakeup. The logic will support prx dev > drx dev which is a valid requirement. Can't assume that first bit will be prx device only*/
          /*A valid assumption that one device has to be PRX is still made*/
           cfg.rflm_handle = (rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE)?
           rfwcdma_data_get_rflm_buf_idx(device):rfwcdma_data_get_rflm_buf_idx(paired_dev);
         }
         else/*Standalone single device Wakeup*/
         {
           cfg.rflm_handle = rfwcdma_data_get_rflm_buf_idx(device);
         }  

         /* Appends tasks which have valid scripts only*/
         cfg.device_cfg.num_tasks_wakeup_script = valid_scripts;

         rfwcdma_mdsp_async_config_rx_wakeup (&cfg);

         num_devices = device_mask;

         /* Update State information */
         while (num_devices != 0)
         {
           device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

           if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))
           {
             path = rfcommon_core_device_to_path(device);
             carrier_mask = rfwcdma_data_get_num_carriers(device);
             RF_MSG_4(RF_HIGH,"rfwcdma_mc_rx_prep_wakeup: MC state update - device: %d, path: %d,"
                      "carrier index: %d number of devices: %d",
                      device, path, carrier_mask, num_devices);

             rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_RX_PREP_WAKEUP);

             num_devices &= (~rf_extract_first_one_bit_mask(num_devices)); 
           }/*if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))*/
           else
           {
             RF_MSG_1(RF_ERROR, "rfwcdma_mc_rx_prep_wakeup: Invalid Device = %d", device);
             ret_val = RFM_WCDMA_ERROR_FAILURE;
             break;
           }
         }/*while(num_devices != 0)*/

         if (RFM_WCDMA_HEALTHY_STATE == ret_val) 
         {
            RF_MSG_3(RF_HIGH, "rfwcdma_mc_rx_prep_wakeup: Dev Mask %d:"
                     "Scripts pushed to DM %d, "
                     "RxLM buffer index %d",
                     device_mask,
                     valid_scripts,
                     cfg.rflm_handle);
         }
       }/*if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))*/
       else
       {
         RF_MSG_1(RF_ERROR, "rfwcdma_mc_rx_prep_wakeup: Invalid Device = %d", device);
         ret_val = RFM_WCDMA_ERROR_FAILURE;
       }
     }/*if device_mask != 0*/
     else
     {
       RF_MSG_1(RF_ERROR, "rfwcdma_mc_rx_prep_wakeup: Invalid Device mask = %d", device_mask);
       ret_val = RFM_WCDMA_ERROR_FAILURE;
     }
  }

  /*Destroy local RF buffers*/
  for ( script_type =0; script_type < RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++)
  {
    rfwcdma_mc_destroy_rf_buff(rf_wakeup_script_buffer[script_type]);
  }

  /* Return the API Wait time */
  return ret_val;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Execute Wakeup Rx sequence to turn on the radio and tune to the given band
  and channel on the passed device(s).

  @details
  This function will send a command to firmware to indicate that Rx needs to be
  started. The command will indicate FW to activate the Script.

  @param device_mask
  This is a bitmask passed indicating Rx devices which need to be turned on.

  @param rxlm_buf_idx
  The Rx Link Manager buffer array that shall be used to configure the
  modem hardware. the array will hold the rxlm handles for only the specified devices

  @return
  Variable indicating the status of API execution. The error codes are
  defined in #rfm_wcdma_error_code_type enumeration.
*/
rfm_api_status_t
rfwcdma_mc_rx_exec_wakeup
(
  uint32 device_mask,
  uint32 rxlm_buf_idx[RFM_MAX_WAN_DEVICES]
)
{
  rfm_api_status_t ret_val = RFM_WCDMA_HEALTHY_STATE; /* Return value */
  rf_path_enum_type path;
  boolean wcdma_device_vote;
  timetick_type prof_cal_func;
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_enable_params = {0};

  uint32 num_devices =0, multicarrier_idx =0;
  rfm_device_enum_type device =0;

  memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  num_devices= device_mask;

  while (num_devices != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_exec_wakeup: Invalid device %d", device);
      return 0;
    }
    path = rfcommon_core_device_to_path(device);

    multicarrier_idx |= rfwcdma_data_get_carrier_mask(device);

    if (0 == multicarrier_idx)
    {
      RF_MSG_2(RF_ERROR, "rfwcdma_mc_rx_exec_wakeup: Multicarrier index = %d must be non zero for device=%d", multicarrier_idx, device);
      return RFM_WCDMA_ERROR_FAILURE;
    }

   RF_MSG_4(RF_HIGH, "rfwcdma_mc_rx_exec_wakeup: Parameters - num_devices %d device: %d path: %d, multicarrier_idx: %d",
             num_devices,device,path, multicarrier_idx);

    RF_MSG(RF_HIGH, "rfwcdma_mc_rx_exec_wakeup(): calling rfwcdma_mdsp_sync_config_ccs ");

    /* Path validation */
    if ( path > RF_PATH_1 )
    {
      RF_MSG_3( RF_ERROR, "rfwcdma_mc_rx_exec_wakeup: Device %d, RxLM %d - "
                "Invalid Path %d mapping",
                device,
                rxlm_buf_idx[device],
                path  );
      return RFM_WCDMA_ERROR_BAD_PATH_MAPPING;
    } /* if ( path > RF_PATH_1 ) */

    /* Set the RxAGC enable params */
    rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
    rxagc_enable_params.rxlm_buf_idx[device] = rxlm_buf_idx[device];
    rxagc_enable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);

      /* Update State information */
     rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_RX);
    
    num_devices &= (~rf_extract_first_one_bit_mask(num_devices));

    #ifdef FEATURE_RF_POWER_REPORTING
    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_2(RF_MED, "rfwcdma_mc.c - rfwcdma_mc_rx_exec_wakeup handle id %d, rx device %d", 
            rxlm_buf_idx[device],
            device);
    #endif
    rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_RX_HANDLE, rxlm_buf_idx[device], RFLM_DIAG_POWER_RFA_STATE_ON);
    #endif

  }/*while (num_devices != 0)*/

  /* Set the remaining RxAGC enable params */
  rxagc_enable_params.multi_carrier_idx = multicarrier_idx;
  rxagc_enable_params.rf_warmup_en = TRUE;
  rxagc_enable_params.nbr_en_flag = FALSE;
  rxagc_enable_params.nbr_rxlm_buf_idx = 0;

   #ifdef FEATURE_RF_HAS_WTR3925
    (void)rfwcdma_core_rxctl_enable_dc_gain(&rxagc_enable_params);
   #endif

  /* Send wakeup AGC cmd to FW */
  rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);

  RF_MSG_2( RF_HIGH, "rfwcdma_mc_rx_exec_wakeup: Device Mask %d, "
            "Multi_carrier %d",
            device_mask,
            multicarrier_idx);

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Rx_exec_wakeup - %d", prof_cal_func);
  }
  /* Return the API Wait time */
  return ret_val;

} /* rfwcdma_mc_rx_exec_wakeup*/
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Turn off the receiver.

  @details
  Moves the WCDMA radio from the Rx state to the Sleep state.  Any circuits
  that are no longer needed are powered down to conserve current.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_WCDMA_RX_SLEEP_COMPLETE event.

  @param device_mask
  This is a bitmask passed indicating Rx devices which need to be turned on.

  @param skip_agc_disable
  Device list on which AGC disable needs to be skipped during the sleep operation

  @param rxlm_buf_idx
  The Rx Link Manager buffer array that shall be used to configure the
  modem hardware. the array will hold the rxlm handles for only the specified devices

  @return
  Variable indicating the status of API execution. The error codes are
  defined in #rfm_wcdma_error_code_type enumeration.
*/

rfm_api_status_t
rfwcdma_mc_rx_sleep
(
  uint32 device_mask,
  boolean skip_agc_disable[RFM_WCDMA_MAX_DEVICE], 
  uint32 rxlm_buf_idx[RFM_WCDMA_MAX_DEVICE]
)
{
  rfm_api_status_t ret_val = RFM_WCDMA_HEALTHY_STATE; /* Return value */
  timetick_type prof_cal_func;
  rf_path_enum_type path = RF_PATH_0;
  boolean wcdma_device_vote = FALSE, send_agc_disable_cmd = FALSE; 
  rfwcdma_data_debug_flags_type *dbg_flag;
  uint8 device_counter =0; 
  uint8 car_idx = 0;
  rfcom_wcdma_band_type source_band = RFCOM_BAND_INVALID; 
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_enable_params = {0};
  uint8 num_devices =0, multicarrier_idx  =0;
  rfm_device_enum_type device =0;
  rfm_device_enum_type paired_dev;
  rfdevice_rx_mode_type rx_mode;

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }

  memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));

  /* Get debug flags */
  dbg_flag = rfwcdma_get_debug_flags();

  if ( dbg_flag->enable_opt_wakeup == FALSE )
  {
    RF_MSG( RF_HIGH, "rfwcdma_mc_sleep_rx: enable_opt_wakeup = FALSE " );
  }

  num_devices= device_mask;

  while (num_devices != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_sleep: Invalid device %d", device);
      return RFM_WCDMA_ERROR_FAILURE;
    }

    if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE) 
    {
      rx_mode = RFDEVICE_PRX_OPER_MODE;
    }
    else if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE) 
    {
      rx_mode = RFDEVICE_DRX_OPER_MODE;
    }
    else
    {
      rx_mode = RFDEVICE_INVALID_OPER_MODE;
    }
    
    paired_dev = rfwcdma_data_get_associated_device_rx(device);

    path = rfcommon_core_device_to_path(device);
    multicarrier_idx |= rfwcdma_data_get_carrier_mask(device);

    uint8 sub_id = rfwcdma_data_get_subscription_type(device);
    RF_MSG_2(RF_HIGH, "Calling rfdevice_wcdma_rx_disable: device %d, sub_id %d", device, sub_id);
	
    /* Put Transceiever Rx and Tx to low power mode */
    source_band = rfwcdma_data_get_curr_band(device);
    rfdevice_wcdma_rx_disable(device,
                              source_band,
                              RFDEVICE_EXECUTE_IMMEDIATE,
                              NULL, paired_dev, rx_mode, (rfdevice_trx_sub_type)sub_id);

 /* Put eLNA to low power mode */
    rfdevice_wcdma_lna_sleep(device,
                             source_band,
                             NULL,
                             RFDEVICE_EXECUTE_IMMEDIATE,
                             0);

 /* Put XSW to sleep */    
    rfdevice_wcdma_xsw_sleep(device,
                             source_band,
                             NULL,
                             RFDEVICE_EXECUTE_IMMEDIATE,
                             0,
                             rfcommon_asdiv_get_current_position(device));

   /* Put the ASM to sleep */
    rfwcdma_core_rxctl_disable_sleep_asm(device,
                                         rfwcdma_data_get_curr_band(device),
                                         NULL,
                                         RFDEVICE_EXECUTE_IMMEDIATE,
                                         RFCOM_INVALID_TIMING_OFFSET,
                                         RFWCDMA_CORE_ASM_TUNER_DIS_ALL_BANDS);

/*Disable tuner on the device passed*/
#ifdef FEATURE_RF_HAS_QTUNER
	  /* Disable the tuners in use for PATH-0*/
    rfwcdma_core_rxctl_disable_tuner(device,
	   			     rfwcdma_data_get_curr_band(device),
 				     RFDEVICE_EXECUTE_IMMEDIATE,
				     NULL,
                 		     RFCOM_INVALID_TIMING_OFFSET,
				     RFWCDMA_CORE_ASM_TUNER_DIS_ALL_BANDS);
#endif

    /* Put GRFCs into Init state for this particular band */
    rfc_wcdma_sleep(device, rfwcdma_data_get_curr_band(device));

    /* Deallocate any Common Notches allocated*/
    (void)rfwcdma_msm_cmn_notch_filter_deallocate(device, rxlm_buf_idx[device], FALSE);

    /* Stop AGC on either path only if it's still running, otherwise
    L1 might already put FW to sleep and this will crash FW, there
    are some other occasions where L1 will do early RF shut down where
    RFM sleep gets called without FW sleep */

    if (rfwcdma_data_get_carrier_mask(device) == 0)
    {
       RF_MSG_1(RF_ERROR, "rfwcdma_mc_rx_sleep: Carrier mask for device=%d cannot be zero", device);
       return RFM_WCDMA_ERROR_BAD_PARAM;
    }
    if ((rfwcdma_core_util_is_rxagc_enabled(device, (rfcom_multi_carrier_id_type)(rf_extract_first_one_bit_lsb_location(rfwcdma_data_get_carrier_mask(device)))) == TRUE))
        //|| (rfwcdma_mc_state.cpc_state_flag == TRUE))
    {
      if (skip_agc_disable[device] != TRUE)
    {
      /* Set the RxAGC enable params */
      rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
      rxagc_enable_params.rxlm_buf_idx[device] = rxlm_buf_idx[device];
      rxagc_enable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);
    }
      else 
        RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_sleep: AGC disable skipped on device %d", device);
    }

   /*-------------WL1 should call exit mode on all devices -------------------@vbhide*/
   
    rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_RX_SLEEP);

    num_devices &= (~rf_extract_first_one_bit_mask(num_devices));

  }/* while (num_devices != 0)*/

  for(device_counter = 0; device_counter < RFM_WCDMA_MAX_DEVICE; device_counter++)
  {
    if( rxagc_enable_params.cmd_action[device_counter] != RFWCDMA_MDSP_AGC_CMD_INVALID)
    {
      send_agc_disable_cmd = TRUE;
      break;
    }
  }

  if(send_agc_disable_cmd) 
  {
    /* Set the remaining RxAGC enable params */
    rxagc_enable_params.multi_carrier_idx = multicarrier_idx;
    rxagc_enable_params.rf_warmup_en = FALSE;
    rxagc_enable_params.nbr_en_flag = FALSE;
    rxagc_enable_params.nbr_rxlm_buf_idx = 0;

   #ifdef FEATURE_RF_HAS_WTR3925
    (void)rfwcdma_core_rxctl_enable_dc_gain(&rxagc_enable_params);
   #endif
    /* Send wakeup AGC cmd to FW */
    rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
  }

  num_devices= device_mask;

/* Abort pending AsDiv Event */
  #ifdef FEATURE_RF_ASDIV
  
    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_sleep: Invalid device %d", device);
      return RFM_WCDMA_ERROR_FAILURE;
    }
    rfwcdma_core_asdiv_abort(rxlm_buf_idx[device]);
  #endif
  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Rx_Sleep - %d", prof_cal_func);
  }

  #ifdef FEATURE_RF_POWER_REPORTING
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_2(RF_MED, "rfwcdma_mc.c - rfwcdma_mc_rx_sleep handle id %d, rx device %d", 
             rxlm_buf_idx[device],
             device);
  #endif
  rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_RX_HANDLE, rxlm_buf_idx[device], RFLM_DIAG_POWER_RFA_STATE_OFF);
  #endif

  /* Return the API Wait time */
  return ret_val;

} /* rfwcdma_mc_rx_sleep */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Prepare to tune the radio on the on the device(s) specified.

  @details
  Prepares scripts for Rx Tune on the provided set of devices.The scripts will be created for
  the band and channel which was requested. The
  function will not perform any operation related to direct access to the
  hardware, as it is not guaranteed that all the necessary clock are enabled
  while calling this function.

  @param device_mask
  This is a bitmask passed indicating Rx devices which need to be tuned

  @param rx_tune_params
  Pointer to the structure that holds the parameters passed by L1 to wakeup WCDMA Rx.

  @return
  Variable indicating the status of API execution. The error codes are
  defined in #rfm_wcdma_error_code_type enumeration.
*/
rfm_api_status_t
rfwcdma_mc_rx_prep_tune
(
  uint32 device_mask,
  rfm_wcdma_rx_tune_params *rx_tune_params
)
{
  rf_path_enum_type path =0;
  uint32 execution_time = 0;
  uint16 pll_settling_time = 200;
  uint32 warmup_time_us = 0, script_type= 0, carrier=0, channel_loop=0;
  uint32 num_devices = 0, valid_scripts = 0;
  rfm_device_enum_type device = 0, paired_dev = 0, pair_tx_dev = 0;
  uint32 carrier_mask = 0;
  uint8 current_mc_state = 0, num_chan = 0;
  rfcom_wcdma_band_type band = RFCOM_BAND_IMT;
  rfwcdma_data_debug_flags_type *dbg_flag;
  rfcom_wcdma_band_type source_band = RFCOM_BAND_INVALID;
  rfwcdma_core_rxctl_dis_first_ind_type disable_ind;
  rfdevice_wcdma_spur_param_type *dyn_notch_filter_settings = NULL;
  rfm_api_status_t ret_val = RFM_WCDMA_HEALTHY_STATE;
  rfcom_band_type_u band_u = {0};
  sys_band_class_e_type sys_band = -1; 

  rfwcdma_mdsp_set_data_param_type set_agc_config_param = { 0 };

  rf_buffer_intf *rf_tune_script_buffer[RFWCDMA_MC_WAKEUP_SCRIPTS_MAX] = { NULL };

  /* Data structures needed for populating the wakeup scripts in DM */
  rfwcdma_mdsp_rx_wakeup_cfg_type cfg;

  uint32 alt_path = 0;

  memset(&cfg, 0, sizeof(rfwcdma_mdsp_rx_wakeup_cfg_type));

  /* Create RF buffer to populate the script. This buffer will be maintained
  until we populate the DM */
  for ( script_type=0; script_type< RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++)
  {
    rfwcdma_mc_create_rf_buff(&rf_tune_script_buffer[script_type]);

    if (rf_tune_script_buffer[script_type]== NULL)
    {
      RF_MSG(RF_ERROR,
             "rfwcdma_mc_rx_prep_tune: rf buffer allocation failed!");
      break;
    }
  }

  /* Clear MC buffer */
  for ( script_type=0; script_type< RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++)
  {
    rf_buffer_clear(rf_tune_script_buffer[script_type]);
  }

  /* Create GRFC for mode config */
  (void)rfwcdma_core_generate_mode_config_grfc_script(rf_tune_script_buffer[RFWCDMA_MC_GRFC_SCRIPTS]);

  /* Use local copy of the passed device mask for all the computations */
  num_devices= device_mask;

  /* Loop over all the devices enabled for tune */
  while (num_devices != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

    if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))
    {
      /* For PRx+ DRx tune, build scripts for Primary first for only PonD and DonP configuration
         but for regular PRx + DRx tune, Primary will be built first automatically */
      paired_dev = rfwcdma_data_get_associated_device_rx(device);

      if( (rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE) &&
          (paired_dev >= RFM_DEVICE_0)&& (paired_dev < RFM_WCDMA_MAX_DEVICE) &&
          ( ((1 << paired_dev) & num_devices) != 0 ) 
         )
      {
        device = paired_dev; // Build scripts for Primary first
      }

       path = rfcommon_core_device_to_path(device);

       carrier_mask =  rx_tune_params->api_params[device].carrier_mask;

       /* Ensure the carrier mask is non-zero*/
       if (0 != carrier_mask)
       {
          carrier = rfwcdma_core_count_carriers(rx_tune_params->api_params[device].carrier_mask);

          if ((RFCOM_SINGLE_CARRIER <= carrier) && (carrier < RFCOM_MAX_CARRIERS))
          {

             /*Update source band before setting current band*/
             source_band = rfwcdma_data_get_curr_band(device); 

             rfwcdma_data_set_curr_band(device,
                                            rfwcdma_core_convert_chan_to_band(rx_tune_params->api_params[device].channel[channel_loop]));

             num_chan = rfwcdma_core_count_carriers(rx_tune_params->api_params[device].carrier_mask);
			 
			 /* MC state variable may not be updated, hence use the information from caller*/
             band = rfwcdma_core_convert_chan_to_band(rx_tune_params->api_params[device].channel[0]);

             if ( rfc_common_check_band_supported( rfcommon_core_convert_rfcom_band_to_sys_band(RFM_IMT_MODE, (rfcom_band_type_u)band), device ) == FALSE )
             {
               ERR_FATAL(" Check with Wl1, rfwcdma_mc_rx_prep_tune:device %d does not support WCDMA RF band %d. Cannot tune RX.",
                         device, band,0 );
             }             
             
             set_agc_config_param.set_data_cmd = RFWCDMA_MDSP_CONFIG_RxAGC_IN_PROGRESS; 
             set_agc_config_param.rflm_handle = rx_tune_params->api_params[device].rxlm_handle;
             set_agc_config_param.data.gen_b = TRUE;
             (void)rfwcdma_mdsp_async_set_data(&set_agc_config_param);
             
             if(source_band != band)
             {
               disable_ind = RFWCDMA_CORE_ASM_TUNER_DIS_IND_BAND;
             }
             else
             {
               disable_ind = RFWCDMA_CORE_ASM_TUNER_NO_DISABLE_OPS;
             }
			 
             rfwcdma_data_update_subscription_type(device, rx_tune_params->api_params[device].sub_type);

             RF_MSG_7(RF_HIGH, "rfwcdma_mc_rx_prep_tune: Parameters - Enabled device mask %d curr dev: %d "
                               " path: %d, carrier_mask: %d,"
                               " channel: %d, Curr_band %d, sub_id %d",
                               num_devices,device,
                               path, carrier_mask,
                               rx_tune_params->api_params[device].channel[0], band, rx_tune_params->api_params[device].sub_type);

             /* Update state machine with carrier mask because in prep tune we read the carrier mask from the 
             state machine while bringing up the Rx device and this still contains the previous value which
             results in us reading a stale value*/

             /* To have a memory of the current multi_carrier index we create a shadow. This is used in exec_tune
             to stop AGC on all the carriers. However, if this is the first tune after enter mode, we save the 
             multicarrier_idx passed in the tune_params  */

             if (rfwcdma_data_get_carrier_mask(device) == 0)
             {
               rfwcdma_data_set_carrier_mask_shadow(device, rx_tune_params->api_params[device].carrier_mask);
             }
             else
             {
               rfwcdma_data_set_carrier_mask_shadow(device, rfwcdma_data_get_carrier_mask(device));
             }

             rfwcdma_data_set_carrier_mask(device,rx_tune_params->api_params[device].carrier_mask);

             /* Update Alt path to RFC*/
             rfwcdma_core_rxctl_update_alt_path(device, band, FALSE, &alt_path);

			 rfwcdma_data_set_alt_path(device,alt_path);

             /* Allocate Rx DM buffers for the given RxLM handle */
             (void)rfwcdma_mdsp_allocate_dm_rx_buf((rflm_dm_handle_id_t)rx_tune_params->api_params[device].rxlm_handle);

             /* Get the global callback data here before calling Set channel */
             dyn_notch_filter_settings = rfwcdma_core_rxctl_get_dynamic_notch_cb_data(device);

             rfwcdma_mc_create_device_scripts(device,
                                              band,
                                              rx_tune_params->api_params[device].channel,
                                              rf_tune_script_buffer,
                                              num_chan,
                                              source_band,
                                              disable_ind,
                                              dyn_notch_filter_settings);

             #ifdef FEATURE_RF_POWER_REPORTING
             /* associate handle with device */
             rflm_dm_handle_meta_s handle_meta;
             rflm_dm_get_handle_info( rx_tune_params->api_params[device].rxlm_handle , &handle_meta);
             handle_meta.devide_id = device;
             handle_meta.device_id = device;
             rflm_dm_set_handle_info( rx_tune_params->api_params[device].rxlm_handle , &handle_meta );
             /* log channel number */
             rf_power_reporting_update_channel( device, rx_tune_params->api_params[device].channel[0] );
             #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
             RF_MSG_2(RF_MED, "rfwcdma_mc.c rfwcdma_mc_rx_prep_tune device %d handle %d" , device, rx_tune_params->api_params[device].rxlm_handle);
             #endif
             #endif


             /* Apply dynamic notches for DBDC during regular tune for SCELL */
             if ( (dyn_notch_filter_settings->apply_notch_immediate == FALSE) && (dyn_notch_filter_settings->current_cell_spur_count > 0) && (rfwcdma_data_get_carrier_mask(device) == 2) )
             {
               (void)rfwcdma_msm_update_rxlm_buffer(device,
                                                     rx_tune_params->api_params[device].channel,
                                                     carrier_mask,
                                                     rx_tune_params->api_params[device].rxlm_handle,
                                                     alt_path,
                                                     enable_nbr_flag,
                                                     dyn_notch_filter_settings, RFWCDMA_MSM_STATIC_PLUS_DYNAMIC_NOTCH_TUNE);
             }


             /* Applying static notches during regular tune */
             else
             {
             (void)rfwcdma_msm_update_rxlm_buffer(device,
                                                  rx_tune_params->api_params[device].channel,
                                                  carrier_mask,
                                                  rx_tune_params->api_params[device].rxlm_handle,
                                                  alt_path,
                                                  enable_nbr_flag,
                                                  dyn_notch_filter_settings, RFWCDMA_MSM_STATIC_NOTCH_TUNE);
             }

                 /* FR18317 - NBR */
                 /* Load up modG for NBR for PRx only  and overall enable is TRUE */
                 dbg_flag = rfwcdma_get_debug_flags();
                 
             if( dbg_flag->enable_rf_nbr_operations && enable_nbr_flag &&
                 (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE))
                 {
                   /* print dbg msg */
                   MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,
                                 "rfwcdma_mc_rx_prep_tune: W_ NBR DBG: Loading up modG on RxLM index %d for PRx",
                                 (uint8)nbr_rxlm_buf_idx);
                   (void)rfwcdma_msm_update_nbr_rxlm_buffer(device,
                                                        rx_tune_params->api_params[device].channel, 
                                                        num_chan, 
                                                        nbr_rxlm_buf_idx,
                                                        alt_path);
                 }

             rfwcdma_core_rxctl_config_band( rx_tune_params->api_params[device].rxlm_handle,
                                             device,
                                             band,
                                             carrier_mask,
                                             RFLM_WCDMA_HOME_MEAS,
                                             alt_path,
                                             MAX_RX_DEV_HANDLE_ITEM);

             rfwcdma_core_rxctl_config_chan( rx_tune_params->api_params[device].rxlm_handle,
                                             device,
                                             band,
                                             rx_tune_params->api_params[device].channel,
                                             carrier_mask,
                                             RFLM_WCDMA_HOME_MEAS,
                                             alt_path,
                                             MAX_RX_DEV_HANDLE_ITEM );


             if (num_chan == RFCOM_SINGLE_CARRIER )
             {
               RFGNSS_MC_V(wg_chan)(rx_tune_params->api_params[device].channel[0]);

               /* Update Notch filter settings */
               RFGNSS_MC_V(program_notch_filter)(RFGNSS_NOTCH_CFG_WCDMA);

             }

            num_devices &=  (~(1 << device));

          }/*if ((RFCOM_SINGLE_CARRIER <= carrier) || (carrier < RFCOM_MAX_CARRIERS))*/

          else
          {
            RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_prep_tune: carrier %d", carrier);
            ret_val = RFM_WCDMA_ERROR_FAILURE;
            break;
          }
       }/*if (0 != carrier_mask)*/

       else
       {
          RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_prep_tune: Invalid carrier_mask = %d", carrier_mask);
          ret_val = RFM_WCDMA_ERROR_FAILURE;
          break;
       }
    }/*if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))*/

    else
    {
      RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_prep_tune: Invalid device %d", device);
      ret_val = RFM_WCDMA_ERROR_FAILURE;
      break;
    }

  }/*while (num_devices != 0)*/

  if (RFM_WCDMA_HEALTHY_STATE == ret_val)
  {
     /* Push the scripts to DM
      1. Compute the warmup time from the WTR script execution time.
      2. Append the valid scripts to the mdsp data structure
      3. Configure RF-MDSP Async interface for RF wakeup config
       */

     /* Step 1 */

     /* Get the WTR script execution time from Device */
     rf_buffer_get_script_execution_time(rf_tune_script_buffer[RFWCDMA_MC_WTR_SCRIPTS],
                                         RF_BUFFER_RFFE,
                                         &execution_time);

     warmup_time_us = (execution_time/10) + pll_settling_time;

     cfg.device_cfg.rf_warmup_time = WCDMA_CONV_US_TO_USTMR(warmup_time_us);

     RF_MSG_3(RF_HIGH, "rfwcdma_mc_rx_prep_tune: [Script Time] Dev Mask %d:"
              "WTR script execution %d, Warmup time:%d",
              device_mask,
              (execution_time/10),
              cfg.device_cfg.rf_warmup_time );

     /* Step 2 */
     for (script_type=0; script_type < RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++ )
     {
       if (rfwcdma_core_get_resource_script_from_buffer(rf_tune_script_buffer[script_type])!= NULL)
       {
         cfg.device_cfg.wakeup[valid_scripts].script= rf_tune_script_buffer[script_type];
         cfg.device_cfg.wakeup[valid_scripts].delay= 0;
         valid_scripts ++;
       }
     }

     if (0 != device_mask)
     {
        /*Always send rflm handle of the first primary device*/
        device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

        if((RFM_DEVICE_0 <= device) && (device < RFM_WCDMA_MAX_DEVICE))
        {               
           paired_dev = rfwcdma_data_get_associated_device_rx(device);  
           
           if ((rx_tune_params->api_params[device].is_valid == TRUE) && (paired_dev < RFM_WCDMA_MAX_DEVICE) && (rx_tune_params->api_params[paired_dev].is_valid == TRUE)) 
           {/* This means Dual tune. The logic will support prx dev > drx dev which is a valid requirement. Can't assume that first bit will be prx device only*/
            /*A valid assumption that one device has to be PRX is still made*/
             cfg.rflm_handle = (rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE)?
                         rfwcdma_data_get_rflm_buf_idx(device):rfwcdma_data_get_rflm_buf_idx(paired_dev);
           }
           else/*Standalone single device tune*/
           {
             cfg.rflm_handle = rfwcdma_data_get_rflm_buf_idx(device);
           }
           
           /* Appends tasks which have valid scripts only*/
           cfg.device_cfg.num_tasks_wakeup_script = valid_scripts;

           /* Step 3*/
           rfwcdma_mdsp_async_config_rx_wakeup (&cfg);

           num_devices = device_mask;

           /* Update State information */
           while (num_devices != 0)
           {
             channel_loop = 0;
             device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

             if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))
             {

                path = rfcommon_core_device_to_path(device);
                carrier_mask =  rx_tune_params->api_params[device].carrier_mask;

                set_agc_config_param.set_data_cmd = RFWCDMA_MDSP_CONFIG_RxAGC_IN_PROGRESS; 
                set_agc_config_param.rflm_handle = rx_tune_params->api_params[device].rxlm_handle;
                set_agc_config_param.data.gen_b = FALSE;
                (void)rfwcdma_mdsp_async_set_data(&set_agc_config_param);


                /* Step4: Update state machine */

                rfwcdma_data_set_num_carriers(device, rfwcdma_core_count_carriers(rx_tune_params->api_params[device].carrier_mask));

                rfwcdma_data_set_rflm_buf_idx(device,rx_tune_params->api_params[device].rxlm_handle);

                rfwcdma_data_update_rf_state(device,RFWCDMA_STATE_RX_PREP);

	        /*If the device is PRX device, update the associated Tx device info from RFC*/
                if(rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE) 
                {    
	           band_u.wcdma_band = band;
	           sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_IMT_MODE, band_u);
	           pair_tx_dev = rfc_common_get_preferred_tx_device(sys_band);
                   rfwcdma_data_set_associated_device_tx(device, pair_tx_dev);
                }
                
                for (carrier = 0; carrier < RFCOM_MAX_CARRIERS; carrier++)
                {
                  if (rx_tune_params->api_params[device].channel[carrier]!=0) 
                  {
                    rfwcdma_data_set_curr_chan(device,carrier,rx_tune_params->api_params[device].channel[channel_loop++]);
                  }
                }
                  
                num_devices &= (~rf_extract_first_one_bit_mask(num_devices));

             }/*if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))*/

             else
             {
               RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_prep_tune: Invalid device %d", device);
               ret_val = RFM_WCDMA_ERROR_FAILURE;
               break;
             }

          }/*while (num_devices != 0)*/

        }/*if((RFM_DEVICE_0 <= device) && (device < RFM_WCDMA_MAX_DEVICE))*/

        else
        {
          RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_prep_tune: Invalid device %d", device);
          ret_val = RFM_WCDMA_ERROR_FAILURE;
        }
     }/*if(0 != device_mask)*/

     else
     {
       RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_prep_tune: Invalid device mask %d", device_mask);
       ret_val = RFM_WCDMA_ERROR_FAILURE;
     }
  }/*if (RFM_WCDMA_ERROR_FAILURE != ret_val)*/

  /*Destroy local RF buffers*/
  for ( script_type=0; script_type< RFWCDMA_MC_WAKEUP_SCRIPTS_MAX; script_type++)
  {
    rfwcdma_mc_destroy_rf_buff(rf_tune_script_buffer[script_type]);
  }
  return ret_val;
}/*rfwcdma_mc_rx_prep_tune*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Execute Rx Tune sequence to tune to the given band and channel on the passed device(s).
  This routine expects that prep_tune or prep_wakeup routine was called which creates the scripts
  and pushes it to DM.

  @details
  This function will send a command to firmware to stop and restart AGC
  started. The MC state will be updated to reflect the current state.

  @param device_mask
  This is a bitmask passed indicating Rx devices which need to be turned on.

  @param carrier_mask
  Variable indicating the carrier mask on the devices

  @param rxlm_buf_idx
  The Rx Link Manager buffer array that shall be used to configure the
  modem hardware. the array will hold the rxlm handles for only the specified devices

  @return
  Variable indicating the status of API execution. The error codes are
  defined in #rfm_wcdma_error_code_type enumeration.
*/
rfm_api_status_t
rfwcdma_mc_rx_exec_tune
(
  uint32 device_mask,
  uint8 carrier_mask[RFM_MAX_WAN_DEVICES],
  uint32 rxlm_buf_idx[RFM_MAX_WAN_DEVICES]
)
{
  rfm_api_status_t ret_val = RFM_WCDMA_HEALTHY_STATE; /* Return value */
  rf_path_enum_type path = 0;
  timetick_type prof_cal_func;
  uint8 current_mc_state = 0, carrier =0;
  uint32 num_devices =0; 
  rfm_device_enum_type device =0, paired_dev, temp_dev;
  rfwcdma_data_debug_flags_type *dbg_flag; 

  /* Create two arrays for storing the AGC command word. This will save additional loops*/
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_enable_params = {0};
  rfwcdma_core_rxctl_enable_rxagc_params rxagc_disable_params = {0};

  /* Initialize the cmd action table */
  memset(&(rxagc_enable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));
  memset(&(rxagc_disable_params.cmd_action), RFWCDMA_MDSP_AGC_CMD_INVALID, (RFM_WCDMA_MAX_DEVICE * sizeof(rfwcdma_mdsp_agc_action_type)));


  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = 0;     /* Function Profiling Variable */

    prof_cal_func = timetick_get();     /* Get Time tick for ISR Profile */
  }
  
  /* Use local copy of the passed device mask for all the computations */
  num_devices= device_mask;

  /* Loop over all the devices enabled for tune */
  while (num_devices != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

    if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_HIGH, "rfwcdma_mc_rx_exec_tune: Invalid device %d", device);
      return RFM_WCDMA_ERROR_FAILURE;
    }

    if (0 == carrier_mask[device])
    {
      RF_MSG_1(RF_ERROR, "rfwcdma_mc_rx_exec_tune: Carrier mask for device = %d must not be equal to zero", device);
      return RFM_WCDMA_ERROR_FAILURE;
    }

    /* Using the shadow value of multi_carrier_idx always. mc_rx_exec_tune will be called ONLY IF 
    prep_tune is called. This is handled by rfm layer*/

    /* Set the RxAGC enable and disable params */
    rxagc_disable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask_shadow(device);
    rxagc_disable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
    rxagc_disable_params.ant_idx[device] = rfwcdma_core_util_device_to_antenna(device);
    rxagc_disable_params.rxlm_buf_idx[device] = rxlm_buf_idx[device];

    rxagc_enable_params.multi_carrier_idx |= rfwcdma_data_get_carrier_mask(device);
    rxagc_enable_params.cmd_action[device] = RFWCDMA_MDSP_AGC_CMD_ENABLE;
    rxagc_enable_params.ant_idx[device] = rxagc_disable_params.ant_idx[device]; // Antenna Idx for enable is same as disable
    rxagc_enable_params.rxlm_buf_idx[device] = rxlm_buf_idx[device];

    RF_MSG_6(RF_HIGH, "rfwcdma_mc_rx_exec_tune: Parameters - device_mask from rfm %d "
             "current device: %d path: %d, carr_idx shadow: %d, carr_idx: %d"
             "multicarrier_idx: %d",device_mask,device,path,
             carrier_mask[device], rxagc_disable_params.multi_carrier_idx,
             rxagc_enable_params.multi_carrier_idx);

    if(rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE)
    {
      paired_dev = rfwcdma_data_get_associated_device_rx(device);

	  if((paired_dev < RFM_DEVICE_0) || (paired_dev >= RFM_WCDMA_MAX_DEVICE))
	   {
	    RF_MSG_2(RF_HIGH, "rfwcdma_mc_rx_exec_tune: Invalid paired device %d for device %d", paired_dev,device);
           }

	 else
	   {
             rxagc_disable_params.cmd_action[paired_dev] = RFWCDMA_MDSP_AGC_CMD_DISABLE;
	     rxagc_disable_params.ant_idx[paired_dev] = rfwcdma_core_util_device_to_antenna(paired_dev);
	   }
    }

    num_devices &= (~rf_extract_first_one_bit_mask(num_devices));

    #ifdef FEATURE_RF_POWER_REPORTING
    #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
    RF_MSG_2(RF_MED, "rfwcdma_mc.c - rfwcdma_mc_rx_exec_wakeup handle id %d, rx device %d", 
            rxlm_buf_idx[device],
            device);
    #endif
    rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_RX_HANDLE, rxlm_buf_idx[device], RFLM_DIAG_POWER_RFA_STATE_ON);
    #endif
  }/*while (num_devices != 0)*/


  /* Set the remaining RxAGC disable params */
  rxagc_disable_params.rf_warmup_en = FALSE;
  rxagc_disable_params.nbr_en_flag = FALSE;
  rxagc_disable_params.nbr_rxlm_buf_idx = 0;

  /* Send disable AGC cmd to FW */
  rfwcdma_core_rxctl_enable_rxagc(&rxagc_disable_params);

  /* Update State information */
  num_devices= device_mask;

  while (num_devices != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(num_devices);

	  if((device < RFM_DEVICE_0) || (device >= RFM_WCDMA_MAX_DEVICE))
    {
      RF_MSG_1(RF_ERROR, "rfwcdma_mc_rx_exec_tune: Invalid device %d", device);
      return RFM_WCDMA_ERROR_FAILURE;
    }

    path = rfcommon_core_device_to_path(device);

    rfwcdma_data_update_rf_state(device, RFWCDMA_STATE_RX);

    num_devices &= (~rf_extract_first_one_bit_mask(num_devices));

  }/* while (num_devices != 0)*/


  /* FR18317 - NBR */
	/* Load up modG for NBR for PRx only  and overall enable is TRUE */
	dbg_flag = rfwcdma_get_debug_flags();
	
    #ifdef FEATURE_RF_HAS_WTR3925
     (void)rfwcdma_core_rxctl_enable_dc_gain(&rxagc_enable_params);
    #endif
  if( dbg_flag->enable_rf_nbr_operations && enable_nbr_flag &&
     (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE))
	{
	  /* print dbg msg */
	  MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,
			"rfwcdma_mc_rx_exec_tune: W_ NBR DBG: Loading up modG on RxLM index %d for PRx",
			(uint8)nbr_rxlm_buf_idx);
	
    /* Set the remaining RxAGC enable params */
    rxagc_enable_params.rf_warmup_en = TRUE;
    rxagc_enable_params.nbr_en_flag = TRUE;
    rxagc_enable_params.nbr_rxlm_buf_idx = (uint8)nbr_rxlm_buf_idx;

      /* Send enable AGC cmd to FW */
    rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
    }
    else
    {
    /* Set the remaining RxAGC enable params */
    rxagc_enable_params.rf_warmup_en = TRUE;
    rxagc_enable_params.nbr_en_flag = FALSE;
    rxagc_enable_params.nbr_rxlm_buf_idx = (uint8)nbr_rxlm_buf_idx;

      /* Send enable AGC cmd to FW */
    rfwcdma_core_rxctl_enable_rxagc(&rxagc_enable_params);
    }
  

  RF_MSG_3( RF_HIGH, "rfwcdma_mc_rx_exec_tune: Device Mask %d, "
            "Multi_carrier shadow %d, Multi_carrier %d",
            device_mask,
            rxagc_disable_params.multi_carrier_idx,
            rxagc_enable_params.multi_carrier_idx);

  if (rfwcdma_mc_enable_time_profiling)
  {
    prof_cal_func = timetick_get_elapsed(prof_cal_func, T_USEC);
    RF_MSG_1(RF_HIGH, "RFWCDMA Execution Time: Exec_tune - %d", prof_cal_func);
  }

  /* Return the API Wait time */
  return ret_val;

} /* rfwcdma_mc_rx_exec_tune*/

/*! @cond vbhide */
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Config the ASM statue bit map rfwcdma_mc_state.asm_status_bit_map[device].

  @details
  This function will set the asm status bit map according to different devices.

  @param device
  The device number indicate the device ID.

  @param band
  The band indication

  @on_off
  Indicate turn the correaponding ASM or nor

  @return
  Variable indicating the status of API execution
*/
boolean
rfwcdma_mc_config_asm_status_bit_map
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type band,
  boolean on_off
)
{
  boolean api_status = TRUE;
  
  if((device >= RFM_DEVICE_0) && (device < RFM_MAX_WAN_DEVICES) && 
    (band >= RFCOM_BAND_IMT) && (band < RFCOM_NUM_WCDMA_BANDS))
  {
    if(TRUE == on_off)
    {
      rfwcdma_mc_state.asm_status_bit_map[device] |= (1 << band);
    }
    else
    {
      rfwcdma_mc_state.asm_status_bit_map[device] &= (~(1 << band));
    }

    RF_MSG_4(RF_HIGH, "ASM Status: device %d band %d on_off %d status 0x%x",
                      device, band, on_off, rfwcdma_mc_state.asm_status_bit_map[device]);
  }
  else
  {
    api_status = FALSE;
    RF_MSG_3(RF_HIGH, "ASM Status: Invalid param - device %d band %d status 0x%x",
                      device, band, rfwcdma_mc_state.asm_status_bit_map[device]);
  }

  return api_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get the ASM statue bit map.

  @details
  This function will set the asm status bit map according to different devices.

  @param device
  The device number indicate the device ID.

  @param band
  The band indication

  @on_off
  Indicate turn the correaponding ASM or nor

  @return
  Variable indicating the status of API execution.
*/
boolean
rfwcdma_mc_get_asm_status_bit
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type band
)
{
  boolean ret_val;
  
  if((device >= RFM_DEVICE_0) && (device < RFM_MAX_WAN_DEVICES) && 
    (band >= RFCOM_BAND_IMT) && (band < RFCOM_NUM_WCDMA_BANDS))
  {
    if(rfwcdma_mc_state.asm_status_bit_map[device] & (1 << band))
    {
      ret_val = TRUE;
    }
    else
    {
      ret_val = FALSE;
    }
  }
  else
  {
    ret_val = FALSE;
  }

  return ret_val;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Config the ASM statue bit map rfwcdma_mc_state.tuner_status_bit_map[device]..

  @details
  This function will set the asm status bit map according to different devices.

  @param device
  The device number indicate the device ID.

  @param band
  The band indication

  @on_off
  Indicate turn the correaponding TUNER or nor

  @return
  Variable indicating the status of API execution.
*/
boolean
rfwcdma_mc_config_tuner_status_bit_map
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type band,
  boolean on_off
)
{
  boolean api_status = TRUE;
  
  if((device >= RFM_DEVICE_0) && (device < RFM_MAX_WAN_DEVICES) && 
    (band >= RFCOM_BAND_IMT) && (band < RFCOM_NUM_WCDMA_BANDS))
  {
    if(TRUE == on_off)
    {
      rfwcdma_mc_state.tuner_status_bit_map[device] |= (1 << band);
    }
    else
    {
      rfwcdma_mc_state.tuner_status_bit_map[device] &= (~(1 << band));
    }

    //RF_MSG_4(RF_HIGH, "TUNER Status: device %d band %d on_off %d status 0x%x",
    //                  device, band, on_off, rfwcdma_mc_state.tuner_status_bit_map[device]);
  }
  else
  {
    api_status = FALSE;
    RF_MSG_3(RF_HIGH, "TUNER Status: Invalid param - device %d band %d status 0x%x",
                      device, band, rfwcdma_mc_state.tuner_status_bit_map[device]);
  }

  return api_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get the TUNER statue bit map.

  @details
  This function will set the tuner status bit map according to different devices.

  @param device
  The device number indicate the device ID.

  @param band
  The band indication

  @on_off
  Indicate turn the correaponding TUNER or nor

  @return
  Variable indicating the status of API execution.
*/
boolean
rfwcdma_mc_get_tuner_status_bit
(
  rfcom_device_enum_type device,
  rfcom_wcdma_band_type band
)
{
  boolean ret_val;
  
  if((device >= RFM_DEVICE_0) && (device < RFM_MAX_WAN_DEVICES) && 
    (band >= RFCOM_BAND_IMT) && (band < RFCOM_NUM_WCDMA_BANDS))
  {
    if(rfwcdma_mc_state.tuner_status_bit_map[device] & (1 << band))
    {
      ret_val = TRUE;
    }
    else
    {
      ret_val = FALSE;
    }
  }
  else
  {
    ret_val = FALSE;
  }

  return ret_val;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get the band info from MC state machine for that particular RFM device

  @details
  Get the band info from MC state machine for that particular RFM device

  @param device
  The device number indicate the device ID.

  @return source_band
  Band info from MC state machine
  
*/
rfcom_wcdma_band_type
rfwcdma_mc_get_source_band
(
  rfcom_device_enum_type device
)
{
  rfcom_wcdma_band_type source_band = RFCOM_BAND_INVALID;
  source_band = rfwcdma_data_get_curr_band(device);
    
  return source_band;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This funcion performs one time initializaion for all devices during enter_mode
  Internally called by rfwcdma_mc_enter_mode

  @details
  This function keeps a voting on all devices being entered and will perform the
  one-time initialization when the first device is being entered.
  This function will not perform HW operation.

  @param device
  The device number indicate the device ID.

  @return boolean
  True if Success
  
*/
boolean
rfwcdma_mc_common_enter
(
  rfm_device_enum_type device
)
{
  boolean api_status = TRUE;

  /* Initialize MDSP and TQ interface only when Meas Enter Handle Mask is zero*/
  if(rfwcdma_data_get_meas_enter_exit_handle_mask() == 0)
  {
    /* Legacy MDSP layer init - need to be cleaned up */
    rfwcdma_mdsp_init();

    /* Initialize MDSP module if it has not been done */
    api_status &= rfwcdma_mdsp_init_once();

    /* Initialize FED CCS module */
    rfwcdma_mdsp_sync_config_ccs(TRUE);
  }

    /* Initialize temp comp data settings */
    rfwcdma_core_temp_comp_data_init();

  return api_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This funcion performs one time clean up for all devices during exit_mode
  Internally called by rfwcdma_mc_enter_mode

  @details
  This function keeps a voting on all devices being exit and will perform the
  one-time cleanup when the last device is being entered.
  This function will not perform HW operation
 
  @param device
  The device number indicate the device ID.

  @return boolean
  True if Success
  
*/
boolean
rfwcdma_mc_common_exit
(
  rfm_device_enum_type device
)
{
  boolean api_status = TRUE;
  rfcommon_fbrx_error_type fbrx_status;
 
  fbrx_status = rfcommon_fbrx_mc_exit(rfwcdma_mc_fbrx_lm_handle, RFCOM_WCDMA_MODE);

  if(fbrx_status == RFCOMMON_FBRX_ERROR)
  {
    RF_MSG(RF_FATAL, "rfcommon_fbrx_mc_exit FBRx DM buffer deallocation failed");
    api_status = FALSE;
  }

  // reset wcdma data module only on the last active device
  rfwcdma_mc_reset_data();

  /* Reset callback data for all devices */
  rfwcdma_core_rxctl_reset_dynamic_notch_cb_data();

  #ifdef FEATURE_JOLOKIA_MODEM
  api_status &= rfwcdma_mdsp_ccs_tq_pair_disassociate_device();
  #endif

  /* Clean up MDSP module*/
  rfwcdma_mdsp_exit();

  RF_MSG_2(RF_HIGH, "rfwcdma_mc_common_exit on device:%d, status:%d ",device, api_status );
     
  return api_status;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This API will power on/initialize or power off/deinitialize
  all HW blocks required for RF WCDMA operation
 
  @details
  This function is the successor of the legacy rfwcdma_mc_power_clock_on() and
  rfwcdma_mc_power_clock_off() which now need to device as parameter
  This function also replace all the HW initializaion in rfm_enter_mode/rfm_exit_mode,
  given the requrement of enter_mode and prep_tune will be called ahead of time, and power
  clock on will be called before the actual exec_tune and exec_wakeup
   
 
  @param device
  rfm device 

  @retval
  RFM API status

*/
boolean
rfwcdma_mc_power_on_off
(
  rfm_wcdma_power_mgmt_param *pwr_param,
  rfm_wcdma_device_type dev_type
)
{
  boolean wcdma_device_vote = FALSE;
  boolean api_status = TRUE;
  rfm_mode_enum_type mode =0;
  rfm_device_enum_type device = pwr_param->device;
  rfm_device_enum_type prx_device = RFM_INVALID_DEVICE;
  rf_lock_data_type* power_on_off_crit_sec = NULL;

  if((device < RFM_DEVICE_0) || (device >=RFM_MAX_WAN_DEVICES))
	 {
	   RF_MSG_1(RF_ERROR, "rfwcdma_mc_power_on_off: Invalid device %d", device);
	   return FALSE ;
	 }

  /*Check to make sure multiple power on request for the same device is ignored*/
  if(((rfwcdma_data_get_rx_power_mask() | (1<<pwr_param->device)) ==  rfwcdma_data_get_rx_power_mask()) && (pwr_param->on_off))
  {
     return TRUE;
  }

  if (dev_type == RFM_WCDMA_INVALID_DEVICE)
  {
    	   RF_MSG_1(RF_ERROR, "rfwcdma_mc_power_on_off: Invalid dev_type %d", dev_type); 
           return FALSE ;
  }

  RF_MSG_2(RF_HIGH, "rfwcdma_mc_power_on_off: dev %d and sub_id %d", device, pwr_param->sub_type);

/*Extract the Sub ID passed and map it to the right IMT mode*/

  if (pwr_param->sub_type != RFM_WCDMA_INVALID_SUB)
  {
    if(pwr_param->sub_type == RFM_WCDMA_SUB_0)
    {
      mode = RFM_IMT_MODE;
    }
    else if (pwr_param->sub_type == RFM_WCDMA_SUB_1) 
    {
      mode = RFM_IMT_MODE_2;
    }
  }
  else 
  {
   RF_MSG_2(RF_ERROR, "rfwcdma_mc_power_on_off: Invalid Sub_id %d on dev %d",pwr_param->sub_type, device );
    return FALSE;
  }

  power_on_off_crit_sec = rfwcdma_get_power_on_off_lock_data();

  /* Enter Critical Section for Power On/Off */
  rf_common_enter_critical_section(power_on_off_crit_sec);

  if (dev_type== RFM_WCDMA_TX_DEVICE) 
  {
    prx_device = rfwcdma_data_get_associated_device_prx_from_tx(device);
    
    if(prx_device >= RFM_MAX_WAN_DEVICES)
    {
      RF_MSG_1( RF_ERROR, "rfwcdma_mc_power_on_off: Invalid PRx device mapped %d ; Use RFM_DEVICE_0 instead",prx_device );
      prx_device = RFM_DEVICE_0;
    }
  }

  if (pwr_param->on_off)
  {
    RF_MSG_2(RF_HIGH, "Calling rfwcdma_mc_power_on_off: dev %d and sub_id %d", device, pwr_param->sub_type);
    /* Update the Subscription type in RFWCDMA data only in Power ON */
    rfwcdma_data_update_subscription_type(device, pwr_param->sub_type);

    /* If both standalone and measurement power on/off masks are zero, Enable HAL Bus and Common RFC voting*/
    if ( (rfwcdma_data_get_power_mask() == 0) &&
         (rfwcdma_data_get_meas_power_on_off_dev_mask() == 0)
        ) 
    {
      rf_hal_bus_enable(TRUE, &rf_wcdma_hal_bus_client_handle, RF_HAL_RFCMD_APP_CLIENT(WCDMA));
      /* Initialize all GPIOs */
      rfc_common_wakeup(RF_PATH_0, RFCOM_WCDMA_MODE);
      rfc_common_wakeup(RF_PATH_1, RFCOM_WCDMA_MODE);
    }

    if (dev_type == RFM_WCDMA_TX_DEVICE) 
    {
      /* We are not using Tx device for voting to dev manager as disable vote for Tx device is 
         not handled in vreg manager. Hence using the PRx device and passing with RxTx state
         to make sure DAC MPP is turned on for Tx state. 
       */
      rfc_wcdma_manage_vregs(prx_device, RFCOM_BAND_IMT, RF_PATH_RXTX_STATE, mode);
    }
    else
    {
      rfc_wcdma_manage_vregs(device, RFCOM_BAND_IMT, RF_PATH_RX_STATE, mode);
    }
     /* vote to enable RFCMD app */
  

    /* WTR device power vote */
    wcdma_device_vote = rfcommon_core_dev_power_vote(device,RFCMN_CORE_DEV_ON, RFCMN_CORE_DEV_NORMAL_VOTE);

    if (wcdma_device_vote == FALSE)
    {
      RF_MSG_1(RF_FATAL,"rfwcdma_mc_power_on_off - Device - %d Power up failed",device);
      api_status &= FALSE;
    }
	
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      if (dev_type == RFM_WCDMA_TX_DEVICE) 
      {
        /* This has to be called after common core vote */
       /* Call WTR wakeup for the current band for the device 
             Remember this is immediate action */
        api_status = rfdevice_wcdma_tx_pwr_wakeup(device);
        if (api_status == FALSE)
        {
          RF_MSG_1(RF_ERROR, "rfdevice_wcdma_tx_pwr_wakeup - Device - %d Power wakeup failed", device);

          /* Leave Critical Section for Power on/off */
          rf_common_leave_critical_section(power_on_off_crit_sec);
          return RFM_WCDMA_ERROR_FAILURE;
        }
      }
      else
      {
        /* This has to be called after common core vote */
        /* Call WTR wakeup for the current band for the device 
            Remember this is immediate action */
        api_status = rfdevice_wcdma_rx_pwr_wakeup(device);
        if (api_status == FALSE)
        {
          RF_MSG_1(RF_ERROR, "rfdevice_wcdma_rx_pwr_wakeup - Device - %d Power wakeup failed", device);

          /* Leave Critical Section for Power on/off */
          rf_common_leave_critical_section(power_on_off_crit_sec);
          return RFM_WCDMA_ERROR_FAILURE;
        }
      }
    } 

    /* Config RFC WCDMA mode: Program the GPIO/GRFC/RF_CTL signals for the WCDMA mode */
    rfc_wcdma_mode_config(device);

    /* Update state mcahine */
    rfwcdma_data_update_power_flag(device, TRUE);
  }
  else
  {	 
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      if (dev_type == RFM_WCDMA_TX_DEVICE) 
      {
        /* This has to be called after common core vote */
        /* Call WTR wakeup for the current band for the device 
           Remember this is immediate action */
        api_status = rfdevice_wcdma_tx_pwr_sleep(device);
        if (api_status == FALSE)
        {
          RF_MSG_1(RF_ERROR, "rfdevice_wcdma_tx_pwr_sleep - Device - %d Power sleep failed", device);

          /* Leave Critical Section for Power on/off */
          rf_common_leave_critical_section(power_on_off_crit_sec);
          return RFM_WCDMA_ERROR_FAILURE;
        }
      }
      else
      {
        /* This has to be called after common core vote */
        /* Call WTR wakeup for the current band for the device 
           Remember this is immediate action */
        api_status = rfdevice_wcdma_rx_pwr_sleep(device);
        if (api_status == FALSE)
        {
          RF_MSG_1(RF_ERROR, "rfdevice_wcdma_rx_pwr_sleep - Device - %d Power sleep failed", device);

          /* Leave Critical Section for Power on/off */
          rf_common_leave_critical_section(power_on_off_crit_sec);
          return RFM_WCDMA_ERROR_FAILURE;
        }
      }
    }
    /* WTR device power vote off */
    wcdma_device_vote = rfcommon_core_dev_power_vote(device,
                        RFCMN_CORE_DEV_OFF,
                        RFCMN_CORE_DEV_NO_VOTE);

    if (wcdma_device_vote == FALSE)
    {
      RF_MSG_1(RF_HIGH,"rfwcdma_mc_power_off on device;%d, device already powered off", device);
    }

    if (dev_type == RFM_WCDMA_TX_DEVICE) 
    {
      /* Updating the corresponding PRx device from Rxtx to Rx state */
      rfc_wcdma_manage_vregs(prx_device, RFCOM_BAND_IMT, RF_PATH_RX_STATE, mode);
    }

    else
    {
       /* VREG off */
       rfc_wcdma_manage_vregs(device, RFCOM_BAND_IMT, RF_PATH_SLEEP_STATE, mode);
    }

    /* Update state machine */
    rfwcdma_data_update_power_flag(device, FALSE);
    
    /* If both standalone and measurement power on/off masks are zero, Enable HAL Bus and Common RFC voting*/
    if ( (rfwcdma_data_get_power_mask() == 0) &&
         (rfwcdma_data_get_meas_power_on_off_dev_mask() == 0)
        ) 
    {
      /* Deinitialize all GPIOs */
      rfc_common_sleep(RF_PATH_0, RFCOM_WCDMA_MODE);
      rfc_common_sleep(RF_PATH_1, RFCOM_WCDMA_MODE);

      /* vote to disable RFCMD app */
      rf_hal_bus_enable(FALSE, &rf_wcdma_hal_bus_client_handle, RF_HAL_RFCMD_APP_CLIENT(WCDMA));
    }    
  }

  RF_MSG_3(RF_HIGH, "rfwcdma_mc_power_on_off on device:%d,on_off:%d status:%d ",device, pwr_param->on_off, api_status );

  /* Leave Critical Section for Power on/off */
  rf_common_leave_critical_section(power_on_off_crit_sec);

  return api_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  query function to provide Tx/Rx frequency ratio based on channel

  @details
*/
int32 rfwcdma_mc_tx_rx_freq_ratio(uint16 dl_channel)
{
  uint64 tx_freq, rx_freq = 0;
  uint32 tx_rx_ratio;
  rfcom_wcdma_band_type band;
  uint16 ul_channel;

  band = rfwcdma_core_convert_chan_to_band(dl_channel);
  rx_freq= rfwcdma_core_get_freq_from_uarfcn_no_limit(band,dl_channel,RFWCDMA_CORE_UARFCN_DL);
  RF_MSG_3( RF_HIGH, "rfwcdma_mc_tx_rx_freq_ratio: for channel:%d rx_freq: %d band is: %d",
              dl_channel,rx_freq, band);


  ul_channel=rfwcdma_core_map_rx_to_tx_chan( dl_channel, band );
  tx_freq = rfwcdma_core_get_freq_from_uarfcn_no_limit(band,ul_channel,RFWCDMA_CORE_UARFCN_UL);
  RF_MSG_3( RF_HIGH, "rfwcdma_mc_tx_rx_freq_ratio: for ul_channel:%d, tx_freq: %d, band is: %d",
              ul_channel,tx_freq,band);
  
  tx_rx_ratio =(uint32)((tx_freq << 15) / rx_freq);

  RF_MSG_4( RF_HIGH, "rfwcdma_mc_tx_rx_freq_ratio: for channel:%d tx_freq:%d, rx_freq: %d tx_rx_freq_ratio is: %d",
              dl_channel, tx_freq, rx_freq, tx_rx_ratio);
  return tx_rx_ratio ;
}

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Funtion to do RF WCDMA data module cleanup for the RFLM handle rquested
 
  @details
  This is usually called just after the RFLM handle is de-allocated. Handle is
  de-allcoated by WL1, so WL1 has to be the one calling this handle based
  MDSP data cleanup
 
  @param rflm_handle
  RFLM handle for which MDSP data cleanup needs to be done
 
  @return
  TRUE if cleanup is successful
*/
boolean
rfwcdma_mc_handle_cleanup
(
  lm_handle_type rflm_handle
)
{
  boolean api_status = FALSE;
  rf_lock_data_type* handle_mdsp_mc_meas_crit_sec = NULL;

  /* Enter Critical Section for MDSP RFLM handle */
  handle_mdsp_mc_meas_crit_sec = rfwcdma_data_get_meas_mc_mdsp_crit_sec_data();

  if (handle_mdsp_mc_meas_crit_sec == NULL) 
  {
    RF_MSG(RF_ERROR,"rfwcdma_mc_handle_cleanup: MC_MEAS Critical section data is NULL in RFWCDMA data");
    return FALSE;
  }
  else
  {
    rf_common_enter_critical_section(handle_mdsp_mc_meas_crit_sec);
  }

  api_status = rfwcdma_mdsp_data_handle_cleanup((rflm_dm_handle_id_t)rflm_handle);

  /* Leave Critical Section for Meas MC TQ MDSP */
  rf_common_leave_critical_section(handle_mdsp_mc_meas_crit_sec);

  return api_status;
}
/*----------------------------------------------------------------------------*/

#ifdef FEATURE_RF_ASDIV


/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

/*! 
  @brief
  Common callback API for AsDiv
  This API is called after a successful AsDiv switch happens.

  @details  
  
  @param data
  
  @param ant_position
  
*/
 void rfwcdma_mc_asdiv_common_callback(void *data, uint32 *ant_position)
 {
   /*FR27546 Update MTPL based on SAR AsDiv value*/
   rfwcdma_core_txplim_set_sar_limit(rfm_common_sar_get_state());

   /*FR30081, eLNA AsDiv handler to update alternate gain states*/
   rfwcdma_mc_asdiv_elna_callback_handler(data,ant_position);

   rfwcdma_mc_asdiv_qat_callback_handler(ant_position);
 }

  
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

/*! 
  @brief
  Funtion to reprogram Rx gain scripts and Tx offsets after AsDiv switch with eLNA for FR30081.
 
  @details
  This is callback API which called when an AsDiv switch happens.
  Freeze LNA updates to FED.
  Re-built Rx gain scripts based on the gain range and offsets
  Update TxAGC offset 
  
  @param data
  
  @param ant_position
  
*/

void rfwcdma_mc_asdiv_elna_callback_handler(void *data, uint32 *ant_position )
{
  int8 asdiv_pos = (int8)*ant_position;

  rfm_device_enum_type device;
  uint32 dev_mask;
  rfm_device_enum_type tx_device;  
  rfwcdma_data_rf_state_type tx_state;
  rfwcdma_mdsp_set_data_param_type set_agc_config_param = { 0 };
  uint8 rxlm_handle = 0;
  int16 rx_gain_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES] = {0};
  int16 lna_rise_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES-1]= {0}; 
  int16 lna_fall_asdiv_offset[RF_WDMA_MAX_NUM_LNA_STATES-1] = {0};
  rfcom_wcdma_band_type band = RFCOM_BAND_INVALID;  
  rfwcdma_nv_rx_asdiv_elna_gain_offset_type *rx_asdiv_offset_ptr = NULL;
  rfwcdma_nv_rx_asdiv_elna_switchpoint_type *rx_asdiv_switchpoint_ptr =NULL;
  rfc_cfg_params_type cfg;
  uint32 nv_container = 0;
  uint32 tx_nv_container = 0;  
  rfc_device_info_type *device_info_ptr;
  uint32 alt_path = 0;
  int8 lna_state = 0;
  uint32 multi_carrier_idx = 0;
  uint16 rx_channels[RFCOM_MAX_CARRIERS] = {0};
  int8 carr_loop = 0;  
  rfwcdma_mdsp_set_data_param_type tx_param;
  uint8 txlm_handle = 0;
  boolean api_status = TRUE;
  boolean sub_api_status = FALSE;
  rf_time_type profile_time; /* Profile Time */
  rf_time_tick_type profile_tick; /* Profile Tick */
  rfdevice_wcdma_rx_tune_param_type rx_params = {0};
  uint8 num_chan = 0;
  boolean asdiv_alt_gain = FALSE;
  
  /* Start Profiling of API */
  profile_tick = rf_time_get_tick();
  
  /*-----Sanity checks and initializations--------------------------------------------------------*/
  
  if (asdiv_pos < ASDIV_POSITION_0 || asdiv_pos >= ASDIV_POSITION_MAX)
   {
	 RF_MSG_1(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: Invalid AsDiv position: %d",asdiv_pos);
	 return;
   }

  if(asdiv_pos == ASDIV_POSITION_1)
  {
    asdiv_alt_gain = TRUE;
  }
  
  /* Get the device mask of all devices in tune or pretune */
  dev_mask = rfwcdma_data_get_rx_tune_device_mask();

  if(dev_mask == 0)
  {
    RF_MSG_1(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: skip FR 30081, no WCDMA devices tuned, dev_mask: %d",dev_mask);
    return;
  }
  else
  {
    RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: START device_mask: %d, asdiv_pos: %d",dev_mask, asdiv_pos);
  } 

    /* Loop over all the devices enabled for tune */
  while (dev_mask != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(dev_mask);

    if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))
    { 
      band = rfwcdma_data_get_curr_band(device);

      if (band >= RFCOM_NUM_WCDMA_BANDS)  
      {
        RF_MSG_1(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: Invalid band: %d",band);
	    break;
      }

      multi_carrier_idx = rfwcdma_data_get_carrier_mask(device);

	  num_chan = rfwcdma_core_count_carriers(multi_carrier_idx);
	  
      rxlm_handle = rfwcdma_data_get_handle(device);

      if (!IS_VALID_RXLM_HANDLE(rxlm_handle))
      {
        RF_MSG_1(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: Invalid rxlm_handle: %d",rxlm_handle);
        break;
      }

      alt_path = rfwcdma_data_get_alt_path(device);
  
      cfg.alternate_path = alt_path;
      cfg.band = band;
      cfg.logical_device = device;
      cfg.rx_tx = RFC_CONFIG_RX;
      cfg.req = RFC_REQ_DEFAULT_GET_DATA;

      if (rfc_wcdma_data_get_device_info(&cfg, &device_info_ptr) == FALSE)
      {	 
        RF_MSG_2( RF_ERROR, "rfwcdma_mc_asdiv_elna_callback_handler: - Cannot get device info for"
						 "device %d Div config, band %d",device,band);
	    break;
      }
  
      if (device_info_ptr->nv_container < RFM_MAX_WAN_DEVICES)
      {
	    nv_container = device_info_ptr->nv_container; 
      }

      for(carr_loop = 0; carr_loop < RFCOM_MAX_CARRIERS ; carr_loop++)
      {
        rx_channels[carr_loop] = rfwcdma_data_get_curr_chan(device,(rfcom_multi_carrier_hspa_id_type)carr_loop);
      }
   
      /*-----  Load AsDiv offset values------------------------------------------------------*/
      rx_asdiv_offset_ptr = rfnv_wcdma_tbl_ptr[band]->rx_static_data.payload.rx_stat_v2.rx_asdiv_elna_offset_ptr[nv_container][asdiv_pos];
      rx_asdiv_switchpoint_ptr = rfnv_wcdma_tbl_ptr[band]->rx_static_data.payload.rx_stat_v2.rx_asdiv_elna_switchpoint_ptr[nv_container][asdiv_pos];

      if(rx_asdiv_offset_ptr != NULL)
      {         
     
        for(lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES; lna_state++)
        {
	      rx_gain_asdiv_offset[lna_state] = rx_asdiv_offset_ptr->rx_gain_offset[lna_state];
        }
		
        if(rx_asdiv_switchpoint_ptr != NULL)
        {
          for(lna_state = 0; lna_state < RF_WDMA_MAX_NUM_LNA_STATES-1; lna_state++)
          {
	        lna_rise_asdiv_offset[lna_state] = rx_asdiv_switchpoint_ptr->lna_rise_fall[lna_state].lna_rise;
	        lna_fall_asdiv_offset[lna_state] = rx_asdiv_switchpoint_ptr->lna_rise_fall[lna_state].lna_fall;
          }
    
  
         /*------STEP 1 : FREEZE LNA Gain updates to FED/ RxAGC-------------------------------- */
          set_agc_config_param.set_data_cmd = RFWCDMA_MDSP_CONFIG_RxAGC_IN_PROGRESS; 
          set_agc_config_param.rflm_handle = rxlm_handle;
          set_agc_config_param.data.gen_b = TRUE;
          sub_api_status = rfwcdma_mdsp_async_set_data(&set_agc_config_param);
          api_status &= sub_api_status;
		  
		  RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: Freeze LNA gain updates device: %d status: %d",device,sub_api_status);
  
         /*------STEP 2 : Reload RxAGC to WXE ------------------------------------------------*/
          rx_params.band = band;

		  rx_params.paired_device = rfwcdma_data_get_associated_device_rx(device); 
		  
		  if (rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE) 
          {
            rx_params.rx_mode = RFDEVICE_PRX_OPER_MODE;
          }
          else if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_DIVERSITY_RX_DEVICE) 
          {
            rx_params.rx_mode = RFDEVICE_DRX_OPER_MODE;
          }
          else
          {
            rx_params.rx_mode = RFDEVICE_INVALID_OPER_MODE;
          }         

          if (num_chan == 0)
          {
            rx_params.bw   = RFDEV_WCDMA_BW_INVALID;
            rx_params.channel[0] = rx_channels[0];
          }
          else if (num_chan == 1)
          {
            rx_params.bw   = RFDEV_WCDMA_BW_2X;
            rx_params.channel[0] = rx_channels[0];
            rx_params.channel[1] = rx_channels[1];

          }
          else if (num_chan ==2 )
          {
            rx_params.bw   = RFDEV_WCDMA_BW_3X;
            rx_params.channel[0] = rx_channels[0];
            rx_params.channel[1] = rx_channels[1];
            rx_params.channel[2] = rx_channels[2];

          }
		  
		  rfdevice_wcdma_rx_get_agc_reload_script(device,
		  	                                      RFM_IMT_MODE,
		  	                                      band,
		  	                                      rx_params,
		  	                                      RFDEVICE_EXECUTE_IMMEDIATE,
		  	                                      NULL,
		  	                                      asdiv_alt_gain);
		 
		 /*----- STEP 3: Config Rx band--------------------------------------------------------*/

          sub_api_status = rfwcdma_core_rxctl_config_band((uint32)rxlm_handle,
                                                          device,
                                                          band,
                                                          multi_carrier_idx,
                                                          RFLM_WCDMA_HOME_MEAS,
                                                          alt_path,
                                                          MAX_RX_DEV_HANDLE_ITEM);
          api_status &= sub_api_status;
		  
          RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: config rx band device: %d status: %d",device,sub_api_status);                                            

        /*----- STEP 4: Config Rx channel----------------------------------------------------- */
          sub_api_status = rfwcdma_core_rxctl_config_chan((uint32)rxlm_handle,
		         								 	      device,
											              band,
											              rx_channels,
											              multi_carrier_idx,
											              RFLM_WCDMA_HOME_MEAS,
											              alt_path,
											              MAX_RX_DEV_HANDLE_ITEM); 
		  api_status &= sub_api_status;
		  
		  RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: config rx channel device: %d status: %d",device,sub_api_status);
  
        /*------STEP 5 : UN-FREEZE LNA Gain updates to FED/ RxAGC------------------------------*/
          set_agc_config_param.set_data_cmd = RFWCDMA_MDSP_CONFIG_RxAGC_IN_PROGRESS; 
          set_agc_config_param.rflm_handle = rxlm_handle;
          set_agc_config_param.data.gen_b = FALSE;
          sub_api_status = rfwcdma_mdsp_async_set_data(&set_agc_config_param);        
		  api_status &= sub_api_status;
		  
		  RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: Un-freeze LNA gain updates device: %d status: %d",device,sub_api_status);
         }
	     else
	     {
	        RF_MSG_2(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: asdiv switchpoints are NULL for "
	 	                  "NV container: %d asdiv pos: %d",nv_container,asdiv_pos);	 
	     }
       }
       else
       {
         RF_MSG_2(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: asdiv gain offsets are NULL for "
	 	                 "NV container: %d asdiv pos: %d",nv_container,asdiv_pos);	 
       }
     }
     else
     {
       RF_MSG_1(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: Invalid device: %d", device);
	   return;
     }
   
     dev_mask &= (~rf_extract_first_one_bit_mask(dev_mask));
   }
  
   /*----- STEP 6: Config Tx data--------------------------------------------------------*/

   tx_device = rfwcdma_data_get_tx_device();   
   
   if(tx_device >= RFM_DEVICE_0 && tx_device < RFM_MAX_WAN_DEVICES)
   {
    tx_state = rfwcdma_data_get_rf_state(tx_device);

	if (tx_state == RFWCDMA_STATE_TX_PREP || tx_state == RFWCDMA_STATE_TX)
	{
	  RF_MSG_3(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: Config Tx, enabled on " 
	  	               "device: %d, tx_state: %d, nv_container: %d",tx_device,tx_state,tx_nv_container);

	  txlm_handle = rfwcdma_data_get_handle(tx_device);

      if (!IS_VALID_TXLM_HANDLE(txlm_handle))
      {
        RF_MSG_1(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: Invalid txlm_handle: %d",rxlm_handle);
      }
	  else
	  {
        band = rfwcdma_data_get_curr_band(tx_device);

        if (band >= RFCOM_NUM_WCDMA_BANDS)  
        {
          RF_MSG_1(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: Invalid band: %d",band);	          
        }
        else
	    {	  
	      tx_param.rflm_handle = txlm_handle;
		  tx_param.set_data_cmd = RFWCDMA_MDSP_SET_ASDIV_TX_GAIN_OFFSET;
		  tx_param.data.asdiv_tx_offset.offset = 
			  rfnv_wcdma_tbl_ptr[band]->tx_static_data.payload.tx_stat_v1.tx_asdiv_elna_offset[tx_nv_container][asdiv_pos];
	      if(rfwcdma_mdsp_async_set_data(&tx_param) == TRUE)
	      {
	      
            RF_MSG(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: mdsp set Tx offset success");		  
	      }
		  else
		  {
     	    RF_MSG(RF_ERROR,"rfwcdma_mc_asdiv_elna_callback_handler: mdsp set Tx offset Failed");
		    api_status = FALSE;
		  }
		}
	  }
	}
   }  

  /* get time spent in API */
  profile_time = rf_time_get_elapsed( profile_tick, RF_USEC );
   
  RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_elna_callback_handler: Complete status: %d, Time elapsed: %d us",api_status,profile_time);		  
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

/*! 
  @brief
  Funtion to update Tx offsets after AsDiv switch using QAT3522.
 
  @details
  This is callback API which called when an AsDiv switch happens.
  Update TxAGC offset 

  @param ant_position
  ant position post ASDiv switch
*/

void rfwcdma_mc_asdiv_qat_callback_handler( uint32 *ant_position )
{
  uint8 asdiv_pos = (uint8)*ant_position;
  rfwcdma_mdsp_set_data_param_type tx_param = {0};
  uint32 dev_mask = 0;
  rfm_device_enum_type device;
  rfm_device_enum_type tx_device;
  rfcom_wcdma_band_type band = RFCOM_BAND_INVALID;
  uint8 txlm_handle = 0;
  rfwcdma_data_rf_state_type tx_state;
  
  dev_mask = rfwcdma_data_get_rx_tune_device_mask();

  if(dev_mask == 0)
  {
    RF_MSG_1(RF_HIGH,"rfwcdma_mc_asdiv_qat_callback_handler:no WCDMA devices tuned, dev_mask: %d",dev_mask);
    return;
  }
  else
  {
    RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_qat_callback_handler:START device_mask: %d, asdiv_pos: %d",dev_mask, asdiv_pos);
	device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(dev_mask);

    if((RFM_DEVICE_0 <= device) && (device < RFM_MAX_WAN_DEVICES))
    { 
      band = rfwcdma_data_get_curr_band(device);
	  if( band == RFCOM_BAND_INVALID )
	  {
		  RF_MSG_1(RF_HIGH,"rfwcdma_mc_asdiv_qat_callback_handler: invalid band for dev %d",device);
		  return;
	  }
	}
  } 
  
  tx_device = rfwcdma_data_get_associated_device_tx(device);
  
  if(tx_device >= RFM_DEVICE_0 && tx_device < RFM_MAX_WAN_DEVICES)
  {
    tx_state = rfwcdma_data_get_rf_state(tx_device);
       
    if (tx_state == RFWCDMA_STATE_TX_PREP || tx_state == RFWCDMA_STATE_TX)
    {
      RF_MSG_2(RF_HIGH,"rfwcdma_mc_asdiv_qat_callback_handler: Config Tx, enabled on " 
      	               "device: %d, tx_state: %d",tx_device,tx_state);
       
      txlm_handle = rfwcdma_data_get_handle(tx_device);
    }
  }
  
  tx_param.rflm_handle = txlm_handle;
  tx_param.set_data_cmd = RFWCDMA_MDSP_APPEND_ASDIV_QAT_TX_GAIN_OFFSET;
  tx_param.data.asdiv_tx_offset.offset = rfcommon_nv_get_asdiv_offset_data(RFM_IMT_MODE, (rfcom_band_type_u)band, asdiv_pos);
  if(rfwcdma_mdsp_async_set_data(&tx_param) == TRUE)
  {
    RF_MSG_1(RF_HIGH,"rfwcdma_mc_asdiv_qat_callback_handler: mdsp set Tx offset success with offset %d",tx_param.data.asdiv_tx_offset.offset);  
  }
  else
  {
    RF_MSG(RF_ERROR,"rfwcdma_mc_asdiv_qat_callback_handler: mdsp set Tx offset Failed");
  }
  
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif

/*
  @brief
  This API will return the forced antenna position based on DSI index and SAR value.

  @details
  RF_SAR_ASDIV_FORCE_OTHER_ANTENNA(0xFFFE) SAR value for an AsDiv position indicates the position is disabled.
  Force L1 to use the other antenna position. 
   
  0xFFFE value will not effect the MTPL calculations. This high limit value will be factored out during MIN determination.

  @return forced antenna position
*/
rfwcdma_mc_sar_forced_antenna_enum_type rfwcdma_mc_get_sar_forced_ant(void)
{
  rfm_common_sar_state_type sar_index = rfm_common_sar_get_state();
  rfm_device_enum_type rfm_tx_device = RFM_MAX_WAN_DEVICES; 
  rfcom_wcdma_band_type band = RFCOM_BAND_INVALID; 
  boolean ant0_disabled = FALSE;
  boolean ant1_disabled = FALSE;
  rfwcdma_mc_sar_forced_antenna_enum_type ret_val = RFWCDMA_MC_SAR_ANTENNA_NONE;
  
  /*SAR state range in the NV is 1-8*/
  if(sar_index <= RF_SAR_STATE_DEFAULT || sar_index >= RF_SAR_STATE_MAX)
  {
    return RFWCDMA_MC_SAR_ANTENNA_NONE;
  }

  rfm_tx_device = rfwcdma_data_get_tx_device();
  if(rfm_tx_device < RFM_DEVICE_0  || rfm_tx_device >= RFM_WCDMA_MAX_DEVICE)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_txplim_get_sar_forced_ant: Invalid tx device %d",rfm_tx_device);
	return RFWCDMA_MC_SAR_ANTENNA_NONE;
  }
  
  band = rfwcdma_data_get_curr_band(rfm_tx_device);
  if(band < RFCOM_BAND_IMT || band >= RFCOM_NUM_WCDMA_BANDS)
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_txplim_get_sar_forced_ant: Invalid band %d",band);
	return RFWCDMA_MC_SAR_ANTENNA_NONE;
  }

  /*Check if AsDiv pos 0 is disabled*/
  if(rfnv_wcdma_tbl_ptr[band]!=NULL)
  {
    if((uint16)(rfnv_wcdma_tbl_ptr[band]->wcdma_db10_sar_back_off_limit[sar_index-1]) == RF_SAR_FORCE_OTHER_ANTENNA)
    {
      ant0_disabled = TRUE;
    }
  }
  else
  {
    RF_MSG_1(RF_ERROR, "rfwcdma_core_txplim_get_sar_forced_ant: NULL NV tbl ptr for band %d",band);
	return RFWCDMA_MC_SAR_ANTENNA_NONE;
  }
  /*Check if AsDiv pos 1 is disabled*/
  if((rfcommon_nv_tbl.rfwcdma_nv_sar_asdiv_table[band].band_enabled == TRUE) &&
  	 (rfcommon_nv_tbl.rfwcdma_nv_sar_asdiv_table[band].sar_back_off_table_asdiv_pos_1[sar_index-1]) == RF_SAR_FORCE_OTHER_ANTENNA)
  {
    ant1_disabled = TRUE;
  }

  if(ant0_disabled == TRUE && ant1_disabled == FALSE)
  {
    ret_val =  RFWCDMA_MC_SAR_ANTENNA_1;
  }
  else if (ant0_disabled == FALSE && ant1_disabled == TRUE)
  {
    ret_val =  RFWCDMA_MC_SAR_ANTENNA_0;
  }
  else if (ant0_disabled == TRUE && ant1_disabled == TRUE)
  {
    ret_val =  RFWCDMA_MC_SAR_ANTENNA_0;   
  }
  else
  {
    ret_val =  RFWCDMA_MC_SAR_ANTENNA_NONE;
  }

  
  RF_MSG_4(RF_HIGH, "rfwcdma_core_txplim_get_sar_forced_ant: sar_index: %d, SAR NV raw ant0: 0x%x, ant1: 0x%x, status: %d",\
  	                 sar_index,\
  	                 (uint16)(rfnv_wcdma_tbl_ptr[band]->wcdma_db10_sar_back_off_limit[sar_index-1]),\
  	                 (rfcommon_nv_tbl.rfwcdma_nv_sar_asdiv_table[band].sar_back_off_table_asdiv_pos_1[sar_index-1]),\
  	                 ret_val);
  
  return ret_val;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Get the NV band mask for requested device
 
  @details
  This function is exposed for WL1 to get the band mask for desired device
  @param device
  Preferred device
 
  @return
  Bandmask
*/
uint64 rfwcdma_mc_get_band_mask ( rfm_device_enum_type device )
{

  switch (device)
{
  case RFM_DEVICE_0 :
    return rfcommon_nv_get_rf_bc_config(RF_PATH_0);

  case RFM_DEVICE_1 :
    return rfcommon_nv_get_rf_bc_config(RF_PATH_1);

  case RFM_DEVICE_2 :
    return rfcommon_nv_get_rf_bc_config(RF_PATH_2);

  case RFM_DEVICE_3 :
    return rfcommon_nv_get_rf_bc_config(RF_PATH_3);

  case RFM_DEVICE_4 :
    /* Internal WWAN NV path 3 is mapped to RFM device 4 */
    return rfcommon_nv_get_rf_bc_config(RF_PATH_4);

  case RFM_DEVICE_5 :
    return rfcommon_nv_get_rf_bc_config(RF_PATH_5);

  default :
    /* Any other device request, return an empty mask */
    return 0;
  } /* switch (device) */
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  collecting variables to make them available in minidump
 
  @details
  This function registers the variables that needs to be there in the minidump
*/

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR 
void
rf_wcdma_QSH_mdump_cb
(
  void
)
{
  rfm_device_enum_type dev_idx; /* Variable for Device Loop */
  ftm_rfstate_enum_type ftm_state=FTM_STATE_UNKNOWN;

  rfwcdma_data_type *ptr_to_rf_wcdma_data = rf_wcdma_data_get_wcdma_data();

  for ( dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_DEVICES; dev_idx++ )
  {
    /*Check if the FTM Mode is active or not*/
    if(ftm_get_mode() == FTM_MODE)
    {
      ftm_state = ftm_get_current_state(dev_idx);
      /*Check if the FTM Mode has W active or not*/
      if ((ftm_state==FTM_STATE_WCDMA) || (ftm_state==FTM_STATE_WCDMA_IRAT) || (ftm_state==FTM_STATE_WCDMA_NS))
      {
        /*if W is active in FTM Mode then call the cb function to register all the FTM Variables*/ 
        ftm_wcdma_QSH_mdump_cb();
        qsh_mdump_collect_high((void *)ptr_to_rf_wcdma_data,sizeof(*ptr_to_rf_wcdma_data));
        break;
      }
    }

    /*Check if the Online Mode has W active or not*/
    else if(rfm_get_current_mode(dev_idx) == RFM_IMT_MODE) 
    {
      rflm_wcdma_data_t *ptr_to_rflm_wcdma_data = rflm_wcdma_get_rflm_data();
      qsh_mdump_collect_high((void *)ptr_to_rflm_wcdma_data,sizeof(*ptr_to_rflm_wcdma_data));
      qsh_mdump_collect_high((void *)ptr_to_rf_wcdma_data,sizeof(*ptr_to_rf_wcdma_data));
    }
  }
} /* rf_wcdma_QSH_mdump_cb */
#endif

/*! @endcond */
