
#ifndef RFLM_CDMA_TUNER__H
#define RFLM_CDMA_TUNER__H
/*
  @file
  rflm_cdma_tuner.h

  @brief
  This file contains RFLM  c2k mc definitions, enumerations and prototypes that
  are used in rflm scope.

  @addtogroup RFLM_C2K_MC
  @{
*/

/*=============================================================================

                RFLM CDMA TUNER   H E A D E R   F I L E 
       
DESCRIPTION
  This file contains declarations and definitions to support RF front end driver 
  module.

Copyright (c) 2013-2014  by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/api/c2k/rflm_cdma_tuner.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
04/18/16   pk      VSWR changes
06/15/15   ADD     Add history here
06/15/15   DAD     Who Dat
===========================================================================*/
#include "rflm.h"
#include "rflm_txagc.h"
#include "rflm_api_c2k.h"
#include "rflm_rxagc_api.h" /* for cell_id_t */
#include "rflm_ccs_rf_intf.h"
#include "rflm_ccs_intf.h"
#include "rflm_cmn_intf.h"
#include "rflm_ccs_task_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*/
void 
rflm_cdma_post_process_fbrx_vswr_measurements
(
  rflm_handle_tx_t            handle_id   ,
  rflm_fbrx_vswr_meas_data_t  vswr_meas_data
) ;

/*---------------------------------------------------------------------------*/
boolean
rflm_cdma_schedule_vswr_captures
(
  rflm_handle_tx_t handle_id ,
  rflm_tech_id_t tech_id ,
  rflm_ustmr_t coupler_switch_xo_time,
  rflm_ustmr_t coupler_switch_back_xo_time 
) ;


/*---------------------------------------------------------------------------*/
rflm_ustmr_t
rflm_cdma_get_vswr_capture_complete_time
(
  rflm_ustmr_t    fw_action_time   ,
  rflm_tech_id_t  tech_id
) ;

#ifdef __cplusplus
}
#endif

/*!   @} */

#endif /* #ifndef RFLM_CDMA_TUNER__H */
