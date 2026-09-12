#ifndef RFDEVICE_RFFE_COUPLER_H
#define RFDEVICE_RFFE_COUPLER_H


/*===========================================================================

Copyright (c) 2015 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_coupler.mpss/1.22/api/rfdevice_rffe_coupler.h#1 $
  $DateTime: 2021/03/09 03:37:09 $
  $Author: pwbldsvc $

when       who   what, where, why
--------   ---   --------------------------------------------------------------- 
07/14/15   px    Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#define RFDEVICE_COUPLER_MFG_ID_GET(x)   (((x) >> 22) & 0x3FF)
#define RFDEVICE_COUPLER_PRD_ID_GET(x)   (((x) >> 14) & 0xFF)
#define RFDEVICE_COUPLER_PORT_NUM_GET(x) ((x) & 0x7F)
#define RFDEVICE_COUPLER_DIR_GET_DEFAULT(x)      (((x) >> 7) & 0x7F)
#define RFDEVICE_COUPLER_RFC_PORT_NUM_INDEX  1
#define RFDEVICE_COUPLER_RFC_DEFAULT_GAIN_INDEX  2
#define RFDEVICE_COUPLER_RFC_DEFAULT_OUTPUT_TYPE_INDEX  3
#define RFDEVICE_COUPLER_RFC_DEFAULT_FWD_DIRECTION_INDEX  4

#ifdef __cplusplus
extern "C" {
#endif

#include "rfc_common.h"
#include "rfdevice_coupler.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_rffe_coupler_typedef.h"

#define MAX_PATH_SIZE 100

class rfdevice_rffe_coupler: public rfdevice_coupler
{
public:

  rfdevice_rffe_coupler(
                    rfdevice_physical_device* phy_obj_ptr,
                    rfc_phy_device_info_type *phy_device_info, 
                    rfc_logical_device_info_type *logical_device_info,
                    rfdevice_rffe_coupler_settings_type *coupler_settings,
                    rfdevice_rffe_coupler_func_tbl_type *coupler_fn_ptrs
                    );   virtual ~rfdevice_rffe_coupler();

  boolean config_coupler
  (
    rfdevice_coupler_tech_cfg_type tech_cfg,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );
  boolean config_coupler_gain
  (
    rfdevice_coupler_tech_cfg_type tech_cfg,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );
  void init(void);

  virtual boolean init 
  (
    int32* rfc_data
  );

  boolean sleep
  (
    int32* rfc_data,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );

  boolean wakeup
  (
    int32* rfc_data,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );

   boolean trigger
  (
    int32* rfc_data,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  );

  boolean set_band_config_map
  (
    rfcom_mode_enum_type mode,
    rfcom_band_type_u band,
    int32 *data_ptr
  );

  boolean init_status;

protected:

  /* physical device pointer to store the parent physical device object  during physical device creation */
  rfdevice_physical_device* rfdevice_phy_obj_ptr; 

  rfdevice_rffe_coupler *rfdevice_rffe_coupler_p;
  
  rfc_device_cfg_info_type coupler_device_info;
  
  rfdevice_rffe_coupler_settings_type coupler_settings;

  rfdevice_rffe_coupler_func_tbl_type coupler_fn_ptrs;

  rfdevice_rffe_coupler_rfc_cfg_band_map_type coupler_rfc_config_mapping;

  rfdevice_rffe_coupler_cfg_type coupler_cfg;

  /* List of API's used by the driver and not exposed */

  boolean config_coupler_default (void);

  boolean rfdevice_rffe_coupler_process_ag_table
  (
    rfdevice_rffe_coupler* coupler_device,
    rf_buffer_intf *rf_buff,
    const uint16* reg_table,
    const int16* data_table,
    uint8 write_size,
    rf_device_execution_type dev_action,
    int16 script_timing,
    uint8 port_index = 0
  );
  
  boolean rfdevice_rffe_coupler_validate
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    int16 script_timing   
  );

  boolean rfdevice_rffe_coupler_construct_driver_ag 
  ( 
    rfc_device_cfg_info_type* cfg,  
    rfdevice_rffe_coupler* coupler_device 
  );

  boolean rfdevice_rffe_coupler_update_cfg
  (
    rfdevice_rffe_coupler* coupler_device,
    rfcom_mode_enum_type mode,
    rfcom_band_type_u band,
    rfdevice_coupling_direction_type direction,
    rfdevice_coupling_gain_type gain,
    rfdevice_coupler_output_type output_type
  );

  boolean rfdevice_rffe_coupler_get_band_config_map
  (
    rfdevice_rffe_coupler* coupler_device,
    rfcom_mode_enum_type mode,
    rfcom_band_type_u band,
    rfdevice_rffe_coupler_rfc_cfg_type **rfc_cfg
  );

  private:

    boolean trigger_internal
    (
      int32* rfc_data,
      rf_buffer_intf *buff_obj_ptr, 
      rf_device_execution_type dev_action, 
      int16 script_timing
    );

    boolean config_coupler_internal
    (
      rfdevice_coupler_tech_cfg_type tech_cfg,
      rf_buffer_intf *buff_obj_ptr, 
      rf_device_execution_type dev_action, 
      int16 script_timing
    );

    boolean config_coupler_gain_internal
    (
      rfdevice_coupler_tech_cfg_type tech_cfg,
      rf_buffer_intf *buff_obj_ptr, 
      rf_device_execution_type dev_action, 
      int16 script_timing
    );

    boolean set_band_config_map_internal
    (
      rfcom_mode_enum_type mode,
      rfcom_band_type_u band,
      int32 *data_ptr
    );

    boolean wakeup_internal
    (
      int32* rfc_data,
      rf_buffer_intf *buff_obj_ptr, 
      rf_device_execution_type dev_action, 
      int16 script_timing
    );

    boolean sleep_internal
    (
      int32* rfc_data,
      rf_buffer_intf *buff_obj_ptr, 
      rf_device_execution_type dev_action, 
      int16 script_timing
    );

};

#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_RFFE_COUPLER_H */
