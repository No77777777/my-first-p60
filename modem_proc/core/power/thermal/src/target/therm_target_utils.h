#ifndef THERM_TARGET_UTILS_H
#define THERM_TARGET_UTILS_H

/*============================================================================
  FILE:         therm_target_utils.h
  
  OVERVIEW:     Thermal NPA mitigation resources header file.
 
                Copyright (c) 2019 QUALCOMM Technologies, Incorporated.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/*=======================================================================
$Header: //components/rel/core.mpss/10.0/power/thermal/src/target/therm_target_utils.h#1 $
$DateTime: 2021/06/23 12:16:38 $
$Author: pwbldsvc $
========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

/*
===============================================================================

                             TYPE DEFINITION(S)

===============================================================================
*/

/*
===============================================================================

                           GLOBAL FUNCTION DECLARATIONS

===============================================================================
*/

/**
  @brief therm_target_utils_init

  Thermal target utils init function.
*/
void therm_target_utils_init( void );


#ifdef __cplusplus
}
#endif

#endif /* THERM_TARGET_UTILS_H */
