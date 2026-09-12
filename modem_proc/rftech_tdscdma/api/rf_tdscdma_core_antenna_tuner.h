#ifndef RF_TDSCDMA_CORE_ANTENNA_TUNER_H
#define RF_TDSCDMA_CORE_ANTENNA_TUNER_H

#ifdef FEATURE_RF_HAS_QTUNER

/*!
  @file
  rf_tdscdma_core_antenna_tuner.h

  @brief
  Provides TDSCDMA antenna tuner functionality

  @details

*/

/*===========================================================================

Copyright (c) 2012 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$DateTime: 2021/03/09 05:09:33 $ $Author: pwbldsvc $
$Header: //components/rel/rftech_tdscdma.mpss/3.11/api/rf_tdscdma_core_antenna_tuner.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/30/15   ndb     Added Tuner IRAT support in RxTx split feature
04/23/15   jak     tuner API interface change and add new function for update tune code
08/20/14   ndb     Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
06/22/14   ych	   Add tuner_disable for both sleep/exit and Tx mode in IRAT
06/03/14   jz      Add tuner tune code override command
04/24/14   ych     Added dynamic tuner script for ifreq
02/12/14   ms      Disable tuner in X2T iRAT case (porting per CR600367)
04/23/13   jyu     Changed the interface for program_rx/tx_mode 
04/10/13   ndb     Removed the unused code 
03/21/13   ms      Tuner TDET support 
11/19/12   kai     Initial version (ported from WCDMA)

============================================================================*/
#include "rfcom.h"
#include "rfdevice_antenna_tuner_intf.h"
#include "rf_tdscdma_core_util.h"


#if 0
/*---------------------------------------------------------------------------*/
boolean rf_tdscdma_core_create_tuner_managers(void);
#endif

/*---------------------------------------------------------------------------*/
boolean rf_tdscdma_core_antenna_tuner_init
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  rfdevice_rx_mode_type rfdevice_op_mode
);

/*---------------------------------------------------------------------------*/
boolean rf_tdscdma_core_antenna_tuner_disable
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode  
);

/*---------------------------------------------------------------------------*/
boolean rf_tdscdma_core_antenna_tuner_program_rx_mode 
( 
  rfm_device_enum_type rfm_device,
  rfcom_tdscdma_band_type band,
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode  
);

/*---------------------------------------------------------------------------*/
boolean rf_tdscdma_core_antenna_tuner_program_tx_mode 
( 
  rfcom_device_enum_type device,
  rfcom_tdscdma_band_type band,
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing 
);

/*----------------------------------------------------------------------------*/
boolean rf_tdscdma_core_antenna_tuner_tx_mode_disable 
( 
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band, 
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing
);

/*---------------------------------------------------------------------------*/
uint32 rf_tdscdma_core_antenna_tuner_tune_code_override( rfm_device_enum_type device,
                                                         rfcom_tdscdma_band_type band,
                                                         uint8 override_flag,
                                                         void *data,
                                                         uint8* tuner_nv_ptr,
                                                         uint8 *tuner_id_ptr);

/*---------------------------------------------------------------------------*/
boolean rf_tdscdma_core_antenna_tuner_update_tune_code
(
  rfm_device_enum_type device,
  rfcom_tdscdma_band_type band, 
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing, 
  uint32* script_token_ptr,
  rfdevice_rx_mode_type rfdevice_op_mode  
);

/*---------------------------------------------------------------------------*/
/*Tuner IRAT API's */
boolean rf_tdscdma_core_antenna_tuner_tune_away
( 
  rfm_device_enum_type rfm_device,
  rfcom_tdscdma_band_type band,
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode
);
/*---------------------------------------------------------------------------*/
boolean rf_tdscdma_core_antenna_tuner_tune_back
( 
  rfm_device_enum_type rfm_device,
  rfcom_tdscdma_band_type band,
  uint16 chan_num,
  rf_device_execution_type rf_tdscdma_exec_type,
  rfc_rxtx_enum_type rx_tx_config,
  rf_buffer_intf* buffer_ptr,
  int16 script_timing,
  rfdevice_rx_mode_type rfdevice_op_mode
);
/*---------------------------------------------------------------------------*/
#endif /* FEATURE_RF_HAS_QTUNER */

#endif /* RF_TDSCDMA_CORE_ANTENNA_TUNER_H */
