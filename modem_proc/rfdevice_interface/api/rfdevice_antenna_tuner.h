#ifndef RFDEVICE_ANTENNA_TUNER_H
#define RFDEVICE_ANTENNA_TUNER_H
/*!
   @file
   rfdevice_antenna_tuner.h

   @brief

*/

/*===========================================================================

Copyright (c) 2012 - 2013,2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rfdevice_interface.mpss/2.11/api/rfdevice_antenna_tuner.h#1 $

when       who   what, where, why
--------   ---   ------------------------------------------------------------------- 
08/10/15   yb    API to get rfc config
06/19/15   yb      ACL clean-up
06/09/15   yb    Added API to get tune code and get_nvg status
06/09/15   yb    Device API clean-up
12/10/14   ndb   Added FBRX based CL enums
08/20/14   ndb   Add Tuner_ID as part of the response in "FTM_SET_TUNER_TUNE_CODE_OVERRIDE"
08/26/14   yb    Added update_dependent_logical_device_obj() API
07/08/14   kg    CL optimization 
05/20/14   yb    Added program_nvg_detune_tuner() API
03/25/14   yb    Added support for linearizer
03/11/14   yb    Fix order of inclusion of files
11/27/13   yb    Added support for default tune code when tuner QCN is missing
                 Removed low power API
11/07/13  yb/ndb Added the support for dynamic tune code size
10/09/13   ndb   Added "rl_disable" API
11/05/13   adk   Added support for extensions to NV item RFNV_ATUNER_ALGO_TYPE
10/31/13   shb   Convert TRX to C++ interface
09/05/13   ndb   Added "tia_range" in qfe_antenna_tuner_rl_req_data_type
07/24/13   adk   Provide RX freq and radio access tech info to the tuner driver
07/18/13   hm    Added tuner_low_power API
07/16/13   ndb   Added is_algo_supported()
07/10/13   aca   RL API expansion
07/01/13   aca   RL measurement update API
06/26/13   aca   RL API update
06/10/13   aca   RL APIs
04/12/13   sr    removed legacy code which is not used.
02/01/13   vb    Added support for program_tune_code
02/01/13   vb    Added support for get_antenna_tuner_instance_val
01/21/13   ndb   Added tune_code_override()API
01/23/13   aca   Added timing support
01/18/13   adk   Added APIs to retrieve chip version and slave id of antenna tuner
01/07/13   ndb   Added Tech specific Settings support in rx_init
10/05/12   gh    Added API for rf_init, rf_disable, get_provisioned_table_revision
                 and get_provisioned_table_oem_string
09/19/12   gh    Added intf API validating a scenario number based on mode/tech
09/17/12   gh    Added intf API for querying revision and OEM string
08/30/12   gh    Updated function prototypes
08/06/12   vb    Initial version
============================================================================*/
#include "rfcom.h"
#include "rfdevice_cmn_type_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "rf_buffer_intf.h"
#include "rfdevice_class.h"
#include "rf_hal_rffe.h"
#include "rfc_common.h"
#ifdef __cplusplus
}
#endif
/*===========================================================================
                         Data Definitions
===========================================================================*/
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Max number of register values associated to scenario programming
*/
#define QFE_ANT_TUNER_MAX_REG_VALS 4


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Data structure containing register values for scenario programming
*/
typedef struct
{
  uint8 reg_val[QFE_ANT_TUNER_MAX_REG_VALS];
}qfe_antenna_tuner_scenario_data_type;


/*----------------------------------------------------------------------------*/
/*!
  @brief
  Enum indicating the device ID/number
*/
typedef enum
{
  RFDEVICE_ANTENNA_TUNER_DEVICE_0,
  RFDEVICE_ANTENNA_TUNER_DEVICE_1,
  RFDEVICE_ANTENNA_TUNER_DEVICE_2,
  RFDEVICE_ANTENNA_TUNER_DEVICE_3,
  RFDEVICE_ANTENNA_TUNER_DEVICE_4,
  RFDEVICE_ANTENNA_TUNER_DEVICE_5,
  RFDEVICE_ANTENNA_TUNER_DEVICE_6,
  RFDEVICE_ANTENNA_TUNER_DEVICE_7,
  RFDEVICE_ANTENNA_TUNER_DEVICE_8,
  RFDEVICE_ANTENNA_TUNER_DEVICE_9,
  RFDEVICE_ANTENNA_TUNER_DEVICE_MAX
} rfdevice_antenna_tuner_device_num_type;


typedef enum
{
  RFDEVICE_ANTENNA_TUNER_OVERRIDE_TUNE_CODE_SUCCESS,
  RFDEVICE_ANTENNA_TUNER_OVERRIDE_TUNER_NOT_PRESENT,
  RFDEVICE_ANTENNA_TUNER_OVERRIDE_TUNE_CODE_FAILURE,
  RFDEVICE_ANTENNA_TUNER_ERROR = 0xFFFFFFFF
}rfdevice_antenna_tuner_device_override_status_type;



typedef enum
{
  RFDEVICE_ANTENNA_TUNER_SWITCH1_MASK = 0x01,
  RFDEVICE_ANTENNA_TUNER_SWITCH2_MASK = 0x02,
  RFDEVICE_ANTENNA_TUNER_SWITCH3_MASK = 0x04,
  RFDEVICE_ANTENNA_TUNER_SWITCH4_MASK = 0x08,
  RFDEVICE_ANTENNA_TUNER_SWITCH5_MASK = 0x10,
  RFDEVICE_ANTENNA_TUNER_SWITCH6_MASK = 0x20,
  RFDEVICE_ANTENNA_TUNER_SWITCH7_MASK = 0x40,
  RFDEVICE_ANTENNA_TUNER_SWITCH8_MASK = 0x80,
  RFDEVICE_ANTENNA_TUNER_SWITCH_MAX = 0xFF
}rfdevice_antenna_tuner_device_switch_mask;

/*!
  @brief
   Enum to capture the algorithm type supported in the tuner
  
  @detail
*/
typedef enum
{
  RFDEVICE_ANTENNA_TUNER_AOL_ONLY_ALGO      = 0,
  RFDEVICE_ANTENNA_TUNER_AOL_PLUS_CL_ALGO   = 1,
  RFDEVICE_ANTENNA_TUNER_OL_ONLY_ALGO       = 2,
  RFDEVICE_ANTENNA_TUNER_OL_PLUS_CL_ALGO    = 3,
  RFDEVICE_ANTENNA_TUNER_AOL_PLUS_CL_FBRX_ALGO   = 4,
  RFDEVICE_ANTENNA_TUNER_OL_PLUS_CL_FBRX_ALGO    = 5,
  RFDEVICE_ANTENNA_TUNER_AOL_PLUS_ACL_FBRX_ALGO   = 6,
  RFDEVICE_ANTENNA_TUNER_OL_PLUS_ACL_FBRX_ALGO    = 7,
  RFDEVICE_ANTENNA_TUNER_MAX_ALGO_SUPPORTED = 0xFF
} rfdevice_atuner_tuner_device_algo_type;


#ifdef __cplusplus
/*===========================================================================
                           INCLUDE FILES
===========================================================================*/

class rfdevice_antenna_tuner:public rfdevice_class
{
public:
/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function for antenna tuner initialization sequence

  @details
  rfdevice_antenna_tuner_init()

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @return
  boolean indicating pass or fail
*/
virtual boolean init
  (
    rf_buffer_intf *buff_obj_ptr, rf_device_execution_type dev_action, int16 script_timing = 0
  ) =0;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  this function programs antenna tuner to initialize tuner for rx operations

  @details

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us
  
  @return
  boolean indicating pass (TRUE) or fail (FALSE)  
*/
virtual boolean rx_init 
( rfcom_mode_enum_type mode,
  rf_buffer_intf *buff_obj_ptr,
  rf_device_execution_type dev_action, 
  int16 script_timing = 0 
  )=0;


/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function to program antenna tuner in tx mode

  @details
  rfdevice_antenna_tuner_tx_mode_enable()
  
  @param mode
  enum indicating the Radio Access Technology

  @param tx_freq
  frequency of Tx PLL being tuned to

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @return
  boolean indicating pass or fail  
*/
virtual boolean tx_mode_enable
  (
    rfcom_mode_enum_type mode,
    rf_buffer_intf *buff_obj_ptr, 
    rf_device_execution_type dev_action,
    int16 script_timing = 0
  ) = 0;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  interface function to shutdown/disable antenna tuner

  @details
  rfdevice_antenna_tuner_disable()

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts    

  @script_timing
  timing in us

  @return
  boolean indicating pass or fail  
*/
virtual boolean disable
  (
    rf_buffer_intf *buff_obj_ptr, rf_device_execution_type dev_action, int16 script_timing = 0
  )=0;


/*----------------------------------------------------------------------------*/
/*!
  @brief
  This utility function returns the max size of the script

  @return
  uint16 max script size
*/
virtual uint16 get_script_size(void) = 0;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This utility function returns the chip version of the antenna tuner

  @return
  int16 chip version of the antenna tuner
*/
virtual int16 read_chip_rev( void) = 0;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This utility function returns the rfc configuration of the device

  @return
  rfc configuration of the device
*/
virtual rfc_device_cfg_info_type get_rfc_device_cfg();

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is used to override the current tuner setting in Tuner hardware

  @details
  tune_code_override()
  
  @param override_flag
  Tuner override flag: Any value greater than '1' indicates the current tuner settings 
  can be overriden.

  @param data
  Pointer to the tuner tunecode config structure 

  @param tuner_nv_ptr
  Pointer to the NV data structure

  @param tuner_id_ptr
  Pointer to the tuner_id_ptr

 
  @return
  rfdevice_antenna_tuner_device_override_status_type 

*/
virtual rfdevice_antenna_tuner_device_override_status_type tune_code_override
(
 uint8 override_flag,
 void *data,
 uint8* tuner_nv_ptr,
 uint8* tuner_id_ptr
) =0;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function is used to program the tune code value obtained from
  external Tuner Data Base

  @details
  program_tune_code()

  @param tune_code
  ptr to the tune codes that need to programmed.

  @param tune_code_size
  size of the tune_code in bytes

  @param buff_obj_ptr
  void pointer to CCS buffer object

  @param dev_action
  indicates if it is immediate write to hw or build scripts 

  @param script_timing
  timing offset to be applied to the script
 
  @return
  success or failure

*/

virtual boolean program_tune_code
(
  uint8 *tune_code_ptr,
  uint8 tune_code_size,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing
)=0;

/*----------------------------------------------------------------------------*/

/*!
  @brief
 API to get default tune code for the tuner device

  @return
  success/failure

*/

virtual boolean program_default_tune_code
(
  uint8 *tune_code_ptr,
  uint8 *tune_code_size,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing
)=0;


/*----------------------------------------------------------------------------*/

/*!
  @brief
  API for enabling/disabling the NVG and to detune the tuner

  @return
  success/failure
*/
virtual boolean program_nvg_detune_tuner
(
  boolean nvg_on_off,
  boolean is_detune_code_valid,
  uint8 tune_code_size,
  uint8 *tune_code_ptr,
  rf_buffer_intf *buff_obj_ptr, 
  rf_device_execution_type dev_action,
  int16 script_timing
);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function returns the tuner instance value 

  @details

  @return
  Tuner instance value
*/
virtual rfdevice_antenna_tuner_device_num_type 
        get_antenna_tuner_instance_val(void) =0;


/*----------------------------------------------------------------------------*/

/*!
  @brief
 API to query whether the given algorthim is supported in the Hardware or not 

  @return
  success/failure

*/
virtual boolean is_algo_supported(rfdevice_atuner_tuner_device_algo_type algotype);

virtual boolean set_band_distortion_config_map(rfcom_mode_enum_type mode,rfcom_band_type_u band,int32 *data_ptr);

virtual void update_dependent_logical_device_obj(rfdevice_class* device_obj, rfdevice_type_enum_type device_type);

/*----------------------------------------------------------------------------*/

/*!
  @brief
  API to query tune code from tuner device hw
  Optional API depending on requirements for the device.

  @param *tune_code_size
  variable that is returned back with actual tune code size

  @param tune_code
  array of tune code values, with size of array as RFDEVICE_QTUNER_MAX_TUNE_CODE_SIZE

  @param tuner_id
  Tuner id

  @return
  success/failure

*/
virtual boolean get_tune_code(uint8 *tune_code_size, uint8* tune_code, uint8* tuner_id);

/*----------------------------------------------------------------------------*/

/*!
  @brief
  API to query staus of NVG.

  @return
  TRUE: NVG is enabled
  FALSE: NVG is disabled

*/
virtual boolean get_nvg_status(void);

};
#endif /*__cplusplus*/

#endif /*RFDEVICE_ANTENNA_TUNER_H*/

