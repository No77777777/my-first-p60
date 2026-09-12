/*!
   @file
   rfgsm_core_meas.c

   @brief implements IRAT related functionality

   @details

*/

/*===========================================================================
Copyright (c) 2012 - 2017 by QUALCOMM Technologies Incorporated.
All Rights Reserved.

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/rftech_gsm.mpss/3.11/rf/core/src/rfgsm_core_meas.c#1 $
  $DateTime: 2021/03/09 05:10:17 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/03/17   bp      Use valid device info in rfgsm_mc_irat_info_get()
02/29/16   dsm     Changes for putting GSM front end components to sleep after X2G IRAT
02/29/16   dsm     Adding support for 1x2G measurement in IRAT parameter calculation
12/17/15   par     Use common iRAT buffer for X2G tuneaway trigger scripts
12/09/15   hkm     NV support to enable/disable LPM feature
10/22/15   cs      Ensure longer rx_alpha available for DR contention
09/11/15   hkm     Fixing issues with nested IRAT gaps for same sub/same device
08/05/15   sc      Deprecate alternate path
07/27/15   sw      FR25072: for device wakeup, vote for lowest/first supported band
07/23/15   cs      Allow for multiple meas exits for the same device
07/16/15   cs      Add bitmask tracking for out of sequence meas enter/exit
                   per device
07/15/15   sw      WTR power saving optimisations (under feature flag)
07/10/15   ndb     Remove the tuner RFFE disable
07/01/15   sw      Reduce the severity of irat F3 messages (error->med)
06/29/15   sw      Return power meas gamma to L1 (for IRAT X2G)
06/12/15   sw      Change TQP F3s to medium severity
06/10/15   sw/cs   Store worst case rx timings at power-up for irat
05/13/15   sw      Device for TQP allocation returned to GL1
05/12/15   rcui    changed voting to be based on device
05/08/15   sw      Use new IRAT DL path enums
05/06/15   sw      Remove RF_HAS_MEAS_INTF_TH_2_0 featurisation
04/10/15   zbz     Interface changes with device driver for split Rx/Tx
04/14/15   sw      Update irat_info_get to use new interface definitions - part2
04/14/15   sw      Update irat_info_get to use new interface definitions
03/30/15   sw      Remove legacy cross-wtr irat api
03/24/15   sw      Use new irat interface definitions (part2)
03/16/15   sw      Use new irat interface definitions
02/13/15   ec      G2W abort support added
01/26/15   sw      Resume therm reads during G2T iRAT
11/07/14   tsr     Fix Alternate path bug in IRAT due to GSM 1800_B band support
                   not available in RFC
11/07/14   piy     Adding Circular Buffer to catch iRAT call flows
10/23/14   sc      Enable event type control from RFSW
10/17/14   rr      Separate rx/tx alternate path core handles
09/12/14   sml     Adding start API Pcycle to know about pre-emption done to RF task
08/30/14   sw      Correct comments and update preload offset to 2500 for X2G irat
08/29/14   tws     Mainline alternate_path support
08/28/14   sc      Clean up Tx data initialisation to reduce wakeup timeline
08/21/14   piy     Adding Additional Info for Debugging GSM Issues
08/11/14   sc      Ensure chain update with alternate path and include IRAT
08/03/14   jr      Suspend temp comp during IRAT time.
07/31/14   tsr     GSM Alternate path support
07/28/14   hk      Enable preconfig/rx disable for SSBI based devices
06/12/14   sw      Use GL1 api to map sub_id to as_id
05/15/14   sc      Use subscription ID passed from GL1 in meas info params
05/14/14   cj/ck   Add Do2G support
04/16/14   sb      IP2 cal Interface and functionality changes
03/26/14   sw      Disable tuners during X2G iRAT scenarios (port from DI3)
03/25/14   sc      Store device in RFLM GSM DM data table for TQ management
03/24/14   sc      Add missing RFLM update for Rx alpha interface
03/05/14   zbz     Fix KW errors
02/28/14   sc      Pass core data through mc to core layers
02/24/14   tsr     Moved RX timing update to core layer
02/20/14   cj      Updated Rxlm index
02/18/14   cj      Abort changes for Bolt
02/17/14   sc      Clean up mDSP APIs
02/10/14   cj      rfgsm_buffer flag added to decide whether to use home/away buffer
02/03/14   cj      T2G preload offset time changed
01/16/14   cj      DM policy check added
01/10/14   cj      rxlm index updated for cleanup script
01/08/14   cj      DM buffer allocation removed from meas APIs
01/02/14   gvn     temp hack to support enqueue policy check
12/27/13   rp      Code changes to replace get critical section pointer with get common device
                   pointer for device comparison.
12/27/13   cj      Debug info updated and rf buffer clear added for build script
12/26/13   hm      Script buffer creation with tech scaling factor, to fix expected vs observed delays in scripts
12/16/13   cj      Code cleanup
12/10/13   cj      Added code to save script creation time
12/09/13   cj      Added time profiling and code optimization
11/27/13   cj      Added gsm tech memory allocation in irat_info_get
11/14/13   cj      Added changes for building preload and trigger script
11/12/13   dps     Updates to support C++ class based GSM Rx and Tx interface
11/07/13   cj      Preload offset time updated in irat_info_get
10/26/13   shb     Updated IRAT APIs to support preload
09/27/13   hk      Disable RX will now be script based instead of direct write.
09/20/13   hk      Removing the SGLTE feature check since all L1s have implemented new header interface.
09/17/13   hm      hooking up WCDMA C++ device driver interface for WTR
09/16/13   zg      Added adaptor obj construction due to new C++ TDSCDMA interface
08/07/13   sml     Fixing KW warning
07/22/13   ec      Use new F3s/Null pointer checks
06/30/13   cj      Trigger type changed to dynamic for G2X
05/23/13   cj      Changed trigger type to dynamic for X2G
05/03/13   cj      Added return status update after device driver code update
04/30/13   cj      Shadow back up and restore moved to IRAT framework
04/30/13   cj      Added changes to reset CCS event status to idle for abort
04/24/13   cj      Added changes to disable rx on second WTR after target meas
04/24/13   cj      Added pre_config function to support IRAT framework changes
04/19/13   cj      Added band enum conversion to preconf required
04/07/13   sc      Assign core rxlm index var with param passed from GL1
03/26/13   jyu     Added the change to allow override trigger type
03/19/13   cj      Removed source device pointer comparison
03/19/13   cj      Added support for LTE CA and W DBDC to G IRAT
02/22/13   cj      Updated rfgsm_core_meas_preconf_required for T2G startup
02/11/13   sc      Added GSM mode flag set to keep track of when in iRAT mode
01/22/13   cj      Change for ARAST-16830
01/18/13   cj      Updated rfgsm_core_meas_write_preconf_script function
01/17/13   cj      Added rfgsm_core_meas_write_preconf_script and
                   rfgsm_core_meas_preconf_required APIs
01/10/13   sc      Add abort mechanism to enter and exit
01/09/13   sc      Allow RXLM settings to not be re-written
01/03/13   sc      Set flag to indicate iRAT mode to ensure therm read not executed
12/20/12   sc      Ensure G2T cleanup buffer retrieved from source params
12/12/12   sc      Accept NULL parameter for LtoG tech_specific_info_param
12/03/12   sc      Cleanup mDSP shared variable interface
12/05/12   sc      Changes to use source params for LTE G->L Cleanup
11/28/12   aka     Changes to use rfgsm_core_handle_ptr from path based handler
11/27/12   sc      Provide only cleanup_time for G2W cleanup case.
11/26/12   aka     Changes to use a path specific gsm handle
11/21/12   av      Added rfm_device parameters to mc/core/mdsp/msm layers.
11/19/12   sc      RF-Only mDSP update shared variables cleanup
11/12/12   sc      Handle bad return from common RF CCS iRAT event creation
11/12/12   pl      remove rf_buffer_clear()
11/07/12   sc      Use common info params type to clean up interface
11/07/12   sc      Calculate the iRAT timings instead of explicitly defining
11/07/12   sc      Added Null poiter checks and assignment to l2g_irat_info from irat_info_param.
10/30/12   sc      Remove use of rfmeas_tech_params_type for W2G startup and G2W cleanup
10/09/12   sr      Added rfdevice and mdsp interface API calls.
10/04/12   sr      Initial version.

===========================================================================*/


#include "rfa_variation.h"
#include "rfcom.h"
#include "msg.h"
#include "rfdevice_cmn_intf.h"
#include "rfdevice_gsm_intf.h"
#include "rfgsm_core_types.h"
#include "rfgsm_msm.h"
#include "rfcommon_core_types.h"
#include "rfc_card_gsm.h"
#include "rfgsm_core.h"
#include "rfgsm_core_temp_comp.h"
#include "rfgsm_core_util.h"
#include "rfgsm_core_meas.h"
#include "rf_hal_buffer.h"
#include "rfmeas_types.h"
#include "rfmeas_mdsp.h"

#include "rfm_internal.h"
#include "rfdevice_trx_gsm_rx.h"
#include "rfdevice_trx_wcdma_rx.h"
#include "rfgsm_data_manager.h"
#include "rflm_dm_api.h"
#include "rflm_meas.h"
#include "gl1_hw.h"
#include "geran_test_mode_api.h"
#include "rfgsm_core.h"
#include "rfcommon_core_device_manager.h"
#include "rfcommon_nv_mm.h"
#ifdef FEATURE_WCDMA
#include "rfdevice_wcdma_type_defs.h"
extern void** rfc_wcdma_get_device
(
  rfm_device_enum_type rfm_dev,
  rfc_rxtx_enum_type rx_tx,
  rfcom_wcdma_band_type band,
  rfdevice_type_enum_type dev_type
);
#endif

#ifdef FEATURE_LTE
#include "rfdevice_lte_type_defs.h"
#include "rfdevice_lte_interface.h"
extern void** rfc_lte_get_device
(
   rfm_device_enum_type rfm_dev,
   rfc_rxtx_enum_type rx_tx,
   rfcom_lte_band_type band,
   rfdevice_type_enum_type dev_type
);
#endif /* #ifdef FEATURE_LTE */


#ifdef FEATURE_TDSCDMA
#include "rfdevice_tdscdma_type_defs.h"
#include "rfdevice_trx_tdscdma_rx.h"
extern void** rfc_tdscdma_get_rf_device_obj
(
  rfm_device_enum_type rfm_dev, rfc_rxtx_enum_type rx_tx,
  rfcom_tdscdma_band_type band, rfdevice_type_enum_type dev_type
);
#include "rfdevice_tdscdma_intf.h"
#endif

#define RFGSM_CORE_IRAT_BUFFER_SIZE 500

typedef struct
{
  rfcom_mode_enum_type src_tech;
  rfcom_mode_enum_type target_tech;
  unsigned long long pcycle;
  rfgsm_core_script_type script_type;
}rfgsm_core_irat_logging;

rfgsm_core_irat_logging rfgsm_core_irat_buffer[RFGSM_CORE_IRAT_BUFFER_SIZE];
uint32 rfgsm_core_irat_buffer_index = 0;

#define RFGSM_CORE_MAX_VALID_SUB_ID 2

/* General bitmask control macros */
#define SET_ACTIVE_BITMASK( bIT_mASK, bIT_cTL ) \
  ( bIT_mASK ) |= ( 1 << ( bIT_cTL ) )

#define CLR_ACTIVE_BITMASK( bIT_mASK, bIT_cTL ) \
  ( bIT_mASK ) &= ~( 1 << ( bIT_cTL ) )

#define IS_ACTIVE_BIT_SET( bIT_mASK, bIT_cTL ) \
  ( ( bIT_mASK ) & ( 1 << ( bIT_cTL ) ) )

/* Bitmask and count used to track iRAT enter/exit pairs per device and sub id */
uint8 rfgsm_core_irat_active_sub_bitmask[RFM_MAX_WAN_DEVICES] = { 0 };
uint8 rfgsm_core_irat_active_cnt[RFM_MAX_WAN_DEVICES]         = { 0 };

/* Used to track bit setting for iRAT meas enter/exit per device */
#define SET_IRAT_ACTIVE_SUB_MEAS_BITMASK( rFM_dEVICE, sUB_iD )                    \
  SET_ACTIVE_BITMASK( rfgsm_core_irat_active_sub_bitmask[( rFM_dEVICE )], ( sUB_iD ) )

#define CLR_IRAT_ACTIVE_SUB_MEAS_BITMASK( rFM_dEVICE, sUB_iD )                    \
  CLR_ACTIVE_BITMASK( rfgsm_core_irat_active_sub_bitmask[( rFM_dEVICE )], ( sUB_iD ) )

#define IS_IRAT_ACTIVE_SUB_MEAS_BIT_SET( rFM_dEVICE, sUB_iD )                     \
  IS_ACTIVE_BIT_SET( rfgsm_core_irat_active_sub_bitmask[( rFM_dEVICE )], ( sUB_iD  ) )

/* This will show if any sub has an active vote for this device */
#define IS_IRAT_MEAS_DEVICE_STILL_NEEDED( rFM_dEVICE )                     \
  ( rfgsm_core_irat_active_sub_bitmask[( rFM_dEVICE )] != 0 )

#define RFGSM_CORE_IRAT_Q(s , t, p, e)                                                            \
{                                                                                                 \
  rfgsm_core_irat_buffer_index = rfgsm_core_irat_buffer_index % RFGSM_CORE_IRAT_BUFFER_SIZE;      \
  rfgsm_core_irat_buffer[rfgsm_core_irat_buffer_index].src_tech= s;                               \
  rfgsm_core_irat_buffer[rfgsm_core_irat_buffer_index].target_tech= t;                            \
  rfgsm_core_irat_buffer[rfgsm_core_irat_buffer_index].pcycle= p;                                 \
  rfgsm_core_irat_buffer[rfgsm_core_irat_buffer_index].script_type = e;                           \
  rfgsm_core_irat_buffer_index ++;                                                                \
}

typedef struct
{
  rf_time_tick_type build_script_start_t;
  rf_time_type build_script_time;
  rf_time_tick_type sw_init_start_t;
  rf_time_type sw_init_time;
  rf_time_tick_type transceiver_cleanup_start_t;
  rf_time_type transceiver_cleanup_total_time;
  rf_time_tick_type transceiver_setup_start_t;
  rf_time_type transceiver_setup_total_time;
  rf_time_tick_type transceiver_script_start_t;
  rf_time_type transceiver_script_time;
  rf_time_tick_type msm_script_start_t;
  rf_time_type msm_script_time;
  rf_time_tick_type mdsp_script_start_t;
  rf_time_type mdsp_script_time;

} rfgsm_meas_build_script_profile_type;

static rfgsm_meas_build_script_profile_type gsm_build_script_profiling;

extern void rfgsm_core_enable_hal_bus( boolean enable, uint8 sub_id );
extern void rfgsm_core_turn_on_vregs(rf_path_enum_type path, uint8 sub_id, rfm_device_enum_type device);
extern void rfgsm_core_turn_off_vregs(rf_path_enum_type path, uint8 sub_id, rfm_device_enum_type device);

static void gsm_meas_clear_profiling_data( void )
{
  memset(&gsm_build_script_profiling,0,sizeof(rfgsm_meas_build_script_profile_type));
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
rfm_meas_result_type rfgsm_core_irat_info_get(rfm_meas_irat_info_type *irat_info_param,rfm_device_enum_type rfm_dev)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfcom_mode_enum_type src_tech = RFM_INVALID_MODE;
  rfcom_mode_enum_type target_tech = RFM_INVALID_MODE;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfc_gsm_core_config_type *rfc_gsm_core_ptr = NULL;
  rfm_meas_xtoy_irat_info_param_type * x2y_irat_info = NULL;

  if ( !irat_info_param )
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Pointer irat_info_param is NULL!", 0);
    return result;
  }

  src_tech = irat_info_param->header.source_tech;
  target_tech = irat_info_param->header.target_tech;

  x2y_irat_info = (rfm_meas_xtoy_irat_info_param_type *)irat_info_param->irat_info_param;

  if( !x2y_irat_info )
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Pointer irat_info_param->irat_info_param is NULL!", 0);
    return result;
  }

  if(target_tech == RFCOM_GSM_MODE)
  {
/*/    rfm_dev = irat_info_param->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;*/
  }
  else if(src_tech == RFCOM_GSM_MODE)
  {
/*    rfm_dev = irat_info_param->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;*/

  /* Preload offset time is initialized to invalid value for G2X */
    irat_info_param->nbr_pretune_offset_time = 0;
    irat_info_param->home_pretune_offset_time = 0;
  }
  else
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "GSM is not source or target tech, source_tech is %d, target tech is %d",
      src_tech,
      target_tech);
  result = RFM_MEAS_COMMON_FAILURE;
  return result;
  }

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  if (rfgsm_core_handle_ptr == NULL)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_handle_ptr is NULL!", 0);
    return result;
  }

  rfc_gsm_core_ptr = rfgsm_core_handle_ptr->rfc_gsm_core_ptr;
  if( rfc_gsm_core_ptr == NULL )
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"gsm rfc_ptr is NULL! Rx PLL Tune Time could not be acquired", 0);
    return result;
  }

  /*********************** X2G Startup and PowerMeas *************************/
  // W2G
  if (src_tech == RFCOM_WCDMA_MODE && target_tech == RFCOM_GSM_MODE)
  {
    /* startup max sbi: 38 SBIs = 99QS = 91us */
    x2y_irat_info->start_duration = RFGSM_STARTUP_TIME_CALC_QS(WTOG_STARTUP_SBI_MAX);
    irat_info_param->startup_time = x2y_irat_info->start_duration;

    /*G2G SBI+ PLL = 99QS+163QS (assuming 150us PLL) = 262QS = 242us*/
    x2y_irat_info->power_meas_setup_duration = RFGSM_PWR_MEAS_TIME_CALC_QS(XTOG_PWR_MEAS_SBI_MAX,
                                                                           rfc_gsm_core_ptr->rx_timing->pll_settling_time_qs);
    /* Time when gsm_preload_script should be preloaded to WXE */
    /* W2G max preload RFFE: 480 us, for DSDS 480*2 + overhead = 2500 us */
    irat_info_param->nbr_pretune_offset_time = RFGSM_PRELOAD_OFFSET_TIME_CALC_QS(2500);

    /* Time from last pwr meas to ccs release = 120qs */
    x2y_irat_info->power_meas_gamma = RFGSM_PWR_MEAS_GAMMA_QS;
  }
  // L2G
  else if (src_tech == RFCOM_LTE_MODE && target_tech == RFCOM_GSM_MODE)
  {
    /* startup max sbi: 150 SBIs = 390QS = 360us */
    x2y_irat_info->start_duration = RFGSM_STARTUP_TIME_CALC_QS(LTOG_STARTUP_SBI_MAX);
    irat_info_param->startup_time = x2y_irat_info->start_duration;

    /*G2G SBI+ PLL = 99QS+163QS (assuming 150us PLL) = 262QS = 242us*/
    x2y_irat_info->power_meas_setup_duration = RFGSM_PWR_MEAS_TIME_CALC_QS(XTOG_PWR_MEAS_SBI_MAX,
                                                                           rfc_gsm_core_ptr->rx_timing->pll_settling_time_qs);
    /* Time when gsm_preload_script should be preloaded to WXE */
    /* L2G max preload RFFE: 480 us, for DSDS 480*2 + overhead = 2500 us */
    irat_info_param->nbr_pretune_offset_time = RFGSM_PRELOAD_OFFSET_TIME_CALC_QS(2500);

    /* Time from last pwr meas to ccs release = 120qs */
    x2y_irat_info->power_meas_gamma = RFGSM_PWR_MEAS_GAMMA_QS;
  }
  //Do2G

  else if (((src_tech == RFCOM_1XEVDO_MODE) || (src_tech == RFCOM_1X_MODE)) && target_tech == RFCOM_GSM_MODE)
  {
    /* startup max sbi: 150 SBIs = 390QS = 360us */
    x2y_irat_info->start_duration = RFGSM_STARTUP_TIME_CALC_QS(DOTOG_STARTUP_SBI_MAX);
    irat_info_param->startup_time = x2y_irat_info->start_duration;

    /*G2G SBI+ PLL = 99QS+163QS (assuming 150us PLL) = 262QS = 242us*/
    x2y_irat_info->power_meas_setup_duration = RFGSM_PWR_MEAS_TIME_CALC_QS(XTOG_PWR_MEAS_SBI_MAX,
                                                                           rfc_gsm_core_ptr->rx_timing->pll_settling_time_qs);
    /* Time when gsm_preload_script should be preloaded to WXE */
    /* Do2G max preload RFFE: 480 us, for DSDS 480*2 + overhead = 2500 us */
    irat_info_param->nbr_pretune_offset_time = RFGSM_PRELOAD_OFFSET_TIME_CALC_QS(2500);

    /* Time from last pwr meas to ccs release = 120qs */
    x2y_irat_info->power_meas_gamma = RFGSM_PWR_MEAS_GAMMA_QS;
  }
  // T2G
  else if (src_tech == RFCOM_TDSCDMA_MODE && target_tech == RFCOM_GSM_MODE)
  {
    /* startup max sbi: 65 SBIs = 177QS = 163us */
    x2y_irat_info->start_duration = RFGSM_STARTUP_TIME_CALC_QS(TTOG_STARTUP_SBI_MAX);
    irat_info_param->startup_time = x2y_irat_info->start_duration;

    /*G2G SBI+ PLL = 99QS+163QS (assuming 150us PLL) = 262QS = 242us*/
    x2y_irat_info->power_meas_setup_duration = RFGSM_PWR_MEAS_TIME_CALC_QS(XTOG_PWR_MEAS_SBI_MAX,
                                                                           rfc_gsm_core_ptr->rx_timing->pll_settling_time_qs);
    /* Time when gsm_preload_script should be preloaded to WXE */
    /* T2G preload will be handled by RF */
    irat_info_param->nbr_pretune_offset_time = RFGSM_PRELOAD_OFFSET_TIME_CALC_QS(2500);

    /* Time from last pwr meas to ccs release = 120qs */
    x2y_irat_info->power_meas_gamma = RFGSM_PWR_MEAS_GAMMA_QS;
  }

  /***************************** G2X Cleanup *********************************/
  // G2W
  else if (src_tech == RFCOM_GSM_MODE && target_tech == RFCOM_WCDMA_MODE)
  {
    /*38 sbis for cleanup to go back to GSM mode in qs*/
    irat_info_param->cleanup_time = RFGSM_CLEANUP_TIME_CALC_QS(WTOG_CLEANUP_SBI_MAX);
  }
  // G2L
  else if (src_tech == RFCOM_GSM_MODE && target_tech == RFCOM_LTE_MODE)
  {
    /*150 sbis for cleanup to go back to GSM mode in qs*/
    x2y_irat_info->cleanup_duration = RFGSM_CLEANUP_TIME_CALC_QS(LTOG_CLEANUP_SBI_MAX);
    irat_info_param->cleanup_time = x2y_irat_info->cleanup_duration;
  }
  // G2T
  else if (src_tech == RFCOM_GSM_MODE && target_tech == RFCOM_TDSCDMA_MODE)
  {
    /*65 sbis for cleanup to go back to GSM mode in qs*/
    x2y_irat_info->cleanup_duration = RFGSM_CLEANUP_TIME_CALC_QS(TTOG_CLEANUP_SBI_MAX);
    irat_info_param->cleanup_time = x2y_irat_info->cleanup_duration;
  }

  /* All X2G accounts for RFFE bus contention in DR-DSDS mode */
  if ( target_tech == RFCOM_GSM_MODE )
  {
    x2y_irat_info->power_meas_alpha_rffe =
      ( x2y_irat_info->power_meas_setup_duration + RFGSM_WORST_CASE_RFFE_BUS_DELAY );
  }

  return ( result );
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function will populate all the Rx Alpha timing information. This
    timing information will be used by FW scheduling the measurements during iRAT gap.

  @param rfm_meas_irat_info_type: data structure for all info to be returned

  @pre
  rfm_init() must have already been called.
  We should be in a certain technology.

  @retval
  Returns an enum type (rfm_meas_result_type) denoting whether the function did work
  correctly or not.
*/
rfm_meas_result_type rfgsm_core_irat_update_rx_timings(rfm_meas_irat_info_type *irat_info_param,rfm_device_enum_type rfm_dev)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_SUCCESS;
  rfgsm_core_data_type rfgsm_core_data;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;

  if (irat_info_param == NULL)
  {
    result = RFM_MEAS_COMMON_FAILURE;
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Pointer irat_info_param is NULL!", 0);
    return result;
  }

  if (irat_info_param->tech_specific_info_param == NULL)
  {
      result = RFM_MEAS_COMMON_FAILURE;
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"Pointer tech_specific_info_param is NULL!", 0);
      return result;
    }

  /* Get the core handle ptr */
  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  if( rfgsm_core_handle_ptr == NULL )
  {
    RF_MSG_3(MSG_LEGACY_ERROR,
             "NULL core handle ptr for source_tech %d, target tech %d, dev %d",
             irat_info_param->header.source_tech, irat_info_param->header.target_tech, rfm_dev);
    result = RFM_MEAS_COMMON_FAILURE;
    return result;
  }

  if(irat_info_param->sub_id >= RFGSM_CORE_MAX_VALID_SUB_ID)
  {
    RF_MSG_4(MSG_LEGACY_ERROR,
             "Invalid sub ID %d for source_tech %d, target tech %d, dev %d",
             irat_info_param->sub_id, irat_info_param->header.source_tech, irat_info_param->header.target_tech, rfm_dev);
    result = RFM_MEAS_COMMON_FAILURE;
    return result;
  }

  /* Set the core data */
  rfgsm_core_data.rf_task = RF_MEAS_CALL;
  rfgsm_core_data.rfm_dev = rfm_dev;
  rfgsm_core_data.sub_id = irat_info_param->sub_id; //Default until support available if required
  rfgsm_core_data.as_id = gl1_hw_rf_map_rf_sub_id_to_as_id(irat_info_param->sub_id);
  rfgsm_core_data.triple_buffer_id = RFLM_GSM_BUFFER_ID_NOT_REQUIRED;
  rfgsm_core_data.rfgsm_core_handle_ptr = rfgsm_core_handle_ptr;

  /* update the timing related shared variables */
  rfgsm_core_update_rx_timing(&rfgsm_core_data);
  if( !rfgsm_core_dm_populate_enter_mode_intf_data(&rfgsm_core_data,
                                                   irat_info_param->tech_specific_info_param ) )
  {
    RF_MSG_3(MSG_LEGACY_ERROR,
             "Populate enter mode intf data for IRAT failed source_tech %d, target tech %d, dev %d",
             irat_info_param->header.source_tech, irat_info_param->header.target_tech, rfm_dev);
    result = RFM_MEAS_COMMON_FAILURE;
    return result;
  }

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
rfm_meas_result_type rfgsm_core_meas_enter( rfm_meas_common_param_type *meas_enter_param )
{
  uint8 sub_id;

  rfm_device_enum_type src_rfm_dev = RFM_DEVICE_0, rfm_dev = RFM_DEVICE_0;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfcom_mode_enum_type src_tech = meas_enter_param->header.source_tech;
  rfcom_mode_enum_type tgt_tech = meas_enter_param->header.target_tech;
  rfm_meas_program_script_type script_type = meas_enter_param->script_type;
  rf_path_enum_type rf_path;

  /* In the case of NULL2X, always use target device */
  if( (tgt_tech == RFCOM_GSM_MODE) || (script_type == RFM_MEAS_PROGRAM_NULL2X) )
  {
    rfm_dev = meas_enter_param->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;

    sub_id = meas_enter_param->target_param.gsm_params.sub_id;
  }
  else if(src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = meas_enter_param->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;

    sub_id = meas_enter_param->source_param.gsm_params.sub_id;
  }
  else
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_core_meas_enter: GSM is not source or target tech, source_tech is %d, target tech is %d",
          src_tech,
          tgt_tech);
    return RFM_MEAS_COMMON_FAILURE;
  }

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_meas_enter: rfgsm_core_handle_ptr is NULL!", RFM_MEAS_COMMON_FAILURE);

  if(tgt_tech == RFCOM_GSM_MODE)
  {
    rfgsm_core_handle_ptr->rfgsm_buffer = RFGSM_AWAY_BUFFER;

    /* Return device to GL1 for TQP allocation */
    /* Only need to do this for X2G IRAT - for G2X, cleanup uses SW port (TQ4) */

      /* If source and target devices are both valid and different => x-wtr IRAT */
      /* For x-wtr/x-device IRAT: return src device for TQP allocation */
      /* For all other scenarios: return tgt device for TQP allocation */
      src_rfm_dev = meas_enter_param->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
      if ( ( RFM_INVALID_DEVICE != src_rfm_dev ) && ( src_rfm_dev != rfm_dev ) ) /* X-WTR/X-DEVICE IRAT */
      {
        meas_enter_param->target_param.gsm_params.tqp_dev = src_rfm_dev;
      MSG_4(MSG_SSID_RF, MSG_LEGACY_MED, "rfgsm_core_meas_enter: Cross-WTR/Cross-Device X2G IRAT,"
                                             " src tech is %d, tgt tech is %d,"
                                         " irat dev is %d, tqp dev is %d",
                                             src_tech, tgt_tech, rfm_dev, meas_enter_param->target_param.gsm_params.tqp_dev );
      }
      else /* All other scenarios (Normal IRAT, QTA, QCTA, QDTA) */
      {
        meas_enter_param->target_param.gsm_params.tqp_dev = rfm_dev;
      MSG_5(MSG_SSID_RF, MSG_LEGACY_MED, "rfgsm_core_meas_enter: Legacy X2G IRAT/QTA/QXTA,"
                                             " src tech is %d, tgt tech is %d,"
                                         " irat dev is %d, tqp dev is %d, rfgsm_core_irat_active_cnt %d",
                                             src_tech, tgt_tech, rfm_dev,
                                             meas_enter_param->target_param.gsm_params.tqp_dev, rfgsm_core_irat_active_cnt[rfm_dev] );
      }
    }
  else
  {
    rfgsm_core_handle_ptr->rfgsm_buffer = RFGSM_HOME_BUFFER;
  }

  /* Set to NOT GSM Standalone mode */
  /* Ensures that temp comp is not done during iRAT */
  rfgsm_core_handle_ptr->rfgsm_mode = RFGSM_IRAT_MODE;

  /* Use bitmask tracking to detect nested enter for same device */
  if ( IS_IRAT_MEAS_DEVICE_STILL_NEEDED( rfm_dev ) )
  {
    MSG_4( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "rfgsm_core_meas_enter: Requested rfm_dev:%d for sub_id:%d"
           " enabled bitmask[%d]:0x%x",
           rfm_dev, sub_id, rfm_dev, rfgsm_core_irat_active_sub_bitmask[rfm_dev] );
  }
  else
  {
    /* For NULL2G, must turn on devices unless already on */
    if( script_type == RFM_MEAS_PROGRAM_NULL2X )
    {
      /* Get logical path */
      rf_path = rfcommon_core_device_to_path(rfm_dev);

      /* Hal Voting (TQ4) */
      rfgsm_core_enable_hal_bus( TRUE, sub_id );

      /* Turn the vregs ON */
      rfgsm_core_turn_on_vregs(rf_path, sub_id, rfm_dev);

      /* Enable Device Power Voting */
      if( !rfcommon_core_dev_power_vote(rfm_dev, RFCMN_CORE_DEV_ON, RFCMN_CORE_DEV_NORMAL_VOTE) )
      {
        RF_MSG( RF_FATAL, "rfgsm_core_meas_enter(): Device Pwr Up failed()");
      }
    }

    /************************************/
    /* New power saving API (LPM -> ON) */
    /************************************/

  //check RFNV_SPARE_1_I value to know LPM feature
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
     /* For X2G, must make sure device is powered on for G */
     if (tgt_tech == RFCOM_GSM_MODE)
     {
       /* Enable RX Device (LPM -> ON) for lowest supported band */
       if ( !rfdevice_gsm_rx_pwr_wakeup( rfm_dev, rfgsm_nv_get_lowest_supported_band(rfm_dev, RFGSM_NV_RX) ) )
       {
         ERR_FATAL( "rfgsm_core_meas_enter: RX Device %d Pwr Up (LPM->ON) failed for band %d",
                   rfm_dev, rfgsm_nv_get_lowest_supported_band(rfm_dev, RFGSM_NV_RX), 0 );
         return FALSE;
       }
     }
   }
  }

  /* Set device control bitmask */
  SET_IRAT_ACTIVE_SUB_MEAS_BITMASK( rfm_dev, sub_id );
  rfgsm_core_irat_active_cnt[rfm_dev]++;

  /* Suspend thermistor reads on this device for G2X */
  if (rfgsm_core_handle_ptr->rfgsm_core_temp_comp_started )
  {
    rfgsm_core_temp_comp_suspend(rfm_dev);
  }

  return RFM_MEAS_COMMON_SUCCESS;
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
rfm_meas_result_type rfgsm_core_meas_build_scripts( rfm_meas_common_param_type *meas_scripts_param)
{
  uint8 sub_id;

  rfcom_mode_enum_type src_tech = meas_scripts_param->header.source_tech;
  rfcom_mode_enum_type target_tech = meas_scripts_param->header.target_tech;
  rfm_meas_param_v2_type *src_params = (rfm_meas_param_v2_type*)meas_scripts_param->header.source_param_v2;
  rfm_meas_param_v2_type *tgt_params = (rfm_meas_param_v2_type*)meas_scripts_param->header.target_param_v2;
  rfdevice_gsm_meas_param_type gsm_device_param;
  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  rf_buffer_intf *rf_buffer = NULL;
  rf_buffer_intf *rf_preload_buffer = NULL, *rf_trigger_buffer = NULL;
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfdevice_meas_script_data_type meas_script_data;
  boolean return_bool = FALSE;
  boolean create_new_script = FALSE;
  boolean display_profiling = FALSE;
  uint16 temp_preload_trans = 0, temp_trx_trans = 0;
  unsigned long long start_pcycle = qurt_get_core_pcycles();
  uint32 rxlm_buf_idx = 0;
  rfm_meas_program_script_type script_type = meas_scripts_param->script_type;

#ifdef G_POLICY_CHECK_EN
  uint16 buf_mask = 0;
  rfmeas_mdsp_policy_check_params_s policy_check;

  rflm_meas_resource_entry_s override_params = { 0 };
  uint32 override_bitmask = 0, override_err_code = 0;
#endif

  gsm_meas_clear_profiling_data();
  gsm_build_script_profiling.build_script_start_t = rf_time_get_tick();

  if ( (target_tech == RFCOM_GSM_MODE) || (script_type == RFM_MEAS_PROGRAM_NULL2X) )
  {
    rfm_dev = tgt_params->rx_dev_handle[DEV_HANDLE_PRX].device;

    sub_id = meas_scripts_param->target_param.gsm_params.sub_id;
  }
  else if(src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = src_params->rx_dev_handle[DEV_HANDLE_PRX].device;

    sub_id = meas_scripts_param->source_param.gsm_params.sub_id;
  }
  else
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_core_meas_build_scripts: GSM is not source or target tech, source_tech is %d, target tech is %d",
          src_tech,
          target_tech);
    return RFM_MEAS_COMMON_FAILURE;
  }

  /* Sanity check if the device ENTER mode has been called first */
  if ( ( rfm_dev < RFM_MAX_WAN_DEVICES ) &&
       ( sub_id < RFGSM_CORE_MAX_VALID_SUB_ID ) )
  {
    if ( !IS_IRAT_ACTIVE_SUB_MEAS_BIT_SET( rfm_dev, sub_id ) )
    {
      MSG_4( MSG_SSID_RF, MSG_LEGACY_HIGH,
             "rfgsm_core_meas_build_scripts: Requested script building rfm_dev:%d for sub_id:%d"
             " before rfgsm_core_meas_enter called active bitmask[%d]:0x%x",
             rfm_dev, sub_id, rfm_dev, rfgsm_core_irat_active_sub_bitmask[rfm_dev] );

      return RFM_MEAS_COMMON_FAILURE;
    }
  }

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_meas_build_scripts: rfgsm_core_handle_ptr is NULL!", RFM_MEAS_COMMON_FAILURE);

  gsm_device_param.source_tech = src_tech;
  gsm_device_param.source_band = src_params->band;
  gsm_device_param.target_tech = target_tech;
  gsm_device_param.target_band = tgt_params->band;

   // W2G, L2G and T2G StartUp Script
  if ( (target_tech == RFCOM_GSM_MODE) &&
     ( (script_type == RFM_MEAS_PROGRAM_STARTUP) || (script_type == RFM_MEAS_PROGRAM_NULL2X) ) )
  {
    RFGSM_CORE_IRAT_Q(src_tech, target_tech, start_pcycle, RFGSM_CORE_IRAT_STARTUP);

    /* For X2G, retrieve the rxlm index for neighbour cell */
    rxlm_buf_idx = (uint32)(tgt_params->rx_dev_handle[DEV_HANDLE_PRX].lm_handle);

    MSG_6(MSG_SSID_RF, MSG_LEGACY_MED, "rfgsm_core_meas_build_scripts STARTUP: src_tech %d, tgt_tech %d, src_band %d, tgt band %d, "
                                         "rxlm_id %d, script_type=%d",
          src_tech, target_tech, gsm_device_param.source_band,
          gsm_device_param.target_band, rxlm_buf_idx, script_type);

    /* Optmized call flow for building script */
    //rf_buffer = (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.meas_nontrx_script;
    //rf_trigger_buffer = (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.meas_trigger_script;
    /* Use common iRAT buffers for script population */
    rf_trigger_buffer = rfmeas_mdsp_buffer_get(RFM_MEAS_PROGRAM_TRIGGER);
    rf_preload_buffer = (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.enter_mode_script;

    /* Clear trigger buffer only for NULL2X. For valid X2G cases, source tech reset scripts 
       would be present in trigger buffer so shouldnt clear the trigger buffer */
    if (script_type == RFM_MEAS_PROGRAM_NULL2X)
    {
      rf_buffer_clear(rf_trigger_buffer);
    }
    rf_buffer_clear(rf_preload_buffer);

    // get the rf_buffer to use in building the scripts based on script type.

    rf_buffer = rfmeas_mdsp_buffer_get(RFM_MEAS_PROGRAM_STARTUP);

    meas_script_data.preload_action = RFDEVICE_MEAS_CREATE_SCRIPT;
    meas_script_data.preload_buf = rf_preload_buffer;
    meas_script_data.meas_action = RFDEVICE_MEAS_CREATE_SCRIPT;
    meas_script_data.meas_buf = rf_trigger_buffer;

    create_new_script = TRUE;
    temp_preload_trans = rf_buffer_get_num_trans(meas_script_data.preload_buf, RF_BUFFER_RFFE);
    temp_trx_trans = rf_buffer_get_num_trans(meas_script_data.meas_buf, RF_BUFFER_RFFE);

    /* optimized call flow for building rf script */
    if ((temp_preload_trans > 0)&&(temp_trx_trans > 0))
    {

      MSG_2(MSG_SSID_RF, MSG_LEGACY_MED,"rfgsm_core_meas_build_scripts: Preload %d & Trigger %d script is already created!",
            temp_preload_trans,temp_trx_trans);

      create_new_script = TRUE;
    }

    gsm_build_script_profiling.transceiver_script_start_t = rf_time_get_tick();

    if(create_new_script)
    {
      /* setup rf device to make GSM measurements */
      return_bool = rfdevice_gsm_build_meas_script(rfm_dev, gsm_device_param, &meas_script_data);
      if( return_bool == FALSE )
      {
        MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_meas_build_scripts: Error in wtr script creation!", 0);
        return RFM_MEAS_COMMON_FAILURE;
      }
    } // ^ create_new_script

    gsm_build_script_profiling.transceiver_script_time = rf_time_get_elapsed(gsm_build_script_profiling.transceiver_script_start_t,RF_USEC);
    gsm_build_script_profiling.msm_script_start_t = rf_time_get_tick();

    /* Setup RFC (rf-front-end devices) to make GSM measurements  */
    rfc_gsm_mode_config(rfm_dev, rf_buffer, RFDEVICE_PRX_OPER_MODE, RFGSM_ALT_PATH_NOT_SUPPORTED);

    /* Only program the RXLM buffer if L1 has not set the neighbor LM idx to invalid */
    if( rxlm_buf_idx != RXLM_INVALID_BUFFER )
    {
      /* Set the core handle var for Rx meas */
      rfgsm_core_handle_ptr->rxlm_buf_index = rxlm_buf_idx;

      /* setup modem baseband to make GSM measurements */
      rfgsm_msm_init( rxlm_buf_idx, rfm_dev, FALSE, RFGSM_ALT_PATH_NOT_SUPPORTED );
    }

    gsm_build_script_profiling.msm_script_time = rf_time_get_elapsed(gsm_build_script_profiling.msm_script_start_t,RF_USEC);

    /* update the FW shared mem with rf_device and RFC settings */
    gsm_build_script_profiling.mdsp_script_start_t = rf_time_get_tick();

#ifdef G_POLICY_CHECK_EN
    policy_check.buff_mask = meas_script_data.buffer_mask;
    policy_check.channel = 1;
    policy_check.rf_band = 1;

    return_bool = rfmeas_mdsp_meas_write_ccs_irat_script_v2( rxlm_buf_idx,
                                                             RFM_MEAS_PROGRAM_STARTUP,
                                                             meas_script_data.preload_buf,
                                                             RFLM_MEAS_DM_PRELOAD_SCRIPT,
                                                             meas_script_data.meas_buf,
                                                             RFLM_MEAS_DM_TRIGGER_SCRIPT,
                                                             rf_buffer,
                                                             RFLM_MEAS_DM_NONTRX_SCRIPT,
                                                             FALSE /* combine_trigger_non_trx */,
                                                             &policy_check );
#else

    return_bool = rfmeas_mdsp_meas_write_ccs_irat_script_v2( rxlm_buf_idx,
                                                             RFM_MEAS_PROGRAM_STARTUP,
                                                             meas_script_data.preload_buf,
                                                             RFLM_MEAS_DM_PRELOAD_SCRIPT,
                                                             meas_script_data.meas_buf,
                                                             RFLM_MEAS_DM_TRIGGER_SCRIPT,
                                                             rf_buffer,
                                                             RFLM_MEAS_DM_NONTRX_SCRIPT,
                                                             FALSE /* combine_trigger_non_trx */,
                                                             NULL );
#endif

    if( return_bool == FALSE )
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_meas_build_scripts: Error in GSM startup iRAT event creation!", 0);
      return RFM_MEAS_COMMON_FAILURE;
    }

    gsm_build_script_profiling.mdsp_script_time = rf_time_get_elapsed(gsm_build_script_profiling.mdsp_script_start_t,RF_USEC);
  } // ^ target_tech == RFCOM_GSM_MODE && script_type == RFM_MEAS_PROGRAM_STARTUP
  // CleanUp script
  else if (src_tech == RFCOM_GSM_MODE)
  {
    RFGSM_CORE_IRAT_Q(src_tech, target_tech, start_pcycle, RFGSM_CORE_IRAT_CLEANUP);

    /* For GX, retrieve the rxlm index for serving cell */
    rxlm_buf_idx = (uint32)(src_params->rx_dev_handle[DEV_HANDLE_PRX].lm_handle);

    MSG_6( MSG_SSID_RF, MSG_LEGACY_MED,
           "rfgsm_core_meas_build_scripts CLEANUP: src_tech %d, tgt_tech %d, src_band %d, tgt band %d, "
           "rxlm_id %d, script_type=%d",
           src_tech, target_tech, gsm_device_param.source_band,
           gsm_device_param.target_band, rxlm_buf_idx, script_type );

    // get the rf_buffer to use in building the scripts based on script type.
    rf_buffer  = rfmeas_mdsp_buffer_get(RFM_MEAS_PROGRAM_CLEANUP);
    //rf_buffer = (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.meas_nontrx_script;

    rf_trigger_buffer = (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.meas_trigger_script;
    rf_buffer_clear(rf_trigger_buffer);

    /* go back to band and channel which was set before the measurement gap */
    gsm_device_param.source_band = rfgsm_core_handle_ptr->band;
    gsm_device_param.arfcn = rfgsm_core_handle_ptr->rf_rx_arfcn;

    meas_script_data.preload_action = RFDEVICE_MEAS_SKIP_EXECUTION;
    meas_script_data.preload_buf = NULL;
    meas_script_data.meas_action = RFDEVICE_MEAS_CREATE_SCRIPT;
    meas_script_data.meas_buf = (rf_buffer_intf*)rfgsm_core_handle_ptr->scripts.meas_trigger_script;

    gsm_build_script_profiling.transceiver_script_start_t = rf_time_get_tick();

    /* setup rf device to GSM after making X tech measurements */
    rfdevice_gsm_build_meas_script(rfm_dev, gsm_device_param,
                                   &meas_script_data);

    gsm_build_script_profiling.transceiver_script_time = rf_time_get_elapsed(gsm_build_script_profiling.transceiver_script_start_t,RF_USEC);
    gsm_build_script_profiling.msm_script_start_t = rf_time_get_tick();

    /* setup RFC(rf-front-end devices) to GSM after making X tech measurements */
    rfc_gsm_mode_config(rfm_dev, rf_buffer, RFDEVICE_PRX_OPER_MODE, RFGSM_ALT_PATH_NOT_SUPPORTED);

    gsm_build_script_profiling.msm_script_time = rf_time_get_elapsed(gsm_build_script_profiling.msm_script_start_t,RF_USEC);

#ifdef G_POLICY_CHECK_EN
    override_params.pending_preload_cnt = 0;
    override_params.pending_trigger_cnt = 0;
    override_params.preload_seq_id = RFMEAS_MDSP_GET_EVT_SEQ_ID(RFLM_USR_GSM,
                                                                    1,
                                                                    1,
                                                                    1);
    override_params.priority = 9;
    override_params.state = RFLM_MEAS_RES_STATE_PRELOAD_DONE;
    override_bitmask = buf_mask;
    override_err_code = rflm_meas_overwrite_resource_state(override_bitmask, override_params);

    if (override_err_code != 0)
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_meas_build_scripts: Non zero error returned "
                                            "from overwrite_resource_state: error code %d",
                                            override_err_code);
    }
#endif // G_POLICY_CHECK_EN
    /* update the FW shared mem with rf_device and RFC settings */
    gsm_build_script_profiling.mdsp_script_start_t = rf_time_get_tick();

    temp_trx_trans = rf_buffer_get_num_trans(meas_script_data.meas_buf, RF_BUFFER_RFFE);

    /* update the FW shared mem with rf_device and RFC settings */
    MSG_1(MSG_SSID_RF, MSG_LEGACY_MED,"rfgsm_core_meas_build_scripts: Trigger %d script is already created!",temp_trx_trans);

#ifdef G_POLICY_CHECK_EN
    policy_check.buff_mask = meas_script_data.buffer_mask;
    policy_check.channel = 1;
    policy_check.rf_band = 1;

    return_bool = rfmeas_mdsp_meas_write_ccs_irat_script_v2( rxlm_buf_idx,
                                                             RFM_MEAS_PROGRAM_CLEANUP,
                                                             meas_script_data.preload_buf,
                                                             RFLM_MEAS_DM_PRELOAD_SCRIPT,
                                                             meas_script_data.meas_buf,
                                                             RFLM_MEAS_DM_TRIGGER_SCRIPT,
                                                             rf_buffer,
                                                             RFLM_MEAS_DM_NONTRX_SCRIPT,
                                                             FALSE /* combine_trigger_non_trx */,
                                                             &policy_check );
#else // ^ G_POLICY_CHECK_EN
    return_bool = rfmeas_mdsp_meas_write_ccs_irat_script_v2( rxlm_buf_idx,
                                                             RFM_MEAS_PROGRAM_CLEANUP,
                                                             meas_script_data.preload_buf,
                                                             RFLM_MEAS_DM_PRELOAD_SCRIPT,
                                                             meas_script_data.meas_buf,
                                                             RFLM_MEAS_DM_TRIGGER_SCRIPT,
                                                             rf_buffer,
                                                             RFLM_MEAS_DM_NONTRX_SCRIPT,
                                                             FALSE /* combine_trigger_non_trx */,
                                                             NULL );
#endif // ^ !G_POLICY_CHECK_EN
    gsm_build_script_profiling.mdsp_script_time = rf_time_get_elapsed(gsm_build_script_profiling.mdsp_script_start_t,RF_USEC);

    if( !return_bool )
    {
      MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_meas_build_scripts: Error in GSM cleanup iRAT event creation!", 0);
      return RFM_MEAS_COMMON_FAILURE;
    }

    /* no need to configure Modem baseband and fw variables as those settings
       are already present in shared memory and it will be configured by FW */
  }
  else
  {
    MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_meas_build_scripts: Incorrect parameters: src_tech = %d, target_tech = %d, script_type=%d",
          src_tech, target_tech, script_type);
    return RFM_MEAS_COMMON_FAILURE;
  }
  gsm_build_script_profiling.build_script_time = rf_time_get_elapsed(gsm_build_script_profiling.build_script_start_t,RF_USEC);

  if ( display_profiling )
  {
    MSG_5(MSG_SSID_RF, MSG_LEGACY_HIGH,
          "rfgsm_core_meas_build_scripts: GSM Build stop timings (us) build:%d"
          "sw_init:%d transceiver:%d asm:%d tuner:%d msm:%d mdsp:%d",
          gsm_build_script_profiling.build_script_time,
          gsm_build_script_profiling.sw_init_time,
          gsm_build_script_profiling.transceiver_script_time,
          gsm_build_script_profiling.msm_script_time,
          gsm_build_script_profiling.mdsp_script_time );

    MSG_1(MSG_SSID_RF, MSG_LEGACY_HIGH,
          "rfgsm_core_meas_build_scripts: LTE Build stop timings (us) transceiver_setup_total %d",
          gsm_build_script_profiling.transceiver_setup_total_time);
  } // ^ display_profiling == TRUE

  return RFM_MEAS_COMMON_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
   This is the core level function to populate GSM reset scripts
   This will be called only when in X2G cases and to reset contents of GSM
   that were configured when GSM was active.

  @param rfm_dev : Logical device ID on which G was active and that which needs to be
                   cleaned up
  @param band : GSM band in which irat measurement is carried out

  @retval
  rfm_meas_result_type
*/

void rfgsm_core_build_reset_scripts(rfm_device_enum_type rfm_dev, uint32 band)
{
  rfm_meas_result_type result = RFM_MEAS_COMMON_FAILURE;
  result= rfgsm_core_meas_cleanup(rfm_dev, band, FALSE);
  if (RFM_MEAS_COMMON_FAILURE == result)
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfgsm_core_build_reset_scripts failed!!", 0);
  }
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
rfm_meas_result_type rfgsm_core_meas_exit( rfm_meas_exit_param_type *meas_exit_param)
{
  rfgsm_core_handle_type *rfgsm_core_handle_ptr = NULL;
  rfcom_mode_enum_type src_tech = meas_exit_param->exit_params->header.source_tech;
  rfcom_mode_enum_type tgt_tech = meas_exit_param->exit_params->header.target_tech;
  rfm_meas_param_v2_type *channel_params = NULL;
  rfm_device_enum_type rfm_dev = RFM_DEVICE_0;
  unsigned long long start_pcycle = qurt_get_core_pcycles();
  rfm_meas_common_param_type* exit_params = meas_exit_param->exit_params;
  uint32 rxlm_buf_idx = 0, band;
  rfm_meas_program_script_type script_type = meas_exit_param->exit_params->script_type;

  uint8 sub_id;
  rf_path_enum_type rf_path;

  if ( (tgt_tech == RFCOM_GSM_MODE) || (script_type == RFM_MEAS_PROGRAM_NULL2X) )
  {
    rfm_dev = exit_params->header.target_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
    band = exit_params->header.target_param_v2[0].band;

    sub_id = meas_exit_param->exit_params->target_param.gsm_params.sub_id;
  }
  else if ( src_tech == RFCOM_GSM_MODE)
  {
    rfm_dev = exit_params->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device;
    band = exit_params->header.source_param_v2[0].band;

    sub_id = meas_exit_param->exit_params->source_param.gsm_params.sub_id;
  }
  else
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR, "rfgsm_core_meas_exit: GSM is not source or target tech, source_tech is %d, target tech is %d",
      src_tech,
      tgt_tech);
  return RFM_MEAS_COMMON_FAILURE;
  }

  rfgsm_core_handle_ptr = rfgsm_core_handle_get(rfm_dev);
  RF_NULL_CHECK_RTN(rfgsm_core_handle_ptr, "rfgsm_core_meas_exit: rfgsm_core_handle_ptr is NULL!", RFM_MEAS_COMMON_FAILURE);

  if ( rfgsm_core_irat_active_cnt[rfm_dev] > 0 )
  {
    rfgsm_core_irat_active_cnt[rfm_dev]--;
  }

  if ( rfgsm_core_irat_active_cnt[rfm_dev] == 0 )
  {
    rfgsm_core_handle_ptr->rfgsm_buffer = RFGSM_HOME_BUFFER;

    /* Set GSM mode flag to Default mode as not in iRAT or Standalone */
    rfgsm_core_handle_ptr->rfgsm_mode = RFGSM_DEFAULT_MODE;
  }

  if((tgt_tech == RFCOM_LTE_MODE) ||
     (tgt_tech == RFCOM_TDSCDMA_MODE) ||
     (tgt_tech == RFCOM_WCDMA_MODE))
  {
    channel_params = (rfm_meas_param_v2_type *)exit_params->header.source_param_v2;
  }
  else
  {
    channel_params = (rfm_meas_param_v2_type *)exit_params->header.target_param_v2;
  }

  rxlm_buf_idx = (uint32)channel_params->rx_dev_handle[DEV_HANDLE_PRX].lm_handle;

  MSG_6(MSG_SSID_RF, MSG_LEGACY_LOW,"rfgsm_core_meas_exit dev %d rxlm handle %d src tech %d tgt_tech %d gap_aborted %d rfgsm_core_irat_active_cnt:%d",
        rfm_dev, rxlm_buf_idx, src_tech, tgt_tech, meas_exit_param->is_measurement_gap_aborted, rfgsm_core_irat_active_cnt[rfm_dev] );

  /* Reset DM event status back to IDLE on abort */
  if (meas_exit_param->is_measurement_gap_aborted)
  {
    RFGSM_CORE_IRAT_Q(src_tech, tgt_tech, start_pcycle, RFGSM_CORE_IRAT_ABORT);

    if (tgt_tech == RFCOM_GSM_MODE)
    {
      if( rfmeas_mdsp_meas_abort_script(rxlm_buf_idx, RFLM_MEAS_DM_PRELOAD_SCRIPT) == FALSE)
      {
      MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR," rfgsm_core_meas_exit: Abort failed for src_tech = %d, tgt_tech = %d and script type = %d",
                                            src_tech, tgt_tech, RFLM_MEAS_DM_PRELOAD_SCRIPT );
      return RFM_MEAS_COMMON_FAILURE;
      }
    }

    if( rfmeas_mdsp_meas_abort_script(rxlm_buf_idx, RFLM_MEAS_DM_TRIGGER_SCRIPT) == FALSE)
    {
      MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR," rfgsm_core_meas_exit: Abort failed for src_tech = %d, tgt_tech = %d and script type = %d",
                                            src_tech, tgt_tech, RFLM_MEAS_DM_TRIGGER_SCRIPT );
      return RFM_MEAS_COMMON_FAILURE;
    }

    if( rfmeas_mdsp_meas_abort_script(rxlm_buf_idx, RFLM_MEAS_DM_NONTRX_SCRIPT) == FALSE)
    {
      MSG_3(MSG_SSID_RF, MSG_LEGACY_ERROR," rfgsm_core_meas_exit: Abort failed for src_tech = %d, tgt_tech = %d and script type = %d",
                                            src_tech, tgt_tech, RFLM_MEAS_DM_NONTRX_SCRIPT );
      /* NON TRX writes are not present unless added by source tech in build reset API*/
      return RFM_MEAS_COMMON_FAILURE;
    }
  }

  /* Sanity check if the device ENTER mode has been called first */
  if ( !IS_IRAT_ACTIVE_SUB_MEAS_BIT_SET( rfm_dev, sub_id ) )
  {
    MSG_5( MSG_SSID_RF, MSG_LEGACY_HIGH,
           "rfgsm_core_meas_exit: Requested EXIT for rfm_dev:%d sub_id:%d"
           " before ENTER called active bitmask[%d]:0x%x gap_abort:%d",
           rfm_dev, sub_id, rfm_dev, rfgsm_core_irat_active_sub_bitmask[rfm_dev],
           meas_exit_param->is_measurement_gap_aborted );

    /* Return SUCCESS so multiple exits will not cause Source tech failure */
    return RFM_MEAS_COMMON_SUCCESS;
  }

  /* Clear device control bitmask */
  CLR_IRAT_ACTIVE_SUB_MEAS_BITMASK( rfm_dev, sub_id );

  /* Check if any active sub vote exists for this device so still active meas gap */
  if ( !IS_IRAT_MEAS_DEVICE_STILL_NEEDED( rfm_dev ) )
  {
    /************************************/
    /* New power saving API (ON -> LPM) */
    /************************************/
  //check RFNV_SPARE_1_I value to know LPM feature	
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    if (tgt_tech == RFCOM_GSM_MODE)
    {
      /* Disable RX Device (ON -> LPM) for lowest supported band */
      if ( !rfdevice_gsm_rx_pwr_sleep( rfm_dev, rfgsm_nv_get_lowest_supported_band(rfm_dev, RFGSM_NV_RX) ) )
      {
        ERR_FATAL( "rfgsm_core_meas_exit: Disable RX Device %d (ON->LPM) failed for band %d",
                  rfm_dev, rfgsm_nv_get_lowest_supported_band(rfm_dev, RFGSM_NV_RX), 0 );
        return FALSE;
      }
    }
  }

    /* For NULL2G, must turn off devices */
    if( script_type == RFM_MEAS_PROGRAM_NULL2X )
    {
      /* Cleanup front end devices (this should ideally go in cleanup cmd but support currently not there) */
      rfgsm_core_meas_cleanup(rfm_dev, band, TRUE);

       /* Get logical path */
      rf_path = rfcommon_core_device_to_path(rfm_dev);

      /* Disable device Power Voting */
      if( !rfcommon_core_dev_power_vote(rfm_dev, RFCMN_CORE_DEV_OFF, RFCMN_CORE_DEV_NO_VOTE) )
      {
        RF_MSG( RF_FATAL, "rfgsm_core_meas_exit(): Device Pwr Down failed()");
      }

      /* Turn the vregs OFF */
      rfgsm_core_turn_off_vregs(rf_path, sub_id, rfm_dev);

      /* Hal Voting (TQ4) */
      rfgsm_core_enable_hal_bus( FALSE, sub_id );
    }
    /* Enable therm reads */
    else if ( rfgsm_core_handle_ptr->rfgsm_core_temp_comp_started )
    {
      rfgsm_core_temp_comp_resume(rfm_dev);
    }
  }

  return RFM_MEAS_COMMON_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
    This function is called to do preconfiguration for target WTR, if required.

  @param preconf_required : If WTR needs to be pre-configured for target tech

  @pre
  rfm_meas_common_enter function must have been called

  @retval
  rfm_meas_result_type
*/

rfm_meas_result_type rfgsm_core_meas_pre_config( rfm_meas_setup_param_type *meas_scripts_param)
{
  /* Placeholder for future use for cross-wtr irat */
  return RFM_MEAS_COMMON_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
   This function is called to put all front end devices to sleep either as immediate
   writes or in scripts (based on the place from where this is called).
   For NULL2X cases where we don't have any source tech, we will do as immediate writes.
   For X2G cases where a valid source tech X is there, we will do it script based.

  @param rfm_dev : Logical device ID on which G was active and that which needs to be
                   cleaned up
  @param band : GSM band in which irat measurement is carried out
  @param is_null2x : Specifies if we are actually called from NULL2x or general case.

@retval
  rfm_meas_result_type
*/

rfm_meas_result_type rfgsm_core_meas_cleanup(rfm_device_enum_type rfm_dev, uint32 band, boolean is_null2x)
{
  rf_buffer_intf *script_buffer = NULL;
  rfcom_gsm_band_type g_band = (rfcom_gsm_band_type)band;

  /* Do input params bound check to ensure passed values like dev and band are correct */
  if ((rfm_dev >= RFM_MAX_WAN_DEVICES)||(g_band >= RFCOM_NUM_GSM_BANDS))
  {
    MSG_2(MSG_SSID_RF, MSG_LEGACY_ERROR,"INVALID INPUT PARAMS! Device=%d, Band=%d", rfm_dev, g_band);
	return RFM_MEAS_COMMON_FAILURE;
  }

  /* Call the RFC API to cleanup the GSM front end devices */
  if (TRUE == is_null2x)
  {
    rfc_gsm_sleep(rfm_dev, g_band, RFGSM_IRAT_NULL2X_MODE, NULL);
  }
  else
  {
    script_buffer = rfmeas_mdsp_buffer_get(RFM_MEAS_PROGRAM_TRIGGER);
    rfc_gsm_sleep(rfm_dev, g_band, RFGSM_IRAT_MODE, script_buffer);
  }
  return RFM_MEAS_COMMON_SUCCESS;
}
