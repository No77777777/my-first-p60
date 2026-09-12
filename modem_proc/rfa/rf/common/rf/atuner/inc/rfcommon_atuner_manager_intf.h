#ifndef RFCOMMON_ATUNER_MANAGER_INTF_H
#define RFCOMMON_ATUNER_MANAGER_INTF_H 
/*!
  @file
  rfcommon_atuner_manager_intf.h
  
  @brief
  Declaration of antenna tuner manager interface class.
  This header file contains all type common logic and implementation related to
  antenna tuner (atuner).
  This implementation supports all the tuner configurations with and without the CA
  configuration. 
  
*/

/*===========================================================================

  Copyright (c) 2013-2016 Qualcomm Technologies, Inc. All Rights Reserved.

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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/rf/atuner/inc/rfcommon_atuner_manager_intf.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
03/21/16   hm      Fix ASDIV - Tuner critical section deadlock
10/26/15   ndb     Added Tuner System Manager States
10/17/15   yb      Moved return loss meas type
06/20/15   yb      Added support for conflict cases: PRx_PRx, DRx_DRx, Tx_Tx
06/16/15   yb      Added support for get tune code API
05/29/15   ndb     Include the file "rfcommon_atuner_manager_factory.h"
05/27/15   ndb     Update the interface tune code override
05/20/15   yb      Tuner optimization
05/08/15   adk     Added FTM API to query CL algo state
04/21/15   ndb     Removal of CA_PORT (dyanmic RFM device requirement)
02/26/15   ndb     Removed the RFCOMMON_ATUNER_STATE_UNKNOWN
01/13/15   yb      Added support for GPS sharing with WWAN
10/21/14   yb      Added priority configuratoin for CRAT scenarios
10/07/14   ndb     Added 3DL CA support(Phase 2)
09/22/14   ndb     Added "rfcommon_atuner_num_active_carriers_type" for Multi-Carrier System
08/20/14   ndb     Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
07/28/14   yb      MCL feature: APIs added - stop_mcl
07/25/14   yb      MCL feature: APIs added - set_scenario_mcl
07/23/14   yb      MCL feature: APIs added - get_scenario_list
05/20/14   yb      Change the Flag usage in the API program_nvg_detune_tuner
05/20/14   yb      Added API's get_port_status(), program_nvg_detune_tuner() & 
                   control_cl_timers()
3/26/14    kg      Added support for LM handle 
03/14/14   kg      Port from Dime
03/10/14   as      Added interface to set update_tune_code_flag
02/18/14   ndb     Added API's set_tuner_config_info() & get_tuner_config_info
02/11/14   vb      Re-design asdiv_tuner to support multiple RF configurations
01/28/14   ndb     Added Atuner operation modes to support for SGLTE/SVLTE/CA
11/29/13   yb      Removed low power API
10/29/13  yb/ndb  Added the support for dynamic tune code size
10/28/13   vb      Support for force_detune()
09/27/13   yb      Added tune_code_override API
09/03/13   ndb     Corrected "RFCOMMON_ATUNER_STATE_LOWPOWER" enum
08/14/13   pl      Added an interface to allow access for port state
08/06/13   vb      Optimized update_tune_code()  
07/18/13   hm      Added tuner_low_power API
07/12/13   aca     Tuner manager cmd update
07/01/13   aca     Over ride support
06/27/13   ndb     Added rfcommon_nv_ant_tuner_cl_ctrl_type in tuner tx mode 
06/20/13   vb      Added API for registering a call back to the Tuner manager
06/15/13   ndb     Tuner Commands that RF APPS task will receive
06/12/13   vb      Added new method tx_disable()
05/25/13   vb      ATUNER_INVALID_TOKEN_VAL support
05/02/13   vb      init version.
============================================================================*/


#ifdef __cplusplus
extern "C"
{
#endif
#include "rfdevice_cmn_type_defs.h"
#ifdef __cplusplus
}
#endif

//Safe C++ headers
#include "rf_buffer_intf.h"
#include "rfcommon_atuner_intf.h"
#include "rfdevice_antenna_tuner.h"
#include "rfdevice_msg_log.h"


#define RFCOMMON_ATUNER_NV_DEBUG 1

/*----------------------------------------------------------------------------*/
/* The below enums define the state of the tuner system manager
*/
typedef enum
{
  RFCOMMON_ATUNER_SYSTEM_OFF_STATE,
  RFCOMMON_ATUNER_SYSTEM_RX_STATE, 
  RFCOMMON_ATUNER_SYSTEM_TX_STATE,
  RFCOMMON_ATUNER_SYSTEM_MAX_STATE, 
  RFCOMMON_ATUNER_SYSTEM_INVALID_STATE = 0xFF, 
}rfcommon_atuner_system_state_type, rfcommon_atuner_system_manager_state_type;

/*----------------------------------------------------------------------------*/
/* The below enums define the algo type for impedance tuners
*/
typedef enum
{
  RFCOMMON_ATUNER_TX_IMPEDANCE_ALGO_OL,
  RFCOMMON_ATUNER_TX_IMPEDANCE_ALGO_AOL,
  RFCOMMON_ATUNER_TX_IMPEDANCE_ALGO_ACL,
  RFCOMMON_ATUNER_TX_IMPEDANCE_ALGO_MAAT,
  RFCOMMON_ATUNER_TX_IMPEDANCE_ALGO_MAX
}rfcommon_atuner_tx_impedance_algo_type;

/*----------------------------------------------------------------------------*/
/* The below enums define the algo type for aperture tuners
*/
typedef enum
{
  RFCOMMON_ATUNER_TX_APERTURE_ALGO_OL,
  RFCOMMON_ATUNER_TX_APERTURE_ALGO_AOL,
  RFCOMMON_ATUNER_TX_APERTURE_ALGO_ACL,
  RFCOMMON_ATUNER_TX_APERTURE_ALGO_MAX
}rfcommon_atuner_tx_aperture_algo_type;

/*----------------------------------------------------------------------------*/
/* The below enums define the algo type for aperture tuners
*/
typedef enum
{
  RFCOMMON_ATUNER_RX_ALGO_OL,
  RFCOMMON_ATUNER_RX_ALGO_AOL,
  RFCOMMON_ATUNER_RX_ALGO_MAAT,
  RFCOMMON_ATUNER_RX_ALGO_OL_ICL,
  RFCOMMON_ATUNER_RX_ALGO_AOL_ICL,
  RFCOMMON_ATUNER_RX_ALGO_MAAT_ICL,
  RFCOMMON_ATUNER_RX_ALGO_MAX
}rfcommon_atuner_rx_algo_type;


/*----------------------------------------------------------------------------*/
/*
  @brief
  This enum defines all cmds that need rf_apps_task to dispatch. To add a new
  cmd to rf_apps_task, one need to add a new cmd in this enum, and then add
  the corresponding cmd handler in rf_apps_cmd_dispatch().
*/
typedef enum
{

  /*Return loss timer expiry  cmd*/
  RFCOMMOM_ATUNER_APPS_TASK_RL_TIMER_EXPIRY_CMD,

  /* End of adding new cmd*/
  RFCOMMOM_ATUNER_APPS_TASK_MAX_CMD,

}rfcommon_atuner_apps_task_cmd_enum_type;
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Default value to be used when ever data is not available
*/
#define ATUNER_MGR_AOL_DEFAULT_VAL 0

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Invalid scenario used for initializing
*/
#define ATUNER_MGR_AOL_INVALID_SCENARIO_VAL 128

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Invalid tune code used for initializing (per byte entry)
*/
#define ATUNER_MGR_AOL_INVALID_TUNE_CODE 0xFF

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Default size of each tune code in bytes
*/
#define ATUNER_MGR_DEFAULT_TUNE_CODE_SIZE 2

typedef enum
{
  RETURN_LOSS_MEAS_ECL =1,
#ifdef FEATURE_RFA_ACL
  RETURN_LOSS_MEAS_ACL =2,
#endif /* FEATURE_RFA_ACL */
  RETURN_LOSS_MEAS_MAX = 0xFF
}return_loss_meas_type;


/* Supported commands by Tuner manager dispatch */
typedef enum
{
  RFCOMMON_ATUNER_OVER_RIDE_RL,
  RFCOMMON_ATUNER_OVER_RIDE_CL,  
  RFCOMMON_ATUNER_OVER_RIDE_INVALID
} rfcommon_atuner_cmd_type;

 #ifdef FEATURE_RFA_ACL
/* MAAT related transitions */
typedef enum
{
  RFCMN_ATUNER_MAAT_TRANSITION_ICL_ENABLE = 0,
  RFCMN_ATUNER_MAAT_TRANSITION_ICL_DISABLE = 1,
  RFCMN_ATUNER_MAAT_TRANSITION_TX_DISABLE = 2,
  RFCMN_ATUNER_MAAT_TRANSITION_CA_UPDATE_ACL_ENABLE = 3,//Same as Tx Enable as well
  RFCMN_ATUNER_MAAT_TRANSITION_CA_UPDATE_ACL_DISABLE = 4,
  RFCMN_ATUNER_MAAT_TRANSITION_AOL_CHANGE = 5,
  RFCMN_ATUNER_MAAT_TRANSITION_ASDIV_TX_TO_RX = 6,
  RFCMN_ATUNER_MAAT_TRANSITION_ASDIV_RX_TO_TX = 7,
  RFCMN_ATUNER_MAAT_TRANSITION_VSWR_UPDATE_AVAILABLE = 8,
  RFCMN_ATUNER_MAAT_TRANSITION_VSWR_UPDATE_NOT_AVAILABLE = 9,
  RFCMN_ATUNER_MAAT_TRANSITION_SLEEP = 10,
  RFCMN_ATUNER_MAAT_TRANSITION_CHANNEL_CHANGE = 11,
  RFCMN_ATUNER_MAAT_TRANSITION_MAX = 0xFF
}rfcmn_atuner_maat_transition_enum_type;
#endif /* FEATURE_RFA_ACL */

/* NV data */
typedef struct
{
  boolean enable_aol;
  boolean enable_cl;
  boolean enable_maat;
  boolean enable_aperture_aol;
  boolean enable_aperture_acl;
  boolean enable_icl;
  boolean rx_enable_aol;
  boolean rx_enable_maat;
  rfm_mode_enum_type mode;
  rfcom_band_type_u band;
  rfnv_atuner_static_settings_type static_params;
  uint64 multi_carrier_cl_mask;
} return_loss_nv_data_type;


typedef enum
{
  RFCOMMON_ATUNER_MANAGER_PRIORITY_TX = 0,
  RFCOMMON_ATUNER_MANAGER_PRIORITY_PRX,
  RFCOMMON_ATUNER_MANAGER_PRIORITY_DRX,
  RFCOMMON_ATUNER_MANAGER_PRIORITY_MAX,
  RFCOMMON_ATUNER_MANAGER_PRIORITY_INVALID = 0xFF
}rfcmn_atuner_mngr_priority_enum_type;

/*!
*/
typedef struct
{
  boolean                          is_enabled;
  uint8                            num_carrier_enabled;
  rfcom_mode_enum_type             mode; /* Current mode of Priority */
  rfm_device_enum_type             carrier_rfm_device[RFCMN_ATUNER_CARRIER_ID_MAX];
  rfm_device_enum_type             conflict_rfm_device;
}rfcmn_atuner_mngr_config_type;


typedef struct
{
  uint8                                 num_carriers; /* Number of active carriers */
  rfcommon_atuner_state_type            state; /* Current State of Tuner */
  rfcom_mode_enum_type                  mode; /* Current mode of Tuner */
  rfcmn_atuner_mngr_priority_enum_type  priority; /* Current Priority */
  rfm_device_enum_type                  rfm_dev_index[RFCMN_ATUNER_CARRIER_ID_MAX]; /* Array containing RFM device numbers of active carriers*/
    /* Array containing RFM device numbers of active Rx carriers corresponding to Tx carriers */
  rfm_device_enum_type                  tx_rx_rfm_dev_index[RFCMN_ATUNER_CARRIER_ID_MAX]; 
}rfcommon_atuner_manager_status_type;

/* Structure to store the config parameters of the Tuner manager
   The info is needed to exchange the configuration parameters between the tuners 
   during ASD switching
*/
typedef struct
{
  /* Parameters holding Current ATuner state */
  boolean                                 tuner_init;
  rfcommon_atuner_state_type              prev_atuner_state;
  rfcommon_atuner_manager_status_type     current_atuner_status;
  rfcommon_atuner_config_req_type         rfm_device_cfg[RFM_MAX_WAN_DEVICES];
  rfcommon_atuner_config_req_type         cfg_prior_tx[RFCMN_ATUNER_CARRIER_ID_MAX];
  rfcmn_atuner_mngr_config_type           priority_cfg[RFCOMMON_ATUNER_MANAGER_PRIORITY_MAX];

  /*CL control params*/
  rfcommon_atuner_cl_ctrl_info_type cl_ctrl_info;

  /*Detune info*/
  rfcom_band_type_u detune_band;
  rfcom_mode_enum_type detune_mode;
}rfcommon_atuner_config_info_params_type;


/* Structure to store the state info parameters of the Tuner manager
   The info is needed by system manager
*/
typedef struct
{
  /* Parameters holding Current ATuner state */
  rfcommon_atuner_manager_status_type     current_atuner_status;
  rfcommon_atuner_config_req_type         carrier_cfg[RFCMN_ATUNER_CARRIER_ID_MAX];
}rfcommon_atuner_state_info_params_type;

/* Structure containing band channel information for get tune code */
typedef struct
{
  boolean            is_this_rx_operaion; /* is this rx operation or not */
  uint32             rx_channels[RFCMN_ATUNER_CARRIER_ID_MAX]; /* Active Rx channels */
  rfcom_band_type_u  bands[RFCMN_ATUNER_CARRIER_ID_MAX]; /* Active bands */
  uint32             tx_channels[RFCMN_ATUNER_CARRIER_ID_MAX]; /* Active Tx channels */
  uint8              num_channels;
  boolean            is_rx_channel_valid[RFCMN_ATUNER_CARRIER_ID_MAX];
}rfcommon_atuner_manager_band_chan_info;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Data structure holding the tuning_data_params
*/
struct rfcommon_atuner_tuning_data_params_type
{
  rfcom_mode_enum_type mode;
  uint8                num_carriers;
  uint8                pcc_band_idx;
  rfcom_band_type_u    bands[RFCMN_ATUNER_CARRIER_ID_MAX];
  uint32               rx_channels[RFCMN_ATUNER_CARRIER_ID_MAX];
  uint32               tx_channels[RFCMN_ATUNER_CARRIER_ID_MAX];
  void print()
  {
  
  RFDEVICE_LOG_MSG_5(LOG_RFA_DEVICE_TUNER_C,MSG_LEGACY_LOW,
    "Mode:%d, band_num_0|PCC Idx|num_carr:0x%x, "
    "band_num_4|band_num_3|band_num_2|band_num_1:0x%x, "
    "band[0]:(Rx-%d,Tx-%d) ",
    mode,
    bands[0].lte_band << 16 | pcc_band_idx << 8 | num_carriers,
    bands[4].lte_band << 24 | bands[3].lte_band << 16 | bands[2].lte_band << 8 | bands[1].lte_band,
    rx_channels[0],tx_channels[0]);
  
    RFDEVICE_LOG_MSG_8(LOG_RFA_DEVICE_TUNER_C,MSG_LEGACY_LOW,
    "band channel list [1]:(Rx-%d,Tx-%d), band[2]:(Rx-%d,Tx-%d), band[3]:(Rx-%d,Tx-%d), band[4]:(Rx-%d,Tx-%d) ",
    rx_channels[1],tx_channels[1],
    rx_channels[2],tx_channels[2],
    rx_channels[3],tx_channels[3],
    rx_channels[4],tx_channels[4]);
  }

  void reset()
  {
    mode = RFM_INVALID_MODE;
    num_carriers = 0;
    pcc_band_idx = RFCMN_ATUNER_CARRIER_ID_INVALID;
    memset(tx_channels,0xFF,RFCMN_ATUNER_CARRIER_ID_MAX*sizeof(uint32));
    memset(rx_channels,0xFF,RFCMN_ATUNER_CARRIER_ID_MAX*sizeof(uint32));
    memset(bands,RFCOM_BAND_LTE_INVALID,RFCMN_ATUNER_CARRIER_ID_MAX*sizeof(bands[0]));
  }
};


#ifdef __cplusplus

#include "rfa.h"
#include "rfdevice_class.h"
#include "rfcommon_atuner_manager_factory.h"
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Interface antenna manager class that contains all the APIs for both of
  Advanced Open Loop (AOL) and Closed Loop (CL) algorithms of antenna tuner.
 
  @details
*/
class rfcommon_atuner_manager_intf : public rfcommon_atuner_intf
{
public:

  /*! Empty virtual destructor - base, interface class has no data so nothing 
  to delete */
  virtual ~rfcommon_atuner_manager_intf()
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
                                    int16 script_timing ) = 0;

  /* Acknowlegement for the script previously provided */
  virtual boolean acknowledge_script( rfcommon_atuner_script_token_type script_token ) = 0;

  /* Disables Tx */
  virtual boolean tx_disable( rfcommon_atuner_config_req_type* atuner_params_p,
                              rfcommon_atuner_script_token_type *script_token,
                              rf_device_execution_type dev_action,
                              rf_buffer_intf *script_buffer,
                              int16 script_timing ) = 0;

  /* Force re-tune (or de-tune) the antenna tuner */
  /* Update the tune code every 200ms */
  boolean force_detune ( rfcommon_atuner_config_req_type* atuner_params_p,
                         rfcommon_atuner_script_token_type *script_token,
                         rf_device_execution_type dev_action,
                         rf_buffer_intf *script_buffer,
                         int16 script_timing) = 0;

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

  virtual boolean set_tuner_config_info( rfcommon_atuner_config_info_params_type *config_payload_ptr,
                                         rfcommon_atuner_script_token_type *script_token,
                                         rf_buffer_intf *script_buffer,
                                         rf_device_execution_type dev_action,  
                                         int16 timing ) = 0;
  
  virtual boolean get_tuner_config_info ( rfcommon_atuner_config_info_params_type *config_payload_ptr ) = 0;

  /* API for getting scenario list for given tech and band */
  virtual boolean get_scenario_list( rfcommon_atuner_config_req_type* atuner_params_p,
                                     rfm_antenna_tuner_scenario_data *scenario_data,
                                     uint8 *free_space_index,
                                     rfm_tuner_handle_type *tuner_handle ) = 0;

  /* API to set scenario for mcl */
  virtual boolean set_scenario_mcl( rfcommon_atuner_config_req_type* atuner_params_p,
                                    uint8 scenario_val,
                                    rfm_tuner_handle_type tuner_handle ) = 0;

  /* API to stop mcl */
  virtual boolean stop_mcl( rfcommon_atuner_config_req_type* atuner_params_p,
                            rfm_tuner_handle_type tuner_handle ) = 0;

  virtual void enter_lock( ) = 0;

  virtual void release_lock( ) = 0;

  virtual void lock_all_tuners( ) = 0;
  
  virtual void release_all_tuners( ) = 0;

  virtual void set_force_update_tune_code_flag(boolean flag_val) = 0; 

  virtual rfdevice_antenna_tuner_device_override_status_type tune_code_override ( uint8  override_flag,
                                                                                  void*  data,
                                                                                  uint8* tuner_nv_ptr,
                                                                                  uint8* tuner_id_ptr) = 0;

  virtual boolean get_tuner_state_info(rfcommon_atuner_state_info_params_type *state_info) = 0;

  /* API to retrieve CL algorithm state: Enabled/Disabled (T/F) */
  virtual boolean get_cl_algo_state() = 0;

  /* API to control the CL timer*/
   virtual  boolean control_cl_timers( boolean cl_off_on ) = 0;
  	
  /*  API to program gps tune code when gps is enabled */
  virtual boolean enable_gps() = 0;

  /*  API to disable tuner when gps is disabled */
  virtual boolean disable_gps() = 0;

  /* API to update tuner state for detune based on system states of all tuners*/
  virtual boolean set_detune_mode( rfcommon_atuner_system_state_type system_state,
                                   rfm_mode_enum_type system_mode, 
                                   rfcom_band_type_u system_band, 
                                   rf_buffer_intf *script_buffer,
                                   rf_device_execution_type dev_action,  
                                   int16 script_timing){return TRUE; }

protected:

private:

};
#endif /* #ifdef __cplusplus */
#endif /* RFCOMMON_ATUNER_MANAGER_INTF_H */

