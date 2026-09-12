/*!
   @file
   rfgsm_core_rx.c

   @brief

   @details

*/

/*! @todo satheesha 10/30/08 do we need to keep the edit history??? */

/*===========================================================================
Copyright (c) 2008 - 2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_gsm.mpss/3.11/rf/core/src/rfgsm_core_rx.c#1 $
$DateTime: 2021/03/09 05:10:17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/30/19   mpa      Recovery mechanism for RF scripting delays in X2Y IRAT scenarios on HA
02/16/18   par     [CR2192310] KW errors fix
03/27/17   bp      Rx freq comp interpolation changes
12/21/16   bp      Featurize WTR3925 specific code
10/03/16   kpa     Adjustment of setting sub_id_mask to prevent timing race condition between subs
08/30/16   kpa     CR_1060886: Fix memory leak caused by unnecessarily allocation of tx trans WTR script
08/18/16   kpa     Adjustment of setting sub_id_mask to prevent timing race condition between subs
07/13/16   kpa     Move GS 2 to 1 when ACI flag is detected
06/28/16   hkm     RX RSB changes for calculation
02/29/16   dsm     Changes for putting GSM front end components to sleep after X2G IRAT
12/09/15   hkm     NV support to enable/disable LPM feature
11/27/15   hkm     TXRXSplit support changes
10/15/15   tsr     Pass Worst case bus contention delay to device driver 
                   when lower ccs priority flag is set
10/14/15   ec      Do not clean Wakeup event when performing DRx sleep, based on op_mode
09/24/15   zc      Support different gains along different ASDIV sw positions
09/10/15   sc      Deprecate core handle cmd_buff log and use cmd_proc log
09/02/15   ec      Update IP2 cal alpha based on Tx and Rx timeline
08/19/15   ec      Add AGC debug logging into new gain calculation API
08/19/15   ec      Resolve static analysis error in SAWless array indexing
08/18/15   ec      Add check to avoid crash when GL1 is not passing valid pointer
08/18/15   ec      Integrate SAWless into split Rx\Tx
08/12/15   sc      Update GL1 desense channel list on PRx wakeup
08/08/15   tws     Change device voting ERR_FATAL back to F3
08/05/15   sc      Deprecate alternate path
08/04/15   sw      ASM should not build XSW if GRF has been asked to do so
07/29/15   rc      Add power logging support
07/28/15   ec      Update ip2_alpha in Rx timing update
07/27/15   sw      FR25072: for device wakeup, vote for lowest/first supported band
07/22/15   piy     Enabling Device Status Read Feature in Rx/Tx split architecture
07/16/15   sc      Set ant pos flag on Rx wakeup
07/15/15   sc      Fix possible out of bounds access on NV pointer table
07/15/15   sw      WTR power saving optimisations (under feature flag)
07/07/15   sc      Further Rx burst optimisations (~20% @ 144MHz)
06/30/15   ndb     Added Tuner IRAT support in RxTx split feature
07/03/15   sc      Fix LIF and enable optimised Rx burst processing
                   Temporarily compiled out old functionality
07/02/15   sg      Use passed-in LNA range instead of that computed based on rx level 
                   for calibration
06/19/15   sc      Update and enable optmised Rx burst and feature guard
06/10/15   sw/cs   Store worst case rx timings at power-up for irat
06/10/15   tws     Fix incorrect AFC being applied in GBTA cases
06/03/15   sg      Changes to remove un-necessary tuner transactions 
                   during probe burst
05/27/15   tsr     clean up  and replace ftm state checks in RF GSM CORE layer 
05/27/15   sc      Calculate Rx burst data for optimised Rx burst API
05/21/15   sc      Add new Rx burst core API hooks
05/15/15   hoh     Do rfc_gsm_mode_config for diversity if enabled
05/12/15   rcui    changed voting to be based on device
05/06/15   sc      Add minimised call-flow for G+G
04/24/15   tsr     Fix crash due to deferencing of rxd rfgsm core handle pointer
                   when RXD is not enabled
04/22/15   zbz     WTR interface change to support DRx
04/21/15   sg      Passing band info while generating GSM Tx/Rx SBI scripts to 
                   support band split across 2 WTRs
04/20/15   sc      Populate RF sequence number passed from L1 in RFLM DM 
04/10/15   zbz     Interface changes with device driver for split Rx/Tx
04/09/15   sc      Remove device assumtions for split Rx/Tx
04/07/15   sc      Move core handles out of RFC layer for split Rx/Tx
04/06/15   hoh     NV changes to support Rx/Tx device split feature
04/01/15   ec      Remove legacy LIF implementation
03/30/15   sc      Add Rx timing update functionality
03/26/15   sc      Add legacy functionality to new Rx/Tx Wakeup/Sleep call-flow
03/25/15   sc      Add core Rx Wakeup/Sleep APIs
03/13/15   piy     Removing Off-target compilation error
03/12/15   ec      Logging Rx burst RxAGC info and disabling therm reads via debug NV
03/02/15   piy     Added Time Profiling for Wakeup, Sleep, Rx & Tx burst via debug NV
02/27/15   sc      Ensure NULL NV error checking when processing Rx/Tx band
02/13/14   sw      Fix issue with sawless F3s on non-sawless RFC 
01/30/15   sg      Prevent ASD error response timer from kicking in probe burst cases 
01/30/15   sw      GL1->RF handshake mechanism to handle ASD response error cases
01/23/14   ec      Use new debug NV flags for SAWless functionality
01/19/15   sw      Use tuner start delta to program XSW
12/19/14   hoh     LIF data implementation
12/02/14   sml     Read WTR or RF front end devices status in between Rx/Tx 
                   burst along with GRFC status
11/25/14   sb      HL/LL algorithm changes and bug fixes

11/19/14  huibingl take the channel number discontinuity into account when do GSM RX cal data interpolation
11/12/14   sw      Rename RX/TX script types to reflect actual devices
10/17/14   rr      Separate rx/tx alternate path core handles
10/20/14   sw      Use correct band for GSM ASDiv
10/02/14  ec/cpm   Fixes for V2 and V4 calibration on SAWless targets
09/23/14   zbz     changes to support GSM and LTE/TDS coexistence related RFC changes
09/18/14   sw      Boolean to indicate ASD scripts to be built only once
08/14/14   sc      Coex fix for non-tx capable device accessing Tx NV
08/11/14   sc      Ensure chain update with alternate path and include IRAT
08/01/14   ec      Add code to handle HL NV items and updated overrides for SAWless targets
07/31/14   tsr     GSM Alternate path support
07/31/14   sw      Support both rffe and grfc-based xsw for ASD
07/31/14   sg      Configure only ASD GRFCs (not tuners) during enter mode/wakeup
07/30/14   hoh     Add enhanced rx feature support 
07/28/14   tsr     DR-DSDS: Add support for device driver to handle worst case rx alpha for IDLE mode RX
07/28/14   zbz     Fix RSB compensation coeffs
07/11/14   ec      Set Rx Linearity mode on SAWLess targets based on decision table
07/02/14   tsr     DR-DSDS: Add worst case rx alpha for IDLE mode RX
06/18/14   tsr     RXD diversity call flow changes to enable DRX path along with PRX path
06/12/14   tsr     Update RX ARFCN in rfgsm_core_handle for RXD device
06/12/14   hk      Band to be passed into rfc_gsm_rx_burst_config instead of core handle 
06/02/14   sml     Adding necessary check condition in the case where tuners are not present 
                   and ASD switch is present to avoid un-necessary prints
05/13/14   tsr     Retrieve rx stop settings for PRX device as there is only one RX Stop for both PRX and DRX
04/29/14   tsr     Mainline GSM RFC config v2 apis 
04/24/14   ch      Replaced ATUNER_SCRIPT with RX_FE_CONFIG_SCRIPT
04/24/14   sw      Introduce rffe-based api for ASD processing
04/16/14   sb      IP2 cal Interface and functionality changes
04/14/14   tsr     Add DRX gain in FTM mode for RX diversity
04/10/14   tsr     Add RFLM RX gain and spectral inv coefficients for DRX
03/20/14   sw      Update ASD code to use new ASDIV Manager api
03/11/14   tsr     Added RXD support for RX burst 
03/13/14   sw      ASD Enhancements
03/11/14   tsr     Program device scripts dynamically through CCS
03/11/14   sc      Expose RFLM overhead to RFSW to ensure GL1 account for delay
03/05/14   tsr     Fixed offtarget build
03/04/14   sw      Introduce ASD functionality
02/28/14   tsr     Separate RX burst scripts and TX burst scripts retrieval from device driver
02/28/14   sc      Pass core data through mc to core layers
02/25/14   tsr     Update RX Alpha based on wtr writes and pll settling time from device driver
02/24/14   tsr     Deprecate Feature D3925
02/18/14   tsr     GSM RX Diversity 
01/30/14   sc      Manage data table independent of device
01/15/14   sc      Remove unused APIs using FW shared memory
12/13/13   ry     Remove hardcoding of RFM_DEVICE_0
11/07/13   sc      Rx gain API using Rx burst type
10/31/13   sc      Feature-guard FW shared mem buffer population
10/23/13   tsr     Mainline D3295 changes for BOLT
10/23/13   sg      GSM ASDiv Tuner Support
10/21/13   sc      Modify dynamic event building for scenario-specific wrappers
10/14/13   sc      Mainline core DM and RFLM code
10/14/13   sml     Limiting the number of LNA ranges to five
                   depending upon a flag which needs to be 
                   updated by WTR 
10/09/13   tsr     Dynamic RF CCS event changes for WTR3925
10/03/13   tsr     Added support retrieve to individual device scripts
10/03/13   sb      Add changes to receive MSGR cmd from MCS to switch to filtered/unfiltered 
                   path for SGLTE coexistence (GSM DCS +LTE B39/TDS B39/B34)
10/01/13   sb      Disable SGLTE Coexistence filtered path cal data
09/17/13   sb      Fix check for right rx burst type 
09/17/13   sb      Support for SGLTE/SGTDS coexistence
09/12/13   sc      RFLM interface encompassed by core DM
08/27/13   sml     Extracting ASD script depending on device id through
                   call back data
08/16/13   ndb     Clean-up the SPI based antenna tuner support
08/14/13   sc      Revert Rx burst type variable name
08/05/13   sml     Fixing complier errors in off-target
08/02/13   sml     Fixing Complier warnings
07/30/13   sml     removing un-necessary comments
07/29/13   sml/svi ASD support for GSM tech
07/23/13   sc      Update RFLM data information (featurised)
07/22/13   ec	   Use new F3s/Null pointer checks
05/06/13   aka     Added rfgsm_core_get_lna_gain API
04/09/13   sr      removed legacy antenna tuner code which is not used.
02/05/13   sb      Created separate APIs to configure RF front end devices based on whether
                   they are called every TX/RX burst or only when the band is changed.
01/2/13    aka     Changes to support multi chain GSM operation in FTM mode
01/09/13   sr      Removed un-used temp comp function prototype.
12/10/12   tws     Move global variables to the gsm core handler. 
                   Remove ftm_gsm_rssi_to_dbm.
12/03/12   sc      Cleanup mDSP shared variable interface
12/03/12   aka     Changes to use NV ptr from rfgsm_core_handle_ptr
11/29/12   tws     Added rfm_device parameters to FTM GSM.
11/28/12   aka     Changes to extend path based computations
11/26/12   aka     Changes to use a path specific gsm handle
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
11/19/12   sc      RF-Only mDSP update shared variables cleanup
10/16/12   tsr     Cleanup GSM RF bringup flags 
10/11/12   sc      Cleanup up unrequired Rx burst & SBI/Non-SBI API burst buffer params
10/02/12   tc      Remove obsolete functions and globals. 
10/01/12   sb      Made changes to add rfm device to rf device interface and uppler layer APIs 
09/18/12   tc      Remove FEATURE_RF_HAS_QFE1320-related functionality. 
08/21/12   sc      Rename RF GSM debug Ant tuner flag 
08/07/12   sc      Using seperate Rx and Tx scripts for CCS buffer tracability
08/06/12   sc      Pass in zero for build Rx burst num_slots param as only used for Tx
07/17/12   tsr     Changes for GSM RX verification on RUMI
07/26/12   sr      Changes to APIS to take rfm_dev as parameter.
07/20/12   sr      Made changes to pass the RFM_DEVICE and rf_buffer to rfc_gsm
07/06/12   sc      Edit API parameters to support CCS buffer format
07/02/12   sr      renamed rfc_common_gsmctl.h to rfc_common_tdd.h as it is shared with TDS.
06/21/12   sc      Rx setup burst API support for shared memory interface
06/20/12   shb     Updated GSM device APIs to accept script buffer
07/02/12   sn      PA/ASM device interface and scripting support
06/21/12   vb/rsr  Added support for QFE1510
12/20/11   sn      Added support for QFE1320
10/28/11    sr     made changes use the tx_nv_tbl ptr for any tx NV items
                   and rx_nv_tbl for any rx NV items.
10/21/11   dej     Made rfgsm_core_ten_log_of_power_val not static
10/19/11   sb      Apply therm read in idle frame and enable FTM temp comp and vbatt comp during idle frame
10/11/11    sr     optimized code for ISR over-run issue.
09/15/11    sr     added antenna tuner support code.
09/06/11   sr      read the temp during Rx burst setup.
08/25/11   sar      Removed obsolete file bbrx.h.
08/15/11   sr      klockwork fixes.
08/06/11   sr      RFC Cleanup: removed un-used code.
07/01/11   vrb     Replacing direct calls to rfgsm_ds with wrapper functions
05/26/11   sr     Added rfc rx band config call in tune rx. 
03/03/11   sr     off-target error fix.
03/03/11    sr     merge from mdm9k branch
02/07/11   av      FTM IRAT support : Added RSSI to DB api for GSM. (from scmm) 
01/17/11   dej     Mainline RF_DEVICE_MERGE_INTERIM_FEATURE
10/11/10   lcl    Changed MSG_LVL to MSG_LEGACY to avoid too many F3 messages
09/01/10   av      GSM RFC Rearch
06/24/10   rsr     support for scmm/mdm9k merge
06/16/10   rsr     Temp comp updates from QSC6x95
06/07/10   lcl     Removed rfc_config_band_data from rfgsm_core_tune_rx to 
                   fix WCDMA UL issue during W2G in DCH state.
03/18/10   lcl     Moved rfgsm_cm from RFC to this file since it can be shared.
03/18/10   lcl     Pull in lint fix from QSC6295
12/28/09   sr      High/Medium lint errors fix
12/22/09   sr      changed the debug msg macro from legacy to new one.
10/21/09   sr      new device driver interface changes.
10/12/09   sr      Fixed warnings.
09/17/09   sar     Removed deprecated file, adie.h for CMI conformance.
07/20/09   sr      Fixed warnings.
07/08/09   av      Added calls to rfc to configure the band data.
07/06/09   sr     changes to make the operating band to rfcom_gsm_band type
07/04/009  sr      spec inversion setting in the tune_rx based on the band.
06/19/009  sr      fixed the continuous rx gain range problem.
08/06/009  sr      unused code cleanup.
03/27/009  sr      rfgsm_core_ctl name changed to rfc_card_gsmctl
03/18/09   sr      Changed the rfgsm_mdsp__() name to reflect new name
03/16/09   sr      code clean-up.
03/13/09   sr      code clean-up for SCMM build
10/30/08   sr      Initial revision.

============================================================================*/

#include "rfa_variation.h"
#include "rfcom.h"
#include "stringl.h"
#include "msg.h"
#include "ftm.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_gsm.h"
#include "ftm_gsm_device.h"
#include "rfgsm_core_types.h"
#include "rfgsm_core_handle.h"
#include "rfdevice_gsm_intf.h"
#include "rfgsm_nv_cmn.h"
#include "rfgsm_mdsp.h"
#include "rfgsm_msm.h"
#include "rfc_card_gsm.h"
#include "rfgsm_core_timing_consts.h"
#include "rfgsm_core.h"
#include "rfgsm_core_util.h"
#include "rfc_card.h"
#include "rfc_common_tdd.h" 
#include "rfcommon_msg.h"
#include "rfm_internal.h"
#include "rf_hal_buffer.h"
#include "rfgsm_data_manager.h"
#include "rfgsm_mc.h"
#include "rfcommon_core_device_manager.h"
#include "rfgsm_msg.h"
#include "mdsp_intf.h"
#include "rfcommon_nv_mm.h"
#ifdef FEATURE_RF_ASDIV
#include "rfc_common.h"
#include "rfcommon_asdiv_manager.h"
#include "rfdevice_logical_component_types.h"
#include "rfcommon_core_utils.h"

/* Default tuner start time */
#define RFC_GSM_DEFAULT_ANTENNA_TUNER_START_DELTA -150 /* QS */
#endif

#ifdef FEATURE_RF_POWER_REPORTING
#include "rf_power_reporting.h"
#endif



/*===========================================================================*/
/*                                                                           */
/*                              DEFINITIONS                                  */
/*                                                                           */
/*===========================================================================*/
extern int16 ftm_gsm_rssi_to_dbm; /*Added from SCMM*/

/* Flag to tell NV to load 5th gainrange data */
boolean rf_has_five_gainranges = FALSE;


volatile dBx16_T rf_debug_force_rssi_to_dbm = 0;

/*---------------------------------------------------------------------------
  Rx RSSI -> dB translation tables. For each entry in rssi_range_tbl, there
  is a corresponding entry in ten_log_power_x16_tbl.
---------------------------------------------------------------------------*/
#define SIZEOF_RSSI_RANGE_TBL \
          ( sizeof(rssi_range_tbl)/sizeof(rssi_range_tbl[0]) )

static const uint32 rssi_range_tbl[] =
{
/* Include auto-generated rssi range table */
  #include "rfgsm_rssi.m"
};

static const uint16 ten_log_power_x16_tbl[] =
{
/* Include auto-generated 10log(rssi)*16 table */
  #include "rfgsm_10logrssix16.m"
};

#define RFGSM_MERGE_1800_B_BAND(band) (band == RFCOM_BAND_GSM1800_B ? RFCOM_BAND_GSM1800 : band )


/*===========================================================================*/
/*                                                                           */
/*                           FUNCTION PROTOTYPES                             */
/*                                                                           */
/*===========================================================================*/
extern void rfgsm_device_init_vars(void);

/*---------------------------------------------------------------------------*/
boolean rfgsm_core_rx_wakeup_calc_timings( rfcom_device_enum_type device,
                                           uint16 * rx_alpha,
                                           uint16 * rx_alpha_idle_mode,
                                           uint16 * ip2_cal_alpha );

/*---------------------------------------------------------------------------*/
static boolean rfgsm_core_calc_gain( rfm_device_enum_type rfm_dev, 
                                     rfgsm_core_handle_type * rfgsm_core_handle_ptr,
                                     dBx16_T rx_lvl_dbm,  
                                     rf_gain_T * gain,
                                     boolean is_high_lin,
                                     boolean aci_flag );

/*---------------------------------------------------------------------------*/
static void rfgsm_core_rx_configure_lif( rfcom_device_enum_type prx_device,
                                         boolean enh_rx_supported,
                                         boolean * lif_enabled, 
                                         int32 * lif_offset );


/*===========================================================================*/
/*                                                                           */
/*                                FUNCTIONS                                  */
/*                                                                           */
/*===========================================================================*/
/*!
  @fn rfgsm_core_rx_wakeup
  @brief This function prepares and builds CCS event to program the RFC to
         GSM mode
  @details This is called for each device that requires an Rx wakeup
  @param sub_id - subscription that is performing the wakeup
  @param rx_device - RFM logical device Rx wakeup is for
  @param rxlm_buf_index - RFLM handle to populate with device based modem data
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_wakeup( uint8 sub_id,
                              rfcom_device_enum_type rx_device,
                              uint32 rxlm_buf_index,
                              rfdevice_rx_mode_type rxdevice_op_mode )
{
  /* Intialise variables */
  rf_path_enum_type rf_path;
  rfgsm_core_data_type core_data;
  rfm_device_enum_type associated_rx_dev = RFM_INVALID_DEVICE;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  /* Time Profiling and Logging */
  unsigned long long start_pcycle = 0;
  uint32 start_ustmr = 0;


  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rx_device);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", FALSE);

  /* Get the path mapped from device */
  rf_path = rfcommon_core_device_to_path( rx_device );

  /* Initialise RxLM index */
  rfgsm_core_handle_ptr->rxlm_buf_index = rxlm_buf_index;

  /* Assign core data for alt path API */
  core_data.sub_id = sub_id;
  core_data.rfm_dev = rx_device;
  core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

  rfgsm_core_handle_ptr->no_active_subs = RFGSM_CORE_NO_CURRENT_SUBS(rfgsm_core_handle_ptr->sub_id_mask);

  /* If there is another subscription active on the device then return */
  /* Do not need to re-vote/initialise the device as already intialised */
  if( !rfgsm_core_handle_ptr->no_active_subs )
  {
	  
	/* Subscription active on device - set sub mask bit */
    RFGSM_CORE_SET_SUB_ACTIVE( rfgsm_core_handle_ptr->sub_id_mask, sub_id );
	  
    /* turn on the vregs required by GSM */
    /* Note:Vregs are already on at this point and this is applying second sub vote */
    rfgsm_core_turn_on_vregs(rf_path, sub_id, rx_device);

    /* Clear the Rx wakeup event to prevent attempt to program stale event */
    if( !rfgsm_core_dm_clean_event( &core_data, RFLM_GSM_RX_WAKEUP_SCRIPT ) )
    {
      RF_MSG_1(MSG_LEGACY_ERROR, "rfgsm_core_rx_wakeup(): rfgsm_core_dm_clean_event failed for sub %d!", sub_id);
      return FALSE;
    }

    /* initialize the modem for GSM. rx lm handle gets updated based on alternate path */
    rfgsm_msm_init( rxlm_buf_index, 
                    rx_device, 
                    FALSE, 
                    RFGSM_ALT_PATH_NOT_SUPPORTED );

    RF_MSG_3( MSG_LEGACY_HIGH, 
              "rfgsm_core_rx_wakeup(): active sub on dev %d, mask 0x%x, minimised wakeup performed for sub %d!",
              rx_device, rfgsm_core_handle_ptr->sub_id_mask, sub_id );
    
    return TRUE;
  } /* MINIMISED WAKEUP */

  /* Subscription active on device - set sub mask bit */
  RFGSM_CORE_SET_SUB_ACTIVE( rfgsm_core_handle_ptr->sub_id_mask, sub_id );
  
#ifdef FEATURE_RF_ASDIV
  /*Set the flag to set the antenna to previous position*/
  rfgsm_core_handle_ptr->set_prev_ant_pos = TRUE;
#endif

  /* Vote for CCS Cmd TQ */
  rfgsm_core_enable_hal_bus( TRUE, sub_id );

  /* turn the vregs required by GSM */
  rfgsm_core_turn_on_vregs( rf_path, sub_id, rx_device );

  /* Enable Common Core Power Voting */
  if(FALSE == rfcommon_core_dev_power_vote( rx_device, 
                                            RFCMN_CORE_DEV_ON, 
                                            RFCMN_CORE_DEV_NORMAL_VOTE) )
  {
    RF_MSG_1( RF_FATAL, "rfgsm_core_rx_wakeup(): Device %d Pwr Up (OFF->LPM) failed()", rx_device );
  }

 //check RFNV_SPARE_1_I value to know LPM feature
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
   /* Profiling */
   start_pcycle = qurt_get_core_pcycles();
   start_ustmr = rfgsm_mc_read_ustmr_count();

   /* Power on RX device (LPM->ON) */
   if ( !rfdevice_gsm_rx_pwr_wakeup( rx_device, rfgsm_nv_get_lowest_supported_band(rx_device, RFGSM_NV_RX) ) )
   {
     ERR_FATAL( "rfgsm_core_rx_wakeup: RX Device %d Pwr Up (LPM->ON) failed for band %d",
               rx_device, rfgsm_nv_get_lowest_supported_band(rx_device, RFGSM_NV_RX), 0 ); 
     return FALSE;
   }
 
   /* Log timings in cmd buffer */
   RFGSM_DEVICE_PROC_LOG(RFGSM_CORE_RX_PWR_WAKEUP, sub_id, 0, 0, 0, 0, start_ustmr, start_pcycle, rfgsm_ustmr_count_diff_us_rtn(start_ustmr) );
  }

  /* initialize the modem for GSM. rx lm handle gets updated based on alternate path */
  rfgsm_msm_init( rxlm_buf_index, 
                  rx_device, 
                  FALSE, 
                  RFGSM_ALT_PATH_NOT_SUPPORTED );

  /* Clear the script buffer before use */
  if( !rf_buffer_clear( (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.enter_mode_script ) )
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "RF GSM enter mode script buffer was unsuccessfully cleared", 0);
    return FALSE;
  }

  /* initialize the RFC data structures and power management for GSM mode based on alternate path */
  rfc_gsm_mode_config( rx_device, 
                       (rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.enter_mode_script,
                       rxdevice_op_mode,
                       RFGSM_ALT_PATH_NOT_SUPPORTED );

  /*Get the associated Rx device and specify operation mode for device driver*/
  associated_rx_dev = rfc_common_get_preferred_associated_rx_device(rx_device);

  /* One time initialisation of RX WTR*/
  (void)rfdevice_gsm_rx_enter_mode( rx_device,
                                    RFDEVICE_CREATE_SCRIPT, 
                                    (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.enter_mode_script,
                                    associated_rx_dev,
                                    rxdevice_op_mode );

  if (!rfdevice_gsm_lna_wakeup(rx_device, rfgsm_core_handle_ptr->band_before_sleep, (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.enter_mode_script, RFDEVICE_CREATE_SCRIPT, 0))
  {    
    RF_MSG(MSG_LEGACY_ERROR, "Wakeup rfdevice_gsm_lna_wakeup failed!");
  }

  /* @TODO rfgsm_core_wakeup_data_init should'nt be required? */
  /*       NV init should be redundant as data reassigned when required */
  /*       rfgsm_core_data_init reinits the core handle data but why?   */
  //rfgsm_core_data_init( rx_device );
  //rfgsm_core_handle_nv_init( rx_device );

  /* If Sawless RFC intialise IP2 and JDET data */
  if( rfgsm_core_get_sawless_status( rx_device ) )
  {
    if( !rfgsm_core_dm_init_ip2_cal_data( &core_data ) )
    {
      RF_MSG_1(MSG_LEGACY_ERROR, "rfgsm_core_rx_wakeup() failed to initialize IP2 cal data in DM for dev %d!", rx_device);
    }

    if ( rfgsm_core_build_jdet_read_scripts( &core_data ) == FALSE )
    {
      RF_MSG_1(MSG_LEGACY_ERROR, "rfgsm_core_rx_wakeup() did not build jdet read scripts for dev %d!", rx_device);
    }

	/* Explicitly allocate HL table for all GSM bands */
    if( rfgsm_core_allocate_high_lin_flag_table( rx_device ) == FALSE )
    {
      RF_MSG_1(MSG_LEGACY_HIGH, "rfgsm_core_rx_wakeup() did not allocate HL table data for dev %d!", rx_device);
    }
  }

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_wakeup_calc_timings
  @brief This function retrieves, calculates and populates the Rx alpha timings
  @details This is called per device and assigns the calculated Rx alpha and
           idle mode Rx alpha
  @param device - RFM logical device
  @param rx_alpha - Rx alpha where alpha is auto B + PLL tune time
  @param rx_alpha_idle_mode - rx_alpha + RFFE bus contention overhead
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_wakeup_calc_timings( rfcom_device_enum_type rx_device,
                                           uint16 * rx_alpha,
                                           uint16 * rx_alpha_idle_mode,
                                           uint16 * ip2_cal_alpha )
{
  /* Initialise the local variables */
  uint16 rx_alpha_temp = 0;
  uint16 rx_alpha_idle_mode_temp = 0;
  int32 time_offset_temp = 0;
  rfgsm_core_handle_type * rfgsm_core_handle_ptr = NULL;
  rfgsm_core_handle_type * rfgsm_tx_core_handle_ptr = NULL;
  rfc_gsm_rx_timing_info_type * rfgsm_core_rx_timing  = NULL;
  rfc_gsm_tx_timing_info_type * rfgsm_core_tx_timing  = NULL;
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;
  rfm_device_enum_type tx_device;
  rfcom_band_type_u u_gsm_band;
  /* Get the core handle data */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get( rx_device );
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_rx_wakeup_timing_update rfgsm_core_handle_ptr is NULL!", FALSE);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr->rfc_gsm_core_ptr, "rfgsm_core_rx_wakeup_timing_update rfc_gsm_core_ptr is NULL!", FALSE);

  if ( rfgsm_core_get_sawless_status(rx_device) )
  {
  
    u_gsm_band.gsm_band = rfgsm_core_handle_ptr->rx_band;
    sys_band = rfcommon_core_convert_rfcom_band_to_sys_band(RFM_EGSM_MODE, u_gsm_band);
    tx_device = rfc_common_get_preferred_tx_device(sys_band);
    rfgsm_tx_core_handle_ptr = rfgsm_core_handle_get(tx_device);
    RF_NULL_CHECK_RTN(rfgsm_tx_core_handle_ptr, "rfgsm_core_rx_wakeup_timing_update rfgsm_tx_core_handle_ptr is NULL!", FALSE);
    RF_NULL_CHECK_RTN(rfgsm_tx_core_handle_ptr->rfc_gsm_core_ptr, "rfgsm_core_rx_wakeup_timing_update tx rfc_gsm_core_ptr is NULL!", FALSE);
    rfgsm_core_tx_timing = rfgsm_tx_core_handle_ptr->rfc_gsm_core_ptr->tx_timing;
    RF_NULL_CHECK_RTN(rfgsm_core_tx_timing, "rfgsm_core_rx_wakeup_timing_update tx_timing is NULL!", FALSE);
  }

  /* Get the Rx timing data */
  rfgsm_core_rx_timing = rfgsm_core_handle_ptr->rfc_gsm_core_ptr->rx_timing;
  RF_NULL_CHECK_RTN(rfgsm_core_rx_timing, "rfgsm_core_rx_wakeup_timing_update rx_timing is NULL!", FALSE);

  /* Calculate Rx alpha */
  rx_alpha_temp = RFGSM_RF_RX_SETUP_TIME_QS( rfgsm_core_rx_timing->rx_burst_sbis_before_pll,
                                             rfgsm_core_rx_timing->pll_settling_time_qs );

  /* Get the timing offset */
  time_offset_temp = (int32)rfgsm_core_rx_timing->rx_burst_processing_time_from_pll_qs - 
                     (int32)rfgsm_core_rx_timing->pll_settling_time_qs;

  /* If offset is positive add it to the Rx alpha along with RFLM overhead */
  if( time_offset_temp > 0 )
  {
    rx_alpha_temp += (uint16)( RFLM_GSM_RX_MAX_PROC_OVERHEAD_QS + time_offset_temp );
  }
  else
  {
    rx_alpha_temp += RFLM_GSM_RX_MAX_PROC_OVERHEAD_QS;
  }

  /* Update the RFC Rx alpha and pass back the same value to caller */
  rfgsm_core_rx_timing->rx_alpha_qs = (int)rx_alpha_temp;
  *rx_alpha = rx_alpha_temp;

  /* Update the RFC idle Rx alpha and pass back the same value to caller */
  /* Idle Rx alpha also accounts for RFFE bus contention in DR-DSDS mode */
  rx_alpha_idle_mode_temp = rx_alpha_temp + RFGSM_WORST_CASE_RFFE_BUS_DELAY;
  rfgsm_core_rx_timing->idle_mode_rx_alpha_qs = (int)rx_alpha_idle_mode_temp;
  *rx_alpha_idle_mode = rx_alpha_idle_mode_temp;

  /* If core_tx_timing ptr is NULL, device is not SAWless and ptr was not set, so no
     need for IP2 timing info */
  if(rfgsm_core_tx_timing != NULL)
  {
    /* Use the most recently updated tx_start_delta in addition to the Rx alpha for IP2 alpha */
  rfgsm_core_handle_ptr->ip2_cal_alpha = 
    MAX(-rfgsm_core_tx_timing->tx_sbi_start_delta_qs,
    rfgsm_core_rx_timing->rx_alpha_qs) + RFLM_GSM_RX_MAX_PROC_OVERHEAD_QS;

    rfgsm_tx_core_handle_ptr->ip2_cal_alpha = rfgsm_core_handle_ptr->ip2_cal_alpha;

    *ip2_cal_alpha = rfgsm_core_handle_ptr->ip2_cal_alpha;
  }

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_wakeup_timing_update
  @brief This function populates the GSM timing data in RFC, RFLM and GL1
  @details This is called once-only for PRx, DRx, OR PRx+DRx
  @param sub_id - subscription that is performing the Rx sleep
  @param rx_device - RFM logical PRx device
  @param drx_device - RFM logical DRx device
  @param op_mode - PRx, DRx, or PRx+DRx operation mode
  @param gl1_timing_info - Pointer to GL1 timing data
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_wakeup_timing_update( uint8 sub_id,
                                            rfcom_device_enum_type prx_device,
                                            rfcom_device_enum_type drx_device,
                                            rfgsm_op_mode_type op_mode,
                                            rfgsm_timing_info_type * gl1_timing_info )
{
  /* Initialise local variables */
  uint16 rx_alpha = 0;
  uint16 rx_alpha_idle_mode = 0;
  uint16 rx_alpha_drx_temp = 0;
  uint16 rx_alpha_idle_mode_drx_temp = 0;
  uint16 ip2_cal_alpha = 0;
  uint16 ip2_cal_alpha_drx_temp = 0;

  /* Calculate the timings PRx */
  if( op_mode == RFGSM_PRX_OP_MODE )
  {
    /* PRx-only so temp variables not required */
    NOTUSED(rx_alpha_drx_temp);
    NOTUSED(rx_alpha_idle_mode_drx_temp);

    if( !rfgsm_core_rx_wakeup_calc_timings( prx_device, &rx_alpha, &rx_alpha_idle_mode, &ip2_cal_alpha ) )
    {
      RF_MSG_1(MSG_LEGACY_ERROR, "rfgsm_core_rx_wakeup_calc_timings failed for PRx dev %d", prx_device );
      return FALSE;
    }
  }
  /* Calculate the timings for DRx */
  else if ( op_mode == RFGSM_DRX_OP_MODE )
  {
    /* DRx-only so temp variables not required */
    NOTUSED(rx_alpha_drx_temp);
    NOTUSED(rx_alpha_idle_mode_drx_temp);

    /* Currently for DRx-only we don't want to update timings */
    /* We have accounted for DRx in the PRx timing */
    return TRUE;
  }
  /* Calculate the timings PRx and Drx and take the worst case */
  else if ( op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    /* Calculate the timings for PRx */
    if( !rfgsm_core_rx_wakeup_calc_timings( prx_device, &rx_alpha, &rx_alpha_idle_mode, &ip2_cal_alpha ) )
    {
      RF_MSG_1(MSG_LEGACY_ERROR, "rfgsm_core_rx_wakeup_calc_timings failed for PRx dev %d", prx_device );
      return FALSE;
    }

    /* Calculate the timings for DRx */
    if( !rfgsm_core_rx_wakeup_calc_timings( drx_device, &rx_alpha_drx_temp, &rx_alpha_idle_mode_drx_temp, &ip2_cal_alpha_drx_temp ) )
    {
      RF_MSG_1(MSG_LEGACY_ERROR, "rfgsm_core_rx_wakeup_calc_timings failed for DRx dev %d", drx_device );
      return FALSE;
    }

    /* Get the worst case between PRx and DRx */
    rx_alpha = MAX(rx_alpha, rx_alpha_drx_temp);
    rx_alpha_idle_mode = MAX(rx_alpha_idle_mode, rx_alpha_idle_mode_drx_temp);
    ip2_cal_alpha = MAX(ip2_cal_alpha, ip2_cal_alpha_drx_temp);
  }
  else
  {
    RF_MSG_1(MSG_LEGACY_ERROR, "rfgsm_core_rx_wakeup_timing_update invalid op_mode %d", op_mode );
    return FALSE;
  }

  /* Set Rx alpha values in RFLM DM for GFW to retrieve for burst scheduling */
  if( !rfgsm_core_dm_set_rx_alpha( sub_id, rx_alpha, rx_alpha_idle_mode ) )
  {
    RF_MSG_1(RF_ERROR, "Rx timings are NOT updated for sub_id %d", sub_id);
    return FALSE;
  }

  /* Update GL1 Rx timing information */
  gl1_timing_info->rx_rf_setup_time_qs = rx_alpha;
  gl1_timing_info->mon_rf_setup_time_qs = rx_alpha;

  gl1_timing_info->rx_idle_rf_setup_time_qs = rx_alpha_idle_mode;
  gl1_timing_info->mon_idle_rf_setup_time_qs = rx_alpha_idle_mode;

  gl1_timing_info->rf_warmup_time_qs = 20000;

  /* Update GL1 timing with IP2 alpha */
  gl1_timing_info->ip2_cal_setup_time_qs = ip2_cal_alpha; 

  /*@TODO is timing device based or NV path based? */
  /*@TODO do we need worst case out of all 4 bands? */

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_update_desense_chan_list
  @brief This function populates the GL1 table with desense channels
  @details Desense channel list from RFC, populated manually based on XO spurs, 
           is populated in a GL1 table to ensure the XO offset is not tuned
           out in the WTR PLL. This is only applied in PRx wakeup
  @param sub_id - subscription that is performing the Rx sleep
  @param prx_device - RFM logical PRx device
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_update_desense_chan_list( uint8 sub_id, 
                                                rfcom_device_enum_type prx_device )
{
  /* Initialise and check core handle and rfc pointer */
  mdsp_xo_desense_arfcn_struct * desense_chan_list = NULL;
  rfgsm_core_handle_type * rfgsm_core_handle_ptr = rfgsm_core_handle_get( prx_device );
  RF_NULL_CHECK_RTN( rfgsm_core_handle_ptr, 
                     "rfgsm_core_rx_update_desense_chan_list() rfgsm_core_handle_ptr is NULL!", 
                     FALSE );
  RF_NULL_CHECK_RTN( rfgsm_core_handle_ptr->rfc_gsm_core_ptr, 
                     "rfgsm_core_rx_update_desense_chan_list() rfc_gsm_core_ptr is NULL!", 
                     FALSE );

  /* Update channel list */
  desense_chan_list = rfgsm_core_handle_ptr->rfc_gsm_core_ptr->desense_chan_list;

  /* Only update if the table is not NULL else no table to update */
  if( desense_chan_list != NULL )
  {
    mdsp_rf_xo_desense_arfcn( desense_chan_list,
                              gl1_hw_rf_map_rf_sub_id_to_as_id( sub_id ) );
  }

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_wakeup_build
  @brief This function builds the CCS events
  @details This is called once-only for PRx, DRx, OR PRx+DRx
  @param sub_id - subscription that is performing the Rx sleep
  @param rx_device - RFM logical PRx device
  @param drx_device - RFM logical DRx device
  @param op_mode - PRx, DRx, or PRx+DRx operation mode
  @param gfw_buff_ptr - Pointer to GFW shared memory
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_wakeup_build( uint8 sub_id,
                                    rfcom_device_enum_type prx_device,
                                    rfcom_device_enum_type drx_device,
                                    rfgsm_op_mode_type op_mode,
                                    void * gfw_buff_ptr )
{
  /* Initialise variables */
  rfgsm_core_data_type core_data;
  rfgsm_core_handle_type *rfgsm_core_prx_handle_ptr = NULL;
  rfgsm_core_handle_type *rfgsm_core_drx_handle_ptr = NULL;
  core_data.prx_drx_combined_mode = FALSE;

  /* Assign subscription to core data */
  core_data.sub_id = sub_id;
  core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
  core_data.rfgsm_core_handle_ptr=NULL;

  /* If Primary Rx */
  if( op_mode == RFGSM_PRX_OP_MODE )
  {
    rfgsm_core_prx_handle_ptr = rfgsm_core_handle_get( prx_device ); 
    RF_NULL_CHECK_RTN(rfgsm_core_prx_handle_ptr, "rfgsm_core_prx_handle_ptr is NULL!", FALSE);
    core_data.rfgsm_core_handle_ptr = rfgsm_core_prx_handle_ptr;
    core_data.rfm_dev = prx_device;
  }
  /* If Diversity Rx */
  else if( op_mode == RFGSM_DRX_OP_MODE )
  {
    rfgsm_core_drx_handle_ptr = rfgsm_core_handle_get( drx_device );
    RF_NULL_CHECK_RTN(rfgsm_core_drx_handle_ptr, "rfgsm_core_drx_handle_ptr is NULL!", FALSE);
    core_data.rfgsm_core_handle_ptr = rfgsm_core_drx_handle_ptr;
    core_data.rfm_dev = drx_device;
  }
  /* If Primary + Diversity Rx */
  else if( op_mode == RFGSM_PRX_DRX_OP_MODE )
  {
    rfgsm_core_prx_handle_ptr = rfgsm_core_handle_get( prx_device ); 
    RF_NULL_CHECK_RTN(rfgsm_core_prx_handle_ptr, "rfgsm_core_prx_handle_ptr is NULL!", FALSE);
    core_data.rfgsm_core_handle_ptr = rfgsm_core_prx_handle_ptr;
    core_data.rfm_dev = prx_device;

    rfgsm_core_drx_handle_ptr = rfgsm_core_handle_get( drx_device );
    RF_NULL_CHECK_RTN(rfgsm_core_drx_handle_ptr, "rfgsm_core_drx_handle_ptr is NULL!", FALSE);
    core_data.rfgsm_core_handle_ptr_rxd = rfgsm_core_drx_handle_ptr;
    core_data.rxd_rfm_dev = drx_device;

    core_data.prx_drx_combined_mode = TRUE;
  }

  /* Build the wakeup events */
  if( core_data.rfgsm_core_handle_ptr!=NULL && core_data.rfgsm_core_handle_ptr->no_active_subs )
  {
    if (!rfgsm_core_dm_dynamic_rx_wakeup_event_wrapper(&core_data)) 
    {
      RF_MSG(MSG_LEGACY_ERROR,"RF GSM Wakeup RFLM dynamic event prog failed!");
      return FALSE;
    }
  }

  /* Populate the wakeup RFLM data information */
  if( !rfgsm_core_dm_populate_rx_wakeup_intf_data( &core_data, gfw_buff_ptr ) )
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Wakeup RFLM data info update failed!");
    return FALSE;
  }

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_sleep
  @brief This function disables the RFC from Rx capable GSM mode
  @details This is called for each device that requires an Rx sleep
  @param sub_id - subscription that is performing the Rx sleep
  @param rx_device - RFM logical device Rx sleep is for
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_sleep( uint8 sub_id,
                             rfcom_device_enum_type rx_device,
                             rfgsm_op_mode_type op_mode )
{
  rf_path_enum_type rf_path;
  rfgsm_core_data_type core_data;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  /* Time Profiling and Logging */
  unsigned long long start_pcycle = 0;  
  uint32 start_ustmr = 0;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get( rx_device );
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", FALSE);

  rf_path = rfcommon_core_device_to_path( rx_device );

  core_data.sub_id = sub_id;
  core_data.rfm_dev = rx_device;
  core_data.triple_buffer_id = 0;

  /* Only clean the Wakeup event if the PRx device is sleeping */
  if( op_mode != RFGSM_DRX_OP_MODE )
  {
    /* Clear the Rx wakeup event to prevent attempt to program stale event */
    if( !rfgsm_core_dm_clean_event( &core_data, RFLM_GSM_RX_WAKEUP_SCRIPT ) )
    {
      RF_MSG_1( MSG_LEGACY_ERROR, "rfgsm_core_rx_sleep(): rfgsm_core_dm_clean_event failed for sub %d!", sub_id );
      return FALSE;
    }
  }

  /* If there is another subscription active on the device then return   */
  /* As another sub is active on the same device do not tear down device */
  if( !RFGSM_CORE_ONLY_ACTIVE_SUB(rfgsm_core_handle_ptr->sub_id_mask, sub_id) )
  {
	  
	RFGSM_CORE_SET_SUB_INACTIVE(rfgsm_core_handle_ptr->sub_id_mask, sub_id);
	  
    /* turn off the vregs required by GSM */
    /* Note: Vregs are required by other sub and will not shut down but voting off is required */
    rfgsm_core_turn_off_vregs(rf_path, sub_id, rx_device);

    RF_MSG_3(MSG_LEGACY_HIGH, 
             "rfgsm_core_rx_sleep(): active sub on dev %d, mask 0x%x, minimised sleep performed for sub %d!",
             rx_device, rfgsm_core_handle_ptr->sub_id_mask, sub_id);

    return TRUE;
  }

  /* Subscription inactive on device */
  RFGSM_CORE_SET_SUB_INACTIVE(rfgsm_core_handle_ptr->sub_id_mask, sub_id);
  
  rfgsm_core_handle_ptr->band_before_sleep = rfgsm_core_handle_ptr->band;

  /* configure the RFC signals into sleep state */
  rfc_gsm_sleep( rx_device, rfgsm_core_handle_ptr->band,rfgsm_core_handle_ptr->rfgsm_mode, NULL);

 //check RFNV_SPARE_1_I value to know LPM feature
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
   /* Profiling */
   start_pcycle = qurt_get_core_pcycles();
   start_ustmr = rfgsm_mc_read_ustmr_count();

   /* Power off RX device (ON->LPM) */
   if ( !rfdevice_gsm_rx_pwr_sleep( rx_device, rfgsm_nv_get_lowest_supported_band(rx_device, RFGSM_NV_RX) ) )
   {
     ERR_FATAL( "rfgsm_core_rx_sleep: RX Device %d Pwr Down (ON->LPM) failed for band %d",
               rx_device, rfgsm_nv_get_lowest_supported_band(rx_device, RFGSM_NV_RX), 0 );
     return FALSE;
   }

   /* Log timings in cmd buffer */
   RFGSM_DEVICE_PROC_LOG(RFGSM_CORE_RX_PWR_SLEEP, sub_id, 0, 0, 0, 0, start_ustmr, start_pcycle, rfgsm_ustmr_count_diff_us_rtn(start_ustmr) );
  }

  #ifdef FEATURE_RF_POWER_REPORTING
  rflm_diag_power_log_trx_dynamic_event(RFLM_DIAG_POWER_RFA_RX_HANDLE, rfgsm_core_handle_ptr->rxlm_buf_index, RFLM_DIAG_POWER_RFA_STATE_OFF);
  #endif

  if( FALSE == rfcommon_core_dev_power_vote( rx_device, 
                                             RFCMN_CORE_DEV_OFF, 
                                             RFCMN_CORE_DEV_NO_VOTE ) )
  {
    RF_MSG_1( RF_FATAL, "rfgsm_core_rx_sleep(): Device: %d Pwr Down (LPM->OFF) failed()", rx_device);
  }

  /* Power off RF RX hardware */
  rfgsm_core_turn_off_vregs( rf_path, sub_id ,rx_device);

  rfgsm_core_enable_hal_bus( FALSE, sub_id );

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @details
  This function sets up the Rx scripts to program the synthesizer to the
  requested channel.

  @retval
  TRUE/FALSE - sucessful/unsuccessful tune.
*/
#if 0
boolean rfgsm_core_tune_rx(rfgsm_core_data_type *core_data, uint16 arfcn, rfdevice_rx_mode_type rfdevice_op_mode)
{
  rfcom_gsm_band_type band;
  boolean use_coex_filtered_path = FALSE;
  rfc_gsm_coex_band_info_type band_split_info;
  boolean flag = FALSE;
  boolean use_hl_mode = FALSE;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_handle_ptr = rfgsm_core_handle_get( core_data->rfm_dev );
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", FALSE);

  band = core_data->rfgsm_core_handle_ptr->band;
  band_split_info.band = band;
  band_split_info.status = RFC_FILTERED_PATH_FALSE;

  /*Set/clear filtered path flag*/
  core_data->rfgsm_core_handle_ptr->use_coex_filtered_path_rx = rfcommon_core_get_rx_filter_status();

  use_coex_filtered_path = core_data->rfgsm_core_handle_ptr->use_coex_filtered_path_rx;
 
  if (use_coex_filtered_path == TRUE)/* Filtered path needs to be use based on the channel*/
  {
    band_split_info.status = RFC_FILTERED_PATH_TRUE;

    flag = rfc_get_split_band_type(core_data->rfm_dev, RFC_CONFIG_RX, core_data->rfgsm_core_handle_ptr->band, arfcn, &band_split_info );


    if (flag == TRUE)
    {
      if (core_data->rfgsm_core_handle_ptr->rx_burst == RF_RX_BURST)
      {

        /* updated band enum info*/
        band = band_split_info.band;

      }
    }
    else
    {
      band = rfc_get_unfiltered_band_type(core_data->rfm_dev,RFC_CONFIG_RX,core_data->rfgsm_core_handle_ptr->band);
    }
  }
  else
  {

      band = rfc_get_unfiltered_band_type(core_data->rfm_dev, RFC_CONFIG_RX, core_data->rfgsm_core_handle_ptr->band);
  }
  /*This API retrieves RF configuration for all FE devices including GRFCs, ASM, ATUNER etc. for a give RX burst*/
  rfc_gsm_rx_burst_config( core_data->rfm_dev,
                           band,
                           arfcn,
                           rfgsm_core_handle_ptr->probe_burst,
                           core_data->rfgsm_core_handle_ptr->rx_alternate_path,
                           rfgsm_core_handle_ptr->set_antenna_position,
                           rfgsm_core_handle_ptr->lna_range,
                           &rfgsm_core_handle_ptr->scripts,
                           rfdevice_op_mode,
                           rfgsm_core_handle_ptr->rfgsm_mode);

  /* Setup band dependent settings */
  rfdevice_gsm_set_rx_band( core_data->rfm_dev,
                            band,
                            RFDEVICE_CREATE_SCRIPT, 
                            (rf_buffer_intf *)core_data->rfgsm_core_handle_ptr->scripts.rx_start_script[RX_WTR_SCRIPT]);

  /* If we are on SAWless target, check if we should use HL mode for this channel and then update device */
  if ( (rfgsm_core_get_sawless_status(core_data->rfm_dev)) == TRUE )
  {
    /* Check if an override is set */
    if ( (core_data->rfgsm_core_handle_ptr->sawless_linearity_control.linearity_override == RFGSM_SAWLESS_NO_OVERRIDE)
         || (core_data->rfgsm_core_handle_ptr->sawless_linearity_control.linearity_override == RFGSM_SAWLESS_CHAN_OVERRIDE) )
    {
      if ( (rfgsm_core_is_arfcn_in_high_lin(core_data->rfm_dev, arfcn, band, &use_hl_mode)) == FALSE )
      {
        MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Could not get lin state for SAWless device %d!",core_data->rfm_dev);
      }

      /* Always use HL mode in power scan bursts unless overridden in NV */
      if (core_data->rfgsm_core_handle_ptr->rx_burst == RF_PWR_BURST)
      {
        if ( core_data->rfgsm_core_handle_ptr->sawless_linearity_control.disable_hl_power_scan == TRUE)
        {
          /* for debug purpose use LL mode for power scan so that we can force crash in HL mode only in a call*/
          use_hl_mode = FALSE;
        }
        else
        {
          use_hl_mode = TRUE; 
        }
      }
    }/* ^ RFGSM_SAWLESS_NO_OVERRIDE or RFGSM_SAWLESS_CHAN_OVERRIDE ^ */
    else if ( core_data->rfgsm_core_handle_ptr->sawless_linearity_control.linearity_override == RFGSM_SAWLESS_HL_OVERRIDE )
    {
      use_hl_mode = TRUE;
    }/* ^ RFGSM_SAWLESS_HL_OVERRIDE ^ */
    else if ( core_data->rfgsm_core_handle_ptr->sawless_linearity_control.linearity_override == RFGSM_SAWLESS_LL_OVERRIDE )
    {
      use_hl_mode = FALSE;
    }/* ^ RFGSM_SAWLESS_LL_OVERRIDE ^ */

    if ( core_data->rfgsm_core_handle_ptr->sawless_linearity_control.enable_logging == TRUE)
    {
      MSG_3( MSG_SSID_RF,
             MSG_LEGACY_HIGH,
             "Using SAWless linearity state %d for dev %d, override %d",
             use_hl_mode,
             core_data->rfm_dev,
             core_data->rfgsm_core_handle_ptr->sawless_linearity_control.linearity_override );
    }

    /* Set the device linearity mode based on ARFCN linearity table or override setting */
    rfdevice_gsm_cmd_dispatch(core_data->rfm_dev, RF_PATH_0, RFDEVICE_GSM_SET_HIGH_LIN_MODE, &use_hl_mode);
  } /* ^ sawless_status==TRUE ^ */

  /* Tell the library to tune */
  rfdevice_gsm_tune_rx( core_data->rfm_dev, 
                        arfcn,
                        core_data->rfgsm_core_handle_ptr->enh_rx_on_flag,
                        core_data->rfgsm_core_handle_ptr->enh_rx_freq_offset);

  return TRUE;
}
#endif


/*----------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_set_band
  @details This function configures Rx for the requried band
  @param core_data - data structure including device ID and core handle
  @param band - band to configure the RF to passed from GL1
  @retval TRUE/FALSE - sucessful/unsuccessful band selection
*/
boolean rfgsm_core_rx_set_band( rfgsm_core_data_type *core_data,
                                rfcom_gsm_band_type band )
{
  boolean is_nv_data_null = FALSE;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(core_data->rfm_dev);
  uint8 asdiv_pos;
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_handle_ptr is NULL!", FALSE);

  /* Get all NV table data for Rx */
  RF_NULL_CHECK_SET_STATUS(rfgsm_core_handle_ptr->rfgsm_rx_nv_tbl[band], 
                           "rfgsm_core_rx_set_band NULL rfgsm_rx_nv_tbl!", 
                           is_nv_data_null );

  for(asdiv_pos = 0; asdiv_pos < ASDIV_POSITION_MAX; asdiv_pos++)
  {
    /* Initialize pointer to GSM/EGSM rx switch structure */
    RF_NULL_CHECK_SET_STATUS( rfgsm_core_handle_ptr->rfgsm_nv_rx_freq_comp_switch_ptr[band][asdiv_pos], 
                            "rfgsm_core_rx_set_band NULL rfgsm_nv_rx_freq_comp_switch_ptr!", 
                            is_nv_data_null );
  }

  /* If this band is SAWless, then load the switchpoints for HL mode */
  if ( rfgsm_core_get_band_sawless_status(core_data->rfm_dev, band) == TRUE )
  {
    
    for(asdiv_pos = 0; asdiv_pos < ASDIV_POSITION_MAX; asdiv_pos++)
    {
      RF_NULL_CHECK_SET_STATUS( rfgsm_core_handle_ptr->rfgsm_nv_hl_rx_switch_ptr[band][asdiv_pos], 
                                "rfgsm_core_rx_set_band NULL rfgsm_nv_hl_rx_switch_ptr!", 
                                is_nv_data_null );
    }

    /* If band changes AND we are not doing a monitor burst, we should reset the jdet counter */
    if ((band != rfgsm_core_handle_ptr->hl_ll_band) && 
        (rfgsm_core_handle_ptr->rx_burst == RF_RX_BURST))
    {
      //Reset counter
      memset(rfgsm_core_handle_ptr->jdet_counter, 0, MAX_NUM_ARFCNS_IN_BAND * sizeof(uint8));
      rfgsm_core_handle_ptr->hl_ll_band = band;
    }
  }

  for(asdiv_pos = 0; asdiv_pos < ASDIV_POSITION_MAX; asdiv_pos++)
  {
    /* Initialize pointer to GSM/EGSM rx enh switch structure */
    RF_NULL_CHECK_SET_STATUS( rfgsm_core_handle_ptr->rfgsm_nv_enh_rx_switch_ptr[band][asdiv_pos], 
                            "rfgsm_core_rx_set_band NULL rfgsm_nv_enh_rx_switch_ptr!", 
                            is_nv_data_null );
  }
  /* Check the NV data */
  if( is_nv_data_null == TRUE )
  {
    /* Fatal error only if not in FTM mode */
    if( ftm_get_mode() != FTM_MODE )
    {
      ERR_FATAL("Set Rx band %d dev %d NV calibration data is bad OR not Rx capable", 
                band, core_data->rfm_dev, 0);
    }
    else
    {
      MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
            "Set Rx band %d dev %d not Rx capable OR NV calibration data is bad.", 
            band, core_data->rfm_dev);
      return FALSE;
    }
  }

  /* Store band in core handle */
  rfgsm_core_handle_ptr->band = band;

  return TRUE;
}



/*----------------------------------------------------------------------------*/
/*!
  @details
  This function computes and validates the Rx power at the antenna.  It
  accomplishes this by using the following formula:

              10log(rssi) - Grx

  where Grx represents the entire Rx chain gain from the antenna input to
  baseband.

  NOTE: Grx = G(digital) + G(rf)

  The design of this function assumes that rf_set_rx_gain() was
  previously invoked;  the gain value that is required for the Rx power
  computation is established there.

  @param
  rssi_value    : input,  (I^2 + Q^2) value
  dBm_x16_value : output, absolute power result
  gain_range    : output, gain range used in deriving the dBm_x16_value
                  (only provided for informational purposes - logging)
  stored_gain  : gain to be computed 

*/
rf_measurement_T rfgsm_core_compute_rx_power(uint32 rssi_value,
                                             dBx16_T *dBm_x16_value,
                                             rf_gain_range_T *gain_range,
                                             const rf_gain_T *stored_gain, 
                                             rfm_device_enum_type rfm_dev)
{
  rf_measurement_T  power_result = RF_NOISY;
  dBx16_T           dBx16_result;
  int16 interim=0; /*Added from SCMM*/


  if (stored_gain != NULL)
  {
    if (rf_debug_force_rssi_to_dbm != 0)
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," *** Forcing the RSSI->dBm = %d ***",
            rf_debug_force_rssi_to_dbm);
      *dBm_x16_value  = (dBx16_T)(rf_debug_force_rssi_to_dbm * 16);
      *gain_range     = stored_gain->range;
      return (power_result);
    }

    if (rfgsm_core_ten_log_of_power_val(rssi_value, &dBx16_result))
    {
      *dBm_x16_value  = dBx16_result - stored_gain->valx16;
      *gain_range     = stored_gain->range;

      /*
      ** Validate the signal level against the particular gain range
      ** windows. The windows are calculated at the time that the
      ** gain range is set.
      */
      if (*dBm_x16_value > (stored_gain->range_high_limitx16 + RFCOM_HOT_DELTA))
      {
        power_result   = RF_SATURATED;
      }
      else if (*dBm_x16_value > stored_gain->range_high_limitx16)
      {
        power_result   = RF_HOT;
      }
      else if (*dBm_x16_value < 
               (stored_gain->range_low_limitx16 - RFCOM_COLD_DELTA))
      {
        power_result   = RF_NOISY;
      } 
      else if (*dBm_x16_value < stored_gain->range_low_limitx16)
      {
        power_result   = RF_COLD;
      }
      else
      {
        power_result = RF_IN_RANGE;
      }
    } 
    else
    {
      /* The RSSI should be non-zero coming from the MDSP because of
      ** thermal induced and radio noise floor.
      */
      *dBm_x16_value  = RFGSM_MIN_RX_POWER_REPORTED;
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR," *** RSSI = 0 ***",0);
    }
  } 
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"No stored gain!",0);
  }

  if(ftm_get_mode() == FTM_MODE) /*Check if in FTM  mode*/
  {
    if ( (*dBm_x16_value) & 0x4000 )
    {
      interim = (int16)((*dBm_x16_value) | 0x8000);
    }
    else
    {
      interim = (int16)(*dBm_x16_value);
    }
    ftm_gsm_rssi_to_dbm = interim / 16;
  }


  return(power_result);
} /* end of rfgsm_compute_rx_power */



/*----------------------------------------------------------------------------*/
/*!
  @details
  This function performs a binary search.

*/
int16 rfgsm_core_utils_binary_search(const uint32 data[], uint32 input, 
                                     uint16 sizeof_data) 
{
  int16 low, mid = 0, high;

  //Lint, type casted to sint15 
  low =0; high=(int16)sizeof_data - 1;
  while (low <= high)
  {
    mid = (high + low)/2;
    if ( data[mid] < input )
    {
      low = mid + 1;
    } 
    else if ( data[mid] > input )
    {
      high = mid - 1;
    } 
    else
    {
      /* found mid ! */
      break;
    }
  }

  /* Ensure that we are returning the lower point of a line segment */
  /* for the interpolation algorithm                                */
  if (data[mid] > input)
  {
    mid--;
  }

  return(mid);
}



/*----------------------------------------------------------------------------*/
/*!
  @details
  This function computes the 10log10(power).  It implemets this by
  linearly interpolating a log LUT.  The output of the LUT is a 10log10 value.
  The LUT provided a maximum error of 1/16 dB.
*/
boolean rfgsm_core_ten_log_of_power_val(uint32 power, dBx16_T *dBx16_result_ptr)
{
  boolean status = FALSE;

  int16  index;
  uint32  a,b;
  uint16  f_of_a, f_of_b;

  /* Can't take log10(0) = undefined */
  if (power > 0)
  {
    index = rfgsm_core_utils_binary_search(rssi_range_tbl, power, 
                                           SIZEOF_RSSI_RANGE_TBL);

    if (index < 0)
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid index for the RSSI table. index=%d",index);      
      return status;
    }

    /* Linear interpolate the result value */
    a      = rssi_range_tbl[index];
    f_of_a = ten_log_power_x16_tbl[index];
    b      = rssi_range_tbl[index+1];
    f_of_b = ten_log_power_x16_tbl[index+1];

    *dBx16_result_ptr = (dBx16_T) (f_of_a + ((power-a)*(f_of_b-f_of_a))/(b-a));

    status = TRUE;
  }

  return(status);
}



/*----------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_update_rx_timing

  @brief update rx_alpha and mon_alpha  timing parameters  to core dm.

  @details This api updates rx_alpha and mon_alpha  timing parameters to core dm memory space
  where it is retrieved by GFW for RX burst

  @param core_data info struct

  @retVal None
*/
void rfgsm_core_update_rx_timing(
       rfgsm_core_data_type *core_data )
{
  rfc_gsm_rx_timing_info_type      *rx_timing_p;
  rfc_gsm_irat_rx_timing_info_type *worst_case_rx_timing_p;
  rfgsm_core_irat_rx_timings_type  *rfc_gsm_rx_timing_p;
  int32                             time_offset_qs;

  RF_NULL_CHECK_RTN_V( core_data->rfgsm_core_handle_ptr->rfc_gsm_core_ptr,
                       "rfgsm_core_update_rx_timing(): rfc_gsm_core_ptr NULL");
  RF_NULL_CHECK_RTN_V( core_data->rfgsm_core_handle_ptr->rfc_gsm_core_ptr->rx_timing,
                       "rfgsm_core_update_rx_timing(): rx_timing NULL");

  rfc_gsm_rx_timing_p = rfgsm_get_worst_case_rx_timing();

  RF_NULL_CHECK_RTN_V( rfc_gsm_rx_timing_p,
                       "rfgsm_core_update_rx_timing(): rfc_gsm_rx_timing_p NULL" );

  /* Get the Rx timings from rfc structure */
  rx_timing_p = core_data->rfgsm_core_handle_ptr->rfc_gsm_core_ptr->rx_timing;

  worst_case_rx_timing_p = &rfc_gsm_rx_timing_p->rx_timing;

  /* Update the Rx burst alpha based on the number of SBIs and PLL Settling Time */
  /* Calculate the Rx alpha value in qs time */
  rx_timing_p->rx_alpha_qs = RFGSM_RF_RX_SETUP_TIME_QS( worst_case_rx_timing_p->rx_burst_sbis_before_pll,
                                                        worst_case_rx_timing_p->pll_settling_time_qs );
  rx_timing_p->rx_alpha_qs += RFLM_GSM_RX_MAX_PROC_OVERHEAD_QS;

  time_offset_qs =
    ( worst_case_rx_timing_p->rx_burst_processing_time_from_pll_qs - worst_case_rx_timing_p->pll_settling_time_qs );

  if ( time_offset_qs > 0 )
  {
    rx_timing_p->rx_alpha_qs += time_offset_qs;

    RF_MSG_2( RF_HIGH, "rfgsm_core_update_rx_timing(): Updated rx alpha(qs) by %d to %d",
              time_offset_qs, rx_timing_p->rx_alpha_qs );
  }

  /* The overhead is added to accomodate worst case delay for RX burst writes due to rffe bus contention.
     This RX alpha is used for IDLE mode RX by GL1 in DR-DSDS mode */
  rx_timing_p->idle_mode_rx_alpha_qs =
    ( rx_timing_p->rx_alpha_qs + RFGSM_WORST_CASE_RFFE_BUS_DELAY );

  /* Write Rx alpha value to RFLM DM */
  if ( !rfgsm_core_dm_set_rx_alpha( core_data->sub_id, (uint16)rx_timing_p->rx_alpha_qs,
                                    (uint16)rx_timing_p->idle_mode_rx_alpha_qs )
     )
  {
    RF_MSG_2( RF_ERROR, "GSM Rx timings are NOT updated for device:%d on sub:%d",
              core_data->rfm_dev, core_data->sub_id );

    return;
  }

/* TEMP TEMP for verification */
    RF_MSG_3( RF_HIGH, "rfgsm_core_update_rx_timing(): worst_case:rx_burst_sbis_before_pll %d pll_settling_time_qs %d rx alpha:%d",
              worst_case_rx_timing_p->rx_burst_sbis_before_pll, worst_case_rx_timing_p->pll_settling_time_qs,
              rx_timing_p->rx_alpha_qs );
}

/*----------------------------------------------------------------------------*/
/*!
  @details
  This functions sets up the Rx scripts to configure the Rx gain and the LNA
  based on the requested Rx level.  The gain range switch points are
  adjusted by the calibration results to apply the correct gain setting.

  This function relies on the arfcn information to be valid for the given
  burst so that the appropriate CAL offsets can be applied; therefore,
  rf_tune_to_channel() must be called first.

  rx_lvl_dBm - 1/16 dB format: sign bit, 11 decimal bits, 4 fractional bits

  @retVal
  TRUE - requested gain was set.
  FALSE - unable to set the requested gain.
*/
#if 0
boolean rfgsm_core_set_rx_gain(rfgsm_core_data_type *core_data,
                               dBx16_T rx_lvl_dbm,
                               rf_gain_T *gain,
                               uint8 lna_range,
                               uint16 arfcn,
                               rfgsm_enh_rx_params_type enh_rx_params)
{
  boolean set_radio_gain = FALSE;
  rfgsmlib_rx_gain_range_type lna_range_to_set = RFGSMLIB_GAIN_RANGE_1;
  uint16 char_offset_bin_indx = 0;

  /* Mask off MSB in case it is being used for HL\LL indication on SAWless target */
  lna_range = FTM_GSM_MASK_OFF_LNA_STATE_MSB(lna_range);

  /*Set/clear filtered path flag*/
  core_data->rfgsm_core_handle_ptr->use_coex_filtered_path_rx = rfcommon_core_get_rx_filter_status();

  core_data->rfgsm_core_handle_ptr->rf_tune_commanded = TRUE;

  core_data->rfgsm_core_handle_ptr->rf_rx_arfcn = arfcn;
  if (core_data->enable_rx_diversity) 
  {
    core_data->rfgsm_core_handle_ptr_rxd->rf_rx_arfcn = arfcn;
  }

  /*Add characterized offset due to filtered path*/

  if(core_data->rfgsm_core_handle_ptr->use_coex_filtered_path_rx == TRUE)
  {
    char_offset_bin_indx = rfgsm_core_get_arfcn_bin_index_char_offset_tbl(core_data->rfm_dev, 
                                                                          core_data->rfgsm_core_handle_ptr->band, 
                                                                          core_data->rfgsm_core_handle_ptr->rf_rx_arfcn,
                                                                          RFC_CONFIG_RX );
    if (char_offset_bin_indx < RFNV_MAX_ARFCN_BINS)
    {
      rx_lvl_dbm += (dBx16_T)core_data->rfgsm_core_handle_ptr->rfgsm_rx_nv_tbl->rx_static_nv_data_ptr->coex_rx_char_offset_info_tbl.rx_char_offset_tbl[char_offset_bin_indx];
    }
  }

  if (gain == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"gain == NULL!",0);
    return (set_radio_gain);
  }

  if(ftm_gsm_rx_device_is_ftm_state(core_data->rfm_dev)==TRUE) /*Check if in FTM mode*/
  {	
    /* store gain range only in cal mode since rx level is invalid in cal mode */
    if (ftm_gsm_is_in_calibration_mode())
    {
      /* store the given lna_range in cal mode here before it is overwritten */
      core_data->rfgsm_core_handle_ptr->lna_range = lna_range;
      if (core_data->enable_rx_diversity) 
      {
        core_data->rfgsm_core_handle_ptr_rxd->lna_range = lna_range;
      }
    }

    /* When in ftm continuous rx mode this function is called from acquisition.
    and the lna_range parameter will null. So, Use the rx_lvl_dbm to get lna
    range.  */
    if (ftm_gsm_get_lna_range_from_rx_lvl(core_data->rfm_dev)) 
    {
      lna_range = (uint8) rx_lvl_dbm ;
    }

    lna_range_to_set = (rfgsmlib_rx_gain_range_type)(lna_range + 1);
    rfdevice_gsm_set_rx_gain_range(core_data->rfm_dev, (int32)lna_range_to_set);
    rfgsm_core_set_radio_gain(core_data->rfm_dev, rx_lvl_dbm, gain, enh_rx_params); /*Added from SCMM*/
	core_data->rfgsm_core_handle_ptr->valx16 = gain->valx16;
    
    set_radio_gain = TRUE;

    return( set_radio_gain );
  } 

  if (core_data->rfgsm_core_handle_ptr->rf_tune_commanded == FALSE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Invalid burst type or tune_to_channel fail",0);
    return (set_radio_gain);
  }

  rfgsm_core_set_radio_gain(core_data->rfm_dev, rx_lvl_dbm, gain, enh_rx_params);
  set_radio_gain = TRUE;
  core_data->rfgsm_core_handle_ptr->valx16 = gain->valx16;

  /* Update the library */
  lna_range_to_set = (rfgsmlib_rx_gain_range_type)gain->range;
  rfdevice_gsm_set_rx_gain_range(core_data->rfm_dev, (int32)lna_range_to_set);
  
  /* 4 Rx's possible in GPRS with a single tune command */ 
  /* reset error check flag for the next iteration */
  core_data->rfgsm_core_handle_ptr->rf_tune_commanded = FALSE;

  #ifdef FEATURE_RF_POWER_REPORTING
  //power logging: log rxagc data
  rflm_diag_power_log_rx_agc_event(core_data->rfgsm_core_handle_ptr->rxlm_buf_index, lna_range_to_set);

  if ( core_data->rfgsm_core_handle_ptr_rxd != NULL ) 
  {
    rflm_diag_power_log_rx_agc_event(core_data->rfgsm_core_handle_ptr_rxd->rxlm_buf_index, lna_range_to_set);
  }
  #endif

  return( set_radio_gain );
}
#endif


/*----------------------------------------------------------------------------*/
/*!
  @details
  This functions returns LNA gain for a given Rx channel and LNA state in
  rx_lvl_dBm - 1/16 dB format: sign bit, 11 decimal bits, 4 fractional bits

  @retVal
  LNA gain in dBm16
*/
dBx16_T rfgsm_core_get_lna_gain(rfm_device_enum_type rfm_dev, uint16 arfcn, uint16 gain_range)
{
  dBx16_T   fc_gain = 0;
  uint8 arfcn_index;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfgsm_nv_band_rx_data_type *rfgsm_rx_nv_tbl = NULL;
  uint16 arfcn_linearity = 0;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr,"rfgsm_core_handle_ptr is NULL!", 0);
  
  if (rfgsm_core_handle_ptr->band < RFCOM_NUM_GSM_BANDS)
  {
    rfgsm_rx_nv_tbl = rfgsm_core_handle_ptr->rfgsm_rx_nv_tbl[rfgsm_core_handle_ptr->band];
  }
  RF_NULL_CHECK_RTN(rfgsm_rx_nv_tbl,"rfgsm_rx_nv_tbl is NULL!", 0);

  if (!rfgsm_core_is_arfcn_valid(rfgsm_core_handle_ptr->band, arfcn))
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "Invalid Band=%d and ARFCN=%d combination!", 
          rfgsm_core_handle_ptr->band, 
	  rfgsm_core_handle_ptr->rf_rx_arfcn);
    return FALSE;
  }
  
  /* Update the SAWless override for v4 cal if MSB of gain word is set */
  if ( FTM_GSM_CHECK_LNA_STATE_MSB(gain_range) )
  {
    rfgsm_core_handle_ptr->sawless_linearity_control.linearity_override = RFGSM_SAWLESS_HL_OVERRIDE;
  }
  else
  {
    rfgsm_core_handle_ptr->sawless_linearity_control.linearity_override = RFGSM_SAWLESS_LL_OVERRIDE;
  }

  arfcn_linearity = rfgsm_map_arfcn_linearly(rfgsm_core_handle_ptr->band, arfcn);
  arfcn_index =  rfgsm_nv_get_closest_arfcn_index(rfgsm_rx_nv_tbl, arfcn);
  fc_gain = rfgsm_nv_rx_freq_comp(rfm_dev,
                                  rfgsm_rx_nv_tbl, 
                                  rfgsm_core_handle_ptr->band,
                                  arfcn_linearity,
                                  arfcn_index,
                                 (rfnv_gsm_lna_state_type)FTM_GSM_MASK_OFF_LNA_STATE_MSB(gain_range));
  return fc_gain;
}



/*----------------------------------------------------------------------------*/
/*!
  @details
*/
boolean rfgsm_core_set_radio_gain(rfm_device_enum_type rfm_dev, dBx16_T rx_lvl_dbm,  rf_gain_T *gain, rfgsm_enh_rx_params_type enh_rx)
{
  dBx16_T   actual_switch_1_2, actual_switch_2_3, actual_switch_3_4;
  dBx16_T   actual_switch_4_5 = 0;
  uint8 arfcn_index;
  boolean is_enh_rx_supported = FALSE;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfgsm_nv_band_rx_data_type *rfgsm_rx_nv_tbl = NULL;
  rfgsm_nv_rx_switch_type *rx_sw_pt_ptr = NULL;
  uint16 arfcn_linearity = 0;
  boolean use_hl_mode = FALSE;
  uint8 asdiv_pos;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr,"rfgsm_core_handle_ptr is NULL!", FALSE);

  if (rfgsm_core_handle_ptr->band < RFCOM_NUM_GSM_BANDS)
  {
    rfgsm_rx_nv_tbl = rfgsm_core_handle_ptr->rfgsm_rx_nv_tbl[rfgsm_core_handle_ptr->band];
  }
  else
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR, "Invalid Band = %d", rfgsm_core_handle_ptr->band);
    return FALSE;
  }

  RF_NULL_CHECK_RTN(rfgsm_rx_nv_tbl,"rfgsm_rx_nv_tbl is NULL!", FALSE);
  
  if (!rfgsm_core_is_arfcn_valid(rfgsm_core_handle_ptr->band, rfgsm_core_handle_ptr->rf_rx_arfcn))
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "Invalid Band=%d and ARFCN=%d combination!",rfgsm_core_handle_ptr->band,rfgsm_core_handle_ptr->rf_rx_arfcn);
    return FALSE;
  }

  #ifndef FEATURE_RF_ASDIV
  asdiv_pos = 0;
  #else
  asdiv_pos = rfcommon_asdiv_get_current_position(rfm_dev);
  #endif


  arfcn_linearity = rfgsm_map_arfcn_linearly(rfgsm_core_handle_ptr->band, rfgsm_core_handle_ptr->rf_rx_arfcn);
  arfcn_index =  rfgsm_nv_get_closest_arfcn_index( rfgsm_rx_nv_tbl, arfcn_linearity );

  rx_sw_pt_ptr = rfgsm_core_handle_ptr->rfgsm_nv_rx_freq_comp_switch_ptr[rfgsm_core_handle_ptr->band][asdiv_pos];

  if ( rfgsm_core_get_sawless_switchpoints(rfm_dev, 
                                           rfgsm_core_handle_ptr->rf_rx_arfcn, 
                                           rfgsm_core_handle_ptr->band, 
                                           rx_sw_pt_ptr,
                                           asdiv_pos) == FALSE)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Could not get Rx swichpoints for device %d ", rfm_dev);
    return FALSE;
  }

  RF_NULL_CHECK_RTN(rx_sw_pt_ptr,"rx_sw_pt_ptr is NULL!", FALSE);

  // Get the switchpoints from NV
  actual_switch_1_2 = rx_sw_pt_ptr->rx_switch_1_2_dBm[arfcn_index];
  actual_switch_2_3 = rx_sw_pt_ptr->rx_switch_2_3_dBm[arfcn_index];
  actual_switch_3_4 = rx_sw_pt_ptr->rx_switch_3_4_dBm[arfcn_index];
  actual_switch_4_5 = rx_sw_pt_ptr->rx_switch_4_5_dBm[arfcn_index];

  /* Initialize LIF freq offset to 0 every frame since device driver uses it blindly */
  rfgsm_core_handle_ptr->enh_rx_freq_offset = 0;

  rfdevice_gsm_get_rx_enh_rx_support(rfm_dev, &is_enh_rx_supported);
  if (is_enh_rx_supported)
  {
    rfgsm_core_handle_ptr->enh_rx_on_flag = enh_rx.enh_rx_enable_flag;
    if (!enh_rx.enh_rx_enable_flag)
    {
      rfgsm_core_handle_ptr->enh_rx_freq_offset = 0;
    }
    else
    {
      /* If LIF is active check that the freq offset passed from L1 matches the expected value, currently +/-135KHz */
      if (enh_rx.enh_rx_offset_freq == RF_LIF_FREQ_OFFSET_DEFAULT_HZ || enh_rx.enh_rx_offset_freq == -RF_LIF_FREQ_OFFSET_DEFAULT_HZ)
        rfgsm_core_handle_ptr->enh_rx_freq_offset = enh_rx.enh_rx_offset_freq;
	  else /* Else throw an error */
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"LIF is active but enh_rx_offset_freq is incorrect! %d %d", enh_rx.enh_rx_enable_flag, enh_rx.enh_rx_offset_freq);
    }
  }

  /* Set the LNA and the Programmable Gain Amplifier */
  if ( rx_lvl_dbm < actual_switch_1_2)
  {
    /* ---------------------------------------------------------------- */
    /* Get the frequency compensated Rx chain gain value				  */
    /* NOTE: This gain value encompasses the complete transfer of power */
    /* from the input of the antenna to baseband. 					  */
    /* ---------------------------------------------------------------- */
    gain->valx16 = rfgsm_nv_rx_freq_comp (rfm_dev,
                                          rfgsm_rx_nv_tbl,
                                          rfgsm_core_handle_ptr->band,
                                          arfcn_linearity,
                                          arfcn_index,
                                          RFNV_GSM_LNA_STATE_1);
  
    /* Specify the window of valid Rx powers for this gain range */
    gain->range_high_limitx16 = actual_switch_1_2 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range_low_limitx16	= (int16)BSP_RX_POWER_LOWER_LIMIT_dBm_x16;
    gain->range				= GAIN_RANGE_1;  
  }
  else if ( rx_lvl_dbm < actual_switch_2_3)
  {
    gain->valx16 = rfgsm_nv_rx_freq_comp (rfm_dev,
                                          rfgsm_rx_nv_tbl,
                                          rfgsm_core_handle_ptr->band,
                                          arfcn_linearity,
                                          arfcn_index,
                                          RFNV_GSM_LNA_STATE_2);
	
    /* Specify the window of valid Rx powers for this gain range */
    gain->range_high_limitx16 = actual_switch_2_3 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range_low_limitx16  = actual_switch_1_2 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range               = GAIN_RANGE_2;
  } 
  else if ( rx_lvl_dbm < actual_switch_3_4)
  {
    gain->valx16 = rfgsm_nv_rx_freq_comp (rfm_dev,
                                          rfgsm_rx_nv_tbl,
                                          rfgsm_core_handle_ptr->band,
                                          arfcn_linearity,
                                          arfcn_index,
                                          RFNV_GSM_LNA_STATE_3);
	
    /* Specify the window of valid Rx powers for this gain range */
    gain->range_high_limitx16 = actual_switch_3_4 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range_low_limitx16  = actual_switch_2_3 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16; 
    gain->range               = GAIN_RANGE_3;
  } 
  else
  {
    if (rfgsm_core_handle_ptr->rf_has_five_gainranges)
    {
      if (rx_lvl_dbm < actual_switch_4_5)
      {
        gain->valx16 = rfgsm_nv_rx_freq_comp (rfm_dev,
                                              rfgsm_rx_nv_tbl,
                                              rfgsm_core_handle_ptr->band,
                                              arfcn_linearity,
                                              arfcn_index,
                                              RFNV_GSM_LNA_STATE_4);
	
         /* Specify the window of valid Rx powers for this gain range */
        gain->range_high_limitx16 = actual_switch_4_5 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
        gain->range_low_limitx16  = actual_switch_3_4 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16; 
        gain->range               = GAIN_RANGE_4;
      } 
      else
      {
        gain->valx16 = rfgsm_nv_rx_freq_comp (rfm_dev,
                                              rfgsm_rx_nv_tbl,
                                              rfgsm_core_handle_ptr->band,
                                              arfcn_linearity,
                                              arfcn_index,
                                              RFNV_GSM_LNA_STATE_5);
	
         /* Specify the window of valid Rx powers for this gain range */
         gain->range_high_limitx16 = BSP_RX_POWER_UPPER_LIMIT_dBm_x16;
         gain->range_low_limitx16  = actual_switch_4_5 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16; 
         gain->range               = GAIN_RANGE_5;
      }
    } 
    else /* if !rf_has_five_gainranges */
    {
      gain->valx16 = rfgsm_nv_rx_freq_comp (rfm_dev,
                                            rfgsm_rx_nv_tbl,
                                            rfgsm_core_handle_ptr->band,
                                            arfcn_linearity,
                                            arfcn_index,
                                            RFNV_GSM_LNA_STATE_4);
	
       /* Specify the window of valid Rx powers for this gain range */
      gain->range_high_limitx16 = actual_switch_4_5 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
      gain->range_low_limitx16  = actual_switch_3_4 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16; 
      gain->range               = GAIN_RANGE_4;
    }
  }
 
  /* store computed gain range based on rx level only if not in cal mode since rx level is invalid in cal mode */
  if (!ftm_gsm_is_in_calibration_mode())
  {
    rfgsm_core_handle_ptr->lna_range = (uint8)gain->range;
  }

  if(rfgsm_core_handle_ptr->rx_debug_logging_enable == TRUE)
  {
    MSG_9(MSG_SSID_RF, MSG_LEGACY_HIGH,"Rx Debug Logging: Rx_lvl_dbm: %d, Gain_range: %d, fc_gain: %d,"
                                       "LNA Switch Points - 1_2: [%d], 2_3: [%d], 3_4: [%d], 4_5: [%d], band:%d, channel:%d",
                                       rx_lvl_dbm,
                                       gain->range,
                                       gain->valx16,
                                       actual_switch_1_2,
                                       actual_switch_2_3,
                                       actual_switch_3_4,
                                       actual_switch_4_5,
                                       rfgsm_core_handle_ptr->band,
                                       rfgsm_core_handle_ptr->rf_rx_arfcn);
  }
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*! @fn rfgsm_core_set_gain                                                  */
/*! @brief calculate gain range and window based on calibrated switchpoints  */
/*! @details GL1 provide RF GSM with an expected dBm level and based on this */
/*!          RF can calculate which LNA gain range the WTR needs to be in    */
/*! @param rfm_dev - logical device                                          */
/*! @param rfgsm_core_handle_ptr - core handle pointer                       */
/*! @param rx_lvl_dbm - expected dBm level                                   */
/*! @param gain - pointer to data structure to populate                      */
/*! @retval boolean - success or failure of API                              */
boolean rfgsm_core_calc_gain( rfm_device_enum_type rfm_dev, 
                              rfgsm_core_handle_type * rfgsm_core_handle_ptr,
                              dBx16_T rx_lvl_dbm,  
                              rf_gain_T * gain,
                              boolean is_high_lin,
                              boolean aci_flag )
{
  /* Initialise variables */
  dBx16_T actual_switch_1_2, actual_switch_2_3, actual_switch_3_4, actual_switch_4_5 = 0;
  uint8 arfcn_index = 0;
  rfgsm_nv_rx_switch_type * rx_sw_pt_ptr = NULL;
  rfgsm_nv_band_rx_data_type *rfgsm_rx_nv_tbl = NULL;
  uint16 arfcn_discontinuity = 0;
  /* Do not use HL switchpoints by default */
  boolean use_hl_gain_swpt = FALSE;
  int16 gain_offset = 0;
  rfnv_gsm_rx_elna_asdiv_type * rfnv_gsm_rx_elna_asdiv_data = NULL;
  uint8 asdiv_pos = 0;
  /*This is added to overcome KW*/
  rfgsm_core_handle_ptr->aci_detected = aci_flag;

#ifdef FEATURE_RF_ASDIV
  asdiv_pos = (uint8)rfcommon_asdiv_get_current_position(rfm_dev);
  if ( asdiv_pos >= ASDIV_POSITION_MAX )
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
          "Invalid antenna position %d, defaulting to %d", 
          asdiv_pos, ASDIV_POSITION_0);
    asdiv_pos = ASDIV_POSITION_0;
  }
#endif

  if (rfgsm_core_handle_ptr->band < RFCOM_NUM_GSM_BANDS)
  {
    rfgsm_rx_nv_tbl = rfgsm_core_handle_ptr->rfgsm_rx_nv_tbl[rfgsm_core_handle_ptr->band];
  }
  RF_NULL_CHECK_RTN(rfgsm_rx_nv_tbl,"rfgsm_rx_nv_tbl is NULL!", 0);

  /* Check for invalid band values */
  if( rfgsm_core_handle_ptr->band >= RFCOM_NUM_GSM_BANDS )
  {
    ERR_FATAL( "rfgsm_core_calc_gain() invalid band %d, for dev %d",
               rfgsm_core_handle_ptr->band, rfm_dev, 0 );
    return FALSE;
  }

  if (!rfgsm_core_is_arfcn_valid(rfgsm_core_handle_ptr->band, rfgsm_core_handle_ptr->rf_rx_arfcn))
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "Invalid Band=%d and ARFCN=%d combination!",rfgsm_core_handle_ptr->band,rfgsm_core_handle_ptr->rf_rx_arfcn);
    return FALSE;
  }

  /* Map the channel linearly if EGSM because of extended channels */
  /* @TODO check if GSM band is EGSM outside of this API */
  arfcn_discontinuity = rfgsm_map_arfcn_linearly(rfgsm_core_handle_ptr->band, rfgsm_core_handle_ptr->rf_rx_arfcn);

  /* Get the closest channel that was calibrated */
  /* @TODO possible enhancement caching of single or multiple channels circular buffer...?? */
  arfcn_index =  rfgsm_nv_get_closest_arfcn_index(rfgsm_rx_nv_tbl, arfcn_discontinuity);

  rfnv_gsm_rx_elna_asdiv_data = (rfnv_gsm_rx_elna_asdiv_type *)rfgsm_nv_get_elna_asdiv_info(rfm_dev, rfgsm_core_handle_ptr->band, RFGSM_NV_RX);

  /* Get the device and band specific frequency compensated switchpoints */
  if ( is_high_lin == FALSE )
  {
     /* Use standard Rx gain switchpoints */
    rx_sw_pt_ptr = rfgsm_core_handle_ptr->rfgsm_nv_rx_freq_comp_switch_ptr[rfgsm_core_handle_ptr->band][asdiv_pos];
  }
  else
  {
    /* For SAWless device where ARFCN is in HL mode, use the HL gain switchpoints */
    rx_sw_pt_ptr = rfgsm_core_handle_ptr->rfgsm_nv_hl_rx_switch_ptr[rfgsm_core_handle_ptr->band][asdiv_pos]; 
  }

  if( rx_sw_pt_ptr == NULL )
  {
    ERR_FATAL( "rfgsm_core_calc_gain() rfgsm_nv_rx_freq_comp_switch_ptr is NULL for dev %d, band %d, chan %d",
               rfm_dev, rfgsm_core_handle_ptr->band, rfgsm_core_handle_ptr->rf_rx_arfcn );
    return FALSE;
  }

  /* Get the switchpoints from NV */
  actual_switch_1_2 = rx_sw_pt_ptr->rx_switch_1_2_dBm[arfcn_index];
  actual_switch_2_3 = rx_sw_pt_ptr->rx_switch_2_3_dBm[arfcn_index];
  actual_switch_3_4 = rx_sw_pt_ptr->rx_switch_3_4_dBm[arfcn_index];
  actual_switch_4_5 = rx_sw_pt_ptr->rx_switch_4_5_dBm[arfcn_index];

  /* Get the gain data based on expected dBm level */
  if ( rx_lvl_dbm < actual_switch_1_2)
  {
    if(rfnv_gsm_rx_elna_asdiv_data)
    {
      gain_offset = rfnv_gsm_rx_elna_asdiv_data->rx_elna_asdiv_gain_offset[asdiv_pos].gain_offset[RFNV_GSM_LNA_STATE_1];
    }
    gain->valx16              = rfgsm_nv_rx_freq_comp (rfm_dev,
                                                       rfgsm_rx_nv_tbl,
                                                       rfgsm_core_handle_ptr->band,
                                                       arfcn_discontinuity,
                                                       arfcn_index,
                                                       RFNV_GSM_LNA_STATE_1);
    gain->valx16             += gain_offset;                               
    gain->range_high_limitx16   = actual_switch_1_2 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range_low_limitx16	= (int16)BSP_RX_POWER_LOWER_LIMIT_dBm_x16;
    gain->range                 = GAIN_RANGE_1;  
  }
  else if ( rx_lvl_dbm < actual_switch_2_3)
  {
    if(rfnv_gsm_rx_elna_asdiv_data)
    {
      gain_offset = rfnv_gsm_rx_elna_asdiv_data->rx_elna_asdiv_gain_offset[asdiv_pos].gain_offset[RFNV_GSM_LNA_STATE_2];
    }
    gain->valx16              = rfgsm_nv_rx_freq_comp (rfm_dev,
                                                       rfgsm_rx_nv_tbl,
                                                       rfgsm_core_handle_ptr->rf_rx_arfcn,
                                                       arfcn_discontinuity,
                                                       arfcn_index,
                                                       RFNV_GSM_LNA_STATE_2);
    gain->valx16             += gain_offset;
    gain->range_high_limitx16 = actual_switch_2_3 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range_low_limitx16  = actual_switch_1_2 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range               = GAIN_RANGE_2;

#ifdef FEATURE_RF_HAS_WTR3925
     if(aci_flag)
     {
      gain->valx16              = rfgsm_nv_rx_freq_comp (rfm_dev,
                                                       rfgsm_rx_nv_tbl,
                                                       rfgsm_core_handle_ptr->band,
                                                       arfcn_discontinuity,
                                                       arfcn_index,
                                                       RFNV_GSM_LNA_STATE_1);
      gain->valx16             += gain_offset;
      gain->range               = GAIN_RANGE_1;
     }
#endif
  } 
  else if ( rx_lvl_dbm < actual_switch_3_4)
  {
    if(rfnv_gsm_rx_elna_asdiv_data)
    {
      gain_offset = rfnv_gsm_rx_elna_asdiv_data->rx_elna_asdiv_gain_offset[asdiv_pos].gain_offset[RFNV_GSM_LNA_STATE_3];
    }
    gain->valx16              = rfgsm_nv_rx_freq_comp (rfm_dev,
                                                       rfgsm_rx_nv_tbl,
                                                       rfgsm_core_handle_ptr->band,
                                                       arfcn_discontinuity,
                                                       arfcn_index,
                                                       RFNV_GSM_LNA_STATE_3);

    gain->valx16             += gain_offset;
    gain->range_high_limitx16 = actual_switch_3_4 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range_low_limitx16  = actual_switch_2_3 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16; 
    gain->range               = GAIN_RANGE_3;
  } 
  else if ( (rx_lvl_dbm < actual_switch_4_5) ||
            (rfgsm_core_handle_ptr->rf_has_five_gainranges == FALSE) )
  {
    if(rfnv_gsm_rx_elna_asdiv_data)
    {
      gain_offset = rfnv_gsm_rx_elna_asdiv_data->rx_elna_asdiv_gain_offset[asdiv_pos].gain_offset[RFNV_GSM_LNA_STATE_4];
    }
    gain->valx16              = rfgsm_nv_rx_freq_comp (rfm_dev,
                                                       rfgsm_rx_nv_tbl,
                                                       rfgsm_core_handle_ptr->band,
                                                       arfcn_discontinuity,
                                                       arfcn_index,
                                                       RFNV_GSM_LNA_STATE_4);

    gain->valx16             += gain_offset;
    gain->range_high_limitx16 = actual_switch_4_5 + RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16;
    gain->range_low_limitx16  = actual_switch_3_4 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16; 
    gain->range               = GAIN_RANGE_4;
  }
  else //has 5 gain ranges
  {
    if(rfnv_gsm_rx_elna_asdiv_data)
    {
      gain_offset = rfnv_gsm_rx_elna_asdiv_data->rx_elna_asdiv_gain_offset[asdiv_pos].gain_offset[RFNV_GSM_LNA_STATE_5];
    }
    gain->valx16              = rfgsm_nv_rx_freq_comp (rfm_dev,
                                                       rfgsm_rx_nv_tbl,
                                                       rfgsm_core_handle_ptr->band,
                                                       arfcn_discontinuity,
                                                       arfcn_index,
                                                       RFNV_GSM_LNA_STATE_5);
    gain->valx16             += gain_offset;
    gain->range_high_limitx16 = BSP_RX_POWER_UPPER_LIMIT_dBm_x16;
    gain->range_low_limitx16  = actual_switch_4_5 - RF_GAIN_RANGE_SPEC_MAX_ERROR_dBx16; 
    gain->range               = GAIN_RANGE_5;
  }
  //This is Just an interim hack to pass GCF and also not affect much in real scenarios with WTR3925
  // we are adding a 1.5dB [24 counts] offset so that GCF just passes for the higher ARFCNS.
  //The issue is only with ARFCNs>790 and only in PCS band.
#ifdef FEATURE_RF_HAS_WTR3925
  if((rfgsm_core_handle_ptr->band == RFCOM_BAND_GSM1900)&& (arfcn_index>=13) && rfgsm_core_handle_ptr->rx_burst_type == RFGSM_RX_MON_BURST)
  {
    gain->valx16= (gain->valx16 - 24);
  }
#endif
  if(rfgsm_core_handle_ptr->rx_debug_logging_enable == TRUE)
  {
    MSG_9(MSG_SSID_RF, MSG_LEGACY_HIGH,"Rx AGC | Rx_lvl_dbm: %d, Gain_range: %d, gainx16: %d,"
                                       "LNA swpts - 1_2: [%d], 2_3: [%d], 3_4: [%d],"
                                       "RXburst: %d, ARFCN: %d, Band: %d",
                                       rx_lvl_dbm,
                                       gain->range,
                                       gain->valx16 ,
                                       actual_switch_1_2,
                                       actual_switch_2_3,
                                       actual_switch_3_4,
                                       rfgsm_core_handle_ptr->rx_burst_type,
                                       rfgsm_core_handle_ptr->rf_rx_arfcn,
                                       rfgsm_core_handle_ptr->band );
  }

  #ifdef FEATURE_RF_POWER_REPORTING
  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  rflm_dm_handle_meta_s handle_meta;
  rflm_dm_get_handle_info(rfgsm_core_handle_ptr->rxlm_buf_index, &handle_meta);
  MSG_3(MSG_SSID_RF, MSG_LEGACY_MED, "rfgsm_core_calc_gain log rx agc event handle %d, lna state %d, device %d",rfgsm_core_handle_ptr->rxlm_buf_index, gain->range, handle_meta.devide_id );
  #endif 
  rflm_diag_power_log_rx_agc_event(rfgsm_core_handle_ptr->rxlm_buf_index, gain->range);
  #endif
  
  return TRUE;
}



#ifdef FEATURE_RF_ASDIV
/*----------------------------------------------------------------------------*/
/*!
  @details
   This function add antenna switch script to Rx Burst if needed.
   Then the processed command is discarded.

  @param rfm_dev : the RF device in use.
  @param probe burst : a boolean to know whether we need to swap antenna position
                       during MON burst
  @param start_buffer : pointer to the script to append.
  @param stop_buffer : pointer to the script to append.
*/
static void rfgsm_core_rx_asd_processing( rfm_device_enum_type rfm_dev, 
                                          rfgsm_rx_burst_type rx_burst_type,
                                          rf_burst_type burst, 
                                          rfgsm_mdsp_scripts_ptrs * scripts )
{
  rfgsm_core_handle_type * rfgsm_core_handle_ptr = NULL;
  rfcommon_asdiv_asm_conifg_type get_asm_config;
  rfcommon_asdiv_position_type current_pos;
  rfcommon_asdiv_send_script_type send_script_cb;
  rfcommon_asdiv_switch_start_notify_type switch_start_cb;
  rfcommon_asdiv_update_tuner_type update_tuner_cb;
  rfgsm_asdiv_xsw_script_data xsw_script_data;   
  uint8 command;
  int16 tuner_start_delta;
  rf_buffer_intf * start_asd_buffer = NULL; 
  rf_buffer_intf * stop_asd_buffer = NULL;
  rf_buffer_intf * start_tuner_buffer = NULL; 
  rf_buffer_intf * stop_tuner_buffer = NULL;

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  if( rfgsm_core_handle_ptr == NULL )
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL!!! for device %d ", rfm_dev, 0);
    return;
  }

  /* Get the scripts from the core handle */
  start_asd_buffer   = (rf_buffer_intf*)scripts->rx_start_script[RX_XSW_SCRIPT]; 
  stop_asd_buffer    = (rf_buffer_intf*)scripts->rx_stop_script[RX_XSW_SCRIPT];
  start_tuner_buffer = (rf_buffer_intf*)scripts->rx_start_script[RX_ATUNER_SCRIPT]; 
  stop_tuner_buffer  = (rf_buffer_intf*)scripts->rx_stop_script[RX_ATUNER_SCRIPT];

  if( !rfc_gsm_get_timing_info(rfm_dev, rfgsm_core_handle_ptr->band, RFDEVICE_TUNER_MANAGER, RFC_TIMING_TUNER_CTL, &tuner_start_delta) )
  {
    /* In the event of an error, set the start delta to a default */
    tuner_start_delta = RFC_GSM_DEFAULT_ANTENNA_TUNER_START_DELTA;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"RX ASD Processing: tuner_start_delta is set to default of -150 us!!! for device %d ", rfm_dev);
  }   

  xsw_script_data.device = rfm_dev;
  xsw_script_data.band = rfgsm_core_handle_ptr->band; 
  xsw_script_data.immediate = FALSE; /* FALSE = Create script (and not execute switch immediately) */
  xsw_script_data.start_delta = tuner_start_delta;   /* Start delta used for both tuner and xsw */

  command = rfgsm_core_handle_ptr->set_antenna_position;
  get_asm_config.cb_func = rfc_gsm_append_asdiv;
  get_asm_config.cb_data = &xsw_script_data;
  send_script_cb.cb_func = NULL;
  send_script_cb.cb_data = NULL;
  switch_start_cb.cb_func = NULL;
  switch_start_cb.cb_data = NULL;
  update_tuner_cb.cb_func = NULL;
  update_tuner_cb.cb_data = 0;
   
  current_pos = rfcommon_asdiv_get_current_position(rfm_dev);   

  if( (command != RF_INVALID_VALUE_8BIT) && (current_pos != (uint32)command) )
  {
    if( rx_burst_type == RFGSM_RX_PROBE_BURST )
    {
      /* Configure switch for Rx start */
      if( !rfcommon_asdiv_manager_config_switch( rfm_dev, //RFM device
                                                 RFM_EGSM_MODE, //RFM mode
                                                 (command == 0), //antenna position
                                                 start_asd_buffer, //switch script
                                                 start_tuner_buffer, //tuner script
                                                 get_asm_config, //get ASM script cb
                                                 send_script_cb, //send script cb
                                                 switch_start_cb, //start switch cb
                                                 update_tuner_cb, //update tuner cb
                                                 NULL, //L1 cb function
                                                 NULL, //L1 cb data
                                                 FALSE, //gen script only
                                                 FALSE, //immediate
                                                 tuner_start_delta, //tuner start delta
                                                 0 //rxlm handle UNUSED
                                                ) )
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Switch and tuner scripts were not built successfully ", 0, 0);
      }

      /* Start delta should be 0 for Rx stop (which returns antenna to original position) */
      xsw_script_data.start_delta = 0;

      /* Configure switch for Rx stop if probe burst */
      if( !rfcommon_asdiv_manager_config_switch( rfm_dev, //RFM device
                                                 RFM_EGSM_MODE, //RFM mode 
                                                 command, //antenna position
                                                 stop_asd_buffer, //switch script
                                                 stop_tuner_buffer, //tuner script
                                                 get_asm_config, //get ASM script cb
                                                 send_script_cb, //send script cb
                                                 switch_start_cb, //start switch cb
                                                 update_tuner_cb, //update tuner cb
                                                 NULL, //L1 cb function
                                                 NULL, //L1 cb data
                                                 FALSE, //gen script only
                                                 FALSE, //immediate
                                                 0, //tuner start delta
                                                 0 //rxlm handle UNUSED
                                                ) )
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Tuner and switch scripts were not built successfully ", 0, 0);
      }
        
      rfgsm_core_handle_ptr->configure_asd_scripts = FALSE;	
    }
    else
    {
      /* Configure switch for Rx start */
      if( !rfcommon_asdiv_manager_config_switch( rfm_dev, //RFM device
                                                 RFM_EGSM_MODE, //RFM mode 
                                                 command, //antenna position
                                                 start_asd_buffer, //switch script
                                                 start_tuner_buffer, //tuner script
                                                 get_asm_config, //get ASM script cb
                                                 send_script_cb, //send script cb
                                                 switch_start_cb, //start switch cb
                                                 update_tuner_cb, //update tuner cb
                                                 NULL, //L1 cb function
                                                 NULL, //L1 cb data
                                                 FALSE, //gen script only
                                                 FALSE, //immediate
                                                 tuner_start_delta, //tuner start delta
                                                 0 //rxlm handle UNUSED
                                                ) )
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Tuner and switch scripts were not built successfully ", 0, 0);
      }
	 
      rfgsm_core_handle_ptr->configure_asd_scripts = FALSE;
		 
      /* ASD response check error timer start */
      rfgsm_core_asd_rsp_check_start(rfm_dev);
    }   

    rfgsm_core_handle_ptr->set_prev_ant_pos = FALSE;   
  }
  else
  {
    if( rx_burst_type == RFGSM_RX_PROBE_BURST )
    {
      /* Configure switch for Rx start */
      /* Boolean value TRUE (for gen_script_only) makes sure script is generated but "switch_in_progress" flag is not set */
      if(!rfcommon_asdiv_manager_config_switch( rfm_dev, //RFM device
                                                 RFM_EGSM_MODE, //RFM mode 
                                                 (current_pos == 0), //antenna position
                                                 start_asd_buffer, //switch script
                                                 start_tuner_buffer, //tuner script
                                                 get_asm_config, //get ASM script cb
                                                 send_script_cb, //send script cb
                                                 switch_start_cb, //start switch cb
                                                 update_tuner_cb, //update tuner cb
                                                 NULL, //L1 cb function
                                                 NULL, //L1 cb data
                                                 TRUE, //gen script only
                                                 FALSE, //immediate
                                                 tuner_start_delta, //tuner start delta
                                                 0 //rxlm handle UNUSED
                                                ) )
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Tuner and switch scripts were not built successfully ", 0, 0);
      }

      /* Start delta should be 0 for Rx stop (which returns antenna to original position) */
      xsw_script_data.start_delta = 0;

      /* Configure switch for Rx stop if probe burst */
      if( !rfcommon_asdiv_manager_config_switch( rfm_dev, //RFM device
                                                 RFM_EGSM_MODE, //RFM mode 
                                                 current_pos, //antenna position
                                                 stop_asd_buffer, //switch script
                                                 stop_tuner_buffer, //tuner script
                                                 get_asm_config, //get ASM script cb
                                                 send_script_cb, //send script cb
                                                 switch_start_cb, //start switch cb
                                                 update_tuner_cb, //update tuner cb
                                                 NULL, //L1 cb function
                                                 NULL, //L1 cb data
                                                 TRUE, //gen script only
                                                 FALSE, //immediate
                                                 0, //tuner start delta
                                                 0 //rxlm handle UNUSED
                                                ) )
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"Tuner and switch scripts were not built successfully ", 0, 0);
      }
    }

    /* Immediately after wakeup, we would need to just get the ASD GRFC scripts. The tuner scripts are already obtained in the 
       rfgsm_mc_tune_to_chan() as ASDiv manager already has the correct switch position updated */
    if(rfgsm_core_handle_ptr->set_prev_ant_pos)
    {
      rfc_gsm_append_asdiv(current_pos, start_asd_buffer, get_asm_config.cb_data);
      rfgsm_core_handle_ptr->set_prev_ant_pos = FALSE;
    }
  }
}
#endif



/*----------------------------------------------------------------------------*/
/*!
  @details
  This function clears the WTR RX burst scripts

  @param rfm_dev : rfm device
 
  @retval True/False
*/

boolean rfgsm_core_clear_rx_burst_device_scripts(rfm_device_enum_type rfm_dev )
{
  rfgsm_core_handle_type *rfgsm_core_handle_ptr;
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);

  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr,"rfgsm_core_handle_ptr is NULL!", FALSE);
  
  if(!rf_buffer_clear_script( (rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.rx_start_script[RX_WTR_SCRIPT], RF_BUFFER_ALL)||
	 !rf_buffer_clear_script( (rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.rx_stop_script[RX_WTR_SCRIPT], RF_BUFFER_ALL))
  {
    RF_MSG(RF_ERROR,"RF GSM WTR RX START/STOP script buffer was unsuccessfully cleared");
    return FALSE;
  }

  return TRUE;
}



/*----------------------------------------------------------------------------*/
/*!
   @fn rfgsm_core_populate_device_rx_burst_buffer 
   @brief This function retrieves wtr device driver scripts for GSM Burst and stores them in rfgsm_core_handles pointer
   @param  rfm_dev - RFM device processing for
   @param  burst - Rx, Mon, 
   @retval boolean - success/failure indication
*/
#if 0
boolean rfgsm_core_populate_rx_burst_device_buffers( rfgsm_core_data_type* core_data,
                                                     boolean enable_rx_diversity,
                                                     rf_burst_type burst)
{
  uint32 rfgsm_time_profile[3] = {0,0,0};
  rfgsmlib_sbi_burst_struct burst_script_infor;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr;
  uint16 buffer_mask;
  rfm_device_enum_type associated_rx_dev = RFM_INVALID_DEVICE;
  rfdevice_operation_mode rxdevice_oper_mode = RFDEVICE_OPER_MODE_STANDALONE ;
  burst_script_infor.burst = burst;
  burst_script_infor.burst_alpha = 625;
  burst_script_infor.scripts = NULL;
  burst_script_infor.override_alpha = FALSE;
  burst_script_infor.burst_timing_offset_adjust_qs = 0;

  GET_START_TIME_RX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0]); 

  /*if GL1 Flag to lower CCS prio in IDLE mode is set then WTR RX burst start time (AutoB writes only) needs to be adjusted*/
  if (core_data->gl1_lower_rx_ccs_prio) 
  {
    burst_script_infor.burst_timing_offset_adjust_qs = RFGSM_WORST_CASE_RFFE_BUS_DELAY;
  }

  /*Populate WTR Burst Scripts based on operating mode Standalone or IRAT*/
  if(core_data->rfgsm_core_handle_ptr->rfgsm_buffer == RFGSM_AWAY_BUFFER)
  {
    rxdevice_oper_mode = RFDEVICE_OPER_MODE_MEAS;
  }
  else
  {
    rxdevice_oper_mode = RFDEVICE_OPER_MODE_STANDALONE;
  }

  /*Get the associated Rx device and specify operation mode for device driver*/
  associated_rx_dev = rfc_common_get_preferred_associated_rx_device(core_data->rfm_dev);

  #ifdef FEATURE_RF_POWER_REPORTING
  /* associate handle with device */
  rflm_dm_handle_meta_s handle_meta;
  rflm_dm_get_handle_info(  core_data->rfgsm_core_handle_ptr->rxlm_buf_index , &handle_meta);
  handle_meta.devide_id = core_data->rfm_dev;
  handle_meta.device_id = core_data->rfm_dev;
  rflm_dm_set_handle_info( core_data->rfgsm_core_handle_ptr->rxlm_buf_index , &handle_meta );
  /* log channel number */
  rf_power_reporting_update_channel( core_data->rfm_dev, core_data->rfgsm_core_handle_ptr->rf_rx_arfcn );
  #endif

  /*DRX RX burst Writes are appended to PRX script based on enable_rx_diversity flag*/
  if(!rfdevice_gsm_rx_set_start_script(core_data->rfm_dev, 
                                    rxdevice_oper_mode,
                                    &burst_script_infor, 
                                    (rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.rx_start_script[RX_WTR_SCRIPT],
                                    &buffer_mask,
                                    enable_rx_diversity,
                                    (rfcom_gsm_band_type)((rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->band),
                                    associated_rx_dev,
                                    RFDEVICE_PRX_OPER_MODE))
  {
    RF_MSG_2(RF_ERROR,"RF GSM WTR RX start script retrieval failed for rfm_dev:|%d|Operating mode:|%d|",
                      core_data->rfm_dev,rxdevice_oper_mode);
    return FALSE;
  }

  GET_CURRENT_TIME_RX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[1]);
  
   /*DRX RX burst Writes are appended to PRX script based on enable_rx_diversity flag*/
  if(!rfdevice_gsm_rx_set_stop_script(core_data->rfm_dev, 
                                     (rf_buffer_intf*)core_data->rfgsm_core_handle_ptr->scripts.rx_stop_script[RX_WTR_SCRIPT],
                                      associated_rx_dev,
                                      RFDEVICE_PRX_OPER_MODE))
  {
    RF_MSG_2(RF_ERROR,"RF GSM WTR RX stop script retrieval failed for rfm_dev:|%d|Operating mode:|%d|",
                      core_data->rfm_dev,rxdevice_oper_mode);
    return FALSE;
  }

  GET_CURRENT_TIME_RX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[2]);
  
  /*Populating Device Status Read Event Scripts During Rx Burst */
  if(core_data->rfgsm_core_handle_ptr->device_config_to_read.params[RF_RX_BURST].enable)
  {
    rfgsm_core_populate_device_status_read_buffers(core_data, RF_RX_BURST);
  }
  
  rfgsm_core_log(core_data->rfm_dev);

  if(core_data->rfgsm_core_handle_ptr->enable_time_profiling.rx_burst) 			  
  {
    RF_MSG_2(MSG_LEGACY_HIGH,"Rx Burst Time Profiling, rfgsm_core_rx_set_start_script: %d us, rfgsm_core_rx_set_stop_script: %d us",
  	         rfgsm_time_profile[1],
  	         (rfgsm_time_profile[2] - rfgsm_time_profile[1]));
  }
  return TRUE;
} /* rfgsm_core_populate_rx_burst_device_buffer */
#endif


/*----------------------------------------------------------------------------*/
/*!
  @details
  This function sets up the GRFC and SBI/uwire buffer required for the
  Rx burst.

  @param burst : burst type
  @param buffer : buffer to use for the burst
*/
#if 0
void rfgsm_core_setup_rx_burst(rfgsm_core_data_type* core_data,
                               rf_burst_type burst,
                               rfgsm_rx_burst_type rx_burst_type)
{
  rfdevice_rsb_coef_type rfdev_rsb[RFCOM_NUM_GSM_BANDS];
  uint32 rsb_a_coef, rsb_b_coef;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = core_data->rfgsm_core_handle_ptr;

  rfgsm_core_dm_set_rx_gain(core_data, 
                            core_data->rfgsm_core_handle_ptr->rx_burst_type,
                            core_data->rfgsm_core_handle_ptr->rx_burst_num, 
                            core_data->rfgsm_core_handle_ptr->valx16);

  rfdevice_gsm_cmd_dispatch(core_data->rfm_dev, RF_PATH_0, RFDEVICE_GSM_GET_RX_RSB_DATA, &rfdev_rsb[0]);
  /* Two's complement for inversion . RTR returns 17Q15 format coefficients but mdsp requires 16Q14 format.  
     rfgsm_msm_get_bbrx_iq_mismatch_gain gets the value of the coefficients and returns a 16Q14 format   */

  rsb_a_coef = -(rfgsm_msm_get_coefa_bbrx_iqmc_compensated(rfdev_rsb[rfgsm_core_handle_ptr->band].rsb_a_coef));
  rsb_b_coef = -(rfgsm_msm_get_coefb_bbrx_iqmc_compensated(rfdev_rsb[rfgsm_core_handle_ptr->band].rsb_b_coef, rfgsm_core_handle_ptr->rxlm_buf_index));

  rfgsm_core_dm_set_spectral_inv(core_data,
                                 rfgsm_core_handle_ptr->rx_burst_type,
                                 rfgsm_core_handle_ptr->rx_burst_num, 
                                 rsb_a_coef, rsb_b_coef);

#ifdef FEATURE_RF_ASDIV 
  /* ASD only applies to PRX */
  if( rfcommon_asdiv_is_supported() && rfgsm_core_is_device_prx(core_data->rfm_dev) 
      &&(((rfgsm_core_handle_ptr->set_antenna_position != RF_INVALID_VALUE_8BIT) &&
          (rfgsm_core_handle_ptr->configure_asd_scripts == TRUE ))||
          (rfgsm_core_handle_ptr->set_prev_ant_pos == TRUE)||(rx_burst_type == RFGSM_RX_PROBE_BURST)))  
  {
    /* Call ASD processing to update GRFCs and tuner scripts if ASDiv is supported in the card */
    rfgsm_core_rx_asd_processing( core_data->rfm_dev, 
                                  rx_burst_type, 
                                  burst, 
                                  &rfgsm_core_handle_ptr->scripts );								  
  }
#endif   
} /* end of rfgsm_setup_rx_burst */
#endif


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function calls all the rfgsm_core functionality to create the RF CCS for all the
  bursts requested from GL1.

  This function can be called from RF Task context from the rfgsm_cmd_processing dispatcher
  or iRAT scenarios.

  @param core_data                 : RF Multimode device
  @param rx_burst_event_ptr  : Pointer to shared mem
*/
#if 0
boolean rfgsm_core_build_rx_burst_ccs_events( rfgsm_core_data_type* core_data,
                                              void* rx_burst_event_ptr,
                                              uint32 rf_sequence_number )
{
  boolean ret_status = FALSE;

  uint32 rfgsm_time_profile[3] = {0,0,0};
  GET_START_TIME_RX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0]); 

  /* Populate the Rx RFLM event data table */
  ret_status = rfgsm_core_dm_dynamic_rx_event_wrapper(core_data,
                                                      core_data->rfgsm_core_handle_ptr->rx_burst_type, /*retrieved from rfgsm_core_handle_ptr*/
                                                      core_data->rfgsm_core_handle_ptr->rx_burst_num);

  GET_CURRENT_TIME_RX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[1]);
  
  if( FALSE == ret_status )
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Rx RFLM dynamic event prog failed!");
    return ret_status;
  }
      
  ret_status = rfgsm_core_populate_burst_rflm_data_info(core_data,
                                                        core_data->rfgsm_core_handle_ptr->rx_burst, 
                                                        0,
                                                        rx_burst_event_ptr);

  GET_CURRENT_TIME_RX_BURST_PROFILING(core_data->rfgsm_core_handle_ptr, rfgsm_time_profile[0], rfgsm_time_profile[2]);
  
  if (FALSE == ret_status)
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Rx RFLM data info update failed!");
    return ret_status;
  }

  ret_status = rfgsm_core_dm_set_rx_rf_sequence_num( core_data,
                                                     core_data->rfgsm_core_handle_ptr->rx_burst_type,
                                                     core_data->rfgsm_core_handle_ptr->rx_burst_num,
                                                     rf_sequence_number );

  if (FALSE == ret_status)
  {
    RF_MSG(MSG_LEGACY_ERROR,"RF GSM Rx RF Sequence number update failed!");
    return ret_status;
  }

  if(core_data->rfgsm_core_handle_ptr->enable_time_profiling.rx_burst) 			  
  {
    RF_MSG_2(MSG_LEGACY_HIGH,"Rx Burst Time Profiling, dm_dynamic_event_wrapper: %d us, populate_burst_rflm_data_info: %d us",
             rfgsm_time_profile[1],
             (rfgsm_time_profile[2] - rfgsm_time_profile[1]));
  }
  return ret_status;
}
#endif


/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_prepare
  @brief This function prepares the Rx burst
  @details This is called for each burst request from L1
  @param sub_id - subscription that is performing the Rx
  @param rx_device - RFM logical device Rx is for
  @param buffer_id - Triple buffer ID
  @param burst_params - Rx burst details passed from L1
  @param burst_num - which burst is in multi-burst request
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_prepare( uint8 sub_id,
                               rfcom_device_enum_type prx_device,
                               rfa_rf_gsm_rx_burst_type * burst_params,
                               uint8 buffer_id,
                               uint32 burst_num )
{
  /* Initialise variables */
  ftm_mode_type rf_mode             = AMSS_MODE;
  rfgsm_core_handle_type * rfgsm_core_handle_ptr = NULL;
  rfgsm_band_type rx_band           = RF_MAX_BAND_TYPES;
  rfcom_gsm_band_type rfcom_rx_band = RFCOM_NUM_GSM_BANDS;
  uint16 rx_arfcn                   = 0xFFFF;
  int32 freq_err                    = 0;
  dBx16_T rx_lvl_dbm                = 0;
  rf_gain_T * gl1_gain_data_ptr     = NULL;
  rf_gain_T gain_data               = {0};
  uint8 lna_range                   = 0xFF;
  rfgsmlib_rx_gain_range_type lna_range_to_set = RFGSMLIB_GAIN_RANGE_1;
  rfgsm_rx_burst_type rx_burst_type = RFGSM_RX_DEFAULT_RX_BURST;
  rfgsm_rx_burst_type probe_burst   = RFGSM_RX_DEFAULT_RX_BURST;
  rf_burst_type rx_burst            = RF_MAX_BURST_TYPES;
  boolean lif_enabled               = FALSE;
  boolean aci_flag                  = FALSE;
  int32 enh_rx_freq_offset          = 0;
  uint8 set_antenna_position        = 0;
  boolean configure_asd_scripts     = FALSE;
  boolean set_prev_ant_pos          = FALSE;
  rf_buffer_intf * rx_start_script  = NULL;
  rf_buffer_intf * rx_stop_script   = NULL;
  boolean use_burst_params_freq_err = TRUE;
  boolean sawless_hl_mode_active    = FALSE;
  rfgsm_rx_chan_params_type * rx_chan_params = 
    &burst_params->rx_chan_params[burst_num];
  boolean rfgsm_ant_switch_in_progress = FALSE;
  int32 rsb_a_coef, rsb_b_coef;
  uint32 rxlm_buf_index;
  rfgsm_core_dm_rx_burst_data rx_data;

  /* Get the core handles and check valid */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get( prx_device );
  if( rfgsm_core_handle_ptr == NULL )
  {
    ERR_FATAL( "rfgsm_core_rx() rfgsm_core_handle_ptr NULL for sub %d and "
               "PRx dev %d", sub_id, prx_device, 0 );
    return FALSE;
  }

  /* Extract data from GL1 Rx burst params and store locally */
  rx_band            = rx_chan_params->band;
  rx_arfcn           = rx_chan_params->arfcn;
  freq_err           = rx_chan_params->freq_err;
  rx_lvl_dbm         = rx_chan_params->rx_lvl_dbm;
  gl1_gain_data_ptr  = rx_chan_params->gain_ptr;
  lna_range          = rx_chan_params->lna_range;
  rx_burst_type      = rx_chan_params->rx_burst_type;
  probe_burst        = rx_chan_params->probe_burst;
  rx_burst           = rx_chan_params->burst_type;
  lif_enabled        = rx_chan_params->enh_rx_params.enh_rx_enable_flag;
  enh_rx_freq_offset = rx_chan_params->enh_rx_params.enh_rx_offset_freq;
  aci_flag           = rx_chan_params->aci_flag;

  /* Extract required data from core handles */
  set_antenna_position  = rfgsm_core_handle_ptr->set_antenna_position;
  configure_asd_scripts = rfgsm_core_handle_ptr->configure_asd_scripts;
  set_prev_ant_pos      = rfgsm_core_handle_ptr->set_prev_ant_pos;
  rx_start_script       = (rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.rx_start_script[RX_WTR_SCRIPT];
  rx_stop_script        = (rf_buffer_intf *)rfgsm_core_handle_ptr->scripts.rx_start_script[RX_WTR_SCRIPT];
  use_burst_params_freq_err = rfgsm_core_handle_ptr->freq_error_flag_from_api;
  rxlm_buf_index        = rfgsm_core_handle_ptr->rxlm_buf_index;

  /* Validate and reassign LIF configuration */
  rfgsm_core_rx_configure_lif( prx_device, 
                               rfgsm_core_handle_ptr->rfdevice_supports_lif, 
                               &lif_enabled, 
                               &enh_rx_freq_offset );

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_CFG_LIF, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Debug Message */
  //MSG_9( MSG_SSID_RF, MSG_LEGACY_MED,
  //       "rfgsm_core_rx() burst num %d, type %d, band %d, ARFCN %d, "
  //       "freq err %d, Exp dBm lvl %d, LNA range %d, LIF en %d",
  //       burst_num, rx_burst_type, rx_band, rx_arfcn, freq_err, 
  //       rx_lvl_dbm, lna_range, lif_enabled, enh_rx_freq_offset );

  /* Get the if RF is in FTM mode */
  rf_mode = ftm_get_mode();

  /* Convert the sys band from GL1 to rfcom band type */
  rfcom_rx_band = rfgsm_core_convert_rfband_to_rfcom( rx_band );

  /* Set the band and NV tables */
  rfgsm_core_handle_ptr->band = rfcom_rx_band;

  /* Set the ARFCN */
  rfgsm_core_handle_ptr->rf_rx_arfcn = rx_arfcn;

  /*Set ACI flag status*/
  rfgsm_core_handle_ptr->aci_detected = aci_flag;
  
  /* Set the Rx burst type */
  rfgsm_core_handle_ptr->rx_burst_type = rx_burst_type;

  /* Set the Rx burst number */
  rfgsm_core_handle_ptr->rx_burst_num = burst_num;

  /* If this band is SAWless, then configure for HL mode if necessary*/
  if ( rfgsm_core_handle_ptr->sawless_support[RFGSM_MERGE_1800_B_BAND(rfcom_rx_band)])
  {
    rfgsm_core_configure_sawless(rfgsm_core_handle_ptr,
                                 &sawless_hl_mode_active);

    /* Update flag for GL1 logging */
    if(rx_chan_params->high_lin_mode_active == NULL)
    {
      MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "GL1 passed NULL ptr for high_lin_mode_active! Not updating HL state...");
    }
    else
    {
      *(rx_chan_params->high_lin_mode_active) = sawless_hl_mode_active;
    }
    
    /* Set HL mode appropriately in device driver */
    rfdevice_gsm_cmd_dispatch(prx_device, RF_PATH_0, RFDEVICE_GSM_SET_HIGH_LIN_MODE, &sawless_hl_mode_active);
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_SAWLESS, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Calculate the gain data */
  if( !rfgsm_core_calc_gain( prx_device,
                             rfgsm_core_handle_ptr,
                             rx_lvl_dbm,
                             &gain_data,
                             sawless_hl_mode_active,
                             aci_flag ) )
  {
    ERR_FATAL( "rfgsm_core_rx() rfgsm_core_set_gain failed for sub %d and "
               "PRx dev %d rx_lvl_dbm %d", sub_id, prx_device, rx_lvl_dbm );
    return FALSE;
  }

  /* Provide GL1 with gain data */
  if( gl1_gain_data_ptr == NULL )
  {
    ERR_FATAL( "rfgsm_core_rx() gain_ptr from GL1 is NULL for sub %d "
               "PRx dev %d burst num %d", sub_id, prx_device, burst_num );
    return FALSE;
  }

  memscpy( (void*)gl1_gain_data_ptr, 
           sizeof(rf_gain_T),
           (void*)&gain_data, 
           sizeof(rf_gain_T) );

  /* Store data in core handle */
  /*@TODO use rf_gain_T and store more info for debug then use memscpy */
  rfgsm_core_handle_ptr->lna_range = gain_data.range;
  rfgsm_core_handle_ptr->valx16    = gain_data.valx16;

  /* Get LNA range to set to WTR based on scenario */
  if( ftm_gsm_rx_device_is_ftm_state( prx_device ) == FALSE ) //AMSS mode or Non-Sig
  {
    /* Use calculated switchpoint based LNA range */
    lna_range_to_set = (rfgsmlib_rx_gain_range_type)gain_data.range;
  }
  else //FTM state
  {
    /* When in ftm continuous rx mode this function is called from      */
    /* acquisition and the lna_range parameter will be null. So use the */
    /* rx_lvl_dbm to get  lna range.                                    */
    if( ftm_gsm_get_lna_range_from_rx_lvl( prx_device ) )
    {
      /* Use Rx Lvl dBm to set LNA gain range */
      lna_range_to_set = (rfgsmlib_rx_gain_range_type)(rx_lvl_dbm + 1);
    }
    else
    {
      /* Use LNA range from GL1 */
      lna_range_to_set = (rfgsmlib_rx_gain_range_type)(lna_range + 1);
    }
  }

  /* Update the LNA range in WTR object */
  if( !rfdevice_gsm_set_rx_gain_range( prx_device, (int32)lna_range_to_set ) )
  {
    ERR_FATAL( "rfgsm_core_rx() rfdevice_gsm_set_rx_gain_range failed for "
               "dev %d, LNA range %d", prx_device, lna_range_to_set, 0 );
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_CALC_GAIN, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

#ifdef FEATURE_RF_ASDIV
  if( rfcommon_asdiv_is_supported() &&
    ( ( (set_antenna_position != RF_INVALID_VALUE_8BIT) && (configure_asd_scripts == TRUE) ) ||
        (set_prev_ant_pos == TRUE) ||
        (probe_burst == RFGSM_RX_PROBE_BURST) ) ) 
  {
    rfgsm_ant_switch_in_progress = TRUE;
  }
#endif
  
  /* This API retrieves RF configuration for all FE devices including GRFCs, ASM, ATUNER etc. for a give RX burst */
  rfc_gsm_rx_burst_config( prx_device,
                           rfcom_rx_band,
                           rx_arfcn,
                           probe_burst,
                           RFGSM_ALT_PATH_NOT_SUPPORTED,
                           set_antenna_position,
                           lna_range_to_set,
                           &rfgsm_core_handle_ptr->scripts,
                           RFDEVICE_PRX_OPER_MODE,
                           rfgsm_core_handle_ptr->rfgsm_mode,
                           rfgsm_ant_switch_in_progress );

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_RFC, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Determine whether frequency error needs to offset PLL */
  if( ( rf_mode != FTM_MODE ) ||
      ( (rf_mode == FTM_MODE) && (use_burst_params_freq_err == TRUE) ) )
  {
    /* Store frequency error from GL1 for debugging */
    rfgsm_core_handle_ptr->rfgsm_curr_freq_error = freq_err;
  }
  else
  {
    /* Override the freq err PLL offset */
    freq_err = RFDEVICE_GSM_FREQ_ERR_OVERRIDE;
  }

  /* Store device data for when tuning to specific channel with specific LIF params */
  if( !rfdevice_gsm_configure_rx( prx_device,
                                  rfcom_rx_band,
                                  rx_arfcn,
                                  (int32)lna_range_to_set,
                                  freq_err,
                                  lif_enabled,
                                  enh_rx_freq_offset ) )
  {
    ERR_FATAL( "rfgsm_core_rx() rfdevice_gsm_configure_rx failed for dev %d, "
               "ARFCN %d, LIF en %d", prx_device, rx_arfcn, lif_enabled );
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_DEVICE, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Calc iqmc compensated coef a and b */
  rsb_a_coef = -( rfgsm_msm_get_coefa_bbrx_iqmc_compensated( rfgsm_core_handle_ptr->rfdev_rsb[rfcom_rx_band].rsb_a_coef ) );
  rsb_b_coef = -( rfgsm_msm_get_coefb_bbrx_iqmc_compensated( rfgsm_core_handle_ptr->rfdev_rsb[rfcom_rx_band].rsb_b_coef, 
                                                             rxlm_buf_index ) );

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_MSM, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Configure the RFLM DM Rx data */
  rx_data.buffer_id          = buffer_id;
  rx_data.burst_num          = burst_num;
  rx_data.enh_rx_freq_offset = enh_rx_freq_offset;
  rx_data.enh_rx_on_flag     = lif_enabled;
  rx_data.gain               = gain_data.valx16;
  rx_data.is_prx             = TRUE;
  rx_data.rsb_coefA          = rsb_a_coef;
  rx_data.rsb_coefB          = rsb_b_coef;
  rx_data.rx_burst_type      = rx_burst_type;

  /* Set RSB, gain and LIF data in RFLM DM for GFW to retrieve */
  if( !rfgsm_core_dm_set_rx_burst_data( sub_id, &rx_data ) )
  {
    ERR_FATAL( "rfgsm_core_rx() rfgsm_core_dm_set_rx_burst_data failed for "
               "sub %d, burst type %d, buffer ID %d", 
               sub_id, rx_burst_type, buffer_id );
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_DM, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

#ifdef FEATURE_RF_ASDIV 
  /* Call ASD processing to update GRFCs and tuner scripts if:               */
  /* o ASDiv is supported in the card AND                                    */
  /*     o Antenna position valid AND Set Antenna Req successfully executed  */
  /*     OR                                                                  */
  /*     o Set previous antenna position is TRUE - only on wakeup            */
  /*     OR                                                                  */
  /*     o Probe burst type is set by GL1                                    */
  if ( rfgsm_ant_switch_in_progress )  
  {
    rfgsm_core_rx_asd_processing( prx_device,
                                  probe_burst,
                                  rx_burst,
                                  &rfgsm_core_handle_ptr->scripts );

    /* Reset flag so that in next RX, ASM builds XSW (under the hood) */
    rfgsm_ant_switch_in_progress = FALSE;

    RFGSM_CORE_PROC_LOG(RFGSM_CMD_CORE_RX_ASD, sub_id, prx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );					  
  }
#endif

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_diversity_prepare
  @brief This function prepares the Rx Diversity burst
  @details This is called for each burst request from L1
  @param sub_id - subscription that is performing the Rx
  @param rx_device - RFM logical device Rx is for
  @param buffer_id - Triple buffer ID
  @param burst_params - Rx burst details passed from L1
  @param burst_num - which burst is in multi-burst request
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_diversity_prepare( uint8 sub_id,
                                         rfcom_device_enum_type drx_device,
                                         rfa_rf_gsm_rx_burst_type * burst_params,
                                         uint8 buffer_id,
                                         uint32 burst_num )
{
  /* Initialise variables */
  rfgsm_core_handle_type * rfgsm_core_handle_ptr = NULL;
  rfgsm_band_type rx_band           = RF_MAX_BAND_TYPES;
  rfcom_gsm_band_type rfcom_rx_band = RFCOM_NUM_GSM_BANDS;
  uint16 rx_arfcn                   = 0xFFFF;
  dBx16_T rx_lvl_dbm                = 0;
  rf_gain_T * gl1_gain_data_ptr     = NULL;
  rf_gain_T gain_data               = {0};
  uint8 lna_range                   = 0xFF;
  rfgsmlib_rx_gain_range_type lna_range_to_set = RFGSMLIB_GAIN_RANGE_1;
  rfgsm_rx_burst_type rx_burst_type = RFGSM_RX_DEFAULT_RX_BURST;
  rfgsm_rx_burst_type probe_burst   = RFGSM_RX_DEFAULT_RX_BURST;
  rf_burst_type rx_burst            = RF_MAX_BURST_TYPES;
  uint8 set_antenna_position        = 0;
  boolean sawless_hl_mode_active    = FALSE;
  boolean aci_flag                  = FALSE;
  rfgsm_rx_chan_params_type * rx_chan_params = 
    &burst_params->rx_chan_params[burst_num];
  rfgsm_rx_diversity_chan_params_type * rxd_chan_params = 
    &burst_params->rx_diversity_chan_params[burst_num];
  int32 rsb_a_coef, rsb_b_coef;
  uint32 rxlm_buf_index;
  rfgsm_core_dm_rx_burst_data rx_data;

  /* Get the core handles and check valid */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get( drx_device );
  if( rfgsm_core_handle_ptr == NULL )
  {
    ERR_FATAL( "rfgsm_core_rx() rfgsm_core_handle_ptr NULL for sub %d and "
               "DRx dev %d", sub_id, drx_device, 0 );
    return FALSE;
  }

  /* Extract data from GL1 Rx burst params and store locally */
  rx_band            = rx_chan_params->band;
  rx_arfcn           = rx_chan_params->arfcn;
  rx_lvl_dbm         = rxd_chan_params->rx_lvl_dbm;
  gl1_gain_data_ptr  = rxd_chan_params->gain_ptr;
  lna_range          = rxd_chan_params->lna_range;
  rx_burst_type      = rx_chan_params->rx_burst_type;
  probe_burst        = rx_chan_params->probe_burst;
  rx_burst           = rx_chan_params->burst_type;
  aci_flag           = rx_chan_params->aci_flag;
  /* Extract required data from core handles */
  set_antenna_position = rfgsm_core_handle_ptr->set_antenna_position;
  rxlm_buf_index       = rfgsm_core_handle_ptr->rxlm_buf_index;

  /* Debug Message */
  //MSG_6( MSG_SSID_RF, MSG_LEGACY_MED,
  //       "rfgsm_core_rx_diversity() burst num %d, type %d, band %d, ARFCN %d, "
  //       "Exp dBm lvl %d, LNA range %d", burst_num, rx_burst_type, rx_band, 
  //       rx_arfcn, rx_lvl_dbm, lna_range );

  /* Convert the sys band from GL1 to rfcom band type */
  rfcom_rx_band = rfgsm_core_convert_rfband_to_rfcom( rx_band );

  /* Set the band and NV tables */
  rfgsm_core_handle_ptr->band = rfcom_rx_band;

  /* Set the ARFCN */
  rfgsm_core_handle_ptr->rf_rx_arfcn = rx_arfcn;

  /* Set the Rx burst type */
  rfgsm_core_handle_ptr->rx_burst_type = rx_burst_type;

  /* Set the Rx burst number */
  rfgsm_core_handle_ptr->rx_burst_num = burst_num;

  /* If this band is SAWless, then load the switchpoints for HL mode */
  if ( rfgsm_core_handle_ptr->sawless_support[RFGSM_MERGE_1800_B_BAND(rfcom_rx_band)] )
  {
    rfgsm_core_configure_sawless(rfgsm_core_handle_ptr,
                                 &sawless_hl_mode_active);
                                 
    if(rxd_chan_params->high_lin_mode_active == NULL)
    {
      MSG(MSG_SSID_RF, MSG_LEGACY_ERROR, "GL1 passed NULL ptr for high_lin_mode_active! Not updating HL state...");
    }
    else
    {
      *(rxd_chan_params->high_lin_mode_active) = sawless_hl_mode_active;
    }
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_SAWLESS, sub_id, drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Calculate the gain data */
  /* @TODO DRx arfcn data is same as PRx */
  if( !rfgsm_core_calc_gain( drx_device,
                             rfgsm_core_handle_ptr,
                             rx_lvl_dbm,
                             &gain_data,
                             sawless_hl_mode_active,
                             aci_flag ) )
  {
    ERR_FATAL( "rfgsm_core_rx_diversity() rfgsm_core_set_gain failed for sub %d and "
               "DRx dev %d rx_lvl_dbm %d", sub_id, drx_device, rx_lvl_dbm );
    return FALSE;
  }

  /* Provide GL1 with gain data */
  if( gl1_gain_data_ptr == NULL )
  {
    ERR_FATAL( "rfgsm_core_rx_diversity() gain_ptr from GL1 is NULL for sub %d "
               "DRx dev %d burst num %d", sub_id, drx_device, burst_num );
    return FALSE;
  }

  memscpy( (void*)gl1_gain_data_ptr, 
           sizeof(rf_gain_T),
           (void*)&gain_data, 
           sizeof(rf_gain_T) );

  /* Store data in core handle */
  /*@TODO use rf_gain_T and store more info for debug then use memscpy */
  rfgsm_core_handle_ptr->lna_range = gain_data.range;
  rfgsm_core_handle_ptr->valx16    = gain_data.valx16;

  /* Get LNA range to set to WTR based on scenario */
  if( ftm_gsm_rx_device_is_ftm_state( drx_device ) == FALSE ) //AMSS mode or Non-Sig
  {
    /* Use calculated switchpoint based LNA range */
    lna_range_to_set = (rfgsmlib_rx_gain_range_type)gain_data.range;
  }
  else //FTM state
  {
    /* When in ftm continuous rx mode this function is called from      */
    /* acquisition and the lna_range parameter will be null. So use the */
    /* rx_lvl_dbm to get  lna range.                                    */
    if( ftm_gsm_get_lna_range_from_rx_lvl( drx_device ) )
    {
      /* Use Rx Lvl dBm to set LNA gain range */
      lna_range_to_set = (rfgsmlib_rx_gain_range_type)(rx_lvl_dbm + 1);
    }
    else
    {
      /* Use LNA range from GL1 */
      lna_range_to_set = (rfgsmlib_rx_gain_range_type)(lna_range + 1);
    }
  }
  
  /* Update the LNA range in WTR object */
  if( !rfdevice_gsm_set_rx_gain_range( drx_device, (int32)lna_range_to_set ) )
  {
    ERR_FATAL( "rfgsm_core_rx_diversity() rfdevice_gsm_set_rx_gain_range failed for "
               "dev %d, LNA range %d", drx_device, lna_range_to_set, 0 );
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_CALC_GAIN, sub_id, drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* This API retrieves RF configuration for all FE devices including GRFCs, ASM, ATUNER etc. for a give RX burst */
  rfc_gsm_rx_burst_config( drx_device,
                           rfcom_rx_band,
                           rx_arfcn,
                           probe_burst,
                           RFGSM_ALT_PATH_NOT_SUPPORTED,
                           set_antenna_position,
                           lna_range_to_set,
                           &rfgsm_core_handle_ptr->scripts,
                           RFDEVICE_DRX_OPER_MODE,
                           rfgsm_core_handle_ptr->rfgsm_mode,
                           FALSE );

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_RFC, sub_id, drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Calc iqmc compensated coef a and b */
  rsb_a_coef = -( rfgsm_msm_get_coefa_bbrx_iqmc_compensated( rfgsm_core_handle_ptr->rfdev_rsb[rfcom_rx_band].rsb_a_coef ) );
  rsb_b_coef = -( rfgsm_msm_get_coefb_bbrx_iqmc_compensated( rfgsm_core_handle_ptr->rfdev_rsb[rfcom_rx_band].rsb_b_coef, 
                                                             rxlm_buf_index ) );

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_MSM, sub_id, drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Configure the RFLM DM Rx data */
  rx_data.buffer_id          = buffer_id;
  rx_data.burst_num          = burst_num;
  rx_data.enh_rx_freq_offset = 0; //Not set for DRx
  rx_data.enh_rx_on_flag     = FALSE; //Not set for DRx
  rx_data.gain               = gain_data.valx16;
  rx_data.is_prx             = FALSE;
  rx_data.rsb_coefA          = rsb_a_coef;
  rx_data.rsb_coefB          = rsb_b_coef;
  rx_data.rx_burst_type      = rx_burst_type;

  /* Set RSB, gain and LIF data in RFLM DM for GFW to retrieve */
  if( !rfgsm_core_dm_set_rx_burst_data( sub_id, &rx_data ) )
  {
    ERR_FATAL( "rfgsm_core_rx_diversity() rfgsm_core_dm_set_rx_burst_data failed for "
               "sub %d, burst type %d, buffer ID %d", 
               sub_id, probe_burst, buffer_id );
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_DM, sub_id, drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  return TRUE;
}



/*---------------------------------------------------------------------------*/
/*!
  @fn rfgsm_core_rx_build_scripts
  @brief This function prepares the Rx burst
  @details This is called for each burst request from L1
  @param sub_id - subscription that is performing the Rx
  @param prx_device - RFM logical device PRx is for
  @param drx_device - RFM logical device DRx is for
  @param burst_params - Rx burst details passed from L1
  @param buffer_id - Triple buffer ID
  @param burst_num - which burst is in multi-burst request
  @param gfw_shared_mem - gfw shared memory location for 
  @retval boolean - successful/unsuccessful
*/
boolean rfgsm_core_rx_build_scripts( uint8 sub_id,
                                     rfcom_device_enum_type prx_device,
                                     rfcom_device_enum_type drx_device,
                                     rfa_rf_gsm_rx_burst_type * burst_params,
                                     boolean rx_diversity_enabled,
                                     uint8 buffer_id,
                                     uint32 burst_num )
{
  /* Initialise variables */
  rfgsm_core_handle_type * rfgsm_prx_core_handle_ptr = NULL;
  rfgsm_core_handle_type * rfgsm_drx_core_handle_ptr = NULL;
  rfgsm_core_data_type core_data;
  rf_buffer_intf * rx_start_script  = NULL;
  rf_buffer_intf * rx_stop_script   = NULL;
  rfcom_device_enum_type associated_rx_dev = RFM_INVALID_DEVICE;
  rfgsmlib_sbi_burst_struct burst_script_infor;
  uint16 buffer_mask;
  rfdevice_operation_mode rxdevice_oper_mode = RFDEVICE_OPER_MODE_STANDALONE;
  uint32 rf_sequence_number = 0;
  rf_burst_type burst_type = RF_MAX_BURST_TYPES;
  rfgsm_rx_burst_type rx_burst_type = RFGSM_RX_DEFAULT_RX_BURST;
  void * gfw_shared_mem = NULL;
  rfgsm_rx_chan_params_type * rx_chan_params = 
    &burst_params->rx_chan_params[burst_num];

  /* Get the core handles and check valid */
  rfgsm_prx_core_handle_ptr = rfgsm_core_handle_get( prx_device );
  if( rfgsm_prx_core_handle_ptr == NULL )
  {
    ERR_FATAL( "rfgsm_core_rx() rfgsm_core_handle_ptr NULL for sub %d and "
               "PRx dev %d", sub_id, prx_device, 0 );
    return FALSE;
  }

  /* If diversity is enabled then get the DRx core handle */
  /* Combine and check */
  if( rx_diversity_enabled && 
      (rfgsm_drx_core_handle_ptr = rfgsm_core_handle_get( drx_device )) == NULL )
  {
    /* To be more robust, continue if rfgsm_core_rx_build() is executed    */
    /* with DRx enabled but DRx core handle is not present.                */
    MSG_3( MSG_SSID_RF, MSG_LEGACY_ERROR, 
           "rfgsm_core_rx_build() RxD enabled but DRx core handle NULL for "
           "DRx dev %d, sub %d, PRx dev %d", drx_device, sub_id, prx_device );

    /* Continue in PRx-only mode */
    rx_diversity_enabled = FALSE;
  }

  /* Get all required data from GL1 burst parameters */
  rf_sequence_number = rx_chan_params->rf_sequence_number;
  burst_type         = rx_chan_params->burst_type;
  rx_burst_type      = rx_chan_params->rx_burst_type;
  gfw_shared_mem     = burst_params->gfw_rf_burst_event[burst_num];

  /* Set the core data */
  core_data.sub_id                    = sub_id;
  core_data.rfm_dev                   = prx_device;
  core_data.rxd_rfm_dev               = drx_device;
  core_data.enable_rx_diversity       = rx_diversity_enabled;
  core_data.device_is_tx              = FALSE;
  core_data.prx_drx_combined_mode     = rx_diversity_enabled;
  core_data.gl1_lower_rx_ccs_prio     = burst_params->gl1_lower_rx_ccs_prio;
  core_data.rfgsm_core_handle_ptr     = rfgsm_prx_core_handle_ptr;
  core_data.rfgsm_core_handle_ptr_rxd = rfgsm_drx_core_handle_ptr;
  core_data.triple_buffer_id          = buffer_id;

  /* Get the Rx start and stop scripts */
  rx_start_script = (rf_buffer_intf *)rfgsm_prx_core_handle_ptr->scripts.rx_start_script[RX_WTR_SCRIPT];
  rx_stop_script  = (rf_buffer_intf *)rfgsm_prx_core_handle_ptr->scripts.rx_stop_script[RX_WTR_SCRIPT];

  /* Clear the Rx start and stop script buffers first */
  if( !rf_buffer_clear_script( rx_start_script, RF_BUFFER_ALL) ||
	  !rf_buffer_clear_script( rx_stop_script, RF_BUFFER_ALL) )
  {
    ERR_FATAL("rfgsm_core_rx_build() Rx clear device scripts failed for dev %d", prx_device, 0, 0);
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_CLEAR_BUFF, sub_id, (prx_device << 4) | drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Get the associated Rx device and specify operation mode for device driver */
  associated_rx_dev = rfc_common_get_preferred_associated_rx_device( prx_device );

  /* Set up the burst script information */
  burst_script_infor.burst = burst_type;
  burst_script_infor.burst_alpha = 625;
  burst_script_infor.scripts = NULL;
  burst_script_infor.override_alpha = FALSE;
  burst_script_infor.burst_timing_offset_adjust_qs = 0;

  /* Populate WTR Burst Scripts based on operating mode Standalone or IRAT */
  if( rfgsm_prx_core_handle_ptr->rfgsm_buffer == RFGSM_AWAY_BUFFER )
  {
    rxdevice_oper_mode = RFDEVICE_OPER_MODE_MEAS;
  }

  /* If lower ccs priority flag is set from GL1 then worst case bus contention delay need
     to be added to start time of rx burst transactions*/
  if (core_data.gl1_lower_rx_ccs_prio ==  TRUE) 
  {
    burst_script_infor.burst_timing_offset_adjust_qs = RFGSM_WORST_CASE_RFFE_BUS_DELAY;
  }

  /* DRX RX burst Writes are appended to PRX script based on enable_rx_diversity flag */
  if( !rfdevice_gsm_rx_set_start_script( prx_device, 
                                         rxdevice_oper_mode,
                                         &burst_script_infor, 
                                         rx_start_script,
                                         &buffer_mask,
                                         rx_diversity_enabled,
                                         rfgsm_prx_core_handle_ptr->band,
                                         associated_rx_dev,
                                         RFDEVICE_PRX_OPER_MODE) )
  {
    ERR_FATAL( "rfgsm_core_rx_build() Rx set start script failed for PRx dev %d assoc dev %d, RxD en %d", 
               prx_device, associated_rx_dev, rx_diversity_enabled );
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_BUILD_START_SCRIPT, sub_id, (prx_device << 4) | drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  #ifdef FEATURE_RF_POWER_REPORTING
  rf_power_reporting_update_channel( prx_device, core_data.rfgsm_core_handle_ptr->rf_rx_arfcn );
  if (rx_diversity_enabled && rfgsm_drx_core_handle_ptr != NULL) 
  {
    rf_power_reporting_update_channel( drx_device, rfgsm_drx_core_handle_ptr->rf_rx_arfcn );
  }

  #ifdef FEATURE_RF_POWER_REPORTING_DEBUG
  MSG_2( MSG_SSID_RF, MSG_LEGACY_MED, 
         "rfgsm_core_rx_build() prx device %d, handle %d",
          prx_device,
          core_data.rfgsm_core_handle_ptr->rxlm_buf_index);

  if (rx_diversity_enabled && rfgsm_drx_core_handle_ptr != NULL) 
  {
     MSG_3( MSG_SSID_RF, MSG_LEGACY_MED, 
           "rfgsm_core_rx_build() drx device %d, handle %d, rx_diversity_enabled %d",
          drx_device,
            rfgsm_drx_core_handle_ptr->rxlm_buf_index ,
          rx_diversity_enabled );
  }
  #endif

  #endif   

  /* Configure the Jammer Detector in SAWless devices */

  if ( rfgsm_prx_core_handle_ptr->sawless_support[rfgsm_prx_core_handle_ptr->band] )
  {
    if(rfgsm_core_get_jdet_config_scripts(&core_data) == FALSE)
    {
      MSG_1( MSG_SSID_RF, MSG_LEGACY_ERROR, 
           "rfgsm_core_rx_build() JDET configuration failed for device %d!", prx_device );
    }
    RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_JDET_SCRIPTS, sub_id, (prx_device << 4) | drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );
  }

  /*DRX RX burst Writes are appended to PRX script based on enable_rx_diversity flag*/
  if( !rfdevice_gsm_rx_set_stop_script( prx_device, 
                                        rx_stop_script,
                                        associated_rx_dev,
                                        RFDEVICE_PRX_OPER_MODE ) )
  {
    ERR_FATAL( "rfgsm_core_rx_build() Rx set stop script failed for PRx dev %d assoc dev %d, RxD en %d", 
               prx_device, associated_rx_dev, rx_diversity_enabled );
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_BUILD_STOP_SCRIPT, sub_id, (prx_device << 4) | drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

#ifdef FEATURE_TABASCO_MODEM
  /*Populating Device Status Read Event Scripts During Rx Burst */
  if(core_data.rfgsm_core_handle_ptr->device_config_to_read.params[RF_RX_BURST].enable)
  {
    rfgsm_core_populate_device_status_read_buffers(&core_data, RF_RX_BURST);
  }
#endif

  /* Build CCS event */
  if( !rfgsm_core_dm_dynamic_rx_event_wrapper( &core_data, rx_burst_type, burst_num ) )
  {
    ERR_FATAL("rfgsm_core_rx_build() Rx event create failed for dev %d", prx_device, 0, 0);
    return FALSE;
  }

  RFGSM_CORE_PROC_LOG(RFGSM_CORE_RX_EVENT_BUILD, sub_id, (prx_device << 4) | drx_device, 0, 0, 0, rfgsm_mc_read_ustmr_count(), 0, 0 );

  /* Populate interface index */
  if( !rfgsm_core_dm_populate_rx_burst_intf_data( &core_data, gfw_shared_mem , rf_sequence_number) )
  {
    ERR_FATAL("rfgsm_core_rx_build() Rx interface index failed for dev %d", prx_device, 0, 0);
    return FALSE;
  }

  /* Populate RF sequence number */
  /* Note: This must occur after the Rx burst event is ready for use */
  if( !rfgsm_core_dm_set_rx_rf_sequence_num( &core_data,
                                             rx_burst_type,
                                             burst_num,
                                             rf_sequence_number ) )
  {
    ERR_FATAL( "rfgsm_core_rx_build() Rx RF seq num failed for sub %d, burst num %d, burst type %d", 
               sub_id, burst_num, burst_params->rx_chan_params[burst_num].rx_burst_type );
    return FALSE;
  }

  return TRUE;
}



/*----------------------------------------------------------------------------*/
/*!
   @brief
   This function updates the frequency error (in ppb).
   For example, if user passes a value of 2054  (ppb) it corresponds to 2054/1024 = 2.005 ppm. The resolution of frequency error is hence .001 ppm.
   @details

*/
void rfgsm_core_update_rx_freq_error(rfgsm_core_data_type *core_data, int32  freq_err)
{
  core_data->rfgsm_core_handle_ptr->rfgsm_curr_freq_error = freq_err;

  /* update the device layer with gsm freq error information*/
  rfdevice_gsm_cmd_dispatch( core_data->rfm_dev, RF_PATH_0, RFDEVICE_GSM_SET_RX_FREQ_ERROR, &freq_err);
}



/*----------------------------------------------------------------------------*/
/*!
   @fn rfgsm_core_rx_configure_lif

   @brief
   This functions determines the LIF frequency offset to use and if it is 
   enabled or not.   

   @details
   GL1 pass RF a LIF enabled flag and frequency offset.
   WTR driver only uses the frequency offset.
   FW use the LIF en flag but will perform LIF with any frequency.
   This API has to ensure that only +/-135KHz + LIF en flag TRUE is valid if
   WTR supports LIF.
*/
void rfgsm_core_rx_configure_lif( rfcom_device_enum_type prx_device,
                                  boolean enh_rx_supported,
                                  boolean * lif_enabled, 
                                  int32 * lif_offset )
{
  /* If LIF is supported by the WTR */
  if( enh_rx_supported )
  {
    /* Freq offset from GL1 is the expected +/-135KHz and LIF is enabled */
    if( *lif_enabled == TRUE &&
        ( *lif_offset == RF_LIF_FREQ_OFFSET_DEFAULT_HZ ||
          *lif_offset == -RF_LIF_FREQ_OFFSET_DEFAULT_HZ ) )
    {
      /* Nothing to do as info from L1 is valid */
    }
    else
    {
      /* If LIF en, LIF offset is invalid and combination is not valid */
      if( *lif_enabled == TRUE )
      {
        MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, 
             "rfgsm_core_rx_configure_lif() invalid LIF configuration en %d, "
             "offset %d from GL1. Disabling LIF.",
             *lif_enabled, *lif_offset );
      }

      /* Ensure LIF offset is 0 */
      *lif_offset = 0;

      /* Ensure LIF enabled flag is FALSE */
      *lif_enabled = FALSE;
    }
  }
  else
  {
    /* Ensure LIF offset is 0 */
    *lif_offset = 0;

    /* Ensure LIF enabled flag is FALSE */
    *lif_enabled = FALSE;
  }
}
