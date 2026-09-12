#ifndef DS3GPPNETWORKOVERRIDEHDLR_H
#define DS3GPPNETWORKOVERRIDEHDLR_H
/*!
  @file
  ds_3gpp_network_override_hdlr.h

  @brief

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/        
/*==============================================================================

  Copyright (c) 2013 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_network_override_hdlr.h#2 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/12/13   vb     Handling of network overriden ESM causecodes

===============================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_3gpp_hdlr.h"
#include "ds_3gppi_utils.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_apn_table.h"
#include "ds3gsubsmgr.h"
#include "ds_3gpp_pdn_throttle_sm.h"


#define DS_3GPP_PDP_UNDEFINED_BMASK        (0x1 << DS_3GPP_PDN_THROT_PDP_UNDEFINED)
#define DS_3GPP_PDP_IPV4_BMASK             (0x1 << DS_3GPP_PDN_THROT_PDP_IPV4)
#define DS_3GPP_PDP_IPV6_BMASK             (0x1 << DS_3GPP_PDN_THROT_PDP_IPV6)

/*===========================================================================
FUNCTION DS_3GPP_NETWORK_OVERRIDE_HDLR_RESET

DESCRIPTION
  This function initializes the data structure to NULL

  Exception:- If rel12_plus check is set to TRUE
  The data structures are to be initialized to NULL only if the network
  override handler was set by PDN CONN REJ received

PARAMETERS 
  subs_id           -- Subscription Id
  rel12_plus_check  -- Release 12 enabled or not
  reset_ip_fallback -- reset is because of IP fallback condition

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_network_override_hdlr_reset
(
  sys_modem_as_id_e_type   subs_id,
  boolean                  rel12_plus_check,
  boolean                  reset_ip_fallback
);

/*===========================================================================
FUNCTION DS_3GPP_NETWORK_OVERRIDE_HDLR_RESET_IF_PLMN_CHANGED

DESCRIPTION
  This function initializes the data structure to NULL

PARAMETERS
   subs_id - Subscription Id

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_network_override_hdlr_reset_if_plmn_changed
(
  sys_modem_as_id_e_type              subs_id,
  sys_plmn_id_s_type                  plmn_id,
   ds_3gpp_throt_rat_e_type           rat_type,
  boolean                             rel12_plus_check
);

/*=========================================================================
FUNCTION DS_3GPP_NETWORK_OVERRIDE_HDLR_SET_OVERRIDDEN_IP

DESCRIPTION
  This function sets the passed IP type based on the apn entry passed.
  If the apn entry does not exist, it would create a new entry

PARAMETERS
   apn                    --  Pointer to APN name 
   ip_type                --  IP type allowed by network
   override cause         --  Network accepted or rejected the connection
   rat                    --  Current rat information
   ip_fallback_enabled    --  Reject is because of IP fallback cause code
   rel12_plus_check       --  Force clear throttling irrespective of the
                              override_cause.
   subs_id                --  Subscription Index

DEPENDENCIES
  None.

RETURN VALUE
  None 

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_network_override_hdlr_set_overridden_ip
(
   byte                                      *apn,
   ds_3gpp_pdn_throt_ntw_o_ridden_ip_type_e   overridden_ip,
   ds_3gpp_pdn_throt_ntw_o_ride_cause         override_cause,
   uint32                                     rat_mask,
   reattempt_ind_T                            reattempt_indicator,
   boolean                                    ip_fallback_enabled,
   boolean                                    rel12_plus_check,
   sys_modem_as_id_e_type                     subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NETWORK_OVERRIDE_HDLR_GET_OVERRIDDEN_IP

DESCRIPTION
  This function returns the overridden IP on the specified rat

PARAMETERS
   byte                              *apn
   rat_type
   sys_modem_as_id_e_type             subs_id

DEPENDENCIES
  None.

RETURN VALUE
  boolean 

SIDE EFFECTS
  None

==============================================================================*/

ds_3gpp_pdn_throt_ntw_o_ridden_ip_type_e 
                                ds_3gpp_network_override_hdlr_get_overridden_ip
(
   byte                              *apn,
   ds_3gpp_throt_rat_e_type           rat_type,
   sys_plmn_id_s_type                *plmn_id_p,
   sys_modem_as_id_e_type             subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_NTW_OVERRIDE_GET_EPLMN_LIST_PER_PLMN

DESCRIPTION
  API used to get the list of throttled ePLMNs on a particular RAT for a
  PLMN ID that is passed to this function. 

PARAMETERS  
  eplmn_list_ptr - Pointer to the ePLMN list 
  rat_type       - Enum that indicates the RAT on which ePLMN list info
                   is needed.
                   
DEPENDENCIES 
  None.
  
RETURN VALUE 
  Returns a pointer to the ePLMN list in a particular RAT
   
SIDE EFFECTS 
  None.
  
===========================================================================*/
sys_plmn_list_s_type* ds_3gpp_ntw_override_get_eplmn_list_per_plmn
(
  sys_plmn_id_s_type         plmn_id,
  ds_3gpp_throt_rat_e_type   rat_type,
  sys_modem_as_id_e_type     subs_id
);

/*===========================================================================
FUNCTION    DS_3GPP_NETWORK_OVERRIDE_THROT_IP_TYPE_TO_PDP_TYPE

DESCRIPTION
  This function maps network override throt IP type to the profile PDP type
 
PARAMETERS 
  overridden_ip_type  -- Network overridden IP type
 
DEPENDENCIES
  None.

RETURN VALUE 
  PDP type
 
SIDE EFFECTS
  None.

===========================================================================*/
ds_umts_pdp_type_enum_type ds_3gpp_network_override_throt_ip_type_to_pdp_type
(
  ds_3gpp_pdn_throt_ntw_o_ridden_ip_type_e   overridden_ip_type
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/
#endif
