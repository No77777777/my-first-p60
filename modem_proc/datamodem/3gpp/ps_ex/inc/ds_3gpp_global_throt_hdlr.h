#ifndef DS_3GPP_GLOBAL_THROT_H
#define DS_3GPP_GLOBAL_THROT_H

/*!
  @file
  ds_3gpp_global_throt_hdlr.h

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_global_throt_hdlr.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/05/11   ss      Changes to support RPM.
08/16/10   hs      Initial version

===========================================================================*/


/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "rex.h"
#include "ds_3gpp_pdn_throttle_sm.h"
#include "ds3gdevmgr.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*===========================================================================
FUNCTION  DS_3GPP_GLOBAL_THROT_PER_SUBS_INIT      

DESCRIPTION
  This function performs subscription specific initialization of the Global
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
void ds_3gpp_global_throt_per_subs_init
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_GLOBAL_THROT_IS_ENABLED     

DESCRIPTION
  This function checks whether Global Throttling is enabled.

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
boolean ds_3gpp_global_throt_is_enabled
(
  sys_modem_as_id_e_type subs_id
);

/*============================================================================
FUNCTION   DS_3GPP_GLOBAL_THROT_CLEAR_THROTTLING

DESCRIPTION
  This function takes care of clearing the PDN and SRV throttling as  part of
  data ON event form UI. It also takes care of informing
  external clients with the ublocked APNs
  
PARAMETERS  
  subs_id : Subs_id
  apn_name : The apn for which the throttling is to be cleared
    
DEPENDENCIES 
  None.
  
RETURN VALUE 
  None.
   
SIDE EFFECTS 
  None
  
===========================================================================*/
void ds_3gpp_global_throt_clear_throttling
(
  byte*                                  apn_name,
  sys_modem_as_id_e_type                 subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_GLOBAL_THROT_POWER_UP_INIT      

DESCRIPTION
  This function performs power up initialization of the Global
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
void ds_3gpp_global_throt_power_up_init
(
  void 
);

/*===========================================================================
FUNCTION      DS_3GPP_GLOBAL_THROT_INFINITE_BLOCK_REQUIRED

DESCRIPTION 
  This function checks whether infinite blocking is required or not based on
  the down reason passed to it

PARAMETERS 
  down_reason - PS Net Down Reason
  subs_id     - Subscription Id

RETURN VALUE 
  TRUE,  if inifinte blocking is required
  FALSE, otherwise

SIDE EFFECTS 
  None
===========================================================================*/
boolean ds_3gpp_global_throt_infinite_block_required
( 
  ps_iface_net_down_reason_type   down_reason
);

#endif /* DS_3GPP_GLOBAL_THROT_H */
