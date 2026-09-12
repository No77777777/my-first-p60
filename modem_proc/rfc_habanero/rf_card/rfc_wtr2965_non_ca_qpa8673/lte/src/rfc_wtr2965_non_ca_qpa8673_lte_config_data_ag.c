
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

#include "rfc_wtr2965_non_ca_qpa8673_cmn_ag.h" 
#include "rfc_common.h" 
#include "rfcom.h" 
#include "wtr2965_typedef_ag.h" 
#include "qpa8673_port_typedef_ag.h" 
#include "qpa8686_port_typedef_ag.h" 
#include "rfdevice_qtuner_api.h" 



rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_init_lte_rx_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_init_lte_rx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_init_lte_tx_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_init_lte_tx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b1_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/  |(8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b1_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 14 gpio 51 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_21,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO 51
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b1_device_info = 
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
     
    {/*AW13512QNR*/
      RFDEVICE_ASM,
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/|(5)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b1_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b1_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND1_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627 */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (3)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b1_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b2_device_info = 
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
      GEN_ASM /* NZ5596*/,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/  |(6)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
   
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b2_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 11 gpio 48 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_07,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b2_device_info = 
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
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/|(7)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b2_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 11 gpio 48 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_07,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_48
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b2_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND2_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627  */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b2_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
{
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //   //   { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO_38
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b3_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/  |(5)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b3_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 11 gpio 48 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_07,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //{ (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b3_device_info = 
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
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ |(6)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b3_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 11 gpio 48 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_07,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO_48
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b3_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND3_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (4)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (5)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b3_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    // // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO_38
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b4_device_info = 
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
      GEN_ASM /* nz5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/  |(7)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
   
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b4_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 14 gpio 51 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_21,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b4_device_info = 
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
      GEN_ASM /* AW13512QNR */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ | (5)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b4_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b4_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND4_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627 */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (2)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (7)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b4_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    // // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO38
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b5_device_info = 
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND5_PLB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};

rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b5_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 14 gpio 51 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_21,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },
    //grfc 9 gpio 46 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b5_device_info = 
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
        (int)WTR2965_LTEFDD_DRXLGY1_BAND5_DLB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* AW13512QNR */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/|(3)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b5_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b5_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND5_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /*NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b5_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO46
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b7_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b7_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //RFC_MSM_RF_PATH_SEL_20
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } }, //gpio45
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    // grfc 8 gpio 45 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b7_device_info = 
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
      GEN_ASM /* MXD68A0S */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ |(9)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b7_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO45
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b7_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND7_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (5)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b7_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
  
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b8_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (2)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b8_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b8_device_info = 
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
        (int)WTR2965_LTEFDD_DRXLGY1_BAND8_DLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* MXD85AOS */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b8_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b8_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND8_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (2)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b8_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};

rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b12_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (14)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
 },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b12_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 5/6 gpio 42/43 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_17,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO42
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO43
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b12_device_info = 
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
      GEN_ASM /* MXD85AOS */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b12_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO46
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b12_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND12_THMLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (14)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b12_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b13_device_info = 
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND13_PLB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    
        {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
 },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b13_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 5/6 gpio 42/43 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_17,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO42
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_15,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO43
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b13_device_info = 
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
      GEN_ASM /* MXD85AOS */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/| (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b13_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO46
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b13_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND13_THMLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (14)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b13_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
   //grfc 7/4 gpio 44/41 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};

rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b17_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (14)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
 },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b17_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 5/6 gpio 42/43 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_17,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO42
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO43
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b17_device_info = 
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
      GEN_ASM /* MXD85AOS */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/| (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b17_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO46
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b17_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND17_THMLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (14)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b17_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};

rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b20_device_info = 
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND20_PLB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b20_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO46
    //grfc 14 gpio 51 rf1 
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_21,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO 51
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b20_device_info = 
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
      GEN_ASM /* MXD86 */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ | (2)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b20_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO46
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b20_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND20_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (14)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* 5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b20_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO46
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};

rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b26_device_info = 
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
        (int)WTR2965_LTEFDD_PRXLGY1_BAND26_PLB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b26_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 14 gpio 51 rf2
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_21,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },
    //grfc 9 gpio 46 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b26_device_info = 
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
        (int)WTR2965_LTEFDD_DRXLGY1_BAND26_DLB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* AW13512QNR */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/|(3)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b26_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b26_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND26_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /*NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b26_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO46
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b28_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b28_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf3
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 5/6 gpio 42/43 rf3
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_17,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO42
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO43
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b28_device_info = 
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
      GEN_ASM /* MXD86 */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ | (2)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
   
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b28_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO46
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b28_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND28_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (13)/*port_num*/,  /* PORT_NUM */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //   },
    // },
    
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b28_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_INTERNAL_GNSS_BLANK_CONCURRENCY,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b28_b_device_info = 
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
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b28_b_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 7/4 gpio 44/41 rf3
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 5/6 gpio 42/43 rf3
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_17,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO42
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_15,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO43
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b28_b_device_info = 
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
      GEN_ASM /* MXD86 */,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ | (2)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
   
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b28_b_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    //grfc 9 gpio 46 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_06,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO46
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b28_b_device_info = 
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
        (int)WTR2965_LTEFDD_TX_BAND28_THMLB2,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (13)/*port_num*/,  /* PORT_NUM */
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
        (0x217 << 22)/*mfg_id*/ | (0x34 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    // {
    //   RFDEVICE_ASM,
    //   GEN_ASM /* NZ5596 */,  /* NAME */
    //   0,  /* DEVICE_MODULE_TYPE_INSTANCE */
    //   0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
    //   {
    //     0  /* Orig setting:  */,  /* INTF_REV */
    //     (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //     0,  /* Array Filler */
    //   },
    // },
    
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b28_b_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
   //grfc 7/4 gpio 44/41 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_05,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO44
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_04,   { RFC_HIGH, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO41
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_INTERNAL_GNSS_BLANK_CONCURRENCY,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b34_device_info = 
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND34_PMB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* nz5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x034E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (3)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
   
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b34_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b34_device_info = 
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
        (int)WTR2965_LTETDD_DRXLGY1_BAND34_DMB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/|(0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b34_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b34_device_info = 
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
        (int)WTR2965_LTETDD_TX_BAND34_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
     GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (4)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b34_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_08,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO50
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_INTERNAL_GNSS_BLANK_CONCURRENCY,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b38_device_info = 
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND38_PHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /*NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b38_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b38_device_info = 
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
        (int)WTR2965_LTETDD_DRXLGY1_BAND38_DHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ |(4)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b38_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b38_device_info = 
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
        (int)WTR2965_LTETDD_TX_BAND38_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b38_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
   //  { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
   //  { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
   // // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_15,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b39_device_info = 
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND39_PMB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (3)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b39_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b39_device_info = 
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
        (int)WTR2965_LTETDD_DRXLGY1_BAND39_DMB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/|(0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b39_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b39_device_info = 
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
        (int)WTR2965_LTETDD_TX_BAND39_THMLB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
     GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    {
      RFDEVICE_ASM,
      GEN_ASM /*NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (4)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b39_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_08,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },//GPIO50
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_INTERNAL_GNSS_BLANK_CONCURRENCY,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b40_device_info = 
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND40_PHB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* 5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b40_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b40_device_info = 
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
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ |(8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b40_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_01,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO37
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_09,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO38
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b40_device_info = 
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
        (int)WTR2965_LTETDD_TX_BAND40_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /*NZ5627 NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (7)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (13)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b40_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b41_device_info = 
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND41_PHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b41_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b41_device_info = 
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
        (int)WTR2965_LTETDD_DRXLGY1_BAND41_DHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ |(4)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b41_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b41_device_info = 
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
        (int)WTR2965_LTETDD_TX_BAND41_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b41_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b41_b_device_info = 
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND41_PHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b41_b_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b41_b_device_info = 
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
        (int)WTR2965_LTETDD_DRXLGY1_BAND41_DHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ |(4)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b41_b_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b41_b_device_info = 
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
        (int)WTR2965_LTETDD_TX_BAND41_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b41_b_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b41_c_device_info = 
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
        (int)WTR2965_LTETDD_PRXLGY1_BAND41_PHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (10)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_0_lte_b41_c_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b41_c_device_info = 
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
        (int)WTR2965_LTETDD_DRXLGY1_BAND41_DHB2,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* HCPA */,  /* NAME */
      2,  /* DEVICE_TYPE_INSTANCE */
      1 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x49 << 22)/*mfg_id*/ | (0x5 << 14)/*prd_id*/ |(4)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_rx_on_rfm_device_1_lte_b41_c_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // grfc 8 gpio 45 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_20,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO45
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b41_c_device_info = 
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
        (int)WTR2965_LTETDD_TX_BAND41_THMLB3,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_VALID | WTR2965_LP_LUT_TYPE << RFDEVICE_PA_STATE_0_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_1_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_2_BSHFT | WTR2965_HP_LUT_TYPE << RFDEVICE_PA_STATE_3_BSHFT ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR2965_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      GEN_PA,  /* NZ5627NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x05 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0  /* Orig setting:  */,  /* SEC_PORT */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },

    {
      RFDEVICE_ASM,
      GEN_ASM /* NZ5596 */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x34E << 22)/*mfg_id*/ | (0x04 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_tx_on_rfm_device_2_lte_b41_c_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_02,   { RFC_HIGH, 0 }, {RFC_LOW, 0 } },//GPIO39
    // { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_11,   { RFC_LOW, 0 }, {RFC_LOW, 0 } },//GPIO_40
    //grfc 10 gpio 47 rf1
    { (int)RFC_WTR2965_NON_CA_QPA8673_RF_PATH_SEL_14,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR2965_NON_CA_QPA8673_TX_GTR_TH,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_ant_swap0_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_ant_swap0_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_ant_swap0_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_ant_swap0_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_ant_swap1_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_ant_swap1_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_ant_swap1_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_ant_swap1_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_ant_swap2_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_en_ant_swap2_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_ant_swap2_device_info = 
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


rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_lte_dis_ant_swap2_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_band_split_info_type rf_card_wtr2965_non_ca_qpa8673_rx0_lte_b28_split_cfg = 
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

rfc_band_split_info_type rf_card_wtr2965_non_ca_qpa8673_rx0_lte_b41_split_cfg = 
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

rfc_band_split_info_type rf_card_wtr2965_non_ca_qpa8673_rx1_lte_b28_split_cfg = 
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

rfc_band_split_info_type rf_card_wtr2965_non_ca_qpa8673_rx1_lte_b41_split_cfg = 
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

rfc_band_split_info_type rf_card_wtr2965_non_ca_qpa8673_tx2_lte_b28_split_cfg = 
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

/**
  B41-a(2496-2585)
  B41-b(2585-2610)
  B41-c(2610-2690)
**/

rfc_band_split_info_type rf_card_wtr2965_non_ca_qpa8673_tx2_lte_b41_split_cfg = 
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

rfc_lte_properties_type rfc_wtr2965_non_ca_qpa8673_lte_properties = 
{
  RFC_ENCODED_REVISION, 
  /* LTE_PARTIAL_BAND_SUPPORTED */
  {
    { RFC_INVALID_PARAM, RFC_INVALID_PARAM, RFC_INVALID_PARAM },
  }
};

