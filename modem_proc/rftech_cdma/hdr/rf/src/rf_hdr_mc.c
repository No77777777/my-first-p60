/*!  @file
  rf_hdr_mc.c
 
  @brief
  RF Driver's HDR MC interface file.

  @details
  This file defines the RFM HDR Main Control interfaces.
 
  @addtogroup RF_CDMA_HDR_MC
  @{ 
*/

/*==============================================================================

  Copyright (c) 2010 - 2018 Qualcomm Technologies, Inc.. All Rights Reserved

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

    $Header: //components/rel/rftech_cdma.mpss/3.11/hdr/rf/src/rf_hdr_mc.c#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
07/16/21   ys      Remove autopin disable from inside of FBRX deinit during sleep_tx
11/11/19   mk      Added hdr Modem Timeline histogram profiling changes
03/27/18   rp      KW Fixes.
03/09/18   mk      KW Errors fix
01/15/18   am      F3 reduction
10/26/17   sk      Enable LTE B71 support
01/18/17   sk      Send Tx handle to XPT api's for LTE
07/21/16   rs      Added support to take care of additional PA RFFE writes in 
                   txagc
07/12/16   pk      Removed reference to rflm_cmn_iref_temp_comp_therm_read since 
                   the compensation is not required on TA
05/28/16   wxl     Update CDMA state machine before autopin check.
05/03/16   rs      Update tuner scripts for Div when updating for Tx
03/24/16   sbo     Move Txagc SEQ update  to Static Scripts in PDMEM.
03/10/16   rs      Changing RF_CDMA_CCS_EVENT* to RFLM_CDMA_MDSP_CCS_EVENT*
03/03/16   rs      Allow Skip-pin only if Apin and PAR support are enabled
03/03/16   rs      Added support for 1x2G QTA
03/03/16   rs      Allow Skip-pin only if Apin is enabled
03/02/16   zhh     update definition of rfcommon_autopin_mc_enable_tx
01/22/16   rs      Set FBRx enable flag for TxAGC depending on FBRx is available or not
01/19/16   rs      Updated DRx antenna position handling
12/24/15   vr      Autopin check-in_2
12/03/15   rs      NV support to enable/disable LPM feature
11/20/15   vr      Autopin check-in
10/14/15   rs      Moved core dev vote inside iRAT flag
10/14/15   rs      Modified sleep API to fix the DO2L iRAT crash and
                   Moved the core dev power vote off out of IRAT check
                   of fix the LTE High DC after L21x QTA
09/21/15    aa     Save TX/FBRx LM buffer info in FBRx state machine
09/14/15   aak     Vote Tx_SLEEP instead of Rx State in Tx Disable
09/11/15   wwl     Add Autopin tx enable and disable support
08/17/15   zhw     Move 1x/HDR meas tune-away & reset script into private buffers
08/19/15   zhw     Fixed TQ Device in TQ allocation
07/29/15   pl      call rfcommon_fbrx_mc_deinit_tx when HDR disabling TX
08/03/15   sty     Use source device info for TQ assignment in IRAT scenarios
07/30/15   cdb     Add runtime check for Autopin enabled
07/22/15   zhw     Added support for dynamic Rx MDSP Chain mapping
07/15/15   aak     Resolve compile error for FR 25072
07/14/15    aa     Added support for FBRx C1 NV container
06/30/15   wwl     Add support for Auto Pin
06/25/15   APU     Deleted old revision history
06/24/15   spa     WTR power/up power down support 
06/03/14   spa     Update device state to Rx only after RxAGC has converged
06/17/15   vc      Correcting the condition check for slave id
05/19/15   spa     Use different event id for prx and drx
05/22/15   sub     Placeholder parameter to support fbrx interface change
04/23/15    ka     Remove hardcoded device 0 for fbrx
05/14/15   zhw     Remove unnecessary time profiling due to stack abuse
05/11/15   zhw     Disable Drx support during L->1x QTA
04/16/15   zhw     Reset SW state machine during sleep_tx() for Tx only device
                   Set traffic state in sleep_tx explicitly
04/15/15   zhw     Integrate device driver change with CDMA
04/14/15   pk      Changes for Tx spectral inversion
04/14/15   zhw     Fix rf state update during sleep_tx
04/09/15   zhw     Fix Tx state check in get TxAGC
04/02/15   zhw     HDR TRx split changes -- bring to same status as 1x
03/30/15   zhw     MC changes to support seperate Tx device tune
                   - Refactored rf_hdr_mc_helper_for_retune
                   - Moved away from configure_rffe_for_retune
                   - Expanded rf_hdr_mc_wakeup_tx() to take prx Device
                   - Copy essential state machine info at the beginning of
                     wakeup_tx() from Prx device
                   - Support new CDMA state RF_CDMA_STATE_TX in MC code
03/16/15   pk      Compiler warning Fix
02/12/15   vr      Placed the device level locks while getting TX ON OFF scripts
12/09/10   aro     Initial version
==============================================================================*/

#include "rfa_variation.h"
#include "comdef.h"
#ifdef FEATURE_CDMA1X
#include "rf_hdr_mc.h"
#include "rf_cdma_temp_comp.h"
#include "rf_cdma_nv.h"
#include "rf_cdma_intelliceiver.h"
#include "rf_hdr_power_limiting.h"
#include "rf_cdma_hdet.h"
#include "rf_cdma_msm.h"
#include "rf_hdr_mdsp.h"
#include "rf_hdr_tx_agc.h"
#include "rf_hdr_rx_agc.h"
#include "rf_cdma_utils.h"
#include "rf_cdma_utils_freq.h"
#include "rfcommon_core.h"
#include "rfcommon_core_xpt.h"
#include "rfcommon_msg.h"
#include "rfcommon_mdsp.h"
#include "rfm_cdma.h"
#include "rfm_internal.h"
#include "modem_mem.h"
#include "rf_cdma_utils.h"
#include "rf_cdma_utils_freq.h"
#include "rf_cdma_time_profile.h"
#include "rfcommon_tx_manager.h"
#include "rfcommon_data.h"

#include "rftxplim_mm_report_tx_power.h"

#include "rf_hdr_temp_comp.h"
#include "rf_cdma_ssma.h"
#include "rfgnss_mc.h" /* For GNSS state updates */
#include "rfm_types.h" /* For RFGNSS_MC() macro */
#include "rfcommon_core_device_manager.h" /* For RF Common Core Device Manager */
#include "ftm.h"
#include "rf_hdr_log.h"
#include "rf_cdma_mc.h"
#include "rf_hdr_fw_response.h"
#include "rf_cdma_constants.h"
#include "rfcommon_time_profile.h"
#include "rfcommon_msm.h"
#include "rf_cdma_fwrsp.h"

/* New Buffer Interface */
#include "rf_hal_buffer.h"

#include "rf_hal_common.h" /* for CCS command app */
#include "rfmeas_mc.h"
#include "rf_hdr_meas.h"

#include "rf_hdr_ept.h"
#include "rf_cdma_ept.h"
#include "rfdevice_class.h" /* rfdevice_id_get() */


#include "rf_cdma_mdsp_ccs_events.h"
#include "rf_cdma_mdsp_modem.h"

#include "rflm_api_cmn.h"
#include "rflm_c2k_mc.h"
#include "rfcommon_fbrx_api.h"
#include "rflm_cmn_asd.h"
#include "rflm_dm_api.h"
#include "rf_cdma_meas.h"
#include "rflm_cdma_tuner.h"
#include "rfcommon_nv_mm.h"

#include "rfcommon_autopin_api.h"
#include "rf_cdma_auto_pin.h"
#include "rf_cdma_rx_agc.h"

#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
#include "prof_hist.h"
#endif

#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
        extern prof_hist_log_packet_handle_t hdr_rf_hist_packet_handle;
        extern prof_hist_param_t   rx_hdr_wakeup_PRx_time_hist;
        extern prof_hist_param_t   rx_hdr_wakeup_DRx_time_hist;
        extern prof_hist_param_t   rx_hdr_wakeup_fw_resp_hist;
        extern prof_hist_param_t   rx_hdr_sleep_PRx_time_hist;
        extern prof_hist_param_t   rx_hdr_sleep_DRx_time_hist;
        extern prof_hist_param_t   rx_hdr_sleep_fw_resp_hist;
        extern prof_hist_param_t   tx_hdr_wakeup_Tx_time_hist;
        extern prof_hist_param_t   tx_hdr_wakeup_fw_resp_hist;
        extern prof_hist_param_t   tx_hdr_sleep_Tx_time_hist;
        extern prof_hist_param_t   tx_hdr_sleep_fw_resp_hist;
#endif
/*============================================================================*/
/*!
  @name Main Control helper functions

  @brief
  This section holds all helper functions used by HDR MC layer
*/

/*! @{ */
/*!
  @brief
  Helper function to aid Rx start and config.

  @details
  This function can be used by any entity that wishes to start
  Rx either on the primary or diversity chain. This function
  will send the rx-config and rx-start messages to the FW.
   
  @param device
  Device on which 1x subsystem is to be entered
 
  @param dev_status_r
  The device status structure for the associated device.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.
 
  @param logical_device 
  The logical device for the given device.
 
  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @return success
  TRUE: Everything went well. Call successful.
  FALSE: Some error. went well. Call unsuccessful.
*/
boolean
rf_hdr_mc_send_start_rx_helper 
(  
  const rfm_device_enum_type device,
  const rf_cdma_data_status_type* dev_status_r ,
  const lm_handle_type rxlm_handle,
  const rfc_cdma_logical_device_params_type* logical_dev,
  boolean is_irat_mode,
  boolean is_retune,
  boolean is_div_device
)
{
  rf_time_type enable_FW_Resp_time; /* TIme spent in function */
  rf_time_tick_type enable_FW_Resp_tick; /* TIme spent in function */
  boolean success = FALSE ;

  rf_cdma_fwrsp_data_type* fwrsp_hk = rf_cdma_get_fwrsp_data( device );
  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_send_start_rx_helper: NULL FWRSP Data" );
  } /* if ( fwrsp_data == NULL ) */
  else
  {
    rf_cdma_mdsp_rx_start_type rx_start_c; /* RxAGC Start Config */
    
    rx_start_c.script_index = (uint32)RFLM_CDMA_DUMMY_EVENT;
    rx_start_c.meas_reset_script_id = (uint32)RFLM_CDMA_DUMMY_EVENT;
    if ( TRUE == is_irat_mode)
    {
      rx_start_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_RF_IRAT_TUNE_IN_ID ;
      rx_start_c.meas_reset_script_id = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_SLEEP_RX_ID;
    }
    else if ( TRUE == is_retune ) 
    {
      rx_start_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_RETUNE_ID ;
    }
    else if ( is_div_device == TRUE )
    {
      rx_start_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_WAKEUP_DRX_ID ;
    }
    else
    {
      rx_start_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_WAKEUP_RX_ID ;
    }

    rx_start_c.rxlm_handle = rxlm_handle; /* RXLM Handle */

    /* Send Start Rx Config message */
    success = rf_hdr_mdsp_start_rx_config( logical_dev->rx_mdsp_path, 
                                           &rx_start_c, NULL, NULL, NULL );
    if ( FALSE == success )
    {
      RF_MSG ( RF_ERROR , "rf_hdr_mc_send_start_rx_helper(): " 
               "rf_hdr_mdsp_start_rx_config() has returned failure ") ;
    }
    else
    {
      if ( is_irat_mode == TRUE )
      {
        RF_MSG ( RF_HIGH ,  "rf_hdr_mc_send_start_rx_helper(): " 
               "device_in_irat == TRUE. Skipping Rx Start " ) ;
      }
      else
      {
        enable_FW_Resp_tick = rf_time_get_tick();
        rf_cdma_mdsp_response_cb_param_type rsp_cb_param ;
        fwrsp_hk->rx_start_rsp.device = device;
        fwrsp_hk->rx_start_rsp.sema_token = rfcommon_semaphore_pop_item();
        rsp_cb_param.rsp_handler = rf_hdr_rx_start_rsp_cb;
        rsp_cb_param.handler_data = &fwrsp_hk->rx_start_rsp;

        success = 
          rf_hdr_mdsp_start_rx( logical_dev->rx_mdsp_path, &rsp_cb_param,
                                              NULL, NULL );

        if ( FALSE == success )
        {  
          RF_MSG ( RF_ERROR , "rf_hdr_mc_send_start_rx_helper(): " 
                 "rf_hdr_mdsp_start_rx() has returned failure ") ;
          /* Message sending failed, free the semaphore */
          RF_MSG_2 ( RF_LOW, "rf_hdr_mc_send_start_rx_helper():"
                     " Freeing the semaphore and not waiting "   
                     " [D%d] RX_START pushed back : Sem @ 0x%x" , 
                     device , fwrsp_hk->rx_start_rsp.sema_token ) ;
        }/*if ( FALSE == success )*/
        else
        {
          RF_MSG_2( RF_LOW, "[HDR][RF-FW Sync] [D%d] RX_START : Start Wait Sem @ "
                            "0x%x", device, fwrsp_hk->rx_start_rsp.sema_token ) ;
          rfcommon_semaphore_wait( fwrsp_hk->rx_start_rsp.sema_token );
        }/*if ( FALSE != success )*/
        enable_FW_Resp_time = rf_time_get_elapsed( enable_FW_Resp_tick, RF_USEC );
       #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
         prof_hist_update(&rx_hdr_wakeup_fw_resp_hist,(uint32)enable_FW_Resp_time);
         if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
         {
           rf_hdr_clear_profiling_hist_packet();
         }	
       #endif

        /* Push back semaphore after wait if mssg was sent successfully,
        or immediately if mssg sending failed*/
        rfcommon_semaphore_push_item( fwrsp_hk->rx_start_rsp.sema_token ) ;

        if ( success == FALSE )
        {
          /* Since HDR Rx start sending failed, we will not be waiting on Rx 
          AGC to converge because we will never get the RxAGC trak indicator 
          response, therefore we should free the semaphore associated with that 
          FW response as well, to avoid semaphore exhaustion */ 
          RF_MSG_2 ( RF_LOW, "rf_hdr_mc_send_start_rx_helper():"
                     " Freeing the semaphore and not waiting "   
                     " [D%d] TRACK_IND pushed back : Sem @ 0x%x" , 
                     device , fwrsp_hk->rxagc_trk_ind.sema_token ) ;
          rfcommon_semaphore_push_item( fwrsp_hk->rxagc_trk_ind.sema_token ) ;
        }/* if ( success == FALSE ) */
      } /* If not in IRAT */
    }/*if ( FALSE != success )*/
  } /* if ( fwrsp_data != NULL ) */

  return success ;
}/* rf_hdr_mc_send_start_rx_helper*/

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Helper function to send Rx Stop to FW.

  @details
  This function can be used by any entity that wishes to stop
  Rx either on the primary or diversity chain. This function
  will send the rx-stop messages to the FW.
   
  @param device
  Device on which 1x subsystem is to be entered
 
  @param dev_status_r
  The device status structure for the associated device.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.
 
  @param logical_device 
  The logical device for the given device.
 
  @return success
  TRUE: Everything went well. Call successful. 
  FALSE: Some error. Call unsuccessful.
*/
boolean
rf_hdr_mc_send_stop_rx_helper 
(  
  const  rfm_device_enum_type device ,
  const  rf_cdma_data_status_type* dev_status_r ,
  const  lm_handle_type rxlm_handle ,
  const  rfc_cdma_logical_device_params_type* logical_dev ,
  boolean skip_rf_front_end_programming ,
  boolean is_slave_device 
)
{
  rf_time_tick_type sleep_FW_resp_time;  /* time spent in API */
  rf_time_type sleep_FW_resp_time_val;
  boolean success = FALSE ;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;

  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device );

    /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_send_stop_rx_helper: NULL FWRSP Data" );
  } /* if ( fwrsp_data == NULL ) */
  else
  {
    rf_cdma_mdsp_response_cb_param_type rsp_cb_param ;
    rf_cdma_mdsp_rx_stop_type rx_stop_data ; /* Rx Stop Data */
    /* 
    The script index of -1 suggests that FW will 
    not program the scripts provided by RF for configuring the RF front-end..
    a valid scenario is retune, where teh whole Rx chain need not be put to sleep on 
    the old channel and then woken up again on the new channel.
    */
    if ( skip_rf_front_end_programming == TRUE )
    {
      rx_stop_data.script_index = -1 ;
    }
    else
    {
      if ( is_slave_device == FALSE  )
      {
        rx_stop_data.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_SLEEP_RX_ID;
      }
      else
      {
        rx_stop_data.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_SLEEP_DRX_ID;
      }
    }

    sleep_FW_resp_time = rf_time_get_tick();
    rx_stop_data.rxlm_handle = rxlm_handle; /* RXLM Handle */
    rx_stop_data.rtc_cx1_trigger = RF_CDMA_RX_STOP_RTC_CX1_TRIGGER; 
    /* RTC time for Rx Stop */

    /* Stop Rx */
    memset( &(fwrsp_hk->rx_stop_rsp), 0, 
            sizeof(rf_cdma_rx_stop_rsp_data_type) );
    fwrsp_hk->rx_stop_rsp.device = device;
    fwrsp_hk->rx_stop_rsp.sema_token = rfcommon_semaphore_pop_item();
    rsp_cb_param.rsp_handler = rf_hdr_rx_stop_rsp_cb;
    rsp_cb_param.handler_data = &fwrsp_hk->rx_stop_rsp;
    success = rf_hdr_mdsp_stop_rx( logical_dev->rx_mdsp_path, &rx_stop_data, 
                         &rsp_cb_param, NULL, NULL );

    /* Check if message sending was success or failure */
    if ( FALSE == success )
    {  
      RF_MSG ( RF_ERROR , "rf_hdr_mc_send_stop_rx_helper(): " 
             "rf_hdr_mdsp_stop_rx() has returned failure ") ;
      /* Message sending failed, free the semaphore */
      RF_MSG_2 ( RF_LOW, "rf_hdr_mc_send_stop_rx_helper():"
                 " Freeing the semaphore and not waiting "   
                 " [D%d] RX_STOP pushed back : Sem @ 0x%x" , 
                 device , fwrsp_hk->rx_stop_rsp.sema_token ) ;
    }/*if ( FALSE == success )*/
    else
    {
      /* Message was sent to FW, wait for semaphore */
      RF_MSG_2( RF_LOW, "[HDR][RF-FW Sync] [D%d] RX_STOP : Start Wait Sem @ "
                        "0x%x", device, fwrsp_hk->rx_stop_rsp.sema_token );
      rfcommon_semaphore_wait( fwrsp_hk->rx_stop_rsp.sema_token );
    }

    /* Push back semaphore after wait if mssg was sent successfully,
    or immediately if mssg sending failed*/
    rfcommon_semaphore_push_item( fwrsp_hk->rx_stop_rsp.sema_token ) ;
    sleep_FW_resp_time_val = rf_time_get_elapsed(sleep_FW_resp_time, RF_USEC );
    #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
      prof_hist_update(&rx_hdr_sleep_fw_resp_hist,(uint32) sleep_FW_resp_time_val);
      if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
      {
        rf_hdr_clear_profiling_hist_packet();
      }
    #endif
    rf_cdma_mc_configure_stop_rx_agc_data( device, rxlm_handle );
  }/* if ( fwrsp_hk != NULL )*/ 

  return success ;
}/* rf_hdr_mc_send_stop_rx_helper */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Helper function to cleanup Rx related CCS events for HDR

  @details
  This function Cleans up RF CCS event handles, Free up allocated handle 
  objects. Reset event status bit field back to IDLE
  
  @param device
  Device on which 1x subsystem is to be entered
 
  @return success
  TRUE: Everything went well. Call successful.
  FALSE: Some error. went well. Call unsuccessful.
*/
static 
boolean 
rf_hdr_mc_cleanup_rx_ccs_events
(
  rfm_device_enum_type device
)
{
  boolean api_status;

  /* Always clean up static event smem and event handle */
  api_status = rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );

  if ( api_status == FALSE )
  {
    RF_MSG_1 ( RF_ERROR , 
               "rf_hdr_mc_cleanup_ccs_events: Cleanup failed for Event: %d ",
               RF_CDMA_STATIC_EVENT );
  }

  /* cleanup Rx on/off events */
  api_status = rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_EVENT_RX_ON);

  if ( api_status == FALSE )
  {
    RF_MSG_1 ( RF_ERROR , 
               "rf_hdr_mc_cleanup_ccs_events:Cleanup failed for Event: %d ", 
               RF_CDMA_STATIC_EVENT );
  }

  api_status &= rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_EVENT_RX_OFF );

  if ( api_status == FALSE )
  {
    RF_MSG_1 ( RF_ERROR , 
               "rf_hdr_mc_cleanup_ccs_events: Cleanup failed for Event: %d ",
               RF_CDMA_STATIC_EVENT );
  }

  return api_status;

} /* rf_hdr_mc_cleanup_rx_ccs_events */

/*! @} */

/*============================================================================*/
/*!
  @name Main Control Interface

  @brief
  This section holds all Main Control Interfaces to be used by RFM layer.
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform HDR-specific initializations

  @details
  This function will not touch any hardware related functionality. It is
  intended to configure the differrent modules (like MDSP) to the correct mode.
   
  @return
  Flag indicating a success (TRUE) or failure (FALSE)
*/
boolean
rf_hdr_mc_init
(
  void
)
{
  boolean init_status = TRUE; /* MDSP init Status */

  /* Initialize mdsp for HDR - this is a one-time init */
  init_status &= rf_hdr_mdsp_init();
  
  /* Perform RF Responses registration */
  init_status &= rf_hdr_fw_response_registration();  
  
  /* register the IRAT APIs with meas module */
  if ( rf_hdr_mc_meas_init() == FALSE )
  {
    RF_MSG ( RF_ERROR , "rf_hdr_mc_init(): "
                        "rf_hdr_mc_meas_init() has returned FALSE" ) ;
  }

  /* Set the FBRx enable flag based on NV */ 
  rf_cdma_data_set_fbrx_enable( RFM_1XEVDO_MODE);

  return init_status;

} /* rf_hdr_mc_init */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform HDR-specific deinitializations
   
  @return
  Flag indicating a success (TRUE) or failure (FALSE)
*/
boolean
rf_hdr_mc_deinit
(
  void
)
{
  boolean deinit_status = TRUE; /* Variable to track if CDMA Init passed */
  uint8 dev_idx;
  rf_hal_bus_client_handle* handle;
  rf_hdr_config_type *rf_hdr_config_ptr; /* Pointer to HDR RF NV data */
  const rfc_cdma_logical_device_params_type *logical_dev;
  uint8 band_idx;

  for (dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_DEVICES; dev_idx++ )
  {
    rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */

    /* Get Read-only and Writable Device Status */
    dev_status_w = rf_cdma_get_mutable_device_status( 
                             (rfm_device_enum_type)dev_idx );

    if ( (NULL != dev_status_w) )
    {

      /*-------- Disable HAL Bus ---------------------------------------------*/

      handle = rf_cdma_data_get_rf_hal_bus_vote_handle(dev_idx);

      if ( handle != NULL )
      {
        if ( dev_status_w->rf_mode == RFM_1XEVDO_MODE )
        {
          #ifndef FEATURE_RFA_ATLAS_MODEM
          rf_hal_bus_enable (FALSE, &handle, RF_HAL_RFCMD_APP_CLIENT(EVDO));
          #endif
          rf_cdma_data_set_rf_hal_bus_vote_handle(dev_idx, handle);
        } /* if ( dev_status_w->rf_mode == RFM_1X_MODE ) */
      } /* if ( handle != NULL ) */

      /*-------- Deinit Multi Lin --------------------------------------------*/

      logical_dev = rfc_cdma_get_logical_device_params( dev_idx );

      if ( logical_dev != NULL )
      {
        for ( band_idx = RFM_CDMA_BC0; band_idx < RFM_CDMA_MAX_BAND; 
              band_idx++ )
        {
          /* ensure that NV is valid for given device and band */
          rf_hdr_config_ptr = 
            (rf_hdr_config_type*)rf_cdma_nv_get_hdr_data( 
                                     logical_dev->tx_nv_path[band_idx], 
                                     band_idx );
          if ( rf_hdr_config_ptr != NULL )
          {
            rfcommon_core_dealloc_tx_band_cal( 
               &(rf_hdr_config_ptr->cdma_common_ptr->tx_cal.tx_multi_lin_cal) );

            if ( rf_hdr_config_ptr->tx_static.tx_pwr_limit_data != NULL )
            {
              modem_mem_free( rf_hdr_config_ptr->tx_static.tx_pwr_limit_data,
                              MODEM_MEM_CLIENT_RFA );
              rf_hdr_config_ptr->tx_static.tx_pwr_limit_data = NULL;
            }
          } /* if ( rf_1x_config_ptr != NULL ) */
        } /* for ( band_idx = RFM_CDMA_BC0; band_idx < RFM_CDMA_MAX_BAND; 
                   band_idx++ ) */
      } /*if ( logical_dev != NULL ) */
    } /* if ( (NULL != dev_status_w) ) */
  } /* for (dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_DEVICES; dev_idx++ ) */

  deinit_status &= rf_hdr_fw_response_deregistration();

  return deinit_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Prepare radio to operate in CDMA HDR mode

  @details
  This function is used to enter HDR. This function will not touch any hardware 
  related functionality. This will only be initializing the software stack.
 
  @param device
  Device on which HDR subsystem is to be entered

  @param user_data_ptr
  Pointer to user data passed into callback
  
  @param cb_handler
  Callback handler
  
  @return
  Flag indicating whether 1x Enter is successful (TRUE) or failed (FALSE)
*/
boolean
rf_hdr_mc_enter_mode
(
  rfm_device_enum_type device,
  void* const user_data_ptr,
  rfm_cb_handler_type cb_handler
)
{
  boolean ret_val = TRUE; /* Return value */
  rf_time_tick_type enter_mode_tick; /* Time spent in API */
  rf_time_type enter_mode_time; /* Time spent in API */

  /* Start Profiling of Rx Warm Up */
  enter_mode_tick = rf_time_get_tick();

  /* this is the first API to be called in the tune sequence for 1x - so init 
     the RF WU vars for this sequence */ 
  rf_cdma_init_wup_time(device, RFM_1XEVDO_MODE, enter_mode_tick );

  if (device > RFM_MAX_DEVICES)
  {
    RF_MSG_1( RF_ERROR,
              "rf_hdr_mc_enter_mode: Invalid device passed (device=%d)",device);
    ret_val = FALSE; /* to indicate failure */
  }

  /* check if device data structures are initialized */
  if (rf_cdma_is_device_initialized(device) == FALSE)
  {
    RF_MSG( RF_ERROR,
            "rf_hdr_mc_enter_mode: Device data structure not initialized - "
            "bailing out"); 
    ret_val = FALSE; 
  }

  /* 1) Continue only if there were no error 
     2) check if current mode is 1X - if yes, then no operations need to be
        done */
  if ( ret_val == TRUE )
  {
    /* perform enter mode operations only if device is NOT in EVDO mode */
    if ( rf_cdma_data_get_rf_mode(device) != RFM_1XEVDO_MODE )
  {

    /* Update RF Mode */
    rf_cdma_data_set_rf_mode( device, RFM_1XEVDO_MODE ) ;
  }
  } /* if ( ret_val == TRUE ) */

  if (rfcommon_autopin_is_enabled(RFM_1XEVDO_MODE))
  {
  if(RFCOMMON_AUTOPIN_ERROR == rfcommon_autopin_mc_enter_mode(RFM_1XEVDO_MODE))
  { 
    RF_MSG(RF_ERROR, "rf_hdr_mc_enter_mode: CDMA HDR AutoPin enter mode failed!");
    ret_val = FALSE;
  }
  }

  /* Execute Callback */
  if ( cb_handler != NULL )
  {
    cb_handler( RFM_ENTER_MODE_COMPLETE, user_data_ptr );
  }

  /* Record the execution time statistics for wakeup logging for this device*/
  enter_mode_time = rf_time_get_elapsed( enter_mode_tick, RF_USEC );

  /* update this component of RF WU time */
  rf_cdma_update_wup_time(device, enter_mode_time);

  /* no operations need to be done in enter_mode for HDR */
  return ret_val;

} /* rf_hdr_mc_enter_mode */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Exit HDR Mode

  @details
  This function tears down HDR Mode.
 
  @param device
  Device on which HDR is to be torn down
 
  @param to_mode
  New Mode to which RF is transitioning to
 
  @return
  Status of HDR Exit Procedure
*/
int32
rf_hdr_mc_exit_mode
(
  rfm_device_enum_type device,
  rfm_device_enum_type to_mode
)
{
  const rf_cdma_data_status_type *dev_status_r; /* Device Status read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  boolean exec_status = TRUE;
  int32 ret_val = 0;
  rf_time_tick_type exit_mode_time;          /* Time spent in API */
  rf_cdma_state_type last_rf_state;
  rf_time_type exit_mode_time_val;

  /* Start Profiling of Rx exit time */
  exit_mode_time = rf_time_get_tick();

  /* Get Mutable Device Data */
  dev_status_r = rf_cdma_get_device_status( device );

  /* NULL Pointer Check */
  if ( dev_status_r == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_exit_mode: Invalid device %d Data", device);
    exec_status &= FALSE; /* to indicate failure */
  }

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* NULL Pointer Check */
  if ( logical_dev == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_exit_mode: Invalid device %d Logical Param", 
              device);
    exec_status &= FALSE; /* to indicate failure */
  }

  /* Continue only if there were no error codes */
  if ( exec_status == TRUE )
  {
    /* disable temp comp */
    rf_cdma_disable_temp_comp( device );
  
    /* disable max power limit algo */
  
    /* disable device-specific algo (such as PN breathing) */
  
    /* disable intelliceiver */
    if ( ( dev_status_r->intelliceiver.ic_supported ) && 
         ( dev_status_r->intelliceiver.ic_state != RF_CDMA_IC_STOPPED ) )
    {
      rf_cdma_ic_stop( rf_cdma_get_mutable_intelliceiver( device ), 
                       logical_dev );
    }

    /* Tear down radio, if needed. Flag this case with error message as
    Radio should have been in SLEEP state before doing exit. */
    if ( dev_status_r->data_hdr.rf_state != RF_CDMA_STATE_SLEEP )
    {
      last_rf_state = dev_status_r->data_hdr.rf_state;

      rf_hdr_mc_tear_down(device);

      RF_MSG_3( RF_ERROR, "rf_hdr_mc_exit_mode: ****** Dev %d Tear down "
                "required [State %d -> %d] *****", device, 
                last_rf_state, dev_status_r->data_hdr.rf_state ); 
    } /* if ( dev_status_w->rf_state != RF_CDMA_STATE_SLEEP ) */

    /* Update RF Mode */
    rf_cdma_data_set_rf_mode( device, RFM_PARKED_MODE );

    /* Return Value */
    ret_val = 1;

    if (rfcommon_autopin_is_enabled(RFM_1XEVDO_MODE))
    {
    if(RFCOMMON_AUTOPIN_ERROR == rfcommon_autopin_mc_exit_mode(RFM_1XEVDO_MODE))
    {
      RF_MSG(RF_ERROR, "rf_hdr_mc_exit_mode: CDMA HDR AutoPin exit mode failed!");
      ret_val = 0;
    }
    }
  }
  else
  {
    /* Return Value */
    ret_val = 0;
  }

  /* Record the execution time statistics for exiting mode for this device*/
  exit_mode_time_val = rf_time_get_elapsed( exit_mode_time, RF_USEC );

  /* Print time elapsed for exit mode */
  RF_MSG_1( RF_HIGH, "rf_hdr_mc_exit_mode: Time elapsed for exit mode %d", 
            exit_mode_time_val);

  return ret_val; /* to indicate success */

} /* rf_hdr_mc_exit_mode */

/*! @} */

/*============================================================================*/
/*!
  @name CDMA HDR subsystem TRM Control Functions

  @brief
  Functions for TRM to enable and disable the HDR portion of the
  CDMA RF driver.
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enable the HDR subsystem of the CDMA RF driver.

  @details
  Prepares the radio for HDR operation on an initial band and channel.
  The HDR subsystem is initialized in the Sleep state with the requested
  band and channel.

  By initialzing HDR with a band and a channel, this avoids the situation
  where "the driver is on but there is no known channel."  This also
  allows SVDO to be implemented correctly when 1x is in traffic and HDR
  needs to acquire or check paging on the primary chain.  By forcing HDR
  to be enabled with a channel, the driver can detect that wideband Rx
  mode is correct, and tune radio to the correct initial frequency.

  If 1x is already active and on an incompatible channel, this API
  call will return an error.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_HDR_ENABLE_COMPLETE event.  The function will attempt to give
  a best estimate of the time remaining in the return value.

  @param device
  The path to enable the HDR subsystem on.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @param band
  The band to initialize the path to.
 
  @param chan
  The channel to initialize the path to.
 
  @return
  The anticipated time in microseconds remaining from the rfm_hdr_enable()
  function return until the operation is complete.

  @todo
  Work out what the error return values are for this function.
*/
rfm_wait_time_t
rf_hdr_mc_enable
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const rfm_cdma_band_class_type band,
  const rfm_cdma_chan_type chan
)
{
  rfm_wait_time_t ret_val = 0; /* Return value */

  RF_MSG_4( RF_MED, "rf_hdr_mc_enable : Dev %d, RxLM Handle %d, Band %d, "
            "Chan %d", device, rxlm_handle, band, chan );

  return ret_val;

  } /* rf_hdr_mc_enable */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Disable the HDR subsystem of the CDMA RF driver.

  @details
  Deactivates the HDR portion of the CDMA RF driver.  This API should
  only be called if HDR is in the Sleep state.  If the radio was
  operating in wideband mode, it is reconfigured for narrowband mode.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_HDR_DISABLE_COMPLETE event.  The function will attempt to give
  a best estimate of the time remaining in the return value.

  @param device
  The path to disable the HDR subsystem on.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @return
  The anticipated time in microseconds remaining from the rfm_hdr_disable()
  function return until the operation is complete.

  @todo
  Work out what the error return values are for this function.
*/
rfm_wait_time_t
rf_hdr_mc_disable
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle
)
{
  rfm_wait_time_t ret_val = 0; /* Return Value */
  
  /* Device Status Write Pointer */  
  const rf_cdma_data_status_type *dev_status_r; 

  /*! Get the current device */ 
  dev_status_r = rf_cdma_get_device_status( device ); 

  if ( dev_status_r == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_disable : Dev %d - NULL ptr !", device );
    return RFM_CDMA_ERROR_FAILURE;
  } 
  
  /*Reset curr_chan and carrier_index for all carriers.
   Resetting current device status in hdr_disable can prevent HDR RF related
   failures, e.g., null NV loaded, Cal data error, from propagating to coming
   CDMA mode entering, which will lead to a phone crash due to uninitialiez
   device status in rf_is_device_init().  */
  rf_cdma_init_freq_info_for_device(device);

  rf_cdma_data_set_assoc_dev( device , RFM_INVALID_DEVICE );

  RF_MSG_2( RF_MED, "rf_hdr_mc_disable : Dev %d, RxLM Handle %d",
            device, rxlm_handle  );

  return ret_val;

} /* rf_hdr_mc_disable */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  helper for enable rxagc logging

  @details
  This function will pass rxagc logging callback function to
  rf_hdr_mdsp_enable_rxagc_logging to enable rxagc logging after wakeup_rx
  and during hdr retune api. 
 
  @param device RF Device on which rxagc logging is enabled
 
  @return
  Flag to indicate whether this api success
*/
static 
boolean
rf_hdr_mc_helper_for_enable_rxagc_logging
(
  rfm_device_enum_type device
)
{
  rf_cdma_mdsp_response_cb_param_type rsp_cb_param;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  boolean api_success = TRUE;

  /* Get pointers to device state */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* logical dev pointer cannot be NULL, check before proceeding */
  if ( logical_dev == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_helper_for_enable_rxagc_logging: NULL "
                      " logical dev data for device: %d", device );
    api_success = FALSE;
  }/*if ( logical_dev == NULL )*/
  else
  {
    /* Get FW Response Data */
    fwrsp_hk = rf_cdma_get_fwrsp_data( device );

    /* NULL Pointer Check */
    if ( fwrsp_hk == NULL )
    {
      RF_MSG( RF_ERROR, "rf_hdr_mc_helper_for_enable_rxagc_logging: "
                        "NULL FWRSP Data" );
      api_success = FALSE;
    } /* if ( fwrsp_data == NULL ) */
    else
    {
      /* Enable RxAGC logging */
      fwrsp_hk->rxagc_logging.device = device;
      rsp_cb_param.rsp_handler = rf_hdr_rxagc_logging_cb;
      rsp_cb_param.handler_data = &fwrsp_hk->rxagc_logging;
      api_success = rf_hdr_mdsp_enable_rxagc_logging( 
                            logical_dev->rx_mdsp_path, &rsp_cb_param );
    }
  }/*if ( logical_dev != NULL )*/

  return api_success;
} /* rf_hdr_mc_helper_for_rx_agc_logging */

/*! @} */

/*============================================================================*/
/*!
  @name HDR L1 Radio Control Functions

  @brief
  Functions for the HDR protocol stack to control the radio
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Prepare to turn on the radio and tune to the given band and channel.

  @details
  Prepare the software to move the 1x reciever from the Sleep state to the Rx 
  state. Powers on all circuits required to recieve an 1x signal and 
  prepares all the scripts required to tune the radio to the requested channel.

  For optimal operation, this API should be called with the same band
  and channel the radio was prepared for in the rfm_hdr_enable(). This function 
  should not be used to re-tune the radio if it already awake.

  @param device
  The receive path to wakeup.

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @param num_band_chan
  Number of carriers to be enabled 
 
  @param band
  Contains the band to be tuned to
 
  @param chan_array
  Contains channel list to be tuned to 

  @param carrier_index
  Contains the carrier-mask - the carrier to be enabled

  @param agc_mode
  AGC mode in which receiver is to be tuned in 

  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @retval RFM_CDMA_ERROR_INVALID_DEV_STATE
  Error code when trying to tune the radio when RF Dev is in Invalid State
 
  @retval RFM_CDMA_ERROR_INVALID_DEV
  Error code when trying to tune the radio in an invalid Device
 
  @retval RFM_CDMA_ERROR_FAILURE
  Error code when RF Fails to Tune in spite of good parameter and state
 
  @param meas_type
  mes type to pass in the script type in IRAT or non-IRAT scenarios
  In IRAT scenarios, startup or cleanup script in No-IRAT mode always invalid. 

  @return
  The anticipated time in microseconds remaining from the rfm_hdr_wakeup_rx()
  function return until the operation is complete.
*/
rfm_wait_time_t
rf_hdr_mc_prep_wakeup_rx
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const uint32 num_band_chan,
  const rfm_cdma_band_class_type band,
  const rfm_cdma_chan_type  * const chan_array,
  const int32 * const carrier_index,
  const rfm_hdr_agc_acq_type* const agc_mode,
  boolean is_irat_mode,
  const rfdevice_meas_scenario_type meas_type
)
{
  rfm_wait_time_t ret_val;
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */    
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */ 
  const rf_hdr_config_type* cdma_config_ptr; /* Pointer to NV data */  
  uint8 carrier_idx; /* Variable for carrier indexing */
  rfm_cdma_chan_type chan_tune;/* The actual Channel for tune */

  rf_time_tick_type prof_vreg_t;    /* Vreg Time Profile */
  rf_time_type prof_vreg_t_val = 0; /* 0 in IRAT case */
  rf_time_tick_type prof_prep_t;    /* Prep Time Profile */
  rf_time_type prof_prep_t_val = 0 ;  


  prof_prep_t = rf_time_get_tick() ;

  /*--------------------------------------------------------------------------*/
  /* Get pointers to device state */
  /*--------------------------------------------------------------------------*/
  logical_dev = rfc_cdma_get_logical_device_params( device );
  
  /* Get Writable Device Status */
  dev_status_w = rf_cdma_get_mutable_device_status( device );

  if ( ( logical_dev == NULL ) || ( dev_status_w == NULL ) )
  {
    /* unable to get a valid device pointer - bail out */
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_prep_wakeup_rx: unable to get a valid "
                        "device pointer for device %d ", device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }
  
  if ( rfc_cdma_set_rx_mdsp_path(device, RF_CDMA_MDSP_CHAIN_0) == FALSE )
  {
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;    
  }

  /*--------------------------------------------------------------------------*/
  /* Check for valid NV */
  /*--------------------------------------------------------------------------*/
  /* Get NV for the requested device and band and check if it is NULL */
  cdma_config_ptr = rf_cdma_nv_get_hdr_data( logical_dev->rx_nv_path[band],
                                             band);
  if ( cdma_config_ptr == NULL )
  {
    /* no NV for the requested device and band - bail out */
    RF_MSG_2( RF_ERROR, "rf_hdr_mc_prep_wakeup_rx: NULL NV for device %d on "
                        "band = %d", device, band);
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  /*--------------------------------------------------------------------------*/
  /* Check for null pointer for carrier_index */
  /*--------------------------------------------------------------------------*/
  if ( NULL == carrier_index )
  {
     RF_MSG( RF_ERROR, "rf_hdr_mc_prep_wakeup_rx(): Null pointer passed for "
                       "carrier_index!!");
     ret_val = RFM_CDMA_ERROR_NULL_DATA;
     return ret_val;
  }

  /*--------------------------------------------------------------------------*/
  /* Check for valid number of carriers */
  /*--------------------------------------------------------------------------*/
  if ( num_band_chan > RFM_CDMA_CARRIER_NUM )
  {
    /* Number of carriers exceeds max allowed - bail out */
    RF_MSG_3( RF_ERROR, "rf_hdr_mc_prep_wakeup_rx: Number of carriers for "
                        "device %d is %d, max allowed is %d", device, 
              num_band_chan, RFM_CDMA_CARRIER_NUM ); 
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }


  /*--------------------------------------------------------------------------*/
  /* Enable Chip Power for all devices in the receiver - Device, RxAGC, RFCard*/
  /*--------------------------------------------------------------------------*/
  if ( is_irat_mode == FALSE )
  {
    prof_vreg_t = rf_time_get_tick();
    rfc_cdma_manage_vregs( device, RFM_1XEVDO_MODE, band,
                           RF_PATH_RX_STATE);
    prof_vreg_t_val = rf_time_get_elapsed( prof_vreg_t, RF_USEC );
  }/* if ( is_irat_mode == FALSE )*/

  /*------------------------------------------------------------------------*/
  /* bring carrier info to initial-state: invalidate all carrier_index &	*/
  /* channels																*/
  /*------------------------------------------------------------------------*/
  rf_cdma_init_freq_info_for_device( device );
  
  rf_cdma_data_set_assoc_dev( device, RFM_INVALID_DEVICE );

    /* update only the valid/appropriate carrier_index and channel and band */
    dev_status_w->curr_band = band;
  
  for( carrier_idx = 0; carrier_idx < num_band_chan; ++carrier_idx )
  {
     rf_cdma_data_set_curr_chan( device , carrier_idx , 
                                    chan_array[carrier_idx] );
     rf_cdma_data_set_carrier_index( device , carrier_idx , 
					(uint16)(carrier_index[carrier_idx]) );
  }
  
  /* update BW info */
    rf_cdma_data_set_rx_bw_khz(device, 
                              rf_cdma_compute_bandwidth( num_band_chan, 
                                                  		band,chan_array ));

  /* Determine the TxBW to be used */
  rf_cdma_data_set_tx_bw_khz( device, RF_CDMA_BW_INVALID );
  rf_cdma_data_set_num_carriers( device, num_band_chan );

    /* set rxlm in rf_cdma_data */
    rf_cdma_data_set_rxlm_handle( device, rxlm_handle);

    /*! allocate asd memory*/
    rflm_cmn_asd_allocate_memory((uint32)rxlm_handle);

  /*--------------------------------------------------------------------------*/
  /* Find out the channel to tune to based on carrier-config                  */
  /*--------------------------------------------------------------------------*/
  chan_tune = rf_cdma_compute_center_tune_chan( num_band_chan,
                                                band, 
                                                chan_array);
  /* Temporarily hold channel to be tuned needed in Exec*/
  rf_cdma_data_set_lo_tuned_to_chan( device , chan_tune );
  
 /*Do2G QTA case IRAT we don't change state*/
  if (meas_type != RFDEVICE_MEAS_CLEANUP_SCRIPT)
  {
    /* Update CDMA Data structure with new RF State and  band/Chan */
    rf_cdma_data_set_hdr_rf_state ( device, RF_CDMA_STATE_RX_PREP);
  }

    dev_status_w->agc_mode.mode = agc_mode->mode;
    dev_status_w->agc_mode.params = agc_mode->params;

    /* Populate the total wait time */
    ret_val = 0;

  prof_prep_t_val = rf_time_get_elapsed( prof_prep_t, RF_USEC );
  RF_MSG_2 ( RF_MED, 
             "rfm_hdr_mc_prep_wakeup_rx : Profile - Vreg %d, "
             "Profile - Total Prep Wakeup %d" ,
               prof_vreg_t_val  , prof_prep_t_val ) ;

  return ret_val;

} /* rf_hdr_mc_prep_wakeup_rx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Execute Wakeup Rx sequence to turn on the radio and tune to the given band 
  and channel.

  @details
  This function will send a command to firmware to indicate that Rx needs to be
  started. The command will indicate FW to activate the RxLM configuration 
  along with the RTR script to be programmed. Based on the Settling time
  sent, FW will start the RxAGC as well.
 
  This API also executes the callback as soon as the above steps are executed. 
 
  When support for handling responses from FW is implemented in RFSW, this API
  will return control to HDR L1 as soon as commands are executed. The callback 
  function will then be moved to the command handler, which would execute the 
  callback when FW sends a response to start_agc command.
 
  @param device
  The receive path to execeute wakeup rx sequence.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.
 
  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @param meas_type
  mes type to pass in the cript type in IRAT or non-IRAT scenarios
  In IRAT scenarios, startup or cleanup script in No-IRAT mode always invalid. 
 
  @retval RFM_CDMA_ERROR_INVALID_DEV_STATE
  Error code when trying to tune the radio when RF Dev is in Invalid State
 
  @retval RFM_CDMA_ERROR_INVALID_DEV
  Error code when trying to tune the radio in an invalid Device
 
  @retval RFM_CDMA_ERROR_FAILURE
  Error code when RF Fails to Tune inspite of good parameter and state

  @retval RFM_CDMA_ERROR_INVALID_MODE
  Error code when trying to tune the radio when RF Mode is not correct.
 
  @return
  The anticipated time in microseconds remaining from the rfm_hdr_wakeup_rx()
  function return until the operation is complete.
*/
rfm_wait_time_t
rf_hdr_mc_exec_wakeup_rx
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  boolean is_irat_mode,
  const rfdevice_meas_scenario_type meas_type
)
{
  rfm_wait_time_t ret_val = 0 ; /* Return value */
  rf_time_tick_type wakeup_exec_time;      /* Warmup Time */
  rf_time_type wakeup_exec_time_val;
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  rfm_cdma_power_mode_type power_mode; /* starting power mode */
  rfm_cdma_band_class_type band;
  boolean exec_success = TRUE; /* tracks success of various APIs called */
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;
  uint32 script_index ;
  rf_cdma_msm_dynamic_rxlm_cfg_type dyn_settings; /* Dynamic RXLM  Settings */

  boolean rx_agc_success = TRUE; /* Flag to track success of rx agc */
  uint32 adc_clock_freq; /* var to store ADC Clock Frequency */
  rfm_cdma_chan_type chan_tune;/* The actual Channel for tune */
  rf_time_tick_type prof_rxlm_t;    /* RXLM Time Profile */
  rf_time_tick_type prof_rxagc_cfg_t; /* RxAGC Config Time Profile */
  rf_time_tick_type prof_ssma_t;          /* SSMA Time Profile */
  rf_time_type prof_rxagc_cfg_t_val;
  rf_time_type prof_ssma_t_val;
  rf_time_type prof_rxlm_t_val;
  rfm_hdr_agc_acq_type agc_mode ;
  boolean dev_voting_status = TRUE; /* Need the flag to perform device voting */
  rf_time_tick_type prof_core_and_hal_t;    /* Vreg Time Profile */
  rf_time_type prof_core_and_hal_t_val = 0 ; /* 0 in IRAT case */
  rfm_device_enum_type tq_device; /* used during IRAT call flow */


  /* start time profiling for API */
  wakeup_exec_time = rf_time_get_tick();

  fwrsp_hk = rf_cdma_get_fwrsp_data( device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_exec_wakeup_rx: NULL FWRSP Data" );
    return RFM_CDMA_ERROR_NULL_DATA;
  } /* if ( fwrsp_data == NULL ) */

  /* Get Device Status */
  dev_status_w = rf_cdma_get_mutable_device_status( device );
  
  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );
  
  /* check for NULL pointers */
  if ( ( dev_status_w == NULL ) || ( logical_dev == NULL ) || 
       ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM ) )
  {
     RF_MSG_1( RF_ERROR, "rf_hdr_mc_exec_wakeup_rx: NULL data for device %d",
                          device );
     return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* check for out of bound condition */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM)
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_exec_wakeup_rx: rx_mdsp_path is "
                         "out of bound %d", logical_dev->rx_mdsp_path  );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
  }

  /* check for valid band */
  band = dev_status_w->curr_band;

  if ( band >= RFM_CDMA_MAX_BAND )
  {
    RF_MSG_1 ( RF_ERROR, "rf_hdr_mc_exec_wakeup_rx: Invalid band %d ", band );
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }

  /*-----------Update the Antennae switch----------- */
#if defined(FEATURE_RF_ASDIV) || defined(FEATURE_RF_HAS_TPTUNER_SWITCH)
  rf_cdma_data_set_ant_position ( device , ((uint8)
                                 rfcommon_asdiv_get_current_position ( device ) ) ) ;
  RF_MSG_1 ( RF_MED , "rf_hdr_mc_exec_wakeup_rx: Ant Switch position %d ", 
             rf_cdma_data_get_ant_position ( device ) ) ;
#else
  rf_cdma_data_set_ant_position ( device , ((uint8) (0) ) ) ;
#endif

  /* -------------------------- Update Alt. Path ---------------------------*/
  exec_success &= rf_cdma_mc_update_alt_rx_path( RFM_1XEVDO_MODE, device,
                                                band , 
                                                is_irat_mode );
  /* ------------------------End of Update Alt. Path -----------------------*/

  /* Use device stored seperately to allocate TQ, in order to support QxTA.
     tq_device is usually the source tech primary device */
  if ( is_irat_mode == FALSE )
  {
    tq_device = device;
  }
  else
  {
    tq_device = dev_status_w->irat_data.irat_tq_device;
  }

  /* allocate resources (data structs, queues, etc) needed for 1x operation */ 
  rf_cdma_mdsp_modem_rx_allocate_resources( tq_device ,rxlm_handle,
                                            RFM_1XEVDO_MODE , is_irat_mode ) ;

  /* Code for putting the WTR into Low power mode, followed up by putting the
  required WTR into power on mode */
  {
    prof_core_and_hal_t = rf_time_get_tick();
    
    /* vote for RFCMD app - this should done as early as possible in the 
       wakeup sequence */
    if ( is_irat_mode == FALSE )
    {
      rf_hal_bus_client_handle* handle = 
                                rf_cdma_data_get_rf_hal_bus_vote_handle(device);
      #ifndef FEATURE_RFA_ATLAS_MODEM
      exec_success &= 
               rf_hal_bus_enable (TRUE, &handle, RF_HAL_RFCMD_APP_CLIENT(EVDO));
      #endif
      exec_success &= rf_cdma_data_set_rf_hal_bus_vote_handle(device, handle);

      /* Vote On for all RF Devices which are needed with RFM Device (device) 
         This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */
      dev_voting_status = rfcommon_core_dev_power_vote(device,
                                                       RFCMN_CORE_DEV_ON,
                                                       RFCMN_CORE_DEV_NORMAL_VOTE);
      if( dev_voting_status == FALSE )
      {
        RF_MSG_1( RF_ERROR, "rf_hdr_mc_exec_wakeup_rx(): Device: %d TCXO buffer pwr up failed",
                  device);
      }
    }

    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      dev_voting_status &= 
                    rfdevice_cdma_rx_pwr_wakeup(logical_dev->rx_device[band]);
      if( dev_voting_status == FALSE )
      {
        RF_MSG_1( RF_ERROR, "rf_hdr_mc_exec_wakeup_rx(): Device: %d chip pwr up failed",
                  device);
      }
    }

    exec_success &= dev_voting_status;
    prof_core_and_hal_t_val = rf_time_get_elapsed( prof_core_and_hal_t , RF_USEC );
  }

  /* check if all operations are successful till now */
  if ( exec_success == TRUE )
  {
    rxf_get_iqmc_coef_type iqmc_data; /* IQMC Coefficient */    
    rf_cdma_msm_static_rxlm_cfg_type static_cfg; /* Static RXLM  Settings */

  /*--------------------------------------------------------------------------*/
  /* Determine ADC clock frequency */
  /*--------------------------------------------------------------------------*/
    chan_tune = dev_status_w->lo_tuned_to_chan ;
  adc_clock_freq = rf_cdma_msm_get_adc_samp_freq( rxlm_handle, band, 
                   chan_tune );


    /* init dynamic settigs before any RXLM operations */
    rf_cdma_msm_init_dyn_settings(&dyn_settings);

    /*------------------------------------------------------------------------*/
    /* Populate RxLM static settings  */
    /*------------------------------------------------------------------------*/
    static_cfg.adc_clock_freq = adc_clock_freq;
    static_cfg.bw_khz = rf_cdma_data_get_rx_bw_khz ( device ) ;
    static_cfg.rxlm_adc = logical_dev->rxlm_adc;
    static_cfg.config_as_et = FALSE;

    /* Tell IC SM it is primary device. 
       Make sure this is the 1st time IC SM is touched */
    (void) rf_cdma_set_ic_support ( &(dev_status_w->intelliceiver) , TRUE ) ;
    /*--------------------------------------------------------------------------*/
    /* Determine Power Mode for desired band and channel */
    /*--------------------------------------------------------------------------*/
    power_mode = rf_cdma_ic_get_starting_power_mode(
                     &(dev_status_w->intelliceiver), 
                     band,
                     chan_tune,
                     dev_status_w->rx_bw_khz );

    /*------------------------------------------------------------------------*/
    /* SSMA operations below                                                    */
    /*------------------------------------------------------------------------*/
    prof_ssma_t = rf_time_get_tick();

    exec_success &= rf_cdma_get_ssma_recos ( device, rxlm_handle,
                                               band, chan_tune,
                                               logical_dev->paired_device );

    /* now read the spur-info table in NV to see if any spurs need to be
     supressed *** The notches will be suppressed when RXLM is configured     */
    exec_success &= rf_cdma_get_spurs_to_suppress( device, rxlm_handle, band,
                      chan_tune, dev_status_w->num_carriers, &dyn_settings );

    /* Update the number of notches used on this device (for tracking use 
    for SSMA callback), notches are per wb chain, the notch use is tracked
    per wb chain, hence use carrier index zero */
    dev_status_w->ssma_cb_data.num_notch_filters_used = 
                    dyn_settings.notch_filt_settings[0].num_notch_filters_used;

    prof_ssma_t_val = rf_time_get_elapsed( prof_ssma_t, RF_USEC );
    /*------------------------------------------------------------------------*/

    /* Populate the IQMC Data for query */
    iqmc_data.lin = power_mode;
    iqmc_data.band = band;
    iqmc_data.chan = chan_tune;
    iqmc_data.sec_chain_rf_state = RF_CDMA_STATE_INVALID;
    iqmc_data.bw_khz = static_cfg.bw_khz;

    /* Get RSB coefficients from device driver */
    rfdevice_cdma_rx_cmd_dispatch( logical_dev->rx_device[band],
                                   RFDEVICE_GET_IQMC_COEFS,
                                   &iqmc_data );

     /* update the IQMC  settings */
    dyn_settings.apply_dynamic_cfg = TRUE;
    dyn_settings.iqmc_coeff_in = iqmc_data.iqmc_a_coeff;
    dyn_settings.iqmc_coeff_quad = iqmc_data.iqmc_b_coeff;
    dyn_settings.num_carriers = dev_status_w->num_carriers;
    dyn_settings.modem_chain =
    logical_dev->rfc_dev_info.rfc_device_info[band]->modem_chain ;


    /*------------------------------------------------------------------------*/
    /* Populate RxLM dynamic settings based on carrier-config  */
    /*------------------------------------------------------------------------*/
    rf_cdma_compute_carrier_info( dev_status_w->curr_band,
                                  dev_status_w->curr_chans,
                                  dev_status_w->num_carriers, 
                                  dev_status_w->carrier_index, 
                                  &dyn_settings.carrier_mask,
                                  dyn_settings.freq_offset_hz,
                                  0 /* no need to add anything to calculated
                                       freq offset */
                                 );

    /*------------------------------------------------------------------------*/
    /* Configure RxLM for given Device, Power Mode, and ADC Sample Frequency  */
    /*------------------------------------------------------------------------*/
    prof_rxlm_t = rf_time_get_tick();
    exec_success &= rf_cdma_msm_configure_rxlm( rfdevice_id_get(logical_dev->rx_device[band]),
                                                  rxlm_handle,
                                                  RFM_1XEVDO_MODE, 
                                                  band,
                                                  power_mode, 
                                                  static_cfg, dyn_settings );
    prof_rxlm_t_val = rf_time_get_elapsed( prof_rxlm_t, RF_USEC );
    
    /*------------------------------------------------------------------------*/
    /* Configure RxAGC */
    /*------------------------------------------------------------------------*/
    prof_rxagc_cfg_t = rf_time_get_tick();

    /*Do2G QTA case we don't run configure Rx AGC*/
    if (meas_type != RFDEVICE_MEAS_CLEANUP_SCRIPT)
    {
    agc_mode.mode = dev_status_w->agc_mode.mode ;
    agc_mode.params =  dev_status_w->agc_mode.params ;
    /*Store failure/success of configure RxAGC in rx_agc_success flag*/
    rx_agc_success &= rf_hdr_configure_rx_agc( device, rxlm_handle,
                                               dev_status_w->num_carriers,
                                               dev_status_w->curr_band,
                                               dev_status_w->curr_chans,
                                               chan_tune,
                                               &agc_mode, power_mode,
                                               RF_CDMA_STATE_RX_PREP,
                                               is_irat_mode ,
                                               dev_status_w->carrier_index );
    }
    exec_success &= rx_agc_success;
    prof_rxagc_cfg_t_val = rf_time_get_elapsed( prof_rxagc_cfg_t, RF_USEC );

    /* Return BAD CONFIG error code if configure RxAGC failed*/
    if(  rx_agc_success == FALSE )
    {
      ret_val = RFM_CDMA_ERROR_BAD_CONFIG;                                             
    }
    else
    {
      ret_val = RFM_CDMA_ERROR_FAILURE;
    }
  } /* if ( wakeup_success == TRUE ) */

  /* Check if all Static Initialization when through. If not, return Failure.
     For successful execution, do the Rx Wakeup/Tune hardware sequence */
  if ( exec_success == TRUE )
  {
    /* Enable RxAGC logging */
    rf_hdr_mc_helper_for_enable_rxagc_logging( device );

    /* prep_wakeup succeeded at this point - safe to update SSMA recos for
       paired device now, and also safe to update device status */
    (void) rf_cdma_update_bb_ssma_recos( logical_dev->paired_device );

    /* update the RF SSMA recos if applicable */
    rf_cdma_update_rf_ssma_recos (logical_dev->paired_device);

    /* update the rotator offsets to the device data structure */
    rf_cdma_data_update_rot_offsets( device, dyn_settings.freq_offset_hz );

  
    /* Record the execution time statistics for prep-wakeup logging */
    RF_MSG_4 ( RF_MED, "rfm_hdr_mc_exec_wakeup_rx :  "
               "HalCore: %d, RxLM %d, RxAGC Config %d, SSMA_time %d", 
               prof_core_and_hal_t_val , prof_rxlm_t_val , 
               prof_rxagc_cfg_t_val, prof_ssma_t_val );

  if ( is_irat_mode == FALSE )
  {
    /*--------------------------------------------------------------------------*/
    /* Determine Power Mode for desired band and channel */
    /*--------------------------------------------------------------------------*/
    power_mode = rf_cdma_ic_get_starting_power_mode( 
                                   &(dev_status_w->intelliceiver), band,
                                   dev_status_w->lo_tuned_to_chan,
                                   dev_status_w->rx_bw_khz );

    /*--------------------------------------------------------------------------*/
    /*  Create Analog Receiver Programming Script for given channel and send
        RX_START command to FW                                                  */
    /*--------------------------------------------------------------------------*/
    script_index = 
        rf_cdma_mdsp_get_device_buffer_index ( RFM_1XEVDO_MODE, rxlm_handle ) ;
  }/* if ( is_irat_mode == FALSE )*/
  else
  {
    /* Set lowest power mode in IRAT scenario */
    power_mode = rf_cdma_ic_get_lowest_power_mode 
                                          ( &( dev_status_w->intelliceiver )) ;
    script_index =  dev_status_w->irat_data.irat_buffer_index ;
  }

  rf_cdma_data_set_script_index( device, script_index ) ;

  /* configure RF front-end to enable the RX chain and send FW command */
  if ( exec_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section(
       logical_dev->rx_device[dev_status_w->curr_band]);
  
    exec_success &= rf_cdma_mc_configure_front_end_for_rx_wakeup( device,
                        RFM_1XEVDO_MODE, FALSE /* is_slave_device */,
                        rxlm_handle, band, 
                        dev_status_w->lo_tuned_to_chan, power_mode, 
                        dev_status_w->script_index, 
                        dev_status_w->rx_bw_khz, dev_status_w->tx_bw_khz, 
                        logical_dev,
                        &dev_status_w->ant_tuner_script_token_data,
                        is_irat_mode,
                        meas_type,
                        FALSE/* immediate_write */);
  
    if ( exec_success == TRUE )
    {
      exec_success &= rf_hdr_mc_send_start_rx_helper ( device , 
                                                        (const rf_cdma_data_status_type*) dev_status_w , 
                                                         rxlm_handle ,
                                                         logical_dev,
                                                         is_irat_mode,
                                                         FALSE /*is_retune*/,
                                                         FALSE /* is Drx */) ;

      if ( ( exec_success == TRUE ) && (is_irat_mode == FALSE) )
      {
        /* do not need to apply notches while in irat (design decision) */
        /* Apply notch filter recommendations from WTR */
        exec_success &= 
        rf_cdma_mc_trigger_dynamic_notch_update(device,
                            dev_status_w->ssma_cb_data.ssma_reco,
                            dev_status_w->ssma_cb_data.num_notch_filters_used,
                            dev_status_w->ssma_cb_data.clear_notch); 
      }

      if ( logical_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script(
          logical_dev->ant_tuner[band],
          &dev_status_w->ant_tuner_script_token_data );
      }

    } /* if ( exec_success == TRUE ) */
  
    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section(
       logical_dev->rx_device[dev_status_w->curr_band]); 
  
  } /* if ( exec_success == TRUE ) */

  /* Dont clean-up in the iRAT case. Let the cleanup API do it */
  if ( is_irat_mode == FALSE )
  {
    /* Call helper API to cleanup static event and Rx on/off events */
    exec_success &= rf_hdr_mc_cleanup_rx_ccs_events( device );

    /* Unconditionally clean-up the Rx AGC LNA SM events here */
    exec_success &= rf_cdma_mc_cleanup_rx_agc_lna_ccs_events( device );
  }

  /*--------------------------------------------------------------------------*/
  /* Configure Antenna Tuner */
  /*--------------------------------------------------------------------------*/

  #ifdef FEATURE_RF_HAS_QTUNER

  if ( exec_success == TRUE )
  {
    /*Program QFE for Rx mode*/
    /* Enable both PRx and DRx tuners in at the time of Rx init so that when
       doing IRAT/meas the init scripts for DRx need not be generated and executed.
       This is to avoid adding burden to IRAT timeline when enabling
       DRx for IRAT/meas (e.g. X2L).
    */
    band = dev_status_w->curr_band;

  } /* if ( exec_success == TRUE ) */

  #endif /* FEATURE_RF_HAS_QTUNER */


  /* continue with wakeup sequence only if command to FW was successful */
  if ( exec_success == TRUE )
  {    
      int32 dummy_result;

    if( dev_status_w->intelliceiver.power_mode == RFM_CDMA_POWER_MODE_NORMAL)
    {
      rfdevice_cdma_rx_post_pll_lock_processing( 
         logical_dev->rx_device[dev_status_w->curr_band] );
    }
    
    /* Start the IntelliCeiver if the Analog Reciever supports/requires it ,
    additionally check for IRAT mode, if we are in IRAT we should skip all
    Intelliceiver operations. IRAT is designed to be only in a single power
    mode */
    if ( ( dev_status_w->intelliceiver.ic_supported == TRUE ) &&
         ( is_irat_mode == FALSE ))
    {
      rf_cdma_ic_start( &(dev_status_w->intelliceiver), logical_dev,
                            dev_status_w->curr_band,
                            dev_status_w->lo_tuned_to_chan );
    }
    else
    {
      /* Print F3 */
      RF_MSG_2( RF_LOW, "rf_hdr_mc_exec_wakeup_rx: skipping IC Start :"
                          "ic_supported: %d ; is_irat_mode:%d", 
                dev_status_w->intelliceiver.ic_supported, 
                is_irat_mode ); 
    }/*if ( ( dev_status_w->intellic.....*/
    
    /*DO2G QTA IRAT case don't need state update,however for regular IRAT mode
    update device state here. For wakeup Rx being called in non IRAT scenarios
    the device state is updated after RxAGC has converged. i.e after we get
    the Track Indicator response from FW */  
    if ((is_irat_mode == TRUE )&&
        (meas_type != RFDEVICE_MEAS_CLEANUP_SCRIPT))
    {    
    /* Update CDMA Data structure with new RF State and  band/Chan */
      rf_cdma_data_set_hdr_rf_state( device, RF_CDMA_STATE_RX );
    }
    
    /* Populate the total wait time */
    ret_val = 0;

      /* Tell GPS about the band and channel we are on. */
      RFGNSS_MC(evdo_band_chan, dummy_result)(band,
                                              chan_tune);

      if (dummy_result == FALSE  )
      {
        RF_MSG( RF_ERROR, "rfgnss_mc_evdo_band_chan API failed");
      }

  } /* if ( exec_success == TRUE )*/
  else
  {
      ret_val = RFM_CDMA_ERROR_FAILURE;
    }
  }

  if ( exec_success == FALSE )
  {
    /* undo changes done to driver state machine */
    rf_cdma_process_wakeup_failure( device, RFM_1XEVDO_MODE, band );  

    /* invalidate all SSMA recos for paired device that were retrieved by the
       rf_cdma_get_ssma_recos() call above */
    rf_cdma_init_ssma_recos( logical_dev->paired_device );

    /* Report failure */
    RF_MSG_2( RF_ERROR, "rf_hdr_mc_exec_wakeup_rx: Failed!! Dev %d, RxLM %d",
              device, dev_status_w->rxlm_handle );
  }

  /* get the time spent in this API */
  wakeup_exec_time_val = rf_time_get_elapsed( wakeup_exec_time, RF_USEC );
  rf_cdma_data_sanity_check( device );
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&rx_hdr_wakeup_PRx_time_hist, (uint32)wakeup_exec_time_val);
    if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
    {
      rf_hdr_clear_profiling_hist_packet();
    }
  #endif
  /* Report time taken */
  RF_MSG_3( RF_MED, 
            "rf_hdr_mc_exec_wakeup_rx: Device %d, RxLM %d, time taken %d us", 
            device, dev_status_w->rxlm_handle, wakeup_exec_time_val);

  return ret_val;

} /* rf_hdr_mc_exec_wakeup_rx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This is a helper function to enable retuning a device to a given band and 
  channel. Refer to rf_hdr_mc_retune() for more details
 
  @param device
  The radio path to re-tune.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.
 
  @param num_band_chan
  The length of the band_chan parameter array.

  @param band
  The band to tune to.
 
  @param chan_array
  An array of channels for tuning multiple carriers in 1xEV-DO revB 
  multi-carrier mode.
 
  @param carrier_index
  An array in integers that specify the carrier the corresponding channel
  should be placed on in the multi-carrier Rx front.  This carrier will
  deliver samples into the corresponding sample RAM for demodulation.
 
  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @return
  The anticipated time in microseconds remaining from the rfm_hdr_tune()
  function return until the operation is complete, or an error code.
  If the API call succeeds the return value will be non-negative; all
  error codes will be negative.
*/
rfm_wait_time_t
rf_hdr_mc_helper_for_retune
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const uint32 num_band_chan,
  const rfm_cdma_band_class_type band,
  const rfm_cdma_chan_type * const chan_array,
  const int32 * const carrier_index,
  const boolean is_slave_device,
  boolean is_irat_mode
)
{
  rfm_wait_time_t ret_val; /* Return value */
  boolean retune_success = TRUE; /* Flag to track the success of rx retune */
  boolean rx_agc_success = TRUE; /* Flag to track success of RxAGC*/
  rf_time_tick_type retune_time; /* Re-Tune Time */
  rf_time_type retune_time_val;
  rfm_cdma_power_mode_type power_mode; /* var to store Power Mode */
  uint32 adc_clock_freq; /* var to store ADC Clock Frequency */
  rfm_hdr_agc_acq_type agc_mode; /* AGC Mode to be set after HHO is done */
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */
  rxf_get_iqmc_coef_type iqmc_data; /* IQMC Coefficient */  
  rf_cdma_msm_static_rxlm_cfg_type static_cfg; /* Static RXLM  Settings */
  rf_cdma_msm_dynamic_rxlm_cfg_type dyn_settings; /* Dynamic RXLM  Settings */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  uint8 carrier_idx; /* Variable for carrier indexing */
  rfm_cdma_chan_type chan_tune;/* The actual channel for tune */
  const rf_hdr_config_type *rf_hdr_config_ptr;   /* Pointer to HDR RF NV data */
  uint32 tx_bw_khz;
  rf_time_tick_type prof_ssma_t;          /* SSMA Time Profile */
  rf_time_type prof_ssma_t_val;
  rf_hdr_mdsp_rx_agc_read_type rx_agc_read; /* RxAGC Read */
  uint32 script_index;   /* Store device buffer index */
  rfcommon_fbrx_tx_param_type fbrx_tx_params;

  /* snap-shot start time of API */
  retune_time = rf_time_get_tick();

  /* Get Read-only and Writable Device Status */
  dev_status_r = rf_cdma_get_device_status( device );
  dev_status_w = rf_cdma_get_mutable_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );
  
  /* Check for NULL pointers */
  if ( ( logical_dev == NULL ) || ( dev_status_w == NULL ) || 
       ( dev_status_r == NULL ))
  {
    /* unable to get a valid device pointer - bail out */
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_helper_for_retune: unable to get a valid "
                        "device pointer for device %d ", device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }
  
  chan_tune = rf_cdma_compute_center_tune_chan(num_band_chan, band, chan_array);

  /* ensure that NV is valid for given device and band */
   rf_hdr_config_ptr = rf_cdma_nv_get_hdr_data( logical_dev->rx_nv_path[band]
                                                , band );

  if ( NULL == rf_hdr_config_ptr )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_helper_for_retune: Failed to retrieve valid"
              " Config data from NV for band %d", band); 
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  /*check value of num_carriers*/
  if (dev_status_r->num_carriers > RFM_CDMA_CARRIER_CUMULATIVE) 
  { 
    retune_success = FALSE;
    RF_MSG_3( RF_ERROR, "rf_hdr_mc_helper_for_retune: Number of carriers cannot "
                        "exceed %d for device :%d, num carriers is:%d",
                        RFM_CDMA_CARRIER_NUM, device, dev_status_r->num_carriers ); 
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/
  
  /* Request power off for old band */
  /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    retune_success &= 
                  rfdevice_cdma_rx_pwr_sleep(
                     logical_dev->rx_device[dev_status_r->curr_band]);
  }

  /* Determine ADC clock frequency */
  adc_clock_freq = rf_cdma_msm_get_adc_samp_freq(rxlm_handle, band, chan_tune);
      
  /* Stop the IntelliCeiver and save the state for the next wakeup on this
  current channel if this device supports/requires it */
  if ( dev_status_w->intelliceiver.ic_supported )
  {
    rf_cdma_ic_stop( rf_cdma_get_mutable_intelliceiver( device ), 
                     logical_dev );
  }

  /*--------------------------------------------------------------------------*/
  /* Query Rx sleep script before sending RX_STOP to clear possible           */ 
  /* notches when chain is still active. The tranceiver sleep script API call */
  /* will trigger notch update, which needs Rx Chain to be active. So querying*/ 
  /* the script before Rx_stop                                                */
  /*--------------------------------------------------------------------------*/

  if ( retune_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[dev_status_r->curr_band] );

    retune_success &= rf_cdma_mc_configure_front_end_for_rx_sleep ( 
                                                     device, 
                                                     RFM_1XEVDO_MODE,
                                                     rxlm_handle, 
                                                     is_slave_device, 
                                                     dev_status_r->curr_band,
                                                     dev_status_r->script_index,
                                                       dev_status_r->rx_bw_khz, 
                                                       dev_status_r->tx_bw_khz,
                                                     logical_dev,
                                                     &dev_status_w->ant_tuner_script_token_data,
                                                     TRUE /* immediate_write*/,
                                                     FALSE /* is_irat_mode */);
    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[dev_status_r->curr_band] );
  }

  /*--------------------------------------------------------------------------*/
  /* signal FW to stop_rx                                                     */
  /*--------------------------------------------------------------------------*/
  retune_success &= rf_hdr_mc_send_stop_rx_helper ( device , dev_status_r ,
                                    rxlm_handle, logical_dev, 
                                    TRUE /* skip_rf_front_end_programming */,
                                    FALSE /* is Drx*/) ; 
  if ( FALSE == retune_success )
  {
    RF_MSG ( RF_ERROR , "rf_hdr_mc_helper_for_retune: "
                        "rf_hdr_mc_rx_stop_helper() returned failure " );
  }
  
  /* Request power on for new band */
  /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    retune_success &= 
                  rfdevice_cdma_rx_pwr_wakeup(logical_dev->rx_device[band]);
  }

  /* -------------------------- Update Alt. Path ---------------------------*/
  retune_success &= rf_cdma_mc_update_alt_rx_path( RFM_1XEVDO_MODE, device,
                                                band , 
                                                FALSE /* No retune in IRAT*/);
  /* ------------------------End of Update Alt. Path -----------------------*/

  /*------------------------------------------------------------------------*/
  /* bring carrier info to initial-state: invalidate all carrier_index &    */
  /* channels                                                               */
  /*------------------------------------------------------------------------*/
  rf_cdma_init_freq_info_for_device(device);

  dev_status_w->curr_band = band;
  for( carrier_idx = 0; carrier_idx < num_band_chan; ++carrier_idx )
  {
    dev_status_w->curr_chans[carrier_idx] = 
                            chan_array[carrier_idx];
    dev_status_w->carrier_index[carrier_idx] = 
                            (uint16)(carrier_index[carrier_idx]);
  }

  rf_cdma_data_set_num_carriers( device , num_band_chan );

  /* Populate static settings */
  static_cfg.adc_clock_freq = adc_clock_freq;
  static_cfg.bw_khz = rf_cdma_compute_bandwidth(num_band_chan, band,
                                                chan_array);
  static_cfg.rxlm_adc = logical_dev->rxlm_adc;
  static_cfg.config_as_et = FALSE;

  if ( dev_status_r->data_hdr.rf_state == RF_CDMA_STATE_RXDIV )
  {
    tx_bw_khz = RF_CDMA_BW_INVALID;
    rf_cdma_data_set_tx_bw_khz( device , tx_bw_khz);
  }
  else if ( dev_status_r->data_hdr.rf_state == RF_CDMA_STATE_RXTX )
  {
    tx_bw_khz = static_cfg.bw_khz;
    rf_cdma_data_set_tx_bw_khz( device , tx_bw_khz);
  }
  else
  {
    tx_bw_khz = RF_CDMA_BW_INVALID;
    rf_cdma_data_set_tx_bw_khz( device , tx_bw_khz );
  }

  rf_cdma_data_set_rx_bw_khz(device,static_cfg.bw_khz);

  /* init dynamic settigs before any operations */
  rf_cdma_msm_init_dyn_settings(&dyn_settings);
  dyn_settings.modem_chain =
  logical_dev->rfc_dev_info.rfc_device_info[band]->modem_chain ;

  /* Disable rx AGC logging */
  rf_hdr_mdsp_disable_rxagc_logging(logical_dev->rx_mdsp_path);

  /* Determine Power Mode for desired band and channel */
  power_mode = rf_cdma_ic_get_starting_power_mode(
                   &(dev_status_w->intelliceiver), band, chan_tune,
                   dev_status_r->rx_bw_khz );

  /*--------------------------------------------------------------------------*/
  /* SSMA operations below                                                    */
  /*--------------------------------------------------------------------------*/
  prof_ssma_t = rf_time_get_tick();

  retune_success &= rf_cdma_get_ssma_recos ( device, rxlm_handle, band, 
                                             chan_tune, 
                                             logical_dev->paired_device);

  /* now read the spur-info table in NV to see if any spurs need to be
     supressed *** The notches will be suppressed when RXLM is configured ***
  */
  retune_success &= rf_cdma_get_spurs_to_suppress( device, rxlm_handle, band, 
                          chan_tune, dev_status_w->num_carriers, &dyn_settings);

  /* Update the number of notches used on this device (for tracking use 
  for SSMA callback), notches are per wb chain, the notch use is tracked
  per wb chain, hence use carrier index zero */
  dev_status_w->ssma_cb_data.num_notch_filters_used = 
                    dyn_settings.notch_filt_settings[0].num_notch_filters_used;

  prof_ssma_t_val = rf_time_get_elapsed( prof_ssma_t, RF_USEC );

  /*--------------------------------------------------------------------------*/
  /* Configure RxLM */
  /*--------------------------------------------------------------------------*/

  /* Populate the IQMC Data for query */
  iqmc_data.lin = power_mode;
  iqmc_data.band = band;
  iqmc_data.chan = chan_tune;
  iqmc_data.sec_chain_rf_state = RF_CDMA_STATE_INVALID;
  iqmc_data.bw_khz = static_cfg.bw_khz;
  
  /* Get RSB coefficients from device driver */
  rfdevice_cdma_rx_cmd_dispatch( logical_dev->rx_device[band],
                                 RFDEVICE_GET_IQMC_COEFS,
                                 &iqmc_data );
     
  dyn_settings.apply_dynamic_cfg = TRUE;
  dyn_settings.iqmc_coeff_in = iqmc_data.iqmc_a_coeff;
  dyn_settings.iqmc_coeff_quad = iqmc_data.iqmc_b_coeff;
  dyn_settings.num_carriers = dev_status_w->num_carriers;
  dyn_settings.modem_chain =
  logical_dev->rfc_dev_info.rfc_device_info[band]->modem_chain ;

  rf_cdma_compute_carrier_info( dev_status_w->curr_band,
                                dev_status_w->curr_chans,
                                dev_status_w->num_carriers, 
                                dev_status_w->carrier_index, 
                                &dyn_settings.carrier_mask,
                                dyn_settings.freq_offset_hz, 
                                0 /* no need to add anything to calculated
                                     freq offset */
                              );
  
  retune_success &= rf_cdma_msm_configure_rxlm( 
                                rfdevice_id_get(logical_dev->rx_device[band]),
                                rxlm_handle,
                                RFM_1XEVDO_MODE,
                                band,
                                power_mode, static_cfg,
                                dyn_settings );
  
  /*--------------------------------------------------------------------------*/
  /* Configure RxAGC */
  /*--------------------------------------------------------------------------*/
  /* Populate Post HHO RxAGC Param */
  agc_mode.mode = RFM_HDR_AGC_MODE_NORMAL;

  dev_status_w->agc_mode.mode = agc_mode.mode;

  /*Store failure/success of configure RxAGC in rx_agc_success flag*/
  rx_agc_success &= rf_hdr_configure_rx_agc( device, rxlm_handle,
                                             dev_status_r->num_carriers,
                                             dev_status_r->curr_band,
                                             dev_status_r->curr_chans,
                                             chan_tune,
                                             &agc_mode, power_mode,
                                             dev_status_r->data_hdr.rf_state,
                                             FALSE,/* is_irat_mode */
                                             dev_status_r->carrier_index );

  retune_success &= rx_agc_success;

  /* Enable RxAGC logging */
  rf_hdr_mc_helper_for_enable_rxagc_logging( device );

  /*------------------------- END OF STATIC INIT -----------------------------*/
  /* No Static de-initialization can be done after this point. Only Sleep   
     hardware sequence is executed                                            */
  /*--------------------------------------------------------------------------*/

  /* Update script index for rx */ 
  script_index = rf_cdma_mdsp_get_device_buffer_index( RFM_1XEVDO_MODE,
                                                                rxlm_handle );

  rf_cdma_data_set_script_index( device, script_index ); 

  /*--------------------------------------------------------------------------*/
  /* configure RF front-end to enable the RX chain and send FW command        */
  /*--------------------------------------------------------------------------*/
  if ( retune_success == TRUE )
  {

    rfdevice_cdma_rx_enter_critical_section(logical_dev->rx_device[band]);
  
    retune_success &= rf_cdma_mc_configure_front_end_for_rx_wakeup (
                                      device, RFM_1XEVDO_MODE, is_slave_device,
                                      rxlm_handle, band, chan_tune, 
                                      power_mode, dev_status_r->script_index, 
                                      static_cfg.bw_khz, 
                                      tx_bw_khz,
                                      logical_dev,
                                      &dev_status_w->ant_tuner_script_token_data,
                                      FALSE /* is_irat_mode */, 
                                      RFDEVICE_MEAS_INVALID_SCRIPT,
                                      TRUE /* immediate_write */);
  
    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[band] );
  
    if ( retune_success == TRUE )
    {
      retune_success &= rf_hdr_mc_send_start_rx_helper ( device , dev_status_r , 
                                                         rxlm_handle ,
                                                         logical_dev,
                                                         is_irat_mode,
                                                         TRUE /*is_retune*/ ,
                                                         FALSE /* is Drx */) ;
    } /* if ( retune_success == TRUE ) */
    else
    {
      RF_MSG( RF_ERROR,"rf_hdr_mc_helper_for_retune: "
                       "Retune failure, skipping mdsp Rx start " );
    }
  
    /* Call helper API to cleanup static event and Rx on/off events */
    retune_success &= rf_hdr_mc_cleanup_rx_ccs_events( device );

    if( retune_success == TRUE )
    {
      /* Wait for RxAGC to converge */
      rf_hdr_mc_rx_agc_convergence( device );

      /* Get Final RxAGC/ LNA State */
      rf_hdr_mdsp_get_rx_agc_data( logical_dev->rx_mdsp_path, &rx_agc_read );

      rx_agc_read.rx_agc[0] = rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10, 
                                                    rx_agc_read.rx_agc[0]);
      rx_agc_read.rx_agc[1] = rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10, 
                                                    rx_agc_read.rx_agc[1]);
      rx_agc_read.rx_agc[2] = rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10, 
                                                    rx_agc_read.rx_agc[2]);
      rx_agc_read.rx_agc[3] = rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10, 
                                                    rx_agc_read.rx_agc[3]);
      /* Report final Rx AGC and LNA state */
    RF_MSG_7( RF_MED, "rf_hdr_mc_helper_for_retune : Dev %d, "
                "RxAGC0 %d (dBm10), RxAGC1 %d (dBm10), "
              "RxAGC2 %d (dBm10), RxAGCAll %d (dBm10), LNA %d, SSMA_time %d",
                device, rx_agc_read.rx_agc[0], rx_agc_read.rx_agc[1], 
                rx_agc_read.rx_agc[2], rx_agc_read.rx_agc[3], 
              rx_agc_read.lna_state, prof_ssma_t_val );
    }
    else
    {
      RF_MSG( RF_ERROR,"rf_hdr_mc_helper_for_retune: "
                       "Retune failure, skipping RxAGC convergence" );
    }
  
  } /* if ( retune_success == TRUE ) */

  /* Unconditionally clean-up the Rx AGC LNA SM events here */
  retune_success &= rf_cdma_mc_cleanup_rx_agc_lna_ccs_events( device );

  /* continue with retune sequence only if command to FW was successful */
  if ( retune_success == TRUE )
  {
    int32 dummy_result;

    /* Tell GPS about the band and channel we are on. */
    RFGNSS_MC(evdo_band_chan, dummy_result)(band,
                                            chan_tune);

    if (dummy_result == FALSE  )
    {
      RF_MSG( RF_ERROR,"rfgnss_mc_evdo_band_chan API failed");
    }

    /* retune() succeeded at this point - safe to update SSMA recos for
       paired device now, and also safe to update device status */
    (void) rf_cdma_update_bb_ssma_recos( logical_dev->paired_device ); 

    /* update the RF SSMA recos if applicable */
    rf_cdma_update_rf_ssma_recos (logical_dev->paired_device);

    /* Safe to now update the channel to which LO is tuned to */
    rf_cdma_data_set_lo_tuned_to_chan( device , chan_tune );

    /* update the rotator offsets to the device data structure */
    rf_cdma_data_update_rot_offsets( device, dyn_settings.freq_offset_hz );

    /* Start the IntelliCeiver if the Analog Reciever supports/requires it */
    if ( dev_status_w->intelliceiver.ic_supported == TRUE )
    {
      rf_cdma_ic_start( &(dev_status_w->intelliceiver), logical_dev,
                        band, dev_status_r->lo_tuned_to_chan );
    }

    rf_cdma_data_set_rxlm_handle( device , rxlm_handle );

    /* return the wait_time */
    ret_val = 0;

  } /* if ( retune_success == TRUE ) */
  else
  {
    /* Return BAD CONFIG error code if configure RxAGC failed*/
    if( rx_agc_success == FALSE )
    {
      ret_val = RFM_CDMA_ERROR_BAD_CONFIG;
    }
    else
    {  
    ret_val = RFM_CDMA_ERROR_FAILURE;
    }
    
    RF_MSG_2( RF_ERROR, "rf_hdr_mc_retune : [Status %d] - Dev %d ", 
              ret_val, device );
  }

  /* get the time spent in this API */
  retune_time_val = rf_time_get_elapsed( retune_time, RF_USEC );

  RF_MSG_5( RF_MED, "rf_hdr_mc_helper_for_retune: [Status %d] - Dev %d, Band %d, "
            "Chan %d, RetuneTime %d ", ret_val, device, band,
            chan_tune, retune_time_val );

  /* return final value */
  return ret_val;  
        
} /* rf_hdr_mc_helper_for_retune */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Re-tune the radio to a new band and/or channel of operation, and
  configure single- or multi-carrier operation.

  @details
  Changes the operating frequency and number of active carriers for the
  HDR radio.  This function may only be called in Sleep or Rx state.

  If this API is called while the radio is in the Sleep state, the
  RF only prepares its internal state for the new frequency.  The
  radio will not be physically tuned unti the reciever is woken up.

  If this API is called while the radio is in the Rx state, the receiver
  (and any associated diversity receivers) will be re-tuned to operate
  in the new band and/or channel.

  To support multi-carrier 1xEV-DO revB, this API takes an array of
  band and channel structures, which specify the number and location
  of the carriers that should be tuned.  This API supports single
  carrier opeation by passing an array of length one.  The maximum
  number of carriers supported depends on the modem hardware, but 
  three carriers is the current maximum limit.

  HDR L1 must specify the exact carrier indicies it expects each channel
  to be assigned to through an array of carrier or sample RAM indicies.

  @param device
  The radio path to re-tune.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.
 
  @param num_band_chan
  The length of the band_chan parameter array.

  @param band
  The band to tune to.
 
  @param chan_array
  An array of channels for tuning multiple carriers in 1xEV-DO revB 
  multi-carrier mode.

  @param carrier_index
  An array in integers that specify the carrier the corresponding channel
  should be placed on in the multi-carrier Rx front.  This carrier will
  deliver samples into the corresponding sample RAM for demodulation.

  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @return
  The anticipated time in microseconds remaining from the rfm_hdr_tune()
  function return until the operation is complete, or an error code.
  If the API call succeeds the return value will be non-negative; all
  error codes will be negative.
*/
rfm_wait_time_t
rf_hdr_mc_retune
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const uint32 num_band_chan,
  const rfm_cdma_band_class_type band,
  const rfm_cdma_chan_type * const chan_array,
  const int32 * const carrier_index,
  boolean is_irat_mode
)
{
  rfm_wait_time_t ret_val; /* final wait time from this API */
  const rf_cdma_data_status_type *dev_status_r_master;/* Dev Status Read Ptr*/
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  boolean notch_status;

  /* get the device status for master device */
  dev_status_r_master = rf_cdma_get_device_status( device );
  dev_status_w = rf_cdma_get_mutable_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r_master == NULL ) || ( dev_status_w == NULL ) ||
       ( logical_dev == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_retune_rx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }


  /* Call HDR Main Control to put device to sleep - use the RXLM handle in the
      dev_status_r_master, since, L1 may pass in a "rxlm_handle" that is 
      different from the one it used during wakeup_rx (this is a valid case).
      As far as RF is concerned, it needs to make sure that the device is put
      to sleep - hence use the RxLM handle that was saved during the prep_wakeup
      API
  */

  /* Tell IC SM it is primary device. 
       Make sure this is the 1st time IC SM is touched */
  (void) rf_cdma_set_ic_support ( &(dev_status_w->intelliceiver) , TRUE ) ;
  ret_val = rf_hdr_mc_helper_for_retune ( device, rxlm_handle, num_band_chan,
                                          band, chan_array, carrier_index, 
                                          FALSE /* is_slave*/,
                                          is_irat_mode );

  /* Apply notch filter recommendations from WTR on the master device */
  notch_status = 
  rf_cdma_mc_trigger_dynamic_notch_update(device,
                      dev_status_w->ssma_cb_data.ssma_reco,
                      dev_status_w->ssma_cb_data.num_notch_filters_used,
                      dev_status_w->ssma_cb_data.clear_notch); 

  /* Notch application failed bail out!*/
  if ( notch_status == FALSE )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_retune_rx: Notch update failed for device %d",
              device );
    ret_val =  RFM_CDMA_ERROR_FAILURE;
  }

  /* Retune the diversity chain if diversity is enabled */
  if (ret_val >= RFM_CDMA_HEALTHY_STATE )
  {
    /* check if diversity chain is to be retuned as well */
    if ( dev_status_r_master->assoc_dev != RFM_INVALID_DEVICE )
    {
      /* Dev Status Read Ptr for slave device */
      const rf_cdma_data_status_type *dev_status_r_slave; 

      /* holds the associated device */
      rfm_device_enum_type slave_dev = dev_status_r_master->assoc_dev;

      /* get device status for slave */
      dev_status_r_slave = rf_cdma_get_device_status(slave_dev);

      /* Perform NULL Pointer check */
      if ( dev_status_r_slave == NULL )
      {
         RF_MSG_1( RF_ERROR, "rf_hdr_mc_tune: NULL data for device %d",
                   device );
         ret_val = RFM_CDMA_ERROR_NULL_DATA;
      } /* if ( dev_status_r_slave == NULL ) */
      else
      { /* FALSE : Support not needed for secondary device */
        (void) rf_cdma_set_ic_support ( 
           &(rf_cdma_get_mutable_device_status(slave_dev)->intelliceiver) , FALSE ) ;
        ret_val = rf_hdr_mc_helper_for_retune ( slave_dev, 
                          dev_status_r_slave->rxlm_handle, num_band_chan,
                           band, chan_array, carrier_index, TRUE /* is_slave*/,
                           is_irat_mode);
  
         rf_cdma_data_sanity_check( slave_dev );
      } /* if ! ( dev_status_r_slave == NULL ) */

      if (ret_val < RFM_CDMA_HEALTHY_STATE)
      {
        RF_MSG_2( RF_ERROR, "rf_hdr_mc_retune: [Status %d] - Failed during"
                  "retune for diversity device=%d", ret_val, slave_dev);
      }

    } /* if ( dev_status_r->assoc_dev != RFM_MAX_DEVICES ) */

  } /* if (ret_val >= RFM_CDMA_HEALTHY_STATE ) */
  else
  {
    RF_MSG_2( RF_ERROR, "rf_hdr_mc_retune_rx: [Status %d] - Failed during"
              "retune for Primary device=%d", ret_val, device);
  }

  rf_cdma_data_sanity_check( device );

  /* return final value */
  return ret_val;  
        
} /* rf_hdr_mc_retune */ 


/*----------------------------------------------------------------------------*/
/*!
  @brief
  MC-layer API to associate a new receive device with a primary control device 
  for diversity operation.

  @details
  This API links the HDR state of slave_device to the state of master_device.
  slave_device will track the operating frequency and Sleep/Rx state of the
  master.  HDR L1 need not make any calls to directy control slave_device
  until diversity mode is terminated with a call to
  rf_hdr_mc_disable_diversity().

  If master_device is in Rx or RxTx mode, slave is woken up if necessary,
  and linked to the operating frequency of master.  If master device is
  in sleep mode, slave remains in sleep mode and will be woken up
  when device1 is woken up.

  Any tuning opeations on device1 will be mirrored on device2 while
  they are linked in diversity mode.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_HDR_ENABLE_DIV_COMPLETE event.  The function will attempt to give
  a best estimate of the time remaining in the return value.

  @param master_device
  The radio device that will be the master.  Device2 is linked as a
  slave to this device.
 
  @param master_rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware for device1, if necessary.

  @param slave_device
  The radio device that will be slaved to master_device.

  @param slave_rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware for slave_device.
 
  @return
  The anticipated time in microseconds remaining from this
  function return until the operation is complete.

*/
rfm_wait_time_t
rf_hdr_mc_enable_diversity
(
  const rfm_device_enum_type master_device,
  const lm_handle_type master_rxlm_handle,
  const rfm_device_enum_type slave_device,
  const lm_handle_type slave_rxlm_handle
)
{
  uint32 adc_clock_freq; /* var to store ADC Clock Frequency */
  /* current chan of master device */
  /* current carrier indexes of master device */
  boolean rx_agc_success = TRUE; /* Flag to track success of RxAGC*/
  rfm_wait_time_t ret_val; /* Return value */
  rfm_cdma_power_mode_type power_mode; /* var to store Power Mode */
  rfm_hdr_agc_acq_type agc_mode; /* var to hold AGC acq mode */
  rxf_get_iqmc_coef_type iqmc_data; /* IQMC Coefficient */  
  rf_cdma_msm_static_rxlm_cfg_type static_cfg; /* Static RXLM  Settings */
  rf_cdma_msm_dynamic_rxlm_cfg_type dyn_settings; /* Dynamic RXLM  Settings */
  const rfc_cdma_logical_device_params_type *logical_m_dev; /* Logical Device */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  uint8 carrier_idx = 0; /* Variable for carrier indexing */
  const rf_hdr_config_type* cdma_config_ptr; /* Pointer to NV data */
  /* AGC from master device to start div with */
  rf_hdr_mdsp_rx_agc_read_type master_rx_agc;

  /* slave device write pointer */ 
  rf_cdma_data_status_type *s_dev_status_w; 

  /* master device read pointer */ 
  const rf_cdma_data_status_type *m_dev_status_r; 
  boolean wakeup_success = TRUE; /* Flag to track the success of enable_div */ 

  rf_time_tick_type prof_vreg_t;      /* Vreg Time Profile */
  rf_time_tick_type prof_rxlm_t;      /* RXLM Time Profile */
  rf_time_tick_type prof_rxagc_cfg_t; /* RxAGC Config Time Profile */
  rf_time_tick_type enable_div_time;  /* Time spent in function */

  rf_time_type prof_vreg_t_val;      /* Vreg Time Profile */
  rf_time_type prof_rxlm_t_val;      /* RXLM Time Profile */
  rf_time_type prof_rxagc_cfg_t_val; /* RxAGC Config Time Profile */
  rf_time_type enable_div_time_val;  /* Time spent in function */

  boolean dev_voting_status; /* Need the flag to perform device voting */
  rfm_cdma_band_class_type band;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;

  /* Start to Time Measurement */
  enable_div_time = rf_time_get_tick();

  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( slave_device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_enable_diversity: NULL FWRSP Data" );
    return RFM_CDMA_ERROR_NULL_DATA;
  } /* if ( fwrsp_data == NULL ) */

  fwrsp_hk = rf_cdma_get_fwrsp_data( slave_device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_enable_diversity: NULL FWRSP Data" );
    return RFM_CDMA_ERROR_NULL_DATA;
  } /* if ( fwrsp_data == NULL ) */

  /* Get Writable Device Status */
  s_dev_status_w = rf_cdma_get_mutable_device_status( slave_device );

  /* Get Logical Device Param */
  logical_m_dev = rfc_cdma_get_logical_device_params( master_device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( slave_device );

  /* Null pointer check */
  if ( ( s_dev_status_w == NULL ) || 
       ( logical_m_dev == NULL ) || ( logical_dev == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_enable_diversity(): Invalid device pointer "
                      "for slave device %d", slave_device );
    return RFM_CDMA_ERROR_FAILURE;
  }

  if ( rfc_cdma_set_rx_mdsp_path(slave_device, RF_CDMA_MDSP_CHAIN_1) == FALSE )
  {
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;    
  }

  /* check for out of bound condition */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM)
  {
     RF_MSG_1( RF_ERROR, "rf_hdr_mc_enable_diversity: rx_mdsp_path is out "
                         "of bound %d", logical_dev->rx_mdsp_path  );
    return RFM_CDMA_ERROR_FAILURE;
  }

  m_dev_status_r = rf_cdma_get_device_status( master_device );

  /* Null pointer check */
  if ( m_dev_status_r == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_enable_diversity(): Invalid device pointer "
                      "for master device %d", master_device );
    return RFM_CDMA_ERROR_FAILURE;
  }

  /* Get NV for the requested device and band and check if it is NULL */
  cdma_config_ptr = rf_cdma_nv_get_hdr_data( 
                             logical_dev->rx_nv_path[m_dev_status_r->curr_band],
                             m_dev_status_r->curr_band );

  if ( cdma_config_ptr == NULL )
  {
    /* no NV for the requested device and band - bail out */
    RF_MSG_2( RF_ERROR, "rf_hdr_mc_enable_diversity: NULL NV for device %d on "
                        "band = %d", slave_device, m_dev_status_r->curr_band);
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  /* validate band */
  band = m_dev_status_r->curr_band;
  if ( band >= RFM_CDMA_MAX_BAND )
  {
    RF_MSG_1 ( RF_ERROR, "rf_hdr_mc_enable_diversity: Invalid band %d ", band );
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/

  /* -------------------------- Update Alt. Path ---------------------------*/
  wakeup_success &= rf_cdma_mc_update_alt_rx_path( RFM_1XEVDO_MODE,
                                                  slave_device, band ,
                                                  FALSE /* No div in IRAT */);


  /*-----------Update the Antennae switch----------- */
  #if defined(FEATURE_RF_ASDIV) || defined(FEATURE_RF_HAS_TPTUNER_SWITCH)
    rf_cdma_data_set_ant_position ( slave_device , 
      ((uint8) rfcommon_asdiv_get_current_position ( slave_device ) ) ) ;

    RF_MSG_1 ( RF_MED , "rf_hdr_mc_enable_diversity: Ant Switch position %d ", 
               rf_cdma_data_get_ant_position ( slave_device ) ) ;
  #else
    rf_cdma_data_set_ant_position ( slave_device , ( (uint8)(0) ) ) ;
  #endif
  /* ------------------------End of Update Alt. Path -----------------------*/

  /* init dynamic settings before any operations */
  rf_cdma_msm_init_dyn_settings(&dyn_settings);

  /* set rxlm handle */
  rf_cdma_data_set_rxlm_handle( slave_device , slave_rxlm_handle );

  /*--------------------------------------------------------------------------*/
  /* Get SSMA recos for slave device, this not only gets Rx recos, but also 
  updates the state machine for the slave device with the relevant info */ 
  wakeup_success &= rf_cdma_get_ssma_recos ( slave_device, 
                                             slave_rxlm_handle,
                                             band, 
                                             m_dev_status_r->lo_tuned_to_chan,
                                             logical_dev->paired_device );

  /* now read the spur-info table in NV to see if any spurs need to be
     supressed *** The notches will be suppressed when RXLM is configured     */
  wakeup_success &= rf_cdma_get_spurs_to_suppress( slave_device, 
                                  slave_rxlm_handle, band, 
                                  m_dev_status_r->lo_tuned_to_chan, 
                                  m_dev_status_r->num_carriers, &dyn_settings );

  /* Update the number of notches used on this device (for tracking use 
  for SSMA callback), notches are per wb chain, the notch use is tracked
  per wb chain, hence use carrier index zero */
  s_dev_status_w->ssma_cb_data.num_notch_filters_used = 
                    dyn_settings.notch_filt_settings[0].num_notch_filters_used;

  /*--------------------------------------------------------------------------*/
  /* Enable Chip Power for all devices in the receiver - Device, RxAGC, RFCard*/
  /*--------------------------------------------------------------------------*/
  prof_vreg_t = rf_time_get_tick();
  rfc_cdma_manage_vregs( slave_device, RFM_1XEVDO_MODE, band, RF_PATH_RX_STATE);

  /* Vote ON for all RF Devices which are needed with RFM Device (device) 
     This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */

  dev_voting_status = rfcommon_core_dev_power_vote(slave_device,
                                                   RFCMN_CORE_DEV_ON,
                                                   RFCMN_CORE_DEV_NORMAL_VOTE);
  /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    dev_voting_status &= 
                      rfdevice_cdma_rx_pwr_wakeup(logical_dev->rx_device[band]);

  }

  if( dev_voting_status == FALSE )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_enable_diversity(): Device: %d Pwr Up failed",
              slave_device);
  }

  wakeup_success &= dev_voting_status;

  prof_vreg_t_val = rf_time_get_elapsed( prof_vreg_t, RF_USEC );


  /* Tell IC SM it is secondary device. 
     Make sure this is the 1st time IC SM is touched 
     FALSE: Because IC support not needed for secondary device. 
  */
  (void) rf_cdma_set_ic_support ( 
     &(rf_cdma_get_mutable_device_status(slave_device)->intelliceiver) , FALSE ) ;
  /*--------------------------------------------------------------------------*/
  /* Determine Power Mode for desired band and channel */
  /*--------------------------------------------------------------------------*/
  power_mode = rf_cdma_ic_get_starting_power_mode(
                    &(s_dev_status_w->intelliceiver), band,
                    m_dev_status_r->lo_tuned_to_chan,m_dev_status_r->rx_bw_khz);

  /*--------------------------------------------------------------------------*/
  /* Determine ADC clock frequency */
  /*--------------------------------------------------------------------------*/
  adc_clock_freq = rf_cdma_msm_get_adc_samp_freq( slave_rxlm_handle, band,
                                             m_dev_status_r->lo_tuned_to_chan );

  /* Populate static settings */
  static_cfg.adc_clock_freq = adc_clock_freq;
  static_cfg.bw_khz = m_dev_status_r->rx_bw_khz;
  static_cfg.rxlm_adc = logical_dev->rxlm_adc;
  static_cfg.config_as_et = FALSE;

  /* Populate the IQMC Data for query */
  iqmc_data.lin = power_mode;
  iqmc_data.band = band;
  iqmc_data.chan = m_dev_status_r->lo_tuned_to_chan;
  iqmc_data.sec_chain_rf_state = RF_CDMA_STATE_RXDIV;
  iqmc_data.bw_khz = static_cfg.bw_khz;

  /* Get RSB coefficients from device driver */
  rfdevice_cdma_rx_cmd_dispatch( logical_dev->rx_device[band],
                                 RFDEVICE_GET_IQMC_COEFS,
                                 &iqmc_data );

  /* init band/chan and carrier info for slave device */
  rf_cdma_init_freq_info_for_device(slave_device);

  dyn_settings.apply_dynamic_cfg = TRUE;
  dyn_settings.iqmc_coeff_in = iqmc_data.iqmc_a_coeff;
  dyn_settings.iqmc_coeff_quad = iqmc_data.iqmc_b_coeff;
  dyn_settings.num_carriers = m_dev_status_r->num_carriers;
  dyn_settings.modem_chain =
                   logical_dev->rfc_dev_info.rfc_device_info[band]->modem_chain;

  rf_cdma_compute_carrier_info( band, m_dev_status_r->curr_chans,
                                m_dev_status_r->num_carriers, 
                                m_dev_status_r->carrier_index, 
                                &dyn_settings.carrier_mask,
                                dyn_settings.freq_offset_hz,
                                0 /* no need to add anything to calculated
                                     freq offset */ );

  /*--------------------------------------------------------------------------*/
  /* Configure RxLM */
  /*--------------------------------------------------------------------------*/
  prof_rxlm_t = rf_time_get_tick();
  wakeup_success &= rf_cdma_msm_configure_rxlm( 
                                rfdevice_id_get(logical_dev->rx_device[band]),
                                slave_rxlm_handle,
                                RFM_1XEVDO_MODE, 
                                band,
                                power_mode, static_cfg,
                                dyn_settings );
  prof_rxlm_t_val = rf_time_get_elapsed( prof_rxlm_t, RF_USEC );

  /*--------------------------------------------------------------------------*/
  /* Configure RxAGC */           
  /*--------------------------------------------------------------------------*/
  prof_rxagc_cfg_t = rf_time_get_tick();

  /* Get the current master device Rx AGC to start the diversity path
     from. */
  rf_hdr_mdsp_get_rx_agc_data( logical_m_dev->rx_mdsp_path, &master_rx_agc );

  /* Use diversity enable mode, which requires the initail Rx AGC.
     Cumulative AGC (index 3) is used in case of MC-DO. */

  agc_mode.params.div_acq.initial_agc = master_rx_agc.rx_agc[3];

  if ( m_dev_status_r->agc_mode.mode == RFM_HDR_AGC_MODE_NORMAL )
  {
    agc_mode.mode = RFM_HDR_AGC_MODE_ENABLE_DIV_NORMAL;
  }
  else
  {
    agc_mode.mode = RFM_HDR_AGC_MODE_ENABLE_DIV;
  }

  RF_MSG_2( RF_LOW,
            "rf_hdr_mc_enable_diversity: Starting div AGC at %d dBm10 (%d), "
            "2 half-slot duration",
            rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                                  agc_mode.params.div_acq.initial_agc),
            agc_mode.params.div_acq.initial_agc );

  /*Associate Prx + Drx device first due to WTR requirement */    
  rf_cdma_data_set_assoc_dev( master_device , slave_device );
  rf_cdma_data_set_assoc_dev( slave_device , master_device );

  /* Safe to hard-code 'is_irat_mode' flag to FALSE when enabling diversity
  because diversity is never going to be enabled in IRAT scenarios*/
  rx_agc_success &= rf_hdr_configure_rx_agc( slave_device, slave_rxlm_handle,
                                             m_dev_status_r->num_carriers,
                                             band,
                                             m_dev_status_r->curr_chans,
                                             m_dev_status_r->lo_tuned_to_chan,
                                             &agc_mode, power_mode,
                                             RF_CDMA_STATE_RXDIV,
                                             FALSE,/* is_irat_mode */
                                             m_dev_status_r->carrier_index );

  wakeup_success &= rx_agc_success;

  prof_rxagc_cfg_t_val = rf_time_get_elapsed( prof_rxagc_cfg_t, RF_USEC );

  /*--------------------------------------------------------------------------*/
  /* Compute Enable RTR diversity Script                                      */
  /*--------------------------------------------------------------------------*/
  rf_cdma_data_set_script_index( slave_device, 
                      rf_cdma_mdsp_get_device_buffer_index( RFM_1XEVDO_MODE,
                                                           slave_rxlm_handle) );

  /*------------------------- END OF STATIC INIT -----------------------------*/
  /* No Static de-initialization can be done after this point. Only Sleep 
     hardware sequence is executed                                            */
  /*--------------------------------------------------------------------------*/

    /* configure RF front-end to enable the RX chain and send FW command */
  if ( wakeup_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section(logical_dev->rx_device[band]);
  
    /* Safe to hard-code 'is_irat_mode' flag to FALSE when enabling diversity
    because diversity is never going to be enabled in IRAT scenarios*/
    wakeup_success &= rf_cdma_mc_configure_front_end_for_rx_wakeup ( 
                          slave_device, RFM_1XEVDO_MODE, 
                          TRUE /* is_slave_device */, 
                          slave_rxlm_handle, band, 
                          m_dev_status_r->lo_tuned_to_chan, power_mode, 
                          s_dev_status_w->script_index, 
                          m_dev_status_r->rx_bw_khz, m_dev_status_r->tx_bw_khz, 
                          logical_dev,
                          &s_dev_status_w->ant_tuner_script_token_data ,
                          FALSE, /*is_irat_mode, always false here */
                          RFDEVICE_MEAS_INVALID_SCRIPT
                          /*None IRAT mode always invalid here */,
                          FALSE/* immediate_write */); 
 
    if ( wakeup_success == TRUE )
    {
      /* Safe to hard-code 'is_irat_mode' flag to FALSE when enabling diversity
      because diversity is never going to be enabled in IRAT scenarios*/
      wakeup_success = rf_hdr_mc_send_start_rx_helper (slave_device , 
                                                         (const rf_cdma_data_status_type*) s_dev_status_w , 
                                                         slave_rxlm_handle , 
                                                         logical_dev,
                                                         FALSE /*is_irat_mode*/,
                                                         FALSE /*is_retune*/,
                                                         TRUE /* is Drx */ ); 
    } /* if ( wakeup_success == TRUE ) */
  
    /* Call helper API to cleanup static event and Rx on/off events */
    wakeup_success &= rf_hdr_mc_cleanup_rx_ccs_events( slave_device );

    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section(logical_dev->rx_device[band]);
  
  } /* if ( wakeup_success == TRUE ) */

  /* Unconditionally clean-up the Rx AGC LNA SM events here */
  wakeup_success &= rf_cdma_mc_cleanup_rx_agc_lna_ccs_events( slave_device );

  /* continue with wakeup sequence only if command to FW was successful */
  if ( wakeup_success == TRUE )
  {
    /* update the rotator offsets to the device data structure */
    rf_cdma_data_update_rot_offsets(master_device, dyn_settings.freq_offset_hz);

    /* Start the IntelliCeiver if the Analog Reciever supports/requires it*/
    if ( s_dev_status_w->intelliceiver.ic_supported == TRUE )
    {
      rf_cdma_ic_start( &(s_dev_status_w->intelliceiver), logical_dev,
                        band, m_dev_status_r->lo_tuned_to_chan );
    }

    /* init band/chan and carrier info with valid values for both devices */
    rf_cdma_data_set_curr_band( slave_device, m_dev_status_r->curr_band);
    for( carrier_idx = 0; carrier_idx < m_dev_status_r->num_carriers; 
         ++carrier_idx ) 
    {
      rf_cdma_data_set_curr_chan( slave_device, carrier_idx, 
                                  m_dev_status_r->curr_chans[carrier_idx] );
      rf_cdma_data_set_carrier_index( slave_device, carrier_idx, 
                               m_dev_status_r->carrier_index[carrier_idx] );

    }

    rf_cdma_data_set_rx_bw_khz( slave_device, m_dev_status_r->rx_bw_khz );
    rf_cdma_data_set_tx_bw_khz( slave_device, RF_CDMA_BW_INVALID ); 
    rf_cdma_data_set_num_carriers( slave_device, m_dev_status_r->num_carriers );

    /* safe to now update the channel to which LO is tuned to - NOTE that
       this should be the same as primary device.. since in diversity, the
       PRX LO is used
     */
    rf_cdma_data_set_lo_tuned_to_chan(slave_device, 
                                      m_dev_status_r->lo_tuned_to_chan );

    /* Populate the total wait time */
    ret_val = 0;

    RF_MSG_3( RF_MED, "rf_hdr_mc_enable_diversity: Time taken (us): Vreg %d, "
              "RxLM %d, RxAGC_cfg %d ", prof_vreg_t_val, 
              prof_rxlm_t_val, prof_rxagc_cfg_t_val );

  } /* if ( wakeup_success == TRUE ) */
  else
  {  
    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      dev_voting_status = 
                       rfdevice_cdma_rx_pwr_sleep(logical_dev->rx_device[band]);
    }
    else
    {
      dev_voting_status = TRUE;
    }
    /* Vote OFF for all RF Devices which are not needed with RFM Device (device) 
       This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */
    dev_voting_status &= rfcommon_core_dev_power_vote(slave_device,
                                                     RFCMN_CORE_DEV_OFF,
                                                     RFCMN_CORE_DEV_NO_VOTE);
    if( dev_voting_status == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_hdr_mc_enable_diversity(): Device: %d Pwr Down failed",
                slave_device);
    }

    rfc_cdma_manage_vregs( slave_device, RFM_1XEVDO_MODE, band, 
                           RF_PATH_SLEEP_STATE);
    /* Return BAD CONFIG error code if configure RxAGC failed*/
    if( rx_agc_success == FALSE )
    {
      ret_val = RFM_CDMA_ERROR_BAD_CONFIG;
    }
    else
    {
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }
  }

  /* get time spent in function */
  enable_div_time_val = rf_time_get_elapsed( enable_div_time, RF_USEC );
   #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&rx_hdr_wakeup_DRx_time_hist,(uint32) enable_div_time_val);
    if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
    {
      rf_hdr_clear_profiling_hist_packet();
    }
  #endif
  /* Report wakeuptimes */
  RF_MSG_4( RF_MED, "rf_hdr_mc_enable_diversity : execution times (microsecs) "
            "Vreg %d, RxLM %d, RxAGC_cfg %d,"
            "total_time %d", prof_vreg_t_val, prof_rxlm_t_val, 
            prof_rxagc_cfg_t_val, enable_div_time_val);

  RF_MSG_2(RF_MED, "rf_hdr_mc_enable_diversity: Ret_val=%d for device=%d", 
           ret_val, slave_device);

  rf_cdma_data_sanity_check( slave_device );  
  rf_cdma_data_sanity_check( master_device ); 

  /* return final result */
  return ret_val;

} /* rf_hdr_mc_enable_diversity */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  MC layer API to disable receive diversity and put the device to sleep

  @details
  This API unlinks the state of device2 from whatever device it was
  associated with in the rfm_hdr_enable_diversity() function.

  If slave_device is in RxDiv, it is put to sleep.

  @param slave_device
  The radio device that will be disconnected from the diversity radio.
  This device must have previously been associated with a master device
  in a call to rfm_hdr_enable_diversity().
 
  @param slave_rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware for device2.

  @return
  The anticipated time in microseconds remaining from the
  rfm_hdr_disable_diversity() function return until the operation is
  complete.
*/
rfm_wait_time_t
rf_hdr_mc_disable_diversity
(
  const rfm_device_enum_type slave_device,
  const lm_handle_type slave_rxlm_handle
)
{
  boolean device_voting_status; /* Need this flag to track device voting status */
  rfm_wait_time_t ret_val; /* Return value */
  boolean sleep_success; /* Flag to track success of diversity sleep */
  const rfc_cdma_logical_device_params_type *s_logical_dev; /* Logical Device */
  rfm_cdma_band_class_type band;
   
  /* device pointers */
  const rf_cdma_data_status_type *s_dev_status_r; /* Device Status Read Pointer*/
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */

  rf_time_tick_type prof_vreg_t;      /* Vreg Time Profile */
//  rf_timetick_type prof_rxagc_cfg_t; /* RxAGC Config Time Profile */
  rf_time_tick_type disable_div_time; /* total time spent in API */

  rf_time_type prof_vreg_t_val=0;      /* Vreg Time Profile */
  rf_time_type prof_rxagc_cfg_t_val=0; /* RxAGC Config Time Profile */
  rf_time_type disable_div_time_val; /* total time spent in API */

  /* get starting time of the API */
  disable_div_time =  rf_time_get_tick();

  /* Get read-only and writable device pointers */
  s_dev_status_r = rf_cdma_get_device_status( slave_device );
  dev_status_w = rf_cdma_get_mutable_device_status( slave_device );
  
  /* Get Logical Device Param */
  s_logical_dev = rfc_cdma_get_logical_device_params( slave_device );

  /* check for NULL pointers */
  if ( (s_dev_status_r == NULL) || (dev_status_w == NULL) || 
       (s_logical_dev == NULL) )
  {
     RF_MSG_1( RF_ERROR, "rf_hdr_mc_disable_diversity: NULL data "
                         "for slave device %d", slave_device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* stop intelliceiver state-machine */
  /*--------------------------------------------------------------------------*/
  if ( dev_status_w->intelliceiver.ic_supported )
  {
    rf_cdma_ic_stop( rf_cdma_get_mutable_intelliceiver( slave_device ), 
                     s_logical_dev );
  }
  
  sleep_success = TRUE; /* initialize to good value */

  /*--------------------------------------------------------------------------*/
  /* Compute Disable RTR diversity Script and send RX_STOP to FW              */
  /*--------------------------------------------------------------------------*/
  rf_cdma_data_set_script_index ( slave_device ,  
                        rf_cdma_mdsp_get_device_buffer_index( RFM_1XEVDO_MODE,
                                                             slave_rxlm_handle) ) ;

  /*------------------------- END OF STATIC INIT -----------------------------*/
  /* No Static de-initialization can be done after this point. Only Sleep 
     hardware sequence is executed                                            */
  /*--------------------------------------------------------------------------*/
  if ( sleep_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section(
       s_logical_dev->rx_device[dev_status_w->curr_band]);

    sleep_success &= rf_cdma_mc_configure_front_end_for_rx_sleep(
                     slave_device, RFM_1XEVDO_MODE,
                     dev_status_w->rxlm_handle, TRUE /* is_slave_dev */,
                     dev_status_w->curr_band,
                     dev_status_w->script_index, 
                     dev_status_w->rx_bw_khz, dev_status_w->tx_bw_khz, 
                     s_logical_dev,
                     &dev_status_w->ant_tuner_script_token_data,
                     FALSE /* immediate_write */ ,
                     FALSE /* is_irat_mode */);

    if ( sleep_success== TRUE )
    {
      /* When device is in SHUTDOWN_FOR_IRATE state, do not send STOP_RX
      as it has already been sent by IRAT APIs */
      if( dev_status_w->data_hdr.rf_state != RF_CDMA_STATE_SHUTDOWN_FOR_IRAT)
      {
        sleep_success =
        rf_hdr_mc_send_stop_rx_helper ( slave_device ,s_dev_status_r ,
                                   slave_rxlm_handle ,s_logical_dev , FALSE,
                                        TRUE /*is drx  */  ) ;
        if ( FALSE == sleep_success )
        {
          RF_MSG ( RF_ERROR , "rf_hdr_mc_disable_diversity(): " 
               "rf_hdr_mc_rx_stop_helper() returned failure" ) ;
        }
      
        band = dev_status_w->curr_band;
        if ( s_logical_dev->ant_tuner[band] != NULL )
        {
          rf_cdma_atuner_ack_script(
            s_logical_dev->ant_tuner[band],
            &dev_status_w->ant_tuner_script_token_data );
        }

      } /* if device in SHUTDOWN_FOR_IRAT state */

    } /* if ( sleep_success== TRUE ) */

    /* Always clean up static event smem and event handle */
    rf_cdma_mc_cleanup_ccs_event( slave_device, RF_CDMA_STATIC_EVENT );

    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section(
       s_logical_dev->rx_device[dev_status_w->curr_band]);

  } /* if ( sleep_success == TRUE ) */

  /* move forward only if command to FW was successful */
  if (sleep_success == TRUE)
  {
    rf_cdma_data_status_type *m_dev_status_w; /* Device Status Write Pointer */  

    /* Revert the Vreg State, since diversity is going to sleep */
    prof_vreg_t = rf_time_get_tick();

    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      device_voting_status = 
                     rfdevice_cdma_rx_pwr_sleep(s_logical_dev->rx_device[dev_status_w->curr_band]);
    }
    else
    {
      device_voting_status = TRUE;
    }
    /* Vote OFF for all RF Devices which are not needed with RFM Device (device) 
       This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */
    device_voting_status &= rfcommon_core_dev_power_vote(slave_device,
                                                        RFCMN_CORE_DEV_OFF,
                                                        RFCMN_CORE_DEV_NO_VOTE);
    if( device_voting_status == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_hdr_mc_disable_diversity(): Device: %d Pwr Down failed",
                slave_device);
      ret_val = RFM_CDMA_ERROR_FAILURE;
    }
   
    rfc_cdma_manage_vregs( slave_device, RFM_1XEVDO_MODE, s_dev_status_r->curr_band,
                           RF_PATH_SLEEP_STATE);

    prof_vreg_t_val = rf_time_get_elapsed( prof_vreg_t, RF_USEC );
    
    /* get a pointer to the master device */
    m_dev_status_w = rf_cdma_get_mutable_device_status(dev_status_w->assoc_dev);
    
    if(  m_dev_status_w == NULL )
    {
      RF_MSG_1( RF_ERROR, "rf_hdr_mc_disable_diversity: NULL data "
                          "for device %d", dev_status_w->assoc_dev );
      ret_val = RFM_CDMA_ERROR_NULL_DATA;
    }
    else
    {
      /* dissociate the devices */
      m_dev_status_w->assoc_dev = RFM_INVALID_DEVICE;
    }

    /*----------------------------------------------------------------------*/
    /* LAST step::: Invalidate the Device info for Slave device */    
    /* bring carrier info to initial-state: invalidate all carrier_index &    */
    /* channels                                                               */
    /*----------------------------------------------------------------------*/
    /* Update CDMA Data structure with new RF State and  band/Chan */
    rf_cdma_reset_sw_state ( slave_device ) ;

    /* Device successfully put to sleep - undo any SSMA operations performed
       on the paired device, since the device is asleep, need to use
       RF_CDMA_INVALID_BAND and RF_CDMA_INVALID_CHAN while calling the SSMA
       routines */
    (void) rf_cdma_get_ssma_recos ( slave_device, 
                                    slave_rxlm_handle, 
                                    RFM_CDMA_MAX_BAND,
                                    RF_CDMA_INVALID_CHAN,
                                    s_logical_dev->paired_device );

    ret_val = 0; /* success - no wait required */
    
  } /* if (sleep_success == TRUE) */
  else
  {
    /* throw error */
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }

  disable_div_time_val = rf_time_get_elapsed( disable_div_time, RF_USEC );

  /* Report wakeup times */
  RF_MSG_3( RF_MED, "rf_hdr_mc_disable_diversity: execution times (microsecs) "
            "Vreg %d, RxAGC_cfg %d, total_time %d", prof_vreg_t_val, 
            prof_rxagc_cfg_t_val, disable_div_time_val);

  RF_MSG_2( RF_MED, "rf_hdr_mc_disable_diversity: Ret_val=%d for "
                    "slave device=%d", ret_val, slave_device );

  rf_cdma_data_sanity_check( slave_device ); 
  

  /* Disable rx AGC logging */
  rf_hdr_mdsp_disable_rxagc_logging(s_logical_dev->rx_mdsp_path); 
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&rx_hdr_sleep_DRx_time_hist,(uint32) disable_div_time_val);
    if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
    {
      rf_hdr_clear_profiling_hist_packet();
    
    }
  #endif
  return ret_val;

} /* rf_hdr_mc_disable_diversity */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enable the transmitter.

  @details
  Moves the HDR radio from the Rx state to the RxTx state.  Powers on all
  circuits required to transmit a HDR signal, and insures the transmitter
  is tuned to the correct band and channel.

  The HDR transmitter is controlled by hardware and software signals;
  calling this API does not guarantee the RF chipset will be emitting
  RF power.  The modulator hardware must also be enabled and actively
  controlling the TX_ON and PA_ON hardware controls.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_HDR_TX_WAKEUP_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.

  @param device
  The transmit path to wakeup.
 
  @param txlm_handle
  The Tx Link Manager buffer that shall be used to configure the
  modem hardware.

  @return
  The anticipated time in microseconds remaining from the rfm_hdr_wakeup_tx()
  function return until the operation is complete.
*/
rfm_wait_time_t
rf_hdr_mc_wakeup_tx
(
  const rfm_device_enum_type device,
  const rfm_device_enum_type rx_device,
  const lm_handle_type txlm_handle
)
{
  rf_time_tick_type wakeup_tx_tick;        /* time spent in API */
  rf_time_type wakeup_tx_time;
  rf_time_type FW_Resp_time=0; /* TIme spent in function */
  rf_time_tick_type FW_Resp_tick; /* TIme spent in function */
  
  rfm_wait_time_t ret_val; /* Return value */
  boolean wakeup_success = TRUE; /* Flag to track the success of wakeup tx */
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rf_cdma_data_status_type *rx_dev_status_r; /* Device Status Read Pointer */
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */

  uint8 carrier_idx; /* Variable for carrier indexing */

  rf_cdma_mdsp_tx_agc_logical_read_type tx_agc_0_read; /* TxAGC0 Read */
  rf_cdma_mdsp_tx_agc_logical_read_type tx_agc_1_read; /* TxAGC1 Read */
  rf_cdma_mdsp_tx_agc_logical_read_type tx_agc_2_read; /* TxAGC2 Read */
  /* Represents which carriers to enable/disable */
  rfm_cdma_carrier_en_mask_type carrier_mask;
  /* Contains frequency offsets */
  int32 freq_offset_hz[RFM_CDMA_CARRIER_NUM];
  rfm_cdma_band_class_type band; /* The actual band for tune. */
  rfdevice_cdma_dac_cal_iref_type iref_data; /* For getting Iref for TxLM */
  rf_cdma_msm_dynamic_txlm_cfg_type dynamic_cfg;
  rf_cdma_msm_static_txlm_cfg_type static_cfg;
  rfdevice_therm_read_type therm_read;/*contains therm read*/
  const rf_hdr_config_type *rf_hdr_config_ptr; /* Pointer to HDR RF NV data */
  /* Cal data pointer, both static and multi lin */
  const rf_cdma_tx_static_type *tx_static_nv_cal_ptr; 
  uint32 modem_chain;
  xpt_cfg_type xpt_cfg;
  rf_cdma_mdsp_response_cb_param_type rsp_cb_param;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;
  uint32 dac_samling_rate;
  uint8 dac_bit_length;
  rfdevice_trx_cdma_tx* tx_device = NULL ;

  /* for fbrx */
  uint32 channels[RFM_CDMA_CARRIER_CUMULATIVE];
  rfcom_band_type_u fbrx_band;
  lm_handle_type fbrx_lm_handle = 0xFFFFFFFF; /* fbrx lm handle */
  uint8 fbrx_modem_chain = 3; /* WB3 is the default FBRX Chain */
  rfcommon_fbrx_error_type fbrx_status = RFCOMMON_FBRX_SUCCESS;

  /* For Tx Spectral inversion */
  rfdevice_cdma_tx_spectral_inversion_type tx_spectral_inversion_data = {RFM_CDMA_MAX_BAND, FALSE};  
  rfcommon_fbrx_tx_param_type fbrx_tx_params;
  boolean pin_ret = FALSE;

     wakeup_tx_tick= rf_time_get_tick();
  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_wakeup_tx: NULL FWRSP Data" );
    return RFM_CDMA_ERROR_NULL_DATA;
  } /* if ( fwrsp_data == NULL ) */

  /* Get Read-only and Writable Device Status */
  dev_status_r = rf_cdma_get_device_status( device );
  dev_status_w = rf_cdma_get_mutable_device_status( device );
  
  rx_dev_status_r = rf_cdma_get_device_status( rx_device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( dev_status_w == NULL ) || 
       ( logical_dev == NULL ) ||( rx_dev_status_r == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_hdr_mc_wakeup_tx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }
  
  /* Update Tx device STM using info from Prx device */
  rf_cdma_data_set_curr_band( device, rx_dev_status_r->curr_band );
  rf_cdma_data_set_lo_tuned_to_chan( device, 
                                     rx_dev_status_r->rf_rx_reco.rf.lo_chan);  
  rf_cdma_data_set_num_carriers( device, rx_dev_status_r->num_carriers );
  rf_cdma_data_set_rx_bw_khz(device, rx_dev_status_r->rx_bw_khz);
  rf_cdma_data_set_rf_mode(device, RFM_1XEVDO_MODE);      
  rf_cdma_mdsp_set_pa_addtl_offset(RFM_1XEVDO_MODE, RF_CDMA_PA_TXAGC_ADDTL_OFFSET);

  for( carrier_idx = 0; carrier_idx < dev_status_r->num_carriers; ++carrier_idx )
  {
     rf_cdma_data_set_curr_chan( device , carrier_idx , rx_dev_status_r->curr_chans[carrier_idx] );
     rf_cdma_data_set_carrier_index( device , carrier_idx , rx_dev_status_r->carrier_index[carrier_idx]);
  }
  /* Get current band */
  band = dev_status_r->curr_band;

  if (band >= RFM_CDMA_MAX_BAND)
  {
    RF_MSG_2 (RF_ERROR,"rf_hdr_mc_wakeup_tx: Invalid Sys band %d .. rf_band %d",
              dev_status_r->curr_band, band);
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }
 
  /* ensure that NV is valid for given device and band */
  rf_hdr_config_ptr = rf_cdma_nv_get_hdr_data( logical_dev->tx_nv_path[band],
                         band);

  if ( NULL == rf_hdr_config_ptr )
  {
    RF_MSG_1 (RF_ERROR, "rf_hdr_mc_wakeup_tx : Failed to retrieve valid Config" 
                        " data from NV for band %d", band); 
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }
                                        
  tx_static_nv_cal_ptr = rf_cdma_nv_get_tx_static( 
                           logical_dev->tx_nv_path[band], band );
  if ( tx_static_nv_cal_ptr == NULL )
  {
    RF_MSG_1 ( RF_ERROR, "rf_hdr_mc_wakeup_tx: "
                     "Failed to get valid static NV pointer "
                     "for device %d", device);
    return RFM_CDMA_ERROR_NULL_DATA;
  }
                                        
  if( ( rfm_get_calibration_state() != TRUE ) && /* If online */
      ( rf_cdma_xpt_is_enabled(device) == TRUE ) && /* xpt enabled */
      ( rf_hdr_config_ptr->cdma_common_ptr->tx_cal.tx_multi_lin_cal.nv_rev_type
          != TX_MULTI_LIN_REV_3 ) /* lin ver is not rev 3 */
    )
  {
    RF_MSG_2( RF_ERROR, "rf_hdr_mc_wakeup_tx : XPT is enabled. Current lin ver %d"
              " is not the required ver %d",
              (rf_hdr_config_ptr->cdma_common_ptr->tx_cal.tx_multi_lin_cal.nv_rev_type+1),
              (TX_MULTI_LIN_REV_3+1) );
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }
 
  /*check value of num_carriers*/
  if (dev_status_r->num_carriers > RFM_CDMA_CARRIER_CUMULATIVE) 
  {
    wakeup_success = FALSE;
    RF_MSG_3( RF_ERROR, "rf_hdr_mc_wakeup_tx: Number of carriers cannot "
                        "exceed %d for device :%d, num carriers is:%d",
                      RFM_CDMA_CARRIER_NUM, device, dev_status_r->num_carriers); 
  }
										                                       
  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/
  /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    /* Wakeup the WTR for tx mode */
    wakeup_success = rfdevice_cdma_tx_pwr_wakeup(logical_dev->tx_device[band]);
  }

  /* Allocate resources ( Tx buffers, queues, etc) needed for 1x Tx operation 
     and if we cannot obtain DM buffers bail TX because nothing can be 
     written to any of the RF peripherals */ 
  if ( FALSE == rf_cdma_mdsp_modem_tx_allocate_resources ( txlm_handle ) )
  {
    RF_MSG_1 ( RF_ERROR, "rf_hdr_mc_wakeup_tx(): "
               " rf_cdma_mdsp_modem_tx_allocate_resources() Failed to for device %d " , 
               device ) ;
    return RFM_CDMA_ERROR_FAILURE ;
  }

  tx_device = logical_dev->tx_device[band] ;

  /* Set Modem Chain value */
  modem_chain = rfc_cdma_get_tx_modem_chain(device, band);

  rf_cdma_data_set_txlm_handle( device, txlm_handle );

  /* Identify if EPT/ET is enabled for Tx operation */
  if ( rf_cdma_xpt_is_enabled(device) == TRUE )
  {
    dev_status_w->ept_online_enabled = TRUE;
  }
  else
  {
    dev_status_w->ept_online_enabled = FALSE;
  }

  if ( rf_cdma_data_get_fbrx_enable( RFM_1XEVDO_MODE ) == TRUE)
  {
    /* Allocate FBRx DM buffers for given FBRxLM handle */
    fbrx_status = rfcommon_fbrx_mc_enter_mode( &fbrx_lm_handle, RFM_1XEVDO_MODE, 0);
    if(fbrx_status == RFCOMMON_FBRX_ERROR)
    {
      RF_MSG(RF_ERROR, "rf_hdr_mc_wakeup_tx: FBRx DM buffer allocation failed");
      wakeup_success = FALSE;
    }
    else
    {
      /* Update fbrx_lm_handle in rf_cdma_data */
      rf_cdma_data_set_fbrx_lm_handle( device , fbrx_lm_handle );
      RF_MSG_1(RF_MED, "rf_hdr_mc_wakeup_tx: FBRx DM buffer allocated handle %d",
                        fbrx_lm_handle);
    }
  } 

  /*--------------------------------------------------------------------------*/
  /* Enable Chip Power for all devices in the receiver - Device, RxAGC, RFCard*/
  /*--------------------------------------------------------------------------*/
  rfc_cdma_manage_vregs( device, RFM_1XEVDO_MODE, band, RF_PATH_RXTX_STATE );
  
  /*--------------------------------------------------------------------------*/
  /* Read therm for temp comp before configure TxAGC */
  /*--------------------------------------------------------------------------*/
  rfdevice_cdma_tx_cmd_dispatch( tx_device ,
                                 RFDEVICE_GET_THERMISTER_VALUE,
                                 &therm_read );
    
  dev_status_w->temp_comp.last_temp_sensor_read = therm_read.temperature;
  dev_status_w->temp_comp.is_last_reading_valid = therm_read.valid_temp_read;
      
  RF_MSG_2( RF_LOW, "rf_hdr_mc_wakeup_tx: therm reading: %d, and %d",
            therm_read.temperature, therm_read.valid_temp_read );

  /* Update the temp comp algo for the first read */ 
  wakeup_success &= rf_cdma_temp_comp_update_algo( &dev_status_w->temp_comp,
                                                   logical_dev, band );			   

  /* Forece HKADCbusy to FALSE*/
  rf_cdma_data_set_isHKADCbusy( device, FALSE );

  /*--------------------------------------------------------------------------*/
  /* Configure TxLM */
  /*--------------------------------------------------------------------------*/

    /* Get Iref info from RF Device */
  iref_data.band = band;
  rfdevice_cdma_tx_get_dac_cal_iref ( tx_device , &iref_data ) ;

  dynamic_cfg.iref_val = iref_data.iref_val;
  dynamic_cfg.num_carriers = dev_status_r->num_carriers;
  dynamic_cfg.tx_modem_chain = 
                rfc_cdma_get_tx_modem_chain ( device, 
                                              dev_status_r->curr_band ) ; 
  dynamic_cfg.tx_spectral_inversion = FALSE;

  static_cfg.txlm_dac = logical_dev->txlm_dac;
  xpt_cfg = (xpt_cfg_type) rf_cdma_xpt_get_cfg_param(device, XPT_CFG);

  static_cfg.xpt = rf_cdma_msm_get_txlm_xpt_mode( TRUE, xpt_cfg );

  /* Tx Specral inversion query from device */
  tx_spectral_inversion_data.band = band;

  /* we are not having success check as other WTR driver might not support this
     cmd */
  rfdevice_cdma_tx_cmd_dispatch( tx_device,
                                 RFDEVICE_IS_TX_SPECTRAL_INVERSION_NEEDED, 
                                 &tx_spectral_inversion_data);

  dynamic_cfg.tx_spectral_inversion = tx_spectral_inversion_data.spectral_inversion;
  
  wakeup_success &= rf_cdma_msm_configure_txlm ( rfdevice_id_get(tx_device) , 
                                                txlm_handle, 
                                                RFM_1XEVDO_MODE, 
                                                device,
                                                dev_status_r->curr_band,
                                                static_cfg,
                                                dynamic_cfg);
  static_cfg.xpt = xpt_cfg;

  if ( wakeup_success == TRUE )
  {
    int16 scdo_iq_gain = 0 ;
    int16 mcdo_iq_gain = 0 ;
    int16 mcdo_v_scdo_gain = 0 ;

    /* Retrieve the default TxLM IQ gain */
    if ( FALSE == rf_cdma_msm_get_default_mcdo_scdo_gains ( 
          dynamic_cfg.tx_modem_chain , txlm_handle , &scdo_iq_gain , 
          &mcdo_iq_gain , &mcdo_v_scdo_gain ) )
    {
      mcdo_iq_gain = scdo_iq_gain = 
        (int16) rf_cdma_msm_get_default_txc_gain( dynamic_cfg.tx_modem_chain, 
                                                  txlm_handle ) ;
    }
    dev_status_w->cdma_iq_gain_val = scdo_iq_gain ;
    dev_status_w->mcdo_iq_gain_val = mcdo_iq_gain ;
    dev_status_w->mcdo_vs_scdo_iq_gain_dB10 = mcdo_v_scdo_gain ;
    RF_MSG_3 ( RF_MED , "rf_hdr_mc_wakeup_tx(): "
               "||IQ gains Linear |SCDO: %d|MCDO: %d|MCDO_vs_SCDO_dB10: %d||",
               dev_status_w->cdma_iq_gain_val , dev_status_w->mcdo_iq_gain_val ,
               dev_status_w->mcdo_vs_scdo_iq_gain_dB10 ) ;
  }

  dev_status_w->ept_iq_gain_val = 
    (int16) rf_cdma_xpt_get_cfg_param( device, EPT_CFG_CMN_IQ_GAIN_UNITY );
 
  /*--------------------- FBRx Settings ----------------------------------*/
  if (( wakeup_success == TRUE ) && ( rf_cdma_data_get_fbrx_enable( RFM_1XEVDO_MODE ) == TRUE ))
  {
    /* Allocate Tx DM buffers for the given FBRx LM handle */
    fbrx_lm_handle =  rf_cdma_data_get_fbrx_lm_handle(device);
    fbrx_status = rfcommon_fbrx_mc_init_tx( fbrx_lm_handle, 
                                            RFCOMMON_FBRX_1X , 
                                            txlm_handle,
                                            RFM_1XEVDO_MODE,
                                            0 );

    fbrx_status &= rfcommon_fbrx_mc_register_vswr_meas_call_back ( 
         rflm_cdma_post_process_fbrx_vswr_measurements , txlm_handle ) ;

    rf_hdr_mdsp_update_fbrx_flag(TRUE);

    /* Updates FBRx frequency-dependent values */
    for(carrier_idx=0; carrier_idx < dev_status_r->num_carriers; ++carrier_idx)
    {
      channels[carrier_idx] = (uint32)(dev_status_w->curr_chans[carrier_idx]);
    }
    fbrx_band.cdma_band = band; 
    fbrx_tx_params.apt_only_mode = FALSE;
    fbrx_modem_chain = rfc_cdma_get_fbrx_modem_chain(device, band);
    fbrx_status &= rfcommon_fbrx_mc_tune_to_chan( 
                         device,
                         txlm_handle, 
                         RFM_1XEVDO_MODE, 
                         fbrx_band,
                         channels,
                         (uint8)dev_status_r->num_carriers,
                         fbrx_lm_handle,
                         fbrx_modem_chain,
                         fbrx_tx_params,
                         RFCOMMON_FBRX_NV_CONTAINER_C0 ); 
  }
  else
  {
    rf_hdr_mdsp_update_fbrx_flag(FALSE);
  }

  //Allocate static PDMEM for TxAGC
  rflm_c2k_allocate_txagc_ccs_mem(RFLM_TECH_HDR);

  /*--------------------------------------------------------------------------*/
  /* Configure TxAGC */
  /*--------------------------------------------------------------------------*/
  wakeup_success &= rf_hdr_configure_tx_agc(device, dev_status_r->num_carriers,
                                            band, 
                                            dev_status_r->lo_tuned_to_chan,
                                            txlm_handle,
                                            rf_hdr_config_ptr);

  /*------------------------------------------------------------------------*/
  /* compute the carrier mask, freq offsets to be programmed to FW          */
  /*------------------------------------------------------------------------*/
  rf_cdma_compute_carrier_info( 
                                dev_status_w->curr_band,
                                dev_status_w->curr_chans,
                                dev_status_w->num_carriers, 
                                dev_status_w->carrier_index, 
                                &carrier_mask,
                                freq_offset_hz, 
                                0 /* no need to add anything to calculated
                                     freq offset */
                              );
  /* Query DAC rate and bit length from txlm */
  wakeup_success &= rf_cdma_msm_get_dac_sampling_rate (txlm_handle,
                                                      &dac_samling_rate);

  dac_bit_length = rf_cdma_msm_get_dac_bit_length();

  /* configure the RF Front-end to enable Tx and send command to FW */
  if ( wakeup_success == TRUE )
  {
    /* Configure Tx frequency offsets */
  rf_hdr_mdsp_configure_tx_freq_offsets( logical_dev->tx_mdsp_path,
                                         dev_status_r->num_carriers, 
                                         carrier_mask, freq_offset_hz,
                                         dac_samling_rate, dac_bit_length,
                                         NULL, NULL, NULL );

  /*--------------------------------------------------------------------------*/
  /* Configure RF Front-end for enabling TX and send FW command - RX_START    */
  /*--------------------------------------------------------------------------*/
    rf_cdma_data_set_script_index ( device ,  
                                  rf_cdma_mdsp_get_device_buffer_index( 
                                                                RFM_1XEVDO_MODE,
                                                                txlm_handle ) ) ;

  /* --------------------------- END OF STATIC INIT ------------------------*/
  /* No Static de-initialization can be done after this point. Only wakeup 
     hardware sequence is executed */
  /*--------------------------------------------------------------------------*/

    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_tx_enter_critical_section ( tx_device ) ;

    wakeup_success &= rf_cdma_mc_configure_front_end_for_tx_wakeup(                
                            device, txlm_handle, RFM_1XEVDO_MODE, band, 
                            dev_status_r->lo_tuned_to_chan, 
                            dev_status_r->script_index, dev_status_r->rx_bw_khz, 
                            dev_status_r->rx_bw_khz, 
          /* TX BW same as Rx BW and will be updated when wakeup_tx succeeds */
                            logical_dev, tx_static_nv_cal_ptr,
                            &dev_status_w->ant_tuner_script_token_data,
                            FALSE/* immediate_write */ );

    /*--------------------- FBRx Settings ----------------------------------*/
    if ( ( wakeup_success == TRUE ) && (rf_cdma_data_get_fbrx_enable( RFM_1XEVDO_MODE ) == TRUE ) )
    {
      fbrx_lm_handle =  rf_cdma_data_get_fbrx_lm_handle(device);
  
      /* Set the pout comp flag that FBRx is enabled. */
      rf_cdma_set_pout_comp_skip ( device , TRUE ) ;
  
      /* Writes the FBRx modem settings to HW */
      fbrx_modem_chain = rfc_cdma_get_fbrx_modem_chain(device, band);
      fbrx_status &= rfcommon_fbrx_enable( fbrx_lm_handle, fbrx_modem_chain );
      if( fbrx_status == RFCOMMON_FBRX_ERROR )
      {
        RF_MSG_1(RF_FATAL, "rf_hdr_mc_wakeup_tx: "
                         "FBRx enable failed: handle %d", fbrx_lm_handle);
      }
    }  

    if ( wakeup_success == TRUE )
    {
      rf_cdma_mdsp_tx_start_type tx_start_c; /* TxAGC Start Config */
      /* Update Tx start Message */
      tx_start_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_WAKEUP_TX_ID;
  
      tx_start_c.txlm_handle = txlm_handle; /* TXLM Handle */
	  tx_start_c.fbrx_lm_handle = fbrx_lm_handle; /* FBRX LM Handle */
       FW_Resp_tick = rf_time_get_tick();
    
      /* Send Command to MDSP to start Tx */
      fwrsp_hk->tx_start_rsp.device = device;
      fwrsp_hk->tx_start_rsp.sema_token = rfcommon_semaphore_pop_item();
      rsp_cb_param.rsp_handler = rf_hdr_tx_start_rsp_cb;
      rsp_cb_param.handler_data = &fwrsp_hk->tx_start_rsp;

      wakeup_success = rf_hdr_mdsp_start_tx( logical_dev->tx_mdsp_path, 
                                             &tx_start_c, &rsp_cb_param, 
                                             NULL, NULL );

      /* Sending of Tx Start to FW failed */
      if ( wakeup_success == FALSE )
      {
        RF_MSG ( RF_ERROR, "rf_hdr_mc_wakeup_tx(): "
                           "rf_hdr_mdsp_start_tx failed "); 
        /* Message sending failed, free the semaphore */
        RF_MSG_2 ( RF_LOW, "rf_hdr_mc_wakeup_tx():"
                   " Freeing the semaphore and not waiting "   
                   " [D%d] TX_START pushed back : Sem @ 0x%x" , 
                   device , fwrsp_hk->tx_start_rsp.sema_token ) ;
      }
      else
      {
        /* Message was sent to FW, wait for semaphore */
        RF_MSG_2( RF_LOW, "[HDR][RF-FW Sync] [D%d] TX_START : Start Wait Sem @ "
                          "0x%x", device, fwrsp_hk->tx_start_rsp.sema_token );
        rfcommon_semaphore_wait( fwrsp_hk->tx_start_rsp.sema_token );
      }

      FW_Resp_time = rf_time_get_elapsed( FW_Resp_tick, RF_USEC );
      /* Push back semaphore after wait if mssg was sent successfully,
      or immediately if mssg sending failed*/
      rfcommon_semaphore_push_item( fwrsp_hk->tx_start_rsp.sema_token );

      if ( logical_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script(
          logical_dev->ant_tuner[band],
          &dev_status_w->ant_tuner_script_token_data );
      }

      xpt_cfg = rf_cdma_xpt_get_operation_mode(device);
      if ( ( xpt_cfg == EPT_CFG ) ||
           ( xpt_cfg == ET_CFG )  ||
           ( xpt_cfg == EPT_ET_CFG ) )
      {
        /* Bring the Tx device into ET mode */
        wakeup_success &= rf_hdr_xpt_configure( 
                                              device,
                                              band,
                                              dev_status_w->lo_tuned_to_chan,
                                              logical_dev->tx_nv_path[band],
                                              txlm_handle,
                                              xpt_cfg );
      }
    } /* if ( wakeup_success == TRUE ) */

    /* Always clean up static event smem and event handle */
    rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );

    /* Leave Device Critical Section */
    rfdevice_cdma_tx_leave_critical_section ( tx_device ) ;


  } /* if ( wakeup_success == TRUE ) */

  /* For successful execution, continure with the Tx Wakeup sequence */
  if ( wakeup_success == TRUE )
  {
    uint32 sv_band_chan_array[MAX_NUM_CHANNELS_SUPPORTED];

    int32 dummy_result;

    if ( rfcommon_autopin_is_enabled(RFM_1XEVDO_MODE) && 
	 rf_cdma_autopin_par_support(device, 
	                        logical_dev, 
	                    RFM_1XEVDO_MODE, 
	                              band))
    {
      /* Set the pout comp flag */	  
      rf_cdma_set_pout_comp_skip ( device , TRUE ) ;
    }

    /* Tell GPS that HDR Tx is on. */
    RFGNSS_MC(wwan_tx_status, dummy_result)(device, TRUE);

    if (dummy_result == FALSE  )
    {
      /* Report error */
      RF_MSG( RF_ERROR,"rfgnss_mc_evdo_band_chan API failed");
    }

    /*------------------------------------------------------------------------*/
    /* Start HDR Power limiting and HDET */
    /*------------------------------------------------------------------------*/
    if ( rf_hdr_plim_start(device,
                           logical_dev,
                           &dev_status_w->plim,
                           band) == TRUE )
    {
      /* Start HDET only if Power Limiting was started successfully. Else no 
      point doing HDET reads */
      if ( rf_cdma_hdet_start(device, &dev_status_w->hdet,
                              RFM_1XEVDO_MODE,
                              logical_dev,
                              band,
                              dev_status_r->lo_tuned_to_chan) == FALSE )
      {
        /* Clean up HDET trig event in case of hdet start failure */
        rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_EVENT_HDET_CONFIG );

        RF_MSG_3( RF_ERROR, "rf_hdr_mc_wakeup_tx: HDET start failed on device %d, "
                            "band: %d, chan: %d", 
                            device, band, dev_status_r->lo_tuned_to_chan );
      }
      else
      {
	  /* Update the initial HDET vs Temp params based on this initial therm read */
      rf_cdma_hdet_update_temperature_params( 
                            &dev_status_w->hdet,
                            dev_status_w->temp_comp.last_temp_sensor_read );
      }
	  
      /* Update lim_vs_temp_vs_freq */
      rf_hdr_plim_update_lim_vs_temp_vs_freq( 
                            device,
                            &dev_status_w->plim,
                            dev_status_w->plim.band,
                            dev_status_w->lo_tuned_to_chan,
                            dev_status_w->temp_comp.last_temp_sensor_read );
    }
    else
    {
      RF_MSG_2( RF_ERROR, "rf_hdr_mc_wakeup_tx: HDR PLIM start failed on device %d,"
                          "band: %d", device, band );
    }

    /*--------------------------------------------------------------------------*/
    /* Start HDR temp comp routine */
    /*--------------------------------------------------------------------------*/
    if ( rf_hdr_temp_comp_start( &dev_status_w->temp_comp,
                                 RFM_1XEVDO_MODE,
                                 logical_dev,
                                 band,
                                 dev_status_r->lo_tuned_to_chan ) == FALSE )
    {
      RF_MSG( RF_ERROR, "rf_hdr_mc_wakeup_tx: temp comp start failed ");
    }
    else
    {
      RF_MSG_1( RF_MED, "rf_hdr_mc_wakeup_tx: temp comp starts at device %d", 
                device); 
    }
	
    /*------------------------------------------------------------------------*/
    /* Update CDMA Data structure with new the RF State */
    if ( rf_cdma_data_get_hdr_rf_state(device) == RF_CDMA_STATE_RX )
    {
    rf_cdma_data_set_hdr_rf_state( device, RF_CDMA_STATE_RXTX );
    }
    else
    {
      rf_cdma_data_set_hdr_rf_state(device, RF_CDMA_STATE_TX); 
    }

    rf_cdma_data_set_tx_bw_khz( device, dev_status_w->rx_bw_khz );

    /* Build script for 1x Autopin  */
    /*------------------------------------------------------------------------*/
	if (rfcommon_autopin_is_enabled(RFM_1XEVDO_MODE))
	{
	  if(RFCOMMON_AUTOPIN_ERROR == 
		  rfcommon_autopin_mc_enable_tx(txlm_handle, RFM_1XEVDO_MODE, NULL))
	  {
		RF_MSG(RF_ERROR, "rf_hdr_mc_wakeup_tx: CDMA HDR AutoPin enable TX failed!");
		fbrx_status &= FALSE;
	  }
	  else
	  {
		rf_cdma_auto_pin_tx_wakeup(device, RFM_1XEVDO_MODE);
	
				  //Update the DM memory
		(void)rf_cdma_autopin_update_dm (device,
					  logical_dev,	  
				  RFM_1XEVDO_MODE,
					  txlm_handle,
							  band, 
				  dev_status_r->curr_chans[0]/*for carrier 0 TBD for MC*/);
	  }
	}
	else
	{
	  RF_MSG(RF_ERROR, "rf_1x_mc_wakeup_tx: CDMA 1X AutoPin is not enaBLED!");
	}

     if ( rfcommon_autopin_is_enabled(RFM_1XEVDO_MODE) &&
	 	  rf_cdma_autopin_par_support(device, 
	 	                          logical_dev, 
	 	                          RFM_1XEVDO_MODE, 
	 	                          band) == TRUE)
     {
         pin_ret = rf_cdma_configure_autopin_trigger (
   				    device,
  				    RFM_1XEVDO_MODE,
				    logical_dev,
			    	    band);
		  
	  if (pin_ret == FALSE)
    	  {
            RF_MSG( RF_ERROR, "rf_hdr_mc_wakeup_tx: pin configure failed ");
    	  }
    }

    /* Notify SV module of Tx stats */
    for(carrier_idx=0; carrier_idx < dev_status_r->num_carriers; ++carrier_idx)
    {
      sv_band_chan_array[carrier_idx] = dev_status_w->curr_chans[carrier_idx];
    }
    rfsv_send_tx_update_notification((boolean)TRUE,
                                     RFM_1XEVDO_MODE,
                                     rf_cdma_convert_rfm_band_to_card_band(
                                     dev_status_w->curr_band),
                                     dev_status_w->num_carriers,
                                     sv_band_chan_array,
                                     FALSE);

    /* Populate the total wait time */
    ret_val = 0;

    /* Get TxAGC Data for reporting */
    rf_hdr_mdsp_get_tx_agc_logical_data( logical_dev->tx_mdsp_path, txlm_handle ,
                                         0 , &tx_agc_0_read ) ;
    rf_hdr_mdsp_get_tx_agc_logical_data( logical_dev->tx_mdsp_path, txlm_handle ,
                                         1 , &tx_agc_1_read ) ;
    rf_hdr_mdsp_get_tx_agc_logical_data( logical_dev->tx_mdsp_path, txlm_handle , 
                                         2 , &tx_agc_2_read ) ;
    
    /* Store the associated master device for Tx */
    rf_cdma_data_set_assoc_master_dev(device, rx_device);    
            
    /* Report wakeup time, Tx AGC and PA state */
    RF_MSG_8( RF_MED, "rf_hdr_mc_wakeup_tx : Dev %d, Band %d, Chan %d, "
              "PA State %d, TxTotalC0 %d, TxTotalC1 %d, TxTotalC2 %d,"
              "TxTotal across carriers %d", 
              device, band, dev_status_r->lo_tuned_to_chan, 
              tx_agc_0_read.pa_state, 
              tx_agc_0_read.total_carrier_power,
              tx_agc_1_read.total_carrier_power, 
              tx_agc_2_read.total_carrier_power,
              tx_agc_0_read.total_power );

  } /* ( tx_wakeup_success == TRUE ) */
  else
  {
    /* Revert the Vreg State, since Rx is not waking up */
    rfc_cdma_manage_vregs( device, RFM_1XEVDO_MODE, band,
                           RF_PATH_RX_STATE);
    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      /* Sleep the WTR for tx mode , since wakeup tx failed */
      wakeup_success = rfdevice_cdma_tx_pwr_sleep(logical_dev->tx_device[band]);
    }

     /* set txlm in rf_cdma_data */
    rf_cdma_data_set_txlm_handle( device, RFM_INVALID_LM_BUFFER);
 
   if ( rf_cdma_data_get_fbrx_enable( RFM_1XEVDO_MODE ) == TRUE ) 
   {
    /* Deallocate FBRx DM buffers for given FBRxLM handle  */
    fbrx_status = rfcommon_fbrx_mc_exit( dev_status_r->fbrx_lm_handle,
                                         RFM_1XEVDO_MODE );

      /* Reset the pout comp flag that FBRx is disabled. */
      rf_cdma_set_pout_comp_skip ( device , FALSE ) ;
    RF_MSG_1( RF_MED, "rf_hdr_mc_wakeup_tx: Failed wakeup: "
                      "Calling rfcommon_fbrx_mc_exit with "
              "fbrx_handle %d, RFM_1XEVDO_MODE", dev_status_r->fbrx_lm_handle );

    if(fbrx_status == RFCOMMON_FBRX_ERROR)
    {
      wakeup_success = FALSE;
      RF_MSG(RF_ERROR, "rf_hdr_mc_wakeup_tx: FBRx DM buffer deallocation failed");
    }
    else
    {
      /* Update fbrx_lm_handle in rf_cdma_data */
      rf_cdma_data_set_fbrx_lm_handle( device , RFLM_DM_INVALID_HANDLE_ID );
    }
   } 
   /* Reset the pout comp flag */
   rf_cdma_set_pout_comp_skip ( device , FALSE ) ;
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }
  rf_cdma_data_sanity_check( device );  
  wakeup_tx_time = rf_time_get_elapsed( wakeup_tx_tick, RF_USEC ); 
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&tx_hdr_wakeup_Tx_time_hist,(uint32) wakeup_tx_time);
    prof_hist_update(&tx_hdr_wakeup_fw_resp_hist,(uint32)FW_Resp_time );
    if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
    {
      rf_hdr_clear_profiling_hist_packet();
    
    }
  #endif
  return ret_val;

} /* rf_hdr_mc_wakeup_tx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Turn off the receiver.

  @details
  Moves the HDR radio from the Rx state to the Sleep state.  Any circuits
  that are no longer needed are powered down to conserve current.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_HDR_RX_SLEEP_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.

  @param device
  The receive path to put to sleep.
 
  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.
 
  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @param is_meas_enter
  Boolean flag to check if the API is being called during IRAT enter mode or non-IRAT scenarios
  In IRAT enter mode scenarios, we should skip taking the dev power vote for the device.
  It will be taken care by L1 as part of rfm_hdr_sleep_rx call after measurement.
 
  @return
  The anticipated time in microseconds remaining from the rf_hdr_mc_sleep_rx()
  function return until the operation is complete. Returns a 0 on success (no
  wait time needed) and a negative value on failure
 
  @note
  We have 2 flags that we use to have mc_sleep_rx behave in the following 4 ways:
  1> is_irat_mode [self explanatory name]
  2> RF_CDMA_STATE_SHUTDOWN_FOR_IRAT [We are in a partial shutdown. Some actions
     have been performed while some are pending and that we can only perform
     limited operations while in this state]

  |===================|===============|==============|==============|==============|
  |===================|===============|==============|==============|==============|
  | Sleep-Action      |D2L [RF SLeep] |D2L [L1 Sleep]| L2D Sleep    | Normal Sleep |
  |===================|===============|==============|==============|==============|
  |===================|===============|==============|==============|==============|
  |Sleep Front End    |  Dont Care    |  YES         |  Dont Care   |   YES        |
  |                   |               |              |              |              |
  |===================|===============|==============|==============|==============|
  | Send Rx Stop      |  YES          |  NO          |NO L1 does it |   YES        |
  |                   |               |              |              |              |
  |===================|===============|==============|==============|==============|
  | Shutdown VREGS    |  NO           |  NO          |  NO          |   YES        |
  |                   |               |              |              |              |
  |===================|===============|==============|==============|==============|
  | Reset CDMA state  |  NO           |  YES         |  YES         |   YES        |
  |                   |               |              |              |              |
  |===================|===============|==============|==============|==============|
  | Call ModemExit()  |  NO           |  YES         | YES          |   YES        | 
  |                   |               |              |              |              |
  |===================|===============|==============|==============|==============|
  |===================|===============|==============|==============|==============|


  Below is the table for how the 2 variables translate to 4 sleep combinations.
  ||===================|=====================||===================||
  ||===================|=====================||===================||
  ||  is_irat_mode     |  SHUTDOWN_FOR_IRAT  ||  ACTION           ||
  ||                   |                     ||                   ||
  ||===================|=====================||===================||
  ||===================|=====================||===================||
  || FALSE             |  FALSE              ||  Normal SLEEP     ||
  ||                   |                     ||                   ||
  ||===================|=====================||===================||
  || FALSE             |  TRUE               || D2L-L1 sleep      ||
  ||                   |                     || after gap         ||
  ||===================|=====================||===================||
  || TRUE              |  FALSE              ||  D2L-RF sleep in  ||
  ||                   |                     || enter_meas API    ||
  ||===================|=====================||===================||
  || TRUE              |  TRUE               || L2D RF Sleep in   ||
  ||                   |                     || exit_meas         ||
  ||===================|=====================||===================||
  ||===================|=====================||===================||

*/
rfm_wait_time_t
rf_hdr_mc_sleep_rx
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  boolean is_irat_mode,
  boolean is_meas_enter
)
{

  rfm_wait_time_t ret_val = RFM_CDMA_HEALTHY_STATE ; /* Return value */
  boolean sleep_success; /* Flag to track the success of sleep rx */
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  rfm_cdma_band_class_type band;
  rf_time_tick_type prof_sleep_t;
  rf_time_type prof_sleep_t_val = 0; 

  /* Flag to indicate if FE programming is required by FW or not
  in IRAT cases it can be skipped */
  boolean skip_frontend_programming = FALSE;

  /* Need this flag to track device voting status */
  boolean device_voting_status = TRUE;

  prof_sleep_t = rf_time_get_tick();
  /* Get Read-only and Writable Device Status */
  dev_status_r = rf_cdma_get_device_status( device );
  dev_status_w = rf_cdma_get_mutable_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* check for NULL pointers */
  if ( (dev_status_r == NULL) || (dev_status_w == NULL) || 
       (logical_dev == NULL) || (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM ))
  {
     RF_MSG_1( RF_ERROR, "rf_hdr_mc_sleep_rx: NULL data for device %d",
               device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* check for valid band */
  band = dev_status_w->curr_band;

  if ( band >= RFM_CDMA_MAX_BAND )
  {
    RF_MSG_1 ( RF_ERROR, "rf_hdr_mc_sleep_rx: Invalid band %d ", band );
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }

  
  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Stop Intelliceiver */
  /*--------------------------------------------------------------------------*/

  /* Stop intelliceiver state-machine, check if ic is supported and if we are in
  IRAT mode or not, all IC operations are skipped in IRAT/Shutdown for IRAT ,
  except in the case of D2L enter meas, because we need to stop IC before 
  starting any IRAT operations */ 
  if ( ( dev_status_w->intelliceiver.ic_supported ) && 
       ( dev_status_w->data_hdr.rf_state != RF_CDMA_STATE_SHUTDOWN_FOR_IRAT ) )
  {
    rf_cdma_ic_stop( rf_cdma_get_mutable_intelliceiver( device ), 
                     logical_dev );
  }
  else
  {
    /* Print F3 */
    RF_MSG_3( RF_LOW, "rf_hdr_mc_sleep_rx: skipping IC Stop :"
                        "ic_supported: %d , is_irat_mode:%d , rf_state:%d", 
              dev_status_w->intelliceiver.ic_supported, 
              is_irat_mode,
              dev_status_w->data_hdr.rf_state  ); 
  }/*if ( ( dev_status_w->intellic.....*/


  sleep_success = TRUE; /* initialize to good value */

  /*--------------------------------------------------------------------------*/
  /* Create RTR Sleep Script and send RX_STOP to FW                           */
  /*--------------------------------------------------------------------------*/

  rf_cdma_data_set_script_index ( device , 
                                  rf_cdma_mdsp_get_device_buffer_index ( 
                                                                RFM_1XEVDO_MODE,
                                                                rxlm_handle ) ) ;

  /*------------------------- END OF STATIC INIT -----------------------------*/
  /* No Static de-initialization can be done after this point. Only Sleep 
     hardware sequence is executed                                            */
  /*--------------------------------------------------------------------------*/
  if ( sleep_success == TRUE )
  {

    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section(logical_dev->rx_device[band]);


    /* In IRAT refrain from sleeping the FE in case it is being shared by Source Tech */
    if ( is_irat_mode == FALSE )
    {     
      sleep_success &= rf_cdma_mc_configure_front_end_for_rx_sleep(
                               device, RFM_1XEVDO_MODE,
                               dev_status_w->rxlm_handle, 
                               FALSE /* is_slave_dev */,                             
                               band, dev_status_w->script_index, 
                               dev_status_w->rx_bw_khz, dev_status_w->tx_bw_khz, 
                               logical_dev,
                               &dev_status_w->ant_tuner_script_token_data ,
                               FALSE /* immediate_write */,
                               FALSE /* is_irat_mode */);

      /* we have programmed a new script in smem for Rx stop for FE
      FE programming needs to be done to execute this script */
      skip_frontend_programming = FALSE;
    }
    else
    {
      /* we are in IRAT mode, we have not updated the frontend scripts
      hence we need to skip FE programming in RX stop, otherwise we can 
      potentially cause FW to execute a wrong event  */
      skip_frontend_programming = TRUE; 
    }
          

    /* In D2L at during meas exit RF_CDMA_STATE_SHUTDOWN_FOR_IRAT will be set 
       in the Gap so L can start measurements
       In L2D when sleep is called RF_CDMA_STATE_SHUTDOWN_FOR_IRAT is set so
       we dont send Rx Stop but still reset the SW state.
    */
    if ( TRUE == sleep_success )  
    {
      if ( dev_status_r->data_hdr.rf_state != RF_CDMA_STATE_SHUTDOWN_FOR_IRAT ) 
      {
        sleep_success =
          rf_hdr_mc_send_stop_rx_helper ( device , dev_status_r ,
                                   rxlm_handle, logical_dev , 
                                   skip_frontend_programming,
                                   FALSE /* is Drx */ ) ;
        if ( FALSE == sleep_success )
        {
          RF_MSG ( RF_ERROR , "rf_hdr_mc_sleep_rx(): " 
                 "rf_hdr_mc_rx_stop_helper() returned failure" ) ;
        }

        if ( logical_dev->ant_tuner[band] != NULL )
        {
          rf_cdma_atuner_ack_script(
            logical_dev->ant_tuner[band],
            &dev_status_w->ant_tuner_script_token_data );
        }

      }
      else
      {
        RF_MSG ( RF_MED , "rf_hdr_mc_sleep_rx(): " 
                 "rf_hdr_mc_rx_stop_helper() Skipped " 
                 "RF_CDMA_STATE_SHUTDOWN_FOR_IRAT is TRUE " ) ;
      }
    }
    else
    {
      RF_MSG ( RF_ERROR , "rf_hdr_mc_sleep_rx(): " 
               "rf_hdr_mc_configure_rffe_for_rx_sleep() returned failure" ) ;
    }

    /* Always clean up static event smem and event handle */
    rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );
    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section(logical_dev->rx_device[band]);

  } /* if ( sleep_success == TRUE ) */

  /* move forward only if command to FW was successful */
  if (sleep_success == TRUE)
  {
    rf_time_tick_type prof_ssma_t; 
    rf_time_type prof_ssma_t_val;

    /* Dynamic RXLM  Settings */
    rf_cdma_msm_dynamic_rxlm_cfg_type dummy_curr_dev; 

    /* init dynamic settigs before any operations */
    rf_cdma_msm_init_dyn_settings(&dummy_curr_dev);


    /*------------------------------------------------------------------------*/
    /* SSMA operations below                                                  */
    /*------------------------------------------------------------------------*/
    /* device successfully put to sleep - undo any SSMA operations performed */

    (void) rf_cdma_get_ssma_recos ( device,  rxlm_handle,
                                    RFM_CDMA_MAX_BAND, RF_CDMA_INVALID_CHAN,
                                    logical_dev->paired_device );

    /* sleep() succeeded at this point - safe to update SSMA recos for
       paired device now */
    (void) rf_cdma_update_bb_ssma_recos( logical_dev->paired_device );

    /* update the RF SSMA recos if applicable */
    rf_cdma_update_rf_ssma_recos (logical_dev->paired_device);

	prof_ssma_t_val = rf_time_get_elapsed( prof_ssma_t, RF_USEC );

    RF_MSG_1( RF_MED, "rf_hdr_mc_sleep_rx: SSMA time %d", prof_ssma_t_val);
    
    /*------------------------------------------------------------------------*/
    if ( (is_meas_enter != TRUE) && (dev_status_r->data_hdr.rf_state !=  RF_CDMA_STATE_RX_PREP) )
    {
      /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
      if (rfcommon_nv_get_lpm_feature_enabled())
      {
        device_voting_status = 
                     rfdevice_cdma_rx_pwr_sleep(logical_dev->rx_device[band]);
      }
      else
      {
        device_voting_status = TRUE;
      }

      if( device_voting_status == FALSE )
      {
        RF_MSG_1( RF_ERROR, "rf_hdr_mc_sleep_rx(): Device: %d Pwr Down failed",
                  device);
        ret_val = RFM_CDMA_ERROR_FAILURE;
      }
      else
      {
        /* no wait_time required (set to 0) */
        ret_val = RFM_CDMA_HEALTHY_STATE;
      }

      sleep_success &= device_voting_status;
    }
    else
    {
      RF_MSG_2( RF_MED, "rf_hdr_mc_sleep_rx(): Device: %d is in prep_wakeup state :%d"
                "or it is IRAT enter mode" ,device,dev_status_r->data_hdr.rf_state);
    }
    /* If in IRAT dont touch the VREGs and power votes when CDMA is either the SRC
       or the Target tech.
       1> When CDMA is the source tech Vregs need to be ON for the Target tech like 
       LTE because turning off the Vregs will cause LTE to not find any energy.
       2> When CDMA is the target tech then the SRC tech handles all the VREG management.   
    */
    if ( is_irat_mode == FALSE )
    {
      /* Vote OFF for all RF Devices which are not needed with RFM Device (device) 
         This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS 
         settings */
      device_voting_status &= rfcommon_core_dev_power_vote(device,
							   RFCMN_CORE_DEV_OFF,
							   RFCMN_CORE_DEV_NO_VOTE);
      /* Abort pending AsDiv Event */
      rf_cdma_mdsp_abort_asd_events( rxlm_handle );

      /*! deallocate asd memory*/
      rflm_cmn_asd_deallocate_memory((uint32)rxlm_handle);

      rfc_cdma_manage_vregs( device, RFM_1XEVDO_MODE, band, RF_PATH_SLEEP_STATE);

      /* un-vote for RFCMD app */
      {
        rf_hal_bus_client_handle* handle = rf_cdma_data_get_rf_hal_bus_vote_handle(device);
        #ifndef FEATURE_RFA_ATLAS_MODEM
        rf_hal_bus_enable (FALSE, &handle, RF_HAL_RFCMD_APP_CLIENT(EVDO));
        #endif
        rf_cdma_data_set_rf_hal_bus_vote_handle(device, handle);
      }
    }
   
    /* ***** Warning ****** Warning ******* Warning ******* Warning ****** Warning ****** Warning ******* 
    **  No writes to anything on SSBI, GRFC  RFFE bus or any peripheral after 
    **  here. TQ will be relinquished */

  /* In D2L in meas_exit , dont destroy the band channel because for CDMA to L measurements,
      the next command after the gap the L1 sends is a sleep command which is when 
      we will reset the SW data structures. */
  if ( ( is_irat_mode == TRUE ) &&
         ( dev_status_r->data_hdr.rf_state != RF_CDMA_STATE_SHUTDOWN_FOR_IRAT ) )
  {
    RF_MSG ( RF_MED , "rf_hdr_mc_sleep_rx(): "
             "In CDMA-2-L meas enter. Skip update of device structures" ) ;
  }
  else
  {
    /* We need to be in RX state to deallocate resources, cannot deallcoate
    if we are in prep since we will end up deallocating resources that 
    were never allocated to HDR in the first place */
    if ( dev_status_r->data_hdr.rf_state == RF_CDMA_STATE_RX_PREP )
    {
      RF_MSG ( RF_MED , "rf_hdr_mc_sleep_rx(): "
           "Skipping RFLM resource deallocation since RF state is 'Prep'" ) ;
    }
    else
    {
	  rfm_device_enum_type tq_device;

	  /* Use device stored seperately to deallocate TQ, in order to support QxTA
	     tq_device is usually the source tech primary device */
	  if ( is_irat_mode == FALSE )
	  {
	    tq_device = device;
	  }
	  else
	  {
	    tq_device = dev_status_r->irat_data.irat_tq_device;
	  }

      /* This function releases all the TQs and TQs are required to 
      queue any read\writes to CCS so this function call should be 
      after all device read or writes. */ 
      rf_cdma_mdsp_modem_rx_deallocate_resources ( tq_device, 
                                                   RFM_1XEVDO_MODE , 
                                                   rxlm_handle ) ;
    } /* !! if ( dev_status_r->data_hdr.rf_state == RF_CDMA_STATE_RX_PREP ) */

      /* Update CDMA Data structure with invalid RF State and  band/Chan */
    rf_cdma_reset_sw_state ( device ) ;
  }
    
  } /* if (sleep_success == TRUE) */
  else
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_sleep_rx(): Failed in "
                      "rfdevice_cdma_create_rx_sleep_script()" );
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }
  prof_sleep_t_val = rf_time_get_elapsed( prof_sleep_t, RF_USEC );
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&rx_hdr_sleep_PRx_time_hist,(uint32)prof_sleep_t_val );
    if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
    {
      rf_hdr_clear_profiling_hist_packet();
    }
  #endif
  /* Throw F3 Message */
  RF_MSG_3( RF_MED, "rf_hdr_mc_sleep_rx : [Status %d] - Dev %d, SSMA_time %d, "
                    "API_time %d", ret_val, device, prof_sleep_t_val );

  rf_cdma_data_sanity_check( device ); 

  /* Disable rx AGC logging */
  rf_hdr_mdsp_disable_rxagc_logging(logical_dev->rx_mdsp_path);

  /* return final result of API */
  return ret_val;

} /* rf_hdr_mc_sleep_rx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Disable the transmitter.

  @details
  Moves the HDR radio from the RxTx state to the Rx state.  Any circuits
  that are no longer needed are powered down to conserve current.

  This API will disable the RF ASIC transmitter, and prevent any power
  from being radiated from the device independent of the TX_ON and PA_ON
  control signals.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_HDR_TX_SLEEP_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.

  @param device
  The transmit path to put to sleep.
 
  @param txlm_handle
  The Tx Link Manager buffer that shall be used to configure the
  modem hardware.

  @return
  The anticipated time in microseconds remaining from the rf_hdr_mc_sleep_tx()
  function return until the operation is complete. Returns a 0 on success (no
  wait time needed) and a negative value on failure
*/
rfm_wait_time_t
rf_hdr_mc_sleep_tx
(
  const rfm_device_enum_type device,
  const lm_handle_type txlm_handle
)
{
  rf_time_tick_type sleep_tx_tick;    /* time spent in API */
  rf_time_type sleep_tx_time;
  rf_time_type FW_Resp_time=0; /* TIme spent in function */
  rf_time_tick_type FW_Resp_tick; /* TIme spent in function */
  rfm_wait_time_t ret_val; /* Return value */
  boolean sleep_success = TRUE; /* Flag to track the success of wakeup tx */
  rfm_cdma_band_class_type band;  /* Band */
  rf_cdma_data_status_type *dev_status_w; /* Device Status Write Pointer */
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  uint32 modem_chain;
  rf_cdma_mdsp_response_cb_param_type rsp_cb_param;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;

  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_sleep_tx: NULL FWRSP Data" );
    return RFM_CDMA_ERROR_NULL_DATA;
  } /* if ( fwrsp_data == NULL ) */

  sleep_tx_tick= rf_time_get_tick();
  /* Get Read/Writable Device Status */
  dev_status_w = rf_cdma_get_mutable_device_status( device );
  dev_status_r = rf_cdma_get_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* check for NULL pointers */
  if ( (dev_status_r == NULL) || (dev_status_w == NULL) || 
       (logical_dev == NULL) )
  {
     RF_MSG_1( RF_ERROR, "rf_hdr_mc_sleep_tx: NULL data for device %d",
               device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/

  /* Populate Current band and chan */
  band = dev_status_r->curr_band;

  /* Set Modem Chain value */
  modem_chain = rfc_cdma_get_tx_modem_chain(device, band);

  /*--------------------------------------------------------------------------*/
  /* Stop 1x temp comp routine*/
  /*--------------------------------------------------------------------------*/
  if ( rf_cdma_debug_flags.disable_temp_comp == 0 )
  {
    sleep_success &= rf_hdr_temp_comp_stop( &dev_status_w->temp_comp );
  }

  RF_MSG_1( RF_MED, "rf_hdr_mc_sleep_tx(): stop temp_comp timer %d", 
            sleep_success ); 


  /* Notify SV module of Tx stats */
  rfsv_send_tx_update_notification((boolean)FALSE,
                                     RFM_1XEVDO_MODE,
                                     0,
                                     0,
                                     NULL,
                                     0);

  sleep_success &= rf_hdr_plim_stop( device, &dev_status_w->plim );
  
  sleep_success &= rf_cdma_hdet_stop( device, &dev_status_w->hdet );

  /*Force isHKACEbusy to FALSE*/
  rf_cdma_data_set_isHKADCbusy( device, FALSE );
  
  /* -------------------------------------------------------------------------*/
  /* No Static Initialization can be done after this point. Only Tx Sleep
     hardware sequence is executed */
  rf_cdma_data_set_script_index ( device ,
                                  rf_cdma_mdsp_get_device_buffer_index( 
                                                                RFM_1XEVDO_MODE,
                                                                txlm_handle ) ) ;

  if ( sleep_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_tx_enter_critical_section (logical_dev->tx_device[band]);
    
    sleep_success &= rf_cdma_mc_configure_front_end_for_tx_sleep( device, 
                           dev_status_w->curr_band,
                           dev_status_w->script_index, logical_dev,
                           &dev_status_w->ant_tuner_script_token_data,
                           FALSE /* immediate_write */);

    if( dev_status_w->ept_online_enabled == TRUE )
    {
      sleep_success &= rf_common_xpt_init_dpd( device, NULL );
    }

    if ( sleep_success == TRUE )
    {
      rf_cdma_mdsp_tx_stop_type tx_stop_c; /* TxAGC Stop Config */
	  /* Reset the pout comp flag. */
	  rf_cdma_set_pout_comp_skip ( device , FALSE ) ;
  
      tx_stop_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_SLEEP_TX_ID;

      tx_stop_c.txlm_handle = txlm_handle; /* TXLM Handle */
      FW_Resp_tick = rf_time_get_tick();
      /* Send Command to MDSP to stop Tx */
      fwrsp_hk->tx_stop_rsp.device = device;
      fwrsp_hk->tx_stop_rsp.sema_token = rfcommon_semaphore_pop_item();
      rsp_cb_param.rsp_handler = rf_hdr_tx_stop_rsp_cb;
      rsp_cb_param.handler_data = &fwrsp_hk->tx_stop_rsp;

      sleep_success = rf_hdr_mdsp_stop_tx( logical_dev->tx_mdsp_path, 
                                        &tx_stop_c, &rsp_cb_param, NULL, NULL );

      if ( sleep_success == FALSE )
      {
        RF_MSG ( RF_ERROR, "rf_hdr_mc_sleep_tx(): "
                           "rf_hdr_mdsp_stop_tx failed "); 
        /* Message sending failed, free the semaphore */
        RF_MSG_2 ( RF_LOW, "rf_hdr_mc_sleep_tx():"
                   " Freeing the semaphore and not waiting "   
                   " [D%d] TX_STOP pushed back : Sem @ 0x%x" , 
                   device , fwrsp_hk->tx_stop_rsp.sema_token ) ;
      }
      else
      {
        RF_MSG_2( RF_LOW, "[HDR][RF-FW Sync] [D%d] TX_STOP : Start Wait Sem @ "
                          "0x%x", device, fwrsp_hk->tx_stop_rsp.sema_token );
        rfcommon_semaphore_wait( fwrsp_hk->tx_stop_rsp.sema_token );
      }

      FW_Resp_time = rf_time_get_elapsed( FW_Resp_tick, RF_USEC );
      /* Push back semaphore after wait if mssg was sent successfully,
      or immediately if mssg sending failed*/
      rfcommon_semaphore_push_item( fwrsp_hk->tx_stop_rsp.sema_token );

      if ( logical_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script(
          logical_dev->ant_tuner[band],
          &dev_status_w->ant_tuner_script_token_data );
      }
    }

    /* Always clean up static event smem and event handle */
    rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );

    /* Leave Device Critical Section */
    rfdevice_cdma_tx_leave_critical_section(logical_dev->tx_device[band] ) ;

    /* [BEGIN] ---------- FRBX related operations ------- */    
    if ( rf_cdma_data_get_fbrx_enable( RFM_1XEVDO_MODE ) == TRUE )
    {
      rfcommon_fbrx_error_type fbrx_status; /* fbrx opreation status */

	  /* clear the FBRx DM state variables */
	  rfcommon_fbrx_mc_deinit_tx(dev_status_r->fbrx_lm_handle);

      /* Deallocate FBRx DM buffers for given FBRxLM handle  */
      fbrx_status = rfcommon_fbrx_mc_exit( dev_status_r->fbrx_lm_handle,
                                           RFM_1XEVDO_MODE );
      /* Reset the pout comp flag. */
      rf_cdma_set_pout_comp_skip ( device , FALSE ) ;

      RF_MSG_1( RF_MED, 
                "rf_hdr_mc_sleep_tx: Calling rfcommon_fbrx_mc_exit with "
                "fbrx_handle %d, RFM_1XEVDO_MODE", 
                dev_status_r->fbrx_lm_handle );

      if(fbrx_status == RFCOMMON_FBRX_ERROR)
      {
        sleep_success = FALSE;
        RF_MSG(RF_ERROR, 
               "rf_hdr_mc_sleep_tx: FBRx DM buffer deallocation failed"); 
      }
      else
      {
        /* Update fbrx_lm_handle in rf_cdma_data */
        rf_cdma_data_set_fbrx_lm_handle( device , RFLM_DM_INVALID_HANDLE_ID );
      }
    } /* ---------- FRBX related operations ------- [END] */  

    if (rfcommon_autopin_is_enabled(RFM_1XEVDO_MODE))
    {
        rfcommon_fbrx_error_type fbrx_status;
        if(RFCOMMON_AUTOPIN_ERROR == 
                rfcommon_autopin_mc_disable_tx( dev_status_r->txlm_handle, RFM_1XEVDO_MODE ))
        {
          RF_MSG(RF_ERROR, "rf_1x_mc_sleep_tx: CDMA HDR AutoPin enable TX failed!");
          fbrx_status = RFCOMMON_FBRX_ERROR;
        }
        else
        {
          rf_cdma_auto_pin_tx_sleep(device, RFM_1XEVDO_MODE);
          rf_cdma_autopin_stop( device );
        }
     }
  } /* if ( sleep_success == TRUE ) */

  /* Deallocate RFLM Tx resources: Abort any pending AOL event */
  rf_cdma_mdsp_modem_tx_deallocate_resources( RFM_1XEVDO_MODE, txlm_handle );

  /* move forward only if command to FW was successful */
  if ( sleep_success == TRUE )
  {
    int32 dummy_result;
    boolean device_power_down_flag;

    /* Revert the Vreg State, since Tx is going to sleep */
    rfc_cdma_manage_vregs( device, RFM_1XEVDO_MODE, dev_status_r->curr_band,
                           RF_PATH_TX_SLEEP_STATE);
    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      /* Sleep the WTR for tx mode */
      device_power_down_flag = 
                      rfdevice_cdma_tx_pwr_sleep(logical_dev->tx_device[band]);
    }
    else
    {
      device_power_down_flag = TRUE;
    }
    if ( device_power_down_flag == FALSE )
    {
      /* Report error */
      RF_MSG_1( RF_ERROR,"rf_hdr_mc_sleep_tx: rfdevice_cdma_tx_pwr_sleep API "
              "failed for device: %d ",device ); 
    }

    /* Update CDMA Data structure with new the RF State */
    if ( rf_cdma_data_get_hdr_rf_state(device) == RF_CDMA_STATE_RXTX )
    {
      rf_cdma_data_set_hdr_rf_state(device, RF_CDMA_STATE_RX); 
    }
    else /* If in Tx only state, reset SW state machine */
    {
      rf_cdma_reset_sw_state( device );
    }

    rf_cdma_data_set_traffic_state( device, FALSE );
    rf_cdma_data_set_txlm_handle( device, RFM_INVALID_LM_BUFFER );
    rf_cdma_data_set_tx_bw_khz( device , RF_CDMA_BW_INVALID );

    //DeAllocate static PDMEM for TxAGC
    rflm_c2k_deallocate_txagc_ccs_mem(RFLM_TECH_HDR);

    /* no wait_time needed (set to 0)*/ 
    ret_val = 0;

    /* Tell GPS that HDR Tx is off. */
    RFGNSS_MC(wwan_tx_status, dummy_result)(device, FALSE);

    if (dummy_result == FALSE  )
    {
      /* Report error */
      RF_MSG( RF_ERROR,"rfgnss_mc_wwan_tx_status API failed");
    }

    /* Reset the associated master device for Tx to invalid */
    rf_cdma_data_set_assoc_master_dev(device, RFM_INVALID_DEVICE);
    /* Flag Callback event as successful */
  } /* if ( sleep_success == TRUE ) */
  else
  {
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }

  sleep_tx_time = rf_time_get_elapsed( sleep_tx_tick, RF_USEC );
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&tx_hdr_sleep_Tx_time_hist,(uint32)sleep_tx_time );
    prof_hist_update(&tx_hdr_sleep_fw_resp_hist,(uint32)FW_Resp_time);
    if( prof_hist_log_periodic( &hdr_rf_hist_packet_handle ) )
    {
      rf_hdr_clear_profiling_hist_packet();
    }
  #endif
  /* Throw F3 Message */
  RF_MSG_2( RF_MED, "rf_hdr_mc_sleep_tx : [Status %d] - Dev %d", 
            ret_val, device );

  rf_cdma_data_sanity_check( device ); 

  return ret_val;

} /* rf_hdr_mc_sleep_tx */

/*! @} */

/*============================================================================*/
/*!
  @name AGC Config Functions

  @brief
  Functions to configure HDR RxAGC and TxAGC
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Converge RxAGC
 
  @details
  This function is used to wait for HDR RxAGC to converge. This will wait for 
  response from HDR-FW indicating that RxAGC loop has moved to tracking state
  from Acquisition state
 
  @param device
  RFM device for which RxAGC convergene is expected
 
  @return
  Flag to indicate the success if the  function
*/
boolean
rf_hdr_mc_rx_agc_convergence
(
  rfm_device_enum_type device
)
{
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;

  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_rx_agc_convergence: NULL FWRSP Data" );
    return FALSE;
  } /* if ( fwrsp_data == NULL ) */

  /* wait for a response from FW for this message */
  RF_MSG_2( RF_LOW, "[HDR][RF-FW Sync] [D%d] AGC_TRK_IND : Start Wait Sem @ "
                    "0x%x", device, fwrsp_hk->rxagc_trk_ind.sema_token );
  rfcommon_semaphore_wait( fwrsp_hk->rxagc_trk_ind.sema_token );
  rfcommon_semaphore_push_item( fwrsp_hk->rxagc_trk_ind.sema_token );

  RF_MSG_1( RF_LOW, "rf_hdr_mc_rx_agc_convergence : Dev %d RxAGC converged", 
           device );

  return TRUE;   

} /* rf_hdr_mc_rx_agc_convergence */

/*! @} */

/*============================================================================*/
/*!
  @name Task Response Handlers

  @brief
  This section includes all the handlers for the responses received from RF
  Task.
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Rx Start Response handler
 
  @details
  This callback function is executed in second level dispatching when
  Rx Start Response is received from firmware. This function will basically post 
  the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_rx_start_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_rx_start_rsp_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_rx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_rx_start_rsp_data_type*)snum_data->snum_cb_data;

  RF_MSG_4( RF_MED, "[HDR][FW->RF] [D%d] RX_START_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->sema_token != NULL )
  {
    ret_val &= rfcommon_semaphore_post( cb_data->sema_token );
  } /* if ( cb_data->sema_token != NULL ) */
  else
  {
    ret_val = FALSE;
    ERR_FATAL( "rf_hdr_rx_start_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_hdr_rx_start_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_hdr_rx_start_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Rx Stop Response handler
 
  @details
  This callback function is executed in second level dispatching when
  Rx Stop Response is received from firmware. This function will basically post 
  the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_rx_stop_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_rx_stop_rsp_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_rx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_rx_stop_rsp_data_type*)snum_data->snum_cb_data;

  RF_MSG_4( RF_MED, "[HDR][FW->RF] [D%d] RX_STOP_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );
  
  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->sema_token != NULL )
  {
    ret_val &= rfcommon_semaphore_post( cb_data->sema_token );
  } /* if ( cb_data->sema_token != NULL ) */
  else
  {
    ret_val = FALSE;
    ERR_FATAL( "rf_hdr_rx_stop_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_hdr_rx_stop_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_hdr_rx_stop_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Tx Start Response handler
 
  @details
  This callback function is executed in second level dispatching when
  Tx Start Response is received from firmware. This function will basically post 
  the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_tx_start_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_tx_start_rsp_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_tx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_tx_start_rsp_data_type*)snum_data->snum_cb_data;

  RF_MSG_4( RF_MED, "[HDR][FW->RF] [D%d] TX_START_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->sema_token != NULL )
  {
    ret_val &= rfcommon_semaphore_post( cb_data->sema_token );
  } /* if ( cb_data->sema_token != NULL ) */
  else
  {
    ret_val = FALSE;
    ERR_FATAL( "rf_hdr_tx_start_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_hdr_tx_start_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_hdr_tx_start_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Tx Start Response handler
 
  @details
  This callback function is executed in second level dispatching when
  Tx Start Response is received from firmware. This function will basically post 
  the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_tx_stop_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_tx_stop_rsp_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_tx_stop_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_tx_stop_rsp_data_type*)snum_data->snum_cb_data;

  RF_MSG_4( RF_MED, "[HDR][FW->RF] [D%d] TX_STOP_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->sema_token != NULL )
  {
    ret_val &= rfcommon_semaphore_post( cb_data->sema_token );
  } /* if ( cb_data->sema_token != NULL ) */
  else
  {
    ret_val = FALSE;
    ERR_FATAL( "rf_hdr_tx_stop_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_hdr_tx_stop_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_hdr_tx_stop_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  FW State Config Response handler
 
  @details
  This callback function is executed in second level dispatching when
  FW State Config Response is received from firmware. This function will
  basically post the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_fws_config_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_fws_cfg_rsp_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_fws_config_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_fws_cfg_rsp_data_type*)snum_data->snum_cb_data;

  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->sema_token != NULL )
  {
    ret_val &= rfcommon_semaphore_post( cb_data->sema_token );
  } /* if ( cb_data->sema_token != NULL ) */
  else
  {
    ret_val = FALSE;
    ERR_FATAL( "rf_hdr_fws_config_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_hdr_fws_config_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_hdr_fws_config_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  iRAT Rx Start Response handler
 
  @details
  This callback function is executed in second level dispatching when
  iRAT Rx Start Response is received from firmware. This function will basically 
  post the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_irat_rx_start_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_irat_rx_start_rsp_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_irat_rx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_irat_rx_start_rsp_data_type*)snum_data->snum_cb_data;

  RF_MSG_4( RF_MED, "[HDR][FW->RF] [D%d] IRAT_RX_START_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  RF_MSG_3( RF_LOW, "rf_hdr_irat_rx_start_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "[Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum );

  return ret_val;

} /* rf_hdr_irat_rx_start_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Pilot meas config Response handler
 
  @details
  This callback function is executed in second level dispatching when
  pilot meas config Response is received from firmware. 
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_pilot_meas_config_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_pilot_meas_config_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_pilot_meas_config_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_pilot_meas_config_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_pilot_meas_config_rsp_cb: NULL CB Data" );
    return FALSE;
  } /* if ( cb_data == NULL ) */

  RF_MSG_4( RF_MED, "[1X][FW->RF] [D%d] PILOT_MEAS_CFG_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  RF_MSG_3( RF_LOW, "rf_hdr_pilot_meas_config_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "[Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum );

  return ret_val;

} /* rf_1x_pilot_meas_config_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Pilot Meas stop stream Response handler
 
  @details
  This callback function is executed in second level dispatching when
  pliot meas stop stream Response is received from firmware. This function will
  basically post the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_pilot_meas_stop_stream_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_pilot_meas_stop_stream_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_pilot_meas_stop_stream_rsp_cb: "
                      "NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_pilot_meas_stop_stream_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_pilot_meas_stop_stream_rsp_cb: NULL CB Data");
    return FALSE;
  } /* if ( cb_data == NULL ) */

  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->sema_token != NULL )
  {
    ret_val &= rfcommon_semaphore_post( cb_data->sema_token );
  } /* if ( cb_data->sema_token != NULL ) */
  else
  {
    ret_val = FALSE;
    ERR_FATAL( "rf_hdr_pilot_meas_stop_stream_rsp_cb: NULL Sem Token", 
               0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_1x_pilot_meas_stop_stream_rsp_cb: "
            "Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_1x_pilot_meas_stop_stream_rsp_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  RxAGC Track Indicator handler
 
  @details
  This callback function is executed in second level dispatching when
  RxAGC Track Indicator is received from firmware. This function will basically
  post the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_rxagc_track_ind_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_rxagc_trk_ind_rsp_data_type *cb_data = NULL;
  const rf_cdma_data_status_type *dev_status = NULL; /* Device Status Read Ptr*/
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_rxagc_track_ind_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_rxagc_trk_ind_rsp_data_type*)snum_data->snum_cb_data;

  RF_MSG_4( RF_MED, "[HDR][FW->RF] [D%d] ACQ_2_TRK_IND 0x%x [ %d | %d ]",
            cb_data->device, 
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->sema_token != NULL )
  {
    /* Get device status */ 
    dev_status = rf_cdma_get_device_status(cb_data->device);

    /* NULL Pointer check */
    if ( dev_status != NULL )
    {
      /* If any of the devices are in HDR mode, then not in IRAT mode.
      Since not in IRAT, post the semaphore*/
      if( dev_status->rf_mode == RFM_1XEVDO_MODE )
      {
        ret_val &= rfcommon_semaphore_post( cb_data->sema_token );

        RF_MSG_4( RF_LOW, "rf_hdr_rxagc_track_ind_cb: Dev %d - [%d] SNUM.0x%x "
                  "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
                  snum_data->snum, cb_data->sema_token );
      } /* if( dev_status->rf_mode == RFM_1XEVDO_MODE ) */
      else
      {
        /* Display AGC for ACK TO TRACK indication */
        int16 agc0 = 0, agc1 = 0, agc2 = 0;
        agc0 = rfm_hdr_get_rx_agc( cb_data->device, RFM_CDMA_CARRIER_0,
                                   RFM_CDMA_AGC_FORMAT__DBM10);
        agc1 = rfm_hdr_get_rx_agc( cb_data->device, RFM_CDMA_CARRIER_1,
                                   RFM_CDMA_AGC_FORMAT__DBM10);
        agc2 = rfm_hdr_get_rx_agc( cb_data->device, RFM_CDMA_CARRIER_2,
                                   RFM_CDMA_AGC_FORMAT__DBM10);
        RF_MSG_6( RF_HIGH, "rf_hdr_rxagc_track_ind_cb: Dev %d - iRAT Mode "
                  "[%d] SNUM.0x%x RxAGC(dBm10) => AGC0 %d, AGC1 %d, AGC2 %d",
                  cb_data->device, snum_data->item_id, snum_data->snum, 
                  agc0, agc1, agc2);
      } /* if ! ( dev_status->rf_mode == RFM_1XEVDO_MODE ) */
    } /* if ( dev_status != NULL ) */
    else
    {
      RF_MSG_1( RF_ERROR, "rf_hdr_rxagc_track_ind_cb: Dev %d - NULL Data",
                cb_data->device );
    } /* if ! ( dev_status != NULL ) */
  } /* if ( cb_data->sema_token != NULL ) */
  else
  {
    ret_val = FALSE;
    ERR_FATAL( "rf_hdr_rxagc_track_ind_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  return ret_val;

} /* rf_hdr_rxagc_track_ind_cb */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  RxAGC logging handler
 
  @details
  This callback function is executed in second level dispatching when
  RxAGC logging cmd is received from firmware. This function will basically 
  post the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_hdr_rxagc_logging_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  rfm_device_enum_type device;
  boolean ret_val = TRUE;
  rf_cdma_rxagc_logging_data_type *cb_data = NULL;
  rf_hdr_rx_log_buffer_index_data_type log_buffer_index_data;
  rf_hdr_mdsp_rx_agc_read_type rx_agc_data[4];
  const rf_cdma_data_status_type *device_status_r;
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */

  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_hdr_rxagc_logging_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_rxagc_logging_data_type*)snum_data->snum_cb_data;

  /* Check if the callback data is NULL */
  if ( cb_data != NULL )
  {
    /* Copy the payload for the command over to the buffer index data
    structure */
    log_buffer_index_data.buffer_index_data_array[0] = 
                                                  (int8)req_ptr->payload[0];
    log_buffer_index_data.buffer_index_data_array[1] = 
                                                  (int8)req_ptr->payload[1];

    device = cb_data->device;

    /* Read MDSP log buffer only if device is in HDR mode and logging
    is enabled */
    device_status_r = rf_cdma_get_device_status( device );

    if ( device_status_r == NULL )
    {
      RF_MSG_1( RF_ERROR, "rf_hdr_rxagc_logging_cb: Dev %d - NULL data", 
                device );
      ret_val = FALSE;
    } /* if ( device_status_r == NULL ) */

    /* Proceed only if everything till this point was OK*/
    if ( ret_val == TRUE )
    {
      if ( device_status_r->rf_mode != RFM_1XEVDO_MODE )
      {
        RF_MSG_1( RF_ERROR, "rf_hdr_rxagc_logging_cb: Dev %d - Not in HDR "
                  "Mode",device );
        ret_val =  FALSE;
      } /* if ( device_status_r->rf_mode != RFM_1XEVDO_MODE ) */
      else
      {
        /* Proceed only if we are in HDR mode*/

        /* Get Logical Device Param */
        logical_dev = rfc_cdma_get_logical_device_params( device );

        /* check for NULL pointers */
        if ( (logical_dev == NULL) || 
             (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM ))
        {
           RF_MSG_1( RF_ERROR, "rf_hdr_rxagc_logging_cb: Dev %d - NULL"
                     " logical data", device );
           ret_val = FALSE;
        } /* if ( (logical_dev == NULL) || 
             (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )) */
        else
        {
          rf_hdr_mdsp_read_rx_log_buffer( logical_dev->rx_mdsp_path,
                                          rx_agc_data,
                                          log_buffer_index_data.buffer_index);
        
          /* Print Rx log buffer data for the index that was passed in */
          RF_MSG_9( RF_LOW, "rf_hdr_rxagc_logging_cb: Dev %d [RxAGC(dBm10), "
                    "LNA]  = [%d, G%d, %d, G%d, %d, G%d, %d, G%d]", 
                    device,
                    rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                      rx_agc_data[0].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                    rx_agc_data[0].lna_state,
                    rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                      rx_agc_data[1].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                    rx_agc_data[1].lna_state,
                    rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                      rx_agc_data[2].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                    rx_agc_data[2].lna_state,
                    rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                      rx_agc_data[3].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                    rx_agc_data[3].lna_state );

          /* If diversity is enabled, perform logging for Div chain as well */
          if ( device_status_r->assoc_dev != RFM_INVALID_DEVICE )
          {
            /* Get Logical Device Param */
            logical_dev = rfc_cdma_get_logical_device_params( 
                                          device_status_r->assoc_dev );

            /* check for NULL pointers */
            if ( (logical_dev == NULL) || 
                 (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM ))
            {
               RF_MSG_1( RF_ERROR, "rf_hdr_rxagc_logging_cb: sDev %d - NULL "
                                  "logical data", device_status_r->assoc_dev);
               ret_val = FALSE;
            } /* if ( (logical_dev == NULL) || 
                 (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )) */
            else
            {
              rf_hdr_mdsp_read_rx_log_buffer( logical_dev->rx_mdsp_path,
                                          rx_agc_data,
                                          log_buffer_index_data.buffer_index);

              /* Print Rx log buffer data for the index that was passed in */
              RF_MSG_9( RF_LOW, "rf_hdr_rxagc_logging_cb: sDev %d "
                        "[RxAGC(dBm10), LNA] = "
                        "[%d, G%d, %d, G%d, %d, G%d, %d, G%d]", 
                        device_status_r->assoc_dev,
                        rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                          rx_agc_data[0].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                        rx_agc_data[0].lna_state,
                        rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                          rx_agc_data[1].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                        rx_agc_data[1].lna_state,
                        rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                          rx_agc_data[2].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                        rx_agc_data[2].lna_state,
                        rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                          rx_agc_data[3].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                        rx_agc_data[3].lna_state );
            } /* if !( (logical_dev == NULL) || 
                  (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )) */
          } /* if ( device_status_r->assoc_dev != RFM_INVALID_DEVICE ) */
        } /* if ! ( (logical_dev == NULL) || 
             (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )) */
      }/* if ! ( device_status_r->rf_mode != RFM_1XEVDO_MODE )*/
    }/*if ( ret_val == TRUE ) (device_status_r != NULL)*/
  }/*if ( cb_data != NULL )*/
  else
  {
    /* This is not a failure case since the callback data might be NULL
    for cases where it has already been deregistered */
    RF_MSG( RF_LOW, "rf_hdr_rxagc_logging_cb:callback data is NULL ");
  }/*if ( cb_data == NULL )*/

  if ( ret_val == FALSE )
  {
    RF_MSG( RF_ERROR, "rf_hdr_rxagc_logging_cb: API Failed!! ");
  }

  return ret_val;

} /* rf_hdr_rxagc_logging_cb */

/*============================================================================*/
/*!
  @name SAR

  @brief
  Functions to be used for SAR.
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Inform the HDR Tx limit loop that the SAR state has changed, so it can
  recompute the target max power, if necessary.

  @details
  Assumes that the RF critical section has already been locked by this thread,
  and that it is safe to access the internal data structures.

  This interface makes no assumption about what devices may or may not be
  in HDR mode, so it will loop through all devices, and perform a power
  limiting update if they are in HDR mode and currently transmitting.
*/
void
rf_hdr_mc_tx_update_sar_backoff
(
  void
)
{
  rfm_device_enum_type device;

  for ( device = RFM_DEVICE_0;
        device < RFM_MAX_DEVICES;
        device++ )
  {
    if ( rfm_get_current_mode(device) == RFM_1XEVDO_MODE )
    {
      const rf_cdma_data_status_type *dev_status_r;

      dev_status_r = rf_cdma_get_device_status( device );

      /* Perform NULL Pointer check */
      if ( dev_status_r == NULL )
      {
         RF_MSG_1( RF_ERROR, "rf_hdr_mc_retune_tx_update_sar_backoff: NULL data"  
                   "for device %d", device );
      }
      else
      {
        if ( dev_status_r->data_hdr.rf_state == RF_CDMA_STATE_RXTX ||
             dev_status_r->data_hdr.rf_state == RF_CDMA_STATE_TX)
        {
          rf_hdr_plim_update_sar_state( device );
        }
      }
    } /* if ( rfm_get_current_mode(device) == RFM_1XEVDO_MODE ) */
  } /* for device 0..N-1 */

  return ;
} /* rf_hdr_mc_tx_update_sar_backoff() */

/*! @} */

/*============================================================================*/
/*!
  @name SV Limit

  @brief
  Functions to be used for SV Limit.
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Inform the HDR Tx limit loop of the SV max power limit, so it can
  recompute the target max power, if necessary.

  @details
  This interface makes no assumption about what devices may or may not be
  in HDR mode, so it will loop through all devices, and perform a power
  limiting update if they are in HDR mode and currently transmitting.

  @param max_power_limit_dbm10
  The max power limit from the SV module, in dBm10 units.
*/
void
rf_hdr_mc_tx_update_sv_limit
(
  int16 max_power_limit_dbm10
)
{
  rfm_device_enum_type device;

  for ( device = RFM_DEVICE_0;
        device < RFM_MAX_DEVICES;
        device++ )
  {
    if ( RFM_1XEVDO_MODE == rfm_get_current_mode(device) )
    {
      const rf_cdma_data_status_type *dev_status_r;
      dev_status_r = rf_cdma_get_device_status( device );

      /* Perform NULL Pointer check */
      if ( dev_status_r == NULL )
      {
         RF_MSG_1( RF_ERROR, "rf_hdr_mc_tx_update_sv_limit: NULL data"  
                   "for device %d", device );
      } /*  if ( dev_status_r == NULL ) */
      else
      {
        if ( RF_CDMA_STATE_RXTX == dev_status_r->data_hdr.rf_state ||
             RF_CDMA_STATE_TX == dev_status_r->data_hdr.rf_state )
        {
          rf_hdr_plim_update_sv_limit( device, max_power_limit_dbm10 );
        } /* if HDR is transmitting */
      } /* if ! if ( dev_status_r == NULL ) */
    } /* if current mode is HDR */
  } /* for device 0..N-1 */
} /* rf_hdr_mc_tx_update_sv_limit() */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Handler for HDR RxAGC log buffer index response received through RF task
 
  @details
  This handler is dispatched once RFSW receives the HDR LOG BUF IND message 
  from HDR FW. The message provides the index of the buffer into which 
  AGC data per slot is being logged and is the latest. RFSW then uses the 
  index to pull out data from SMEM.
 
  @param module_id
  RF Task Module ID for which this handler function is called.
 
  @param req_id
  Command ID pertaining to this handler
 
  @param req_payload_size
  Size of payload assoicated with the commands for this handler function
 
  @param req_ptr
  Pointer to RF Task command
*/
void
rf_hdr_mc_handle_rx_log_ind_msg
(
  rf_fwrsp_cmd_type* req_ptr
)
{
  rfm_device_enum_type device;
  const rf_cdma_data_status_type *device_status_r;
  const rf_cdma_debug_flag_type *debug_flag_data;
  rf_hdr_rx_log_buffer_index_data_type log_buffer_index_data;
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  rf_hdr_mdsp_rx_agc_read_type rx_agc_data[4];
  boolean logging_enabled;

  /* NULL pointer check */
  if ( req_ptr == NULL )
  {
    RF_MSG( RF_ERROR, "rf_hdr_mc_handle_rx_log_ind_msg: "
                      "NULL RF Cmd Ptr" );
    return;
  }

  /* Copy the payload for the command over to the buffer index data
  structure */
  log_buffer_index_data.buffer_index_data_array[0] = (int8)req_ptr->payload[0];
  log_buffer_index_data.buffer_index_data_array[1] = (int8)req_ptr->payload[1];

  /* Use internal RF log packet mask to enable/disable logging across all 
  devices */
  logging_enabled = log_status( LOG_RF_DEV_C );

  for ( device = RFM_DEVICE_0; device < RFM_MAX_DEVICES; device++ )
  {
    /* Read MDSP log buffer only if device is in HDR mode and logging
    is enabled */
    device_status_r = rf_cdma_get_device_status( device );

    /* NULL pointer check */
    if ( device_status_r == NULL )
    {
      return;
    }

    debug_flag_data = rf_cdma_get_debug_flag_data();

    /* continue only if Logging has been enabled from QXDM or if the debug 
       flag has been enabled
    */
    if ( (logging_enabled || debug_flag_data->rx_agc_logging[device] == TRUE)
         && device_status_r->rf_mode == RFM_1XEVDO_MODE) 
    {
      /* Get Logical Device Param */
      logical_dev = rfc_cdma_get_logical_device_params( device );

      /* check for NULL pointers */
      if ( (logical_dev == NULL) || 
           (logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM ))
      {
        /* commenting out for now to avoid spamming QXDM */
        /* 
         RF_MSG_1( RF_ERROR, "rf_hdr_mc_handle_rx_log_ind_msg: "
                             "NULL data for Dev %d",
                   device );
        */
      }
      else
      {
        rf_hdr_mdsp_read_rx_log_buffer( logical_dev->rx_mdsp_path,
                                        rx_agc_data,
                                        log_buffer_index_data.buffer_index );
      
        /* Print Rx log buffer data for the index that was passed in */
        RF_MSG_8( RF_LOW, "rf_hdr_mc_handle_rx_log_ind_msg: "
                          "[RxAGC(dBm10), LNA] = "
                          "[%d, G%d, %d, G%d, %d, G%d, %d, G%d]",
                  rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                    rx_agc_data[0].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                  rx_agc_data[0].lna_state,
                  rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                    rx_agc_data[1].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                  rx_agc_data[1].lna_state,
                  rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                    rx_agc_data[2].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                  rx_agc_data[2].lna_state,
                  rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10,
                    rx_agc_data[3].rx_agc[RFM_CDMA_CARRIER_CUMULATIVE]),
                  rx_agc_data[3].lna_state );
        RF_MSG_1( RF_LOW, "rf_hdr_mc_handle_rx_log_ind_msg: "
                          "Handled for Dev %d",
                  device );
      }
    }
  }

  return;

} /* rf_hdr_mc_handle_rx_log_ind_msg */

/*! @} */ /* End SV Limit Section */

/*============================================================================*/
/*!
  @name Tear Down

  @brief 
  This section contains functions pertaining to Tear Down
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Tear Down RF

  @details
  This function will Tear Down RF to Sleep State, no mater what the current
  state is. 
 
  @param device RF Device on which RF is to be torn down
 
  @return
  Flag for the function to indicate whether tear down was required or not. Based 
  on this parameter, rfm_enter_mode() will be called in calling function. If 
  TRUE, rfm_enter_mode() is not called. This will also be FALSE for the case,
  when RFM failure is seen during tear-down; thus indicating the calling
  function that rfm_enter_mode() has to be done to reset everything.
*/
boolean
rf_hdr_mc_tear_down
(
  rfm_device_enum_type device
)
{
  boolean was_tear_down_req = TRUE; /* Flag indicating if tear down is needed */
  boolean rfm_ret = TRUE; /* Return val from RFM */
  rf_cdma_data_status_type *dev_status = NULL; /* RF Dev Status */
  rf_cdma_data_status_type *assoc_dev_status = NULL; /* RF assoc Dev Status */

  /* Query the current RF State Ptr for master device */
  dev_status = rf_cdma_get_mutable_device_status(device);

  /* NULL pointer check*/
  if(  dev_status == NULL )
  {
    RF_MSG_1(RF_ERROR,"rf_hdr_mc_tear_down: NULL data for device %d",device);
    was_tear_down_req = FALSE;
  }/* if(  dev_status == NULL ) */

  else
  {
    switch ( dev_status->data_hdr.rf_state )
    {
    case RF_CDMA_STATE_RX:
      /* Check if there is other associated Device with this device. If there
      in any other associated device, then diversity must be disabled on
      the associated device */
      if ( dev_status->assoc_dev != RFM_INVALID_DEVICE )
      {
        assoc_dev_status = rf_cdma_get_mutable_device_status(
                                                 dev_status->assoc_dev);
                
        /* NULL pointer check*/
        if(  assoc_dev_status == NULL )
        {
          RF_MSG_1(RF_ERROR,"rf_hdr_mc_tear_down: NULL data for device %d"
                   ,device);
          was_tear_down_req = FALSE;
        }/* if(  assoc_dev_status == NULL ) */
        else
        {
          rfm_ret &= ( rfm_hdr_disable_diversity( dev_status->assoc_dev,
                                                 assoc_dev_status->rxlm_handle,
                                                 NULL, 
                                                 NULL ) >= 0 );
          if (rfm_ret == TRUE)
          {
            assoc_dev_status->data_hdr.rf_state = RF_CDMA_STATE_SLEEP;
            assoc_dev_status->assoc_dev = RFM_INVALID_DEVICE;
            dev_status->assoc_dev = RFM_INVALID_DEVICE;
          }
          else
          {
            RF_MSG_1( RF_ERROR, "rf_hdr_mc_tear_down: Failed to disable " 
                      "diversity on associated device %d", 
                      dev_status->assoc_dev ); }
        }/* if ! (  assoc_dev_status == NULL ) */
      }
  
      rfm_ret &= ( rfm_hdr_sleep_rx( device, dev_status->rxlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      dev_status->data_hdr.rf_state = RF_CDMA_STATE_SLEEP;
  
      break;
  
    case RF_CDMA_STATE_RXTX:
      /* Check if there is other associated Device with this device. If there
         is any other associated device, then Diversity must be disabled on
         the associated device */
      if ( dev_status->assoc_dev != RFM_INVALID_DEVICE )
      {
        assoc_dev_status = rf_cdma_get_mutable_device_status(
                                                 dev_status->assoc_dev);
                     
        /* NULL pointer check*/
        if(  assoc_dev_status == NULL )
        {
          RF_MSG_1(RF_ERROR,"rf_hdr_mc_tear_down: NULL data for device %d"
                   ,device);
          was_tear_down_req = FALSE;
        }/* if(  assoc_dev_status == NULL ) */
        
        else
        {
          rfm_ret &= ( rfm_hdr_disable_diversity( dev_status->assoc_dev, 
                                                 assoc_dev_status->rxlm_handle,
                                                 NULL, 
                                                 NULL ) >= 0 );
    
          if (rfm_ret == TRUE)
          {
            assoc_dev_status->data_hdr.rf_state = RF_CDMA_STATE_SLEEP;
            assoc_dev_status->assoc_dev = RFM_INVALID_DEVICE;
            dev_status->assoc_dev = RFM_INVALID_DEVICE;
          }
          else
          {
            RF_MSG_1( RF_ERROR, "rf_hdr_mc_tear_down: Failed to disable "
                                "diversity on associated device %d", 
                                  dev_status->assoc_dev ); }
        }/* if ! (  assoc_dev_status == NULL ) */
      }
      rfm_ret &= ( rfm_hdr_sleep_tx( device, dev_status->txlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      rfm_ret &= ( rfm_hdr_sleep_rx( device, dev_status->rxlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      dev_status->data_hdr.rf_state = RF_CDMA_STATE_SLEEP;
  
      break;
    case RF_CDMA_STATE_TX:
    
      rfm_ret &= ( rfm_hdr_sleep_tx( device, dev_status->txlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      rf_cdma_data_set_hdr_rf_state ( device, RF_CDMA_STATE_SLEEP ); 
  
    
      break;
  
    case RF_CDMA_STATE_RXDIV:
      /* Disable the diversity for this device, as it is associated with other
      Master Device */
      assoc_dev_status = rf_cdma_get_mutable_device_status(
                                               dev_status->assoc_dev); 
             
      /* NULL pointer check*/
      if(  assoc_dev_status == NULL )
      {
        RF_MSG_1(RF_ERROR,"rf_hdr_mc_tear_down: NULL data for device %d"
                 ,device);
        was_tear_down_req = FALSE;
      }/* if(  dev_status->assoc_dev == NULL ) */
      else
      {
        rfm_ret &= ( rfm_hdr_disable_diversity( device, dev_status->rxlm_handle,
                                               NULL, 
                                               NULL ) >= 0 );
    
        if (rfm_ret == TRUE)
        {
          assoc_dev_status->assoc_dev = RFM_INVALID_DEVICE;
          dev_status->data_hdr.rf_state = RF_CDMA_STATE_SLEEP;
          dev_status->assoc_dev = RFM_INVALID_DEVICE;
        }
        else
        {
          RF_MSG_1( RF_ERROR, "rf_hdr_mc_tear_down: Failed to disable diversity"
                    " on associated device %d", dev_status->assoc_dev ); 
        }  
      }/* if ! (  dev_status->assoc_dev == NULL ) */

      break;
  
    case RF_CDMA_STATE_SLEEP:
    case RF_CDMA_STATE_INVALID:
    default:
      was_tear_down_req = FALSE;
      RF_MSG( RF_LOW, "rf_hdr_mc_tear_down: Tear Down RF : Not Required" );
      break;
    }
  
    if ( rfm_ret == TRUE )
    {
      dev_status->data_hdr.rf_state = RF_CDMA_STATE_SLEEP;
    }
    else
    {
      dev_status->data_hdr.rf_state = RF_CDMA_STATE_SLEEP;
      was_tear_down_req = FALSE;
    }
  
    RF_MSG_1( RF_LOW, "rf_hdr_mc_tear_down :Tear Down on Dev %d done", device );
  
  } /* if ! (  dev_status == NULL ) */

  /* Return */
  return was_tear_down_req;

} /* rf_hdr_mc_tear_down */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  rf wrapper function to get L1SRAM Sample Memory
  Address for CDMA HDR.

  @details
  Returns the L1SRAM Sample Memory Address for CDMA HDR.

  @param device
  The radio path that is being queried.

  @param carrier
  When operating in multi-carrier mode, and as fimrware supports
  SRAM Base Adresses per-carrier basis for HDR, this parameter can be used to
  get the SRAM mempool Base Adress for a specific carrier.

  @return
  L1SRAM Sample Memory Base Address.

*/
 uint32
 rf_hdr_get_mem_pool_base_addr
 (
   const rfm_device_enum_type device,
   const rfm_hdr_carrier_id_type carrier
 )
 {
   const rfc_cdma_logical_device_params_type* logical_dev;

   if ( device >= RFM_MAX_DEVICES || carrier >= RFM_CDMA_CARRIER_CUMULATIVE )
   {
	 RF_MSG_2( RF_ERROR, "rf_hdr_get_mem_pool_base_addr: Invalid device %d or carrier %d ",
				   device, carrier);
	 return NULL;
   }
   /* Get and validate Logical Device */
   logical_dev = rfc_cdma_get_logical_device_params( device );

   if ( logical_dev == NULL )
   {
	  RF_MSG_DBG_1 ( RF_ERROR, "rf_hdr_get_mem_pool_base_addr(): Invalid logical device "
							   "for device %d", device);
	  return NULL ;
	}

	/* Validate Rx mdsp path */
	if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
	{
	  RF_MSG_1( RF_ERROR, "rf_hdr_get_mem_pool_base_addr(): Invalid rx_mdsp_path %d",
					logical_dev->rx_mdsp_path );

	  return NULL ;
	}

   return (rf_hdr_mdsp_get_mem_pool_base_addr( carrier, logical_dev->rx_mdsp_path));

}/* rf_hdr_get_mem_pool_base_addr */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  utility API to clear hdr time profiling histogram packect 0x18e9,

  @param 
   void
 
  @return
  void 
   
*/
void rf_hdr_clear_profiling_hist_packet
(
  void
)
{
#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_clear(&rx_hdr_wakeup_PRx_time_hist);
    prof_hist_clear(&rx_hdr_wakeup_DRx_time_hist);
    prof_hist_clear(&rx_hdr_wakeup_fw_resp_hist);
    prof_hist_clear(&rx_hdr_sleep_PRx_time_hist);
    prof_hist_clear(&rx_hdr_sleep_DRx_time_hist);
    prof_hist_clear(&rx_hdr_sleep_fw_resp_hist);
    prof_hist_clear(&tx_hdr_wakeup_Tx_time_hist);
    prof_hist_clear(&tx_hdr_wakeup_fw_resp_hist);
    prof_hist_clear(&tx_hdr_sleep_Tx_time_hist);
    prof_hist_clear(&tx_hdr_sleep_fw_resp_hist);
#endif
  
} /*rf_hdr_clear_profiling_hist_packet*/

#endif /* FEATURE_CDMA1X */

/*! \} */

/*! @} */
