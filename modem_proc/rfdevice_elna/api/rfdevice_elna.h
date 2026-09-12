#ifndef RFDEVICE_ELNA_H
#define RFDEVICE_ELNA_H


/*===========================================================================

  Copyright (c) 2014-2015 Qualcomm Technologies Inc. All Rights Reserved

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
  written permission of Qualcomm Technologies Inc.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_elna.mpss/1.22/api/rfdevice_elna.h#1 $
  $DateTime: 2021/03/09 03:41:28 $
  $Author: pwbldsvc $

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
09/02/15   px    Added support for alternate gain map
12/03/14   vv    Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "rfc_common.h"
#include "rfdevice_type_defs.h"
#include "rfdevice_lna.h"
#include "rfdevice_lna_cfg.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_physical_third_party.h"
#include "rfdevice_elna_typedef.h"

#define MAX_PATH_SIZE 100

/*API to create 3rd party lna with physical device interface support.
Once created successfully it registers itself as a child of the physical device object passed to it*/
rfdevice_logical_component* create_gen_elna_object(
                                                  rfdevice_physical_device          *rfdevice_physical_third_party_p,
                                                  rfc_logical_device_info_type      *logical_device_cfg
                                                  ); 


class rfdevice_elna: public rfdevice_lna
{
  public:

 
    virtual ~rfdevice_elna();


    rfdevice_elna(
                  rfdevice_physical_device* phy_obj_ptr,
                  rfc_phy_device_info_type *phy_device_info, 
                  rfc_logical_device_info_type *logical_device_info
                  ); 

    /*Default constructor*/
    rfdevice_elna(void);

    boolean init_status;

  /*!
    @brief
    interface function for lna sleep

    @details
    rfdevice_lna_sleep()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info  
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
   boolean sleep
   (
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

  /*!
    @brief
    interface function for lna wakeup

    @details
    rfdevice_lna_wakeup()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info  
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
   boolean wakeup
   (
    rfdevice_lna_cfg* lna_cfg_p, 
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action, 
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

  /*!
    @brief
    returns the lna gain settings 

    @details
    returns the lna gain settings 

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
   boolean set_lna_gain
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

  /*!
    @brief
    interface function for lna trigger, to latch the pending settings.

    @details
    rfdevice_lna_trigger()
   
    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info
   
    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    time offset for the script where a negative value refers to the time BEFORE the event

    @return
    boolean indicating pass or fail
  */
   boolean trigger
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

   /*!
    @brief
    populates the band map information from rfc

    @details
    populates the band map information from rfc

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param *data_ptr
    band mapping information passed as int32 pointer
    
    @param size
    number of valid words in the *data_ptr
    
    @return
    boolean indicating pass or fail
  */
   boolean set_band_map
   (
    rfdevice_lna_cfg* lna_cfg_p,
    int32 *data_ptr, 
    uint8 size
   );

  /*!
    @brief
    returns the lna gain for the requested system gain

    @details
    the techs would call this api to get the lna gain to system gain mapping for a particular tech and band.

    @param rfdevice_lna_cfg
    pointer to a lna cfg object which contains lna specific config info

    @param lna_gain
    returns an enum indicating the lna gain

    @param use_alt_gain
    set to TRUE to use alternate gain mapping (such as for asdiv)
   
    @return
    boolean indicating pass or fail
  */
   boolean get_lna_gain_index
   (
    rfdevice_lna_cfg* lna_cfg_p,
    rfdevice_lna_gain_enum_type *lna_gain,
    boolean use_alt_gain
   );

  protected:

   rfdevice_elna *rfdevice_elna_p;

   /*To store the parent physical device object*/
   rfdevice_physical_device* rfdevice_phy_obj_ptr;

   rfc_device_cfg_info_type elna_device_info;

   rfdevice_elna_settings_type elna_settings;

   rfdevice_elna_port_info_type port_info;

   rfdevice_elna_gain_info_type gain_info;

   rfdevice_elna_gain_info_type gain_info_alt;

   boolean rfdevice_elna_param_validity
   (
     rfdevice_lna_cfg* lna_cfg_p,
     rf_buffer_intf *buff_obj_ptr,
     rf_device_execution_type dev_action,
     int16 script_timing
   );
    
   boolean rfdevice_elna_process_reg_settings
   (
     rf_buffer_intf *rf_buff,
     const uint16* reg_table,
     const int16* data_table,
     uint8 write_size,
     rf_device_execution_type dev_action,
     int16 script_timing,
     rfdevice_lna_trigger_enum_type  trigger_type = RFDEVICE_LNA_TRIGGER_NONE
   );

  boolean rfdevice_elna_get_tech_port_info
  (
    rfm_device_enum_type rfm_device,
    rfcom_mode_enum_type mode, 
    rfcom_band_type_u band,
    uint8** port_data_ptr 
  );

  boolean rfdevice_elna_get_elna_gain_info
  (
    rfm_device_enum_type rfm_device,
    rfcom_mode_enum_type mode, 
    rfcom_band_type_u band,
    uint8** gain_data_ptr,
    boolean use_alt_gain
  );

  private:
 
  };

#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_ELNA_H */
