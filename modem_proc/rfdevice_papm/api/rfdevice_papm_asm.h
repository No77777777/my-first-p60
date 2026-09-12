#ifndef RFDEVICE_PAPM_ASM_H
#define RFDEVICE_PAPM_ASM_H
/*!
   @file
   rfdevice_papm_asm.h

   @brief

*/

/*===========================================================================

Copyright (c) 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/rfdevice_papm.mpss/1.32/api/rfdevice_papm_asm.h#1 $

when       who    what, where, why
--------   ---   -------------------------------------------------------------------
05/08/15   sn     Initial version
============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#include "rf_buffer_intf.h"
#include "rf_rffe_common.h"
#include "rfdevice_class.h"
#include "rfdevice_cmn_type_defs.h"
#include "rfdevice_asm.h"
#include "rfdevice_asm_cfg.h"
#include "rfdevice_papm_types.h"
#include "rfdevice_papm_data.h"
#include "rfdevice_physical_device.h"

#define RFDEVICE_PAPM_MAX_REG_SIZE 30

class rfdevice_papm_asm:public rfdevice_asm
{
public:
  rfdevice_papm_asm();

  rfdevice_papm_asm
  (
    rfdevice_physical_device *phy_dev_obj_ptr,
    rfc_phy_device_info_type *phy_device_info, 
    rfc_logical_device_info_type *logical_device_info
  ) ;
  
  ~rfdevice_papm_asm();
  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    interface function for asm rx config

    @details
    rfdevice_papm_asm_config_rx()

    @param mode
    enum indicating the Radio Access Technology

    @param band
    enum indicating band number

    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
  boolean enable
  (
    rfdevice_asm_cfg* asm_cfg_p, rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action, int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  ) ;

  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    interface function for asm disable rx

    @details
    rfdevice_papm_asm_disable_rx()

    @param mode
    enum indicating the Radio Access Technology

    @param band
    enum indicating band number

    @param buff_obj_ptr
    void pointer to CCS buffer object

    @param dev_action
    indicates if it is immediate write to hw or build scripts

    @param script_timing
    timing offset for CCS scripts

    @return
    boolean indicating pass or fail
  */
  boolean disable
  (
    rfdevice_asm_cfg* asm_cfg_p, rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action, int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  ) ;

  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    interface function for asm sleep

    @details
    rfdevice_papm_asm_sleep()

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
    rfdevice_asm_cfg* asm_cfg_p, rf_buffer_intf *buff_obj_ptr, rf_device_execution_type dev_action, int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  ) ;

  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    interface function for asm wakeup

    @details
    rfdevice_papm_asm_wakeup()

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
    rfdevice_asm_cfg* asm_cfg_p, rf_buffer_intf *buff_obj_ptr, rf_device_execution_type dev_action, int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  ) ;


  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    interface function for asm trigger, to latch the pending settings.

    @details
    rfdevice_papm_asm_trigger()

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
    rfdevice_asm_cfg* asm_cfg_p,
    rf_buffer_intf *buff_obj_ptr,
    rf_device_execution_type dev_action,
    int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
  ) ;

  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    query max script size

    @details
    rfdevice_papm_asm_get_script_size()

    @return
    max script size of type uint16
  */
  uint16 get_script_size
  (
    void
  ) ;

  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    populates the rx band map information into the asm device class

    @details
    populates the rx band port map information into the asm device class

    @param mode
    enum indicating the Radio Access Technology

    @param band
    enum indicating band number

    @param data
    band mapping information passed as int32 pointer
  */
  boolean set_rx_band_map
  (
    rfm_device_enum_type rfm_device, rfcom_mode_enum_type mode, rfcom_band_type_u band,
    int32 *data_ptr, uint8 size
  ) ;

  /*----------------------------------------------------------------------------*/
  /*!
    @brief
    populates the tx band map information into the asm device class

    @details
    populates the tx band port map information into the asm device class

    @param mode
    enum indicating the Radio Access Technology

    @param band
    enum indicating band number

    @param data
    band mapping information passed as int32 pointer
  */
  boolean set_tx_band_map
  (
    rfm_device_enum_type rfm_device, rfcom_mode_enum_type mode, rfcom_band_type_u band,
    int32 *data_ptr, uint8 size
  ) ;  

protected:

  rfdevice_physical_device* parent_physical_device_ptr;

  /*! pointer to the PA device reg settings data object */
  rfdevice_papm_data *papm_data_ptr;

  rfdevice_papm_misc_info ET_voltage_info;

  boolean init_status; /* PA device initialization status */
  
  uint8 rffe_bus;  /*! RFFE channel/bus information */    
  uint8 default_usid; /*! RFFE Default Unique Slave ID */
  uint8 assigned_usid;  /*! RFFE Programmable Unique Slave ID*/  
  uint8 gsid;  /*! RFFE Group ID */

  uint16 hw_rev_reg; /*! Stores HW revision register address*/
  uint8 hw_revision;    /*! HW revision*/

  uint16 hw_part_reg; /*! Stores HW part register address*/
  uint8 hw_part;       /*! HW part number*/

  uint16 bias;  /*! PAPM bias value and flag */
  rfcom_band_type_u curr_band;

  boolean trigger_write_flag;
  int16 non_trigger_script_timing;
  boolean standby_delay_flag;

  /*! PA band_port info for each tech/band/rfm_device */
  uint8 cdma_band_port[RFM_MAX_DEVICES][RFM_CDMA_MAX_BAND];
  uint8 wcdma_band_port[RFM_MAX_DEVICES][RFCOM_NUM_WCDMA_BANDS];
  uint8 lte_band_port[RFM_MAX_DEVICES][RFCOM_NUM_LTE_BANDS];
  uint8 gsm_band_port[RFM_MAX_DEVICES][RFCOM_NUM_GSM_BANDS];
  uint8 tdscdma_band_port[RFM_MAX_DEVICES][RFCOM_NUM_TDSCDMA_BANDS];

  rf_rffe_data_type rffe_script[RFDEVICE_PAPM_MAX_REG_SIZE];

  boolean auto_bypass_disable;


private:

  boolean config(rfm_device_enum_type rfm_device, rfcom_mode_enum_type mode, 
                 rfcom_band_type_u band, rfdevice_papm_data_req_enum_type papm_state, 
                 rfdevice_papm_bw_enum_type papm_bw, uint16 bias, 
                 rf_device_execution_type execution_type, 
                 rf_buffer_intf *script_buffer, int16 script_timing = 0);
  
  boolean prepare_buffer(rfdevice_reg_settings_type *settings, 
						             rf_device_execution_type execution_type, 
						             int16 timing, uint8 *num_trans);

  uint8 get_hw_device_rev(void);
  uint8 get_hw_device_part(void);
  void print_device_info(void);

void reset_rffe_script(void);

};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
