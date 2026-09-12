/*==============================================================================

FILE:      NOC_error_dal.c

DESCRIPTION: This file implements DAL platform specific NOC Error driver.

PUBLIC CLASSES:  Not Applicable

INITIALIZATION AND SEQUENCING REQUIREMENTS:  N/A

Edit History

//#CHANGE - Update when put in the depot
$Header: //components/rel/core.mpss/10.0/systemdrivers/icb/src/wcss/NOC_error_dal.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $
$Change: 18983508 $

When        Who    What, where, why
----------  ---    ----------------------------------------------------------- 
2016/02/19  pm     New file for Adrastea (Modem User PD)
2014/12/09  tb     Split OEM modifiable data from internal data
2014/09/03  tb     Created

        Copyright (c) 2014 - 2016 QUALCOMM Technologies, Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
==============================================================================*/

#include "NOC_error.h"
#include "NOC_error_platform.h"
#include "DALSys.h"
#include "DALStdErr.h"
#include "DALDeviceId.h"
#include "err.h"
#include "ABT_internal.h"

/*============================================================================
                          DEFINEs/MACROs
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
const NOCERR_propdata_type* NOC_Error_Platform_Get_Propdata(void)
{
   DALSYSPropertyVar NOCErrorPropVar;
   DALSYS_PROPERTY_HANDLE_DECLARE(hNOCErrorProp);

   /* Get default configuration from config data */
   if (DAL_SUCCESS != DALSYS_GetDALPropertyHandleStr( "/dev/NOCError",
                                                      hNOCErrorProp ))
   {
     // Can't get property handle
     ABT_ERR("NOCError: Can't get DAL property handle.", 0, 0, 0);
     return NULL;
   }

   if ((DAL_SUCCESS == DALSYS_GetPropertyValue(hNOCErrorProp,
                                              "NOCError_Propdata", 0, &NOCErrorPropVar)))
   {
       if((0 >= NOCErrorPropVar.dwLen))
       {
           ABT_ERR("NOCError: Invalid property data length", 0, 0, 0);
           return NULL;
       }
       return (const NOCERR_propdata_type*) NOCErrorPropVar.Val.pStruct;
   }

   ABT_ERR("NOCError: Can't get DAL property value", 0, 0, 0);
   return NULL;
}

