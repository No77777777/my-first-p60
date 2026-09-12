/*!
   @file rfgsm_core_tx.c

   @brief

   @details

*/

/*===========================================================================
Copyright (c) 2008 - 2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/rf/core/src/rfgsm_core_tx.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/16/20   ab      dummy check-in for CSDS promotion
07/16/20   mpa     To bind the VSWR code for wtr 3k only 
12/30/19   mpa     Recovery mechanism for RF scripting delays in X2Y IRAT scenarios on HA
11/07/19   mpa     Changes to set QET5102 papm to standby in wakeup
05/22/18   mpa     [CR2066053] GSM/EDGE Modulation Type Issue on PA driver
08/06/16   hkm     VSWR feature applicablity on MSM8953 and MSM8940
12/09/15   hkm     NV support to enable/disable LPM feature
11/10/15   tws     Add FBRX VSWR support
11/03/15   sas     Fix for klkwrk issue
11/03/15   sas     Fix for therm read error
10/07/15   sc      Tx burst execution time optimisations
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
09/02/15   ec      Update IP2 cal alpha based on Tx and Rx timeline
08/18/15   ggs     Missing functionality from Split Rx/Tx merge: 
                   Pass back Tx power in  dBm100 to GL1 in Tx confirmation
08/05/15   sc      Deprecate alternate path
07/29/15   rc      Add power logging support
07/28/15   ec      Update ip2_alpha in Tx timing update
07/27/15   sw      FR25072: for device wakeup, vote for lowest/first supported band
07/15/15   sw      WTR power saving optimisations (under feature flag)
06/10/15   tws     Fix incorrect AFC being applied in GBTA cases
05/28/15   sc      Enable thermistor and vbatt reads
04/21/15   sg      Passing band info while generating GSM Tx/Rx SBI scripts to 
                   support band split across 2 WTRs
04/20/15   sc      Populate RF sequence number passed from L1 in RFLM DM 
04/17/15   ggs     Pass back Tx power in  dBm100 to GL1 in Tx confirmation
04/10/15   zbz     Interface changes with device driver for split Rx/Tx
04/07/15   sc      Move core handles out of RFC layer for split Rx/Tx
04/06/15   hoh     NV changes to support Rx/Tx device split feature
03/31/15   sc      Add Tx wakeup event build and RFLM index APIs
03/30/15   sc      Add Tx timing update functionality
03/26/15   sc      Add legacy functionality to new Rx/Tx Wakeup/Sleep call-flow
03/25/15   sc      Add core Tx Wakeup/Sleep APIs
03/13/15   ec      Fix for Offtarget compiler error
03/12/15   ec      Print debug message in Tx Enable when Therm Read has been disabled
03/02/15   piy     Added Time Profiling for Wakeup, Sleep, Rx & Tx burst via debug NV
02/27/15   sc      Ensure NULL NV error checking when processing Rx/Tx band
12/02/14   sml     Read WTR or RF front end devices status in between Rx/Tx 
                   burst along with GRFC status
11/20/14   hoh     Apply temp comp to originally calculated frequency compensated AMAM/AMPM table
11/19/14   sc      Read and calculate delay match DM1-3 in RFLM GSM TxFE config
11/18/14   hoh     Set amam_ampm_recalc_required flag before ping-pong buffer switch
10/17/14   rr      Separate rx/tx alternate path core handles
10/07/14   sc      Program all delay match settings in RFLM DRIF
09/23/14   zbz     changes to support GSM and LTE/TDS coexistence related RFC changes
09/22/14   sc      Update band specific Tx timing adjust data when Tx band change
08/28/14   sc      Clean up Tx data initialisation to reduce wakeup timeline
08/11/14   shb     Fixed offtarget compiler error
08/11/14   cj      Added debug prints for Tx
08/11/14   sc      Ensure chain update with alternate path
07/24/14   tsr     DR-DSDS: Add support for device driver to handle worst case rx alpha for IDLE mode RX
07/01/14   jr      Enable therm read on sub-2
06/24/14   av      Compiler warning fix
06/18/14   tsr     RXD diversity call flow changes to enable DRX path along with PRX path
06/02/14    av     Fix memory corruption/writing to out of bounds.
05/20/14   zbz     Fix OFT error
06/05/14   ggs     AMAM/AMPM tables should only be recalculated 
                   for GSM when modulation type is 8PSK
04/29/14   sc      Scale the Tx PA ramps based on subscription
04/29/14   tsr     Mainline GSM RFC config v2 apis 
04/25/14   zbz     Up sample pa ramp table from NV using spline interpolation algorithm
04/16/14   sb      IP2 cal Interface and functionality changes
03/06/14   tsr     Program device scripts dynamically through CCS
03/06/14   sc      Correct the PA ramp up table for accurate PA ramp adjust
03/06/14   ggs     Ensure that rfgsm_core_init_master_pa_ramp_tables does not return
                   FALSE if the ramp tables are initialised for one of the supported 
                   bands.
03/04/14   tsr     Separate RX burst scripts and TX burst scripts retrieval from device driver
03/04/14   tsr     Move TX specific initialisation into Tx enter mode
02/28/14   sc      Pass core data through mc to core layers
02/25/14   tsr     Deprecate Feature D3925
02/18/14   tsr     GSM RX Diversity 
02/17/14   sc      Clean up mDSP APIs
01/30/14   sc      Manage data table independent of device
01/21/14   tsr     WTR TXAGC support for Multislot TX
01/21/14   ggs     Initial changes for TXC DCOC and DPD calibration
01/15/14   sc      Remove unused APIs using FW shared memory
01/14/14   av      Delay Calibration and npler based delay support
12/10/13   sc      Add set envelope gain API functionality
11/01/13   tsr     Append APT script to TASKQA(Non-atomic event) TX start script
10/31/13   sc      Feature-guard FW shared mem buffer population
10/28/13   ggs     Downsample base AMAM temp comp LUT by a factor of 4 
10/28/13   ggs     Add function to switch amam/ampm buffer index
10/28/13   ggs     Copy AMAM and AMPM tables to DM memory
10/25/13   tsr     Fix for appending APT Script to TX Non-sbi start script
10/23/13   tsr     Mainline D3295 changes for BOLT
10/21/13   sc      Modify dynamic event building for scenario-specific wrappers
10/21/13   sc      Set the PA ramp tables to RFLM GSM DM
10/14/13   tsr     Temporarily append new APT script to TX start script
10/14/13   sc      Mainline core DM and RFLM code
10/10/13   sc      Use RFLM band information to initialise PA ramp tables
10/09/13   tsr     Dynamic RF CCS event changes for WTR3925
10/08/13   sc      Correct RFLM table population PA ramp band loop
10/01/13   tsr     Added support retrieve to individual device scripts
10/03/13   sb      Add changes to receive MSGR cmd from MCS to switch to filtered/unfiltered 
                   path for SGLTE coexistence (GSM DCS +LTE B39/TDS B39/B34)
10/03/13   sc      RFLM DM interface data and dynamic event development
10/03/13   sg      Enabling GSM channel 1 phase data for QRD SGLTE card
10/01/13   sb      Disable SGLTE Coexistence filtered path cal data
09/27/13   sc      Modify RFLM GSM interface data programming functionality
09/25/13   sb      Added changes to support GSM TX calibration/online mode operation in coexistence
09/17/13   sb      Support for SGLTE/SGTDS coexistence
09/12/13   sc      RFLM interface encompassed by core DM
09/09/13   kab     Add temporary featurized changes to enable GSM phase data on chain 1 for 1625 SGLTE RF card
09/04/13   sb      Add temporary featurized changes to enable GSM phase data on chain 1 for SGLTE DSDA RF card
08/19/13   sc      Rename default num PA ramp entries
08/16/13   ndb     Clean-up the SPI based antenna tuner support
08/13/13   sc      Rename definitions ready for RFLM GSM merging
08/08/13   sc      Modify the ramp scaling to scale from a stored master table
08/08/13   sc      Add PA ramp initialisation storing NV tables in core DM
08/05/13   sc      Add PA ramp data management functions (featurised)
07/23/13   sc      Update RFLM data information (featurised)
07/22/13   ec	   Use new F3s/Null pointer checks
07/16/13   sc      Move mc/core functionality out of dispatcher and into mc/core layer
06/03/13   sb      Included right header file to enable ftm gsm logging
05/03/13   sb      Fixes for vbatt read taking too long
04/09/13   tsr     Added RF APPS->RF Task MSGR framework for temp comp to make temp comp data thread safe
04/09/13   sr      removed legacy antenna tuner code which is not used.
03/28/13   aka     GSM frame sweep feature changes.
02/07/13   tsr     Ensure that Temp comp calculation is thread safe
02/05/13   sb      Created separate APIs to configure RF front end devices based on whether
                   they are called every TX/RX burst or only when the band is changed.
01/22/12   sb      Changes to enable QPOET transactions to go out every gsm frame
01/09/13   sr      Made changes to run vbatt/temp comp in rf app task context.
01/3/13    av      Changes to support chain2 for GSM
12/19/12   sc      Remove Tx band change check as L1 monitor Tx band now
12/10/12   tws     Use core ftm_path delay value.
12/05/12   aka     changes to make path delay rfm device dependent
12/03/12   aka     Changes to use NV ptr from rfgsm_core_handle_ptr
11/27/12   tws     Add device id & as_id to FTM.
11/26/12   aka     Compiler warning fixes
11/26/12   aka     Changes to use a path specific gsm handle
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
11/19/12   sc      RF-Only mDSP update shared variables cleanup
11/16/12   aka     Added RFDEVICE_GSM_SET_FREQ_ERROR to set freq err in WTR driver
11/15/12   ec      Using new GL1 test api for Triton compatibility
11/07/12   sb      Made changes to correctly apply path delay
10/18/12   sb      Made changes to include ssbis for PA/ASM devices in TX burst script (SVVZ bringup)
10/11/12   sc      Cleanup up unrequired Tx burst & SBI/Non-SBI API burst buffer params
10/01/12   sb      Made changes to add rfm device to rf device interface and uppler layer APIs 
09/25/12   tc      Mainline RF_HAS_RFA_GSM_TASK.  
09/24/12   sb      Made changes to update TXLM,RXLM modem chain
09/18/12   tc      Remove FEATURE_RF_HAS_QFE1320-related functionality.  
09/06/12   ggs     Removed ftm_tx_cal_set_band. Set Tx band should always 
                   be called from GL1 and not from ftm directly 
08/09/12   ggs     Added ftm_tx_cal_set_band and rearranged call flow for rfgsm_set_tx_band
08/07/12   sc      Using seperate Rx and Tx scripts for CCS buffer tracability
08/06/12   sc      Move rf_buffer_clear for scripts to the dispatcher APIs
08/02/12   sc      Add num slots to setup Tx burst API
07/26/12   sr      Changes to APIS to take rfm_dev as parameter.
07/20/12   sr      Made changes to pass the RFM_DEVICE and rf_buffer to rfc_gsm
07/12/12   sc      Cast to rf_buffer_intf type for device calls
07/06/12   sc      Edit API parameters to support CCS buffer format
07/02/12   sr      renamed rfc_common_gsmctl.h to rfc_common_tdd.h as it is shared with TDS.
06/27/12   sc      Passing correct pointer to device for tx band CCS script populating
06/21/12   sc      Enable shared memory support
06/20/12   shb     Updated GSM device APIs to accept script buffer
07/02/12   sn      PA/ASM device interface and scripting support
06/21/12   vb/rsr  Added support for QFE1510 
04/09/12   sb      Add changes to fix frequency error adjustment in FTM mode
02/28/12   dej     Hooked up PA start/stop delta adjustments in FTM mode. 
                   ( Added call to reconstruct grfc scripts in setup_tx_burst if tx_timing_adjust flag is set ) 
02/10/12   sb      Changes to support FTM logging for GSM
01/20/12   sn      Updated call flow for QFE1320, calling rfgsm_mdsp_set_qpa_tx_params
12/20/11   sn      Added support for QFE1320
01/10/12   sr      changes to enable vbatt comp.
10/28/11    sr     made changes use the tx_nv_tbl ptr for any tx NV items
                   and rx_nv_tbl for any rx NV items.
10/25/11   sr      Made changes to configure rfc signals to tx enable/disable mode.
10/19/11   sb      Apply therm read in idle frame and enable FTM temp comp and vbatt comp during idle frame
10/13/11   lcl     Added antenna tuner support for TX.
13/11/11    sr     made changes add F3 msgs via DPC task. 
                   and made changes to remove rfc calls from enable/disable tx.
10/11/11    sr     added rfc tx disable/enable calls to configure RFC signals 
                   for tx disable/enable state.
09/20/11    sr     made changes to update the tx_band variable correctly.
09/19/11   av      set_tx_band needs to be called by L1 before tuning for TX. 
                   Removing the assumption that set_rx_band is called and updating
                   from rx_band.
09/15/11    sr     added antenna tuner support code.
09/07/11   sb/rsr  Changes for Delay match setting to use new FW interface.
08/26/11   sr      made changes to call the rfm_set_tx_band() for cal.
08/06/11   sr      RFC Cleanup: removed un-used code.
07/11/11    sr     removed un-used code.
06/08/11    sr     Send the PA RANGE information fw with band change.
07/01/11   vrb     Replacing direct calls to rfgsm_ds with wrapper functions
03/23/11    sr     temp comp code cleanup.
03/21/11    sr     Added LM support.
03/10/11   sr      Moved Tx power related functions to new rfgsm_core_tx_power.c file
03/04/11    sr     featurization for nikel build
03/03/11    sr     merge from mdm9k branch
01/27/11   tsr     Enhanced Temp Comp Scheme for Predistorted PCL       
01/21/11    sc      Corrected VBATT compensation calculation arithmetic (+ to -)
                    CR258210 (Checked in before in gsmctl file on 12/08/10)
01/17/11   dej     Mainline RF_DEVICE_MERGE_INTERIM_FEATURE
01/12/11   lcl     Added hook to init TX indicator buffer
12/15/10   av      RFGSMCTL files cleanup
12/03/10   tsr     Changes to ensure that temp comp is not performed if in FTM mode       
10/18/10  av/lcl   Clean up common RFC support
10/19/10   lcl     Removed not useful F3 message
10/11/10   lcl     Changed MSG_LVL to MSG_LEGACY to avoid too many F3 messages
09/01/10   av      GSM RFC Rearch
08/19/10   lcl     Pick up temp comp from QSC6x95: 
                   Make sure we perform temp comp at the very first instance of set tx band 
07/27/10   lcl     Compiler warning fix.
06/15/10   rsr     Picked up Temp comp changes from QSC6x95...
                   [
                    04/13/10 jps  Adding support to add slope to AMAM table based on temp bin 
                    02/09/10 jps  Picked up LCU fixes. Fix to match TX slot number sent down 
                                  from GL1 
                    01/31/10 jps  Added check for modulation type change in set_tx_power API
                    01/18/10 sr   Setup msm and rfc for tx_band.
                    01/18/10 jps  Added support for path delay scaling based on NPLER / DIV mode  
                   ]
03/19/10   lcl     Pull in changes from QSC6295
03/18/10   lcl     Added declaration of rfgsm_core_scaled_path_delay
03/09/10   tsr     Defined and initialised rfgsm_mdsp_tx_pow_dbm
12/22/09   sr      changed the debug msg macro from legacy to new one.
12/22/09   sr/lp   Moved rfgsm_mdsp_set_delay_value from set band to tune tx to 
                   avoid crash DA Cal
12/06/09   sr      removed un-used code.
12/05/09   sr      removed un-used code. Moved the ramp table load function to
                   rfgsm_mdsp file.
12/02/09   sr      modified the use of rfgsm_card.tx_band.
10/21/09   sr      new device driver interface changes.
10/12/09   sr      Fixed warnings.
07/27/09   rsr     Cleanup
07/08/09   av      Added calls to rfc to configure the band data.
07/06/09   sr      changes to make the operating band to rfcom_gsm_band type
07/04/009  sr      changed the code to use tx_band instead of band.
06/26/009  sr      code cleanup.
06/06/009  sr      unused code cleanup.
04/02/009  sr      replaced the direct mdsp write with rfgsm_mdsp interface
03/27/009  sr      rfgsm_core_ctl name changed to rfc_card_gsmctl
03/18/09   sr      Changed the rfgsm_mdsp__() name to reflect new name
03/16/09   sr      removed the code which is not needed.
03/13/09   sr      code cleanup for SCMM build
10/30/08   sr      Initial revision.

============================================================================*/

#include "rfa_variation.h"
#include "rfcom.h"
#include "msg.h"
#include "ftm.h"
#include "ftm_common.h"
#include "rfcommon_fbrx_api.h"
#include "rfdevice_gsm_coupler_intf.h"
#include "rfdevice_gsm_intf.h"
#include "rfdevice_type_defs.h"
#include "rfgsm_core_types.h"
#include "rfgsm_nv_cmn.h"
#include "rfgsm_mdsp.h"
#include "rfgsm_msm.h"
#include "rfgsm_core_timing_consts.h"
#include "rfgsm_core_tx.h"
#include "rfc_card.h"
#include "rfgsm_core.h"
#include "rfc_common_tdd.h" 
#include "mdsp_intf.h"
#include "rfgsm_core_util.h"
#include "rfc_card_gsm.h"
#include "rf_hal_buffer.h"
#include "ftm_gsm_rfctl.h"
#include "geran_test_mode_api.h"
#include "rfgsm_core_temp_comp.h"
#include "rfgsm_core_vbatt_comp.h"
#include "rflm_api_fbrx_fw_hal.h"
#include "stringl.h"
#include "rfcommon_nv_mm.h"

#include "rflm_dtr_rx_struct_ag.h"
#include "rflm_dtr_tx_struct_ag.h"
#include "rflm_dtr_rx_typedef_ag.h"
#include "rflm_dtr_tx_typedef_ag.h"
#include "rflm_dtr_tx_dac_ctl.h"
#include "rflm_dtr_rx_hal_ag.h"

#include "rfgsm_data_manager.h"

#include "rfm_internal.h"
#include "rfgsm_mc.h"

#include "gl1_hw.h"
#ifdef FEATURE_RF_POWER_REPORTING
#include "rf_power_reporting.h"
#endif

/*===========================================================================*/
/*                                                                           */
/*                               MACROS                                      */
/*                                                                           */
/*===========================================================================*/

/* Memset uses a char value. Cannot use 0xFF because this is -1 which is valid Tx power.
So instead we use 0x7F which ensures that the Tx power is initialised to a large (invalid) value */

#define RFGSM_INVALID_TX_POWER 0x7F7F7F7F /* 2139062143dec */

#define RFGSM_INVALID_TX_POWER_BIT_SHIFT 24 /* RFGSM_INVALID_TX_POWER >> RFGSM_INVALID_TX_POWER_BIT_SHIFT = 0x7F*/

/*===========================================================================*/
/*                                                                           */
/*                               PROTOTYPES                                  */
/*                                                                           */
/*===========================================================================*/
void rfgsm_core_compute_linear_freq_comp (rfm_device_enum_type rfm_dev, rfcom_gsm_band_type band, uint16 arfcn);
void rfgsm_core_compute_pcl_vs_power (rfm_device_enum_type rfm_dev, rfcom_gsm_band_type band, uint16 arfcn);
void rfgsm_core_log_send_to_ftm_task(rfm_device_enum_type rfm_dev);
boolean rfgsm_core_config_fbrx_rflm( rfgsm_core_data_type * core_data, rfm_device_enum_type rfm_dev );


/*===========================================================================*/
/*                                                                           */
/*                                FUNCTIONS                                  */
/*                                                                           */
/*===========================================================================*/
/*!
  @fn rfgsm_core_tx_wakeup
  @brief This function prepares and builds CCS event to program the RFC to
         GSM mode
  @details This is called for each device that requires an Tx wakeup
  @param sub_id - subscription that is performing the Tx wakeup
  @param tx_device - RFM logical device Tx wakeup is for
  @param txlm_buf_index - RFLM handle to populate with device based modem data
  @param timing_info - pointer to GL1 data to populate with Tx timing data
  @retval boolean
*/
boolean rfgsm_core_tx_wakeup( uint8 sub_id,
                              rfcom_device_enum_type tx_device,
                              uint32 txlm_buf_index )
{
  /* Initialise variables */
  uint32 i = 0;
  uint16 rf_dev_sub_mask = ( (uint16)tx_device | (uint16)(sub_id << 8) );
  rfgsm_core_data_type core_data;
  /* Time Profiling and Logging */
  unsigned long long start_pcycle = 0;
  uint32 start_ustmr = 0;

  core_data.rfm_dev = tx_device;
  core_data.sub_id = sub_id;
  core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(sub_id);
  core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_get(tx_device);

  RF_NULL_CHECK_RTN(core_data.rfgsm_core_handle_ptr, "rfgsm_core_tx_wakeup rfgsm_core_handle_ptr is NULL!", FALSE);
  
/*updating current active device status*/
  core_data.rfgsm_core_handle_ptr->active_tx_device = TRUE;
  
  /* Initialise the master PA ramp tables in DM from NV */
  if( !rfgsm_core_init_master_pa_ramp_tables( &core_data ) )
  {
    RF_MSG(MSG_LEGACY_ERROR, "RF GSM core DM init master PA ramp tables failed!");
    return FALSE;
  }

  for( i=0; i < RFGSM_DEVICE_MAX_TX_SLOTS; i++ )
  {
    /* Clear the script buffer before use */
    rf_buffer_clear_script((rf_buffer_intf *)core_data.rfgsm_core_handle_ptr->scripts.tx_agc_trigger_gmsk_script[i], 
                             RF_BUFFER_ALL);
    rf_buffer_clear_script((rf_buffer_intf *)core_data.rfgsm_core_handle_ptr->scripts.tx_agc_trigger_8psk_script[i], 
                             RF_BUFFER_ALL);
  }

  /* Clear the script buffer before use */
  /* @TODO temporarily reusing enter mode script */
  if( !rf_buffer_clear( (rf_buffer_intf*)core_data.rfgsm_core_handle_ptr->scripts.enter_mode_script ) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RF GSM enter mode script buffer was unsuccessfully cleared", 0);
    return FALSE;
  }

 //check RFNV_SPARE_1_I value to know LPM feature
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
   /* Profiling */
    start_pcycle = qurt_get_core_pcycles();
    start_ustmr = rfgsm_mc_read_ustmr_count();

    /* ********************************************************** */
    /* WTR was placed in LPM as part of rx_wakeup                 */
    /* Assumption is that rx_wakeup always comes before tx_wakeup */
    /* ********************************************************** */
    /* Power on TX device (LPM->ON) */
    if ( !rfdevice_gsm_tx_pwr_wakeup( tx_device, rfgsm_nv_get_lowest_supported_band(tx_device, RFGSM_NV_TX) ) )
    {
      ERR_FATAL( "rfgsm_core_tx_wakeup: TX Device %d Pwr Up (LPM->ON) failed for band %d",
                tx_device, rfgsm_nv_get_lowest_supported_band(tx_device, RFGSM_NV_TX), 0 ); 
      return FALSE;
    }

    /* Log timings to command buffer */
    RFGSM_DEVICE_PROC_LOG(RFGSM_CORE_TX_PWR_WAKEUP, sub_id, 0, 0, 0, 0, start_ustmr, start_pcycle, rfgsm_ustmr_count_diff_us_rtn(start_ustmr) );
  }

  /*TX TXAGC trigger retrieval from WTR for tx trans script*/
  rfdevice_gsm_get_trans_script( tx_device,
                                 (rf_buffer_intf**)core_data.rfgsm_core_handle_ptr->scripts.tx_agc_trigger_gmsk_script,
                                 (rf_buffer_intf**)core_data.rfgsm_core_handle_ptr->scripts.tx_agc_trigger_8psk_script);

  /* Ensure Tx AGC update is executed */
  rfgsm_core_set_slot_override_flag(core_data.rfgsm_core_handle_ptr, FALSE);

  /* Enable therm reads */
  if( rfgsm_debug_flags.disable_temp_comp == FALSE )
  {
    /* trigger thermistor read */
    rfgsm_core_therm_read_trigger( 0, rf_dev_sub_mask );

    /* Start PA Temp Tx compensation timer.*/
    rfgsm_core_temp_comp_start( tx_device, sub_id );
  }

  /* Enable vbatt reads */
  rfgsm_core_vbatt_read_trigger( 0, tx_device, FALSE );

  /* Start Vbatt Tx compensation timer. */
  rfgsm_core_vbatt_comp_start( tx_device );

  /* Set the TX stop priority for the HMSC case */
  rfgsm_core_dm_set_hmsc_tx_stop_priority( sub_id, rfgsm_mdsp_get_hmsc_tx_stop_priority() ); 

  /* Static settings update to DTR buffer for TxLM handle */
  rfgsm_msm_enable_tx( txlm_buf_index, tx_device, FALSE, RFGSM_ALT_PATH_NOT_SUPPORTED );

  if( core_data.rfgsm_core_handle_ptr->fbrx_vswr_enabled )
  {
    rfgsm_core_config_fbrx_rflm( &core_data, tx_device );
  }


  /* Store the TxLM handle in the core handle for later use */
  core_data.rfgsm_core_handle_ptr->txlm_buf_index = txlm_buf_index;

  /* Get the Tx Wakeup script */
  /* @TODO temporarily reusing enter mode script */
  if( !rfdevice_gsm_tx_enter_mode( tx_device,
                                   RFDEVICE_CREATE_SCRIPT,
                                   (rf_buffer_intf*)core_data.rfgsm_core_handle_ptr->scripts.enter_mode_script ) )
  {
    RF_MSG_2( MSG_LEGACY_ERROR, 
              "rfgsm_core_tx_wakeup() failed Tx script build for sub %d, dev %d!", 
              sub_id, tx_device );
    return FALSE;
  }

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_tx_wakeup_timing_update
  @brief This function populates the GSM timing data in RFC, RFLM and GL1
  @details This is called to update Tx timing data for GL1 scheduling
  @param sub_id - subscription that is performing the Rx sleep
  @param tx_device - RFM logical device
  @param gl1_timing_info - Pointer to GL1 timing data
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_tx_wakeup_timing_update( uint8 sub_id,
                                            rfcom_device_enum_type tx_device,
                                            rfgsm_timing_info_type * gl1_timing_info )
{
  uint16 tx_alpha = 0;
  rfgsm_mdsp_dpll_training_seq_type dpll_seq;
  uint32 num_gb = 0;
  uint32 num_pll_trans = 0; //absorbed in clock recovery sequence
  int32 tx_start_delta = 0;
  rfgsm_core_handle_type * rfgsm_core_handle_ptr = NULL;
  rfc_gsm_tx_timing_info_type * rfgsm_core_tx_timing = NULL;
  rfc_gsm_core_config_type * rfc_gsm_core_ptr = NULL;

  uint16 vswr_meas_duration_qs = 150; 
   /*! @todo get these from the WTR driver */
  uint16 fbrx_ldo_settle_time_qs = 20;
  
  /* Get the core handle and RFC data */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get( tx_device );
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_tx_wakeup_timing_update rfgsm_core_handle_ptr is NULL!", FALSE);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr->rfc_gsm_core_ptr, "rfgsm_core_tx_wakeup_timing_update rfc_gsm_core_ptr is NULL!", FALSE);

  /* Get the tx timing data */
  rfgsm_core_tx_timing = rfgsm_core_handle_ptr->rfc_gsm_core_ptr->tx_timing;
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr->rfc_gsm_core_ptr->tx_timing, "rfgsm_core_tx_wakeup_timing_update tx_timing is NULL!", FALSE);

  /* Get the pll clock recovery length and data */
  /* Clock recovery sequence also accounts for writes before PLL */
  rfdevice_gsm_cmd_dispatch( tx_device, RF_PATH_0, RFDEVICE_GSM_GET_TX_DPLL_TRAINING_SEQUENCE, (void*)(&dpll_seq));

  /* Saturate number of guard bits at RFGSM_MAX_GUARD_BITS */
  num_gb = dpll_seq.num_guard_bits > RFGSM_MAX_DPLL_GUARD_BITS ? RFGSM_MAX_DPLL_GUARD_BITS : dpll_seq.num_guard_bits;

  /* Calculate the Tx start delta */
  tx_start_delta = RFGSM_TX_SBI_START_DELTA_QS( num_gb, num_pll_trans );
  
  /* Set the Tx alpha */
  tx_alpha = (uint16)(-tx_start_delta);

  /* Update Tx RFFE start time to WTR */
  rfdevice_gsm_cmd_dispatch( tx_device, RF_PATH_0, RFDEVICE_GSM_UPDATE_TX_TIMING, (void*)(&tx_start_delta) );

  /* Update RFC copies of the TX timing info */
  rfgsm_core_tx_timing->kv_length = (uint16)num_gb;
  rfgsm_core_tx_timing->num_pll_sbis = (uint16)num_pll_trans;
  rfgsm_core_tx_timing->tx_sbi_start_delta_qs = (int)tx_start_delta;
  rfgsm_core_tx_timing->tx_alpha_qs = (int)tx_alpha;

  /* Program the KV data into RFLM DM */
  if( !rfgsm_core_dm_set_kv_data( sub_id, num_gb, dpll_seq.sequence ) )
  {
    RF_MSG_2(MSG_LEGACY_ERROR, "Tx KV data NOT updated for sub %d, device %d", sub_id, tx_device);
    return FALSE;
  }

  /* Write Tx alpha value to RFLM DM */
  if( !rfgsm_core_dm_set_tx_alpha( sub_id, tx_alpha ) )
  {
    RF_MSG_2(MSG_LEGACY_ERROR, "Tx timings are NOT updated for sub %d, device %d", sub_id, tx_device);
    return FALSE;
  }

  /*! @todo Get the number of RFFEs from the actual script */
  fbrx_ldo_settle_time_qs = fbrx_ldo_settle_time_qs + RFGSM_MB_RFFE_TIME_CALC_QS(3);

  rfgsm_core_dm_set_vswr_meas_timing( sub_id, 
                                      vswr_meas_duration_qs, 
                                      fbrx_ldo_settle_time_qs );

  /* Update GL1 Tx timing info */
  gl1_timing_info->tx_rf_setup_time_qs = tx_alpha;
  gl1_timing_info->tx_rf_setup_time_vswr_qs = tx_alpha + fbrx_ldo_settle_time_qs + vswr_meas_duration_qs; 


  RF_MSG_2( MSG_LEGACY_ERROR, "tx_alpha = %d, tx_alpha_vswr_qs = %d", 
            gl1_timing_info->tx_rf_setup_time_qs, 
            gl1_timing_info->tx_rf_setup_time_vswr_qs );

  /* Update GL1 timing with IP2 alpha (updated in rx_timing_calc call for preferred Rx device */
  gl1_timing_info->ip2_cal_setup_time_qs = rfgsm_core_handle_ptr->ip2_cal_alpha; 

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_tx_wakeup_build
  @brief This function builds the CCS events
  @details This is called once-only for Tx device
  @param sub_id - subscription that is performing the Rx sleep
  @param tx_device - RFM logical Tx device
  @param gfw_buff_ptr - Pointer to GFW shared memory
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_tx_wakeup_build( uint8 sub_id, 
                                    rfcom_device_enum_type tx_device, 
                                    void * gfw_buff_ptr )
{
  /* Initialise data */
  rfgsm_core_data_type core_data;
  core_data.rfm_dev = tx_device;
  core_data.sub_id = sub_id;
  core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id( sub_id );
  core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_get( tx_device );
  core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;

  /* Build the Tx wakeup events */
  if( !rfgsm_core_dm_dynamic_tx_wakeup_event_wrapper( &core_data ) )
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Tx Wakeup RFLM dynamic event prog failed!");
    return FALSE;
  }

  /* Populate the Tx wakeup RFLM data information */
  if( !rfgsm_core_dm_populate_tx_wakeup_intf_data( &core_data, gfw_buff_ptr ) )
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Tx Wakeup RFLM data info update failed!");
    return FALSE;
  }

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_tx_sleep
  @brief This function disables the RFC from Tx capable GSM mode
  @details This is called for each device that requires an Tx sleep
  @param sub_id - subscription that is performing the Tx sleep
  @param tx_device - RFM logical device Tx sleep is for
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_tx_sleep( uint8 sub_id, 
                             rfm_device_enum_type tx_device )
{
  rfgsm_core_handle_type * rfgsm_core_handle_ptr = NULL;
  /* Time Profiling and Logging */
  unsigned long long start_pcycle = 0;
  uint32 start_ustmr = 0;

  rfcommon_fbrx_error_type fbrx_status;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get( tx_device );
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_tx_sleep rfgsm_core_handle_ptr is NULL!", FALSE);

  /* Stop Vbatt Tx pwr compensation */
  rfgsm_core_vbatt_comp_stop( tx_device );

  /* Stop PA Temp pwr compensation */
  rfgsm_core_temp_comp_stop( tx_device );

  rfgsm_msm_disable_tx();

  rfc_gsm_tx_disable( tx_device, 
                      rfgsm_core_handle_ptr->tx_band, 
                      rfgsm_core_handle_ptr->rf_tx_arfcn );

  /* Stop VSWR measurements */
  if( rfgsm_core_handle_ptr->fbrx_vswr_enabled )
  {
    rfgsm_core_dm_set_vswr_meas_state( sub_id, RFLM_GSM_VSWR_MEAS_SLEEP );

    #if 0
    if( rfcommon_fbrx_mc_exit( rfgsm_core_handle_ptr->fbrx_buf_index, RFM_EGSM_MODE )
        != LM_SUCCESS )
    {
      RF_MSG(RF_ERROR, "FBRx buffer deallocation failed");
    }
    #endif
    if(!rfcommon_fbrx_msm_deallocate_rxlm_buffer(rfgsm_core_handle_ptr->fbrx_buf_index))
    {
      ERR_FATAL("FBRx buffer deallocation failed",0,0,0);
    }

    rfgsm_core_handle_ptr->fbrx_buf_index = RFGSM_RXLM_BUF_IDX_INVALID;

    rfgsm_core_dm_set_fbrx_buf_index( sub_id, rfgsm_core_handle_ptr->fbrx_buf_index );
  }

 //check RFNV_SPARE_1_I value to know LPM feature
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
   /* Profiling */
   start_pcycle = qurt_get_core_pcycles();
   start_ustmr = rfgsm_mc_read_ustmr_count();

    /* Power off TX device (ON->LPM) */
   if ( !rfdevice_gsm_tx_pwr_sleep( tx_device, rfgsm_nv_get_lowest_supported_band(tx_device, RFGSM_NV_TX) ) )
   {
     ERR_FATAL( "rfgsm_core_tx_sleep: TX Device %d Pwr Down (ON->LPM) failed for band %d",
               tx_device, rfgsm_nv_get_lowest_supported_band(tx_device, RFGSM_NV_TX), 0 );
     return FALSE;
   }

    /* Log timings to command buffer */
    RFGSM_DEVICE_PROC_LOG(RFGSM_CORE_TX_PWR_SLEEP, sub_id, 0, 0, 0, 0, start_ustmr, start_pcycle, rfgsm_ustmr_count_diff_us_rtn(start_ustmr) );
  }

  /*updating current active device status*/
  rfgsm_core_handle_ptr->active_tx_device = FALSE;

  return TRUE;
}



/*----------------------------------------------------------------------------*/
/*!
  @details
  This function sets up the GRFC and SBI/uwire buffer required for the
  Tx burst.

  @retval
  tx_alpha - Tx alpha value for the burst set up
*/
void rfgsm_core_setup_tx_burst( rfgsm_core_data_type *core_data, 
                                  uint8 num_tx_slots, 
                                  void *tx_burst_event_ptr,
                                  uint32 rf_sequence_number )
{
  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_NONE;
  uint16 delay_backoff;
  int16 final_delay_val;

  uint32 rfgsm_time_profile[5] = {0,0,0,0,0};
  GET_START_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0]);

  core_data->rfgsm_core_handle_ptr->tx_log.freq_error = rfgsm_core_get_tx_freq_error(core_data->rfgsm_core_handle_ptr);
  core_data->rfgsm_core_handle_ptr->tx_log.fn = geran_test_mode_api_get_FN(as_id);
  core_data->rfgsm_core_handle_ptr->tx_log.temp_comp_index = rfgsm_core_temp_comp_bin_index_get(core_data->rfm_dev);

   /* Set PA start/stop time in rf scripts. */
  if(core_data->rfgsm_core_handle_ptr->tx_timing_adjust_flag == TRUE)
  {
    /*If requested, recalculate timing based on the current tx band*/
    rfcom_gsm_band_type rfgsm_tx_band = core_data->rfgsm_core_handle_ptr->tx_band;
    rfc_gsm_tx_timing_adjust( core_data->rfm_dev, 
                              rfgsm_tx_band, 
                              core_data->rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl, 
                              core_data->rfgsm_core_handle_ptr->rf_pa_start_time, 
                              core_data->rfgsm_core_handle_ptr->rf_pa_stop_time );

    /*Clear Flag*/
    core_data->rfgsm_core_handle_ptr->tx_timing_adjust_flag = FALSE;
  }

  /*Clear the TX script buffers first*/
  if(!rfgsm_core_clear_tx_burst_device_scripts(core_data->rfgsm_core_handle_ptr))
  {
    RF_MSG_1(RF_ERROR,"RF GSM WTR TX START/STOP script buffer was unsuccessfully cleared for rfm_dev|%d|",
                              core_data->rfm_dev);
  }
  /* Retrieve WTR device scripts for TX Burst */
  if( !rfgsm_core_populate_tx_burst_device_buffers(core_data, 
                                                   RF_TX_BURST) )
  {
    RF_MSG_1(RF_ERROR,"RF GSM Tx Burst WTR script buffers are not retrieved for rfm_dev|%d|",
                      core_data->rfm_dev);
  }

  GET_CURRENT_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[1]);
  
  /* Populate the Tx RFLM event data table */
  if( !rfgsm_core_dm_dynamic_tx_event_wrapper(core_data, 
                                              num_tx_slots) )
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Tx RFLM dynamic event prog failed!");
  }

  GET_CURRENT_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[2]);

  /* Populate the Tx burst RFLM data information */
  rfgsm_core_populate_burst_rflm_data_info( core_data, RF_TX_BURST, num_tx_slots, tx_burst_event_ptr,rf_sequence_number );

  /* Populate the Tx burst RF sequence number */
  if( !rfgsm_core_dm_set_tx_rf_sequence_num( core_data,
                                             rf_sequence_number ) )
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Tx RF Sequence number update failed!");
  }

  GET_CURRENT_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[3]);
  
  /* get path delay value for given rfm device*/
  rfdevice_gsm_cmd_dispatch(core_data->rfm_dev, RF_PATH_0, RFDEVICE_GSM_GET_DELAY_BACKOFF, (void*)&delay_backoff);

  rfgsm_nv_calc_path_delay( core_data->rfm_dev, 
                            delay_backoff, 
                            &final_delay_val );

  rfgsm_core_dm_set_tx_path_delay( core_data->sub_id,
                                   core_data->triple_buffer_id,
                                   (int64)final_delay_val );
  
  /* log the path delay value in Tx log packet*/
  core_data->rfgsm_core_handle_ptr->tx_log.delay_val = final_delay_val;

  #ifdef FTM_HAS_LOGGING
  rfgsm_core_log_send_to_ftm_task(core_data->rfm_dev);
  #endif
  
  GET_CURRENT_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[4]);
  if(core_data->rfgsm_core_handle_ptr->enable_time_profiling.tx_burst)				
  {
    RF_MSG_4(MSG_LEGACY_HIGH, "Tx Burst Time Profiling, populate_tx_burst_device_buffers: %d us, dynamic_tx_event_wrapper: %d us,"
             "populate_burst_rflm_data_info: %d us, rfgsm_core_setup_tx_burst_end_time: %d us",
             rfgsm_time_profile[1],
             (rfgsm_time_profile[2] - rfgsm_time_profile[1]),
             (rfgsm_time_profile[3] - rfgsm_time_profile[2]),
             rfgsm_time_profile[4]);
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @details
  This function returns Tx power in each slot in dBm100

  @retval
  boolean
*/

boolean rfgsm_core_update_tx_power_in_response_message( rfgsm_core_data_type *core_data, int32 *tx_pwr_in_dBm100 )
{
   uint32 i;

   /* First memset the Tx power returned to GL1 to invalid values. This is required to ensure that
      the max number of tx slots in a frame are set to invalid and not just the active num slots in 
      this frame */
   memset( &tx_pwr_in_dBm100[0], (RFGSM_INVALID_TX_POWER >> RFGSM_INVALID_TX_POWER_BIT_SHIFT), 
           RFGSM_MAX_TX_SLOTS_PER_FRAME*sizeof(int32) );

   if ( core_data->rfgsm_core_handle_ptr->last_num_tx_slots > RFGSM_MAX_TX_SLOTS_PER_FRAME )
   {
      return FALSE;
   }

   for ( i = 0; i < core_data->rfgsm_core_handle_ptr->last_num_tx_slots; i++ )
   {
      tx_pwr_in_dBm100[i] = core_data->rfgsm_core_handle_ptr->rfc_gsmctl_linear_pa_tx_gain_buff[i].txPower;
   }
   return TRUE;
}


/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_set_tx_band
  @details This function prepares scripts to set the Tx band for a specified
           band passed from GL1
  @param core_data - data structure including device ID and core handle
  @param band - band to configure the RF to passed from GL1
  @param shared_mem_ptr - FW memory location to populate the interface index
  @retval void
*/
void rfgsm_core_set_tx_band( rfgsm_core_data_type *core_data,
                             rfcom_gsm_band_type band, 
                             void *shared_mem_ptr )
{
  core_data->rfgsm_core_handle_ptr->tx_log.tx_band = band;
  core_data->rfgsm_core_handle_ptr->tx_log.log_flag = TRUE;

  /* setup tx nv table pointer */
  core_data->rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl = (rfgsm_nv_band_tx_data_type *)rfgsm_nv_get_tbl_ptr(core_data->rfm_dev, band, RFGSM_NV_TX);

  /* Verify the NVs are present for Tx on the device being used */
  if( (core_data->rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl == NULL) ||
      (core_data->rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl->tx_static_nv_data_ptr == NULL) )
  {
    /* Fatal error only if not in FTM mode */
    if( ftm_get_mode() != FTM_MODE )
    {
      ERR_FATAL("Set Tx band %d dev %d NV calibration data is bad OR not Tx capable", 
                band, core_data->rfm_dev, 0);
    }
    else
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
            "Set Tx band %d dev %d NV calibration data is bad OR not Tx capable", 
            band, core_data->rfm_dev);
      return;
    }
  }

  /* Reset the previous tx channel when changing band. */
  core_data->rfgsm_core_handle_ptr->rf_tx_arfcn = RF_INVALID_VALUE_16BIT;

  /* msm_set_tx_band() need to be called before the rfc_tx_band_config, because we need to switch
  the PM mux from tcxo to PA_ON before we turn on the pmic mux */
  rfgsm_msm_set_tx_band(band);

  if( core_data->rfgsm_core_handle_ptr->fbrx_vswr_enabled )
  {

    RF_MSG( MSG_LEGACY_ERROR, "ck-debug: Tx Band script to configure fbrx" );

    rfgsm_core_dm_set_vswr_meas_state( core_data->sub_id, RFLM_GSM_VSWR_MEAS_IDLE );
  }
  /* Set RFC configuration for Tx capability */
	 rfc_gsm_tx_enable( core_data->rfm_dev,band );

  /* Clear the TX band script buffer before use */
  if(!rf_buffer_clear((rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_band_script))
  {
    RF_MSG( MSG_LEGACY_ERROR, "RF GSM card Tx Band script buffer was unsuccessfully cleared" );
  }
 

  /* explicitly typecasting the rfcom_gsm_band_type to rf_card_band_type to
     fix  warnings. When the band parameter is used in rfc_<target>_gsm.c file
     it will be used as rfcom_gsm_band_type. */
  rfc_gsm_tx_band_config(core_data->rfm_dev, 
                         band, 
                         (rf_buffer_intf *)core_data->rfgsm_core_handle_ptr->scripts.tx_band_script); 

  /* Update the RFLM DM with the pa start and stop offsets */
  if( !rfgsm_core_dm_set_ramp_deltas( core_data->sub_id,
                                      core_data->rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl->tx_static_nv_data_ptr->tx_timing_data.pa_start_offset_adj,
                                      core_data->rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl->tx_static_nv_data_ptr->tx_timing_data.pa_stop_offset_adj ) )
  {
    RF_MSG(MSG_LEGACY_ERROR, "Tx ramp deltas are NOT updated");
    return;
  }

  /* Even at band change ensure that the mDSP GL1 vars updated for burst offset */
  rfgsm_mdsp_set_tx_band(core_data);

  /* Setup band dependent settings */
  /* For FTM TX calibration, WTR device need to perform set tx band sbi writes via SW CCS cmd interface instead
     of via FW CCS interface */
  rfdevice_gsm_set_tx_band( core_data->rfm_dev, band, RFDEVICE_CREATE_SCRIPT,(rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_band_script);

  /* Populate the Tx band RFLM event data table */
  if( !rfgsm_core_dm_dynamic_tx_band_event_wrapper(core_data) )
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Tx band RFLM dynamic event prog failed!");
  }

  /* Populate the Tx band RFLM data information */
  if( !rfgsm_core_dm_populate_tx_band_intf_data( core_data, (void*)shared_mem_ptr ) )
  {
    MSG(MSG_SSID_RF, MSG_LEGACY_ERROR,"RF GSM Tx band RFLM intf data update failed!");
  }

  /* Calculate the vbatt compensation value based on new band. */ 
  rfgsm_core_vbatt_comp_trigger(core_data->rfm_dev);

  /* Update tx profile due to tx band change */
  rfgsm_core_set_tx_profile_update_flag(core_data->rfgsm_core_handle_ptr);

  core_data->rfgsm_core_handle_ptr->tx_band = band;   
} /* end of rfgsm_set_tx_band */



/*----------------------------------------------------------------------------*/
/*!
  @details
  This function sets up the Tx scripts to program the synthesizer to the
  requested channel.

  rfgsm_set_band must be called before this function or the 
  FEATURE_SET_BAND_IN_GL1 must be turned off so it may be called in this 
  function before proceseding

  @retval
  TRUE/FALSE - sucessful/unsuccessful tune.
*/

boolean rfgsm_core_tune_tx(rfgsm_core_data_type *core_data, uint16 arfcn, rfgsm_modulation_type mod_type)
{
  rfcom_gsm_band_type tx_band = core_data->rfgsm_core_handle_ptr->tx_band;

  /*This API retrieves RF configuration for all FE devices including GRFCs,ASM,PA,ATUNER etc. for a give TX burst*/
  rfc_gsm_tx_burst_config( core_data->rfm_dev,
                           tx_band, 
                           arfcn,
                           core_data->rfgsm_core_handle_ptr->txlm_buf_index,
                           core_data->rfgsm_core_handle_ptr->rfgsm_tx_nv_tbl,
                           &core_data->rfgsm_core_handle_ptr->scripts,
                           mod_type);

  /* Tell the library to tune. */
  rfdevice_gsm_tune_tx( core_data->rfm_dev, arfcn );

  if ( (core_data->rfgsm_core_handle_ptr->rf_tx_arfcn != arfcn) || 
       (core_data->rfgsm_core_handle_ptr->temp_comp.temp_comp_updated == TRUE) )
  {
    /* Set the amam_ampm recalc required flag so it will be done during next tx_power update */
    core_data->rfgsm_core_handle_ptr->amam_ampm_recalc_required = TRUE;    

    if( !rfgsm_core_tx_amam_ampm_data_buffer_switch( core_data->rfm_dev ) )
    {
      RF_MSG(MSG_LEGACY_ERROR, "RF GSM AMAM AMPM table ping-pong failed!");
      return FALSE;
    }
  }

  /* Recompute freq comp for amam table , if frequency is changing */
  if( core_data->rfgsm_core_handle_ptr->rf_tx_arfcn != arfcn )
  { 
    core_data->rfgsm_core_handle_ptr->tx_log.tx_chan = arfcn;
    core_data->rfgsm_core_handle_ptr->tx_log.log_flag = TRUE;

    rfgsm_core_compute_linear_freq_comp(core_data->rfm_dev, tx_band, arfcn);

    /* Compensate pcl vs power curve with calibrated err data */
    rfgsm_core_compute_pcl_vs_power(core_data->rfm_dev, tx_band, arfcn);

    core_data->rfgsm_core_handle_ptr->ftm_path_delay_cal_override = FALSE;

    rfgsm_core_set_tx_profile_update_flag(core_data->rfgsm_core_handle_ptr);
  } /* rf_tx_arfcn != arfcn */

  /* Update the tx arfcn */
  core_data->rfgsm_core_handle_ptr->rf_tx_arfcn = arfcn;

  return( TRUE );
} /* end of rfgsm_tune_tx */

/*----------------------------------------------------------------------------*/
/*!
   @brief
  This function updates the frequency error (in ppb).
  For example, if user passes a value of 2054  (ppb) it corresponds to 2054/1024 = 2.005 ppm. The resolution of frequency error is hence .001 ppm.
  @details

*/
void rfgsm_core_update_tx_freq_error(rfgsm_core_data_type *core_data, int32  freq_err)
{
  core_data->rfgsm_core_handle_ptr->rfgsm_curr_freq_error_tx= freq_err;
  /* update the device layer with gsm freq error information*/
  rfdevice_gsm_cmd_dispatch( core_data->rfm_dev, RF_PATH_0, RFDEVICE_GSM_SET_TX_FREQ_ERROR, &freq_err);
}

/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function returns the value of frequency error (in ppb)
   @details

   @param 

   @retval 
   
*/
int32 rfgsm_core_get_tx_freq_error(rfgsm_core_handle_type *rfgsm_core_handle_ptr)
{
  return rfgsm_core_handle_ptr->rfgsm_curr_freq_error_tx;
}



void rfgsm_core_set_gl1_freq_error_flag ( rfm_device_enum_type rfm_dev, boolean pass_flag)
{

	 rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
	 
	 rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
	 
     RF_NULL_CHECK_RTN_V(rfgsm_core_handle_ptr,"rfgsm_core_handle_ptr is NULL!");
    
     rfgsm_core_handle_ptr->freq_error_flag_from_api = pass_flag;
     //MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH," FLAG set by FREQ ADJUST command : :%d", freq_error_flag_from_api);

   
}

boolean rfgsm_core_get_gl1_freq_error_flag(rfgsm_core_handle_type *rfgsm_core_handle_ptr)
{
  return rfgsm_core_handle_ptr->freq_error_flag_from_api;
}



/*---------------------------------------------------------------------------*/
/*! @brief This function populates the default master PA tables to the DM MEM*/
/*! @details This function populates the band-specific tables for the maximum*/
/*!          possible number of slots.                                       */
/*! @param rfm_dev - which RFM device to program event for                   */
/*! @retval boolean - success status                                         */
boolean rfgsm_core_init_master_pa_ramp_tables( rfgsm_core_data_type *core_data )
{
  uint16 *master_ramp_up_tbl_ptr = NULL;
  uint16 *master_ramp_down_tbl_ptr = NULL;
  rfgsm_nv_band_tx_data_type *rfgsm_temp_tx_nv_tbl = NULL;
  gsm_pa_ramp_lut_type *polar_ramp_ptr = NULL;
  rflm_gsm_pa_ramp_type temp_ramps_var; //used for memscpy
  rflm_gsm_band_type tx_band = RFLM_GSM_NUM_GSM_BANDS;
  boolean ret_val = FALSE;
  uint16 rfgsm_pa_ramp_up_76p8[RFLM_GSM_MAX_NUM_RAMP_SAMPLES] = {0};
  uint16 rfgsm_pa_ramp_down_76p8[RFLM_GSM_MAX_NUM_RAMP_SAMPLES] = {0};

  /*-------------------------------------------------------------------------*/
  /*@TODO As the NV PA ramp tables are only for one WTR sampling frequency we*/
  /*      need to modify the table if the WTR is of another freq. Therefore, */
  /*      based on device, the ramp may or may not need up-sampling and      */
  /*      spline interpolation. A call to device will return the sampling    */
  /*      freq of that WTR and then we can make the decision and interpolate */
  /*      here if required. The result will be based on polar_ramp_ptr but   */
  /*      assigned to a seperate temporary ramp table local variable which   */
  /*      will then be copied to the master core DM table.                   */
  /*      NOTE: Currently only 52MHz (24 entries per table) is supported here*/

  /* Copy all of the band tables */
  for ( tx_band = 0; tx_band < RFLM_GSM_NUM_GSM_BANDS; tx_band++ )
  {
    /* Get the band-specific Tx NV ptr */
    rfgsm_temp_tx_nv_tbl = (rfgsm_nv_band_tx_data_type *)rfgsm_nv_get_tbl_ptr(core_data->rfm_dev, (rfcom_gsm_band_type)tx_band, RFGSM_NV_TX);

    if ( (rfgsm_temp_tx_nv_tbl != NULL) && (rfgsm_temp_tx_nv_tbl->tx_static_nv_data_ptr != NULL) )
    {
      /* Get NV ramp data address */
      polar_ramp_ptr = &rfgsm_temp_tx_nv_tbl->tx_static_nv_data_ptr->polar_paramp_lut;

      /* Get band-specific master core DM ramp tables */
      master_ramp_up_tbl_ptr = rfgsm_core_dm_get_master_pa_ramp_up_table_addr( core_data->sub_id, tx_band );
      master_ramp_down_tbl_ptr = rfgsm_core_dm_get_master_pa_ramp_down_table_addr( core_data->sub_id, tx_band );
      RF_NULL_CHECK_RTN( master_ramp_up_tbl_ptr, "RF GSM init ramps master_ramp_up_tbl_ptr is NULL!", FALSE );
      RF_NULL_CHECK_RTN( master_ramp_down_tbl_ptr, "RF GSM init ramps master_ramp_down_tbl_ptr is NULL!", FALSE );

      #ifdef FEATURE_BOLT_MODEM
      rfgsm_mdsp_upsample_pa_ramp(polar_ramp_ptr->ramp_up, rfgsm_pa_ramp_up_76p8, RFGSM_MDSP_SCALE_3P25_TO_4P8MHZ);
      rfgsm_mdsp_upsample_pa_ramp(polar_ramp_ptr->ramp_down, rfgsm_pa_ramp_down_76p8, RFGSM_MDSP_SCALE_3P25_TO_4P8MHZ);
      #else
      memscpy(rfgsm_pa_ramp_up_76p8,sizeof(rfgsm_pa_ramp_up_76p8),polar_ramp_ptr->ramp_up,sizeof(rfgsm_pa_ramp_up_76p8));
      memscpy(rfgsm_pa_ramp_down_76p8,sizeof(rfgsm_pa_ramp_down_76p8),polar_ramp_ptr->ramp_down,sizeof(rfgsm_pa_ramp_down_76p8));
      #endif

      /* Copy the master NV ramp tables to master core DM table memory */
      memscpy( master_ramp_up_tbl_ptr,
               sizeof(temp_ramps_var.pa_ramp_up_table),
               rfgsm_pa_ramp_up_76p8,
               sizeof(rfgsm_pa_ramp_up_76p8) );

      memscpy( master_ramp_down_tbl_ptr,
               sizeof(temp_ramps_var.pa_ramp_down_table),
               rfgsm_pa_ramp_down_76p8,
               sizeof(rfgsm_pa_ramp_down_76p8) );
      ret_val = TRUE;
    }
  }

  return ret_val;
}



/*---------------------------------------------------------------------------*/
/*! @brief This function scales the ramps                                    */
/*! @details This function scales the band-specific tables based on scale    */
/*!          num_pa_ramp_table_entries can be:                               */
/*!          24=DIME52MHz, 26=BOLT57p6MHz, or 36=BOLT76p8MHz                 */
/*! @param rfm_dev - which RFM device to program event for                   */
/*! @param tx_slot - which Tx slot's ramp tables to scale                    */
/*! @param pa_scale - scale factor                                           */
/*! @param ramp_table_entries - 24=DIME52MHz, 26=BOLT57p6MHz, 36=BOLT76p8MHz */
/*! @retval boolean - success status                                         */
#define RFGSM_SCALE_FACTOR_SHIFT 15
#define RFGSM_SATURATE(val, mn,mx) MIN(mx, MAX((val),(mn)) )

boolean rfgsm_core_scale_tx_pa_ramp_data( rfm_device_enum_type rfm_dev,
                                          uint8 sub_id,
                                          uint8 tx_slot,
                                          uint16 pa_scale )
{
  /* Initialise local variables */
  uint16 *ramp_up_tbl_ptr = NULL;
  uint16 *ramp_down_tbl_ptr = NULL;
  uint16 *master_ramp_up_tbl_ptr = NULL;
  uint16 *master_ramp_down_tbl_ptr = NULL;
  uint32 pa_ramp_up_lut_entry;
  uint32 pa_ramp_down_lut_entry;
  uint32 index;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;

  /* Get the device-specific core handle data */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  /* Check the core handle pointer is not NULL for rfm_dev ID */
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, 
                    "rfgsm_core_set_tx_ramp_data core handle is NULL!", 
                    FALSE);

  /* Check the Tx slot number is valid */
  if ( tx_slot >= RFGSM_MAX_TX_SLOTS_PER_FRAME )
  {
    RFGC_MSG_1(MSG_LEGACY_ERROR,"rfgsm_core_scale_tx_ramp_data Tx slot val %d invalid", tx_slot);
    return FALSE;
  }

  /* Get the master PA ramp table pointers to GSM core DM memory */
  master_ramp_up_tbl_ptr = rfgsm_core_dm_get_master_pa_ramp_up_table_addr( sub_id, (rflm_gsm_band_type)rfgsm_core_handle_ptr->tx_band );
  master_ramp_down_tbl_ptr = rfgsm_core_dm_get_master_pa_ramp_down_table_addr( sub_id, (rflm_gsm_band_type)rfgsm_core_handle_ptr->tx_band );

  RF_NULL_CHECK_RTN(master_ramp_up_tbl_ptr, "master_ramp_up_tbl_ptr is NULL!", FALSE);
  RF_NULL_CHECK_RTN(master_ramp_down_tbl_ptr, "master_ramp_down_tbl_ptr is NULL!", FALSE);

  /* Get the PA ramp table pointers to GSM core DM memory */
  ramp_up_tbl_ptr = rfgsm_core_dm_get_pa_ramp_up_table_addr( sub_id, rfgsm_core_handle_ptr->pa_ramp_buffer_id, tx_slot );
  ramp_down_tbl_ptr = rfgsm_core_dm_get_pa_ramp_down_table_addr( sub_id, rfgsm_core_handle_ptr->pa_ramp_buffer_id, tx_slot );

  RF_NULL_CHECK_RTN(ramp_up_tbl_ptr, "ramp_up_tbl_ptr is NULL!", FALSE);
  RF_NULL_CHECK_RTN(ramp_down_tbl_ptr, "ramp_down_tbl_ptr is NULL!", FALSE);

  /* Overwrite the GSM core DM ramp entries with the scaled values */
  for( index = 0; index < RFLM_GSM_MAX_NUM_RAMP_SAMPLES; index++ )
  {
    /* Assign temp var with scaled master ramp entry for up and down table */
    pa_ramp_up_lut_entry = ( pa_scale * ((uint32)master_ramp_up_tbl_ptr[index]) );
    pa_ramp_down_lut_entry = ( pa_scale * ((uint32)master_ramp_down_tbl_ptr[index]) );

    /* Shift the value back by unity to get the scaled ramp values */
    pa_ramp_up_lut_entry >>= RFGSM_SCALE_FACTOR_SHIFT;
    pa_ramp_down_lut_entry >>= RFGSM_SCALE_FACTOR_SHIFT;

    /* Ensure the ramp entry is not greater than the maximum allowed value or less than the minimum */
    ramp_up_tbl_ptr[index] = (uint16)RFGSM_SATURATE( pa_ramp_up_lut_entry,
                                                     RFGSM_RAMP_SATURATION_LOW_LIMIT, 
                                                     RFGSM_RAMP_SATURATION_HIGH_LIMIT);
    ramp_down_tbl_ptr[index] = (uint16)RFGSM_SATURATE( pa_ramp_down_lut_entry,
                                                       RFGSM_RAMP_SATURATION_LOW_LIMIT, 
                                                       RFGSM_RAMP_SATURATION_HIGH_LIMIT);
  }

  /* NOTE: Fractional interpolation due to unmatched samples/rates drift is  */
  /*       executed in FW context in RFLM as this may require knowledge from */
  /*       FW.                                                               */

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*! @brief This function switches the ping-pong buffer for Tx pa ramps       */
/*! @details When the ramp is programmed the ping-pong id needs switching to */
/*!          ensure that the next frame's programming is happening on the    */
/*!          other ramp table memory to prevent read/write collisions        */
/*! @param rfm_dev - which RFM device to program event for                   */
/*! @retval boolean - success status                                         */
boolean rfgsm_core_tx_pa_ramp_data_buffer_switch( rfm_device_enum_type rfm_dev )
{
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;

  /* Get the device-specific core handle data */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  /* Check the core handle pointer is not NULL for rfm_dev ID */
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, 
                    "rfgsm_core_tx_ramp_data_buffer_switch core handle is NULL!", 
                    FALSE);

  /* Switch buffers for next iteration */
  /* PA ramp ping-pong'ing needs to be handled by core handle to ensure seperate ping-pong ID per device */
  rfgsm_core_handle_ptr->pa_ramp_buffer_id = 
     ( rfgsm_core_handle_ptr->pa_ramp_buffer_id == RFGSM_CORE_DOUBLE_BUF0 ) ? RFGSM_CORE_DOUBLE_BUF1 : RFGSM_CORE_DOUBLE_BUF0;

  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*! @brief This function sets the ping-pong buffer for Tx pa ramps           */
/*! @details During Tx calibration we force the PA ramp buffer to use        */
/*!          ping-pong buffer index 0                                        */
/*! @param rfm_dev - which RFM device to program event for                   */
/*! @param buffer_index - Which ping-pong buffer index to use                */
/*! @retval boolean - success status                                         */

boolean rfgsm_core_tx_set_pa_ramp_data_buffer( rfm_device_enum_type rfm_dev, uint8 buffer_index )
{
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;

  /* Get the device-specific core handle data */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  /* Check the core handle pointer is not NULL for rfm_dev ID */
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, 
                    "rfgsm_core_tx_ramp_data_buffer_switch core handle is NULL!", 
                    FALSE);

  /* Switch buffers for next iteration */
  /* PA ramp ping-pong'ing needs to be handled by core handle to ensure seperate ping-pong ID per device */
  rfgsm_core_handle_ptr->pa_ramp_buffer_id = buffer_index;

  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*! @brief This function switches the ping-pong buffer for AMAM and AMPM tables */
/*! @details When the amam and ampm tables are programmed the ping-pong id      */ 
/*!           needs switching to                                                */
/*!          ensure that the next frame's programming is happening on the       */
/*!          other amam/ampm table memory to prevent read/write collisions           */
/*! @param rfm_dev - which RFM device to program event for                      */
/*! @retval boolean - success status                                            */
boolean rfgsm_core_tx_amam_ampm_data_buffer_switch( rfm_device_enum_type rfm_dev )
{
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;

  /* Get the device-specific core handle data */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  /* Check the core handle pointer is not NULL for rfm_dev ID */
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, 
                    "rfgsm_core_tx_ramp_data_buffer_switch core handle is NULL!", 
                    FALSE);

  /* Switch buffers for next iteration */
  /* AMAM AMPM ramp ping-pong'ing needs to be handled by core handle to ensure seperate ping-pong ID per device */
  rfgsm_core_handle_ptr->amam_ampm_buffer_id = 
     ( rfgsm_core_handle_ptr->amam_ampm_buffer_id == RFGSM_CORE_DOUBLE_BUF0 ) ? RFGSM_CORE_DOUBLE_BUF1 : RFGSM_CORE_DOUBLE_BUF0;

   //RFGC_MSG_1(MSG_LEGACY_ERROR,"AMAM AMPM Ping-Pong buffer switched. New ID = %d", rfgsm_core_handle_ptr->amam_ampm_buffer_id);

  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
   @fn rfgsm_core_set_tx_transition_bufferr 
   @brief This function sets the TX transition buffer to TXAGC trigger scripts based on modulation type
   @param  rfm_dev - RFM device 
   @param  tx_slot - TX slot
   @param  mod_type - Modulation type 
   @retval None
*/
void rfgsm_core_set_tx_transition_buffer( rfm_device_enum_type rfm_dev,
                                          uint8 tx_slot,
                                          rfgsm_modulation_type mod_type )
{

  rfgsm_core_handle_type *rfgsm_core_handle_ptr;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  RF_NULL_CHECK_RTN_V(rfgsm_core_handle_ptr,"rfgsm_core_handle_ptr is NULL!");

  /*update the TX Trans buffer with approroate TXAGC trigger based modulation*/
  if (mod_type== RF_MOD_GMSK)
  {
    rfgsm_core_handle_ptr->scripts.tx_trans_script[TX_TRANS_WTR_SCRIPT][tx_slot] = rfgsm_core_handle_ptr->scripts.tx_agc_trigger_gmsk_script[tx_slot];
  }
  else
  {
    rfgsm_core_handle_ptr->scripts.tx_trans_script[TX_TRANS_WTR_SCRIPT][tx_slot] = rfgsm_core_handle_ptr->scripts.tx_agc_trigger_8psk_script[tx_slot];
  }

}

/*----------------------------------------------------------------------------*/
/*!
  @details
  This function clears the WTR TX burst scripts
 
  @param rfm_dev : rfm device
 
  @retval True/False
*/

boolean rfgsm_core_clear_tx_burst_device_scripts( rfgsm_core_handle_type *rfgsm_core_handle_ptr)
{

  if( !rf_buffer_clear_script( (rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.tx_start_script[TX_WTR_SCRIPT], RF_BUFFER_ALL)||
      !rf_buffer_clear_script( (rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.tx_stop_script[TX_WTR_SCRIPT], RF_BUFFER_ALL) )
  {
    return FALSE;
  }

  return TRUE;
}

/*----------------------------------------------------------------------------*/
/*!
   @fn rfgsm_core_populate_tx_burst_device_buffers 
   @brief This function retrieves wtr device driver scripts for GSM Burst and stores them in rfgsm_core_handles pointer
   @param  rfm_dev - RFM device processing for
   @param  burst - TX
   @retval boolean - success/failure indication
*/
boolean rfgsm_core_populate_tx_burst_device_buffers( rfgsm_core_data_type *core_data,
                                                     rf_burst_type burst)
{

  rfgsmlib_sbi_burst_struct burst_script_infor;
  uint16 buffer_mask;
  #ifdef FEATURE_TX_SHARING
  uint16 temp_tx_num_trans = 0;
  #endif
  uint32 rfgsm_time_profile[3] = {0,0,0};
  burst_script_infor.burst = burst;
  burst_script_infor.burst_alpha = 625;
  burst_script_infor.scripts = NULL;
  burst_script_infor.override_alpha = FALSE;
  burst_script_infor.burst_timing_offset_adjust_qs = 0;
  uint8 fbrx_enable_timing;
  int8 fbrx_disable_timing;
  boolean fbrx_path_enabled = (core_data->rfgsm_core_handle_ptr->fbrx_vswr_enabled && 
                              rfgsm_core_dm_vswr_get_meas_requested(core_data->sub_id)) ||
                              core_data->rfgsm_core_handle_ptr->enable_fbrx_every_burst;

  GET_START_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0]);

  #ifdef FEATURE_RF_POWER_REPORTING
  /* log channel number */
  rf_power_reporting_update_channel( core_data->rfm_dev, core_data->rfgsm_core_handle_ptr->rf_tx_arfcn);
  #endif

  /*Populate WTR Burst Scripts*/
  if(!rfdevice_gsm_tx_set_start_script(core_data->rfm_dev,
                                       RFDEVICE_OPER_MODE_STANDALONE,
                                       &burst_script_infor, 
                                       (rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_start_script[TX_WTR_SCRIPT],
                                       &buffer_mask,
                                       (rfcom_gsm_band_type)((rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->tx_band)))
  {
    RF_MSG_1(RF_ERROR,"RF GSM WTR TX start script retrieval failed for rfm_dev|%d|",
                      core_data->rfm_dev);
    return FALSE;
  }

  if( fbrx_path_enabled )
  {
    // Add FBRX Enable to the TX start script - should be 250qs before the burst
    if( !rfdevice_gsm_fbrx_vswr_meas_enable( core_data->rfm_dev,
                                             core_data->rfgsm_core_handle_ptr->band,
                                             3,
                                             &fbrx_enable_timing,
                                             RFDEVICE_CREATE_SCRIPT,
                                             (rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_start_script[TX_WTR_SCRIPT] ) )
    {
      RF_MSG_1(RF_ERROR,"RF GSM WTR TX start script FBRX enable failed %d",
                        core_data->rfm_dev);
      return FALSE;
    }
  }

  GET_CURRENT_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[1]);

#ifdef FEATURE_TX_SHARING
  
  temp_tx_num_trans = rf_buffer_get_num_trans((rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_start_script[TX_WTR_SCRIPT],
											RF_BUFFER_RFFE);

  RF_MSG_1(MSG_LEGACY_ERROR, "Tx start script num trans %d", temp_tx_num_trans);
#endif

  if(!rfdevice_gsm_tx_set_stop_script(core_data->rfm_dev, 
                                     (rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_stop_script[TX_WTR_SCRIPT]))
  {
    RF_MSG_1(RF_ERROR,"RF GSM WTR TX stop script retrieval failed for rfm_dev|%d|",
                      core_data->rfm_dev);
    return FALSE;
  }

  if( fbrx_path_enabled )
  {
    // Add on FBRX Disable
    if(!rfdevice_gsm_fbrx_vswr_meas_disable( core_data->rfm_dev,
                                            RFDEVICE_CREATE_SCRIPT,
                                             &fbrx_disable_timing,
                                            (rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_stop_script[TX_WTR_SCRIPT] ) )
    {
      RF_MSG_1(RF_ERROR,"RF GSM WTR TX start script FBRX disable failed %d",
                        core_data->rfm_dev);
      return FALSE;
    }
  }


  GET_CURRENT_TIME_TX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[2]);

#ifdef FEATURE_TX_SHARING
	
  temp_tx_num_trans = rf_buffer_get_num_trans((rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.tx_stop_script[TX_WTR_SCRIPT],
											  RF_BUFFER_RFFE);

  RF_MSG_1(MSG_LEGACY_ERROR, "Tx start script num trans %d", temp_tx_num_trans);
#endif

  /*Populating Device Status Read Event Scripts During Tx Burst */
  if(core_data->rfgsm_core_handle_ptr->device_config_to_read.params[RF_TX_BURST].enable)
  {
    rfgsm_core_populate_device_status_read_buffers(core_data, RF_TX_BURST);
  }
  
  rfgsm_core_log(core_data->rfm_dev);

  if(core_data->rfgsm_core_handle_ptr->enable_time_profiling.tx_burst)				
  {
    RF_MSG_2(MSG_LEGACY_HIGH,"Tx Burst Time Profiling, rfgsm_core_tx_set_start_script: %d us, rfgsm_core_tx_set_stop_script: %d us",
             rfgsm_time_profile[1],
             (rfgsm_time_profile[2] - rfgsm_time_profile[1]));
  }
  
  return TRUE;
} /* rfgsm_core_populate_device_tx_burst_buffer */



/*----------------------------------------------------------------------------*/
/*!
   @fn rfgsm_core_update_linear_amam_ampm_profiles
   @brief Performs frequency and temperature compensation of amam and ampm tables
   @details
   @param
   @retval None
*/

void rfgsm_core_update_linear_amam_ampm_profiles(rfgsm_core_data_type *core_data)
{
  uint16* dm_amam_table_ptr = NULL; 
  uint16* dm_ampm_table_ptr = NULL;

  uint16* final_amam_table_ptr = NULL;
  uint16* final_ampm_table_ptr = NULL;

  /* Compute the freq comp AMAM/AMPM tables */
  rfgsm_mdsp_compute_linear_amam_freq_comp(core_data->rfm_dev, core_data->rfgsm_core_handle_ptr->tx_band, core_data->rfgsm_core_handle_ptr->rf_tx_arfcn);

  /* Add slope to AMAM table based on temp bin */
  rfgsm_mdsp_compute_linear_amam_temp_comp(core_data->rfm_dev, TRUE);

  /******** AMAM Table Data manager updates ********/
  /* Get AMAM table location in Data Manager */
  dm_amam_table_ptr = rfgsm_core_dm_get_amam_table_addr( core_data->sub_id, 
                                                         core_data->rfgsm_core_handle_ptr->amam_ampm_buffer_id );

  /* Get the Final AMAM Table location from rfgsm_mdsp layer table */
  final_amam_table_ptr = rfgsm_mdsp_get_final_amam_ampm_table( core_data->rfm_dev, AMAM_TABLE );

  /* Copy the contents of the final AMAM table location to memory space in Data manager */
  if ( (final_amam_table_ptr != NULL) && (dm_amam_table_ptr != NULL) )
     {
        memscpy (dm_amam_table_ptr,
                 RFLM_GSM_MAX_NUM_AMAM_SAMPLES*sizeof(uint16),
                 final_amam_table_ptr,
                 RFLM_GSM_MAX_NUM_AMAM_SAMPLES*(sizeof(uint16))); /* Table size is reduced by a factor of 4 */
     }
     else
     {
        RF_MSG_2(MSG_LEGACY_ERROR,"Final AMAM Table or DM AMAM Table ptr is NULL: 0x%x, 0x%x",final_amam_table_ptr,dm_amam_table_ptr);
        return;
     }

  /******* AMPM Table Data Manager updates *******/
  /* Get AMPM table location in Data Manager */
  dm_ampm_table_ptr = rfgsm_core_dm_get_ampm_table_addr( core_data->sub_id, 
                                                          core_data->rfgsm_core_handle_ptr->amam_ampm_buffer_id );

  /* Get the Final AMPM Table location from rfgsm_mdsp layer table */
  final_ampm_table_ptr = rfgsm_mdsp_get_final_amam_ampm_table( core_data->rfm_dev, AMPM_TABLE );

  /* Copy the contents of the final AMPM table location to memory space in Data manager */
  if ( (final_ampm_table_ptr != NULL) && (dm_ampm_table_ptr != NULL) )
     {
         memscpy (dm_ampm_table_ptr,
                 RFLM_GSM_MAX_NUM_AMPM_SAMPLES*sizeof(uint16),
                 final_ampm_table_ptr,
                 RFLM_GSM_MAX_NUM_AMAM_SAMPLES*(sizeof(uint16))); /* Table size is reduced by a factor of 4 */
     }
     else
     {
       RF_MSG_2(MSG_LEGACY_ERROR,"Final AMPM Table or DM AMPM Table ptr is NULL: 0x%x, 0x%x",final_ampm_table_ptr,dm_ampm_table_ptr);
        return;
     }

  return;
}

/*----------------------------------------------------------------------------*/
/*! 
   @fn rfgsm_core_update_linear_amam_temp_comp
   @brief Performs temperature compensation of amam table
   @details
   @param
   @retval None
*/

void rfgsm_core_update_linear_amam_temp_comp(rfgsm_core_data_type *core_data)
{
  uint16* dm_amam_table_ptr = NULL; 
  uint16* final_amam_table_ptr = NULL;

  /* Add slope to AMAM table based on temp bin */
  rfgsm_mdsp_compute_linear_amam_temp_comp(core_data->rfm_dev, TRUE);

  /******** AMAM Table Data manager updates ********/
  /* Get AMAM table location in Data Manager */
  dm_amam_table_ptr = rfgsm_core_dm_get_amam_table_addr( core_data->sub_id, 
                                                         core_data->rfgsm_core_handle_ptr->amam_ampm_buffer_id );

  /* Get the Final AMAM Table location from rfgsm_mdsp layer table */
  final_amam_table_ptr = rfgsm_mdsp_get_final_amam_ampm_table( core_data->rfm_dev, AMAM_TABLE );

  /* Copy the contents of the final AMAM table location to memory space in Data manager */
  if ( (final_amam_table_ptr != NULL) && (dm_amam_table_ptr != NULL) )
  {
     memscpy (dm_amam_table_ptr,
              RFLM_GSM_MAX_NUM_AMAM_SAMPLES*sizeof(uint16),
              final_amam_table_ptr,
              RFLM_GSM_MAX_NUM_AMAM_SAMPLES*(sizeof(uint16))); /* Table size is reduced by a factor of 4 */
  }
  else
  {
     RF_MSG_2(MSG_LEGACY_ERROR,"Final AMAM Table or DM AMAM Table ptr is NULL: 0x%x, 0x%x",final_amam_table_ptr,dm_amam_table_ptr);
     return;
  }

  return;
}

/*----------------------------------------------------------------------------*/

/*!
   @brief
   Configures the FBRX chain by allocating a RXLM handle, updating the settings
   and writing the settings to the modem.
   
   @param *core_data_ptr - Contains core data used to extract device.

   @retval boolean success/failure indication
   
*/
boolean rfgsm_core_config_fbrx_rflm( rfgsm_core_data_type * core_data,
                                     rfm_device_enum_type tx_device )
{

#ifdef FEATURE_RF_HAS_WTR3925
  rxlm_state_cfg_params rxlm_cfg_params = {
       RFLM_DTR_RX_NUM_CARR_DEFAULT, RFLM_DTR_RX_BANDWIDTH_DEFAULT, 
       RFLM_DTR_RX_MODE_GSM_ONLINE_POWER, RFLM_DTR_RX_DEBUG_PT_INVALID , 
       RFLM_DTR_RX_TRX_WTR3925};
	   
  rflm_dtr_rx_indices_struct rx_nb_indices;
  lm_status_type lm_status = LM_SUCCESS;

  uint8 fbrx_modem_chain = 3; /* Deault FB chain for most FDD bands on Jacala is 3*/
  boolean success_flag = TRUE;
  if(!rfcommon_fbrx_msm_allocate_rxlm_buffer(&core_data->rfgsm_core_handle_ptr->fbrx_buf_index))
  {
    ERR_FATAL("FBRx buffer allocation failed",0,0,0);
    return FALSE;
  }

  if( rxlm_update_static_settings( core_data->rfgsm_core_handle_ptr->fbrx_buf_index, 
                                   (void*)&rxlm_cfg_params) != LM_SUCCESS )
  {
    ERR_FATAL("FBRx buffer static settings failed",0,0,0);
    return FALSE;
  }

  lm_status = rxlm_get_buffer_content ( core_data->rfgsm_core_handle_ptr->fbrx_buf_index ,
                     (uint32)RFLM_DTR_RX_INDICES ,
                     (uint8 *)&rx_nb_indices);


  RF_MSG_5( RF_HIGH, "rf_gsm_msm_get_wb_nb_indices:| WB:0X%x| NB[0]: 0X%x | "
                    "NB[1]:0X%x | NB[2]:0X%x for handle :%d",
            rx_nb_indices.wbw_idx_0,
            rx_nb_indices.nbn_idx_0,
            rx_nb_indices.nbn_idx_1,
            rx_nb_indices.nbn_idx_2,
            core_data->rfgsm_core_handle_ptr->fbrx_buf_index);

  /* @todo: remove hard-coding, tech to update the fbrx chain from rfc */
  if( rfcommon_fbrx_enable( core_data->rfgsm_core_handle_ptr->fbrx_buf_index, fbrx_modem_chain) != RFCOMMON_FBRX_SUCCESS)
  {
    ERR_FATAL("FBRx enable failed",0,0,0);
    return FALSE;
  }

  // Update the FBRX buf index in RFLM
  success_flag = rfgsm_core_dm_set_fbrx_buf_index( core_data->sub_id, 
                                    core_data->rfgsm_core_handle_ptr->fbrx_buf_index );

  return success_flag;
#else 

  return TRUE;

#endif
}

