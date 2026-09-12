#ifndef __ABTIMEOUT_H__
#define __ABTIMEOUT_H__
/*============================================================================

FILE:      ABTtimeout.h

DESCRIPTION: Function and data structure declarations for AHB timeout driver Interface

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Header: //components/rel/core.mpss/10.0/api/systemdrivers/icb/ABTimeout.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/05/16  bc     Removing non-public API.
2016/02/12  pm     Branch for Adrastea (Modem User PD)
2014/12/22  tb     Added optional disable for clocks
2014/03/14  ddk    Added support for additional clock voting.
2014/03/10  ddk    Added support for hwio mapping.
2013/11/14  tb     Added support for multiple enable/status registers
2013/04/16  pm     Added interrupt priority
2012/10/04  av     Support for disabling ABT
2012/05/31  av     Created

          Copyright (c) 2012-2014, 2016 QUALCOMM Technologies, Incorporated.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
============================================================================*/
#include "com_dtypes.h"

/*============================================================================
                          TYPE DEFINITION
============================================================================*/

/**
 * ICBCFG Error types.
 */ 
typedef enum
{
  ABT_SUCCESS = 0,                  /**< Success */
  ABT_ERROR_INVALID_PARAM,          /**< Invalid Parameters */
  ABT_ERROR_INIT_FAILURE,           /**< Initialization Failure */
  ABT_ERROR_INIT_NOT_DONE,			/**< ABT Init not done */
  ABT_ERROR_CLOCK_NOT_ENABLED,		/**< Required clocks are not enabled */
  /* Add new errors here */         

  ABT_ERROR_SIZE = 0x7FFFFFFF,          /* force 32-bit enum */
  ABT_ERROR = -1
}ABT_error_type;

/*============================================================================
                                 FUNCTIONS
============================================================================*/

//*============================================================================*/
/**
@brief 
      Initializes AHB Timeout driver. It configures interrupt and timeout values
      for AHB timeout slaves
 
@param[in]  None.

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
void ABT_Init(void);

//*============================================================================*/
/**
@brief 
      De-Initializes AHB config driver.
 
@param[in]  None.

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
void ABT_DeInit(void);

/*===========================================================================*/
/**
@brief 
      This function is handling shadow register interrupt which is routed 
      outside of wcss for captures and logs syndrome registers from
      corresponding ABT slaves.
 
@param[in] uint32 intr_vector - Interrupt vector ID.

@return    
      ABT_ERROR_INIT_FAILURE - Unable to process this ABT interrupt
      ABT_SUCCESS - Success

@dependencies
      None.
 
@sideeffects 
      ERR_FATAL() is called if an AHB Timeout is found.
*/ 
/*==============================================================================*/
ABT_error_type ABT_Handle_External_Interrupt(uint32 intr_vector );


#endif /* __ABTIMEOUT_H__ */
