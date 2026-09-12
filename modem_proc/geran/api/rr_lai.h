#ifndef RR_LAI_H
#define RR_LAI_H
/*============================================================================
 $Header: //components/rel/geran.mpss/7.4.0/api/rr_lai.h#1 $$DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $

 @file rr_lai.h

 Header file containing the definition of the LAI_T type

 Copyright (c) 2008-2015 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Qualcomm Confidential and Proprietary
============================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/
#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "sys_plmn.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

// represents an LAI
typedef struct
{
  PLMN_id_T plmn_id;
  word      location_area_code;
} LAI_T;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 *--------------------------------------------------------------------------*/


#endif /* #ifndef RR_LAI_H */

/* EOF */

