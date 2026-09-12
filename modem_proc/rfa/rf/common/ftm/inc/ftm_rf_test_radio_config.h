#ifndef FTM_RF_TEST_RADIO_CONFIG_H
#define FTM_RF_TEST_RADIO_CONFIG_H
/*!
  @file
  ftm_rf_test_radio_config.h

  @brief
  Module to handle Radio config command
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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_rf_test_radio_config.h#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------------------------------
08/03/15   zhw     Added support for Rx mode
07/29/15   zhw     Moved REQ & RSP payload type to interface
04/16/15   aro     Removed unused variable
04/16/15   aro     Removed tech requirement to pass the dispatch data
04/15/15   aro     unpack function optimization
04/15/15   aro     Fix for same tech CRAT
04/15/15   aro     Added error code
04/15/15   aro     Added subcriber info in radio config unpack data structure
04/15/15   aro     Doxygen fix
04/15/15   aro     Expanded Property value to 64 bit
04/15/15   aro     PACK fix for OFT
04/14/15   aro     Extern C
04/14/15   aro     Added Radio Congig test case
04/14/15   aro     Added generic dispatch mechanism
04/14/15   aro     Support for Callback registration
04/13/15   aro     [1] Added support for radio config handler function
                   [2] Added error reporting
04/13/15   aro     Added common response infrastructure
04/13/15   aro     Support to release subscriber
04/13/15   aro     Restructuring/Renaming
04/13/15   aro     Code partitioning
04/13/15   aro     Renamed filenames
04/13/15   aro     Added radio config unpack function
04/08/15   aro     [1] Added error code
                   [2] Rename type/fuunc/variable name
                   [3] Initial support for radio config handler
04/08/15   aro     Added subscriber acquire/release for a tech
04/08/15   aro     Added rfm device acquire/release for a tech
04/08/15   aro     Added Test dispatch function
04/08/15   aro     Initial Release

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
  @addtogroup FTM_TEST_TOOLS_CMD_RADIO_CONFIG
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
/*! Enumeration to indicate the version number for Radio config command */
typedef enum
{
  FTM_RF_TEST_RADIO_CONFIG_VERSION_0 = 0, /*!< 0 : Radio Config Version 0 */

  FTM_RF_TEST_RADIO_CONFIG_VERSION_1 = 1, /*!< 1 : Radio Config Version 1 */

  FTM_RF_TEST_RADIO_CONFIG_VERSION_NUM  /*!< Max : Defines maximum number of version defintion */

} ftm_rf_test_radio_config_version_type;

/*! @} */

/*====================================================================================================================*/
/*!
  @name Response Packet

  @brief
  Contains command definition for radio config Response packet
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Enumeration to indicate the type of subscribers */
typedef enum
{
  FTM_RF_TEST_RC_EC_DEV_GRANT_INTERIM = 0, /*!< 0 : Indicates RFM device allocation failure */

  FTM_RF_TEST_RC_EC_DEV_GRANT_CONFIRM = 1, /*!< 1 : Indicates RFM device allocation failure */

  FTM_RF_TEST_RC_EC_DEV_GRANT_RELEASE = 2, /*!< 2 : Indicates RFM device allocation failure */

  FTM_RF_TEST_RC_EC_NULL_PARAM = 3, /*!< 3 : Indicates NULL parameter */

  FTM_RF_TEST_RC_EC_PROP_INCOMPATIBLE = 4, /*!< 4 : Indicates incompatible property */

  FTM_RF_TEST_RC_EC_PROP_BAD_VAL = 5, /*!< 5 : Indicates Bad property value */

  FTM_RF_TEST_RC_EC_MALLOC_FAILURE = 6, /*!< 6 : Indicates malloc failure */

  FTM_RF_TEST_RC_EC_NULL_CB = 7, /*!< 7 : NULL Tech callback */

  FTM_RF_TEST_RC_EC_CORRUPTION = 8, /*!< 8 : Indicates generic corruption */

  FTM_RF_TEST_RC_EC_NUM  /*!< Max : Defines maximum number of error codes */

} ftm_rf_test_radio_config_error_code_enum_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Defintion of response payload for Radio config command */
typedef PACK(struct)
{

  ftm_rf_test_field_version_type version;
  /*!< Version of packet */

  ftm_rf_test_field_error_code_mask_type error_code;
  /*!< Error code where each bit indicates an unique error defined by #ftm_rf_test_error_code_enum_type. Zero value 
  indictaes, there was no error */

} ftm_rf_test_radio_config_rsp_payload_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Defintion of Radio config response command */
typedef PACK(struct)
{

  ftm_rf_test_header_rsp_type rsp_header;
  /*!< Generic Command header for FTM Test command response packet */

  ftm_rf_test_radio_config_rsp_payload_type payload;
  /*!< Payload specific to radio config response */

} ftm_rf_test_radio_config_rsp_type;

/*! @} */

/*! @} */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Enumeration to indicate the device allocation status */
typedef enum
{
  FTM_TEST_DEVICE_AVAILABLE,
  /*!< Indicates that the RFM device is free and can be assigned to requesting tech */

  FTM_TEST_DEVICE_GRANT_INTERIM,
  /*!< Indicates that the device is granted temporarily to a tech. This grant will be promoted to "confirmed" state 
  once the tech successfully completes the device tune sequence successfully. In case of failure in successful 
  completion of tune sequence, this granted will be demoted and device will be in AVAILABLE state */ 

  FTM_TEST_DEVICE_GRANTED
  /*!< Indicates that the tech is given a confirmed grant state */

} ftm_test_device_alloc_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold unpack data for radio config command */
typedef struct
{
  rfm_subscriber_type subscriber;
  /*!< Subscriber number for the given */

  struct ftm_rf_test_rc_drop_list_t
  {
    ftm_rf_test_rfm_device_mask_type device_mask;
    /*!< Mask indicating the devices which are required to be torn down */

  } drop_list;
  /*!< Structure holding the information of all the devices that are required to be dropped. Techs are required to 
  tear down the radio on these devices. */

  struct ftm_rf_test_rc_reconfig_list_t 
  {
 
    ftm_rf_test_rfm_device_mask_type device_mask;
    /*!< Mask indicating the devices which are required to be reconfigured */
    
    struct ftm_rf_test_rcrl_cmd_params_t /* rcrl = radio config reconfig list */ 
    {
      ftm_rf_test_device_radio_trx_t trx;
      /*!< Indicates if the device is Rx or Tx */

      boolean is_primary;
      /*!< Indicates if the device is to be configured as primary device or diversity device */

      rfm_device_enum_type assoc_rfm_device;
      /*!< Associated RFM device. If the array corresponds to Primary device, then associated device will 
      be the diversity device of the primary device; and vice versa*/

      ftm_rf_test_band_type band;
      /*!< Band to which the radio is to be configured */

      ftm_rf_test_carrier_mask_type carrier_mask;
      /*!< Carrier mask indicating the carriers to be enabled */

      ftm_rf_test_rx_mode_type rx_mode;
      /*!< Rx Mode for supporting burst/continous mode receive*/

      struct ftm_rf_test_rcrl_per_carrier_t 
      {
        ftm_rf_test_channel_type channel;
      /*!< List of channels */

        ftm_rf_test_bandwidth_type chan_bw;
        /*!< Bandwidth to which the radio is to configured to */
        
      } per_carrier[FTM_RF_TEST_CHANNEL_NUM];
      /*!< Per carrier configuration */

    }cmd_params[RFM_MAX_WAN_DEVICES];
    /*!< Information for each devices which is to be added or configured. The data for each container is valid, only 
    if the corresponding bit in device_mask is enabled. */

  } reconfig_list;
  /*!< Structure holding the information of all the devices that are required to be reconfigured. 
  Techs are required to take action on this devices only after dropping the list of devices in drop_list*/

  struct ftm_rf_test_rc_add_list_t 
  {
 
    ftm_rf_test_rfm_device_mask_type device_mask;
    /*!< Mask indicating the devices which are required to be added */
    
    struct ftm_rf_test_rcal_cmd_params_t 
    {

      ftm_rf_test_device_radio_trx_t trx;
      /*!< Indicates if the device is Rx or Tx */

      boolean is_primary;
      /*!< Indicates if the device is to be configured as primary device or diversity device */

      rfm_device_enum_type assoc_rfm_device;
      /*!< Associated RFM device. If the array corresponds to Primary device, then associated device will 
      be the diversity device of the primary device; and vice versa*/

      ftm_rf_test_band_type band;
      /*!< Band to which the radio is to be configured */

      ftm_rf_test_carrier_mask_type carrier_mask;
      /*!< Carrier mask indicating the carriers to be enabled */

      ftm_rf_test_rx_mode_type rx_mode;
      /*!< Rx Mode for supporting burst/continous mode receive*/

      struct ftm_rf_test_rcal_per_carrier_t 
      {
        ftm_rf_test_channel_type channel;
      /*!< List of channels */

        ftm_rf_test_bandwidth_type chan_bw;
        /*!< Bandwidth to which the radio is to configured to */

      } per_carrier[FTM_RF_TEST_CHANNEL_NUM];
      /*!< Per carrier configuration */
      
    }cmd_params[RFM_MAX_WAN_DEVICES];
    /*!< Information for each devices which is to be added or configured. The data for each container is valid, only 
    if the corresponding bit in device_mask is enabled. */

  } add_list;
  /*!< Structure holding the information of all the devices that are required to be added. 
  Techs are required to take action on this devices only after dropping the list of devices in drop_list*/

} ftm_rf_test_radio_config_unpack_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold per device data for each RFM device */
typedef struct
{
  rfm_mode_enum_type owner_tech;
  /*!< Indicates which Tech is the owener of this device */

  ftm_test_device_alloc_t alloc_state;
  /*!< Indicates the device allocation state */

  rfm_subscriber_type subscriber;
  /*!< Subscriber number for the given */
  
  rfm_mode_enum_type interim_backup_owner_tech;
  /*!< Backed up Owner tech before the interim state transition */

  ftm_test_device_alloc_t interim_backup_alloc_state;
  /*!< Backed up allocation state before the interim state transition */

  rfm_subscriber_type interim_backup_subscriber;
  /*!< Backed up Subscriber number before the interim state transition */
  
} ftm_rf_test_radio_config_grant_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for tech specific callback function pointer */
typedef ftm_rf_test_field_error_code_mask_type (*ftm_rf_test_radio_config_tech_callback_fp)
( 
  ftm_rf_test_radio_config_unpack_data_t *command_unpack_data
);

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold mode specific data for radio config */
typedef struct
{
  ftm_rf_test_radio_config_tech_callback_fp callback_fp;
  /*!< Tech specific callback function */

  ftm_rf_test_radio_config_unpack_data_t last_unpack[RFM_SUBSCRIBER_NUM];
  /*!< Structure which holds the last unpacked data for radio config command for each subscriber */

}ftm_rf_test_radio_config_mode_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold Radio Config command data */
typedef struct
{
  ftm_rf_test_radio_config_grant_data_t device_grant[RFM_MAX_WAN_DEVICES];
  /*!< Structure to hold data used to grant/release device to techs */
  
  ftm_rf_test_radio_config_mode_data_t mode_data[RFM_NUM_MODES];
  /*!< Structure to hold mode specific data for radio config */

} ftm_rf_test_radio_config_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_radio_config_init
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_radio_config_deinit
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_radio_config_register_tech_cb
(
  rfm_mode_enum_type technology,
  ftm_rf_test_radio_config_tech_callback_fp callback_fp
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_radio_config_sub_release_ready
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data,
  rfm_subscriber_type subscriber,
  rfm_mode_enum_type technology
);

#ifdef T_WINNT
#error code not present
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FTM_RF_TEST_RADIO_CONFIG_H */

