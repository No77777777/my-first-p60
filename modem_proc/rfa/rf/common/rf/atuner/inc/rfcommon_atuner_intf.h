#ifndef RFCOMMON_ATUNER_INTF_H
#define RFCOMMON_ATUNER_INTF_H
/*!
  @file
  rfcommon_atuner_intf.h
  
  @brief
  Declaration of antenna tuner interface class.
  This header file contains all type common logic and implementation related to
  antenna tuner (atuner).
  This file the provides the interfaces for all the antenna tuner
  configuration supported in the current RF card and all the clients/callers
  (like mc/core/ftm etc) should use the APIs defined in this file
  to interface with any antenna tuner functionality.
  The APIs provided in this file are the interface APIs to program the
  tuner device available on the phone.
  
*/

/*===========================================================================

  Copyright (c) 2013-2018 Qualcomm Technologies, Inc. All Rights Reserved.

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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/rf/atuner/inc/rfcommon_atuner_intf.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
11/05/15   ndb     Added new states for detune framework
11/23/15   yb      Add API to switch LM handle
09/02/15   aak     Add ant num and freq as fields in rfcommon_atuner_config_req_type
06/21/15   yb      Added IRAT APIs
06/12/15   yb      Put back CA Port enum
06/12/15   yb      Cleaning up CA Port enum, Adding enum for Single Band Dual Carrier 
                   - single Tune
05/29/15   ndb     Added a new command "RFCOMMON_ATUNER_CMD_IRAT_TUNE_AWAY"
                   and cleaned-up the un-used state/interfaces
05/27/15   ndb     Fix the Compiler Error
05/25/15   ndb     Remove the un-used interfaces & Update to the interface tune_code_override()
05/20/15   yb      Tuner optimization
05/08/15   adk     Added FTM API to query CL algo state
04/17/15   ndb     Interface update due to the dyanmic RFM device requirement
                   Stubbed out the old interfaces + Merged the interfaces
04/17/15   ndb     Interface update due to the dyanmic RFM device requirement
11/18/14   aca     AOL helper function
10/21/14   yb      Added priority configuratoin for CRAT scenarios
10/07/14   ndb     Added 3DL CA support(Phase 2)
08/20/14   ndb     Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
07/28/14   yb      MCL feature: APIs added - stop_mcl
07/25/14   yb      MCL feature: APIs added - set_scenario_mcl
07/23/14   yb      MCL feature: APIs added - get_scenario_list
3/26/14    kg      Added support for LM handle 
11/29/13   yb      Removed low power API
10/28/13   vb      Support for force_detune()
09/27/13   yb      Added tune_code_override API
08/06/13   vb      Optimized update_tune_code() 
07/03/13   hm      Added tuner_low_power API
06/27/13   ndb     Added rfcommon_nv_ant_tuner_cl_ctrl_type in tuner tx mode 
06/27/13   ndb     Added rfcommon_nv_ant_tuner_cl_ctrl_type in tuner tx mode 
06/20/13   vb      Test case for CL call back registration function
06/12/13   vb      Added new method tx_disable()
05/25/13   vb      ATUNER_INVALID_TOKEN_VAL support
05/02/13   vb      init version.
============================================================================*/


#ifdef __cplusplus
extern "C"
{
#endif
#include "rfdevice_cmn_type_defs.h"
#include "rfcommon_nv.h"
#include "rfdevice_antenna_tuner.h"
#ifdef __cplusplus
}
#endif

//Safe C++ headers
#include "rf_buffer_intf.h"

#define RFCMN_ATUNER_INVALID_FREQ 0xFFFFFFFF
#define RFCMN_ATUNER_INVALID_CHAN 0xFFFFFFFF

/*!
  @brief
  Max size of each tune code value. 

  This is the max size (in bytes)of each tune code value that is currently supported.
*/
#define ATUNER_MGR_MAX_TUNE_CODE_SIZE 8


/*!
  @brief
  Carrier_ID Enum indicates which carrier is getting programmed.
*/
typedef enum
{
  RFCMN_ATUNER_CARRIER_ID_0 = 0,          /* Enum Indicating First Carrier programming */
  RFCMN_ATUNER_CARRIER_ID_0_PLUS_1 = RFCMN_ATUNER_CARRIER_ID_0,          /* Enum Indicating Single Band Dual Carrier - single Tune */
  RFCMN_ATUNER_CARRIER_ID_1 = 1,              /* Enum Indicating Second Carrier programming */
  RFCMN_ATUNER_CARRIER_ID_2 = 2,              /* Enum Indicating Third Carrier programming */
  RFCMN_ATUNER_CARRIER_ID_3 = 3,              /* Enum Indicating Fourth Carrier programming */
  RFCMN_ATUNER_CARRIER_ID_4 = 4,              /* Enum Indicating Fifth Carrier programming */
  RFCMN_ATUNER_CARRIER_ID_MAX,
  RFCMN_ATUNER_CARRIER_ID_INVALID= 0xFF,  /* Enum Indicating Invalid Carrier programming */
} rfcommon_atuner_carrier_id_type;

#define RFCMN_ATUNER_MAX_NUM_CHANNELS RFCMN_ATUNER_CARRIER_ID_MAX*2


/*!
  @brief
  Device type enum indicates the type of device 
*/
typedef enum
{
  RFCMN_ATUNER_DRX_DEVICE,         /* Enum Indicating Tuner device is on DRx */
  RFCMN_ATUNER_PRX_DEVICE,             /* Enum Indicating Tuner device is on PRx */
  RFCMN_ATUNER_TX_DEVICE,              /* Enum Indicating Tuner device is on Tx */
  RFCMN_ATUNER_MAX_DEVICE,
  RFCMN_ATUNER_INVALID_DEVICE  = 0xFF  /* Enum Indicating Tuner device is on Invalid type */
} rfcommon_atuner_device_type;


//Enum for the different tuner states
typedef enum
{
  RFCOMMON_ATUNER_STATE_INIT,
  RFCOMMON_ATUNER_STATE_RX_MODE,
  RFCOMMON_ATUNER_STATE_TX_MODE,
  RFCOMMON_ATUNER_STATE_DISABLE,
  RFCOMMON_ATUNER_STATE_RX_DETUNE_MODE,
  RFCOMMON_ATUNER_STATE_TX_DETUNE_MODE,
  RFCOMMON_ATUNER_STATE_MAX
} rfcommon_atuner_state_type;


//Enum for the different tuner commands
typedef enum
{
  RFCOMMON_ATUNER_CMD_INIT,
  RFCOMMON_ATUNER_CMD_RX_MODE,
  RFCOMMON_ATUNER_CMD_TX_MODE,
  RFCOMMON_ATUNER_CMD_TX_DISABLE,
  RFCOMMON_ATUNER_CMD_DISABLE,
  RFCOMMON_ATUNER_CMD_PROGRAM_TUNE_CODE,
  RFCOMMON_ATUNER_CMD_ASDIV_GET,
  RFCOMMON_ATUNER_CMD_ASDIV_SET,
  RFCOMMON_ATUNER_CMD_IRAT_TUNE_AWAY,
  RFCOMMON_ATUNER_CMD_IRAT_TUNE_BACK,
  RFCOMMON_ATUNER_CMD_INVALID  = 0xFF
} rfcommon_atuner_cmd_enum_type;

typedef enum
{
  RFCOMMON_ATUNER_MAAT_ALGO_PCC_TX_ONLY = 0,
  RFCOMMON_ATUNER_MAAT_ALGO_PCC_RX_ONLY = 1,
  RFCOMMON_ATUNER_MAAT_ALGO_PCC_TX_AND_RX = 2 ,
  RFCOMMON_ATUNER_MAAT_ALGO_SCC_TX_ONLY = 3,
  RFCOMMON_ATUNER_MAAT_ALGO_SCC_RX_ONLY = 4,
  RFCOMMON_ATUNER_MAAT_ALGO_BALANCE_ALL_RX = 5,
  RFCOMMON_ATUNER_MAAT_ALGO_MAX = 6
}rfcommon_atuner_maat_algo_type;


/*!
  @brief
  Atuner Config Req Type
 */
typedef struct
{
  rfm_device_enum_type             rfm_device;
  rfcom_mode_enum_type             mode;
  rfcommon_atuner_carrier_id_type  carrier_id;
  rfcommon_atuner_device_type      rx_tx;
  rfcom_band_type_u                band;
  uint32                           chan_num;
  rfcommon_atuner_cmd_enum_type    cmd;
  uint32                           ant_num;
  uint32                           freq;
} rfcommon_atuner_config_req_type;


/*!
  @brief
  Structure to hold the CL NV data & LM handle 
*/
typedef struct
{
  rfcommon_nv_ant_tuner_cl_ctrl_type *cl_nv_ctrl_info;
  uint32 lm_handle;
} rfcommon_atuner_cl_ctrl_info_type;

/*!
  @brief
  Token type for all the tuner scripts being provided
*/
typedef uint32 rfcommon_atuner_script_token_type;

/*!
  @brief
  Invalid token value. 

  If Tuner APIs return this token value, then the callers need not ack and assume
  that no tuner device script is provided part of the call.
*/
#define ATUNER_INVALID_TOKEN_VAL 0xFFFFFFFF


/*!
  @brief
   Enum to capture the return status of the tune code override FTM command:"FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
  
  @detail
*/
typedef enum
{
  RFCMN_ATUNER_OVERRIDE_TUNE_CODE_SUCCESS,
  RFCMN_ATUNER_NOT_PRESENT,
  RFCMN_ATUNER_TUNE_CODE_FAILURE,
  RFCMN_ATUNER_ERROR = 0xFFFFFFFF
} rfcommon_atuner_override_status_type;

/*TODO:::TO BE CLEANED-UP ONCE ALL TECH MOVED TO NEW INTERFACES*/
typedef uint32 script_token_type;

/*TODO:::TO BE CLEANED-UP ONCE ALL TECH MOVED TO NEW INTERFACES*/
typedef void (*rf_cl_event_register_cb_fn)( void *class_obj_ptr );


#ifdef __cplusplus

/* These header files are only valid for C++ compilation */
#include "rfa.h"
#include "rfdevice_class.h"

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Interface for antenna tuner class that contains all the APIs for both of
  Advanced Open Loop (AOL) and Closed Loop (CL) algorithms of antenna tuner.
 
  @details
*/
class rfcommon_atuner_intf : public rfdevice_class
{
public:

  /*! Empty virtual destructor - base, interface class has no data so nothing 
  to delete */
  virtual ~rfcommon_atuner_intf()
  { }

  /* Initializes the Tuner */
  virtual boolean init( rfcommon_atuner_config_req_type* atuner_params_p,
                        rfcommon_atuner_script_token_type *script_token,
                        rf_device_execution_type dev_action,
                        rf_buffer_intf *script_buffer,
                        int16 script_timing ) = 0;

  /* Programs the associated antenna tuner in Rx only mode and 
     applies best possible tune code for Rx only mode */
  virtual boolean set_rx_mode( rfcommon_atuner_config_req_type* atuner_params_p,
                               rfcommon_atuner_script_token_type *script_token,
                               rf_device_execution_type dev_action,
                               rf_buffer_intf *script_buffer,
                               int16 script_timing ) = 0;

  /* Programs the associated tuner in Tx mode and
     applies best possible tune code for Tx mode */
  virtual boolean set_tx_mode( rfcommon_atuner_config_req_type* atuner_params_p,
                               rfcommon_atuner_cl_ctrl_info_type *cl_ctrl_info,
                               rfcommon_atuner_script_token_type *script_token,
                               rf_device_execution_type dev_action,
                               rf_buffer_intf *script_buffer,
                               int16 script_timing ) = 0;

  /* Update the tune code every 200ms */
  virtual boolean update_tune_code( rfcommon_atuner_config_req_type* atuner_params_p,
                                    rfcommon_atuner_script_token_type *script_token,
                                    rf_device_execution_type dev_action,
                                    rf_buffer_intf *script_buffer,
                                    int16 script_timing ) { return TRUE; }
 
  /* Update the tune code every 200ms */
  
  virtual boolean force_detune ( rfcommon_atuner_config_req_type* atuner_params_p,
                                 rfcommon_atuner_script_token_type *script_token,
                                 rf_device_execution_type dev_action,
                                 rf_buffer_intf *script_buffer,
                                 int16 script_timing ) {return TRUE; }

  /* Acknowlegement for the script previously provided */
  virtual boolean acknowledge_script( rfcommon_atuner_script_token_type script_token ) {return TRUE; }


  /* Disables the tuner Tx mode */
  virtual boolean tx_disable( rfcommon_atuner_config_req_type* atuner_params_p,
                              rfcommon_atuner_script_token_type *script_token,
                              rf_device_execution_type dev_action,
                              rf_buffer_intf *script_buffer,
                              int16 script_timing ) = 0;

  /* Disables the associated tuner */
  virtual boolean disable( rfcommon_atuner_config_req_type* atuner_params_p,
                           rfcommon_atuner_script_token_type *script_token,
                           rf_device_execution_type dev_action,
                           rf_buffer_intf *script_buffer,
                           int16 script_timing ) = 0;

  /* Programs the tuner in tune-away for IRAT and QTA scenarios */
  virtual boolean tune_away( rfcommon_atuner_config_req_type* atuner_params_p,
                           rfcommon_atuner_script_token_type *script_token,
                           rf_device_execution_type dev_action,
                           rf_buffer_intf *script_buffer,
                           int16 script_timing ) = 0;

  /* Programs the tuner in tune-back for IRAT and QTA scenarios */
  virtual boolean tune_back( rfcommon_atuner_config_req_type* atuner_params_p,
                           rfcommon_atuner_script_token_type *script_token,
                           rf_device_execution_type dev_action,
                           rf_buffer_intf *script_buffer,
                           int16 script_timing ) = 0;

  /* API for getting scenario list for given tech and band */
  virtual boolean get_scenario_list( rfcommon_atuner_config_req_type* atuner_params_p,
                                     rfm_antenna_tuner_scenario_data *scenario_data,
                                     uint8 *free_space_index,
                                     rfm_tuner_handle_type *tuner_handle ) { return FALSE; };

  /* API to set scenario for mcl */
  virtual boolean set_scenario_mcl( rfcommon_atuner_config_req_type* atuner_params_p,
                                    uint8 scenario_val,
                                    rfm_tuner_handle_type tuner_handle ) {return TRUE; }

  /* API to stop mcl */
  virtual boolean stop_mcl( rfcommon_atuner_config_req_type* atuner_params_p,
                            rfm_tuner_handle_type tuner_handle ) { return TRUE; };

  /*API to do Tune code overide */
  virtual rfdevice_antenna_tuner_device_override_status_type 
    tune_code_override ( uint8  override_flag, void* data, uint8* tuner_nv_ptr, 
                         uint8* tuner_id_ptr ) {return RFDEVICE_ANTENNA_TUNER_ERROR; }

  /* API to retrieve CL algorithm state: Enabled/Disabled (T/F) */
  virtual boolean get_cl_algo_state() {return FALSE; }

  /* API to switch lm handles */
  virtual boolean switch_lm_handle( uint32 old_handle,
                                    uint32 new_handle) { return TRUE; };

protected:

private:

};

#else

struct rfcommon_atuner_s;
typedef struct rfcommon_atuner_s rfcommon_atuner_intf;

#endif /* #ifdef __cplusplus */

#ifdef __cplusplus
extern "C"
{
#endif
/*----------------------------------------------------------------------------*/
           /*Start of New Tuner Interfaces*/
/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_init
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_set_rx_mode
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_set_tx_mode
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_cl_ctrl_info_type *cl_ctrl_info,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_update_tune_code
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_force_detune
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_acknowledge_script
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_script_token_type script_token
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_tx_disable
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_disable
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_tune_away
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_tune_back
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfcommon_atuner_script_token_type *script_token,
  rf_device_execution_type dev_action,
  rf_buffer_intf *script_buffer,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
extern rfdevice_antenna_tuner_device_override_status_type rfcommon_atuner_tunecode_override
(
  rfcommon_atuner_intf *atuner_ptr,
  uint8  override_flag,
  void*  data,
  uint8* tuner_nv_ptr,
  uint8* tuner_id_ptr
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_get_cl_algo_state
(
  rfcommon_atuner_intf *atuner_ptr
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_get_scenario_list
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfm_antenna_tuner_scenario_data *scenario_data,
  uint8 *free_space_index,
  rfm_tuner_handle_type *tuner_handle
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_set_scenario_mcl 
(
  rfcommon_atuner_intf *atuner_ptr, 
  rfcommon_atuner_config_req_type *atuner_config_p,
  uint8 scenario_val,
  rfm_tuner_handle_type tuner_handle
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_atuner_stop_mcl 
(
  rfcommon_atuner_intf *atuner_ptr,
  rfcommon_atuner_config_req_type *atuner_config_p,
  rfm_tuner_handle_type tuner_handle
);

/*----------------------------------------------------------------------------*/
boolean rfcommon_atuner_is_update_required();

/*----------------------------------------------------------------------------*/
boolean rfcommon_atuner_switch_lm_handle
(
  rfcommon_atuner_intf *atuner_ptr,
  uint32 old_handle,
  uint32 new_handle
);

/*----------------------------------------------------------------------------*/
           /*End of New Tuner Interfaces*/
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif /* RFCOMMON_ATUNER_INTF_H */
