
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



rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_gnss_device_info = 
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
      0,  /* DEVICE_MODULE_TYPE_INSTANCE */
      0,  /* PHY_PATH_NUM */
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


rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_gnss_sig_cfg = 
{
  RFC_ENCODED_REVISION, 
  {
    { (int)RFC_SIG_LIST_END,   { RFC_LOW, 0 }, {RFC_LOW, 0 } }
  },
};


