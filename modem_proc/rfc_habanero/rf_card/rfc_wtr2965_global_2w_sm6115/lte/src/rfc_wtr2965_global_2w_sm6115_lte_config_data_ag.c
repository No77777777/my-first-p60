
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

#include "rfc_wtr2965_global_2w_sm6115_cmn_ag.h"
#include "rfc_common.h"
#include "rfcom.h"
#include "wtr2965_typedef_ag.h"
#include "rfdevice_qtuner_api.h"



rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_init_lte_rx_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_init_lte_rx_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_init_lte_tx_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_init_lte_tx_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b1_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND1_PMB3,  /* PORT */
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b1_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b1_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND1_DMB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (6)/* RF7 */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b1_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b1_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND1_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (2)/* LTE_B1_MB3 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (15)/* TRX8_SWOUT_HB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b1_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b2_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND2_PMB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b2_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // MXD8648 - RF3
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b2_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND2_DMB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (0)/*RF1*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b2_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b2_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND2_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (91)/* LTE_B2_MB5 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (15)/* TRX9_SWOUT_HB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b2_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b3_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND3_PMB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (7)/*port_num: TRX8*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b3_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2653 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b3_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND3_DMB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (7)/*port_num:RF8*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b3_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2811 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b3_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND3_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (6)/* LTE_B3_MB2 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (15)/* TRX8_SWOUT_HB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b3_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b4_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND4_PMB3,  /* PORT */
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b4_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b4_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND4_DMB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b4_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b4_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND4_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (6)/* LTE_B4_MB2 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (15)/* TRX8_SWOUT_HB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b4_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b5_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND5_PLB1,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (6)/*port_num: TRX7*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b5_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2651 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b5_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND5_DLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (11)/*port_num:RF12*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b5_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b5_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND5_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (14)/* LTE_B5_LB5 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX7_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b5_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b7_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND7_PHB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (9)/*port_num: TRX10*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b7_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b7_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND7_DHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (5)/*port_num:RF6*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b7_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b7_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND7_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (15)/* LTE_B7_HB1 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (9)/* TRX10 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b7_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b8_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND8_PLB1,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (5)/*port_num: TRX6*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b8_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2653 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b8_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND8_DLB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (11)/*port_num:RF12*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b8_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b8_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND8_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (19)/* LTE_B8_LB1 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX6_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b8_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b12_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND12_PLB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (3)/* TRX4 */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b12_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2502 - MXD8648 - RF3
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b12_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND12_DLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (3)/*port_num:RF4*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b12_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2851 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b12_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  5,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND12_THMLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (98)/* LTE_B12_LB2_LB2IN */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* SM2916 */,  /* NAME */
    //   1,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (3)/* TRX4 */,  /* PORT_NUM */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //   },
    // },
    {
      RFDEVICE_PAPM,
      QET4101,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b12_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2403 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_02,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_11,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b13_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND13_PLB3,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (0)/* TRX1 */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b13_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b13_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND13_DLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (3)/*port_num:RF4*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b13_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2851 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b13_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  5,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND13_THMLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (100)/* LTE_B13_LB4_LB2IN */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* SM2916 */,  /* NAME */
    //   1,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (0)/* TRX1 */,  /* PORT_NUM */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //   },
    // },
    {
      RFDEVICE_PAPM,
      QET4101,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b13_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b17_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND17_PLB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (3)/*port_num: TRX4*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b17_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2502 - MXD8648 - RF3
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b17_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND17_DLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (3)/*port_num:RF4*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b17_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2851 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b17_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  5,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND17_THMLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (98)/* LTE_B12_LB2_LB2IN */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* SM2916 */,  /* NAME */
    //   1,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (3)/* TRX4 */,  /* PORT_NUM */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //   },
    // },
    {
      RFDEVICE_PAPM,
      QET4101,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b17_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2403 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_02,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_11,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b18_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND18_PLB1,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (6)/*port_num: TRX7*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b18_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2651 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b18_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND18_DLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (11)/*port_num:RF12*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b18_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b18_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND18_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (14)/* LTE_B5_LB5 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX7_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b18_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b19_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND19_PLB1,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (6)/*port_num: TRX7*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b19_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2651 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b19_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND19_DLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (11)/*port_num:RF12*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b19_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b19_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND19_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (14)/* LTE_B5_LB5 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX7_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b19_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b20_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND20_PLB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (1)/*port_num: TRX2*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b20_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2502 - MXD8648 - RF4
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b20_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND20_DLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (8)/*port_num:RF9*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b20_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2851 - MXD8648 - RF4
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b20_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND20_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (12)/* LTE_B20_LB3 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX2_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b20_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b25_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND25_PMB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (8)/*port_num: TRX9*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b25_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2653 - MXD8648 - RF3
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b25_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND25_DMB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (0)/*port_num:RF1*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b25_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2811 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b25_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND25_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (91)/* LTE_B25_MB5 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (15)/* TRX9_SWOUT_HB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b25_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b26_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND26_PLB1,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (6)/*port_num: TRX7*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b26_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2651 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b26_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND26_DLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (11)/*port_num:RF12*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b26_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b26_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND26_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (14)/* LTE_B26_LB5 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX7_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b26_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b28_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND28_PLB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (4)/*port_num: TRX5*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b28_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2502 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b28_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND28_DLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (2)/*port_num:RF3*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b28_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2851 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b28_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND28_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (25)/* LTE_B28_LB2 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX5_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b28_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2403 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_02,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_11,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b28_b_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND28_PLB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (4)/*port_num: TRX5*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b28_b_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2502 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b28_b_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND28_DLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (2)/*port_num:RF3*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b28_b_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2851 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b28_b_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND28_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (25)/* LTE_B28_LB2 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (14)/* TRX5_SWOUT_LB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b28_b_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2403 - MXD8648 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_02,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_11,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b34_device_info =
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND34_PMB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (10)/*port_num: TRX11*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b34_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2653 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b34_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_DRXLGY1_BAND34_DMB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (10)/*port_num:RF11*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b34_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2811 - MXD8648 - RF4
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b34_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_TX_BAND34_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA /* SM2916 */,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (5)/* TDDLTE B34 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b34_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_INTERNAL_GNSS_BLANK_CONCURRENCY,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b38_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND38_PHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM5643 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (3)/*port_num: HB4->HBRX2*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/*port_num: TRX14*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b38_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_01,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b38_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_DRXLGY1_BAND38_DHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (4)/*port_num:RF5*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b38_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2815 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b38_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_TX_BAND38_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (18)/* LTE_B38_HB4 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/* TRX14 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b38_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b39_device_info =
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND39_PMB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (11)/*port_num: TRX12*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b39_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2653 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b39_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_DRXLGY1_BAND39_DMB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (10)/*port_num:RF11*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b39_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2811 - MXD8648 - RF3
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b39_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_TX_BAND39_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA /* SM2916 */,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (5)/* TDDLTE B39 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b39_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_INTERNAL_GNSS_BLANK_CONCURRENCY,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b40_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND40_PHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM5643 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (1)/*port_num: HB2->HBRX2*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (12)/*port_num: TRX13*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b40_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_01,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b40_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_DRXLGY1_BAND40_DHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (9)/*port_num:RF10*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b40_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2815 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b40_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_TX_BAND40_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_LUT_PWR_INVALID << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (40)/* LTE_B40_HB2 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (12)/* TRX13 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b40_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b41_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND41_PHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM5643 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (3)/*port_num: HB4->HBRX2*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/*port_num: TRX14*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b41_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_01,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b41_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_DRXLGY1_BAND41_DHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (4)/*port_num:RF5*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b41_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2815 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b41_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_TX_BAND41_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (18)/* LTE_B41_HB4 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/* TRX14 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b41_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b41_b_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND41_PHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM5643 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (3)/*port_num: HB4->HBRX2*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/*port_num: TRX14*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b41_b_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_01,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b41_b_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_DRXLGY1_BAND41_DHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (4)/*port_num:RF5*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b41_b_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2815 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b41_b_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_TX_BAND41_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (18)/* LTE_B41_HB4 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/* TRX14 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b41_b_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b41_c_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND41_PHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* SM5643 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (3)/*port_num: HB4->HBRX2*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/*port_num: TRX14*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b41_c_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_01,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b41_c_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_DRXLGY1_BAND41_DHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (4)/*port_num:RF5*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b41_c_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2815 - RF1
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b41_c_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTETDD_TX_BAND41_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (18)/* LTE_B41_HB4 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (13)/* TRX14 */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b41_c_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b66_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND66_PMB3,  /* PORT */
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b66_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b66_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND66_DMB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b66_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b66_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  6,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND66_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (6)/* LTE_B66_MB2 */,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (15)/* TRX8_SWOUT_HB */,  /* PORT_NUM */
        0,  /* Array Filler */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b66_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b71_device_info =
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND71_PLB2,  /* PORT */
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
        (0x2E9 << 22)/*mfg_id*/ | (0x29 << 14)/*prd_id*/ | (2)/*port_num: TRX3*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_0_lte_b71_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2502 - MXD8648 - RF2
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_20,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b71_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  2,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_DRXLGY1_BAND71_DLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85C0F */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x0381 << 22)/*mfg_id*/ | (0x01 << 14)/*prd_id*/ | (1)/*port_num:RF2*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_rx_on_rfm_device_1_lte_b71_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2851 - MXD8648 - RF3
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b71_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  5,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR2965_LTEFDD_TX_BAND71_THMLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM,  /* SM5643 */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x02E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (4)/* Switch off */,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x2E9 << 22)/*mfg_id*/ | (0x89 << 14)/*prd_id*/ | (103)/* LTE_B71_LB2_LB2IN */,  /* PORT_NUM */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_HDET,
      TRX_HDET,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_tx_on_rfm_device_2_lte_b71_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    // U2403 - MXD8648 - RF3
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_02,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_11,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_RF_PATH_SEL_21,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_GLOBAL_2W_SM6115_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_device_info =
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
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        RFDEVICE_FBRX_PATH_INTERNAL_ADC,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_device_info =
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
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        RFDEVICE_FBRX_PATH_INTERNAL_ADC,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  RFC_INVALID_PARAM /* Warning: Not Specified */,   /* Antenna */
  1,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        RFDEVICE_FBRX_PATH_DRX_IQ,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_device_info =
{
  RFC_ENCODED_REVISION,
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  RFC_INVALID_PARAM /* Warning: Not Specified */,   /* Antenna */
  1,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR2965,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        RFDEVICE_FBRX_PATH_DRX_IQ,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_ant_swap0_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_ant_swap0_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_ant_swap0_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_ant_swap0_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_ant_swap1_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_ant_swap1_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_ant_swap1_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_ant_swap1_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_ant_swap2_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_en_ant_swap2_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_ant_swap2_device_info =
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


rfc_sig_info_type rf_card_wtr2965_global_2w_sm6115_lte_dis_ant_swap2_sig_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_band_split_info_type rf_card_wtr2965_global_2w_sm6115_rx0_lte_b28_split_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { 9210, 9374, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 0, 0 },
    { 9210, 9384, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 0, 0 },
    { 9210, 9409, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 0, 0 },
    { 9210, 9434, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 0, 0 },
    { 9210, 9459, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 0, 0 },
    { 9375, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 1, 0 },
    { 9385, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 1, 0 },
    { 9410, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 1, 0 },
    { 9435, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 1, 0 },
    { 9460, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 1, 0 },
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM }
  }
};

rfc_band_split_info_type rf_card_wtr2965_global_2w_sm6115_rx0_lte_b41_split_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 0, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 1, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 2, 0 },
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM }
  }
};

rfc_band_split_info_type rf_card_wtr2965_global_2w_sm6115_rx1_lte_b28_split_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { 9210, 9374, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 0, 0 },
    { 9210, 9384, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 0, 0 },
    { 9210, 9409, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 0, 0 },
    { 9210, 9434, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 0, 0 },
    { 9210, 9459, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 0, 0 },
    { 9375, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 1, 0 },
    { 9385, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 1, 0 },
    { 9410, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 1, 0 },
    { 9435, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 1, 0 },
    { 9460, 9659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 1, 0 },
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM }
  }
};

rfc_band_split_info_type rf_card_wtr2965_global_2w_sm6115_rx1_lte_b41_split_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 0, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 1, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 2, 0 },
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM }
  }
};

rfc_band_split_info_type rf_card_wtr2965_global_2w_sm6115_tx2_lte_b28_split_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { 27210, 27374, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 0, 0 },
    { 27210, 27384, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 0, 0 },
    { 27210, 27409, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 0, 0 },
    { 27210, 27434, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 0, 0 },
    { 27210, 27459, (int)RFCOM_BAND_LTE_B28, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 0, 0 },
    { 27375, 27659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 1, 0 },
    { 27385, 27659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 1, 0 },
    { 27410, 27659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 1, 0 },
    { 27435, 27659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 1, 0 },
    { 27460, 27659, (int)RFCOM_BAND_LTE_B28_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 1, 0 },
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM }
  }
};

rfc_band_split_info_type rf_card_wtr2965_global_2w_sm6115_tx2_lte_b41_split_cfg =
{
  RFC_ENCODED_REVISION,
  {
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 0, 0 },
    { 39650, 40265, (int)RFCOM_BAND_LTE_B41, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 0, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 1, 0 },
    { 40053, 41100, (int)RFCOM_BAND_LTE_B41_B, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 1, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_3MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_5MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_10MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_15MHZ, 2, 0 },
    { 40900, 41589, (int)RFCOM_BAND_LTE_B41_C, RFC_FILTER_CFG_NOT_APP, RFC_BW_20MHZ, 2, 0 },
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM }
  }
};

rfc_lte_properties_type rfc_wtr2965_global_2w_sm6115_lte_properties =
{
  RFC_ENCODED_REVISION,
  /* LTE_PARTIAL_BAND_SUPPORTED */
  {
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM },
  }
};

