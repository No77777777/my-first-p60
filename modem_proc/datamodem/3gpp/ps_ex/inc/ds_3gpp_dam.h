#ifndef DS_3GPP_DAM_H
#define DS_3GPP_DAM_H

/*!
  @file
  ds_3gpp_dam.h

  @brief
  Internal utility functions and routines

  @detail
  OPTIONAL detailed description of this C header file.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2014 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_dam.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/17/16   vs      New file added for DAM
===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "rex.h"
#include "ds_3gpp_pdn_throttle_sm.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*---------------------------------------------------------------------------- 
  This structure is used to update the apn from EFS
  ----------------------------------------------------------------------------*/
typedef struct
{
  byte           *apn;
}ds_3gpp_dam_info_s;

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_APN_REJ_SM_INIT

DESCRIPTION
  This function initialize the apn reject sm

PARAMETERS

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_dam_init
(
  void
);

/*===========================================================================
FUNCTION  DS_3GPP_DAM_T3402_TIMER_EXPIRY_CMD_HDLR

DESCRIPTION
  This function resets the sm on timer expiry

PARAMETERS
   void            
 
DEPENDENCIES
  None.

RETURN VALUE 
  void 

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_dam_t3402_timer_expiry_cmd_hdlr
(
  const ds_cmd_type *cmd_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_DAM_VALIDATE_APN_NAME

DESCRIPTION
  This function checks the EFS item to see if enable APN reject has been set.
  It it has not been set the apn_name will be NULL

PARAMETERS
                     
 
DEPENDENCIES
  None.

RETURN VALUE
   True - If the apn has been set in EFS
   False - Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_dam_validate_apn_name
(
  sys_modem_as_id_e_type  subs_id
);


/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_APN_REJ_SM_INIT

DESCRIPTION
  This function initialize the apn reject sm

PARAMETERS   :

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_dam_init
(
  void
);
/*===========================================================================
  FUNCTION ds_3gpp_dam_read_efs_per_subs_id

DESCRIPTION
  This function initializes the apn reject throttling information per
  subscription by reading from apn_reject efs

PARAMETERS
  subs_id -subscription_id 
 
DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_dam_read_efs_per_subs_id
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_CC_SP_INFO_TBL_PER_SUBS_INIT

DESCRIPTION
  This function initializes throttle info tbl to get timer value per sub

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_dam_info_tbl_per_subs_init
(
   sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
  FUNCTION ds_3gpp_dam_refresh_per_subs_id

DESCRIPTION
  This function resets the apn reject throttling information per
  subscription.

PARAMETERS
  subs_id -subscription_id 
 
DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_dam_refresh_per_subs_id
(
  sys_modem_as_id_e_type  subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_DAM_CLEAR_THROTTLING

DESCRIPTION
  Stop T3402 timer if it is already running and reset 
  the throttling flag to false.

PARAMETERS
  sys_modem_as_id_e_type            subs_id
 
DEPENDENCIES
  None.

RETURN VALUE 
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_dam_clear_throttling
(
  sys_modem_as_id_e_type          subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_DAM_PDN_BLOCKED_STATUS

DESCRIPTION
  This function gets the PDN blocked flag from Reject SM

PARAMETERS
  void           
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If the flag is set
  FALSE - Otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_dam_get_pdn_blocked_status
(      
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_DAM_IS_APN_DAM_APN

DESCRIPTION
  This function compares the provided apn with the dam apn to see if they
  match.  It will return false if the DAM APN is not configured.

PARAMETERS 
  apn_name
  subs_id
 
DEPENDENCIES
  None.

RETURN VALUE 
  None 

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds_3gpp_dam_is_apn_dam_apn
(
  char                     *apn_name,
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_DAM_POWER_UP_INIT      

DESCRIPTION
  This function performs power up initialization of the DAM Throttling module

PARAMETERS
  None.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_dam_power_up_init
(
  void 
);

#endif /* DS_3GPP_DAM_H */
