#ifndef FTM_RF_TEST_RX_MEASURE_H
#define FTM_RF_TEST_RX_MEASURE_H
/*!
  @file
  ftm_rf_test_rx_measure.h

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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/inc/ftm_rf_test_rx_measure.h#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------------------------------
09/10/15   zhw     Updated default response allocate helper API for Rx Measure
08/07/15   zhw     Added support for Common Rx Repack API
08/04/15   zhw     Addes support for action type and measure mode
08/03/15   zhw     Added support for tech bypass dispatch
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
  @addtogroup FTM_TEST_TOOLS_CMD_RX_MEASURE
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
/*! Type definition to define the RxAGC Data type during Rx Measure.  */
typedef int16 ftm_rf_test_rx_measure_agc_type;

/*! Type definition to define the LNA Gain State type during Rx Measure.  */
typedef uint16 ftm_rf_test_rx_measure_lna_gain_state_type;

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
  FTM_RF_TEST_RX_EC_SUCCESS = 0, /*!< 0 : Indicates success */

  FTM_RF_TEST_RX_EC_ERROR = 1, /*!< 1 : Indicates failure */

} ftm_rf_test_rx_measure_error_code_enum_type;


/*! Enumeration to indicate the type of properties for FTM Rx Measure Command. 
  These enumeration are used to define the content of #ftm_rf_test_field_property_type in FTM test command packet */
typedef enum
{
  RX_MEASURE_RSP_DEFAULT_CARRIER_PROP, /*!< 0 : Carrier property map */

  RX_MEASURE_RSP_DEFAULT_DEVICE_PROP, /*!< 1 : Device property map*/

  RX_MEASURE_RSP_DEFAULT_AGC_PROP, /*!< 2 : AGC property map*/
                    
  RX_MEASURE_RSP_DEFAULT_LNA_GS_PROP, /*!< 1 : LNA Gain State property map */
                    
  RX_MEASURE_RSP_DEFAULT_PROPERTY_NUM  /*!< Max : Defines maximum number of properties per measurement response*/

} ftm_rf_test_rx_measure_response_default_property_map_type;

/*! @} */

/*! @} */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold input and output data for each RxAGC measurement. Techs 
  may get a list of below structs and are expected to execute and fill each of
  them */
typedef struct
{
  boolean response_prop_mask[FTM_RX_MEASURE_PROP_NUM];

  struct ftm_rf_test_rx_measure_in_t
  {
    rfm_device_enum_type device;
    /*!< Device required to be measured*/  

    ftm_rf_test_carrier_type carrier;
    /*!< Carrier required to be measured*/  

    ftm_rf_test_rx_measure_agc_type expected_agc;
    /*!< Expected RxAGC value for this measurement. Some technologies requires 
      expected RxAGC value to determine LNA gain state. */     

  } input ;
  /*!< Input parameters. All params required to perform a Rx measurement goes 
    here */

  struct ftm_rf_test_rx_measure_out_t
  {
    ftm_rf_test_rx_measure_agc_type rxagc;
    /*!< Measured RxAGC value, 16bits*/  

    ftm_rf_test_rx_measure_lna_gain_state_type lna_gain_state;
    /*!< LNA gain state, 16 bits */  
    
  } output;
  /*!< Output parameters. All results goes here */  

} ftm_rf_test_rx_measure_unpacked_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Index type for response properties from measurement data. The order is fixed 
  and should not be changed. Can only be expanded. All results must be converted
  to property type as part of reponse payload. So any new type of results should
  have a new index and rx_measurement_result_type will be expanded automatically
  */
typedef enum
{
  FTM_RF_TEST_RX_MEAS_RSP_INDEX_CARRIER = 0,
  FTM_RF_TEST_RX_MEAS_RSP_INDEX_DEVICE = 1,
  FTM_RF_TEST_RX_MEAS_RSP_INDEX_RXAGC_VALUE = 2,
  FTM_RF_TEST_RX_MEAS_RSP_INDEX_LNA_GAIN_STATE = 3,
  FTM_RF_TEST_RX_MEAS_RSP_NUM  
} ftm_rf_test_rx_measure_rsp_index_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Response Data structure for each measurement. It packs N number of properties 
  in to one structs for ease of impelemntation. The final payload will have a
  list of measure_result_types. IMPORTANT: This type should not be modified.
  Any new type of properties to be added should be added to
  ftm_rf_test_rx_measure_rsp_index_type. The result_type will be expanded
  automatically*/
typedef PACK(struct)
{
  ftm_rf_test_field_error_code_mask_type error_code;
  /*!< Error code where each bit indicates an unique error defined by #ftm_rf_test_error_code_enum_type. Zero value 
  indictaes, there was no error */

  uint32 number_of_properties;
  /*!< Number of Properties accumulated based on the measurement results. The ultimate reponse payload carries this 
    number to inform tools the size of the list*/

  ftm_rf_test_property_data_type* result_prop_list;
  /*!< List of Response properties. The list begives at the address of result_properties and continues further*/

} ftm_rf_test_rx_measure_rsp_payload_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for tech specific callback function pointer */
typedef ftm_rf_test_field_error_code_mask_type (*ftm_rf_test_rx_measure_tech_callback_fp)
( 
  ftm_rf_test_rx_measure_unpacked_data_t *unpacked_measurement_list,
  /*!< List of Measurement Data */

  uint32 num_of_measurements
  /*!< Number of measurements*/
);

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for tech specific callback function pointer */
typedef boolean (*ftm_rf_test_rx_measure_allocate_response_callback_fp)
(
   ftm_rf_test_reponse_data_type* response_allocator_data,
   /*!< Data from Response Allocator. */

   ftm_rf_test_rx_measure_rsp_payload_type* response_payload,
   /*!< Payload Data  */

   uint32 response_payload_size
   /*!< Size of Payload Data. Must equal to the size of reponse payload in byte */
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

  ftm_rf_test_reponse_data_type* rsp_allocator_cb_data;
  /*!< Response allocator callback data. Pass through to rsp_alloc_cb */

  ftm_rf_test_rx_measure_allocate_response_callback_fp rsp_alloc_cb;
  /*!< Response allocation callback. Call this API to populate payload into response */

}ftm_rf_test_rx_measure_tech_override_data_type;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Type definition for tech specific callback function pointer. Provide tech the option 
  to override default unpacking & dispatching. Tech is responsible for allocating response packet. */
typedef ftm_rf_test_field_error_code_mask_type (*ftm_rf_test_rx_measure_tech_override_callback_fp)
( 
  ftm_rf_test_rx_measure_tech_override_data_type* override_data
  /*!< ftm_rf_test_rx_measure_tech_override_data_type */
);

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold mode specific data for rx measure */
typedef struct
{
  ftm_rf_test_rx_measure_tech_override_callback_fp override_callback_fp;
  /*!< Tech specific callback function */

  ftm_rf_test_rx_measure_tech_callback_fp callback_fp;
  /*!< Tech specific callback function */

  uint32 unpacked_data_num;
  /*!< Number of Measurement to perform for this dispatch. Automatically reset to 0 before each dispatch happens */

  ftm_rf_test_rx_measure_unpacked_data_t* unpacked_data;
  /*!< Unpacked measurement data containing all params and measurement results. It is a list that has N items 
    where N equals number of measurement*/

  ftm_rf_test_rx_measure_rsp_payload_type rsp_payload_data;
  /*!< Data for response payload */

}ftm_rf_test_rx_measure_mode_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Structure to hold Rx Measure command data */
typedef struct
{
  ftm_rf_test_rx_measure_mode_data_t mode_data[RFM_NUM_MODES];
  /*!< Structure to hold mode specific data for radio config */
                        
} ftm_rf_test_rx_measure_command_data_t;

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_rx_measure_init
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_rx_measure_deinit
(
  ftm_rf_test_command_dispatch_data_t *dispatch_data
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_rx_measure_register_tech_cb
(
  rfm_mode_enum_type technology,
  ftm_rf_test_rx_measure_tech_callback_fp callback_fp
);

/*--------------------------------------------------------------------------------------------------------------------*/
boolean
ftm_rf_test_rx_measure_register_tech_override_cb
(
  rfm_mode_enum_type technology,
  ftm_rf_test_rx_measure_tech_override_callback_fp override_callback_fp
);

#ifdef T_WINNT
#error code not present
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FTM_RF_TEST_RX_MEASURE_H */

