#ifndef QFE2520_HDET_LTE_H
#define QFE2520_HDET_LTE_H
/*!
   @file
   qfe2520_hdet_lte.h

   @brief
   QFE2520 HDET 
   lte driver header file

*/

/*===========================================================================

Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: 

when       who   what, where, why
--------   ---   ------------------------------------------------------------------- 
06/16/14   sbm     HDET online brinup
04/01/14   ndb    Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "rfdevice_hdet_lte.h"
#include "rfdevice_hdet_cmn.h"
#include "qfe2520_hdet_cmn.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rfcommon_locks.h"

#ifdef __cplusplus
}
#endif

class qfe2520_hdet_lte: public rfdevice_hdet_lte
{
  public:

  /* Constructor overloaded to get the RFFE device details */   
  qfe2520_hdet_lte( rfdevice_hdet_device_num_type device_id,
                    rf_path_enum_type rf_path,
                    void *qfe2520_hdet_cmn_p
                   ); 
 
  // Destructor
  ~qfe2520_hdet_lte();   

  void init();
  
  boolean hdet_init
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint32 tx_freq,
    rfm_device_enum_type device,
    rfcom_lte_band_type band
  );

  boolean enable_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    rfdevice_lte_hdet_enable_type *hdet_data,
    rfm_device_enum_type device,
    rfcom_lte_band_type band
  );

  boolean disable_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    rfm_device_enum_type device,
    rfcom_lte_band_type band
  );
  boolean do_full_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    rfdevice_lte_hdet_settings_type *hdet_data,
    rfm_device_enum_type device,
    rfcom_lte_band_type band
  );

  boolean do_hdet_read
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    rfdevice_lte_hdet_settings_type *hdet_data
  );

  boolean get_hdet_setting
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    rfdevice_lte_hdet_therm_data_type* lte_hdet_therm_settings,
    rfdevice_lte_script_data_type* script_data_ptr,
    rfm_device_enum_type device,
    rfcom_lte_band_type band
  );
  boolean get_hdet_val
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    rfdevice_lte_hdet_therm_data_type* lte_hdet_therm_settings,
    rfm_device_enum_type device,
    rfcom_lte_band_type band
  );

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

#endif /* QFE2520_HDET_LTE_H */

