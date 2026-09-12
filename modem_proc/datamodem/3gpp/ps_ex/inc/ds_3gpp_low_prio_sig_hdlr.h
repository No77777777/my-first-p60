/*!
  @file
  ds_3gpp_low_prio_sig_hdlr.h

  @brief
  This header file defines the externalized function that interface DSEPS 
  and ESM.

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/        
/*==============================================================================

  Copyright (c) 2008 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

==============================================================================*/
/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_low_prio_sig_hdlr.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
10/09/15   vb      Low priority signaling handler

==============================================================================*/

/*===========================================================================
FUNCTION  DS_3GPP_GET_LOW_PRIORITY_SIGNALING_BIT

DESCRIPTION
  The function decides the low priority signaling bit's value.
  ie
  Low priority signaling bit will be set if UE is configured for
  Low priority signaling with the below exceptions
 
  Low priority signaling will not be set for
  1. Emergency calls
  2. If emergency PDN is already UP
  3. Access Class from the SIM is 11-15
  
 
PARAMETERS
  pdn_cntxt_p - PDN context pointer
 
DEPENDENCIES
  None.

RETURN VALUE 
  Boolean
 
SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_get_low_priority_signaling_bit
(
  ds_pdn_context_s       *pdn_cntxt_p
);
