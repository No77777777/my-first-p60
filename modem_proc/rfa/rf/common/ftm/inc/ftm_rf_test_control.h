#ifndef FTM_RF_TEST_CONTROL_H
#define FTM_RF_TEST_CONTROL_H
/*!
  @file
  ftm_rf_test_control.h

  @brief
  Dispatcher for FTM radio test in FTM mode
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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_rf_test_control.h#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------------------------------
01/24/20   moh     RF MDUMP Changes
08/11/15   zhw     Added support for Tx Control Command
08/03/15   zhw     Added support for Rx mode & Common Rx Repack API
07/29/15   zhw     Added common unpack API
04/15/15   aro     unpack function optimization
04/15/15   aro     Fix for same tech CRAT
04/15/15   aro     Added subcriber info in radio config unpack data structure
04/15/15   aro     Added support for compressed packet
04/14/15   aro     Extern C
04/14/15   aro     Added Radio Congig test case
04/14/15   aro     Added generic dispatch mechanism
04/14/15   aro     Support for Callback registration
04/13/15   aro     [1] Added support for radio config handler function
                   [2] Added error reporting
04/13/15   aro     Added common response infrastructure
04/13/15   aro     Code partitioning
04/13/15   aro     Renamed filenames
04/08/15   aro     [1] Added error code
                   [2] Rename type/fuunc/variable name
                   [3] Initial support for radio config handler
04/08/15   aro     Added Test dispatch function
04/08/15   aro     Initial Release

======================================================================================================================*/

#include "comdef.h"
#include "ftmdiag.h"
#include "ftm_rf_test_interface.h"
#include "rfm_subscriber_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Forward declaration of Response data type */
struct ftm_rf_test_reponse_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Forward declaration of command data type */
struct ftm_rf_test_command_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Function pointer defnition to allocation response packet */
typedef void* (*ftm_rf_test_allocate_response_fp_t)( struct ftm_rf_test_reponse_data_t *response_data, 
                                                     uint16 cmd_payload_rsp_size );

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Function pointer type definition for command handler */
typedef boolean (*ftm_rf_test_command_handler_fp)( struct ftm_rf_test_command_data_t *command_data );

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold command specific dispatch data */
typedef struct
{
  ftm_rf_test_command_handler_fp command_handler;
  /*!< Command handler function pointer */

  void *command_data;
  /*!< Pointer to command specific house keeping data */

} ftm_rf_test_command_dispatch_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold data need to create response packet */
typedef struct ftm_rf_test_reponse_data_t
{
  boolean is_allocated;
  /*!< Flag indicating, if the responce packet is allocated or not */

  ftm_rf_test_allocate_response_fp_t alloc_response_fp;
  /*!< Function pointer to allocated memory */

  ftm_rsp_pkt_type rsp_pkt;
  /*!< Structure to hold response packet for this command */
  
  uint16 cmd_payload_rsp_size;
  /*!< Size of command specific response payload */

  uint16 total_payload_rsp_size;
  /*!< Total size of response packet. This includes the summation of size of command specific payload and 
  size of command infrastructure payload */

  ftm_rf_test_req_header_type *diag_request_header;
  /*!< Pointer to resquest data received from Diag */

  ftm_rf_test_field_error_code_mask_type error_code;
  /*!< Error code that can be recorded during response packet creation */

  boolean common_framework_alloc;
  /*!< FLag indicating if the allocation request is from common framework */

} ftm_rf_test_reponse_data_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold command specific dispatch data */
typedef struct ftm_rf_test_command_data_t
{
  void *diag_request_data;
  /*!< Pointer to resquest data received from Diag */

  ftm_rf_test_command_dispatch_data_t *dispatch_data;
  /*!< Pointer to command specific state data pointer */

  rfm_subscriber_type subscriber;
  /*!< Subscriber */

  rfm_mode_enum_type technology;
  /*!< RFM mode for which the command is dispatched */

  ftm_rf_test_reponse_data_type response_data;
  /*!< Structure to hold data need to create response packet */

} ftm_rf_test_command_data_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to keep track of state data for FTM Test infrastructure */
typedef struct
{

  rfm_mode_enum_type subscriber_owner[RFM_SUBSCRIBER_NUM];
  /*!< Variable to keep track the ownership for each subscriber by a technology */

  ftm_rf_test_command_dispatch_data_t dispatch_data[FTM_RF_TEST_CMD_NUM];
  /*!< Structure to hold command specific dispatch data */

} ftm_rf_test_data_type;


/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define the size of the "Number of properties" field in FTM test command packet.  */
typedef uint32 ftm_rf_test_field_num_prop_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define the size of the property field in FTM test command packet.  */
typedef uint32 ftm_rf_test_field_property_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define the size of the property value field in FTM test command packet.  */
typedef uint64 ftm_rf_test_field_property_val_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Defintion of property type and value in FTM test request comamnd */
typedef PACK(struct)
{

  ftm_rf_test_field_property_type property_type;
  /*!< Defines the type of property defined by #ftm_rf_test_radio_config_property_type */

  ftm_rf_test_field_property_val_type property_val;
  /*!< Variable to hold the value for the give property */

} ftm_rf_test_property_data_type;


/*--------------------------------------------------------------------------------------------------------------------*/
/*! Defintion of payload for Radio config command */
typedef PACK(struct)
{

  ftm_rf_test_field_num_prop_type num_of_properties;
  /*!< Total number of properties available */

  ftm_rf_test_property_data_type property_data;
  /*!< List of property data. This will be an array of size defined by num_of_properties field. Since it will of 
  variable length, software will use read pointer (of size #ftm_rf_test_property_data_type), to traverse through 
  all the valid properties. */ 

} ftm_rf_test_req_payload_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Defintion of Radio config request command */
typedef PACK(struct)
{
  ftm_rf_test_req_header_type test_header;
  /*!< Defintion of command header for FTM RF Test command request packet */

  ftm_rf_test_req_payload_type payload;
  /*!< Payload specific to radio config command */

} ftm_rf_test_req_type;


/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define RFM device mask */
typedef uint16 ftm_rf_test_rfm_device_mask_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define carrier */
typedef uint32 ftm_rf_test_carrier_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define carrier mask */
typedef uint8 ftm_rf_test_carrier_mask_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define band */
typedef uint32 ftm_rf_test_band_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define channel */
typedef uint32 ftm_rf_test_channel_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define bandwidth */
typedef uint32 ftm_rf_test_bandwidth_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition to define Rx mode */
typedef uint32 ftm_rf_test_rx_mode_type;
/*--------------------------------------------------------------------------------------------------------------------*/
/*! Maximum number of channels per device in multicarrier mode */
#define FTM_RF_TEST_CHANNEL_NUM 3

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Enumeration to indicate if a device is Rx or Tx */
typedef enum
{
  FTM_RF_TEST_RADIO_TRX_INVALID,
  /*!<  Enumeration indicating invalid device radio type */

  FTM_RF_TEST_RADIO_RX,
  /*!<  Enumeration to indicate that a radio is Rx device */

  FTM_RF_TEST_RADIO_TX,
  /*!<  Enumeration to indicate that a radio is Tx device */

} ftm_rf_test_device_radio_trx_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Enumeration to indicate if a device is Primary chain or diversity chain */
typedef enum
{
  FTM_RF_TEST_RADIO_INVALID,
  /*!<  Enumeration indicating invalid device redundancy type */

  FTM_RF_TEST_RADIO_PRI,
  /*!<  Enumeration to indicate that a radio is in Primary chain */

  FTM_RF_TEST_RADIO_DIV,
  /*!<  Enumeration to indicate that a radio is in Diversity chain */

} ftm_rf_test_device_radio_redundancy_t;

typedef ftm_rf_test_field_error_code_mask_type (*ftm_rf_test_unpack_fp_type)
(
  uint32 num_of_props,
  ftm_rf_test_property_data_type * property_list,
  void* command_data,
  boolean debug_mode 
);

typedef boolean (*ftm_rf_test_property_delimiter_fp_type)
(
   uint32 property_type
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_init
(
  void
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_deinit
(
  void
);

/*--------------------------------------------------------------------------------------------------------------------*/
ftm_rsp_pkt_type
ftm_rf_test_dispatch
(
  void *ftm_req_data,
  uint32 request_packet_size
);

/*--------------------------------------------------------------------------------------------------------------------*/
ftm_rf_test_command_dispatch_data_t*
ftm_rf_test_get_dispatch_data
(
  ftm_rf_test_command_enum_type ftm_rf_test_command
);

/*--------------------------------------------------------------------------------------------------------------------*/
ftm_rf_test_field_error_code_mask_type
ftm_rf_test_unpack_property_list
(
  ftm_rf_test_field_num_prop_type total_num_of_props,
  ftm_rf_test_property_data_type *property_list,
  void* command_mode_data,
  boolean debug_mode,
  ftm_rf_test_unpack_fp_type command_unpack_callback,
  ftm_rf_test_property_delimiter_fp_type delimiter_callback
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_alloc_response_helper
(
  ftm_rf_test_reponse_data_type* response_allocator_data,
  void* response_payload,
  uint32 response_payload_size
);

/*Used for dumping rf_test_data to mdump using QSH */
#ifdef FEATURE_QSH_MDUMP
ftm_rf_test_data_type *get_ftm_rf_test_data();
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FTM_RF_TEST_CONTROL_H */
