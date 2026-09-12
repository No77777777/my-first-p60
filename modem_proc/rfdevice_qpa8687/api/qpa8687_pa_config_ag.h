
#ifndef QPA8687_PA_CONFIG_AG_H
#define QPA8687_PA_CONFIG_AG_H/*
WARNING: This QPA8687 driver is auto-generated.

Generated using: Process_QPA.pl 
Generated from-  

	File: QPA8687_RFFE_Settings.xlsx 
	Released: 3/18/2019
	Author: jose
	Revision: 1.03 ()
	Change Note: ModifiedG0settingsforGSMLB(notEDGE)toimproveRXBN
	Tab: qpa8687_pa_settings

*/

/*=============================================================================

          RF DEVICE  A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the configuration of the QPA8687 PA.

  Copyright (c) 2013-2016 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.


$ Header: $
$ Author: $
$ DateTime: $ 

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/


#include "comdef.h"
#include "rfc_common.h"
#include "rfdevice_qpa_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif  

boolean rfdevice_qpa_qpa8687_construct_driver_ag
(
  rfdevice_qpa_4g_settings_type* qpa_settings,
  rfdevice_qpa_4g_func_tbl_type* pa_fn_ptrs
);

uint8 rfdevice_qpa_qpa8687_get_efs_data_column
(
  uint8 port_index, 
  rfdevice_qpa_4g_tx_bw_data_type lcl_bw
);

boolean rfdevice_qpa_qpa8687_is_dpd_port
(
  uint8 port_index
);


#ifdef __cplusplus
}
#endif
#endif