/*!
  @file
  fw_psamp_volterra_intf.h

  @brief
  Public definitions for volterra sample processing.
*/

/*===========================================================================

  Copyright (c) 2015-16 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //source/qcom/qct/modem/fw/components/fw_psamp.mpss/rel/17.0/api/fw_psamp_volterra_intf.h#1 $
$DateTime: 2019/06/16 13:34:40 $
$Author: deepemm $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/30/16   DG      Reorganize code for reuse.
03/30/16   DG      Output additional compression values.
03/28/16   DG      Optionally time align with correlation coefficient.
02/26/16   DG      Compute auto-delay error.
02/26/16   DG      Compute auto-pin error.
02/26/16   DG      Number of auto-pin kernels is independent from kernel mask.
12/10/15   DG      Find forward model compression point (for M-line.)
12/10/15   DG      Export distortion table normalization scaling (for M-line.)
12/03/15   DG      Allow configurable Rx spectral inversion.
11/13/15   DG      Add Volterra calibration.
11/13/15   DG      Initial version.
===========================================================================*/

#ifndef _FW_PSAMP_VOLTERRA_INTF_H_
#define _FW_PSAMP_VOLTERRA_INTF_H_

// Maximum number of Tx samples.
//
#define FW_PSAMP_VOLTERRA_TX_SAMP (512)

// Maximum number of Rx samples.
//
#define FW_PSAMP_VOLTERRA_RX_SAMP (512)

// Kernel rows -- FW_PSAMP_VOLTERRA_KERNEL_ROWS entry array of 3 bit counts.
//
#define FW_PSAMP_VOLTERRA_KERNEL_ROWS 9

// Maximum number of row 0 kernels.
//
#define FW_PSAMP_VOLTERRA_MAX_ROW0_KERNELS 5

// Maximum number of kernels.
//
#define FW_PSAMP_VOLTERRA_MAX_KERNELS 5 // Atlas is 20

// Define number of entries in volterra HW memoryless LUT's (0-1).
//
#define FW_PSAMP_VOLTERRA_LUT_0_1_SIZE  32

// Define number of entries in volterra HW memory LUT's (2-17).
//
#define FW_PSAMP_VOLTERRA_LUT_2_17_SIZE 8

// Construct kernel mask from individual row counts.
//
#define FW_PSAMP_VOLTERRA_KERNEL_MASK(r0,r1,r2,r3,r4,r5,r6,r7,r8) \
    ( ((r0)<<(8*3))| \
      ((r1)<<(7*3))|((r2)<<(6*3))|((r3)<<(5*3))|((r4)<<(4*3))| \
      ((r5)<<(3*3))|((r6)<<(2*3))|((r7)<<(1*3))|((r8)<<(0*3)) )

// Extract per-row kernel count from kernel mask.
//
#define FW_PSAMP_VOLTERRA_KERNELS_FOR_ROW(mask,row) \
    (((mask) >> (3 * (8 - (row)))) & (~(~0 << 3)))

// Bit mask of kernels using MEM1 rows.
//
#define FW_PSAMP_VOLTERRA_KERNEL_MASK_MEM1 \
    FW_PSAMP_VOLTERRA_KERNEL_MASK(0,7,7,7,7,0,0,0,0)

// Bit mask of kernels using MEM2 rows.
//
#define FW_PSAMP_VOLTERRA_KERNEL_MASK_MEM2 \
    FW_PSAMP_VOLTERRA_KERNEL_MASK(0,0,0,0,0,7,7,7,7)

// Bit mask of kernels using MEM1 or MEM2 rows.
//
#define FW_PSAMP_VOLTERRA_KERNEL_MASK_MEM \
    FW_PSAMP_VOLTERRA_KERNEL_MASK(0,7,7,7,7,7,7,7,7)

// Bit mask of kernels using DDR1 rows.
//
#define FW_PSAMP_VOLTERRA_KERNEL_MASK_DDR1 \
    FW_PSAMP_VOLTERRA_KERNEL_MASK(0,7,7,0,0,7,7,0,0)

// Bit mask of kernels using DDR2 rows.
//
#define FW_PSAMP_VOLTERRA_KERNEL_MASK_DDR2 \
    FW_PSAMP_VOLTERRA_KERNEL_MASK(0,0,0,7,7,0,0,7,7)

#ifdef __cplusplus
extern "C" {
#endif

// Volterra SC16 weight.
//
typedef union
{
  struct
  {
    int16 I;
    int16 Q;
  };
  uint32 uw;
} fw_psamp_volterra_weight_s;

// Volterra LUT's prior to interlacing for HW.
//
typedef union // 384 bytes
{
  struct
  {
    int16 lut0 [FW_PSAMP_VOLTERRA_LUT_0_1_SIZE];
    int16 lut1 [FW_PSAMP_VOLTERRA_LUT_0_1_SIZE];
    int16 lut2 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut3 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut4 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut5 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut6 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut7 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut8 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut9 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut10 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut11 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut12 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut13 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut14 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut15 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut16 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
    int16 lut17 [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE];
  };
  // Force uint64 alignment of structure (also of each LUT.)
  uint64 ud[1];
} fw_psamp_volterra_lut_s;

// Volterra memoryless LUT's (0..1) interlaced for HW.
//
typedef union // 128 bytes
{
  int16  h  [FW_PSAMP_VOLTERRA_LUT_0_1_SIZE * 2 / 1];
  uint32 uw [FW_PSAMP_VOLTERRA_LUT_0_1_SIZE * 2 / 2];
  uint64 ud [FW_PSAMP_VOLTERRA_LUT_0_1_SIZE * 2 / 4];
} fw_psamp_volterra_lut_0_1_hw_s;

// Volterra memoryless LUT's (2..17) interlaced for HW. The LUT's are stored
// in four structures corresponding to HW addresses (and QLNK packets used to
// sent them to HW.)
// (1) 2..5,10..13 even entries
// (2) 2..5,10..13 odd entries
// (3) 6..9,14..17 even entries
// (4) 6..9,14..17 odd entries
//
typedef union // 64 bytes
{
  int16  h  [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE * 16 / 4 / 1];
  uint32 uw [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE * 16 / 4 / 2];
  uint64 ud [FW_PSAMP_VOLTERRA_LUT_2_17_SIZE * 16 / 4 / 4];
} fw_psamp_volterra_lut_2_17_hw_frag_s;

typedef struct
{
  // Caller specified tag to manage multiple processing requests.
  //
  uint32 handle;

  // Tx samples.
  //
  uint32 * tx_capture;

  // Rx samples.
  //
  uint32 * rx_capture;

  // Number of Tx samples.
  //
  uint16 tx_len;

  // Number of Rx samples.
  //
  uint16 rx_len;

  // Width of search for Tx within Rx (in samples.)
  //
  int16 search_width;

  // Offset of search for Tx within Rx (in samples.) Can be positive or
  // negative. Offset is non-zero only for testing.
  //
  int16 rx_offset;

  // Specification of enabled kernels. Bits (3*(8-N))+(0:2) specify the number
  // of enabled kernels for row N=0..8.
  //
  uint32 kernel_mask;

  // Forward model target compression ratio.
  //
  int16 compression;

  // Sample delay for MEM1 kernels -- must be greater than zero.
  //
  uint32 tap_delay_1 : 3;

  // Sample delay for MEM1 kernels -- must be greater than MEM1 delay.
  //
  uint32 tap_delay_2 : 3;

  // Flag to use forward modeling (for SDPD construction.)
  // Otherwise use reverse modeling (required for Volterra LUT construction.)
  //
  uint32 forward : 1;

  uint32 row0 : 1; // AI: remove -- left in SWI to avoid SW compilation errors

  // Flag to enable Rx spectral inversion.
  //
  uint32 rx_spec_inv : 1;

  // Flag to enable normalization (cross-correlation coefficient) during
  // time alignment.
  //
  uint32 time_align_norm : 1;

  // Flag to enable auto-delay measurement.
  //
  uint32 auto_delay : 1;

  // Envelope scale for one shot and legacy tracking Auto-Pin.
  //
  uint16 env_scale; // Added to Jacala

  // Number of row 0 (memoryless) kernels. After weights are constructed for
  // kernels specified in (kernel_mask,) construct a second set of weights for
  // the row 0 kernels. Row 0 kernels are used for Auto-Pin.
  //
  uint8 n_row0_kernel;

  // Number of reference kernels for auto-pin measurement. Zero disables
  // auto-pin error measurement.
  //
  uint8 n_ref_kernel;

  // Auto-Pin reference kernel weight Q factor.
  //
  int8 ref_weight_qfactor;

  // Auto-Pin reference kernel weights.
  //
  fw_psamp_volterra_weight_s ref_weight [FW_PSAMP_VOLTERRA_MAX_ROW0_KERNELS];

} fw_psamp_volterra_param_s;

// Outputs from sample processing function.
//
typedef struct
{
  // Processing status. Non-zero values indicate error.
  //
  uint8 status;

  // Tx DC I power in 1/100 dB full-scale.
  //
  int16 tx_dc_i;

  // Tx DC Q power in 1/100 dB full-scale.
  //
  int16 tx_dc_q;

  // Tx maximum magnitude in 1/100 dB full-scale.
  //
  int16 tx_max;

  // Post-DC cancellation Tx RMS power in 1/100 dB full-scale.
  //
  int16 tx_rms;

  // Rx DC I power in 1/100 dB full-scale.
  //
  int16 rx_dc_i;

  // Rx DC Q power in 1/100 dB full-scale.
  //
  int16 rx_dc_q;

  // Rx maximum magnitude in 1/100 dB full-scale.
  //
  int16 rx_max;

  // Post-DC cancellation Rx peak magnitude in 1/100 dB full-scale. Peak
  // magnitude excludes the highest 99% magnitudes -- excludes the highest 1%.
  //
  int16 rx_peak;

  // Rx RMS power in 1/100 dB full-scale.
  //
  int16 rx_rms;

  // Rx scaling applied during conversion of PCFL20 samples to SC16.
  // Units are 1/100 dB.
  //
  int16 rx_scale;

  // Time offset from Tx to Rx samples. Units are 1e-4.
  //
  int32 peak_offset;

  // Flatness of Tx-Rx correlation peak -- maximum integer offset magnitude
  // relative to average of both shoulders. Units are 1/100 dB.
  //
  int16 peak_flatness;

  // Correlation coefficient for Tx-Rx integer sample time alignment.
  // Units are 1/100 dB.
  //
  int16 peak_corr;

  // Tx RMS power after Tx-Rx time alignment. Units are 1/100 dB full-scale.
  //
  int16 corr_tx_rms;

  // Rx RMS power after Tx-Rx time alignment. Units are 1/100 dB full-scale.
  //
  int16 corr_rx_rms;

  // Correlation coefficient for Tx-Rx sub-sample time alignment.
  // Units are 1/100 dB.
  //
  int16 corr_coeff;

  // Time alignment SNR. Units are 1/100 dB.
  //
  int16 corr_snr;

  // Kernel weight construction SNR. Units are 1/100 dB.
  //
  int16 kernel_snr;

  // Row 0 kernel weight construction SNR. Units are 1/100 dB.
  //
  int16 row0_snr;

  // Kernel weight Q factor.
  //
  int8 kernel_weight_qfactor;

  // Row 0 kernel weight Q factor.
  //
  int8 row0_weight_qfactor;

  // Number of kernels (including pruned kernels.)
  //
  uint8 n_kernel;

  uint8 n_row0_kernel; // AI: remove -- left in SWI to avoid SW compilation errors

  // Number of pruned kernels.
  //
  uint8 n_prune;

  // Number of pruned row 0 kernels.
  //
  uint8 n_row0_prune;

  // Bit mask of pruned row 0 kernels.
  //
  uint8 row0_prune_mask;

  // Bit mask of pruned kernels.
  //
  uint32 prune_mask;

  // Rx gain for minimum Tx. Units are 1/100 dB.
  //
  int16 rx_gain_low;

  // Rx gain used to reconstruct Rx DC. Units are 1/100 dB.
  //
  int16 rx_gain_dc;

  // Maximum Rx gain. Units are 1/100 dB.
  //
  int16 rx_gain_max;

  // Rx gain at maximum observed Tx. Units are 1/100 dB.
  //
  int16 rx_gain_tx_max;

  // Rx gain for full-scale Tx. Units are 1/100 dB.
  //
  int16 rx_gain_high;

  // Compression point Tx value. Units are 1/100 dB full-scale.
  //
  int16 compress_tx;

  // Compression point Rx value. Units are 1/100 dB full-scale.
  //
  int16 compress_rx;

  // Auto-Pin error. Units are 1e-6.
  //
  int32 apin_err;

  // Auto-Delay error. Units are 1e-6.
  //
  int32 autodelay_err;

  // Kernel weights.
  //
  fw_psamp_volterra_weight_s kernel_weight [FW_PSAMP_VOLTERRA_MAX_KERNELS];

  // Row 0 kernel weights.
  //
  fw_psamp_volterra_weight_s row0_weight [FW_PSAMP_VOLTERRA_MAX_ROW0_KERNELS];

} fw_psamp_volterra_result_s;

// Processing queue entry.
//
typedef struct _fw_psamp_volterra_job_s
{
  // Link to another structure. This is used to manage the list of pending
  // sample captures within the processing code.
  //
  struct _fw_psamp_volterra_job_s * link;

  // Function called when processing completes.
  //
  void (* result_funcptr) (struct _fw_psamp_volterra_job_s * pout);

  // Input parameters.
  //
  fw_psamp_volterra_param_s param;

  // Processing results.
  //
  fw_psamp_volterra_result_s result;

} fw_psamp_volterra_job_s;

// Public API exported to SW.
//
typedef struct
{
  // Function to process samples.
  //
  void (* proc_samp_funcptr) (fw_psamp_volterra_job_s * job);

  // Function to construct forward model, AMAM, AMPM tables from forward
  // model kernel weights.
  //
  // Return value is scaling to convert distortion values to 1.0 full scale.
  //
  float (* construct_dpd_funcptr) (

    // Output distortion table. Size is (dpd_size * interp_per_dpd).
    // If pointer is NULL, output table is not constructed.
    //
    uint16 * dist_tab,

    // Output AMAM table. Size is (dpd_size).
    // If pointer is NULL, output table is not constructed.
    //
    uint16 * am_tab,

    // Output AMPM table. Size is (dpd_size).
    // If pointer is NULL, output table is not constructed.
    //
    int16 * pm_tab,

    // Kernel weights.
    //
    fw_psamp_volterra_weight_s * weight,

    // Kernel mask.
    //
    uint32 kernel_mask,

    // Kernel weight Q factor.
    //
    int32 weight_qfactor,

    // AMAM, AMPM table sizes.
    //
    int32 dpd_size,

    // Number of interpolation points per AMAM, AMPM entry. Increasing
    // interpolation points increases AMAM, AMPM accuracy. If constructing
    // the distortion table, that table size is the total number of
    // interpolation points -- dpd_size * interp_per_dpd.
    //
    int32 interp_per_dpd);

  // Function to construct volterra LUT's from volterra kernels.
  //
  void (* construct_lut_funcptr) (

    // Output linear ordered LUT values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_s * lut,

    // Output HW ordered Volterra LUT 0,1 values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_0_1_hw_s * base,

    // Output HW ordered Volterra LUT 2..5,10..13 even index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * low_even,

    // Output HW ordered Volterra LUT 2..5,10..13 odd index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * low_odd,

    // Output HW ordered Volterra LUT 6..9,13..17 even index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * high_even,

    // Output HW ordered Volterra LUT 6..9,13..17 odd index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * high_odd,

    // Kernel weights.
    //
    fw_psamp_volterra_weight_s * weight,

    // Kernel mask.
    //
    uint32 kernel_mask,

    // Kernel weight Q factor.
    //
    int32 weight_qfactor);

  // Function to reformat Volterra LUT's for HW.
  //
  void (* interlace_hw_lut_funcptr) (

    // Output linear ordered LUT values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_s * lut,

    // Output HW ordered Volterra LUT 0,1 values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_0_1_hw_s * base,

    // Output HW ordered Volterra LUT 2..5,10..13 even index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * low_even,

    // Output HW ordered Volterra LUT 2..5,10..13 odd index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * low_odd,

    // Output HW ordered Volterra LUT 6..9,13..17 even index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * high_even,

    // Output HW ordered Volterra LUT 6..9,13..17 odd index values.
    // If pointer is NULL, output table is not constructed.
    //
    fw_psamp_volterra_lut_2_17_hw_frag_s * high_odd);

} fw_psamp_volterra_api_s;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _FW_PSAMP_VOLTERRA_INTF_H_
