
/*!
  @file
  ds_3gpp_iwlan_to_lte_ho.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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
#ifndef DS_3GPP_IWLAN_TO_LTE_HO_H
#define DS_3GPP_IWLAN_TO_LTE_HO_H

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_iwlan_to_lte_ho.h#2 $

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"
#include "dsgen_iface6_hdlr.h"
#include "ds_profile.h"

#define DS_3GPP_IWLAN_TO_LTE_HO_PDN_MAX 1

typedef struct
{
  boolean              is_valid;
  boolean              handover_flag;
  ds_profile_num_type  profile_num;
  boolean              ipv6_addr_valid;
  struct ps_in6_addr   ipv6_addr;
  uint32               valid_lifetime;  /* Time in sec that the prefix is valid */
  uint32               pref_lifetime;   /* Time in sec that the prefix is preferred */
  uint16               ra_life_time;    
  uint8                prefix_length;
}ds_3gpp_iwlan_to_lte_ho_info_type;


/*===========================================================================
FUNCTION DS_3GPP_IWLAN_TO_LTE_HO_CONFIG_V6_PARAMS

DESCRIPTION
  This function configures v6 address from IWLAN side to LTE and fakes RA indication
 
PARAMETERS
  dssnet6_sm_cb_type *instance_ptr - pointer to dssnet6_sm_cb_type
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

=============================================================================*/
void ds_3gpp_iwlan_to_lte_ho_config_v6_params
(
   dssnet6_sm_cb_type *instance_ptr
);

/*===========================================================================
FUNCTION DS_3GPP_IWLAN_TO_LTE_HO_IS_PDN_INFO_VALID

DESCRIPTION
  This function checks if the pdn index is valid
 
PARAMETERS
  uint8 index - index of pdn in iwlan_to_lte_ho table
 
DEPENDENCIES
  None.

RETURN VALUE
  boolean .

SIDE EFFECTS
  None

=============================================================================*/
boolean ds_3gpp_iwlan_to_lte_ho_is_pdn_info_valid
(
  uint8 index
);

/*===========================================================================
FUNCTION DS_3GPP_IWLAN_TO_LTE_HO_FIND_PDN_INDEX

DESCRIPTION
  This function finds the pdn index from the profile number
 
PARAMETERS
  ds_profile_num_type profile_num
 
DEPENDENCIES
  None.

RETURN VALUE
  uint8 index - index of pdn in iwlan_to_lte_ho table

SIDE EFFECTS
  None

=============================================================================*/
uint8 ds_3gpp_iwlan_to_lte_ho_find_pdn_index
(
  ds_profile_num_type profile_num
);

/*===========================================================================
FUNCTION DS_3GPP_IWLAN_TO_LTE_HO_GET_NEW_PDN_INDEX

DESCRIPTION
  This function create new entry in iwlan_to_lte_ho
  table and returns the pdn index from the profile number
 
PARAMETERS
  ds_profile_num_type profile_num
 
DEPENDENCIES
  None.

RETURN VALUE
  uint8 index - index of pdn in iwlan_to_lte_ho table

SIDE EFFECTS
  None

=============================================================================*/
uint8 ds_3gpp_iwlan_to_lte_ho_get_new_pdn_index
(
  ds_profile_num_type profile_num
);

/*===========================================================================
FUNCTION DS_3GPP_IWLAN_TO_LTE_HO_GET_PDN_INFO

DESCRIPTION
  This function returns the pdn info from the profile number
 
PARAMETERS
  ds_profile_num_type profile_num
 
DEPENDENCIES
  None.

RETURN VALUE
  ds_3gpp_iwlan_to_lte_ho_info_type* - pdn info in iwlan_to_lte_ho table

SIDE EFFECTS
  None

=============================================================================*/
ds_3gpp_iwlan_to_lte_ho_info_type* ds_3gpp_iwlan_to_lte_ho_get_pdn_info
(
  ds_profile_num_type profile_num
);

/*===========================================================================
FUNCTION DS_3GPP_IWLAN_TO_LTE_HO_IS_HO_ENABLED_FOR_PDN

DESCRIPTION
  This function checks to see if ho_to_lte is enabled with for pdn
 
PARAMETERS
  ds_profile_num_type profile_num
 
DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE/FALSE

SIDE EFFECTS
  None

=============================================================================*/

boolean ds_3gpp_iwlan_to_lte_ho_is_ho_enabled_for_pdn
(
  ds_profile_num_type profile_num
);

/*===========================================================================
FUNCTION DS_3GPP_IWLAN_TO_LTE_HO_RESET_HO_INFO

DESCRIPTION
  This function resets iwlan_to_lte_ho table
 
PARAMETERS
  ds_profile_num_type profile_num
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void ds_3gpp_iwlan_to_lte_ho_reset_ho_info
(
  ds_profile_num_type profile_num
);

#endif /*DS_3GPP_IWLAN_TO_LTE_HO_H*/
