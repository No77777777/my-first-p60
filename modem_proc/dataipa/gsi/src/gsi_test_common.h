/*!
  @file
  gsi_test_common.h

  @brief
  This file contains declaration of the GSI driver's test wrappers for the
  common operations.


*/
/*===========================================================================

  Copyright (c) 2015-2016 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/17    tz      8-Bytes TRE support
04/04/16   da      Add SSR-related tests
02/01/16   tz      KW error fixes
03/12/15   tz      GSI test-framework changes
03/03/15   tz      Initial version
===========================================================================*/
#ifndef GSI_TEST_COMMON_H
#define GSI_TEST_COMMON_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include <comdef.h>
#include "gsi_hw_common.h"
#include "gsi.h"
#include "gsi_osal.h"
#include "gsi_core.h"
#include "ipa_cfg.h"
/*==============================================================================

                      Macros

==============================================================================*/

#ifdef FEATURE_IPA_ON_TARGET_TEST
  #define GSI_OSAL_TEST_MSG(msg) GSI_OSAL_MSG(msg)
  #define GSI_OSAL_TEST_MSG_ERROR(msg) GSI_OSAL_MSG_ERROR(msg) 
  #define GSI_OSAL_TEST_MSG_ERROR_1(msg, a1) GSI_OSAL_MSG_ERROR_1(msg, a1) 
  #define GSI_OSAL_TEST_MSG_ERROR_2(msg, a1, a2) GSI_OSAL_MSG_ERROR_2(msg, a1, a2) 
  #define GSI_OSAL_TEST_MSG_ERROR_3(msg, a1, a2, a3) \
      GSI_OSAL_MSG_ERROR_3(msg, (a1), (a2), (a3))
  #define GSI_OSAL_TEST_MSG_SPRINTF_1(msg, a1) IPA_MSG_SPRINTF_1(msg, a1)
  #define GSI_OSAL_TEST_MSG_SPRINTF_2(msg, a1, a2) GSI_OSAL_MSG_SPRINTF_2(msg, a1, a2) 
  #define GSI_OSAL_TEST_MSG_SPRINTF_3(msg, a1, a2, a3) GSI_OSAL_MSG_SPRINTF_3(msg, a1, a2, a3) 
  #define GSI_OSAL_TEST_MSG_SPRINTF_4(msg, a1, a2, a3, a4)  GSI_OSAL_MSG_SPRINTF_4(msg, a1, a2, a3, a4) 
#else
  #define GSI_OSAL_TEST_MSG(msg) 
  #define GSI_OSAL_TEST_MSG_ERROR(msg) 
  #define GSI_OSAL_TEST_MSG_ERROR_1(msg, a1) 
  #define GSI_OSAL_TEST_MSG_ERROR_2(msg, a1, a2)
  #define GSI_OSAL_TEST_MSG_ERROR_3(msg, a1, a2, a3) 
  #define GSI_OSAL_TEST_MSG_SPRINTF_1(msg, a1) 
  #define GSI_OSAL_TEST_MSG_SPRINTF_2(msg, a1, a2) 
  #define GSI_OSAL_TEST_MSG_SPRINTF_3(msg, a1, a2, a3) 
  #define GSI_OSAL_TEST_MSG_SPRINTF_4(msg, a1, a2, a3, a4)  
#endif
/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

typedef enum {
  GSI_TEST_SUITE_EE                      = 0x0,
  GSI_TEST_SUITE_CHANNEL                 = 0x1,
  GSI_TEST_SUITE_DATA_TRANSFER           = 0x2,
  GSI_TEST_SUITE_CHANNEL_STATES          = 0x3,
  GSI_TEST_SUITE_FULL_REG                = 0xFF
}gsi_test_suite_type;

typedef enum {
  GSI_TS_EE_INIT_EE_CONFIG               = 0x0,
  GSI_TS_EE_INIT_EE_INVALID_CONFIG       = 0x1,
  GSI_TS_EE_INIT_BACK_TO_BACK_NO_DEINIT  = 0x2,
  GSI_TS_EE_GLOBAL_ERROR                 = 0x3,
  GSI_TS_EE_INIT_BACK_TO_BACK            = 0x4
}gsi_test_suite_ee_test;

typedef enum {
  GSI_TS_EE_INIT_CHANNEL_INIT_NO_ER          = 0x0,
  GSI_TS_EE_INIT_CHANNEL_INIT_WITH_ER        = 0x1,
  GSI_TS_EE_INIT_CHANNEL_INIT_BACK_TO_BACK   = 0x2,
  GSI_TS_EE_INIT_CHANNEL_INIT_INVALID_CONFIG = 0x3,
  GSI_TS_EE_INIT_CHANNEL_ERROR               = 0x4,
  GSI_TS_EE_INIT_CHANNEL_INVALID_ER          = 0x5,
  GSI_TS_EE_INIT_CHANNEL_IS_EMPTY            = 0x6,
  GSI_TS_EE_INIT_CHANNEL_IC_SIMPLE           = 0x7,
  GSI_TS_EE_INIT_CHANNEL_IC_DMA_TASK         = 0x8,
  GSI_TS_EE_INIT_CHANNEL_TRANSFER_SIMPLE     = 0x9,
  GSI_TS_EE_POST_SSR_INIT_INITED_CH          = 0xA,
  GSI_TS_EE_POST_SSR_INIT_STARTED_CH         = 0xB,
  GSI_TS_EE_POST_SSR_STARTED_CH_TRANSFER_SIMPLE
                                             = 0xC,
  GSI_TS_EE_POST_SSR_STARTED_CH_PROD_OPEN_AGGR_TRANSFER_SIMPLE
                                             = 0xD,
  GSI_TS_EE_INIT_CHANNEL_IC_DECIPHER_1       = 0xE,
  GSI_TS_EE_INIT_CHANNEL_IC_DECIPHER_2       = 0xF,
  GSI_TS_EE_INIT_CHANNEL_TRANS_8B            = 0x10,
  GSI_TS_EE_INIT_CHANNEL_TRANS_8B_DEC        = 0x11,
}gsi_test_suite_channel_test;

typedef enum {
  GSI_DATA_TRANSFER_CONFIG_1               = 0x0,
  GSI_DATA_TRANSFER_CONFIG_1_OVERFLOW      = 0x1,
  GSI_DATA_TRANSFER_CONFIG_1_RANDOM        = 0x2,
  GSI_DATA_TRANSFER_CONFIG_2               = 0x3,
  GSI_DATA_TRANSFER_CONFIG_3               = 0x4,
  GSI_DATA_TRANSFER_CONFIG_1_INTMOD        = 0x5,
  GSI_DATA_TRANSFER_CONFIG_1_FLAG_INT      = 0x6,
  GSI_DATA_TRANSFER_CONFIG_1_FLAG_IEOT     = 0x7,
  GSI_DATA_TRANSFER_CONFIG_1_FLAG_IEOB     = 0x8,
  GSI_DATA_TRANSFER_CONFIG_1_RANDOM_POST_SSR
                                           = 0x9,
  GSI_DATA_TRANSFER_CONFIG_1_DECIPHER      = 0xA,
  GSI_DATA_TRANSFER_CONFIG_1_DECIPHER_NOP  = 0xB
}gsi_test_suite_data_transfer_test;


typedef enum {
  GSI_TS_EE_INIT_CHANNEL_STATES_STOP_START       = 0x0,
  GSI_TS_EE_INIT_CHANNEL_STATES_RESET            = 0x1,
  GSI_TS_EE_INIT_CHANNEL_STATES_STOP_START_XFER  = 0x2,
  GSI_TS_EE_INIT_CHANNEL_STATES_RESET_XFER       = 0x3
}gsi_test_suite_channel_states_test;
/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/


/*==============================================================================

                                FUNCTIONS

==============================================================================*/

/*==============================================================================

  FUNCTION:  gsi_test_init

==============================================================================*/
/*!
  @brief
  Initialize GSI test framework


  @param
  

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_init(void);



/*==============================================================================

  FUNCTION:  gsi_test_ts_data_transfer

==============================================================================*/
/*!
  @brief
  Data transfer test suite


  @param
  test_case, test case to run

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_ts_data_transfer(uint32 test_case );

/*==============================================================================

  FUNCTION:  gsi_test_ts_ee

==============================================================================*/
/*!
  @brief
  EE initialization test suite


  @param
  test_case, test case to run

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_ts_ee(uint32 test_case );

/*==============================================================================

  FUNCTION:  gsi_test_ts_channel

==============================================================================*/
/*!
  @brief
  Channel operations test suite


  @param
  test_case, test case to run

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_ts_channel(uint32 test_case );


/*==============================================================================

  FUNCTION:  gsi_test_ts_ch_states

==============================================================================*/
/*!
  @brief
  Channel states operations test suite


  @param
  test_case, test case to run

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_ts_ch_states(uint32 test_case );

/*==============================================================================

  FUNCTION:  gsi_test_common_q6_ut_setup

==============================================================================*/
/*!
  @brief
  Perform GSI/IPA setup for running GSI Unit-Tests on the Q6 regression environment


  @param
  None

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_common_q6_ut_setup(void);

/*==============================================================================

  FUNCTION:  gsi_test_common_q6_ut_destroy

==============================================================================*/
/*!
  @brief
  Perform GSI/IPA test teardown for  GSI Unit-Tests on the Q6 regression environment -
  we recover here the original configuration of the channel/event as configured by the 
  PER SIM initialization code.


  @param
  None

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_common_q6_ut_destroy(void);

/*==============================================================================

  FUNCTION:  gsi_test_run_test_suite

==============================================================================*/
/*!
  @brief
  Run regression test cases for specified test suite


  @param
  test_suite, test suite to run

  @retval GSI_STATUS_SUCCESS
  @retval other error codes for failure

*/
/*============================================================================*/
gsi_status_e gsi_test_run_test_suite(uint32 test_suite );

void gsi_if_invoke_event_hook(gsi_core_event_info_s* event_info);


/* External test wrapper for ciphering configuration */
void ipa_test_engine_ciph_cfg_ex(void);

/* IPA test wrapper for DSM buffer deciphering */
void ipa_test_engine_ciph_dsm_buffer_ex
(
  dsm_item_type *tx_dsm_ptr
);

/* IPA test wrapper getting current deciphering parameters */
void ipa_test_engine_get_cipher_params_ex
(
  ipa_wan_cipher_param_u* params, uint32* count
);

/* IPA test wrapper for setting count parameter */
void ipa_test_engine_set_count_parameter_ex
(
  uint32 count
);

/* IPA test wrapper for ciphering configuration removal */
void ipa_test_engine_ciph_destroy_ex(void);

#endif /* GSI_TEST_COMMON_H */
