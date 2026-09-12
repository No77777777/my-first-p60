#ifndef CM_EMGEXT_H
#define CM_EMGEXT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            C A L L   M A N A G E R   C A L L   M A N A G E R  LTE Emergency call

GENERAL DESCRIPTION
  This module contains LTE emergency call access for remote IMS.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  cmdbm_init() must be called to initialize this module before any other
  function declared in this module is being called.

Copyright (c) 2013 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmemgext.h#2 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
04/29/13    xs      Initial version

===========================================================================*/
#include "comdef.h"
#include "cmll_v.h"

#define EXT_E911_CALL_ID 0xFE

typedef struct
{
  boolean is_ready;
  cm_ac_reason_e_type reason;
  boolean is_redial;
  cm_mode_pref_e_type emerg_mode_pref;
  cm_call_csfb_e_type ext_call_csfb_type;
  boolean ext_call_is_ppcsfb;
}cmemg_sub_info_s_type;

typedef struct
{
  cmemg_sub_info_s_type *sub_info[MAX_SIMS];
} cmemg_s_type;

/*===========================================================================

FUNCTION cmemg_get_act_id

DESCRIPTION
  Return act id for access control

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
uint32 cmemg_get_act_id (sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION cmemg_init

DESCRIPTION
  Access control initialize

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void cmemg_init( void );

/*===========================================================================

FUNCTION cmemg_ptr

DESCRIPTION
  CM emergency global ptr

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cmemg_s_type *cmemg_ptr(void);

/*===========================================================================

FUNCTION cmemg_msgr_cmd_proc

DESCRIPTION
  Process reports from the lower layers.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmemg_msgr_cmd_proc(

  const void   *cmd_ptr,
  /* Pointer to the report sent by Message Router */

  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmemg_sd_rpt_proc

DESCRIPTION
  Process System Determination reports


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmemg_sd_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to SD reports */
);
/*===========================================================================

FUNCTION cmemg_exit_emgergency_state

DESCRIPTION
  Request access control to exit emergency state.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmemg_exit_emgergency_state (
  boolean                  is_ecbm_required,
  void                    *user_data,
  sys_modem_as_id_e_type   asubs_id
);

/*===========================================================================

FUNCTION cmemg_send_call_mode_ind

DESCRIPTION
 Sends Volte only mode indication to clients via msgr


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmemg_send_call_mode_ind(sys_modem_as_id_e_type subs_id, sys_call_mode_t call_mode, boolean force_send);

/*===========================================================================

FUNCTION cmemg_handle_esr_rsp

DESCRIPTION
 Handles the ESR response


DEPENDENCIES
  none

RETURN VALUE
  none
===========================================================================*/
void cmemg_handle_esr_rsp(uint8 call_id);

/*===========================================================================

FUNCTION cmemg_is_ext_e911_mode

DESCRIPTION
  Return if UE in third party E911 mode.

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
boolean  cmemg_is_ext_e911_mode(void);

/*===========================================================================

FUNCTION cmemg_set_reason_per_sub

DESCRIPTION
  Set the reason for access control for the specified sub.

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmemg_set_reason_per_sub(
  cm_ac_reason_e_type     res,
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmemg_is_redial

DESCRIPTION
  Returns if e911 enter is for redial with LTE
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean cmemg_is_redial(
  sys_modem_as_id_e_type  asubs_id
);

/*===========================================================================

FUNCTION cmemg_reset_emerg_state

DESCRIPTION
  Reset emergency state related variables.

RETURN VALUE
  none

===========================================================================*/
void cmemg_reset_emerg_state(
  sys_modem_as_id_e_type asubs_id
);

#ifdef FEATURE_CM_LTE
/*===========================================================================

FUNCTION cmemg_determine_ss_for_origination

DESCRIPTION

  Based on mode and hybrid preference, determine ss for call object /
  cmpref_proc_cmd_pref_sys_chgd() function.

  The assumption is that for call originations sys pref change will be sent
  to either 1X or HDR (depending on where the origination will go) but not both.
  Sys pref change goes to both only when hybr_pref is toggled from on
  to off or vice versa.

DEPENDENCIES
  None

RETURN VALUE
  ss

SIDE EFFECTS
  None

===========================================================================*/
multimode_stack_e_type cmemg_determine_ss_for_origination(

  cm_hybr_pref_e_type    hybr_pref,
  sys_modem_as_id_e_type as_id
);
#endif


/*===========================================================================

FUNCTION cmemg_sub_info_ptr

DESCRIPTION
  CM emergency global ptr

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cmemg_sub_info_s_type *cmemg_sub_info_ptr(
  sys_modem_as_id_e_type as_id
);

/*===========================================================================

FUNCTION cmemg_is_csfb_type

DESCRIPTION
  Returnscsfb type for e911 enter mode
  
DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
extern cm_call_csfb_e_type cmemg_is_csfb_type(
  sys_modem_as_id_e_type asubs_id
);

/*===========================================================================

FUNCTION cmemg_is_sub_ext_e911_mode

DESCRIPTION
  Return if UE in third party E911 mode for the specified sub.

RETURN VALUE
  boolean

===========================================================================*/
boolean  cmemg_is_sub_ext_e911_mode(
  sys_modem_as_id_e_type  asubs_id
);


#endif
