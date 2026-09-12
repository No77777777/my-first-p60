
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

#include "rfc_wtr2965_non_ca_2w_sku2_wcdma_config_ag.h" 
#include "rfc_wtr2965_non_ca_2w_sku2_cmn_ag.h" 
#include "rfc_common.h" 
#include "rfcom.h" 



extern "C" 
{
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_rx_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_rx_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_tx_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_tx_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b1_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b2_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b4_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b4_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b5_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b5_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b8_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b8_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap0_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap0_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap0_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap0_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap1_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap1_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap1_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap2_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap2_sig_cfg;
  extern rfc_device_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap2_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap2_sig_cfg;
} /* extern "C" */


rfc_wcdma_data * rfc_wtr2965_non_ca_2w_sku2_wcdma_ag::get_instance()
{
  if (rfc_wcdma_data_ptr == NULL)
  {
    rfc_wcdma_data_ptr = (rfc_wcdma_data *)new rfc_wtr2965_non_ca_2w_sku2_wcdma_ag();
  }
  return( (rfc_wcdma_data *)rfc_wcdma_data_ptr);
}

//constructor
rfc_wtr2965_non_ca_2w_sku2_wcdma_ag::rfc_wtr2965_non_ca_2w_sku2_wcdma_ag()
  :rfc_wcdma_data()
{
}

boolean rfc_wtr2965_non_ca_2w_sku2_wcdma_ag::sig_cfg_data_get( rfc_cfg_params_type *cfg, rfc_sig_cfg_type **ptr )
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
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_rx_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_tx_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_IMT ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_IMT ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_IMT ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_1900 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_1900 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_1900 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b4_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_800 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_800 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_800 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b5_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b8_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_FBRX ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_FBRX ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap0_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap0_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap1_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap2_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if (ret_val == FALSE)
  {
   /* 
 RF_MSG_5( RF_ERROR, "rfc_wtr2965_non_ca_2w_sku2 RFC detected error. Unsupported params combo: device %d band %d altp %d rxtx %d reqtype %d",
              cfg->logical_device, cfg->band, cfg->alternate_path, cfg->rx_tx, cfg->req );
 */ 
 }

  return ret_val;
}

boolean rfc_wtr2965_non_ca_2w_sku2_wcdma_ag::devices_cfg_data_get( rfc_cfg_params_type *cfg, rfc_device_info_type **ptr )
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
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_rx_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_INIT ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_init_wcdma_tx_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_IMT ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_IMT ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_IMT ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b1_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_1900 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_1900 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_1900 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b2_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC4 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b4_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_800 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_800 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_800 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b5_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_0_wcdma_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_RX ) && ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_rx_on_rfm_device_1_wcdma_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->band == (int)RFCOM_BAND_BC8 ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_tx_on_rfm_device_2_wcdma_b8_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_FBRX ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_internal_adc_enable_fbrx_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_FBRX ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_internal_adc_disable_fbrx_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_XPT_CAPTURE ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_et_cal0_fbrx_path_drx_iq_enable_xpt_capture_device_info);  ret_val = TRUE; }

  if ( ( cfg->rx_tx == RFC_CONFIG_TX ) && ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_XPT_CAPTURE ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_et_cal0_fbrx_path_drx_iq_disable_xpt_capture_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap0_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_0 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap0_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap1_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_1 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap1_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_ENABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_en_ant_swap2_device_info);  ret_val = TRUE; }

  if ( ( cfg->logical_device == RFM_DEVICE_2 ) && ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( ( cfg->band == (int)RFCOM_BAND_IMT )||  ( cfg->band == (int)RFCOM_BAND_800 )||  ( cfg->band == (int)RFCOM_BAND_BC8 )) && ( cfg->req == RFC_REQ_DISABLE_ANT_SWAP_1 ) && !ret_val )
  { *ptr = &(rf_card_wtr2965_non_ca_2w_sku2_wcdma_dis_ant_swap2_device_info);  ret_val = TRUE; }

  if (ret_val == FALSE)
  {
   /* 
 RF_MSG_5( RF_ERROR, "rfc_wtr2965_non_ca_2w_sku2 RFC detected error. Unsupported params combo: device %d band %d altp %d rxtx %d reqtype %d",
              cfg->logical_device, cfg->band, cfg->alternate_path, cfg->rx_tx, cfg->req );
 */ 
 }

  return ret_val;
}

boolean rfc_wtr2965_non_ca_2w_sku2_wcdma_ag::band_split_cfg_data_get( rfc_cfg_params_type *cfg, rfc_band_split_info_type **ptr )
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

  return ret_val;
}

