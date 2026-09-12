/*!
  @file ftm_lte_rf_test.h

  @brief
  This files contains the definition of the rf test framework in addition to some common utilities 
  
*/

/*===========================================================================

  Copyright (c) 2009 - 2015 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_lte.mpss/3.11/api/ftm_lte_rf_test.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/18/16   vkm      Fix call flow issue for ftm mdsp exit
07/31/15   kali     Initial Check-in
===========================================================================*/

#ifndef FTM_LTE_RF_TEST_H
#define FTM_LTE_RF_TEST_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "ftm.h"
#include "ftm_rf_test_radio_config.h"
#include "rflte_msg.h"
#include "ftm_rf_test_rx_measure.h"
/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/



/*===========================================================================

  FUNCTION:  ftm_lte_msgr_init  

===========================================================================*/
/*!
  @brief
  FTM LTE initialization for Message router


  @return
  NONE
*/
/*=========================================================================*/
void ftm_lte_rf_test_init(void);

/*===========================================================================

  FUNCTION:  ftm_lte_msgr_deinit  

===========================================================================*/
/*!
  @brief
  FTM LTE de-initialization for Message router


  @return
  NONE
*/
/*=========================================================================*/
void ftm_lte_rf_test_deinit(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Handles the LTE radio test command
 
  @details
  This function is used to handle the LTE radio test command sent from the common framework
  Pre-requisite : The API should be registered as a callback.
 
  @param dispatch_data
  Poitner to the command_unpack_data containing all the params required for the radio setup.
 
  @return
  Error code indicating the status of the API
*/
ftm_rf_test_field_error_code_mask_type ftm_lte_rf_test_radio_config(ftm_rf_test_radio_config_unpack_data_t *command_unpack_data);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Call back to handle Rx Measure command

  @param unpacked_data
  Poitner to the command_unpack_data containing all the params required for 
  corresponding operation

  @return
  Error code indicating the status of the API
*/
ftm_rf_test_field_error_code_mask_type ftm_lte_rf_test_rx_measure(ftm_rf_test_rx_measure_unpacked_data_t *query_data, uint32 num_of_measurements);

/*----------------------------------------------------------------------------*/
/*! LTE specfic enter mode param structure  */
typedef struct
{
   rf_lte_dev_type device_type;
   /*!< Enter mode device type */
}
rfm_lte_enter_mode_param;

typedef struct
{
   int16 num_buffers;
   uint32 handle[50]; // Need to remove hard code value. for now set it to a bigger array.
   ftm_rf_test_device_radio_trx_t txrx[50];
}
deallocated_buffers;

typedef struct
{
   uint8 carrier;
   rfcom_lte_earfcn_type rx_channel;
   rfcom_lte_bw_type bandwidth;
   boolean is_horxd_enabled;
} carrier_inf;

uint32 num_current_chain;
uint32 num_request_chain;
rfa_rf_lte_config_req_s rflte_config_req;
rfa_rf_lte_config_cnf_s rflte_config_cnf;
rfa_rf_lte_config_req_s rflte_current_config;
uint32 total_drxpath_per_carrier[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
carrier_inf carriers[RFLTE_MC_MAX_CELL_IDX_SUPPORTED];
uint8 total_carrier;
deallocated_buffers deallocate_buffers;

boolean is_ftm_mdsp_enabled;

typedef enum
{
   UNTOUCH = 0,
   USE,
   DEALLOCATE,
   DISABLE_DEALLOCATE,
   INVALID
}ActionFlag;

#endif /*  FTM_LTE_RF_TEST_H */
