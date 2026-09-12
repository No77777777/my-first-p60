
#ifndef RFC_WTR3925_QRD6225_CA_QPA6560_LTE_CONFIG_AG
#define RFC_WTR3925_QRD6225_CA_QPA6560_LTE_CONFIG_AG


#ifdef __cplusplus
extern "C" {
#endif

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

#include "rfc_msm_typedef.h" 
#include "rfc_common.h" 
#include "rfc_lte_data.h" 



class rfc_wtr3925_qrd6225_ca_qpa6560_lte_ag:public rfc_lte_data
{
public:
  static rfc_lte_data * get_instance();
    boolean sig_cfg_data_get( rfc_cfg_params_type *cfg, rfc_sig_cfg_type **ptr );
    boolean devices_cfg_data_get( rfc_cfg_params_type *cfg, rfc_device_info_type **ptr );
    boolean timing_cfg_data_get( rfc_cfg_params_type *cfg, rfc_timing_info_type **ptr );
    boolean ca_scell_log_path_swap_data_get( rfc_band_pair_list_type **ptr );
    boolean band_split_cfg_data_get( rfc_cfg_params_type *cfg, rfc_band_split_info_type **ptr );

protected:
  rfc_wtr3925_qrd6225_ca_qpa6560_lte_ag(void);  /*  Constructor  */
};


#ifdef __cplusplus
}
#endif



#endif


