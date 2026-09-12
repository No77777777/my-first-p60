/*!
  @file
  rfwcdma_core_temp_comp.c

  @brief
  Provides WCDMA Temperature Compensation functionality

*/

/*===========================================================================

Copyright (c) 2008 -2016 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:08:47 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_wcdma.mpss/3.11/rf/core/src/rfwcdma_core_auto_pin.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/01/16   ag      Add logging of reflog pin for WTR3925 Autopin
02/15/16   ag      Add logging of ref pin from NV during autopin run
01/27/16   vs      Add txc_rate_ustmr to Autopin structure
12/15/15   ag      Add API to update pin obtained from autopin algo 
                   Store wtr capture output so as to support logging
12/10/15   ag      Calculate and store extra capture time and delay in dm
12/09/15   ag      Store wtr capture output so as to support logging
12/08/15   ag      Make autopin in progress false only when WTr and fw captures are done
12/01/15   ag      Fix KW warnings
11/23/15   ag      Add APi to populate Autopin input parameters
                   Update autopin in progress to false in callback API in SW
11/11/15   ag      Add API to return whether WTR supports Autopin
11/05/15   ag      Add Autopin read APIs to be caalled as callback from FED
09/18/15   aa      Add Reference NV pin value to autopin accumulator
06/11/15   aa      Initial version.

============================================================================*/
#include "rfwcdma_core_temp_comp.h"
#include "rfwcdma_core_auto_pin.h"
#include "rflm_wcdma_msg.h"
#include "rfcommon_autopin.h"
#include "DDIAdc.h"
#include "comdef.h"
#include "rfwcdma_core_txlin.h"
#include "rfwcdma_core_txplim.h"
#include "rfwcdma_core_rxctl.h"
#include "rfnv_wcdma.h"
#include "rfdevice_wcdma_intf.h"
#include "rfcommon_nv_mm.h"
#include "ftm.h"
#include "pm.h"
#include "rfc_card.h"
#include "rfwcdma_mdsp.h"
#include "rfwcdma_mdsp_async.h"
#include "rfwcdma_mc.h"
#include "rfm_internal.h"
#include "rfcommon_math.h"
#include "rfwcdma_core.h"
#include "rfwcdma_core_util.h"
#include "rfwcdma_core_xpt.h"
#include "rfcommon_fbrx_api.h"
#include "rfwcdma_data.h"
#include "rfcommon_locks.h"
#include "rflm_wcdma_sw_api.h"
#include "rfdevice_hdet_cmn_intf.h"
#include "rfdevice_hdet_wcdma_intf.h"
#include "rf_cmd_interface.h"
#include "rfm_device_types.h"
#include "rflm_wcdma_autopin.h"
#include "lm_types.h"
#include "rfcommon_autopin_msm.h"
#include "txlm_hal.h"



extern rfwcdma_mc_state_type rfwcdma_mc_state;
extern rfwcdma_core_txlin_record_type rfwcdma_core_current_txlin_data;
extern rfwcdma_core_temp_comp_value_type rfwcdma_core_temp_comp_value;
int16 pout_comp_auto_pin_ref[RFCOM_TXLIN_NUM_LINEARIZERS] = {0};
int16 pin_comp_auto_pin_ref[RFCOM_TXLIN_NUM_LINEARIZERS] = {0};

extern boolean is_autopin_supported;

/*----------------------------------------------------------------------------*/
/*!
  @brief
 Function to check Auto Pin support 
 
  @details
  This function is to check if  Auto Pin support is there in WTR driver. If support is not there then autopin will not be scheduled.
*/

void  rfwcdma_auto_pin_supported
(
rfm_device_enum_type device,
rfcom_wcdma_band_type band
)
{
  
  rfdevice_rxtx_common_class* dev_ptr = NULL;
     /* Get the device pointer for the WTR */
  dev_ptr = rfdevice_wcdma_tx_get_common_dev_ptr(device, band);

   if(dev_ptr !=NULL)
   {
     rfdevice_cmn_get_autopin_par_support(dev_ptr, &is_autopin_supported);
   }
   else
   {
     RF_MSG(RF_ERROR,"rfwcdma_auto_pin_supported: dev_ptr is NULL");
   }
 }

/*----------------------------------------------------------------------------*/
/*!
  @brief
 Function to populate input parameters for Autopin captures
  @details
  This function is populate the input parameters required for WTR and Reflog captures for Autopin. Must be populate before script building and after state machine update
*/

void rfwcdma_core_autopin_init
(
uint32 tx_handle
)

{
#ifdef FEATURE_TABASCO_MODEM
 rflm_autopin_dm_template_t *autopin_template = NULL;
 fw_autopin_capture_input_t *reflog_capture_input = NULL;
 uint16 *wtr_capture_size;
 double fbrx_adc_clock_freq_mhz;
 rfdevice_rxtx_common_class *device_ptr = NULL;
 rfm_device_enum_type device;
 uint16 band = 0,chan = 0;
 rflm_wcdma_dm_buf_type *wcdma_dm_data_ptr = NULL;
 boolean buffer_valid = FALSE;
 uint32 extra_capture_time;
 uint32 new_delay;
 uint32 txc_rate_ustmr;
 txlm_chain_type tx_chain = TXLM_CHAIN_MAX;

 device = rfwcdma_data_get_tx_device();
 chan = rfwcdma_data_get_curr_chan(device,RFCOM_SINGLE_CARRIER);
 band = rfwcdma_data_get_curr_band(device);

 buffer_valid = rflm_dm_get_handle_tech_data((rflm_dm_handle_id_t)tx_handle, (void *)&wcdma_dm_data_ptr, RFLM_DM_TECH_DATA_GET_PTR);

 autopin_template = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(tx_handle);
 if (autopin_template == NULL)
 {
   RF_MSG_1(RF_ERROR,"rfwcdma_core_autopin_init: Failed to get AutoPin buffer from DM for Tx handle %d, can't populate input parameters", tx_handle);
   return;
 }

  /*Initialize pin_accum corresponding to all pA states to NULL at every tune/retune*/
  memset(autopin_template->autopin_ctl.pin_accum,0,RFLM_TXAGC_PA_STATE_MAX*sizeof(rflm_db10_t));

  reflog_capture_input = &autopin_template->autopin_ctl.reflog_capture_input;
  wtr_capture_size = &autopin_template->autopin_ctl.wtr_capture_size;

  rfcommon_msm_get_txc_rollover_period((lm_handle_type)tx_handle,
	 		                          &reflog_capture_input->txc_input_freq_hz, 
				                      &reflog_capture_input->txc_rate_ns,
				                      &reflog_capture_input->txrot_clk_hz);
	
  rfcommon_msm_get_par_capture_params(RFLM_TECH_WCDMA,
				                    &(reflog_capture_input->capture_length),
				                    &(reflog_capture_input->txc_down_sampling_rate), 
			                        &(reflog_capture_input->tx_capture_delay),
                                     wtr_capture_size);

  txc_rate_ustmr = ((uint32)(19200000 / 1000000.0 * reflog_capture_input->txc_rate_ns / 1000));
  
  autopin_template->autopin_ctl.txc_rate_ustmr = txc_rate_ustmr;
  
  /*delay has to be less than txc rate as per fw requirement, hence calculate a new delay smaller than txc rate and add the extra time removed from delay to the main capture time. Time at which WTR capture sgould happen = action time+ extra capture time+ delay, 
      Time we should pass FW to start reflog capture  = action time + extra capture tim, FW adds delw by itself */
  /*calculate new delay and extra capture time so as net delay is less than txc rate and adjust time lost in delay with extra capture time*/
  extra_capture_time = (((uint32)reflog_capture_input->tx_capture_delay)/txc_rate_ustmr)*txc_rate_ustmr;
  new_delay = reflog_capture_input->tx_capture_delay - extra_capture_time;

  if ((buffer_valid  == TRUE) && (wcdma_dm_data_ptr != NULL))
  {
    wcdma_dm_data_ptr->tx_data.autopin_cfg.extra_capture_time = extra_capture_time;
  }
  else
  {	
    RF_MSG_2(RF_ERROR,"rfwcdma_core_autopin_init(): wcdma_dm_data_ptr can't be fetched,buffer_valid %d,wcdma_dm_data_ptr %d",buffer_valid, wcdma_dm_data_ptr);
  }

  reflog_capture_input->tx_capture_delay = new_delay;
  
  (void)txlm_hal_get_chain(tx_handle, &tx_chain);
  reflog_capture_input->tx_chain = (uint32)tx_chain;

  /* Get the device pointer for the WTR */
  device_ptr = rfdevice_wcdma_tx_get_common_dev_ptr(device, band);
  
  rfdevice_cmn_get_fbrx_adc_clk_freq(device_ptr,
  	                                 RFM_IMT_MODE,
  	                                 band,
  	                                 chan,
  	                                 &fbrx_adc_clock_freq_mhz);
  
  *wtr_capture_size = (uint16)((AUTOPIN_PAR_CAPTURE_WINDOW_WCDMA * fbrx_adc_clock_freq_mhz)/6);

  RF_MSG_2(RF_HIGH, "rfcommon_msm_get_par_capture_params(): fbrx_adc_clock_freq_mhz %.1f,wtr_capture_length %d", fbrx_adc_clock_freq_mhz,*wtr_capture_size);

#endif	  
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Auto Pin update Handler function
 
  @details
  This function will be called by RF APPS task dispatcher when auto pin
  command is posted to the command Q beloging to RF APPS task. This function
  will perform Linearizer update and applies Pin value
*/
void
rfwcdma_auto_pin_update_handler
(
  void *cmd_ptr,
  rf_dispatch_cid_info_type *cid_info,
  void *cb_data
  )
{
  rf_time_type autopin_time;      /* prep_wakeup Time */
  rf_time_tick_type autopin_tick;      /* prep_wakeup Time */
  rfm_device_enum_type device = RFM_DEVICE_0;
  rf_path_enum_type path = RF_PATH_0;
  uint8  i;
  rflm_wcdma_process_auto_pin_payload_type *auto_pin_data = NULL;
  rfwcdma_core_txagc_comp_type update_type[RFCOM_TXLIN_NUM_LINEARIZERS];
  int16 pin_comp_offset;
  rfwcdma_data_status_type* radio_status = NULL;
  rflm_autopin_dm_template_t *autopin_template = NULL;	
  uint32 handle_id;
  /* Start Profiling of API */
  autopin_tick = rf_time_get_tick();

  /* Sanity - check if data ptr from MSGR is valid */
  if (cmd_ptr == NULL)
  {
    RF_MSG(RF_ERROR, "rfwcdma_auto_pin_update_handler: NULL MSGR data ptr!");
    return;
  }

  /* Get the payload from MSGR cmd_ptr */
  auto_pin_data = (rflm_wcdma_process_auto_pin_payload_type *)((rf_cmd_type *)cmd_ptr)->payload;
  device = rfwcdma_data_get_tx_device();
  update_type[auto_pin_data->pa_state] = TX_AGC_AUTO_PIN_COMP;
  pin_comp_offset = auto_pin_data->pin_error;
  handle_id = auto_pin_data->handle_id;

  /* Throw F3 Message */
  RF_MSG_3(RF_HIGH, "rfwcdma_auto_pin_update_handler : START on device %d, pin = %d, tx_handle = %d", device, pin_comp_offset,  auto_pin_data->handle_id  );

  radio_status = rfwcdma_get_radio_status(device);

  autopin_template = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle_id);
  
  if ( radio_status == NULL)
  {
     RF_MSG_1(RF_ERROR, "rfwcdma_auto_pin_update_handler : NULL radio status for device %d", device );
     return;
  }
  
  RF_MSG_3(RF_ERROR, "rfwcdma_auto_pin_update_handler : rflm handle on device %d = %d, passed tx_handle = %d", device, radio_status->rflm_handle  ,   auto_pin_data->handle_id  );

  if ( radio_status->rflm_handle == auto_pin_data->handle_id  )//rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_TX)
  {

    /*Temp compensation should not happen during RF Calibration(FTM state)*/
    if ( (!IS_FTM_IN_TEST_MODE()) || ( rfm_get_calibration_state() == FALSE ) )
    {

      if (autopin_template == NULL)
      {
        RF_MSG_1(RF_ERROR,"rfwcdma_core_autopin_update_pin: Failed to get AutoPin buffer from DM, can't log ref pin %d", pin_comp_auto_pin_ref[auto_pin_data->pa_state]);
      }
      else
      {
        autopin_template->autopin_ctl.pin_init = pin_comp_auto_pin_ref[auto_pin_data->pa_state];
      }
      pin_comp_offset += pin_comp_auto_pin_ref[auto_pin_data->pa_state]  ; 
      //RF_MSG_3(RF_ERROR, "rfwcdma_auto_pin_update_handler :  pin error = %d, pin ref = %d , PA state  = %d", pin_comp_offset, pin_comp_auto_pin_ref[auto_pin_data->pa_state], auto_pin_data->pa_state);
      rfwcdma_core_txlin_auto_pin_comp_update(device, handle_id, pin_comp_offset, auto_pin_data->pa_state );
    }


  }
  else
  {
    RF_MSG_1(RF_HIGH, "rfwcdma_temp_comp_update_handler: Skipping "
             "RadioState %d", rfwcdma_data_get_rf_state(device));
  }

/* get time spent in API */
  autopin_time = rf_time_get_elapsed(autopin_tick, RF_USEC);

  RF_MSG_1(RF_HIGH, "rfwcdma_auto_pin_update_handler : TimeTaken %d us",
           autopin_time);

} /* rfwcdma_auto_pin_update_handler */

/*----------------------------------------------------------------------------*/

/*!
  @brief
  Auto Pin update update function
 
  @details
  This function will be called bwhen both WTR and Reflog captures are done and cumulative pin accum valure is calcualted. This function
  will perform Linearizer update and applies Pin value  */

void rfwcdma_core_autopin_update_pin
(
uint32 handle_id, 
uint8 pa_state,
int16 pin_comp_offset
)
{
   rfm_device_enum_type device = RFM_DEVICE_0;
   rfwcdma_core_txagc_comp_type update_type[RFCOM_TXLIN_NUM_LINEARIZERS];
   rfwcdma_data_status_type* radio_status = NULL;
   rflm_autopin_dm_template_t *autopin_template = NULL;	
   device = rfwcdma_data_get_tx_device();
   if(device >= RFM_MAX_WAN_DEVICES)
   {
     RF_MSG_2(RF_ERROR, "rfwcdma_core_autopin_update_pin : invalid Tx device %d for handle %d, can't apply pin", device, handle_id);
   }
   	
   update_type[pa_state] = TX_AGC_AUTO_PIN_COMP;
	
   radio_status = rfwcdma_get_radio_status(device);

  if ( radio_status == NULL)
  {
     RF_MSG_1(RF_ERROR, "rfwcdma_auto_pin_update_handler : NULL radio status for device %d", device );
     return;
  }
  
  autopin_template = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(handle_id);

  if ( radio_status->rflm_handle == handle_id  )//rfwcdma_data_get_rf_state(device) == RFWCDMA_STATE_TX)
  {
	
    /*Temp compensation should not happen during RF Calibration(FTM state)*/
    if ( (!IS_FTM_IN_TEST_MODE()) || ( rfm_get_calibration_state() == FALSE ) )
    {  
      if (autopin_template == NULL)
      {
        RF_MSG_1(RF_ERROR,"rfwcdma_core_autopin_update_pin: Failed to get AutoPin buffer from DM, can't log ref pin %d", pin_comp_auto_pin_ref[pa_state]);
      }
      else
      {
        autopin_template->autopin_ctl.pin_init = pin_comp_auto_pin_ref[pa_state];
      }
      pin_comp_offset += pin_comp_auto_pin_ref[pa_state]  ; 

	   RF_MSG_5(RF_HIGH, 
				"rfwcdma_core_autopin_update_pin [Dev %d] [handle %d] :  pin_comp_val = %d, pin ref = %d , PA state = %d",
				device,
				handle_id,	
				pin_comp_offset, 
				pin_comp_auto_pin_ref[pa_state], 
				pa_state);


       rfwcdma_core_txlin_auto_pin_comp_update(device, handle_id, pin_comp_offset, pa_state );
     }

   }
	  
   else
   {
      RF_MSG_1(RF_HIGH, "rfwcdma_temp_comp_update_handler: Skipping "
				 "RadioState %d", radio_status->rf_state);
   }
	
} /* rfwcdma_auto_pin_update_pin */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Auto Pin read Handler function
 
  @details
  This function will be called by RF APPS task dispatcher when auto pin 
  command is posted to the command Q beloging to RF APPS task. This function
  calls PAR read API.
*/


void rfwcdma_auto_pin_trigger_read_handler
(
  void *cmd_ptr,
  rf_dispatch_cid_info_type *cid_info,
  void *cb_data
)
{ 
  rfwcdma_auto_pin_trigger_read();
} /*rfwcdma_auto_pin_trigger_read_handler */

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Auto Pin trigger read function
 
  @details
  This function will be called by RF APPS task dispatcher when auto pin 
  command is posted to the command Q beloging to RF APPS task. This function
  reads PAR for WTR capture
*/

void rfwcdma_auto_pin_trigger_read()
{
  rfdevice_rxtx_common_class* cmn_device_ptr = NULL;
  rfm_device_enum_type tx_device;
  rfcom_wcdma_band_type band;
  rflm_handle_tx_t handle;

  rflm_wcdma_dm_buf_type *wcdma_dm_data_ptr = NULL;
  boolean buffer_valid = FALSE;

  rfcommon_autopin_capture_control_type control_input = { 0 };
  boolean status = FALSE;

  /*get current tx device*/
  tx_device = rfwcdma_data_get_tx_device();

  if((tx_device < RFM_DEVICE_0)||(tx_device >= RFM_MAX_WAN_DEVICES))
  {
   RF_MSG_1(RF_ERROR, 
           "rfwcdma_auto_pin_trigger_read failed,Invalid tx device %d", 
            tx_device);
   return;
  }

    /*get handle of tx device*/
  handle = (rflm_handle_tx_t)rfwcdma_data_get_rflm_buf_idx(tx_device);

   if( !IS_VALID_TXLM_HANDLE (handle))
   	{
      RF_MSG_1(RF_ERROR, 
               "rfwcdma_auto_pin_trigger_read failed,Invalid tx handle %d", 
               handle); 	
	  
	  return;
   	}
  
  band = rfwcdma_data_get_curr_band(tx_device);

     
  /* Get the device pointer for the WTR */
  cmn_device_ptr = rfdevice_wcdma_tx_get_common_dev_ptr(tx_device, band);
    
  if(cmn_device_ptr == NULL)
  {
    RF_MSG(RF_ERROR, 
          "rfwcdma_auto_pin_trigger_read: cmn_device_ptr is NULL  ");
	return;
  }

  status = rfdevice_cmn_fbrx_autopin_read_and_disable(cmn_device_ptr,
                                             &control_input.wtr_capture_result,
                                             FALSE,
                                             NULL);


  rflm_wcdma_unlock_meter_trigger(handle);    
 
  buffer_valid = rflm_dm_get_handle_tech_data((rflm_dm_handle_id_t)handle, (void *)&wcdma_dm_data_ptr, RFLM_DM_TECH_DATA_GET_PTR);

  if ((buffer_valid  == TRUE) && (wcdma_dm_data_ptr != NULL))
  {
    wcdma_dm_data_ptr->tx_data.tx_status.enable_autopin_in_progress = FALSE;

    RF_MSG_1(RF_LOW,
             "rfwcdma_auto_pin_trigger_read:enable_autopin_in_progress%d",
              wcdma_dm_data_ptr->tx_data.tx_status.enable_autopin_in_progress);
  }
  else
  {
    RF_MSG_2(RF_ERROR,
            "rfwcdma_auto_pin_trigger_read:dm_tech_get_data failed, buffer_valid %d, wcdma_dm_data_ptr 0x%x",
            (int32) buffer_valid,(int32)wcdma_dm_data_ptr);
  }


  control_input.capture_type = AUTOPIN_PAR_WTR_CAPTURE;
  control_input.valid_capture = status;
  control_input.par = control_input.wtr_capture_result.par;

  rfcommon_autopin_capture_control(handle,
                                   control_input);
  
#if 0
  RF_MSG_8(RF_MED,
           "rfwcdma_auto_pin_trigger_read: 0x%x, 0x%x, 0x%x, %d, %d, %d, %d, %d",
            control_input.wtr_capture_result.sum_i_q_sqr, control_input.wtr_capture_result.sum_i_and_q_sqr_ave, control_input.wtr_capture_result.p_ave, control_input.wtr_capture_result.sum_i, control_input.wtr_capture_result.sum_q, control_input.wtr_capture_result.i_dc, control_input.wtr_capture_result.q_dc, control_input.wtr_capture_result.fbrx_accum);

  RF_MSG_3(RF_MED,
           "rfwcdma_auto_pin_trigger_read:%d %d, %d",
            control_input.wtr_capture_result.sum_peaks,control_input.wtr_capture_result.num_peaks, control_input.wtr_capture_result.par);
#endif 

}/* rfwcdma_auto_pin_trigger_read */



