#ifndef __NOC_ERROR_H__
#define __NOC_ERROR_H__
/*============================================================================

FILE:      NOC_Error.h

DESCRIPTION: APIs and data structure declarations 
             for NOC Error Handler

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/systemdrivers/icb/src/wcss/NOC_error.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/01/21  pm     Branch for Adrastea (Modem User PD)
2015/11/23  pm     Added Sideband Manager support
2014/12/09  tb     Split OEM modifiable data from internal data
2013/05/10  pm     Added NOC_ERRLOG6 macro to support FlexNoc 2.8
2012/10/03  av     Created

                Copyright (c) 2014 - 2016 QUALCOMM Technologies, Incorporated.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
============================================================================*/
#include "stdint.h"
#include "NOC_error_cfg.h"

/*============================================================================
                          DEFINEs/MACROs
============================================================================*/


/*============================================================================
                          TYPE DEFINITION
============================================================================*/

/**
 * NOC Error Syndrome Register Data Type
 */
typedef struct
{
    char* name;                     /**< NOC name */
    uint32_t ERRLOG0;
    uint32_t ERRLOG1;
    uint32_t ERRLOG2;
    uint32_t ERRLOG3;
    uint32_t ERRLOG4;
    uint32_t ERRLOG5;
    uint32_t ERRLOG6;
    uint32_t FLAGINSTATUS0;
    uint32_t SENSEIN0;
}NOCERR_syndrome_type;

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
      Performs De-initialization for NOC Error Handler.

@param[in]  None.

@return
      None.

@dependencies
      None.

@sideeffects
      None.
*/
/*============================================================================*/
void NOC_Error_DeInit(void);

/*===========================================================================*/
/**
@brief 
      Handler function for NOC error interrupt.
 
@param[in]  void

@return    
      void

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*==============================================================================*/
void NOC_Error_Handle_Interrupt(void);

#endif /* __NOC_ERROR_H__ */
