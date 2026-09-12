#ifndef RFLTE_MC_MEAS_H
#define RFLTE_MC_MEAS_H
/*!
  @file rflte_mc_meas.h

  @brief
  This file contains the LTE meas functions.

  @details

*/

/*===========================================================================

  Copyright (c) 2009 - 2020 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_mc_meas.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
03/28/18   ks      NULL2L IRAT handling
11/15/17   ks      NULL2L IRAT handling
10/28/15   sml     Deprecate un-used L2L API
07/22/15   Saul    Error checking. 
06/25/15   cj      L2L Interface change
11/08/12   pl      Move internal function definitions to internal files 
11/06/12   pl      Adding support for clean up script generation 
10/04/12   pl      move meas_init function to LTE meas module
09/20/12   pl      remove obsolete interfaces
10/23/10   jyu     Add L2L interfreq support 
08/16/10   pl      Added script-based IRAT functions
05/07/10   pl      Update for LTE IRAT build_start_script support  
04/12/10   qma     Initial Check-in
===========================================================================*/

#include "rfmeas_lte.h"
void rflte_meas_mc_init(void);

boolean rflte_mc_meas_build_tuneback_scripts(uint8 sub_id, rfm_meas_common_param_type *lte_meas_script_param );

boolean rflte_mc_meas_build_tuneaway_scripts(uint8 sub_id, rfm_meas_common_param_type *lte_meas_script_param );
void rflte_mc_meas_config_pwr_on_off(rfm_device_enum_type device, rfcom_lte_band_type band, uint8 sub_id,boolean on_off);

#endif /*RFLTE_MC_MEAS_H*/
