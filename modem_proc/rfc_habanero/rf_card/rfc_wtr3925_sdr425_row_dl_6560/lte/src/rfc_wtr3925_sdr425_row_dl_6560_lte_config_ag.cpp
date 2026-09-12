
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

#include "rfc_wtr3925_sdr425_row_dl_6560_lte_config_ag.h" 
#include "rfc_wtr3925_sdr425_row_dl_6560_cmn_ag.h" 
#include "rfc_common.h" 
#include "rfcom.h" 



extern "C" 
{
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_lte_rx_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_lte_rx_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_lte_tx_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_init_lte_tx_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b3_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b3_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b3_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b3_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b3_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b3_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b3_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b3_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b3_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b3_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b7_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b7_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b7_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b7_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b7_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b7_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b7_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b7_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b7_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b7_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b11_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b11_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b11_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b11_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b11_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b11_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b11_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b11_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b11_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b11_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b18_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b18_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b18_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b18_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b18_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b18_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b18_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b18_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b18_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b18_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b19_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b19_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b19_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b19_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b19_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b19_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b19_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b19_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b19_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b19_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b20_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b20_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b20_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b20_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b20_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b20_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b20_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b20_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b20_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b20_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b21_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b21_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b21_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b21_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b21_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b21_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b21_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b21_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b21_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b21_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b26_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b26_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b26_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b26_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b26_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b26_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b26_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b26_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b26_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b26_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b27_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b27_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b27_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b27_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b27_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b27_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b27_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b27_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b27_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b27_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_b_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_b_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_b_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_b_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_b_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_b_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_b_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_b_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_b_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_b_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b29_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b29_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b29_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b29_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b29_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b29_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b29_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b29_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b32_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b32_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b32_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b32_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b32_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b32_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b32_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b32_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b34_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b34_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b34_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b34_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b34_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b34_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b34_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b34_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b34_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b34_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b38_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b38_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b38_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b38_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b38_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b38_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b38_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b38_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b38_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b38_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b39_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b39_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b39_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b39_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b39_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b39_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b39_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b39_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b39_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b39_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b40_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b40_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b40_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b40_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b40_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b40_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b40_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b40_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b40_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b40_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b41_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b41_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b41_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b41_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b41_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b41_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b41_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b41_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b41_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b41_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal0_enable_fbrx_enable_xpt_capture_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal0_enable_fbrx_enable_xpt_capture_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal0_disable_fbrx_disable_xpt_capture_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal0_disable_fbrx_disable_xpt_capture_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal1_enable_fbrx_enable_xpt_capture_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal1_enable_fbrx_enable_xpt_capture_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal1_disable_fbrx_disable_xpt_capture_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal1_disable_fbrx_disable_xpt_capture_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_altpath_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_altpath_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_altpath_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_altpath_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_altpath_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_altpath_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_altpath_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_alt_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_alt_sig_cfg;
  extern rfc_device_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_altpath_device_info;
  extern rfc_sig_info_type rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_altpath_sig_cfg;

  extern rfc_timing_info_type rfc_msm_timing_info[RFC_TIMING_PARAMS_NUM];
} /* extern "C" */


rfc_lte_data * rfc_wtr3925_sdr425_row_dl_6560_lte_ag::get_instance()
{
  if (rfc_lte_data_ptr == NULL)
  {
    rfc_lte_data_ptr = (rfc_lte_data *)new rfc_wtr3925_sdr425_row_dl_6560_lte_ag();
  }
  return( (rfc_lte_data *)rfc_lte_data_ptr);
}

//constructor
rfc_wtr3925_sdr425_row_dl_6560_lte_ag::rfc_wtr3925_sdr425_row_dl_6560_lte_ag()
  :rfc_lte_data()
{
}

boolean rfc_wtr3925_sdr425_row_dl_6560_lte_ag::sig_cfg_data_get( rfc_cfg_params_type *cfg, rfc_sig_cfg_type **ptr )
{

  boolean ret_val = FALSE;

  if (NULL==ptr)
  {
    return FALSE;
  }

  if (NULL==cfg)
  {
    *ptr = NULL;
    return FALSE;
  }

  *ptr = NULL;

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_init_lte_rx_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_init_lte_tx_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b3_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b3_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b3_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b3_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b3_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b7_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b7_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b7_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b7_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b7_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b11_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b11_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b11_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b11_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b11_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b18_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b18_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b18_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b18_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b18_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b19_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b19_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b19_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b19_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b19_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b20_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b20_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b20_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b20_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b20_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b21_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b21_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b21_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b21_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b21_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b26_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b26_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b26_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b26_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b26_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b27_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b27_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b27_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b27_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b27_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_b_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_b_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_b_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_b_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_b_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b29_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b29_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b29_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b29_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b32_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b32_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b32_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b32_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b34_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b34_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b34_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b34_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b34_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b38_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b38_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b38_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b38_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b38_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b39_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b39_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b39_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b39_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b39_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b40_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b40_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b40_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b40_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b40_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b41_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b41_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b41_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b41_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b41_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( ( cfg->req == RFC_REQ_ENABLE_FBRX )||  ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal0_enable_fbrx_enable_xpt_capture_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( ( cfg->req == RFC_REQ_DISABLE_FBRX )||  ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal0_disable_fbrx_disable_xpt_capture_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( ( cfg->req == RFC_REQ_ENABLE_FBRX )||  ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal1_enable_fbrx_enable_xpt_capture_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( ( cfg->req == RFC_REQ_DISABLE_FBRX )||  ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal1_disable_fbrx_disable_xpt_capture_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_alt_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_altpath_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if (ret_val == FALSE)
  {
   /* 
 RF_MSG_5( RF_ERROR, "rfc_wtr3925_sdr425_row_dl_6560 RFC detected error. Unsupported params combo: device %d band %d altp %d rxtx %d reqtype %d",
              cfg->logical_device, cfg->band, cfg->alternate_path, cfg->rx_tx, cfg->req );
 */ 
 }

  return ret_val;
}

boolean rfc_wtr3925_sdr425_row_dl_6560_lte_ag::devices_cfg_data_get( rfc_cfg_params_type *cfg, rfc_device_info_type **ptr )
{

  boolean ret_val = FALSE;

  if (NULL==ptr)
  {
    return FALSE;
  }

  if (NULL==cfg)
  {
    *ptr = NULL;
    return FALSE;
  }

  *ptr = NULL;

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_init_lte_rx_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_init_lte_tx_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b3_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b3_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b3_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b3_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b3_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b7_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b7_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b7_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b7_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b7_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b11_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b11_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b11_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b11_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b11_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b18_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b18_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b18_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b18_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b18_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b19_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b19_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b19_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b19_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b19_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b20_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b20_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b20_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b20_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b20_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b21_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b21_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b21_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b21_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b21_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b26_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b26_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b26_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b26_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b26_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b27_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b27_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b27_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b27_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b27_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b28_b_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b28_b_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b28_b_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b28_b_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b28_b_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b29_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b29_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b29_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b29_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b32_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b32_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b32_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b32_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b34_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b34_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b34_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b34_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b34_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b38_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b38_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b38_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b38_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b38_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b39_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b39_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b39_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b39_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b39_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b40_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b40_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b40_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b40_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b40_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_2_lte_b41_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_3_lte_b41_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_0_lte_b41_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_rx_on_rfm_device_1_lte_b41_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_tx_on_rfm_device_4_lte_b41_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( ( cfg->req == RFC_REQ_ENABLE_FBRX )||  ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal0_enable_fbrx_enable_xpt_capture_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( ( cfg->req == RFC_REQ_DISABLE_FBRX )||  ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal0_disable_fbrx_disable_xpt_capture_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( ( cfg->req == RFC_REQ_ENABLE_FBRX )||  ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_et_cal1_enable_fbrx_enable_xpt_capture_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( ( cfg->req == RFC_REQ_DISABLE_FBRX )||  ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_et_cal1_disable_fbrx_disable_xpt_capture_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap0_altpath_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap0_altpath_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap1_altpath_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap1_altpath_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap2_altpath_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap2_altpath_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_en_ant_swap3_altpath_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_alt_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr3925_sdr425_row_dl_6560_lte_dis_ant_swap3_altpath_device_info);  ret_val = TRUE; }

  if (ret_val == FALSE)
  {
   /* 
 RF_MSG_5( RF_ERROR, "rfc_wtr3925_sdr425_row_dl_6560 RFC detected error. Unsupported params combo: device %d band %d altp %d rxtx %d reqtype %d",
              cfg->logical_device, cfg->band, cfg->alternate_path, cfg->rx_tx, cfg->req );
 */ 
 }

  return ret_val;
}

boolean rfc_wtr3925_sdr425_row_dl_6560_lte_ag::timing_cfg_data_get( rfc_cfg_params_type *cfg, rfc_timing_info_type **ptr )
{

  boolean ret_val = FALSE;

  if (NULL==ptr)
  {
    return FALSE;
  }

  if (NULL==cfg)
  {
    *ptr = NULL;
    return FALSE;
  }

  *ptr = NULL;

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B1 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B2 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B3 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B5 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B7 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B11 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B18 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B19 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B20 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B21 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B26 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B27 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B28_B ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B29 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B32 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B34 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B38 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B39 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B40 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_3 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_LTE_B41 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( ( cfg->req == RFC_REQ_ENABLE_FBRX )||  ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( ( cfg->req == RFC_REQ_DISABLE_FBRX )||  ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( ( cfg->req == RFC_REQ_ENABLE_FBRX )||  ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( ( cfg->req == RFC_REQ_DISABLE_FBRX )||  ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE )) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B1 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B2 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B3 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B4 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B5 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B8 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B18 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B19 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B26 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B27 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B7 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B11 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B21 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B20 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B28_B )||  ( cfg->band == (int)RFCOM_BAND_LTE_B34 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B39 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_4 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_LTE_B32 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B38 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B40 )||  ( cfg->band == (int)RFCOM_BAND_LTE_B41 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rfc_msm_timing_info[0]);  ret_val = TRUE; }

  if (ret_val == FALSE)
  {
   /* 
 RF_MSG_5( RF_ERROR, "rfc_wtr3925_sdr425_row_dl_6560 RFC detected error. Unsupported params combo: device %d band %d altp %d rxtx %d reqtype %d",
              cfg->logical_device, cfg->band, cfg->alternate_path, cfg->rx_tx, cfg->req );
 */ 
 }

  return ret_val;
}

boolean rfc_wtr3925_sdr425_row_dl_6560_lte_ag::ca_scell_log_path_swap_data_get( rfc_band_pair_list_type **ptr )
{
  if (NULL==ptr)
  {
    return FALSE;
  }

  *ptr = NULL;

  return TRUE;
}

extern "C" 
{
  extern rfc_band_split_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx2_lte_b28_split_cfg;
  extern rfc_band_split_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx3_lte_b28_split_cfg;
  extern rfc_band_split_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx0_lte_b28_split_cfg;
  extern rfc_band_split_info_type rf_card_wtr3925_sdr425_row_dl_6560_rx1_lte_b28_split_cfg;
  extern rfc_band_split_info_type rf_card_wtr3925_sdr425_row_dl_6560_tx4_lte_b28_split_cfg;
}  /* extern "C" */

boolean rfc_wtr3925_sdr425_row_dl_6560_lte_ag::band_split_cfg_data_get( rfc_cfg_params_type *cfg, rfc_band_split_info_type **ptr )
{
  boolean ret_val = FALSE;

  if (NULL==ptr)
  {
    return FALSE;
  }

  if (NULL==cfg)
  {
    *ptr = NULL;
    return FALSE;
  }

  *ptr = NULL;

  if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_2) && (cfg->band == (int)RFCOM_BAND_LTE_B28) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx2_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_2) && (cfg->band == (int)RFCOM_BAND_LTE_B28_B) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx2_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_3) && (cfg->band == (int)RFCOM_BAND_LTE_B28) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx3_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_3) && (cfg->band == (int)RFCOM_BAND_LTE_B28_B) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx3_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_0) && (cfg->band == (int)RFCOM_BAND_LTE_B28) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx0_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_0) && (cfg->band == (int)RFCOM_BAND_LTE_B28_B) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx0_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_1) && (cfg->band == (int)RFCOM_BAND_LTE_B28) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx1_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_RX) && (cfg->logical_device == RFM_DEVICE_1) && (cfg->band == (int)RFCOM_BAND_LTE_B28_B) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_rx1_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_TX) && (cfg->logical_device == RFM_DEVICE_4) && (cfg->band == (int)RFCOM_BAND_LTE_B28) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_tx4_lte_b28_split_cfg; ret_val = TRUE; }

  else if ( (cfg->rx_tx == RFC_CONFIG_TX) && (cfg->logical_device == RFM_DEVICE_4) && (cfg->band == (int)RFCOM_BAND_LTE_B28_B) )
  { *ptr = &rf_card_wtr3925_sdr425_row_dl_6560_tx4_lte_b28_split_cfg; ret_val = TRUE; }

  return ret_val;
}

