#ifndef QFE2520_HDET_TDSCDMA_H
#define QFE2520_HDET_TDSCDMA_H
/*!
   @file
   qfe2520_hdet_tdscdma.h

   @brief
   QFE2520 HDET tdscdma driver header file

*/

/*===========================================================================

Copyright (c) 2014  by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: 

when       who   what, where, why
--------   ---   -------------------------------------------------------------------
04/01/14   ndb   Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rfdevice_hdet_tdscdma.h"
#include "rfdevice_hdet_cmn.h"
#include "qfe2520_hdet_cmn.h"
#include "rfdevice_tdscdma_type_defs.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "rfcommon_locks.h"

#ifdef __cplusplus
}
#endif

class qfe2520_hdet_tdscdma: public rfdevice_hdet_tdscdma
{
  public:

  /* Constructor overloaded to get the RFFE device details */   
  qfe2520_hdet_tdscdma( rfdevice_hdet_device_num_type device_id,
                        rf_path_enum_type rf_path,
                        void *qfe2520_hdet_cmn_p
                     ); 
 
  // Destructor
  ~qfe2520_hdet_tdscdma();   

  void init();
   
  boolean hdet_init
  (
    rf_buffer_intf *buff_obj_ptr, 
	rf_device_execution_type dev_action,
    uint32 tx_freq
  );
 
  boolean get_mdsp_config_data
  (
    rfdevice_hdet_mdsp_config_data_type *data 
  );
 
  boolean read_mdsp_triggered_incident
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint16 *hdet_val
  );
  
  boolean tx_hdet_read
  (
  rfm_device_enum_type rfm_dev,
  rfcom_tdscdma_band_type rf_band,
  boolean mdsp_triggered_read,
  uint16 *hdet_value,
  rf_buffer_intf* buff_obj_ptr,
  rf_device_execution_type dev_action
  );

  boolean tx_hdet_setup
  (   
  rfm_device_enum_type rfm_dev,
  rfcom_tdscdma_band_type rf_band,
  rf_device_execution_type dev_action,
  rf_buffer_intf* buff_obj_ptr,
  void *data
  );

  boolean tx_hdet_script_read
  (   
  rfm_device_enum_type rfm_dev,
  rfcom_tdscdma_band_type rf_band,
  rf_device_execution_type dev_action,
  rf_buffer_intf* buff_obj_ptr,
  void* data
  );

  boolean calc_hdet_measurement 
 ( 
  rfm_device_enum_type rfm_dev,
  rfcom_tdscdma_band_type rf_band,
  rfdevice_tdscdma_calc_hdet_measurement_type*  calc_hdet_meas);

  protected:
 
  private:
  /*! Device communication prototype */
  rfdevice_hdet_device_num_type device_num;

  /*! The RF path this device is associated to 
      This is needed in order to retrive the correct bc_config */
  rf_path_enum_type device_rf_path;
  

  /*! TX Frequency */
  uint32 tx_freq;

  /*! common hdet instance*/
  rfdevice_hdet_cmn *qfe2520_hdet_cmn_p;
};

#endif /*QFE2520_HDET_TDSCDMA_H*/
