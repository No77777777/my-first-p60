#ifndef RFDEVICE_QTUNER_H
#define RFDEVICE_QTUNER_H


/*===========================================================================

Copyright (c) 2013,2014 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_qtuner.mpss/1.32/api/rfdevice_qtuner.h#1 $
  $DateTime: 2021/03/09 04:14:45 $
  $Author: pwbldsvc $

when         who   what, where, why
--------   ---   --------------------------------------------------------------- 
02/17/16   ndb   Remove the chip level dependency in the common qtuner driver
08/10/15   yb    API to get rfc config
06/09/15   yb    Added support for device level log packet
06/09/15   yb    Added API to get tune code and get_nvg status
06/09/15   yb    Legacy RL Clean-up, Device API clean-up
03/11/15   yb    Fix to not trigger disable if device is already disabled
08/20/14   ndb   Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
08/26/14   yb    Add coupler device and band info to tuner device
08/21/14   vv    Clean up unused device constructor
07/01/14   yb    Add support to make tuner transactions non-extended via rfc
05/29/14   yb    1. Fixed tune code override in online mode
                 2. Program ctuner compensation settings in program tune code
05/27/14   vv    Physical device support
05/20/14   yb    Added program_nvg_detune_tuner() API
05/02/14   yb    Added max read size for rfhal_rffe_execute_rf_buf to avoid buffer overflow
04/09/14   ndb   Added extended_cmd in rfdevice_qtuner
03/25/14   yb    Added support for linearizer
03/11/14   yb    Fixed mode and freq settings
12/26/13   ndb   Added program_default_tune_code (default implementation)
11/30/13   ndb   Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rfcom.h"
#include "rfc_common.h"
#include "rfdevice_antenna_tuner.h"
#include "rfdevice_qtuner_typedef.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_ccs_rffe_comm_impl.h"
#include "rfdevice_comm_intf.h"
#include "rfdevice_qtuner_log.h"

#define RFDEVICE_QTUNER_RFC_DISTORTION_CONFIG_INDEX 1


class rfdevice_qtuner: public rfdevice_antenna_tuner
{
public:
 
rfdevice_qtuner(void);
 
/* Overloaded constructor for physical device support */
rfdevice_qtuner(rfdevice_physical_device* phy_obj_ptr,
                rfc_phy_device_info_type *phy_device_info, 
                rfc_logical_device_info_type *logical_device_info,
                rfdevice_qtuner_settings_type *qtuner_settings,
                rfdevice_qtuner_func_tbl_type *qtuner_fn_ptrs,
                rfdevice_qtuner_params_type *qtuner_params,
                boolean extended_cmd);  

virtual ~rfdevice_qtuner();


/*Start of Virtual functions*/
boolean init
(
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
);

boolean rx_init 
( 
  rfcom_mode_enum_type mode,
  rf_buffer_intf *buff_obj_ptr,
  rf_device_execution_type dev_action, 
  int16 script_timing = RFCOM_INVALID_TIMING_OFFSET 
);

boolean tx_mode_enable
(
  rfcom_mode_enum_type mode, 
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
);

boolean disable
(
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action, 
  int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
);

uint16 get_script_size
(
  void
);

int16 read_chip_rev
( 
	 void
);

rfc_device_cfg_info_type get_rfc_device_cfg(void);

rfdevice_antenna_tuner_device_override_status_type tune_code_override
(
  uint8 override_flag,
  void *data,
  uint8* tuner_nv_ptr,
  uint8* tuner_id_ptr
);

boolean program_tune_code
(
  uint8 *tune_code_ptr,
  uint8 tune_code_size,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing
);

boolean program_default_tune_code
(
  uint8 *tune_code_ptr,
  uint8 *tune_code_size,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing
);

rfdevice_antenna_tuner_device_num_type get_antenna_tuner_instance_val
(
  void
);

void log_qtuner_device_info_data (
  rfdevice_qtuner_log_transaction_type transaction_type,
  rf_rffe_data_type* rffe_script,
  uint8 num_transactions
);

/*End of Virtual functions -- Need to check whether we need to specify all the virtual
function*/

boolean is_algo_supported
(
  rfdevice_atuner_tuner_device_algo_type algotype
);


/*API for programming the default tune code */
boolean program_tune_code
(
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing
);

boolean perform_r_tuner_cal
(
  uint8* r_tuner_val,
  uint8 num
);

boolean perform_c_tuner_cal
(
  uint8* c_tuner_idx,
  uint8 num
);

boolean load_r_tuner_cal
(
  uint8* r_tuner_val,
  uint8 num
);

boolean load_c_tuner_cal
(
  uint8* c_tuner_idx,
  uint8 num
);

boolean set_band_distortion_config_map
(
   rfcom_mode_enum_type mode,
   rfcom_band_type_u band,
   int32 *data_ptr
);

/* For turning ON/OFF NVG and to detune tuner */
boolean program_nvg_detune_tuner 
(
  boolean nvg_on_off,
  boolean is_program_nvg,
  uint8 tune_code_size,
  uint8 *tune_code_ptr,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing 
);

uint8 get_ctuner_comp_settings
(
  uint8 c_in, 
  uint8 c_index
);

void update_dependent_logical_device_obj
(
  rfdevice_class* device_obj,
  rfdevice_type_enum_type device_type
);

boolean get_tune_code(uint8 *tune_code_size, uint8* tune_code, uint8* device_id);

boolean get_nvg_status(void);

protected:
  
rfdevice_qtuner *rfdevice_qtuner_p;
  

/*To store the parent physical device object*/
rfdevice_physical_device* rfdevice_phy_obj_ptr;


rfc_device_cfg_info_type qtuner_device_info;
  
rfdevice_qtuner_settings_type qtuner_settings;

rfdevice_qtuner_params_type   qtuner_params;

rfdevice_qtuner_func_tbl_type qtuner_fn_ptrs;

rf_buffer_intf *seq_script_buffer;

rfdevice_qtuner_distortion_config_map_type distortion_config_mapping;

/*! TX Frequency */
uint32 tx_freq;

/*! TX Band */
rfcom_band_type_u tx_band;

/*! Current tech */
rfcom_mode_enum_type mode;

/*! Flag to track tune code override command */
boolean tune_code_override_flag;

/*! overrided tune code */
uint8 tune_code_override_data[RFDEVICE_QTUNER_MAX_TUNE_CODE_SIZE];

/*!Information regarding comm class associated with RFA communication channel
associated with this device*/

rfdevice_comm_intf* comm_object;

/*! Comm class buffer associated with this device */
rfdevice_ccs_rffe_comm_buffer_type comm_buffer;

/*! Script entry holder type for extended RFFE writes */
rfdevice_ccs_rffe_comm_script_entry_holder_type rffe_script_entry_holder;

/*! flag to keep track of whether device is initialized or not */
boolean is_device_enabled;

/*! flag to indicate rl disable is already done or not */
boolean is_rl_disabled;

/* List of API's used by the driver and not exposed */

/*Preconfig- API used to preconfig the tuner before starting for normal operation */
boolean preconfig
(
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing
);

boolean rfdevice_qtuner_process_ag_table
(
  rf_buffer_intf *rf_buff,
  rf_rffe_data_type *script_table,
  uint8 script_size,
  rf_device_execution_type dev_action,
  int16 script_timing
);
  
boolean rfdevice_qtuner_validate
(
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing   
);

boolean rfdevice_qtuner_process_script
(
  rf_buffer_intf *rf_buff,
  rf_hal_bus_rffe_type* rffe_return_script,
  uint8 *read_size,
  rf_device_execution_type dev_action,
  int16 script_timing,
  uint8 max_read_size
);


boolean rfdevice_qtuner_map_ctuner_bin_val_to_tuner_val_idx
(
  int c_bin_val, 
  uint8* c_tuner_val_idx,
  uint8 index,
  uint8 c_min_val,
  uint8 c_max_val,
  uint8 num_bins,
  uint8 num_values
);

boolean rfdevice_qtuner_do_monotonicity_check_on_cap_comp_tbls
(
  uint8 index,
  uint8 num_values
);

uint8 rfdevice_qtuner_rcm_cmn_get_c_cap_val
(
  uint8 index,
  uint8 i
);

int8 rfdevice_qtuner_rcm_cmn_get_c_bin_val
(
  uint8 index, 
  uint8 j
);

uint8 rfdevice_qtuner_rcm_cmn_get_c_comp_val
(
  uint8 index,
  uint8 i,
  uint8 j
);

boolean rfdevice_qtuner_get_tech_distortion_config_info
(
  rfdevice_qtuner* qtuner_device,
  rfcom_mode_enum_type mode, 
  rfcom_band_type_u band,    
  rfdevice_qtuner_distortion_config_type** distortion_config_data_ptr 
);

boolean get_ctuner_comp_tbl_row_idx
(
  uint8 *row_idx,
  uint8 c_index,
  uint8 cap_val_in,
  uint8 cap_min_val,
  uint8 cap_max_val,
  uint8 c_tbl_row_dim
);

};


#endif /* RFDEVICE_QTUNER_H */
