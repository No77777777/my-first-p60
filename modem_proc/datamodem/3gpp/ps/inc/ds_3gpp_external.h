/*!
  @file
  ds_3gpp_external.h

  @brief
  Redial interface

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2008-2017 Qualcomm Technologies Incorporated. All Rights Reserved

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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/inc/ds_3gpp_external.h#2 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
06/21/11   rr      Support for pdn redial functionality
==============================================================================*/

#ifndef DS3GPPEXTERNAL_H
#define DS3GPPEXTERNAL_H

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_bearer_context.h"
#include "ds_eps_tlb.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*===========================================================================
FUNCTION ds_3gpp_external_pdn_cntxt_no_redial_hdlr

DESCRIPTION
  Customer specific code to handle NO_REDIAL case scenarios

PARAMETERS   
  pdn_cntx_p         - pdn context that we are workin on
  bearer_cntx_p      - bearer context that was originally deactivted and
                       needs to be checked for redial
  down_reason        - reason for bearer deactivation
  call_was_connected - whether the call was connected or not
  redial_not_performed - whether redial needs to be performed or not
    
DEPENDENCIES
  None

RETURN VALUE
  TRUE : if redial doesnt need be performed
  FALSE : otherwise
  
 
SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_external_pdn_cntxt_no_redial_hdlr
(
  ds_pdn_context_s     *pdn_cntx_p, 
  ds_bearer_context_s  *bearer_cntx_p,
  ds_3gpp_down_reason_t down_reason,
  boolean               call_was_connected,
  boolean               redial_not_performed
);

/*===========================================================================
FUNCTION ds_3gpp_external_pdn_cntxt_eps_tlb_msg_hdlr

DESCRIPTION
  This function is a customer visible fucntion that handles messages coming
  from Test loopback module
 
PARAMETERS
 tlb_state : This is the current state of test loopback mode
 tld mode : TLB mode: Mode A or B or C
 subs_id : subscription id

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/

void ds_3gpp_external_pdn_cntxt_eps_tlb_msg_hdlr
(
  ds_eps_bearer_tlb_state_e  tlb_state,
  lte_tlb_mode_e             mode,
  sys_modem_as_id_e_type     subs_id 
);

/*===========================================================================
FUNCTION ds_3gpp_external_get_flow_filters_from_bearer_id

DESCRIPTION
  This function gets the flow filter information based on bearer id

PARAMETERS
  bearer_id - NSAPI or eps bearer id
  app_qos_p - pointer to where the info will be filled. memory must have been already
              allocated for this
  subs_id   - Subscriber id for which the flow filter is requested
    
DEPENDENCIES
  None.

RETURN VALUE
  TRUE  if the filters are succesfully retrieved
  FALSE if failure occurs

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_external_get_flow_filters_from_bearer_id
( 
  uint8                               bearer_id,
  qos_spec_type                      *app_qos_p,
  sys_modem_as_id_e_type              subs_id
);

/*===========================================================================
FUNCTION ds_3gpp_external_get_pdn_limit_throttling_time
 
DESCRIPTION
  This function returns the value of pdn_req_throttling_time

PARAMETERS 
  subs_id: subscription id
 
DEPENDENCIES 
  None. 
  
RETURN VALUE 
  RetuDrns the pdn requested throttling timer value
 
SIDE EFFECTS 
  None 
  
===========================================================================*/
uint16 ds_3gpp_external_get_pdn_limit_throttling_time
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_EXTERNAL_PDN_LIMIT_IS_ENABLED

  DESCRIPTION
  This is a wrapper function that is customer visible and checks whether
  PDN Limit handling is enabled.
  
  PARAMETERS  
  subs_id: subscription id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  TRUE if enabled
  FALSE otherwise
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_external_pdn_limit_is_enabled
(
  sys_modem_as_id_e_type          subs_id
);

#endif /* DS3GPPEXTERNAL_H */
