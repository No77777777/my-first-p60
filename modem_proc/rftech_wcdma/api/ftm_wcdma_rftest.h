/*!
   @file
   rfc_wcdma.h

   @brief
   This file contains all the WCDMA header functionality for the WCDMA RF test
*/

/*===========================================================================

Copyright (c) 2011-2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_wcdma.mpss/3.11/api/ftm_wcdma_rftest.h#1 $

when       who     what, where, why
------------------------------------------------------------------------------- 
08/13/15   vbh     Added support for the rx meas command
05/05/15   ak      Implementaion of the start, add, drop and reconfig APIs
05/04/15   ak      Initial Version 
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "ftm_rf_test_interface.h"
#include "ftm_rf_test_radio_config.h"
#include "rfcommon_math.h"
#include "ftm_wcdma_ctl.h"
#include "rfwcdma_data.h"
#include "ftm_rf_test_rx_measure.h"

#define MAX_RFTEST_IDX 10

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
);


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
);


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
);


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
);



/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the WCDMA radio test command
 
  @details
  This function is called to set the current mode of operation
 
  @param band
  Specifies the band/mode to be set
*/
void ftm_wcdma_rftest_set_mode
(
  rfcom_wcdma_band_type band
);

ftm_rf_test_field_error_code_mask_type
ftm_wcdma_rftest_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
);

#ifdef __cplusplus
}
#endif
