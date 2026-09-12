#ifndef WL1RFMIF_H
#define WL1RFMIF_H

/*===========================================================================
                WL1 RF Driver interface header file

DESCRIPTION
  This file is WL1 RF Driver interface header file that supports multimode
  external interface while maintaning backward compatibility. This file
  contains all declarations and definitions necessary for WL1 to use the
  RF drivers. WL1 modules call inline function in this and this takes care of
  all RF API changes featurization.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None. All are passive inline function calling RF modules based on feature.
  All initialization requirment of respective RF modules should be followed

Copyright (c) 2006 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1rfmif.h#5 $
$DateTime: 2020/01/28 05:44:36 $
$Author: pwbldsvc $

when       who     what, where, why
--------------------------------------------------------------------------------------------------------- 
12/06/18   ss      Corrected typecasting logic
05/16/17   rn      FR 50745 MAAT for WCDMA
01/11/18   vn      ML logging changes
12/15/17   svh     Check for diversity device for AGC in diversity only mode.
12/06/17   ss      Check RF TX is enabled before TxAGC query
06/01/16   sl      Changes to not log AGC inside QTA gap
03/23/16   pvr     Mods to avoid calling RFM API rfm_wcdma_get_rxagc_info
                   if W holds invalid device
03/24/16   ac      Change wl1_rfm_write_dependent_nv_items_to_mdsp to take device as param
02/08/16   npn     Changes to log 0x4176 inside QTA gaps
10/16/15   rsr     0x4176 AGC Logging Changes.
12/15/15   sks     Make l1_rf_channel_tune_list a local.
11/04/15   sad     Use policyman api to get default band information
10/21/15   vr      Compilation error fixes for AT.
10/21/15   vr      ATLAS: L1 support for RFM API to get BTF delay
10/21/15   vr      Compilation errors when RFM_PATH is defined in ATLAS full stack build
09/29/15   us      Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
10/20/15   raj     Passing sub-id to rfm_wcdma_reduce_mptl_db_for_therm to correctly 
                   reflect MTPL for given sub-id
10/19/15   vr      RF API support for ATLAS.
09/18/15   us      Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
09/15/15   us      Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
10/17/15   vr      Changes and fixes needed for ATLAS RUMI.
10/17/15   vr      Replace rfm_device_enum_type with rfm_path_type for new RF APIs on ATLAS.
10/16/15   rn      Added Rumi framework enhancement changes for searcher module. 
09/07/15   svh     Pass device id to RF when writing tech dep NV items.
09/13/15   pv      Macro for querying RF ENTER MODE state.
08/31/15   ar      Ensure that previous Tx device is not invalidated prior to Exit mode
08/28/15   us      Updates for including rfm_path_type
08/19/15   sad     DBDC + Rx/Tx split feature check-in
08/20/15   ac      Add wl1_rfm_convert_sys_band_to_rfcom_band()
08/18/15   rsr     Added LW+W FTM support.
08/17/15   ac      Use RxAGC v2 APIs for RSCP
08/05/15   svh     Change RF api to check for diversity support.
08/01/15   vr      Changes and fixes needed for ATLAS RUMI.
07/29/15   ar      Ensure that RF Tx exit mode is called on previous device in ul_modify_band_handler
07/23/15   vn      Get correct RF device ID for RxAGC reading
07/20/15   nga     Ensure that RSCP RxAGC APIs use RAW as RxAGC type
07/14/15   vs      Use primary tuned RF device while querying the RxAgc value.
07/09/15   vr      Atlas compilation error fixes.
07/08/15   amj     RF Power clock on/off changes for Bolt W+W.
07/01/15   vr      Atlas specific changes for RF TX APIs.
07/01/15   pr      Atlas specific changes for rfm_wcdma_rx_get_agc
07/01/15   ar      Changes to move Tx enter/exit mode to UL context, along with RF Tx init/deinit
06/24/15   rs      L1-RF changes for Atlas
06/27/15   vr      UL SW changes for unified and modified RF APIs on ATLAS.
06/23/15   pr      FW Compiler error fixes for Atlas Modem
06/20/15   ac      Restore non-RX_TX_SPLIT handling for BO
06/18/15   br      Changes to support modified RF DSR API  
06/11/15   sad     RF state machine changes for handling meas device
06/04/15   sad     Added new api to return the current RF device on which we performed tune
06/03/15   pr      W+W Phase8 Checkin.
06/02/15   sks     Minimize RX_TX_SPLIT featurization.
05/20/15   sks     Compilation error fix.
05/19/15   sad     QTA cleanup code for idle tech
05/19/15   sad     Change to  perform an exit/sleep on old device and enter on new device
05/13/15   ar      Replace rfm_get_curr_band with rfm_wcdma_get_curr_band
05/08/15   vr      UL changes for RX-TX split.
05/06/15   abs     RX/TX Split support
05/07/15   rs      Rx-Tx Split changes
05/06/15   sad     Moving inline function from .h to .c file
04/10/15   ks      New RFM meas interface changes
03/31/15   vr      Fixing compilation error for RUMI.
03/29/15   pr      v2 WplusW changes for Diag F3.
03/13/15   rsr     Added Fix to Extern Global Defn.
03/05/15   sad     wl1_rf_state change
03/09/15   sks     Mods for full sleep in G2W TA mode.
02/26/15   sad     Change to double internal structure
02/24/15   sks     Changes for combined PRx/DRx tune.
02/19/15   vr      UL changes for structure based RF APIs.
02/14/15   nd/sa   Made changes for QTF with CRM
01/21/15   nd      Made changes for QTF with CRM
01/30/14   sad     Global re-org for W+W
01/29/14   pr      WL1M global reorg for WplusW
01/22/15   mm      IRAT globals re-organization for W+W
01/19/15   sks     Global re-org for W+W
12/23/14   psr     Added wrapper for rfm_wcdma_get_tx_max_power_nv()
10/28/14   sr      Initial checkin for 0x187B WCDMA power log pkt
10/07/14   sks     Compilation fix. 
09/17/14   mit     Skip enter mode calls from wl1 in FTM mode
09/05/14   pv      Removal of calls to unused RF APIs and enabling latest BO supported RF SLEEP API for Jolokia. 
08/28/14   vr      Mainlining code under FEATURE_WCDMA_DIME_SW, FEATURE_WCDMA_DIME_TX_STATE_REPORTING,
                   FEATURE_TRITON_MODEM for BO.2.0.
08/13/14   mit     FTM_RF/FTM_cal optimizations in WL1
08/13/14   sks     Skip sending AGC disable for early RF shutdown.
08/08/14   ymu     Fix Compilation warnings
08/04/14   sks     Set the correct status for the primary RF chain by checking with WRM rather than by hard coding it.
07/26/14   rkmk    Reduce image size by using right macro depending upon number of parameters
07/18/14   sad     RF api changes and inter freq tune changes
07/15/14   bj      WCDMA Narrow Band Rejection for System Search.
07/11/14   gp      Added support for RF Temp Comp Split architecture
07/10/14   pv/sks  Replaced all tune to chan calls by prep tune and exec tune. Replaced sleep_rx by rx_sleep.
07/03/14   sks     Changes for FCDC
07/02/14   sks     Temporarily revert the previous changes
06/26/14   pv/sks  Replaced all tune to chan calls by prep tune and exec tune. Replaced sleep_rx by rx_sleep.
06/24/14   mk      Support for changing EARFCN variable type from uint16 to uint32.
06/11/14   sks     Update the div chain awake status when doing a combined RF wakeup of both PRx and DRx
06/03/14   tsk     Compilation fixes for JOLOKIA 1.0
05/29/14   sks     Combined RF wakeup and tune optimizations on BO.2.0
05/27/14   mm      Removed featurization for DUAL_SIM
05/20/14   ar      Cleanup FEATURE_DIME_MODEM
05/19/14   vs      Removing RFM dynamic update API calls.
05/14/14   sks     IDLE Wakeup Optimizations and cleanup
04/18/14   ks      Pass correct rxlm index values for w2w build scripts
02/12/14   sad     DB-3C API check in
04/01/14   pj      DIME PM RUMI support changes
03/24/14   ac      Changed RFM_IMT_MODE to RFCOM_WCDMA_MODE
03/12/14   sks     Call rfm_Exit_mode when L1 stack deactivates.
03/10/14   ks      Change W2W tuning to have target param always use SC carrier type
03/10/14   ks      Bolt FED API checkin
02/14/14   km      rfm_tune_chan featurization consistent with rf api
01/30/14   ymu     Call pre_rf_init for DC-HSUPA call setup.
01/21/14   abs     Mainlining wl1_rfm_get_ul_freq_from_dl_arfcn()
12/13/13   sr      Fixed a merge issue for wl1_rf_carrier_enum_type.
12/11/13   mm      3C CM/IRAT support
12/10/13   pv      Support of RFM PREP and EXEC wakeup APIs.
12/03/13   vc      Added a function to get rf default band and channel info.
11/21/13   tsk     FR2573: SAR management with Integrated DSDA changes.
10/23/13   sm      Set RxD wakup flag when enter mode is called for correct behavior in PCH
11/05/13   vs      cleaned up the API wl1_rfm_get_curr_band() 
10/23/13   sad     Update to inverse DL freq calculations
10/17/13   bs      Preliminary 3C Support
10/15/13   vs      Modified the HS api for LNA config.
10/11/13   cc      Add a current Rx mode check before acquiring the 2nd device id 
10/03/13   pv      Back out previous check-in.
10/08/13   rsr     3C API Changes Checkin
09/30/13   cc      Fix the incorrect band issue for DBDC interF
09/29/13   as      Continuing port of Dual Sim from Triton to Dime.
09/23/13   as      Porting Dual Sim from Triton to Dime.
09/12/13   jd      Searcher redesign
09/03/13   km      kw fix
08/27/13   rsr     Changed API call for Div. Only mode from "Enter_mode" to "Rfm_wakeup"
08/12/13   cc      DBDC CM W2W feature check in
08/12/13   km      Featurize rfm_tune_chan until RF checks in DBDC changes
07/26/13   at      rfm_wcdma_wakeup_tx() should not be called back-to-back after rfm_init_wcdma_tx()
07/24/13   jd      Changes for Dual band Dual carrier
07/08/13   mm      Update the internal W band enum type to the one used by RF
07/02/13   yw      Extend RF API to Freeze TxAccum for Sec carrier.
                   Extend rfm_init_wcdma_tx to notify DC cfged mode.
05/22/13   oh      Adding DC-HSUPA framework support
04/23/13   pr      Use new rfm_wakeup_tx api and rfm_init_tx_needed flag to reduce RF wakeup times.
03/29/13   pkg     Tx state reporting to MCS for DIME.
03/12/13   at      RF API to decfg Txlm buffer for secondary Tx path
02/22/13   ms      Check div chain RxLM buffer allocation instead of RxD active state
02/05/13   ms      Added support for HS RxLM
01/28/13   dp      Added support for Antenna Tuner
02/05/13   pv      Support for FR 2001 - RF Wakeup Optimization.
01/27/13   geg     Removed FEATURE_WCDMA_RUMI_SUPPORT flag from RxAGC API's as RF support is now available.
01/15/12   ash     Removed FEATURE_WCDMA_RUMI_SUPPORT flag from rfm_do_temp_comp() as RF support is now available.
01/14/12   ash     Removed FEATURE_WCDMA_RUMI_SUPPORT flag from rfm_set_tx_pwr_limit() as RF support is now available.
12/07/12   scm     Pass trm_get_rf_device() return value into RFM APIs.
08/29/12   pv      Changed the RF awake API and moved flag update to end of API.
08/03/12   gsk     Moving rfm_pretune func inside FEATURE_WCDMA
08/03/12   gsk     Updating the RxAGC read with new RF api
08/01/12    pv     New AGC read APIs for Offline PICH.
11/20/12   gsk     Support for Idle mode search and reselections.
11/19/12   pr      Fixed Merge Error.
11/17/12   cc      W2W interF and CM feature for Dime
11/09/12   pr      Remove FEATURE_RUMI_SUPPORT for all supported api's. 
10/23/12   ash     Changes for Dime Bring up
10/01/12   ash     Fixed Dime Compiler warnings
09/20/12   ash     Dime Tx bring-up changes
08/23/12   gsk     Backing out changes for CR 380698
08/15/12   gsk     Moving rfm_pretune func inside FEATURE_WCDMA
08/15/12   gsk     Updating the RxAGC read with new RF api
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
05/31/12   at      Added new CM API support for PA gain state and correct TX power support
07/05/12    pv     Offline PICH changes - tracking div chain status.
07/05/12   gsk     Idle mode FRs 1782/1705/1947/2152/1950
06/04/12   gsk     Fix for Reacq failure on serving during interband tuning.
05/15/12   gsk     FR 1782: WCDMA Inter- Frequency Tuning Optimization.
06/29/12   zr      Added function to get uncompensated RxAGC
05/08/12   pr      Added API to inform RF state when CPC-DRX or E-DRX is active
05/03/12   vr      Fixed warning.
04/19/12   yh      Pass extra flag for rfm_meas_exit() for W2L
04/17/12   at      Fix in RF API for GSM dependency.
04/16/12   at      Remove RF Tx enable from UL PhyChan Add for NikeL
03/30/12   at      Added new CM API support for PA gain state and correct TX power support
03/21/12   ar      For invalid RF CAL data, WL1 will call CM API to make UE go to 
                   offline mode and send Cphy_setup_failure to RRC.03/19/12   vr      Added check to avoid RF TX disable twice
03/07/12   zr      Adding function that converts an Rx EARFCN to its corresponding
                   carrier frequency
02/01/12   gnk     Nikel Feature cleanup.
01/27/12   vs      Nikel feature cleanup.
01/07/12   stk     Updated the rfm_get_receive_agc_val_for_rscp with correct parameters for NIKE targets.
                   This API will now return the RAW agc values in dbm instead of noise compensated value.
11/30/11   rc      Added DSR support under FEATURE_WCDMA_DSR_SUPPORT   
11/29/11   gnk     Removed some unused features
10/20/11   gnk     Removed MTD related code for NikeL
09/21/11   sp      Added NPA Support for Thermal Mitigation under FEATURE_WCDMA_NPA_THERMAL
09/16/11   sj      Added support for RxD API to change Rx Chain Configuration for next DCH state
09/06/11   vsr     Update APIs to work in Div-Only mode
08/20/11   oh      Call rfm_init during ul phychan add.
08/11/11   mmr     Removed xo calls from rfm interface. Added an api to get
                   freq from a given ARFCN.
08/03/11   dp      Re-worked dynamic tune time code for NikeL bringup.
07/07/11   hk      Up-merge from offline/main/latest
06/30/11   mk      Warning fixes
06/17/11   mk      Removed depricated RF API includes rfagc.h
06/09/11   dp      Checked in fixes for dynamic tune time code
05/27/11   mmr     Added XO fixes
05/26/11   dp      Added changes to make irat tuning time value dynamic
05/26/11   dp      Added support for W2W XO Parameter passing to FW
04/28/11   mmr     Added support for Feature XO.
04/22/11   yh      Removed RxLM buf idx from CM RF functions
04/13/11   yh      Enabled RxLM code for RUMI builds
04/04/11   oh      Changes to support UL DCH on NikeL
04/04/11   yh      Added support for RxLM/TxLM
03/30/11   gnk     Mainlined several features
03/17/11   sv      Added RF wrapper function to get rxagc for diversity chain.
02/17/11   gnk     Populate the MTPL to FW for the correct antenna
02/11/11   sp      Added support for Thermal Mitigation under FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
12/01/10   dp      modified featurization around rfm_meas_common_xxxx functions 
                   (and #include dependancies) so rf script tuning works in 
                   idle w2l
10/27/10   mm      Corrected typo in RF API name
10/15/10   mr/yh   Added support for W2L Compressed mode
10/04/10   sv      Fix compiler warning.
08/30/10   yh      Make carrier_spacing uint16
07/09/10   vsr     Updated arguments in wl1_rfm_tune_to_chan_rx()
06/19/10   sv      Added function that returns RF AGC for secondary carrier.
06/18/10   pv      Fixed compiler warnings.
06/02/10   sa      Changed function "wl1_rfm_get_tx_agc_val_in_dB12" to
                   wl1_rfm_get_tx_agc_val_in_mdsp_units to support txagc in 
                   dB10 format. 
05/21/10   yh      Added support API to convert sys_band to rf_card_band
03/19/10   yh      Add DC+CM support.
03/17/10   rgn     Added code to switch between sc and dc mode
03/17/10   rgn     Added code to control dc carrier spacing 
02/26/10   rvs     Fixes for Mustang.
11/18/09   gv      Use db10 instead of db12 units for computation
10/24/09   ms      Featurization changes
05/29/09   m       Introduced wrapper function for wl1_rfm_get_tx_pwr_limit
04/29/09   mg      Fixed compiler warnings
02/18/09   prk     Changed the declaration of Inline functions to pass offline builds.
01/28/09   rmak    Added state variable for tracking WL1's knowledge of RF On/Sleep
12/01/08   saik    Enabling rfm_sleep in wl1_rfm_sleep by taking out #ifndef RUMI_BRING_UP
11/25/08   asm     Removed RUMI_BRING_UP featurization from freezeaccum and pa_off functions
11/13/08   yh      Add CM support
10/07/08   gnk     Added support for logging tx accum from SW side
10/03/08   hk      Bringing in 7k Mailine fixes
09/06/07   stk     Changed RFM_IMT_MODE to RFCOM_WCDMA_MODE, which is passed to rfm_enter_mode()
08/15/07   mg      Remove rf_pm_ldo_on_has_extra_delay declaration.
08/02/07   asm     New API to feed RF the beta values and HS status
05/31/07   nag     Featurized functions for the EDGE only build
05/02/07   ms      Created separate APIs for HS & R99 LNA switchpoint 
                   programming.
04/30/07   ms      Featurized LNA switchpoint programming implementation under
                   FEATURE_HSDPA_PROGRAM_LNA_SWITCH.
04/16/07   ms      Added API's to query RF whether LNA switchpoint update
                   is required or not and to program LNA NV items to mDSP
03/01/07   vrao    Added rfm_indicate_txagc_threshold_crossing() to process
                   FW interrupt based on WCDMA Tx AGC
02/09/07   asm     Changed RF API to set Tx General Ctl register
12/20/06   rmak    Changed wl1_rfm_get_rf_warmup_time to return uint32
12/11/06   mg      Add wl1_rfm_get_rf_warmup_time
12/08/06   rmak    Mainlining FEATURE MODEM SLEEP CONTROLLER INSTANCE 1
                   and FEATURE INTER SYSTEM TIMING MAINT THROUGH SLEEPCTLR
10/30/06   rmak    rf_pm_ldo_on_has_extra_delay featurized under
                   FEATURE INTER SYSTEM TIMING MAINT THROUGH SLEEP CTLR
08/29/06   rc      Added wl1_rfm_get_ue_max_tx_power_in_dB12() 
                   and wl1_rfm_get_tx_agc_val_in_dB12() functions
                   for Evt 6D evaluation.
08/16/06   rc      Changed RF API calls in wl1_rfm_get_ue_max_tx_power() 
                   and wl1_rfm_get_tx_agc_val() functions. 
08/15/06   rmak    Add extern rf_pm_ldo_on_has_extra_delay to determine RF LDO timing
07/19/06   yh      Fixed compiler error on Saber in wl1_rfm_do_tx_pwr_limit()
07/17/06   yh      Added new functions wl1_rfm_set_tx_pwr_limit, wl1_rfm_enable_tx,
                   wl1_rfm_do_tx_pwr_limit, wl1_rfm_do_temp_comp and
                   wl1_rfm_disable_tx to support new RFM API.
07/17/06   asm     Created function to determine if DM needed for HDET
05/26/06   bd      Fixed compiler error on Saber in wl1_rfm_get_rx_agc_val()
05/19/06   au      Unified interface to RF APIs through wl1rfmif header.
03/14/06   scm     Implement wl1_rfm_get_curr_band().
02/20/06   bbd     Removed rf_l1_cmn_defs.h per RF driver request
01/18/06   yiz     Fixed compile errors due to MSM specific features
01/17/06   gs/bd   Updated RFAPI calls
01/13/06   gs      Created this file
===========================================================================*/
#include "wcdma_variation.h"
#include "rfm.h"
#include "rex.h"

#ifdef FEATURE_WCDMA

/* ---------------------------- */
/* Check for necessary features */
/* ---------------------------- */

#include "l1utils.h"
#include "wl1xlm.h"
#include "wl1trm.h"
#include "rxdiv_v.h"
#include "rxlm_intf.h"
#include "rfm_wcdma.h"

#ifdef FEATURE_WCDMA_ATLAS_MODEM
#include "policyman.h"
#endif /* FEATURE_WCDMA_ATLAS_MODEM */

#if defined(FEATURE_WCDMA_TO_LTE) && (defined(FEATURE_WCDMA_CM_LTE_SEARCH) || defined(FEATURE_W2L_IDLE_FW_RF_TUNE))
#include "rfmeas_lte.h"
#include "rfmeas_wcdma.h"
#include "rfmeas_types.h"
#include "rfm_meas.h"
#include "rfm_mode_types.h"
#endif

/* SAR feature set is a superset of DIME TX state reporting. */
#ifdef FEATURE_WCDMA_SAR_TX_PWR_FILTER
#include "wl1ulsarmgr.h"
#endif /* FEATURE_WCDMA_SAR_TX_PWR_FILTER */
#include "rfm_wcdma_param_types.h"
#include "mdspsync.h"

/* =======================================================================
**                        Externed Variables
** ======================================================================= */

#define WL1_RF_TUNE_CARR_0 0x1
#define WL1_RF_TUNE_CARR_1 0x2
#define WL1_RF_TUNE_CARR_2 0x4

#define NUM_CARR_0 0
#define NUM_CARR_1 1
#ifdef FEATURE_WCDMA_DC_HSDPA
#define NUM_CARR_2 2
#ifdef FEATURE_WCDMA_3C_HSDPA
#define NUM_CARR_3 3
#endif
#endif

#define WL1_RF_HEADER_SB_NUM_SOURCE 1
#define WL1_RF_HEADER_DB_NUM_SOURCE 2

#ifdef FEATURE_WCDMA_3C_HSDPA
#define WL1_RF_MAX_NUM_CARR 3
#elif defined (FEATURE_WCDMA_DC_HSDPA)
#define WL1_RF_MAX_NUM_CARR 2
#else
#define WL1_RF_MAX_NUM_CARR 1
#endif

#ifdef FEATURE_WCDMA_RUMI_SUPPORT
extern uint8 wl1_rumi_dc_enable[2];
#endif

/* Num of RF source items for Single-Band */
#define WL1_RF_HEADER_SB_NUM_SOURCE 1
/* Num of RF source items for Dual-Band */
#define WL1_RF_HEADER_DB_NUM_SOURCE 2

#define PRIMARY_CHAIN 0
#define SECONDARY_CHAIN 1

/* Following combination is used for each subscription
** C3     C2     C1     C0
** Sub-1  Sub-1  Sub-0  Sub-0 */
#define GET_TUNE_BMSK(carr_idx) (0x1 << (GET_CARR_ID_FOR_SUB(carr_idx)))

#define GET_CARR_ID_FOR_SUB(carr_idx) (carr_idx + (0x2 * wsub_id))

#define WL1_INVALID_FREQ 0x0

#ifdef FEATURE_WCDMA_ATLAS_MODEM
  /* Mapping SYS_SBAND*/

  /**< WCDMA Europe, Japan, and China IMT 2100 band. = 22*/
  #define WCMDA_SYS_SBAND_WCDMA_I_IMT_2000 SYS_SBAND_WCDMA_I_IMT_2000

  /**< WCDMA US PCS 1900 band. = 23*/
  #define WCMDA_SYS_SBAND_WCDMA_II_PCS_1900 SYS_SBAND_WCDMA_II_PCS_1900

  /**< WCDMA Europe and China DCS 1800 band. = 24*/
  #define WCMDA_SYS_SBAND_WCDMA_III_1700 SYS_SBAND_WCDMA_III_1700

  /**< WCDMA US 1700 band. = 25*/
  #define WCMDA_SYS_SBAND_WCDMA_IV_1700 SYS_SBAND_WCDMA_IV_1700

  /**< WCDMA US850 band. = 26*/
  #define WCMDA_SYS_SBAND_WCDMA_V_850 SYS_SBAND_WCDMA_V_850

  /**< WCDMA Japan 800 band. = 27*/
  #define WCMDA_SYS_SBAND_WCDMA_VI_800 SYS_SBAND_WCDMA_VI_800

  /**< WCDMA Europe 2600 band. = 48*/
  #define WCMDA_SYS_SBAND_WCDMA_VII_2600 SYS_SBAND_WCDMA_VII_2600

  /**< WCDMA Europe and Japan 900 band. = 49*/
  #define WCMDA_SYS_SBAND_WCDMA_VIII_900 SYS_SBAND_WCDMA_VIII_900

  /**< WCDMA Japan 1700 band. = 50*/
  #define WCMDA_SYS_SBAND_WCDMA_IX_1700 SYS_SBAND_WCDMA_IX_1700

  /**< WCDMA Japan 850 band. = 60*/
  #define WCMDA_SYS_SBAND_WCDMA_XIX_850 SYS_SBAND_WCDMA_XIX_850

  /**< WCDMA 1500 band. = 61*/
  #define WCMDA_SYS_SBAND_WCDMA_XI_1500 SYS_SBAND_WCDMA_XI_1500

  #define WCMDA_SYS_SBAND_MAX SYS_SBAND_MAX

#define WCDMA_SBAND_BMSK  ((0x1 << WCMDA_SYS_SBAND_WCDMA_I_IMT_2000) | (0x1 << WCMDA_SYS_SBAND_WCDMA_II_PCS_1900) \
                         | (0x1 << WCMDA_SYS_SBAND_WCDMA_III_1700)   | (0x1 << WCMDA_SYS_SBAND_WCDMA_IV_1700)     \
                         | (0x1 << WCMDA_SYS_SBAND_WCDMA_V_850)      | (0x1 << WCMDA_SYS_SBAND_WCDMA_VI_800)      \
                         | (0x1 << WCMDA_SYS_SBAND_WCDMA_VII_2600)   | (0x1 << WCMDA_SYS_SBAND_WCDMA_VIII_900)    \
                         | (0x1 << WCMDA_SYS_SBAND_WCDMA_IX_1700)    | (0x1 << WCMDA_SYS_SBAND_WCDMA_XIX_850))     \
                         | (0x1 << WCMDA_SYS_SBAND_WCDMA_XI_1500)

#endif /* FEATURE_WCDMA_ATLAS_MODEM */

#define WL1_RF_TX_POWER_MIN_DBM -70

#define WL1_RF_TX_POWER_MIN_DB10 -700

#define WL1_RF_TX_DEFAULT_XPT_MODE 0x0

#define WL1_RF_DEFAULT_PA_RANGE 0xF
/* Create alias to be used in WL1 instead of RF type */
typedef rfcom_wcdma_band_type wl1_rf_card_band_enum_type;

typedef rfcom_tx_multi_carrier_cfg_type wl1_rfcom_tx_multi_carrier_cfg_type;

typedef rfm_wcdma_txagc_log_param_type wl1_txagc_params_type;

/* ENUM to keep track of RF TX state*/
typedef enum 
{
  WL1_RF_TX_ENABLED,
  WL1_RF_TX_DISABLED
} wl1_rf_tx_status_enum;

typedef enum
{
  WL1_RFM_WAKEUP_PREP,
  WL1_RFM_WAKEUP_EXEC,
  WL1_RFM_SLEEP_EXEC,
  WL1_RFM_PREP_TUNE,
  WL1_RFM_EXEC_TUNE
}wl1_rfm_wakeup_type;

#ifdef FEATURE_WCDMA_RX_TX_SPLIT
typedef enum
{
  RF_EXIT_MODE,
  RF_ENTER_MODE,
  RF_PREP_TUNE_COMPLETE,
  RF_AWAKE,
  RF_SLEEP_IN_PROG,
  RF_ASLEEP,
}wl1_rf_sleep_status_enum_type;

#else
typedef enum
{
  RF_ASLEEP,
  RF_AWAKE,
  RF_SLEEP_IN_PROG
}wl1_rf_sleep_status_enum_type;

#endif /*FEATURE_WCDMA_RX_TX_SPLIT */

/* Carrier information */
typedef enum
{
  WL1_RFM_SINGLE_CARRIER,
  WL1_RFM_DUAL_CARRIER,
  WL1_RFM_3_CARRIER,
  WL1_RFM_MAX_NUM_CARRIERS
} wl1_rfm_carrier_mode_enum_type;

typedef enum
{  
  WL1_RFM_WAKEUP_PRIMARY,
  WL1_RFM_WAKEUP_DIVERSITY,
  WL1_RFM_WAKEUP_DUAL,
  WL1_RFM_WAKEUP_MAX
}wl1_rfm_wakeup_chain_type;

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
typedef enum
{
  WL1_DL_PRI_PATH,
  WL1_DL_DIV_PATH,
  WL1_DL_PRI_CA_PATH,
  WL1_DL_DIV_CA_PATH,
  WL1_MAX_PATH
}wl1_rfm_path_type;

typedef struct
{
  /* Current tune path */
  rfm_path_type rfm_path;
  /*freq array */
  int32 curr_tuned_chan_rx_freq_kHz;
  /* enum to hold the RF operation to be performed on a chain(s). */
  wl1_rf_sleep_status_enum_type rf_state;
}wl1_rfm_path_db;

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
 typedef enum
{
  WL1_DL_PRI_DEVICE,
  WL1_DL_DIV_DEVICE,
  WL1_DL_PRI_CA_DEVICE,
  WL1_DL_DIV_CA_DEVICE,
  WL1_MAX_DEVICE
}wl1_rfm_device_type;

typedef struct
{
  /* Current tune devices */
  rfm_device_enum_type device_id;
  /*freq array */
  int32 curr_tuned_chan_rx_freq_kHz;
  /* enum to hold the RF operation to be performed on a chain(s). */
  wl1_rf_sleep_status_enum_type rf_state;
}wl1_rf_device_db;

#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
typedef enum {
  WL1_UL_DEVICE_TO_ENTER_EXIT_CURRENT_DEVICE,
  WL1_UL_DEVICE_TO_ENTER_EXIT_PREV_DEVICE,
  WL1_UL_DEVICE_TO_ENTER_EXIT_MAX
}wl1_ul_device_to_enter_exit_e_type;

/*----------------------------------------------------------------------------*/
/*! Enumeration for RFM WCDMA FTM state. Use to distinguish different modes of 
  operation in FTM for RF */
typedef enum
{
  WL1_RFM_WCDMA_FTM_NONE = 0,
  /*!< RF in non-FTM state i.e. online mode */

  WL1_RFM_WCDMA_FTM_CAL = RFM_WCDMA_FTM_CAL,
  /*!< RF in FTM Cal state */

  WL1_RFM_WCDMA_FTM_RF = RFM_WCDMA_FTM_RF,
  /*!< RF in FTM RF state */

  WL1_RFM_WCDMA_FTM_NONSIG = RFM_WCDMA_FTM_NONSIG,
  /*!< RF in FTM non-signaling state */

  WL1_RFM_WCDMA_FTM_RXONLY = RFM_WCDMA_FTM_RXONLY,
  /*!< RF in FTM Rx only mode */ 

  WL1_RFM_WCDMA_FTM_W_PLUS_W = RFM_WCDMA_FTM_W_PLUS_W,
  /*!< RF in FTM W+W mode  */

  #ifdef FEATURE_WCDMA_ATLAS_MODEM
  WL1_RFM_WCDMA_FTM_CAL_TEST = RFM_WCDMA_FTM_CAL_TEST,
  /*!< RF in FTM Cal test state. */
  #endif /* FEATURE_WCDMA_ATLAS_MODEM */

  WL1_RFM_WCDMA_FTM_INVALID
  /*!< RF in FTM state max */

} wl1_rfm_wcdma_ftm_state_type;

typedef struct
{
  /* carrier tune bmsk */
  uint8 tune_bmsk;
  /*freq array */
  uint16 uarfcn[WL1_RF_MAX_NUM_CARR];
  /* current tuned center freq */
  int32 curr_tuned_chan_rx_freq_kHz;
  /*Is RF tune required in current CPHY*/
  boolean rf_tune_needed;
}wl1_rf_config_struct_type;

typedef enum
{
  WL1_DL_QTA_RF_START,
  WL1_DL_QTA_RF_ENTER,
  WL1_DL_QTA_RF_EXECUTE_SCRIPT,
  WL1_DL_QTA_RF_EXIT,
  WL1_DL_QTA_RF_OPS_DONE
}wl1_dl_qta_rf_ops_enum_type;

typedef enum {
  WL1_MDSP_AGC_CMD_NOOP,
  WL1_MDSP_AGC_CMD_DISABLE,
  WL1_MDSP_AGC_CMD_ENABLE,
  WL1_MDSP_AGC_CMD_INVALID
}wl1_mdsp_agc_action_type;

#ifdef FEATURE_WCDMA_ATLAS_MODEM
typedef struct
{
  /*Holds the status of nbr enable and its corresponding rxlm index*/
  boolean nbr_enable_state;
  uint32 nbr_lm_idx;
}wl1_rf_nbr_state;
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/

typedef struct
{

  rfm_wcdma_rx_wakeup_params wl1_wakeup_params_copy;

  rfm_wcdma_rx_wakeup_params wl1_wakeup_params;

  rfm_wcdma_rx_tune_params wl1_tune_params;

  rfm_wcdma_rx_tune_params wl1_tune_params_copy;

  rfm_wcdma_rx_sleep_params wl1_sleep_params_copy;

  /* This flag is used to inform RF to skip sending AGC command to FW. It will be set to TRUE in case of early RF shutdown
   since RF and FW sleep can happen in parallel with the latest optimizations */
  boolean wl1_skip_sending_agc_disable;

  rfm_wcdma_rx_sleep_params wl1_sleep_params;

  boolean wl1_rfm_power_on_needed;

  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/* enums to hold the RF operation to be performed on a chain(s) */
  wl1_rfm_path_db rfm_path_db[WL1_MAX_PATH];  
  #else
/* enums to hold the RF operation to be performed on a chain(s) */
  wl1_rf_device_db rf_device_db[WL1_MAX_DEVICE];
  #endif

#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_wcdma_log_agc_handle_type wl1_rfm_log_agc_handle;
  rfm_wcdma_rx_init_param_type wl1_rx_init_params;
  rfm_wcdma_rx_init_param_type wl1_rx_init_params_copy;
  rfm_wcdma_rx_wakeup_param_type wl1_rx_wakeup_params;
  rfm_wcdma_rx_wakeup_param_type wl1_rx_wakeup_params_copy;
  wl1_rf_nbr_state wl1_nbr_status;
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/

  wl1_dl_qta_rf_ops_enum_type wl1_x2w_qta_rf_state;

  rfm_wcdma_log_agc_handle_type wl1_rfm_log_agc_handle;
  rfm_wcdma_maat_rx_info_type_s wcdma_maat_params;
}wl1rfmif_int_cntrl_params_type;

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif


extern wl1_rf_tx_status_enum wl1_rf_tx_state;

extern wl1rfmif_int_cntrl_params_type wl1rfmif_int_cntrl_params[WCDMA_NUM_SUBS];

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
#define WL1_RF_AWAKE()          (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[WL1_DL_PRI_PATH].rf_state == RF_AWAKE)

#define WL1_RF_DIV_AWAKE()      (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[WL1_DL_DIV_PATH].rf_state == RF_AWAKE)
#define WL1_RF_DIV_CA_AWAKE()      (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[WL1_DL_DIV_CA_PATH].rf_state == RF_AWAKE)

#ifdef FEATURE_WCDMA_RX_TX_SPLIT

#define WL1_RF_IN_PREP_TUNE_STATE(chain)  (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[chain].rf_state  == RF_PREP_TUNE_COMPLETE)

#define WL1_GET_RF_STATE(chain)           (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[chain].rf_state)

#define WL1_ENTER_MODE_POSSIBLE(chain) ( \
   (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[chain].rf_state == RF_EXIT_MODE) || \
   (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[chain].rf_state == RF_ASLEEP))

#define WL1_RF_IN_ENTER_MODE_STATE(chain)  (wl1rfmif_int_cntrl_params[wsub_id].rfm_path_db[chain].rf_state  == RF_ENTER_MODE)
#endif /* FEATURE_WCDMA_RX_TX_SPLIT */

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#define WL1_RF_AWAKE()          (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[WL1_DL_PRI_DEVICE].rf_state == RF_AWAKE)

#define WL1_RF_DIV_AWAKE()      (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[WL1_DL_DIV_DEVICE].rf_state == RF_AWAKE)
#define WL1_RF_DIV_CA_AWAKE()      (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[WL1_DL_DIV_CA_DEVICE].rf_state == RF_AWAKE)

#ifdef FEATURE_WCDMA_RX_TX_SPLIT

#define WL1_RF_IN_PREP_TUNE_STATE(chain)  (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[chain].rf_state  == RF_PREP_TUNE_COMPLETE)

#define WL1_GET_RF_STATE(chain)           (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[chain].rf_state)

#define WL1_ENTER_MODE_POSSIBLE(chain) ( \
   (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[chain].rf_state == RF_EXIT_MODE) || \
   (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[chain].rf_state == RF_ASLEEP))

#define WL1_RF_IN_ENTER_MODE_STATE(chain)  (wl1rfmif_int_cntrl_params[wsub_id].rf_device_db[chain].rf_state  == RF_ENTER_MODE)
#endif /* FEATURE_WCDMA_RX_TX_SPLIT */
#endif   /*(FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)*/

#ifdef FEATURE_WCDMA_ATLAS_MODEM
#define WL1_RF_GET_NBR_STATUS()   wl1rfmif_int_cntrl_params[wsub_id].wl1_nbr_status.nbr_enable_state
#define WL1_RF_GET_NBR_HANDLE()   wl1rfmif_int_cntrl_params[wsub_id].wl1_nbr_status.nbr_lm_idx
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/

/* =======================================================================
**                        Function Declarations
** ======================================================================= */
extern wl1_rf_config_struct_type* wl1_dl_get_rf_db(uint8 , wsub_id_e_type wsub_id);

extern void wl1_rfm_log_agc( wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
extern int32 rfm_exit_mode (rfm_path_type path, rfcom_mode_enum_type to_rfmode);
#else
extern int32 rfm_exit_mode (rfcom_device_enum_type device,
                     rfcom_mode_enum_type to_rfmode);
#endif

extern rfm_wcdma_ftm_state_type rfm_wcdma_get_ftm_state(void);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
 FUNCTION  WL1_RFM_WCDMA_POWER_ON_OFF

 DESCRIPTION
   Function used to turn rfm power clock on or off depending on the parameter
   passed to it

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
boolean wl1_rfm_wcdma_power_on_off(boolean power_on_off, rfm_path_type rfm_path, rfm_cb_handler_type callback, rfm_path_state rf_state, wsub_id_e_type wsub_id);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
 FUNCTION  WL1_RFM_WCDMA_POWER_ON_OFF

 DESCRIPTION
   Function used to turn rfm power clock on or off depending on the parameter
   passed to it

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
boolean wl1_rfm_wcdma_power_on_off(boolean power_on_off, uint16 device_id, rfm_cb_handler_type callback, rfm_path_state rf_state, wsub_id_e_type wsub_id);

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_prep_wakeup_combined

DESCRIPTION
  This calls the new prep wakeup function from RF.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_prep_wakeup_combined(wl1_rfm_wakeup_chain_type chain, wsub_id_e_type wsub_id);


#if defined FEATURE_WCDMA_ATLAS_MODEM || defined FEATURE_WCDMA_THOR_MODEM || defined FEATURE_WCDMA_TABASCO_MODEM
/*===========================================================================
 FUNCTION  wl1_rfm_log_agc_create_handle

 DESCRIPTION
   Creates the AGC handle to log 0x4176

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
extern void wl1_rfm_log_agc_create_handle(wsub_id_e_type wsub_id);
/*===========================================================================
 FUNCTION  wl1_rfm_get_device_type_from_device_id

 DESCRIPTION
   Deletes the AGC handle to log 0x4176

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
extern void wl1_rfm_log_agc_delete_handle(wsub_id_e_type wsub_id);

#endif

/*===========================================================================
FUNCTION  wl1_rfm_write_dependent_nv_items_to_mdsp() 

DESCRIPTION
  This function writes HS dependent lna nv items to mdsp

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void wl1_rfm_write_dependent_nv_items_to_mdsp(rfcom_wcdma_band_type band, 
                                                     rfcom_tech_type tech_type, 
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
                                                     rfm_path_type path,
#else
                                                     rfm_device_enum_type device, 
#endif
                                                     wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_RX_TX_SPLIT

/*===========================================================================
FUNCTION  wl1_rfm_enter_mode_tx_chain

DESCRIPTION 
  Enter IMT mode on TX Chain
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_enter_mode_tx_chain(
  uint32 buf_idx,
  wl1_ul_device_to_enter_exit_e_type device_type,
  wsub_id_e_type wsub_id);

#endif /* FEATURE_WCDMA_RX_TX_SPLIT */
/*===========================================================================
 FUNCTION  wl1_rfm_int_cntrl_params_init

 DESCRIPTION
   Init function for RF db

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_rfm_int_cntrl_params_init(wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_populate_rfm_slp_wkup_params

DESCRIPTION
  This function populates the parameters of the RF wakeup/sleep/tune structures for dispatching to RF to perform the .
  respective operations Takes as arguments the RxLM buffer index, the rfm_path, chains to be operated upon - PRx 
  or PRx/DRx, and nature of the operation - wakeup, tune or sleep.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_populate_rfm_slp_wkup_params
(
  uint32 buf_idx,
  rfm_path_type rfm_path,
  wl1_rfm_wakeup_chain_type chain,
  wl1_rfm_wakeup_type wakeup_type,
  uint16 freq_info,
  wsub_id_e_type wsub_id
);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_populate_rfm_slp_wkup_params

DESCRIPTION
  This function populates the parameters of the RF wakeup/sleep/tune structures for dispatching to RF to perform the .
  respective operations Takes as arguments the RxLM buffer index, the device_id, chains to be operated upon - PRx 
  or PRx/DRx, and nature of the operation - wakeup, tune or sleep.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_populate_rfm_slp_wkup_params
(
  uint32 buf_idx,
  uint16 device_id,
  wl1_rfm_wakeup_chain_type chain,
  wl1_rfm_wakeup_type wakeup_type,
  uint16 freq_info,
  wsub_id_e_type wsub_id
);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */


#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_sleep_combined

DESCRIPTION
  This calls the new sleep function from RF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_sleep_combined(rfm_cb_handler_type cb_handler, rfm_path_type rfm_path, wsub_id_e_type wsub_id);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_sleep_combined

DESCRIPTION
  This calls the new sleep function from RF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_sleep_combined(rfm_cb_handler_type cb_handler, uint16 device_id, wsub_id_e_type wsub_id);

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_exec_wakeup_combined

DESCRIPTION
  This calls the new exec wakeup function from RF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_exec_wakeup_combined(rfm_cb_handler_type cb_handler, rfm_path_type rfm_path, wl1_rfm_wakeup_chain_type chain, uint16 freq_info, wsub_id_e_type wsub_id);
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_exec_wakeup_combined

DESCRIPTION
  This calls the new exec wakeup function from RF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_exec_wakeup_combined(rfm_cb_handler_type cb_handler, rfm_device_enum_type device_id, wl1_rfm_wakeup_chain_type chain, uint16 freq_info, wsub_id_e_type wsub_id);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_prep_tune

DESCRIPTION
  This calls the new prep tune functionality. Takes channel number as a parameter apart from buffer index and chain type.
  Called only in idle mode, so the carrier index is always 1 in prep tune.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_prep_tune(uint16 channel,uint32 buf_idx, wl1_rfm_wakeup_chain_type chain ,rfm_path_type rfm_path, wsub_id_e_type wsub_id);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_prep_tune

DESCRIPTION
  This calls the new prep tune functionality. Takes channel number as a parameter apart from buffer index and chain type.
  Called only in idle mode, so the carrier index is always 1 in prep tune.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_prep_tune(uint16 channel,uint32 buf_idx, wl1_rfm_wakeup_chain_type chain ,rfm_device_enum_type device_id, wsub_id_e_type wsub_id);
#endif   /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_exec_tune

DESCRIPTION
  This calls the new exec tune functionality. Takes channel number as a parameter apart from buffer index and chain type.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_exec_tune
(
  rfm_path_type rfm_path, 
  uint16 *freq_tune_list, 
  uint8 tune_bmsk, 
  uint32 buf_idx,
  wl1_rfm_wakeup_chain_type chain, 
  wsub_id_e_type wsub_id
);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_exec_tune

DESCRIPTION
  This calls the new exec tune functionality. Takes channel number as a parameter apart from buffer index and chain type.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_exec_tune
(
  uint8 device_id,
  uint16 *freq_tune_list,
  uint8 tune_bmsk,
  uint32 buf_idx,
  wl1_rfm_wakeup_chain_type chain,
  wsub_id_e_type wsub_id
);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_sleep_div_chain

DESCRIPTION
  Put diversity RF to sleep

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_sleep_div_chain
(
  rfm_cb_handler_type cb_handler,
  uint32 buf_idx,
  rfm_path_type rfm_path,
  wsub_id_e_type wsub_id
);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_sleep_div_chain

DESCRIPTION
  Put diversity RF to sleep

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_sleep_div_chain
(
  rfm_cb_handler_type cb_handler,
  uint32 buf_idx,
  uint16 device_id,
  wsub_id_e_type wsub_id
);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_wakeup_div_chain

DESCRIPTION
  Wakeup diversity RF

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_wakeup_div_chain
(
  rfm_cb_handler_type cb_handler,
  uint32 buf_idx,
  rfm_path_type rfm_path,
  wsub_id_e_type wsub_id
);

#else   /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_wakeup_div_chain

DESCRIPTION
  Wakeup diversity RF

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_wakeup_div_chain
(
  rfm_cb_handler_type cb_handler,
  uint32 buf_idx,
  uint16 device_id,
  wsub_id_e_type wsub_id
);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_exit_mode

DESCRIPTION 
  Exit IMT mode
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_exit_mode(
    rfm_path_type rfm_path, 
    wsub_id_e_type wsub_id);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_exit_mode

DESCRIPTION 
  Exit IMT mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_exit_mode(uint16 device_id, wsub_id_e_type wsub_id);

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_exit_mode_tx_chain

DESCRIPTION 
  Exit IMT mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_exit_mode_tx_chain(wl1_ul_device_to_enter_exit_e_type device_type, wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_enter_mode

DESCRIPTION 
  Enter IMT mode
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_enter_mode(
  uint32 buf_idx,
  rfm_path_type rfm_path,
  wsub_id_e_type wsub_id
);

#else    /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_enter_mode

DESCRIPTION 
  Enter IMT mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_enter_mode
(
  uint32 buf_idx,
  uint16 device_id,
  wsub_id_e_type wsub_id
);
#endif    /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_tune_to_chan

DESCRIPTION
  Tune TX and RX to freq passed as parameter

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
uint32 wl1_rfm_tune_to_chan
(
  uint32 buf_idx,
  rfm_path_type rfm_path,
  rfm_synth_tune_enum_type rfm_tune_type,
  wl1_rfm_wakeup_chain_type chain,
  wsub_id_e_type wsub_id
);
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_tune_to_chan

DESCRIPTION
  Tune TX and RX to freq passed as parameter

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
uint32 wl1_rfm_tune_to_chan
(
  uint32 buf_idx,
  rfm_device_enum_type rf_device_id,
  rfm_synth_tune_enum_type rfm_tune_type,
  wl1_rfm_wakeup_chain_type chain,
  wsub_id_e_type wsub_id
);

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  WL1_RFM_ENTER_MODE_DIV_CHAIN

DESCRIPTION 
  Enter IMT mode on diversity branch
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_enter_mode_div_chain
(
  rfm_cb_handler_type cb_handler,
  uint32 buf_idx,
  rfm_path_type rfm_path,
  uint16 freq,
  boolean enable_rf,
  wsub_id_e_type wsub_id
);
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_enter_mode_div_chain

DESCRIPTION 
  Enter IMT mode on diversity branch

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_enter_mode_div_chain
(
  rfm_cb_handler_type cb_handler,
  uint32 buf_idx,
  uint16 device_id,
  uint16 freq,
  boolean enable_rf,
  wsub_id_e_type wsub_id
);

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_init_wcdma_tx

DESCRIPTION
  Initialization functionality related to W Tx is called here.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_init_wcdma_tx(wl1_rfcom_tx_multi_carrier_cfg_type carr_cfg, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_enable_tx

DESCRIPTION
  Enables transmitter for mode of operation RF driver is currently in.
  The index into txlm buffer needs to passed as part of NikeL

DEPENDENCIES
  None

RETURN VALUE
  Returns total delay in microseconds this function will wait for RF
  hardware to settle.

SIDE EFFECTS
  None

===========================================================================*/
boolean wl1_rfm_enable_tx(wl1_rfcom_tx_multi_carrier_cfg_type carr_cfg, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_disable_tx

DESCRIPTION
  Disables transmitter for mode of operation RF driver is currently
  operating in.

DEPENDENCIES
  rfm_enable_tx() should have already been called.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_disable_tx(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_decfg_tx

DESCRIPTION
  Txlm decfg for Chain 1 since RF maintains Txlm buffer for secondary Tx path.

DEPENDENCIES
  This function should be called before sending mdsp_sync_send_txlm_cfg_cmd (disable)
  to FW.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_wcdma_tx_deinit(wl1_ul_device_to_enter_exit_e_type device_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_RFM_DL_ARFCN_TO_CARRIER

DESCRIPTION
  This function gets translates ARFCN values into a carrier frequency, taking
  care of the dual carrier case when necessary.

DEPENDENCIES
  None

RETURN VALUE
  Uint32 value representing the carrier frequency in kHz

SIDE EFFECTS
  None
===========================================================================*/
uint32 wl1_rfm_dl_arfcn_to_carrier
(
  /* the arfcn(s)  index 1 stores the dual carrier frequency if present */
  uint16 arfcn[], 
  /* the number of carriers */
  uint8 num_car
);

/*===========================================================================
FUNCTION  wl1_rfm_get_irat_w2w_tune_times

DESCRIPTION
  This function returns the tune-away and tune-back times for a w2w search
  in microseconds.

DEPENDENCIES
  None

RETURN VALUE
  None.  Addresses of input arguments are filled in with tune times.

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_get_irat_w2w_tune_times
(
  /* address of the tune away time variable (in microseconds) */
  uint16 *ssbi_write_time, 
  /* address of the tune back time variable (in microseconds) */
  uint16 *pll_settling_time,
  /* address of a pll_settling_time variable (in microseconds)*/
  uint16 *agc_settling_time,
  /* address of a nbr_preload_offset_time variable (in microseconds)*/
  uint16 *nbr_preload_offset_time,
  /* address of a home_preload_offset_time variable (in microseconds)*/
  uint16 *home_preload_offset_time,
  wsub_id_e_type wsub_id
);

/*===========================================================================
FUNCTION  WL1_RFM_MEAS_COMMON_BUILD_SCRIPTS

DESCRIPTION
  The function sends a rfm_meas_common_script_build_scripts before a W2W gap search

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_meas_common_build_scripts(rfm_meas_common_param_type* common_ptr, wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
 FUNCTION  WL1_SET_RF_STATE

 DESCRIPTION
   API used to set the current RF state. RFM PATH specifies the RF chain -
   Primary or diversity

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_set_rf_state(wl1_rf_sleep_status_enum_type , wl1_rfm_path_type , wsub_id_e_type );
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
 FUNCTION  WL1_SET_RF_STATE

 DESCRIPTION
   API used to set the current RF state. Device ID specifies the RF chain -
   Primary or diversity

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_set_rf_state(wl1_rf_sleep_status_enum_type , wl1_rfm_device_type , wsub_id_e_type );
#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION  WL1_RFM_MEAS_COMMON_GET_IRAT_INFO

DESCRIPTION
  The function gets RF tune time (startup and cleanup).
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_meas_common_get_irat_info(rfm_meas_irat_info_type* rf_meas_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_sleep

DESCRIPTION
  Put primary RF to sleep

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_sleep
(
  uint32 buf_idx,
  wrm_resource_enum_type resource,
  wsub_id_e_type wsub_id
);

/*===========================================================================
FUNCTION  wl1_rfm_wakeup

DESCRIPTION
  Wakeup primary RF

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_wakeup
(
  rfm_cb_handler_type cb_handler,
  uint32 buf_idx,
  wl1_rfm_wakeup_chain_type chain,
  boolean prep_needed,
  wsub_id_e_type wsub_id
);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     wl1_dl_tune_to_chan

DESCRIPTION
  This function is used to tune RF to the desired freq

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_dl_tune_to_chan(
  uint32 buf_idx,
  rfm_path_type rfm_path,
  uint8 rf_tune_time,
  MDSP_WCDMA_L1_EVENT_CB_FUNC_TYPE *cb_func,
  wl1_rfm_wakeup_chain_type chain,
  wsub_id_e_type wsub_id
);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION     wl1_dl_carrcfg_tune_to_channel

DESCRIPTION
  This function is used to tune RF to the desired freq

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_dl_tune_to_chan(uint32 , rfm_device_enum_type , uint8 , MDSP_WCDMA_L1_EVENT_CB_FUNC_TYPE*, wl1_rfm_wakeup_chain_type , wsub_id_e_type );

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
 FUNCTION  wl1_rfm_wcdma_tx_retune_bw

 DESCRIPTION
   TX retune for DCHSUPA activation/de-activation.

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
boolean wl1_rfm_wcdma_tx_retune_bw(wl1_rfcom_tx_multi_carrier_cfg_type carr_cfg, wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
 FUNCTION  wl1_rfm_de_init_rf_path_db

 DESCRIPTION
   Init function for RF db

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_rfm_init_rf_path_db(wl1_rfm_path_type path_idx, wsub_id_e_type wsub_id);

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
 FUNCTION  wl1_rfm_de_init_rf_device_db

 DESCRIPTION
   Init function for RF db

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_rfm_init_rf_device_db(wl1_rfm_device_type device_idx, wsub_id_e_type wsub_id);

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
 FUNCTION  wl1_rf_ops_in_x2w_qta

 DESCRIPTION
   call for idle mode to build script and enter 

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_rf_ops_in_x2w_qta(
  wl1_dl_qta_rf_ops_enum_type rf_ops,
  rfm_meas_common_param_type *common_param_ptr,
  uint16 freq,
  wl1_rfm_wakeup_chain_type chain_type,
  wsub_id_e_type wsub_id
);

/*===========================================================================
 FUNCTION  wl1_get_x2w_qta_rf_state

 DESCRIPTION
   This function will return the current RF state of the sub being called

 DEPENDENCIES
   None

 RETURN VALUE
  wl1_dl_qta_rf_ops_enum_type
     WL1_DL_QTA_RF_START,
     WL1_DL_QTA_RF_ENTER,
     WL1_DL_QTA_RF_EXECUTE_SCRIPT,
     WL1_DL_QTA_RF_EXIT,
     WL1_DL_QTA_RF_OPS_DONE

 SIDE EFFECTS
   None
===========================================================================*/
wl1_dl_qta_rf_ops_enum_type wl1_get_x2w_qta_rf_state(wsub_id_e_type wsub_id);

/*===========================================================================
 FUNCTION  wl1_set_x2w_qta_rf_state

 DESCRIPTION
   This function will set the current RF state to what is being passed

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_set_x2w_qta_rf_state(wl1_dl_qta_rf_ops_enum_type rf_state, wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  WL1_RFM_GET_CURR_PRIMARY_RF_PATH_TUNED

DESCRIPTION 
  Returns the path on which we are tuned currently. Will return invalid
  device if we are asleep

DEPENDENCIES
  None

RETURN VALUE
  rfm_path_type

SIDE EFFECTS
  None
===========================================================================*/
rfm_path_type wl1_rfm_get_curr_primary_rf_path_tuned(wsub_id_e_type wsub_id);
#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  WL1_RFM_GET_CURR_PRIMARY_RF_DEVICE_TUNED

DESCRIPTION 
  Returns the device on which we are tuned currently. Will return invalid
  device if we are asleep

DEPENDENCIES
  None

RETURN VALUE
  rfm_device_enum_type

SIDE EFFECTS
  None
===========================================================================*/
rfm_device_enum_type wl1_rfm_get_curr_primary_rf_device_tuned(wsub_id_e_type wsub_id);
#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/* =======================================================================
**                        Inline Function Declarations
** ======================================================================= */

/*===========================================================================
FUNCTION wl1_rfm_set_txfreezeaccum

DESCRIPTION
  This function freeze or unfreeze Tx AGC accumulation
  Input values: 1 freeze, 0 unfreeze

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_set_txfreezeaccum(boolean val, wl1_ul_carr_id_enum_type carr_idx,
                                  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_rfm_get_txagctablestore

DESCRIPTION
  This function returns the contribution to the transmit power that reflects 
  the accumulation of UL TPC commands

DEPENDENCIES
  None

RETURN VALUE
  Tx AGC accumulation by TPC command

SIDE EFFECTS
  None
===========================================================================*/
  #ifdef __GNUC__
static inline 
#else 
INLINE 
#endif 
int16 wl1_rfm_get_txagctablestore(wl1_ul_carr_id_enum_type carr_idx)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  return rfm_get_txagctablestore((rfcom_multi_carrier_id_type)carr_idx);
#else
  return (0);
#endif
}

/*===========================================================================
FUNCTION wl1_rfm_convert_tx_agc_to_dbm_unit

DESCRIPTION
  This function converts Tx AGC from linear to dBm units

DEPENDENCIES
  None

RETURN VALUE
  dBm value of Tx AGC

SIDE EFFECTS
  None
===========================================================================*/
  #ifdef __GNUC__
static inline 
#else 
INLINE 
#endif 
int16 wl1_rfm_convert_tx_agc_to_dbm_unit(
  /* Tx AGC value in linear unit */
  int16 txagcval)
{
  #ifdef FEATURE_WCDMA_RUMI_SUPPORT
    return (-60);
  #else
    #ifdef FEATURE_WCDMA_ATLAS_MODEM
      return rfm_wcdma_tx_agc_to_dbm(txagcval);
    #else
      return rfm_convert_tx_agc_to_dbm_unit(txagcval);
    #endif /* FEATURE_WCDMA_ATLAS_MODEM */
  #endif
}

/*===========================================================================
FUNCTION wl1_rfm_convert_tx_dbm_to_agc_unit

DESCRIPTION
  This function converts Tx AGC dBm value to linear AGC units

DEPENDENCIES
  None

RETURN VALUE
  Tx AGC linear value

SIDE EFFECTS
  None
===========================================================================*/
  #ifdef __GNUC__
static inline 
#else 
INLINE 
#endif 
int16 wl1_rfm_convert_tx_dbm_to_agc_unit(
  /* Tx AGC value in dBm units */
  int16 dbmval)
{
  #ifdef FEATURE_WCDMA_RUMI_SUPPORT
    return (0);
  #else
    #ifdef FEATURE_WCDMA_ATLAS_MODEM
      return rfm_wcdma_tx_dbm_to_agc(dbmval);
    #else
      return rfm_convert_tx_dbm_to_agc_unit(dbmval);
    #endif /* FEATURE_WCDMA_ATLAS_MODEM */
  #endif
}

#ifdef FEATURE_WCDMA_MTPL_THERMAL_BACKOFF
/*===========================================================================
FUNCTION wl1_rfm_reduce_mptl_db_for_therm
         
DESCRIPTION
  This function calls RF API to reduce MTPL value in dB

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif 
void wl1_rfm_reduce_mptl_db_for_therm(uint8 backoff_db, wsub_id_e_type wsub_id)
{
  WL1_MSG1(HIGH, "MTPL_BACKOFF: Reducing MTPL by %d dB", backoff_db);

  #if defined(FEATURE_WCDMA_RUMI_SUPPORT) && (!defined(FEATURE_WCDMA_ATLAS_MODEM))
  #else
    #ifdef FEATURE_WCDMA_ATLAS_MODEM
      rfm_wcdma_tx_set_power_limit_backoff_param_type tx_params;
      rfm_api_status_t                                ret_val;

      #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
      tx_params.rfm_path            = wl1_trm_get_rf_path(WCDMA_TRM_TX, wsub_id);
      #else
      tx_params.rfm_path.rfm_device = wl1_trm_get_primary_tx_rf_device(wsub_id);
      #endif /* defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
      tx_params.tx_limit_backoff_db10 = (((uint16) backoff_db) * 10); //Convert to db10

      ret_val = rfm_wcdma_tx_set_power_limit_backoff(&tx_params);

      if (ret_val != RFM_WCDMA_HEALTHY_STATE)
      {
        WL1_MSG2(ERROR, "MTPL_BACKOFF: Incorrect value: %d dB, retVal=%d",
                 backoff_db, ret_val);
      }
    #else
      rfm_wcdma_reduce_mptl_db_for_therm(backoff_db,(rfm_wcdma_subscription_type)wsub_id);
    #endif /* FEATURE_WCDMA_ATLAS_MODEM */
  #endif /* FEATURE_WCDMA_RUMI_SUPPORT */
}
#endif /* FEATURE_WCDMA_MTPL_THERMAL_BACKOFF */

/*===========================================================================
FUNCTION wl1_rfm_turn_off_pa
         wl1_rfm_turn_on_pa

DESCRIPTION
  This function turns PA off or on

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_turn_off_pa(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_rfm_turn_on_pa
         wl1_rfm_turn_on_pa

DESCRIPTION
  This function turns on/off PA

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_turn_on_pa(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_rfm_pa_is_on

DESCRIPTION
  This function returns PA on status

DEPENDENCIES
  None

RETURN VALUE
  PA on/off status

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif 
uint8 wl1_rfm_pa_is_on(void)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  return rfm_pa_is_on();
#else
  return 1;
#endif
}

/*===========================================================================
FUNCTION wl1_rfm_enable_pwr_ctrl

DESCRIPTION
  This function enables power control

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif 
void wl1_rfm_enable_pwr_ctrl(void)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  rfm_enable_pwr_ctrl();
#endif
}

/*===========================================================================
FUNCTION wl1_rfm_get_ue_max_tx_power

DESCRIPTION
  This function return UE max power limit UE is capable of transmitting

DEPENDENCIES
  None

RETURN VALUE
  UE max power limit in dBm units

SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_ue_max_tx_power(rfcom_txplim_channel_type chan_type,
                                  wsub_id_e_type            wsub_id);

/*===========================================================================
FUNCTION wl1_rfm_get_min_tx_power_val

DESCRIPTION
  This function return UE min power limit.

DEPENDENCIES
  None

RETURN VALUE
  UE min power limit in dBm units

SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_min_tx_power_val(rfcom_txplim_channel_type chan_type,
                                   wsub_id_e_type            wsub_id);

/*===========================================================================
FUNCTION WL1_RFM_GET_UE_MAX_TX_POWER_IN_AGC

DESCRIPTION
  This function returns UE max power limit UE is capable of transmitting in 
  TX AGC units

DEPENDENCIES
  None

RETURN VALUE
  UE max power limit in TX AGC units

SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_ue_max_tx_power_in_agc(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_rfm_get_tx_rotator_angle

DESCRIPTION
  This function return UE tx rotator angle

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Pointer argument passed are updated
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif 
void wl1_rfm_get_tx_rotator_angle(
  uint16 *tx_rot_ang_pa_00_p,  /* Pointer to tx rotator angle pa state 00 */
  uint16 *tx_rot_ang_pa_01_p,  /* Pointer to tx rotator angle pa state 01 */
  uint16 *tx_rot_ang_pa_10_p,  /* Pointer to tx rotator angle pa state 10 */
  uint16 *tx_rot_ang_pa_11_p)   /* Pointer to tx rotator angle pa state 11 */
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  rfm_get_tx_rotator_angle(tx_rot_ang_pa_00_p, tx_rot_ang_pa_01_p,
                           tx_rot_ang_pa_10_p, tx_rot_ang_pa_11_p);
#endif
}

/*===========================================================================
FUNCTION  wl1_rfm_get_max_tx_pwr_limit

DESCRIPTION
  The function updates the passed rf_max_tx_pwr_limit_type structure with 
  MTPL (Max Tx Power Limit) values for difference BCS case in Tx AGC counts. 
  L1 should call this function on every frame boundary and update 
  the L1-DSP table with MTPL values.  

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Update MTPL structure passed as argument
===========================================================================*/
void wl1_rfm_get_max_tx_pwr_limit(rf_max_tx_pwr_limit_type *rf_mtpl,
                                  wsub_id_e_type            wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_get_cgagc_settling_time

DESCRIPTION
  The function gets CGAGC settling time
  
DEPENDENCIES
  None
  
RETURN VALUE
  CGAGC settling time
  
SIDE EFFECTS
  Nome
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_cgagc_settling_time(void)
{
  return 0;
}

/*===========================================================================
FUNCTION  wl1_rfm_get_rf_warmup_time

DESCRIPTION
  The function gets RF warmup time
  
DEPENDENCIES
  None
  
RETURN VALUE
  RF warmup time
  
SIDE EFFECTS
  Nome
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
uint16 wl1_rfm_get_rf_warmup_time(void)
{
  return rfm_get_rf_warmup_time();
}

/*===========================================================================
FUNCTION  wl1_rfm_enable_trk_lo_adj_pdm

DESCRIPTION
  The function enables/disables trk lo PDM adjust
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_enable_trk_lo_adj_pdm(
  /* indicate enable or disble TRK LO adj PDM */
  boolean enable)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  rfm_enable_trk_lo_adj_pdm(enable);
#endif
}

/*===========================================================================
FUNCTION  wl1_rfm_get_wtow_rf_ctl_buf

DESCRIPTION
  The function get inter-freq CM RF buffer for given freq
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  Update buffer pointer passed with RF setup prameters
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_get_wtow_rf_ctl_buf(
  /* inter-freq ARFCN */
  uint16 arfcn,
  /* RF setup buffer pointer */
  void *buf_ptr
  )
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  rfm_get_wtow_rf_ctl_buf(arfcn, buf_ptr);
#endif
   return;
}

#ifdef FEATURE_WCDMA_DC_HSDPA
/*===========================================================================
FUNCTION  wl1_rfm_get_multi_carrier_wtow_rf_ctl_buf

DESCRIPTION
  The function gets inter-freq CM RF buffer for given freqs. Supports DC
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  Update buffer pointer passed with RF setup prameters
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_get_multi_carrier_wtow_rf_ctl_buf(
  /* inter-freq ARFCN */
  uint16 *arfcn,
  /* Carrier type */
  rfcom_multi_carrier_hspa_id_type carrier_type,
  /* RF setup buffer pointer */
  void *buf_ptr
  )
{
  rfm_get_multi_carrier_wtow_rf_ctl_buf(arfcn, carrier_type, buf_ptr);
}
#endif /* FEATURE_WCDMA_DC_HSDPA */

/*===========================================================================
FUNCTION  wl1_rfm_wakeup_opt_status

DESCRIPTION
  Wakeup primary RF with/without optimization
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_wakeup_opt_status(boolean enable_opt, wsub_id_e_type wsub_id)
{
  WL1_MSG1(HIGH, "FR2001: Updating the RF opt status(%d) from L1", enable_opt);
  rfm_set_wakeup_optimization(enable_opt);
}

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_prep_wakeup_div_chain

DESCRIPTION
  prepares RF scripts prior to RF wakeup
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_prep_wakeup_div_chain(uint32 buf_idx, wsub_id_e_type wsub_id)
{
  rfm_path_type rfm_path_temp = wl1_trm_get_rf_path(WCDMA_TRM_SECONDARY, wsub_id);
  rfm_wcdma_prep_wakeup_rx(rfm_path_temp.rfm_device,buf_idx, NULL, NULL);
}

#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION  wl1_rfm_prep_wakeup_div_chain

DESCRIPTION
  prepares RF scripts prior to RF wakeup
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_prep_wakeup_div_chain(uint32 buf_idx, wsub_id_e_type wsub_id)
{
  rfm_wcdma_prep_wakeup_rx(wl1_trm_get_secondary_rf_device(wsub_id),buf_idx, NULL, NULL);
}
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION  wl1_rfm_get_rx_agc_val

DESCRIPTION
  Get rx AGV value

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_WCDMA_ATLAS_MODEM
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_rx_agc_val( uint8 rf_carr_idx, rfm_wcdma_rx_agc_type agc_type, rfm_wcdma_rx_agc_unit_type agc_unit, wsub_id_e_type wsub_id)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
  intf_param.rfm_path = wl1_trm_get_rf_path(WCDMA_TRM, wsub_id);
  #else
  intf_param.rfm_device = wl1_trm_get_primary_rf_device(wsub_id);
  #endif
  intf_param.carrier = rf_carr_idx;
  intf_param.agc_type = agc_type;
  intf_param.agc_unit = agc_unit;
  api_status = rfm_wcdma_rx_get_agc( &intf_param, &intf_out );
  if ( api_status == RFM_WCDMA_HEALTHY_STATE )
  {
    return intf_out.rx_agc;
  }
  else
  {
    return 0;
  }
#else
  return 0;
#endif
}
#else
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_rx_agc_val( wsub_id_e_type wsub_id)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  #ifdef FEATURE_MULTI_RF_CHAIN_API
  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
    return rfm_get_rx_agc_val(wl1_trm_get_rf_path(WCDMA_TRM, wsub_id));
  #else
  if (wl1_rxd_in_div_only_mode(wsub_id) == FALSE) 
  {
    return rfm_get_rx_agc_val(wl1_trm_get_primary_rf_device(wsub_id));
  }
  else
  {
    return rfm_get_rx_agc_val(wl1_trm_get_secondary_rf_device(wsub_id));
  }
  #endif
  #else
    return rfm_get_rx_agc_val();
  #endif
#else
  return 0;
#endif
}
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
/*===========================================================================
FUNCTION  wl1_rfm_get_tx_agc_val

DESCRIPTION
  Get Tx AGV value in dB units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_tx_agc_val(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_get_tx_agc_val_in_mdsp_units

DESCRIPTION
  Get the HDET corrected Tx AGV value in mdsp units 

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_tx_agc_val_in_mdsp_units(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_RFM_GET_CURRENT_PA_STATE

DESCRIPTION
  Get the PA state from RFA SW 

DEPENDENCIES
  None

RETURN VALUE
  uint8

SIDE EFFECTS
  None
===========================================================================*/
uint8 wl1_rfm_get_current_pa_state(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_RFM_CONVERT_CHAN_TO_BAND

DESCRIPTION
  The function converts a WCDMA channel into rfcom_wcdma_band_type.

DEPENDENCIES
  None
  
RETURN VALUE
  rfcom_wcdma_band_type
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
rfcom_wcdma_band_type wl1_rfm_convert_chan_to_band(uint16 freq)
{
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  return ((rfcom_wcdma_band_type)rfm_wcdma_convert_to_rx_band(freq));
#else
  return rfm_convert_chan_to_band(freq);
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
}

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_get_receive_agc_val_db12

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_agc_val_db10
(
  rfm_path_type rfcom_transceiver,
  uint8 carr_idx, 
  wsub_id_e_type wsub_id
)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  intf_param.rfm_path = rfcom_transceiver;
  intf_param.carrier = GET_CARR_ID_FOR_SUB(carr_idx);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DB10;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else
  return (-60);
#endif
}

#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_get_receive_agc_val_db12

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_agc_val_db10
(
  rfcom_device_enum_type rfcom_transceiver,uint8 carr_idx, wsub_id_e_type wsub_id
#ifndef FEATURE_WCDMA_ATLAS_MODEM
  ,rfm_wcdma_antenna_index ant_idx
#endif
)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  intf_param.rfm_device = rfcom_transceiver;
  intf_param.carrier = GET_CARR_ID_FOR_SUB(carr_idx);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DB10;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  rxagc_input_info.device = rfcom_transceiver;
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(carr_idx);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = ant_idx;

  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  } 
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
#else
  return (-60);
#endif
}
#endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_force_read_agc_val_db12

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/

#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_force_read_agc_val_db10
(
  rfm_path_type rfcom_transceiver, 
  uint8 carr_idx,
  wsub_id_e_type wsub_id
)
{
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  intf_param.rfm_path = rfcom_transceiver;
  intf_param.carrier = GET_CARR_ID_FOR_SUB(carr_idx);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL_FORCE_READ;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DB10;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
}
#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_force_read_agc_val_db12

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_force_read_agc_val_db10
(
  rfcom_device_enum_type rfcom_transceiver, uint8 carr_idx,wsub_id_e_type wsub_id
#ifndef FEATURE_WCDMA_ATLAS_MODEM
  ,rfm_wcdma_antenna_index ant_idx
#endif
)
{
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  intf_param.rfm_device = rfcom_transceiver;
  intf_param.carrier = GET_CARR_ID_FOR_SUB(carr_idx);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL_FORCE_READ;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DB10;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  rxagc_input_info.device = rfcom_transceiver;
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_FORCE_READ_RXAGC;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(carr_idx);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = ant_idx;

  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
}

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  WL1_RFM_GET_RECEIVE_AGC_VAL_FOR_ROUNDED_RSCP

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/

#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_agc_val_for_rounded_rscp
(
  rfm_path_type rfcom_transceiver, 
  uint8 carr_idx, 
  wsub_id_e_type wsub_id
)
{
  #ifndef FEATURE_WCDMA_RUMI_SUPPORT
      rfm_api_status_t api_status;
      rfm_wcdma_rx_get_agc_param_type intf_param;
      rfm_wcdma_rx_get_agc_output_type intf_out;
   
      intf_param.rfm_path   = rfcom_transceiver;
      intf_param.carrier    = GET_CARR_ID_FOR_SUB(carr_idx);
      intf_param.agc_type   = RFM_WCDMA_RX_AGC_SERVINGCELL_RAW;
      intf_param.agc_unit   = RFM_WCDMA_RX_AGC_DB10;
      api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
      return intf_out.rx_agc;
  #else
    return(-60*12);
  #endif
}
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  WL1_RFM_GET_RECEIVE_AGC_VAL_FOR_ROUNDED_RSCP

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_agc_val_for_rounded_rscp
(
  rfcom_device_enum_type rfcom_transceiver, uint8 carr_idx, wsub_id_e_type wsub_id
#ifndef FEATURE_WCDMA_ATLAS_MODEM
  ,rfm_wcdma_antenna_index ant_idx
#endif
)
{
  #ifndef FEATURE_WCDMA_RUMI_SUPPORT
    #ifdef FEATURE_WCDMA_ATLAS_MODEM
      rfm_api_status_t api_status;
      rfm_wcdma_rx_get_agc_param_type intf_param;
      rfm_wcdma_rx_get_agc_output_type intf_out;
   
      intf_param.rfm_device = rfcom_transceiver;
      intf_param.carrier    = GET_CARR_ID_FOR_SUB(carr_idx);
      intf_param.agc_type   = RFM_WCDMA_RX_AGC_SERVINGCELL_RAW;
      intf_param.agc_unit   = RFM_WCDMA_RX_AGC_DB10;
      api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
      return intf_out.rx_agc;
    #else
      rfm_api_status_t api_status;
      rfm_wcdma_rxagc_input_params rxagc_input_info;
      rfm_wcdma_rxagc_output_params rxagc_output_info;
    
      rxagc_input_info.device = rfcom_transceiver;
      rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC_RAW;
      rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(carr_idx);
      rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;
      rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
      rxagc_input_info.ant_idx = ant_idx;

    if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
    {
      api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
      return rxagc_output_info.rxagc_val;
    }
    else
    {
      api_status = -1;
      return (-512);
    }
    #endif /*FEATURE_WCDMA_ATLAS_MODEM*/
  #else
    return(-60*12);
  #endif
}

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  WL1_RFM_FORCE_READ_AGC_VAL_FOR_ROUNDED_RSCP

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/

#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_force_read_agc_val_for_rounded_rscp
(
  rfm_path_type rfcom_transceiver, 
  uint8 carr_idx, 
  wsub_id_e_type wsub_id
)
{
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  intf_param.rfm_path = rfcom_transceiver;
  intf_param.carrier = GET_CARR_ID_FOR_SUB(carr_idx);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL_FORCE_READ_RAW;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DB10;

  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
}
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION  WL1_RFM_FORCE_READ_AGC_VAL_FOR_ROUNDED_RSCP

DESCRIPTION
  The function get RF AGC sample logged for serving cell in db/12 units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_force_read_agc_val_for_rounded_rscp
(
  rfcom_device_enum_type rfcom_transceiver, uint8 carr_idx, wsub_id_e_type wsub_id
#ifndef FEATURE_WCDMA_ATLAS_MODEM
  ,rfm_wcdma_antenna_index ant_idx
#endif
)
{
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  intf_param.rfm_device = rfcom_transceiver;
  intf_param.carrier = GET_CARR_ID_FOR_SUB(carr_idx);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL_FORCE_READ_RAW;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DB10;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  rxagc_input_info.device = rfcom_transceiver;
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_FORCE_READ_RXAGC_RAW;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(carr_idx);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DB10;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = ant_idx;

  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
}

#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION  wl1_rfm_get_receive_agc_val_dbm

DESCRIPTION
  The function get RF AGC sample logged in db units

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_agc_val_dbm(uint8 carr_idx, wsub_id_e_type wsub_id)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  if (wl1_rxd_in_div_only_mode(wsub_id) == FALSE) 
  {
     #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
     intf_param.rfm_path = wl1_rfm_get_curr_primary_rf_path_tuned(wsub_id);
     #else
     intf_param.rfm_device = wl1_rfm_get_curr_primary_rf_device_tuned(wsub_id);
     #endif
  }
  else
  {
     #ifdef FEATURE_WCDMA_RF_USES_RFM_CONTEXT_INTERFACE
     intf_param.rfm_context = wl1_trm_get_rf_path(WCDMA_TRM_SECONDARY, wsub_id);
     #else  /*FEATURE_WCDMA_RF_USES_RFM_CONTEXT_INTERFACE*/
     intf_param.rfm_path = wl1_trm_get_rf_path(WCDMA_TRM_SECONDARY, wsub_id);
     #endif /*FEATURE_WCDMA_RF_USES_RFM_CONTEXT_INTERFACE*/
   }

  intf_param.carrier = GET_CARR_ID_FOR_SUB(carr_idx);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DBM;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else /* FEATURE_WCDMA_ATLAS_MODEM */
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  rxagc_input_info.device = wl1_rfm_get_curr_primary_rf_device_tuned(wsub_id);
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(carr_idx);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;

  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
#else /*FEATURE_WCDMA_RUMI_SUPPORT*/
  return (-60);
#endif /*FEATURE_WCDMA_RUMI_SUPPORT*/
}

/*===========================================================================
FUNCTION  wl1_rfm_force_read_agc_val_dbm

DESCRIPTION
  The function get RF AGC sample logged in db units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
 #ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_force_read_agc_val_dbm
(
  wsub_id_e_type wsub_id
)
{
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
  intf_param.rfm_path = wl1_rfm_get_curr_primary_rf_path_tuned(wsub_id);
  #else
  intf_param.rfm_device = wl1_rfm_get_curr_primary_rf_device_tuned(wsub_id);
  #endif
  intf_param.carrier = GET_CARR_ID_FOR_SUB(CARR_0);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL_FORCE_READ;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DBM;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else /*FEATURE_WCDMA_ATLAS_MODEM*/
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  rxagc_input_info.device = wl1_rfm_get_curr_primary_rf_device_tuned(wsub_id);
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_FORCE_READ_RXAGC;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(CARR_0);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
}

/*===========================================================================
FUNCTION  wl1_rfm_get_receive_div_agc_val_dbm

DESCRIPTION
  The function gets RF AGC sample logged in db units for diversity chain.
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_div_agc_val_dbm
(
  uint8 carr_idx, wsub_id_e_type wsub_id
)
{
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
  intf_param.rfm_path = wl1_trm_get_rf_path(WCDMA_TRM_SECONDARY, wsub_id);
  #else
  intf_param.rfm_device = wl1_trm_get_secondary_rf_device(wsub_id);
  #endif
  intf_param.carrier = GET_CARR_ID_FOR_SUB(CARR_0);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DBM;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else /*FEATURE_WCDMA_ATLAS_MODEM*/
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  rxagc_input_info.device = wl1_trm_get_secondary_rf_device(wsub_id);
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(carr_idx);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/

}

/*===========================================================================
FUNCTION  wl1_rfm_force_read_div_agc_val_dbm

DESCRIPTION
  The function gets RF AGC sample logged in db units for diversity chain.

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_force_read_div_agc_val_dbm
(
  wsub_id_e_type wsub_id
)
{
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  rfm_api_status_t api_status;
  rfm_wcdma_rx_get_agc_param_type intf_param;
  rfm_wcdma_rx_get_agc_output_type intf_out;

  #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
  intf_param.rfm_path = wl1_trm_get_rf_path(WCDMA_TRM_SECONDARY, wsub_id);
  #else
  intf_param.rfm_device = wl1_trm_get_secondary_rf_device(wsub_id);
  #endif
  intf_param.carrier = GET_CARR_ID_FOR_SUB(CARR_0);
  intf_param.agc_type = RFM_WCDMA_RX_AGC_SERVINGCELL_FORCE_READ;
  intf_param.agc_unit = RFM_WCDMA_RX_AGC_DBM;
  api_status = rfm_wcdma_rx_get_agc(&intf_param, &intf_out);
  return intf_out.rx_agc;
#else /*FEATURE_WCDMA_ATLAS_MODEM*/
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  rxagc_input_info.device = wl1_trm_get_secondary_rf_device(wsub_id);
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_FORCE_READ_RXAGC;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(CARR_0);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
  
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
}

/*===========================================================================
FUNCTION  wl1_rfm_get_receive_agc_val_for_rscp

DESCRIPTION
  The function get RF AGC sample logged for rscp, serving cell

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_agc_val_for_rscp(wsub_id_e_type wsub_id)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  #ifdef FEATURE_MULTI_RF_CHAIN_API
  #ifdef FEATURE_WCDMA_ATLAS_MODEM
  return wl1_rfm_get_rx_agc_val(CARR_0, RFM_WCDMA_RX_AGC_SERVINGCELL_RAW, RFM_WCDMA_RX_AGC_DBM, wsub_id);
  #else
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
  rxagc_input_info.path = wl1_rfm_get_curr_primary_rf_path_tuned(wsub_id);
  #else
  rxagc_input_info.device = wl1_rfm_get_curr_primary_rf_device_tuned(wsub_id);
  #endif
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_RXAGC_RAW;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(CARR_0);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
      
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
  #endif /*FEATURE_WCDMA_ATLAS_MODEM*/
  #else
  return rfm_get_receive_agc_val_for_rscp(RFCOM_SERVINGCELL_RXAGC);
  #endif
#else
  return(-60);
#endif
}

/*===========================================================================
FUNCTION  wl1_rfm_force_read_agc_val_for_rscp

DESCRIPTION
  The function get RF AGC sample logged for rscp, serving cell

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_force_read_agc_val_for_rscp( wsub_id_e_type wsub_id)
{
#ifdef FEATURE_MULTI_RF_CHAIN_API
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  return wl1_rfm_get_rx_agc_val(CARR_0, RFM_WCDMA_RX_AGC_SERVINGCELL_FORCE_READ_RAW, RFM_WCDMA_RX_AGC_DBM, wsub_id);
#else
  rfm_api_status_t api_status;
  rfm_wcdma_rxagc_input_params rxagc_input_info;
  rfm_wcdma_rxagc_output_params rxagc_output_info;

  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
  rxagc_input_info.path = wl1_rfm_get_curr_primary_rf_path_tuned(wsub_id);
  #else
  rxagc_input_info.device = wl1_rfm_get_curr_primary_rf_device_tuned(wsub_id);
  #endif
  
  rxagc_input_info.rxagc_type = RFCOM_SERVINGCELL_FORCE_READ_RXAGC_RAW;
  rxagc_input_info.car_id = GET_CARR_ID_FOR_SUB(CARR_0);
  rxagc_input_info.rxagc_unit = RFCOM_RXAGC_IN_DBM;
  rxagc_input_info.band = wl1_rfm_convert_chan_to_band(wl1_srch_get_curr_freq(wsub_id));
  rxagc_input_info.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
    
  if ((rxagc_input_info.device >= RFM_DEVICE_0) && (rxagc_input_info.device < RFM_WCDMA_MAX_DEVICE))
  {
    api_status = rfm_wcdma_get_rxagc_info(&rxagc_input_info, &rxagc_output_info);
    return rxagc_output_info.rxagc_val;
  }
  else
  {
    api_status = -1;
    return (-512);
  }
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
#else
  return rfm_get_receive_agc_val_for_rscp(RFCOM_SERVINGCELL_FORCE_READ_RXAGC);
#endif
}

/*===========================================================================
FUNCTION  wl1_rfm_get_receive_agc_val_for_rscp_ingap

DESCRIPTION
  The function get RF AGC sample logged for rscp measured in gap.

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_receive_agc_val_for_rscp_ingap( wsub_id_e_type wsub_id)
{
#ifdef FEATURE_MULTI_RF_CHAIN_API
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  return wl1_rfm_get_rx_agc_val(CARR_0, RFM_WCDMA_RX_AGC_INGAP_RAW, RFM_WCDMA_RX_AGC_DBM, wsub_id);
#else
  return rfm_get_receive_agc_val_for_rscp(wl1_trm_get_primary_rf_device(wsub_id), RFCOM_INGAP_RXAGC_RAW);
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
#else
  return rfm_get_receive_agc_val_for_rscp(RFCOM_INGAP_RXAGC);
#endif /* FEATURE_MULTI_RF_CHAIN_API */
}

/*===========================================================================
FUNCTION  wl1_rfm_log_single_agc_data

DESCRIPTION
  The function get RF AGC sample logged.

DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_rfm_log_single_agc_data( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION  wl1_rfm_set_tx_general_ctl

DESCRIPTION
  The function sets TX_GEN_CONTFOL register in H/W

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_set_tx_general_ctl(rfcom_txplim_channel_type chan_type)
{
  rfm_set_tx_general_ctl(chan_type);
}

/*===========================================================================
FUNCTION  wl1_rfm_get_max_tx_power_nv_item

DESCRIPTION
  The function get maximum TX power indicated by NV item depending on the
  channel type

DEPENDENCIES
  None

RETURN VALUE
  The max TX power value.

SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_max_tx_power_nv_item(rfcom_txplim_channel_type chan_type,
                                       wsub_id_e_type            wsub_id);
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION  wl1_rfm_get_max_tx_power_in_rf_sleep

DESCRIPTION
  The function get maximum TX power indicated by NV item depending on the
  channel type if rf is asleep
  
DEPENDENCIES
  None
  
RETURN VALUE
  The max TX power value.
  
SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_max_tx_power_in_rf_sleep(
                    rfm_path_type path,
                    rfcom_txplim_channel_type chan_type,
                    rfcom_wcdma_band_type     band,
                    wsub_id_e_type            wsub_id);
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION  wl1_rfm_get_max_tx_power_in_rf_sleep

DESCRIPTION
  The function get maximum TX power indicated by NV item depending on the
  channel type if rf is asleep
  
DEPENDENCIES
  None
  
RETURN VALUE
  The max TX power value.
  
SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_max_tx_power_in_rf_sleep(
                    rfm_device_enum_type device,
                    rfcom_txplim_channel_type chan_type,
                    rfcom_wcdma_band_type     band,
                    wsub_id_e_type            wsub_id);
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

/*===========================================================================
FUNCTION  wl1_rfm_get_curr_band

DESCRIPTION
  The function returns the current active WCDMA carrier band.  Possibilities
  are:

  SYS_BAND_WCDMA_I_IMT_2000  - WCDMA EUROPE JAPAN & CHINA IMT 2100 band
  SYS_BAND_WCDMA_II_PCS_1900 - WCDMA US PCS 1900 band
  SYS_BAND_WCDMA_III_1700    - WCDMA EUROPE&CHINA DCS 1800 band
  SYS_BAND_WCDMA_IV_1700     - WCDMA US 1700 band
  SYS_BAND_WCDMA_V_850       - WCDMA US 850 band
  SYS_BAND_WCDMA_VI_800      - WCDMA JAPAN 800 band
  SYS_BAND_WCDMA_VII_2600    - WCDMA EUROPE 2600 band
  SYS_BAND_WCDMA_VIII_900    - WCDMA EUROPE & CHINA 900 band
  SYS_BAND_WCDMA_IX_1700     - WCDMA JAPAN 1700 band
  
DEPENDENCIES
  None
  
RETURN VALUE
  sys_band_class_e_type
  
SIDE EFFECTS
  None
===========================================================================*/
extern sys_band_class_e_type wl1_rfm_get_curr_band(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_band_supports_rxd

DESCRIPTION
  The function return whether or not RxD is supported on the band passed to 
  this function

DEPENDENCIES
  None
  
RETURN VALUE
  boolean
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1_rfm_band_supports_rxd(sys_band_class_e_type sys_band, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_convert_sys_band_to_rf_card_band

DESCRIPTION
  The function converts a sys_band_class_e_type into rf_card_band_type.

DEPENDENCIES
  None
  
RETURN VALUE
  rf_card_band_type
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
rf_card_band_type wl1_rfm_convert_sys_band_to_rf_card_band(sys_band_class_e_type sys_band)
{
  return rfm_convert_sys_band_to_rf_card_band(sys_band);
}

/*===========================================================================
FUNCTION  wl1_rfm_convert_sys_band_to_rfcom_band

DESCRIPTION
  The function converts a sys_band_class_e_type into rfcom_wcdma_band_type.
  This is a temporary implementation until RF provides common API.

DEPENDENCIES
  None
  
RETURN VALUE
  rfcom_wcdma_band_type
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
rfcom_wcdma_band_type wl1_rfm_convert_sys_band_to_rfcom_band(sys_band_class_e_type sys_band)
{
  rfcom_wcdma_band_type band = RFCOM_BAND_INVALID;

  switch (sys_band)
  {
    case SYS_BAND_WCDMA_I_IMT_2000:
      band = RFCOM_BAND_IMT;
      break;
    
    case SYS_BAND_WCDMA_II_PCS_1900:
      band = RFCOM_BAND_1900;
      break;
    
    case SYS_BAND_WCDMA_III_1700:
      band = RFCOM_BAND_BC3;
      break;
    
    case SYS_BAND_WCDMA_IV_1700:
      band = RFCOM_BAND_BC4;
      break;
    
    case SYS_BAND_WCDMA_V_850:
      band = RFCOM_BAND_800;
      break;
    
    case SYS_BAND_WCDMA_VI_800:
      band = RFCOM_BAND_800;
      break;
    
    case SYS_BAND_WCDMA_VIII_900:
      band = RFCOM_BAND_BC8;
      break;
    
    case SYS_BAND_WCDMA_IX_1700:
      band = RFCOM_BAND_BC9;
      break;
    
    case SYS_BAND_WCDMA_XI_1500:
      band = RFCOM_BAND_BC11;
      break; 
    
    case SYS_BAND_WCDMA_XIX_850:
      band = RFCOM_BAND_BC19; 
      break; 

  default:
    break;
  }
  return band;
}
#if defined (FEATURE_WCDMA_BOLT_MODEM) && !(defined(FEATURE_WCDMA_THOR_MODEM))
/*===========================================================================
FUNCTION  wl1_rfm_convert_sys_band_to_rf_card_band

DESCRIPTION
  The function converts a sys_band_class_e_type into wl1_rf_card_band_enum_type.

DEPENDENCIES
  None
  
RETURN VALUE
  wl1_rf_card_band_enum_type
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
rfcom_wcdma_band_type wl1_rfm_convert_sys_band_to_rfcom_band(sys_band_class_e_type sys_band)
{
  return rfm_wcdma_convert_sys_band_to_rfcom_band(sys_band);
}
#endif

/*===========================================================================
FUNCTION  wl1_rfm_get_enc_btf_val

DESCRIPTION
  The function get BTF delay value
  
DEPENDENCIES
  None
  
RETURN VALUE
  BTF delay value
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
uint16 wl1_rfm_get_enc_btf_val(wsub_id_e_type wsub_id)
{
  #ifdef FEATURE_WCDMA_RUMI_SUPPORT
    return 31;
  #else
    #ifdef FEATURE_WCDMA_ATLAS_MODEM
      rfm_wcdma_tx_get_btf_param_type  tx_in_params;
      rfm_wcdma_tx_get_btf_output_type tx_out_params;

      tx_in_params.wcdma_band   = (rfm_wcdma_band_type) wl1_rfm_convert_sys_band_to_rfcom_band(wl1_rfm_get_curr_band(wsub_id));
      #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
        tx_in_params.rfm_path   = wl1_trm_get_rf_path(WCDMA_TRM, wsub_id);
      #else /*FEATURE_WCDMA_RFM_PATH_SUPPORTED*/
        tx_in_params.rfm_device = wl1_trm_get_primary_tx_rf_device(wsub_id);
      #endif/*FEATURE_WCDMA_RFM_PATH_SUPPORTED*/

      rfm_wcdma_tx_get_btf(&tx_in_params, &tx_out_params);

      return tx_out_params.enc_btf;
    #else
      rfcom_wcdma_band_type  rf_com_band;
      rf_com_band = wl1_rfm_convert_sys_band_to_rfcom_band(wl1_rfm_get_curr_band(wsub_id));
      return rfm_wcdma_get_enc_btf_val(rf_com_band);
    #endif /* FEATURE_WCDMA_ATLAS_MODEM */
  #endif /*FEATURE_WCDMA_RUMI_SUPPORT*/
}

/*===========================================================================
FUNCTION  wl1_rfm_get_DM_required_for_HDET

DESCRIPTION
  The function queries RF to see if the Data Mover clock is required for HDET
  operation.
  
DEPENDENCIES
  None
  
RETURN VALUE
  TRUE if DM is required for HDET operation, FALSE otherwise
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
boolean wl1_rfm_get_DM_required_for_HDET( wsub_id_e_type wsub_id)
{
#ifdef FEATURE_WCDMA_RUMI_SUPPORT
  WL1_MSG0(ERROR, "CODE NOT PRESENT");
#else
  return rfm_get_rf_capability( (rfcom_rf_cap_type)RF_CAP_DM_FOR_HDET );
#endif /* FEATURE_WCDMA_RUMI_SUPPORT */
  return FALSE;
}

/*===========================================================================
FUNCTION  wl1_rfm_set_tx_pwr_limit

DESCRIPTION
  The function sets RF TX power limit as RACH for IDLE states

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_rfm_set_tx_pwr_limit(
  int16 tx_limit_dbm,
  rfcom_txplim_channel_type chan_type,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_wcdma_tx_post_sleep

DESCRIPTION
  API used for WCDMA Wakeup.

DEPENDENCIES
  None
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean wl1_rfm_wcdma_tx_post_sleep(wl1_rfcom_tx_multi_carrier_cfg_type carr_cfg, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_prep_init_wcdma_tx

DESCRIPTION
  Initialization functionality related to W Tx is called here.
  Specific to DC HSUPA 
DEPENDENCIES
  None
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_prep_init_wcdma_tx(uint32 multi_carr_idx, wsub_id_e_type wsub_id)
{
   int16 ret_val = 0;

#ifdef FEATURE_WCDMA_BOLT_2_0
#else
   ret_val = (int16) rfm_wcdma_prep_init_tx(
                     RFM_DEVICE_0,
                     wl1_xlm_get_txlm_buf(WL1_XLM_TXLM_CHAIN_0 ,wsub_id),
                     multi_carr_idx,
                     NULL,
                     NULL
                     );
#endif /* FEATURE_WCDMA_BOLT_2_0 */
   return ret_val;
}

/*===========================================================================
FUNCTION  wl1_rfm_set_dsr_mode

DESCRIPTION
  Set DSR mode to RF

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wl1_rfm_set_dsr_mode(rfcom_lpm_config_enum_type lpm_mode, wl1_xlm_rx_client_enum_type rxlm_c0, 
                             wl1_xlm_rx_client_enum_type rxlm_c1, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  wl1_rfm_get_rf_capability_for_lna_switch() 

DESCRIPTION
  This function returns whether LNA switchpoint updates are 
  required by the current RF target.
    
DEPENDENCIES
  rf_init is called.
  
RETURN VALUE
  TRUE if feature is needed or enabled. FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
boolean wl1_rfm_get_rf_capability_for_lna_switch(void)
{
 #if defined FEATURE_RFM_INTERFACE
  return rfm_get_rf_capability(RF_CAP_TECH_DEPENDENT_LNA_SWITCH);
 #else
  return FALSE;
 #endif
}

/*===========================================================================
FUNCTION  wl1_rfm_get_tx_pwr_limit

DESCRIPTION
  Get Tx AGV value in dB units
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
int16 wl1_rfm_get_tx_pwr_limit(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_RFM_GET_HDET_APPLY_THRESHOLD_AGC

DESCRIPTION
  The function gets the threshold in TX AGC unit to apply HDET correction to
  Tx power
  
DEPENDENCIES
  None
  
RETURN VALUE
  the threshold in TX AGC unit
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int16 wl1_rfm_get_hdet_apply_threshold_agc(void)
{
  return rfm_get_hdet_apply_threshold_agc(); 
}

/*===========================================================================
FUNCTION  WL1_RFM_MEAS_COMMON_ENTER

DESCRIPTION
  The function sends a rfm_meas_common_enter before search
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_meas_common_enter(
  rfm_meas_common_param_type *common_param_ptr,
  wsub_id_e_type wsub_id)
{
  rfm_meas_enter_param_type wcdma_enter;

  wcdma_enter.enter_params = common_param_ptr;
  rfm_meas_common_script_enter(&wcdma_enter, NULL);
}

/*===========================================================================
FUNCTION  WL1_RFM_MEAS_COMMON_EXIT

DESCRIPTION
  The function sends a rfm_meas_common_exit after search
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_meas_common_exit(
  rfm_meas_common_param_type* common_param_ptr, 
  boolean gap_aborted, wsub_id_e_type wsub_id)
{
  rfm_meas_exit_param_type wcdma_exit;

  wcdma_exit.exit_params = common_param_ptr;
  wcdma_exit.is_measurement_gap_aborted = gap_aborted;

  rfm_meas_common_script_exit(&wcdma_exit, NULL);
}

/*===========================================================================
FUNCTION  wl1_rfm_get_freq_from_dl_arfcn

DESCRIPTION
  Get freq from arfcn
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
int32 wl1_rfm_get_freq_from_dl_arfcn(uint16 freq_param)
{
#ifdef FEATURE_WCDMA_ATLAS_MODEM
  return  (rfm_wcdma_convert_to_rx_frequency(freq_param));
#else
  return(rfm_get_freq_from_dl_arfcn(freq_param));
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
}

/*===========================================================================
FUNCTION  wl1_rfm_get_ul_freq_from_dl_arfcn

DESCRIPTION
  Get ul freq from dl uarfcn
  
DEPENDENCIES
  None
  
RETURN VALUE
  int
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
uint32 wl1_rfm_get_ul_freq_from_dl_arfcn(sys_band_class_e_type sys_band, uint16 freq_param)
{
  return(rfm_get_ul_freq_from_dl_uarfcn(sys_band, freq_param));
}

#ifdef FEATURE_WCDMA_TO_LTE
/*===========================================================================
 FUNCTION  wl1_rfm_convert_rx_earfcn_to_freq

 DESCRIPTION
   Returns the carrier frequency, in kHz for the given Rx EARFCN
  
 DEPENDENCIES
   None
  
 RETURN VALUE
   The carrier frequency, in kHz, as an unsigned 32-bit integer
  
 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
uint32 wl1_rfm_convert_rx_earfcn_to_freq(uint32 earfcn)
{
  /* This parameter is unused, but we have to pass in something. */
  rfcom_band_type_u band;
  band.lte_band = RFCOM_BAND_LTE_INVALID;

  return rfm_get_rx_freq_from_rx_earfcn(earfcn, band, RFM_LTE_MODE);
}
#endif /* FEATURE_WCDMA_TO_LTE */

/*===========================================================================
 FUNCTION  wl1_rfm_register_cpc_drx_state

 DESCRIPTION
   This API is used to inform RF state when CPC-DRX or E-DRX is active

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_register_cpc_drx_state(boolean rtr_ctrl_state)
{
  rfm_register_cpc_drx_state(rtr_ctrl_state);
}
#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     WL1_RFM_WCDMA_GET_NBR_PHY_DEV_IND

DESCRIPTION
  This function calls RF API to get a proper nbr dev index, and it will be sent 
  to FW by cm_init cmd to know which dev will be used to tune to the nbr freq.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  uint8 - WTR dev index used for nbr freq

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
uint8 wl1_rfm_wcdma_get_nbr_phy_dev_ind(
  uint16 freq, 
  rfm_path_type path
)
{
  return rfm_wcdma_get_nbr_phy_dev_ind(freq, path.rfm_device);
}
#else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION     WL1_RFM_WCDMA_GET_NBR_PHY_DEV_IND

DESCRIPTION
  This function calls RF API to get a proper nbr dev index, and it will be sent 
  to FW by cm_init cmd to know which dev will be used to tune to the nbr freq.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  uint8 - WTR dev index used for nbr freq

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
uint8 wl1_rfm_wcdma_get_nbr_phy_dev_ind(
  uint16 freq, 
  rfm_device_enum_type device
)
{
  return rfm_wcdma_get_nbr_phy_dev_ind(freq, device);
}
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     WL1_RFM_UPDATE_RXLM_BUFFER

DESCRIPTION
  This function calls rfm_update_rxlm_buffer to update the neighbor rxlm buffer contents.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
boolean wl1_rfm_update_rxlm_buffer(rfm_path_type path,
                                   uint16 *interf_arfcn,
                                   uint32 rxlm_buf_idx)
{
  return rfm_update_rxlm_buffer(path.rfm_device, 
                                interf_arfcn, 
                                RFCOM_SINGLE_CARRIER, 
                                rxlm_buf_idx);
}

#else /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
FUNCTION     WL1_RFM_UPDATE_RXLM_BUFFER

DESCRIPTION
  This function calls rfm_update_rxlm_buffer to update the neighbor rxlm buffer contents.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
boolean wl1_rfm_update_rxlm_buffer(rfm_device_enum_type device,
                                   uint16 *interf_arfcn,
                                   uint32 rxlm_buf_idx)
{
  return rfm_update_rxlm_buffer(device, 
                                interf_arfcn, 
                                RFCOM_SINGLE_CARRIER, 
                                rxlm_buf_idx);
}
#endif  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
/*===========================================================================
 FUNCTION  wl1_rfm_wcdma_antenna_tuner_tick

 DESCRIPTION
   Tells RF its time for them to make a small tuner adjustment.  Should be 
   called periodically by the wl1 ATuner module.
  
 DEPENDENCIES
   None
  
 RETURN VALUE
   void
  
 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_wcdma_antenna_tuner_tick(void)
{
  rfm_wcdma_antenna_tuner_tick();
}

#ifdef FEATURE_WCDMA_ATLAS_MODEM
/*===========================================================================
 FUNCTION  wl1_map_sys_sband_e_type_to_sys_band_class_e_type

 DESCRIPTION
   The function maps sys_sband_e_type used by policyman to 
   sys_band_class_e_type used by TRM

 DEPENDENCIES
   None

 RETURN VALUE
   sys_band_class_e_type: Mapped band

 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
sys_band_class_e_type  wl1_map_sys_sband_e_type_to_sys_band_class_e_type(sys_sband_e_type sband)
{
  switch(sband)
  {
    case WCMDA_SYS_SBAND_WCDMA_I_IMT_2000: return SYS_BAND_WCDMA_I_IMT_2000;
    /**< WCDMA Europe, Japan, and China IMT 2100 band. */

    case WCMDA_SYS_SBAND_WCDMA_II_PCS_1900: return SYS_BAND_WCDMA_II_PCS_1900;
    /**< WCDMA US PCS 1900 band. */

    case WCMDA_SYS_SBAND_WCDMA_III_1700: return SYS_BAND_WCDMA_III_1700;
    /**< WCDMA Europe and China DCS 1800 band. */

    case WCMDA_SYS_SBAND_WCDMA_IV_1700: return SYS_BAND_WCDMA_IV_1700;
    /**< WCDMA US 1700 band. */

    case WCMDA_SYS_SBAND_WCDMA_V_850: return SYS_BAND_WCDMA_V_850;
    /**< WCDMA US850 band. */

    case WCMDA_SYS_SBAND_WCDMA_VI_800: return SYS_BAND_WCDMA_VI_800;
    /**< WCDMA Japan 800 band. */

    case WCMDA_SYS_SBAND_WCDMA_VII_2600: return SYS_BAND_WCDMA_VII_2600;
    /**< WCDMA Europe 2600 band. */

    case WCMDA_SYS_SBAND_WCDMA_VIII_900: return SYS_BAND_WCDMA_VIII_900;
    /**< WCDMA Europe and Japan 900 band. */

    case WCMDA_SYS_SBAND_WCDMA_IX_1700: return SYS_BAND_WCDMA_IX_1700;
    /**< WCDMA Japan 1700 band. */

    case WCMDA_SYS_SBAND_WCDMA_XIX_850: return SYS_BAND_WCDMA_XIX_850;
    /**< WCDMA Japan 850 band. */

    case WCMDA_SYS_SBAND_WCDMA_XI_1500: return SYS_BAND_WCDMA_XI_1500;
    /**< WCDMA 1500 band.  */

    default : return SYS_BAND_CLASS_MAX;
  }
}

/*===========================================================================
 FUNCTION  wl1_rfm_get_default_band_chan

 DESCRIPTION
   Returns one of the default band supported on the device
 
 DEPENDENCIES
   None

 RETURN VALUE
   sys_band_class_e_type: default band or MAX value in case we don't find any

 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
sys_band_class_e_type wl1_rfm_get_default_band_chan(wsub_id_e_type wsub_id)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT
  sys_band_mask_type       pGwBands;
  uint8 wcdma_sband = WCMDA_SYS_SBAND_MAX;

  policyman_get_hardware_rats_bands(NULL, &pGwBands, NULL, NULL);

  if ((WCDMA_SBAND_BMSK & pGwBands) != 0)
  {
    for(wcdma_sband = (uint8)WCMDA_SYS_SBAND_WCDMA_I_IMT_2000; wcdma_sband <= (uint8)WCMDA_SYS_SBAND_WCDMA_XI_1500; wcdma_sband++)
    {
      if(SYS_BM_64BIT(wcdma_sband) & (WCDMA_SBAND_BMSK & pGwBands	))
      {
        break;
      }
    }
  }

  WL1_MSG3(HIGH, "Wl1_rfm: Using sband %d as default band. W bands supported: First 32 bits 0x%x Last 32 bits 0x%x", wcdma_sband, (uint32)(((WCDMA_SBAND_BMSK & pGwBands) & 0xffffffff00000000) >> 32 ), (uint32)((WCDMA_SBAND_BMSK & pGwBands) & 0x00000000ffffffff));

  return wl1_map_sys_sband_e_type_to_sys_band_class_e_type(wcdma_sband);
#else
  return SYS_BAND_WCDMA_I_IMT_2000;
#endif /* FEATURE_WCDMA_RUMI_SUPPORT */
}

#else
/*===========================================================================
 FUNCTION  wl1_rfm_get_default_band_chan

 DESCRIPTION
   get the default band and channel info.
 
 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_get_default_band_chan(rfi_band_type *band, uint16 *chan)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT  
  rfm_wcdma_get_default_band_chan(band, chan);
#else
  *band = RFI_IMT_BAND; 
  *chan = 10700;
#endif
}

#endif /* FEATURE_WCDMA_ATLAS_MODEM */
/*===========================================================================
FUNCTION  wl1_rfm_perp_temp_comp

DESCRIPTION
  This function will prepare RF for temperature compensation by performing static update. 
  This update includes the reading of current temparature and applying the temperature 
  specific compensation to Tx linearizer tables. This function calls a CB function at the completion
  of the computations and L1 will trigger Part 2 temp comp based on this.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Will call CB function specified by L1, which in turn will trigger Part2 Temp Comp

===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_prep_temp_comp(rfm_cb_handler_type cb_handler, wsub_id_e_type wsub_id)
{
  #ifdef FEATURE_WCDMA_RUMI_SUPPORT
  #else
    rfm_wcdma_temp_comp_param intf_param;

    #ifdef FEATURE_WCDMA_ATLAS_MODEM
    #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
      intf_param.rfm_path = wl1_trm_get_rf_path(WCDMA_TRM_TX, wsub_id);
    #else
      intf_param.rfm_device = wl1_trm_get_primary_tx_rf_device(wsub_id);
    #endif
    #else
      intf_param.device = wl1_trm_get_primary_tx_rf_device(wsub_id);
    #endif /* FEATURE_WCDMA_ATLAS_MODEM */
  
    rfm_wcdma_prep_temp_comp(&intf_param, cb_handler, NULL);
  #endif
}

/*===========================================================================
FUNCTION  wl1_rfm_exec_temp_comp

DESCRIPTION
  This function will perform dynamic update to WFW for RF TX Temp Comp which
  includes performing rxlm dynamic update to FW to perform temperature based IQMC.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Calls MPSP Sync function to update WFW for the RF Temp Comp Procedure

===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_exec_temp_comp( wsub_id_e_type wsub_id)
{
  #ifdef FEATURE_WCDMA_RUMI_SUPPORT
  #else
    rfm_wcdma_temp_comp_param intf_param;

    #ifdef FEATURE_WCDMA_ATLAS_MODEM
    #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
      intf_param.rfm_path = wl1_trm_get_rf_path(WCDMA_TRM_TX, wsub_id);
    #else
      intf_param.rfm_device = wl1_trm_get_primary_tx_rf_device(wsub_id);
    #endif
    #else
      intf_param.device = wl1_trm_get_primary_tx_rf_device(wsub_id);
    #endif /* FEATURE_WCDMA_ATLAS_MODEM */
  
    rfm_wcdma_exec_temp_comp(&intf_param, NULL, NULL);
  #endif
}

/*===========================================================================
FUNCTION  wl1_rfm_abort_temp_comp

DESCRIPTION
  Aborts the temp comp computation that is performed as part of rfm_wcdma_prep_temp_comp()

DEPENDENCIES
  rfm_wcdma_prep_temp_comp() must have been called before calling this API

RETURN VALUE
  None

SIDE EFFECTS
  Will call rfm_wcdma_abort_temp_comp which will abort the RF Temp Comp Part1 Computations
  performed in rfm_wcdma_prep_temp_comp()

===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_abort_temp_comp( wsub_id_e_type wsub_id)
{
  #ifdef FEATURE_WCDMA_RUMI_SUPPORT
  #else
    rfm_wcdma_temp_comp_param intf_param;

    #ifdef FEATURE_WCDMA_ATLAS_MODEM
    #ifdef FEATURE_WCDMA_RFM_PATH_SUPPORTED
      intf_param.rfm_path = wl1_trm_get_rf_path(WCDMA_TRM_TX, wsub_id);
    #else
      intf_param.rfm_device = wl1_trm_get_primary_tx_rf_device(wsub_id);
    #endif
    #else
      intf_param.device = wl1_trm_get_primary_tx_rf_device(wsub_id);
    #endif /* FEATURE_WCDMA_ATLAS_MODEM */
  
    rfm_wcdma_abort_temp_comp(&intf_param, NULL, NULL);
  #endif
}

/*===========================================================================
 FUNCTION  wl1_rfm_flush_agc_data

 DESCRIPTION
   Flushes AGC data collected till this point. Function will be called 
   after a long tune away in dsds mode.

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_flush_agc_data(wsub_id_e_type wsub_id)
{
#ifndef FEATURE_WCDMA_RUMI_SUPPORT  
#if defined FEATURE_WCDMA_ATLAS_MODEM  || defined FEATURE_WCDMA_THOR_MODEM || defined FEATURE_WCDMA_TABASCO_MODEM
  rfm_wcdma_log_agc_flush(wl1rfmif_int_cntrl_params[wsub_id].wl1_rfm_log_agc_handle);
  WL1_MSG1(HIGH, "Flushing Log Packet, SUB:%d", wsub_id);
#else
  rfm_flush_agc_data(RFCOM_TRANSCEIVER_0);
#endif /*FEATURE_WCDMA_ATLAS_MODEM*/
#endif
}
/*===========================================================================
 FUNCTION  wl1_rfm_set_nbr_enable

 DESCRIPTION
   Enable NBR config to RF and send the RxLM buffer index.

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
extern void wl1_rfm_set_nbr_enable(boolean nbr_enable, uint32 rxlm_buf_idx, wsub_id_e_type wsub_id);
/*===========================================================================
 FUNCTION  wl1_skip_agc_disable_cmd

 DESCRIPTION
   API sets the flag used by RF to skip sending AGC cmd to FW 
   in case of early RF shutdown

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_skip_agc_disable_cmd(boolean skip_agc, wsub_id_e_type wsub_id)
{
  wl1rfmif_int_cntrl_params[wsub_id].wl1_skip_sending_agc_disable = skip_agc;
}

/*===========================================================================
 FUNCTION  wl1_get_txagc_params

 DESCRIPTION
   

 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
void wl1_get_txagc_params(wl1_txagc_params_type *txagc_info, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WL1_RFM_SET_RF_POWER_ON_NEEDED

DESCRIPTION 
  Exit IMT mode
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
void wl1_rfm_set_rf_power_on_needed(boolean rf_power_on,wsub_id_e_type wsub_id)
{
  wl1rfmif_int_cntrl_params[wsub_id].wl1_rfm_power_on_needed = rf_power_on;
}

/*===========================================================================
FUNCTION  WL1_RFM_GET_RF_POWER_ON_NEEDED

DESCRIPTION 
  Exit IMT mode
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
#ifdef __GNUC__
static inline 
#else 
INLINE 
#endif
boolean wl1_rfm_get_rf_power_on_needed(wsub_id_e_type wsub_id)
{
  return(wl1rfmif_int_cntrl_params[wsub_id].wl1_rfm_power_on_needed);
}


#ifdef FEATURE_WCDMA_ATLAS_MODEM
/*===========================================================================
FUNCTION     wl1_rfm_wcdma_tx_get_agc

DESCRIPTION  Get TXAGC params:
               tx_agc_db10
               tx_agc_dbm
               xpt_mode
               pa_range
  
DEPENDENCIES None
  
RETURN VALUE BOOLEAN
  
SIDE EFFECTS None
===========================================================================*/
boolean wl1_rfm_wcdma_tx_get_agc(rfm_wcdma_tx_get_agc_output_type *tx_agc_data,
                                 wsub_id_e_type                    wsub_id);

/*===========================================================================
FUNCTION     wl1_rfm_wcdma_tx_get_power_limit

DESCRIPTION  Get MTPL params:
               min_tx_power_db10
               mtpl_desired_dbm
               mtpl_nv_dbm
               mtpl_rf
               mtpl_mdsp
               mtpl_bcs
  
DEPENDENCIES None
  
RETURN VALUE BOOLEAN
  
SIDE EFFECTS None
===========================================================================*/
boolean wl1_rfm_wcdma_tx_get_power_limit(rfm_wcdma_tx_get_power_limit_output_type *power_limit_vals,
                                         rfm_wcdma_tx_protocol_state_type          chan_type,
                                         wsub_id_e_type                            wsub_id);
#endif /* FEATURE_WCDMA_ATLAS_MODEM */

#if defined(FEATURE_WCDMA_RUMI_SUPPORT) && defined(FEATURE_WCDMA_ATLAS_MODEM)
/*===========================================================================
 FUNCTION     wl1_rflm_dtr_rx_config_pbs

 DESCRIPTION  This configures PBS_CTL_ADDR to enable IQ or Y1Y2 streaming,
              depending on what 'rflm_dtr_pbs_cfg' is set to.
                rflm_dtr_pbs_cfg = 0x0:  -> Do nothing
                rflm_dtr_pbs_cfg = 0x1:  -> RUMI IQ streaming
                rflm_dtr_pbs_cfg = 0x2:  -> RUMI Y1Y2 streaming - bitexact
                rflm_dtr_pbs_cfg = 0x3:  -> RUMI Y1Y2 streaming - nonbitexact
                rflm_dtr_pbs_cfg = 0x10: -> XXX TODO Update for CDP once settings are available
                rflm_dtr_pbs_cfg = 0x11: -> XXX TODO Update for CDP once settings are available
                rflm_dtr_pbs_cfg = 0x20: -> RUMI TX flex capture

 DEPENDENCIES This needs to be called after "wl1_enable_clocks".

 RETURN VALUE None

 SIDE EFFECTS None
===========================================================================*/
void wl1_rflm_dtr_rx_config_pbs(wsub_id_e_type wsub_id);
#endif /* defined(FEATURE_WCDMA_RUMI_SUPPORT) && defined(FEATURE_WCDMA_ATLAS_MODEM) */

/*===========================================================================
FUNCTION     wl1_rfm_wcdma_maat_parm_update

DESCRIPTION  This function fills the parameters for MAAT and call RF API accordingly

DEPENDENCIES 

RETURN VALUE None

SIDE EFFECTS None
===========================================================================*/

extern void wl1_rfm_wcdma_maat_parm_update(uint16 ecio_prim_c0,uint16 ecio_div_c0,uint16 ecio_prim_c1,uint16 ecio_div_c1 , boolean is_div_enable, wsub_id_e_type wsub_id);

#endif /* FEATURE_WCDMA */
#endif /* WL1RFMIF_H */
