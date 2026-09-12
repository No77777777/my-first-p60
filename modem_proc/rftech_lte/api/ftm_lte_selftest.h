
#ifndef FTM_LTE_SELFTEST_H
#define FTM_LTE_SELFTEST_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              FTM LTE Self Test

GENERAL DESCRIPTION
  This is the header file for FTM LTE Self Testing.

Copyright (c) 2013 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_lte.mpss/3.11/api/ftm_lte_selftest.h#1 $ 
 
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
08/14/15   tjm/dbz Selftest Re-architecture 
10/16/14   jmf     [selftest] Implement multi-gain stage switch for ACLR improvement
10/14/14   jmf     [selftest] Implement FBRx Noise Floor Cal
10/07/14   dbz     [selftest] Add spectrum flatness measurement feature
09/19/14   jmf     Add definitions for VSWR v2 Meas
06/10/14   daa     Created

===========================================================================*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "ftm.h"
#include "ftm_lte_common_dispatch.h"
#include "ftm_common_control.h"


/*===========================================================================

                         EXTERNAL GLOBAL VARIABLES

===========================================================================*/

/*===========================================================================

                              DEFINITIONS

===========================================================================*/

#define TXRX_OFFSET_CALC_PROCESS_LEN 200

// LEGACY struct
typedef struct {

  boolean enable_tx_pwr_meas;
  boolean enable_aclr_meas;
  boolean enable_evm_meas;
  boolean enable_vswr_meas;
  boolean enable_vswr_meas_v2;
  boolean enable_flatness_meas;

  uint16 num_averages;
  uint32 capture_offset;
  uint32 tx_measurement_config;

} ftm_lte_selftest_meas_params_struct;

// Legacy Selftest function
/* Only expose necessary functions, no reason to expose the helper functions */
uint8 ftm_lte_selftest
(
  ftm_lte_selftest_meas_params_struct params,
  ftm_rf_lte_fbrx_iq_acquired_samples_data_type *ftm_rf_lte_fbrx_iq_acquired_samples_data_input,
  boolean *ftm_selftest_pwr_only,
  boolean perform_noise_cal
);


// NV item functions invoked by wrapper in FTM Common Selftest
void ftm_lte_obtain_fbrx_noise_pwr(ftm_common_power_per_bin_struct_type * noise_pwr_to_subtract, rfcom_lte_bw_type bw);
boolean ftm_lte_obtain_fbrx_gain_switchpoint_nv_item(rfcom_lte_band_type band, rfnv_item_id_enum_type *nv_item);
boolean ftm_lte_obtain_fbrx_gain_stage(uint8 * gain_stage);
boolean ftm_lte_obtain_fbrx_noise_nv_item(const uint8 nv_band, rfnv_item_id_enum_type * nv_item_to_write);

#endif /* FTM_LTE_SELFTEST_H */

