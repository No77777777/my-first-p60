#ifndef DS_3GPP_GENERIC_THROT_HDLR_H
#define DS_3GPP_GENERIC_THROT_HDLR_H

/*!
  @file
  ds_3gpp_generic_throt_hdlr.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/13/18   psn      Initial version

===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "rex.h"
#include "ds_3gpp_pdn_throttle_sm.h"
#include "ds_3gpp_pdn_throt_rules.h"
#include "ds_3gpp_pdn_throt_config.h"
#include "ds3gdevmgr.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*-------------------------------------------------------------------- 
  Max default failure timers is 7 currently
---------------------------------------------------------------------*/
#define DS_3GPP_GENERIC_THROT_MAX_DEFAULT_FAILURE_TIMERS 7  

/*-------------------------------------------------------------------- 
  Max default reset conditions is 2 currently
---------------------------------------------------------------------*/
#define DS_3GPP_GENERIC_THROT_MAX_DEFAULT_RESET_CONDITIONS 2

/*--------------------------------------------------------------------
  Struct used to store the info from the node where a particular 
  apn is found. Can be expanded to add more fields
---------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_throt_rat_e_type      rat_type;
} ds_3gpp_generic_throt_params_s;

/*===========================================================================
FUNCTION  DS_3GPP_GENERIC_THROT_PER_SUBS_INIT      

DESCRIPTION
  This function performs subscription specific initialization of the Generic
  Throttling module

PARAMETERS
  subs_id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_generic_throt_per_subs_init
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_GENERIC_THROT_IS_ENABLED     

DESCRIPTION
  This function checks whether Generic Throttling is enabled.

PARAMETERS
  Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if enabled
  FALSE, otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_generic_throt_is_enabled
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_GENERIC_THROT_POWER_UP_INIT      

DESCRIPTION
  This function performs power up initialization of the Generic
  Throttling module

PARAMETERS
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_generic_throt_power_up_init
(
  void 
);
/*===========================================================================
FUNCTION DS_3GPP_GENERIC_THROT_GET_APN_CONFIG_PARAMS

DESCRIPTION
  This function fills the APN params from the XML and returns the pointer
  to the data block where the match occured . If the CC is not configured, then
  a pointer to the default data block is returned
 
PARAMETERS 
  ds_3gpp_generic_throt_params_s  : Params to be filled in
  throt_rule_info_p : pointer to the throttling rule
  subs_id: subscription id

DEPENDENCIES 
  None.
  
RETURN VALUE 
  ds_3gpp_config_mgr_pdn_throt_type* - pointer to the data block
 
SIDE EFFECTS 
  None.
  
===========================================================================*/
ds_3gpp_config_mgr_pdn_throt_type*  ds_3gpp_generic_throt_get_apn_config_params
(
  ds_3gpp_generic_throt_params_s       *throt_params_p,
  ds_pdn_throt_rule_info               *throt_rule_info_p,
  sys_modem_as_id_e_type               subs_id
);

#endif /* DS_3GPP_GENERIC_THROT_HDLR_H */
