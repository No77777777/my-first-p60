/*!
  @file
  rf_1x_mc.c
 
  @brief
  RF Driver's 1x MC interface file.

  @details
  This file defines the RFM 1X Main Control interfaces.
 
  @addtogroup RF_CDMA_1X_MC
  @{
*/

/*==============================================================================

  Copyright (c) 2010 - 2017 Qualcomm Technologies Incorporated. All Rights Reserved

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

    $Header: //components/rel/rftech_cdma.mpss/3.11/1x/rf/src/rf_1x_mc.c#3 $
  
when       who     what, where, why
--------   ---     -------------------------------------------------------------
06/20/22   ms      Added NULL check for logical_dev inside timed_sleep_rx
04/14/22   ys      Call WTR pwr_sleep API during timed_sleep_rx
11/11/19   mk      Added 1x Modem Timeline histogram profiling changes
03/09/18   mk      KW Errors fix
10/26/17   sk      Enable LTE B71 support
01/18/17   sk      Send Tx handle to XPT api's for LTE
07/21/16   rs      Added support to take care of additional PA RFFE writes in 
                   txagc
07/12/16   pk      Removed reference to rflm_cmn_iref_temp_comp_therm_read since 
                   the compensation is not required on TA
05/28/16   wxl     Update CDMA state machine before autopin check.
05/11/16   rs      Removed excessive F3 msgs
05/03/16   rs      Update tuner scripts for Div when updating for Tx
03/24/16   sbo     Move Txagc SEQ update  to Static Scripts in PDMEM.
03/14/16   pk      Changed logic to call rf_cdma_mc_configure_stop_rx_agc_data
03/10/16   rs      Changing RF_CDMA_CCS_EVENT* to RFLM_CDMA_MDSP_CCS_EVENT*
03/03/16   rs      Allow Skip-pin only if Apin and PAR support are enabled
03/03/16   rs      Added support for 1x2G QTA
03/03/16   rs      Allow Skip-pin only if Apin is enabled
03/02/16   zhh     update definition of rfcommon_autopin_mc_enable_tx
01/22/16   rs      Set FBRx enable flag for TxAGC depending on FBRx is available or not
01/20/16   rs      Fixed RxAGC data cleanup in hard handoff case
01/19/16   rs      Updated DRx antenna position handling
12/24/15   vr      Autopin check-in_2
12/03/15   rs      NV support to enable/disable LPM feature
12/02/15   vr      Added Auto-Pin DM update
11/26/15   rs      Added FL DRX override support
11/25/15   rs      IQ capture support
11/25/15   rs      RxAGC not being cleaned in hard handoff case
11/20/15   vr      Autopin check-in
10/14/15   rs      Moved core dev vote inside iRAT flag
10/14/15   rs      Modified sleep API and moved the Dev power unvote out of 
                   iRAT check to fix the High DC in LTE after L21x QTA and 
                   DO2L iRAT crash
09/21/15    aa     Save TX/FBRx LM buffer info in FBRx state machine
09/14/15   aak     Vote Tx_SLEEP instead of Rx State in Tx Disable
08/31/15   wwl     Add function calls to update autopin C2K state machine
08/18/15   zhw     Skip script inside disable_diversity if in irat
08/17/15   zhw     Move 1x/HDR meas tune-away & reset script into private buffers
07/30/15   cdb     Add runtime check for Autopin enabled
07/29/15   pl      call rfcommon_fbrx_mc_deinit_tx when disabling TX 
07/24/15   sty     initialize power_mode in rf_1x_mc_enable_diversity()
07/22/15   zhw     Added support for dynamic Rx MDSP Chain mapping
07/14/15    aa     Added support for FBRx C1 NV container
06/30/15   wwl     Add support for Auto Pin
06/25/15   APU     Deleted old revision history
06/24/15   spa     WTR power/up power down support
06/17/15   vc      Correcting the condition check for slave id
06/14/15   vc      Added dynamic txr_iq_dac setting update support
05/19/15   spa     Use different event id for prx and drx
05/22/15   sub     Placeholder parameter to support fbrx interface change
04/23/15    ka     Remove hardcoded device 0 for fbrx
05/11/15   zhw     Disable Drx support during L->1x QTA
04/26/15   zhw     1x QCTA use source tech device in irat data for TQ allocation
04/17/15   pk      Workaround for TA_BU to make LB to HB Handoff
04/16/15   zhw     Reset SW state machine during sleep_tx() for Tx only device
04/15/15   zhw     Integrate device driver change with CDMA
                   Remove deprecated API
04/14/15   zhw     Fix rf state update during sleep_tx
04/02/15   zhw     Move Tx RF On/off config into configure_front_end_tx
                   Ensures same call flow for tx_retune and wakeup_tx
04/01/15   zhw     Further refactor retune. Call retune_tx/rx helper seperately
03/30/15   zhw     MC changes to support seperate Tx device tune
                   - Rename rf_1x_mc_retune_rx to rf_1x_mc_retune
                   - New Tx retune helper API rf_1x_mc_helper_for_tx_retune
                   - Refactored rf_1x_mc_helper_for_retune
                   - Moved away from configure_rffe_for_retune
                   - Expanded rf_1x_mc_wakeup_tx() to take prx Device
                   - Copy essential state machine info at the beginning of
                     wakeup_tx() from Prx device
                   - Support new CDMA state RF_CDMA_STATE_TX in MC code
03/26/15   wwl     Fix few bugs causing code to crash
03/23/15   wwl     Add rf_1x_mc_wakeup_tx_v2
03/16/15   zhw     Support Prx+Drx tune in 1x iRAT/MEAS tune-away call flow
04/14/15   pk      Tx spectral inversion changes
03/16/15   pk      Compiler warning Fix
03/10/14   spa     Use Stop TxAGC MDSP API to freeze/unfreeze TxAGC on RFLM side
12/09/10   aro     Initial version
==============================================================================*/

#include "rfa_variation.h"
#include "comdef.h"
#ifdef FEATURE_CDMA1X
#include "rf_1x_mc.h"
#include "rf_cdma_data.h"
#include "rf_cdma_mdsp.h"
#include "rf_cdma_intelliceiver.h"
#include "rf_1x_power_limiting.h"
#include "rf_cdma_hdet.h"
#include "rf_cdma_temp_comp.h"
#include "rf_1x_mdsp.h"
#include "rf_cdma_mdsp.h"
#include "rf_1x_rx_agc.h"
#include "rf_1x_tx_agc.h"
#include "rfcommon_core.h"
#include "rfcommon_core_xpt.h"
#include "rfcommon_msg.h"
#include "rfcommon_mdsp.h"
#include "rfm_cdma.h"
#include "rf_cdma_msm.h"
#include "rfc_cdma.h"
#include "modem_mem.h"
#include "rf_cdma_nv.h"
#include "rf_cdma_utils.h"
#include "rf_cdma_utils_freq.h"
#include "rf_cdma_time_profile.h"
#include "rfm_internal.h"
#include "rfdevice_cdma_interface.h"
#include "rfdevice_intf_cmd.h"
#include "rf_cdma_ssma.h"
#include "rfgnss_mc.h"
#include "rfm_types.h"
#include "rfcommon_core_device_manager.h"
#include "ftm.h"
#include "rf_cdma_mc.h"
#include "rf_1x_fw_response.h"
#include "rf_cdma_constants.h"
#include "rf_1x_meas.h"
#include "rfcommon_tx_manager.h"
#include "rfcommon_time_profile.h"
#include "rfcommon_data.h"

#include "rfcommon_autopin_api.h"

/* New Buffer Interface */
#include "rf_hal_buffer.h"
#include "rf_hal_bus_types.h"
#include "rfcommon_mc.h"
#include "rfcommon_data.h"
#include "rfcommon_msm.h"

#include "rf_hal_common.h" /* for CCS command app */

#include "rf_1x_ept.h"
#include "rf_1x_log.h"
#include "rfdevice_class.h" /* rfdevice_id_get() */

#include "rf_cdma_mdsp_ccs_events.h"
#include "rf_cdma_mdsp_modem.h"

#include "rfcommon_fbrx_api.h"
#include "rflm_cmn_asd.h"
#include "rflm_dm_api.h"
#include "rflm_cdma_tuner.h"
#include "rf_cdma_auto_pin.h"
#include "rf_cdma_rx_agc.h"
#include "rflm_c2k_mc.h"
#include "rfcommon_nv_mm.h"
#include "rf_cdma_meas.h"

#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
#include "prof_hist.h"
#endif

#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
        extern prof_hist_log_packet_handle_t cdma_rf_hist_packet_handle;
        extern prof_hist_param_t   rx_1x_wakeup_PRx_time_hist;
        extern prof_hist_param_t   rx_1x_wakeup_DRx_time_hist;
        extern prof_hist_param_t   rx_1x_wakeup_fw_resp_hist;
        extern prof_hist_param_t   rx_1x_sleep_PRx_time_hist;
        extern prof_hist_param_t   rx_1x_sleep_DRx_time_hist;
        extern prof_hist_param_t   rx_1x_sleep_fw_resp_hist;
        extern prof_hist_param_t   tx_1x_wakeup_Tx_time_hist;
        extern prof_hist_param_t   tx_1x_wakeup_fw_resp_hist;
        extern prof_hist_param_t   tx_1x_sleep_Tx_time_hist;
        extern prof_hist_param_t   tx_1x_sleep_fw_resp_hist;
#endif


/*============================================================================*/
/*!
  @name Main Control helper functions

  @brief
  This section holds all helper functions used by 1x MC layer
*/
/*! @{ */
/*----------------------------------------------------------------------------*/
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
 
  @param is_retune
  Boolean flag to check if the API is being called for wakeup_rx or for retune
 
  @param is_div_device
  Boolean flag to check if the API is being called for enable_diversity
 
  @return success
  TRUE: Everything went well. Call successful.
  FALSE: Some error. went well. Call unsuccessful.
*/ 
boolean 
rf_1x_mc_send_start_rx_helper 
( 
   const rfm_device_enum_type device ,
   const rf_cdma_data_status_type* dev_status_r ,
   const lm_handle_type rxlm_handle  ,
   const rfc_cdma_logical_device_params_type* logical_dev ,
   boolean is_irat_mode,
   boolean is_retune,
   boolean is_div_device
)
{
  rf_time_type enable_FW_Resp_time; /* TIme spent in function */
  rf_time_tick_type enable_FW_Resp_tick; /* TIme spent in function */
  rf_cdma_mdsp_response_cb_param_type rsp_cb_param;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;
  boolean success = FALSE ;

   /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_mc_send_start_rx_helper: NULL FWRSP Data" );
  } /* if ( fwrsp_data == NULL ) */
  else
  {
    rf_cdma_mdsp_rx_start_type rx_start_c; /* RxAGC Start Config */
    rx_start_c.script_index = (uint32)RFLM_CDMA_DUMMY_EVENT;
    rx_start_c.meas_reset_script_id = (uint32)RFLM_CDMA_DUMMY_EVENT;

    if ( TRUE == is_irat_mode)
    {
      if ( is_div_device == FALSE )
      {        
        rx_start_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_RF_IRAT_TUNE_IN_ID ;
        rx_start_c.meas_reset_script_id = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_SLEEP_RX_ID;
      }
      else
      {
        rx_start_c.script_index = (uint32)RFLM_CDMA_DUMMY_EVENT; 
        rx_start_c.meas_reset_script_id = (uint32)RFLM_CDMA_DUMMY_EVENT;
      }
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

    /* Start Rx */
    success = 
      rf_1x_mdsp_start_rx_config( logical_dev->rx_mdsp_path, &rx_start_c , 
                                  NULL, NULL, NULL );
    if ( FALSE == success )
    {
      RF_MSG ( RF_ERROR, "rf_1x_mc_send_start_rx_helper(): " 
               "rf_1x_mdsp_start_rx_config failed " ) ;
    }
    else
    { /* In iRat we dont send rx_start so skip it */
      if ( is_irat_mode == TRUE )
      {
        RF_MSG ( RF_HIGH ,  "rf_1x_mc_send_start_rx_helper(): " 
               "device_in_irat == TRUE. Skipping Rx Start " ) ;
      }
      else
      {
      	enable_FW_Resp_tick = rf_time_get_tick();
        fwrsp_hk->rx_start_rsp.device = device ;
        fwrsp_hk->rx_start_rsp.sema_token = rfcommon_semaphore_pop_item();
        rsp_cb_param.rsp_handler = rf_1x_rx_start_rsp_cb;
        rsp_cb_param.handler_data = &fwrsp_hk->rx_start_rsp;

        success = 
          rf_1x_mdsp_start_rx( logical_dev->rx_mdsp_path, &rsp_cb_param,
                              NULL , NULL );
        if ( FALSE == success )
        {
          RF_MSG ( RF_ERROR, "rf_1x_mc_send_start_rx_helper(): " 
                   "rf_1x_mdsp_start_rx failed " ) ;
          /* Message sending failed, free the semaphore */
          RF_MSG_2 ( RF_LOW, "rf_1x_mc_send_start_rx_helper():"
                     " Freeing the semaphore and not waiting "   
                     " [D%d] RX_START pushed back : Sem @ 0x%x" , 
                     device , fwrsp_hk->rx_start_rsp.sema_token ) ;
        }
        else
        {
          RF_MSG_2( RF_LOW, "[1X][RF-FW Sync] [D%d] RX_START : Start Wait Sem @ "
                            "0x%x", device , fwrsp_hk->rx_start_rsp.sema_token );
          rfcommon_semaphore_wait( fwrsp_hk->rx_start_rsp.sema_token );


        }/*if ( FALSE != success )*/

        enable_FW_Resp_time = rf_time_get_elapsed( enable_FW_Resp_tick, RF_USEC );
         #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
           prof_hist_update(&rx_1x_wakeup_fw_resp_hist,(uint32)enable_FW_Resp_time);
           if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
           {
             rf_1x_clear_profiling_hist_packet();
           }
         #endif
        /* Push back semaphore after wait if mssg was sent successfully,
        or immediately if mssg sending failed*/
        rfcommon_semaphore_push_item( fwrsp_hk->rx_start_rsp.sema_token );

        if ( success == TRUE )
        {
          /* Wait for RxAGC to converge ... this is the right place to wait for 
             agc to converge. This ensures that when rfm_exec_wakeup_rx()
             completes L1 is assured of a converged AGC */
          success = rf_1x_mc_rx_agc_convergence( device ) ;

        }
        else
        {
          RF_MSG ( RF_MED, "rf_1x_mc_send_start_rx_helper(): " 
                   "not waiting for RxAGC convergence since Rx start failed" ) ;
        }
      } /* If not doing irat */
    }/*if ( FALSE != success )*/
  }/*if ( fwrsp_hk != NULL )*/
  return success ;
}/* rf_1x_mc_send_start_rx_helper*/

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
 
  @param rtc_cx1_trigger
  RTC time for Rx Stop
 
  @param skip_rf_front_end_programming
  Boolean to let the function know if RF front end programming
  needs to be skipped for cases like retune. This flag is also used to 
  determine if RxAGC operations need to be performed. In case of retune, 
  this flag is set - so front-end programming and 
  rf_cdma_mc_configure_stop_rx_agc_data are to be skipped
 
  @param is_offline_qpch
  Boolean to let FW know that this is timed sleep.
 
  @return success
  TRUE: Everything went well. Call successful. 
  FALSE: Some error. Call unsuccessful.
*/
boolean
rf_1x_mc_send_stop_rx_helper 
(  
  const  rfm_device_enum_type device ,
  const  rf_cdma_data_status_type* dev_status_r ,
  const  lm_handle_type rxlm_handle ,
  const  rfc_cdma_logical_device_params_type* logical_dev ,
  uint32 rtc_cx1_trigger ,
  boolean skip_rf_front_end_programming ,
  boolean is_offline_qpch,
  boolean is_slave_device 
)
{
  rf_time_tick_type prof_sleep_FW_resp_time;		/* time spent in API */
  rf_time_type prof_sleep_FW_resp_time_val;
  boolean success = FALSE ;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;

  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device ) ;

    /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_mc_send_stop_rx_helper: NULL FWRSP Data" );
  } /* if ( fwrsp_data == NULL ) */
  else
  {
    rf_cdma_mdsp_response_cb_param_type rsp_cb_param ;
    rf_cdma_mdsp_rx_stop_type rx_stop_data ; /* Rx Stop Data */
   /* The script index of -1 suggests that FW will 
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
    prof_sleep_FW_resp_time = rf_time_get_tick();
    rx_stop_data.rxlm_handle = rxlm_handle; /* RXLM Handle */
    /* PLL Settling time (Cx1) */

    rx_stop_data.rtc_cx1_trigger = rtc_cx1_trigger ; 
    /* RTC time for Rx Stop */

    /* Stop Rx for primary device */
    fwrsp_hk->rx_stop_rsp.device = device;
    fwrsp_hk->rx_stop_rsp.sema_token = rfcommon_semaphore_pop_item();
    fwrsp_hk->rx_stop_rsp.callback = NULL;
    fwrsp_hk->rx_stop_rsp.userdata = NULL;
    fwrsp_hk->rx_stop_rsp.blocking_call = TRUE;
    fwrsp_hk->rx_stop_rsp.timed_sleep_success = FALSE;
    rsp_cb_param.rsp_handler = rf_1x_rx_stop_rsp_cb;
    rsp_cb_param.handler_data = &fwrsp_hk->rx_stop_rsp;

    success = rf_1x_mdsp_stop_rx( logical_dev->rx_mdsp_path, &rx_stop_data,
                                  is_offline_qpch , &rsp_cb_param, NULL, NULL );

    /* Check if message sending was success or failure */
    if ( FALSE == success )
    {
      RF_MSG ( RF_ERROR, "rf_1x_mc_send_stop_rx_helper(): " 
               "rf_1x_mdsp_stop_rx failed " ) ;
      /* Message sending failed, free the semaphore */
      RF_MSG_2 ( RF_LOW, "rf_1x_mc_send_stop_rx_helper():"
                 " Freeing the semaphore and not waiting "   
                 " [D%d] RX_STOP pushed back : Sem @ 0x%x" , 
                 device , fwrsp_hk->rx_stop_rsp.sema_token ) ;
    }
    else
    {
      /* Message was sent to FW, wait for semaphore */
      RF_MSG_2( RF_LOW, "[1X][RF-FW Sync] [D%d] RX_STOP : Start Wait Sem @ "
                        "0x%x", device, fwrsp_hk->rx_stop_rsp.sema_token );
      rfcommon_semaphore_wait( fwrsp_hk->rx_stop_rsp.sema_token );
    }

    prof_sleep_FW_resp_time_val = rf_time_get_elapsed( prof_sleep_FW_resp_time, RF_USEC );
    #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
       prof_hist_update(&rx_1x_sleep_fw_resp_hist,(uint32) prof_sleep_FW_resp_time_val);
       if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
       {
         rf_1x_clear_profiling_hist_packet();
       }
     #endif
    /* Push back semaphore after wait if mssg was sent successfully,
    or immediately if mssg sending failed*/
    rfcommon_semaphore_push_item( fwrsp_hk->rx_stop_rsp.sema_token );

    if (( is_offline_qpch == TRUE ) && ( fwrsp_hk->rx_stop_rsp.timed_sleep_success == FALSE ))
    {
      /* Notify that Timed sleep failed */
      RF_MSG( RF_ERROR, "rf_1x_mc_send_stop_rx_helper: Timed sleep Failed!!");
    }

    /* If Rx stop was succesful, clear the cmn rx agc data only in following 
    scenarios:
    1.  Skip_rf_front_end_programming is FALSE, meaning we actually disabled the 
    front end
    */ 
    if ( skip_rf_front_end_programming == FALSE )
    {
       /* 2. If QPCH is NOT offline OR Timed sleep on FW side was a success 
             and if QPCH is offline */  
       /* Note: If Timed sleep failed, L1 will call regular sleep*/
       /* Based on current implementation while calling Timed sleep, L1 is 
          expected to set QPCH to offline */
      if (( is_offline_qpch == FALSE ) || 
          ( ( fwrsp_hk->rx_stop_rsp.timed_sleep_success == TRUE ) && ( is_offline_qpch == TRUE )))
      {
        rf_cdma_mc_configure_stop_rx_agc_data( device, rxlm_handle );
      }
    }
  }/* if ( fwrsp_hk != NULL )*/
  return success ;
}/* rf_1x_mc_send_stop_rx_helper */

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
  Perform 1x-specific initializations

  @details
  This function will not touch any hardware related functionality. It is
  intended to configure the differrent modules (like MDSP) to the correct mode.
   
  @return
  Flag indicating a success (TRUE) or failure (FALSE)
*/
boolean
rf_1x_mc_init
(
  void
)
{
  boolean init_status = TRUE; /* 1x MDSP Init Statu */

  /* Initialize mdsp for 1x - this is a one-time init*/
  init_status &= rf_1x_mdsp_init();

  /* Perform RF Responses registration */
  init_status &= rf_1x_fw_response_registration();
  
  /* Register IRAT functions */
  init_status &= rf_1x_mc_meas_init();
  
  /* Set the FBRx enable flag based on NV */ 
  rf_cdma_data_set_fbrx_enable( RFM_1X_MODE);

  return init_status;

} /* rf_1x_mc_init */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform 1x-specific deinitializations

  @return
  Flag indicating a success (TRUE) or failure (FALSE)
*/
boolean
rf_1x_mc_deinit
(
  void
)
{

  boolean deinit_status = TRUE; /* Variable to track if CDMA Init passed */
  uint8 dev_idx;
  rf_hal_bus_client_handle* handle;
  const rf_1x_config_type *rf_1x_config_ptr; /* Pointer to 1x RF NV data */
  const rfc_cdma_logical_device_params_type *logical_dev;
  uint8 band_idx;

  for (dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_DEVICES; dev_idx++ )
  {
    /* Get Read-only Device Status */
    const rf_cdma_data_status_type *dev_status_r = 
                     rf_cdma_get_device_status( (rfm_device_enum_type)dev_idx );

    if ( (NULL != dev_status_r) )
    {
      /*-------- Disable HAL Bus ---------------------------------------------*/
      handle = rf_cdma_data_get_rf_hal_bus_vote_handle(dev_idx);

      if ( handle != NULL )
      {
        if ( dev_status_r->rf_mode == RFM_1X_MODE )
        {
          #ifndef FEATURE_RFA_ATLAS_MODEM
          rf_hal_bus_enable (FALSE, &handle, RF_HAL_RFCMD_APP_CLIENT(1X));
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
          rf_1x_config_ptr = rf_cdma_nv_get_1x_data( 
                                              logical_dev->tx_nv_path[band_idx],
                                              band_idx );
          if ( rf_1x_config_ptr != NULL )
          {
            rfcommon_core_dealloc_tx_band_cal( 
               &(rf_1x_config_ptr->cdma_common_ptr->tx_cal.tx_multi_lin_cal) );
          } /* if ( rf_1x_config_ptr != NULL ) */
        } /* for ( band_idx = RFM_CDMA_BC0; band_idx < RFM_CDMA_MAX_BAND; 
                   band_idx++ ) */
      } /*if ( logical_dev != NULL ) */
    } /* if ( (NULL != dev_status_w) ) */
  } /* for (dev_idx = RFM_DEVICE_0; dev_idx < RFM_MAX_DEVICES; dev_idx++ ) */

  deinit_status &= rf_1x_fw_response_deregistration();

  return deinit_status;
} /* rf_1x_mc_deinit */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Prepare radio to operate in CDMA 1x mode

  @details
  This function is used to enter 1x. This function will not touch any hardware 
  related functionality. This will only be initializing the software stack.
 
  @param device
  Device on which 1x subsystem is to be entered

  @param user_data_ptr
  Pointer to user data passed into callback
  
  @param cb_handler
  Callback handler
  
  @return
  Flag indicating whether 1x Enter is successful (TRUE) or failed (FALSE)
*/
boolean
rf_1x_mc_enter_mode
(
  rfm_device_enum_type device,
  void* const user_data_ptr,
  rfm_cb_handler_type cb_handler
)
{
  boolean ret_val = TRUE; /* Return value */
  rf_time_type enter_mode_time; 
  rf_time_tick_type enter_mode_tick;

  /* Start Profiling of Rx Warm Up */
  enter_mode_tick = rf_time_get_tick();

  /* this is the first API to be called in the tune sequence for 1x - so init 
     the RF WU vars for this sequence */ 
  rf_cdma_init_wup_time( device, RFM_1X_MODE, enter_mode_tick );

  if (device >= RFM_MAX_DEVICES)
  {
    RF_MSG_1( RF_ERROR,
          "rf_1x_mc_enter_mode: Invalid device passed (device=%d)",device);
    ret_val = FALSE; /* to indicate failure */
  }

  /* check if device data structures are initialized */
  if (rf_cdma_is_device_initialized(device) == FALSE)
  {
    RF_MSG( RF_ERROR,
            "rf_1x_mc_enter_mode: Device data structure not initialized - "
            "bailing out"); 
    ret_val = FALSE; 
  }

  /* 1) Continue only if there were no error 
     2) check if current mode is 1X - if yes, then no operations need to be
        done
  */
  if ( ret_val == TRUE )
  {
    /* perform enter mode operations only if device is NOT in 1x mode */
    if ( rf_cdma_data_get_rf_mode (device) != RFM_1X_MODE )
    {
      /* Update RF Mode */
      rf_cdma_data_set_rf_mode( device, RFM_1X_MODE );
    }
  } /* if ( ret_val == TRUE ) */

  if (rfcommon_autopin_is_enabled(RFM_1X_MODE))
  {
    if(RFCOMMON_AUTOPIN_ERROR == rfcommon_autopin_mc_enter_mode(RFM_1X_MODE))
    { 
      RF_MSG(RF_ERROR, "rf_1x_mc_enter_mode: CDMA 1X AutoPin enter mode failed!");
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
  rf_cdma_update_wup_time( device, enter_mode_time );

  /* no operations need to be done in enter_mode for 1x */
  return ret_val;

} /* rf_1x_mc_enter_mode */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Execute steps needed to exit the mode the driver is currently configured to 
  operate in.

  @details
  This function tears down 1x Mode.
 
  @param device
  Device on which 1x is to be torn down
 
  @param to_mode
  New Mode to which RF is transitioning to
 
  @return
  Status of 1x Exit Procedure
*/
int32
rf_1x_mc_exit_mode
(
  rfm_device_enum_type device,
  rfm_mode_enum_type to_mode
)
{
  boolean exec_status = TRUE;
  int32 ret_val = 0;
  rf_time_tick_type exit_mode_time;          /* Time spent in API */
  rf_time_type exit_mode_time_val;
  rf_cdma_state_type last_rf_state;

  const rf_cdma_data_status_type *dev_status_r; /* Device Status read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */

  rf_cdma_ic_data_type *ic_data_w; /* Intelliceiver write pointer*/

  /* Start Profiling of Rx exit time */
  exit_mode_time = rf_time_get_tick();

  /* Get Device Data */
  dev_status_r = rf_cdma_get_device_status( device );

  /* NULL Pointer Check */
  if ( dev_status_r == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_exit_mode: Invalid device %d Data", device);
    exec_status &= FALSE; /* to indicate failure */
  }

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Get CDMA write data pointer */
  ic_data_w = rf_cdma_get_mutable_intelliceiver( device );

  /* NULL Pointer Check */
  if ( logical_dev == NULL || ic_data_w == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_exit_mode: Invalid device %d Logical Param", 
              device );
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
      rf_cdma_ic_stop( ic_data_w, logical_dev );
    }
  
    /* Tear down radio, if needed. Flag this case with error message as
    Radio should have been in SLEEP state before doing exit. */
    if ( dev_status_r->data_1x.rf_state != RF_CDMA_STATE_SLEEP )
    {
      last_rf_state = dev_status_r->data_1x.rf_state;

      rf_1x_mc_tear_down(device);

      RF_MSG_3( RF_ERROR, "rf_1x_mc_exit_mode: ****** Dev %d Tear down "
                "required [State %d -> %d] *****", device, 
                last_rf_state, dev_status_r->data_1x.rf_state ); 
    } /* if ( dev_status_r->rf_state != RF_CDMA_STATE_SLEEP ) */

    /* Update RF Mode */
    rf_cdma_data_set_rf_mode(device,RFM_PARKED_MODE);
  
    ret_val = 1;

    if (rfcommon_autopin_is_enabled(RFM_1X_MODE))
    {
      if(RFCOMMON_AUTOPIN_ERROR == rfcommon_autopin_mc_exit_mode(RFM_1X_MODE))
      {
        RF_MSG(RF_ERROR, "rf_1x_mc_exit_mode: CDMA 1X AutoPin exit mode failed!");
        ret_val = 0;
      }
    }
  }
  else
  {
    ret_val = 0;
  }
  
  /* Record the execution time statistics for exiting mode for this device*/
  exit_mode_time_val = rf_time_get_elapsed( exit_mode_time, RF_USEC );

  /* Print time elapsed for exit mode */
  RF_MSG_1( RF_HIGH, "rf_1x_mc_exit_mode: Time elapsed for exit mode %d", 
            exit_mode_time_val);

  return ret_val; /* to indicate success */
} /* rf_1x_mc_exit_mode */

/*! @} */

/*============================================================================*/
/*!
  @name CDMA 1x subsystem TRM Control Functions

  @brief
  Functions for TRM to enable and disable the 1x portion of the
  CDMA RF driver.
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enable the 1x subsystem of the CDMA RF driver.

  @details
  Prepares the radio for 1x operation on an initial band and channel.
  The 1x subsystem is initialized in the Sleep state with the requested
  band and channel.

  By initialzing 1x with a band and a channel, this avoids the situation
  where "the driver is on but there is no known channel."  This also
  allows SVDO to be implemented correctly when HDR is in traffic and 1x
  needs to transition to the primary chain in preparation for CDMA
  operation.  By forcing 1x to be enabled with a channel, the driver
  can detect that wideband Rx mode is correct, and tune radio to the
  correct initial frequency.

  If HDR is already active and on an incompatible channel, this API
  call will return an error.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_ENABLE_COMPLETE event.  The function will attempt to give
  a best estimate of the time remaining in the return value.

  @param device
  The path to enable the 1x subsystem on.

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @param band
  RF CDMA band on which 1x radio is to be enabled.

  @param chan
  RF CDMA channel on which 1x radio is to be enabled.

  @return
  The anticipated time in microseconds remaining from the rfm_1x_enable()
  function return until the operation is complete.

  @todo
  Work out what the error return values are for this function.
*/
rfm_wait_time_t
rf_1x_mc_enable
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const rfm_cdma_band_class_type band,
  const rfm_cdma_chan_type chan
)
{
  rfm_wait_time_t ret_val = 0; /* Return value */

  RF_MSG_4( RF_MED, "rf_1x_mc_enable : Dev %d, RxLM Handle %d, Band %d, "
            "Chan %d", device, rxlm_handle, band, chan  );

  return ret_val;

} /* rf_1x_mc_enable */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Disable the 1x subsystem of the CDMA RF driver.

  @details
  Deactivates the 1x portion of the CDMA RF driver.  This API should
  only be called if 1x is in the Sleep state.  If the radio was
  operating in wideband mode, it is reconfigured for narrowband mode.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_DISABLE_COMPLETE event.  The function will attempt to give
  a best estimate of the time remaining in the return value.

  @param device
  The path to disable the 1x subsystem on.

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @return
  The anticipated time in microseconds remaining from the rfm_1x_disable()
  function return until the operation is complete.

  @todo
  Work out what the error return values are for this function.
*/
rfm_wait_time_t
rf_1x_mc_disable
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle
)
{
  rfm_wait_time_t ret_val = 0; /* Return Value */

  /* baseband and RF recos are initialized to their defaults */
  rf_cdma_init_freq_info_for_device(device);

  rf_cdma_data_set_assoc_dev( device, RFM_INVALID_DEVICE );

  RF_MSG_2( RF_MED, "rf_1x_mc_disable : Dev %d, RxLM Handle %d",
             device, rxlm_handle  );

  return ret_val;
} /* rf_1x_mc_disable */

/*! @} */

/*============================================================================*/
/*!
  @name 1x L1 Radio Control Functions

  @brief
  Functions for the 1x protocol stack to control the radio
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform all steps needed to retune a given device to a new band and/or channel

  @details
  This is a helper function for rf_1x_mc_retune() - all steps except for power
  collpase is done in this function.

  @param device
  The radio path to re-tune

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @param new_band
  RF CDMA band to which 1x radio is to be retuned.
 
  @param new_chan
  RF CDMA channel to which 1x radio is to be retuned.
 
  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @return
  The anticipated time in microseconds remaining from the rfm_1x_tune()
  function return until the operation is complete, or an error code.
  If the API call succeeds the return value will be non-negative; all
  error codes will be negative.
*/
static rfm_wait_time_t
rf_1x_mc_helper_for_tx_retune
(
  const rfm_device_enum_type rx_device,
  const rfm_device_enum_type tx_device,
  const rfm_cdma_band_class_type new_band,
  const rfm_cdma_chan_type new_chan,
  boolean is_start_tx
)
{
  rfm_wait_time_t ret_val;
  boolean retune_helper_success = TRUE;
  const rf_cdma_data_status_type *tx_dev_status_r;/* Dev Status Read Ptr*/  
  const rf_cdma_data_status_type* rx_dev_status_r;/* Dev Status Read Ptr*/  
  const rfc_cdma_logical_device_params_type *tx_logical_dev; /* Logical Device */
  rf_cdma_temp_comp_data_type *temp_comp_w; /* Temp comp write pointer*/
  rf_cdma_hdet_data_type *hdet_w; /* HDET write pointer*/
  rf_cdma_plim_data_type *plim_w; /* PLIM write pointer*/
  const rf_cdma_tx_static_type *tx_static_nv_cal_ptr = NULL; /*CDMA static NV pointer*/  
  const rf_1x_config_type *rf_1x_config_ptr;   /* Pointer to 1x RF NV data */
  rfdevice_cdma_therm_read_type therm_read;
  rfdevice_cdma_dac_cal_iref_type iref_data;
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/

  rfcommon_fbrx_tx_param_type fbrx_tx_params;
  uint8 fbrx_modem_chain = 3; /* WB3 is the default FBRX Chain */

  rx_dev_status_r = rf_cdma_get_device_status( rx_device );
         
  tx_dev_status_r = rf_cdma_get_device_status( tx_device ); /* Get Device Status Read Pointer*/    
  tx_logical_dev = rfc_cdma_get_logical_device_params( tx_device );/* Get Logical Device Param */
  temp_comp_w = rf_cdma_get_mutable_temp_comp( tx_device );
  hdet_w = rf_cdma_get_mutable_hdet( tx_device );
  plim_w = rf_cdma_get_mutable_plim( tx_device );
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( tx_device );
  boolean pin_ret;

  /* Perform NULL Pointer check */
  if ( ( tx_logical_dev == NULL ) || ( temp_comp_w == NULL ) || 
       ( rx_dev_status_r == NULL) || ( hdet_w == NULL ) || 
       ( plim_w == NULL ) || ( ant_tuner_token_w == NULL) ||
       (tx_dev_status_r == NULL ) )
       
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_tx_retune(): NULL data for tx device %d",
              tx_device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  if ( tx_dev_status_r->data_1x.rf_state != RF_CDMA_STATE_RXTX &&
       tx_dev_status_r->data_1x.rf_state != RF_CDMA_STATE_TX )
  {
    RF_MSG_1( RF_LOW, "rf_1x_mc_helper_for_tx_retune(): "
                        "Tx device %d not in Tx state",
                        tx_device );
    return RFM_CDMA_HEALTHY_STATE;
  }

  if ( tx_logical_dev != NULL )
  {

    /* ensure that NV is valid for given device and band */
    rf_1x_config_ptr = rf_cdma_nv_get_1x_data( tx_logical_dev->tx_nv_path[new_band], 
                                               new_band );

    if ( NULL == rf_1x_config_ptr )
    {
      RF_MSG_1 (RF_ERROR, "rf_1x_mc_helper_for_tx_retune: Failed to retrieve valid"
                          " Config data from NV for band %d", new_band ); 
      return RFM_CDMA_ERROR_BAD_CONFIG;
    }

    if ( tx_logical_dev->tx_nv_path[new_band] >= RF_CDMA_NV_PATH_NUM )
    {
        RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_tx_retune(): Invalid tx_nv_path %d",
                  tx_logical_dev->tx_nv_path[new_band] );
      return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
    }

    tx_static_nv_cal_ptr = rf_cdma_nv_get_tx_static( 
                                              tx_logical_dev->tx_nv_path[new_band], 
                                              new_band );
    if ( tx_static_nv_cal_ptr == NULL )
    {
      RF_MSG_1 ( RF_ERROR, "rf_1x_mc_helper_for_tx_retune: "
                       "Failed to get valid static TX NV pointer "
                       "for device %d", tx_device);
      return RFM_CDMA_ERROR_BAD_CONFIG;
    }
  }

  if ( is_start_tx == FALSE )
  {
    if ( rfm_get_calibration_state() == FALSE ) 
    {
      if( tx_dev_status_r->ept_online_enabled == TRUE )
      {
        retune_helper_success &= rf_common_xpt_init_dpd( tx_device, NULL );
      }
    }

    /* Stop temp comp */
    if ( rf_1x_temp_comp_stop( temp_comp_w ) == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_tx_retune: Failed to stop temp comp" 
                "for retune on device %d", tx_device );
    }

    /* Stop PLIM and HDET */
    if ( rf_1x_plim_stop( tx_device, plim_w ) == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_tx_retune: Failed to stop PLIM for"
                          "retune on device %d", tx_device );
    }

    if ( rf_cdma_hdet_stop( tx_device, hdet_w ) == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_tx_retune: Failed to stop HDET for"
                          "retune on device %d", tx_device );
    }
     
     if ( rfcommon_autopin_is_enabled(RFM_1X_MODE) == TRUE )
     {
        if (rf_cdma_autopin_stop( tx_device ) == FALSE)
        {
  	    RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_tx_retune: Failed to stop AutoPin for"
							" device %d", tx_device );
        }
     }
    /* Since Temp Comp and HDET have been disabled, force isHKADCbusy to
    FALSE */
    rf_cdma_data_set_isHKADCbusy( tx_device, FALSE );

    /* Since 1x can retune with TX enabled, need to first stop TxAGC updates
       by FW, before any changes are done to the shared memory or rf_on_off
       scripts.
       No need to do this is cal mode, since FW is in override mode during cal
    */
    rf_1x_mdsp_stop_txagc_update(TRUE);

    rfdevice_cdma_tx_enter_critical_section ( 
                        tx_logical_dev->tx_device[tx_dev_status_r->curr_band] );

    retune_helper_success &= rf_cdma_mc_configure_front_end_for_tx_sleep(
                                   tx_device, 
                                   tx_dev_status_r->curr_band,
                                   tx_dev_status_r->script_index, 
                                   tx_logical_dev,
                                   ant_tuner_token_w,
                                   TRUE /* immediate_write */ );

    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      /* Sleep the WTR for tx mode */
      retune_helper_success &= 
                      rfdevice_cdma_tx_pwr_sleep(tx_logical_dev->tx_device
                                                [tx_dev_status_r->curr_band]);
    }
    rfdevice_cdma_tx_leave_critical_section( 
                         tx_logical_dev->tx_device[tx_dev_status_r->curr_band] );
    
  }/* if ( is_start_tx == FALSE ) */
  else
  {    
    rf_cdma_msm_static_txlm_cfg_type static_txlm_cfg;
    rf_cdma_msm_dynamic_txlm_cfg_type dynamic_cfg;
    uint32 tx_bw_khz = rf_cdma_compute_bandwidth( 1, new_band, NULL );

    rfcommon_fbrx_error_type fbrx_status = RFCOMMON_FBRX_SUCCESS;
    lm_handle_type fbrx_lm_handle = 0xFFFFFFFF; /* fbrx lm handle */
    uint32 channels[RFM_CDMA_CARRIER_CUMULATIVE];
    rfcom_band_type_u fbrx_band;

    rf_cdma_data_set_tx_bw_khz( tx_device, tx_bw_khz );   
    rf_cdma_data_set_curr_band( tx_device, new_band );
    rf_cdma_data_set_curr_chan( tx_device , RFM_CDMA_CARRIER_0, new_chan );
    rf_cdma_data_set_carrier_index( tx_device , RFM_CDMA_CARRIER_0 , 0 );
    rf_cdma_data_set_num_carriers( tx_device , 1 );    
    rf_cdma_data_set_lo_tuned_to_chan( tx_device , rx_dev_status_r->rf_rx_reco.rf.lo_chan );

    rfdevice_cdma_tx_enter_critical_section ( tx_logical_dev->tx_device[new_band] );

    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {

      /* Wakeup the WTR for tx mode */
      retune_helper_success = rfdevice_cdma_tx_pwr_wakeup(
                                          tx_logical_dev->tx_device[new_band]);
    }

    retune_helper_success &= rf_cdma_mc_configure_front_end_for_tx_wakeup(
                               tx_device, 
                               rf_cdma_data_get_txlm_handle (tx_device), 
                               RFM_1X_MODE, new_band, new_chan, 
                               rx_dev_status_r->rx_bw_khz, 
                               RF_CDMA_BW_1X, 
                               tx_dev_status_r->script_index, 
                               tx_logical_dev,
                               tx_static_nv_cal_ptr,
                               ant_tuner_token_w,
                               TRUE /* immediate_write */ );

    rfdevice_cdma_tx_leave_critical_section( tx_logical_dev->tx_device[new_band] );

    if ( rf_cdma_data_get_fbrx_enable (RFM_1X_MODE) == TRUE )
    {
      /* Get the FBRx lm handle */
      fbrx_lm_handle = rf_cdma_data_get_fbrx_lm_handle(tx_device);

      /* Updates FBRx frequency-dependent values */    
      /* @todo: remove hard-coding, tech to update the fbrx chain from rfc */ 
      channels[0] = (uint32)new_chan;
      fbrx_band.cdma_band = new_band; 
      fbrx_tx_params.apt_only_mode = FALSE; 		
      fbrx_modem_chain = rfc_cdma_get_fbrx_modem_chain(tx_device, new_band);
      fbrx_status = rfcommon_fbrx_mc_tune_to_chan(
                                    tx_device, 
                                    tx_dev_status_r->txlm_handle, 
                                    RFM_1X_MODE, 
                                    fbrx_band,
                                    channels,
                                    RFCOMMON_FBRX_1_CARRIER,
                                    fbrx_lm_handle,
                                    fbrx_modem_chain,
                                    fbrx_tx_params,
                                    RFCOMMON_FBRX_NV_CONTAINER_C0 );
    }

	/*Set Pout Skip to FALSE*/      
	rf_cdma_set_pout_comp_skip ( tx_device , FALSE ) ;
    /*----------------------------------------------------------------------*/
    /* Do SW therm read */
    /*----------------------------------------------------------------------*/
    rfdevice_cdma_read_therm( tx_logical_dev->tx_device[new_band],
                              FALSE,
                              &therm_read );

    temp_comp_w->last_temp_sensor_read = therm_read.therm_reading;
    temp_comp_w->is_last_reading_valid = therm_read.therm_quality;

    RF_MSG_2( RF_LOW, "rf_1x_mc_helper_for_tx_retune: therm reading: %d, and %d",
              therm_read.therm_reading, therm_read.therm_quality );

     /* Tx spectral inverstion feature enabled for MB\HB, in-order to inform the 
      MSM about this, we need to update the txr iq dac group settings otherwise our tx
      spectrum will be inverted and the data will not be decoded properly by BS.
      Example Sceario: If we make call on LB (BC0) and retune to HB\MB (BC1 or BC6)
      then we need to inform the MSM about the spectral inverstion feature for the new bands*/
  
      retune_helper_success &= 
      rf_cdma_mc_trigger_dyn_txr_iq_dac_update(tx_device,
  	                                        new_band );

      /* If  TxR_IQ_DAC dynamic update fails, then HO will fail in Tx mode
      so check the status and return immediately with Error flag*/
      if ( retune_helper_success == FALSE )
      {
        RF_MSG_2( RF_ERROR, "rf_1x_mc_helper_for_retune:  TxR_IQ_DAC dynamic update" 
		            " failed for device %d band %d",
                            tx_device, new_band );
      }	 

    retune_helper_success &= rf_1x_configure_tx_agc( tx_device, new_band, 
                                                     new_chan,
                                                     tx_dev_status_r->txlm_handle,
                                                     rf_1x_config_ptr );

    /* Reset the Tx AGC state machine in order to retune without turning 
    Tx OFF. This is explicitly required for 1x, because retune may involve 
    transitioning between bands with different Tx AGC params such as xPT 
    mode, PA state configuration etc. */
    rflm_txagc_reset_txagc_stm( tx_dev_status_r->txlm_handle, TRUE );

    /* indicate to FW that it can now start using the updated SM, and run 
       txAGC in free-running mode - this needs to be done only AFTER sending 
       configure_tx_agc command to FW */
    retune_helper_success &= rf_1x_mdsp_stop_txagc_update(FALSE);

    if ( rfm_get_calibration_state() == FALSE )
    {
      static_txlm_cfg.txlm_dac = tx_logical_dev->txlm_dac;
      static_txlm_cfg.xpt = rf_cdma_xpt_get_operation_mode( tx_device );

      iref_data.band = new_band;
      (void) rfdevice_cdma_tx_get_dac_cal_iref ( 
         tx_logical_dev->tx_device[new_band], &iref_data ) ;
      dynamic_cfg.iref_val = iref_data.iref_val;
      dynamic_cfg.num_carriers = tx_dev_status_r->num_carriers;
      dynamic_cfg.tx_modem_chain = rfc_cdma_get_tx_modem_chain( tx_device, new_band );

      if ( ( static_txlm_cfg.xpt == EPT_CFG ) ||
           ( static_txlm_cfg.xpt == ET_CFG )  ||
           ( static_txlm_cfg.xpt == EPT_ET_CFG ) )
      {
        /* bring the device up to ET mode */
        retune_helper_success &= rf_1x_xpt_configure( 
                                    tx_device,
                                    new_band,
                                    new_chan,
                                    tx_logical_dev->tx_nv_path[new_band],
                                    tx_dev_status_r->txlm_handle, 
                                    static_txlm_cfg.xpt );
      }
    }
    /*----------------------------------------------------------------------*/
    /* Start 1x temp comp module */
    /*----------------------------------------------------------------------*/
    /* Start temp comp */
    if ( rf_1x_temp_comp_start( temp_comp_w,
                                RFM_1X_MODE,
                                tx_logical_dev,
                                new_band,
                                new_chan) == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_tx_retune: Failed to start "
                          "temp comp for retune on device %d", tx_device ); 
    }

    /*----------------------------------------------------------------------*/
    /* Start 1x Power limiting and HDET */
    /*----------------------------------------------------------------------*/
    if ( rf_1x_plim_start(tx_device,
                          tx_logical_dev,
                          plim_w,
                          new_band) == TRUE )
    {
      /* Start HDET only if Power Limiting was started successfully. Else no 
      point doing HDET reads */
      if ( rf_cdma_hdet_start(tx_device, hdet_w,
                            RFM_1X_MODE,
                            tx_logical_dev,
                            new_band,
                            new_chan) == FALSE )
      {
        /* Clean up HDET trig event in case of hdet start failure */
        rf_cdma_mc_cleanup_ccs_event( tx_device, RF_CDMA_EVENT_HDET_CONFIG );

        RF_MSG_3( RF_ERROR, "rf_1x_mc_helper_for_tx_retune: HDET start failed on "
                            "device %d, band: %d, chan: %d", 
                  tx_device, new_band, new_chan );
      }

      /* Update lim_vs_temp_vs_freq */
      rf_1x_plim_update_lim_vs_temp_vs_freq( 
                            tx_device,
                            plim_w,
                            tx_dev_status_r->plim.band,
                            tx_dev_status_r->lo_tuned_to_chan,
                            tx_dev_status_r->temp_comp.last_temp_sensor_read );
    }
    else
    {
      RF_MSG_2( RF_HIGH, "rf_1x_mc_helper_for_tx_retune: 1x PLIM not started on "
                "device %d, band: %d", tx_device, new_band );
    }

    /*--------------------- FBRx Settings ----------------------------------*/
    if ( rf_cdma_data_get_fbrx_enable (RFM_1X_MODE) == TRUE )
    {
      /* Writes the FBRx modem settings to HW if tx bw changes */
      if ( tx_bw_khz != tx_dev_status_r->tx_bw_khz )
      {
        /* Set the pout comp flag that FBRx is enabled. */
        rf_cdma_set_pout_comp_skip ( tx_device , TRUE ) ;

	fbrx_modem_chain = rfc_cdma_get_fbrx_modem_chain(tx_device, new_band);
        fbrx_status &= rfcommon_fbrx_enable( fbrx_lm_handle, fbrx_modem_chain );
        if( fbrx_status == RFCOMMON_FBRX_ERROR )
        {
          RF_MSG_1(RF_FATAL, "rf_1x_mc_helper_for_tx_retune: "
                             "FBRx enable failed: handle %d", fbrx_lm_handle);
        }
      }
    }	

    /*------------------------------------------------------------------------*/
    /* Build script for 1x Autopin  */
    /*------------------------------------------------------------------------*/    
    if ( rfcommon_autopin_is_enabled(RFM_1X_MODE)  &&
	 rf_cdma_autopin_par_support(
                                 tx_device, 
	 	                 tx_logical_dev, 
	 	                 RFM_1X_MODE, 
 	                         new_band ) == TRUE)
      {
       /* Get the scripts for AutoPin*/
       	pin_ret = rf_cdma_configure_autopin_trigger (
   				    tx_device,
  				    RFM_1X_MODE,
				    tx_logical_dev,
			    	    new_band);
    	if (pin_ret == FALSE)
    	{
          RF_MSG( RF_ERROR, "rf_1x_mc_wakeup_tx: pin configure failed ");
		  
    	}
       }
    if ( rfcommon_autopin_is_enabled(RFM_1X_MODE) &&
	 rf_cdma_autopin_par_support(tx_device, 
 	                        tx_logical_dev, 
	 	                   RFM_1X_MODE, 
 	                            new_band ))
    {
	/* Set the pout comp flag that FBRx is enabled. */
    rf_cdma_set_pout_comp_skip ( tx_device , TRUE ) ;
  }
     
  }/* if ( is_start_tx == TRUE ) */

  if ( retune_helper_success == TRUE  )
  {
    ret_val = RFM_CDMA_HEALTHY_STATE;
  }
  else
  {
    RF_MSG( RF_ERROR, 
            "rf_1x_mc_helper_for_retune_tx: Retune helper API failed !!" );
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }

  return ret_val;
}/* rf_1x_mc_helper_for_retune_tx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform all steps needed to retune a given device to a new band and/or channel

  @details
  This is a helper function for rf_1x_mc_retune() - all steps except for power
  collpase is done in this function.

  @param device
  The radio path to re-tune

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @param new_band
  RF CDMA band to which 1x radio is to be retuned.
 
  @param new_chan
  RF CDMA channel to which 1x radio is to be retuned.
 
  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @return
  The anticipated time in microseconds remaining from the rfm_1x_tune()
  function return until the operation is complete, or an error code.
  If the API call succeeds the return value will be non-negative; all
  error codes will be negative.
*/
static rfm_wait_time_t
rf_1x_mc_helper_for_rx_retune 
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const rfm_cdma_band_class_type new_band,
  const rfm_cdma_chan_type new_chan,
  boolean is_irat_mode
)
{
  rfm_wait_time_t ret_val; /* final result of API */
  /* flag to track result of interim steps in the API */
  boolean retune_helper_success = TRUE; 

  uint32 adc_clock_freq; /* var to store ADC Clock Frequency */
  rf_cdma_msm_static_rxlm_cfg_type static_cfg; /* Static RXLM  Settings */
  rxf_get_iqmc_coef_type iqmc_data; /* IQMC Coefficient data capsule */  
  rf_cdma_msm_dynamic_rxlm_cfg_type dyn_settings; /* Dynamic RXLM  Settings */
  rfm_cdma_power_mode_type power_mode; /* var to store Power Mode */
  rf_1x_mdsp_rx_agc_read_type rx_agc_read; /* RxAGC Read */
  const rf_cdma_data_status_type *dev_status_r;/* Dev Status Read Ptr*/
  rf_cdma_data_status_type *dev_status_w;/* Dev Status write Ptr*/
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  rf_cdma_ic_data_type *ic_data_w; /* Intelliceiver write pointer*/
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/
  rf_1x_agc_param_type agc_mode_info; /* 1x RxAGC Mode info */
  rf_time_tick_type prof_ssma_t;          /* SSMA Time Profile */
  rf_time_type prof_ssma_t_val;
  boolean is_slave_device;
  
  /* channel array to be tuned to */
  uint16 new_chan_array[RFM_CDMA_CARRIER_NUM] = { RF_CDMA_INVALID_CHAN, 
                                                  RF_CDMA_INVALID_CHAN, 
                                                  RF_CDMA_INVALID_CHAN }; 

  /*--------------------------------------------------------------------------*/
  /* check for valid device */
  if ( device >= RFM_MAX_DEVICES )
  {
    RF_MSG_1( RF_ERROR,
          "rf_1x_mc_helper_for_retune: Invalid device %d",device);
    return RFM_CDMA_ERROR_BAD_CONFIG; 
  }

  if ( rf_cdma_mc_is_band_chan_supported(device, new_band, new_chan) == FALSE )
  {
    RF_MSG_3( RF_ERROR, "rf_1x_mc_helper_for_retune: Not tunable on "
                        "device %d, Band %d, Chan %d", device, new_band, 
                        new_chan ); 
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }

  /* Get Device Status Read Pointer*/
  dev_status_r = rf_cdma_get_device_status( device );

  /* Get Device Status Read Pointer*/
  dev_status_w = rf_cdma_get_mutable_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Get CDMA write data pointer */
  ic_data_w = rf_cdma_get_mutable_intelliceiver( device );
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) ||
       ( dev_status_w == NULL ) || 
       ( ant_tuner_token_w == NULL ) || ( ic_data_w == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_retune(): NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* Validate Rx mdsp path */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_retune(): Invalid rx_mdsp_path %d",
              logical_dev->rx_mdsp_path );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/
  /* Request power off for old band */
  /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    retune_helper_success &= 
                  rfdevice_cdma_rx_pwr_sleep(
                     logical_dev->rx_device[dev_status_r->curr_band]);
  }

  new_chan_array[RFM_CDMA_CARRIER_0] = new_chan; /* Only carrier-0 used for 1x*/

  /*---- execute relevant steps from sleep_rx() sequence here ---- */

  rf_cdma_data_set_script_index( device, 
                           rf_cdma_mdsp_get_device_buffer_index( RFM_1X_MODE,
                                                                 rxlm_handle) );

  /*--------------------------------------------------------------------------*/
  /* Stop intelliceiver state-machine */
  /*--------------------------------------------------------------------------*/
  if ( dev_status_r->intelliceiver.ic_supported )
  {
    rf_cdma_ic_stop( ic_data_w, logical_dev );
  }

  /*--------------------------------------------------------------------------*/
  /* Query Rx sleep script before sending RX_STOP to clear possible           */ 
  /* notches when chain is still active. The tranceiver sleep script API call */
  /* will trigger notch update, which needs Rx Chain to be active. So querying*/ 
  /* the script before Rx_stop                                                */
  /*--------------------------------------------------------------------------*/
  if ( dev_status_r->data_1x.rf_state == RF_CDMA_STATE_RXDIV )
  {
    is_slave_device = TRUE;
  }
  else
  {
    is_slave_device = FALSE;
  }

  if ( retune_helper_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[dev_status_r->curr_band] );

    retune_helper_success &= rf_cdma_mc_configure_front_end_for_rx_sleep ( 
                                                     device, 
                                                     RFM_1X_MODE,
                                                     rxlm_handle, 
                                                     is_slave_device, 
                                                     dev_status_r->curr_band,
                                                     dev_status_r->script_index,
                                                       dev_status_r->rx_bw_khz, 
                                                     RF_CDMA_BW_1X, 
                                                     logical_dev,
                                                     ant_tuner_token_w,
                                                     TRUE /* immediate_write*/,
                                                     FALSE /* is_irat_mode */ );
    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[dev_status_r->curr_band] );
  }

  /*--------------------------------------------------------------------------*/
  /* signal FW to stop_rx                                                     */
  /*--------------------------------------------------------------------------*/
  retune_helper_success = 
    rf_1x_mc_send_stop_rx_helper ( device , dev_status_r , rxlm_handle , 
                              logical_dev , RF_CDMA_RX_STOP_RTC_CX1_TRIGGER ,
                              TRUE , /* Skip FE Programming? y/n  */  
                              FALSE,  /* Is QPCH? y/n */
                              FALSE /* is DRx*/ ) ;
  if ( FALSE == retune_helper_success )      
  {
    RF_MSG ( RF_ERROR , "rf_1x_mc_helper_for_retune(): " 
               "rf_1x_mc_rx_stop_helper() returned failure" ) ;
  }

  /*--------------------------------------------------------------------------*/
  /* New band & Chan                                                          */
  /*--------------------------------------------------------------------------*/
  /* Request power on for new band */
  /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    retune_helper_success &= 
                  rfdevice_cdma_rx_pwr_wakeup(logical_dev->rx_device[new_band]);
  }
  /* -------------------------- Update Alt. Path ---------------------------*/
  retune_helper_success &= rf_cdma_mc_update_alt_rx_path( 
                                                RFM_1X_MODE, device,
                                                new_band , 
                                                FALSE /* No retune in IRAT*/);
  /* ------------------------End of Update Alt. Path -----------------------*/

 /*--------------------------------------------------------------------------*/
  /* SSMA operations below                                                    */
  /*--------------------------------------------------------------------------*/

  /* baseband and RF recos are initialized to their defaults */
  rf_cdma_init_ssma_recos(device);

  /* init dynamic settings before any operations */
  rf_cdma_msm_init_dyn_settings(&dyn_settings);
  dyn_settings.modem_chain =
  logical_dev->rfc_dev_info.rfc_device_info[new_band]->modem_chain ;

  prof_ssma_t = rf_time_get_tick();

  retune_helper_success &= rf_cdma_get_ssma_recos ( device, rxlm_handle,
                                                    new_band, new_chan,
                                                    logical_dev->paired_device);

  /* now read the spur-info table in NV to see if any spurs need to be
     supressed *** The notches will be suppressed when RXLM is configured ***
  */
  retune_helper_success &= rf_cdma_get_spurs_to_suppress( device, rxlm_handle, 
                                 new_band, new_chan, dev_status_r->num_carriers, 
                                 &dyn_settings );

  /* Update the number of notches used on this device (for tracking use 
  for SSMA callback) 1x has only one carrier ,use index zero*/
  dev_status_w->ssma_cb_data.num_notch_filters_used = 
                   dyn_settings.notch_filt_settings[0].num_notch_filters_used;

  prof_ssma_t_val = rf_time_get_elapsed( prof_ssma_t, RF_USEC );
 
  /*--------------------------------------------------------------------------*/
  /* Determine ADC clock frequency */
  /*--------------------------------------------------------------------------*/
  adc_clock_freq = rf_cdma_msm_get_adc_samp_freq( rxlm_handle, new_band, 
                                                  new_chan );

  /* Populate static settings */
  static_cfg.adc_clock_freq = adc_clock_freq;
  /* pass in NULL for band_chan array since only one carrier is active for 1x */
  static_cfg.bw_khz = rf_cdma_compute_bandwidth( 1, new_band, NULL );
  static_cfg.rxlm_adc = logical_dev->rxlm_adc;
  static_cfg.config_as_et = FALSE;
  
  /*--------------------------------------------------------------------------*/
  /* Determine Power Mode for desired band and channel */
  /*--------------------------------------------------------------------------*/
  power_mode = rf_cdma_ic_get_starting_power_mode( ic_data_w, 
                                                   new_band,
                                                   new_chan,
                                                   static_cfg.bw_khz );
    
  /*--------------------------------------------------------------------------*/
  /* Configure RxLM */
  /*--------------------------------------------------------------------------*/

  /* Populate the IQMC Data for query */
  iqmc_data.lin = power_mode;
  iqmc_data.band = new_band;
  iqmc_data.chan = new_chan;

  iqmc_data.sec_chain_rf_state = dev_status_r->data_1x.rf_state;
  iqmc_data.bw_khz = static_cfg.bw_khz;

  /* Get RSB coefficients from device driver */
  rfdevice_cdma_rx_cmd_dispatch( logical_dev->rx_device[new_band],
                                 RFDEVICE_GET_IQMC_COEFS,
                                 &iqmc_data );

  /*--------------------------------------------------------------------------*/
  /* Populate RxLM dynamic settings based on carrier-config  */  
  /*--------------------------------------------------------------------------*/
  dyn_settings.apply_dynamic_cfg = TRUE;
  dyn_settings.iqmc_coeff_in = iqmc_data.iqmc_a_coeff;
  dyn_settings.iqmc_coeff_quad = iqmc_data.iqmc_b_coeff;
  dyn_settings.num_carriers = dev_status_r->num_carriers;
  dyn_settings.modem_chain =
  logical_dev->rfc_dev_info.rfc_device_info[new_band]->modem_chain ;

  rf_cdma_compute_carrier_info( 
                                new_band,
                                new_chan_array,
                                1 /* num_carriers = 1 for 1x*/, 
                                dev_status_r->carrier_index, 
                                &dyn_settings.carrier_mask,
                                dyn_settings.freq_offset_hz, 
                                0 /* no need to add anything to calculated
                                     freq offset */
                              );

  /* Configure Rx Link Manager Buffer for given Device, Power Mode, and
      ADC Sample Frequency */
  retune_helper_success &= 
    rf_cdma_msm_configure_rxlm( rfdevice_id_get(logical_dev->rx_device[new_band]),
                                rxlm_handle, RFM_1X_MODE,new_band,
                                power_mode, static_cfg, dyn_settings );

  /*--------------------------------------------------------------------------*/
  /* Configure RxAGC */
  /*--------------------------------------------------------------------------*/

  /* Populate the AGC Mode */
  agc_mode_info.mode = RF_1X_AGC_MODE_NORMAL;
  agc_mode_info.acq_duration = rf_cdma_msm_get_1x_default_agc_acq_duration();

  /* Configure AGC Param for the given device and Channel */
  retune_helper_success &= rf_1x_configure_rx_agc( device, rxlm_handle, 
                                       new_band, new_chan, power_mode, 
                                       dev_status_r->data_1x.rf_state, &agc_mode_info );

  if ( retune_helper_success == TRUE )
  {
    rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[new_band] );

    retune_helper_success &= rf_cdma_mc_configure_front_end_for_rx_wakeup (
                                      device, RFM_1X_MODE, is_slave_device,
                                      rxlm_handle, new_band, new_chan, 
                                      power_mode, dev_status_r->script_index, 
                                      dev_status_r->rx_bw_khz, 
                                      RF_CDMA_BW_1X,
                                      logical_dev,
                                      ant_tuner_token_w,
                                      FALSE /* is_irat_mode */, 
                                      RFDEVICE_MEAS_INVALID_SCRIPT,
                                      TRUE /* immediate_write */);

    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[new_band] );

    if ( TRUE == retune_helper_success )
    {
      retune_helper_success &= rf_1x_mc_send_start_rx_helper ( 
                                           device, dev_status_r,
                                           rxlm_handle  , logical_dev,
                                           is_irat_mode, TRUE /*is_retune*/,
                                           FALSE /*is_div_device*/ ) ;
      if ( TRUE == retune_helper_success )
      {
        if ( logical_dev->ant_tuner[new_band] != NULL )
        {
          rf_cdma_atuner_ack_script( logical_dev->ant_tuner[new_band], 
                                     ant_tuner_token_w );
        }
      }
    } /* if ( retune_helper_success == TRUE ) */
  } /* if ( retune_helper_success == TRUE ) */

  /*---- execute relevant steps from execute_wakeup_rx sequence here ---- */
  if ( retune_helper_success == TRUE )
  {
    int32 dummy_result;

    /* Tell GPS about the band and channel we are on. */
    RFGNSS_MC(1x_band_chan, dummy_result)( new_band, new_chan);
    if ( dummy_result == FALSE )
    {
      RF_MSG( RF_ERROR, 
              "rf_1x_mc_helper_for_retune: rfgnss_mc API failed !!" );
    }

    /* Unconditionally clean-up the Rx AGC LNA SM events here */
    rf_cdma_mc_cleanup_rx_agc_lna_ccs_events( device );

    rf_cdma_data_set_curr_band( device, new_band );
    /* update only the valid/appropriate carrier_index and channel */
    rf_cdma_data_set_curr_chan( device , RFM_CDMA_CARRIER_0, new_chan );
    rf_cdma_data_set_carrier_index( device , RFM_CDMA_CARRIER_0 , 0 );
    rf_cdma_data_set_rx_bw_khz( device , static_cfg.bw_khz );
    rf_cdma_data_set_num_carriers( device , 1 );

    /* retune has succeeded at this point - safe to update SSMA recos for paired
       device now, and also safe to update device status  */
    rf_cdma_update_bb_ssma_recos( logical_dev->paired_device ); 

    /* update the rotator offsets to the device data structure */
    rf_cdma_data_update_rot_offsets( device, dyn_settings.freq_offset_hz );

    rf_cdma_data_set_lo_tuned_to_chan( device , dev_status_r->rf_rx_reco.rf.lo_chan );

    if (dev_status_r->intelliceiver.power_mode == RFM_CDMA_POWER_MODE_NORMAL)
    {
      rfdevice_cdma_rx_post_pll_lock_processing( logical_dev->rx_device[new_band] );
    }

    /* Start the IntelliCeiver if the Analog Reciever supports/requires it */
    if ( dev_status_r->intelliceiver.ic_supported == TRUE )
    {
      rf_cdma_ic_start( ic_data_w, logical_dev, new_band,                     
                        dev_status_r->curr_chans[RFM_CDMA_CARRIER_0] );
    }

    /* Get Final RxAGC/ LNA State */
    rf_1x_mdsp_get_rx_agc_data( logical_dev->rx_mdsp_path, &rx_agc_read );

    rx_agc_read.rx_agc = rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10, 
                                             rx_agc_read.rx_agc);

    ret_val = 0;  /* success */

    /* Report wakeup time and final Rx AGC and LNA state */
    RF_MSG_4( RF_MED, "rf_1x_mc_helper_for_retune: Dev %d, RxLM %d, "
              "RxAGC (dBm10) %d, LNA %d", device, rxlm_handle, 
              rx_agc_read.rx_agc, rx_agc_read.lna_state );

    /* Report band/chan to which device has been tuned to */
    RF_MSG_4( RF_MED, "rf_1x_mc_helper_for_retune: [Status ]Dev %d, Band %d, "
            "Chan %d, SSMA_chan %d", device, new_band, 
            new_chan, dev_status_r->lo_tuned_to_chan);

  } /* if ( retune_helper_success == TRUE ) */
  else
  {
    RF_MSG( RF_ERROR, 
            "rf_1x_mc_helper_for_retune: Retune helper API failed !!" );
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }

  /* Report wakeup time and final Rx AGC and LNA state */
  RF_MSG_1( RF_MED, "rf_1x_mc_helper_for_retune: Time profile - SSMA %d", 
            prof_ssma_t_val );
  
  /* return final result of API */
  return ret_val;

} /* rf_1x_mc_helper_for_retune */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Prepare to turn on the radio and tune to the given band and channel.

  @details
  Prepare the software to move the 1x reciever from the Sleep state to the Rx 
  state. Powers on all circuits required to recieve a 1x signal.

  For optimal operation, this API should be called with the same band
  and channel the radio was prepared for in the rfm_1x_enable(). This function 
  should not be used to re-tune the radio if it already awake.

  This API does not interact with FW or the RF device - the actual programming 
  of the hardware is done in rf_1x_mc_exec_wakeup_rx()

  @image html rf_cdma_wakeup_rx_sequence.jpg

  @param device
  The receive path to wakeup.

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @param band
  RF CDMA band on which 1x radio is to be enabled.

  @param chan
  RF CDMA channel on which 1x radio is to be enabled.
 
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
 
  @return
  The anticipated time in microseconds remaining from the rfm_1x_wakeup_rx()
  function return until the operation is complete.
*/
rfm_wait_time_t
rf_1x_mc_prep_wakeup_rx
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const rfm_cdma_band_class_type band,
  const rfm_cdma_chan_type chan,
  boolean is_irat_mode
)
{
  rfm_wait_time_t ret_val; /* Return value */
  boolean wakeup_success = TRUE; /* Flag to track the success of wakeup rx */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  const rf_1x_config_type* cdma_config_ptr; /* Pointer to NV data */

  rf_time_tick_type prof_vreg_t;      /* Vreg Time Profile */
  rf_time_type prof_vreg_t_val=0;      /* Vreg Time Profile */

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Perform NULL Pointer check */
  if ( logical_dev == NULL ) 
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_prep_wakeup_rx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  if ( rfc_cdma_set_rx_mdsp_path(device, RF_CDMA_MDSP_CHAIN_0) == FALSE )
  {
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;    
  }
    
  /* Validate Rx mdsp path */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_prep_wakeup_rx(): Invalid rx_mdsp_path %d",
              logical_dev->rx_mdsp_path );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
  }

  /* Get NV for the requested device and band and check if it is NULL */
  cdma_config_ptr = rf_cdma_nv_get_1x_data( logical_dev->rx_nv_path[band],
                                            band );
  if ( cdma_config_ptr == NULL )
  {
    /* no NV for the requested device and band - bail out */
    RF_MSG_2( RF_ERROR, "rf_1x_mc_prep_wakeup_rx: NULL NV for device %d on "
                        "band = %d", device, band);
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed                              */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* bring carrier info to initial-state: invalidate all carrier_index &      */
  /* channels                                                                 */
  /*--------------------------------------------------------------------------*/
  rf_cdma_init_freq_info_for_device(device);
  rf_cdma_data_set_assoc_dev( device, RFM_INVALID_DEVICE );

  /* assign the desired band and chan */
  rf_cdma_data_set_curr_band( device, band );

  /* update only the valid/appropriate carrier_index and channel */
  rf_cdma_data_set_curr_chan( device , RFM_CDMA_CARRIER_0, chan );
  rf_cdma_data_set_carrier_index( device , RFM_CDMA_CARRIER_0 , 0 );

  /* assign correct values for Rx and Tx BW */
  rf_cdma_data_set_rx_bw_khz( device , RF_CDMA_BW_1X );/* 1.25MHz for 1x */

  /* TX BW is 0Mhz since Tx is not yet active */
  rf_cdma_data_set_tx_bw_khz( device , RF_CDMA_BW_INVALID  );

  /* num of carriers is 1 for OneX*/
  rf_cdma_data_set_num_carriers( device , 1 );  
  
  /* set rxlm in rf_cdma_data */
  rf_cdma_data_set_rxlm_handle( device, rxlm_handle);

  /*! allocate asd memory*/
  rflm_cmn_asd_allocate_memory((uint32)rxlm_handle);
  
  /*--------------------------------------------------------------------------*/
  /* Enable Chip Power for all devices in the receiver - Device, RxAGC, RFCard*/
  /*--------------------------------------------------------------------------*/
  if ( is_irat_mode == FALSE )
  {
    prof_vreg_t = rf_time_get_tick();

    wakeup_success &= rfc_cdma_manage_vregs( device, RFM_1X_MODE, band,
                                             RF_PATH_RX_STATE);

    prof_vreg_t_val = rf_time_get_elapsed( prof_vreg_t, RF_USEC );

    /* report time taken */
    RF_MSG_1 ( RF_MED, "rf_1x_mc_prep_wakeup_rx: time profile - Vreg %d ", 
               prof_vreg_t_val );
  }


  /* --------------------------- END OF STATIC INIT --------------------------*/
  /* No Static de-initialization can be done after this point. Only wakeup 
     hardware sequence is executed */
  /*--------------------------------------------------------------------------*/

  /* Check if all Static Initialization when through. If not, return Failure.
  For successful execution, do the Rx Wakeup/Tune hardware sequence. */
  if ( wakeup_success == TRUE )
  {
    int32 dummy_result;

    /* Tell GPS about the band and channel we are on. */
    RFGNSS_MC(1x_band_chan, dummy_result)(band, chan);
    if ( dummy_result == FALSE )
    {
      RF_MSG( RF_ERROR, 
              "rf_1x_mc_prep_wakeup_rx: rfgnss_mc API failed !!" );
    }

    /* Update CDMA Data structure with new RF State and band/Chan */
    rf_cdma_data_set_1x_rf_state( device, RF_CDMA_STATE_RX_PREP );
    rf_cdma_data_set_curr_chan( device, RFM_CDMA_CARRIER_0, (uint16) chan );
    rf_cdma_data_set_curr_band( device,  band );

    rf_cdma_data_sanity_check( device );

    /* Report success */
    RF_MSG_4( RF_MED, "rf_1x_mc_prep_wakeup_rx: Dev %d, RxLM %d, Band %d, "
              "Chan %d", device, rxlm_handle, band, chan);

    /* No Wait time */
    ret_val = 0;

  } /* if ( wakeup_success == TRUE ) */
  else
  {
    /* undo changes done to driver state machine */
    rf_cdma_process_wakeup_failure( device, RFM_1X_MODE, band );    

    /* Report failure */
    RF_MSG_4( RF_ERROR, "rf_1x_mc_prep_wakeup_rx: Failed!! Dev %d, RxLM %d, "
              " Band %d, Chan %d", device, rxlm_handle, band, chan);

    ret_val = RFM_CDMA_ERROR_FAILURE;
  }

  /* Return the API Wait time */
  return ret_val;

} /* rf_1x_mc_prep_wakeup_rx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Execute Wakeup Rx sequence to turn on the radio and tune to the given band 
  and channel.

  @details
  
  Prepares all the scripts required to tune the radio to the requested channel.

  This function will send a command to firmware to indicate that Rx needs to be
  started. The command will indicate FW to activate the RxLM configuration 
  along with the RTR script to be programmed. Based on the Settling time
  sent, FW will start the RxAGC as well.

  @image html rf_cdma_wakeup_rx_sequence.jpg

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
  The anticipated time in microseconds remaining from the
  rfm_1x_exec_wakeup_rx() function return until the operation is complete.
*/
rfm_wait_time_t
rf_1x_mc_exec_wakeup_rx
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  boolean is_irat_mode,
  const rfdevice_meas_scenario_type meas_type
)
{
  rf_time_tick_type Prx_wakeup_time_tick;        /* time spent in API */
  rf_time_type Prx_wakeup_time_val=0;
  rfm_wait_time_t ret_val; /* Return value */
  boolean exec_success = TRUE; /* tracks results of various APIs called */
  rfm_cdma_power_mode_type power_mode;
  rf_1x_agc_param_type agc_mode_info; /* 1x RxAGC Mode info */  

  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  rf_cdma_data_status_type *dev_status_w;/* Dev Status write Ptr*/
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */

  rf_cdma_ic_data_type *ic_data_w; /* Intelliceiver write pointer*/
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/

  timetick_type prof_rxagc_cfg_t; /* RxAGC Config Time Profile */
  timetick_type prof_rxlm_t;      /* RXLM Time Profile */
  timetick_type prof_ssma_t;      /* SSMA Time Profile */

  uint32 adc_clock_freq; /* var to store ADC Clock Frequency */
  rxf_get_iqmc_coef_type iqmc_data; /* IQMC Coefficient */  
  rf_cdma_msm_static_rxlm_cfg_type static_cfg; /* Static RXLM  Settings */
  rf_cdma_msm_dynamic_rxlm_cfg_type dyn_settings; /* Dynamic RXLM  Settings */
  rfm_cdma_band_class_type band;
  uint32 script_index ;
  rfm_device_enum_type  tq_device;

  Prx_wakeup_time_tick=rf_time_get_tick();
  /* Get Device Status Read Pointer*/
  dev_status_r = rf_cdma_get_device_status( device );

  /* Get Device Status mutable Pointer*/
  dev_status_w = rf_cdma_get_mutable_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );
  
  /* Get CDMA Data Write Pointer*/
  ic_data_w = rf_cdma_get_mutable_intelliceiver( device );
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) || 
       ( dev_status_w == NULL ) || 
       ( ic_data_w == NULL) || ( ant_tuner_token_w == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_exec_wakeup_rx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* Validate Rx mdsp path */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_helper_for_retune(): Invalid rx_mdsp_path %d",
              logical_dev->rx_mdsp_path );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;

  }

  /* check for valid band */
  band = dev_status_r->curr_band;
  
  if ( band >= RFM_CDMA_MAX_BAND )
  {
    RF_MSG_1 ( RF_ERROR, "rf_1x_mc_exec_wakeup_rx: Invalid band %d ", band );
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }
  
  /* check for valid carriers */
  if ( dev_status_r->num_carriers >= RFM_CDMA_CARRIER_NUM )
  {
    RF_MSG_1 ( RF_ERROR, "rf_1x_mc_exec_wakeup_rx: Invalid num_carriers %d ", 
               dev_status_r->num_carriers );
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed                              */
  /*--------------------------------------------------------------------------*/

  /* -------------------------- Update Alt. Path ---------------------------*/
  exec_success &= rf_cdma_mc_update_alt_rx_path( RFM_1X_MODE, device,
                                                 band ,
                                                 is_irat_mode);
  /* ------------------------End of Update Alt. Path -----------------------*/

    /*-----------Update the Antennae switch----------- */
#if defined(FEATURE_RF_ASDIV) || defined(FEATURE_RF_HAS_TPTUNER_SWITCH)
  rf_cdma_data_set_ant_position ( device , 
    ((uint8) rfcommon_asdiv_get_current_position ( device ) ) ) ;

  RF_MSG_1 ( RF_MED , "rf_1x_mc_exec_wakeup_rx: Ant Switch position %d ", 
             rf_cdma_data_get_ant_position ( device ) ) ;
#else
  rf_cdma_data_set_ant_position ( device , ( (uint8)(0) ) ) ;
#endif


  /* Use device stored seperately to allocate TQ, in order to support QCTA.
     tq_device is usually the source tech primary device */
  if ( is_irat_mode == FALSE )
  {
    tq_device = device;
  }
  else
  {
    tq_device = dev_status_r->irat_data.irat_tq_device;
  }

  /* allocate resources (data structs, queues, etc) needed for 1x operation */ 
  rf_cdma_mdsp_modem_rx_allocate_resources( tq_device, rxlm_handle,
                                            RFM_1X_MODE , is_irat_mode ) ;

  /* init dynamic settigs before any RXLM operations */
  rf_cdma_msm_init_dyn_settings(&dyn_settings);

  if ( is_irat_mode == FALSE )
  {
    /* vote for RFCMD app - this should done as early as possible in the 
             wakeup sequence however in IRAT the source tech already does this */
    {
      rf_hal_bus_client_handle* handle = rf_cdma_data_get_rf_hal_bus_vote_handle(device);
      #ifndef FEATURE_RFA_ATLAS_MODEM
      exec_success &= rf_hal_bus_enable (TRUE, &handle, RF_HAL_RFCMD_APP_CLIENT(1X));
      #endif
      exec_success &= rf_cdma_data_set_rf_hal_bus_vote_handle(device, handle);
    }
    
    /* Tell IC SM it is primary device. 
       Make sure this is the 1st time IC SM is touched */
    (void) rf_cdma_set_ic_support ( ic_data_w , TRUE ) ;

    /* Determine Power Mode for desired band and channel */
    power_mode = rf_cdma_ic_get_starting_power_mode( 
                                   ic_data_w, 
                                   band,
                                   dev_status_r->curr_chans[RFM_CDMA_CARRIER_0],
                                   dev_status_r->rx_bw_khz );
    script_index = 
      rf_cdma_mdsp_get_device_buffer_index(RFM_1X_MODE, rxlm_handle ) ;

    /* Vote ON for all RF Devices which as needed with RFM Device (device) 
       This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */
    exec_success &= rfcommon_core_dev_power_vote( device,
                                                RFCMN_CORE_DEV_ON,
                                                RFCMN_CORE_DEV_NORMAL_VOTE );
	
  }/* if ( is_irat_mode == FALSE ) */
  else
  {
    /* Set lowest power mode in IRAT scenario */
    power_mode = rf_cdma_ic_get_lowest_power_mode ( ic_data_w ); ;
    script_index = dev_status_r->irat_data.irat_buffer_index ;
  }

  /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
  if (rfcommon_nv_get_lpm_feature_enabled())
  {
    exec_success &= rfdevice_cdma_rx_pwr_wakeup(logical_dev->rx_device[band]);
  }

  /*--------------------------------------------------------------------------*/
  /* SSMA operations below                                                    */
  /*--------------------------------------------------------------------------*/
  prof_ssma_t = timetick_get();

  exec_success &= rf_cdma_get_ssma_recos ( device, rxlm_handle, 
                                   band, 
                                   dev_status_r->curr_chans[RFM_CDMA_CARRIER_0],
                                   logical_dev->paired_device );

  /* now read the spur-info table in NV to see if any spurs need to be
     supressed *** The notches will be suppressed when RXLM is configured     */
  exec_success &= rf_cdma_get_spurs_to_suppress ( 
                                   device, rxlm_handle, band, 
                                   dev_status_r->curr_chans[RFM_CDMA_CARRIER_0], 
                                   dev_status_r->num_carriers, &dyn_settings );

  /* Update the number of notches used on this device (for tracking use 
  for SSMA callback) 1x has only one carrier ,use index zero*/
  dev_status_w->ssma_cb_data.num_notch_filters_used = 
                dyn_settings.notch_filt_settings[0].num_notch_filters_used;

  prof_ssma_t = timetick_get_elapsed( prof_ssma_t, T_USEC );
  /*--------------------------------------------------------------------------*/
  /* Configure RxLM */
  /*--------------------------------------------------------------------------*/
  prof_rxlm_t = timetick_get();

  adc_clock_freq = rf_cdma_msm_get_adc_samp_freq( rxlm_handle, 
                                 band, 
                                 dev_status_r->curr_chans[RFM_CDMA_CARRIER_0] );

  /* Populate the IQMC Data for query */
  iqmc_data.lin = power_mode;
  iqmc_data.band = band;
  iqmc_data.chan = dev_status_r->curr_chans[RFM_CDMA_CARRIER_0];
  iqmc_data.sec_chain_rf_state = RF_CDMA_STATE_INVALID;
  iqmc_data.bw_khz = dev_status_r->rx_bw_khz;

  /* Populate static settings */
  static_cfg.adc_clock_freq = adc_clock_freq;
  static_cfg.bw_khz = dev_status_r->rx_bw_khz;
  static_cfg.rxlm_adc = logical_dev->rxlm_adc;
  static_cfg.config_as_et = FALSE;
  
  /* Get RSB coefficients from device driver */
  rfdevice_cdma_rx_cmd_dispatch( logical_dev->rx_device[band],
                                 RFDEVICE_GET_IQMC_COEFS,
                                 &iqmc_data);

  dyn_settings.apply_dynamic_cfg = TRUE;
  dyn_settings.iqmc_coeff_in = iqmc_data.iqmc_a_coeff;
  dyn_settings.iqmc_coeff_quad = iqmc_data.iqmc_b_coeff;
  dyn_settings.num_carriers = dev_status_r->num_carriers;
  dyn_settings.modem_chain =
                   logical_dev->rfc_dev_info.rfc_device_info[band]->modem_chain;

  /* update dyn_settings with rotator offset based on num_carriers,
     lo_offset_KHz and curr_chans */
  rf_cdma_compute_carrier_info( band,
                                dev_status_r->curr_chans,
                                dev_status_r->num_carriers, 
                                dev_status_r->carrier_index, 
                                &dyn_settings.carrier_mask,
                                dyn_settings.freq_offset_hz,
                                dev_status_r->rf_rx_reco.rf.lo_offset_KHz*1000
                              );

  exec_success &= 
    rf_cdma_msm_configure_rxlm( rfdevice_id_get(logical_dev->rx_device[band]),
                                rxlm_handle, RFM_1X_MODE, band,
                                power_mode, static_cfg, dyn_settings );

  prof_rxlm_t = timetick_get_elapsed( prof_rxlm_t, T_USEC );

  /*--------------------------------------------------------------------------*/
  /* Configure RxAGC */
  /*--------------------------------------------------------------------------*/

  /* Populate the AGC Mode */
  agc_mode_info.mode = RF_1X_AGC_MODE_NORMAL;
  agc_mode_info.acq_duration = rf_cdma_msm_get_1x_default_agc_acq_duration();

  /* Configure AGC Param for the given device and Channel */
  prof_rxagc_cfg_t = timetick_get();
  
  exec_success &= rf_1x_configure_rx_agc( device, rxlm_handle, 
                                          band, 
                                          dev_status_r->rf_rx_reco.rf.lo_chan, 
                                          power_mode, 
                                          dev_status_r->data_1x.rf_state,
                                          &agc_mode_info );

  prof_rxagc_cfg_t = timetick_get_elapsed( prof_rxagc_cfg_t, T_USEC );

  /*--------------------------------------------------------------------------*/
  /* Configure Analog Script and send RX_START to FW                          */
  /*--------------------------------------------------------------------------*/
  rf_cdma_data_set_script_index( device, script_index ) ;

  if ( exec_success == TRUE )
  {  
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[band] );

    exec_success &= rf_cdma_mc_configure_front_end_for_rx_wakeup (
                      device, RFM_1X_MODE, FALSE /*is_slave_device*/,
                      rxlm_handle, band, dev_status_r->rf_rx_reco.rf.lo_chan, 
                      power_mode, dev_status_r->script_index, 
                      dev_status_r->rx_bw_khz, dev_status_r->tx_bw_khz,
                      logical_dev,
                      ant_tuner_token_w,
                      is_irat_mode, 
                      meas_type,
                      FALSE);

    if ( exec_success == TRUE )
    {
      exec_success = 
      rf_1x_mc_send_start_rx_helper ( device, dev_status_r, rxlm_handle, 
                                      logical_dev, is_irat_mode, 
                                      FALSE /*is_retune*/,
                                      FALSE /*is_div_device*/);
    
      if ( logical_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script( logical_dev->ant_tuner[band], 
                                   ant_tuner_token_w );
      }     
    } /* ( exec_success == TRUE ) */

    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[band] );

  } /* if ( exec_success == TRUE ) */
   
  if ( ( exec_success == TRUE ) && (is_irat_mode == FALSE) )
  {
    /* do not need to apply notches while in irat (design decision) */
    /* Apply notch filter recommendations from WTR */
    exec_success &= 
    rf_cdma_mc_trigger_dynamic_notch_update(device,
                        dev_status_r->ssma_cb_data.ssma_reco,
                        dev_status_r->ssma_cb_data.num_notch_filters_used,
                        dev_status_r->ssma_cb_data.clear_notch); 
  }

    /* Unconditionally clean-up the Rx AGC LNA SM events here */
    exec_success &= rf_cdma_mc_cleanup_rx_agc_lna_ccs_events( device );
      
  if ( exec_success == TRUE )
  {
    if ( dev_status_r->intelliceiver.power_mode == RFM_CDMA_POWER_MODE_NORMAL)
    {
      rfdevice_cdma_rx_post_pll_lock_processing( logical_dev->rx_device[band] );
    }

    /* Start the IntelliCeiver if the Analog Reciever supports/requires it ,
    additionally check for IRAT mode, if we are in IRAT we should skip all
    Intelliceiver operations. IRAT is designed to be only in a single power
    mode */
    if ( ( dev_status_r->intelliceiver.ic_supported == TRUE ) &&
         ( is_irat_mode == FALSE ))
    {
      rf_cdma_ic_start( ic_data_w, logical_dev,
                        band, dev_status_r->curr_chans[RFM_CDMA_CARRIER_0] );
    }
    else
    {
      /* Print F3 */
      RF_MSG_2( RF_LOW, "rf_1x_mc_exec_wakeup_rx: skipping IC Start :"
                          "ic_supported: %d ; is_irat_mode:%d", 
                dev_status_r->intelliceiver.ic_supported, 
                is_irat_mode ); 
    }/*if ( ( dev_status_w->intellic.....*/

    /* exec has succeeded at this point - safe to update SSMA recos for paired
       device now, and also safe to update device status  */
    rf_cdma_update_bb_ssma_recos( logical_dev->paired_device );
      
    /* Update CDMA Data structure with new RF State and  band/Chan */
    rf_cdma_data_set_1x_rf_state( device, RF_CDMA_STATE_RX );
    rf_cdma_data_set_lo_tuned_to_chan( device, 
                                          dev_status_r->rf_rx_reco.rf.lo_chan );

    /* update the rotator offsets to the device data structure */
    rf_cdma_data_update_rot_offsets( device, dyn_settings.freq_offset_hz );

    ret_val = 0; /* no wait needed by L1 */

    rf_cdma_data_sanity_check( device );

  } /* if ( exec_success == TRUE ) */
  else
  { 
    /* undo changes done to driver state machine */
    rf_cdma_process_wakeup_failure( device, RFM_1X_MODE, band );  

    /* invalidate all SSMA recos for paired device that were retrieved by the
       rf_cdma_get_ssma_recos() call above */
    rf_cdma_init_ssma_recos ( logical_dev->paired_device );
 
    /* Report failure */
    RF_MSG_2( RF_ERROR, "rf_1x_mc_exec_wakeup_rx: Failed!! Dev %d, RxLM %d",
              device, dev_status_r->rxlm_handle );
 
    ret_val = RFM_CDMA_ERROR_FAILURE;

  } /* if ( exec_success == FALSE ) */

  Prx_wakeup_time_val = rf_time_get_elapsed( Prx_wakeup_time_tick, RF_USEC );
  /* Report time taken for AGC and results */
  RF_MSG_5 ( RF_MED, 
            "rf_1x_mc_exec_wakeup_rx: [Status %d] Device %d, RxLM %d, "
            "RxAGC time %d, RxLM config time %d ",
            ret_val, device, dev_status_r->rxlm_handle, prof_rxagc_cfg_t, 
            prof_rxlm_t );

    /*updating the histogram for rx */
   #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
   prof_hist_update(&rx_1x_wakeup_PRx_time_hist, (uint32)Prx_wakeup_time_val);
   if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
   {
     rf_1x_clear_profiling_hist_packet();
   }
  #endif
  /* Return the API result */
  return ret_val;

} /* rf_1x_mc_exec_wakeup_rx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Turn off the receiver.

  @details
  Moves the 1x radio from the Rx state to the Sleep state.  Any circuits
  that are no longer needed are powered down to conserve current.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_RX_SLEEP_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.
 
  If SLEEP is requested from RX state, then make sure that iC is stopped,
  RTR is programmed to Sleep state, and RX_STOP is
  sent to FW. However, if SLEEP is requested from RX_PREP state, there is no 
  need of reprogramming RTR : Just disabling VREG is enough, as RTR programming 
  (and FW RX_START) is done only from execute_Rx_Wakeup (not from 
  prep_rx_wakeup). Moreover, we cannot send RX_STOP without prior RX_START. Thus
  if we dont skip RX_STOP during RX_PREP to RX_SLEEP transition, then FW will
  assert a crash.

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
  The anticipated time in microseconds remaining from the rfm_1x_sleep_rx()
  function return until the operation is complete. Returns a 0 on success (no
  wait time needed) and a negative value on failure.
 
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
rf_1x_mc_sleep_rx
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  boolean is_irat_mode,
  boolean is_meas_enter
)
{
  rf_time_tick_type Prx_sleep_time_tick;        /* time spent in API */
  rf_time_type Prx_sleep_time_val;
  rfm_wait_time_t ret_val = 0; /* Return value */
  boolean sleep_success = TRUE; /* Flag to track the success of sleep rx */
  boolean dev_voting_status =TRUE; /* Need the flag to perform device voting */

  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device*/

  rf_cdma_ic_data_type *ic_data_w; /* Intelliceiver write pointer*/
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/

  rf_time_tick_type prof_sleep_t = rf_time_get_tick();        /* time spent in API */
  rf_time_type prof_sleep_t_val;

  /* Flag to indicate if FE programming is required by FW or not
  in IRAT cases it can be skipped */
  boolean skip_frontend_programming = FALSE;
  Prx_sleep_time_tick=rf_time_get_tick();

  /* Get Device Status Read Pointer*/
  dev_status_r = rf_cdma_get_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Get CDMA Data Write Pointer*/
  ic_data_w = rf_cdma_get_mutable_intelliceiver( device );
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) || 
       ( ic_data_w == NULL) || ( ant_tuner_token_w == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_sleep_rx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* Validate Rx mdsp path */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_sleep_rx(): Invalid rx_mdsp_path %d",
              logical_dev->rx_mdsp_path );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/

  /* If SLEEP is requested from RX state, then make sure that iC is stopped,
  RTR is programmed to Sleep state, and RX_STOP is sent to FW. However, if SLEEP 
  is requested from RX_PREP state, there is no need of reprogramming RTR : Just 
  disabling VREG is enough, as RTR programming (and FW RX_START) is done only 
  from execute_Rx_Wakeup (not from prep_rx_wakeup). Moreover, we cannot send 
  RX_STOP without prior RX_START. Thus if we dont skip RX_STOP during RX_PREP to 
  RX_SLEEP transition, then FW will assert a crash. */
  if ( dev_status_r->data_1x.rf_state == RF_CDMA_STATE_RX )
  {
    rfm_cdma_band_class_type band = dev_status_r->curr_band;

  /*------------------------------------------------------------------------*/
  /* Stop intelliceiver state-machine,check if ic is supported and if we are 
     in IRAT mode or not, all IC operations are skipped in IRAT */
  /*------------------------------------------------------------------------*/
  if ( ( dev_status_r->intelliceiver.ic_supported ) &&
         ( is_irat_mode == FALSE ) )
  {
    rf_cdma_ic_stop( ic_data_w, logical_dev );
  }
  else
  {
    /* Print F3 */
      RF_MSG_2( RF_LOW, "rf_1x_mc_sleep_rx: skipping IC Stop :"
                          "ic_supported: %d ; is_irat_mode:%d", 
              dev_status_r->intelliceiver.ic_supported, 
                is_irat_mode ); 
  }/*if ( ( dev_status_w->intellic.....*/

    /*------------------------------------------------------------------------*/
    /* Configure RF-front end for Sleep and send RX_STOP to FW                */
    /*------------------------------------------------------------------------*/
    rf_cdma_data_set_script_index( device, 
                            rf_cdma_mdsp_get_device_buffer_index(RFM_1X_MODE,
                                                                 rxlm_handle) );
  
    /* Enter Device Critical Section, so that the device call is thread safe*/
    rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[band] );
  
    /* In IRAT refrain from sleeping the FE in case it is being shared by Source Tech */
    if ( is_irat_mode == FALSE )
    {
      sleep_success &= rf_cdma_mc_configure_front_end_for_rx_sleep ( 
                               device, RFM_1X_MODE, 
                               rxlm_handle, FALSE /* is_slave_device */, 
                               band,
                               dev_status_r->script_index,
                               dev_status_r->rx_bw_khz, 
                               dev_status_r->tx_bw_khz, logical_dev,
                               ant_tuner_token_w,
                               FALSE /* immediate_write */,
                               FALSE /* is_irat_mode */ );
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
    }/*if ( is_irat_mode == FALSE )*/
  
     /* In 1x2L at during meas exit RF_CDMA_STATE_SHUTDOWN_FOR_IRAT will be set 
       in the Gap so L can start measurements
       In 1x2D when sleep is called RF_CDMA_STATE_SHUTDOWN_FOR_IRAT is set so
       we dont send Rx Stop but still reset the SW state.
    */
    if ( sleep_success == TRUE )  
    {
      if ( dev_status_r->data_1x.rf_state != RF_CDMA_STATE_SHUTDOWN_FOR_IRAT ) 
      {

        sleep_success = 
          rf_1x_mc_send_stop_rx_helper ( device , dev_status_r , rxlm_handle , 
                                    logical_dev , RF_CDMA_RX_STOP_RTC_CX1_TRIGGER ,
                                    skip_frontend_programming , 
                                    /* Skip FE Programming? y/n  */  
                                    FALSE , /* Is QPCH? y/n */
                                    FALSE /* Is DRx*/  ) ;

        /* In IRAT the LNA FSM needs to be cleaned up. For other cases Stop Helper
         takes care of this */
        if ( is_irat_mode == TRUE )
        {
          (void)rf_cdma_mc_cleanup_lna_fsm(rxlm_handle);
        }

        if ( sleep_success == FALSE )
        {
          RF_MSG ( RF_ERROR , "rf_1x_mc_sleep_rx: "
                          "rf_1x_mc_rx_stop_helper() returned failure " );
        }

        if ( logical_dev->ant_tuner[band] != NULL )
        {
          rf_cdma_atuner_ack_script( logical_dev->ant_tuner[band],
                                     ant_tuner_token_w );
        }
      }
      else
      {
        RF_MSG ( RF_MED , "rf_1x_mc_sleep_rx(): " 
                 "rf_1x_mc_rx_stop_helper() Skipped " 
                 "RF_CDMA_STATE_SHUTDOWN_FOR_IRAT is TRUE " ) ;
      }
    } /* if ( sleep_success == TRUE ) */
  
    /* Clean up RF CCS event smem and event handle */
    rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );
  
    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[band] );
  } /* if ( dev_status_r->rf_state == RF_CDMA_STATE_RX ) */

  /* Disable Vreg and Update CDMA Data structure if ret_val is good. This part 
  of code will be executed for both RX_PREP->RX_SLEEP or RX->RX_SLEEP 
  transitions */
  if ( sleep_success == TRUE )
  {
    rf_time_tick_type prof_ssma_t; /* tracks time taken by SSMA routines */
    rf_time_type prof_ssma_t_val;

    /*------------------------------------------------------------------------*/
    /* rf_1x_mc_sleep_rx can be called after prep_wakeup or after exec_wakeup.
       If called after prep_wakeup, no need to un-vote if RF is in 
       RF_CDMA_STATE_RX_PREP, since prep_wakeup_rx() does not vote for TCXO 
       buffer (i.e., it does not call rfcommon_core_dev_power_vote ) */

      if ( (is_meas_enter != TRUE) && (dev_status_r->data_1x.rf_state !=  RF_CDMA_STATE_RX_PREP) )
      {
        /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
	if (rfcommon_nv_get_lpm_feature_enabled())
	{
          dev_voting_status = 
                        rfdevice_cdma_rx_pwr_sleep(
                             logical_dev->rx_device[dev_status_r->curr_band]);
	}
        else
        {
          dev_voting_status =  TRUE;
        }

        if( dev_voting_status == FALSE )
        {
          RF_MSG_1( RF_ERROR, "rf_1x_mc_sleep_rx(): Device: %d Pwr Down failed"
                            ,device);
          ret_val = RFM_CDMA_ERROR_FAILURE;
        }
        sleep_success &= dev_voting_status;
      } /* if ( dev_status_r->rf_state !=  RF_CDMA_STATE_RX_PREP ) */
    else
    {
      RF_MSG_2( RF_MED, "rf_1x_mc_sleep_rx(): Device: %d is in prep_wakeup state :%d" 
	  	        "or it is IRAT enter mode" ,device,dev_status_r->data_1x.rf_state);
    }

    /* If in IRAT dont touch the VREGs and power votes when CDMA is either the SRC
       or the Target tech.
       1> When CDMA is the source tech Vregs need to be ON for the Target tech like 
          LTE because turning off the Vregs will cause LTE to not find any energy.
       2> When CDMA is the target tech then the SRC tech handles all the VREG management.   
    */	  
    if ( is_irat_mode == FALSE )
    { 

      /* Vote OFF for all RF Devices which are not needed with RFM Device 
         This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS 
         settings */
      dev_voting_status &= rfcommon_core_dev_power_vote(device,
                                                        RFCMN_CORE_DEV_OFF,
                                                        RFCMN_CORE_DEV_NO_VOTE);
	   
      /* Revert the Vreg State, since Rx is going to sleep */
      rfc_cdma_manage_vregs( device, RFM_1X_MODE, dev_status_r->curr_band,
                             RF_PATH_SLEEP_STATE);

      /* un-vote for RFCMD app */
      {
        rf_hal_bus_client_handle* handle = rf_cdma_data_get_rf_hal_bus_vote_handle(device);
        #ifndef FEATURE_RFA_ATLAS_MODEM
        rf_hal_bus_enable (FALSE, &handle, RF_HAL_RFCMD_APP_CLIENT(1X));
        #endif
        rf_cdma_data_set_rf_hal_bus_vote_handle(device, handle);
      }

      /* Abort pending AsDiv Event */
      rf_cdma_mdsp_abort_asd_events( rxlm_handle );
	  
      /*! deallocate asd memory*/
      rflm_cmn_asd_deallocate_memory((uint32)rxlm_handle);
    }/* if ( is_irat_mode == FALSE )*/

   Prx_sleep_time_val = rf_time_get_elapsed( Prx_sleep_time_tick, RF_USEC );
   #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
     prof_hist_update(&rx_1x_sleep_PRx_time_hist,(uint32)Prx_sleep_time_val );
     if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
     {
       rf_1x_clear_profiling_hist_packet();
     }
   #endif
    /*------------------------------------------------------------------------*/
    /* Undo any SSMA operations performed on paired_device                    */
    /* NOTE: device status must be updated (as done above) before calling SSMA*/ 
    /*------------------------------------------------------------------------*/
    prof_ssma_t = rf_time_get_tick();   /* SSMA Time Profile */

    /* device successfully put to sleep - undo any SSMA operations performed
       on the paired device... since the device is asleep, need to use
       RF_CDMA_INVALID_BAND and RF_CDMA_INVALID_CHAN while calling the SSMA
       routines */
    (void) rf_cdma_get_ssma_recos ( device, rxlm_handle, RFM_CDMA_MAX_BAND,
                                    RF_CDMA_INVALID_CHAN,
                                    logical_dev->paired_device );

    (void) rf_cdma_update_bb_ssma_recos( logical_dev->paired_device );

    prof_ssma_t_val = rf_time_get_elapsed( prof_ssma_t, RF_USEC );
    RF_MSG_3( RF_MED, "rf_1x_mc_sleep_rx: completed for Dev %d, RxLM %d "
                      "SSMA time %d", device, rxlm_handle, prof_ssma_t_val );

    /* ***** Warning ****** Warning ******* Warning ******* Warning ****** Warning ****** Warning ******* 
    **  No writes to anything on SSBI, GRFC  RFFE bus or any peripheral after 
    **  here. TQ will be relinquished */

    /* In 1x2L in meas_exit , dont destroy the band channel because for CDMA to L measurements,
      the next command after the gap the L1 sends is a sleep command which is when 
      we will reset the SW data structures. */
    if ( ( is_irat_mode == TRUE ) &&
       ( dev_status_r->data_1x.rf_state != RF_CDMA_STATE_SHUTDOWN_FOR_IRAT ) )
    {
      RF_MSG ( RF_MED , "rf_1x_mc_sleep_rx(): "
             "In CDMA-2-L meas enter. Skip update of device structures" ) ;
      RF_MSG ( RF_MED , "rf_1x_mc_sleep_rx(): "
             "in CDMA-2-L meas exit. Skip rf_cdma_mdsp_modem_exit()" ) ;
    }
    else
    {
      /* We need to be in RX state to deallocate resources, cannot deallcoate
      if we are in prep since we will end up deallocating resources that 
      were never allocated to 1x in the first place */
      if ( dev_status_r->data_1x.rf_state == RF_CDMA_STATE_RX_PREP )
      {
        RF_MSG ( RF_MED , "rf_1x_mc_sleep_rx(): "
             "Skipping RFLM resource deallocation since RF state is 'Prep'" ) ;
      }
      else
      {
        rfm_device_enum_type tq_device;

        /* Use device stored seperately to deallocate TQ, in order to support QCTA.
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
        rf_cdma_mdsp_modem_rx_deallocate_resources ( tq_device , RFM_1X_MODE ,
                                                      rxlm_handle ) ;
      }

      rf_cdma_reset_sw_state ( device ) ;
    }
    rf_cdma_data_sanity_check( device );

  } /* if ( sleep_success == TRUE )*/ 
  else
  {
    RF_MSG_2( RF_ERROR, "rf_1x_mc_sleep_rx: error occured for Dev %d, RxLM %d ", 
              device, rxlm_handle); 
    ret_val =  RFM_CDMA_ERROR_FAILURE;
  }

  prof_sleep_t_val = rf_time_get_elapsed( prof_sleep_t, RF_USEC );


  RF_MSG_3( RF_MED, "rf_1x_mc_sleep_rx: Dev %d, RxLM %d, time_taken %d", 
            device, rxlm_handle, prof_sleep_t_val );

  return ret_val;

} /* rf_1x_mc_sleep_rx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Turn off the receiver in future time.

  @details
  Based on the time provided, this funciton moves the 1x radio from the Rx state 
  to the Sleep state. This function will basically create RTR/GRFC scripts and
  sends the RX_STOP command indicating that the radio should be stopped
  in future time.
 
  Any circuits that are no longer needed will be programeed and powered down
  when RF receives a response back from FW.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_RX_TIMED_SLEEP_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.

  @param device
  The receive path to put to sleep.

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.
 
  @param rtc_cx1_trigger
  RTC time in Cx1 units. This is the time when RX should be stoppped.
 
  @param is_offline_qpch
  Flag indicating, if this function is called in reference to the Offline
  QPCH sequence

  @return
  The anticipated time in microseconds remaining from the rfm_1x_sleep_rx()
  function return until the operation is complete. Returns a 0 on success (no
  wait time needed) and a negative value on failure.
*/
rfm_wait_time_t
rf_1x_mc_timed_sleep_rx
(
  const rfm_device_enum_type device,
  const lm_handle_type rxlm_handle,
  const uint32 rtc_cx1_trigger,
  const boolean is_offline_qpch
)
{
  rfm_wait_time_t ret_val = 0; /* Return value */
  boolean sleep_success = TRUE; /* Flag to track the success of sleep rx */

  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device*/

  rf_cdma_ic_data_type *ic_data_w; /* Intelliceiver write pointer*/
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/

  rfm_cdma_band_class_type band;

  /* Get Device Status Read Pointer*/
  dev_status_r = rf_cdma_get_device_status( device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Get CDMA Data Write Pointer*/
  ic_data_w = rf_cdma_get_mutable_intelliceiver( device );
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) || 
       ( ic_data_w == NULL) || ( ant_tuner_token_w == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_timed_sleep_rx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* Validate Rx mdsp path */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_timed_sleep_rx(): Invalid rx_mdsp_path %d",
              logical_dev->rx_mdsp_path );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
  }

  /* Get current band */
  band = dev_status_r->curr_band;

  if ( band >= RFM_CDMA_MAX_BAND )
  {
    RF_MSG_1 ( RF_ERROR, "rf_1x_mc_timed_sleep_rx: Invalid band %d ", band );
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }

  /*--------------------------------------------------------------------------*/
  /* Stop intelliceiver state-machine */
  /*--------------------------------------------------------------------------*/
  if ( dev_status_r->intelliceiver.ic_supported )
  {
    rf_cdma_ic_stop( ic_data_w, logical_dev );
  }

  /* Abort pending AsDiv Event */
  rf_cdma_mdsp_abort_asd_events( rxlm_handle );  
  
  /*! deallocate asd memory*/
  rflm_cmn_asd_deallocate_memory((uint32)rxlm_handle);

  /* --------------------------- END OF STATIC INIT ------------------------*/
  /* No Static de-initialization can be done after this point. Only Sleep 
     hardware sequence is executed */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* configure RF Front-end for Sleep and send RX_STOP to FW                  */
  /*--------------------------------------------------------------------------*/
  rf_cdma_data_set_script_index( device, 
                            rf_cdma_mdsp_get_device_buffer_index(RFM_1X_MODE,
                                                                 rxlm_handle) );

  if ( sleep_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe*/
    rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[dev_status_r->curr_band] );

    sleep_success &= rf_cdma_mc_configure_front_end_for_rx_sleep ( 
                       device, RFM_1X_MODE, rxlm_handle, 
                       FALSE /* is_slave_device  */, band,
                       dev_status_r->script_index, dev_status_r->rx_bw_khz,
                       dev_status_r->tx_bw_khz, logical_dev,
                       ant_tuner_token_w,
                       FALSE /* immediate_write */,
                       FALSE /* is_irat_mode */);

    if ( sleep_success == TRUE )
    {
      sleep_success = 
        rf_1x_mc_send_stop_rx_helper ( device , dev_status_r ,rxlm_handle , 
                                  logical_dev , rtc_cx1_trigger ,
                                  FALSE , /* Skip FE Programming? y/n  */ 
                                  is_offline_qpch , /* Is QPCH? y/n */
                                  FALSE ) ;
      if ( FALSE == sleep_success )
      {
        RF_MSG ( RF_ERROR , "rf_hdr_mc_timed_sleep(): " 
               "rf_1x_mc_rx_stop_helper() returned failure" ) ;
      }
    } /* if ( sleep_success == TRUE ) */
  
    /* Clean up RF CCS event smem and event handle regardless faliure or not*/
    rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );

    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( logical_dev->rx_device[band] );

  } /* if ( sleep_success == TRUE ) */

  /* Check if all Static de-initialization when through. If not, return 
  Failure. For successful execution, do the sleep hardware sequence. */
  if ( sleep_success == TRUE )
  {
    /* Do not Revert the Vreg State now. These will be done when RF_TASK 
       receives RX_STOP done message */

    /* Update CDMA Data structure with new RF State and  band/Chan */
    rf_cdma_data_set_1x_rf_state( device, RF_CDMA_STATE_SLEEP_SCHEDULED);

    /*---------------------------------------------------------------------*/
    /* Undo any SSMA operations performed on paired_device                 */
    /* NOTE: device status must be updated (as done above) before calling  */ 
    /* SSMA                                                                */ 
    /*---------------------------------------------------------------------*/
    /* device successfully put to sleep - undo any SSMA operations performed
    on the paired device... since the device is asleep, need to use
    RF_CDMA_INVALID_BAND and RF_CDMA_INVALID_CHAN while calling the SSMA
    routines */
    (void) rf_cdma_get_ssma_recos ( device, rxlm_handle, RFM_CDMA_MAX_BAND,
                                    RF_CDMA_INVALID_CHAN,
                                    logical_dev->paired_device );

    (void) rf_cdma_update_bb_ssma_recos( logical_dev->paired_device );

     rf_cdma_data_set_assoc_dev( device, RFM_INVALID_DEVICE );
    /* no wait_time required (set to 0) */
    ret_val = 0;
  }
  else
  {
    ret_val = RFM_CDMA_ERROR_FAILURE;
  }

  /* Throw F3 Message */
  RF_MSG_2( RF_MED, "rf_1x_mc_timed_sleep_rx :  Dev %d, RxLM %d", device,
            rxlm_handle);

  return ret_val;

} /* rf_1x_mc_timed_sleep_rx */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Complete the timed sleep rx procedure

  @details
  This second half of timed sleep rx will be called once RF receives the
  response from FW for RX_STOP command.
 
  This function will basically disable the VREG, and reset the CDMA data
  structure to sleep state.

  @param device
  The receive path to put to sleep.
 
  @param timed_sleep_success
  Flag indicating if the timed sleep Rx was successful in FW side or not.
 
  @param callback
  A function pointer that will be called when the entire sleep sequence
  is complete.  This function will be passed the RFM_1X_RX_TIMED_SLEEP_COMPLETE
  event when all actions associated with this API are finished.

  @param userdata
  An opaque data handle that may be used by the calling module to pass
  additional information to the callback function.
 
  @return
  Flag indicating, if the timed sleep rx is completed successfully or not
*/
rfm_wait_time_t
rf_1x_mc_complete_timed_sleep_rx
(
  const rfm_device_enum_type device,
  boolean timed_sleep_success,
  const rfm_cb_handler_type callback,
  void* const userdata
)
{
  rfm_cb_event_enum_type cb_event = RFM_EXECUTION_FAILURE; /* CB event type */
  rfm_wait_time_t ret_val = RFM_CDMA_ERROR_FAILURE;
  boolean dev_voting_status; /* Need the flag to perform device voting */

  const rf_cdma_data_status_type *dev_status_r; /* Device Status Pointer */

  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/

  /* Get Device Status Read Pointer*/
  dev_status_r = rf_cdma_get_device_status( device );

  /* Get CDMA Data Write Pointer*/
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

  /* Check if timed_sleep was successfully done by FW */
  if ( timed_sleep_success == TRUE )
  {
    /* NULL pointer check */
    if ( dev_status_r == NULL || ant_tuner_token_w == NULL )
    {
      RF_MSG( RF_ERROR, "rf_1x_mc_complete_timed_sleep_rx: NULL RF Dev data" );
      ret_val = RFM_CDMA_ERROR_NULL_DATA;
    } /* if ( dev_status_w == NULL ) */
    else
    {
      const rfc_cdma_logical_device_params_type *logical_dev =
        rfc_cdma_get_logical_device_params( device );
      const rfm_cdma_band_class_type band = dev_status_r->curr_band;

      if ( logical_dev != NULL && logical_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script( logical_dev->ant_tuner[band], 
                                   ant_tuner_token_w );
      }
      else
      {
        RF_MSG_1 ( RF_ERROR , "rf_1x_mc_complete_timed_sleep_rx(): " 
             "Null logical device for RFM device: %d" , device ) ;
      }

      if(dev_status_r->data_1x.rf_state !=  RF_CDMA_STATE_RX_PREP)
      {
        /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
        if (rfcommon_nv_get_lpm_feature_enabled())
        {
          if (logical_dev != NULL && logical_dev->rx_device[dev_status_r->curr_band] != NULL)
          {
            dev_voting_status = rfdevice_cdma_rx_pwr_sleep(
                              logical_dev->rx_device[dev_status_r->curr_band]);
		  
            RF_MSG_2( RF_MED,"rf_1x_mc_complete_timed_sleep_rx : "
                           "rfdevice_cdma_rx_pwr_sleep for band %d, status %d",
                           logical_dev->rx_device[dev_status_r->curr_band], 
                           dev_voting_status);
          }
          else
          {
             RF_MSG_1 ( RF_ERROR , "rf_1x_mc_complete_timed_sleep_rx(): " 
                        "Null logical device for RFM device: %d" , device ) ;
          }	
        }
      }

      /* RF state should be in Sleep Scheduled state to execute this function */
      if ( dev_status_r->data_1x.rf_state == RF_CDMA_STATE_SLEEP_SCHEDULED )
      {

        /* This function releases all the TQs and TQs are required to 
        queue any read\writes to CCS so this function call should be 
        after all device read or writes. */ 
        rf_cdma_mdsp_modem_rx_deallocate_resources ( device , RFM_1X_MODE ,
                                     rf_cdma_data_get_rxlm_handle (device) ) ;

        /* Vote OFF for all RF Devices which are not needed with RFM Device 
           This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS 
           settings */
        dev_voting_status = rfcommon_core_dev_power_vote(device,
                                                         RFCMN_CORE_DEV_OFF,
                                                         RFCMN_CORE_DEV_NO_VOTE);
        if( dev_voting_status == FALSE )
        {
          RF_MSG_1( RF_ERROR, "rf_1x_mc_complete_timed_sleep_rx(): "
                    "Device: %d Pwr Down failed ", device);
          ret_val = RFM_CDMA_ERROR_FAILURE;
        }
        else
        {
          ret_val = RFM_CDMA_HEALTHY_STATE;
        }
  
        /* un-vote for RFCMD app */
        {
          rf_hal_bus_client_handle* handle = 
                                rf_cdma_data_get_rf_hal_bus_vote_handle(device);
          #ifndef FEATURE_RFA_ATLAS_MODEM
          rf_hal_bus_enable (FALSE, &handle, RF_HAL_RFCMD_APP_CLIENT(1X));
          #endif
          rf_cdma_data_set_rf_hal_bus_vote_handle(device, handle);
        }

        /* Revert the Vreg State, since Rx is going to sleep */
        rfc_cdma_manage_vregs( device, RFM_1X_MODE, dev_status_r->curr_band,
                               RF_PATH_SLEEP_STATE);
      
        /* Update CDMA Data structure with new RF State and  band/Chan */
        rf_cdma_reset_sw_state ( device ) ;

        if( ret_val >= RFM_CDMA_HEALTHY_STATE )
        {
          /* Flag Callback event as successful */
          cb_event = RFM_1X_RX_TIMED_SLEEP_COMPLETE;
        }
        else if ( ret_val == RFM_CDMA_ERROR_FAILURE )
        {
          /* Flag Callback event as unsuccessful */
          cb_event = RFM_EXECUTION_FAILURE;      
        }
      }
      else
      {
        ret_val = RFM_CDMA_ERROR_INVALID_DEV_STATE;
      }
    } /* if ! ( dev_status_w == NULL ) */
  } /* if ( timed_sleep_success == TRUE ) */
  else
  {
    /* FW failed to complete the Timed Rx Sleep action successfully. Thus
    flag the callback event accordingly. */
    cb_event = RFM_1X_RX_TIMED_SLEEP_FW_FAILED;

    /* Since FW failed to put the radio to sleep, we are moving the
    RF State back to Rx State. */
    rf_cdma_data_set_1x_rf_state( device, RF_CDMA_STATE_RX );

    /* Since this is an expected behaviour from RFM API, flagging the return
    value as successful */
    ret_val = RFM_CDMA_HEALTHY_STATE;

    RF_MSG_1( RF_HIGH, "rf_1x_mc_complete_timed_sleep_rx : Dev %d - FW failed"
                       "to complete. 1xL1 will try regular sleep", device );
  } /* if ! ( timed_sleep_success == TRUE ) */

  /* Execute Callback */
  if ( callback != NULL )
  {
    callback( cb_event, userdata );
  }

  /* Throw F3 Message */
  RF_MSG_2( RF_MED,"rf_1x_mc_complete_timed_sleep_rx : [Status %d] - Dev %d",
            ret_val, device );

  return ret_val;

} /* rf_1x_mc_complete_timed_sleep_rx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enable the transmitter.

  @details
  Moves the 1x radio from the Rx state to the RxTx state.  Powers on all
  circuits required to transmit a 1x signal, and insures the transmitter
  is tuned to the correct band and channel.

  The 1x transmitter is controlled by hardware and software signals;
  calling this API does not guarantee the RF chipset will be emitting
  RF power.  The modulator hardware must also be enabled and actively
  controlling the TX_ON and PA_ON hardware controls.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_TX_WAKEUP_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.

  @param device
  The transmit path to wakeup.

  @param txlm_handle
  The Tx Link Manager buffer that shall be used to configure the
  modem hardware.

  @return
  The anticipated time in microseconds remaining from the rfm_1x_wakeup_tx()
  function return until the operation is complete.
*/
rfm_wait_time_t
rf_1x_mc_wakeup_tx
(
  const rfm_device_enum_type device,
  const rfm_device_enum_type rx_device,
  const lm_handle_type txlm_handle
)
{
  rf_time_tick_type wakeup_Tx_time_tick;        /* time spent in API */
  rf_time_type wakeup_Tx_time;
  rf_time_type FW_Resp_time=0; /* TIme spent in function */
  rf_time_tick_type FW_Resp_tick; /* TIme spent in function */
  rfm_wait_time_t ret_val; /* Return value */
  boolean wakeup_success = TRUE; /* Flag to track the success of wakeup tx */

  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rf_cdma_data_status_type *rx_dev_status_r; /* Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */

  rf_cdma_temp_comp_data_type *temp_comp_w; /* Temp comp write pointer*/
  rf_cdma_hdet_data_type *hdet_w; /* HDET write pointer*/
  rf_cdma_plim_data_type *plim_w; /* PLIM write pointer*/
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/

  rf_cdma_mdsp_tx_agc_logical_read_type tx_agc_l_read; /* TxAGC Read */
  /* <band, chan> to which device is currently tuned to */
  rfm_cdma_band_class_type band;
  rfm_cdma_chan_type chan; 
  rfdevice_cdma_dac_cal_iref_type iref_data; /* For getting Iref for TxLM */
  rf_cdma_msm_dynamic_txlm_cfg_type dynamic_cfg;
  rf_cdma_msm_static_txlm_cfg_type static_cfg;
  rfdevice_therm_read_type therm_read;
  uint32 modem_chain;
  rf_tx_resource_mgr_config_data_type txmgr_cfg;
  xpt_cfg_type xpt_cfg;
  const rf_cdma_tx_static_type *tx_static_nv_cal_ptr; /* Cal data pointer, both static and multi lin */

  /* for fbrx */
  rfcommon_fbrx_error_type fbrx_status = RFCOMMON_FBRX_SUCCESS;
  lm_handle_type fbrx_lm_handle = 0xFFFFFFFF; /* fbrx lm handle */
  uint8 fbrx_modem_chain = 3; /* WB3 is the default FBRX Chain */
  uint32 channels[RFM_CDMA_CARRIER_CUMULATIVE];
  rfcom_band_type_u fbrx_band;
  rfdevice_trx_cdma_tx* tx_device = NULL ;

  /* NV related variables */
  const rf_1x_config_type *rf_1x_config_ptr;   /* Pointer to 1x RF NV data */
  rf_cdma_mdsp_response_cb_param_type rsp_cb_param;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;

  rfcommon_fbrx_tx_param_type fbrx_tx_params;

  /* For Tx Spectral inversion */
  rfdevice_cdma_tx_spectral_inversion_type tx_spectral_inversion_data = {RFM_CDMA_MAX_BAND, FALSE};  

  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device );
  boolean pin_ret;

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_mc_wakeup_tx: NULL FWRSP Data" );
    return RFM_CDMA_ERROR_NULL_DATA;
  } /* if ( fwrsp_data == NULL ) */

    wakeup_Tx_time_tick = rf_time_get_tick();
  /* Get Device Status Read Pointer*/
  dev_status_r = rf_cdma_get_device_status( device );
  rx_dev_status_r = rf_cdma_get_device_status( rx_device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Get CDMA Data Write Pointer*/
  temp_comp_w = rf_cdma_get_mutable_temp_comp( device );
  hdet_w = rf_cdma_get_mutable_hdet( device );
  plim_w = rf_cdma_get_mutable_plim( device );
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) ||( logical_dev == NULL ) ||
       ( temp_comp_w == NULL ) ||( hdet_w == NULL ) ||
       ( plim_w == NULL ) ||( ant_tuner_token_w == NULL ) ||
       ( rx_dev_status_r == NULL ))
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_wakeup_tx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* Update Tx device STM using info from Prx device */
  rf_cdma_data_set_curr_band( device, rx_dev_status_r->curr_band );
  rf_cdma_data_set_curr_chan( device, RFM_CDMA_CARRIER_0, 
                              rx_dev_status_r->curr_chans[RFM_CDMA_CARRIER_0] );
  rf_cdma_data_set_lo_tuned_to_chan( device, 
                                     rx_dev_status_r->rf_rx_reco.rf.lo_chan);  
  rf_cdma_data_set_num_carriers( device, rx_dev_status_r->num_carriers );
  rf_cdma_data_set_rx_bw_khz(device, rx_dev_status_r->rx_bw_khz);
  rf_cdma_data_set_rf_mode(device, RFM_1X_MODE);
  rf_cdma_mdsp_set_pa_addtl_offset(RFM_1X_MODE, RF_CDMA_PA_TXAGC_ADDTL_OFFSET);

  /* Get current band */
  band = dev_status_r->curr_band;
  chan = dev_status_r->curr_chans[RFM_CDMA_CARRIER_0];

  if (band >= RFM_CDMA_MAX_BAND)
  {
    RF_MSG_2 (RF_ERROR, "rf_1x_mc_wakeup_tx: Invalid Sys band %d .. rf_band %d",
              dev_status_r->curr_band, band);
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }
  
  /* ensure that NV is valid for given device and band */
  rf_1x_config_ptr = rf_cdma_nv_get_1x_data( logical_dev->tx_nv_path[band], 
                                             band );

  if ( NULL == rf_1x_config_ptr )
  {
    RF_MSG_1 (RF_ERROR, "rf_1x_mc_wakeup_tx: Failed to retrieve valid Config" 
                        " data from NV for band %d", band); 
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  tx_static_nv_cal_ptr = rf_cdma_nv_get_tx_static( 
                           logical_dev->tx_nv_path[band], band );
  if ( tx_static_nv_cal_ptr == NULL )
  {
    RF_MSG_1 ( RF_ERROR, "rf_1x_mc_wakeup_tx: "
                     "Failed to get valid static NV pointer "
                     "for device %d", device);
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  if( ( rfm_get_calibration_state() != TRUE ) && /* If online */
      ( rf_cdma_xpt_is_enabled(device) == TRUE ) && /* xpt enabled */
      ( rf_1x_config_ptr->cdma_common_ptr->tx_cal.tx_multi_lin_cal.nv_rev_type
          != TX_MULTI_LIN_REV_3 ) /* lin ver is not rev 3 */
    )
  {
    RF_MSG_2( RF_ERROR, "rf_1x_mc_wakeup_tx : XPT is enabled. Current lin ver %d"
              " is not the required ver %d",
              (rf_1x_config_ptr->cdma_common_ptr->tx_cal.tx_multi_lin_cal.nv_rev_type+1),
              (TX_MULTI_LIN_REV_3+1) );
    return RFM_CDMA_ERROR_BAD_CONFIG;
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
    RF_MSG_1 ( RF_ERROR, "rf_1x_mc_wakeup_tx(): "
               " rf_cdma_mdsp_modem_tx_allocate_resources() Failed to for device %d " , 
               device ) ;
    return RFM_CDMA_ERROR_FAILURE ;
  }

  tx_device = logical_dev->tx_device[band] ;
  if ( NULL == tx_device )
  {
    RF_MSG_2 ( RF_ERROR, "rf_1x_mc_wakeup_tx(): "
               " tx_device is NULL for device: %d , band: %d " , 
               device , band ) ;
    return RFM_CDMA_ERROR_FAILURE ;
  }

  /* Set Modem Chain value */
  modem_chain = rfc_cdma_get_tx_modem_chain(device, band);

  /* set txlm in rf_cdma_data */
  rf_cdma_data_set_txlm_handle( device, txlm_handle);

  /* Identify if EPT/ET is enabled for Tx operation */
  if ( rf_cdma_xpt_is_enabled(device) == TRUE )
  {
    rf_cdma_data_set_ept_online_enabled( device, TRUE );
  }
  else
  {
    rf_cdma_data_set_ept_online_enabled( device, FALSE );
  }

  if ( rf_cdma_data_get_fbrx_enable (RFM_1X_MODE) == TRUE )
  {
  /* Allocate FBRx DM buffers for given FBRxLM handle */
  fbrx_status = rfcommon_fbrx_mc_enter_mode( &fbrx_lm_handle, RFM_1X_MODE, 0);
  if(fbrx_status == RFCOMMON_FBRX_ERROR)
  {
    RF_MSG(RF_ERROR, "rf_1x_mc_wakeup_tx: FBRx DM buffer allocation failed");
    wakeup_success = FALSE;
  }
  else
  {
    /* Update fbrx_lm_handle in rf_cdma_data */
    rf_cdma_data_set_fbrx_lm_handle( device , fbrx_lm_handle );
    RF_MSG_1(RF_MED, "rf_1x_mc_wakeup_tx: FBRx DM buffer allocated handle %d",
                      fbrx_lm_handle);
  }
  } 

  /*--------------------------------------------------------------------------*/
  /* Enable Chip Power for all devices in the receiver - Device, RxAGC, RFCard*/
  /*--------------------------------------------------------------------------*/
  rfc_cdma_manage_vregs(device, RFM_1X_MODE, band, RF_PATH_RXTX_STATE);
 
  /*--------------------------------------------------------------------------*/
  /* Do therm read before configuring TX AGC*/
  /*--------------------------------------------------------------------------*/
  rfdevice_cdma_tx_cmd_dispatch( tx_device ,
                                 RFDEVICE_GET_THERMISTER_VALUE,
                                 &therm_read );
  
  temp_comp_w->last_temp_sensor_read = therm_read.temperature;
  temp_comp_w->is_last_reading_valid = therm_read.valid_temp_read;
    
  RF_MSG_2( RF_LOW, "rf_1x_mc_wakeup_tx: therm reading: %d, and %d",
            therm_read.temperature, therm_read.valid_temp_read ); 

  /* Update the temp comp algo for the first read */ 
  wakeup_success &= rf_cdma_temp_comp_update_algo( temp_comp_w,
                                                   logical_dev, band );

  /* Force isHKADCbusy to FALSE*/
  rf_cdma_data_set_isHKADCbusy( device, FALSE );

  /*--------------------------------------------------------------------------*/
  /* Configure TxLM */
  /*--------------------------------------------------------------------------*/

  /* Get Iref info from RF Device */
  iref_data.band = band;
  rfdevice_cdma_tx_get_dac_cal_iref( tx_device , &iref_data ) ;

  dynamic_cfg.iref_val = iref_data.iref_val;
  dynamic_cfg.num_carriers = dev_status_r->num_carriers;
  dynamic_cfg.tx_modem_chain = 
    rfc_cdma_get_tx_modem_chain ( device , dev_status_r->curr_band ) ;
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
  
  wakeup_success &= rf_cdma_msm_configure_txlm( rfdevice_id_get(tx_device),
                                                txlm_handle, 
                                                RFM_1X_MODE, 
                                                device,
                                                dev_status_r->curr_band, 
                                                static_cfg,
                                                dynamic_cfg);
  static_cfg.xpt = xpt_cfg;

  /* Retrieve the default TxLM IQ gain */
  rf_cdma_data_set_cdma_iq_gain_val( device, 
      (int16) rf_cdma_msm_get_default_txc_gain ( dynamic_cfg.tx_modem_chain,
                                                 txlm_handle ) );
  rf_cdma_data_set_ept_iq_gain_val( device,  
      (int16) rf_cdma_xpt_get_cfg_param( device, EPT_CFG_CMN_IQ_GAIN_UNITY ) );

  /*--------------------- FBRx Settings ----------------------------------*/
  if (( wakeup_success == TRUE ) && ( rf_cdma_data_get_fbrx_enable (RFM_1X_MODE) == TRUE ))
  {
    /* Allocate Tx DM buffers for the given FBRx LM handle */
    fbrx_lm_handle =  rf_cdma_data_get_fbrx_lm_handle(device);
    fbrx_status = rfcommon_fbrx_mc_init_tx( fbrx_lm_handle, 
                                            RFCOMMON_FBRX_1X, 
                                            txlm_handle,
                                            RFM_1X_MODE,
                                            0 );

    fbrx_status &= rfcommon_fbrx_mc_register_vswr_meas_call_back ( 
         rflm_cdma_post_process_fbrx_vswr_measurements , txlm_handle ) ;

    rf_1x_mdsp_update_fbrx_flag(TRUE);

    /* Updates FBRx frequency-dependent values */
    /* @todo: remove hard-coding, tech to update the fbrx chain from rfc */
    channels[0] = (uint32)chan;  
    fbrx_band.cdma_band = band;
    fbrx_tx_params.apt_only_mode = FALSE;	
    fbrx_modem_chain = rfc_cdma_get_fbrx_modem_chain(device, band);
    fbrx_status &= rfcommon_fbrx_mc_tune_to_chan( 
                           device,
                           txlm_handle, 
                           RFM_1X_MODE, 
                           fbrx_band,
                           channels,
                           RFCOMMON_FBRX_1_CARRIER,
                           fbrx_lm_handle,
                           fbrx_modem_chain,
                           fbrx_tx_params,
                           RFCOMMON_FBRX_NV_CONTAINER_C0 );
  }
  else
  {
    rf_1x_mdsp_update_fbrx_flag(FALSE);
  }

  //Allocate static PDMEM for TxAGC
  rflm_c2k_allocate_txagc_ccs_mem(RFLM_TECH_1X);

 
  /*--------------------------------------------------------------------------*/
  /* Configure TxAGC */
  /*--------------------------------------------------------------------------*/
  wakeup_success &= rf_1x_configure_tx_agc( device, band, 
                                  dev_status_r->curr_chans[RFM_CDMA_CARRIER_0],
                                  txlm_handle,
                                  rf_1x_config_ptr );

  /*--------------------------------------------------------------------------*/
  /* Compute RF Front-end for Wakeup and send TX_START to FW                  */
  /*--------------------------------------------------------------------------*/
  rf_cdma_data_set_script_index( device, 
                            rf_cdma_mdsp_get_device_buffer_index(RFM_1X_MODE,
                                                                 txlm_handle) );

  if ( wakeup_success == TRUE )
  {

    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_tx_enter_critical_section ( tx_device ) ;

    wakeup_success &= rf_cdma_mc_configure_front_end_for_tx_wakeup (
                               device, txlm_handle, RFM_1X_MODE, band, chan, 
                               dev_status_r->rx_bw_khz, RF_CDMA_BW_1X, 
                               dev_status_r->script_index, logical_dev,
                               tx_static_nv_cal_ptr,
                               ant_tuner_token_w,
                               FALSE );

     /*--------------------- FBRx Settings ----------------------------------*/
    if (( wakeup_success == TRUE ) && ( rf_cdma_data_get_fbrx_enable (RFM_1X_MODE) == TRUE ))
    {
      fbrx_lm_handle =  rf_cdma_data_get_fbrx_lm_handle(device);

	  RF_MSG_1(RF_FATAL, "rf_1x_mc_wakeup_tx: "
                           "FBRx enabled %d", fbrx_lm_handle);
      /* Set the pout comp flag that FBRx is enabled. */
      rf_cdma_set_pout_comp_skip ( device , TRUE ) ;
  
      /* Writes the FBRx modem settings to HW */
      fbrx_modem_chain = rfc_cdma_get_fbrx_modem_chain(device, band);
      fbrx_status &= rfcommon_fbrx_enable( fbrx_lm_handle, 3 );
      if( fbrx_status == RFCOMMON_FBRX_ERROR )
      {
        RF_MSG_1(RF_FATAL, "rf_1x_mc_wakeup_tx: "
                           "FBRx enable failed: handle %d", fbrx_lm_handle);
      }
    }

    if ( wakeup_success == TRUE )
    {
      rf_cdma_mdsp_tx_start_type tx_start_c;
      tx_start_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_WAKEUP_TX_ID;

      tx_start_c.txlm_handle = txlm_handle; /* TXLM Handle */
	  tx_start_c.fbrx_lm_handle = fbrx_lm_handle; /* FBRX LM Handle */

      FW_Resp_tick = rf_time_get_tick();
      /* Send Command to MDSP to start Tx */
      fwrsp_hk->tx_start_rsp.device = device;
      fwrsp_hk->tx_start_rsp.sema_token = rfcommon_semaphore_pop_item();
      rsp_cb_param.rsp_handler = rf_1x_tx_start_rsp_cb;
      rsp_cb_param.handler_data = &fwrsp_hk->tx_start_rsp;

      wakeup_success = rf_1x_mdsp_start_tx( logical_dev->tx_mdsp_path, 
                                    &tx_start_c, &rsp_cb_param, NULL, NULL ); 

      /* Sending of Tx Start to FW failed */
      if ( wakeup_success == FALSE )
      {
        RF_MSG ( RF_ERROR, "rf_1x_mc_wakeup_tx(): rf_1x_mdsp_start_tx failed "); 
        /* Message sending failed, free the semaphore */
        RF_MSG_2 ( RF_LOW, "rf_1x_mc_wakeup_tx():"
                   " Freeing the semaphore and not waiting "   
                   " [D%d] TX_START pushed back : Sem @ 0x%x" , 
                   device , fwrsp_hk->tx_start_rsp.sema_token ) ;
      }
      else
      {
        /* Message was sent to FW, wait for semaphore */
        RF_MSG_2( RF_LOW, "[1X][RF-FW Sync] [D%d] TX_START : Start Wait Sem @ "
                          "0x%x", device, fwrsp_hk->tx_start_rsp.sema_token );
        rfcommon_semaphore_wait( fwrsp_hk->tx_start_rsp.sema_token );
      }

      FW_Resp_time = rf_time_get_elapsed( FW_Resp_tick, RF_USEC );
      /* Push back semaphore after wait if mssg was sent successfully,
      or immediately if mssg sending failed*/
      rfcommon_semaphore_push_item( fwrsp_hk->tx_start_rsp.sema_token );

      if ( logical_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script( logical_dev->ant_tuner[band], 
                                   ant_tuner_token_w );
      }
      xpt_cfg = rf_cdma_xpt_get_operation_mode(device);
      if ( ( xpt_cfg == EPT_CFG ) ||
          ( xpt_cfg == ET_CFG )  ||
          ( xpt_cfg == EPT_ET_CFG ) )
      {
        /* Enable the Tx device for xPT during wakeup */
        wakeup_success &= rf_1x_xpt_configure( 
                                  device,
                                  band,
                                  chan,
                                  logical_dev->tx_nv_path[band],
                                  txlm_handle,
                                  xpt_cfg );
      }
    } /* if ( wakeup_success == TRUE ) */

    /* Clean up RF CCS event smem and event handle regardless faliure or not*/
    rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );

    /* Leave Device Critical Section */
    rfdevice_cdma_tx_leave_critical_section( tx_device ) ;


  } /* if ( wakeup_success == TRUE ) */

  /* Check if all Static Initialization went through. If not, return Failure.
     For successful execution, do the Tx Wakeup hardware sequence. */

  if ( wakeup_success == TRUE )
  {
    int32 dummy_result;

    /* Tell GPS that 1x Tx is on. */
    RFGNSS_MC(wwan_tx_status, dummy_result)(device, TRUE);
    if ( dummy_result == FALSE )
    {
      RF_MSG( RF_ERROR, 
              "rf_1x_mc_wakeup_tx: rfgnss_mc API failed !!" );
    }

    /*------------------------------------------------------------------------*/
    /* Start 1x Power limiting and HDET */
    /*------------------------------------------------------------------------*/
    if ( rf_1x_plim_start(device, logical_dev, plim_w, band) == TRUE)
    {
      /* Start HDET only if Power Limiting was started successfully. Else no 
      point doing HDET reads */
      if ( rf_cdma_hdet_start(device, hdet_w, RFM_1X_MODE,
                              logical_dev, band, chan) == FALSE )
      {
          /* Clean up HDET trig event in case of hdet start failure */
          rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_EVENT_HDET_CONFIG );

          RF_MSG_3( RF_ERROR, "rf_1x_mc_wakeup_tx: HDET start failed on device "
                              "%d, band: %d, chan: %d", device, band, chan );
      }
      else
      {
      /* Update the initial HDET vs Temp params based on this initial therm read */
      rf_cdma_hdet_update_temperature_params( hdet_w, 
                            dev_status_r->temp_comp.last_temp_sensor_read );
      }

      /* Update lim_vs_temp_vs_freq */
      rf_1x_plim_update_lim_vs_temp_vs_freq( 
                            device,
                            plim_w,
                            dev_status_r->plim.band,
                            dev_status_r->lo_tuned_to_chan,
                            dev_status_r->temp_comp.last_temp_sensor_read );
    }
    else
    {
      RF_MSG_2( RF_HIGH, "rf_1x_mc_wakeup_tx: 1x PLIM not started on device %d,"
                  "band: %d", device, band );
    }

    /*------------------------------------------------------------------------*/
    /* Start 1x temp comp routine */
    /*------------------------------------------------------------------------*/
    if ( rf_1x_temp_comp_start( temp_comp_w, RFM_1X_MODE,
                                logical_dev, band, chan) == FALSE )
    {
      RF_MSG( RF_ERROR, "rf_1x_mc_wakeup_tx: temp comp start failed ");
    }
    else
    {
      RF_MSG_1( RF_MED, "rf_1x_mc_wakeup_tx: temp comp starts at device %d", 
                device); 
    }
    /*------------------------------------------------------------------------*/
    /* Update CDMA Data structure with new the RF State */
    if ( rf_cdma_data_get_1x_rf_state(device) == RF_CDMA_STATE_RX )
    {
    rf_cdma_data_set_1x_rf_state( device, RF_CDMA_STATE_RXTX );
    }
    else
    {
      rf_cdma_data_set_1x_rf_state(device, RF_CDMA_STATE_TX); 
    }
    rf_cdma_data_set_rf_mode( device, RFM_1X_MODE );
    /* Populate the Tx Bandwidth */
    rf_cdma_data_set_tx_bw_khz( device, RF_CDMA_BW_1X );

    /* Build script for 1x Autopin  */
    /*------------------------------------------------------------------------*/
	if (rfcommon_autopin_is_enabled(RFM_1X_MODE))
	  {
		 if(RFCOMMON_AUTOPIN_ERROR == rfcommon_autopin_mc_enable_tx(txlm_handle, RFM_1X_MODE, NULL))
		 {
		   RF_MSG(RF_ERROR, "rf_1x_mc_wakeup_tx: CDMA 1X AutoPin enable TX failed!");
		   fbrx_status &= FALSE;
		 }
		 else
		 {
		   rf_cdma_auto_pin_tx_wakeup(device, RFM_1X_MODE);
	
	   //Update the DM memory for AutoPin
	   (void)rf_cdma_autopin_update_dm (
							   device,
					   logical_dev,    
				   RFM_1X_MODE,
					   txlm_handle,
							   band, 
				   chan);
		 }
	  }
	  else
	  {
		 RF_MSG(RF_ERROR, "rf_1x_mc_wakeup_tx: CDMA 1X AutoPin is not enaBLED!");
	  }

      if ( rfcommon_autopin_is_enabled(RFM_1X_MODE) &&
	   rf_cdma_autopin_par_support(
                              device, 
	 	              logical_dev, 
	 	              RFM_1X_MODE, 
	 	              band ) == TRUE)
      {
      	  pin_ret = rf_cdma_configure_autopin_trigger (
   				    device,
  				    RFM_1X_MODE,
				    logical_dev,
			    	    band);
	  if (pin_ret == FALSE)
    	  {
            RF_MSG( RF_ERROR, "rf_1x_mc_wakeup_tx: pin configure failed ");
		  
    	  }
        }

    /* Populate the total wait time */
    ret_val = 0;

    /* Get TxAGC Data for reporting */
    rf_1x_mdsp_get_tx_agc_logical_data( logical_dev->tx_mdsp_path, 
                                        dev_status_r->txlm_handle, 
                                        &tx_agc_l_read );

    rf_cdma_data_sanity_check( device );

    if ( rfcommon_autopin_is_enabled(RFM_1X_MODE) &&
	 rf_cdma_autopin_par_support(device, 
       	 	                logical_dev, 
	 	                RFM_1X_MODE, 
	 	                    band ))
    {
      /* Set the pout comp flag */	   
      rf_cdma_set_pout_comp_skip ( device , TRUE ) ;
    }

    /* Report wakeup time, Tx AGC and PA state */
    RF_MSG_5( RF_MED, "rf_1x_mc_wakeup_tx : Dev %d, Band %d, Chan %d, "
              "PA State %d, TxTotal %d", device, band,
              dev_status_r->curr_chans[RFM_CDMA_CARRIER_0], 
              tx_agc_l_read.pa_state, tx_agc_l_read.total_power );
	
    /* Store the associated master device for Tx */
    rf_cdma_data_set_assoc_master_dev(device, rx_device);
  } /* ( wakeup_success == TRUE ) */
  else
  {
    /* Revert the Vreg State, since Tx is not waking up */
    rfc_cdma_manage_vregs( device, RFM_1X_MODE, band, RF_PATH_RX_STATE);

    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      /* Sleep the WTR for tx mode , since wakeup tx failed */
      wakeup_success = rfdevice_cdma_tx_pwr_sleep(logical_dev->tx_device[band]);
    }

    /* set txlm in rf_cdma_data */
    rf_cdma_data_set_txlm_handle( device, RFM_INVALID_LM_BUFFER);

    if ( rf_cdma_data_get_fbrx_enable (RFM_1X_MODE) == TRUE )
    {
    /* Deallocate FBRx DM buffers for given FBRxLM handle  */
    fbrx_status = rfcommon_fbrx_mc_exit( dev_status_r->fbrx_lm_handle,
                                         RFM_1X_MODE );

    /* Reset the pout comp flag that FBRx is Disabled. */
    rf_cdma_set_pout_comp_skip ( device , FALSE ) ;

    RF_MSG_1( RF_MED, "rf_1x_mc_wakeup_tx: Failed wakeup: "
                      "Calling rfcommon_fbrx_mc_exit with "
              "fbrx_handle %d, RFM_1X_MODE", dev_status_r->fbrx_lm_handle );

    if(fbrx_status == RFCOMMON_FBRX_ERROR)
    {
      wakeup_success = FALSE;
      RF_MSG(RF_ERROR, "rf_1x_mc_wakeup_tx: FBRx DM buffer deallocation failed");
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

    /* Report wakeup time, Tx AGC and PA state */
    RF_MSG_2( RF_ERROR, "rf_1x_mc_wakeup_tx: [Status %d] - Dev %d ",
              ret_val, device );
  }
  wakeup_Tx_time = rf_time_get_elapsed( wakeup_Tx_time_tick, RF_USEC );  
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
   prof_hist_update(&tx_1x_wakeup_Tx_time_hist,(uint32) wakeup_Tx_time);
   prof_hist_update(&tx_1x_wakeup_fw_resp_hist,(uint32)FW_Resp_time );
   if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
   {
     rf_1x_clear_profiling_hist_packet();
   }
  #endif
  return ret_val;

} /* rf_1x_mc_wakeup_tx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Disable the transmitter.

  @details
  Moves the 1x radio from the RxTx state to the Rx state.  Any circuits
  that are no longer needed are powered down to conserve current.

  This API will disable the RF ASIC transmitter, and prevent any power
  from being radiated from the device independent of the TX_ON and PA_ON
  control signals.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_TX_SLEEP_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.

  @param device
  The transmit path to put to sleep.

  @param txlm_handle
  The Tx Link Manager buffer that shall be used to configure the
  modem hardware.

  @return
  The anticipated time in microseconds remaining from the rfm_1x_sleep_tx()
  function return until the operation is complete. Returns a 0 on success (no
  wait time needed) and a negative value on failure
*/
rfm_wait_time_t
rf_1x_mc_sleep_tx
(
  const rfm_device_enum_type device,
  const lm_handle_type txlm_handle
)
{
  rf_time_tick_type sleep_tx_tick;        /* time spent in API */
  rf_time_type sleep_tx_time;
  rf_time_type FW_Resp_time=0; /* TIme spent in function */
  rf_time_tick_type FW_Resp_tick; /* TIme spent in function */

  rfm_wait_time_t ret_val = 0; /* Return value */
  boolean sleep_success = TRUE; /* Flag to track the success of wakeup tx */
  rfm_cdma_band_class_type band;  /* Band */

  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */

  rf_cdma_temp_comp_data_type *temp_comp_w; /* Temp comp write pointer*/
  rf_cdma_hdet_data_type *hdet_w; /* HDET write pointer*/
  rf_cdma_plim_data_type *plim_w; /* PLIM write pointer*/
  ant_tuner_device_tokens_type* ant_tuner_token_w; /*Tuner token write pointer*/

  uint32 modem_chain;
  rf_cdma_mdsp_response_cb_param_type rsp_cb_param;
  rf_cdma_fwrsp_data_type *fwrsp_hk = NULL;
  sleep_tx_tick= rf_time_get_tick();

  /* Get FW Response Data */
  fwrsp_hk = rf_cdma_get_fwrsp_data( device );

  /* NULL Pointer Check */
  if ( fwrsp_hk == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_mc_sleep_tx: NULL FWRSP Data" );
    return RFM_CDMA_ERROR_NULL_DATA;
  } /* if ( fwrsp_data == NULL ) */

  /* Get Device Status Read Pointer*/
  dev_status_r = rf_cdma_get_device_status( device );
  
  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  /* Get CDMA Data Write Pointer*/
  temp_comp_w = rf_cdma_get_mutable_temp_comp( device );
  hdet_w = rf_cdma_get_mutable_hdet( device );
  plim_w = rf_cdma_get_mutable_plim( device );
  ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( device );

  /* Perform NULL Pointer check */
  if ( ( dev_status_r == NULL ) || ( logical_dev == NULL ) ||
       ( temp_comp_w == NULL ) ||( hdet_w == NULL ) ||
       ( plim_w == NULL ) ||( ant_tuner_token_w == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_sleep_tx: NULL data for device %d",
              device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }
  
  /*-------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*-------------------------------------------------------------------------*/

  /* Populate Current band and chan */
  band = dev_status_r->curr_band;
  
  /* Set Modem Chain value */
  modem_chain = rfc_cdma_get_tx_modem_chain(device, band);
  
  /*--------------------------------------------------------------------------*/
  /* Stop Power Limiting */
  /*--------------------------------------------------------------------------*/
  sleep_success &= rf_1x_plim_stop( device, plim_w );
  
  /*--------------------------------------------------------------------------*/
  /* Stop 1x temp comp routine*/
  /*--------------------------------------------------------------------------*/
  if ( rf_cdma_debug_flags.disable_temp_comp == 0 )
  {
    sleep_success &= rf_1x_temp_comp_stop( temp_comp_w );
  }

  /*--------------------------------------------------------------------------*/
  /* Stop HDET */
  /*--------------------------------------------------------------------------*/
  sleep_success &= rf_cdma_hdet_stop( device, hdet_w );

  RF_MSG_1( RF_MED, "rf_1x_mc_sleep_tx(): stop temp_comp timer %d", 
            sleep_success ); 

  /* Force isHKADCbusy to FALSE*/
  rf_cdma_data_set_isHKADCbusy( device, FALSE );
  
  /* --------------------------- END OF STATIC INIT ------------------------*/
  /* No Static de-initialization can be done after this point. Only wakeup 
     hardware sequence is executed */
  /*--------------------------------------------------------------------------*/
  
  /*--------------------------------------------------------------------------*/
  /* Configure RF-front-end for Sleep and send RX_STOP to FW                  */
  /*--------------------------------------------------------------------------*/
  rf_cdma_data_set_script_index( device, 
                          rf_cdma_mdsp_get_device_buffer_index( RFM_1X_MODE,
                                                                txlm_handle ) );

  if ( sleep_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_tx_enter_critical_section( logical_dev->tx_device[band] );

    sleep_success &= rf_cdma_mc_configure_front_end_for_tx_sleep ( 
                                   device,
                                   dev_status_r->curr_band,
                                   dev_status_r->script_index, 
                                   logical_dev,
                                   ant_tuner_token_w,
                                   FALSE );

    if( dev_status_r->ept_online_enabled == TRUE )
    {
      sleep_success &= rf_common_xpt_init_dpd( device, NULL );
    }

    if (sleep_success == TRUE)
    {
      rf_cdma_mdsp_tx_stop_type tx_stop_c; /* TxAGC Stop Config */

      tx_stop_c.script_index = (uint32)RFLM_CDMA_MDSP_CCS_EVENT_SLEEP_TX_ID;

      tx_stop_c.txlm_handle = txlm_handle; /* TXLM Handle */

      FW_Resp_tick = rf_time_get_tick();
      /* Send Command to MDSP to stop Tx */
      fwrsp_hk->tx_stop_rsp.device = device;
      fwrsp_hk->tx_stop_rsp.sema_token = rfcommon_semaphore_pop_item();
      rsp_cb_param.rsp_handler = rf_1x_tx_stop_rsp_cb;
      rsp_cb_param.handler_data = &fwrsp_hk->tx_stop_rsp;

      sleep_success = rf_1x_mdsp_stop_tx( logical_dev->tx_mdsp_path, 
                                          &tx_stop_c, &rsp_cb_param, NULL, 
                                          NULL );

      /* Sending of Tx Stop to FW failed */
      if ( sleep_success == FALSE )
      {
        RF_MSG ( RF_ERROR, "rf_1x_mc_sleep_tx(): rf_1x_mdsp_stop_tx failed "); 
        /* Message sending failed, free the semaphore */
        RF_MSG_2 ( RF_LOW, "rf_1x_mc_sleep_tx():"
                   " Freeing the semaphore and not waiting "   
                   " [D%d] TX_STOP pushed back : Sem @ 0x%x" , 
                   device , fwrsp_hk->tx_stop_rsp.sema_token ) ;
      }
      else
      {
        /* Message was sent to FW, wait for semaphore */
        RF_MSG_2( RF_LOW, "[1X][RF-FW Sync] [D%d] TX_STOP : Start Wait Sem @ "
                          "0x%x", device, fwrsp_hk->tx_stop_rsp.sema_token );
        rfcommon_semaphore_wait( fwrsp_hk->tx_stop_rsp.sema_token );
      }

      FW_Resp_time = rf_time_get_elapsed( FW_Resp_tick, RF_USEC );
      /* Push back semaphore after wait if mssg was sent successfully,
      or immediately if mssg sending failed*/
      rfcommon_semaphore_push_item( fwrsp_hk->tx_stop_rsp.sema_token );
    } /* if ( sleep_success == TRUE ) */

    /* Clean up static event smem and event handle regardless faliure or not*/
    rf_cdma_mc_cleanup_ccs_event( device, RF_CDMA_STATIC_EVENT );

    /* Leave Device Critical Section */
    rfdevice_cdma_tx_leave_critical_section( logical_dev->tx_device[band] );
    
    /* [BEGIN] ---------- FRBX related operations ------- */    
    if ( rf_cdma_data_get_fbrx_enable (RFM_1X_MODE) == TRUE )
    {
      rfcommon_fbrx_error_type fbrx_status; /* fbrx opreation status */
      /* Reset the pout comp flag. */
      rf_cdma_set_pout_comp_skip ( device , FALSE ) ;

	  /* clear the FBRx DM state variables */
	  rfcommon_fbrx_mc_deinit_tx(dev_status_r->fbrx_lm_handle);

      /* Deallocate FBRx DM buffers for given FBRxLM handle  */
      fbrx_status = rfcommon_fbrx_mc_exit( dev_status_r->fbrx_lm_handle,
                                           RFM_1X_MODE );

      RF_MSG_1( RF_MED, "rf_1x_mc_sleep_tx: Calling rfcommon_fbrx_mc_exit with "
                "fbrx_handle %d, RFM_1X_MODE", dev_status_r->fbrx_lm_handle );

           
      if(fbrx_status == RFCOMMON_FBRX_ERROR)
      {
        sleep_success = FALSE;
        RF_MSG( RF_ERROR, 
                "rf_1x_mc_sleep_tx: FBRx DM buffer deallocation failed" );
      }
      else
      {
        /* Update fbrx_lm_handle in rf_cdma_data */
        rf_cdma_data_set_fbrx_lm_handle( device , RFLM_DM_INVALID_HANDLE_ID );
      }

    } /* ---------- FRBX related operations ------- [END] */    
	
    if (rfcommon_autopin_is_enabled(RFM_1X_MODE))
    {
       rfcommon_fbrx_error_type fbrx_status;
       if(RFCOMMON_AUTOPIN_ERROR == 
                rfcommon_autopin_mc_disable_tx( dev_status_r->txlm_handle, RFM_1X_MODE ))
       {
          RF_MSG(RF_ERROR, "rf_1x_mc_sleep_tx: CDMA 1X AutoPin enable TX failed!");
          fbrx_status = RFCOMMON_FBRX_ERROR;
        }
        else
        {
          /* Stop Autopin */
          rf_cdma_auto_pin_tx_sleep(device, RFM_1X_MODE);
          rf_cdma_autopin_stop( device );
        }
     }
  } /* if ( sleep_success == TRUE ) */

  /* Deallocate RFLM Tx resources: Abort any pending AOL event */
  rf_cdma_mdsp_modem_tx_deallocate_resources( RFM_1X_MODE, txlm_handle );

  /* Check if all Static Initialization when through. If not, return Failure.
     For successful execution, do the Rx sleep hardware sequence. */
  if ( sleep_success == TRUE )
  {
    int32 dummy_result;
    boolean device_power_down_flag;

    /* Reset the pout comp flag. */
    rf_cdma_set_pout_comp_skip ( device , FALSE ) ;

    /* Revert the Vreg State, since Tx is going to sleep */
    rfc_cdma_manage_vregs( device, RFM_1X_MODE, dev_status_r->curr_band,
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
      RF_MSG_1( RF_ERROR,"rf_1x_mc_sleep_tx: rfdevice_cdma_tx_pwr_sleep API "
              "failed for device: %d ",device ); 
    }

    /* Update CDMA Data structure with new the RF State */
    if ( rf_cdma_data_get_1x_rf_state(device) == RF_CDMA_STATE_RXTX )
    {
    rf_cdma_data_set_1x_rf_state( device, RF_CDMA_STATE_RX);
    }
    else /* If in Tx only state, reset SW state machine */
    {
      rf_cdma_reset_sw_state( device );
    }

    rf_cdma_data_set_traffic_state( device, FALSE );
    rf_cdma_data_set_txlm_handle( device, RFM_INVALID_LM_BUFFER );
    rf_cdma_data_set_tx_bw_khz( device, RF_CDMA_BW_INVALID );

    /* Tell GPS that 1x Tx is off. */
    RFGNSS_MC(wwan_tx_status, dummy_result)(device, FALSE);
    if ( dummy_result == FALSE )
    {
      RF_MSG( RF_ERROR, 
              "rf_1x_mc_sleep_tx: rfgnss_mc API failed !!" );
    }

    rf_cdma_data_sanity_check( device );

    //DeAllocate static PDMEM for TxAGC
    rflm_c2k_deallocate_txagc_ccs_mem(RFLM_TECH_1X);

    /* Reset the associated master device for Tx to invalid */
    rf_cdma_data_set_assoc_master_dev(device, RFM_INVALID_DEVICE);

    /* no wait_time needed (set to 0)*/ 
    ret_val = 0;
  } /* if (sleep_success == TRUE) */
  else
  {
    ret_val = RFM_CDMA_ERROR_FAILURE;

    /* Throw error Message */
    RF_MSG_2( RF_ERROR, "rf_1x_mc_sleep_tx: [Status %d] Dev %d", 
              ret_val, device ); 
  }

   sleep_tx_time = rf_time_get_elapsed( sleep_tx_tick, RF_USEC );
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&tx_1x_sleep_Tx_time_hist,(uint32)sleep_tx_time );
    prof_hist_update(&tx_1x_sleep_fw_resp_hist,(uint32)FW_Resp_time);
    if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
    {
      rf_1x_clear_profiling_hist_packet();
    }
  #endif  
  return ret_val;

} /* rf_1x_mc_sleep_tx */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Associate a new receive device with a primary control device for
  diversity operation.

  @details
  This API links the 1x state of slave_dev to the state of device1.
  Device2 will track the operating frequency and Sleep/Rx state of
  device1.  1x L1 need not make any calls to directy control slave_dev
  until diversity mode is terminated with a call to
  rfm_1x_disable_diversity().

  If master_dev is in Rx or RxTx mode, slave_dev is woken up if necessary,
  and linked to the operating frequency of device1.  If device1 is
  in sleep mode, slave_dev remains in sleep mode and will be woken up
  when device1 is woken up.

  Any tuning opeations on device1 will be mirrored on slave_dev while
  they are linked in diversity mode.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_ENABLE_DIV_COMPLETE event.  The function will attempt to give
  a best estimate of the time remaining in the return value.

  @param master_dev
  The radio device that will be the master.  Device2 is linked as a
  slave to this device.

  @param rxlm_handle_master_dev
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware for device1, if necessary.

  @param slave_dev
  The radio device that will be slaved to device1.

  @param rxlm_handle_slave_dev
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware for slave_dev.
 
  @return
  The anticipated time in microseconds remaining from the
  rfm_1x_enable_diversity() function return until the operation is
  complete.

*/
rfm_wait_time_t
rf_1x_mc_enable_diversity
(
  const rfm_device_enum_type master_dev,
  const lm_handle_type rxlm_handle_master_dev,
  const rfm_device_enum_type slave_dev,
  const lm_handle_type rxlm_handle_slave_dev,
  boolean is_irat_mode,
  const rfdevice_meas_scenario_type meas_type
)
{
  rfm_wait_time_t ret_val; /* Return value */
  rfm_cdma_power_mode_type power_mode; /* var to store Power Mode */
  uint32 adc_clock_freq; /* var to store ADC Clock Frequency */
  boolean wakeup_success = TRUE; /* Flag to track the success of enable_div */ 
  rf_1x_mdsp_rx_agc_read_type rx_agc_read;/* RxAGC Data */

  const rf_cdma_data_status_type *m_dev_status; /* Master device status */
  const rf_cdma_data_status_type *s_dev_status; /* Slave device status */
  rf_cdma_data_status_type *s_dev_status_w;/* Mutable Slave device status */
  const rfc_cdma_logical_device_params_type *logical_m_dev; /* Logical Master Device */
  const rfc_cdma_logical_device_params_type *logical_s_dev; /* Logical Slave Device */

  rf_cdma_ic_data_type *s_ic_data_w; /* Intelliceiver write pointer*/
  ant_tuner_device_tokens_type* s_ant_tuner_token_w; /*Slave tuner token write pointer*/

  rxf_get_iqmc_coef_type iqmc_data; /* IQMC Coefficient */  
  rf_cdma_msm_static_rxlm_cfg_type static_cfg; /* Static RXLM  Settings */
  rf_cdma_msm_dynamic_rxlm_cfg_type dyn_settings; /* Dynamic RXLM  Settings */
  const rf_1x_config_type* cdma_config_ptr; /* Pointer to NV data */
  rf_1x_agc_param_type agc_mode_info; /* 1x RxAGC Mode info */

  /* Flag to track the success of wakeup rx */
  boolean dev_voting_status;

  rf_time_tick_type prof_vreg_t;      /* Vreg Time Profile */
  rf_time_tick_type prof_rxlm_t;      /* RXLM Time Profile */
  rf_time_tick_type prof_rxagc_cfg_t; /* RxAGC Config Time Profile */
  rf_time_tick_type enable_div_time;   /* Time spent in function */
  rf_time_type prof_vreg_t_val=0;      /* Vreg Time Profile */
  rf_time_type prof_rxlm_t_val;      /* RXLM Time Profile */
  rf_time_type prof_rxagc_cfg_t_val; /* RxAGC Config Time Profile */
  rf_time_type enable_div_time_val;  /* Time spent in function */
  rfm_cdma_band_class_type band;
  enable_div_time=rf_time_get_tick();
  /* Get Device Status for master and slave devices */
  m_dev_status = rf_cdma_get_device_status( master_dev );
  s_dev_status = rf_cdma_get_device_status( slave_dev );
  s_dev_status_w = rf_cdma_get_mutable_device_status( slave_dev );

  /* Get Logical Devices Param */
  logical_m_dev = rfc_cdma_get_logical_device_params( master_dev );
  logical_s_dev = rfc_cdma_get_logical_device_params( slave_dev );

  /* Get CDMA Data Write Pointer*/
  s_ic_data_w = rf_cdma_get_mutable_intelliceiver( slave_dev );
  s_ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( slave_dev );

  /* Perform NULL Pointer check */
  if ( ( m_dev_status == NULL ) || ( s_dev_status == NULL ) ||
       ( s_dev_status_w == NULL ) ||
       ( logical_m_dev == NULL ) || ( logical_s_dev == NULL ) || 
       ( s_ic_data_w == NULL ) || ( s_ant_tuner_token_w == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_1x_mc_enable_diversity: NULL data for device" );
    return RFM_CDMA_ERROR_NULL_DATA;
  }
  
  if ( rfc_cdma_set_rx_mdsp_path(slave_dev, RF_CDMA_MDSP_CHAIN_1) == FALSE )
  {
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;    
  }
    
  /* Validate Rx mdsp path */
  if ( logical_s_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_enable_diversity(): Invalid rx_mdsp_path %d",
              logical_s_dev->rx_mdsp_path );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
  }

  /* Get current band */
  band = m_dev_status->curr_band;

  if ( band >= RFM_CDMA_MAX_BAND )
  {
    RF_MSG_1 ( RF_ERROR, "rf_1x_mc_enable_diversity: Invalid band %d ", band );
    return RFM_CDMA_ERROR_INVALID_BAND_CHAN;
  }

  if ( m_dev_status->num_carriers >= RFM_CDMA_CARRIER_NUM )
  {
    RF_MSG_1 ( RF_ERROR, "rf_1x_mc_enable_diversity: Invalid num_carriers %d ", 
               m_dev_status->num_carriers );
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  /* Get NV for the requested device and band and check if it is NULL */
  cdma_config_ptr = rf_cdma_nv_get_1x_data( 
                             logical_s_dev->rx_nv_path[m_dev_status->curr_band],
                             m_dev_status->curr_band );

  if ( cdma_config_ptr == NULL )
  {
    /* no NV for the requested device and band - bail out */
    RF_MSG_2( RF_ERROR, "rf_1x_mc_enable_diversity: NULL NV for device %d on "
                        "band = %d", slave_dev, m_dev_status->curr_band);
    return RFM_CDMA_ERROR_BAD_CONFIG;
  }

  /*--------------------------------------------------------------------------*/
  /* all initial checks passed - safe to proceed */
  /*--------------------------------------------------------------------------*/

  /* -------------------------- Update Alt. Path ---------------------------*/
  wakeup_success &= rf_cdma_mc_update_alt_rx_path( RFM_1X_MODE, slave_dev,
                                                  m_dev_status->curr_band ,
                                                  is_irat_mode );


  /*-----------Update the Antennae switch----------- */
  #if defined(FEATURE_RF_ASDIV) || defined(FEATURE_RF_HAS_TPTUNER_SWITCH)
    rf_cdma_data_set_ant_position ( slave_dev , 
      ((uint8) rfcommon_asdiv_get_current_position ( slave_dev ) ) ) ;

    RF_MSG_1 ( RF_MED , "rf_1x_mc_enable_diversity: Ant Switch position %d ", 
               rf_cdma_data_get_ant_position ( slave_dev ) ) ;
  #else
    rf_cdma_data_set_ant_position ( slave_dev , ( (uint8)(0) ) ) ;
  #endif

  /* ------------------------End of Update Alt. Path -----------------------*/

  if ( is_irat_mode == FALSE )
  {
    /*--------------------------------------------------------------------------*/
    /* Enable Chip Power for all devices in the receiver - Device, RxAGC, RFCard*/
    /*--------------------------------------------------------------------------*/
    prof_vreg_t = rf_time_get_tick();
    rfc_cdma_manage_vregs( slave_dev, RFM_1X_MODE, m_dev_status->curr_band,
                           RF_PATH_RX_STATE);
    
    /* Vote ON for all RF Devices which as needed with RFM Device (device) 
       This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */
    dev_voting_status = rfcommon_core_dev_power_vote(slave_dev,
                                                     RFCMN_CORE_DEV_ON,
                                                     RFCMN_CORE_DEV_NORMAL_VOTE);

    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      dev_voting_status &= 
                   rfdevice_cdma_rx_pwr_wakeup(logical_s_dev->rx_device[band]);
    }
    
    if( dev_voting_status == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_1x_mc_enable_diversity(): Device:%d Pwr UP failed",
                          slave_dev);
    }
    wakeup_success &= dev_voting_status;
    
    prof_vreg_t_val = rf_time_get_elapsed( prof_vreg_t, RF_USEC );
  }

  /* init dynamic settigs before any operations */
  rf_cdma_msm_init_dyn_settings(&dyn_settings);

  /* init band/chan and carrier info for slave device */
  rf_cdma_init_freq_info_for_device(slave_dev);

  /* set rxlm handle */
  rf_cdma_data_set_rxlm_handle( slave_dev , rxlm_handle_slave_dev );

  /* FALSE:  IC not needed for Secondary device */
  (void) rf_cdma_set_ic_support ( s_ic_data_w , FALSE ) ;

  if ( is_irat_mode == FALSE )
  {
    /*------------------------------------------------------------------------*/
    /* Determine Power Mode for desired band and channel */
    /*------------------------------------------------------------------------*/
    power_mode = rf_cdma_ic_get_starting_power_mode( 
                                                 s_ic_data_w, 
                                                 m_dev_status->curr_band,
                                                 m_dev_status->lo_tuned_to_chan,
                                                 m_dev_status->rx_bw_khz );
  }
  else
  {
    /* for IRAT use only default power mode */
    power_mode = RFM_CDMA_POWER_MODE_NORMAL;
  }

  /*--------------------------------------------------------------------------*/
  /* Determine ADC clock frequency */
  /*--------------------------------------------------------------------------*/
  adc_clock_freq = rf_cdma_msm_get_adc_samp_freq( rxlm_handle_slave_dev, 
                                        m_dev_status->curr_band, 
                                        m_dev_status->lo_tuned_to_chan );

  /* Populate static settings */
  static_cfg.adc_clock_freq = adc_clock_freq;
  static_cfg.bw_khz = m_dev_status->rx_bw_khz;
  static_cfg.rxlm_adc = logical_s_dev->rxlm_adc;
  static_cfg.config_as_et = FALSE;

  /*--------------------------------------------------------------------------*/
  /* Configure RxLM */
  /*--------------------------------------------------------------------------*/

  /* Populate the IQMC Data for query */
  iqmc_data.lin = power_mode;
  iqmc_data.band = m_dev_status->curr_band;
  iqmc_data.chan = m_dev_status->lo_tuned_to_chan;
  iqmc_data.sec_chain_rf_state = RF_CDMA_STATE_RXDIV;
  iqmc_data.bw_khz = static_cfg.bw_khz;

  /* Get RSB coefficients from device driver */
  rfdevice_cdma_rx_cmd_dispatch( logical_s_dev->rx_device[iqmc_data.band],
                                 RFDEVICE_GET_IQMC_COEFS, &iqmc_data);

  /* init dynamic settigs before any operations */
  rf_cdma_msm_init_dyn_settings(&dyn_settings);

  dyn_settings.apply_dynamic_cfg = TRUE;
  /*--------------------------------------------------------------------------*/
  /* Get SSMA recos for slave device, this not only gets Rx recos, but also 
  updates the state machine for the slave device with the relevant info */ 
  wakeup_success &= rf_cdma_get_ssma_recos ( slave_dev, rxlm_handle_slave_dev, 
                                             m_dev_status->curr_band, 
                                             m_dev_status->lo_tuned_to_chan,
                                             logical_s_dev->paired_device );

  /* now read the spur-info table in NV to see if any spurs need to be
     supressed *** The notches will be suppressed when RXLM is configured     */
  wakeup_success &= rf_cdma_get_spurs_to_suppress( slave_dev, 
                    rxlm_handle_slave_dev, m_dev_status->curr_band, 
                    m_dev_status->lo_tuned_to_chan, 
                    m_dev_status->num_carriers, &dyn_settings );

  /* Update the number of notches used on this device (for tracking use 
  for SSMA callback) 1x has only one carrier ,use index zero */
  s_dev_status_w->ssma_cb_data.num_notch_filters_used = 
                  dyn_settings.notch_filt_settings[0].num_notch_filters_used;

  dyn_settings.iqmc_coeff_in = iqmc_data.iqmc_a_coeff;
  dyn_settings.iqmc_coeff_quad = iqmc_data.iqmc_b_coeff;
  dyn_settings.num_carriers = m_dev_status->num_carriers;
  dyn_settings.modem_chain =
  rfc_cdma_get_rx_modem_chain ( slave_dev , m_dev_status->curr_band ) ;
  
  rf_cdma_compute_carrier_info( 
                                m_dev_status->curr_band,
                                m_dev_status->curr_chans,
                                m_dev_status->num_carriers, 
                                m_dev_status->carrier_index, 
                                &dyn_settings.carrier_mask,
                                dyn_settings.freq_offset_hz,
                                0 /* no need to add anything to calculated
                                     freq offset */
                              );
  /* Configure Rx Link Manager Buffer for given Device, Power Mode, and
      ADC Sample Frequency */
  prof_rxlm_t = rf_time_get_tick();
  wakeup_success &= rf_cdma_msm_configure_rxlm( rfdevice_id_get(logical_s_dev->rx_device[band]),
                                                rxlm_handle_slave_dev, RFM_1X_MODE,
                                                m_dev_status->curr_band,
                                                power_mode, static_cfg, dyn_settings );
  prof_rxlm_t_val = rf_time_get_elapsed( prof_rxlm_t, RF_USEC );

  /*----------------------------------------------------------------------*/
  /* Configure RxAGC */
  /*----------------------------------------------------------------------*/

  /* Populate the AGC Mode */
  agc_mode_info.mode = RF_1X_AGC_MODE_NORMAL;
  agc_mode_info.acq_duration = rf_cdma_msm_get_1x_default_agc_acq_duration();

  prof_rxagc_cfg_t = rf_time_get_tick();

  /*Associate Prx + Drx device first due to WTR requirement */
  rf_cdma_data_set_assoc_dev( slave_dev , master_dev );
  rf_cdma_data_set_assoc_dev( master_dev , slave_dev );

  wakeup_success &= rf_1x_configure_rx_agc( slave_dev, rxlm_handle_slave_dev,
                                            m_dev_status->curr_band, 
                                            m_dev_status->lo_tuned_to_chan, 
                                            power_mode, RF_CDMA_STATE_RXDIV,
                                            &agc_mode_info );

  prof_rxagc_cfg_t_val = rf_time_get_elapsed( prof_rxagc_cfg_t, RF_USEC );
  
  /*------------------------- END OF STATIC INIT -------------------------*/
  /* No Static de-initialization can be done after this point. Only Sleep 
     hardware sequence is executed                                        */
  /*----------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Configure RF Front-end and send RX_START to FW                           */
  /*--------------------------------------------------------------------------*/
  rf_cdma_data_set_script_index( slave_dev, 
                  rf_cdma_mdsp_get_device_buffer_index(RFM_1X_MODE,
                                                       rxlm_handle_slave_dev) );

  if ( wakeup_success == TRUE )
  {
    /* Enter Device Critical Section, so that the device call is thread safe */
    rfdevice_cdma_rx_enter_critical_section( logical_s_dev->rx_device[m_dev_status->curr_band] );

    wakeup_success &= rf_cdma_mc_configure_front_end_for_rx_wakeup (
                      slave_dev, RFM_1X_MODE, TRUE /*is_slave_device*/,
                      rxlm_handle_slave_dev, m_dev_status->curr_band, 
                      m_dev_status->lo_tuned_to_chan, 
                      power_mode, 
                      s_dev_status->script_index,
                      m_dev_status->rx_bw_khz, 
                      m_dev_status->tx_bw_khz,
                      logical_s_dev,
                      s_ant_tuner_token_w,
                      is_irat_mode, 
                      meas_type,
                      FALSE);

    if ( wakeup_success == TRUE )
    {
      wakeup_success = rf_1x_mc_send_start_rx_helper ( slave_dev , 
                                                       s_dev_status ,
                                                       rxlm_handle_slave_dev,  
                                                       logical_s_dev ,
                                                       is_irat_mode, 
                                                       FALSE /*is_retune */,
                                                       TRUE /*is_div_device*/ );

      if ( logical_s_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script(
          logical_s_dev->ant_tuner[band],
          s_ant_tuner_token_w );
      }
    } /* ( exec_success == TRUE ) */

    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section( 
       logical_s_dev->rx_device[m_dev_status->curr_band] );

  } /* if ( wakeup_success == TRUE ) */

  /* Unconditionally clean-up the Rx AGC LNA SM events here */
  wakeup_success &= rf_cdma_mc_cleanup_rx_agc_lna_ccs_events( slave_dev );

  /* Check if all Static Initialization when through. If not, return Failure.
  For successful execution, do the Rx Wakeup/Tune hardware sequence. */
  if ( wakeup_success == TRUE )
  {
    /* update the rotator offsets to the device data structure */
    rf_cdma_data_update_rot_offsets( master_dev, dyn_settings.freq_offset_hz );

    /* Start the IntelliCeiver if the Analog Reciever supports/requires it */
    if ( s_dev_status->intelliceiver.ic_supported == TRUE && 
         is_irat_mode == FALSE )
    {
      rf_cdma_ic_start( s_ic_data_w, 
                        logical_s_dev,
                        m_dev_status->curr_band,
                        m_dev_status->lo_tuned_to_chan );
    }

    /* Update CDMA Data structure with new RF State and  band/Chan */
    rf_cdma_data_set_1x_rf_state( slave_dev, RF_CDMA_STATE_RXDIV );
    rf_cdma_data_set_curr_band( slave_dev, m_dev_status->curr_band );
    rf_cdma_data_set_curr_chan( slave_dev, RFM_CDMA_CARRIER_0, 
                      m_dev_status->curr_chans[RFM_CDMA_CARRIER_0] );
    rf_cdma_data_set_carrier_index( slave_dev , RFM_CDMA_CARRIER_0 , 0 );
    rf_cdma_data_set_rx_bw_khz( slave_dev , m_dev_status->rx_bw_khz );
    rf_cdma_data_set_tx_bw_khz( slave_dev , RF_CDMA_BW_INVALID );
    rf_cdma_data_set_num_carriers( slave_dev , 1 );
    rf_cdma_data_set_lo_tuned_to_chan( slave_dev , 
                                     m_dev_status->lo_tuned_to_chan );
    

    /* Get Final RxAGC and LNA State */
    rf_1x_mdsp_get_rx_agc_data( logical_s_dev->rx_mdsp_path, &rx_agc_read );

    rx_agc_read.rx_agc = rf_cdma_convert_rxagc(RFM_CDMA_AGC_FORMAT__DBM10, 
                                               rx_agc_read.rx_agc);

    rf_cdma_data_sanity_check( slave_dev );
    rf_cdma_data_sanity_check( master_dev );

    /* No Wait time */
    ret_val = 0;

    RF_MSG_5( RF_MED, "rf_1x_mc_enable_diversity: SlaveDev %d, "
                      "RxAGC(dBm10) %d, RxAGC_Time (us) %d, "
                      "RxLM_time (us) %d, Vreg_Time (us %d ", 
              slave_dev, rx_agc_read.rx_agc, prof_rxagc_cfg_t_val,
              prof_rxlm_t_val, prof_vreg_t_val );

  } /* if ( wakeup_success == TRUE ) */
  else
  {
    /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
    if (rfcommon_nv_get_lpm_feature_enabled())
    {
      dev_voting_status = 
                   rfdevice_cdma_rx_pwr_sleep(logical_s_dev->rx_device[band]);
    }
    else
    {
      dev_voting_status = TRUE;
    }

    /* Vote OFF for all RF Devices which as not needed with RFM Device (device) 
       This API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */
    dev_voting_status &= rfcommon_core_dev_power_vote(slave_dev,
                                                     RFCMN_CORE_DEV_OFF,
                                                     RFCMN_CORE_DEV_NO_VOTE);
    if( dev_voting_status == FALSE )
    {
      RF_MSG_1( RF_ERROR, "rf_1x_mc_enable_diversity(): Device:%d Pwr Down failed "
                       , slave_dev);
    }

    /* Revert the Vreg State, since Rx is not waking up */
    rfc_cdma_manage_vregs( slave_dev, RFM_1X_MODE, m_dev_status->curr_band,
                           RF_PATH_SLEEP_STATE);

    ret_val = RFM_CDMA_ERROR_FAILURE;

    RF_MSG_3( RF_ERROR, 
              "rf_1x_mc_enable_diversity - failed: [Status %d] - MasterDev %d, "
              "SlaveDev %d, EnableDivTime %d", ret_val, master_dev, slave_dev );
  }

    enable_div_time_val = rf_time_get_elapsed( enable_div_time, RF_USEC );
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&rx_1x_wakeup_DRx_time_hist,(uint32) enable_div_time_val);
    if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
    {
      rf_1x_clear_profiling_hist_packet();
    }
  #endif
  RF_MSG_4( RF_MED, "rf_1x_mc_enable_diversity : [Status %d] - MasterDev %d, "
            "SlaveDev %d, EnableDiv time %d", ret_val, master_dev, slave_dev,enable_div_time_val);

  return ret_val;
} /* rf_1x_mc_enable_diversity */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Disable receive diversity operation and put the device to sleep.

  @details
  This API unlinks the state of slave_dev from whatever device it was
  associated with in the rfm_1x_enable_diversity() function.

  If slave_dev is in Rx or RxTx mode, slave_dev is put to sleep.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_DISABLE_DIV_COMPLETE event.  The function will attempt to give
  a best estimate of the time remaining in the return value.

  @param slave_dev
  The radio device that will be disconnected from the diversity radio.
  This device must have previously been associated with a master device
  in a call to rfm_1x_enable_diversity().

  @param rxlm_handle_slave_dev
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware for slave_dev.

  @return
  The anticipated time in microseconds remaining from the
  rf_1x_mc_disable_diversity() function return until the operation is
  complete.

*/
rfm_wait_time_t
rf_1x_mc_disable_diversity
(
  const rfm_device_enum_type slave_dev,
  const lm_handle_type rxlm_handle_slave_dev,
  const boolean is_irat_mode
)
{
  rfm_wait_time_t ret_val; /* Return value */
  boolean sleep_success = TRUE; /* Flag to track the success of sleep rx */
  boolean dev_voting_status; /* Need the flag to perform device voting */

  const rf_cdma_data_status_type *s_dev_status; /*Device Status Read Pointer */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */

  rf_cdma_ic_data_type *s_ic_data_w; /* Intelliceiver write pointer*/
  ant_tuner_device_tokens_type* s_ant_tuner_token_w; /*Tuner token write pointer*/

  /*--------------------------------------------------------------------------*/
  /* Store master_dev first before invalidating s_dev_status->assoc_dev */
  /* since Write Pointers will be removed once set APIs are ready             */
  /*--------------------------------------------------------------------------*/
  rfm_device_enum_type master_dev;
  rf_time_tick_type disable_div_time; /* total time spent in API */
  rf_time_type disable_div_time_val; /* total time spent in API */
  disable_div_time=rf_time_get_tick();

  /* Get Device Status Read Pointer*/
  s_dev_status = rf_cdma_get_device_status( slave_dev );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( slave_dev );

  /* Get CDMA Data Write Pointer*/
  s_ic_data_w = rf_cdma_get_mutable_intelliceiver( slave_dev );
  s_ant_tuner_token_w = rf_cdma_get_mutable_ant_tuner_token( slave_dev );

  /* Perform NULL Pointer check */
  if ( ( s_dev_status == NULL ) || ( logical_dev == NULL ) ||
       ( s_ic_data_w == NULL) || ( s_ant_tuner_token_w == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_1x_mc_disable_diversity: NULL data for device" );
    return RFM_CDMA_ERROR_NULL_DATA;
  }
  
  /* Validate Rx mdsp path */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_disable_diversity(): Invalid rx_mdsp_path %d",
              logical_dev->rx_mdsp_path );
    return RFM_CDMA_ERROR_INVALID_LOGICAL_DEV_PARAM;
  }

  /*--------------------------------------------------------------------------*/
  /* stop intelliceiver state-machine */
  /*--------------------------------------------------------------------------*/
  if ( s_dev_status->intelliceiver.ic_supported )
  {
    rf_cdma_ic_stop( s_ic_data_w, logical_dev );
  }

  /*--------------------------------------------------------------------------*/
  /* Compute Disable RTR diversity Script and send RX_stop to FW              */
  /*--------------------------------------------------------------------------*/

  rf_cdma_data_set_script_index( slave_dev, 
                  rf_cdma_mdsp_get_device_buffer_index(RFM_1X_MODE,
                                                       rxlm_handle_slave_dev) );

  
  if ( sleep_success == TRUE && is_irat_mode == FALSE )
  {
    rfm_cdma_band_class_type band = s_dev_status->curr_band;

    /* Enter Device Critical Section, so that the device call is thread safe*/
    rfdevice_cdma_rx_enter_critical_section( logical_dev->rx_device[band] );

    sleep_success &= rf_cdma_mc_configure_front_end_for_rx_sleep ( 
                     slave_dev, RFM_1X_MODE, 
                     rxlm_handle_slave_dev, TRUE/* is_slave_device*/,
                     band,
                     s_dev_status->script_index, 
                     s_dev_status->rx_bw_khz, 
                     s_dev_status->tx_bw_khz, logical_dev,
                     s_ant_tuner_token_w,
                     FALSE /* immediate_write */ ,
                     is_irat_mode );

    if ( sleep_success == TRUE )
    {
        sleep_success = 
          rf_1x_mc_send_stop_rx_helper ( slave_dev , s_dev_status , rxlm_handle_slave_dev , 
                                    logical_dev , RF_CDMA_RX_STOP_RTC_CX1_TRIGGER ,
                                    FALSE , /* Skip FE Programming? y/n  */ 
                                    FALSE,  /* Is QPCH? y/n */
                                    TRUE /* is DRx */) ;
        if ( FALSE == sleep_success )
        {
          RF_MSG ( RF_ERROR , "rf_1x_mc_disable_diversity(): " 
                 "rf_1x_mc_rx_stop_helper() returned failure" ) ;
        }

      if ( logical_dev->ant_tuner[band] != NULL )
      {
        rf_cdma_atuner_ack_script( logical_dev->ant_tuner[band], 
                                   s_ant_tuner_token_w);
      }

    } /* if ( sleep_success == TRUE ) */

    /* Clean up static event smem and event handle regardless faliure or not*/
    rf_cdma_mc_cleanup_ccs_event( slave_dev, RF_CDMA_STATIC_EVENT );

    /* Leave Device Critical Section */
    rfdevice_cdma_rx_leave_critical_section(logical_dev->rx_device[band]);

  } /* if ( sleep_success == TRUE ) */

  /* Check if all Initialization when through. If not, return Failure.
     For successful execution, do the Rx sleep hardware sequence. */
  if ( sleep_success == TRUE )
  {
    /* Skip power voting for iRAT mode */
    if ( is_irat_mode == FALSE )
    {
      /* check RFNV_SPARE_1_I value to know LPM feature enabled through NV */
      if (rfcommon_nv_get_lpm_feature_enabled())
      {
        dev_voting_status = 
                     rfdevice_cdma_rx_pwr_sleep(
                                logical_dev->rx_device[s_dev_status->curr_band]);
      }
      else
      {
        dev_voting_status = TRUE;
      }
      /* Vote OFF for all RF Devices which are not needed with RFM Device. This 
         API call help to vote ON/OFF for TCXO buffer and NPLER BYPASS settings */
      dev_voting_status &= rfcommon_core_dev_power_vote(slave_dev,
                                                       RFCMN_CORE_DEV_OFF,
                                                       RFCMN_CORE_DEV_NO_VOTE);
      if( dev_voting_status == FALSE )
      {
        RF_MSG_1( RF_ERROR, "rf_1x_mc_disable_diversity(): Device: %d Pwr Down failed "
                         , slave_dev);
        ret_val = RFM_CDMA_ERROR_FAILURE;
      }

      sleep_success &= dev_voting_status;

      if ( sleep_success == TRUE )
      {
        /* Revert the Vreg State, since Rx is going to sleep */
        rfc_cdma_manage_vregs( slave_dev, RFM_1X_MODE, 
                               s_dev_status->curr_band,
                               RF_PATH_SLEEP_STATE);
      }
    }/* if ( is_irat_mode == FALSE ) */

    /*-----------------------------------------------------------------------*/
    /* Need to store the master dev number to invalid assoc_dev              */
    /* since Write Pointers will be removed once set APIs are ready          */
    /* or if assoc_dev of slave_dev is invalidated first, we lose master dev */
    /*-----------------------------------------------------------------------*/
    master_dev = s_dev_status->assoc_dev;
    /*----------------------------------------------------------------------*/
    /* LAST step::: Invalidate the Device info for Slave device             */    
    /* bring carrier info to initial-state: invalidate all carrier_index &    */
    /* channels                                                               */
    /*----------------------------------------------------------------------*/
    rf_cdma_reset_sw_state ( slave_dev ) ;
    rf_cdma_data_set_assoc_dev( master_dev , RFM_INVALID_DEVICE );
    rf_cdma_data_sanity_check( slave_dev ) ;
    rf_cdma_data_sanity_check( master_dev ) ;

    /* Device successfully put to sleep - undo any SSMA operations performed
       on the paired device, since the device is asleep, need to use
       RF_CDMA_INVALID_BAND and RF_CDMA_INVALID_CHAN while calling the SSMA
       routines */
    (void) rf_cdma_get_ssma_recos ( slave_dev, 
                                    rxlm_handle_slave_dev, 
                                    RFM_CDMA_MAX_BAND,
                                    RF_CDMA_INVALID_CHAN,
                                    logical_dev->paired_device );

    ret_val = 0; /* success */
      /* This function releases all the TQs and TQs are required to 
    queue any read\writes to CCS so this function call should be 
    after all device read or writes. */ 
  }
  else
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_disable_diversity: error during config " 
              "for device %d", slave_dev );
    ret_val =  RFM_CDMA_ERROR_FAILURE;
  }

  disable_div_time_val = rf_time_get_elapsed( disable_div_time, RF_USEC );
  #if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_update(&rx_1x_sleep_DRx_time_hist,(uint32) disable_div_time_val);
    if( prof_hist_log_periodic( &cdma_rf_hist_packet_handle ) )
    {
      rf_1x_clear_profiling_hist_packet();
    }
  #endif
  return ret_val;

} /* rf_1x_mc_disable_diversity */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Re-tune the 1x radio for a new band and or channel of operation.

  @details
  Changes the operating frequency of the 1x radio.  This function may
  only be called in Rx state.

  If this API is called while the radio is in the Rx state, the receiver
  (and any associated diversity receivers) will be re-tuned to operate
  in the new band and/or channel.

  If this API is called while the radio is in the RxTx state, the
  function will return with the error code RFM_1X_TUNE_ERROR__INVALID_MODE
  without changing anything in the current radio configuration.
  This means the radio will remain in the RxTx state on the previous
  channel.

  This API is asynchronous, and the radio cannot be considered stable
  until the user callback function has been called with the
  RFM_1X_TUNE_COMPLETE event.  The function will attempt to give a
  best estimate of the time remaining in the return value.
 
  If this API is called with same band and chan back to back, the call will 
  still be processed and retune will be done again on same band and channel.

  @param device
  The radio path to re-tune.

  @param rxlm_handle
  The Rx Link Manager buffer that shall be used to configure the
  modem hardware.

  @param new_band
  RF CDMA band on which 1x radio is to be tuned.
 
  @param new_chan
  RF CDMA channel on which 1x radio is to be tuned.
 
  @param is_irat_mode
  Boolean flag to check if the API is being called in IRAT or non-IRAT scenarios
  In IRAT scenarios, a lot of API calls can be skipped ( ex. DLNA Rx on/off 
  scripting, vregs etc.)
 
  @return
  The anticipated time in microseconds remaining from the rfm_1x_tune()
  function return until the operation is complete, or an error code.
  If the API call succeeds the return value will be non-negative; all
  error codes will be negative.

  @retval RFM_1X_TUNE_ERROR__INVALID_BAND
  The band the user requested is not supported by the current chipset
  configuration.

  @retval RFM_1X_TUNE_ERROR__INVALID_CHANNEL
  The channel the user requested is not supported by the current chipset
  configuration.

  @retval RFM_1X_TUNE_ERROR__INVALID_MODE
  The radio was in an invalid mode and the radio could not be retuned.
*/
rfm_wait_time_t
rf_1x_mc_retune
(
  const rfm_device_enum_type rx_device,
  const rfm_device_enum_type tx_device,
  const lm_handle_type rxlm_handle,
  const rfm_cdma_band_class_type new_band,
  const rfm_cdma_chan_type new_chan,
  boolean is_irat_mode
)
{
  rfm_wait_time_t ret_val = 0; /* final wait time from this API */
  const rf_cdma_data_status_type *m_dev_status;/* Dev Status Read Ptr*/
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  boolean notch_status;
  rf_time_tick_type prof_retune_t;      /* Retune Time Profile */
  rf_time_type prof_retune_t_val;      /* Retune Time Profile */

  prof_retune_t = rf_time_get_tick( );

  /* get the device status for master device */
  m_dev_status = rf_cdma_get_device_status( rx_device );

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( rx_device );

  /* Perform NULL Pointer check */
  if ( ( m_dev_status == NULL ) || ( logical_dev == NULL ) )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_retune: NULL data for device %d",
              rx_device );
    return RFM_CDMA_ERROR_NULL_DATA;
  }

  /* If Tx device is valid, deconfig and sleep Tx first before entering 
     Rx retune procedure */
  if ( tx_device != RFM_INVALID_DEVICE )
  {
    /* Try Retune Tx if a valid Tx device is present  */
    ret_val = rf_1x_mc_helper_for_tx_retune( rx_device, 
                                             tx_device, 
                                             new_band, 
                                             new_chan, 
                                             FALSE /* is_start_tx */ );
  }

  if ( ret_val >= RFM_CDMA_HEALTHY_STATE )
  {
  /* Call 1x Main Control to put device to sleep - use the RXLM handle in the
     dev_status_r_master, since, L1 may pass in a "rxlm_handle" that is 
     different from the one it used during wakeup_rx (this is a valid case).
     As far as RF is concerned, it needs to make sure that the device is put
     to sleep - hence use the RxLM handle that was saved during the prep_wakeup
     API
  */
    /* Tell IC SM it is primary device. 
       Make sure this is the 1st time IC SM is touched */
    (void) rf_cdma_set_ic_support ( 
       &(rf_cdma_get_mutable_device_status(rx_device)->intelliceiver) , TRUE ) ;
    ret_val = rf_1x_mc_helper_for_rx_retune(rx_device, 
                                            rxlm_handle, new_band, 
                                         new_chan, is_irat_mode );
  }

  /* Configure and retune Tx device after Rx device is tuned */
  if ( ret_val >= RFM_CDMA_HEALTHY_STATE && tx_device != RFM_INVALID_DEVICE )
  {  
    ret_val = rf_1x_mc_helper_for_tx_retune( rx_device, 
                                             tx_device, 
                                             new_band, 
                                             new_chan, 
                                             TRUE /* is_start_tx */ );
  } /* if ( retune_helper_success == TRUE ) */

  /* Apply notch filter recommendations from WTR on the master device */
  notch_status = 
  rf_cdma_mc_trigger_dynamic_notch_update(rx_device,
                      m_dev_status->ssma_cb_data.ssma_reco,
                      m_dev_status->ssma_cb_data.num_notch_filters_used,
                      m_dev_status->ssma_cb_data.clear_notch); 

  /* Notch application failed bail out!*/
  if ( notch_status == FALSE )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_retune: Notch update failed for device %d",
              rx_device );
    ret_val =  RFM_CDMA_ERROR_FAILURE;
  }

  /* Retune the diversity chain if diversity is enabled */
  if (ret_val >= RFM_CDMA_HEALTHY_STATE )
  {
    /* check if diversity chain is to be retuned as well */
    if ( m_dev_status->assoc_dev != RFM_INVALID_DEVICE )
    {
      /* Dev Status Read Ptr for slave device */
      const rf_cdma_data_status_type *dev_status_r_slave; 
    
      /* holds the associated device */
      rfm_device_enum_type slave_dev = m_dev_status->assoc_dev;
    
      /* get device status for slave */
      dev_status_r_slave = rf_cdma_get_device_status(slave_dev);

      /*NULL pointer check*/
      if( dev_status_r_slave == NULL )
      {
        RF_MSG_1( RF_ERROR, "rf_1x_mc_retune: Dev %d NULL Data", slave_dev );
        ret_val = RFM_CDMA_ERROR_NULL_DATA;
      } /* if( dev_status_r_slave == NULL ) */
      else
      { /* FALSE: IC support not needed for secondary device */
        (void) rf_cdma_set_ic_support ( 
           &(rf_cdma_get_mutable_device_status(slave_dev)->intelliceiver) , FALSE ) ;
        ret_val = rf_1x_mc_helper_for_rx_retune ( slave_dev,
                                               dev_status_r_slave->rxlm_handle, 
                                               new_band, new_chan,
                                               is_irat_mode );

      } /* if ! ( dev_status_r_slave == NULL ) */

      if (ret_val < RFM_CDMA_HEALTHY_STATE)
      {
        RF_MSG_2( RF_ERROR, "rf_1x_mc_retune : [Status %d] - Failed during"
                  "retune for diversity device=%d", ret_val, slave_dev);
      } /* if (ret_val < RFM_CDMA_HEALTHY_STATE) */
    
    } /* if ( dev_status_r->assoc_dev != RFM_MAX_DEVICES ) */

  } /* if (ret_val >= RFM_CDMA_HEALTHY_STATE ) */
  else
  {
    RF_MSG_2( RF_ERROR, "rf_1x_mc_retune : [Status %d] - Failed during"
              "retune for Primary device=%d", ret_val, rx_device);
  }

  rf_cdma_data_sanity_check( rx_device );

  prof_retune_t_val = rf_time_get_elapsed( prof_retune_t, RF_USEC );

  RF_MSG_1 ( RF_LOW, "rf_1x_mc_retune_rx: time profile"
                     "Retune Time: " , prof_retune_t_val );

  return ret_val;
} /* rf_1x_mc_retune_rx */

/*! @} */

/*============================================================================*/
/*!
  @name AGC Config Functions

  @brief
  Functions to configure 1x RxAGC and TxAGC
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Converge RxAGC
 
  @details
  This function is used to wait for 1x RxAGC to converge. This will wait for 
  acq_down_ctr to go below 0 and to protect from spinning in an infinite loop,
  wait of 2 ms is added before bailing out
 
  @param device
  RFM device for which RxAGC convergene is expected
 
  @return
  Flag to indicate the success if the  function
*/
boolean
rf_1x_mc_rx_agc_convergence
(
  rfm_device_enum_type device
)
{
  rf_1x_mdsp_rx_agc_read_type rx_agc_data; /* place holder for RxAGC data */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  uint8 loop_ctr = 0; /* poll acqTimeDnCntr variable in FW --> RxAGC cannot be 
  assumed to have to have settled until acqTimeDnCntr < 0 */
  uint8 max_times_to_poll = 40 ;
  /* query 1x rxagc update duration (1 combiner IQR duarion) */
  uint16 rxagc_update_duration = rf_cdma_msm_get_combiner_interrupt_period();

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  if ( logical_dev ==  NULL)
  {
    RF_MSG_1( RF_ERROR,"rf_1x_mc_rx_agc_convergence: "
                       "NULL logical data for device %d", device );
    return FALSE;   
  }

  /* While calibrating we should converge much faster so set the polling 
     attempts to a lower value so the max wait time during calibration will
     be 6 x 104 uSec = 624 uSec  */
  if ( rfm_get_calibration_state() == TRUE )
  {
    max_times_to_poll = 6 ;
  }

  /* wait for acq_down_ctr to go below 0 - to protect from spinning in
  in an infinite loop, wait for 2 ms and then bail out */
  do /* while ( rx_agc_data.acq_down_ctr > 0); */
  {
    /* wait for 1 combiner IRQ duration when the RxAGC parameter gets updated before polling) */
    DALSYS_BusyWait( rxagc_update_duration ) ; 

    /* poll FW to see if RxAGC is in tracking mode */
    rf_1x_mdsp_get_rx_agc_data( logical_dev->rx_mdsp_path, &rx_agc_data );

    loop_ctr++;

    /* check to see if it is more than 2ms ( 40 * 52 uS = 2800 uS) */
    if ( loop_ctr > max_times_to_poll ) 
    {
      RF_MSG( RF_ERROR, "rf_1x_mc_rx_agc_convergence: RxAGC did NOT "
                        "converge in 2.8 ms - bailing out !!");
      break;
    } /* if ( loop_ctr > 40 )  */

    RF_MSG_2( RF_LOW, "rf_1x_mc_rx_agc_convergence: RxAGC converge loop-> %d "
                      "acqDwn_ctr-> %d ", loop_ctr, rx_agc_data.acq_down_ctr);

  } while ( rx_agc_data.acq_down_ctr >= 0);  /* -1 = acq is done */

  return TRUE;   

} /* rf_1x_mc_rx_agc_convergence */

/*! @} */

/*============================================================================*/
/*!
  @name 1x AGC Query Functions

  @brief
  Functions to query 1x Rx and Tx AGC
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Total Tx Filtered Power
 
  @details
  Obtains the total Tx filtered level from MDSP for the given device.
 
  @param device
  The radio path for which to get the filtered power.
 
  @return
  Total Tx Filtered Power in dBm10 units
*/
int16
rf_1x_mc_get_total_tx_filtered_power
( 
  rfm_device_enum_type device
)
{
  int16 filtered_power_dbm10; /* Filtered power in dBm10 */
  int16 K; /* turn-around const */
  rf_cdma_mdsp_tx_agc_logical_read_type tx_agc_read; /* TxAGC Read */
  const rfc_cdma_logical_device_params_type *logical_dev; /* Logical Device */
  const rf_cdma_data_status_type *dev_status_r; /* Device Status Read Pointer */

  /* Get Logical Device Param */
  logical_dev = rfc_cdma_get_logical_device_params( device );
  
  /* Perform NULL Pointer check */
  if( logical_dev == NULL )
  {
    RF_MSG_1( RF_ERROR, "rf_1x_mc_get_total_tx_filtered_power: NULL Logical "
              "data for device %d", device );
    return 0xFFFF;
  }
  
  /* get read pointer to device - need this to find out current band */
  dev_status_r = rf_cdma_get_device_status( device );

  /*NUll pointer check*/
  if( dev_status_r == NULL )
  {
    RF_MSG_1( RF_ERROR, "ftm_1x_log_get_primary_device: Dev %d NULL Data",
               device );
    return 0xFFFF;
  }

  /* Read Tx AGC */
  rf_1x_mdsp_get_tx_agc_logical_data( logical_dev->rx_mdsp_path, 
                                      rf_cdma_data_get_txlm_handle (device) ,
                                      &tx_agc_read ) ;

  /* get turn-around constant for current band */
  K = rf_cdma_get_turnaround_constant(dev_status_r->curr_band);

  /* Convert filtered power to dBm10 */
  filtered_power_dbm10 = rf_cdma_convert_txagc( RFM_CDMA_AGC_FORMAT__DBM10, K, 
                                        tx_agc_read.total_tx_filtered_power );

  return filtered_power_dbm10;
}

/*! @} */

/*============================================================================*/
/*!
  @name RF-FW Synchronization SNUM based Response handlers

  @brief
  This section includes all the SNUM based Response handlers to be used
  for RF-FW messaging synchroziation.
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
rf_1x_rx_start_rsp_cb
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
    RF_MSG( RF_ERROR, "rf_1x_rx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_rx_start_rsp_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_rx_start_rsp_cb: NULL CB Data" );
    return FALSE;
  } /* if ( cb_data == NULL ) */

  RF_MSG_4( RF_MED, "[1X][FW->RF] [D%d] RX_START_RSP 0x%x [ %d | %d ]", 
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
    ERR_FATAL( "rf_1x_rx_start_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_1x_rx_start_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_1x_rx_start_rsp_cb */

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
rf_1x_rx_stop_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  int32 rf_ret;
  rf_cdma_rx_stop_rsp_data_type *cb_data = NULL;
  cfw_rx_config_rsp_info_t *rsp_payload;
  rfm_device_enum_type device;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_1x_rx_stop_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Typecast the payload to cfw_rx_stop_rsp_msg_t */
  rsp_payload = (cfw_rx_config_rsp_info_t*)req_ptr->payload;

  /* Extract the callback data */
  cb_data = (rf_cdma_rx_stop_rsp_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_rx_stop_rsp_cb: NULL CB Data" );
    return FALSE;
  } /* if ( cb_data == NULL ) */

  RF_MSG_4( RF_MED, "[1X][FW->RF] [D%d] RX_STOP_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );
  
  /* Get the Times Stop Status */
  if ( (boolean)rsp_payload->timed_stop_status == RX_STOP_SUCCESS )
  {
    cb_data->timed_sleep_success = TRUE;
  } /* if ( (boolean)rsp_payload->timed_stop_status == RX_STOP_SUCCESS ) */
  else
  {
    cb_data->timed_sleep_success = FALSE;
  } /* if ! ( (boolean)rsp_payload->timed_stop_status == RX_STOP_SUCCESS ) */

  if ( cb_data->blocking_call == TRUE )
  {
    /* Post the Semaphore only if an active semaphore is available */
    if ( cb_data->sema_token != NULL )
    {
      if ( cb_data->sema_token->is_sem_waiting == FALSE )
      {
        RF_MSG_2( RF_ERROR, "rf_1x_rx_stop_rsp_cb: sNUM %d "
                  "Sem @ 0x%x Possible POST-BEFORE-WAIT", 
                  snum_data->snum, cb_data->sema_token );
      } /* if ( fwrsp_data->rx_stop_rsp.sema_token.is_sem_waiting != TRUE ) */
  
      /* Post the Semaphore */
      rfcommon_semaphore_post( cb_data->sema_token );
      cb_data->blocking_call = TRUE;
    } /* if ( fwrsp_data->rx_stop_rsp.sema_token != NULL ) */
    else
    {
      ret_val = FALSE;
      ERR_FATAL( "rf_1x_rx_stop_rsp_cb: NULL Sem Token", 0, 0, 0 );
    } /* if ! ( fwrsp_data->rx_stop_rsp.sema_token != NULL ) */
  } /* if ( fwrsp_data->rx_stop_rsp.blocking_call == TRUE ) */
  else
  {

    /* Get the active device */
    device = cb_data->device;

    /* Complete Timed Sleep Rx */
    rf_ret = rf_1x_mc_complete_timed_sleep_rx( device,
                                                cb_data->timed_sleep_success,
                                                cb_data->callback,
                                                cb_data->userdata );

      /* Invalidate the housekeeping data */
     cb_data->blocking_call = TRUE;
     cb_data->callback = NULL;
     cb_data->userdata = NULL;

    /* Return value checking */
    if ( rf_ret < RFM_CDMA_HEALTHY_STATE )
    {
      ret_val = FALSE;
      RF_MSG_1( RF_ERROR, "rf_1x_rx_stop_rsp_cb: Complete Timed"
                          "Sleep Rx Failed for dev %d", device );
    } /* if ( ret_val == FALSE ) */
  } /* if ! ( fwrsp_data->rx_stop_rsp.blocking_call == TRUE ) */

  RF_MSG_4( RF_LOW, "rf_1x_rx_stop_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_1x_rx_stop_rsp_cb */

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
rf_1x_tx_start_rsp_cb
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
    RF_MSG( RF_ERROR, "rf_1x_tx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_tx_start_rsp_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_tx_start_rsp_cb: NULL CB Data" );
    return FALSE;
  } /* if ( cb_data == NULL ) */

  RF_MSG_4( RF_MED, "[1X][FW->RF] [D%d] TX_START_RSP 0x%x [ %d | %d ]", 
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
    ERR_FATAL( "rf_1x_tx_start_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_1x_tx_start_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_1x_tx_start_rsp_cb */

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
rf_1x_tx_stop_rsp_cb
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
    RF_MSG( RF_ERROR, "rf_1x_tx_stop_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_tx_stop_rsp_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_tx_stop_rsp_cb: NULL CB Data" );
    return FALSE;
  } /* if ( cb_data == NULL ) */

  RF_MSG_4( RF_MED, "[1X][FW->RF] [D%d] TX_STOP_RSP 0x%x [ %d | %d ]", 
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
    ERR_FATAL( "rf_1x_tx_stop_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_1x_tx_stop_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_1x_tx_stop_rsp_cb */

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
rf_1x_fws_config_rsp_cb
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
    RF_MSG( RF_ERROR, "rf_1x_fws_config_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_fws_cfg_rsp_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_fws_config_rsp_cb: NULL CB Data" );
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
    ERR_FATAL( "rf_1x_fws_config_rsp_cb: NULL Sem Token", 0, 0, 0 );
  } /* if ! ( cb_data->sema_token != NULL ) */

  RF_MSG_4( RF_LOW, "rf_1x_fws_config_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum, cb_data->sema_token );

  return ret_val;

} /* rf_1x_fws_config_rsp_cb */

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
rf_1x_irat_rx_start_rsp_cb
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
    RF_MSG( RF_ERROR, "rf_1x_irat_rx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_irat_rx_start_rsp_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_irat_rx_start_rsp_cb: NULL CB Data" );
    return FALSE;
  } /* if ( cb_data == NULL ) */

  RF_MSG_4( RF_MED, "[1X][FW->RF] [D%d] IRAT_RX_START_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  RF_MSG_3( RF_LOW, "rf_1x_irat_rx_start_rsp_cb: Dev %d - [%d] SNUM.0x%x "
            "[Done]", cb_data->device, snum_data->item_id, 
            snum_data->snum );

  return ret_val;

} /* rf_1x_irat_rx_start_rsp_cb */

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
rf_1x_pilot_meas_config_rsp_cb
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
    RF_MSG( RF_ERROR, "rf_1x_irat_rx_start_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_pilot_meas_config_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_pilot_meas_config_rsp_cb: NULL CB Data" );
    return FALSE;
  } /* if ( cb_data == NULL ) */

  RF_MSG_4( RF_MED, "[1X][FW->RF] [D%d] PILOT_MEAS_CFG_RSP 0x%x [ %d | %d ]", 
            cb_data->device,
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  RF_MSG_3( RF_LOW, "rf_1x_pilot_meas_config_rsp_cb: Dev %d - [%d] SNUM.0x%x "
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
rf_1x_pilot_meas_stop_stream_rsp_cb
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
    RF_MSG( RF_ERROR, "rf_1x_pilot_meas_stop_stream_rsp_cb: "
                      "NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_pilot_meas_stop_stream_data_type*)snum_data->snum_cb_data;

  /* Check NULL for cb_data */
  if ( cb_data == NULL )
  {
    RF_MSG( RF_ERROR, "rf_1x_pilot_meas_stop_stream_rsp_cb: NULL CB Data");
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
    ERR_FATAL( "rf_1x_pilot_meas_stop_stream_rsp_cb: NULL Sem Token", 
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
  FL DRX Override Response handler
 
  @details
  This callback function is executed in second level dispatching when
  FL DRX Override Response is received from firmware. This function will basically post 
  the semaphore so that blocking call can be unblocked.
 
  @param snum_data
  Pointer to Sequence number data structure
 
  @return
  Flag indcating the status of handling of response
*/
boolean
rf_1x_mdsp_fl_drx_override_rsp_cb
(
  void *cmd_ptr,
  const struct rf_dispatch_snum_info_type_t *snum_data
)
{
  boolean ret_val = TRUE;
  rf_cdma_fl_drx_override_rsp_data_type *cb_data = NULL;
  rf_fwrsp_cmd_type *req_ptr;
  /* Type cast to FWRSP type*/
  req_ptr = (rf_fwrsp_cmd_type*)cmd_ptr;

  /* NULL Pointer check */
  if ( ( req_ptr == NULL ) || ( snum_data == NULL ) )
  {
    RF_MSG( RF_ERROR, "rf_1x_mdsp_fl_drx_override_rsp_cb: NULL Argument(s)" );
    return FALSE;
  } /* if ( ( req_ptr == NULL ) || ( snum_data == NULL ) ) */

  /* Extract the callback data */
  cb_data = (rf_cdma_fl_drx_override_rsp_data_type*)snum_data->snum_cb_data;

  RF_MSG_3( RF_MED, "[CDMA][FW->RF] FL_DRX_OVERRIDE_RSP 0x%x [ %d | %d ]", 
            snum_data->snum, 
            RFCOMMON_MDSP_SNUM_TECH(snum_data->snum),
            RFCOMMON_MDSP_SNUM_COUNT(snum_data->snum) );

  /* Post the Semaphore only if an active semaphore is available */
  if ( cb_data->blocking_call == TRUE )
  {
    if ( cb_data->sema_token != NULL )
    {
      ret_val &= rfcommon_semaphore_post( cb_data->sema_token );
    } /* if ( cb_data->sema_token != NULL ) */
    else
    {
      ret_val = FALSE;
      ERR_FATAL( "rf_1x_mdsp_fl_drx_override_rsp_cb: NULL Sem Token", 0, 0, 0 );
    } /* if ! ( cb_data->sema_token != NULL ) */

    RF_MSG_4( RF_LOW, "rf_1x_mdsp_fl_drx_override_rsp_cb: Dev %d - [%d] SNUM.0x%x "
              "Sem @ 0x%x [Done]", cb_data->device, snum_data->item_id, 
              snum_data->snum, cb_data->sema_token );

  } /* if ( cb_data->blocking_call == TRUE ) */
  else
  {
    RF_MSG_3( RF_LOW, "rf_1x_mdsp_fl_drx_override_rsp_cb: Dev %d - [%d] SNUM.0x%x "
              "[Done]", cb_data->device, snum_data->item_id, snum_data->snum );
  } /* if ! ( cb_data->blocking_call == TRUE ) */

  return ret_val;

} /* rf_1x_mdsp_fl_drx_override_rsp_cb */

/*! @} */

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
  Inform the 1x Tx limit loop that the SAR state has changed, so it can
  recompute the target max power, if necessary.

  @details
  Assumes that the RF critical section has already been locked by this thread,
  and that it is safe to access the internal data structures.

  This interface makes no assumption about what devices may or may not be
  in 1x mode, so it will loop through all devices, and perform a power
  limiting update if they are in 1x mode and currently transmitting.
*/
void
rf_1x_mc_tx_update_sar_backoff
(
  void
)
{
  rfm_device_enum_type device;

  for ( device = RFM_DEVICE_0;
        device < RFM_MAX_DEVICES;
        device++ )
  {
    if ( rfm_get_current_mode(device) == RFM_1X_MODE )
    {
      const rf_cdma_data_status_type *dev_status_r;

      dev_status_r = rf_cdma_get_device_status( device );

      /*NULL Pointer check*/
      if( dev_status_r == NULL )
      {
        RF_MSG_1( RF_HIGH, "rf_1x_mc_tx_update_sar_backoff: Dev %d NULL Data",
                 device );
      }/*NULL Pointer check*/
      else
      {
      if ( dev_status_r->data_1x.rf_state == RF_CDMA_STATE_RXTX ||
           dev_status_r->data_1x.rf_state == RF_CDMA_STATE_TX )
      {
        rf_1x_plim_update_sar_state( device );
      } /* if ( dev_status_r->rf_state == RF_CDMA_STATE_RXTX ) */
      } /* if! ( dev_status_r == NULL ) */
    } /* if ( rfm_get_current_mode(device) == RFM_1X_MODE ) */
  } /* for device 0..N-1 */
} /* rf_1x_mc_tx_update_sar_backoff() */

/*! @} */

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
rf_1x_mc_tear_down
(
  rfm_device_enum_type device
)
{
  boolean was_tear_down_req = TRUE; /* Flag indicating if tear down is needed */
  boolean rfm_ret = TRUE; /* Return val from RFM */
  const rf_cdma_data_status_type *dev_status = NULL; /* RF Dev Status */

  /* Query the current RF State Ptr for master device */
  dev_status = rf_cdma_get_device_status(device);

  /* NULL pointer check*/
  if(  dev_status == NULL )
  {
    RF_MSG_1(RF_ERROR,"rf_1x_mc_tear_down: NULL data for device %d",device);
    was_tear_down_req = FALSE;
  }
  else
  {
    switch ( dev_status->data_1x.rf_state )
    {
    case RF_CDMA_STATE_RX:
    {
      
      /* Check if there is other associated Device with this device. If there
      in any other associated device, then diversity must be disabled on
      the associated device */
      if ( dev_status->assoc_dev != RFM_INVALID_DEVICE )
      {
        /* place holder for associated device */
        rfm_device_enum_type assoc_dev = dev_status->assoc_dev;

        /* status of RF assoc Dev Status */
        const rf_cdma_data_status_type *assoc_dev_status = 
                                         rf_cdma_get_device_status( assoc_dev );
                
        /* NULL pointer check*/
        if(  assoc_dev_status == NULL )
        {
          RF_MSG_1( RF_ERROR,"rf_1x_mc_tear_down: NULL data for device %d",
                    device );
          was_tear_down_req = FALSE;
        }
        else
        {
          rfm_ret &= ( rfm_1x_disable_diversity( assoc_dev,
                                                 assoc_dev_status->rxlm_handle,
                                                 NULL, 
                                                 NULL ) >= 0 );
          if (rfm_ret == TRUE)
          {
            rf_cdma_data_set_1x_rf_state ( assoc_dev, RF_CDMA_STATE_SLEEP );
            rf_cdma_data_set_assoc_dev( assoc_dev, RFM_INVALID_DEVICE );
            rf_cdma_data_set_assoc_dev( device, RFM_INVALID_DEVICE );
          }
          else
          {
            RF_MSG_1( RF_ERROR, "rf_1x_mc_tear_down: Failed to disable  "
                                "diversity on associated device %d", 
                                  dev_status->assoc_dev ); 
          }

        }/* if!(  assoc_dev_status == NULL ) */

      } /* if ( dev_status->assoc_dev != RFM_INVALID_DEVICE ) */
  
      rfm_ret &= ( rfm_1x_sleep_rx( device, dev_status->rxlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      rf_cdma_data_set_1x_rf_state( device, RF_CDMA_STATE_SLEEP );
  
    } /* case RF_CDMA_STATE_RX: */
      break;
  
    case RF_CDMA_STATE_RXTX:
    {
      /* Check if there is other associated Device with this device. If there
         is any other associated device, then Diversity must be disabled on
         the associated device */
      if ( dev_status->assoc_dev != RFM_INVALID_DEVICE )
      {

        rfm_device_enum_type assoc_dev = dev_status->assoc_dev;

        /* status of RF assoc Dev Status */
        const rf_cdma_data_status_type *assoc_dev_status = 
                                         rf_cdma_get_device_status( assoc_dev );
                        
        /* NULL pointer check*/
        if(  assoc_dev_status == NULL )
        {
          RF_MSG_1( RF_ERROR,"rf_1x_mc_tear_down: NULL data for device %d" , 
                    device );
          was_tear_down_req = FALSE;
        } 
        else
        {
          rfm_ret &= ( rfm_1x_disable_diversity( assoc_dev, 
                                                 assoc_dev_status->rxlm_handle,
                                                 NULL, 
                                                 NULL ) >= 0 );
    
          if (rfm_ret == TRUE)
          {
            rf_cdma_data_set_1x_rf_state ( assoc_dev, RF_CDMA_STATE_SLEEP ); 
            rf_cdma_data_set_assoc_dev( assoc_dev, RFM_INVALID_DEVICE );  
            rf_cdma_data_set_assoc_dev( device, RFM_INVALID_DEVICE );     
          }
          else
          {
            RF_MSG_1( RF_ERROR, "rf_1x_mc_tear_down: Failed to disable  "
                                "diversity on associated " "device %d", 
                                  dev_status->assoc_dev ); 
          }
        }/* if ! (  assoc_dev_status == NULL ) */

      } /* if ( dev_status->assoc_dev != RFM_INVALID_DEVICE ) */

      rfm_ret &= ( rfm_1x_sleep_tx( device, dev_status->txlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      rfm_ret &= ( rfm_1x_sleep_rx( device, dev_status->rxlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      rf_cdma_data_set_1x_rf_state ( device, RF_CDMA_STATE_SLEEP ); 
  
    } /* case RF_CDMA_STATE_RXTX: */
      break;
  
    case RF_CDMA_STATE_TX:
    {
      rfm_ret &= ( rfm_1x_sleep_tx( device, dev_status->txlm_handle, 
                                    NULL, NULL ) >= 0 );
  
      rf_cdma_data_set_1x_rf_state ( device, RF_CDMA_STATE_SLEEP ); 
  
    } /* case RF_CDMA_STATE_RXTX: */
      break;
  
    case RF_CDMA_STATE_RXDIV:
    {
      /* Disable the diversity for this device, as it is associated with other
      Master Device */
      rfm_device_enum_type assoc_dev = dev_status->assoc_dev;

      const rf_cdma_data_status_type* assoc_dev_status = 
                                          rf_cdma_get_device_status( assoc_dev);
              
      /* NULL pointer check*/
      if(  assoc_dev_status == NULL )
      {
        RF_MSG_1( RF_ERROR,"rf_1x_mc_tear_down: NULL data for device %d",
                  device );
        was_tear_down_req = FALSE;
      }/* if(  dev_status->assoc_dev == NULL ) */
      else
      {
        rfm_ret &= ( rfm_1x_disable_diversity( device, dev_status->rxlm_handle,
                                               NULL, NULL ) >= 0 );
    
        if (rfm_ret == TRUE)
        {
          rf_cdma_data_set_assoc_dev( assoc_dev, RFM_INVALID_DEVICE); 
          rf_cdma_data_set_1x_rf_state ( device, RF_CDMA_STATE_SLEEP ); 
          rf_cdma_data_set_assoc_dev( device, RFM_INVALID_DEVICE ); 
        }
        else
        {
          RF_MSG_1( RF_ERROR, "rf_1x_mc_tear_down: Failed to disable "
                    "diversity on associated device %d", dev_status->assoc_dev);
        }
      }/* if ! (  assoc_dev_status == NULL ) */

    } /* case RF_CDMA_STATE_RXDIV: */
      break;
  
    case RF_CDMA_STATE_SLEEP:
    case RF_CDMA_STATE_INVALID:
    default:
      was_tear_down_req = FALSE;
      RF_MSG( RF_LOW, "rf_1x_mc_tear_down: Tear Down RF : Not Required" );
      break;
  
    } /* switch ( dev_status->rf_state ) */

    rf_cdma_data_set_1x_rf_state ( device, RF_CDMA_STATE_SLEEP );
  
    if ( rfm_ret == FALSE )
    {
      was_tear_down_req = FALSE;
    }
  
    RF_MSG_1( RF_LOW, "rf_1x_mc_tear_down : Tear Down on Dev %d done", device );
  
  } /* if ! (  dev_status == NULL ) */

  return was_tear_down_req;  /* Return */

} /* rf_1x_mc_tear_down */


/*! \} */

/*============================================================================*/
/*!
  @name NV

  @brief
  This section contains functions pertaining to NV
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Reading NV from 1x technology

  @details
  Retrieves all the NV from 1x technology. Reads 1x mode
  specific RF configuration parameters from NV. This is called as part of the 
  LOAD_RF_NV function case where we need to read the calibration NV and write 
  them without having to reset.
 
  @param device
  RFM device for which NV is to be reloaded
 
  @param caller_tcb_ptr
  Calling task's TCB pointer
 
  @param task_nv_wait_sig
  Task signal to wait for when reading NV

  @param task_wait_func_ptr
  Task's wait function to be called when reading NV

  @return
  TRUE if NV retrieve is successful, otherwise returns FALSE.
*/
boolean
rf_1x_mc_reload_nv
(
  rfm_device_enum_type device, 
  rex_tcb_type *caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (*task_wait_func_ptr)( rex_sigs_type )
)
{
  /* Clean up allocated memory and reset NV data structures before loading
  NVs again. Otherwise we may run out of memory space */
  rf_cdma_nv_cleanup();

  return rf_cdma_nv_load( caller_tcb_ptr, task_nv_wait_sig,
                          task_wait_func_ptr );
} /* rf_1x_mc_reload_nv */

/*! @} */

/*============================================================================*/
/*!
  @name RF-MC utility functions

  @brief
  This section holds all utility APIs pertaining to 1x MC 
*/
/*! @{ */

/*----------------------------------------------------------------------------*/
/*
  @brief
  This API will return the synth status for a given Tech.Path and synthesizer 

  @param device
  RFM device for which PLL status is queried
 
  @param rf_mode
  current mode/tech in which tye device is operating in
 
  @synth_type
  TX PLL or RX PLL
 
  @param is_synth_locked
  holds the PLL status 
*/
void 
rf_1x_mc_get_synth_lock_status
(
  rfm_device_enum_type device, 
  rfcom_mode_enum_type rf_mode,
  rfcom_synth_type synth_type,
  void *is_synth_locked
)
{
 const rfc_cdma_logical_device_params_type *logical_dev = NULL;        
 logical_dev = rfc_cdma_get_logical_device_params(device);

 if ( logical_dev!= NULL )
 {
     /* Device Status Read Pointer - to get current band */
     const rf_cdma_data_status_type 
                            *dev_status_r = rf_cdma_get_device_status( device );

   if(synth_type == RF_SYNTH_RX)
   {

     /* Perform NULL Pointer check */
     if ( dev_status_r == NULL )
     {
       RF_MSG_1( RF_ERROR, "rf_1x_mc_get_synth_lock_status: "
                           "NULL data for device %d", device );
     }
     else
     {
       rfdevice_cdma_rx_cmd_dispatch( 
               logical_dev->rx_device[dev_status_r->curr_band], 
               RFDEVICE_GET_RX_SYNC_LOCK,
               is_synth_locked );
     }
   } /* if(synth_type == RF_SYNTH_RX) */
   else if (synth_type == RF_SYNTH_TX) 
   {
     /* Perform NULL Pointer check */
     if ( dev_status_r == NULL )
     {
       RF_MSG_1( RF_ERROR, "rf_1x_mc_get_synth_lock_status: "
                           "NULL data for device %d", device );
     }
     else
     {
       rfdevice_cdma_tx_cmd_dispatch( 
          logical_dev->tx_device[dev_status_r->curr_band] , 
                                      RFDEVICE_GET_TX_SYNC_LOCK, 
                                      is_synth_locked );
     }
   }
   else
   {
     RF_MSG_1( RF_ERROR, "Invalid SYNTH query(%d) for SYNTH in for"
                         "rfm_get_synth_lock_status", synth_type );
   }
 }
 else
 {
   RF_MSG_1( RF_ERROR, "rf_1x_mc_get_synth_lock_status: Null pointer for "
                       "logical device (%d)", device );
 }

} /* rf_1x_mc_get_synth_lock_status */


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Rf wrapper function to get L1SRAM Sample Memory Address
  for CDMA 1X.

  @details
  Returns the L1SRAM Sample Memory Address for CDMA 1X.

  @param device
  The radio path that is being queried.

  @return
  L1SRAM Sample Memory Base Address.
*/
uint32
rf_1x_get_mem_pool_base_addr
(
  const rfm_device_enum_type device
)
{
  const rfc_cdma_logical_device_params_type* logical_dev;  /* Logical Dev */

  if ( device >= RFM_MAX_DEVICES )
  {
	RF_MSG_1( RF_ERROR, "rf_1x_get_mem_pool_base_addr: Invalid device %d ",
	device);

	return NULL ;
  }

  /* Get and validate Logical Device */
  logical_dev = rfc_cdma_get_logical_device_params( device );

  if ( logical_dev == NULL )
  {
	RF_MSG_DBG_1 ( RF_ERROR, "rf_1x_get_mem_pool_base_addr(): Invalid logical device "
							"for device %d", device);
	return NULL ;
  }

  /* Validate Rx mdsp path */
  if ( logical_dev->rx_mdsp_path >= RF_CDMA_MDSP_CHAIN_NUM )
  {
	RF_MSG_1( RF_ERROR, "rf_1x_get_mem_pool_base_addr(): Invalid rx_mdsp_path %d",
		 logical_dev->rx_mdsp_path );

	return NULL ;
  }

  return (rf_1x_mdsp_get_mem_pool_base_addr( logical_dev->rx_mdsp_path ));

} /* rf_1x_get_mem_pool_base_addr */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Rf wrapper function to start the IQ capture
  for CDMA 1X.

  @details
  Rf wrapper function to start the IQ capture
  for CDMA 1X.

  @param device
  The radio path that needs the IQ capture.

  @return
  The status of the IQ capture.
*/

boolean
rf_1x_mc_capture_iq
(
  rfm_device_enum_type device
)
{
  rf_cdma_mdsp_chain_enum_type mdsp_chain;
  rf_cdma_state_type rf_state = rf_cdma_data_get_1x_rf_state( device );

  if((device == RFM_DEVICE_0 || device == RFM_DEVICE_2) && rf_state == RF_CDMA_STATE_RX )
  {
    mdsp_chain = RF_CDMA_MDSP_CHAIN_0;
  }
  else if((device == RFM_DEVICE_1 || device == RFM_DEVICE_3) && rf_state == RF_CDMA_STATE_RXDIV )
  {
    mdsp_chain = RF_CDMA_MDSP_CHAIN_1;
  }
  else
  {
    RF_MSG_2 (RF_ERROR, "rf_1x_mc_capture_iq: device %d is not in proper state %d ", device, rf_state);
	return FALSE;
  }
  
  return(rf_1x_mdsp_capture_iq(mdsp_chain));
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  utility API to clear 1x time profiling histogram packect 0x18e9,

  @param 
   void
 
  @return
  void 

*/
void rf_1x_clear_profiling_hist_packet
(
  void
)
{

#if (RFLM_FEATURE_BUILD_MODE != RFLM_FEATURE_BUILD_FW_LIB)
    prof_hist_clear(&rx_1x_wakeup_PRx_time_hist);
    prof_hist_clear(&rx_1x_wakeup_DRx_time_hist);
    prof_hist_clear(&rx_1x_wakeup_fw_resp_hist);
    prof_hist_clear(&rx_1x_sleep_PRx_time_hist);
    prof_hist_clear(&rx_1x_sleep_DRx_time_hist);
    prof_hist_clear(&rx_1x_sleep_fw_resp_hist);
    prof_hist_clear(&tx_1x_wakeup_Tx_time_hist);
    prof_hist_clear(&tx_1x_wakeup_fw_resp_hist);
    prof_hist_clear(&tx_1x_sleep_Tx_time_hist);
    prof_hist_clear(&tx_1x_sleep_fw_resp_hist);
  #endif
} /*rf_1x_clear_profiling_hist_packet()*/

#endif /* FEATURE_CDMA1X */
/*! @} */

/*! @} */
