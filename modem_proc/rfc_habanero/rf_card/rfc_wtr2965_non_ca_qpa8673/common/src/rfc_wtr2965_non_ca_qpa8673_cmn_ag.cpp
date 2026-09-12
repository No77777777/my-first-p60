
/*
WARNING: This file is auto-generated.

Generated using: rfc_autogen.exe
Generated from:  RFC_WTR3925_CHILE_UL_DLCA_3550 of RFC_HWSWCD.xlsm
*/

/*=============================================================================

          R F C     A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the RF Card.

Copyright (c) 2017 Qualcomm Technologies Incorporated.  All Rights Reserved.

$Header: //source/qcom/qct/modem/rfc/habanero/main/1.11/common/etc/rfc_src_autogen.pm#1 n

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/
#include "comdef.h"

#include <stringl/stringl.h>
#include "rfc_msm_signal_info_ag.h" 
#include "rfc_wtr2965_non_ca_qpa8673_cmn_ag.h" 
#include "DDITlmm.h" 
#include "rfc_common.h" 
#include "rfcommon_core_sbi.h" 

#ifdef FEATURE_GSM
#include "rfc_wtr2965_non_ca_qpa8673_gsm_config_ag.h" 
#endif 

#ifdef FEATURE_CDMA1X
#include "rfc_wtr2965_non_ca_qpa8673_cdma_config_ag.h" 
#endif 

#ifdef FEATURE_WCDMA
#include "rfc_wtr2965_non_ca_qpa8673_wcdma_config_ag.h" 
#endif 

#ifdef FEATURE_LTE
#include "rfc_wtr2965_non_ca_qpa8673_lte_config_ag.h" 
#endif 

#ifdef FEATURE_TDSCDMA
#include "rfc_wtr2965_non_ca_qpa8673_tdscdma_config_ag.h" 
#endif 

#ifdef FEATURE_CGPS
#include "rfc_wtr2965_non_ca_qpa8673_gnss_config_ag.h" 
#endif 



rfc_phy_device_info_type rfc_wtr2965_non_ca_qpa8673_phy_devices_list[] = 
{
  { /*Device: WTR2965 */ 
    WTR2965, /* PHY_DEVICE_NAME */ 
    0, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    4,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0xCA, /* PHY_DEVICE_PRODUCT_ID */ 
    0, /* PHY_DEVICE_PRODUCT_REV */ 
    0x1, /* DEFAULT USID RANGE START */
    0x1, /* DEFAULT USID RANGE END */
    0x1, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_TX_MODEM_CHAIN_0, /* ASSOCIATED_DAC */ 
  }, /* END - Device: WTR2965 */ 

  { /*Device: nz5596 */ 
    GEN_DEVICE, /* PHY_DEVICE_NAME */ 
    1, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x034E, /* PHY_DEVICE_MANUFACTURER_ID */  //register 0x1E and 0x1F bits [5:4],Lower  eight  bits  of  MIPI  Manufacturer  ID(0x34) | Upper  two  bits  of  MIPI  Manufacturer  ID(0x01) << 8
    0x04, /* PHY_DEVICE_PRODUCT_ID */ //register 0x1D,Lower  eight  bits  of  Product  Number(0x23)
    0, /* PHY_DEVICE_PRODUCT_REV */  //register 0x21  product's revision  number
    0xF, /* DEFAULT USID RANGE START */ //register 0x1F   USID[3:0]
    0xF, /* DEFAULT USID RANGE END */  //register 0x1F   USID[3:0]
    0xF, /* PHY_DEVICE_ASSIGNED_USID */   //register 0x1F   USID[3:0]
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */
    FALSE, /* INIT */
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */
  }, /* END - Device: nz5596g */

  { /*Device: nz5627 */ 
    GEN_DEVICE, /* PHY_DEVICE_NAME */ 
    2, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x034E, /* PHY_DEVICE_MANUFACTURER_ID */  //register 0x1E and 0x1F bits [5:4],Lower  eight  bits  of  MIPI  Manufacturer  ID | Upper  two  bits  of  MIPI  Manufacturer  ID << 8
    0x05, /* PHY_DEVICE_PRODUCT_ID */ //register 0x1D,Lower  eight  bits  of  Product  Number
    0, /* PHY_DEVICE_PRODUCT_REV */  //register 0x21  product's revision  number
    0xF, /* DEFAULT USID RANGE START */ //register 0x1F   USID[3:0]
    0xF, /* DEFAULT USID RANGE END */  //register 0x1F   USID[3:0]
    0xD, /* PHY_DEVICE_ASSIGNED_USID */   //register 0x1F   USID[3:0]  ??? 0xC ???
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */
    FALSE, /* INIT */
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */
  }, /* END - Device: nz5627g */

  { /*Device: QET4101 */ 
    QET4101, /* PHY_DEVICE_NAME */ 
    3, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x34, /* PHY_DEVICE_PRODUCT_ID */ 
    0, /* PHY_DEVICE_PRODUCT_REV */ 
    0x4, /* DEFAULT USID RANGE START */
    0x4, /* DEFAULT USID RANGE END */
    0x4, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    TRUE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QET4101 */ 

  { /*Device: AW13512QNR */ 
    GEN_ASM, /* PHY_DEVICE_NAME */ 
    4, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x49, /* PHY_DEVICE_MANUFACTURER_ID */  //register 0x1E and 0x1F bits [5:4],Lower  eight  bits  of  MIPI  Manufacturer  ID | Upper  two  bits  of  MIPI  Manufacturer  ID << 8
    0x05, /* PHY_DEVICE_PRODUCT_ID */ //register 0x1D,Lower  eight  bits  of  Product  Number
    0, /* PHY_DEVICE_PRODUCT_REV */  //register 0x21  product's revision  number
    0xB, /* DEFAULT USID RANGE START */ //register 0x1F   USID[3:0] 
    0xB, /* DEFAULT USID RANGE END */  //register 0x1F   USID[3:0] 
    0xB, /* PHY_DEVICE_ASSIGNED_USID */   //register 0x1F   USID[3:0] 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: AW13512QNR */ 

  { /*Device: RFDEVICE_INVALID */ 
    RFDEVICE_INVALID, /* PHY_DEVICE_NAME */ 
    0 /*Warning: Not specified*/, /* PHY_DEVICE_INSTANCE */ 
    0 /*Warning: Not specified*/, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_INVALID, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    0 /* 0 not specified */,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0 /*Warning: Not specified*/, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0 /*Warning: Not specified*/, /* PHY_DEVICE_PRODUCT_ID */ 
    0 /*Warning: Not specified*/, /* PHY_DEVICE_PRODUCT_REV */ 
    0 /*Warning: Not specified*/, /* DEFAULT USID RANGE START */
    0 /*Warning: Not specified*/, /* DEFAULT USID RANGE END */
    0 /*Warning: Not specified*/, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: RFDEVICE_INVALID */ 

};


rfc_logical_device_info_type rfc_wtr2965_non_ca_qpa8673_logical_devices_list[] = 
{
  { /*Device: WTR2965 */ 
    RFDEVICE_TRANSCEIVER, /* DEVICE_MODULE_TYPE */ 
    WTR2965, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    0, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: WTR2965 */ 

  { /*Device: QET4101 */
    RFDEVICE_PAPM, /* DEVICE_MODULE_TYPE */
    QET4101, /* DEVICE_MODULE_NAME */
    0, /* DEVICE_MODULE_TYPE_INSTANCE */
    3, /* ASSOCIATED_PHY_DEVICE_INSTANCE */
  }, /* END - Device: QET4101 */

  { /*Device: nz5596g pa*/
    RFDEVICE_PA, /* DEVICE_MODULE_TYPE */
    GEN_PA, /* DEVICE_MODULE_NAME */
    0, /* DEVICE_MODULE_TYPE_INSTANCE */
    1, /* ASSOCIATED_PHY_DEVICE_INSTANCE */
  }, /* END - Device: nz5596g */

  { /*Device: nz5627g pa */
    RFDEVICE_PA, /* DEVICE_MODULE_TYPE */
    GEN_PA, /* DEVICE_MODULE_NAME */
    1, /* DEVICE_MODULE_TYPE_INSTANCE */
    2, /* ASSOCIATED_PHY_DEVICE_INSTANCE */
  }, /* END*/

  { /*Device: nz5596g asm */
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */
    GEN_ASM, /* DEVICE_MODULE_NAME */
    0, /* DEVICE_MODULE_TYPE_INSTANCE */
    1, /* ASSOCIATED_PHY_DEVICE_INSTANCE */
  },  /*END*/

  { /*Device: nz5627g asm */
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */
    GEN_ASM, /* DEVICE_MODULE_NAME */
    1, /* DEVICE_MODULE_TYPE_INSTANCE */
    2, /* ASSOCIATED_PHY_DEVICE_INSTANCE */
  },  /*END*/

  { /*Device: AW13512QNR */
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */
    GEN_ASM, /* DEVICE_MODULE_NAME */
    2, /* DEVICE_MODULE_TYPE_INSTANCE */
    4, /* ASSOCIATED_PHY_DEVICE_INSTANCE */
  }, /* END - Device: AW13512QNR */

  { /*Device: TRX_HDET */ 
    RFDEVICE_HDET, /* DEVICE_MODULE_TYPE */ 
    TRX_HDET, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    0, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: TRX_HDET */ 

  { /*Device: RFDEVICE_INVALID */ 
    RFDEVICE_TYPE_INVALID, /* DEVICE_MODULE_TYPE */ 
    RFDEVICE_INVALID, /* DEVICE_MODULE_NAME */ 
    0 /*Warning: Not specified*/, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    0 /*Warning: Not specified*/, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: RFDEVICE_INVALID */ 

};


rfc_signal_info_type rfc_wtr2965_non_ca_qpa8673_sig_info[RFC_WTR2965_NON_CA_QPA8673_SIG_NUM + 1] = 
{
  { RFC_MSM_TIMING_PA_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PA_CTL */ 
  { RFC_MSM_TIMING_PA_RANGE , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PA_RANGE */ 
  { RFC_MSM_TIMING_ASM_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_ASM_CTL */ 
  { RFC_MSM_TIMING_TUNER_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_TUNER_CTL */ 
  { RFC_MSM_TIMING_PAPM_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PAPM_CTL */ 
  { RFC_MSM_TIMING_TX_TX_RF_ON0 , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_TX_TX_RF_ON0 */ 
  { RFC_MSM_TIMING_TX_RX_RF_ON0 , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_TX_RX_RF_ON0 */ 
  { RFC_MSM_TIMING_ASM_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_ASM_TRIGGER */ 
  { RFC_MSM_TIMING_PAPM_TX_TX_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PAPM_TX_TX_TRIGGER */ 
  { RFC_MSM_TIMING_PAPM_OFF_TX_RX_TX_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PAPM_OFF_TX_RX_TX_TRIGGER */ 
  { RFC_MSM_TIMING_PA_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PA_TRIGGER */ 
  { RFC_MSM_TIMING_PAPM_OFF_TX_RX_TX_CTL , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PAPM_OFF_TX_RX_TX_CTL */ 
  { RFC_MSM_TIMING_PAPM_MULTISLOT_CTL , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PAPM_MULTISLOT_CTL */ 
  { RFC_MSM_TIMING_PAPM_TX_TX_CTL , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TIMING_PAPM_TX_TX_CTL */ 
  { RFC_MSM_RF_PATH_SEL_01 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01 */ 
  { RFC_MSM_RF_PATH_SEL_09 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09 */ 
  //grfc 4 -- gpio 41
  { RFC_MSM_RF_PATH_SEL_04 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04 */ 
  //grfc 6 -- gpio 43
  { RFC_MSM_RF_PATH_SEL_15 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_15 */ 
  //grfc 5 -- gpio 42 
  { RFC_MSM_RF_PATH_SEL_17 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_17 */ 
  //grfc 7 -- gpio 44
  { RFC_MSM_RF_PATH_SEL_05 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05 */ 
  //grfc 8 -- gpio 45
  { RFC_MSM_RF_PATH_SEL_20 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20 */ 
  //grfc 9 -- gpio 46
  { RFC_MSM_RF_PATH_SEL_06 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06 */ 
  //grfc 11 -- gpio 48
  { RFC_MSM_RF_PATH_SEL_07 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_07 */
  //grfc 14 -- gpio 51
  { RFC_MSM_RF_PATH_SEL_21 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_21 */
  //grfc 10 -- gpio 47
  { RFC_MSM_RF_PATH_SEL_14 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14 */
  { RFC_MSM_GPDATA0_0 , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_GPDATA0_0 */ 
  { RFC_MSM_RFFE5_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE5_CLK */ 
  { RFC_MSM_RFFE5_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE5_DATA */ 
  { RFC_MSM_RFFE1_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE1_CLK */ 
  { RFC_MSM_RFFE1_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE1_DATA */ 
  { RFC_MSM_RFFE2_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE2_CLK */ 
  { RFC_MSM_RFFE2_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE2_DATA */ 
  { RFC_MSM_RFFE4_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE4_CLK */ 
  { RFC_MSM_RFFE4_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_RFFE4_DATA */ 
  { RFC_MSM_INTERNAL_GNSS_BLANK , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_INTERNAL_GNSS_BLANK */ 
  { RFC_MSM_INTERNAL_GNSS_BLANK_CONCURRENCY , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_INTERNAL_GNSS_BLANK_CONCURRENCY */ 
  { RFC_MSM_TX_GTR_TH , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH */ 
  { RFC_MSM_PA_IND , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR2965_NON_CA_QPA8673_PA_IND */ 
  { (rfc_msm_signal_type)RFC_SIG_LIST_END   , (rfc_logic_type)RFC_ENCODED_REVISION, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL } /* LAST SIG INDICATOR */ 
};


rfc_common_data* rfc_wtr2965_non_ca_qpa8673_cmn_ag::get_instance(rf_hw_type rf_hw)
{
  if (rfc_common_data_ptr == NULL)
  {
    rfc_common_data_ptr = (rfc_common_data *)new rfc_wtr2965_non_ca_qpa8673_cmn_ag(rf_hw);
  }
  return( (rfc_common_data *)rfc_common_data_ptr);
}

//constructor
rfc_wtr2965_non_ca_qpa8673_cmn_ag::rfc_wtr2965_non_ca_qpa8673_cmn_ag(rf_hw_type rf_hw)
  :rfc_common_data(rf_hw)
{
}


uint32 rfc_wtr2965_non_ca_qpa8673_cmn_ag::sig_info_table_get(rfc_signal_info_type **rfc_info_table)
{
  if (NULL == rfc_info_table)
  {
    return 0;
  }

  *rfc_info_table = &rfc_wtr2965_non_ca_qpa8673_sig_info[0];

#ifdef FEATURE_GSM
  // Create GSM RFC AG Data Object
  rfc_gsm_data *rfc_gsm_data = rfc_wtr2965_non_ca_qpa8673_gsm_ag::get_instance(); 
  if (rfc_gsm_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR GSM Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_GSM */

#ifdef FEATURE_CDMA1X
  // Create CDMA RFC AG Data Object
  rfc_cdma_data *rfc_cdma_data = rfc_wtr2965_non_ca_qpa8673_cdma_ag::get_instance(); 
  if (rfc_cdma_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR CDMA Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_CDMA */

#ifdef FEATURE_WCDMA
  // Create WCDMA RFC AG Data Object
  rfc_wcdma_data *rfc_wcdma_data = rfc_wtr2965_non_ca_qpa8673_wcdma_ag::get_instance(); 
  if (rfc_wcdma_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR WCDMA Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_LTE
  // Create LTE RFC AG Data Object
  rfc_lte_data *rfc_lte_data = rfc_wtr2965_non_ca_qpa8673_lte_ag::get_instance(); 
  if (rfc_lte_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR LTE Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_LTE */

#ifdef FEATURE_TDSCDMA
  // Create TDSCDMA RFC AG Data Object
  rfc_tdscdma_data *rfc_tdscdma_data = rfc_wtr2965_non_ca_qpa8673_tdscdma_ag::get_instance(); 
  if (rfc_tdscdma_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR TDSCDMA Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_TDSCDMA */

#ifdef FEATURE_CGPS
  // Create GNSS RFC AG Data Object
  rfc_gnss_data *rfc_gnss_data = rfc_wtr2965_non_ca_qpa8673_gnss_ag::get_instance(); 
  if (rfc_gnss_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR GNSS Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_GNSS */

  return RFC_WTR2965_NON_CA_QPA8673_SIG_NUM;
}

rfc_phy_device_info_type* rfc_wtr2965_non_ca_qpa8673_cmn_ag::get_phy_device_cfg( void )
{
  return (&rfc_wtr2965_non_ca_qpa8673_phy_devices_list[0]);
}

rfc_logical_device_info_type* rfc_wtr2965_non_ca_qpa8673_cmn_ag::get_logical_device_cfg( void )
{
  return (&rfc_wtr2965_non_ca_qpa8673_logical_devices_list[0]);
}


rfm_devices_configuration_type rfc_wtr2965_non_ca_qpa8673_logical_device_properties = 
{
  564, /* HWID */
  { /* device_support */ 
    { /* Logical Device 0 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_III_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_IV_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND12 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND13 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND17 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) |( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND26 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) ),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_III_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_IV_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND12 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND13 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND17 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) |( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND26 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) ),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( RFM_DEVICE_RX_SUPPORTED),
      RFM_DEVICE_1, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_0 (Valid for Rx Devs only)*/
      RFM_DEVICE_2, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_0 (Valid for Rx Devs only)*/
    }, /* End Logical Device 0 */ 
    { /* Logical Device 1 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_III_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_IV_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND12 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND13 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND17 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) |( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND26 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) ),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( RFM_DEVICE_RX_SUPPORTED),
      RFM_DEVICE_0, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_1 (Valid for Rx Devs only)*/
      RFM_DEVICE_2, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_1 (Valid for Rx Devs only)*/
    }, /* End Logical Device 1 */ 
    { /* Logical Device 2 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_III_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_IV_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND12 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND13 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND17 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) |( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND26 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) ),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_III_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_IV_1700 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND4 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND12 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND13 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND17 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) |( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND26 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) ),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( RFM_DEVICE_TX_SUPPORTED),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_2 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_2 (Valid for Rx Devs only)*/
    }, /* End Logical Device 2 */ 
    { /* Logical Device 3 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( 0),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_3 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_3 (Valid for Rx Devs only)*/
    }, /* End Logical Device 3 */ 
    { /* Logical Device 4 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( 0),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_4 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_4 (Valid for Rx Devs only)*/
    }, /* End Logical Device 4 */ 
    { /* Logical Device 5 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( 0),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_5 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_5 (Valid for Rx Devs only)*/
    }, /* End Logical Device 5 */ 
    { /* Logical Device 6 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( 0),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_6 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_6 (Valid for Rx Devs only)*/
    }, /* End Logical Device 6 */ 
    { /* Logical Device 7 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( 0),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_7 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_7 (Valid for Rx Devs only)*/
    }, /* End Logical Device 7 */ 
    { /* Logical Device 8 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( 0),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_8 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_8 (Valid for Rx Devs only)*/
    }, /* End Logical Device 8 */ 
    { /* Logical Device 9 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      /* ho_rxd_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          (0),
          /* Bit mask element 2 */ 
          (0),
        },
      },
      ( 0),
      RFM_INVALID_DEVICE, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_9 (Valid for Rx Devs only)*/
      RFM_INVALID_DEVICE, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_9 (Valid for Rx Devs only)*/
    }, /* End Logical Device 9 */ 
  },
  {
    0, /* num_restriction_groups */ 
    NULL, /* No Concurrency Restrictions */ 
  },
  #ifdef FEATURE_CUST_1
  {
    0, /* num_dglna_combos */ 
    NULL, /* No DGLNA Combos */ 
  },
  #endif //FEATURE_CUST_1
  FALSE, /* antenna_swap_supported */ 
  0, /* concurrency_features */ 
};

boolean rfc_wtr2965_non_ca_qpa8673_cmn_ag::get_logical_path_config(rfm_devices_configuration_type* dev_cfg)
{
  if ( dev_cfg == NULL )
  {
    RF_MSG_1(RF_ERROR, "rfm_get_devices_configuration: Invalid Container", 0);
    return FALSE;
  }

  memscpy(dev_cfg,
          sizeof(rfm_devices_configuration_type),
          &rfc_wtr2965_non_ca_qpa8673_logical_device_properties,
          sizeof(rfm_devices_configuration_type));

  return TRUE;
}

const rfm_devices_configuration_type* rfc_wtr2965_non_ca_qpa8673_cmn_ag::get_logical_device_properties( void )
{
  return &rfc_wtr2965_non_ca_qpa8673_logical_device_properties;
}



rfc_cmn_properties_type rfc_wtr2965_non_ca_qpa8673_cmn_properties = 
{
  RFC_ENCODED_REVISION,   
  RFC_ATTENUATION_STATE_MID,
};

boolean rfc_wtr2965_non_ca_qpa8673_cmn_ag::get_cmn_properties(rfc_cmn_properties_type **ptr)
{
  if (NULL==ptr)
  {
    return FALSE;
  }

  *ptr = &rfc_wtr2965_non_ca_qpa8673_cmn_properties;
  return TRUE;
}

boolean rfc_wtr2965_non_ca_qpa8673_cmn_ag::rfc_get_remapped_device_info
(
  rfc_cal_device_remap_info_type *source_device_info,
  rfc_cal_device_remap_info_type *remapped_device_info
)
{
  return TRUE;
}