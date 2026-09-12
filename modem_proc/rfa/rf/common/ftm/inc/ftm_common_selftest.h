#ifndef FTM_COMMON_SELFTEST_H
#define FTM_COMMON_SELFTEST_H

/*! 
  @file
  ftm_common_selftest.h
 
  @brief
  This file defines function prototypes, structures and global variables for 
  self-test for any technology
  
  @brief
  This file defines function prototypes, structures and global variables for 
  self-test for any technology
  @{
*/

/*==============================================================================

  Copyright (c) 2013 - 2014 QUALCOMM Technologies Inc. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_common_selftest.h#1 $


when       who     what, where, why
--------   ---     -------------------------------------------------------------
09/09/15   dbz     KW error fix
09/03/15   tjm/dbz Selftest Re-architecture
08/07/15   ck      Add support for GSM selftest power meas
08/03/15   zbz     Changes to support RFFE coupelr
07/15/15   zbz     pass device id into ftm iq processing
02/11/15   jmf     [selftest] Define (LTE) Spectrum Flatness Measurement Type
11/04/14   dbz     [selftest] Add support for VSWR V2 processing
10/16/14   jmf     [selftest] Implement multi-gain stage switch for ACLR improvement
10/14/14   jmf     [selftest] Implement FBRx Noise Floor Cal
09/19/14   jmf     Define VSWR v2 Measurement
08/18/14   jmf     [SelfTest] ACLR meas changes
07/31/14   jmf     Correct Enum type for EVM capture
                   Add API to obtain current self test flag
07/24/14   jmf     [SelfTest] Move self test mode global flag to .c
07/24/14   dbz     Fix VSWR measurement mask
07/17/14   jmf     Add api to set self test flag / Define flag for self test mode
06/30/14   daa     Created.

==============================================================================*/

#include "ftm.h"
#include "DALStdDef.h"

#define ENABLE_TX_PWR_MEAS    0x01
#define ENABLE_ACLR_MEAS      0x04
#define ENABLE_IQ_CAPTURE_RX  0x10
#define ENABLE_EVM_MEAS       0x02
#define ENABLE_VSWR_MEAS      0x20
#define ENABLE_VSWR_MEAS_V2   0x40
#define ENABLE_SPEC_FLAT_MEAS 0x80

typedef struct
{

  rfcom_band_type_u com_band;
  uint8 selftest_fbrx_gs;
  uint8 wtr_lte_bw;
  boolean override_enable;

} ftm_common_selftest_fbrxgs_info_type;

typedef struct {
  int16 vswr_v2_return_loss;
  /*!< value of VSWR Return Loss in  dB100 */

  int16 vswr_v2_mag;
  /*!< value of VSWR Magnitude in dB100*/

  int16 vswr_v2_phase;
  /*!< value of VSWR phase in degree100*/

  int16 vswr_v2_fwd_pwr;
  /*!< Value of forward power in dB100 */

  int16 vswr_v2_rev_pwr;
  /*!< Value of reverse power in dB100 */

} ftm_common_selftest_vswr_v2_results_type;

/*
Self test parameters structure, common to all techs
*/

typedef enum
{
  FTM_COUPLER_DIRECTION_FWD,
  FTM_COUPLER_DIRECTION_REV,
  FTM_COUPLER_DIRECTION_INVALID
} ftm_common_selftest_coupler_dir_type;

typedef struct {

  rfcom_device_enum_type device; // Tx device for WCDMA, TDSCDMA, LTE, PRX device for CDMA
  rfcom_device_enum_type prx_device;
  rfcom_device_enum_type tx_device;
  ftm_rf_technology_type tech; 
  uint16 num_averages;
  uint32 capture_offset;
  uint32 tx_measurement_config;

} ftm_common_selftest_meas_params_struct;

/*
Struct containing flags that determine which capture to perform
*/
typedef struct {

  boolean tx_power_capture;
  boolean evm_capture;
  boolean rx_rms_capture;
  boolean vswr_v2_capture;
  boolean spec_flat_capture;

} ftm_common_selftest_capture_flags;

/*
Struct containing flags that indicate whether a particular measurement has failed. 
Used when preparing the response packet, garbage values are sent for failed measurements.
*/
typedef struct {

  boolean tx_power_success;
  boolean evm_success;
  boolean aclr_success;
  boolean vswr_success; 
  boolean vswr_v2_success;
  boolean spec_flat_success;
} ftm_common_selftest_success_flags;

void ftm_common_selftest_get_coupler_script
(
  rfm_mode_enum_type tech,
  rfcom_band_type_u band,
  rfm_device_enum_type tx_device
);



/*
Struct containing sample information, common to all techs.
*/
typedef struct {

  rfcom_device_enum_type device;
  int32 * iq_buf_tx;
  int32 * iq_buf_rx;
  int32 * iq_buf_tx_rsp_data;
  int32 * iq_buf_rx_rsp_data;
  uint16 tx_iq_data_size;
  uint16 rx_iq_data_size;
  uint32 sampling_frequency;
  boolean data_available;
  // ftm_rf_fbrx_noise_binsum_type noise_meas_results[4]

} ftm_rf_common_fbrx_iq_acquired_samples;

/*
Struct containing measurement results.
*/
typedef struct {

  int16 tx_power;
  uint64 evm_value;
  int64 aclr_value[6];
  int16 vswr_ratio_db10;
  uint64 rx_rms_mag;
  uint64 rx_rms_reverse_mag;
  int64 vswr_v2_value[5];
  int64 spec_flat_value[8]; // LTE Only

} ftm_rf_common_tx_params;

// Entry point for selftest procedure
uint8 ftm_common_selftest_main(ftm_common_selftest_meas_params_struct params, 
#ifdef FEATURE_LTE

ftm_lte_pkt_type * req_pkt,
#else
int32 *dummy_var,
#endif
ftm_rsp_pkt_type * rsp_pkt);


// Legacy IQ capture function
boolean ftm_common_fbrx_iq_capture_processing(ftm_rf_technology_type ftm_rf_tech,
                                              uint8 fbrx_chain,
                                              uint32 ftm_selftest_capture_type,
                                              uint16 num_samples,
                                              int32 **iq_buf_rx,
                                              int32 **iq_buf_tx,
                                              uint16 *evm_value_ptr,
                                              uint32 *rx_rms_mag_ptr,
                                              uint32 *samp_rate,
                                              ftm_common_selftest_fbrxgs_info_type * fbrx_gs_info,
                                              rfm_device_enum_type device);
void ftm_common_selftest_set_flag( uint8 * flag_value);
uint8 ftm_common_get_selftest_capture_flag(void);
void ftm_common_selftest_delete_coupler_script(void);

// Common wrappers for tech-specific NV functions
boolean ftm_common_selftest_fbrx_noise_floor_nvwrite(const ftm_rf_technology_type tech, uint8 nv_band, rfcommon_nv_fbrx_noise_data_type * fbrx_noise_nv_struct);
boolean ftm_common_selftest_obtain_fbrx_noise_nv_item(const ftm_rf_technology_type tech, const uint8 nv_band, rfnv_item_id_enum_type * nv_item_to_write);
boolean ftm_common_selftest_obtain_fbrx_gain_stage(const ftm_rf_technology_type tech, uint8 * gain_stage);
boolean ftm_common_selftest_obtain_fbrx_gain_switchpoint_nv_item(const ftm_rf_technology_type tech, const rfcom_band_type_u band, rfnv_item_id_enum_type * nv_item);
void ftm_common_set_coupler_direction(uint8 coupler_dir);

#endif
