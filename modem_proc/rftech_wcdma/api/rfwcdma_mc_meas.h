#ifndef RFWCDMA_MC_MEAS_H
#define RFWCDMA_MC_MEAS_H
/*!
  @file rfwcdma_mc_meas.h

  @brief
  This file contains the WCDMA meas functions.

  @details

*/

/*===========================================================================

  Copyright (c) 2009 - 2013 Qualcomm Technologies Incorporated. All Rights Reserved

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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_wcdma.mpss/3.11/api/rfwcdma_mc_meas.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
11/11/15   sd/rmb  Add time profile for eLNA scripts in IRAT
07/27/15   asn     New Power ON OFF API
04/28/15   rmb     Remove feature flag RF_HAS_MEAS_INTF_TH_2_0 but retain code
                   added under this flag.
04/07/15   rmb     Remove reference to unused structure
04/06/15   rmb     Merge back meas interface changes onto Mainline with feature flag. 
03/19/15   rmb     First phase of WCDMA Meas interface v2 checkin.
09/19/14   rmb     Time profile X2W and W2X build scripts.
07/03/14   rmb     Cleanup of unused API X2W CGAGC params.
03/25/14   rmb     Add a new structure type for RxAGC during X2W measurements. 
08/10/13   aa      DBDC W2W 
07/17/13   vs       Add X to W API for getting CGAGC params
02/19/13   pl       Update interface param type
11/14/11   dw       Fix warning 
08/24/10   kguo     Split idle and connected mode APIs for IRAT 
05/24/10   kguo     Fixed type error 
04/19/10   kguo     Changed API name to be consistant with other techs  
04/12/10   kguo     Initial Revision
===========================================================================*/
#include "rfcom.h"
#include "rfcommon_time_profile.h"

typedef struct
{
  rfcom_wcdma_band_type wcdma_band;
  boolean bypass_rxagcon_flag;
} rfwcdma_mc_meas_rxagc_xtow;

typedef struct
{
  rf_time_tick_type build_script_start_t; 
  rf_time_type build_script_time;
  rf_time_tick_type sw_init_start_t; 
  rf_time_type sw_init_time;
  rf_time_tick_type cleanup_start_t; 
  rf_time_type cleanup_total_time;
  rf_time_tick_type transceiver_script_start_t; 
  rf_time_type transceiver_script_time;
  rf_time_tick_type asm_script_start_t; 
  rf_time_type asm_script_time;
  rf_time_tick_type tuner_script_start_t; 
  rf_time_type tuner_script_time;
  rf_time_tick_type grfc_script_start_t; 
  rf_time_type grfc_script_time;
  rf_time_tick_type msm_script_start_t; 
  rf_time_type msm_script_time;
  rf_time_tick_type mdsp_script_start_t; 
  rf_time_type mdsp_script_time;
  rf_time_tick_type elna_script_start_t; 
  rf_time_type elna_script_time;
} rfwcdma_mc_meas_build_script_profile_type;

void rfwcdma_mc_meas_init(void);

#endif /*RFWCDMA_MC_MEAS_H*/
