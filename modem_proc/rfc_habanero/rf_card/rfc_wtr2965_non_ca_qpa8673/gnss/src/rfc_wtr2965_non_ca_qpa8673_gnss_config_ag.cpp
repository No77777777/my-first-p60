
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

#include "rfc_wtr2965_non_ca_qpa8673_gnss_config_ag.h" 
#include "rfc_wtr2965_non_ca_qpa8673_cmn_ag.h" 
#include "rfc_common.h" 
#include "rfcom.h" 



extern "C" 
{
  extern rfc_device_info_type rf_card_wtr2965_non_ca_qpa8673_gnss_device_info;
  extern rfc_sig_info_type rf_card_wtr2965_non_ca_qpa8673_gnss_sig_cfg;
} /* extern "C" */


rfc_gnss_data * rfc_wtr2965_non_ca_qpa8673_gnss_ag::get_instance()
{
  if (rfc_gnss_data_ptr == NULL)
  {
    rfc_gnss_data_ptr = (rfc_gnss_data *)new rfc_wtr2965_non_ca_qpa8673_gnss_ag();
  }
  return( (rfc_gnss_data *)rfc_gnss_data_ptr);
}

//constructor
rfc_wtr2965_non_ca_qpa8673_gnss_ag::rfc_wtr2965_non_ca_qpa8673_gnss_ag()
  :rfc_gnss_data()
{
}

boolean rfc_wtr2965_non_ca_qpa8673_gnss_ag::sig_cfg_data_get( rfc_cfg_params_type *cfg, rfc_sig_cfg_type **ptr )
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

  if ( ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) )
  { *ptr = &(rf_card_wtr2965_non_ca_qpa8673_gnss_sig_cfg.cfg_sig_list[0]);  ret_val = TRUE; }

  if (ret_val == FALSE)
  {
   /* 
 RF_MSG_5( RF_ERROR, "rfc_wtr2965_non_ca_qpa8673 RFC detected error. Unsupported params combo: device %d band %d altp %d rxtx %d reqtype %d",
              cfg->logical_device, cfg->band, cfg->alternate_path, cfg->rx_tx, cfg->req );
 */ 
 }

  return ret_val;
}

boolean rfc_wtr2965_non_ca_qpa8673_gnss_ag::devices_cfg_data_get( rfc_cfg_params_type *cfg, rfc_device_info_type **ptr )
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

  if ( ( cfg->alternate_path == 0 /*Warning: not specified*/ ) && ( cfg->req == RFC_REQ_DEFAULT_GET_DATA ) )
  { *ptr = &(rf_card_wtr2965_non_ca_qpa8673_gnss_device_info);  ret_val = TRUE; }

  if (ret_val == FALSE)
  {
   /* 
 RF_MSG_5( RF_ERROR, "rfc_wtr2965_non_ca_qpa8673 RFC detected error. Unsupported params combo: device %d band %d altp %d rxtx %d reqtype %d",
              cfg->logical_device, cfg->band, cfg->alternate_path, cfg->rx_tx, cfg->req );
 */ 
 }

  return ret_val;
}

boolean rfc_wtr2965_non_ca_qpa8673_gnss_ag::band_split_cfg_data_get( rfc_cfg_params_type *cfg, rfc_band_split_info_type **ptr )
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

