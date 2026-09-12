#ifndef FTM_RF_TEST_TX_CONTROL_H
#define FTM_RF_TEST_TX_CONTROL_H
/*!
  @file
  ftm_rf_test_tx_control.h

  @brief
  Module to handle Rx Measure command
*/

/*======================================================================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document are confidential 
  and proprietary information of Qualcomm Technologies Incorporated and all rights therein are 
  expressly reserved. By accepting this material the recipient agrees that this material and the 
  information contained therein are held in confidence and in trust and will not be used, copied, 
  reproduced in whole or in part, nor its contents revealed in any manner to others without the 
  express written permission of Qualcomm Technologies Incorporated.

======================================================================================================================*/

/*======================================================================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_rf_test_tx_control.h#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------------------------------
07/30/15   zhw     Initial Release

======================================================================================================================*/

#include "comdef.h"
#include "ftmdiag.h"
#include "ftm_rf_test_interface.h"
#include "ftm_rf_test_control.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef T_WINNT
#error code not present
#endif

/*====================================================================================================================*/
/*!
  @addtogroup FTM_TEST_TOOLS_CMD_TX_CONTROL
  @{
*/

/*====================================================================================================================*/
/*!
  @name Request Packet

  @brief
  Contains command definition for radio config request packet
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define the desired Tx Power in dBm10.  */
typedef int16 ftm_rf_test_tx_control_power_dbm10_type;

/*! Type definition to define the RB Config type.  */
typedef uint16 ftm_rf_test_tx_control_rb_config_type;

/*! Type definition to define the TX Action type.  */
typedef uint16 ftm_rf_test_tx_control_action_type;

/*! @} */

/*====================================================================================================================*/
/*!
  @name Response Packet

  @brief
  Contains command definition for TX Control Response packet
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Enumeration to indicate the type of subscribers */

typedef enum
{
  FTM_RF_TEST_TX_EC_SUCCESS = 0, /*!< 0 : Indicates success */

  FTM_RF_TEST_TX_EC_ERROR = 1, /*!< 1 : Indicates failure */

} ftm_rf_test_tx_control_error_code_enum_type;

/*! @} */

/*! @} */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold input and output data for each Tx Control. */
typedef struct
{
  struct ftm_rf_test_tx_action_in_t
  {
    rfm_device_enum_type device;
    /*!< Device required to be measured*/  

    ftm_rf_test_carrier_type carrier;
    /*!< Carrier required to be measured*/  

    ftm_rf_test_tx_control_action_type tx_action;
    /*!< Tx Action to perform for this command. Can be ON or OFF  */     

    ftm_rf_test_tx_control_rb_config_type rb_config;
    /*!< Resource Block Config. */     

    ftm_rf_test_tx_control_power_dbm10_type tx_power;
    /*!< Desired Tx Power in dBm10. */     

  } input ;
  /*!< Input parameters. All params required to perform a Tx Control goes 
    here */

} ftm_rf_test_tx_control_unpacked_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Response Data structure for Tx Control*/
typedef PACK(struct)
{
  ftm_rf_test_field_error_code_mask_type error_code;
  /*!< Error code where each bit indicates an unique error defined by #ftm_rf_test_error_code_enum_type. Zero value 
  indictaes, there was no error */

} ftm_rf_test_tx_control_rsp_payload_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for tech specific callback function pointer */
typedef ftm_rf_test_field_error_code_mask_type (*ftm_rf_test_tx_control_tech_callback_fp)
( 
  ftm_rf_test_tx_control_unpacked_data_t *unpacked_action_list,
  /*!< List of Measurement Data */

  uint32 num_of_measurements
  /*!< Number of measurements*/
);

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for tech specific callback function pointer. Provide tech the option 
  to override default unpacking & dispatching. Tech is responsible for allocating response packet. */
typedef PACK(struct)
{
  ftm_rf_test_field_num_prop_type req_prop_num;
  /*!< Number of properties in REQ */

  ftm_rf_test_property_data_type *req_prop_list;
  /*!< List of Properties in REQ */

}ftm_rf_test_tx_control_advanced_dispatch_data_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for tech specific callback function pointer. Provide tech the option 
  to override default unpacking & dispatching. Tech is responsible for allocating response packet. */
typedef ftm_rf_test_field_error_code_mask_type (*ftm_rf_test_tx_control_advanced_tech_dispatch_callback_fp)
( 
  ftm_rf_test_tx_control_advanced_dispatch_data_type* override_data
  /*!< ftm_rf_test_tx_control_tech_override_data_type */
);

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold mode specific data for rx measure */
typedef struct
{
  ftm_rf_test_tx_control_advanced_tech_dispatch_callback_fp advanced_tech_callback_fp;
  /*!< Tech specific callback function */

  ftm_rf_test_tx_control_tech_callback_fp default_callback_fp;
  /*!< Tech specific callback function */

  uint32 unpacked_data_num;
  /*!< Number of Measurement to perform for this dispatch. Automatically reset to 0 before each dispatch happens */

  ftm_rf_test_tx_control_unpacked_data_t* unpacked_data;
  /*!< Unpacked action data containing all params and action results. It is a list that has N items 
    where N equals number of actions*/

  ftm_rf_test_tx_control_rsp_payload_type rsp_payload_data;
  /*!< Data for response payload */

}ftm_rf_test_tx_control_mode_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold Rx Measure command data */
typedef struct
{
  ftm_rf_test_tx_control_mode_data_t mode_data[RFM_NUM_MODES];
  /*!< Structure to hold mode specific data for radio config */
                        
} ftm_rf_test_tx_control_command_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_tx_control_init
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_tx_control_deinit
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_tx_control_register_default_tech_cb
(
  rfm_mode_enum_type technology,
  ftm_rf_test_tx_control_tech_callback_fp callback_fp
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_tx_control_register_advanced_tech_cb
(
  rfm_mode_enum_type technology,
  ftm_rf_test_tx_control_advanced_tech_dispatch_callback_fp tech_callback_fp
);

#ifdef T_WINNT
#error code not present
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FTM_RF_TEST_RX_MEASURE_H */

