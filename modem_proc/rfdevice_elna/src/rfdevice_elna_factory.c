
/*=============================================================================

Copyright (c) 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S.
  Government. Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

  $Header: //components/rel/rfdevice_elna.mpss/1.22/src/rfdevice_elna_factory.c#1 $
  $DateTime: 2021/03/09 03:41:28 $
  $Author: pwbldsvc $

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/

#include "rfc_common.h"
#include "rfdevice_elna_typedef.h"
#include "rfdevice_elna_test_settings.h"
#include "rfdevice_elna_customer_settings.h"

boolean rfdevice_elna_choose_settings (
                                       uint16 mfg_id, 
                                       uint8  prd_id, 
                                       uint8  prd_rev, 
                                       rfdevice_elna_settings_type *elna_settings
                                       )
{
  boolean status= FALSE;
  
 /* NULL Pointer Check*/
  if(elna_settings == NULL)
  {
    return FALSE;
  }

/*!Uncomment the below code to enable customer elna settings  with the correct mfg_id, prd_id and prd_rev matching the RFC*/
//  
//  if ( mfg_id ==  0xFF &&  prd_id == 0xFF  && prd_rev == 0xFF)
//  {
//    return(get_elna_customer_settings(elna_settings));
//  }
//  else 
//  {

      /*By default choose the intenral test elna device settings*/
      return(get_elna_internal_test_settings(elna_settings));
//  }

//  return status;
}



