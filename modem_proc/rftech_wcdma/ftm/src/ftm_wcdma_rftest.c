/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              FTM Log

GENERAL DESCRIPTION
  This is the Source File for RF test framework for WCDMA.

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_wcdma.mpss/3.11/ftm/src/ftm_wcdma_rftest.c#1 $
  $DateTime: 2021/03/09 05:08:47 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/13/15   vbh     Added support for the rx meas command
6/29/15   vbh       populate PRx device param for enter mode
06/09/15   ag      Fix klocwork warnings
05/05/15   ak      Implementaion of the start, add, drop and reconfig APIs
05/04/15   ak      Initial revision.

===========================================================================*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rfa_variation.h"
#include "customer.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmicap.h"

#ifdef FTM_HAS_UMTS
#include "comdef.h"
#include "target.h"

#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "ctype.h"  /*lint -esym(766,ctype.h)*/
#include "ftm.h"
#include "rfumts.h"
#include "ftm_wcdma_dispatch.h"
#include "rfwcdma_core_util.h"
#include "rfm_wcdma.h"
#include "ftm_msg.h"
#include "ftm_wcdma_rftest.h"
#include "rfm_wcdma_ftm.h"



#ifdef FEATURE_FTM_HWTC
#include "ftm_hwtc_dc.h"
#endif
/* memscpy */
#include "stringl.h"


extern ftm_rf_test_radio_config_unpack_data_t * ftm_rftest_dbg[MAX_RFTEST_IDX];

extern uint8 ftm_rftest_cntr;

extern ftm_lm_buf_type ftm_wcdma_rxlm_buffer[RFM_MAX_WAN_DEVICES];
extern ftm_lm_buf_type ftm_wcdma_txlm_buffer;
extern ftm_rf_mode_type ftm_current_rf_mode;
extern rfcom_wcdma_band_type ftm_curr_wcdma_mode;
extern boolean ftm_wcdma_tx_on_status;
extern boolean ftm_wcdma_div_status[RFM_MAX_WAN_DEVICES];


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the WCDMA radio test command
 
  @details
  This function is used to handle the WCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_wcdma_start_radiotest
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
ftm_rf_test_field_error_code_mask_type status = 0;

FTM_MSG_1(FTM_HIGH, "ftm_wcdma_start_radiotest: Received start command ", 0);

/*Keep track of last 10 radio config commands*/
ftm_rftest_dbg[ftm_rftest_cntr++ % MAX_RFTEST_IDX] = command_unpack_data;

/*Follow this order of Drop, Add and Reconfig*/

/*Step 1 : Process the Drop list*/
status |= ftm_wcdma_process_rftest_drop(command_unpack_data);

/*Step 2 :  Process the Add list*/
status |= ftm_wcdma_process_rftest_add(command_unpack_data);

/*Step 3: Process the Reconfig list*/
status |= ftm_wcdma_process_rftest_reconfig(command_unpack_data);

FTM_MSG_1(FTM_HIGH, "ftm_wcdma_start_radiotest: Processed start command ", 0);

return status;
}


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the WCDMA radio test command
 
  @details
  This function is used to handle the WCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_wcdma_process_rftest_add
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
 ftm_rf_test_radio_config_unpack_data_t * rftest_add_ptr = NULL;
 ftm_rf_test_rfm_device_mask_type device_mask = 0;
 rfm_device_enum_type device = RFM_MAX_WAN_DEVICES;
 rfm_wcdma_enter_mode_param enter_param = {0};
 rfm_wcdma_power_mgmt_param pwr_param = {0};
 uint8 num_chan = 0;
 uint16 channel_list[FTM_RF_TEST_CHANNEL_NUM] = {0};
 rfcom_wcdma_band_type band = RFCOM_BAND_INVALID;
 rfm_wcdma_tx_init_params tx_params = {0};
 uint8 carrier_mask = 0;
 ftm_rf_test_field_error_code_mask_type status = 0;
 
 rftest_add_ptr = command_unpack_data;

 /*Process the enabled add list device mask to extract each of the devices*/
 device_mask = rftest_add_ptr->add_list.device_mask;

 /*Populate the device independent rx power params*/
 pwr_param.on_off  = TRUE;
 /*This subtraction for enum compatibility*/
 pwr_param.sub_type = (rfm_wcdma_subscription_type)(rftest_add_ptr->subscriber -1);

 FTM_MSG_1(FTM_HIGH, "ftm_wcdma_process_rftest_add: Received Radio Add Request ", 0);
 
 while (device_mask != 0)
 {
   device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);
   if((device < RFM_DEVICE_0)||(device >= RFM_MAX_WAN_DEVICES))
   {
      status |= FTM_RF_TEST_EC_NULL_PARAM; /*Check with Anuj on the error codes mask to be used*/
      FTM_MSG_1(FTM_ERROR, "ftm_wcdma_process_rftest_add: Invalid device id %d", device);
      return status;
   }

   band = (rfcom_wcdma_band_type)rftest_add_ptr->add_list.cmd_params[device].band;
   
   if((band >= RFCOM_NUM_WCDMA_BANDS))
   {
     status |= FTM_RF_TEST_EC_NULL_PARAM; /*Check with Anuj on the error codes mask to be used*/
     FTM_MSG_1(FTM_ERROR, "ftm_wcdma_process_rftest_add: Invalid band %d", band);
     return status;
    }

   carrier_mask = rftest_add_ptr->add_list.cmd_params[device].carrier_mask;
   /*This has to become device specific for CRAT Testing going forward. Can't Change it now due to compilation dependency across techs*/
   ftm_wcdma_rftest_set_mode(band);

   if(rftest_add_ptr->add_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_RX)
   {/*Device is an Rx Device*/        

      while(carrier_mask != 0)
	  {
		channel_list[num_chan] = rftest_add_ptr->add_list.cmd_params[device].per_carrier[num_chan].channel;
        FTM_MSG_4(RF_HIGH, "ftm_wcdma_process_rftest_add: Channel: %d on device %d, chan number %d, carrier_mask ",
                                         channel_list[num_chan],device,num_chan, carrier_mask);
        num_chan++;
        carrier_mask &= (~rf_extract_first_one_bit_mask(carrier_mask));
	  } 
	 
     if(rftest_add_ptr->add_list.cmd_params[device].is_primary == TRUE)
	 {/*Device Entered as primary Rx Device*/	 
       enter_param.device_type = RFM_WCDMA_PRIMARY_RX_DEVICE;	
       enter_param.prx_device = RFM_INVALID_DEVICE;
          
	   ftm_rfmode_enter(device, FTM_STATE_WCDMA );	   
	   rfm_enter_mode(device, RFCOM_WCDMA_MODE, 
					   &enter_param, NULL, ftm_wcdma_rxlm_buffer[device].buf_idx);
	   pwr_param.device = device;
	   rfm_wcdma_power_on_off(&pwr_param,NULL,NULL);

       num_chan = rf_count_one_bits(rftest_add_ptr->add_list.cmd_params[device].carrier_mask);
	   (void)((num_chan == 1) ? ftm_wcdma_tune_to_chan(device, channel_list[0]):
	   						ftm_wcdma_tune_to_multi_chan_v2(device, channel_list));
	 }
	 else
	 {/*Device Entered as Diversity Device*/
	   enter_param.device_type = RFM_WCDMA_DIVERSITY_RX_DEVICE;
       enter_param.prx_device = RFM_INVALID_DEVICE;

	   /*This will work only if the associated Rx has already entered as primary. Set Secondary Chain has a check inside*/
	   ftm_wcdma_set_secondary_chain(device, TRUE);
	 }

   }

   else if(rftest_add_ptr->add_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_TX)
   {/*Device is a Tx Device*/
     tx_params.carrier_mask = rftest_add_ptr->add_list.cmd_params[device].carrier_mask;
	 tx_params.device = device;
	 tx_params.txlm_handle = ftm_wcdma_txlm_buffer.buf_idx;
	 tx_params.tx_channel[0] = rftest_add_ptr->add_list.cmd_params[device].per_carrier[0].channel;
     ftm_wcdma_set_tx_v2(device, TRUE, tx_params);
   }
   else
   {
    FTM_MSG_1(FTM_ERROR, "ftm_wcdma_process_rftest_add: Invalid Tranceiver type %d ", 
						rftest_add_ptr->add_list.cmd_params[device].trx);
   }
   device_mask &= (~rf_extract_first_one_bit_mask(device_mask));
 }

 return status;

}


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the WCDMA radio test command
 
  @details
  This function is used to handle the WCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_wcdma_process_rftest_reconfig
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
  ftm_rf_test_radio_config_unpack_data_t * rftest_reconfig_ptr = NULL;
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_MAX_WAN_DEVICES;
  rfcom_wcdma_band_type band = RFCOM_BAND_INVALID;  
  uint8 num_chan = 0, carrier_mask =0;
  uint16 channel_list[FTM_RF_TEST_CHANNEL_NUM] = {0};
  rfm_wcdma_tx_init_params tx_params = {0};
 rfm_wcdma_enter_mode_param enter_param = {0};
  rfm_wcdma_power_mgmt_param pwr_param = {0};
  ftm_rf_test_field_error_code_mask_type status = 0;  
  
  rftest_reconfig_ptr = command_unpack_data;
  /*Process the enabled add list device mask to extract each of the devices*/
  device_mask = rftest_reconfig_ptr->reconfig_list.device_mask;

  while (device_mask != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);
    if((device < RFM_DEVICE_0)||(device >= RFM_MAX_WAN_DEVICES))
    {
      status |= FTM_RF_TEST_EC_NULL_PARAM; /*Check with Anuj on the error codes mask to be used*/
      FTM_MSG_1(FTM_ERROR, "ftm_wcdma_process_rftest_reconfig: Invalid device %d", device);
      return status;
    }

    band = (rfcom_wcdma_band_type)rftest_reconfig_ptr->reconfig_list.cmd_params[device].band;

    if((band >= RFCOM_NUM_WCDMA_BANDS))
    {
      status |= FTM_RF_TEST_EC_NULL_PARAM; /*Check with Anuj on the error codes mask to be used*/
      FTM_MSG_1(FTM_ERROR, "ftm_wcdma_process_rftest_reconfig: Invalid band %d", band);
      return status;
    }
	/*This has to become device specific for CRAT Testing going forward. 
	   Can't Change it now due to compilation dependency across techs*/
	ftm_wcdma_rftest_set_mode(band);
    carrier_mask =rftest_reconfig_ptr->reconfig_list.cmd_params[device].carrier_mask;

	if(rftest_reconfig_ptr->reconfig_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_RX)
	{/*Device is an Rx Device*/ 	   
	  while(carrier_mask != 0)
	  {
		channel_list[num_chan] = rftest_reconfig_ptr->reconfig_list.cmd_params[device].per_carrier[num_chan].channel;
        num_chan++;
        carrier_mask &= (~rf_extract_first_one_bit_mask(carrier_mask));
	  } 
	  
	  if(rftest_reconfig_ptr->reconfig_list.cmd_params[device].is_primary == TRUE)
	  {/*Device Entered as primary Rx Device. Diversity will be re-tuned if it is already enabled*/
	   /*PRX/DRX only retune is not allowed if the other device is also added a;read*/
		(void)((num_chan == 1) ? ftm_wcdma_tune_to_chan(device, channel_list[0]):
					 ftm_wcdma_tune_to_multi_chan_v2(device, channel_list));
	  }
	  else
	  {/*Diversity device to be re-configured based on the PRX devices in tune to multichan if it is enabled*/
	   /*No need to support PRX only re-tune when diversity is enabled and the other way. No use case*/
#if 0
	  /*Device Entered as Diversity Device*/
		enter_param.device_type = RFM_WCDMA_DIVERSITY_RX_DEVICE;
		if(ftm_wcdma_div_status[device] == TRUE)
		{
		  ftm_wcdma_set_secondary_chain(device, FALSE);
		}
		/*This will work only if the associated Rx has already entered as primary. Set Secondary Chain has a check inside*/
		ftm_wcdma_set_secondary_chain(device, TRUE);
#endif		
	  }	
	}
	
	else if(rftest_reconfig_ptr->reconfig_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_TX)
	{/*Device is a Tx Device*/
	  tx_params.carrier_mask = rftest_reconfig_ptr->reconfig_list.cmd_params[device].carrier_mask;
	  tx_params.device = device;
	  tx_params.txlm_handle = ftm_wcdma_txlm_buffer.buf_idx;
	  tx_params.tx_channel[0] = rftest_reconfig_ptr->reconfig_list.cmd_params[device].per_carrier[0].channel;
	  if(ftm_wcdma_tx_on_status == TRUE)
	  {
        ftm_wcdma_set_tx_v2(device, FALSE, tx_params);
	  }
	  ftm_wcdma_set_tx_v2(device, TRUE, tx_params);
	}
	else
	{
	 FTM_MSG_1(FTM_ERROR, "ftm_wcdma_process_rftest_reconfig: Invalid Tranceiver type %d ", 
						 rftest_reconfig_ptr->reconfig_list.cmd_params[device].trx);
	}
    device_mask &= (~rf_extract_first_one_bit_mask(device_mask));
  }  

return status;

}

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the WCDMA radio test command
 
  @details
  This function is used to handle the WCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_wcdma_process_rftest_drop
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
  ftm_rf_test_radio_config_unpack_data_t * rftest_drop_ptr = NULL;
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_MAX_WAN_DEVICES;
  rfcom_wcdma_band_type band = RFCOM_BAND_INVALID;  
  uint8 num_chan = 0;
  uint16 channel_list[FTM_RF_TEST_CHANNEL_NUM] = {0};
  rfm_wcdma_tx_init_params tx_params = {0};
  uint8 carrier_mask = 0;
  rfm_wcdma_enter_mode_param enter_param = {0};
  rfm_wcdma_power_mgmt_param pwr_param = {0};
  ftm_rf_test_field_error_code_mask_type status = 0;	
	
  rftest_drop_ptr = command_unpack_data;
  /*Process the enabled add list device mask to extract each of the devices*/
  device_mask = rftest_drop_ptr->drop_list.device_mask;

  while (device_mask != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

	 if((device < RFM_DEVICE_0) || (device >=RFM_MAX_WAN_DEVICES))
	 {
	   RF_MSG_1(FTM_ERROR, "ftm_wcdma_process_rftest_drop: Invalid device id %d", device);
	   status |= FTM_RF_TEST_EC_NULL_PARAM;
	   return status;
	 }
	
	/*This has to become device specific for CRAT Testing going forward. 
	   Can't Change it now due to compilation dependency across techs*/
	//ftm_wcdma_rftest_set_mode(band);
	if(rfwcdma_data_get_device_type(device) == RFM_WCDMA_PRIMARY_RX_DEVICE)
	{/*Device is an Rx Device*/ 	    	
      ftm_wcdma_sleep(device);
	  ftm_rfmode_exit(device,FTM_STATE_WCDMA);
	}
	else if(rfwcdma_data_get_device_type(device) == RFM_WCDMA_DIVERSITY_RX_DEVICE)
    {/*Device Entered as Diversity Device*/
		enter_param.device_type = RFM_WCDMA_DIVERSITY_RX_DEVICE;
		if(ftm_wcdma_div_status[device] == TRUE)
		{
		  ftm_wcdma_set_secondary_chain(device, FALSE);
		}
	}	
	else if(rfwcdma_data_get_device_type(device) == RFM_WCDMA_TX_DEVICE)
	{/*Device is a Tx Device*/
	  tx_params.carrier_mask = carrier_mask;
	  tx_params.device = device;
	  tx_params.txlm_handle = ftm_wcdma_txlm_buffer.buf_idx;
	  tx_params.tx_channel[0] = rfwcdma_data_get_curr_chan(device, RFCOM_SINGLE_CARRIER);
	  if(ftm_wcdma_tx_on_status == TRUE)
	  {
        ftm_wcdma_set_tx_v2(device, FALSE, tx_params);
	  }
	}
	else
	{
	 FTM_MSG_2(FTM_ERROR, "ftm_wcdma_process_rftest_drop: Invalid Tranceiver type %d for device %d", 
						 rfwcdma_data_get_device_type(device), device);
	}
    device_mask &= (~rf_extract_first_one_bit_mask(device_mask));
  }

  return status;
}


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the WCDMA radio test command
 
  @details
  This function is called to set the current mode of operation
 
  @param band
  Specifies the band/mode to be set
*/

void
ftm_wcdma_rftest_set_mode
(
rfcom_wcdma_band_type band
)
{
 ftm_curr_wcdma_mode = band;

 switch(band)
  {
    case RFCOM_BAND_IMT:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_IMT;
      break;

    case RFCOM_BAND_1900:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_1900MHZ_A;
      break;

    case RFCOM_BAND_BC3:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_BC3;
      break;

    case RFCOM_BAND_BC4:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_BC4;
      break;

    case RFCOM_BAND_800:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_800MHZ;
      break;

    case RFCOM_BAND_BC8:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_BC8;
      break;

    case RFCOM_BAND_BC9:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_BC9;
      break;

    case RFCOM_BAND_BC11:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_BC11;
      break;

    case RFCOM_BAND_BC19:
      ftm_current_rf_mode = FTM_DB_RF_WCDMA_BC19;
      break;

    default:
      break;
  }
}
/*---------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles Rx Measure command

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/
ftm_rf_test_field_error_code_mask_type
ftm_wcdma_rftest_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
)
{
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  rfm_api_status_t api_status = TRUE;
  uint8 meas_index =0, carrier_mask = 0;
  rfm_device_enum_type device;
  rfm_wcdma_ftm_rxagc_info ftm_wcdma_agc_state; 
  rfm_wcdma_rxagc_input_params input_param;
  rfm_wcdma_rxagc_output_params output_param;

  for( meas_index = 0; meas_index < num_of_measurements;
       meas_index ++ )
  {
    
    device = query_data[meas_index].input.device;
    carrier_mask = query_data[meas_index].input.carrier;

    input_param.device= device;
    input_param.car_id = carrier_mask;
    input_param.rxagc_type= RFCOM_SERVINGCELL_RXAGC;
    input_param.rxagc_unit = RFCOM_RXAGC_IN_DBM;
    input_param.band = rfwcdma_data_get_curr_band(device);

    if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_DIVERSITY_RX_DEVICE)
    {
      input_param.ant_idx = RFM_WCDMA_DIVERSITY_ANTENNA;
    }
    else if (rfwcdma_data_get_device_type(device)== RFM_WCDMA_PRIMARY_RX_DEVICE)
    {
      input_param.ant_idx = RFM_WCDMA_PRIMARY_ANTENNA;
    }

    rfm_read_wtr_regs(0);

    api_status = rfm_wcdma_get_rxagc_info(&input_param, &output_param);
   
    if (api_status == RFM_WCDMA_HEALTHY_STATE) 
		{
		  query_data[meas_index].output.rxagc = 10*(output_param.rxagc_val);
    }
    else
    {
      FTM_MSG_2(FTM_ERROR,"ftm_wcdma_rftest_rx_measure: error when reading AGC on device %d, carrier mask %d",
                                                                             device, carrier_mask );
      error_code = 1;
    }
  }

  return error_code;
}/* ftm_wcdma_rftest_rx_measure */



#endif /* FTM_HAS_UMTS */
#endif /* FEATURE_FACTORY_TESTMODE */

