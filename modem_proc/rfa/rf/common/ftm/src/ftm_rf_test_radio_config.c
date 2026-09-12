/*!
  @file
  ftm_test_control.c

  @brief
  Common framework to perform radio test in FTM mode
*/

/*======================================================================================================================

  Copyright (c) 2015 - 2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/src/ftm_rf_test_radio_config.c#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------------------------------
03/17/15    rp     KW Fix.
08/03/15   zhw     Added support for Rx mode
08/04/15   zhw     Added Tech Override support for Rx Measure Command
07/29/15   zhw     Moved REQ & RSP payload type to interface. Make Property type common 
04/17/15   aro     KW fix
04/16/15   aro     Removed tech requirement to pass the dispatch data
04/15/15   aro     unpack function optimization
04/15/15   aro     Fix for same tech CRAT
04/15/15   aro     Added a separate reconfig list
04/15/15   aro     Fixed device revert
04/15/15   aro     Fixed error code handling
04/15/15   aro     Added subcriber info in radio config unpack data structure
04/15/15   aro     Doxygen fix
04/15/15   aro     Fixed last group not loading issue
04/15/15   aro     Added support for compressed packet
04/15/15   aro     Allowed PRI and DIV device on same property group
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
04/08/15   aro     [1] Subscriber acquire/release function
                   [2] State data query interface
04/08/15   aro     Added Test dispatch function
04/08/15   aro     Initial Release

======================================================================================================================*/

#include "ftm_rf_test_radio_config.h"
#include "ftm_rf_test_interface.h"
#include "ftm_msg.h"
#include "ftm_common_control.h"
#include "modem_mem.h"
#include "stringl.h"
#include "rfcommon_math.h"

/*====================================================================================================================*/
/*!
  @addtogroup FTM_TEST_TOOLS_CMD_RADIO_CONFIG
  @{
*/

/*====================================================================================================================*/
/*!
  @name RFM Device Management

  @brief
  Functions to manage RFM device allocation management
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Acquire Interim grant on device
 
  @details
  This function is called when a technology wants to acquire an interim grant on device. The device will be granted, 
  only if it is not acquired by other techs.
 
  @param subscriber
  RFM subscriber
 
  @param technology
  RFM technology which intends to acquire a device
 
  @param rfm_device
  RFM device which is intended to be acquierd by a tech
 
  @param grant_data
  Pointer to structure containg device grant data
 
  @return
  Flag indicating the status of aquisition
*/
static boolean
radio_config_device_grant_intrim
(
  rfm_subscriber_type subscriber,
  rfm_mode_enum_type technology,
  rfm_device_enum_type rfm_device,
  ftm_rf_test_radio_config_grant_data_t *grant_data
)
{
  boolean ret_val = TRUE;
  
  /* Validate the Test data pointer. If not valid, return FAILRUE */
  if ( grant_data == NULL )
  {
    FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_intrim: Sub %d, Tech %d, RFMDevice %d - "
                          "NULL Grant Data", subscriber, technology, rfm_device );
    return FALSE;
  } /* if ( grant_data == NULL ) */

  /* Param Validation */
  if ( ( technology >= RFM_NUM_MODES ) || ( rfm_device >= RFM_MAX_WAN_DEVICES ) || ( subscriber >= RFM_SUBSCRIBER_NUM ))
  {
    FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_intrim: Sub %d, Tech %d, RFMDevice %d - "
                          "Bad Param", subscriber, technology, rfm_device );
    return FALSE;
  } /* if ( ( technology >= RFM_NUM_MODES ) || ( rfm_device >= RFM_MAX_WAN_DEVICES ) || ( subscriber >= RFM_SUBSCRIBER_NUM )) */

  if ( ( ( grant_data->owner_tech == RFM_PARKED_MODE ) &&
         ( grant_data->alloc_state == FTM_TEST_DEVICE_AVAILABLE ) &&
         ( grant_data->subscriber == RFM_SUBSCRIBER_UNASSGINED ) ) ||
       ( ( grant_data->owner_tech == technology ) &&
         ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANTED ) &&
         ( grant_data->subscriber == subscriber ) ) )
  {
    /* Backup owner and state */
    grant_data->interim_backup_owner_tech = grant_data->owner_tech;
    grant_data->interim_backup_alloc_state = grant_data->alloc_state;
    grant_data->interim_backup_subscriber = grant_data->subscriber;

    /* Update owner and state */
    grant_data->subscriber = subscriber;
    grant_data->owner_tech = technology;
    grant_data->alloc_state = FTM_TEST_DEVICE_GRANT_INTERIM;
    ret_val &= TRUE;
    FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_intrim: Sub %d, Tech %d, RFMDevice %d - "
                         "Interim Grant", subscriber, technology, rfm_device );
  } /* if ( ( ( grant_data->owner_tech == RFM_PARKED_MODE ) &&
         ( grant_data->alloc_state == FTM_TEST_DEVICE_AVAILABLE ) ) ||
       ( ( grant_data->owner_tech == technology ) &&
         ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANTED ) ) ) */
  else
  {
    ret_val = FALSE;
    FTM_MSG_4( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_intrim: Sub %d, Tech %d, RFMDevice %d - "
                         "Denied (Current Tech %d)", subscriber, technology, rfm_device, grant_data->owner_tech );
  } /* if !( ( ( grant_data->owner_tech == RFM_PARKED_MODE ) &&
         ( grant_data->alloc_state == FTM_TEST_DEVICE_AVAILABLE ) ) ||
       ( ( grant_data->owner_tech == technology ) &&
         ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANTED ) ) ) */

  return ret_val;

} /* radio_config_device_grant_intrim */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Confirm grant on device
 
  @details
  This function is called when the interim grant of the device is requried to be confirmed.
 
  @param subscriber
  RFM subscriber
 
  @param technology
  RFM technology which intends to acquire a device
 
  @param rfm_device
  RFM device which is intended to be acquierd by a tech
 
  @param grant_data
  Pointer to structure containg device grant data

  @return
  Flag indicating the status of aquisition
*/
static boolean
radio_config_device_grant_confirm
(
  rfm_subscriber_type subscriber,
  rfm_mode_enum_type technology,
  rfm_device_enum_type rfm_device,
  ftm_rf_test_radio_config_grant_data_t *grant_data
)
{
  boolean ret_val = TRUE;
  
  /* Validate the Test data pointer. If not valid, return FAILRUE */
  if ( grant_data == NULL )
  {
    FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_confirm: Sub %d, Tech %d, "
                          "RFMDevice %d - NULL Grant Data", subscriber, technology, rfm_device );
    return FALSE;
  } /* if ( grant_data == NULL ) */

  /* Param Validation */
  if ( ( technology >= RFM_NUM_MODES ) || ( rfm_device >= RFM_MAX_WAN_DEVICES ) || ( subscriber >= RFM_SUBSCRIBER_NUM ))
  {
    FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_confirm: Sub %d, Tech %d, "
                          "RFMDevice %d - Bad Param", subscriber, technology, rfm_device );
    return FALSE;
  } /* if ( ( technology >= RFM_NUM_MODES ) || ( rfm_device >= RFM_MAX_WAN_DEVICES ) || ( subscriber >= RFM_SUBSCRIBER_NUM )) */

  if ( ( grant_data->owner_tech == technology ) &&
       ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANT_INTERIM ) &&
       ( grant_data->subscriber == subscriber ) )
  {
    grant_data->owner_tech = technology;
    grant_data->alloc_state = FTM_TEST_DEVICE_GRANTED;

    ret_val &= TRUE;
    FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_confirm: Sub %d, Tech %d, RFMDevice %d - "
                         "Granted", subscriber, technology, rfm_device );
  } /* if ( ( grant_data->owner_tech == technology ) &&
       ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANT_INTERIM ) ) */
  else
  {
    ret_val = FALSE;
    FTM_MSG_5( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_confirm: Sub %d, Tech %d, RFMDevice %d - "
               "Skip (Current Tech %d, State %d)", subscriber, technology, rfm_device, grant_data->owner_tech,
               grant_data->alloc_state );
  } /* if !( ( grant_data->owner_tech == technology ) &&
       ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANT_INTERIM ) ) */

  return ret_val;

} /* radio_config_device_grant_confirm */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Release grant on device
 
  @details
  This function is called when the grant of the device is requried to be released.
 
  @param subscriber
  RFM subscriber
 
  @param technology
  RFM technology which intends to release a device
 
  @param rfm_device
  RFM device which is intended to be released by a tech
 
  @param grant_data
  Pointer to structure containg device grant data
 
  @return
  Flag indicating the status of release
*/
static boolean
radio_config_device_grant_release
(
  rfm_subscriber_type subscriber,
  rfm_mode_enum_type technology,
  rfm_device_enum_type rfm_device,
  ftm_rf_test_radio_config_grant_data_t *grant_data
)
{
  boolean ret_val = TRUE;
  
  /* Validate the Test data pointer. If not valid, return FAILRUE */
  if ( grant_data == NULL )
  {
    FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_release: Sub %d, Tech %d, "
                          "RFMDevice %d - NULL Grant Data", subscriber, technology, rfm_device );
    return FALSE;
  } /* if ( grant_data == NULL ) */

  /* Param Validation */
  if ( ( technology >= RFM_NUM_MODES ) || ( rfm_device >= RFM_MAX_WAN_DEVICES ) )
  {
    FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_release: Sub %d, Tech %d, "
                          "RFMDevice %d - Bad Param", subscriber, technology, rfm_device );
    return FALSE;
  } /* if ( ( technology >= RFM_NUM_MODES ) || ( rfm_device >= RFM_MAX_WAN_DEVICES ) ) */

  if ( ( grant_data->owner_tech == technology ) &&
       ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANTED ) &&
       ( grant_data->subscriber == subscriber ) )
  {
    grant_data->owner_tech = RFM_PARKED_MODE;
    grant_data->alloc_state = FTM_TEST_DEVICE_AVAILABLE;
    grant_data->subscriber = RFM_SUBSCRIBER_UNASSGINED;
    ret_val &= TRUE;
    FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_release: Sub %d, Tech %d, RFMDevice %d - "
                         "Released", subscriber, technology, rfm_device );
  } /* if ( ( grant_data->owner_tech == technology ) &&
       ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANTED )&&
       ( grant_data->subscriber == subscriber ) ) */
  else if ( ( grant_data->owner_tech == technology ) &&
            ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANT_INTERIM ) &&
            ( grant_data->subscriber == subscriber ) )
  {
    grant_data->owner_tech = grant_data->interim_backup_owner_tech;
    grant_data->alloc_state = grant_data->interim_backup_alloc_state;
    grant_data->subscriber = grant_data->interim_backup_subscriber;
    ret_val &= TRUE;
    FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_device_grant_release: Sub %d, Tech %d, RFMDevice %d - "
                         "Reverted", subscriber, technology, rfm_device );
  } /* else if ( ( grant_data->owner_tech == technology ) &&
            ( grant_data->alloc_state == FTM_TEST_DEVICE_GRANT_INTERIM ) &&
            ( grant_data->subscriber == subscriber ) ) */

  return ret_val;

} /* radio_config_device_grant_release */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Unpack

  @brief
  Functions to manage Radio config command unpack
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Unpack radio config command packet
 
  @details
  This function unpacks the radio config command. The unpack process will result on two structure - one containing
  the list of devices which needs to be dropped by the tech; and other containining the list of devices and
  configuration data to be added and reconfigured. Some rules assumed during unpacking are as follows
  #- List of properties are grouped into different groups which is delimited by Rx or Tx carrier property
  #- Each group cannot have more than one entries of "same" property
  #- The devices not present in the new command config data as compared to previous command config data will be
     assumed to the devices to be dropped.
 
  @param num_of_props
  Number of properties in teh property list
 
  @param property_list
  Pointer to the property list
 
  @param unpack_data
  Structrue where the unpack data will be stored
 
  @param last_unpack_data
  Pointer last unpack data used to diff with the new command config to generate the drop list
 
  @param subscriber
  Subscriber number
 
  @param debug_mode
  Flag indicating the debug mode
 
  @return
  Error code generated by this function. The unpack data is valid only if the error code is 0.
*/
static ftm_rf_test_field_error_code_mask_type
radio_config_unpack
(
  ftm_rf_test_field_num_prop_type num_of_props,
  ftm_rf_test_property_data_type *property_list,
  ftm_rf_test_radio_config_unpack_data_t *unpack_data,
  ftm_rf_test_radio_config_unpack_data_t *last_unpack_data,
  rfm_subscriber_type subscriber,
  rfm_mode_enum_type technology,
  boolean debug_mode
)
{
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  ftm_rf_test_property_data_type *prop_ptr = NULL;
  uint32 prop_index = 0;
  uint8 chan_idx;
  uint8 dev_idx;

  /*------------------------------------------------------------------------------------------------------------------*/
  /* Variables used to temporarily store the unpacked data; until they are pushed to the unpack structure. */
  /* These variables must be reset to default value before starting to unpack a property group. */
  /*------------------------------------------------------------------------------------------------------------------*/
  ftm_rf_test_field_property_type property_type = FTM_RF_TEST_RADIO_CFG_PROP_UNASSIGNED;
  ftm_rf_test_field_property_val_type property_val = (ftm_rf_test_field_property_val_type)(-1);
  rfm_device_enum_type rfm_device_pri;
  rfm_device_enum_type rfm_device_div[RFM_MAX_WAN_DEVICES];
  ftm_rf_test_rfm_device_mask_type rfm_device_mask;
  ftm_rf_test_device_radio_trx_t trx;
  ftm_rf_test_band_type band;
  ftm_rf_test_channel_type channel[FTM_RF_TEST_CHANNEL_NUM];
  ftm_rf_test_bandwidth_type chan_bw[FTM_RF_TEST_CHANNEL_NUM];
  rfm_device_enum_type assoc_prx_rfm_device;
  boolean is_primary[RFM_MAX_WAN_DEVICES];
  ftm_rf_test_carrier_mask_type carrier_mask;
  ftm_rf_test_carrier_type carrier;
  ftm_rf_test_rx_mode_type rx_mode = 0;
  uint32 property_found_mask;
  uint8 diversity_count;
  /*------------------------------------------------------------------------------------------------------------------*/

  if ( ( property_list == NULL ) || ( unpack_data == NULL ) )
  {
    FTM_MSG( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_unpack: Null param " );
    error_code |= (1<<FTM_RF_TEST_RC_EC_NULL_PARAM);
    return error_code;
  } /* if ( ( property_list == NULL ) || ( unpack_data == NULL ) ) */

  /*------------------------------------------------------------------------------------------------------------------*/
  /* Reset the local variables to store the unpackes data for each property group */  
  /*------------------------------------------------------------------------------------------------------------------*/
  for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ )
  {
    channel[chan_idx] = (ftm_rf_test_channel_type)(-1);
    chan_bw[chan_idx] = (ftm_rf_test_bandwidth_type)(-1);
  } /* for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ ) */
  for ( dev_idx = 0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx++ )
  {
    rfm_device_div[dev_idx] = RFM_MAX_WAN_DEVICES;
    is_primary[dev_idx] = FALSE;
  } /* for ( dev_idx = 0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx++ ) */
  diversity_count = 0;
  assoc_prx_rfm_device = RFM_MAX_WAN_DEVICES;
  carrier_mask = 0;
  carrier = (ftm_rf_test_carrier_type)(-1);
  band = (ftm_rf_test_band_type)(-1);
  trx = FTM_RF_TEST_RADIO_TRX_INVALID;
  property_found_mask = 0;
  rfm_device_mask = 0;
  /*------------------------------------------------------------------------------------------------------------------*/

  /* Initialize Propert pointer */
  prop_ptr = property_list;

  /* Clear memory for unpack data */
  memset( unpack_data, 0, sizeof(ftm_rf_test_radio_config_unpack_data_t) );

  /*------------------------------------------------------------------------------------------------------------------*/
  /* Populate the ADD list */
  /*------------------------------------------------------------------------------------------------------------------*/

  /* Continue, if the number if properties is not 0 */
  if ( num_of_props != 0 )
  {
    /* Loop thrrough the number of properties. Loop is allowed to till num_of_props; to latch the last propert group */
    for ( prop_index = 0; prop_index <= num_of_props; prop_index++ )
    {
      /* Do Prop query, if property index is less than number of props. This is added, because the for LOOP is allowed
      to till num_of_props to allow the latching of last prop group */
      if ( prop_index < num_of_props )
      {
        /* Get property type */
        property_type = prop_ptr->property_type;

        /* Get the property Value */
        property_val = prop_ptr->property_val;

        /* If the debug mode is enabled, then print property information */
        if ( debug_mode == TRUE )
        {
          FTM_MSG_4( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG][UNPACK]: ADD  - [%3d][@ 0x%8x][Property %5d][Value %8d]", 
                     prop_index, (uint32)prop_index, property_type, property_val );
        } /* if ( debug_mode == TRUE ) */
      } /* if ( prop_index < num_of_props ) */

      /* RX_CARRIER or TX_CARRIER indicate the start of the property block. Thus, if the property type is
      encountered to be Rx or Tx Carrier, then push the accumumlated information to the unpack structure pointer.
      Since the first property of Radio config command will always be Rx or Tx carrier, make sure that the push to 
      unpack structure does not happen for the first time. */ 
      if ( ( ( prop_index != 0 ) && 
             ( ( property_type == FTM_RF_TEST_RADIO_CFG_PROP_RX_CARRIER ) || 
               ( property_type == FTM_RF_TEST_RADIO_CFG_PROP_TX_CARRIER ) ) ) ||
           ( prop_index == num_of_props ) )
      {
        ftm_rf_test_rfm_device_mask_type rfm_device_mask_temp  = rfm_device_mask;

        /* Loop through all the enabled devices */
        while ( rfm_device_mask_temp != 0 )
        {
          rfm_device_enum_type rfm_device;
                    
          /* Extract the first active bit location from device_mask. This corresponds to the active RFM device */
          rfm_device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(rfm_device_mask_temp);

          /* Proceed if the device is valid */
          if ( rfm_device < RFM_MAX_WAN_DEVICES )
          {
            /* If device is already active on this tech, then move populate in reconfig list */
             if ( (technology != RFM_LTE_MODE)&& (( last_unpack_data->add_list.device_mask & (1<<rfm_device) ) ||
                                                  ( last_unpack_data->reconfig_list.device_mask & (1<<rfm_device) )) )
            {
              unpack_data->reconfig_list.device_mask |= rfm_device_mask;
              unpack_data->reconfig_list.cmd_params[rfm_device].carrier_mask = carrier_mask;
              unpack_data->reconfig_list.cmd_params[rfm_device].band = band;
              for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ )
              {
                unpack_data->reconfig_list.cmd_params[rfm_device].per_carrier[chan_idx].channel = channel[chan_idx];
                unpack_data->reconfig_list.cmd_params[rfm_device].per_carrier[chan_idx].chan_bw = chan_bw[chan_idx];
              } /* for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ ) */
              unpack_data->reconfig_list.cmd_params[rfm_device].trx = trx;
              unpack_data->reconfig_list.cmd_params[rfm_device].is_primary = is_primary[rfm_device];
              unpack_data->reconfig_list.cmd_params[rfm_device].rx_mode = rx_mode;

              if ( is_primary[rfm_device] == TRUE )
              {
                unpack_data->reconfig_list.cmd_params[rfm_device].assoc_rfm_device = rfm_device;
              } /* if ( is_primary[rfm_device] == TRUE ) */
            else
            {
                unpack_data->reconfig_list.cmd_params[rfm_device].assoc_rfm_device = assoc_prx_rfm_device;
              } /* if !( is_primary[rfm_device] == TRUE ) */

              FTM_MSG_1( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG][UNPACK]: Loaded to RECONFIGList RFMDevice %d", 
                         rfm_device );
            } /* if ( ( last_unpack_data->add_list.device_mask & (1<<rfm_device_temp) ) ||
                 ( last_unpack_data->reconfig_list.device_mask & (1<<rfm_device_temp) ) ) */
            else
            {
              unpack_data->add_list.device_mask |= rfm_device_mask;
              unpack_data->add_list.cmd_params[rfm_device].carrier_mask = carrier_mask;
              unpack_data->add_list.cmd_params[rfm_device].band = band;
              for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ )
              {
                unpack_data->add_list.cmd_params[rfm_device].per_carrier[chan_idx].channel = channel[chan_idx];
                unpack_data->add_list.cmd_params[rfm_device].per_carrier[chan_idx].chan_bw = chan_bw[chan_idx];
              } /* for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ ) */
              unpack_data->add_list.cmd_params[rfm_device].trx = trx;
              unpack_data->add_list.cmd_params[rfm_device].is_primary = is_primary[rfm_device];
              unpack_data->add_list.cmd_params[rfm_device].rx_mode = rx_mode;
              if ( is_primary[rfm_device] == TRUE )
          {
                unpack_data->add_list.cmd_params[rfm_device].assoc_rfm_device = rfm_device;
              } /* if ( is_primary[rfm_device] == TRUE ) */
              else
            {
                unpack_data->add_list.cmd_params[rfm_device].assoc_rfm_device = assoc_prx_rfm_device;
              } /* if !( is_primary[rfm_device] == TRUE ) */

              FTM_MSG_1( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG][UNPACK]: Loaded to ADDList RFMDevice %d", 
                         rfm_device );
            } /* if ( ( last_unpack_data->add_list.device_mask & (1<<rfm_device_temp) ) ||
                 ( last_unpack_data->reconfig_list.device_mask & (1<<rfm_device_temp) ) ) */
          } /* if ( rfm_device_temp < RFM_MAX_WAN_DEVICES ) */
          else
          {
            FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG][UNPACK]: Invalid RFMDevice %d", 
                       rfm_device );
            error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_BAD_VAL);
          } /* if !( rfm_device_temp < RFM_MAX_WAN_DEVICES ) */

          /* Update device mask to notch out first active device LSB */
          rfm_device_mask_temp &= ( ~rf_extract_first_one_bit_mask( rfm_device_mask_temp ) );

        } /* while ( rfm_device_mask_temp != 0 ) */

        /*------------------------------------------------------------------------------------------------------------*/
        /* Reset the local variables to store the unpackes data for each property group */  
        /*------------------------------------------------------------------------------------------------------------*/
          for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ )
          {
            channel[chan_idx] = (ftm_rf_test_channel_type)(-1);
          chan_bw[chan_idx] = (ftm_rf_test_bandwidth_type)(-1);
          } /* for ( chan_idx = 0; chan_idx < FTM_RF_TEST_CHANNEL_NUM ; chan_idx++ ) */
        for ( dev_idx = 0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx++ )
        {
          rfm_device_div[dev_idx] = RFM_MAX_WAN_DEVICES;
          is_primary[dev_idx] = FALSE;
        } /* for ( dev_idx = 0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx++ ) */
        diversity_count = 0;
        assoc_prx_rfm_device = RFM_MAX_WAN_DEVICES;
          carrier_mask = 0;
          carrier = (ftm_rf_test_carrier_type)(-1);
          band = (ftm_rf_test_band_type)(-1);
        trx = FTM_RF_TEST_RADIO_TRX_INVALID;
          property_found_mask = 0;
        rfm_device_mask = 0;
        rx_mode = 0;
        /*------------------------------------------------------------------------------------------------------------*/

      } /* if ( ( prop_index != 0 ) && 
                ( ( property_type == FTM_RF_TEST_PROP_RX_CARRIER ) || 
                  ( property_type == FTM_RF_TEST_PROP_TX_CARRIER ) ) ) */
      
      /* Do Prop query, if property index is less than number of props. This is added, because the for LOOP is allowed
      to till num_of_props to allow the latching of last prop group */
      if ( prop_index < num_of_props )
      {
        /* In one group, there cannot be more than 1 number of same properties. (not applicable for Diversity device).
        Thus make sure this check is valid */
        if ( ( ( property_found_mask & (1<<prop_ptr->property_type) )  == 0 ) || 
             ( prop_ptr->property_type == FTM_RF_TEST_RADIO_CFG_PROP_RFM_DEVICE_DIV ) )
        {
          /* check the validity of property and load into the local variable. THis will be latched into the unpack 
          structure when next Rx or Tx property is encoutnered in the property list */

          property_found_mask |= (1<<prop_ptr->property_type);

          switch ( prop_ptr->property_type )
          {
          case FTM_RF_TEST_RADIO_CFG_PROP_RX_CARRIER:
            carrier = property_val;
            carrier_mask |= (1<<property_val);
            trx = FTM_RF_TEST_RADIO_RX;
            break;

          case FTM_RF_TEST_RADIO_CFG_PROP_TX_CARRIER:
            carrier = property_val;
            carrier_mask |= (1<<property_val);
            trx = FTM_RF_TEST_RADIO_TX;
            break;

          case FTM_RF_TEST_RADIO_CFG_PROP_RFM_DEVICE_PRI:
            rfm_device_pri = (rfm_device_enum_type)property_val;
            if (  rfm_device_pri < RFM_MAX_WAN_DEVICES  )
            {
              rfm_device_mask |= (1<<rfm_device_pri);
              is_primary[rfm_device_pri] = TRUE;
            } /* if (  rfm_device_pri < RFM_MAX_WAN_DEVICES  ) */
            assoc_prx_rfm_device = rfm_device_pri;
            break;

          case FTM_RF_TEST_RADIO_CFG_PROP_RFM_DEVICE_DIV:
            rfm_device_div[diversity_count] = (rfm_device_enum_type)property_val;
            if (  rfm_device_div[diversity_count] < RFM_MAX_WAN_DEVICES  )
            {
              rfm_device_mask |= (1<<rfm_device_div[diversity_count]);
              is_primary[rfm_device_div[diversity_count]] = FALSE;
            } /* if (  rfm_device_div[diversity_count] < RFM_MAX_WAN_DEVICES  ) */
            diversity_count++;
            break;

          case FTM_RF_TEST_RADIO_CFG_PROP_BAND:
            band = (ftm_rf_test_band_type)property_val;
            break;

          case FTM_RF_TEST_RADIO_CFG_PROP_CHANNEL:
            if ( carrier < FTM_RF_TEST_CHANNEL_NUM )
            {
              channel[carrier] = (ftm_rf_test_channel_type)property_val;
            } /* if ( carrier < FTM_RF_TEST_CHANNEL_NUM ) */
            else
            {
              FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: Carrier %d is invalid",
                         prop_ptr->property_type );
              error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_BAD_VAL);
            } /* if ( carrier < FTM_RF_TEST_CHANNEL_NUM ) */
            break;

          case FTM_RF_TEST_RADIO_CFG_PROP_BANDWIDTH:
            if ( carrier < FTM_RF_TEST_CHANNEL_NUM )
            {
              chan_bw[carrier] = (ftm_rf_test_bandwidth_type)property_val;
            } /* if ( carrier < FTM_RF_TEST_CHANNEL_NUM ) */
            else
            {
              FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: Carrier %d is invalid",
                         prop_ptr->property_type );
              error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_BAD_VAL);
            } /* if ( carrier < FTM_RF_TEST_CHANNEL_NUM ) */
            break;

          case FTM_RF_TEST_RADIO_CFG_PROP_RX_MODE:
            rx_mode = (ftm_rf_test_rx_mode_type)property_val;
            break;

          default:
            error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_INCOMPATIBLE);
            break;

          } /* switch ( prop_ptr->property_type ) */
        } /* if ( ( property_found_mask & (1<<prop_ptr->property_type) )  == 0 ) */
        else
        {
          FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: Duplicate PropType %d in a group",
                     prop_ptr->property_type );
          error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_INCOMPATIBLE);
        } /* if !( ( property_found_mask & (1<<prop_ptr->property_type) )  == 0 ) */
      } /* if ( prop_index < num_of_props ) */

      /* Increment the poperty pointer */
      prop_ptr++;
    } /* for ( prop_index = 0; prop_index < num_of_props; prop_index++ ) */
  } /* if ( num_of_props != 0 ) */

  unpack_data->subscriber = subscriber;

  /*------------------------------------------------------------------------------------------------------------------*/
  /* Populate the DROP list */
  /*------------------------------------------------------------------------------------------------------------------*/

  /* Proceed only if there were no errors */
  if ( error_code == 0 )
  {
    ftm_rf_test_rfm_device_mask_type curr_dev_mask;
    ftm_rf_test_rfm_device_mask_type new_dev_mask;
    ftm_rf_test_rfm_device_mask_type drop_dev_mask;

    /* Compute drop device mask */
    curr_dev_mask = (last_unpack_data->add_list.device_mask)|(last_unpack_data->reconfig_list.device_mask);
    new_dev_mask = (unpack_data->add_list.device_mask)|(unpack_data->reconfig_list.device_mask);

    drop_dev_mask = (curr_dev_mask^new_dev_mask)&curr_dev_mask;

    unpack_data->drop_list.device_mask = drop_dev_mask;

    if ( debug_mode == TRUE )
    {
      FTM_MSG_1( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG][UNPACK]: DROP - [0x%x]", drop_dev_mask );
    } /* if ( debug_mode == TRUE ) */
    
  } /* if ( error_code == 0 ) */
  else
  {
    unpack_data->drop_list.device_mask = 0;
  } /* if !( error_code == 0 ) */

  /*------------------------------------------------------------------------------------------------------------------*/
  /* Error handling */
  /*------------------------------------------------------------------------------------------------------------------*/

  /* If error code is not zero, then clear the unpack data */
  if ( error_code != 0 )
  {
    /* Clear memory for unpack data */
    memset( unpack_data, 0, sizeof(ftm_rf_test_radio_config_unpack_data_t) );
    FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: [ErrorCode 0x%x] Clearing unpack data",
               error_code );
  } /* if ( error_code != 0 ) */

  return error_code;

} /* radio_config_unpack */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Command Processing

  @brief
  Functions to handle Radio Config command
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Configure the radio
 
  @details
  This function is the handler function to configure Radio to given mode of operation. When a radio config command is
  received, it is broken into two parts
  #- RF devices that are no longer used/removed
  #- RF devices that are currently used and being re-tuned /  RF devices that are newly used and must be woken up
 
  Each technology is expected to reconfigure their driver/devices in this order. Devices which are no longer used after
  a RADIO_CONFIG are returned to the system for other subscriptions to use in the future. A RADIO_CONFIG command 
  received for a subscription+tech which is currently configured and contains no carriers means to release all resources
 
  @param command_data
  Pointer to the structure containing command specific dispatch command
 
  @return
  Response Packet for radio config command
*/
static boolean
radio_config_handler
(
  struct ftm_rf_test_command_data_t *command_data
)
{
  boolean ret_val = TRUE;
  ftm_rf_test_radio_config_data_t *radio_config_data;
  ftm_rf_test_req_type *req_data;
  ftm_rf_test_radio_config_rsp_payload_type *rsp_payload;
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  ftm_rf_test_field_error_code_mask_type ec_unpack = 0;
  ftm_rf_test_field_error_code_mask_type ec_tech_cb = 0;
  ftm_rf_test_radio_config_unpack_data_t *unpack_data;

  /* Param validation */
  if ( ( command_data == NULL ) || ( command_data->dispatch_data == NULL ) ||
       ( command_data->diag_request_data == NULL ) )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Null param" );
    return FALSE;
  } /* if ( ( command_instance == NULL ) || ( command_instance->sw_command_data == NULL ) ||
       ( command_instance->diag_request_data == NULL ) ) */

  /* Param validation */
  if ( command_data->technology >= RFM_NUM_MODES )
  {
    FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Bad Tech %d",
               command_data->technology);
    return FALSE;
  } /* if ( command_instance->technology >= RFM_NUM_MODES ) */

  /* Create Response packet */
  rsp_payload = command_data->response_data.alloc_response_fp( &(command_data->response_data),
                                                                   sizeof(ftm_rf_test_radio_config_rsp_payload_type) );
  /* NULL pointer check */
  if ( rsp_payload != NULL )
  {
    /* Get Radio Config Data */
    radio_config_data = (ftm_rf_test_radio_config_data_t*)(command_data->dispatch_data->command_data);

    /* Get Diag Request Data */
    req_data = command_data->diag_request_data;

    /* Make sure that the Radio config Data and diag request data is not NULL, before proceeding */
    if ( ( radio_config_data != NULL ) && ( req_data != NULL ) )
    {
      /* Allocate memory for unpack data */
      unpack_data = 
        (ftm_rf_test_radio_config_unpack_data_t*)modem_mem_alloc( sizeof(ftm_rf_test_radio_config_unpack_data_t),
                                                                  MODEM_MEM_CLIENT_RFA );

      /* Perform NULL pointer check */
      if ( unpack_data != NULL )
      {
        /* Unpack data */
        ec_unpack = radio_config_unpack( req_data->payload.num_of_properties,
                                         &(req_data->payload.property_data),
                                         unpack_data,
                                         &(radio_config_data->mode_data[command_data->technology].last_unpack[command_data->subscriber]),
                                         command_data->subscriber,
                                         command_data->technology,
                                         TRUE );

        /* Continue if the UNPACK error code is 0 */
        if ( ec_unpack == 0 )
        {
          rfm_device_enum_type rfm_device;
          ftm_rf_test_rfm_device_mask_type device_mask;
          boolean grant_interim = TRUE;
          boolean grant_release = TRUE;
          boolean grant_confirm = TRUE;
          boolean grant_revert = TRUE;

          /*----------------------------------------------------------------------------------------------------------*/
          /* Get interim grant for device */
          /*----------------------------------------------------------------------------------------------------------*/
          device_mask = unpack_data->add_list.device_mask;

          while ( device_mask != 0 )
          {
            /* Extract the first active bit location from device_mask. This corresponds to the active RFM device */
            rfm_device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

            if (( rfm_device < RFM_MAX_WAN_DEVICES ) && ( rfm_device >= 0)) 
            {

              grant_interim &= radio_config_device_grant_intrim( command_data->subscriber,
                                                                 command_data->technology, 
                                                                 rfm_device,
                                                                      &(radio_config_data->device_grant[rfm_device]) );
            } /* if ( rfm_device < RFM_MAX_WAN_DEVICES ) */
            else
            {
              FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: radio_config_handler - Invalid RFMDevice %d", 
                         rfm_device );
              error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_BAD_VAL);
            } /* if !( rfm_device < RFM_MAX_WAN_DEVICES ) */

            /* Update device mask to notch out first active device LSB */
            device_mask &= ( ~rf_extract_first_one_bit_mask( device_mask ) );
            
          } /* while ( device_mask != 0 ) */

          /*----------------------------------------------------------------------------------------------------------*/
          /* Call Tech callback */
          /*----------------------------------------------------------------------------------------------------------*/

          if ( grant_interim == TRUE )
          {
            /* Call Tech callback */
            if ( radio_config_data->mode_data[command_data->technology].callback_fp != NULL )
            {
              ec_tech_cb = radio_config_data->mode_data[command_data->technology].callback_fp( unpack_data );
            } /* if ( radio_config_data->mode_data[command_instance->technology].callback_fp != NULL ) */
            else
            {
              ec_tech_cb = (1<<FTM_RF_TEST_RC_EC_NULL_CB);
            } /* if ! ( radio_config_data->mode_data[command_instance->technology].callback_fp != NULL ) */

          } /* if ( grant_interim == TRUE ) */
          else
          {
            FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Interim Grant failure" );
            error_code |= (1<<FTM_RF_TEST_RC_EC_DEV_GRANT_INTERIM);
          } /* if ( grant_interim == TRUE ) */

          /*----------------------------------------------------------------------------------------------------------*/
          /* Confirm grant and release grant for dropped devices */
          /*----------------------------------------------------------------------------------------------------------*/

          if ( ( ec_tech_cb == 0 ) && ( grant_interim == TRUE ) )
          {
            /*--------------------------------------------------------------------------------------------------------*/
            /* Confirm grant */
            /*--------------------------------------------------------------------------------------------------------*/

            device_mask = unpack_data->add_list.device_mask;

            while ( device_mask != 0 )
            {
              /* Extract the first active bit location from device_mask. This corresponds to the active RFM device */
              rfm_device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

              if (( rfm_device < RFM_MAX_WAN_DEVICES ) && ( rfm_device >= 0)) 
              {
                grant_confirm &= radio_config_device_grant_confirm( command_data->subscriber,
                                                                    command_data->technology,
                                                                    rfm_device,
                                                                    &(radio_config_data->device_grant[rfm_device]) );
              } /* if ( rfm_device < RFM_MAX_WAN_DEVICES ) */
              else
              {
                FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: radio_config_handler - Invalid RFMDevice %d", 
                           rfm_device );
                error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_BAD_VAL);
              } /* if !( rfm_device < RFM_MAX_WAN_DEVICES ) */

              /* Update device mask to notch out first active device LSB */
              device_mask &= ( ~rf_extract_first_one_bit_mask( device_mask ) );

            } /* while ( device_mask != 0 ) */

            if ( grant_confirm == FALSE )
            {
              FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Confirm Grant failure" );
              error_code |= (1<<FTM_RF_TEST_RC_EC_DEV_GRANT_CONFIRM);
            } /* if ( grant_confirm == TRUE ) */

            /*--------------------------------------------------------------------------------------------------------*/
            /* Release grant */
            /*--------------------------------------------------------------------------------------------------------*/

            device_mask = unpack_data->drop_list.device_mask;

            while ( device_mask != 0 )
            {
              /* Extract the first active bit location from device_mask. This corresponds to the active RFM device */
              rfm_device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);

               if (( rfm_device < RFM_MAX_WAN_DEVICES ) && ( rfm_device >= 0)) 
              {
                grant_release &= radio_config_device_grant_release( command_data->subscriber,
                                                                    command_data->technology, 
                                                                    rfm_device,
                                                                    &(radio_config_data->device_grant[rfm_device]) );
              } /* if ( rfm_device < RFM_MAX_WAN_DEVICES ) */
              else
              {
                FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: radio_config_handler - Invalid RFMDevice %d", 
                           rfm_device );
                error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_BAD_VAL);
              } /* if !( rfm_device < RFM_MAX_WAN_DEVICES ) */

              /* Update device mask to notch out first active device LSB */
              device_mask &= ( ~rf_extract_first_one_bit_mask( device_mask ) );

            } /* while ( device_mask != 0 ) */

            if ( grant_release == FALSE )
            {
              FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Release Grant failure" );
              error_code |= (1<<FTM_RF_TEST_RC_EC_DEV_GRANT_RELEASE);
            } /* if ( grant_release == TRUE ) */

            /* Backup the unpack data */

            if(command_data->subscriber < RFM_SUBSCRIBER_INVALID)
            {
              memscpy( &(radio_config_data->mode_data[command_data->technology].last_unpack[command_data->subscriber]),
                         sizeof(ftm_rf_test_radio_config_unpack_data_t),
                         unpack_data,
                         sizeof(ftm_rf_test_radio_config_unpack_data_t) );
            }
            else
            {
              FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: radio_config_handler - Invalid Subscriber Type %d", 
                                    command_data->subscriber );
            }

          } /* if ( ( ec_tech_cb == 0 ) && ( grant_interim == TRUE ) ) */
          else
          {

            /*----------------------------------------------------------------------------------------------------------*/
            /* Revert grant, if the tech callback failed */
            /*----------------------------------------------------------------------------------------------------------*/

            device_mask = unpack_data->add_list.device_mask;
          
            while ( device_mask != 0 )
            {
              /* Extract the first active bit location from device_mask. This corresponds to the active RFM device */
              rfm_device = (rfm_device_enum_type)rf_extract_first_one_bit_lsb_location(device_mask);
          
             if (( rfm_device < RFM_MAX_WAN_DEVICES ) && ( rfm_device >= 0)) 
              {
                grant_revert &= radio_config_device_grant_release( command_data->subscriber,
                                                                   command_data->technology, 
                                                                   rfm_device,
                                                                   &(radio_config_data->device_grant[rfm_device]) );
              } /* if ( rfm_device < RFM_MAX_WAN_DEVICES ) */
              else
              {
                FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]: radio_config_handler - Invalid RFMDevice %d", 
                           rfm_device );
                error_code |= (1<<FTM_RF_TEST_RC_EC_PROP_BAD_VAL);
              } /* if !( rfm_device < RFM_MAX_WAN_DEVICES ) */
          
              /* Update device mask to notch out first active device LSB */
              device_mask &= ( ~rf_extract_first_one_bit_mask( device_mask ) );
          
            } /* while ( device_mask != 0 ) */

            if ( grant_revert == FALSE )
            {
              FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Revert Grant failure" );
              error_code |= (1<<FTM_RF_TEST_RC_EC_DEV_GRANT_RELEASE);
            } /* if ( grant_revert == TRUE ) */
          } /* if !( ( ec_tech_cb == 0 ) && ( grant_interim == TRUE ) ) */
          
        } /* if ( ec_unpack == 0 ) */
        else
        {
          FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Unpack failure" );
        }

        /* Free up the memory */
        modem_mem_free( unpack_data, MODEM_MEM_CLIENT_RFA );
        unpack_data = NULL;
      } /* if ( unpack_data != NULL ) */
      else
      {
        FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Malloc Failure" );
        error_code |= (1<<FTM_RF_TEST_RC_EC_MALLOC_FAILURE);
      } /* if !( unpack_data != NULL ) */
    } /* if ( ( radio_config_data != NULL ) && ( req_data != NULL ) ) */
    else
    {
      FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Null RC / REQ Data" );
      error_code |= (1<<FTM_RF_TEST_RC_EC_NULL_PARAM);
      ret_val = FALSE;
    } /* if !( ( radio_config_data != NULL ) && ( req_data != NULL ) ) */

    /* Accumulate error code */
    error_code |= ec_unpack;
    error_code |= ec_tech_cb;

    /* Populate response packet */
    rsp_payload->version = 0;
    rsp_payload->error_code = error_code;

    if ( error_code != 0 )
    {
      ret_val = FALSE;
    } /* if ( error_code != 0 ) */

    FTM_MSG_1( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: [ErrorCode 0x%x]", error_code );


  } /* if ( rsp_payload != NULL ) */
  else
  {
    ret_val = FALSE;
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]radio_config_handler: Response malloc failure" );
  } /* if !( rsp_payload != NULL ) */

  return ret_val;

} /* radio_config_handler */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Init

  @brief
  Functions to handle Radio Config command initialization for registration
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Initialize Radio Config Command handler module
 
  @details
  This function initializes the radio configure command module. Initialization includes, allocating the memory for
  house keeping data for radio config command and populating the command handler function
 
  @param dispatch_data
  Poitner to dispatch data specific to radio config command
 
  @return
  Flag indicating the status of radio config initialization
*/
boolean
ftm_rf_test_radio_config_init
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
)
{
  boolean ret_val = TRUE;

  /* Param Validation */
  if ( dispatch_data == NULL )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_init: Null param" );
    return FALSE;
  } /* if ( dispatch_data == NULL ) */

  /* Allocate memory for Radio Config command handler */ 

  if ( dispatch_data->command_data == NULL )
  {
    dispatch_data->command_data = (void*)modem_mem_alloc( sizeof(ftm_rf_test_radio_config_data_t),
                                                          MODEM_MEM_CLIENT_RFA );
  } /* if ( dispatch_data->command_data == NULL ) */

  if ( dispatch_data->command_data != NULL )
  {
    memset( dispatch_data->command_data, 0, sizeof(ftm_rf_test_radio_config_data_t) );
    dispatch_data->command_handler = radio_config_handler;
  } /* if ( dispatch_data->command_data != NULL ) */
  else
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_init: Malloc failure" );
    ret_val = FALSE;
  } /* if !( dispatch_data->command_data != NULL ) */

  return ret_val;

} /* ftm_rf_test_radio_config_init */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  DeInitialize Radio Config Command handler module
 
  @details
  This function deinitializes the radio configure command module. DeInitialization includes, deallocating the memory for
  house keeping data for radio config command
 
  @param dispatch_data
  Poitner to dispatch data specific to radio config command
 
  @return
  Flag indicating the status of radio config deinitialization
*/
boolean
ftm_rf_test_radio_config_deinit
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
)
{
  boolean ret_val = TRUE;

  /* Param Validation */
  if ( dispatch_data == NULL )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_deinit: Null param" );
    return FALSE;
  } /* if ( dispatch_data == NULL ) */

  /* Free memory allocated for Radio Config command handler */ 
  if ( dispatch_data->command_data != NULL )
  {
    modem_mem_free( dispatch_data->command_data, MODEM_MEM_CLIENT_RFA );
    dispatch_data->command_data = NULL;
  } /* if ( dispatch_data->command_data != NULL ) */

  dispatch_data->command_handler = NULL;

  return ret_val;

} /* ftm_rf_test_radio_config_deinit */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Tech Registration

  @brief
  Interface for technologies to perform callback registration with the FTM RF Test module for Radio config command
*/
/*! @{ */


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Register Tech callback
 
  @details
  This function is used to register a tech callback function for Radio Config command
 
  @param technology
  Technology for which the callback is to be registered
 
  @param callback_fp
  Callback funtion pointer to be registered
 
  @return
  Flag indicating the status of callback registration
*/
boolean
ftm_rf_test_radio_config_register_tech_cb
(
  rfm_mode_enum_type technology,
  ftm_rf_test_radio_config_tech_callback_fp callback_fp
)
{
  ftm_rf_test_command_dispatch_data_t *dispatch_data;
  ftm_rf_test_radio_config_data_t *radio_config_data;

  dispatch_data = ftm_rf_test_get_dispatch_data( FTM_RF_TEST_CMD_RADIO_CONFIGURE );

  /* Param validation */
  if ( dispatch_data == NULL )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_register_tech_cb: Null param" );
    return FALSE;
  } /* if ( dispatch_data == NULL ) ) */

  radio_config_data = (ftm_rf_test_radio_config_data_t*)dispatch_data->command_data;

  /* Param validation */
  if ( technology >= RFM_NUM_MODES )
  {
    FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_register_tech_cb: Bad Tech %d",
               technology);
    return FALSE;
  } /* if ( technology >= RFM_NUM_MODES ) */

  /* Save callback function */
  radio_config_data->mode_data[technology].callback_fp = callback_fp;

  FTM_MSG_1( FTM_HIGH, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_register_tech_cb: Registered for Tech %d",
             technology);

  return TRUE;

} /* ftm_rf_test_radio_config_register_tech_cb */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Query

  @brief
  Functions to handle Radio Config command
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Check if the TECH is ready to release subscriber
 
  @details
  This function scans the device grant table to check if the given tech has any RFM device resource actively granted.
  If the device is granted, then the subsriber is NOT ready to be released. Otherwise, the subscriber will be
  indicated to be released.
 
  @param dispatch_data
  Pointer to radio config data
 
  @param subscriber
  RFM subscriber ID
 
  @param technology
  Tech for which the release ready check is to be done
 
  @return
  Flag indicating, if the subscriber can be released or not
*/
boolean
ftm_rf_test_radio_config_sub_release_ready
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data,
  rfm_subscriber_type subscriber,
  rfm_mode_enum_type technology
)
{
  boolean release_ready = TRUE;
  uint8 dev_idx;
  ftm_rf_test_radio_config_data_t *radio_config_data;

  /* Param validation */
  if ( dispatch_data == NULL )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_sub_release_ready: Null param" );
    return FALSE;
  } /* if ( dispatch_data == NULL ) ) */

  radio_config_data = (ftm_rf_test_radio_config_data_t*)dispatch_data->command_data;

  /* Param validation */
  if ( technology >= RFM_NUM_MODES )
  {
    FTM_MSG_2( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_sub_release_ready: Sub %d - Bad Tech %d",
               subscriber, technology);
    return FALSE;
  } /* if ( technology >= RFM_NUM_MODES ) */

  /* Param validation */
  if ( subscriber >= RFM_SUBSCRIBER_NUM )
  {
    FTM_MSG_2( FTM_ERROR, "[FTM.RFTEST][RADIO_CONFIG]ftm_rf_test_radio_config_sub_release_ready: Tech %d - Bad Sub %d",
               technology, subscriber);
    return FALSE;
  } /* if ( subscriber >= RFM_SUBSCRIBER_NUM ) */


  for ( dev_idx = 0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx++ )
  {
    if ( ( radio_config_data->device_grant[dev_idx].owner_tech == technology ) &&
         ( radio_config_data->device_grant[dev_idx].subscriber == subscriber ) )
    {
      if (radio_config_data->device_grant[dev_idx].alloc_state == FTM_TEST_DEVICE_GRANTED )
      {
        release_ready = FALSE;
        break;
      } /* if ( ( radio_config_data->device_grant[dev_idx].owner_tech == technology ) &&
         ( radio_config_data->device_grant[dev_idx].subscriber == subscriber ) ) */
    }
  } /* for ( dev_idx = 0; dev_idx < RFM_MAX_WAN_DEVICES; dev_idx++ ) */

  return release_ready;

} /* ftm_rf_test_radio_config_sub_release_ready */

/*! @} */

/*! @} */ /* FTM_TEST_TOOLS_CMD_RADIO_CONFIG */


