/*!
   @file
   rfgsm_mc.c

   @brief
   This file contains all type declarations and definitions for the GSM Main
   Control layer.


   @details

*/

/*===========================================================================
Copyright (c) 1999 - 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/rf/mc/src/rfgsm_mc.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/23/19   mpa     Enable therm read in HA
12/09/19   mpa     Disable therm read in HA
05/22/17   mpa     [CR2066053] GSM/EDGE Modulation Type Issue on PA driver
02/03/17   bp      Use valid device info in rfgsm_mc_irat_info_get()
03/08/16   dsm     Report to RFM layer to disable asdiv switching on specific sar code
10/27/15   cs      Mutex protect iRAT meas enter/exit to avoid overlap
09/18/15   sc      Tx burst execution time optimisations
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
08/18/15   ec      Deprecate unused API to update SAWless lin state in GL1 params
08/12/15   sc      Add split Rx/Tx state machines - part 1 (deprecation)
08/03/15   hk      Return zero as default ccs_exe_us for invalid devices
08/05/15   sc      Deprecate alternate path
08/05/15   ec      Remove device dependent code from CM Enter\Exit
07/07/15   sc      Further Rx burst optimisations (~20% @ 144MHz)
07/03/15   sc      Fix LIF and enable optimised Rx burst processing
                   Temporarily compiled out old functionality
07/02/15   ec      Return SAWless support back to layer 1 after GSM start
05/26/15   tsr     Temporarily pass the RX chan params using local structure until GL1 replaces
                   old RXD chan params structure with RX chan params as part of RX burst payload
06/19/15   sc      Update and enable optmised Rx burst and feature guard
06/16/15   zbz     Return Tx/Rx wake up CCS time profiling to L1
06/10/15   sw/cs   Use worst case rx timings for CM_ENTER
06/10/15   tws     Fix incorrect AFC being applied in GBTA cases
06/03/15   sg      Code cleanup for edge core
06/03/15   sg      Changes to remove un-necessary tuner transactions
                   during probe burst
05/28/15   sc      Enable thermistor and vbatt reads
05/14/15   sc      Move Rx burst checks and logic from command proc to mc
05/12/15   zgl     Changes for FR 24616: Algorithm to mitigate VBatt droop (down to 2.5V) via TX backoff
05/08/15   sc      Implement command processing log
05/08/15   sw      Use new IRAT DL path enums
04/24/15   rmb     Add a source flag for enter, build, reset scripts and exit.
04/22/15   sc      Remove unused functionality
04/21/15   sw      Add NULL2X IRAT support
04/20/15   sc      Populate RF sequence number passed from L1 in RFLM DM
04/17/15   ggs     Pass back Tx power in  dBm100 to GL1 in Tx confirmation
04/14/15   sw      Update irat_info_get to use new interface definitions
04/09/15   piy     Fixed bug in Device status read feature for Mon Burst
04/07/15   sw      Merge GSM meas changes to mainline
03/30/15   sw      Update legacy cross-wtr api with new interface definitions
03/25/15   bp      Add BB headroom computation in NV reloading
03/25/15   sc      Add core Start/Stop GSM APIs
03/25/15   sc      Add Start/Stop MC APIs
03/24/15   sw      Use new irat interface definitions
03/02/15   piy     Added Time Profiling for Wakeup, Sleep, Rx & Tx burst via debug NV
02/27/15   sc      Ensure NULL NV error checking when processing Rx/Tx band
02/18/15   sw      Introduce hooks for new GRF state machine
01/30/15   sw      GL1->RF handshake mechanism to handle ASD response error
12/22/14   sw      Correct therm read F3 format specifier
12/02/14   zbz     Make rfc gsm timing ptr per device
12/03/14   sb       Call jdet on sequence before RX burst
12/02/14   sml     Read WTR or RF front end devices status in between Rx/Tx
                   burst along with GRFC status
11/25/14   hoh     Apply temp comp to originally calculated frequency compensated AMAM/AMPM table
11/25/14   sw      Disable therm read during WTR dedicated hopping
11/18/14   sw/sc   Deprecate legacy state machines and move cmd buff out of rfa
11/14/14   zbz     Explicitly set RF Device frequency error to 0 in Enter Mode
11/11/14   sw      Re-enable temp comp
11/07/14   hoh     Disable temp comp temporarily to unblock MST testing
11/04/14   tws     Enable sawless, vbatt and temp comp on JO
10/23/14   sc      Reduce code execution limiting use of get qsym API
10/17/14   rr      Separate rx/tx alternate path core handles
10/06/14   sc      Deprecate unused functionality
10/08/14   ch      Store gmsk_modulation flag
10/01/14   tws     Disable sawless, vbatt and temp comp on JO
10/01/14   tsr     GSM IDLE Mode RXD support
09/27/14   ch      Disable the CL Tuner on the 8PSK or unknown modulation
09/26/14   hoh     Remove temp code to default LIF to 0 now that L1/FW changes are in
09/23/14   zbz     changes to support GSM and LTE/TDS coexistence related RFC changes
09/19/14   sw      Enable GSM thermistor read
09/15/14   rr      Fix for nv container loading
09/12/14   sml     Adding start API Pcycle to know about pre-emption done to RF task
09/11/14   sml     Fixing KW warning
11/19/13   cpm     support for FTM_PROGRAM_QFE_SCENARIO and Antenna Tuner force_detune()
08/22/14   rr      Changes to support nv c3
08/19/14   zbz     Add RF GSM CCS execution time profiling
08/13/14   ggs     Disable VBatt and therm read if T_RUMI_EMULATION is defined
08/11/14   zbz     Add sanity check for GSM dynamic temp comp
08/04/14   tws     Stub functionlaity for RUMI pre-silicon
07/31/14   ggs     Fix bug which was causing core handle mutex to not be released if exit mode fails
07/30/14   hoh     Add enhanced rx feature support
07/24/14   tsr     DR-DSDS: Add support for device driver to handle worst case rx alpha for IDLE mode RX
07/28/14   hk      Enable WTR2605 L2G measurements
07/28/14   ec      Correct MC layer burst metrics processing API
07/21/14   sc      Add vreg vote count per sub per path
07/17/14   av      Disable therm read for DPM till final fixes for multisim therm are identified
07/11/14   ec      Add MC layer function to process burst metrics, enable SAWless feature on SAWless devices
07/04/14   jr      Changes for TSTS support
07/01/14   jr      Enable therm read on sub-2
06/25/14   zbz     Add feature to adjust temp comp table based on therm read during cal
06/24/14   av      Enable predistortion for DPM
06/18/14   tsr     RXD diversity call flow changes to enable DRX path along with PRX path
06/10/14   hk      Edge core parameter from GL1.
06/07/14   zhh     correct FEATURE_BOLT_PLUS_MODEM for rftech_gsm
05/29/14   sw      Re-enable therm read and vbatt compensation for BOLT single-sim (disabled by accident)
05/29/14   sw      Move therm read from enter mode to tx enable
05/28/14   sc      Allow MC init state in wakeup for IDLE WTR hopping
05/27/14   sb      Changes to add RFM APIs for GSM arfcn to frequency conversion
05/08/14   sc      Add RFLM GSM init once API interface
05/08/14   sc      Remove profiling for meas APIs as as_id cannot be acquired
05/02/14   sc      Remove profiling for meas APIs as as_id cannot be acquired
04/29/14   sc      Clean up debug flags and temp disable multi-SIM therm reads
04/24/14   ggs     Port CR581892: Update Coex Backoff parameters from GL1
                   for each frame
03/28/14   sw      Disable tuners during X2G iRAT scenarios (port from Dime3)
04/10/14   sc      Fix KW error of possible dereference to RxD argument
04/07/14   ggs     KW fixes
03/31/14   tsr     Added support for handling combined Enter Mode for PRX and DRX
03/28/14   sw      Enable temperature compensation
03/26/14   sb      Add support for IP2 cal for Dime PM
03/18/14   sc      Add subscription based execution intelligence
03/11/14   tsr     Added RXD support for RX burst
03/11/14   sw      Enable vbatt compensation
03/04/14   sw      ASD support on wakeup
02/28/14   sc      Pass core data through mc to core layers
02/26/14   sc      Pass core data through cmd processing and mc layer
02/19/14   tsr     GSM RX Diversity
02/17/14   sc      Clean up mDSP APIs
02/13/14   ggs     Enable Pre-distortion for Bolt
02/11/14   vv      backedout the previous change
02/11/14   sw      Remove therm read TRITON featurisation
02/06/14   sg      Handle the RFC_DEVICE_MISSING_FATAL scenario for RF daughter card support
01/15/14   ggs     Disable AMAM AMPM table override for Bolt
01/15/14   sc      Remove unused APIs using FW shared memory
01/08/14   cj      Added API to update buffer index in rfgsm_core_handles
12/13/13   ry      Remove hardcoding of RFM_DEVICE_0
11/12/13   cj      pre_config and build_reset_scripts functions de-registered for Bolt
10/28/13   ggs     Downsample base AMAM temp comp LUT by a factor of 4
10/23/13   sg      Added new flag to know if wakeup was done before the start of GSM burst
10/17/13   sc      Feature-guard population of GFW shared mem variables
10/17/13   ggs     Add support to override amam and ampm tables with default
                   values and disable pre-distortion and disable Vbatt/temp
                   comp for BOLT using debug flags
10/14/13   sc      Mainline core DM and RFLM code
10/11/13   ec      Enable GTA state tracking (Triton Only)
09/25/13   sb      Added changes to support GSM TX calibration/online mode operation in coexistence
09/23/13   ggs     Disable Temp and Vbatt compensation on RUMI platform
09/17/13   sb      Fix check for right rx burst type
09/17/13   sb      Support for SGLTE/SGTDS coexistence
09/12/13   sc      RFLM interface encompassed by core DM
08/28/13   ec      Creating RF GSM Core state machine with functions to init and
                   drive
08/16/13   ndb     Clean-up the SPI based antenna tuner support
08/14/13   sc      Revert Rx burst type variable name
08/02/13   sml     Fixing Complier warnings
07/31/13   sb      Make changes for BC config to be NV container based
07/31/13   av      GBTA support for both GL1 sub1 and GL1 sub2 call RF to build RX scripts
07/29/13   sml/svi ASD support for GSM tech
07/24/13   ec    Get as_id for Frame Number logging from RF Task
07/18/13   sc      Update RFLM data information (featurised)
07/23/13   ec    Fix Compilation Error
07/22/13   ec    Use new F3s/Null pointer checks
07/15/13   cj      SGLTE changes for IRAT
07/16/13   sc      Move mc/core functionality out of dispatcher and into mc/core layer
07/12/13   sc      Add process Tx burst API
07/12/13   av      GTA EXIT mode
06/28/13   ec      Hardcode task context logged to CMD buffer in meas apis, gate mc
                   layer task id from rfgsm_mode from core handle prevent overhead in
                   IRAT scenario.
06/28/13   ec      Log the rf task context commands are executing in. Add macros
           to update the mc_state variable in the core handle.
06/25/13   ec      Make changes to support Therm Read Cancellation
06/20/13   ec      Add support for GSM Tune Away (GTA) enter mode. Enable
                   temperature compensation on DS targets.
06/20/13   ec      Enable vbatt comp in DSDA/DSDS mode
05/30/13   tws     Enable temp comp in DSDA/DSDS mode
05/08/13   tws     Use rfgsm_core_enable_hal_bus()
05/07/13   tc      removed obsolete SPI function calls
04/30/13   aka     fix for GSM reload where bad NV ptsr are left in core handles
04/24/13   cj      Added rfgsm_mc_pre_config and rfgsm_mc_build_reset_scripts API
04/08/13   pl      Add null function pointer to support pre_config
03/21/13   tsr     Added RF APPS->RF Task MSGR framework for temp comp
03/25/13   tws     Enable Vbatt compensation on non-DSDA PLs.
03/21/13   ec      Replace deprecated RFM_MAX_DEVICES with RFM_MAX_WAN_DEVICES
03/18/13   aka     GSM NV reload fix -use NV deinit for clean-up
03/12/13   tsr     Added to tag to each therm read event handle to identify the successful retrieval
01/03/13   tws     Disable temperature compensation for Triton bringup.
02/25/13   sc      Added cm enter/exit to init/de-init iRAT during gl1 cm init/terminate
02/19/13   sc      Added APIs to allow GL1 to alloc and free Rx meas buffers
02/15/13   tsr     Enable Temp Comp algorithm
02/15/13   sc      Call rfgsm_core_exit_mode
02/12/13   sr      changes to free the memory alloocated during rfm_init()
01/17/12   aka     Logic clean-up in rfgsm_mc_update_freq_error function
12/17/12   aka     Removing legacy GSM NV support
01/11/13   aka     Changes to support simultaneous Tx SAR
01/09/13   sr      Made changes to run vbatt/temp comp in rf app task context.
01/03/12   sc      Added therm read full call flow framework
12/18/12   sc      Added therm read event generation framework
01/3/13    av      Changes to support chain2 for GSM
12/03/12   sc      Cleanup mDSP shared variable interface
11/26/12   aka     Changes to use a path specific gsm handle
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
11/15/12   ec      Using new GL1 test apis for Triton compatibility
11/07/12   pl      Added cleanup function pointer support in API structure
11/02/12   jfc     Replace rf_hal_enable_rfcmd_app with rf_hal_bus_enable
10/16/12   tsr     Cleanup GSM RF bringup flags
10/11/12   sc      Added prepare Rx burst API for inline GL1 iRAT readiness
10/11/12   sc      Cleanup up unrequired Rx burst & SBI/Non-SBI API burst buffer params
10/05/12   av      Expanded rf task architecture to include more tasks.
10/05/12   tsr     Fixed compiler errors
10/05/12   tsr     First cut of time profile code
10/04/12   sr      included the rfgsm_core_meas file.
10/04/12   sr      Added IRAT APIs.
09/25/12   tc      Mainline RF_HAS_RFA_GSM_TASK.
09/24/12   sb      Made changes to update TXLM,RXLM modem chain
09/18/12   sb      Added un-voting of GSM client through RF HAL API for requesting and releasing RFCMD app
08/28/12   sb/tsr  Added disable temp comp flag for bringup and removed the RF bring up feature
08/21/12   sc      Add and cleanup the bringup flags for GSM RF
08/13/12   tsr     Replaced Bringup feature with rumi_bup_gsm flag to facilitate Bringup
08/10/12   tsr     RF GSM CCS debug and Time profile flags added
08/08/12   sr      added rfm_dev parameter to tx_power api
08/06/12   sc      Edit API to update CCS header FW vars
08/02/12   sc      Add num slots to setup tx burst API
07/26/12   sr      Changes to APIS to take rfm_dev as parameter.
07/17/12   tsr     Additional GSM debug flags for RUMI Bring Up
07/12/12   tsr     Add GSM debug flags for RUMI Bring Up
07/06/12   sc      Edit API parameters to support CCS buffer format
07/06/12   sc      Remove unrequired num transactions init functionality
06/25/12   ggs     Added rfgsm_ftm_dispatch_init() to initialise rfgsm ftm msgr module
06/19/12   sc      Added Fast Scan status for RFGSM MC scope and corrected return types
06/19/12   sc      Added shared memory interface integration for Enter mode, Wakeup,
                   Tx band, Rx/Tx burst, and Tx power APIs
05/20/12   sb      Made changes to let RF drivers populate timing info correctly before passing onto FW
04/09/12   sb      Add changes to fix frequency error adjustment in FTM mode
03/28/12   bm      Reverting the clean up changes done in previous revision
08/26/11   aka     Update MC state during sleep and wakeup and some clean-up
                   during MC state change
02/06/12   kg      Reverting:return parameter to hardcoded value "TRUE"
02/01/12   kg      Change return parameter from hardcoded value "TRUE" to "nv_init_success" in rfgsm_mc_init()
01/10/12   sr      changes to enable vbatt comp.
12/08/11   aak     Change to not power off the vregs in rfgsm_mc_exit_mode()
11/29/11   sb      Added functionality for GSM IQ CAPTURE
10/27/11    sr     made changes to trigger SAR state change for GSM.
10/19/11   sb      Apply therm read in idle frame and enable FTM temp comp and vbatt comp during idle frame
10/18/11   lcl     Fixed compilation warning.
10/17/11    sr     made changes add F3 msgs via DPC task for isr overrun issue.
10/13/11   lcl     Added antenna tuner device init to enter_mode
10/11/11    sr     Added to log RFGSM_MC functions time profile,
09/20/11    sr     made changes to update the tx_band variable correctly.
08/25/11   sar      Removed obsolete file bbrx.h.
03/21/11    sr     Added LM support and timing info access interface.
11/22/10   te      correct rfgsm_mc_disable_tx() return value
10/14/10   mkv/whc Integrating Reload RFNV from SCMM into MDM9x00 (CL 1315259)
10/11/10   lcl     Changed MSG_LVL to MSG_LEGACY to avoid too many F3 messages
09/21/10   ggs     Added new api to return rf_warmup time
07/21/10   lcl     Remove SWC clock enabling again.
07/18/10   lcl     GL1 doesn't seem to doing SWC correct--failed GSM RX cal.
                   Put in a temporary hack for now.
07/14/10   lcl     Remove SWC clock enabling.  GL1 should take care of enabling
                   the SWC clock after rfm_enter_mode(GSM) is done.
02/11/09   sr      Removed the mc_state check in chan_tune to make the wtog work.
02/09/09   sr      moving the pmic calls to RFC.
02/08/09   sr      lint errors fix
29/01/10   sr      Changed the <tech>_mc_init() signature to fix warnings in
                   <tech> specific builds
01/28/10    sr     added  power_clock_o/off functions
12/28/09   sr      High/Medium lint errors fix
12/22/09   sr     changed the debug msg macro from legacy to new one.
12/01/09   sr     return TRUE from rfgsm_mc_init() until we fix the nv_status.
                  Do not call the rfgsm_enter_mode() if the common_init is not successfull.
11/30/09   bmg    Added common_init_successful parameter to GSM initialization
                  Skip GSM initialization if the common NV could not be loaded
                  Changed init function return to boolean
11/18/09   dyc    Use correct PMIC switcher PM_VREG_RF2_ID vs PM_VREG_RF_ID
10/21/09   bmg    Changed pm_vreg.h to pm.h - proper include method for PMIC
10/15/09   wen    enable RF GSM sleep/wakeup
09/22/09   dyc    Support for SMPS freq setting.
09/17/09   sar    Removed deprecated file, adie.h for CMI conformance.
07/20/09   sr     moved the init_once() call to mc_init() and code cleanup.
07/13/09   dyc    Added temporary feature FEATURE_RF_USE_07_REL_CLKREG to support
                  clk_regime API transition.
07/06/09   sr     changes to make the operating band to rfcom_gsm_band type
06/26/09   sr     Code clean up
08/06/09   sr     SCMM bringup changes
03/25/09   sr     Added state transitions to block core functions calls with init failure
03/13/09   sr     code cleanup for SCMM build
10/14/08   sr     Initial version of GSM Main Control.

============================================================================*/

#include "qurt.h"
#include "msg.h"
#include "rfcom.h"
#include "rfcommon_core_types.h"
#include "rfgsm_mc.h"
#include "rfgsm_mc_rx.h"
#include "rfgsm_core.h"
#include "rfgsm_core_log.h"
#include "rfgsm_core_types.h"
#include "rfgsm_core_tx.h"
#include "rfgsm_core_rx.h"
#include "rfgsm_core_util.h"
#include "ftm.h"
#include "rfm_internal.h" // For RUMI Bring up
#include "rf_hal_common.h"
#include "rfmeas_mc.h"
#include "rfgsm_core_meas.h"
#include "gtmrs.h"
#include "geran_test_mode_api.h"
#include "rfgsm_core_temp_comp.h"
#include "rfgsm_core_handle.h"
#include "rfcommon_locks.h"
#include "rfcommon_core.h"
#include "rfcommon_nv_mm.h"
#include "rfdevice_antenna_tuner.h"
#include "rfgsm_core_intf.h"
#include "stringl.h"

/*=============================================================================

                      STATIC AND EXTERN FUNCTION PROTOTYPES

=============================================================================*/
rfgsm_mc_status_type rfgsm_mc_meas_init(void);
rfgsm_mc_status_type rfgsm_mc_meas_deinit(void);

static rfgsm_mc_status_type rfgsm_mc_get_irat_timing_info(
                              rfgsm_timing_info_type *timing_info_p );

#ifdef FEATURE_RF_ASDIV
/*---------------------------------------------------------------------------------------*/
extern boolean rfgsm_core_process_asd_read_rsp(rfgsm_core_data_type *core_data, uint8 ant_pos);

/*---------------------------------------------------------------------------------------*/
extern void rfgsm_core_set_antenna(rfgsm_core_data_type *core_data, uint8 ant_pos);

#endif

extern boolean rfgsm_core_process_device_status_read_rsp(uint8 sub_id,
                                                         uint8 buffer_id,
                                                         rf_burst_type burst_type,
                                                         uint8 event_type,
                                                         uint8 event_tag,
                                                         uint8 burst_num);

static rfgsm_mc_sar_forced_antenna_enum_type sar_antenna = RFGSM_MC_SAR_ANTENNA_NONE;



/*=============================================================================

                                DEFINITIONS

=============================================================================*/
// Struct that holds gsm debug flags
rfgsm_bup_flags_t  rfgsm_debug_flags =
{
  1,        /* Disable GSM Antenna Tuner Flag */
  0,        /* Time Profiling Flag */
#if defined (T_RUMI_EMULATION)
  1,        /* Disable temp comp*/
  1,        /* Disable vbatt comp*/
#else
  0,        /* Disable temp comp*/
  0,        /* Disable vbatt comp*/
#endif
  1,        /* rfgsm_init only Flag */
  0,        /* override_amam_ampm_tables */
  0,        /* predist_override: Disable Predistortion  */
  0,        /* sawless_override: Enable SAWless features */
};

#define RFGSM_MC_TASK(p, t)                 \
{                                           \
    if(p->rfgsm_mode == RFGSM_IRAT_MODE)    \
    {                                       \
        t = RF_MEAS_CALL;                   \
    } else                                  \
    {                                       \
        t = rfgsm_core_get_task_num();      \
    }                                       \
}

rf_lock_data_type rfgsm_core_irat_meas_crit_sect;

/*=============================================================================

                               FUNCTION DEFINITIONS

=============================================================================*/
/*!
  @brief

  @details

  @param device: RF device
  @param caller_tcb_ptr: Calling task TCB pointer
  @param task_nv_wait_sig: Task signal to wait for when reading NV
  @param task_wait_func_ptr: Task's wait function to be called when reading NV
*/
boolean rfgsm_mc_init(boolean common_init_successful,
                      rex_tcb_type *caller_tcb_ptr,
                      rex_sigs_type task_nv_wait_sig,
                      void (*task_wait_func_ptr)( rex_sigs_type ))
{

  rfcommon_nv_status_type nv_status = RFCOMMON_NV_READ_FAILED;
  static boolean nv_init_success = FALSE;
  static boolean nv_items_read = FALSE;
  boolean status = FALSE;
  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  rfgsm_core_handle_type* rfgsm_core_handle_ptr = NULL;
  rfgsm_nv_cmn_data_type* rfgsm_cmn_nv_ptr = NULL;

  uint8 flag = (uint8)FALSE;

  RF_MSG_1(MSG_LEGACY_HIGH,"rfgsm_mc_init: START. CMN success %d", common_init_successful);

  rfgsm_nv_populate_coex_band_map_tbl();

  /* Initialise the mc Rx init once */
  if( rfgsm_mc_rx_init_once() != RFGSM_MC_SUCCESS )
  {
    /* MC Rx init once initialisation failed */
    RF_MSG(MSG_LEGACY_ERROR,"rfgsm_mc_init: rfgsm_mc_rx_init_once failed. GSM init not complete.");
    return FALSE;
  }

  /* Initialise the core init once */
  if( rfgsm_core_init_once() != TRUE )
  {
    /* Core init once initialisation failed */
    RF_MSG(MSG_LEGACY_ERROR,"rfgsm_mc_init: rfgsm_core_init_once failed. GSM init not complete.");
    return FALSE;
  }

  if ( common_init_successful )
  {
    if (!nv_items_read)
    {

      rfgsm_nv_data_structure_init();

      /*Assumptions: Which needs to be corrected later when RFNV layer accepts nv container
      or we need a design in place for it. Right now we just loop for RFM devices to see if it is
      supported on any bands for RX or TX. the cfg structure isn't used. This should work for the
      present RFCs and near future RFCs*/
      for (rfm_dev=0;rfm_dev<RFM_MAX_WAN_DEVICES;rfm_dev++)
      {
        flag = (uint8)FALSE;

        /*load GSM NVs*/
        /*GSM INIT is not called for multiple paths as of now, need to check it for Triton*/
         nv_status = rfgsm_nv_load_items(rfm_dev, caller_tcb_ptr, task_nv_wait_sig,
                                           task_wait_func_ptr, &flag);

        if (flag != (uint8)TRUE) // Check if logical device supported in rfc
        {
          continue;
        }

        if (nv_status == RFCOMMON_NV_READ_SUCCESS) // Check if all NVs were read correctly
        {
          /* Not missing critical NV items*/
          nv_init_success = TRUE;
        }

        /* Remember that we've read the NV. */
        nv_items_read = TRUE;

        status = rfgsm_core_init(rfm_dev);
        if (!status)
        {
          RF_MSG_2(MSG_LEGACY_ERROR,
                   "rfgsm_mc_init: rfgsm_core_init() Failed : rfm_dev %d, nv_init_success = %d. GSM init not complete.",
                    rfm_dev, nv_init_success);
          return status;
        }
        rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

        RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "NULL ptr when trying to set MC state!", FALSE);

        rfgsm_cmn_nv_ptr = rfgsm_nv_get_cmn_tbl_ptr(rfm_dev);
        if (rfgsm_cmn_nv_ptr != NULL)
        {
          if ((rfgsm_cmn_nv_ptr->cal_temp_nv_enable) && (rfgsm_cmn_nv_ptr->enh_tempcomp_enable_nv))
          {
            rfgsm_core_temp_comp_tbl_adjust(rfm_dev);
          }
        }
        else
        {
          RF_MSG_1(MSG_LEGACY_ERROR, " rfgsm_cmn_nv_ptr is NULL! rfm_dev = %d ",rfm_dev);
        }
      }

      rfgsm_core_validate_rx_timing_info();

    }

  } /* if ( common_init_successful ) */
  else
  {
    RF_MSG( RF_ERROR, "rfgsm_mc_init: GSM init not complete due to cmn init failure." );
  }

  /*Moved the dispatch inits to rf_task and rf_Task_2
  so that initialization can happen if task has only started */

  /* register the IRAT APIs with meas module */
  if( rfgsm_mc_meas_init() != RFGSM_MC_SUCCESS )
  {
    /* Initialisation of the IRAT fp table failed */
    RF_MSG( MSG_LEGACY_ERROR, "rfgsm_mc_init: rfgsm_mc_meas_init failed. GSM init not complete." );
    return FALSE;
  }

  RF_MSG( MSG_LEGACY_HIGH, "rfgsm_mc_init: END. success 1" );

  /* Return TRUE since several calibrated NV's have critical attribute in GSM*/
  return TRUE;
}



/*----------------------------------------------------------------------------*/
/*!
  @brief free-up all the resources allocated during rfgsm_mc_init()

  @details
  free-up all the resources allocated during rfgsm_mc_init()

*/
boolean rfgsm_mc_deinit(void)
{
  rfm_device_enum_type rfm_dev;
  rfcom_gsm_band_type band;
  boolean rfm_device_supported = FALSE;
  rfc_cfg_info_type cfg_struct;
  rfgsm_core_handle_type* rfgsm_core_handle_ptr = NULL;

  /* register NULL IRAT function ptr table */
  if( rfgsm_mc_meas_deinit() != RFGSM_MC_SUCCESS )
  {
    return FALSE;
  }

  rfgsm_core_deinit();

  /* Initialise the mc Rx init once */
  if( rfgsm_mc_rx_destroy() != RFGSM_MC_SUCCESS )
  {
    /* MC Rx deinit failed */
    RF_MSG(MSG_LEGACY_ERROR,"rfgsm_mc_rx_destroy failed");
    return FALSE;
  }

  for (rfm_dev=0;rfm_dev<RFM_MAX_WAN_DEVICES;rfm_dev++)
  {
    for (band=0;band<RFCOM_NUM_GSM_BANDS;band++)
    {
      rfm_device_supported = rfc_gsm_get_cfg_info( rfm_dev, band, RFC_CONFIG_RX, &cfg_struct, RFGSM_ALT_PATH_NOT_SUPPORTED );

      if (rfm_device_supported == FALSE)
      {
        rfm_device_supported = rfc_gsm_get_cfg_info( rfm_dev, band, RFC_CONFIG_TX, &cfg_struct, RFGSM_ALT_PATH_NOT_SUPPORTED );
      }

      if (rfm_device_supported == TRUE)
      {
        break;
      }
    }
    if (rfm_device_supported == TRUE)
    {
      rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
      RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL in mc deinit!", FALSE);
      rfgsm_nv_deinit(rfm_dev);
    }
  }

  return TRUE;
}



/*----------------------------------------------------------------------------*/
/*!
  @fn rfgsm_mc_start_gsm
  @brief This function allocates the RFLM DM memory for subscription
  @param sub_id : subscription to allocate memory for
  @retval rfgsm_mc_status_type : success/failure status of the MC API
*/
rfgsm_mc_status_type rfgsm_mc_start_gsm( uint8 sub_id, sawless_support_table_t sawless_support_table )
{
  rfcom_gsm_band_type rfcom_band_index = 0;
  rfm_device_enum_type rfm_dev_index = 0;

  /* Execute core functionality to start GSM */
  if( rfgsm_core_start_gsm(sub_id) != TRUE )
  {
    /* Failed to allocate RFLM DM memory for GSM and cannot continue */
    ERR_FATAL("RF GSM Start GSM failed to allocate DM memory", sub_id, 0, 0);
    return RFGSM_MC_CORE_API_FAILURE;
  }

  /* Iterate through each device and band and populate SAWless support into table for GL1 */
  for ( rfm_dev_index = 0 ; rfm_dev_index < RFM_MAX_WAN_DEVICES ; rfm_dev_index++ )
  {
    for ( rfcom_band_index = 0 ; rfcom_band_index < RFGSM_MAX_UNIQUE_RFCOM_BANDS ; rfcom_band_index++ )
    {
      sawless_support_table[rfm_dev_index][rfcom_band_index] = rfgsm_core_get_band_sawless_status(rfm_dev_index, rfcom_band_index);
    }
  }

  return RFGSM_MC_SUCCESS;
}



/*----------------------------------------------------------------------------*/
/*!
  @fn rfgsm_mc_stop_gsm
  @brief This function frees the RFLM DM memory for subscription
  @param sub_id : subscription to free memory for
  @retval rfgsm_mc_status_type : success/failure status of the MC API
*/
rfgsm_mc_status_type rfgsm_mc_stop_gsm( uint8 sub_id )
{
  /* Execute core functionality to start GSM */
  if( rfgsm_core_stop_gsm(sub_id) != TRUE )
  {
    /* Failed to deallocate RFLM DM memory for GSM and should not continue */
    ERR_FATAL("RF GSM Stop GSM failed to deallocate DM memory", sub_id, 0, 0);
    return RFGSM_MC_CORE_API_FAILURE;
  }

  return RFGSM_MC_SUCCESS;
}



/*----------------------------------------------------------------------------*/
/*
  @brief
  initialise for RF GSM iRAT
  called during gl1 cm rf init
*/
rfgsm_mc_status_type rfgsm_mc_cm_enter( rfgsm_core_data_type *core_data,
                                        rfgsm_timing_info_type *timing_info )
{
  /* Need to allocate the Rx measurement script buffers */
  rfgsm_mc_status_type cm_enter_status = RFGSM_MC_ERROR;

  /* Allocate the Rx script buffers for IRAT */
  if( rfgsm_core_start_gsm(core_data->sub_id) != TRUE )
  {
    /* Failed to allocate RFLM DM memory for GSM and cannot continue */
    ERR_FATAL("RF GSM CM Enter failed to allocate DM memory", core_data->sub_id, 0, 0);
    cm_enter_status = RFGSM_MC_CORE_API_FAILURE;
  }

  cm_enter_status = rfgsm_mc_get_irat_timing_info( timing_info );

  if ( cm_enter_status != RFGSM_MC_SUCCESS )
  {
    RF_MSG(MSG_LEGACY_ERROR, "rfgsm_mc_get_irat_timing_info failed!");
  }

  return cm_enter_status;
}



/*----------------------------------------------------------------------------*/
/*
  @brief
  de-init for exiting RF GSM iRAT
  called during gl1 cm terminate
*/
rfgsm_mc_status_type rfgsm_mc_cm_exit( rfgsm_core_data_type *core_data )
{
  /* Need to free the Rx measurement script buffers */
  rfgsm_mc_status_type cm_exit_status = RFGSM_MC_ERROR;

  /* Execute core functionality to start GSM */
  if( rfgsm_core_stop_gsm(core_data->sub_id) != TRUE )
  {
    /* Failed to deallocate RFLM DM memory for GSM and should not continue */
    ERR_FATAL("RF GSM CM Exit failed to deallocate DM memory", core_data->sub_id, 0, 0);
    cm_exit_status = RFGSM_MC_CORE_API_FAILURE;
  }

  return cm_exit_status;
}



/*----------------------------------------------------------------------------*/
/*
  @brief rfgsm_mc_process_device_status_read_rsp

  @details This API is called when the Response of Device Status Read is to be processed
           and it calls rfgsm_core_process_device_status_read_rsp API to further process it.

  @param sub_id           : Subscription ID
  @param buffer_id        : Buffer ID
  @param burst_type       : Burst Type (Rx/Tx)
  @param event_type       : Event Type
  @param event_tag        : Device Status Read Event Tag
*/
rfgsm_mc_status_type rfgsm_mc_process_device_status_read_rsp(uint8 sub_id,
                                                             uint8 buffer_id,
                                                             rf_burst_type burst_type,
                                                             uint8 event_type,
                                                             uint8 event_tag,
                                                             uint8 burst_num)
{
  if( !rfgsm_core_process_device_status_read_rsp(sub_id, buffer_id, burst_type, event_type, event_tag, burst_num))
  {
    RF_MSG( MSG_LEGACY_ERROR, "rfgsm_core_process_device_status_read_rsp() Failed" );
    return RFGSM_MC_CORE_API_FAILURE;
  }

  return RFGSM_MC_SUCCESS;
}


#ifdef FEATURE_RF_ASDIV
/*----------------------------------------------------------------------------*/
/*
  @brief

  @details

  @param user_data_ptr  : Pointer to user data passed into callback
  @param rfm_dev        : Device ID
*/

/*lint -esym(715, rfm_dev) */
rfgsm_mc_status_type rfgsm_mc_process_asd_read_rsp(rfgsm_core_data_type *core_data,
                                                   uint8 ant_pos)
{
  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  if( !rfgsm_core_process_asd_read_rsp(core_data, ant_pos) )
  {
    RF_MSG(MSG_LEGACY_ERROR,"rfgsm_core_process_asd_read_rsp() Failed");
    return RFGSM_MC_CORE_API_FAILURE;
  }
  return RFGSM_MC_SUCCESS;
}
#endif



/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function set the temp comp flag upon receiving a msg from RF apps task
  and set the TX profile update flag.

  @param
  rfm_dev        : Device ID
  int16 temp_comp_scaled_value
  uint8 current_temp_bin_16

  @retval boolean
*/
rfgsm_mc_status_type rfgsm_mc_update_temp_comp_req(rfgsm_core_data_type *core_data,
                                                   int16 temp_comp_scaled_value,
                                                   uint8 current_temp_bin_16)
{
  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  /* Update temp comp flag to indicate therm read is updated*/
  rfgsm_core_temp_comp_flag_set(core_data->rfgsm_core_handle_ptr);

  /*update scaled value to be used by TXAGC( used for calculating pcl based Tx power compensation*/
  rfgsm_core_temp_comp_pwr_update(core_data->rfgsm_core_handle_ptr, temp_comp_scaled_value);

  /*update temp comp bin index to be used by TXAGC( compensation for amam/pm tables)*/
  rfgsm_core_temp_comp_bin_index_update(core_data->rfgsm_core_handle_ptr, current_temp_bin_16);

  /*update tx profile update flag */
  rfgsm_core_set_tx_profile_update_flag(core_data->rfgsm_core_handle_ptr);

  return RFGSM_MC_SUCCESS;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_mc_set_rx_band
  @brief This function configures the RF for a specified Rx band
  @details This function converts the system band value to an RF GSM band value
           and configures RF variables for a specified band, used later for
           RF script building.
  @param core_data - data structure including device ID and core handle
  @param band - band to configure the RF to passed from GL1
  @retval mc status - description of the success/failure of the API
*/
rfgsm_mc_status_type rfgsm_mc_set_rx_band( rfgsm_core_data_type *core_data, rfgsm_band_type band )
{
  rfgsm_mc_status_type set_rx_band_status = RFGSM_MC_SUCCESS;
  rfcom_gsm_band_type rfcom_band = RFCOM_NUM_GSM_BANDS;

  /* convert to rfcom band and which is the band type the rfgsm core will
  operate on */
  rfcom_band = rfgsm_core_convert_rfband_to_rfcom(band);

  /* */
  if( !rfgsm_core_rx_set_band(core_data, rfcom_band) )
  {
    set_rx_band_status = RFGSM_MC_CORE_API_FAILURE;
  }

  return set_rx_band_status;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_mc_set_tx_band
  @brief This function configures the RF for a specified Tx band
  @details This function converts the system band value to an RF GSM band value
           and executes Tx band core functionality create RF CCS events to
           be scheduled by GFW to program RF to the specified band
  @param core_data - data structure including device ID and core handle
  @param band - band to configure the RF to passed from GL1
  @param shared_mem_ptr - FW memory location to populate the interface index
  @retval mc status - description of the success/failure of the API
*/
rfgsm_mc_status_type rfgsm_mc_set_tx_band( rfgsm_core_data_type *core_data,
                                           rfgsm_band_type tx_band,
                                           void *shared_mem_ptr)
{
  rfcom_gsm_band_type rfcom_band = RFCOM_NUM_GSM_BANDS;

  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  /* convert to rfcom band and which is the band type the rfgsm core will
  operate on */
  rfcom_band = rfgsm_core_convert_rfband_to_rfcom(tx_band);

  /* @TODO add core API return and handling */

  rfgsm_core_set_tx_band(core_data, rfcom_band, shared_mem_ptr);

  return RFGSM_MC_SUCCESS;
}



/*----------------------------------------------------------------------------*/
/*
  @brief

  @details
*/

boolean rfgsm_mc_update_tx_power_in_response_message( rfgsm_core_data_type *core_data, int32 *tx_pwr_in_dBm100 )
{
  if( !rfgsm_core_update_tx_power_in_response_message( core_data, tx_pwr_in_dBm100 ) )
  {
     return FALSE;
  }
  else
  {
     return TRUE;
  }
}



/*----------------------------------------------------------------------------*/
/*!
  @details
   This function apply SAR limit passed from MCS

  @param sar_limit_gmsk[5] : SAR limits for GSM for all 5 different multislot configs
         sar_limit_8psk[5] : SAR limits for EDGE for all 5 different multislot configs

*/
rfgsm_mc_status_type rfgsm_mc_set_simult_sar_limit( rfgsm_core_data_type *core_data,
                                                    int16 sar_limit_gmsk[5],
                                                    int16 sar_limit_8psk[5])
{
  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  /* @TODO add core API return and handling */

  rfgsm_core_set_simult_sar_limit(core_data->rfgsm_core_handle_ptr, sar_limit_gmsk, sar_limit_8psk);

  return RFGSM_MC_SUCCESS;
}


/*----------------------------------------------------------------------------*/
/*!
  @details
   This function apply VBATT limit passed from MCS

  @param vbatt_limit_gmsk[5] : VBATT limits for GSM for all 5 different multislot configs
         vbatt_limit_8psk[5] : VBATT limits for EDGE for all 5 different multislot configs

*/
rfgsm_mc_status_type rfgsm_mc_set_simult_vbatt_limit( rfgsm_core_data_type *core_data,
                                                    int16 vbatt_limit_gmsk[5],
                                                    int16 vbatt_limit_8psk[5])
{
  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  /* @TODO add core API return and handling */

  rfgsm_core_set_simult_vbatt_limit(core_data->rfgsm_core_handle_ptr, vbatt_limit_gmsk, vbatt_limit_8psk);

  return RFGSM_MC_SUCCESS;
}



/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function calls all the rfgsm_mc functionality to create the RF CCS for all the
  bursts requested from GL1.

  This function can be called from RF Task context from the rfgsm_cmd_processing dispatcher

  @param device           : RF Multimode device
  @param tx_burst_params  : Tx burst parameters for each frame
*/
rfgsm_mc_status_type rfgsm_mc_process_tx_burst( rfgsm_core_data_type *core_data,
                                                rfa_rf_gsm_tx_burst_type *tx_burst_params )
{
  /* Initialise variables */
  uint32 index = 0;
  rfgsm_power_profile_type pwr_profiles[RFGSM_MAX_TX_SLOTS_PER_FRAME];
  rfgsm_mc_status_type mc_ret_status = RFGSM_MC_ERROR;
  boolean gmsk_flag = TRUE;
  uint32 rfgsm_time_profile[] = {rfgsm_mc_read_ustmr_count(),0,0,0,0};
  uint8 log_index = 1;

  if ( tx_burst_params == NULL )
  {
    RF_MSG(MSG_LEGACY_ERROR,"tx_burst_params = NULL. Did not process Tx burst.");
    return RFGSM_MC_INVALID_PARAM;
  }

  /* Check number of Tx slots is not larger than max */
  if( tx_burst_params->num_tx_slots > RFGSM_MAX_TX_SLOTS_PER_FRAME )
  {
    RF_MSG_2(MSG_LEGACY_ERROR,
             "num_tx_slots %d too large. Max Tx slots is %d",
             tx_burst_params->num_tx_slots, RFGSM_MAX_TX_SLOTS_PER_FRAME);
    return RFGSM_MC_INVALID_PARAM;
  }

  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  /*Initialise all the pwr profile data to zero*/
  memset(pwr_profiles, 0x0, sizeof(pwr_profiles));

  /* Fill in the power profile structure for the RF driver for each tx slot required */
  for (index = 0; index < tx_burst_params->num_tx_slots; index++)
  {
    pwr_profiles[index].power_level = tx_burst_params->tx_pwr_ctl_params[index].power_level;
    pwr_profiles[index].modulation = tx_burst_params->tx_pwr_ctl_params[index].modulation;
    /* sets GMSK flag to true if modulation is GMSK (else false for unknown or 8PSK)*/
    gmsk_flag &= (RF_MOD_GMSK == pwr_profiles[index].modulation);
    pwr_profiles[index].backoff[RFGSM_COEX_DESENSE_BACKOFF] = tx_burst_params->tx_pwr_ctl_params[index].backoff[RFGSM_COEX_DESENSE_BACKOFF];
    pwr_profiles[index].backoff[RFGSM_COEX_VBATT_BACKOFF] = tx_burst_params->tx_pwr_ctl_params[index].backoff[RFGSM_COEX_VBATT_BACKOFF];
  }

  /* Update the frequency error */
  if( (ftm_get_mode() != FTM_MODE) ||
      // In FTM mode, send the freq error update only if it is not in override mode
      (ftm_get_mode() == FTM_MODE && (rfgsm_core_get_gl1_freq_error_flag(core_data->rfgsm_core_handle_ptr) == TRUE)) )
  {
    rfgsm_core_update_tx_freq_error( core_data, tx_burst_params->tx_chan_params.freq_err );
  }
  rfgsm_time_profile[log_index] = rfgsm_mc_read_ustmr_count();
  RFGSM_MC_PROC_LOG(RFGSM_MC_TX_UPDATE_FREQ_ERROR, core_data->sub_id, 0, 0, 0, 0, rfgsm_time_profile[log_index++], qurt_get_core_pcycles(), 0 );

  /* Tune the UHF Tx synthesizers to the channel specified */
  (void)rfgsm_core_tune_tx( core_data, tx_burst_params->tx_chan_params.arfcn, pwr_profiles[0].modulation );

  rfgsm_time_profile[log_index] = rfgsm_mc_read_ustmr_count();
  RFGSM_MC_PROC_LOG(RFGSM_MC_TX_TUNE, core_data->sub_id, 0, 0, 0, 0, rfgsm_time_profile[log_index++], qurt_get_core_pcycles(), 0 );

  /* Configure the AM/AM and AM/PM tables for polar RF */
  rfgsm_core_set_tx_power( core_data,
                           pwr_profiles,
                           tx_burst_params->num_tx_slots,
                           tx_burst_params->num_assigned_uplink_slots );
  rfgsm_time_profile[log_index] = rfgsm_mc_read_ustmr_count();
  RFGSM_MC_PROC_LOG(RFGSM_MC_TX_SET_POWER, core_data->sub_id, 0, 0, 0, 0, rfgsm_time_profile[log_index++], qurt_get_core_pcycles(), 0 );

  /* Setup the start, transition and stop buffers for SBI, GRFC, and RFFE necessary
   * for the Tx frame. */
  rfgsm_core_setup_tx_burst( core_data,
                             tx_burst_params->num_tx_slots,
                             tx_burst_params->tx_burst_event_ptr,
                             tx_burst_params->tx_chan_params.rf_sequence_number );

  /* Note: this is the last log in API therefore log_index not incremented */
  rfgsm_time_profile[log_index] = rfgsm_mc_read_ustmr_count();
  RFGSM_MC_PROC_LOG(RFGSM_MC_TX_SETUP_BURST, core_data->sub_id, 0, 0, 0, 0, rfgsm_time_profile[log_index], qurt_get_core_pcycles(), 0 );

  /* Print profile data if debug NV is set */
  if(core_data->rfgsm_core_handle_ptr->enable_time_profiling.tx_burst)
  {
    RF_MSG_4( MSG_LEGACY_HIGH,
              "Tx Burst Time Profiling, mc_update_freq_error: %d us, "
              "mc_tune_to_chan: %d us, mc_set_amam_profile: %d us, "
              "mc_setup_tx_burst: %d us",
              rfgsm_time_profile[1] - rfgsm_time_profile[0],
              rfgsm_time_profile[2] - rfgsm_time_profile[1],
              rfgsm_time_profile[3] - rfgsm_time_profile[2],
              rfgsm_time_profile[4] - rfgsm_time_profile[3] );
  }

  return RFGSM_MC_SUCCESS;
}



/*----------------------------------------------------------------------------*/
/*!
  @brief
  Reading NV for GSM and populating the necessary variables.

  @details
  Retrieves all the NV from GSM mode. Reads common and mode specific RF
  configuration parameters from NV. This is called as part of the
  LOAD_RF_NV function case where we need to read the calibration NV and write
  them without having to reset.

  @param device: RF device
  @param caller_tcb_ptr: Calling task TCB pointer
  @param task_nv_wait_sig: Task signal to wait for when reading NV
  @param task_wait_func_ptr: Task's wait function to be called when reading NV
*/

boolean rfgsm_mc_reload_nv(rfm_device_enum_type rfm_dev,
                           rex_tcb_type *caller_tcb_ptr,
                           rex_sigs_type task_nv_wait_sig,
                           void (*task_wait_func_ptr)( rex_sigs_type ))
{
  uint8 flag = (uint8)FALSE;

  RF_MSG( MSG_LEGACY_HIGH, "rfgsm_mc_reload_nv" );

  /* free up all allocated buffers, reload NV allocates it again*/
  rfgsm_nv_deinit(rfm_dev);

  /* read GSM NVs and save in new allocated structures*/
  (void)rfgsm_nv_load_items(rfm_dev,
                            caller_tcb_ptr,
                            task_nv_wait_sig,
                            task_wait_func_ptr,
                            &flag);

  if (flag != (uint8)TRUE)
  {
    return FALSE;
  }
  /* re-compute all switch points based on new NV value and program Tx timing values to mdsp*/
  rfgsm_core_nv_init(rfm_dev);

  /* re-init NV ptrs in core handles*/
  rfgsm_core_handle_nv_init(rfm_dev);

  rfgsm_core_temp_comp_init(rfm_dev);

  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
  @fn rfgsm_mc_get_irat_timing_info
  @brief This function returns worst case timing for iRAT
  @param timing_info_p : timing info data struct
  @retval rfgsm_mc_status_type : success/failure status of the MC API
*/
rfgsm_mc_status_type rfgsm_mc_get_irat_timing_info(
                       rfgsm_timing_info_type *timing_info_p )
{
  rfgsm_core_get_irat_timing_info( timing_info_p );

  return ( RFGSM_MC_SUCCESS );
}


/*----------------------------------------------------------------------------*/
/*
  @brief
  Do processing scheduled in a GSM idle frame.

  @details

  @param
  None

*/
rfgsm_mc_status_type rfgsm_mc_do_idle_frame_processing(rfgsm_core_data_type *core_data)
{
  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  rfgsm_core_do_idle_frame_processing(core_data);
  return RFGSM_MC_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/*
  @brief

  @details
*/
boolean rfgsm_mc_update_sar_backoff(rfm_device_enum_type rfm_dev)
{
  rfgsm_core_handle_type* rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", FALSE);

  rfgsm_core_set_tx_profile_update_flag(rfgsm_core_handle_ptr);
  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  This function relays the pointer to GL1 structure containing information about IQ capture to rfgsm_core
  @details
*/
boolean rfgsm_mc_log_iq_data(rfm_device_enum_type rfm_dev, GfwIqSamplesBuffer* data)
{
  rfgsm_core_log_iq_data(rfm_dev, data);
  return TRUE;
}



/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function will populate all the timing information related to GSM IRAT measurements. This
    timing information will be used by L1 scheduling the measurements during IRAT gap.

  @param rfm_meas_irat_info_type: data structure for all info to be returned

  @pre
  rfm_init() must have already been called.
  We should be in a certain technology.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfgsm_mc_irat_info_get(rfm_meas_irat_info_type *irat_info_param)
{
  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type src_tech, target_tech;
  rfgsm_core_handle_type* rfgsm_core_handle_ptr = NULL;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  RF_NULL_CHECK_RTN(irat_info_param, "irat_info_param is NULL!", RFM_MEAS_COMMON_FAILURE);

  /* Log the beginning of the get IRAT info API */
  RFGSM_MC_PROC_LOG_CMN_START(RFGSM_MC_MEAS_IRAT_INFO_GET_BEGIN, irat_info_param->sub_id, 0, 0);

  src_tech = irat_info_param->header.source_tech;
  target_tech = irat_info_param->header.target_tech;

  if ( target_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = irat_info_param->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }
  else if( src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = irat_info_param->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }

  if(rfgsm_core_get_device_rx_capability(rfm_dev))
  {
    rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  }
  else
  {
    for(rfm_dev = RFM_DEVICE_0;rfm_dev < RFM_MAX_WAN_DEVICES; rfm_dev++)
    {
      if(rfgsm_core_get_device_rx_capability(rfm_dev))
      {
        rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
        break;
      }
    }

    if( rfgsm_core_handle_ptr== NULL )
    {
      ERR_FATAL( "NULL core handle ptr for all Rx devices for source_tech %d, target tech %d",
                irat_info_param->header.source_tech, irat_info_param->header.target_tech,0 );
      result = RFM_MEAS_COMMON_FAILURE;
      return result;
    }
  }

  result = rfgsm_core_irat_info_get(irat_info_param,rfm_dev);

  if( target_tech == RFCOM_GSM_MODE )
  {
    result &= rfgsm_core_irat_update_rx_timings(irat_info_param,rfm_dev);
  }

  /* Log the end of the get IRAT info API */
  RFGSM_MC_PROC_LOG_CMN_STOP(RFGSM_MC_MEAS_IRAT_INFO_GET_END, irat_info_param->sub_id, 0, 0);

  return(result);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called first when the measurement is requested. Each technology
    basically prepares for the measurement.

  @param rfm_meas_enter_param : This is a pointer that basically has a header that has
  the target technology and the source technology. Based on this header, the rest is casted as
  a tech specific pointer.

  @pre
  rfm_init() must have already been called.
  We should be in a certain technology.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfgsm_mc_meas_enter( rfm_meas_enter_param_type *meas_enter_param, boolean is_source)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* Validate input */
  RF_NULL_CHECK_RTN(meas_enter_param, "rfgsm_mc_meas_enter: meas_enter_param is NULL!", RFM_MEAS_COMMON_FAILURE);
  RF_NULL_CHECK_RTN(meas_enter_param->enter_params, "rfgsm_mc_meas_enter: enter_params is NULL!", RFM_MEAS_COMMON_FAILURE);

  /* Need to mutex lock meas enter as W can call from two threads for same device */
  rf_common_enter_critical_section( &rfgsm_core_irat_meas_crit_sect );

  /* Log the beginning of the meas enter API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_MC_MEAS_ENTER_BEGIN, 0xFF, 0, 0);

  result = rfgsm_core_meas_enter(meas_enter_param->enter_params);

  /* Log the end of the meas enter API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_MC_MEAS_ENTER_END, 0xFF, 0, 0);

  rf_common_leave_critical_section( &rfgsm_core_irat_meas_crit_sect );

  return(result);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called to prepare the clean-up and start-up scripts.

  @param rfm_meas_setup_param : This is a pointer that basically has a header that has
  the target technology and the source technology. Based on this header, the rest is casted as
  a tech specific pointer.

  @pre
  rfm_meas_common_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfgsm_mc_meas_build_scripts( rfm_meas_setup_param_type *meas_scripts_param, boolean is_source)
{
  rfm_meas_program_script_type script_type;

  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type tgt_tech = RFM_INVALID_MODE;
  rfm_meas_common_param_type *setup_params = NULL;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* Validate input */
  RF_NULL_CHECK_RTN(meas_scripts_param, "rfgsm_mc_meas_build_scripts: meas_scripts_param is NULL!", RFM_MEAS_COMMON_FAILURE);
  RF_NULL_CHECK_RTN(meas_scripts_param->setup_params, "rfgsm_mc_meas_build_scripts: setup_params is NULL!", RFM_MEAS_COMMON_FAILURE);

  /* Log the beginning of the build scripts API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_MC_MEAS_BUILD_SCRIPTS_BEGIN, 0xFF, 0, 0);

  script_type = meas_scripts_param->setup_params->script_type;

  setup_params = meas_scripts_param->setup_params;
  src_tech = setup_params->header.source_tech;
  tgt_tech = setup_params->header.target_tech;

  if ( (tgt_tech == RFCOM_GSM_MODE) || (script_type == RFM_MEAS_PROGRAM_NULL2X) )
  {
    rfm_dev = setup_params->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }
  else if( src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = setup_params->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }

  /* Validate device */
  /* In the case of G2X NULL2X IRAT, source dev will be invalid.
     But we should never get here as GSM meas_enter is not called for G2X NULL2X IRAT */
  if (rfm_dev == RFM_INVALID_DEVICE)
  {
    RF_MSG_4(MSG_LEGACY_ERROR,"rfgsm_mc_meas_build_scripts: Invalid RFM Device %d"
                              " src_tech=%d, tgt_tech=%d and script_type=%d",
                              rfm_dev, src_tech, tgt_tech, script_type);

    return RFM_MEAS_COMMON_FAILURE;
  }

  result = rfgsm_core_meas_build_scripts(setup_params);

  /* Log the end of the build scripts API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_MC_MEAS_BUILD_SCRIPTS_END, 0xFF, 0, 0);

  return(result);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called to exit the measurement mode. It is used to put the RF
    in the right state and do any clean-ups required.

  @param rfm_meas_setup_param : This is a pointer that basically has a header that has
  the target technology and the source technology. Based on this header, the rest is casted as
  a tech specific pointer.

  @pre
  rfm_meas_common_script_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfgsm_mc_meas_exit( rfm_meas_exit_param_type *meas_exit_param, boolean is_source)
{
  rfm_meas_program_script_type script_type;

  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type tgt_tech = RFM_INVALID_MODE;
  rfm_meas_common_param_type *exit_params=NULL;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* Validate input */
  RF_NULL_CHECK_RTN(meas_exit_param, "rfgsm_mc_meas_exit: meas_exit_param is NULL!", RFM_MEAS_COMMON_FAILURE);
  RF_NULL_CHECK_RTN(meas_exit_param->exit_params, "rfgsm_mc_meas_exit: exit_params is NULL!", RFM_MEAS_COMMON_FAILURE);

  /* Need to mutex lock meas enter as W can call from two threads for same device */
  rf_common_enter_critical_section( &rfgsm_core_irat_meas_crit_sect );

  /* Log the beginning of the meas exit API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_MC_MEAS_EXIT_BEGIN, 0xFF, 0, 0);

  script_type = meas_exit_param->exit_params->script_type;

  exit_params = meas_exit_param->exit_params;
  src_tech = exit_params->header.source_tech;
  tgt_tech = exit_params->header.target_tech;

  if ( (tgt_tech == RFCOM_GSM_MODE) || (script_type == RFM_MEAS_PROGRAM_NULL2X) )
  {
    rfm_dev = exit_params->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }
  else if( src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = exit_params->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }

  /* Validate device */
  /* In the case of G2X NULL2X IRAT, source dev will be invalid.
     But we should never get here as GSM meas_enter is not called for G2X NULL2X IRAT */
  if (rfm_dev == RFM_INVALID_DEVICE)
  {
    RF_MSG_4(MSG_LEGACY_ERROR,"rfgsm_mc_meas_exit: Invalid RFM Device %d"
                              " src_tech=%d, tgt_tech=%d and script_type=%d",
                              rfm_dev, src_tech, tgt_tech, script_type);

    rf_common_leave_critical_section( &rfgsm_core_irat_meas_crit_sect );

    return RFM_MEAS_COMMON_FAILURE;
  }

  result = rfgsm_core_meas_exit(meas_exit_param);

  /* Log the end of the meas exit API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_MC_MEAS_EXIT_END, 0xFF, 0, 0);

  rf_common_leave_critical_section( &rfgsm_core_irat_meas_crit_sect );

  return(result);
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called to preconfig target WTR, if required.

  @param rfm_meas_setup_param : This is a pointer that has a header with target
   technology and the source technology info. Based on this header, the
   rest is casted as a tech specific pointer.

  @pre
  rfm_meas_common_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfgsm_mc_pre_config( rfm_meas_setup_param_type *meas_scripts_param, boolean is_source)
{
  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type src_tech, target_tech;
  rfm_meas_common_param_type *setup_params = NULL;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* Validate input */
  RF_NULL_CHECK_RTN(meas_scripts_param, "rfgsm_mc_pre_config: meas_scripts_param is NULL!", RFM_MEAS_COMMON_FAILURE);
  RF_NULL_CHECK_RTN(meas_scripts_param->setup_params, "rfgsm_mc_pre_config: setup_params is NULL!", RFM_MEAS_COMMON_FAILURE);

  /* Log the beginning of the pre config API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_MC_MEAS_PRE_CONFIG_BEGIN, 0xFF, 0, 0);

  setup_params = meas_scripts_param->setup_params;
  src_tech = setup_params->header.source_tech;
  target_tech = setup_params->header.target_tech;

  if ( target_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = setup_params->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }
  else if( src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = setup_params->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }

  result = rfgsm_core_meas_pre_config(meas_scripts_param);

  /* Log the end of the pre config API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_MC_MEAS_PRE_CONFIG_END, 0xFF, 0, 0);

  return(result);
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function disable second WTR after target tech measurements,
    in case if source tech and target tech are on different WTRs.

  @param rfm_meas_setup_param_type : This is a pointer that has a header with target
   technology and the source technology info. Based on this header, t
   he rest is casted as a tech specific pointer.


  @retval rfm_meas_result_type : an enum type denoting whether the function did work
  correctly or not.
 */
rfm_meas_result_type rfgsm_mc_build_reset_scripts(rfm_meas_setup_param_type *meas_scripts_param, boolean is_source)
{
  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type target_tech = RFM_INVALID_MODE;
  rfm_meas_common_param_type *setup_params = NULL;

  /* Time Profiling and Logging variables */
  RFGSM_CMD_PROC_LOG_VARS();

  /* Validate input */
  RF_NULL_CHECK_RTN(meas_scripts_param, "rfgsm_mc_build_reset_scripts: meas_scripts_param is NULL!", RFM_MEAS_COMMON_FAILURE);
  RF_NULL_CHECK_RTN(meas_scripts_param->setup_params, "rfgsm_mc_build_reset_scripts: setup_params is NULL!", RFM_MEAS_COMMON_FAILURE);

  /* Log the beginning of the build reset scripts API */
  RFGSM_CMD_PROC_LOG_CMN_START(RFGSM_MC_MEAS_BUILD_RESET_SCRIPTS_BEGIN, 0xFF, 0, 0);

  setup_params = meas_scripts_param->setup_params;
  src_tech = setup_params->header.source_tech;
  target_tech = setup_params->header.target_tech;

  if ( target_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = setup_params->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }
  else if( src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = setup_params->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
  }

  /* Log the end of the build reset scripts API */
  RFGSM_CMD_PROC_LOG_CMN_STOP(RFGSM_MC_MEAS_BUILD_RESET_SCRIPTS_END, 0xFF, 0, 0);

  return(result);
}


#ifdef FEATURE_RF_ASDIV
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function route the set antenna request upon receiving a msg from RF apps task.

  @param
  rfm_dev        : Device ID
  uint8          : Antenna Position

  @retval None
*/
rfgsm_mc_status_type rfgsm_mc_set_antenna_req(rfgsm_core_data_type *core_data, uint8 ant_pos)
{
  /* Temporarily assign until core layer deprecation of core handle var */
  core_data->rfgsm_core_handle_ptr->buffer_id = core_data->triple_buffer_id;

  /* Transfer the request to rfgsm_core for processing*/
  rfgsm_core_set_antenna(core_data, ant_pos);
  return RFGSM_MC_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Stops the callback that calls rfgsm_core_asd_rsp_trigger .It is called when there is no need to send abort request.
*/
void rfgsm_mc_asd_rsp_check_stop( rfm_device_enum_type rfm_dev )
{
  rfgsm_core_asd_rsp_check_stop(rfm_dev);
}
#endif

/*----------------------------------------------------------------------------*/

static rfmeas_mc_func_tbl_type rfgsm_mc_meas_apis =
{
  rfgsm_mc_irat_info_get, /* rfmeas_mc_irat_info_get_fp*/
  rfgsm_mc_meas_enter, /* rfmeas_mc_enter_fp */
  rfgsm_mc_pre_config, /* rfmeas_mc_pre_config_fp */
  rfgsm_mc_build_reset_scripts, /* rfmeas_build_reset_scripts_fp */
  rfgsm_mc_meas_build_scripts, /* rfmeas_build_scripts_fp */
  rfgsm_mc_meas_exit, /* rfmeas_mc_exit_fp */
};

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function registers the IRAT APIs for GSM with meas module.

  @param None

  @retval None
*/
rfgsm_mc_status_type rfgsm_mc_meas_init()
{
  if(!rfmeas_mc_register(RFCOM_GSM_MODE, &rfgsm_mc_meas_apis))
  {
    return RFGSM_MC_ERROR;
  }

  return RFGSM_MC_SUCCESS;
}


/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function registers NULL for GSM with meas module.

  @param None

  @retval None
*/
rfgsm_mc_status_type rfgsm_mc_meas_deinit()
{
  /* Cmn function does not accept NULL fp table and therefore ignore error */
  (void)rfmeas_mc_register(RFCOM_GSM_MODE, (rfmeas_mc_func_tbl_type *)NULL);
  return RFGSM_MC_SUCCESS;
}




/*----------------------------------------------------------------------------*/

boolean rfgsm_mc_build_ip2_cal_script(rfgsm_core_data_type *core_data_ptr)
{
  return rfgsm_core_build_ip2_cal_script(core_data_ptr);
}



boolean rfgsm_mc_msm_init_ip2_cal(rfgsm_core_data_type *core_data_ptr, uint32 ip2_rxlm_idx, uint32 ip2_txlm_idx)
{
  return rfgsm_core_msm_init_ip2_cal(core_data_ptr,ip2_rxlm_idx,ip2_txlm_idx);
}


uint32 rfgsm_mc_convert_arfcn_to_tx_freq(rfgsm_band_type band, uint16 arfcn)
{
  rfcom_gsm_band_type rfcom_band = RFCOM_NUM_GSM_BANDS;

  if(band >= RF_MAX_BAND_TYPES)
  {

    RF_MSG_1( MSG_LEGACY_ERROR, "rfgsm_mc_convert_arfcn_to_tx_freq failed. Invalid band:%d", band );
    return 0xFFFFFFFF;
  }

  rfcom_band = rfgsm_core_convert_rfband_to_rfcom(band);

  return (rfgsm_core_convert_arfcn_to_tx_freq( rfcom_band, arfcn ));

}



uint32 rfgsm_mc_convert_arfcn_to_rx_freq(rfgsm_band_type band, uint16 arfcn)
{
  rfcom_gsm_band_type rfcom_band = RFCOM_NUM_GSM_BANDS;

  if(band >= RF_MAX_BAND_TYPES)
  {

    RF_MSG_1( MSG_LEGACY_ERROR, "rfgsm_mc_convert_arfcn_to_rx_freq failed. Invalid band:%d", band );
    return 0xFFFFFFFF;
  }

  rfcom_band = rfgsm_core_convert_rfband_to_rfcom(band);

  return (rfgsm_core_convert_arfcn_to_rx_freq( rfcom_band, arfcn ));

}


/*----------------------------------------------------------------------------*/
/*!
 @brief
   This function process burst metrics passed in by GL1
 @return
   boolean success or failure
*/
boolean rfgsm_mc_process_burst_metrics(rfm_device_enum_type rfm_dev,
                                       rfa_rf_gsm_rx_burst_metrics_type **metrics_ptr,
                                       uint8 num_entries)
{
  uint8 i = 0;
  boolean ret_val = TRUE;

  if (metrics_ptr == NULL)
  {
    return FALSE;
  }

  if ( rfgsm_debug_flags.sawless_override == TRUE )
  {
    return FALSE;
  }

  if ( num_entries > RFGSM_MAX_METRICS_ENTRIES )
  {
    return FALSE;
  }

  for ( i = 0; i < num_entries; i++ )
  {
    if ( metrics_ptr[i] == NULL)
    {
      RFGC_MSG_2(MSG_LEGACY_ERROR,
                 "Null metrics ptr encountered in array from GL1! index %d of %d",
                 i,
                 num_entries);
    }
    else if( rfgsm_core_process_burst_metrics(rfm_dev,
                                              metrics_ptr[i]->band,
                                              metrics_ptr[i]->arfcn,
                                              metrics_ptr[i]->rx_lvl_dbm,
                                              metrics_ptr[i]->snr,
                                              metrics_ptr[i]->jdet_value) == FALSE )
    {
      RFGC_MSG_5(MSG_LEGACY_ERROR,
                 "Failed to process burst metrics for band %d, arfcn %d, rx_lvl %d, snr %d, jdet %d",
                 metrics_ptr[i]->band,
                 metrics_ptr[i]->arfcn,
                 metrics_ptr[i]->rx_lvl_dbm,
                 metrics_ptr[i]->snr,
                 metrics_ptr[i]->jdet_value);
    }
  }

  return ret_val;
}

/*----------------------------------------------------------------------------*/
/*!
 @brief
   This function will return SAWLess support for each band on each logical device
   into an 2dimensional array sized by RFM_MAX_WAN_DEVICES and (RFCOM_NUM_GSM_BANDS-1)
 @return
   boolean success or failure
*/
boolean rfgsm_mc_retrieve_sawless_support(boolean support_matrix[RFM_MAX_WAN_DEVICES][RFGSM_MAX_UNIQUE_RFCOM_BANDS])
{
  rfm_device_enum_type i = 0;
  rfcom_gsm_band_type j = 0;

  if (support_matrix == NULL)
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_mc_process_burst_metrics: metrics_ptr is NULL!");
    return FALSE;
  }

  for ( i = 0; i < RFM_MAX_WAN_DEVICES; i++ )
  {
    for ( j = 0 ; j < (RFCOM_NUM_GSM_BANDS-1) ; j++ )
    {
      /*! @todo - This is currently not band specific, so the result will be the same
        for all bands */
      support_matrix[i][j] = rfgsm_core_get_sawless_status( i );
    }
  }

  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*
  @brief
  Get GSM CCS execution time

  @details

  @param
  device ID: rfm_dev
  rfgsm_ccs_exe_info_type: ccs_time_us
*/

uint32 rfgsm_mc_get_wakeup_ccs_exe_us(rfm_device_enum_type rfm_dev)
{
  uint32 mc_ccs_time_us = 0;

  rfgsm_core_handle_type* rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  if((RFM_MAX_DEVICES == rfm_dev)|| (RFM_INVALID_DEVICE == rfm_dev))
  {
     return mc_ccs_time_us;
  }


  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", 0);

  mc_ccs_time_us = rfgsm_core_get_ccs_exe_us((rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.enter_mode_script);

  return mc_ccs_time_us;
}

/*----------------------------------------------------------------------------*/

/*!
 @brief
   This function programs the QFE scenario present in the atuner manager into the hardware

 @return
   boolean TRUE or FALSE
*/
boolean rfgsm_mc_program_qfe_scenario(void)
{
  boolean status;

#if 0
  rf_time_tick_type prof_tick;
  rf_time_type prof_time = 0; /* Time spent in function */
#endif

  status = rfgsm_core_antenna_tuner_program_qfe_scenario();

#if 0
  /*!@todo add time profiling using RFGSM_DEBUG_EN_Q */
  /* End Time Measurement */
  prof_time = rf_time_get_elapsed( prof_tick, RF_USEC );
  MSG_2(MSG_SSID_RF, MSG_LEGACY_MED, "rfgsm_core_antenna_tuner_program_qfe_scenario()::status=%d, prof_time=%d(us)", status, prof_time);
#endif

  return status;
}

#ifdef FEATURE_RF_ASDIV

/*!
 @brief
   Function sets/gets sar_forced_antenna if it is forced

 @return
   Return the antenna to be forced if sar value is 0xFFFE
*/

void rfgsm_mc_set_sar_forced_ant(rfgsm_mc_sar_forced_antenna_enum_type sar_ant_type)
{
  sar_antenna = sar_ant_type;
}

rfgsm_mc_sar_forced_antenna_enum_type rfgsm_mc_get_sar_forced_ant(void)
{
  return sar_antenna;
}

#endif

