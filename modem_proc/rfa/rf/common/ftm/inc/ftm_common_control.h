#ifndef FTM_COMMON_CONTROL_H
#define FTM_COMMON_CONTROL_H
/*!
  @file
  ftm_common_control.h

  @brief
  This module contains FTM common Control code.
*/

/*==============================================================================

  Copyright (c) 2011 - 2015 QUALCOMM Technologies, Inc. All Rights Reserved.

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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_common_control.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
01/27/15   zhh     Add the new API to send out the autoPin calc command
11/23/15   dw      Added ftm_wcdma_reset_device()
11/19/15   ak      Calibration support for Rx/Tx Split
08/03/15   zbz     Changes to support RFFE coupelr
06/16/15   yb      Added support for get tune code API
05/08/15   adk     Added API ftm_common_get_tuner_mgr_object
04/08/15   pkg     Changes to support Tx Rx Split Band design.
01/16/15   vv      Added support for ADC DC calibration
10/14/14   jmf     [selftest] Implement FBRx Noise Floor Cal
10/09/14   pl      Added FTM MCPM request co-ordination between ftm APIs and 
                   FTM protocol activation\deactivation 
07/23/14   dbz     Added support for selftest VSWR split capture
08/26/13   jr      Added new FTM command: FTM_GET_EFS_VERIFICATION_DATA
08/27/13   aka     1x and WCDMA updates
08/19/13   spa     Added support for SVDO enter mode
07/29/13   aka     removed ftm_common_calc_aclr API
07/23/13   aka     Added prototypes for FFT related types
07/15/13   aca     ftm_common_get_tuner_object update for multiple tuner objects
06/29/13   hm      Add new FTM command for tuner self cal: "FTM_DO_TUNER_SELF_CAL"
06/18/13   JJ      Remove declaration of register/de-register fw msgs funcs
06/18/13   JJ      Add register/de-register fw msgs funcs during 1x/hdr enter/exit
06/12/13   aca     ftm_common_get_tuner_device update for multiple tuner components
04/09/13   cri     Add init and deinit for cal data module
03/21/13  yzw      Add FTM cmd and device driver support to query RFFE device information
03/13/13   aca     Tuner characterization
02/28/13   hdz     Added ftm_common_npa_vote_for_modem_bus_clk
02/22/13   dw      Support for FTM common Cal routines without set mode
02/11/13   aca     DIME DAC cal-disable DC cal as it is not required
01/14/13   aca     DIME DAC cal NV update
10/29/12   aro     Interface to perform FTM tech to RFM tech mapping
05/16/12   vb      Added enum for dac_cal_rsp
04/05/12   vb      Added function to perform DAC cal only
03/18/12   aro     Common function to create response packet
01/17/12   aro     Added "New Mode" as a argument in FTM EXIT function
01/16/12   aro     Added common interface to get therm read from any device
                   for a given mode
09/25/11   vb      Support for FTM_DO_SELF_CAL
08/09/11   aro     Moved send FTM command from CDMA to FTM common
08/04/11   vb      Added FTM common Internal device calibration support
07/28/11   aro     Added iRAT support on FTM common control
06/21/11   aro     Added function to map FTM Mode to RF Mode
04/26/11   sty     Renamed ftm_state_enum_type to ftm_rfstate_enum_type
04/25/11   aro     Renamed FTM RFMODE to FTM State
03/28/11   aro     Renamed function to ftm_cdma_send_command
03/28/11   aro     [1] Changed ftm_cdma_create_ftm_q_msg() signature to return
                   flag indicating success and failure
                   [2] Added functionality to send FTM message from this func
03/28/11   aro     Fixes to create FTM Command Queue Message
03/28/11   aro     Added function to create FTM Queue message
03/24/11   aro     Renamed FTM Enter Mode function
03/14/11   aro     Doxygen documentation update
03/02/11   aro     Added LTE and TDS-CDMA mode enumerations
03/02/11   aro     Renamed FTM and FTM-NS mode enums
03/02/11   aro     Doxygen update
03/02/11   aro     Moved Set/Get Calibration state interface to RF
03/01/11   aro     Added return value for ftm exit Interface
02/16/11   aro     Added HDR CAL mode enum
02/15/11   aro     [1] Removed unused functions
                   [2] Changed mode enum type name
                   [3] Moved 1x enter and exit to 1x file
02/14/11   sty     Changed return type for ftm_rfmode_1x_cal
02/10/11   sty     Added C++ protection
02/02/11   aro     Added FTM CDMA Init
02/01/11   aro     Moved malloc function to FTM Common
01/15/11   aro     Initial Release

==============================================================================*/

#include "comdef.h"
#include "rfm_mode_types.h"
#include "ftmdiag.h"
#include "rfm_device_types.h"
#include "rfdevice_logical_component_types.h"
#include "npa.h"
#include "rfm_subscriber_types.h"
#include "ftm_rf_test_interface.h"
#include "rflte_ext_mc.h"


#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
/*! Maximum Q6 Speed in KHz */
#define RF_Q6_MAX_CPU_SPEED_KHZ NPA_MAX_STATE

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enumeration for all availaible RF Modes for FTM
*/
typedef enum
{

  FTM_STATE_UNKNOWN = -1,
  /*!< Unknown RF mode*/

  FTM_STATE_PARK,
  /*!< Parked RF Mode */

  FTM_STATE_1X,
  /*!< Mode to use for normal FTM or Calibration FTM in 1x */

  FTM_STATE_1X_NS,
  /*!< Mode to use for Non Signaling FTM in 1x  */

  FTM_STATE_1X_IRAT,
  /*!< Mode to use for FTM 1x iRAT  */

  FTM_STATE_HDR,
  /*!< Mode to use for normal FTM or Calibration FTM in HDR */

  FTM_STATE_HDR_NS,
  /*!< Mode to use for Non Signaling FTM in HDR  */

  FTM_STATE_HDR_IRAT,
  /*!< Mode to use for FTM HDR iRAT  */

  FTM_STATE_SVDO,
  /*!< Mode to use for FTM SVDO  */

  FTM_STATE_GSM,
  /*!< Mode to use for normal FTM or Calibration FTM in GSM */

  FTM_STATE_GSM_NS,
  /*!< Mode to use for Non Signaling FTM in GSM  */

  FTM_STATE_GSM_IRAT,
  /*!< Mode to use for FTM GSM iRAT  */

  FTM_STATE_LTE,
  /*!< Mode to use for normal FTM or Calibration FTM in LTE */

  FTM_STATE_LTE_NS,
  /*!< Mode to use for Non Signaling FTM in LTE */

  FTM_STATE_LTE_IRAT,
  /*!< Mode to use for FTM LTE iRAT  */

  FTM_STATE_TDSCDMA,
  /*!< Mode to use for normal FTM or Calibration FTM in TDS-CDMA */

  FTM_STATE_TDSCDMA_NS,
  /*!< Mode to use for Non Signaling FTM in TDS-CDMA  */

  FTM_STATE_TDSCDMA_IRAT,
  /*!< Mode to use for FTM TDSCDMA iRAT  */

  FTM_STATE_WCDMA,
  /*!< Mode to use for normal FTM or Calibration FTM in WCDMA */

  FTM_STATE_WCDMA_NS,
  /*!< Mode to use for Non Signaling FTM in WCDMA */

  FTM_STATE_WCDMA_IRAT,
  /*!< Mode to use for FTM WCDMA iRAT  */

  FTM_STATE_MAX
  /*!< Maximum number Modes */

} ftm_rfstate_enum_type;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Lists the FTM Common Command Reponse status.
*/
typedef enum
{

  FTM_COMMON_BAD_RSP  = 0,
  /*!< Bad Response Packet to diag */

  FTM_COMMON_GOOD_RSP = 1,
  /*!< Good Response Packet to diag */

  FTM_COMMON_NO_RSP   = 2
  /*!< No response packet to diag Response Packet */

} ftm_common_rsp_type;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Lists all the DAC Cal statuses.
*/
typedef enum
{

  FTM_COMMON_DAC_CAL_FAILURE  = 0,
  /*!< DAC Cal failure  */

  FTM_COMMON_DAC_CAL_NOT_ATTEMPTED = 1,
  /*!< DAC Cal not attempted */

  FTM_COMMON_DAC_CAL_SUCCESS = 2
  /*!< DAC Cal success */

} ftm_common_dac_cal_rsp_type;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Structure to hold the band and tech information for a given rf mode passed in from FTM task or tool side
*/
typedef PACK(struct)
{
	uint8 rf_tech;
	uint8 band;
}rf_tech_band_info;


/*----------------------------------------------------------------------------*/

typedef enum
{
  DAC_0_MSB_B_MSK  	= 0x1,
  DAC_0_DC_B_MSK  	= 0x2,
  DAC_0_ET_B_MSK		= 0x4,
  DAC_1_MSB_B_MSK 	= 0x8,
  DAC_1_DC_B_MSK		= 0x10,
  DAC_1_ET_B_MSK		= 0x20,
} ftm_common_dac_cal_bit_mask;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Status of the MCMPM resource request process
*/

typedef enum
{
  REQ_NONE = 0x00,
  /* MCPM Invalid resource request */  
  REQ_SKIPPED = 0x01,    
  /* MCPM resource request is skipped due to teh drplicate request */
  REQ_DONE = 0x02,
  /* MCPM resource request is processed successfully */
} ftm_rf_mcpm_resource_req_status;

/*----------------------------------------------------------------------------*/
typedef struct{
int16 aclr1_neg;
int16 aclr1_pos;
int16 aclr2_neg;
int16 aclr2_pos;
int16 aclr_eutra_neg;
int16 aclr_eutra_pos;
} ftm_common_aclr_result_struct_type;

/*----------------------------------------------------------------------------*/

typedef struct{
uint16  accum_count; 
int64 carrier_chan;
int64 utra1_neg;
int64 utra1_pos;
int64 utra2_neg;
int64 utra2_pos;
int64 eutra_neg;
int64 eutra_pos;
}ftm_common_power_per_bin_struct_type;
/*----------------------------------------------------------------------------*/

typedef struct{
int16 adj_neg;
int16 adj_pos;
int16 alt1_neg;
int16 alt1_pos;
int16 alt2_neg;
int16 alt2_pos;
} ftm_common_1x_aclr_result_struct_type;


/*----------------------------------------------------------------------------*/
void
ftm_vote_cpu_speed
(
  npa_resource_state req_freq
);

/*----------------------------------------------------------------------------*/
void
ftm_unvote_cpu_speed
(
  void
);

/*----------------------------------------------------------------------------*/
void
ftm_common_init
(
 void
);

/*----------------------------------------------------------------------------*/
boolean
ftm_rfmode_exit
(
  rfm_device_enum_type device,
  ftm_rfstate_enum_type new_mode
);

/*----------------------------------------------------------------------------*/
boolean
ftm_rfmode_enter
(
  rfm_device_enum_type device,
  ftm_rfstate_enum_type mode
);

/*----------------------------------------------------------------------------*/
ftm_rfstate_enum_type
ftm_get_current_state
(
  rfm_device_enum_type device
);

/*----------------------------------------------------------------------------*/
rfm_mode_enum_type
ftm_common_resolve_rf_mode
(
  ftm_mode_id_type mode
);

/*----------------------------------------------------------------------------*/
rfm_mode_enum_type
ftm_common_ftm_tech_to_rfm_mode
(
  ftm_rf_technology_type ftm_tech
);

/*----------------------------------------------------------------------------*/
rfm_subscriber_type 
ftm_common_convert_to_subscriber
(
  ftm_rf_test_subscriber_enum_type ftm_subscriber
);

/*----------------------------------------------------------------------------*/
ftm_rsp_pkt_type
ftm_common_create_rsp_pkt
(
  ftm_common_rsp_type status
);

/*----------------------------------------------------------------------------*/
ftm_rsp_pkt_type
ftm_common_malloc_rsp_pkt
(
  void * ftm_req_data,
  uint16 rsp_pkt_size
);

/*----------------------------------------------------------------------------*/
boolean
ftm_common_send_radio_command
(
  void *cmd_param,
  uint32 param_size
);

/*----------------------------------------------------------------------------*/
boolean
ftm_common_self_calibration
(
  void
);

/*----------------------------------------------------------------------------*/
boolean
ftm_common_get_efs_verifcation_data
(
  void
);

/*-----------------------------NOK_Hsinih-------------------------------------*/
boolean
ftm_common_tuner_self_calibration
(
  void
);

/*----------------------------------------------------------------------------*/
boolean
ftm_common_dac_calibration
(
  void
);

/*----------------------------------------------------------------------------*/
boolean
ftm_common_adc_calibration
(
  void
);

/*----------------------------------------------------------------------------*/
ftm_rsp_pkt_type
ftm_common_get_thermistor_read
(
  void *ftm_req_data
);

/*----------------------------------------------------------------------------*/
boolean
ftm_common_resource_config_request
(
  boolean enable
);

/*----------------------------------------------------------------------------*/
void
ftm_common_npa_vote_for_modem_bus_clk
(
   uint32 bus_clk
);
/*----------------------------------------------------------------------------*/

boolean ftm_common_set_tuner_hdet_gain(void* hdet_obj, uint8 hdet_pid, uint8 gain_state);

/*----------------------------------------------------------------------------*/
uint16 ftm_common_get_tuner_hdet_power(void* hdet_obj, uint8 hdet_pid);

/*----------------------------------------------------------------------------*/

void* ftm_common_get_tuner_object(rfm_device_enum_type device, rfdevice_type_enum_type device_type);

/*----------------------------------------------------------------------------*/

void** ftm_common_get_tuner_device_object_list
(
  rfm_device_enum_type rfm_device, 
  ftm_rfstate_enum_type    tech,
  ftm_mode_id_type         mode_band,
  uint8                    device_type
);

/*----------------------------------------------------------------------------*/

ftm_rsp_pkt_type
ftm_common_process_get_tune_code
(
  ftm_pkt_type          *ftm_req_data,
  rfm_device_enum_type  rfm_dev_index,
  ftm_rfstate_enum_type tech, 
  ftm_mode_id_type      mode_band,
  uint8                 device_type
);

/*----------------------------------------------------------------------------*/

void* ftm_common_get_tuner_mgr_object( rfm_device_enum_type device, ftm_rfstate_enum_type tech);

/*----------------------------------------------------------------------------*/
boolean
ftm_common_get_band_tech_from_rf_mode
(
  ftm_mode_id_type rf_mode,
  uint8 * band,
  ftm_rf_technology_type *rf_tech
);

void
ftm_common_set_fb_path_state
(
  void* ftm_req_data,
  ftm_rf_technology_type ftm_rf_tech
);

/*----------------------------------------------------------------------------*/
ftm_rf_mcpm_resource_req_status 
ftm_common_resource_config_mcpm
(
  boolean enable
);

/*----------------------------------------------------------------------------*/

boolean ftm_lte_deallocate_all_lm_buffers
(
  rflte_mc_cell_idx_type carrier_idx
);

/*----------------------------------------------------------------------------*/
rfm_device_enum_type
ftm_common_get_preferred_tx_device
(
  uint8 band,
  ftm_rf_technology_type rf_tech
);
/*----------------------------------------------------------------------------*/
void ftm_wcdma_reset_device (rfm_device_enum_type device);

/*----------------------------------------------------------------------------*/
void
ftm_common_send_calc_delta_pin_pout_cmd
(
  void* ftm_req_data
);

#ifdef __cplusplus
}
#endif

#endif /* FTM_COMMON_CONTROL_H */
