/*!
  @file
  fw_psamp_autopin_intf.h

  @brief
  Public definitions for auto-Pin sample processing.
*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document
  are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly
  reserved. By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in trust
  and will not be used, copied, reproduced in whole or in part, nor its
  contents revealed in any manner to others without the express written
  permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/fw/components/fw_psamp.mpss/rel/17.0/api/fw_psamp_autopin_intf.h#1 $
$DateTime: 2019/06/16 13:34:40 $
$Author: deepemm $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/27/13   DG      Add Tx sample rotation.
08/21/15   DG      Remove interface for SW to call processing code.
08/21/15   DG      Add more debug return values.
07/16/15   DG      Initial version.
===========================================================================*/

#ifndef _FW_PSAMP_AUTOPIN_INTF_H_
#define _FW_PSAMP_AUTOPIN_INTF_H_

#include "fw_psamp_volterra_intf.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FW_PSAMP_VOLTERRA_MAX_KERNELS
#define FW_PSAMP_VOLTERRA_MAX_KERNELS 5
#endif

typedef enum
{
  LEGACY_AMAM,
  ONESHOT_AMAM
}fw_psamp_autopin_type;

typedef struct
{
  uint32 handle;
  uint32 * tx_capture;
  uint32 * rx_capture;
  uint32 * tx_samp;
  uint32 * rx_samp;
  uint32 * tx_aligned;
  uint32 * rx_aligned;
  uint16 tx_len;
  uint16 rx_len;
  uint32 * rx_filt;
  uint16 rx_filt_len;
  uint16 rx_filt_shift;
  uint32 env_scale;
  int32 tx_freq; // (rotations/samp)/2^24
  fw_psamp_autopin_type type;
} fw_psamp_autopin_input_s;

typedef struct
{
  uint32 handle;
  uint8 status; // (>0)=error
  int32 tx_freq; // (rotations/samp)/2^24
  int16 tx_dc_i; // dBfs/100
  int16 tx_dc_q; // dBfs/100
  int16 tx_rms; // dBfs/100
  int16 rx_dc_i; // dBfs/100
  int16 rx_dc_q; // dBfs/100
  int16 rx_rms; // dBfs/100
  int32 peak_offset; // 1e-4
  int16 peak_flatness; // dB/100
  int16 peak_corr; // dB/100
  int16 corr_tx_rms; // dBfs/100
  int16 corr_tx_max; // dBfs/100
  int16 clip_tx_rms; // dBfs/100
  int16 clip_tx_mag; // dBfs/100
  int16 corr_rx_rms; // dBfs/100
  int16 corr_rx_max; // dBfs/100
  int16 corr_coeff; // dB/100
  int16 corr_snr; // dB/100
  int32 amam_err; // 1e-6
  int16 ss_gain_db; // for logging
  uint16 reflog_max_idx; // AMAM LUT max index - for logging
  int16 kernal_weights_real[FW_PSAMP_VOLTERRA_MAX_KERNELS]; // Forward model, kernel weights real part - for logging
  int16 kernal_weights_imag[FW_PSAMP_VOLTERRA_MAX_KERNELS]; // Forward model, kernel weights imaginary part - for logging
  uint32* tx_capture;  // Just for logging, debug purpose
  uint32* rx_capture;  // Just for logging, debug purpose
} fw_psamp_autopin_result_s;

extern void fw_psamp_autopin_enable (void);

extern void fw_psamp_autopin_proc_samp (
  fw_psamp_volterra_job_s * job,
  void (* result_funcptr) (fw_psamp_volterra_job_s * pout));

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _FW_PSAMP_AUTOPIN_INTF_H_