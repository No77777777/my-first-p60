#ifndef QFE2520_HDET_CDMA_H
#define QFE2520_HDET_CDMA_H
/*!
   @file
   qfe2520_hdet_cdma.h

   @brief
   QFE2520 HDET 
   cdma driver header file

*/

/*===========================================================================

Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: 

when       who   what, where, why
--------   ---   ------------------------------------------------------------------- 
04/01/14  ndb    Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "comdef.h"
#ifdef FEATURE_CDMA
#include "rfdevice_hdet_cdma.h"
#include "rfdevice_hdet_cmn.h"
#include "qfe2520_hdet_cmn.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "rfcommon_locks.h"

#ifdef __cplusplus
}
#endif

class qfe2520_hdet_cdma: public rfdevice_hdet_cdma
{
  public:

  /* Constructor overloaded to get the RFFE device details */   
  qfe2520_hdet_cdma( rfdevice_hdet_device_num_type device_id,
                          rf_path_enum_type rf_path,
                          void *qfe2520_hdet_cmn_p
                         ); 
 
  // Destructor
  ~qfe2520_hdet_cdma();   

  void init();
  
  boolean hdet_init
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    uint32 tx_freq
  );
    
  boolean setup_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
  	rf_device_execution_type dev_action,
    rfdevice_cdma_hdet_power_mode_type power_mode
  );

  boolean enable_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action
  );

  boolean disable_hdet
  (
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action
  );

  boolean read_hdet
  (
    boolean mdsp_triggered_read,
    rfdevice_cdma_hdet_read_type *hdet_output
  );

  uint16 multi_read_hdet
  (
    uint16 num_of_reads
  );

  boolean get_hdet_param
  (
    rfdevice_cdma_hdet_param_type* hdet_param
  );

  boolean get_hdet_scripts
  (
    rfdevice_cdma_hdet_scripts_type* hdet_scripts
  );

  boolean get_fw_trigger_script
  (
    rf_buffer_intf* buff_obj_ptr
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

#endif /* QFE2520_HDET_CDMA_H */
#endif /* FEATURE_CDMA */
