/*!
  @file
  ds_3gpp_hdlr_ex.h

  @brief
  REQUIRED brief Interface with DSMGR for 3GPP technologies data services.

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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_hdlr_ex.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/04/18   rs      Initial version
===========================================================================*/

#ifndef DS3GPPHDLR_EX_H
#define DS3GPPHDLR_EX_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"

#ifdef FEATURE_DATA_3GPP

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/


/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================
FUNCTION                    DS3GPP_EX_INFO_INIT
 
DESCRIPTION
  This function initializes all global information in ds_3gpp_hdlr_ex.c.
 
PARAMETERS 
  None

DEPENDENCIES 
  None

RETURN VALUE 
  void
 
SIDE EFFECTS 
===========================================================================*/
void  ds3gpp_ex_info_init
(
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================

FUNCTION          DS3GPP_EX_PROCESSPSCMD

DESCRIPTION
  This function processes the PS commands received by the DS task. 

PARAMETERS
  cmd_ptr   - Ptr to the command information

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds3gpp_ex_processPSCmd
(
  const ds_cmd_type                   *cmd_ptr 
);

#ifdef FEATURE_LAPP
/*===========================================================================
FUNCTION  DS3GPP_HDLR_EX_OVERRIDE_SYS_MODE

DESCRIPTION
  This function overrides sys mode to LTE if the new mode is NR5G to make
  sure LTE specific handlers are called

PARAMETERS 
  mode     -- Current sys mode
  subs_id  -- Subscription ID
  
DEPENDENCIES
  None.

RETURN VALUE
  Sys mode.

SIDE EFFECTS
  None.

===========================================================================*/
sys_sys_mode_e_type ds3gpp_hdlr_ex_override_sys_mode
(
  sys_sys_mode_e_type                           mode,
  sys_modem_as_id_e_type			subs_id
);
#endif /* FEATURE_LAPP */

/*===========================================================================
FUNCTION  DS3GPP_DISCONNECT_UNUSED_PDN_GWT_ALLOWED

DESCRIPTION
  This function decide if we need to teardown unused pdn after irat from lte to GWT. 

PARAMETERS
  subs_id                       subscription id
  sys_sys_mode_e_type           new_mode
  sys_sys_mode_e_type           old_mode

DEPENDENCIES
  None.

RETURN VALUE
  boolean
  return:need to tear down unused pdn
  false:vice verse

SIDE EFFECTS
  None.

===========================================================================*/
boolean ds3gpp_disconnect_unused_pdn_gwt_allowed
(
  sys_modem_as_id_e_type        subs_id,
  sys_sys_mode_e_type           new_mode,
  sys_sys_mode_e_type           old_mode
);

#endif /* FEATURE_DATA_3GPP */
#endif /* DS3GPPHDLR_EX_H */
