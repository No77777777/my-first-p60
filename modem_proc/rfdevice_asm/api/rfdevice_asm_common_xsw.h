#ifndef RFDEVICE_ASM_COMMON_XSW_H
#define RFDEVICE_ASM_COMMON_XSW_H


/*===========================================================================

Copyright (c) 2019-2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfdevice_asm.mpss/1.32/api/rfdevice_asm_common_xsw.h#2 $
  $DateTime: 2021/09/30 02:07:28 $
  $Author: mplcsds1 $

when       who   what, where, why
--------   ---   ---------------------------------------------------------------
09/22/21   ks    Added update_phy_dev_reg_list API
02/01/18   vp    Added support for RFFE based XSW
10/12/15   dbc   Add create_gen_asm_common_xsw_object prototype
01/15/15   dbc   Add critical section lock for all pertinent APIs
10/02/14   dbc   Implement rfdevice_asm_xsw_intf interface for tracking XSW position 
09/24/14   px    Added EFS support for all QXSW device specific scripts
09/19/14   ndb   Remove the un-used interfaces init()
08/20/14   vv    Remove unused old constructor
08/07/14   vv    Added support to decouple qxsw factory to chip specific drivers 
05/12/14   vv    Physical device support
04/01/14   dbc   Initial version

============================================================================*/

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "rfc_common.h"
#include "rfdevice_xsw.h"
#include "rfdevice_physical_device.h"
#include "rfdevice_asm_data.h"
#include "rfdevice_physical_third_party.h"
#include "rf_rffe_common.h"

#define RFDEVICE_ASM_XSM_MAX_REG_SIZE 10

typedef enum
{
  RFDEVICE_XSW_DEFAULT_STATE,
  RFDEVICE_XSW_SWAPPED_STATE,
  RFDEVICE_XSW_NUM_STATE,
  RFDEVICE_XSW_INVALID_STATE = RFDEVICE_XSW_NUM_STATE,
} rfdevice_xsw_switch_state_enum_type;

/*API to create 3rd party xsw with physical device interface support */
rfdevice_logical_component* create_gen_asm_common_xsw_object(rfdevice_physical_device *rfdevice_physical_ptr,
                                                             rfc_logical_device_info_type *logical_device_cfg);
class rfdevice_asm_common_xsw: public rfdevice_xsw
{
public:

  boolean set_cross_switch_config(rfdevice_xsw_config_type* xsconfig,
                                  rf_buffer_intf*           buff_obj_ptr, 
                                  rf_device_execution_type  dev_action, 
                                  int16 script_timing       = RFCOM_INVALID_TIMING_OFFSET);

  boolean set_band_map_default(rfcom_mode_enum_type mode, 
                               rfcom_band_type_u band, int32 *data_ptr, uint8 size );

  boolean set_band_map_swap(rfcom_mode_enum_type mode, 
                            rfcom_band_type_u band, int32 *data_ptr, uint8 size );
 
  boolean set_srs_band_map_default(rfcom_mode_enum_type mode, 
                                   rfcom_band_type_u band, int32 *data_ptr, uint8 size );

  boolean set_srs_band_map_swap(rfcom_mode_enum_type mode, 
                                rfcom_band_type_u band, int32 *data_ptr, uint8 size );

  boolean trigger(rfdevice_xsw_config_type* xsconfig, rf_buffer_intf *buff_obj_ptr, rf_device_execution_type  dev_action, 
                  int16 script_timing = RFCOM_INVALID_TIMING_OFFSET);


  boolean sleep
  (
    rfdevice_xsw_config_type  *xsconfig, 
    rf_buffer_intf            *buff_obj_ptr, 
    rf_device_execution_type  dev_action, 
    int16 script_timing       = RFCOM_INVALID_TIMING_OFFSET
  );

  boolean wakeup
  (
    rfdevice_xsw_config_type  *xsconfig, 
    rf_buffer_intf            *buff_obj_ptr, 
    rf_device_execution_type  dev_action, 
    int16 script_timing       = RFCOM_INVALID_TIMING_OFFSET
  );
   
   /*Update the physical device register address list*/ 
  void update_phy_dev_reg_list(void);
  
  virtual ~rfdevice_asm_common_xsw();
  
  rfdevice_asm_common_xsw(rfdevice_physical_device *phy_dev_obj_ptr,
                          rfc_phy_device_info_type *phy_device_info,
                          rfc_logical_device_info_type *logical_device_info);

  boolean self_test(uint32 mfg_id, uint32 prd_id, uint32 rev_id);

  protected:
  
    /*! ASM band_port info for each tech/band */
    uint8 cdma_band_port[RFM_CDMA_MAX_BAND][RFDEVICE_XSW_NUM_STATE];
    uint8 wcdma_band_port[RFCOM_NUM_WCDMA_BANDS][RFDEVICE_XSW_NUM_STATE];
    uint8 gsm_band_port[RFCOM_NUM_GSM_BANDS][RFDEVICE_XSW_NUM_STATE];
    uint8 lte_band_port[RFCOM_NUM_LTE_BANDS][RFDEVICE_XSW_NUM_STATE];
    uint8 tdscdma_band_port[RFCOM_NUM_TDSCDMA_BANDS][RFDEVICE_XSW_NUM_STATE];
  
    rf_rffe_data_type rffe_script[RFDEVICE_ASM_XSM_MAX_REG_SIZE];

    uint32 wakeup_vote[RFM_NUM_MODES];

    boolean init_status;
    rfdevice_asm_data *asm_data_ptr;

    rfdevice_physical_device *phy_device_ptr;
    rfc_phy_device_info_type *phy_device_info_ptr;
    rfc_logical_device_info_type *logical_device_info_ptr;

  private:

    boolean set_port(rfcom_mode_enum_type mode, rfcom_band_type_u band,
                     rfdevice_xsw_switch_state_enum_type switch_state,
                     int32 *data_ptr, uint8 size);

    uint8 get_port(rfdevice_xsw_config_type* xsconfig);

    boolean prepare_internal_script(rfdevice_asm_reg_settings_type *asm_settings,
                                    int16 timing, uint32 *num_trans);
    void print_device_info(void);

    void init();

};


#ifdef __cplusplus
}
#endif

#endif /* RFDEVICE_ASM_COMMON_XSW_H */
