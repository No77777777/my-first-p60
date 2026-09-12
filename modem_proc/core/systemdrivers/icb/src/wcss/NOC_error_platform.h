#ifndef __NOC_ERROR_PLATFORM_H__
#define __NOC_ERROR_PLATFORM_H__
/*============================================================================

FILE:      NOC_error_platform.h

DESCRIPTION: Platform specific function and data structure declarations 
             for NOC Error driver

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A
 
Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/systemdrivers/icb/src/wcss/NOC_error_platform.h#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $ 

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/02/19  pm     Branch for Adrastea (Modem User PD)
2014/12/09  tb     Split OEM modifiable data from internal data
2014/09/03  tb     Created

                Copyright (c) 2014 - 2016 QUALCOMM Technologies, Incorporated.
                             All Rights Reserved.
                          QUALCOMM Proprietary/GTDR
============================================================================*/

#include "NOC_error.h"

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
      Gets internal platfrom specific property data structure for configuration.
 
@param[in]  None.

@return    
      NOCERR_propdata_type* Pointer to internal property data structure.

@dependencies
      None.
 
@sideeffects 
      None. 
*/ 
/*============================================================================*/
const NOCERR_propdata_type* NOC_Error_Platform_Get_Propdata(void);

#endif /* __NOC_ERROR_PLATFORM_H__ */

