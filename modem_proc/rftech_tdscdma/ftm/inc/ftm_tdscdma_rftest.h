/*!
   @file
   rfc_tdscdma.h

   @brief
   This file contains all the TDSCDMA header functionality for the TDSCDMA RF test
*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_tdscdma.mpss/3.11/ftm/inc/ftm_tdscdma_rftest.h#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
01/21/20   hal     Support QSH minidump for RFTECH TDSCDMA
08/14/15   lwu      Implementaion of the start, add, drop and reconfig APIs
08/14/15   lwu      Initial Version 
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "ftm_rf_test_interface.h"
#include "ftm_rf_test_radio_config.h"
#include "ftm_rf_test_rx_measure.h"
#include "ftm_rf_test_tx_control.h"
#include "rfcommon_math.h"
#include "ftm_tdscdma_ctl.h"

#define MAX_RFTEST_IDX 10

typedef struct {
  rfm_subscriber_type	sub;
  ftm_mode_id_type	band;
  uint16	channel;
  rfm_device_enum_type pri_device;
  rfm_device_enum_type sec_device;
  rfm_device_enum_type tx_device;  
  boolean	div_status;
  boolean	tx_status;
} ftm_tdscdma_rftest_state_t;

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
);


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
);


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
);


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
);



/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the TDSCDMA radio test command
 
  @details
  This function is called to set the current mode of operation
 
  @param band
  Specifies the band/mode to be set
*/
ftm_rf_test_field_error_code_mask_type ftm_tdscdma_rftest_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
);

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
);

#ifdef __cplusplus
}
#endif
