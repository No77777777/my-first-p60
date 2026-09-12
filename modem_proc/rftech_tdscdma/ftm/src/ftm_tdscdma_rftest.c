/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              FTM

GENERAL DESCRIPTION
  This is the Source File for RF test framework for TDSCDMA.

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rftech_tdscdma.mpss/3.11/ftm/src/ftm_tdscdma_rftest.c#1 $
  $DateTime: 2021/03/09 05:09:33 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/21/20   hal     Support QSH minidump for RFTECH TDSCDMA
08/14/15   lwu      Implementaion of Rx measurment 
08/14/15   lwu      Implementaion of the start, add, drop and reconfig APIs
06/05/15   jak      Initial revision.

===========================================================================*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "rfa_variation.h"
#include "customer.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftmicap.h"

#ifdef FEATURE_TDSCDMA
#include "comdef.h"
#include "target.h"

#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "ctype.h"  /*lint -esym(766,ctype.h)*/
#include "ftm.h"
#include "ftm_tdscdma_dispatch.h"
#include "rf_tdscdma_core_util.h"
#include "ftm_tdscdma_ctl.h"
#include "ftm_msg.h"
#include "ftm_tdscdma_rftest.h"
#include "ftm_common_control.h"
#include "ftm_rf_test_rx_measure.h"


#ifdef FEATURE_FTM_HWTC
#include "ftm_hwtc_dc.h"
#endif
/* memscpy */
#include "stringl.h"

extern ftm_rf_test_radio_config_unpack_data_t * ftm_rftest_dbg[MAX_RFTEST_IDX];

extern uint8 ftm_rftest_cntr;

ftm_tdscdma_rftest_state_t ftm_tdscdma_rftest_tgt_state = {RFM_SUBSCRIBER_INVALID,
                                                           FTM_PHONE_MODE_MAX,
														   0xFF,
                                                           RFM_INVALID_DEVICE,
                                                           RFM_INVALID_DEVICE,
                                                           RFM_INVALID_DEVICE,
                                                           FALSE,
                                                           FALSE};

ftm_tdscdma_rftest_state_t ftm_tdscdma_rftest_curr_state = {RFM_SUBSCRIBER_INVALID,
                                                           FTM_PHONE_MODE_MAX,
														   0xFF,
                                                           RFM_INVALID_DEVICE,
                                                           RFM_INVALID_DEVICE,
                                                           RFM_INVALID_DEVICE,
                                                           FALSE,
                                                           FALSE};
/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the TDSCDMA radio test command
 
  @details
  This function is used to handle the TDSCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_tdscdma_start_radiotest
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
  ftm_rf_test_field_error_code_mask_type status = 0;
 
  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_start_radiotest: Received start command ", 0);

  /*Keep track of last 10 radio config commands*/
  ftm_rftest_dbg[ftm_rftest_cntr++ % MAX_RFTEST_IDX] = command_unpack_data;

  /*Follow this order of Drop, Add and Reconfig*/

  /*Step 1 : Process the Drop list*/
  status |= ftm_tdscdma_process_rftest_drop(command_unpack_data);

  /*Step 2 :  Process the Add list*/
  status |= ftm_tdscdma_process_rftest_add(command_unpack_data);

  /*Step 3: Process the Reconfig list*/
  status |= ftm_tdscdma_process_rftest_reconfig(command_unpack_data);

  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_start_radiotest: Processed start command ", 0);

  return status;
}


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the TDSCDMA radio test command
 
  @details
  This function is used to handle the TDSCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_tdscdma_process_rftest_add
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
 ftm_rf_test_radio_config_unpack_data_t * rftest_add_ptr = NULL;
 ftm_rf_test_rfm_device_mask_type device_mask = 0;
 rfm_device_enum_type device = RFM_MAX_WAN_DEVICES;
 ftm_mode_id_type band = FTM_PHONE_MODE_MAX;
 ftm_rf_test_field_error_code_mask_type status = 0;
 
 rftest_add_ptr = command_unpack_data;

 /*Process the enabled add list device mask to extract each of the devices*/
 device_mask = rftest_add_ptr->add_list.device_mask;
  
 FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_add: Received Radio Add Request ", 0);
 
 while (device_mask != 0)
 {
   device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);
   
   /*Parameter check*/
   if((device < RFM_DEVICE_0)||(device >= RFM_MAX_WAN_DEVICES))
   {
      status |= FTM_RF_TEST_EC_NULL_PARAM; 
      FTM_MSG_1(FTM_ERROR, "ftm_tdscdma_process_rftest_add: Invalid device id %d", device);
      return status;
   }

   band = rftest_add_ptr->add_list.cmd_params[device].band;

   if((band != FTM_PHONE_MODE_TDSCDMA_B34)||(band != FTM_PHONE_MODE_TDSCDMA_B39))
   {
      status |= FTM_RF_TEST_EC_NULL_PARAM; 
      FTM_MSG_1(FTM_ERROR, "ftm_tdscdma_process_rftest_add: Invalid band %d", band);
      return status;
   }

   if(rftest_add_ptr->add_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_RX) /*Device is an Rx Device*/
   {
     if(rftest_add_ptr->add_list.cmd_params[device].is_primary) /* for primary */
     {
       ftm_tdscdma_rftest_tgt_state.pri_device = device;
       ftm_tdscdma_rftest_tgt_state.sec_device = rftest_add_ptr->add_list.cmd_params[device].assoc_rfm_device;
	   ftm_tdscdma_rftest_tgt_state.band = band;
	   ftm_tdscdma_rftest_tgt_state.channel = rftest_add_ptr->add_list.cmd_params[device].per_carrier[0].channel;
	   FTM_MSG_4(FTM_HIGH, "ftm_tdscdma_process_rftest_add: Prx added. Prx device:%d, assoc device:%d, band:%d, chan:%d.", 
		   ftm_tdscdma_rftest_tgt_state.pri_device,
		   ftm_tdscdma_rftest_tgt_state.sec_device,
		   ftm_tdscdma_rftest_tgt_state.band,
		   ftm_tdscdma_rftest_tgt_state.channel);
     }
	 else /* for secondary */
	 {
	   ftm_tdscdma_rftest_tgt_state.sec_device = device;
	   ftm_tdscdma_rftest_tgt_state.div_status = TRUE;
	   FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_add: Drx added. Drx device: %d", ftm_tdscdma_rftest_tgt_state.sec_device);
	 }
   }
   else if(rftest_add_ptr->add_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_TX) /*Device is a Tx Device*/
   {
     ftm_tdscdma_rftest_tgt_state.tx_device = device;
//	 ftm_tdscdma_rftest_tgt_state.tx_status = TRUE;
	 FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_add: Tx added. Tx device: %d", ftm_tdscdma_rftest_tgt_state.tx_device);
   }
   else
   {
    FTM_MSG_2(FTM_ERROR, "ftm_tdscdma_process_rftest_add: Invalid trx type %d for device %d", 
						rftest_add_ptr->add_list.cmd_params[device].trx , device);
   }
   device_mask &= (~rf_extract_first_one_bit_mask(device_mask));
 }

 return status;

}


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the TDSCDMA radio test command
 
  @details
  This function is used to handle the TDSCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_tdscdma_process_rftest_reconfig
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
  ftm_rf_test_radio_config_unpack_data_t * rftest_reconfig_ptr = NULL;
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_MAX_WAN_DEVICES;
  ftm_mode_id_type band = FTM_PHONE_MODE_MAX;
  ftm_rf_test_field_error_code_mask_type status = 0;
  
  rftest_reconfig_ptr = command_unpack_data;
  
  /*Process the enabled add list device mask to extract each of the devices*/
  device_mask = rftest_reconfig_ptr->reconfig_list.device_mask;
  
  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: Received Radio Reconfig Request ", 0);
  
  while (device_mask != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);
   
    /*Parameter check*/
    if((device < RFM_DEVICE_0)||(device >= RFM_MAX_WAN_DEVICES))
    {
      status |= FTM_RF_TEST_EC_NULL_PARAM; 
      FTM_MSG_1(FTM_ERROR, "ftm_tdscdma_process_rftest_reconfig: Invalid device id %d", device);
      return status;
    }
    
	band = rftest_reconfig_ptr->reconfig_list.cmd_params[device].band;

    if((band != FTM_PHONE_MODE_TDSCDMA_B34)||(band != FTM_PHONE_MODE_TDSCDMA_B39))
    {
      status |= FTM_RF_TEST_EC_NULL_PARAM; 
      FTM_MSG_1(FTM_ERROR, "ftm_tdscdma_process_rftest_reconfig: Invalid band %d", band);
      return status;
    }

    if(rftest_reconfig_ptr->reconfig_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_RX) 
    {
  	  if(rftest_reconfig_ptr->reconfig_list.cmd_params[device].is_primary) 
  	  {
  	    ftm_tdscdma_rftest_tgt_state.pri_device = device;
  	    ftm_tdscdma_rftest_tgt_state.sec_device = rftest_reconfig_ptr->reconfig_list.cmd_params[device].assoc_rfm_device;
  	    ftm_tdscdma_rftest_tgt_state.band = band;
  	    ftm_tdscdma_rftest_tgt_state.channel = rftest_reconfig_ptr->reconfig_list.cmd_params[device].per_carrier[0].channel;
        FTM_MSG_4(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: Prx added. Prx device:%d, assoc device:%d, band:%d, chan:%d.", 
		   ftm_tdscdma_rftest_tgt_state.pri_device,
		   ftm_tdscdma_rftest_tgt_state.sec_device,
		   ftm_tdscdma_rftest_tgt_state.band,
		   ftm_tdscdma_rftest_tgt_state.channel);
  	  }
  	  else /* for secondary */
  	  {
  	  ftm_tdscdma_rftest_tgt_state.sec_device = device;
  	  ftm_tdscdma_rftest_tgt_state.div_status = TRUE;
	  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: Drx added. Drx device: %d", ftm_tdscdma_rftest_tgt_state.sec_device);
  	  }
    }
    else if(rftest_reconfig_ptr->reconfig_list.cmd_params[device].trx == FTM_RF_TEST_RADIO_TX) /*Device is a Tx Device*/
    {
  	  ftm_tdscdma_rftest_tgt_state.tx_device = device;
  	//  ftm_tdscdma_rftest_tgt_state.tx_status = TRUE;
	  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_add: Tx added. Tx device: %d", ftm_tdscdma_rftest_tgt_state.tx_device);
    }
    else
    {
     FTM_MSG_2(FTM_ERROR, "ftm_tdscdma_process_rftest_reconfig: Invalid trx type %d for device %d", 
  					   rftest_reconfig_ptr->reconfig_list.cmd_params[device].trx , device);
    }
    device_mask &= (~rf_extract_first_one_bit_mask(device_mask));
  }

  /* *******************************************************************************************
   ** start to config :
   **   1. if pri_device changes , we need to exit then enter mode again to track right PRX/DRX/Tx device ID inside FTM TDS layer
   **   2. If div or chan changes , we need to tune_to_chan again
   **   3. if Tx status change , send cmd with intended Tx status
   ** ******************************************************************************************/
  FTM_MSG_2(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: tgt device %d, curr device %d", 
  					   ftm_tdscdma_rftest_tgt_state.pri_device , ftm_tdscdma_rftest_curr_state.pri_device);

  /* if pri_device changes , we need to exit then set mode again to track curr FTM device */
  if ( ftm_tdscdma_rftest_tgt_state.pri_device != ftm_tdscdma_rftest_curr_state.pri_device ) 
  {
    if ( ftm_tdscdma_rftest_curr_state.pri_device != RFM_INVALID_DEVICE ) 
    {
	  /* make sure we turn off Rx & Tx before exit mode */
      ftm_tdscdma_burst_tx( FALSE ); 
      ftm_tdscdma_burst_rx( -1600, FALSE );	  
	  ftm_tdscdma_set_secondary_chain( FALSE );
      ftm_rfmode_exit( ftm_tdscdma_rftest_curr_state.pri_device ,FTM_STATE_TDSCDMA ); 
	  /* clear all curr state */
      ftm_tdscdma_rftest_curr_state.band = FTM_PHONE_MODE_MAX;
	  ftm_tdscdma_rftest_curr_state.channel = 0xFF;
	  ftm_tdscdma_rftest_curr_state.pri_device = RFM_INVALID_DEVICE;
      ftm_tdscdma_rftest_curr_state.sec_device = RFM_INVALID_DEVICE;
      ftm_tdscdma_rftest_curr_state.tx_device = RFM_INVALID_DEVICE;	  	
	  ftm_tdscdma_rftest_curr_state.div_status = FALSE;
	  ftm_tdscdma_rftest_curr_state.tx_status = FALSE;

	  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: exit mode", 0);
    }

	if (ftm_tdscdma_rftest_tgt_state.pri_device != RFM_INVALID_DEVICE)
	{
	  ftm_tdscdma_set_mode( ftm_tdscdma_rftest_tgt_state.band,ftm_tdscdma_rftest_tgt_state.pri_device );
	}
	
	ftm_tdscdma_rftest_curr_state.pri_device = ftm_tdscdma_rftest_tgt_state.pri_device;
  }
  /*retune if target channel or diversity status is different from current status*/
  if(ftm_tdscdma_rftest_tgt_state.pri_device != RFM_INVALID_DEVICE)
  {
    if ( ftm_tdscdma_rftest_curr_state.channel != ftm_tdscdma_rftest_tgt_state.channel || 
         ftm_tdscdma_rftest_curr_state.div_status!= ftm_tdscdma_rftest_tgt_state.div_status )
    { 
	//  int16 DefExpRx = -650;
	  FTM_MSG_4(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: tgt_chan %d, tgt_div_status %d, curr_chan %d, curr_div_status %d", 
			                 ftm_tdscdma_rftest_tgt_state.channel, ftm_tdscdma_rftest_tgt_state.div_status, ftm_tdscdma_rftest_curr_state.channel, ftm_tdscdma_rftest_curr_state.div_status);

      ftm_tdscdma_set_secondary_chain( ftm_tdscdma_rftest_tgt_state.div_status );  
      ftm_tdscdma_tune_to_chan ( ftm_tdscdma_rftest_tgt_state.pri_device , ftm_tdscdma_rftest_tgt_state.channel); 
    //  ftm_tdscdma_burst_rx( DefExpRx , TRUE );
	  ftm_tdscdma_rftest_curr_state.band = ftm_tdscdma_rftest_tgt_state.band;  
      ftm_tdscdma_rftest_curr_state.channel = ftm_tdscdma_rftest_tgt_state.channel;  
	  ftm_tdscdma_rftest_curr_state.div_status = ftm_tdscdma_rftest_tgt_state.div_status;
	  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: Done! Tune to channel %d", ftm_tdscdma_rftest_tgt_state.channel);
    }

	/* if curr Tx status changes */
    if ( ftm_tdscdma_rftest_curr_state.tx_device!= ftm_tdscdma_rftest_tgt_state.tx_device) 
    {
	  FTM_MSG_2(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: tgt_tx_device %d, curr_tx_device %d", 
			                 ftm_tdscdma_rftest_tgt_state.tx_status, ftm_tdscdma_rftest_curr_state.tx_status);
	//  if(ftm_tdscdma_rftest_tgt_state.tx_status==TRUE)
	//  {
	//   ftm_tdscdma_burst_rx( DefExpRx , FALSE ); 
	//  }
    //  ftm_tdscdma_burst_tx ( ftm_tdscdma_rftest_tgt_state.tx_status); 
	  ftm_tdscdma_rftest_curr_state.tx_device = ftm_tdscdma_rftest_tgt_state.tx_device;
	//  ftm_tdscdma_rftest_curr_state.tx_status = ftm_tdscdma_rftest_tgt_state.tx_status;
    }

  }
  else
  {
    FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_reconfig: target pri_device is invalid. device %d", device);
  }
  
  return status;

}

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the TDSCDMA radio test command
 
  @details
  This function is used to handle the TDSCDMA radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_tdscdma_process_rftest_drop
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
)
{
  ftm_rf_test_radio_config_unpack_data_t * rftest_drop_ptr = NULL;
  ftm_rf_test_rfm_device_mask_type device_mask = 0;
  rfm_device_enum_type device = RFM_MAX_WAN_DEVICES;
  ftm_rf_test_field_error_code_mask_type status = 0;	
	
  rftest_drop_ptr = command_unpack_data;
  /*Process the enabled add list device mask to extract each of the devices*/
  device_mask = rftest_drop_ptr->drop_list.device_mask;

  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_drop: Received Radio Drop Request ", 0);

  while (device_mask != 0)
  {
    device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

	if ( device == ftm_tdscdma_rftest_tgt_state.pri_device )
	{
	  ftm_tdscdma_rftest_tgt_state.pri_device = RFM_INVALID_DEVICE;
	  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_drop: device %d", device);
	}
	else if ( device == ftm_tdscdma_rftest_tgt_state.sec_device )
    {
	  ftm_tdscdma_rftest_tgt_state.sec_device = RFM_INVALID_DEVICE;
	  ftm_tdscdma_rftest_tgt_state.div_status = FALSE;
	  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_drop: device %d", device);
	}	
	else if ( device == ftm_tdscdma_rftest_tgt_state.tx_device )
    {
	  ftm_tdscdma_rftest_tgt_state.tx_device = RFM_INVALID_DEVICE;
	  ftm_tdscdma_rftest_tgt_state.tx_status = FALSE;
	  FTM_MSG_1(FTM_HIGH, "ftm_tdscdma_process_rftest_drop: device %d", device);
	}	
	else
	{
	 FTM_MSG_1(FTM_ERROR, "ftm_tdscdma_process_rftest_drop: Invalid device %d", device);
	}
    device_mask &= (~rf_extract_first_one_bit_mask(device_mask));
  }

  return status;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the TDSCDMA radio test command
 
  @details
  This function is used to handle the TDSCDMA radio test rx measure command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_tdscdma_rftest_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
)
{
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  boolean api_status = TRUE;
  uint8 meas_index;
  int16 rx_level_agc;
  int16 rx_level_dbm;
  uint16 lna_state;
  rfm_device_enum_type device;
  ftm_tdscdma_burst_rx_state_type rx_state;
  int16 ExpRx = query_data[0].input.expected_agc;

  /*turn on rx before measurement*/
  ftm_tdscdma_burst_rx( ExpRx , TRUE );  
  
  for( meas_index = 0; meas_index < num_of_measurements;
       meas_index ++ )
  {   
    device = query_data[meas_index].input.device;
    
	lna_state = ftm_tdscdma_get_lna_state(); 
	api_status &= ftm_tdscdma_get_rx_level_dbm(device, &rx_level_agc, &rx_level_dbm);

    if( api_status == TRUE )
    {  
      query_data[meas_index].output.rxagc = rx_level_dbm;
      query_data[meas_index].output.lna_gain_state = lna_state;
    }
    else
    {
      FTM_MSG_1(FTM_ERROR,"ftm_tdscdma_rftest_rx_measure: error when reading AGC on device %d", device );
      error_code = 1;
    }
  }
  /*turn off rx after measurement*/
  ftm_tdscdma_burst_rx( ExpRx , FALSE );  

  return error_code;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Call back to handle Tx Control command

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/

ftm_rf_test_field_error_code_mask_type ftm_tdscdma_rftest_tx_control
( 
  ftm_rf_test_tx_control_unpacked_data_t *query_data,
  uint32 num_of_actions
)
{
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  boolean api_status = TRUE;
  uint8 action_index;
  rfm_device_enum_type device;
  int16 tx_power;
  int16 tx_pwr_mdsp_units;
  boolean enable_tx;

  for( action_index = 0; action_index < num_of_actions; action_index ++ )
  { 
    device = query_data[action_index].input.device;
    enable_tx = (boolean)query_data[action_index].input.tx_action;
    tx_power = (int16)query_data[action_index].input.tx_power;

    FTM_MSG_3( FTM_LOW, "ftm_tdscdma_rftest_tx_control: Device %d, Tx ON %d, TxPwr(dBm10) %d",
                         device, enable_tx, tx_power );

    if ( device == ftm_tdscdma_rftest_curr_state.tx_device )
    {
      ftm_tdscdma_burst_tx(enable_tx);
      ftm_tdscdma_rftest_curr_state.tx_status = enable_tx;
	  if(enable_tx)
	  {
		/*hardcode for now to be continuous tx with*/
       api_status = ftm_tdscdma_set_burst_tx_params(0,1,tx_power,tx_power,tx_power,tx_power,tx_power);
	  }
    }

	else
	{
      RF_MSG_1( RF_ERROR, "ftm_tdscdma_rftest_tx_control(): invalid Tx device %d", device );
	}

	if( api_status == FALSE )
    {
      FTM_MSG_1(FTM_ERROR,"ftm_tdscdma_rftest_tx_control: error when setting Tx power on device %d", device );

      error_code = 1;

      break;
    }
  }/* for( action_index = 0; action_index < num_of_actions; action_index ++ ) */
  ftm_tdscdma_burst_tx(FALSE);
  ftm_tdscdma_rftest_curr_state.tx_status = FALSE;

  return error_code;
}/* ftm_tdscdma_rftest_tx_control */

#endif 
#endif /* FEATURE_FACTORY_TESTMODE */

