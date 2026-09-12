#ifndef DS_EPS_UDT_MGR_H
#define DS_EPS_UDT_MGR_H
/*!
  @file
  ds_eps_udt_mgr.h

  @brief

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
11/14/17   ag     Initial version of unattended data module

===============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "lte_rrc_ext_msg.h"

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/

/*-----------------------------------------------------------------------------
  Macro's for the range of the random timer value 
-----------------------------------------------------------------------------*/

#define DS_EPS_UDT_RESTRICTING_MIN_RANDOM_TIME 0
#define DS_EPS_UDT_RESTRICTING_MAX_RANDOM_TIME 1000 // msecs

/*-----------------------------------------------------------------------------
  Enum to indicate the state of the UDT info manager.
  UNBLOCKED      --- background data allowed
  BLOCKED        --- udt restricting is TRUE, background data not allowed
  TIMER_RUNNING  --- in blocked state, received udt restricting is not included
                    (FALSE)and barring timer. 
                     Before unblock, run the timer first.
                     When the timer expires, will move to UNBLOCKED state.
                     If in between receive udt restricting is TRUE, timer will
                     stop and move to BLOCKED state.
-----------------------------------------------------------------------------*/

typedef enum
{
  DS_EPS_UDT_STATE_UNBLOCKED = 0x0,
  DS_EPS_UDT_STATE_BLOCKED,
  DS_EPS_UDT_STATE_TIMER_RUNNING,
  DS_EPS_UDT_STATE_STATE_MAX
}ds_eps_udt_state_type;


/*----------------------------------------------------------------------- 
  UDT info structure
  state:                            --- state indicates if the UDT data is 
                                        blocked or unblocked
  udt_restricting_common_present    --- indicates if the common UDT restricting
                                        info is present or not
  udt_common:                       --- structure containing udt common info 
                                        from LTE RRC
  udt_restricting_plmn_list_present --- indicates if the per plmn UDT restricting
                                        info is present or not
  udt_plmn_list:                       --- structure containing per plmn udt
                                        info from LTE RRC
  num_plmn                          --- Num of PLMN's in the per plmn info
  barring timer                     --- timer to run based on the timer value
                                        in the udt restricting info
  plmn_is_changed                   --- indicates if the plmn is changed or not
------------------------------------------------------------------------*/

typedef struct
{
  ds_eps_udt_state_type            state;
  boolean                          udt_restricting_common_present;
#ifdef FEATURE_RELEASE_13_UNSUPP_UNATTENDED_DATA
  lte_rrc_udt_restricting_param_s  udt_common; 
#endif /* FEATURE_RELEASE_13_UNSUPP_UNATTENDED_DATA */
  boolean                          udt_restricting_plmn_list_present;
  uint32                           num_plmn;
#ifdef FEATURE_RELEASE_13_UNSUPP_UNATTENDED_DATA
  lte_rrc_udt_res_plmn_param_s     udt_plmn_list[LTE_RRC_MAX_PLMN_LIST_SIZE];
#endif /* FEATURE_RELEASE_13_UNSUPP_UNATTENDED_DATA */
  rex_timer_type                   barring_timer;
  boolean                          plmn_is_changed;
}ds_eps_udt_info_type;

/*===========================================================================

                      EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*=============================================================================
FUNCTION      DS_EPS_UDT_MGR_INIT

DESCRIPTION   This function performs initialization of the module.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
=============================================================================*/
void ds_eps_udt_mgr_init( void );

/*===========================================================================
FUNCTION  DS_EPS_UDT_MGR_RRC_UDT_RESTRICTING_IND_HDLR

DESCRIPTION
  This function is used to handle the Udt restricting message from LTE RRC

PARAMETERS
  msgrtype  - UMID of the message sent
  subs_id   - Subscription ID
  dsmsg_ptr - Pointer to the payload

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - if the message header is correct and we can handle the message
  FALSE - otherwise

  Note: We will not return FALSE for errors in the message payload

SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_eps_udt_mgr_rrc_udt_restricting_ind_hdlr
(
  msgr_umid_type               msgrtype,
  sys_modem_as_id_e_type       subs_id,
  const msgr_hdr_struct_type  *dsmsg_ptr
);

/*===========================================================================
FUNCTION  DS_EPS_UDT_MGR_PLMN_CHG_HDLR

DESCRIPTION
  This function processes plmn change indication

PARAMETERS
  subs_id   - Subscription ID

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ds_eps_udt_mgr_plmn_chg_hdlr
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_EPS_UDT_MGR_BARRING_TIMER_EXP_CB

DESCRIPTION    Function registered when the timer expires

PARAMETERS
  Callback data

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void ds_eps_udt_mgr_barring_timer_exp_cb
(
  unsigned long  callback_data
);

/*===========================================================================
FUNCTION       DS_EPS_UDT_MGR_PROCESS_BARRING_TIMER_EXP

DESCRIPTION    This function process the barring timer expiry cmd

PARAMETERS
  command pointer

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void ds_eps_udt_mgr_process_barring_timer_exp
(
  const ds_cmd_type  *cmd_ptr
);


/*===========================================================================
FUNCTION       DS_EPS_UDT_MGR_RESET_UDT_INFO

DESCRIPTION    Function to reset all the udt info

PARAMETERS
  subs_id   - Subscription ID

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void ds_eps_udt_mgr_reset_udt_info
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_EPS_UDT_MGR_SET_PLMN_CHANGE_IND

DESCRIPTION    Function to set plmn change ind value

PARAMETERS
  subs_id   - Subscription ID
  plmn_is_changed - indicate if plmn is changed

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
===========================================================================*/
void ds_eps_udt_mgr_set_plmn_change_ind
(
  sys_modem_as_id_e_type  subs_id,
  boolean                 plmn_is_changed
);

#endif /* DS_EPS_UDT_MGR_H */
