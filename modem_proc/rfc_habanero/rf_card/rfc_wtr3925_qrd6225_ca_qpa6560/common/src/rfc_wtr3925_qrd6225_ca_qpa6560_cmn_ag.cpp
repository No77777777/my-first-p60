
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
#include "rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag.h" 
#include "DDITlmm.h" 
#include "rfc_common.h" 
#include "rfcommon_core_sbi.h" 

#ifdef FEATURE_GSM
#include "rfc_wtr3925_qrd6225_ca_qpa6560_gsm_config_ag.h" 
#endif 

#ifdef FEATURE_CDMA1X
#include "rfc_wtr3925_qrd6225_ca_qpa6560_cdma_config_ag.h" 
#endif 

#ifdef FEATURE_WCDMA
#include "rfc_wtr3925_qrd6225_ca_qpa6560_wcdma_config_ag.h" 
#endif 

#ifdef FEATURE_LTE
#include "rfc_wtr3925_qrd6225_ca_qpa6560_lte_config_ag.h" 
#endif 

#ifdef FEATURE_TDSCDMA
#include "rfc_wtr3925_qrd6225_ca_qpa6560_tdscdma_config_ag.h" 
#endif 

#ifdef FEATURE_CGPS
#include "rfc_wtr3925_qrd6225_ca_qpa6560_gnss_config_ag.h" 
#endif 



rfc_phy_device_info_type rfc_wtr3925_qrd6225_ca_qpa6560_phy_devices_list[] = 
{
  { /*Device: WTR3925 */ 
    WTR3925, /* PHY_DEVICE_NAME */ 
    0, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    4,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0xC0, /* PHY_DEVICE_PRODUCT_ID */ 
    0, /* PHY_DEVICE_PRODUCT_REV */ 
    0x1, /* DEFAULT USID RANGE START */
    0x1, /* DEFAULT USID RANGE END */
    0x1, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_TX_MODEM_CHAIN_0, /* ASSOCIATED_DAC */ 
  }, /* END - Device: WTR3925 */ 

  { /*Device: QET5102 */ 
    QET5102, /* PHY_DEVICE_NAME */ 
    1, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x36, /* PHY_DEVICE_PRODUCT_ID */ 
    ( 0x00 | RFC_SKIP_RFFE_DETECT_BIT_IND ), /* PHY_DEVICE_PRODUCT_REV */ 
    0x04, /* DEFAULT USID RANGE START */
    0x04, /* DEFAULT USID RANGE END */
    0x04, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    TRUE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QET5102 */ 

  { /*Device: QPA6560 */ 
    QPA6560, /* PHY_DEVICE_NAME */ 
    2, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x123, /* PHY_DEVICE_PRODUCT_ID */ 
    ( 0x00 | RFC_SKIP_RFFE_DETECT_BIT_IND ), /* PHY_DEVICE_PRODUCT_REV */ 
    0xC, /* DEFAULT USID RANGE START */
    0xC, /* DEFAULT USID RANGE END */
    0xC, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    TRUE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QPA6560 */ 

  { /*Device: QPA6560_HB */ 
    QPA6560_HB, /* PHY_DEVICE_NAME */ 
    3, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x124, /* PHY_DEVICE_PRODUCT_ID */ 
    ( 0x00 | RFC_SKIP_RFFE_DETECT_BIT_IND ), /* PHY_DEVICE_PRODUCT_REV */ 
    0xD, /* DEFAULT USID RANGE START */
    0xD, /* DEFAULT USID RANGE END */
    0xD, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    TRUE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QPA6560_HB */ 

  { /*Device: QSW8574 */ 
    QSW8574, /* PHY_DEVICE_NAME */ 
    4, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x73, /* PHY_DEVICE_PRODUCT_ID */ 
    0x02, /* PHY_DEVICE_PRODUCT_REV */ 
    0x0A, /* DEFAULT USID RANGE START */
    0x0A, /* DEFAULT USID RANGE END */
    0x0A, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QSW8574 */ 

  { /*Device: HB_ASM_SP8T */ 
    GEN_ASM /* HB_ASM_SP8T */, /* PHY_DEVICE_NAME */ 
    5, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    1,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x020C, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0xB8, /* PHY_DEVICE_PRODUCT_ID */ 
    0x00, /* PHY_DEVICE_PRODUCT_REV */ 
    0x9, /* DEFAULT USID RANGE START */
    0x9, /* DEFAULT USID RANGE END */
    0x9, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: HB_ASM_SP8T */ 

  { /*Device: LMB_ASDIV_DPDT */ 
    GEN_ASM_XSW /* LMB_ASDIV_DPDT */, /* PHY_DEVICE_NAME */ 
    6, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2_BURST, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    2,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x1B0, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x1F5B, /* PHY_DEVICE_PRODUCT_ID */ 
    0, /* PHY_DEVICE_PRODUCT_REV */ 
    0x0B, /* DEFAULT USID RANGE START */
    0x0B, /* DEFAULT USID RANGE END */
    0x0B, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: LMB_ASDIV_DPDT */ 

  { /*Device: HB_ASDIV_DPDT */ 
    GEN_ASM_XSW /* HB_ASDIV_DPDT */, /* PHY_DEVICE_NAME */ 
    7, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2_BURST, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    2,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x1B0, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x1F5B, /* PHY_DEVICE_PRODUCT_ID */ 
    0, /* PHY_DEVICE_PRODUCT_REV */ 
    0x0A, /* DEFAULT USID RANGE START */
    0x0A, /* DEFAULT USID RANGE END */
    0x0A, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: HB_ASDIV_DPDT */ 

  { /*Device: HB_DRX_SP6T */ 
    GEN_ASM /* HB_DRX_SP6T */, /* PHY_DEVICE_NAME */ 
    8, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_DEFAULT, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    3,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x020C, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0xB8, /* PHY_DEVICE_PRODUCT_ID */ 
    0x01, /* PHY_DEVICE_PRODUCT_REV */ 
    0x09, /* DEFAULT USID RANGE START */
    0x09, /* DEFAULT USID RANGE END */
    0x9, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: HB_DRX_SP6T */ 

  { /*Device: RSW8509 */ 
    GEN_ASM /* RSW8509 */, /* PHY_DEVICE_NAME */ 
    9, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2_BURST, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    3,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x49A, /* PHY_DEVICE_PRODUCT_ID */ 
    0x10, /* PHY_DEVICE_PRODUCT_REV */ 
    0x0B, /* DEFAULT USID RANGE START */
    0x0B, /* DEFAULT USID RANGE END */
    0x0D, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: RSW8509 */ 

  { /*Device: FBRX_SP3T */ 
    GEN_ASM /* FBRX_SP3T */, /* PHY_DEVICE_NAME */ 
    10, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2_BURST, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    3,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x01B0, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x3660, /* PHY_DEVICE_PRODUCT_ID */ 
    0, /* PHY_DEVICE_PRODUCT_REV */ 
    0x0A, /* DEFAULT USID RANGE START */
    0x0A, /* DEFAULT USID RANGE END */
    0x0A, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: FBRX_SP3T */ 

  { /*Device: QAT3555 */ 
    QAT3555, /* PHY_DEVICE_NAME */ 
    11, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2_BURST, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    2,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x687, /* PHY_DEVICE_PRODUCT_ID */ 
    ( 0x00 | RFC_SKIP_RFFE_DETECT_BIT_IND ), /* PHY_DEVICE_PRODUCT_REV */ 
    0x7, /* DEFAULT USID RANGE START */
    0x7, /* DEFAULT USID RANGE END */
    0x7, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    FALSE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QAT3555 */ 

  { /*Device: QAT5515 */ 
    QAT5515, /* PHY_DEVICE_NAME */ 
    12, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2_BURST, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    2,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x68A, /* PHY_DEVICE_PRODUCT_ID */ 
    ( 0x00 | RFC_SKIP_RFFE_DETECT_BIT_IND ), /* PHY_DEVICE_PRODUCT_REV */ 
    0x2, /* DEFAULT USID RANGE START */
    0x2, /* DEFAULT USID RANGE END */
    0x2, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    TRUE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QAT5515 */ 

  { /*Device: QAT5515 */ 
    QAT5515, /* PHY_DEVICE_NAME */ 
    13, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_RFFE, /* PHY_DEVICE_COMM_PROTOCOL */ 
    RFDEVICE_COMM_PROTO_VERSION_2_BURST, /* PHY_DEVICE_COMM_PROTOCOL_VERSION */ 
    {    2,0 /* 0 not specified */,}, /* PHY_DEVICE_COMM_BUS */ 
    0x217, /* PHY_DEVICE_MANUFACTURER_ID */ 
    0x68A, /* PHY_DEVICE_PRODUCT_ID */ 
    ( 0x00 | RFC_SKIP_RFFE_DETECT_BIT_IND ), /* PHY_DEVICE_PRODUCT_REV */ 
    0x1, /* DEFAULT USID RANGE START */
    0x1, /* DEFAULT USID RANGE END */
    0x1, /* PHY_DEVICE_ASSIGNED_USID */ 
    0 /*Warning: Not specified*/, /* RFFE_GROUP_ID */ 
    TRUE, /* INIT */ 
    RFC_INVALID_PARAM, /* ASSOCIATED_DAC */ 
  }, /* END - Device: QAT5515 */ 

  { /*Device: TEST_GRFC_COUPLER_1 */ 
    GEN_COUPLER /* TEST_GRFC_COUPLER_1 */, /* PHY_DEVICE_NAME */ 
    14, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_GRFC, /* PHY_DEVICE_COMM_PROTOCOL */ 
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
  }, /* END - Device: TEST_GRFC_COUPLER_1 */ 

  { /*Device: TEST_GRFC_COUPLER_2 */ 
    GEN_COUPLER /* TEST_GRFC_COUPLER_2 */, /* PHY_DEVICE_NAME */ 
    15, /* PHY_DEVICE_INSTANCE */ 
    RFC_NO_ALTERNATE_PART, /* PHY_DEVICE_ALT_PART_NUM_OF_INSTANCE */ 
    RFDEVICE_COMM_PROTO_GRFC, /* PHY_DEVICE_COMM_PROTOCOL */ 
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
  }, /* END - Device: TEST_GRFC_COUPLER_2 */ 

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


rfc_logical_device_info_type rfc_wtr3925_qrd6225_ca_qpa6560_logical_devices_list[] = 
{
  { /*Device: WTR3925 */ 
    RFDEVICE_TRANSCEIVER, /* DEVICE_MODULE_TYPE */ 
    WTR3925, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    0, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: WTR3925 */ 

  { /*Device: QET5102 */ 
    RFDEVICE_PAPM, /* DEVICE_MODULE_TYPE */ 
    QET5102, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    1, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QET5102 */ 

  { /*Device: QPA6560 */ 
    RFDEVICE_PA, /* DEVICE_MODULE_TYPE */ 
    QPA6560, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    2, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QPA6560 */ 

  { /*Device: QPA6560 */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    QPA6560, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    2, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QPA6560 */ 

  { /*Device: QPA6560_HB */ 
    RFDEVICE_PA, /* DEVICE_MODULE_TYPE */ 
    QPA6560_HB, /* DEVICE_MODULE_NAME */ 
    1, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    3, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QPA6560_HB */ 

  { /*Device: QPA6560_GSM */ 
    RFDEVICE_PA, /* DEVICE_MODULE_TYPE */ 
    QPA6560_GSM, /* DEVICE_MODULE_NAME */ 
    2, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    2, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QPA6560_GSM */ 

  { /*Device: QPA6560_HB */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    QPA6560_HB, /* DEVICE_MODULE_NAME */ 
    1, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    3, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QPA6560_HB */ 

  { /*Device: QSW8574_LB */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    QSW8574_LB, /* DEVICE_MODULE_NAME */ 
    2, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    4, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QSW8574_LB */ 

  { /*Device: QSW8574_MB */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    QSW8574_MB, /* DEVICE_MODULE_NAME */ 
    3, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    4, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QSW8574_MB */ 

  { /*Device: HB_ASM_SP8T */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    GEN_ASM /* HB_ASM_SP8T */, /* DEVICE_MODULE_NAME */ 
    4, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    5, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: HB_ASM_SP8T */ 

  { /*Device: HB_DRX_SP6T */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    GEN_ASM /* HB_DRX_SP6T */, /* DEVICE_MODULE_NAME */ 
    5, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    8, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: HB_DRX_SP6T */ 

  { /*Device: RSW8509 */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    GEN_ASM /* RSW8509 */, /* DEVICE_MODULE_NAME */ 
    6, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    9, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: RSW8509 */ 

  { /*Device: FBRX_SP3T */ 
    RFDEVICE_ASM, /* DEVICE_MODULE_TYPE */ 
    GEN_ASM /* FBRX_SP3T */, /* DEVICE_MODULE_NAME */ 
    7, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    10, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: FBRX_SP3T */ 

  { /*Device: LMB_ASDIV_DPDT */ 
    RFDEVICE_XSW, /* DEVICE_MODULE_TYPE */ 
    GEN_ASM_XSW /* LMB_ASDIV_DPDT */, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    6, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: LMB_ASDIV_DPDT */ 

  { /*Device: HB_ASDIV_DPDT */ 
    RFDEVICE_XSW, /* DEVICE_MODULE_TYPE */ 
    GEN_ASM_XSW /* HB_ASDIV_DPDT */, /* DEVICE_MODULE_NAME */ 
    1, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    7, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: HB_ASDIV_DPDT */ 

  { /*Device: QAT3555 */ 
    RFDEVICE_TUNER, /* DEVICE_MODULE_TYPE */ 
    QAT3555, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    11, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QAT3555 */ 

  { /*Device: QAT5515 */ 
    RFDEVICE_TUNER, /* DEVICE_MODULE_TYPE */ 
    QAT5515, /* DEVICE_MODULE_NAME */ 
    1, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    12, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QAT5515 */ 

  { /*Device: QAT5515 */ 
    RFDEVICE_TUNER, /* DEVICE_MODULE_TYPE */ 
    QAT5515, /* DEVICE_MODULE_NAME */ 
    2, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    13, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: QAT5515 */ 

  { /*Device: TEST_GRFC_COUPLER_1 */ 
    RFDEVICE_COUPLER, /* DEVICE_MODULE_TYPE */ 
    GEN_COUPLER /* TEST_GRFC_COUPLER_1 */, /* DEVICE_MODULE_NAME */ 
    0, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    14, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: TEST_GRFC_COUPLER_1 */ 

  { /*Device: TEST_GRFC_COUPLER_2 */ 
    RFDEVICE_COUPLER, /* DEVICE_MODULE_TYPE */ 
    GEN_COUPLER /* TEST_GRFC_COUPLER_2 */, /* DEVICE_MODULE_NAME */ 
    1, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    15, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: TEST_GRFC_COUPLER_2 */ 

  { /*Device: RFDEVICE_INVALID */ 
    RFDEVICE_TYPE_INVALID, /* DEVICE_MODULE_TYPE */ 
    RFDEVICE_INVALID, /* DEVICE_MODULE_NAME */ 
    0 /*Warning: Not specified*/, /* DEVICE_MODULE_TYPE_INSTANCE */ 
    0 /*Warning: Not specified*/, /* ASSOCIATED_PHY_DEVICE_INSTANCE */ 
  }, /* END - Device: RFDEVICE_INVALID */ 

};


rfc_signal_info_type rfc_wtr3925_qrd6225_ca_qpa6560_sig_info[RFC_WTR3925_QRD6225_CA_QPA6560_SIG_NUM + 1] = 
{
  { RFC_MSM_TIMING_PA_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PA_CTL */ 
  { RFC_MSM_TIMING_PA_RANGE , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PA_RANGE */ 
  { RFC_MSM_TIMING_ASM_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_ASM_CTL */ 
  { RFC_MSM_TIMING_TUNER_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_TUNER_CTL */ 
  { RFC_MSM_TIMING_PAPM_CTL , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PAPM_CTL */ 
  { RFC_MSM_TIMING_TX_TX_RF_ON0 , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_TX_TX_RF_ON0 */ 
  { RFC_MSM_TIMING_TX_RX_RF_ON0 , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_TX_RX_RF_ON0 */ 
  { RFC_MSM_TIMING_ASM_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_ASM_TRIGGER */ 
  { RFC_MSM_TIMING_PAPM_TX_TX_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PAPM_TX_TX_TRIGGER */ 
  { RFC_MSM_TIMING_PAPM_OFF_TX_RX_TX_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PAPM_OFF_TX_RX_TX_TRIGGER */ 
  { RFC_MSM_TIMING_PA_TRIGGER , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PA_TRIGGER */ 
  { RFC_MSM_TIMING_PAPM_OFF_TX_RX_TX_CTL , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PAPM_OFF_TX_RX_TX_CTL */ 
  { RFC_MSM_TIMING_PAPM_TX_TX_CTL , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PAPM_TX_TX_CTL */ 
  { RFC_MSM_TIMING_PAPM_MULTISLOT_CTL , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TIMING_PAPM_MULTISLOT_CTL */ 
  { RFC_MSM_PA_ON_10 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_PA_ON_10 */ 
  { RFC_MSM_PA1_R2 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_PA1_R2 */ 
  { RFC_MSM_PA0_R1 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_PA0_R1 */ 
  { RFC_MSM_RF_PATH_SEL_01 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_01 */ 
  { RFC_MSM_RF_PATH_SEL_09 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_09 */ 
  { RFC_MSM_RF_PATH_SEL_02 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_02 */ 
  { RFC_MSM_RF_PATH_SEL_11 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_11 */ 
  { RFC_MSM_RF_PATH_SEL_04 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_04 */ 
  { RFC_MSM_RF_PATH_SEL_17 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_17 */ 
  { RFC_MSM_RF_PATH_SEL_15 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_15 */ 
  { RFC_MSM_RF_PATH_SEL_05 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_05 */ 
  { RFC_MSM_RF_PATH_SEL_20 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_20 */ 
  { RFC_MSM_RF_PATH_SEL_06 , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_06 */ 
  { RFC_MSM_RF_PATH_SEL_14 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_14 */ 
  { RFC_MSM_RF_PATH_SEL_07 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_07 */ 
  { RFC_MSM_RF_PATH_SEL_16 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_16 */ 
  { RFC_MSM_RF_PATH_SEL_08 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_08 */ 
  { RFC_MSM_RF_PATH_SEL_21 , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RF_PATH_SEL_21 */ 
  { RFC_MSM_GPDATA0_0 , RFC_CONFIG_ONLY, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_GPDATA0_0 */ 
  { RFC_MSM_RFFE1_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE1_CLK */ 
  { RFC_MSM_RFFE1_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE1_DATA */ 
  { RFC_MSM_RFFE2_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE2_CLK */ 
  { RFC_MSM_RFFE2_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE2_DATA */ 
  { RFC_MSM_RFFE3_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_4MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE3_CLK */ 
  { RFC_MSM_RFFE3_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_4MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE3_DATA */ 
  { RFC_MSM_RFFE4_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE4_CLK */ 
  { RFC_MSM_RFFE4_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE4_DATA */ 
  { RFC_MSM_RFFE5_CLK , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE5_CLK */ 
  { RFC_MSM_RFFE5_DATA , RFC_CONFIG_ONLY, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_RFFE5_DATA */ 
  { RFC_MSM_INTERNAL_GNSS_BLANK , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_INTERNAL_GNSS_BLANK */ 
  { RFC_MSM_INTERNAL_GNSS_BLANK_CONCURRENCY , RFC_LOW, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_INTERNAL_GNSS_BLANK_CONCURRENCY */ 
  { RFC_MSM_TX_GTR_TH , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_TX_GTR_TH */ 
  { RFC_MSM_PA_IND , RFC_LOW, DAL_GPIO_PULL_DOWN, DAL_GPIO_2MA, (DALGpioIdType)NULL }, /* RFC_WTR3925_QRD6225_CA_QPA6560_PA_IND */ 
  { (rfc_msm_signal_type)RFC_SIG_LIST_END   , (rfc_logic_type)RFC_ENCODED_REVISION, DAL_GPIO_NO_PULL, DAL_GPIO_2MA, (DALGpioIdType)NULL } /* LAST SIG INDICATOR */ 
};


rfc_common_data* rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::get_instance(rf_hw_type rf_hw)
{
  if (rfc_common_data_ptr == NULL)
  {
    rfc_common_data_ptr = (rfc_common_data *)new rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag(rf_hw);
  }
  return( (rfc_common_data *)rfc_common_data_ptr);
}

//constructor
rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag(rf_hw_type rf_hw)
  :rfc_common_data(rf_hw)
{
}


uint32 rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::sig_info_table_get(rfc_signal_info_type **rfc_info_table)
{
  if (NULL == rfc_info_table)
  {
    return 0;
  }

  *rfc_info_table = &rfc_wtr3925_qrd6225_ca_qpa6560_sig_info[0];

#ifdef FEATURE_GSM
  // Create GSM RFC AG Data Object
  rfc_gsm_data *rfc_gsm_data = rfc_wtr3925_qrd6225_ca_qpa6560_gsm_ag::get_instance(); 
  if (rfc_gsm_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR GSM Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_GSM */

#ifdef FEATURE_CDMA1X
  // Create CDMA RFC AG Data Object
  rfc_cdma_data *rfc_cdma_data = rfc_wtr3925_qrd6225_ca_qpa6560_cdma_ag::get_instance(); 
  if (rfc_cdma_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR CDMA Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_CDMA */

#ifdef FEATURE_WCDMA
  // Create WCDMA RFC AG Data Object
  rfc_wcdma_data *rfc_wcdma_data = rfc_wtr3925_qrd6225_ca_qpa6560_wcdma_ag::get_instance(); 
  if (rfc_wcdma_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR WCDMA Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_LTE
  // Create LTE RFC AG Data Object
  rfc_lte_data *rfc_lte_data = rfc_wtr3925_qrd6225_ca_qpa6560_lte_ag::get_instance(); 
  if (rfc_lte_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR LTE Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_LTE */

#ifdef FEATURE_TDSCDMA
  // Create TDSCDMA RFC AG Data Object
  rfc_tdscdma_data *rfc_tdscdma_data = rfc_wtr3925_qrd6225_ca_qpa6560_tdscdma_ag::get_instance(); 
  if (rfc_tdscdma_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR TDSCDMA Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_TDSCDMA */

#ifdef FEATURE_CGPS
  // Create GNSS RFC AG Data Object
  rfc_gnss_data *rfc_gnss_data = rfc_wtr3925_qrd6225_ca_qpa6560_gnss_ag::get_instance(); 
  if (rfc_gnss_data == NULL)
  {
    RF_MSG_1(RF_ERROR,"RFC ERROR GNSS Data Object is NOT Created for HWID %d. Cannot operate in this tech.", m_rf_hw);
    return 0;
}
#endif /* FEATURE_GNSS */

  return RFC_WTR3925_QRD6225_CA_QPA6560_SIG_NUM;
}

rfc_phy_device_info_type* rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::get_phy_device_cfg( void )
{
  return (&rfc_wtr3925_qrd6225_ca_qpa6560_phy_devices_list[0]);
}

rfc_logical_device_info_type* rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::get_logical_device_cfg( void )
{
  return (&rfc_wtr3925_qrd6225_ca_qpa6560_logical_devices_list[0]);
}


rfm_devices_configuration_type rfc_wtr3925_qrd6225_ca_qpa6560_logical_device_properties = 
{
  904, /* HWID */
  { /* device_support */ 
    { /* Logical Device 0 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND32 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) ),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND32 - 128) ) ),
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
      RFM_DEVICE_4, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_0 (Valid for Rx Devs only)*/
    }, /* End Logical Device 0 */ 
    { /* Logical Device 1 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND32 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) ),
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
      RFM_DEVICE_4, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_1 (Valid for Rx Devs only)*/
    }, /* End Logical Device 1 */ 
    { /* Logical Device 2 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND32 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDA - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDF - 128) ) ),
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
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDA - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDF - 128) ) ),
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
      RFM_DEVICE_3, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_2 (Valid for Rx Devs only)*/
      RFM_DEVICE_4, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_2 (Valid for Rx Devs only)*/
    }, /* End Logical Device 2 */ 
    { /* Logical Device 3 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          (0),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND32 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDA - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDF - 128) ) ),
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
      RFM_DEVICE_2, /* VCO Based Preferred Associated Rx Device for RFM_DEVICE_3 (Valid for Rx Devs only)*/
      RFM_DEVICE_4, /* WTR Based Preferred Associated Tx Device for RFM_DEVICE_3 (Valid for Rx Devs only)*/
    }, /* End Logical Device 3 */ 
    { /* Logical Device 4 */ 
      /* bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDA - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDF - 128) ) ),
        },
      },
      /* preferred_bands_supported */ 
      {
        {
          /* Bit mask element 0 */ 
          ( ( (uint64)1 << (SYS_BAND_GSM_850 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_EGSM_900 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_DCS_1800 - 0) ) | ( (uint64)1 << (SYS_BAND_GSM_PCS_1900 - 0) ) | ( (uint64)1 << (SYS_BAND_BC0 - 0) ) ),
          /* Bit mask element 1 */ 
          ( ( (uint64)1 << (SYS_BAND_WCDMA_I_IMT_2000 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_II_PCS_1900 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_V_850 - 64) ) | ( (uint64)1 << (SYS_BAND_WCDMA_VIII_900 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND1 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND2 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND3 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND5 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND7 - 64) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND8 - 64) ) ),
          /* Bit mask element 2 */ 
          ( ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND20 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND28 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND34 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND38 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND39 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND40 - 128) ) | ( (uint64)1 << (SYS_BAND_LTE_EUTRAN_BAND41 - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDA - 128) ) | ( (uint64)1 << (SYS_BAND_TDS_BANDF - 128) ) ),
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
  TRUE, /* antenna_swap_supported */ 
  0, /* concurrency_features */ 
};

boolean rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::get_logical_path_config(rfm_devices_configuration_type* dev_cfg)
{
  if ( dev_cfg == NULL )
  {
    RF_MSG_1(RF_ERROR, "rfm_get_devices_configuration: Invalid Container", 0);
    return FALSE;
  }

  memscpy(dev_cfg,
          sizeof(rfm_devices_configuration_type),
          &rfc_wtr3925_qrd6225_ca_qpa6560_logical_device_properties,
          sizeof(rfm_devices_configuration_type));

  return TRUE;
}

const rfm_devices_configuration_type* rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::get_logical_device_properties( void )
{
  return &rfc_wtr3925_qrd6225_ca_qpa6560_logical_device_properties;
}



rfc_cmn_properties_type rfc_wtr3925_qrd6225_ca_qpa6560_cmn_properties = 
{
  RFC_ENCODED_REVISION,   
  RFC_ATTENUATION_STATE_MID,
};

boolean rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::get_cmn_properties(rfc_cmn_properties_type **ptr)
{
  if (NULL==ptr)
  {
    return FALSE;
  }

  *ptr = &rfc_wtr3925_qrd6225_ca_qpa6560_cmn_properties;
  return TRUE;
}

boolean rfc_wtr3925_qrd6225_ca_qpa6560_cmn_ag::rfc_get_remapped_device_info
(
  rfc_cal_device_remap_info_type *source_device_info,
  rfc_cal_device_remap_info_type *remapped_device_info
)
{
  return TRUE;
}