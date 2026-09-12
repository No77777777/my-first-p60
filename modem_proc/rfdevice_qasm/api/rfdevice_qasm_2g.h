#ifndef RFDEVICE_QASM_2G_H
#define RFDEVICE_QASM_2G_H


/*===========================================================================

Copyright (c) 2019-2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_qasm.mpss/1.32/api/rfdevice_qasm_2g.h#2 $
  $DateTime: 2021/09/30 02:07:23 $
  $Author: mplcsds1 $

when       who    what, where, why
--------   ---    ---------------------------------------------------------------
09/22/21   ks     Add support to get RFFE device register address list for port dependent settings
12/27/19   smi    Add support to get RFFE device register address list
11/17/15   dbc    Voting rework to cover more ASM sharing scenarios
10/08/15   px     Add common tab methods
09/30/15   dbc    Added ASM votes for LTE ULCA and RFM scenarios
08/05/13   aca    Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "rfc_common.h"
#include "rfdevice_asm.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_qasm_2g_typedef.h"

#define MAX_PATH_SIZE 100

  class rfdevice_qasm_2g: public rfdevice_asm
  {
  public:

    boolean enable
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean sleep
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean wakeup
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean set_tx_band_map(rfm_device_enum_type rfm_device, rfcom_mode_enum_type mode,
	                        rfcom_band_type_u band,
                            int32 *data_ptr,uint8 size);

    boolean set_rx_band_map(rfm_device_enum_type rfm_device, rfcom_mode_enum_type mode,
                            rfcom_band_type_u band, int32 *data_ptr,
                            uint8 size);	
    	
    boolean disable
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean trigger
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

#ifdef FEATURE_RFA_DEVICE_DEBUG
   #error code not present
#endif /* FEATURE_RFA_DEVICE_DEBUG */

    virtual ~rfdevice_qasm_2g();


  /* Overloaded constructor for physical device support */
    rfdevice_qasm_2g(
                  rfdevice_physical_device* phy_obj_ptr,
                  rfc_phy_device_info_type *phy_device_info, 
                  rfc_logical_device_info_type *logical_device_info,
                  rfdevice_qasm_2g_settings_type *qasm_settings
                  ); 

    rfdevice_qasm_2g(void);

    /*--------   Pure virtual functions not needed by QASM ----------*/

    uint16 get_script_size(void);

    /*--------  End Pure virtual functions not needed by QASM -------*/


  protected:

    rfdevice_qasm_2g *rfdevice_qasm_2g_p;

   /*To store the parent physical device object*/
   rfdevice_physical_device* rfdevice_phy_obj_ptr;

  
    rfdevice_qasm_2g_settings_type qasm_settings;

    rfc_device_cfg_info_type qasm_device_info;

    rfdevice_qasm_2g_ag_cfg_type qasm_device_cfg;

    rfdevice_qasm_2g_port_info_type rx_port_mapping;

    rfdevice_qasm_2g_port_info_type tx_port_mapping;


    boolean enable_rx
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean enable_tx
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean disable_rx
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean disable_tx
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean rfdevice_qasm_2g_get_port_settings
    (
      rfdevice_qasm_2g* qasm_device,
	  rfm_device_enum_type rfm_device,
      rfcom_mode_enum_type mode,
      rfcom_band_type_u band,
      rfdevice_qasm_2g_port_type port_select,
      uint8* port_index,
      rfdevice_qasm_2g_port_settings_type** tech_settings
    );

    boolean rfdevice_qasm_2g_get_tech_port_info
    (
      rfdevice_qasm_2g* qasm_device,
	  rfm_device_enum_type rfm_device,
      rfcom_mode_enum_type mode,
      rfcom_band_type_u band,
      rfdevice_qasm_2g_port_type port_select,
      uint8** port_data_ptr
    );

    boolean rfdevice_qasm_2g_process_ag_table
    (
      rfdevice_qasm_2g* qasm_device,
      rf_buffer_intf *rf_buff,
      const uint16* reg_table,
      const int16* data_table,
      uint8 write_size,
      rf_device_execution_type dev_action,
      int16 script_timing,
      boolean combined_trigger = FALSE,
      uint8 port_index = 0
    );

    void init(void);

    /*Update physical device register address list*/
    void update_phy_dev_reg_list();

    boolean update_one_script_reg_addr(const uint16 ** script_tbl, uint8 size);
	
	boolean update_port_script_reg_addr(const uint16 ** reg_tbl, uint8 size);

    boolean rfdevice_qasm_2g_write_efs(rfdevice_qasm_2g* qasm_device);

    boolean rfdevice_qasm_2g_construct_driver_ag
    (
      rfc_device_cfg_info_type* cfg,
      rfdevice_qasm_2g* qasm_device
    );


    boolean rfdevice_qasm_2g_param_validity
    (
      rfcom_mode_enum_type mode,
      rfcom_band_type_u band,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing
    );

    /* Common Tab Methods */
    boolean common_power_up_init();

    boolean common_sleep
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

    boolean common_wakeup
    (
      rfdevice_asm_cfg *asm_cfg_p,
      rf_buffer_intf *buff_obj_ptr,
      rf_device_execution_type dev_action,
      int16 script_timing = RFCOM_INVALID_TIMING_OFFSET
    );

#ifdef FEATURE_RFA_DEVICE_DEBUG	
  #error code not present
#endif /* FEATURE_RFA_DEVICE_DEBUG */

  uint64 rx_tx_vote[RFM_NUM_MODES][2];	
  uint64 sleep_wakeup_vote[RFM_NUM_MODES][2];  
  uint64 lte_ca_vote[RFM_MAX_WAN_DEVICES][RFDEVICE_ASM_RX_TX_MAX];
  uint32 sv_vote[RFM_MAX_WAN_DEVICES];
    
  private:
    uint8* efs_data_ptr;
    rfdevice_qasm_2g_efs_settings_type* efs_settings;
    rfdevice_qasm_2g_efs_settings_type* efs_settings_enable_tx;
    rfdevice_qasm_2g_efs_settings_type* efs_settings_disable_tx;
    rfdevice_qasm_2g_efs_settings_type* efs_settings_enable_rx;
    rfdevice_qasm_2g_efs_settings_type* efs_settings_disable_rx;
    void get_efs_qasm_data(void);
    rfdevice_qasm_2g_efs_settings_type* get_efs_qasm_settings(char const* script_label);
  };

#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_QASM_H */
