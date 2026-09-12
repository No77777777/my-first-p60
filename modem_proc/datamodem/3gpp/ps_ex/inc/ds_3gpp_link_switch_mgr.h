#ifndef DS_3GPP_LINK_SWITCH_MGR_H
#define DS_3GPP_LINK_SWITCH_MGR_H
/*===========================================================================

            LINK SWITCH MODULE

===========================================================================*/
/*!
  @file
  ds_3gpp_link_switch_mgr.h

  @brief
  This header file defines the externalized function that interfaces with
  RRC, PDCP and MPPM.

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/        
/*==============================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved

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


when       who     what, where, why
--------   ---     -------------------------------------------------------------
01/09/18   rs      Initial version

==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "dsumtspdpreg.h"
#include "ds_3gpp_bearer_context.h"
#ifdef FEATURE_LAPP_UFS 
#error code not present
#endif /* FEATURE_LAPP_UFS */

#ifdef FEATURE_LAPP_UFS 
#error code not present
#endif /* FEATURE_LAPP_UFS  */

#ifdef FEATURE_LAPP
/*===========================================================================
FUNCTION      DS_3GPP_LINK_SWITCH_SET_NR5G_PREF_SYS

DESCRIPTION
  This function is used to update NR5G pref system status. 
   
PARAMETERS 
  nr5g_status    -- whether NR5G is the preferred system or not
  subs_id        -- Subscription ID
 
DEPENDENCIES
  None.

RETURN VALUE 
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_link_switch_set_nr5g_pref_sys
(
  boolean                                   nr5g_status,
  sys_modem_as_id_e_type                    subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_LINK_SWITCH_REG_PDCP_UL_BEARER_CNF

DESCRIPTION
  This function is called to indicate uplink wm registration is complete
   
PARAMETERS
  eps_id    -- EPS ID of the bearer 
  subs_id   -- Subscription ID
 
DEPENDENCIES
  None.

RETURN VALUE 
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void ds_3gpp_link_switch_reg_pdcp_ul_bearer_cnf
(
  eps_bearer_id_T                           eps_id,
  sys_modem_as_id_e_type                    subs_id
);

#endif /* FEATURE_LAPP */
#endif /* DS_3GPP_LINK_SWITCH_MGR_H */
