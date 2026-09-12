/*!
  @file
  fw_rf_common_intf.h

  @brief
  Generic interface for common RF interface

*/

/*===========================================================================

  Copyright (c) 2011-14 Qualcomm Technologies Incorporated. All Rights Reserved

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

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/12/14   BW      Clean up CCS RF/QTF/ASD related API structures
10/25/13   DG      Add Rx RMS capture -- faster Rx RMS measurement.
07/22/13   DG      Add EVM capture -- compute EVM from RefMode=1 (Tx I,Q).
06/14/13   BW	   Add settling_time for CL measurement.
05/22/13   BW      Add ASD command intf
04/29/13   DG      SWI changes for ET delay, FBRx cal -- SWI only, no code.
04/29/13   DG      SWI+API changes to separate sample capture and processing.
04/18/13   BW      Add QTF enum to support UMTS DBDC
02/01/13   BW      Add QTF antenna tuner intf
01/30/13   DG      Move RF cal sample buffers to overlay memory.
01/22/13   TZ      Remove DAC Cal struct from shared memory
12/05/12   NB      Interface for RSB calibration
11/07/12   NB      Updated DPD interface for multiple sample capture 
10/18/12   NB      XPT calibration updates for Dime
03/01/12   NG      Updates for SAPT
07/18/11   NB      Updated DAC calibration interface
05/06/11   NB      Increased buffer size (added word in header) file is 
                   backword compatible 
04/27/11   NB      Initial version
===========================================================================*/
#ifndef FW_RF_COMMON_INTF_H
#define FW_RF_COMMON_INTF_H

/* Include memory map file for interface location */
#include "modem_fw_memmap.h"
#include "fw_dpd_intf.h"
#include "fw_autopin_intf.h"

/*! @brief Bridge ID of AHB mempool bridge that RF SW should use to 
 * directly read out samples from mempool */
#define FW_RF_COMMON_SW_MEMPL_RD_AHB_BRDG_ID 1

/*! @brief size of debug log buffer. This buffer is used for logging 
  debug data or IQ data. The data is packed in 32 bit format. The
  Q_size of the actual data copied is provided in the header.

  GSM overlays a structure on log buffer. */
#define FW_RF_COMMON_DEBUG_BUF_SIZE  (18*1024)

/*! @brief Number of common captures */
#define FW_RF_COMMON_NUM_CAPTURES  4

/*! @brief Maximum number of sample buffers for DPD */
#define FW_DPD_MAX_SAMP_BUF 12

/*! @brief Maximum number of result buffers for DPD */
#define FW_DPD_MAX_RESULTS  12

typedef enum
{
  /* Result is invalid */
  FW_RF_COMMON_RESULT_INVALID = 0,
  /* Result cannot be updated (some error) */
  FW_RF_COMMON_RESULT_FAIL,
  /* Result copy in progress */
  FW_RF_COMMON_RESULT_IN_PROGRESS,
  /* Result copy completed successfully */
  FW_RF_COMMON_RESULT_SUCC
} fw_rf_common_result_e;

/*! @brief Control structure for debug logging */
typedef struct 
{
  /*! @brief Number of items */
  uint32 length;
  /*! @brief Number of 32 bit words */
  uint32 size32;
  /*! @brief Index of first used sample (number of samples to skip) */
  uint8 first_samp_idx;
  /*! @brief item bit width */
  uint8 q_size;
  /*! @brief storage bit width */
  uint8 mem_q_size;
  /*! @brief Whether capture was success or not */
  fw_rf_common_result_e result;
} fw_rf_common_header_s;

/*! @brief Structure to capture debug samples or IQ samples logged 
    or SAPT samples*/
typedef struct 
{
  /*! @brief header for logging results */
  fw_rf_common_header_s rf_header;
  /*! @brief Data buffer */
  uint32 data[FW_RF_COMMON_DEBUG_BUF_SIZE] ALIGN(32);
} fw_rf_common_log_buffer_s;

/*! @brief Structure for DPD sample Tx capture */
typedef struct
{
  /*! @brief Header information */
  fw_rf_common_header_s header;
  /*! @brief data buffer */
  uint32 data[FW_DPD_MAX_TX_CAPTURE_SIZE32] ALIGN(32);
} fw_rf_dpd_tx_samp_buffer_s;

/*! @brief Structure for DPD sample Rx capture */
typedef struct
{
  /*! @brief Header information */
  fw_rf_common_header_s header;
  /*! @brief data buffer */
  uint32 data[FW_DPD_MAX_RX_CAPTURE_SIZE32] ALIGN(32);
} fw_rf_dpd_rx_samp_buffer_s;

/*! @brief Structure that contains captured RX and TX samples */
typedef struct
{
  /*! @brief TX samples */
  fw_rf_dpd_tx_samp_buffer_s tx;
  /*! @brief RX samples */
  fw_rf_dpd_rx_samp_buffer_s rx;
} fw_rf_dpd_samp_buffer_s;

/*! @brief DAC calibration results are of size 128 */
#define FW_RF_COMMON_DAC_CAL_BUF_SIZE  128

/*! @brief sample capture types */
typedef enum
{
  /*! @brief construct DPD */
  FW_RF_CAP_DPD,
  /*! @brief ET path delay (split capture) */
  FW_RF_CAP_ET_PATH_DELAY,
  /*! @brief feedback Rx (gain/phase imbalance, LO feed through, Tx gain */
  FW_RF_CAP_FBRX,
  /*! @brief EVM measurement */
  FW_RF_CAP_EVM,
  /*! @brief Rx RMS measurement */
  FW_RF_CAP_RX_RMS,
  /*! @brief VSWR measurement */
  FW_RF_CAP_VSWR,
  /*! @brief GSM power measurement */
  FW_RF_CAP_GSM_PWR_MES // AI: rename
} fw_rf_capture_type;

/*! @brief Structure for common sample capture parameters */
typedef struct
{
  /*! @brief Number of samples */
  uint16 num_samp;
  /*! @brief Captyre type */
  fw_rf_capture_type capture_type;
  /*! @brief Number of samples before split capture config change,
      0 = no split capture */
  uint16 split_samp;
  /*! @brief Tx chain */
  uint8 tx_chain;
  /*! @brief Rx chain */
  uint8 rx_chain;
  /*! @brief Buffer for RxLM configuration */
  uint8 rxlm_buf_id;
  /*! @brief Flag to compressed Rx samples.
      0=no, 20 bit samples are expanded to 32 bits, upper 12 bits are ignored
      1=yes, 20 bit samples have no padding between samples */
  uint8 rx_compact;
  /*! @brief Tx decimation ratio -- 0,1 disables */
  uint8 tx_decimate;
  /*! @brief Tx upsample ratio -- 0,1 disables */
  uint8 tx_upsamp;
  /*! @brief Flag to enable FW processing */
  uint8 do_processing;
} fw_rf_capture_config_t; // in RF common SWI, shared for all captures

/*! @brief Structure for parameters common to all tech sample capture commands.
    The structure is included in each tech sample capture command. */
typedef struct
{
  /*! @brief Message Router tag for response */
  uint32 seq_num;
  /*! @brief Sample buffer index */
  uint32 samp_idx : 8;
  /*! @brief Result buffer index */
  uint32 result_idx : 8;
  /*! @brief Flag indicating first capture in sequence */
  uint32 first_trig : 1;
  /*! @brief Flag indicating last capture in sequence */
  uint32 last_trig : 1;
} fw_rf_capture_param_t;

/*! @brief Common FW/RF interface. This contains 
  a) Debug log buffer
  b) XPT results
  c) DAC calibration results
  d) Antenna tuner SPI configuration.
  e) QTF antenna tuner common FW intf
  f) Antenna switch diversity intf
 */
typedef struct
{
  union {
    /*! @brief For legacy copy */
    fw_rf_common_log_buffer_s rf_log_buffer;
    /*! @brief Make an array for generic copies */
    fw_rf_common_log_buffer_s rf_common_log_buf[FW_RF_COMMON_NUM_CAPTURES];
  };
  /*! @brief Common parameters for sample capture */
  fw_rf_capture_config_t capture_config;
  /*! @brief Common paramenters for DPD calibration */
  fw_dpd_cal_config_t dpd_cal_config;
  /*! @brief XPT calibration results */
  fw_dpd_cal_result_t dpd_cal_result[FW_DPD_MAX_RESULTS];
  /*! @brief Autopin processing function pointer */
  fw_autopin_proc fw_autopin_fn;

} fw_rf_common_intf_s;

/*! @brief Common FW/RF calibration sample buffers. The sample buffers for
    different types of calibration are overlayed with each other then
    overlayed with crash dump memory.
 */
typedef struct
{
  union
  {
    /*! @brief Make an array for generic copies */
    fw_rf_common_log_buffer_s rf_common_log_buf[FW_RF_COMMON_NUM_CAPTURES];
    /*! @brief Sample captures for DPD calibration */
    fw_rf_dpd_samp_buffer_s dpd_samp_buffer[FW_DPD_MAX_SAMP_BUF];
  };
} fw_rf_calibration_intf_s;

#endif // FW_RF_COMMON_INTF_H
