#ifndef QFE2520_HDET_CMN_H
#define QFE2520_HDET_CMN_H
/*!
   @file
   qfe2520_hdet_cmn.h

   @brief
   QFE2520 HDET common driver header file

*/

/*===========================================================================

Copyright (c)  2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header:  //source/qcom/qct/modem/rfdevice/qfe2520/main/latest/protected/qfe2520_hdet_cmn.h#3

when       who   what, where, why
--------   ---   ------------------------------------------------------------------- 
10/17/14   adk   Removed device calibration code
05/22/14   vv    Overloaded constructor for physical device support in bolt & DPM2.0
05/02/14   yb    Added max read size for rfhal_rffe_execute_rf_buf to avoid buffer overflow
04/01/14   ndb   Init Version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"
#include "rfdevice_ccs_rffe_comm_impl.h"
#include "rfdevice_comm_intf.h"
#include "rfdevice_physical_device.h"

#ifdef FEATURE_WCDMA
#include "qfe2520_hdet_wcdma.h"
#endif
#ifdef FEATURE_CDMA 
#include "qfe2520_hdet_cdma.h"
#endif
#ifdef FEATURE_TDSCDMA
#include "qfe2520_hdet_tdscdma.h"
#endif
#ifdef FEATURE_LTE
#include "qfe2520_hdet_lte.h"
#endif /*FEATURE_LTE*/
#include "rfdevice_hdet_cmn.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rfcommon_locks.h"
#include "rfc_common.h"

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------*/
/*!
  @brief
  QFE2520 HDET common class

  @details
*/
class qfe2520_hdet_cmn: public rfdevice_hdet_cmn
{
  public:

  /*Default Constructor*/
  qfe2520_hdet_cmn();
  
  /* Common HDET constructor  */   
  qfe2520_hdet_cmn(      
                         rfc_device_cfg_info_type* cfg,
                         rfdevice_comm_intf* comm_object,
                         rf_path_enum_type rf_path,
                         boolean extended_cmd = TRUE 
                  );    

  /* Overloaded constructor for physical device support */
  qfe2520_hdet_cmn(
                        rfdevice_physical_device* phy_obj_ptr,
                        rfc_phy_device_info_type *phy_device_info, 
                        rfc_logical_device_info_type *logical_device_info,
                        rfdevice_comm_intf* comm_object,
                        rf_path_enum_type rf_path,
                        boolean extended_cmd = TRUE 
                   );   
     
  /*To store the parent physical device object*/
  rfdevice_physical_device* rfdevice_phy_obj_ptr;
     
  // Destructor
  ~qfe2520_hdet_cmn();  

  void init();

  boolean hdet_init
  ( uint32 tx_freq, 
  	rfcom_mode_enum_type mode
  );

  boolean config_hdet_gain
  (
    rf_buffer_intf *script_ptr,
    rf_device_execution_type dev_action,
    uint32 tx_freq,
    boolean set_lp_settings
  );

  boolean hdet_trigger_setup
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    int delay,
    int number_of_samples,
    int16 script_timing
  );
  
  boolean do_read_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint16 *adc_val,
    uint8* status_flag
  );
  
  boolean disable_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action
  );

  boolean get_gated_incident_val
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint16 *hdet_inc_pwr,
    boolean trigger_flag,
    int delay,
    int number_of_samples
  );

  int get_adjusted_sample_val( int number_of_samples);

  void* get_hdet_tech_object( rfm_mode_enum_type tech );


  /*******************************************************************************
								 Unused API's
   *******************************************************************************/

  /* API's that are not used.The below API's can be removed once other Driver
   Compilation Dependencies have been removed*/

  boolean auto_cal
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action
  );

  /*The below API is not used */
  boolean update_k_comp( void );

  int16 get_k_comp( void );
 
/*******************************************************************************
	                             K Sensor APIs 
*******************************************************************************/

  boolean get_k_sensor_val_scripts( rf_buffer_intf* rf_buff,
                                    rf_device_execution_type dev_action,
                                    uint16 *k_val );

  boolean update_temp_comp_param( rf_buffer_intf* rf_buff,
                                  rf_device_execution_type dev_action );

  boolean update_temp_comp_param_cb( qfe_script_type* temp_comp_data_ptr );

  boolean program_k0_val(uint16 k0_value);

  /*******************************************************************************
	                                FTM APIs
  *******************************************************************************/

  /*FTM cmd :FTM_GET_QFE_HDET_INCIDENT_PWR = 855*/
  boolean hkadc_set_incident_path 
  (
	rf_buffer_intf *buff_obj_ptr, 
	rf_device_execution_type dev_action
  );

  /*FTM cmd :FTM_GET_QFE_HDET_REFLECTED_PWR = 856*/
  boolean hkadc_set_ref_path 
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action
  );

  
  /*FTM cmd :FTM_GET_QFE_HDET_INCIDENT_PWR = 856*/
  boolean get_incident_val
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint16 *hdet_inc_pwr,
    boolean trigger_flag
  );

  /*FTM cmd :FTM_GET_QFE_HDET_REFLECTED_PWR = 856*/
  boolean get_ref_val
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint16 *hdet_ref_pwr,
    boolean trigger_flag
  );

  /*FTM cmd :FTM_GET_QFE_HDET_POWER = 862*/
  boolean get_power
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint8 hdet_pid,
    uint16 *hdet_inc_pwr
  );

  /*FTM cmd :FTM_GET_QFE_HDET_POWER = 852*/

  boolean get_dc_offset_val
  (
    uint16 *hdet_inc_dc_offset_val,
    uint16 *hdet_ref_dc_offset_val
  );

  /*FTM cmd :FTM_GET_QFE_HDET_POWER = 852*/
  boolean do_hdet_dc_cal( void );

  /*FTM cmd :FTM_GET_QFE_K_SENSOR_VAL = 857*/
  boolean get_k_sensor_val( uint16 *k_val );
  
  /*FTM cmd :FTM_GET_QFE_K_SENSOR_VAL = 851*/
  boolean set_gain_state
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    rfdevice_hdet_gain_state_type hdet_gain_state,
    uint8 hdet_pid = 0
  );

  
  /*FTM cmd :FTM_GET_QFE_HDET_POWER = 852*/
  boolean setup_power_read
   (
     rf_buffer_intf *buff_obj_ptr, 
     rf_device_execution_type dev_action,
     uint8 num_avg,
     uint16 wait_time
   );

  protected:

  private:

 
  /*********CommObject Creation: Start************************************** */

  /*!Information regarding comm class associated with RFA communication channel
   associated with this device*/
  rfdevice_comm_intf* hdet_comm_obj;

  /*! Comm class buffer associated with this device */
  rfdevice_ccs_rffe_comm_buffer_type comm_buffer;

  /*! Script entry holder type for extended RFFE writes */
  rfdevice_ccs_rffe_comm_script_entry_holder_type rffe_script_entry_holder;

  /*! Flag to track the source of the CommObject
   0 -Created by logical device
   1- Created by physical device*/
  boolean Check_CommObject;

  /*********CommObject Creation: End************************************** */

  rf_buffer_intf *script_buffer;
  
  /*! Indicator for whether hdet is initialized and enabled */
  boolean is_hdet_enabled;

  /*! Device communication prototype */
  rfdevice_hdet_device_num_type device_num;

  /*! The RF path this device is associated to 
      This is needed in order to retrive the correct bc_config */
  rf_path_enum_type device_rf_path;

  /*! Tech specific hdet instance*/
  #ifdef FEATURE_WCDMA
  rfdevice_hdet_wcdma *qfe2520_hdet_wcdma_p;
  #endif
  #ifdef FEATURE_CDMA
  rfdevice_hdet_cdma *qfe2520_hdet_cdma_p;
  #endif
  #ifdef FEATURE_TDSCDMA
  rfdevice_hdet_tdscdma *qfe2520_hdet_tdscdma_p;
  #endif
  #ifdef FEATURE_LTE
  rfdevice_hdet_lte *qfe2520_hdet_lte_p;
  #endif /*FEATURE_LTE*/
 
  /*! Private Functions*/
  boolean process_script
  (
    rf_buffer_intf *rf_buff,
    rf_hal_bus_rffe_type* rffe_return_script,
    uint8 *read_size,
    rf_device_execution_type dev_action,
    int16 script_timing,
    uint8 max_read_size
  );

  boolean validate_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action
  );
};

#endif /*QFE2520_HDET_CMN_H*/

