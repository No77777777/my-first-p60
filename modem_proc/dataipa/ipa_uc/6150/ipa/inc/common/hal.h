/*!
  @file
  hal.h

  @brief
  IPA SWI defintions

*/
/*===========================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/dataipa.mpss/3.2/ipa_uc/6150/ipa/inc/common/hal.h#1 $
==============================================================================*/

#ifndef HAL_H_
#define HAL_H_

#include "msmhwioreg.h"

#define HAL_BAM_PIPE_MODE_SYSTEM     0x20   /* BAM - System Memory mode */
#define HAL_BAM_PIPE_ENABLE          0x2    /* Enable pipe register value*/

/**
 * @brief   The number of BAM pipes in ipa
*/
#define HAL_IPAIF_NUM_OF_PIPES 31   // GSI supports 31 channels; BAM supports 20 channels; Take the larger of the two.

#define HAL_UC_MEM_ADDR_FIRST	    HWIO_ADDR(IPA_RAM_UC_MEM_ADDR_FIRST)
#define HAL_UC_MEM_ADDR_LAST   	 HWIO_ADDR(IPA_RAM_UC_MEM_ADDR_FIRST) + 0x3400  //(13K HW FIFO Region)

typedef enum
{
   HAL_QMB_DIRECTION_READ  = 0, 	/**< Read from System NOC to local memory.  */
   HAL_QMB_DIRECTION_WRITE = 1 	/**< Write to System NOC from local memory.  */
}HAL_QMB_DIRECTIONS;

/**
 * @brief   Values that represent HAL_QMB_LOCAL_ADDRESS_TYPE. 
*/
typedef enum
{
   HAL_QMB_LOCAL_ADDRESS_UC  = 0,	   /**< uC local memory.  */
   HAL_QMB_LOCAL_ADDRESS_IPA = 1 	   /**< IPA shared memory  */
}HAL_QMB_LOCAL_ADDRESS_TYPE;

/**
 * @brief   QMB posting modes 
*/
typedef enum
{
   HAL_QMB_POSTING_DATA_POSTED     = 0, //Async QMB Write comes back as soon as transacation is placed on bus (NA for READ)
   HAL_QMB_POSTING_RESP_POSTED      = 1, //Async QMB Read/Write comes back only after Slave asserts the response signal
   HAL_QMB_POSTING_DATA_COMPLETE   = 2, //Sync QMB Write comes back as soon as transacation is placed on bus (NA for READ)
   HAL_QMB_POSTING_RESP_COMPLETE    = 3 //Sync QMB Read/Write comes back only after Slave asserts the response signal
}HAL_QMB_POSTING;

/**
 * @brief   Values that represent the various execution environments.
*/
typedef enum
{
   HAL_EE_A5      = 0,
   HAL_EE_Q6      = 1,
   HAL_EE_GSI_UC  = 2, /** This EE needs to be used for GSI register access by uC*/
   HAL_EE_UC      = 3 /**< micro controller */
} HAL_EE_TYPE;

#endif /* HAL_H_ */
