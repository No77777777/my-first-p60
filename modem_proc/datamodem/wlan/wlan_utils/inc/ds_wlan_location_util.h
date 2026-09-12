#ifndef DS_WLAN_LOCATION_UTIL_H
#define DS_WLAN_LOCATION_UTIL_H
/*===========================================================================
                           DS_WLAN_LOCATION_UTIL
===========================================================================*/

/*!
  @file
  ds_wlan_location_util.h

  @brief
  This module implements wlan location util functions

  @detail
*/

/*===========================================================================

  Copyright (c) 2018 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/wlan/wlan_utils/inc/ds_wlan_location_util.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/17/18   op      Created module
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_WLAN_MAPCON
#include "ds3gmmgsdiif.h"
#include "ds3geventmgr.h"
#include "dstask_v.h"
#include "ps_sys.h"

/*===========================================================================
                             PUBLIC DATA DECLARATIONS
===========================================================================*/
/*===========================================================================
                          VARIABLES AND MACRO DEFINITIONS
==========================================================================*/
typedef struct ds_wlan_util_mcc_to_isocc_map_type_s
{
  int16     mcc;
  char      iso_cc[PS_SYS_MAX_COUNTRY_CODE_LEN];
}ds_wlan_util_mcc_to_isocc_map_type;

/*===========================================================================
                       EXTERNAL FUNCTION DEFINITIONS
===========================================================================*/
/*===========================================================================
FUNCTION       ds_wlan_location_util_get_isocc_from_mcc

DESCRIPTION    Returns the 2 character ISO Country Code for a given MCC

PARAMETERS     in_mcc - MCC to convert to ISO CC

DEPENDENCIES   NONE

RETURN VALUE   char* - ISO Country Code 
               NULL - mapping not found 

SIDE EFFECTS   NONE
===========================================================================*/
char*  ds_wlan_location_util_get_isocc_from_mcc
(
  uint16     in_mcc
);

/*===========================================================================
FUNCTION       ds_wlan_location_util_get_mcc_from_isocc

DESCRIPTION    Returns the MCC for a given ISO Country Code

PARAMETERS     in_iso_cc - ISO CC to convert to MCC

DEPENDENCIES   NONE

RETURN VALUE   int16 - MCC value in integer form 
               -1 - mapping not found 

SIDE EFFECTS   NONE
===========================================================================*/
int16   ds_wlan_location_util_get_mcc_from_isocc
(
  char*      in_iso_cc
);

#endif /* FEATURE_DATA_WLAN_MAPCON */
#endif /* DS_WLAN_LOCATION_UTIL_H */
