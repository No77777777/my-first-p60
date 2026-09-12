#ifndef FTM_1X_RF_TEST_H
#define FTM_1X_RF_TEST_H
/*!
   @file
   ftm_1x_rf_test.h

   @brief
   This file contains all the cdma header functionality for the 1x RF test
*/

/*==============================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_cdma.mpss/3.11/1x/ftm/inc/ftm_1x_rf_test.h#1 $

when       who     what, where, why
--------------------------------------------------------------------------------
08/11/15   zhw     Added support for Tx Control Command
08/03/15   zhw     Initial Revision
==============================================================================*/

/*==============================================================================
                           INCLUDE FILES
==============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "ftm_rf_test_interface.h"
#include "ftm_rf_test_radio_config.h"
#include "ftm_rf_test_rx_measure.h"
#include "ftm_rf_test_tx_control.h"

/*----------------------------------------------------------------------------*/
ftm_rf_test_field_error_code_mask_type
ftm_1x_rf_test_radio_config
(
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
);

/*----------------------------------------------------------------------------*/
ftm_rf_test_field_error_code_mask_type
ftm_1x_rf_test_rx_measure
(
  ftm_rf_test_rx_measure_unpacked_data_t *query_data,
  uint32 num_of_measurements
);

/*----------------------------------------------------------------------------*/
ftm_rf_test_field_error_code_mask_type 
ftm_1x_rf_test_tx_control
( 
  ftm_rf_test_tx_control_unpacked_data_t *query_data,
  uint32 num_of_actions
);

#ifdef __cplusplus
}
#endif

#endif /* FTM_1X_RF_TEST_H */
