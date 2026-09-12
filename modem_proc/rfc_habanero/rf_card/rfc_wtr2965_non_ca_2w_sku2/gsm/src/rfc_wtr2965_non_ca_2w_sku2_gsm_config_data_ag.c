
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

#include "rfc_wtr2965_non_ca_2w_sku2_cmn_ag.h" 
#include "rfc_common.h" 
#include "rfcom.h" 
#include "wtr2965_typedef_ag.h" 
#include "qpa8673_port_typedef_ag.h" 
#include "qpa8686_port_typedef_ag.h" 
#include "rfdevice_qtuner_api.h" 



rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_init_gsm_rx_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  RFC_INVALID_PARAM /* Warning: Not Specified */,   /* Antenna */
  1,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0 /*Warning: Not specified*/,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        0  /* Invalid TRx port*/ ,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_init_gsm_rx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_init_gsm_tx_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  RFC_INVALID_PARAM /* Warning: Not Specified */,   /* Antenna */
  1,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0 /*Warning: Not specified*/,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        0  /* Invalid TRx port*/ ,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_init_gsm_tx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g850_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_PRXLGY1_BAND850_PLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM2916 */,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (3)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g850_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};

rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g850_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_TX_BAND850_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (2 << 7)/* sec_port_num */ | (0)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
	{
      RFDEVICE_PAPM,
      QET4101,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g850_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -1 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_09,   { RFC_LOW, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_PRXLGY1_BAND900_PLB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM2916 */,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (2)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_TX_BAND900_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (2 << 7)/* sec_port_num */ | (0)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
	{
      RFDEVICE_PAPM,
      QET4101,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -1 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_09,   { RFC_LOW, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g1800_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_PRXLGY1_BAND1800_PMB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM2916 */,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (7)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g1800_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g1800_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_TX_BAND1800_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (3 << 7)/* sec_port_num */ | (1)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
	{
      RFDEVICE_PAPM,
      QET4101,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g1800_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -1 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_09,   { RFC_LOW, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g1900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_PRXLGY1_BAND1900_PMB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM2916 */,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (5)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    }
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_gsm_g1900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g1900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_GSM_TX_BAND1900_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (3 << 7)/* sec_port_num */ | (1)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
	{
      RFDEVICE_PAPM,
      QET4101,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_gsm_g1900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -1 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_RF_PATH_SEL_09,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR2965_NON_CA_2W_SKU2_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_en_ant_swap3_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  1,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT5515,  /* NAME */
      3,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_XSW,
      QAT3522,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x82 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_en_ant_swap3_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_dis_ant_swap3_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  0,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT5515,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_XSW,
      QAT3522,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x82 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_dis_ant_swap3_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_en_ant_swap4_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  0,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT5515,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_XSW,
      QAT3522,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x82 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_en_ant_swap4_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_dis_ant_swap4_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  1,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT5515,  /* NAME */
      3,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_XSW,
      QAT3522,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x82 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_dis_ant_swap4_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_en_ant_swap5_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  1,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT5515,  /* NAME */
      3,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_XSW,
      QAT3522,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x82 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_en_ant_swap5_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_dis_ant_swap5_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  0,   /* Antenna */
  3,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT5515,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_XSW,
      QAT3522,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x82 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_gsm_dis_ant_swap5_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_gsm_properties_type rfc_wtr2965_non_ca_2w_sku2_gsm_properties = 
{
  RFC_ENCODED_REVISION, 
  /* GSM_DESENSE_CHANNEL (Max: 25 per band) */
  {
    { (uint32)RFCOM_BAND_GSM850 , 198 },
    { (uint32)RFCOM_BAND_GSM900 , 29 },
    { (uint32)RFCOM_BAND_GSM1800 , 548 },
    { (uint32)RFCOM_BAND_GSM1800 , 549 },
    { (uint32)RFCOM_BAND_GSM1800 , 606 },
    { (uint32)RFCOM_BAND_GSM1800 , 702 },
    { (uint32)RFCOM_BAND_GSM1800 , 798 },
    { (uint32)RFCOM_BAND_GSM1900 , 549 },
    { (uint32)RFCOM_BAND_GSM1900 , 557 },
    { (uint32)RFCOM_BAND_GSM1900 , 605 },
    { (uint32)RFCOM_BAND_GSM1900 , 653 },
    { (uint32)RFCOM_BAND_GSM1900 , 701 },
    { (uint32)RFCOM_BAND_GSM1900 , 749 },
    { (uint32)RFCOM_NUM_GSM_BANDS , RFC_INVALID_PARAM },
  },
};

