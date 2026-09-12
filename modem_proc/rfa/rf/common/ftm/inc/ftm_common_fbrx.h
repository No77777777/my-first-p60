#ifndef FTMCOMMON_FBRX_H
#define FTMCOMMON_FBRX_H
/*!
  @file
  ftm_fcommon_fbrx.h

  @brief
  This module contains interface of common FTM FBRx.
  
*/

/*==============================================================================

  Copyright (c) 2013 Qualcomm Technologies Incorporated. All Rights Reserved

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

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_common_fbrx.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
03/23/16   sk      Jacala LTE FBRX tune-away support
03/01/15    aa     Adjust autocal to use default fixed Rx scale of 2048 
10/06/15    aa     Adjust autocal algorithm to resolve saturation and qnoise issues 
10/06/15    aa     Added more ftm FBRx debug commands 
09/21/15    aa     Added suport for FBRx debug ftm command
08/07/15    aa     Added support for FBRx autocal
03/11/15    aa     Added changes to get valid tx handle from tech
01/29/15    kg     Added FTM API to return FBRX sample captures
01/29/15    aa     Added support for dynamic memory alloc to store FBRX samples
10/08/14    ka     Replace rfc with device api for retrieving device pointer
09/08/14   jmf     [SelfTest] Implement Tx Power measurement using TxAGC loop
08/26/14    aa     Added support for FBRX QRCT logging
08/11/14    aa     Update run continuous cmd to control fbrx enable/disable
06/12/14    ka     Add enable parameter to run continuous cmd
05/26/14    ka     Return results from single trigger cmd
04/07/14    ka     Update single trigger. Remove obsolete code.
03/20/14    ka     Added support for storing captures during cal
02/13/14    ka     Added continuous run api, updated capture params
02/06/14    aa     Added api to set ftm cal state in fbrx dm
01/30/14    aa     Added API to get valid Tx handle for FBRx
01/22/14    ka     Added fbrx set params API
12/23/13    ka     Added API for calibration
11/25/13    ka     Added store results. Updated response packet.
11/13/13    ka     Added FTM fbrx common test framework
09/23/13    ka     Initial version

==============================================================================*/

#include "comdef.h"
#include "ftm.h"
#include "ftmdiag.h"
#include "rfcommon_fbrx_api.h"
#include "rfcommon_mdsp_types.h"
#include "ftm_log.h"
#include "math.h"
#include "rfm_mode_types.h"

#define FTM_COMMON_FBRX_RET_MAG_LSE(lse_r, lse_i) ( rflm_abs_satw(lse_r)>rflm_abs_satw(lse_i) ? rflm_abs_satw(lse_r) : rflm_abs_satw(lse_i))
#define RFCOMMON_FBRX_NUM_DATA_BUF 6
#define RFCOMMON_FBRX_DATA_BUF_SIZE 391
#define RFCOMMON_FBRX_DATA_BUF_SIZE_V2 800
#define RFCOMMON_FBRX_MAX_FILE_LENGTH 21
#define RFCOMMON_FBRX_CAL_DEBUG_THRESH 0x2000
#define RFCOMMON_FBRX_MAX_TXPWRMEAS_TIME_P1USEC 300

#define RFCOMMON_FBRX_MV10_NUMERATOR 0x2AF80000
#define RFCOMMON_FBRX_DIVNREF_SCALE 0x200000

#define FTM_FBRX_MAX_NUM_SC_DATA_BUF 136 /* Max number of sample capture buffers i.e. (16 ch + 1 for ubias/rxscale computation) * 2 modes * 4 gain states */
#define FTM_FBRX_CAPTURE_MAX_NUMBER_OF_BYTES_DIAG 3700
#define FTM_FBRX_SAMPLE_CAPTURE_VERSION 1

#define FTM_FBRX_RXSCALE_CAL_VAL 2048 /* Default RX SCALE to be used during FBRx Cal */
#define FTM_FBRX_SAMPLE_CAPTURE_RESULTS_MAX 2 
/*----------------------------------------------------------------------------*/
/*! Generic FBRx debug sample captures error return codes. */
typedef enum 
{
  FTM_FBRX_ERR_NONE                     = 0,
  FTM_FBRX_ERR_MEM_ALLOC_FAILED         = 1,  
  FTM_FBRX_ERR_API_FAILED               = 2,
} ftm_fbrx_err_t;

/*----------------------------------------------------------------------------*/
/*! FBRx action type */
typedef enum 
{
  FTM_FBRX_COMPUTE_UBIAS_RXSCALE                = 0,
  FTM_FBRX_USE_UBIAS_RXSCALE                    = 1,  
  FTM_FBRX_CALC_UBIAS_RXSCALE_BASED_ON_EXP_PWR  = 2,
  FTM_FBRX_MAX_SUPPORTED_ACTION
} ftm_fbrx_action_t;

/*----------------------------------------------------------------------------*/
/*! Generic FBRx debug command codes. */
typedef enum 
{
  FTM_FBRX_SET_BSC_THRESH_CONDITION             = 0,
  FTM_FBRX_SET_GAIN_EST_THRESH_CONDITION        = 1,
  FTM_FBRX_SET_GAIN_PA_STATE_THRESH_CONDITION   = 2,
  FTM_FBRX_SET_TX_RX_DELAY                      = 3,
  FTM_FBRX_SET_PROC_TYPE                        = 4,
  FTM_FBRX_MAX_SUPPORTED_COMMAND        
} ftm_fbrx_debug_cmd_t;

/*----------------------------------------------------------------------------*/
/*! This type specifies the data packet for the FBRX sample capture results log */
typedef PACK(struct)
{                           
 uint32 channel;   // UL channel                       
 uint8 gain_state;                        
 uint8 fbrx_mode;           
 boolean fbrx_sample_is_good;
 uint16 tx_sample_length;
 uint16 rx_sample_length;
 uint32 tx_samples[RFCOMMON_FBRX_DATA_BUF_SIZE_V2]; 
 uint32 rx_samples[RFCOMMON_FBRX_DATA_BUF_SIZE_V2];
}ftm_common_fbrx_debug_data_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold ftm_common_fbrx_debug_data_type info parameters */
typedef struct
{
  uint8   tech;
  uint8   band;
  uint8   max_num_valid_data_buf;   /* Max number of valid debug sample capture buffers */
  ftm_fbrx_err_t generic_err_code;  /* Generic error code to denote memory allocation failure */
  ftm_common_fbrx_debug_data_type* buffer_ptr[FTM_FBRX_MAX_NUM_SC_DATA_BUF]; /* sample capture data buffer pointer */
}ftm_common_fbrx_sample_captures_init_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to send out the debug samples */
typedef PACK(struct)
{
  uint8   cmd;
  uint8   sub_sys;
  uint16  mode;
  uint16  subsys_cmd;
  uint16  req_len;
  uint16  rsp_len;
  uint16  num_of_bytes;                             
  uint8   version;   /* ftm_common_fbrx_debug_type version */    
  uint8   tech;
  uint8   band;                        
  boolean is_last_packet;
  uint32  err_code;  /* Generic error code */     
  uint8   num_of_captures; /* number of valid sample captures of type ftm_common_fbrx_debug_type */                             
  uint8   capture_data[FTM_FBRX_CAPTURE_MAX_NUMBER_OF_BYTES_DIAG]; /* pointer to the fbrx debug type sample captures  to be returned by common fbrx */
} ftm_fbrx_debug_sample_captures_response_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_ENTER_MODE input parameters */
typedef PACK(struct)
{
  rfm_mode_enum_type tech;
} ftm_fbrx_enter_mode_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_INIT_TX input parameters */
typedef PACK(struct)
{
  rfcommon_fbrx_tx_cfg_type cfg;
  rfm_mode_enum_type tech;
  uint32 device;
  uint32 carrier_idx;
} ftm_fbrx_init_tx_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_ENABLE input parameters */
typedef PACK(struct)
{
  int8 reserved;
} ftm_fbrx_enable_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_EXIT input parameters */
typedef PACK(struct)
{
  rfm_mode_enum_type tech;
} ftm_fbrx_exit_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_ACTIVATE_CHAIN input parameters */
typedef PACK(struct)
{
  int8 reserved;
} ftm_fbrx_activate_chain_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_BRIDGE_CONFIG input parameters */
typedef PACK(struct)
{
  boolean enable;
} ftm_fbrx_bridge_config_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_SINGLE_TRIGGER input parameters */
typedef PACK(struct)
{
  uint8  tech; //rfm_mode_enum_type
  uint8  tx_device;
  uint8  carrier_idx;
  uint8  fbrx_gain_state;
  uint8  ubias;
  uint16 scale_rx;
} ftm_fbrx_single_trigger_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_RUN_CONTINUOUS input parameters */
typedef PACK(struct)
{
  uint8 tech; //rfm_mode_enum_type
  uint8  tx_device;
  uint8  carrier_idx;
  boolean enable;
} ftm_fbrx_run_control_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_DEBUG_CONTROL input parameters */
typedef PACK(struct)
{
  uint8     rfm_tech;               /* rfm_mode_enum_type tech id*/
  uint8     carrier_idx;            /* carrier index */
  uint8     fbrx_debug_cmd;         /* ftm_fbrx_debug_cmd_t */
  uint8     crash_en;               /* set to 1 to enable crash for debug cmd */
  uint16    crash_counter;          /* counter upon which crash cmd is send if crash_en == 1 */
  int16     low_txagc_thresh;       /* low txagc threshold  */
  int16     high_txagc_thresh;      /* high txagc threshold */
  uint8     meas_type;              /* FBRx Measurement request type enum rflm_fbrx_meas_req_type_e */
  uint8     run_type;               /* FBRx Run type enum rflm_fbrx_debug_run_type */
  int32     fbrx_debug_val_1;       /* fbrx debug control param 1 val */
  int32     fbrx_debug_val_2;       /* fbrx debug control param 2 val */
  int32     fbrx_debug_val_3;       /* fbrx debug control param 3 val */
  int32     fbrx_debug_val_4;       /* fbrx debug control param 4 val */
  int32     fbrx_debug_val_5;       /* fbrx debug control param 5 val */
} ftm_fbrx_debug_ctrl_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_ENABLE_RF input parameters */
typedef PACK(struct)
{
  uint8  tech; //rfm_mode_enum_type
  uint8  tx_device;
  uint8  carrier_idx;
  boolean enable;
  uint8   gain_state;
} ftm_fbrx_enable_rf_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold FTM_FBRX_STORE_RESULTS input parameters */
typedef PACK(struct)
{
  uint8 tech; //rfm_mode_enum_type
  uint8 device;
  uint8 carrier_idx;
  uint32 num_tx;
  uint32 num_rx;
} ftm_fbrx_store_results_param_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold output parameters */
typedef PACK(struct)
{
  uint8   cmd;
  uint8   sub_sys;
  uint16  mode;
  uint16  subsys_cmd;
  uint16  req_len;
  uint16  rsp_len;
  uint8   status;
  uint32  handle_id;
} ftm_fbrx_handle_response_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold store results output parameters */
typedef PACK(struct)
{
  uint8   cmd;
  uint8   sub_sys;
  uint16  mode;
  uint16  subsys_cmd;
  uint16  req_len;
  uint16  rsp_len;
  uint8   status;
  uint32   handle_id;
  uint16  store_data_counter;
} ftm_fbrx_store_results_response_type;

/*----------------------------------------------------------------------------*/
/*! Structure used to hold Single Trigger output parameters */
typedef PACK(struct)
{
  uint8   cmd;
  uint8   sub_sys;
  uint16  mode;
  uint16  subsys_cmd;
  uint16  req_len;
  uint16  rsp_len;
  uint8   status;
  uint8   tx_handle;
  int16   gain;
  uint32  ls_err;
  uint16  xcorr_log[RFLM_FBRX_XCORR_LOG_SIZE];
} ftm_fbrx_single_trigger_response_type;

typedef PACK(struct)
{
  uint8   cmd;
  uint8   sub_sys;
  uint16  mode;
  uint16  subsys_cmd;
  uint16  req_len;
  uint16  rsp_len;
  uint8   status;
  uint8   tx_handle;
  int16   gain;
  uint8   curr_ubias;
  uint16  curr_scale_rx;
  int16  cal_pwr;                 ///< tx power at which fbrx gain cal was done                            
  uint16 cal_rxscale;             ///< rx_scale value used during fbrx gain cal
  uint8  cal_ubias;               ///< ubias used during fbrx gain cal 
  uint16 last_cal_gain;           ///< fbrx cal gain value
  int16  last_temp_comp;          ///< temp comp value to apply
  uint32  ls_err;
  uint16  xcorr_log[RFLM_FBRX_XCORR_LOG_SIZE];
} ftm_fbrx_single_trigger_response_type_v2;

/*----------------------------------------------------------------------------*/
/*! ftm_common_fbrx_capture input parameters */
typedef struct
{
  uint8 tx_device_id;
  rfcommon_mdsp_xpt_mode_enum_type xpt_mode;
  /* Parameter used for equaling (1tau, tau) TA interpolator gain droop */
  int16 gain_comp;
  uint8 fbrx_gain_state;
  uint8   ubias;
  uint16  scale_rx;
  boolean store_capture_data;
  rfm_mode_enum_type tech;
  rfcom_band_type_u band;
} ftm_common_fbrx_capture_input_type;

/*----------------------------------------------------------------------------*/
/*! ftm_common_fbrx_capture input parameters */
typedef struct
{
  uint8 tx_device_id;
  uint8 fbrx_gain_state;
  /* FBRx action value: 
   0 = Perform UBias RxScale Computation.
   1 = Re-Use Ubias and RxScale.                                                                                             
   2 = ReCalculate Ubias, RxScale based on Exp Power.   
  */
  uint8 fbrx_action; 
  /* Store sample captures to heap if TRUE to enable FBRx debug mode */
  boolean store_capture_data;
  rfm_mode_enum_type tech;
  rfcom_band_type_u band;
} ftm_common_fbrx_capture_input_v2_type;

/*----------------------------------------------------------------------------*/
/*! ftm_common_fbrx_capture output parameters */
typedef struct
{
  rfcommon_fbrx_error_type status;
  int16          gain;
  uint32         loft; /* complex */
  uint32         rsb;  /* complex */
  int16          gain_imbalance;
  int16          phase_imbalance;
  uint32 ls_err;
  uint16 xcorr_log[RFLM_FBRX_XCORR_LOG_SIZE];
} ftm_common_fbrx_capture_result_type;

/*----------------------------------------------------------------------------*/
/*! ftm_common_fbrx_capture output parameters */
typedef struct
{
  uint8     fb_gain_state;
  uint8     fbrx_mode;

  int16     gain;
  int16     ubias;
  int16     rxscale;
  uint32    residual_dc; 

  uint32    rsb;
  int16     gain_imbalance;
  int16     phase_imbalance;

  uint32    tx_tx_conj_corr;
  uint32    tx_tx_corr;
  uint32    tx_sum;
  uint32    rx_tx_conj_corr;
  uint32    rx_tx_corr;
  uint32    rx_sum;
  uint32    rx_rx_conj_corr;
  uint32    rx_rx_corr;
  /* signal level */
  uint32    rms_sig_level;

  uint32    ls_err;
  uint16    xcorr_peak_val;
  uint8     xcorr_peak_idx;

  uint16 xcorr_log[RFLM_FBRX_XCORR_LOG_SIZE];
} ftm_common_fbrx_capture_autocal_results_type_t;

typedef struct
{
  uint8     num_of_captures;
  rfcommon_fbrx_error_type status;
  ftm_common_fbrx_capture_autocal_results_type_t results[FTM_FBRX_SAMPLE_CAPTURE_RESULTS_MAX];
} ftm_common_fbrx_capture_result_v2_type;

typedef struct
{
  boolean data_valid;
  uint16  rxscale;
  uint8   ubias;
}ftm_fbrx_autocal_type;

typedef struct
{
   ftm_fbrx_autocal_type fb_gs[RFCOMMON_FBRX_GAIN_VS_FREQ_NUM_V2];
}ftm_fbrx_gs_autocal_type;

typedef struct
{
   ftm_fbrx_gs_autocal_type fbrx_mode[RFCOMMON_FBRX_NUM_MODES];
}ftm_fbrx_mode_autocal_type;

typedef struct
{
  ftm_fbrx_mode_autocal_type cdma_data[RFM_CDMA_MAX_BAND];
  ftm_fbrx_mode_autocal_type wcdma_data[RFCOM_NUM_WCDMA_BANDS];
  ftm_fbrx_mode_autocal_type lte_data[RFCOMMON_FBRX_NV_CONTAINER_MAX][RFCOM_NUM_LTE_BANDS];
  ftm_fbrx_mode_autocal_type tds_data[RFCOM_NUM_TDSCDMA_BANDS];
}ftm_fbrx_autocal_data_init_type;

/*----------------------------------------------------------------------------*/
/*! This type specifies the data packet for the FBRX results log */
typedef PACK(struct)
{
 uint8  version;       /*!< Log packet version. As additional paramters are added the version number can increase */
 int16  txagc;         /*!< Current TxAGC value in dB10 */
 uint8  pa;            /*!< flag indicating the PA State */ 
 uint8  gs;            /*!< flag indicating the fbrx gain state (0,1,2 or 3) */
 int16  gn_est;        /*!< Current Gain estimate result */
 int16  gn_err;        /*!< The FBRx Gain Error in dB10 uint for current txagc */
 int16  mag_lse;       /*!< magnitude of the least squared error estimate result */
 int16  temp_comp;     /*!< Temp Comp value for current tech/band/chan/mode/gs */
 uint16 ubias;         /*!< Updated ubias value for current tech/band/chan/mode/gs */
 uint16 rxscale;       /*!< Updated Rx Scale value for current tech/band/chan/mode/gs */
 uint16 txscale;       /*!< Updated Tx Scale value for current tech/band/chan/mode/gs */
 int16  cal_pwr;       /*!< FBRx Cal Power for current gain state */
 int16  cal_gn;        /*!< FBRx Expected Cal Gain value */
 uint8  tech;          /*!< rflm tech id ( 0=1X, 1=HDR, 2=LTE, 3=WCDMA, 4=TDSCDMA, 5=GSM ) */
 uint8  band;          /*!< flag indicating the current Band of type (rfcom_band_type_u) */ 
 uint8  bandwidth;     /*!< rfcommon_fbrx_tx_cfg_type tech bandwidth */
 uint8  fbrx_mode;     /*!< flag indicating the fbrx mode (0,1,2 or 3) */
 uint8  slot;          /*!< flag indicating the slot index (0,1, or 2) per SF for current fbrx run */ 
 uint8  nv;            /*!< flag indicating the FBRX SYS common NV control value for current tech */ 
} ftm_common_fbrx_log_results_data_type;

/*----------------------------------------------------------------------------*/
/*! This type specifies the header/data packet for the FBRX results log */
typedef PACK(struct)
{
  ftm_log_gen_type      header;
  ftm_common_fbrx_log_results_data_type data;
} ftm_common_fbrx_log_results_t;

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif


/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_enter_mode(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_init_tx(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_enable(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_exit(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_activate_chain(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
#ifdef FEATURE_RF_HAS_WTR3925
extern boolean ftm_common_fbrx_reconfigure_conflict_chain(lm_handle_type rx_handle);
#endif

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_bridge_config(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_single_trigger(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_single_trigger_v2(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_run_control(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_set_debug_control_condition(ftm_common_pkt_type *cmd_ptr); 

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_enable_rf(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_debug_get_sample_captures(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_rsp_pkt_type ftm_common_fbrx_store_results(ftm_common_pkt_type *cmd_ptr);

/*----------------------------------------------------------------------------*/
extern ftm_common_fbrx_capture_result_type ftm_common_fbrx_capture_and_process
(
  ftm_common_fbrx_capture_input_type input
);

extern ftm_common_fbrx_capture_result_type ftm_mode_fbrx_single_trigger
(
  ftm_common_fbrx_capture_input_type input,
  int16  *cal_pwr,                 ///< tx power at which fbrx gain cal was done                            
  uint16 *cal_rxscale,            ///< rx_scale value used during fbrx gain cal
  uint8  *cal_ubias,              ///< ubias used during fbrx gain cal 
  uint16 *last_cal_gain,         ///< fbrx cal gain value
  int16  *last_temp_comp,
  uint8  *curr_ubias,
  uint16 *curr_scale_rx
);
/*----------------------------------------------------------------------------*/
extern ftm_common_fbrx_capture_result_v2_type ftm_common_fbrx_capture_and_process_v2
(
  ftm_common_fbrx_capture_input_v2_type input
);

/*----------------------------------------------------------------------------*/
extern void ftm_common_fbrx_lm_test( void );

/*----------------------------------------------------------------------------*/
extern rflm_handle_tx_t  
ftm_common_fbrx_get_valid_tx_handle( rfm_mode_enum_type tech, 
                                     rfm_device_enum_type device, 
                                     uint32 carrier_idx);

/*----------------------------------------------------------------------------*/
extern void ftm_common_fbrx_set_cal_state( boolean ftm_cal_mode );

/*----------------------------------------------------------------------------*/
extern uint8 ftm_common_fbrx_get_mode( rfm_mode_enum_type tech, 
                                       rfm_device_enum_type device, 
                                       uint32 carrier_idx );
/*----------------------------------------------------------------------------*/
extern void ftm_common_fbrx_log_results_init_func( void);

/*----------------------------------------------------------------------------*/
extern void ftm_common_fbrx_log_results_exit_func( void);

/*----------------------------------------------------------------------------*/
extern void ftm_common_fbrx_log_results_cb(int4 interval);

extern void ftm_common_fbrx_dealloc_mem_sample_capture_buf(void);
/*----------------------------------------------------------------------------*/
extern boolean ftm_common_fbrx_meas_ant_pwr( 
   int16 * pwr_est_db10, 
   int16 * pwr_est_db100, 
   uint8 num_averages, 
   uint8 * fbrx_gs );

/*----------------------------------------------------------------------------*/
extern ftm_fbrx_autocal_type* 
ftm_common_fbrx_get_autocal_init_data( rflm_fbrx_dm_template_t* fbrx_data_ptr, uint8 gs_idx);

/*----------------------------------------------------------------------------*/
extern uint16 ftm_common_fbrx_get_corr_setpoint( rfm_mode_enum_type tech);

/*----------------------------------------------------------------------------*/   
#ifdef __cplusplus
}
#endif

#endif /* FTMCOMMON_FBRX_H */
