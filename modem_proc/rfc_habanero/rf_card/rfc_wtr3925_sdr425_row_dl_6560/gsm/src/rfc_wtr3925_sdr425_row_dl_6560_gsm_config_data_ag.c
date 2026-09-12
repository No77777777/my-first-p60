
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

#include "rfc_wtr3925_sdr425_row_dl_6560_cmn_ag.h" 
#include "rfc_common.h" 
#include "rfcom.h" 
#include "wtr3925_typedef_ag.h" 
#include "qpa6560_port_typedef_ag.h" 
#include "qpa6560_gsm_port_typedef_ag.h" 
#include "qpa6560_hb_port_typedef_ag.h" 
#include "rfdevice_qtuner_api.h" 
#include "rfdevice_grfc_coupler.h" 



rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_gsm_rx_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  RFC_INVALID_PARAM /* Warning: Not Specified */,   /* Antenna */
  1,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0 /*Warning: Not specified*/,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        0  /* Invalid TRx port*/ ,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_gsm_rx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_gsm_tx_device_info = 
{
  RFC_ENCODED_REVISION, 
  0 /* Warning: Not Specified */,   /* Modem Chain */
  0 /* Warning: Not Specified */,   /* NV Container */
  RFC_INVALID_PARAM /* Warning: Not Specified */,   /* Antenna */
  1,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0 /*Warning: Not specified*/,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        0  /* Invalid TRx port*/ ,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_gsm_tx_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g850_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  5,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_PRXLGY1_BAND850_PLB4,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_LB,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (5)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* LB_PRX_SP4T_2 */,  /* NAME */
      7,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x020C << 22)/*mfg_id*/ | (0x98 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        (0x01B0 << 22)/*mfg_id*/ | (0x77 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g850_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g850_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  5,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_DRXLGY1_BAND850_DLB4,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* RSW8509 */,  /* NAME */
      12,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x9A << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* LB_DRX_SP4T */,  /* NAME */
      10,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x1B0 << 22)/*mfg_id*/ | (0x6A << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        (0x1B0 << 22)/*mfg_id*/ | (0x77 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g850_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g850_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  8,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_TX_BAND850_TLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      QPA6560_GSM,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (QPA6560_GSM_GSM_BAND850_PORT_GSM_LB)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QPA6560,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* FBRX_SP6T */,  /* NAME */
      13,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x020C << 22)/*mfg_id*/ | (0x83 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_LB,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_COUPLER,
      GEN_COUPLER /* TEST_GRFC_COUPLER */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /* Orig setting:  */,  /* INTF_REV */
        RFDEVICE_GRFC_COUPLER_SIG_ID_OPEN_FWD_REV_LOGIC(RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04, RFC_LOW/*OPEN*/, RFC_LOW/*FWD*/, RFC_HIGH/*REV*/ ),  /* GRFC_CONFIG_0 */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g850_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -3 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_PA_IND,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_PRXLGY1_BAND900_PLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_LB,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_DRXLGY1_BAND900_DLB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* RSW8509 */,  /* NAME */
      12,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x9A << 14)/*prd_id*/ | (4)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  8,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_TX_BAND900_TLB1,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      QPA6560_GSM,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (QPA6560_GSM_GSM_BAND900_PORT_GSM_LB)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QPA6560,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (12)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* FBRX_SP6T */,  /* NAME */
      13,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x020C << 22)/*mfg_id*/ | (0x83 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_LB,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (6)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_COUPLER,
      GEN_COUPLER /* TEST_GRFC_COUPLER */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /* Orig setting:  */,  /* INTF_REV */
        RFDEVICE_GRFC_COUPLER_SIG_ID_OPEN_FWD_REV_LOGIC(RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04, RFC_LOW/*OPEN*/, RFC_LOW/*FWD*/, RFC_HIGH/*REV*/ ),  /* GRFC_CONFIG_0 */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -3 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_PA_IND,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g1800_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_PRXLGY1_BAND1800_PMB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_MB,  /* NAME */
      3,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g1800_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g1800_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_DRXLGY1_BAND1800_DMB3,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* RSW8509 */,  /* NAME */
      12,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x9A << 14)/*prd_id*/ | (9)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g1800_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g1800_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  9,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_TX_BAND1800_THMB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      QPA6560_GSM,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (QPA6560_GSM_GSM_BAND1800_PORT_GSM_MB)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PAPM,
      QET5102,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x36 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QPA6560,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (13)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* FBRX_SP6T */,  /* NAME */
      13,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x020C << 22)/*mfg_id*/ | (0x83 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_MB,  /* NAME */
      3,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_COUPLER,
      GEN_COUPLER /* TEST_GRFC_COUPLER */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /* Orig setting:  */,  /* INTF_REV */
        RFDEVICE_GRFC_COUPLER_SIG_ID_OPEN_FWD_REV_LOGIC(RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04, RFC_LOW/*OPEN*/, RFC_LOW/*FWD*/, RFC_HIGH/*REV*/ ),  /* GRFC_CONFIG_0 */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g1800_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -3 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_PA_IND,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g1900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_PRXLGY1_BAND1900_PMB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_MB,  /* NAME */
      3,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (0)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_gsm_g1900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g1900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_RX_MODEM_CHAIN_1,   /* Modem Chain */
  1,   /* NV Container */
  1,   /* Antenna */
  4,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      1,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_DRXLGY1_BAND1900_DMB1,  /* PORT */
        ( RFDEVICE_RX_GAIN_STATE_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_ATTN_DEFAULT,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* RSW8509 */,  /* NAME */
      12,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x9A << 14)/*prd_id*/ | (11)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      1,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_gsm_g1900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -40 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g1900_device_info = 
{
  RFC_ENCODED_REVISION, 
  RFC_TX_MODEM_CHAIN_0,   /* Modem Chain */
  0,   /* NV Container */
  0,   /* Antenna */
  9,  /* NUM_DEVICES_TO_CONFIGURE */
  {
    {
      RFDEVICE_TRANSCEIVER,
      WTR3925,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
      {
        0 /*Warning: Not specified*/,  /* INTF_REV */
        (int)WTR3925_GSM_TX_BAND1900_THMB4,  /* PORT */
        ( RFDEVICE_PA_LUT_MAPPING_INVALID ),  /* RF_ASIC_BAND_AGC_LUT_MAPPING */
        FALSE,  /* TXAGC_LUT */
        WTR3925_FBRX_LOW_ATTN_MODE,  /* FBRX_ATTN_STATE */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PA,
      QPA6560_GSM,  /* NAME */
      2,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (QPA6560_GSM_GSM_BAND1900_PORT_GSM_MB)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_PAPM,
      QET5102,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x36 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QPA6560,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x23 << 14)/*prd_id*/ | (13)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      GEN_ASM /* FBRX_SP6T */,  /* NAME */
      13,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x020C << 22)/*mfg_id*/ | (0x83 << 14)/*prd_id*/ | (1)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_ASM,
      QSW8574_MB,  /* NAME */
      3,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (0x217 << 22)/*mfg_id*/ | (0x73 << 14)/*prd_id*/ | (8)/*port_num*/,  /* PORT_NUM */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_TUNER,
      QAT3555,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0  /* Orig setting:  */,  /* INTF_REV */
        (int)RFDEVICE_QTUNER_IM_DISTORTION_INVALID,  /* DISTORTION_CONFIG */
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting:  */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
    {
      RFDEVICE_COUPLER,
      GEN_COUPLER /* TEST_GRFC_COUPLER */,  /* NAME */
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0 /*Warning: Not specified*/,  /* PHY_PATH_NUM */
      {
        0 /* Orig setting:  */,  /* INTF_REV */
        RFDEVICE_GRFC_COUPLER_SIG_ID_OPEN_FWD_REV_LOGIC(RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04, RFC_LOW/*OPEN*/, RFC_LOW/*FWD*/, RFC_HIGH/*REV*/ ),  /* GRFC_CONFIG_0 */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_gsm_g1900_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PA_RANGE,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -3 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_ASM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_TUNER_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -150 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -100 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TIMING_PAPM_MULTISLOT_CTL,   { RFC_CONFIG_ONLY  /*Warning: Not specified*/ , -6 }, {RFC_CONFIG_ONLY  /*Warning: Not specified*/ , 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_04,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_RF_PATH_SEL_05,   { RFC_LOW, 0/*Warning: Not specified*/ }, {RFC_LOW, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_GPDATA0_0,   { RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }, {RFC_CONFIG_ONLY, 0/*Warning: Not specified*/ }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_INTERNAL_GNSS_BLANK,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_TX_GTR_TH,   { RFC_CONFIG_ONLY, -10 }, {RFC_LOW, 66 }  },
    { (int)RFC_WTR3925_SDR425_ROW_DL_6560_PA_IND,   { RFC_HIGH, -10 }, {RFC_LOW, 0 }  },
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_gsm_en_ant_swap4_device_info = 
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
        0  /* Orig setting: MB/LB PRX */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting: MB/LB PRX */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting: MB/LB PRX */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_gsm_en_ant_swap4_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_gsm_dis_ant_swap4_device_info = 
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
        0  /* Orig setting: MB/LB PRX */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting: MB/LB PRX */,  /* NOTES (REFERENCE ONLY) */
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
        0  /* Orig setting: MB/LB PRX */,  /* NOTES (REFERENCE ONLY) */
        0,  /* Array Filler */
        0,  /* Array Filler */
        0,  /* Array Filler */
      },
    },
  },
};


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_gsm_dis_ant_swap4_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


rfc_gsm_properties_type rfc_wtr3925_sdr425_row_dl_6560_gsm_properties = 
{
  RFC_ENCODED_REVISION, 
  /* GSM_DESENSE_CHANNEL (Max: 25 per band) */
  {
    { (uint32)RFCOM_BAND_GSM850 , 198 },
    { (uint32)RFCOM_BAND_GSM900 , 29 },
    { (uint32)RFCOM_BAND_GSM1800 , 702 },
    { (uint32)RFCOM_BAND_GSM1800 , 861 },
    { (uint32)RFCOM_NUM_GSM_BANDS , RFC_INVALID_PARAM },
  },
};

