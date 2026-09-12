/*!
  @file
  ftm_test.c

  @brief
  Dispatcher for FTM radio test in FTM mode
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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/src/ftm_rf_test_control.c#1 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------------------------------------------------
01/24/20   moh     RF MDUMP Changes
03/14/18    rp     KW Fix.
05/06/16    vc     Added deinit Tx Control Command
08/11/15   zhw     Added support for Tx Control Command
08/07/15   zhw     Added support for Common Rx Repack API
07/30/15   zhw     Fix wrong accumulation in unpack API
07/30/15   zhw     Added Tech Override support for Rx Measure Command
07/30/15   zhw     Updated support for Rx Measure Command
07/29/15   zhw     Added support for Rx Measure Command
04/15/15   aro     unpack function optimization
04/15/15   aro     Fix for same tech CRAT
04/15/15   aro     Fixed error code handling
04/15/15   aro     Added subcriber info in radio config unpack data structure
04/15/15   aro     Dynamically compute the packet length to allow proper uncompression for compress payload
04/15/15   aro     Added support for compressed packet
04/14/15   aro     Added Radio Congig test case
04/14/15   aro     Radio Config Init
04/14/15   aro     Added generic dispatch mechanism
04/14/15   aro     Support for Callback registration
04/13/15   aro     [1] Added support for radio config handler function
                   [2] Added error reporting
04/13/15   aro     Added common response infrastructure
04/13/15   aro     Support to release subscriber
04/13/15   aro     Restructuring/Renaming
04/13/15   aro     Code partitioning
04/13/15   aro     Renamed filenames
04/08/15   aro     [1] Added error code
                   [2] Rename type/fuunc/variable name
                   [3] Initial support for radio config handler
04/08/15   aro     Added Test dispatch function
04/08/15   aro     Initial Release

======================================================================================================================*/

#include "ftm_rf_test_control.h"
#include "ftm_rf_test_interface.h"
#include "ftm_rf_test_radio_config.h"
#include "ftm_rf_test_rx_measure.h"
#include "ftm_rf_test_tx_control.h"
#include "ftm_common_control.h"
#include "ftm_msg.h"
#include "modem_mem.h"
#include "zlib.h"
#include "stringl.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif
/*====================================================================================================================*/
/*!
  @addtogroup FTM_TEST_TOOLS_SW_MC
  @{
*/

/*====================================================================================================================*/
/*!
  @name Data

  @brief
  Managed FTM Test data
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! Static global data to store FTM Test data. The memory will be allocated only when FTM protocol is activated 
and deallocted when the protocol is deactivated */
static ftm_rf_test_data_type *ftm_rf_test_data;


/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  API to get the ftm_rf_test_data
 
  @details
  This function is called by rflm ccs qsh callbacl to collect ftm_rf_test_data 
  using QSH framework.
 
  @param subscriber
  None.
 
  @return
  ftm_rf_test_data
*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
ftm_rf_test_data_type *get_ftm_rf_test_data()
{
    return ftm_rf_test_data;
}
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Query FTM Test data
 
  @details
  This function will return a pointer to the test data
 
  @return
  Pointer to FTM test data
*/
#ifndef T_WINNT
static 
#endif /* T_WINNT */
ftm_rf_test_data_type*
ftm_rf_test_get_data
(
  void
)
{
  return ftm_rf_test_data;
} /* ftm_rf_test_get_data */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Subscriber Management

  @brief
  Functions to manage subscriber allocation management
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Acquire Subscriber
 
  @details
  This function is called when a technology wants to acquire a subscriber. The subscriber will be granted, only if it is
  not acquired by other techs.
 
  @param technology
  RFM technology which intends to acquire a subscriber
 
  @param subscriber
  Subscriber which is intended to be acquierd by a tech
 
  @param test_data
  Poitner to test framework data
 
  @return
  Flag indicating the status of aquisition
*/
static boolean
ftm_rf_test_subscriber_acquire
(
  rfm_mode_enum_type technology,
  rfm_subscriber_type subscriber,
  ftm_rf_test_data_type *test_data
)
{
  boolean ret_val = TRUE;
  
  /* Validate the Test data pointer. If not valid, return FAILRUE */
  if ( test_data == NULL )
  {
    FTM_MSG_2( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_acquire: Tech %d, Sub %d - NULL Test Data",
               technology, subscriber );
    return FALSE;
  } /* if ( test_data == NULL ) */

  /* Param Validation */
  if ( ( technology >= RFM_NUM_MODES ) || ( subscriber >= RFM_SUBSCRIBER_NUM ) )
  {
    FTM_MSG_2( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_acquire: Tech %d, Sub %d - Bad Param",
               technology, subscriber );
    return FALSE;
  } /* if ( ( technology >= RFM_NUM_MODES ) || ( subscriber >= RFM_SUBSCRIBER_NUM ) ) */

  if ( ( test_data->subscriber_owner[subscriber] == RFM_PARKED_MODE ) ||
       ( test_data->subscriber_owner[subscriber] == technology ) )
  {
    test_data->subscriber_owner[subscriber] = technology;
    FTM_MSG_2( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_acquire: Tech %d, Sub %d - Assigned", 
               technology, subscriber );
  } /* if ( ( test_data->subscriber_owner[subscriber] == RFM_PARKED_MODE ) ||
       ( test_data->subscriber_owner[subscriber] == technology ) ) */
  else
  {
    FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_acquire: Tech %d, Sub %d - Denied "
                         "(Current Tech %d)", technology, subscriber, test_data->subscriber_owner[subscriber] );
    ret_val = FALSE;
  } /* if !( ( test_data->subscriber_owner[subscriber] == RFM_PARKED_MODE ) ||
       ( test_data->subscriber_owner[subscriber] == technology ) ) */

  return ret_val;

} /* ftm_rf_test_subscriber_acquire */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Released Subscriber
 
  @details
  This function is called when a technology wants to release a subscriber. The subscriber will be released, only if 
  the requeting tech is the current owner of the subscriber.
 
  @param technology
  RFM technology which intends to release a subscriber
 
  @param subscriber
  Subscriber which is intended to be released by a tech
 
  @param test_data
  Poitner to test framework data

  @return
  Flag indicating the status of release
*/
static boolean
ftm_rf_test_subscriber_release
(
  rfm_mode_enum_type technology,
  rfm_subscriber_type subscriber,
  ftm_rf_test_data_type *test_data
)
{
  boolean ret_val = TRUE;

  /* Validate the Test data pointer. If not valid, return FAILRUE */
  if ( test_data == NULL )
  {
    FTM_MSG_2( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_release: Tech %d, Sub %d - NULL Test Data",
               technology, subscriber );
    return FALSE;
  } /* if ( test_data == NULL ) */

  /* Param Validation */
  if ( ( technology >= RFM_NUM_MODES ) || ( subscriber >= RFM_SUBSCRIBER_NUM ) )
  {
    FTM_MSG_2( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_release: Tech %d, Sub %d - Bad Param",
               technology, subscriber );
    return FALSE;
  } /* if ( ( technology >= RFM_NUM_MODES ) || ( subscriber >= RFM_SUBSCRIBER_NUM ) ) */

  if ( test_data->subscriber_owner[subscriber] == technology )
  {
    test_data->subscriber_owner[subscriber] = RFM_PARKED_MODE;
    FTM_MSG_2( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_release: Tech %d, Sub %d - Released", 
               technology, subscriber );
  } /* if ( test_data->subscriber_owner[subscriber] == technology ) */
  else
  {
    FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_subscriber_release: Tech %d, Sub %d - Skipped (Owned by %d)", 
               technology, subscriber, test_data->subscriber_owner[subscriber] );
    ret_val = FALSE;
  } /* if !( test_data->subscriber_owner[subscriber] == technology ) */

  return ret_val;

} /* ftm_rf_test_subscriber_release */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Response

  @brief
  Functions to manage subscriber allocation management
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Allcoate Response packet
 
  @details
  This is a callback function passed into each command handler function to allocate memeory for response packet.
  This function allocates memory of size equating the summation of command specific payload and common command
  payload
 
  @param response_data
  Pointer to the structure containing the response data
 
  @param cmd_payload_rsp_size
  Command specific response size
 
  @return
  Pointer to the memory to which each command is allowed to write
*/
static void*
ftm_rf_test_allocate_response
(
  struct ftm_rf_test_reponse_data_t *response_data, 
  uint16 cmd_payload_rsp_size
)
{
  ftm_rf_test_header_rsp_type *header_rsp;
  uint16 total_rsp_size;
  uint8 *payload_ptr_for_command = NULL;

  if ( response_data == NULL )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_allocate_response: NULL param" );
    return payload_ptr_for_command;
  } /* if ( response_data == NULL ) */

  if ( ( cmd_payload_rsp_size == 0 ) && ( response_data->common_framework_alloc == FALSE ) )
  {
    FTM_MSG( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_allocate_response: Skipping alloc for 0 size" );
    return payload_ptr_for_command;
  } /* if ( ( cmd_payload_rsp_size == 0 ) && ( response_data->common_framework_alloc == FALSE ) ) */

  /* Compute the total response length */
  total_rsp_size = sizeof(ftm_rf_test_header_rsp_type) + cmd_payload_rsp_size;

  /* Allocate memory for a new reponse packet in the ftm heap */
  response_data->rsp_pkt = ftmdiag_create_new_pkt( total_rsp_size );  

  /* Make sure that the allocation is successfull */
  if ( response_data->rsp_pkt.pkt_payload != NULL )
  {
    /* Populate the Payload sizes */
    response_data->cmd_payload_rsp_size = cmd_payload_rsp_size;
    response_data->total_payload_rsp_size = total_rsp_size;

    /* Shorten header_rsp size */
    header_rsp = response_data->rsp_pkt.pkt_payload;

    /* Populate the remaining params */
    header_rsp->diag_header.diag_cmd = response_data->diag_request_header->diag_header.diag_cmd;
    header_rsp->diag_header.diag_subsys_id = response_data->diag_request_header->diag_header.diag_subsys_id;
    header_rsp->diag_header.rf_subsys_id = response_data->diag_request_header->diag_header.rf_subsys_id;
    header_rsp->diag_header.test_command = response_data->diag_request_header->diag_header.test_command;
    header_rsp->diag_header.req_len = response_data->diag_request_header->diag_header.req_len;
    header_rsp->diag_header.rsp_len = response_data->total_payload_rsp_size;

    /* Determine the payload pointer for the command. This should be after the common framework payload*/
    payload_ptr_for_command = (uint8*)response_data->rsp_pkt.pkt_payload;
    if ( response_data->common_framework_alloc == FALSE )
    {
      payload_ptr_for_command += sizeof(ftm_rf_test_header_rsp_type);
    } /* if ( response_data->common_framework_alloc == FALSE ) */
    else
    {
      payload_ptr_for_command += sizeof(ftm_rf_test_diag_header_type);
    } /* if !( response_data->common_framework_alloc == FALSE ) */

    response_data->is_allocated = TRUE;

    FTM_MSG_2( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_allocate_response: Main Payload 0x%x, Command Payload 0x%x",
               (uint32)response_data->rsp_pkt.pkt_payload, (uint32)payload_ptr_for_command );
    
  } /* if ( response_data->rsp_pkt.pkt_payload != NULL ) */
  else
  {
    response_data->is_allocated = FALSE;
    response_data->error_code |= (1<<FTM_RF_TEST_EC_RSP_CREATION);

    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_allocate_response: Malloc Failed" );
  } /* if !( response_data->rsp_pkt.pkt_payload != NULL ) */

  return (void*)payload_ptr_for_command;

} /* ftm_rf_test_allocate_response */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Initialization

  @brief
  Functions to initialize and de-initialize FTM Test module
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Initialize FTM Test module
 
  @details
  This function initializes the FTM test module. Initialization process includes allocating the memeory for
  FTM test state data. If the data was already allocated, then the content of the memroy will be cleared
 
  @return
  Flag indicating the status of initialization
*/
boolean
ftm_rf_test_init
(
  void
)
{
  boolean ret_val = TRUE;

  /* Allocate state data memory, if it was not allocated */
  if ( ftm_rf_test_data == NULL )
  {
    ftm_rf_test_data = (ftm_rf_test_data_type*)modem_mem_alloc( sizeof(ftm_rf_test_data_type), 
                                                                MODEM_MEM_CLIENT_RFA );
  } /* if ( ftm_test_data == NULL ) */

  /* Check if the memory was successfully allocated or not */
  if ( ftm_rf_test_data != NULL )
  {
    /* Clear the memory */
    memset( ftm_rf_test_data, 0x0, sizeof(ftm_rf_test_data_type) );
  } /* if ( ftm_test_data != NULL ) */

  /* Initialize Radio Config Command */
  ret_val &= ftm_rf_test_radio_config_init( &(ftm_rf_test_data->dispatch_data[FTM_RF_TEST_CMD_RADIO_CONFIGURE]) );

  /* Initialize Rx Measure Command */
  ret_val &= ftm_rf_test_rx_measure_init( &(ftm_rf_test_data->dispatch_data[FTM_RF_TEST_CMD_RX_MEASURE]) );

  /* Initialize Tx Control Command */
  ret_val &= ftm_rf_test_tx_control_init( &(ftm_rf_test_data->dispatch_data[FTM_RF_TEST_CMD_TX_CONTROL]) );

  FTM_MSG_1( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_init: [Status %d] Done", ret_val );

  return ret_val;

} /* ftm_rf_test_init */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  DeInitialize FTM Test module
 
  @details
  This function deinitializes the FTM test module. DeInitialization process includes deallocating the memeory for
  FTM test state data.
 
  @return
  Flag indicating the status of deinitialization
*/
boolean
ftm_rf_test_deinit
(
  void
)
{
  boolean ret_val = TRUE;

  /* Initialize Radio Config Command */
  ret_val &= ftm_rf_test_radio_config_deinit( &(ftm_rf_test_data->dispatch_data[FTM_RF_TEST_CMD_RADIO_CONFIGURE]) );

  /* De-Initialize Rx measure Command */
  ret_val &= ftm_rf_test_rx_measure_deinit( &(ftm_rf_test_data->dispatch_data[FTM_RF_TEST_CMD_RX_MEASURE]) );

  /* De-Initialize Tx Control Command */
  ret_val &= ftm_rf_test_tx_control_deinit( &(ftm_rf_test_data->dispatch_data[FTM_RF_TEST_CMD_TX_CONTROL]) );

  /* Allocate state data memory, if it was not allocated */
  if ( ftm_rf_test_data != NULL )
  {
    modem_mem_free( ftm_rf_test_data, MODEM_MEM_CLIENT_RFA );
    ftm_rf_test_data = NULL;

  } /* if ( ftm_test_data == NULL ) */

  FTM_MSG_1( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_deinit: [Status %d] Done", ret_val );

  return ret_val;

} /* ftm_rf_test_deinit */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Dispatcher

  @brief
  Functions to handle dispatching of RF Test command
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  FTM Test Dispatch function

  @details
  This function handles requests to run test for various test commands by dispatching to the appropriate functions.

  @param *ftm_req_data
  Request Data Packet
 
  @param request_packet_size
  Size of diag request packet

  @return
  Response packet to the diag
*/
ftm_rsp_pkt_type
ftm_rf_test_dispatch
(
  void *ftm_req_data,
  uint32 request_packet_size
)
{
  ftm_rf_test_req_header_type *test_header_ip = (ftm_rf_test_req_header_type*)ftm_req_data;
  ftm_rf_test_req_header_type *test_header_un = NULL;
  ftm_rf_test_comamnd_type test_command = 0;
  ftm_rf_test_command_header_type *command_header = NULL;
  ftm_rf_test_data_type *test_data = NULL;
  ftm_rf_test_command_data_type command_data;
  boolean command_ret = TRUE;
  ftm_rf_test_command_rsp_header_type *rsp_payload = NULL;
  ftm_rf_test_field_error_code_mask_type error_code = 0;
  rfm_mode_enum_type technology = RFM_INVALID_MODE;
  rfm_subscriber_type subscriber = RFM_SUBSCRIBER_INVALID;
  int8 uncompress_status = Z_ERRNO; /* Packet Uncompression Status */
  uint32 uncompress_size = 0;

  /* Populate command_instance */
  memset( &command_data, 0, sizeof(ftm_rf_test_command_data_type) );
  
  /* Check if the payload is compressed or not: If the first 4 bytes is non zero, then the payload is considered
  to be a compressed payload. Thus uncompression is needed. */
  if ( test_header_ip->command_header.reserved != 0 )
  {
    /*----------------------------------------------------------------------------------------------------------------*/
    /* Uncompress Payload */
    /*----------------------------------------------------------------------------------------------------------------*/

    /* Malloc memory to store uncompressed data */
    test_header_un = (ftm_rf_test_req_header_type*)modem_mem_alloc( test_header_ip->diag_header.req_len, 
                                                                    MODEM_MEM_CLIENT_RFA );

    /* NULL pointer check */
    if ( test_header_un != NULL )
    {
      /* Decompress FTM Payload Section */

      /* Update uncompress size */
      uncompress_size = test_header_ip->diag_header.req_len;

      /* Copy the diag header as it is not compressed */
      memscpy( test_header_un, sizeof(ftm_rf_test_diag_header_type), 
               test_header_ip, sizeof(ftm_rf_test_diag_header_type) );
      
      /* Uncompress the data */
      uncompress_status = uncompress( (uint8*)(&test_header_un->command_header),   
                                      &uncompress_size,
                                      (const uint8 *)(&test_header_ip->command_header), 
                                      request_packet_size );
      
      /* Error checking for uncompress status */
      if ( uncompress_status != Z_OK )
      {
        FTM_MSG( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: Uncompression Failed" );
        error_code |= (1<<FTM_RF_TEST_EC_UMCOMPRESS_FAIL);
      } /* if ( uncompress_status != Z_OK ) */
      else
      {
        FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [%d -> %d (%d) bytes] - Uncompression done",
                   request_packet_size, uncompress_size, test_header_ip->diag_header.req_len ); 
      } /* if !( uncompress_status != Z_OK ) */
    } /* if ( test_header_un != NULL ) */
    else
    {
      FTM_MSG( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: Uncompression Malloc Failed" );
      error_code |= (1<<FTM_RF_TEST_EC_UMCOMPRESS_FAIL);
      error_code |= (1<<FTM_RF_TEST_EC_MALLOC_FAILRE);
    } /* if !( test_header_un != NULL ) */

  } /* if ( test_header_ip->command_header.reserved != 0 ) */
  else
  {
    test_header_un = test_header_ip;
    FTM_MSG( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: Already Uncompressed version" );
  } /* if !( test_header_ip->command_header.reserved != 0 ) */

  /* Proceed, if everything is okay. */
  if ( error_code == 0 )
  {
    /* Get Test Data pointer */
    test_data = ftm_rf_test_get_data();

    /* Get the pointer to Command Header */
    command_header = &(test_header_un->command_header);

    /* Get the Test command */
    test_command = test_header_un->diag_header.test_command;

    /* Get Technology and subscription */
    technology = ftm_common_ftm_tech_to_rfm_mode( (ftm_rf_technology_type)command_header->technology );
    subscriber = ftm_common_convert_to_subscriber( (ftm_rf_test_subscriber_enum_type)command_header->subscriber );

    command_data.subscriber = subscriber;
    command_data.technology = technology;
    command_data.diag_request_data = (void*)test_header_un;
    command_data.response_data.alloc_response_fp = ftm_rf_test_allocate_response;
    command_data.response_data.diag_request_header = (ftm_rf_test_req_header_type*)test_header_un;

    /* Perform NULL pointer check */
    if ( test_data != NULL )
    {
      /*--------------------------------------------------------------------------------------------------------------*/
      /* Acquire Subscriber */
      /*--------------------------------------------------------------------------------------------------------------*/

      /* Make sure that the technolgy acquires the subscriber */
      if ( TRUE == ftm_rf_test_subscriber_acquire( technology, subscriber, test_data ) )
      {

        /*------------------------------------------------------------------------------------------------------------*/
        /* Dispatch Commands */
        /*------------------------------------------------------------------------------------------------------------*/
        if ( test_command < FTM_RF_TEST_CMD_NUM )
        {
          command_data.dispatch_data = (void*)(&(test_data->dispatch_data[test_command]));
          if ( test_data->dispatch_data[test_command].command_handler != NULL )
          {
            command_ret = test_data->dispatch_data[test_command].command_handler( &command_data );
          } /* if ( test_data->dispatch_data[test_command].command_handler != NULL ) */
          else
          {
            FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - "
                                  "NULL handler", subscriber, technology, test_command ); 
            error_code |= (1<<FTM_RF_TEST_EC_BAD_CMD_HANDLER);
          } /* !if ( test_data->dispatch_data[test_command].command_handler != NULL ) */
        } /* if ( test_command < FTM_RF_TEST_CMD_NUM ) */
        else
        {
          FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - Bad Command", 
                     subscriber, technology, test_command ); 
          error_code |= (1<<FTM_RF_TEST_EC_BAD_CMD);
        } /* if !( test_command < FTM_RF_TEST_CMD_NUM ) */

        /* Update error code based on CB return status */
        if ( command_ret == FALSE )
        {
          FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - Handler "
                                "Failure", subscriber, technology, test_command ); 
          error_code |= (1<<FTM_RF_TEST_CMD_HANDLER_FAIL);
        } /* if ( command_ret == FALSE ) */

        /*------------------------------------------------------------------------------------------------------------*/
        /* Release Subscriber */
        /*------------------------------------------------------------------------------------------------------------*/

        /* Release the subscriber, if applicable */
        if ( ftm_rf_test_radio_config_sub_release_ready( &(test_data->dispatch_data[FTM_RF_TEST_CMD_RADIO_CONFIGURE]), 
                                                         subscriber, technology ) == TRUE )
        {
          if ( ftm_rf_test_subscriber_release( technology, subscriber, test_data ) == FALSE )
          {
            FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - "
                                  "SUB Release Failure", subscriber, technology, test_command ); 
            error_code |= (1<<FTM_RF_TEST_EC_SUB_RELEASE);
          } /* if ( ftm_rf_test_subscriber_release( technology, subscription, test_data ) == FALSE ) */
        } /* if(ftm_rf_test_radio_config_sub_release_ready(&(test_data->dispatch_data[FTM_RF_TEST_CMD_RADIO_CONFIGURE]), 
                                                         technology ) == TRUE ) */
      
        FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - Handled", 
                   subscriber, technology, test_command );
      } /* if ( TRUE == ftm_rf_test_subscriber_acquire( technology, subscription, test_data ) ) */
      else
      {
        error_code |= (1<<FTM_RF_TEST_EC_SUB_ALLOC);
        FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - SUB failure", 
                   subscriber, technology, test_command );
      } /* if !( TRUE == ftm_rf_test_subscriber_acquire( technology, subscription, test_data ) ) */
    } /* if ( test_data != NULL ) */
    else
    {
      error_code |= (1<<FTM_RF_TEST_EC_NULL_PARAM);
      FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - NULL Test Data", 
                 subscriber, technology, test_command );
    } /* if !( test_data != NULL ) */

  } /* if ( error_code == 0 ) */

  /*----------------------------------------------------------------------------------------------------------------*/
  /* Prepare response packet */
  /*----------------------------------------------------------------------------------------------------------------*/

  /* If the response packet was not allocated by command handler, then allocate the memory  */
  if ( command_data.response_data.is_allocated == FALSE )
  {
    command_data.response_data.common_framework_alloc = TRUE;
    rsp_payload = ftm_rf_test_allocate_response( &(command_data.response_data), 0 );
  } /* if ( command_instance.response_data.is_allocated == FALSE ) */
  else
  {
    rsp_payload = 
      &(((ftm_rf_test_header_rsp_type*)(command_data.response_data.rsp_pkt.pkt_payload))->command_header);
  } /* if !( command_instance.response_data.is_allocated == FALSE ) */

  /* Check if he response payload was successfully allocated or not (or it was ever attempted to be allocated or not). 
  If it is not; then send a default BAD response packet as a last resort (to avoid crash in DIAG) */
  if ( rsp_payload != NULL )
  {
    /* Populate Version */
    rsp_payload->reserved = 0;
    rsp_payload->version = 0;

    /* Populate Error Code */
    rsp_payload->common_error_code = 0;
    rsp_payload->common_error_code |= command_data.response_data.error_code;
    rsp_payload->common_error_code = error_code;

    /* Return */
    return command_data.response_data.rsp_pkt;
  } /* if ( rsp_payload != NULL ) */
  else
  {
    /* Create Response Packet : Defaults to Bad Packet */
    ftm_rsp_pkt_type ftm_rsp_data = ftm_common_create_rsp_pkt( FTM_COMMON_BAD_RSP );
    FTM_MSG_3( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_dispatch: [SUB %d TECH %d] Command %d - Sending default "
                          "BAD response", subscriber, technology, test_command );
    return ftm_rsp_data;
  } /* if !( rsp_payload != NULL ) */
  
} /* ftm_rf_test_dispatch */

/*! @} */

/*====================================================================================================================*/
/*!
  @name Query

  @brief
  Functions to handle FTM RF TEST query
*/
/*! @{ */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! 
  @brief
  Query Dispatch data for given RF Test command
 
  @details
  This function will return the pointer to the dispatch data for given FTM RF Test command
 
  @param ftm_rf_test_command
  FTM RF Test command
 
  @return
  Return a pointer to dispatch data
*/
ftm_rf_test_command_dispatch_data_t*
ftm_rf_test_get_dispatch_data
(
  ftm_rf_test_command_enum_type ftm_rf_test_command
)
{
  ftm_rf_test_data_type *test_data = NULL;

  /* Param validation */
  if ( ftm_rf_test_command >= FTM_RF_TEST_CMD_NUM )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_get_dispatch_data: Command %d - Invalid" );
    return NULL;
  } /* if ( ftm_rf_test_command >= FTM_RF_TEST_CMD_NUM ) */

  /* Get Test Data pointer */
  test_data = ftm_rf_test_get_data();

  /* Param validation */
  if ( test_data == NULL )
  {
    FTM_MSG( FTM_ERROR, "[FTM.RFTEST][CONTROL]ftm_rf_test_get_dispatch_data: Command %d - NULL Test Data" );
    return NULL;
  } /* if ( test_data == NULL ) */

  return &(test_data->dispatch_data[ftm_rf_test_command]);

} /* ftm_rf_test_get_dispatch_data */

/*--------------------------------------------------------------------------------------------------------------------*/
/*! 
  @brief
  Common Helper API for unpacking a list of properties. The API goes through all items in the list, and invokes
  command_unpack_callback each time it has collected a bundle of data to be processed. All items between two delimiters
  are consider to belong to one bundle of data
 
  Property Type      Rx Carrier  -- Delimiter - command_unpack_callback( &Rx Carrier, 5 items)
  Property Value     0
  Property Type      Device
  Property Value     0
  Property Type      Device 
  Property Value     1
  Property Type      Band
  Property Value     0
  Property Type      Channel
  Property Value     0
  Property Type      Rx Carrier  -- Delimiter - command_unpack_callback( &Rx Carrier, 2 items)
  Property Value     1
  Property Type      Device
  Property Value     2
 
  @return
  Error Code
*/
ftm_rf_test_field_error_code_mask_type
ftm_rf_test_unpack_property_list
(
  ftm_rf_test_field_num_prop_type total_num_of_props,
  ftm_rf_test_property_data_type *property_list,
  void* command_mode_data,
  boolean debug_mode,
  ftm_rf_test_unpack_fp_type command_unpack_callback,
  ftm_rf_test_property_delimiter_fp_type delimiter_callback
)
{

  ftm_rf_test_field_error_code_mask_type error_code = 0;
  ftm_rf_test_property_data_type *prop_ptr = NULL;
  ftm_rf_test_property_data_type *action_to_process_prop_ptr = NULL;
  uint32 prop_index = 0;
  uint32 carrier = 0;
  uint32 num_of_delimiter = 0;
  uint32 accum_num_of_props = 0;

  /*------------------------------------------------------------------------------------------------------------------*/
  /* Variables used to temporarily store the unpacked data; until they are pushed to the unpack structure. */
  /* These variables must be reset to default value before starting to unpack a property group. */
  /*------------------------------------------------------------------------------------------------------------------*/
  ftm_rf_test_field_property_type property_type = FTM_RF_MEASURE_PROP_UNASSIGNED;
  /*------------------------------------------------------------------------------------------------------------------*/

  if ( property_list == NULL || delimiter_callback == NULL ||
       command_unpack_callback == NULL )
  {
    FTM_MSG( FTM_HIGH, "[FTM.RFTEST][UNPACK]: Null param " );
    error_code |= (1<<FTM_RF_TEST_RC_EC_NULL_PARAM);
    return error_code;
  } /* if ( ( property_list == NULL ) || ( rx_measure_data == NULL ) ) */

  /*------------------------------------------------------------------------------------------------------------------*/
  /* Reset the local variables to store the unpackes data for each property group */  
  /*------------------------------------------------------------------------------------------------------------------*/
  /*------------------------------------------------------------------------------------------------------------------*/

  /* Initialize Propert pointer */
  num_of_delimiter = 0;
  accum_num_of_props = 0;
  prop_ptr = property_list;
  action_to_process_prop_ptr = prop_ptr;
  /*------------------------------------------------------------------------------------------------------------------*/
  /* Populate the Device to Measure list */
  /*------------------------------------------------------------------------------------------------------------------*/

  /* Continue, if the number if properties is not 0 */
  if ( total_num_of_props != 0 )
  {
    /* Loop thrrough the number of properties. Loop is allowed to till num_of_props; to latch the last propert group */
    for ( prop_index = 0; prop_index <= total_num_of_props; prop_index++ )
    {
      /* Get property type */
      property_type = prop_ptr->property_type;
      
      /* If the debug mode is enabled, then print property information */
      if ( debug_mode == TRUE )
      {
        FTM_MSG_3( FTM_HIGH, "[FTM.RFTEST][UNPACK]: [%2d][Property Type %2d][Value %3d]", 
                   prop_index, property_type, prop_ptr->property_val);
      } /* if ( debug_mode == TRUE ) */

      if ( delimiter_callback(property_type) || prop_index == total_num_of_props )
      {
        num_of_delimiter++;

        if ( debug_mode == TRUE )
        {
          FTM_MSG_3( FTM_MED, "[FTM.RFTEST][UNPACK]: [%2d][DELIMITER %2d][ITEMS %d]", 
                                prop_index, num_of_delimiter, accum_num_of_props);

        } /* if ( debug_mode == TRUE ) */
        
        if ( num_of_delimiter == 1 )
        {
          /* Skip Dispatch since it's the first delimiter */
        }
        else
        {
          error_code = command_unpack_callback(accum_num_of_props, 
                                               action_to_process_prop_ptr,
                                               command_mode_data,
                                               debug_mode);
        }

        action_to_process_prop_ptr = prop_ptr;

        accum_num_of_props = 0;
      }

      /* Continue searching for delimiter or end of list. Increment number
      of params for the current action */
      accum_num_of_props++;
      
      /* Last step of property extraction. Once the end is reached, don't
      access anymore memory */
      if ( prop_index == total_num_of_props )
      {
        break;
      }

      /* Increment the poperty pointer */
      prop_ptr++;
    } /* for ( prop_index = 0; prop_index < num_of_props; prop_index++ ) */
  } /* if ( num_of_props != 0 ) */
 
  /*------------------------------------------------------------------------------------------------------------------*/
  /* Error handling */
  /*------------------------------------------------------------------------------------------------------------------*/

  /* If error code is not zero, then clear the unpack data */
  if ( error_code != 0 )
  {
    FTM_MSG_1( FTM_ERROR, "[FTM.RFTEST][UNPACK]: [ErrorCode 0x%x] Clearing unpack data",
               error_code );
  } /* if ( error_code != 0 ) */

  return error_code;
}

/*====================================================================================================================*/
/*!
  @name Repack Rx Measure Data from Default format to payload property format.
  Future development for other formats can be based on this API.

  @brief
  Functions to handle Rx Measure command
*/
/*! @{ */

boolean
ftm_rf_test_alloc_response_helper
(
  ftm_rf_test_reponse_data_type* response_allocator_data,
  void* response_payload,
  uint32 response_payload_size
)
{
  boolean ret_val = TRUE;
  void* rsp_payload;

  if ( response_payload == NULL || response_allocator_data == NULL )
  {
    FTM_MSG(FTM_ERROR,"[FTM.RFTEST][RSP][REPACK]: NULL POINTER!");
    ret_val = FALSE;
  }
  else
  {
    /* Create Response packet */
    rsp_payload = response_allocator_data->alloc_response_fp( response_allocator_data, response_payload_size);

    /* NULL pointer check */
    if ( rsp_payload != NULL )
    {
      /* Populate response packet */
      memscpy( rsp_payload, response_payload_size, response_payload, response_payload_size );
    } /* if ( rsp_payload != NULL ) */
    else
    {
      ret_val = FALSE;
      FTM_MSG( FTM_ERROR, "[FTM.RFTEST][RSP][REPACK]: Response malloc failure" );
    } /* if !( rsp_payload != NULL ) */
  }

  return ret_val; 
}/* ftm_rf_test_repack_property_list */

/*! @} */

/*! @} */ /* FTM_TEST_TOOLS_SW_MC */


