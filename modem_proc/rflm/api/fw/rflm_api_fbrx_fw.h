/*!
  @file
  rflm_api_fbrx_fw.h

  @brief
  RF LM API definitions for FBRx for use by FW
  
  @detail

  
  @todo


*/

/*==============================================================================

  Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

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

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/api/fw/rflm_api_fbrx_fw.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/01/16    aa     Updated <rx/tx>_sum, <tx_tx/rx_rx>_conj_corr to uint64
02/25/16    aa     Added rx/tx_debug_sample_byte_offset param to fbrx proc results
02/11/16    aa     Added xcorr_peak_idx to fbrx proc results
01/22/16    aa     FBRx interface changes v1 for Jacala 
01/18/16    am     Added a new rx_tx_delay api for intraband fbrx
09/23/15    aa     TH FBRx feature interface changes v3
07/24/15    aa     TH FBRx feature dummy interface changes v2
06/24/15    bn     TH FBRx feature interface changes
02/25/15    aa     Get FBRx static settings from autogen 
11/10/14    ka     Add API to determine if handle is valid for operation
07/10/14    aa     Move threshold_gain & scale_tx to power dependent params
02/21/14    aa     Added predet index to pwr dep params
02/06/14    ka     Added rx scale to pwr dep params
01/24/14    ka     Restore temporary fields for fw compilation
01/22/14    ka     Updated results type, added delay interface
12/20/13    ka     Add support for LTE TxAGC parameters
12/17/13    ka     Added additional parameters
12/16/13    ka     Temporarily revert version number to fix crash. API updates.
12/13/13    ka     Updated for latest uK APIs
11/14/13    ka     Updated for latest uK APIs
11/12/13    ka     Updated for latest uK APIs
10/30/13    ka     Updated for latest uK APIs
10/02/13   hdz     Added fbrx  api version number
09/05/13    ka     Remove unused header 
08/15/13    ka     Initial version
==============================================================================*/

#include "rflm_fbrx_typedef_ag.h"
#include "rflm_api_autopin_fw.h"
#include "rflm_api_cmn.h"

#ifndef RFLM_API_FBRX_FW_H
#define RFLM_API_FBRX_FW_H

/*! @brief Version of the rflm fbrx module.
*/
#define RFLM_API_FBRX_MAJOR_VERSION            1
#define RFLM_API_FBRX_MINOR_VERSION            8
#define RFLM_API_FBRX_VERSION    \
              ((RFLM_API_FBRX_MAJOR_VERSION << 16) | RFLM_API_FBRX_MINOR_VERSION )


/*==============================================================================

                           INCLUDE FILES

==============================================================================*/


/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/
#define RFLM_FBRX_RX_FILTER_TAPS_SIZE   RFLM_FBRX_RX_FILTER_TAPS_NUM
#define RFLM_FBRX_TX_FILTER_TAPS_SIZE   RFLM_FBRX_TX_UP_FILTER_TAPS_NUM
#define RFLM_FBRX_AMAM_SIZE             RFLM_FBRX_MATRIX_PREDIST_AMAM_NUM
#define RFLM_FBRX_PCOEFF_SIZE           RFLM_FBRX_DPD_PCOEFF_0_NUM
#define RFLM_FBRX_DPD_K_SIZE            RFLM_FBRX_DPD_K_NUM

#define RFLM_FBRX_LTE_MAX_RUNS_PER_SF 3
#define RFLM_FBRX_XCORR_LOG_SIZE 32

/*==============================================================================

  FBRx API input/output structure definitions
  
==============================================================================*/

/*==============================================================================
  For rflm_fbrx_get_static_settings
==============================================================================*/
/*! @brief  Input parameters for rflm_fbrx_get_static_settings API */
typedef struct
{
  uint8 reserved;  ///< placeholder. no input for now
} rflm_fbrx_get_static_settings_in_t;


/*! @brief  Output parameters for rflm_fbrx_get_static_settings API */
typedef struct
{
  rflm_fbrx_static_param_t static_settings; ///< static uK parameters
} rflm_fbrx_get_static_settings_out_t;

/*==============================================================================
  For rflm_fbrx_update_results
==============================================================================*/
/*! @brief  Input parameters for rflm_fbrx_update_results API per block */
typedef struct
{
  rflm_tech_id_t tech_id;
  int16          gain_est;
  uint16         next_gain;
  uint32         loft; /* complex */
  uint32         next_loft;
  uint32         rsb;  /* complex */
  uint32         next_rsb;
  int16          gain_imbalance;
  int16          phase_imbalance;
  uint32         cgain_est;
  uint32         model_a;
  uint32         model_b;
  uint32         model_c;
  uint16         xcorr_log[RFLM_FBRX_XCORR_LOG_SIZE];
  uint32         ls_err;
  uint32         fbrx_start_time; ///< From tech FW, for debugging 
  uint64         tx_tx_conj_corr; ///< Tx Rms: RFSW to convert to dBFS 
  uint64         tx_sum;          ///< Tx dc : RFSW to convert to dBFS
  uint64         rx_sum;          ///< Rx dc : RFSW to convert to dBFS
  uint64         rx_rx_conj_corr; ///< Rx Rms: RFSW to convert to dBFS
  uint32         return_capture_id;  ///< capture ID for the current results 
  rflm_fbrx_meas_req_type_e return_meas_type;  ///< meas type for the returned capture id
  rflm_fbrx_algorithm_t fbrx_proc_type;        ///< fbrx processing type 
  uint32         imm_dtr_timer;   ///< imm dtr timer   
  uint8          xcorr_peak_idx;  ///< x_corr peak index
  uint32         rx_debug_sample_address; ///< DEBUG RX samples LMEM start address 
  uint32         tx_debug_sample_address; ///< DEBUG TX samples LMEM start address  
  uint32         reserved;        ///< reserved field for future use    
} rflm_fbrx_proc_results_t;

/*! @brief  Input parameters for rflm_fbrx_update_results API */
typedef struct
{
  uint8 num_valid_results;
  rflm_fbrx_proc_results_t results[RFLM_FBRX_LTE_MAX_RUNS_PER_SF];
} rflm_fbrx_update_results_in_t;

/*! @brief  Output parameters for rflm_fbrx_update_results API */
typedef struct
{
  uint8 reserved; /* placeholder */
  rflm_autopin_capture_param_t autopin_params;
} rflm_fbrx_update_results_out_t;

#ifdef __cplusplus
extern "C" {
#endif


/*==============================================================================
  FBRx FW API's prototypes
==============================================================================*/

/*----------------------------------------------------------------------------*/
extern rflm_err_t rflm_fbrx_get_static_settings( rflm_handle_tx_t handle,
                                                 rflm_fbrx_get_static_settings_in_t  *in,
                                                 rflm_fbrx_get_static_settings_out_t *out );

/*----------------------------------------------------------------------------*/
extern rflm_err_t rflm_fbrx_update_results( rflm_handle_tx_t handle,
                                            rflm_fbrx_update_results_in_t  *in,
                                            rflm_fbrx_update_results_out_t *out );

/*----------------------------------------------------------------------------*/
extern uint32 rflm_fbrx_get_rx_tx_delay ( rflm_handle_tx_t handle );

extern uint32 rflm_fbrx_get_rx_tx_delay_intraband ( rflm_handle_tx_t pcc_handle, 
                                                    rflm_handle_tx_t scc_handle);

/*----------------------------------------------------------------------------*/
extern boolean rflm_fbrx_is_handle_valid_for_operation( rflm_handle_tx_t tx_handle );

/*----------------------------------------------------------------------------*/
extern rflm_err_t rflm_fbrx_update_env_scale(rflm_handle_tx_t tx_handle , uint8 run_idx ,uint32 env_scale);

/*----------------------------------------------------------------------------*/
/* Returns the FBRX LM handle for given TX handle */
rflm_err_t rflm_fbrx_get_fbrx_handle( rflm_handle_tx_t handle_id, uint32 *fbrx_lm_handle );

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* RFLM_API_FBRX_FW_H */
