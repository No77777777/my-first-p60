/*! 
  @file
  rf_1x_meas.c
 
  @brief
  This file defines the 1x Measurement Interface.
 
  @addtogroup RF_CDMA_1X_MEAS
  @{
*/

/*==============================================================================

  Copyright (c) 2010 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc..

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_cdma.mpss/3.11/1x/rf/src/rf_1x_meas.c#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
11/24/17   rs      Added support for FR34448 NULL2L IRAT measurement.
11/15/17   am      back out fix for issue during 1x/D) -> L QTA 
08/09/17   am      fixed crash in 1x RF while doing 1x + W QTA on TA
03/10/16   rs      Changing RF_CDMA_CCS_EVENT* to RFLM_CDMA_MDSP_CCS_EVENT*
03/03/16   rs      Added support for 1x2G QTA
12/11/15   rs      Fixed rflm_cmn_release_tq_pair() releasing 1x TQ failed
12/03/15   rs      NV support to enable/disable LPM feature
12/01/15   rs      Accessing the band info from v2 params for meas aborted cases
10/14/15   rs      Modified sleep API to fix DO2L iRAT crash
08/17/15   zhw     Move 1x/HDR meas tune-away & reset script into private buffers
08/18/15   zhw     Moved disable diversity call to meas exit
08/10/15   zhw     Populate tuneback reset script with prx settings
08/10/15   zhw     Use TGT PRx Device for TQ Device if SRC Device is invalid
07/27/15   zhw     Updated TQ device query in QCTA
07/27/15   zhw     Handled TQ device query in QCTA SCC only measurement
06/30/15   ndb     Added Tuner IRAT support in RxTx split feature
06/26/15   spa     Add band as argument to wtr power down API 
06/24/15   spa     Power down WTR in case of abort 
05/14/15   zhw     Added more messages for callflow exceptions
05/11/15   zhw     Disable Drx support during L->1x QTA
05/08/15   wwl     Add W/T to 1x QTA support
04/28/15   zhw     Reworked Tuner API
04/24/15   rmb     Add a source flag for enter, build, reset scripts and exit.
03/26/15   zhw     1x QCTA use source tech device in irat data for TQ allocation
03/26/15   zhw     Convert tech params from pointer to unions to unions
03/16/15   zhw     Support Prx+Drx tune in 1x iRAT/MEAS tune-away call flow
03/12/15   zhw     Adapt to unified meas param interface
02/26/15   zhw     Hook up existing FTM iRAT test functionality with param v2
02/03/15   vr     Cleanup the TQP resources in case of a aborted GAP.
12/01/14   as/spa  Protected tuner configuration under ASDiv lock
06/28/14   fh      Added api for cleanup script
02/13/14   APU     Request TQ just before starting Rx. Release TQ before going 
                   to sleep thus guaranteeing symmetric TQ request\release.
11/27/13   APU     IRAT Changes FOR bolt.
01/16/13   pk      IRAT Reset Script for Tuner disable
08/22/13   spa     Check for OHO/abort only when CDMA is target tech
08/15/13   Saul    SB. Made rf_state 1x/hdr specific.
08/13/13   spa     Get device based on 1x being source or target in meas exit
08/06/12   APU     In meas_exit, check target_param for NULL before accessing it
08/01/13   APU     Support for optimised hand off case where build scripts is 
                   skipped.    
07/08/13   APU     Use L1 abort flag to skip meas exit events.
07/05/13   spa     Use rf_cdma_meas_cleanup_ccs_events for CCS event cleanup
07/03/13   APU     Added support for IRAT abort
07/03/13   spa     Renamed scripts_build_scripts and added documentation
07/01/13   APU     Call RX_STOP from 1 place and remove 
                   rf_1x_cleanup_post_measurement() because we use sleep when 
                   call meas_exit()
06/27/13   spa     Removed unused/no-op APIs 
06/27/13   spa     Add 'is_irat_mode' flag to APIs to check if API call is in 
                   IRAT or non IRAT scenarios 
06/27/13   APU     Fix for 1x2L irat crash where sleep after gap had invalidated
                   Variables.
06/14/13   APU     IRAT APIs now use the MC APIs instead of having seperate code
                   and call flow.
06/14/13   spa     Use CDMA wrapper object for antenna tuner
06/04/13   cd      Add support to clean-up Rx AGC LNA SM events
05/30/13   spa     Mainlined "use_new_fwrsp", use new RF dispatch implementation 
05/29/13   spa     Added delay param to APIs for getting GRFC scripts
05/24/13   APU     Eliminated unwanted event information structures.
05/22/13   sty     Acquire CS only when needed - in 
                   rf_1x_cleanup_post_measurement
05/09/13   spa     Added missing header (compiler warning fix)
05/08/13   spa     Use common buffer for creating post meas script (as target)
04/26/13   spa     Fix check for source/target in build reset script before 
                   printing F3 
04/10/13   ndb     Removed the unused Qtuner Nikel Functions
04/08/13   pl      rename build_cleanup_script top build_reset_script
04/08/13   pl      Add null function pointer to support pre_config
04/04/13   spa     Cleaned up unused code
04/01/13   spa     Migrated to RF Dispatch for RF-FW synchronization
03/19/13   spa     Updated cleanup CCS events API
03/19/13   spa     Removed redundant APIs checking for SV devices
02/06/13   bmg     Adding initial antenna tuner manager support
01/28/13   zhw     RF front end device wakeup/sleep API refactor
01/12/13   cd      RxAGC LNA SM event configuration support
12/27/12   shb     Updates for different physical rx_device per band
12/18/12   sty     KW fix for possible NULL pointer dereference
12/04/12   aro     F3 message update
11/29/12   APU     Changed the name of IRAT events as per Code Review.
11/29/12   av      Using proper featurization and removing featurization that is not needed
11/26/12   APU     Changes to clean CCS events
11/20/12   gvn     Featurize LTE for Triton 
11/20/12   APU     Fixed a bug. Use the index 0 for 1 carrier. The 0th carrier
                   does not correspond to0th NB anymore in DIME. 0th bit means 
                   use 1 carrier, 1st means use 2 carriers, 2nd means 
                   3 carriers. 
11/19/12   APU     Implemented rf_1x_mc_meas_build_cleanup_scripts().
11/13/12   APU     Make changes to accomodate the meas framework changes.
11/09/12   zhw     Fix compiler warning
11/07/12   zhw     Support for RF device scripting. Rename RF event script
11/07/12   pl      Added cleanup function pointer support in API structure
10/31/12   adk     Merged antenna tuner code from Nikel.
10/17/12   sty     Added F3 for failure condition in rf_1x_mc_meas_init
10/12/12   sty     Deleted refs to ant_tuner
10/10/12   APU     New IRAT interface changes.
09/28/12   aro     Migrated 1x MC to use new RF-FW Sync
09/28/12   zhw     Fix Compiler warning
09/27/12   zhw     Remove write_sbi/grfc_via_rfsw flag
09/17/12   sty     Changed order of args in rf_cdma_mdsp_configure_devices
09/14/12   spa     Removed refs to unsupported SPI antenna tuner scripts
09/13/12   sty     Renamed RF_CDMA_EVENT_ANALOG_SCRIPT to RF_CDMA_STATIC_EVENT
09/10/12   spa     Removed references to v2 device scripting APIs
09/10/12   spa     Removed unused API append_new_style_script_to_old_style
09/10/12   spa     Added WTR scripting support and removed legacy methods 
09/06/12   aro     FWRSP registration for IRAT_RX_START response
09/05/12   aro     Added RF-FW Sync callback parameter in MDSP Interface
08/28/12   spa     changed asm device to per band 
08/22/12   aro     Renamed interface names and type definitions (time prof)
08/22/12   aro     Changes to migrate to new precide time profiler
08/21/12   APU     Added support to get modem_chain from rfc_device_info
08/15/12   zhw     Added RF event handle in rf_cdma_data, which are used to 
                   control CCS event flow
08/08/12   adk     Added support for scripting buffer interface.
08/08/12   cvd     Replace timetick with rfcommon_time_profile interface
07/27/12   APU     Enabled scripting for PA ASM. 
07/23/12   Saul    CDMA. Now using semaphore APIs from RF COMMON.
07/11/12   shb     Corrected type of rf buffer script ptr
07/11/12   shb     Updated script_buffer to be of rf_buffer_intf* type 
                   instead of void ptr
07/06/12   spa     Updated rfc_cdma_generate_rx_wakeup_grfc_script to new params 
07/03/12   spa     Changed do_grfc_writes_in_rf_pre_state to boolean type 
07/03/12   spa     Changed do_sbi_in_rf_pre_state to boolean type 
06/29/12   spa     Compiler warning fix 
06/28/12   APU     Removed old QFE specific calls. Replaced with new generic PA 
                   interface. 
06/28/12   APU     Changed QFE to the new generic PA interface. 
06/19/12   zhw     QFE1320 scripting support
06/13/12   aro     Updated RxLM interface to indicate, if dynamic config is to 
                   be applied or not + if ET configuration is to be applied
06/06/12   Saul    CDMA. Sending scripts as Dime shared mem format.
                   MC now using mdsp events.
05/29/12   zhw     Moved debug flag do_grfc_write to rf_cdma_data 
                   Removed inclusion of rfm_internal.h  
05/29/12   zhw     Refactored API naming  
05/19/12   zhw     Moved debug flag do_sbi_write to rf_cdma_data 
05/14/12   aro     Removed CritSect from Meas Script Enter and Exit function
05/11/12   zw      Updated GRFC scripting generation APIs
05/10/12   zw      Renamed rtr_handle to script_index 
05/09/12   aro     Moved the semaphore POP before message is sent to FW to avoid
                   race condition
05/09/12   aro     Updated RX_STOP, TX_START, TX_STOP to use Sem Pool for 
                   synchronization
05/01/12   zhaow   Updated rf_cdma_ic_stop()
04/27/12   zhaow   Updated rf_cdma_ic_stop()
04/26/12   sty     Removed zeroing out num_grfcs
04/25/12   zhaow   Added support for CDMA #defined Constants.
04/22/12   cd      LTE-1x connected mode measurement fix. Initialize return 
                   status variable.
04/19/12   Saul    1X/HDR. Handoff fix. Zero out GRFC num only once.
04/17/12   sty     Deleted references to enable_sbi_scripting
04/16/12   sty     Added GRFC script to rf_1x_mdsp_configure_analog_script() 
04/15/12   sty     Added leave_critical section that was missed 
04/12/12   aro     Set blocking call variable before sending the message
                   to firmware
04/11/12   Saul    QFE. Initial support.
04/09/12   sty     Added helper functions that:
                  (a)get and release device-locks                  
                  (b) program the RTR and grfcs
                  (c) send relevant commands to MDSP (RX_START, etc)
04/03/12   aro     CB functions in RX_STOP interface
04/02/12   aro     Added CB as argument in Rx / Tx MDSP interfaces
04/02/12   aro     Added CB as argument in Rx MDSP interfaces
03/29/12   shb     Added rf_state arg to rf_1x_configure_rx_agc as it is needed 
                   by device lna phase offset query api 
03/29/12   sty     use do_grfc_writes_in_rf_sw instead of DO_GRFC_WRITES_IN_RF_SW
03/22/12   aro     Migrated 1x FW response handling to RF_FWRSP task
03/21/12   Saul    IRAT. KW Fix from previous check-in
03/21/12   Saul    IRAT. 1X to set second device to sleep in L21X.
03/21/12   spa     Added max device check in rf_cdma_get_mutable_device_status()
03/21/12   adk     Update rxf_get_iqmc_coef_type to retrieve IQMC coeff for SHDR
03/15/12  Saul/aro IRAT. 1x2L 1x Stop Rx sequence change to delay premature 
                   RF sleep/vreg shutdown which affects LTE measurements.
03/14/12   vrb     Configure RxLM API requires band of operation
02/27/12   spa     Updated execute_wakeup_rx to use CDMA specific band and chan  
02/08/12   shb     Added argument in CDMA device rx tune api to distinguish 
                   between MEAS script generation and standalone wakeup
01/20/12   shb     Updated CDMA device interface to use generic 
                   rfdevice_ssbi_write_type and rfdevice_script_type instead of
                   CDMA specific structures
01/19/12   Saul    MCDO. Added ICIFIR dynamic configuration.
01/19/12   aro     Updated all HAL interfaces to use RxLM/TxLm handle rather
                   than logical device
01/11/12   Saul    MCDO. Passing bandwidth for IQMC coefficients query.
                   Passing mode to Tx tune script.
01/04/12   aro     Cleaned up the Rx and Tx bandwidth configuration
01/03/12   aro     Interface to query TxLM chain for the given TxLM buffer
12/29/11   aro     Interface update to pass ADC info to RxLM configure function
12/29/11   aro     Updated configure RxAGC interface to have RxLM buffer
12/21/11   aro     KW Fix
12/21/11   aro     Updated 1x script functions to have one return in the code
                   to avoid early returns
12/21/11   aro     Added code to acquire and release CDMA critical section
12/19/11   sty     Init dyn_settings before using it 
12/17/11   aro     Fixed the bug where the Script Flag gets hung in case of
                   iRAT script generation failure
12/13/11   Saul    MCDO. Passing rx/tx bandwidth to device driver.
12/06/11   adk     Code cleanup
11/23/11   Saul    IRAT update. Added RTR reset in Rx reset API to fix lower
                   Rx AGC readings that happen in some channels.
11/21/11   Saul    IRAT update. Fixed Rx AGC.
10/28/11   Saul    IRAT update. Added debug messages.
10/28/11   Saul    IRAT update. Antenna Tuner support.
10/05/11   Saul    IRAT update. Changed PLL settling from 500 to 615 CX1 units.
09/28/11   Saul    IRAT update. Added debug message.
09/27/11   adk     Moved SSBI scripting-related flags to rf_cdma_debug_flags.
09/21/11   adk     Removed obsolete code.
09/19/11   Saul    IRAT update. Use 1x Rx AGC Acq Duration of 2 ms for IRAT.
09/11/11   aro     Merged CDMA device interface to delete
                   rfdevice_1x_interface.h
09/08/11   Saul    IRAT bug-fix. Build scripts regardless of target tech.
09/08/11   Saul    IRAT update. Use narrow-band 2.
08/04/11   Saul    IRAT update. Added control to use actual RTR handle.
08/03/11   Saul    IRAT update. Now configuring flags so that FW writes scripts.
07/29/11   Saul    IRAT update. Skip setting GPIOs during IRAT.
07/28/11   Saul    IRAT updates. 
                   - Now sending rx start config message to fw.
                   - Re-store AGC ACCUM
07/25/11   Saul    IRAT updates. 
                   - Now sending pilot_meas_cfg message.
                   - GRFC start delta sent in cx8 units.
                   - Override of debug flags done in FTM meas APIs.
07/23/11   Saul    Converted rx wake up APIs to use GRFC generate script.
07/18/11   aro     Updated RxAGC Config interface to pass AGC Mode Param
07/15/11   Saul    IRAT updates.
07/14/11   Saul    IRAT updates.
07/08/11   Saul    Preliminary IRAT changes.
07/08/11   Saul    Preliminary IRAT changes.
01/08/11   aro     Changed @ingroup to @addtogroup
01/05/11   aro     Moved @defgroup to rf_cdma_documentation.h
12/13/10   aro     Added 1x Measurement APIs
12/07/10   aro     Initial Revision.

==============================================================================*/

#include "rfa_variation.h"
#include "comdef.h"
#ifdef FEATURE_CDMA1X
#include "rf_1x_meas.h"
#include "rfm_cdma.h"
#include "rfcommon_msg.h"
#include "rf_cdma_intelliceiver.h"
#include "rf_cdma_msm.h"
#include "rfc_cdma.h"
#include "rf_cdma_utils.h"
#include "rf_cdma_utils_freq.h"
#include "rfdevice_cdma_interface.h"
#include "rf_1x_mdsp.h"
#include "rf_1x_mc.h"
#include "rf_1x_rx_agc.h"
#include "rf_cdma_data.h"
#include "modem_mem.h"
#include "rf_cdma_constants.h"
#include "rfcommon_time_profile.h"
#include "rf_hal_buffer.h"
#include "rf_cdma_mc.h"
#include "rfm_internal.h"
#include "rfmeas_mdsp.h"
#include "rf_cdma_meas.h"
#include "rfcommon_nv_mm.h"

static rf_buffer_intf *rf_1x_meas_setup_buffer = NULL;
static rf_buffer_intf *rf_1x_meas_cleanup_buffer = NULL;

/*============================================================================*/
/*!
  @name Helper functions for 1x Meas

  @brief
  This section holds all helper functions used by 1x Meas
*/
/*! @{ */

/*! @} */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Setup and Build 1x Script

  @details
  This functions initializes and start to build iRAT script for 1x, by calling 
  MC layer APIs, and populating FW shared mem with script to Wakeup Rx
  Once done, L1 sends the pilot meas config command to FW to trigger Rx 
  measurements and collect Rx AGC samples.

  @param cdma1x_setup_and_build_scripts_param
  Build Script Parameter

  @param meas_type
  mes type to pass in the cript type in IRAT or non-IRAT scenarios
  In IRAT scenarios, startup or cleanup script in No-IRAT mode always invalid.  
 
  @return
  Status of function execution 
*/
static rfm_meas_result_type 
rf_1x_meas_configure_for_tune_away 
(
  rfm_meas_setup_param_type *rfm_meas_setup_param,
  rfm_meas_1x_params_type* irat_meas_param
)
{
  rfm_meas_result_type result_type = RFM_MEAS_COMMON_FAILURE;

  RF_MSG( RF_HIGH, "rf_1x_meas_script_build_scripts : START" );
 
  /* Acquire Critical Section Lock */
  rf_common_enter_critical_section(rf_cdma_get_lock_data());

  if(irat_meas_param == NULL)
  {
    RF_MSG( RF_ERROR, "RF 1X IRAT - NULL ptr detected." );
  } /* if(irat_meas_param == NULL) */
  else
  {  
    rfm_device_enum_type device;
    rfm_device_enum_type div_device;
    rfm_cdma_band_class_type band;
    rfm_cdma_chan_type chan;
    uint32 buffer_index;
    lm_handle_type rxlm_handle;
    lm_handle_type div_rxlm_handle;
    uint8 dev_handle_idx;
    rfm_device_enum_type meas_tq_device;

    /*------------------------------------------------------------------------*/
    /* Init local vars */
    /*------------------------------------------------------------------------*/
    boolean irat_cdma_success = FALSE ; 

    device = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[0].device; 
    rxlm_handle = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[0].lm_handle;

    div_device = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[1].device; 
    div_rxlm_handle = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[1].lm_handle;

    meas_tq_device = rfm_meas_setup_param->setup_params->header.source_param_v2[0].rx_dev_handle[0].device; 

    /* It's possible that LTE (Source Tech) does QTA (MEAS) on SCC only, in which
    case 1x has to request a new TQ Pair. The Interface will be arranged like below.
         PCC QTA Case:                             SCC QTA Case
    | LTE DEV 0  1x DEV 0 |                     | ---        1x DEV 0 |
    | ---        ---      |                     | LTE DEV 0  ---      |
    | ---        ---      |                     | ---        ---      |
    | ---        ---      |                     | ---        ---      | 
    */
    if ( meas_tq_device == RFM_INVALID_DEVICE )
    {
      /* If TQ Device is INVALID from SOURCE TECH, fall back to use TGT PRX 
      Device and request a new TQ Pair. This is valid for QCTA/QDTA cases */
      meas_tq_device = device;     
    }    

    RF_MSG_5 ( RF_HIGH  , "rf_1x_meas_script_build_scripts(): " 
               "| Dev: %d | rxlm: %d | Div Dev %d | Div RxLM %d | TQ Dev: %d" ,
               device , rxlm_handle, div_device, div_rxlm_handle, 
               meas_tq_device ) ;

    if ( meas_tq_device == RFM_INVALID_DEVICE  )
    {
      RF_MSG(RF_ERROR,"rf_1x_meas_configure_for_rx_wakeup: INVALID TQ Device. "
                      "Script building failure");
      result_type = RFM_MEAS_COMMON_FAILURE ;
    }
    else
    {    
      band = irat_meas_param->band_chan.band; 
      chan = irat_meas_param->band_chan.chan_num;

      buffer_index = 0; /* Buffer index for iRAT is always 0 now */
    
    
      /* Call API to initialize IRAT specific part of CDMA data structure, this 
      API updates the buffer index, rxlm handle to the params sent in by L1 & 
      intelliceiver power mode to the lowest power mode*/ 
      irat_cdma_success = 
        rf_cdma_init_irat ( device , buffer_index , rxlm_handle, meas_tq_device ) ;
    
      /* Clear 1x global meas buffer before building any script */         
      irat_cdma_success &= 
        rf_buffer_clear(rf_1x_meas_mdsp_buffer_get(RFM_MEAS_PROGRAM_STARTUP));

      if ( irat_cdma_success != TRUE )
      {
        RF_MSG(RF_ERROR, "rf_1x_mc_meas_build_reset_scripts: Buffer Clean up failed");
      }
      
      if ( TRUE == irat_cdma_success )
      {
        /* Prepare radio for wakeup. This API is intended to 
        update the CDMA data structure and state machines. In IRAT scenario
        this API must NOT touch the vregs */ 
        irat_cdma_success = 
          ( rf_1x_mc_prep_wakeup_rx ( device, rxlm_handle, band, chan,
                                      TRUE /* IRAT mode */ ) >= 0 ) ;
        if ( TRUE == irat_cdma_success )
        {
          rf_buffer_intf *rx_tune_script;

          /* Call exec_wakeup_rx, this will prepare the digital and analog front 
          end for Rx, in IRAT mode , skip device power vote, DLNA update,
          Skip sending of Rx Start mssg and cleanup of Rx start CCS events */ 
          irat_cdma_success = 
            ( rf_1x_mc_exec_wakeup_rx ( device, rxlm_handle,TRUE/*Irat mode*/,
                                        RFDEVICE_MEAS_STARTUP_SCRIPT ) >=  0) ; 

          if ( div_device != RFM_INVALID_DEVICE )
          {
            /* If there is a valid Drx device, configure that logical path
            for Rx in the same manner that Prx is configured. */
            irat_cdma_success = 
               ( rf_1x_mc_enable_diversity ( device, rxlm_handle, 
                                        div_device, div_rxlm_handle, TRUE,
                                             RFDEVICE_MEAS_STARTUP_SCRIPT ) >= 0); 
          }

          /* Device script is configured to PRx logical path resources after
          Both Prx and Drx (if applicable) have settings populated */
          rx_tune_script = rf_1x_meas_mdsp_buffer_get(RFM_MEAS_PROGRAM_STARTUP);

          irat_cdma_success &= rf_cdma_mc_configure_script_helper(  device, 
                                                                    RFM_1X_MODE,
                                                                    rxlm_handle,
                                                                    buffer_index,
                                                                    rx_tune_script,
                                                                    RF_CDMA_EVENT_IRAT_TUNE_IN,
                                                                    RFLM_CDMA_MDSP_CCS_EVENT_RF_IRAT_TUNE_IN_ID );

          if ( TRUE != irat_cdma_success )
          {
            RF_MSG ( RF_ERROR , "rf_1x_meas_script_build_scripts(): "
                                "script building has failed " ) ;
          }
        }/* if ( TRUE == irat_cdma_success )*/
        else
        {
          RF_MSG ( RF_ERROR , "rf_hdr_meas_script_build_scripts(): "
                              "rf_1x_mc_prep_wakeup_rx() has failed " ) ;
        }/* if ( TRUE != irat_cdma_success )*/
      }/* if ( TRUE == irat_cdma_success )*/
      else
      {
        RF_MSG ( RF_ERROR ,"rf_hdr_meas_script_build_scripts(): "
                           "rf_cdma_init_irat() has failed " ) ; 
      }/* if ( TRUE != irat_cdma_success )*/
    
      if ( TRUE == irat_cdma_success )
      {
        result_type = RFM_MEAS_COMMON_SUCCESS ;
      }
      else
      { 
        RF_MSG( RF_ERROR, "rf_1x_meas_script_build_scripts(): "
                  "rf_1x_mdsp_start_rx_irat Failed  " ) ;
      }
    }/* if ( meas_tq_device != RFM_INVALID_DEVICE  ) */
  }/* if(  irat_meas_param == NULL ) */
     
  RF_MSG_1( RF_HIGH, "rf_1x_meas_script_build_scripts : Status [%d]",
            result_type );

  /* Release Critical Section Lock */
  rf_common_leave_critical_section(rf_cdma_get_lock_data());
  return result_type;

} /* rf_1x_meas_script_build_scripts */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Helper function for tuneback   

  @details
  Tuneback function for QTA 

  @param irat_meas_param
  Build Script Parameter

  @param meas_type
  meas type to pass in the script type in IRAT or non-IRAT scenarios
  
  @return
  Status of function execution 
*/
static rfm_meas_result_type
rf_1x_meas_configure_for_tune_back
(
  rfm_meas_setup_param_type* rfm_meas_setup_param, 
  rfm_meas_1x_params_type *irat_meas_param,
  const rfdevice_meas_scenario_type meas_type
)
{
  rfm_meas_result_type result_type = RFM_MEAS_COMMON_FAILURE ;
  rfm_device_enum_type device;
  uint32 buffer_index;
  lm_handle_type rxlm_handle;
 
  /* Acquire Critical Section Lock */
  rf_common_enter_critical_section(rf_cdma_get_lock_data());

  /* Continue only if irat_meas_param is not NULL */
  if(irat_meas_param == NULL)
  {
    RF_MSG( RF_ERROR, "RF 1x IRAT - NULL ptr detected for meas params." );
  } /* if(irat_meas_param == NULL) */
  else
  {
    /*------------------------------------------------------------------------*/
    /* Init local vars and extract the info from the meas params              */
    /*------------------------------------------------------------------------*/
    boolean irat_cdma_success = FALSE ; 
    rfm_device_enum_type meas_tq_device = 
      rfm_meas_setup_param->setup_params->header.source_param_v2[0].rx_dev_handle[0].device; 
    device = 
      rfm_meas_setup_param->setup_params->header.source_param_v2[0].rx_dev_handle[0].device;
    rxlm_handle = 
      rfm_meas_setup_param->setup_params->header.source_param_v2[0].rx_dev_handle[0].lm_handle;
    buffer_index = 0 ; /* Buffer index for iRAT is always 0 now */

    RF_MSG_3 ( RF_HIGH  , "rf_1x_meas_configure_for_tune_back(): " 
			   "| Dev: %d | rxlm: %d | TQ Dev: %d" , 
                           device , rxlm_handle, 
			   meas_tq_device ) ;

    /* Call API to initialize IRAT specific part of CDMA data structure, this 
       API updates the buffer index, rxlm handle to the params sent in by L1 & 
       intelliceiver power mode to the lowest power mode */ 
    irat_cdma_success = 
      rf_cdma_init_irat ( device , buffer_index , rxlm_handle, meas_tq_device );

    if ( TRUE == irat_cdma_success )
    {
      if(RFM_CDMA_HEALTHY_STATE != rf_cdma_mc_tune_back( device,RFM_1X_MODE ))
      {
          irat_cdma_success = FALSE;
      }
    }/* if ( TRUE == irat_cdma_success ) */
    else
    {
      RF_MSG ( RF_ERROR ,"rf_1x_meas_configure_for_tune_back(): "
                         "rf_cdma_init_irat() failed " ) ;
    }/* if ( TRUE != irat_cdma_success ) */

    if ( TRUE == irat_cdma_success )
    {
      result_type = RFM_MEAS_COMMON_SUCCESS ;
    }
    else
    {
      RF_MSG( RF_ERROR, "rf_1x_meas_configure_for_tune_back(): "
                  "rf_1x_mc_tune_back Failed  " ) ;
    }
  } /* if !(irat_meas_param == NULL) */

  RF_MSG_1( RF_HIGH, "rf_1x_meas_configure_for_tune_back : Status [%d]",
            result_type );

  /* Release Critical Section Lock */
  rf_common_leave_critical_section(rf_cdma_get_lock_data());

  return result_type;

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Cleanup 1x to prepapre 1x_2_X measurement

  @details
  This function will be called to perform cleanup on 1x before any 1x to X
  iRAT. Cleanup refers to the stopping of RX. This will move the RF State
  to RF_CDMA_STATE_SHUTDOWN_FOR_IRAT state.
 
  @param irat_meas_param
  Pointer to iRAT measurement data pertaining to 1x
 
  @return
  Status of function execution
*/
rfm_meas_result_type
rf_1x_stop_rx_for_irat_meas
(
  rfm_device_enum_type device
)
{
  rfm_meas_result_type result_type = RFM_MEAS_COMMON_SUCCESS;
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */
  /* Acquire Critical Section Lock */
  rf_common_enter_critical_section(rf_cdma_get_lock_data());

  RF_MSG( RF_HIGH, "rf_1x_cleanup_for_measurement - START" );

    /* From CDMA to X, scriptBufferIndex and RxLM buffer ishould be the original ones */
    /* Get Writable Device Status */
  dev_status_w = rf_cdma_get_mutable_device_status( device );
    if ( NULL != dev_status_w )
    {
    boolean success = rf_cdma_init_irat ( device , 
                                          dev_status_w->script_index ,
                                          dev_status_w->rxlm_handle,
                                          device ) ;
      if ( TRUE == success )
      {
        if ( rf_1x_mc_sleep_rx ( device , dev_status_w->rxlm_handle, 
                                 TRUE, /* IRAT mode */ 
                                 TRUE /* IRAT enter mode */)  < 0 )
        {
          RF_MSG ( RF_ERROR, "rf_1x_cleanup_for_measurement(): "
                   "Call to rf_1x_mc_sleep_rx() failed " ) ; 
          result_type = RFM_MEAS_COMMON_FAILURE;
        }
      }
    }
    
  RF_MSG_1( RF_HIGH, "rf_1x_cleanup_for_measurement : Status [%d]",
            result_type );

  /* Release Critical Section Lock */
  rf_common_leave_critical_section(rf_cdma_get_lock_data());

  return result_type;

} /* rf_1x_cleanup_for_measurement */
            

/*! @} */

/*============================================================================*/
/*!
  @name 1X APIs for IRAT

  @brief New APIs for the new IRAT model
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function will populate all the timing information related to IRAT
  measurements. This timing information will be used by L1 scheduling the 
  measurements during IRAT gap.
 
  @param rfm_meas_irat_info_type: data structure for all info to be returned
 
  @pre
  rfm_init() must have already been called. We should be in a certain 
  technology.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function
  did work correctly or not.
*/
rfm_meas_result_type 
rf_1x_mc_irat_info_get
( 
  rfm_meas_irat_info_type *rfm_meas_irat_info_param
)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;

  RF_MSG ( RF_MED , "rf_1x_mc_irat_info_get: Start" ) ;
  
  if ( rfm_meas_irat_info_param != NULL ) 
  {
    /* Target Tech is 1X; Source Tech does not matter  */
    if ( rfm_meas_irat_info_param->header.target_tech == RFM_1X_MODE )
    {
      if ( ( (NUM_OF_MAX_SBI_AFTER_WRITES * SBI_WRITE_TIME_IN_NS)/1000 ) < 
           GENERIC_PLL_LOCK_TIME_IN_US )
      {
        rfm_meas_irat_info_param->startup_time = 
          (NUM_OF_MAX_SBI_BEFORE_WRITES * SBI_WRITE_TIME_IN_NS)/1000 + 
          GENERIC_PLL_LOCK_TIME_IN_US ;
      }
      else
      {
        rfm_meas_irat_info_param->startup_time = 
           ( (NUM_OF_MAX_SBI_BEFORE_WRITES + NUM_OF_MAX_SBI_AFTER_WRITES) *
              SBI_WRITE_TIME_IN_NS )/1000;
      }
    }

    /* Source Tech is 1X; Target Tech does not matter  */
    if ( rfm_meas_irat_info_param->header.source_tech == RFM_1X_MODE )
    {
      if ((NUM_OF_MAX_SBI_AFTER_WRITES*SBI_WRITE_TIME_IN_NS)/1000 < GENERIC_PLL_LOCK_TIME_IN_US)
      {
        rfm_meas_irat_info_param->cleanup_time = 
          ((NUM_OF_MAX_SBI_BEFORE_WRITES * SBI_WRITE_TIME_IN_NS)/1000) + 
          GENERIC_PLL_LOCK_TIME_IN_US ;
      }
      else
      {
        rfm_meas_irat_info_param->cleanup_time = 
          ( (NUM_OF_MAX_SBI_BEFORE_WRITES + NUM_OF_MAX_SBI_AFTER_WRITES ) * 
            SBI_WRITE_TIME_IN_NS )/1000;
      }
    }
  }
  else
  {
    result = RFM_MEAS_COMMON_FAILURE ;
    RF_MSG ( RF_ERROR , "rf_1x_mc_irat_info_get(): "
             "rfm_meas_irat_info_param is NULL " ) ;
  }
  return result ;

} /* rf_1x_mc_irat_info_get */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is called first when the measurement is requested. Each
  technology basically prepares for the measurement.
 
  @param rfm_meas_enter_param : This is a pointer that basically has a header
  that has the target technology and the source technology. Based on this 
  header, the rest is casted as a tech specific pointer.
 
  @pre
  rfm_init() must have already been called.
  We should be in a certain technology.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function
  did work correctly or not.
*/
rfm_meas_result_type 
rf_1x_mc_meas_enter
( 
  rfm_meas_enter_param_type *rfm_meas_enter_param,
  boolean is_source
)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfm_meas_header_type *meas_header_p = 
              &(rfm_meas_enter_param->enter_params->header);
  if ( rfm_meas_enter_param == NULL ) 
  {
    result = RFM_MEAS_COMMON_FAILURE ;
    RF_MSG ( RF_ERROR , "rf_1x_mc_meas_enter(): "
             "rfm_meas_enter_param is NULL " ) ;
  }
  else if (  rfm_meas_enter_param->enter_params->script_type == RFM_MEAS_PROGRAM_NULL2X )
  {
    /* Intentional left blank. */
  }
  else
  {
    RF_MSG_2 ( RF_MED , "rf_1x_mc_meas_enter: Start SRC = %d, TGT = %d", 
               meas_header_p->source_tech, meas_header_p->target_tech ) ;

    /* Common Steps for any target tech begin **** */
    /* Common Steps for any target tech end ****** */

    /* Source Tech = 1X ; Target Tech = {header.target_tech} */
    switch (meas_header_p->target_tech)
    {
      /* 1x --> LTE */
      case RFCOM_LTE_MODE:
      /* 1x --> T */
      case RFCOM_TDSCDMA_MODE:
        /* Perform Clean-up on 1x for 1x2X measurement */
        result &= 
          rf_1x_stop_rx_for_irat_meas ( meas_header_p->source_param_v2[0].rx_dev_handle[0].device );
        if ( FALSE == result ) 
        {
          RF_MSG ( RF_ERROR , 
                 "rf_1x_mc_meas_enter(): "
                 "rf_1x_cleanup_for_measurement has returned FALSE ") ;
        }
        break;

      case RFCOM_GSM_MODE:
      /* 1x --> W */
      case RFCOM_WCDMA_MODE:
        /* No need to do anything as 1x is the source tech and also in QTA, 
           1x stack is expected to run till the GAP starts in order to have 
           better throughput (main advantage of QTA compared to iRAT where rxagc of 
           1x is stopped during enter mode itself) */
        break ;

      /* Add case statements here for other target techs like W, G etc.. */
      default:
        /* ***** 
        Nothing to be done in default for now. 
        Either we are source or target tech if we have reached this point in the code 
        ***** */
        break;
    }

    /* Target Tech = 1X ; Source Tech = {header.source_tech} */
    switch (meas_header_p->source_tech)
    {
      /* Initialize tqp device for Abort case */
      /* Target Device */
      rfm_device_enum_type meas_tq_device = 
        meas_header_p->source_param_v2[0].rx_dev_handle[0].device ;

      /* Source Params */
      rfm_device_enum_type device = 
         meas_header_p->target_param_v2[0].rx_dev_handle[0].device ;
      lm_handle_type rxlm_handle = 
         meas_header_p->target_param_v2[0].rx_dev_handle[0].lm_handle ;

      boolean init_irat_success = 
        rf_cdma_init_irat ( device , 0 , /* buffer_index of 0 as unused */
                        rxlm_handle, meas_tq_device ) ;
      RF_MSG_4 ( RF_LOW ,  "rf_1x_mc_meas_enter(): X21x "
                "rf_cdma_init_irat()[Status: %d] for Abort case "
                "||SrcDev: %d|TgtDev: %d|TgtLM: %d|| " ,
                 init_irat_success ,meas_tq_device , device , rxlm_handle ) ;
 
      /* LTE --> 1X */
      case RFCOM_LTE_MODE:
      /* 1x --> W */
      case RFCOM_WCDMA_MODE:
      /* 1x --> T */
      case RFCOM_TDSCDMA_MODE:
        RF_MSG ( RF_LOW ,  "rf_1x_mc_meas_enter(): "
               "Nothing to do here when target tech ") ;
        break;

       /* Add case statements here for other source techs like W, G etc.. */

      default:
        /* ***** 
        Nothing to be done in default for now. 
        Either we are source or target tech if we have reached this point in the code 
        ***** */
        break;
    }
  }
  return result ;

} /* rf_1x_mc_meas_enter */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is called to prepare the clean-up and start-up scripts.
 
  @param rfm_meas_setup_param : This is a pointer that basically has a header
  that has the target technology and the source technology. Based on this 
  header, the rest is casted as a tech specific pointer.
 
  @pre
  rfm_meas_common_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function
  did work correctly or not.
*/
rfm_meas_result_type 
rf_1x_mc_meas_build_reset_scripts /* rfmeas_build_reset_script_fp */
( 
  rfm_meas_setup_param_type *rfm_meas_setup_param,
  boolean is_source
)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS ;
   
  rfm_device_enum_type device;
  lm_handle_type rxlm_handle;
  rfm_device_enum_type div_device;
  lm_handle_type div_rxlm_handle;
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/
  rfm_cdma_band_class_type band;
  boolean status = TRUE;
  rf_buffer_intf * clean_up_script = NULL;

  RF_MSG ( RF_MED , "rf_1x_mc_meas_build_reset_scripts: Start " ) ;
  
  if (rfm_meas_setup_param == NULL || rfm_meas_setup_param->setup_params == NULL)
  {
  RF_MSG ( RF_ERROR , "rf_1x_mc_meas_build_reset_scripts: NULL Param Pointer " );
  return (RFM_MEAS_COMMON_FAILURE);
  }

  if((rfm_meas_setup_param->setup_params->header.source_tech != RFM_1X_MODE)&&
          (rfm_meas_setup_param->setup_params->header.target_tech == RFM_1X_MODE))
  {
    rfm_wait_time_t ret_val;

    /* Clear 1x global meas buffer before building any script */         
    status &= rf_buffer_clear(rf_1x_meas_mdsp_buffer_get(RFM_MEAS_PROGRAM_CLEANUP));

    if ( status != TRUE )
    {
      RF_MSG(RF_ERROR, "rf_1x_mc_meas_build_reset_scripts: Buffer Clean up failed");
    }

    /* 1x is target tech, get device from target tech param */
    device = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[0].device;
    rxlm_handle = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[0].lm_handle;

    div_device = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[1].device; 
    div_rxlm_handle = rfm_meas_setup_param->setup_params->header.target_param_v2[0].rx_dev_handle[1].lm_handle;

    /* Get PRX Logical Device Param, Device Status Read Pointer, and tuner token  */
    dev_status_r = rf_cdma_get_device_status( device );
    logical_dev = rfc_cdma_get_logical_device_params( device );
    ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

    /* Perform NULL Pointer check */
    if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) || 
         ( ant_tuner_token_w == NULL ) )
    {
      RF_MSG_1( RF_ERROR, "rf_1x_mc_meas_build_reset_scripts: "
                          "NULL data for device %d", device );
      return RFM_MEAS_COMMON_FAILURE;
    }
    /* check for valid band */
    band = dev_status_r->curr_band;

    /* Construct DRX Reset Script if Div device is Valid */
    if ( div_device != RFM_INVALID_DEVICE )
    {
      const rf_cdma_data_status_type* div_device_status;
      const rfc_cdma_logical_device_params_type* div_logical_dev;
      ant_tuner_device_tokens_type* div_ant_tuner_token_w; 

      div_device_status = rf_cdma_get_device_status( div_device );
      div_logical_dev = rfc_cdma_get_logical_device_params( div_device );
      div_ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( div_device );

      /* Perform NULL Pointer check */
      if ( ( div_device_status == NULL ) || ( div_logical_dev == NULL ) || 
           ( div_ant_tuner_token_w == NULL ) )
      {
        RF_MSG_1( RF_ERROR, "rf_1x_mc_meas_build_reset_scripts: "
                            "NULL data for Div device %d", div_device );
        return RFM_MEAS_COMMON_FAILURE;
      }

      /* Enter Device Critical Section, so that the device call is thread safe*/
      rfdevice_cdma_rx_enter_critical_section( div_logical_dev->rx_device[band] );

      status &= rf_cdma_mc_configure_front_end_for_rx_sleep ( 
                             div_device, RFM_1X_MODE, 
                             div_rxlm_handle, TRUE/* is_slave_device*/,
                             band,
                             div_device_status->script_index, 
                             div_device_status->rx_bw_khz, 
                             div_device_status->tx_bw_khz, 
                             div_logical_dev,
                             div_ant_tuner_token_w,
                             FALSE /* immediate_write */ ,
                             TRUE /* is_irat_mode */ );

      /* Leave Device Critical Section */
      rfdevice_cdma_rx_leave_critical_section(div_logical_dev->rx_device[band]);
    }/* if ( div_device != RFM_INVALID_DEVICE ) */

    /* Construct PRX Reset Script */
    if ( device != RFM_INVALID_DEVICE )
    {
      /* Enter Device Critical Section, so that the device call is thread safe */
      rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[band] );

      /* Call configure script in iRAT mode will populate reset script to 
      CLEANUP buffer. This is needed now since it's possible for 1x to do QTA
      on a seperate RF path/VCO than Home Tech. Thus leaving the path on may
      affect the performance */
      status &= rf_cdma_mc_configure_front_end_for_rx_sleep ( 
                             device, RFM_1X_MODE, 
                             rxlm_handle, FALSE /* is_slave_device */, 
                             band,
                             dev_status_r->script_index,
                             dev_status_r->rx_bw_khz, 
                             dev_status_r->tx_bw_khz, 
                             logical_dev,
                             ant_tuner_token_w,
                             FALSE /* immediate_write */,
                             TRUE /* is_irat_mode */ );

      /* Leave Device Critical Section */
      rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[band] );

      /* Configure clean up script to SLEEP_RX buffer. 1x FW will send it out
      at the end of QTA/iRAT gap */
      clean_up_script = rf_1x_meas_mdsp_buffer_get(RFM_MEAS_PROGRAM_CLEANUP);

      status &= rf_cdma_mc_configure_script_helper( device, 
                                                   RFM_1X_MODE,
                                                   rxlm_handle,
                                                   dev_status_r->script_index,
                                                   clean_up_script,
                                                   RF_CDMA_STATIC_EVENT,
                                                   RFLM_CDMA_MDSP_CCS_EVENT_SLEEP_RX_ID );
    }
  }
  else
  {
    RF_MSG( RF_ERROR, "rf_1x_mc_meas_build_reset_scripts: Unsupported tech combo" );
  }
  
  if (status == TRUE)
  {
    return (result);
  }
  else
  {
    RF_MSG ( RF_ERROR , "rf_1x_mc_meas_build_reset_scripts: status is FALSE " );
    return (RFM_MEAS_COMMON_FAILURE);
  }
} /* rf_1x_mc_meas_build_reset_scripts */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is called to prepare the clean-up and start-up scripts.
 
  @param rfm_meas_setup_param : This is a pointer that basically has a header
  that has the target technology and the source technology. Based on this 
  header, the rest is casted as a tech specific pointer.
 
  @pre
  rfm_meas_common_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function
  did work correctly or not.
*/
rfm_meas_result_type 
rf_1x_mc_meas_build_scripts
( 
  rfm_meas_setup_param_type *rfm_meas_setup_param,
  boolean is_source
)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS ;

  RF_MSG ( RF_MED , "rf_1x_mc_meas_build_scripts: Start " ) ;
  if (rfm_meas_setup_param == NULL || rfm_meas_setup_param->setup_params == NULL )
  {
    RF_MSG ( RF_ERROR , "rf_1x_mc_meas_build_scripts(): "
             "NULL pointer in rf_1x_mc_meas_build_scripts");
    result = RFM_MEAS_COMMON_FAILURE ;
  }
  else if (  rfm_meas_setup_param->setup_params->script_type == RFM_MEAS_PROGRAM_NULL2X )
  {
    result = rf_1x_meas_configure_for_tune_away ( rfm_meas_setup_param,
                                                  &(rfm_meas_setup_param->setup_params->target_param.onex_params) ) ;
    if ( FALSE == result) 
    {
      RF_MSG ( RF_ERROR , "rf_1x_mc_meas_build_scripts(): "
                          "rf_1x_meas_script_build_scripts returned FALSE" ) ;
    }
  }
  else 
  { /* Source Tech is 1X ; Target Tech is header.target_tech */
    switch (rfm_meas_setup_param->setup_params->header.target_tech)
    {
      /* 1x->LTE */
      case RFCOM_LTE_MODE:
        RF_MSG ( RF_MED , "rf_1x_mc_meas_build_scripts(): "
                          "ST -> 1X | TT -> LTE.. Nothing to be done here" ) ;
        break;
      /* 1x->T */
      case RFCOM_TDSCDMA_MODE:
        RF_MSG ( RF_MED , "rf_1x_mc_meas_build_scripts(): "
                          "ST -> 1X | TT -> T.. Nothing to be done here" ) ;
        break;

      /* 1x -> GSM*/
      case RFCOM_GSM_MODE:
      /* 1x -> W*/
      case RFCOM_WCDMA_MODE:

        result = rf_1x_meas_configure_for_tune_back (
           rfm_meas_setup_param,
           &(rfm_meas_setup_param->setup_params->source_param.onex_params),
           RFDEVICE_MEAS_CLEANUP_SCRIPT) ;
        if ( FALSE == result) 
        {
          RF_MSG ( RF_ERROR , "rf_1x_mc_meas_build_scripts(): " 
                   "rf_1x_meas_configure_for_tune_back Failed" ) ;
        }
        break;

      default:
        break;
    }

    /* Target Tech is 1X ; Source Tech is header.source_tech */
    switch (rfm_meas_setup_param->setup_params->header.source_tech)
    {
      /* LTE --> 1x */
      case RFCOM_LTE_MODE:
      /* W->1x */
      case RFCOM_WCDMA_MODE:
      /* T->1x */
      case RFCOM_TDSCDMA_MODE:
        /*1x is target tech, so build startup script for away buffer*/ 
        result = rf_1x_meas_configure_for_tune_away ( 
           rfm_meas_setup_param,
           &(rfm_meas_setup_param->setup_params->target_param.onex_params) ) ;
        if ( FALSE == result) 
        {
          RF_MSG ( RF_ERROR , "rf_1x_mc_meas_build_scripts(): " 
                   "rf_1x_meas_script_build_scripts returned FALSE" ) ;
        }
        break ;
      default:
        break ;
    } /* switch (rfm_meas_setup_param->header.source_tech) */
  } /* else if (rfm_meas_setup_param == NULL) */
  return result ;
} /* rf_1x_mc_meas_build_scripts */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is called to exit the measurement mode. It is used to put the
  RF in the right state and do any clean-ups required.
 
  @param rfm_meas_setup_param : This is a pointer that basically has a header
  that has the target technology and the source technology. Based on this 
  header, the rest is casted as a tech specific pointer.
 
  @pre
  rfm_meas_common_script_enter function must have been called.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function
  did work correctly or not.
*/
rfm_meas_result_type 
rf_1x_mc_meas_exit
( 
  rfm_meas_exit_param_type *rfm_meas_exit_param,
  boolean is_source
)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS ;

  if ( rfm_meas_exit_param == NULL )
  {
    RF_MSG ( RF_ERROR , "rf_1x_mc_meas_exit(): "
             "NULL pointer in rf_1x_mc_meas_exit");
    result = RFM_MEAS_COMMON_FAILURE ;
  }
  else 
  { 
    if ( rfm_meas_exit_param->exit_params != NULL )
    {
      rfm_meas_header_type *meas_header_p = 
              &(rfm_meas_exit_param->exit_params->header);
      RF_MSG_2 ( RF_MED ,"rf_1x_mc_meas_exit(): Exit. SRC = %d, TGT = %d", 
                 meas_header_p->source_tech, meas_header_p->target_tech ) ;

      rfm_device_enum_type device = RFM_MAX_DEVICES;

      /* Source Tech is 1X  ; Target Tech = header.target_tech */
      switch (meas_header_p->target_tech)
      {
        /* 1x --> LTE */
        case RFCOM_LTE_MODE:
        /* 1x->T */
        case RFCOM_TDSCDMA_MODE:
        {   
          /* HDR is source tech, get device from source tech param */
          device = meas_header_p->source_param_v2[0].rx_dev_handle[0].device;

          RF_MSG_1( RF_LOW, "rf_1x_mc_meas_exit(): Updating state for device" 
                            " %d to RF_CDMA_STATE_SHUTDOWN_FOR_IRAT",device ) ;

          /* Set device state to SHUTDOWN_FOR_IRAT */
          rf_cdma_data_set_1x_rf_state ( device , RF_CDMA_STATE_SHUTDOWN_FOR_IRAT ) ;
          break;
        }

        /* 1x --> G */
        case RFCOM_GSM_MODE:
        /* 1x->W */
        case RFCOM_WCDMA_MODE:
        {
          /* 1x is source tech, get device from source tech param */
          device = 
            rfm_meas_exit_param->exit_params->header.source_param_v2[0].rx_dev_handle[0].device;
          /* Handle measurement_gap_aborted for 1x2G QTA case*/
          if ( TRUE == rfm_meas_exit_param->is_measurement_gap_aborted )
          { 
            RF_MSG_1 ( RF_MED , "rf_1x_mc_meas_exit():Abort happened Abort: %d " ,
                         rfm_meas_exit_param->is_measurement_gap_aborted) ;
          }
          else  
          {
            boolean ret_val;
            /* ASM/Tuner driver state machines are impacted when GSM comes
               in the gap, to restore the states to what they were prior to the
               gap, call this helper API. This resets the states for the tuner
               if the 1x was in RxTx state and restores the ASM/XSW back to
               1x mode (since it gets set to GSM in the gap) */
            ret_val = rf_cdma_mc_reset_device_states_after_meas_gap( device ,
                                                               RFM_1X_MODE);

            if ( ret_val == FALSE )
            {
              RF_MSG_1 ( RF_ERROR , "rf_1x_mc_meas_exit():failed to reset"
                                  " ASM/Tuner for device %d " , device) ;
              result = RFM_MEAS_COMMON_FAILURE;
            }
          }
          break;
        }
        default:
          break;
      }

      /* Target Tech is 1X ;  Source Tech header.source_tech */
      switch ( meas_header_p->source_tech )
      {
    #if defined (FEATURE_LTE) || defined (FEATURE_TDSCDMA) || defined(FEATURE_WCDMA)
        /* LTE to 1X */
        case RFCOM_LTE_MODE:
        /* W->1x */
        case RFCOM_WCDMA_MODE:
        /* T->1x */
        case RFCOM_TDSCDMA_MODE:
          {
            boolean skip_full_sleep = TRUE ;
            lm_handle_type lm_handle;
            const rf_cdma_data_status_type* dev_status_r;
            rfm_device_enum_type irat_tq_device = RFM_INVALID_DEVICE;
            rfm_device_enum_type div_device = RFM_INVALID_DEVICE;
            lm_handle_type div_rxlm_handle;

            /* 1x is target tech, get device from target tech param */
            device = meas_header_p->target_param_v2[0].rx_dev_handle[0].device;
            lm_handle = meas_header_p->target_param_v2[0].rx_dev_handle[0].lm_handle;

            /* Get Drx device & LM Handle*/
            div_device = meas_header_p->target_param_v2[0].rx_dev_handle[1].device;
            div_rxlm_handle = meas_header_p->target_param_v2[0].rx_dev_handle[1].lm_handle;

            RF_MSG_4 ( RF_HIGH  , "rf_1x_meas_exit: " 
                       "|| Dev: %d | rxlm: %d | Div Dev %d | Div RxLM %d ||" ,
                       device , lm_handle, div_device, div_rxlm_handle ) ;

            /* If DRX device is not invalid, call disable diversity to clean up
            Drx device */
            if ( div_device != RFM_INVALID_DEVICE  )
            {
              rfm_wait_time_t ret_val;

              /* Disable Diversity with IRAT mode == TRUE to populate clean up script */
              ret_val =  rf_1x_mc_disable_diversity( div_device, div_rxlm_handle, TRUE );
            
              /* check if MC API was successful */
              if ( ret_val < RFM_CDMA_HEALTHY_STATE)
              {
                RF_MSG_2(RF_ERROR, "rf_1x_meas_exit:"
                                   "error during disable_diversity: "
                                   "Div device %d Div Handle %d ", 
                                   div_device, div_rxlm_handle );
                result = RFM_MEAS_COMMON_FAILURE;
              }
              else
              {
                result = RFM_MEAS_COMMON_SUCCESS;
              }
            }

            dev_status_r = rf_cdma_get_device_status(device);

            if ( dev_status_r != NULL )
            {
              irat_tq_device = dev_status_r->irat_data.irat_tq_device;
            }
            else
            {
              RF_MSG_1( RF_ERROR, "rf_1x_mc_meas_exit(): NULL device status"
                                  "Ptr for device %d", device );
            }

            /* If we were never woken up, say in optimized handoff case, then we 
            should avoid doing full sleep */
            if ( rf_cdma_data_get_1x_rf_state ( device ) == RF_CDMA_STATE_RX ) 
            {
              skip_full_sleep = FALSE ;
            }

            RF_MSG_1( RF_LOW, "rf_1x_mc_meas_exit(): Updating state for device" 
                              " %d to RF_CDMA_STATE_SHUTDOWN_FOR_IRAT",device );

            /* Set device state to SHUTDOWN_FOR_IRAT */
            rf_cdma_data_set_1x_rf_state ( device , RF_CDMA_STATE_SHUTDOWN_FOR_IRAT ) ;

            /* Perform 1X Clean-up after L21X measurement */
            result =   rf_cdma_meas_cleanup_ccs_events( device ) ;

             /* We dont want to sleep if we were never woken up meaning if 
            wakeup_rx was not called we shouldnt be calling sleep_rx. This can 
            happen in cases like:
            A> abort 
            B> optimized handover 
            For events like A and B, where only meas_enter and meas_exit are called, 
            all other intermediate IRAT calls are skipped in such cases JUST clean RF state
            OTHERWISE
            Go on ahead with the RF sleep otherwise  */
            if ( ( TRUE == rfm_meas_exit_param->is_measurement_gap_aborted ) ||
                 ( TRUE == skip_full_sleep ) )
            { 
              rfm_cdma_band_class_type band;
              band = (rfm_cdma_band_class_type)meas_header_p->target_param_v2[0].band;

              /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
              if (rfcommon_nv_get_lpm_feature_enabled())
              {
                /* Power down WTR since gap was aborted */
                rf_cdma_meas_power_down_wtr_for_abort( device, band );
              }

              //Cleanup the TQP resources in case of a aborted GAP
              if ( TRUE == rfm_meas_exit_param->is_measurement_gap_aborted )
              {
                /* In QCTA, use Source tech device to handle TQ resource
                tq_device is usually the source tech primary device */

                rf_cdma_mdsp_modem_rx_deallocate_resources( irat_tq_device, 
                                                            RFM_1X_MODE, 
                                                            lm_handle );
              }
              /* In abort case reset SW state machine so we can do a 
              successful enterMode-wakeup or measurement*/
              rf_cdma_reset_sw_state ( device ) ;
              RF_MSG_2 ( RF_ERROR , "rf_1x_mc_meas_exit(): "  
                       "wakeup failure or abort happened. Skipping sleep: "
                       "|| Abort: %d | SkipFullSleep: %d ||" ,
                        rfm_meas_exit_param->is_measurement_gap_aborted ,
                        skip_full_sleep ) ;
            }
            else
            {
              /* Sleep with IRAT_SHUTDOWN == TRUE so we dont send Rx Stop again  */
              rf_1x_mc_sleep_rx ( device , lm_handle , TRUE, /* in IRAT mode */ 
                                  FALSE /* Not IRAT enter mode */); 
            }
          }
          break;
    #endif /*FEATURE_LTE|WCDMA|TDS, needs to be present for Targets that do not have LTE*/
        default:
          break;
      } /* Switch */
      rf_cdma_deinit_irat  ( device ) ;
    } /* (rfm_meas_exit_param->target_param) != NULL */
    else
    {
      RF_MSG( RF_ERROR, 
              "rf_1x_mc_meas_exit(): rfm_meas_exit_param->target_param is NULL." ) ;
      result = RFM_MEAS_COMMON_FAILURE;
    }
  } /* else if (rfm_meas_setup_param == NULL) */
  return result ;
} /* rf_1x_mc_meas_exit */

/*!
  @brief
  List of function pointers needed for IRAT
 
*/
static rfmeas_mc_func_tbl_type rf_1x_mc_meas_apis =             
{
  rf_1x_mc_irat_info_get , /* rfmeas_mc_irat_info_get_fp*/
  rf_1x_mc_meas_enter , /* rfmeas_mc_enter_fp */
  NULL,                  /* rfmeas_mc_pre_config_fp */
  rf_1x_mc_meas_build_reset_scripts , /* rfmeas_build_reset_script_fp */
  rf_1x_mc_meas_build_scripts , /* rfmeas_build_scripts_fp */
  rf_1x_mc_meas_exit  /* rfmeas_mc_exit_fp */
} ;

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function registers the IRAT APIs for CDMA/1X with meas module.
 
  @retval TRUE on success and FALSE on failure
*/
boolean 
rf_1x_mc_meas_init
(
  void 
)
{

  boolean res;

  res = rfmeas_mc_register( (rfcom_mode_enum_type) RFM_1X_MODE, 
                            &rf_1x_mc_meas_apis );

  /* create setup and & cleanup rf buffers */
  rf_1x_meas_setup_buffer = rf_buffer_create(RFLM_CCS_IRAT_MAX_NUM_OF_SBI,
                                              RFLM_CCS_IRAT_MAX_NUM_OF_RFFE,
                                              20);

  rf_1x_meas_cleanup_buffer = rf_buffer_create(RFLM_CCS_IRAT_MAX_NUM_OF_SBI,
                                                RFLM_CCS_IRAT_MAX_NUM_OF_RFFE_CLEANUP,
                                                20);

  if ( res == FALSE )
  {
    RF_MSG ( RF_ERROR , "rf_1x_mc_meas_init has returned FALSE" ) ;
  }

  return res;

} /* rf_1x_mc_meas_init */

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function returns the global script buffer used by 1x in meas call flow.
    Buffers should be cleared before each use.
 
  @retval TRUE on success and FALSE on failure
*/
rf_buffer_intf *
rf_1x_meas_mdsp_buffer_get
(
   rfm_meas_program_script_type script_type
)
{
  rf_buffer_intf *rf_buffer = NULL;
  switch(script_type)
  {
     case RFM_MEAS_PROGRAM_STARTUP:
       rf_buffer = rf_1x_meas_setup_buffer;
       break;
     case RFM_MEAS_PROGRAM_CLEANUP:
       rf_buffer = rf_1x_meas_cleanup_buffer;
       break;
     default:
       RF_MSG_1(RF_ERROR,"rf_1x_meas_mdsp_buffer_get: Unsupported meas script type %d", script_type);
       break;
  }

  return (rf_buffer);
}

#endif /* FEATURE_CDMA1X */
/*! @} */

/*! @} */
