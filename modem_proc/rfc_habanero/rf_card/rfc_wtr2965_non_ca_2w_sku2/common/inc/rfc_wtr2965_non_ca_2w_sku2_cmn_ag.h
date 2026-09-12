
#ifndef RFC_WTR2965_NON_CA_2W_SKU2_CMN_AG
#define RFC_WTR2965_NON_CA_2W_SKU2_CMN_AG


#ifdef __cplusplus
extern "C" {
#endif

/*
WARNING: This file is auto-generated.

Generated using: rfc_autogen.exe
Generated from:  v6.0.19 of RFC_HWSWCD.xlsm
*/

/*=============================================================================

          R F C     A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

Copyright (c) 2017 Qualcomm Technologies Incorporated.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfc/habanero/main/1.0/common/etc/rfc_src_autogen.pm#2 n

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include "rfc_common.h" 



typedef enum
{
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_CTL,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_RANGE,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_CTL,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TX_TX_RF_ON0,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TX_RX_RF_ON0,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_TRIGGER,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_TX_TX_TRIGGER,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_OFF_TX_RX_TX_TRIGGER,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_TRIGGER,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_OFF_TX_RX_TX_CTL,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_MULTISLOT_CTL,
  RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_TX_TX_CTL,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_01,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_09,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_04,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_15,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_05,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_20,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_11,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_02,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_06,
  RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_14,
  RFC_WTR2965_NON_CA_2W_SKU2_GPDATA0_0,
  RFC_WTR2965_NON_CA_2W_SKU2_RFFE5_CLK,
  RFC_WTR2965_NON_CA_2W_SKU2_RFFE5_DATA,
  RFC_WTR2965_NON_CA_2W_SKU2_RFFE1_CLK,
  RFC_WTR2965_NON_CA_2W_SKU2_RFFE1_DATA,
/*modified by liumaobo, gpio released for AP, 2022/04/25 begin*/  
  //RFC_WTR2965_NON_CA_2W_SKU2_RFFE2_CLK,
  //RFC_WTR2965_NON_CA_2W_SKU2_RFFE2_DATA,
  //RFC_WTR2965_NON_CA_2W_SKU2_RFFE4_CLK,
/*modified by liumaobo, gpio released for AP, 2022/04/25 end*/    
  RFC_WTR2965_NON_CA_2W_SKU2_RFFE4_DATA,
  RFC_WTR2965_NON_CA_2W_SKU2_INTERNAL_GNSS_BLANK,
  RFC_WTR2965_NON_CA_2W_SKU2_INTERNAL_GNSS_BLANK_CONCURRENCY,
  RFC_WTR2965_NON_CA_2W_SKU2_TX_GTR_TH,
/*modified by liumaobo, gpio49 released for AP, 2022/04/22 begin*/
  //RFC_WTR2965_NON_CA_2W_SKU2_PA_IND,
/*modified by liumaobo, gpio49 released for AP, 2022/04/22 end*/
  RFC_WTR2965_NON_CA_2W_SKU2_SIG_NUM,
  RFC_WTR2965_NON_CA_2W_SKU2_SIG_INVALID,
}wtr2965_non_ca_2w_sku2_sig_type;


#ifdef __cplusplus

#include "rfc_common_data.h"

class rfc_wtr2965_non_ca_2w_sku2_cmn_ag:public rfc_common_data
{
  public:
    uint32 sig_info_table_get(rfc_signal_info_type **rfc_info_table);
    rfc_phy_device_info_type* get_phy_device_cfg( void );
    rfc_logical_device_info_type* get_logical_device_cfg( void );
    boolean get_logical_path_config(rfm_devices_configuration_type* dev_cfg);
    const rfm_devices_configuration_type* get_logical_device_properties( void );
    boolean get_cmn_properties(rfc_cmn_properties_type **ptr);
    static rfc_common_data * get_instance(rf_hw_type rf_hw);
    boolean rfc_get_remapped_device_info 
     ( 
       rfc_cal_device_remap_info_type *source_device_info, 
       rfc_cal_device_remap_info_type *remapped_device_info 
     );


  protected:
    rfc_wtr2965_non_ca_2w_sku2_cmn_ag(rf_hw_type rf_hw);
};

#endif   /*  __cplusplus  */


#ifdef __cplusplus
}
#endif



#endif


