#ifndef __ICBERROR_H__
#define __ICBERROR_H__
/*============================================================================

FILE:      icb_error.h

DESCRIPTION: Initialization for ICB bus error handlers

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

$Header: //components/rel/core.mpss/10.0/api/systemdrivers/icb/icb_error.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2017/06/01  sds    Add deferred NoC error programming support
2017/02/08  sds    Created

                Copyright (c) 2017 Qualcomm Technologies Incorporated.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
============================================================================*/
#include "com_dtypes.h"
#include "ClockDefs.h"

/*============================================================================
                          DEFINEs/MACROs
============================================================================*/

/*============================================================================
                          TYPE DEFINITION
============================================================================*/

/*============================================================================
                                 FUNCTIONS
============================================================================*/

//*============================================================================*/
/**
@brief 
      Performs initialization for NOC error handler.
      It enables interrupts required to handle NOC errors.
 
@param[in]  None.

@return    
      None.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
void NOC_Error_Init(void);

//*============================================================================*/
/**
@brief 
      Performs deferred initialization for NOC error handler.
      It enables interrupts required to handle NOC errors.
 
@return    
      None.

@dependencies
      NOC_Error_Init() must have been called first.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
void NOC_Error_InitDeferred(void);

#endif /* __ICBERROR_H__ */
