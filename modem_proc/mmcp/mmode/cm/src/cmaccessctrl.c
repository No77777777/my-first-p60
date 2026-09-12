/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             C A L L   M A N A G E R   A C C E S S   C O N T R O L

GENERAL DESCRIPTION
  This module contains declarations used by the call manager in processing
  Access Control command. The module uses the callback function for the caller
  and also MSGR is used for broadcasting the information between CM and all
  other modules who are interested in the information of the access controller

INITIALIZATION AND SEQUENCING REQUIREMENTS
  cmac_init() must be called to initialize this module before any other
  function declared in this module is being called.

Copyright (c) 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmaccessctrl.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/05/13   xs      Added debug code
08/08/13   xs      Remove emg handling from CMAC
07/19/13   jvo     Remove all MDM/MSM Fusion features
06/18/13   gm      FR 2617 - OMADM IMS and ADMIN PDN Disable Requirement
02/15/13   th      Initial design

===========================================================================*/

#include "mmcp_variation.h"
#include "customer.h"  /* Customer configuration file */

#include <stdlib.h>
#include <string.h>

#include "comdef.h"    /* Definition for basic types and macros */

#include "cm_msgr_msg.h"

#include "cmph.h"      /* Interface to CM phone object */
#include "cmclnup.h"
#include "cmdbg.h"
#include "cmtaski.h"
#include "cmclient.h"
#include "cmaccessctrl.h"
#include "cmcall.h"
#include "cmregprxi.h"
#ifdef CM_GW_SUPPORTED
#include "cmwcall.h"
#endif
#include "cai.h"
#include "cmpmprx.h"
#include "sys.h"       /* System wide definitions */
#include "cmmsc.h"

#define CMAC_1X_DEREG_TIMER 2
/* 2 sec timer for retrying de-register */

#define CMAC_UPTIME_NONE  (dword) (-1)

cmac_s_type *cmac_local[MAX_SIMS]  = {NULL};

/*===========================================================================

FUNCTION cmac_is_me_blocked

DESCRIPTION
  Returns TRUE if CM AC is in emergency due to reject cause #6 from NAS

===========================================================================*/
boolean cmac_is_me_blocked(sys_modem_as_id_e_type asubs_id);

/*===========================================================================

FUNCTION cmac_ptr

DESCRIPTION
  Return  access control ptr for the requested sub

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
cmac_s_type *cmac_ptr(sys_modem_as_id_e_type asubs_id)
{
  if (cmutil_is_as_id_valid(asubs_id))
  {
    return cmac_local[asubs_id];
  }
  return NULL; /* Compilation warning. */
}

/*===========================================================================

FUNCTION cmac_ptr

DESCRIPTION
  Return  address of access control ptr for the requested sub

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif

cmac_s_type **cmac_ptr_addr()
{
    return cmac_local;
}

/*===========================================================================

FUNCTION cmac_get_act_id

DESCRIPTION
  Return act id for access control

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static cm_act_id_type cmac_get_act_id (sys_modem_as_id_e_type asubs_id)
{
  if (cmutil_is_as_id_valid(asubs_id))
  {
    return (cm_act_id_type) cmac_local[asubs_id];
  }
  sys_err_fatal_null_ptr_exception();
  return CM_ACT_ID_NONE; /* Compilation warning. */
} /* cmac_get_act_id() */

/*===========================================================================

FUNCTION cmac_is_in_thermal_emergency_level2

DESCRIPTION
  Returns TRUE if CM AC is in level2 thermal emergency due to UNSAFE temperature

===========================================================================*/
boolean cmac_is_in_thermal_emergency_level2(sys_modem_as_id_e_type asubs_id)
{
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL)
  {
    return FALSE;
  }

  return (ac_ptr->state == CMAC_STATE_THERMAL_EMERG_LEVEL_2);
}

/*===========================================================================

FUNCTION cmac_is_in_thermal_emergency

DESCRIPTION
  Returns TRUE if CM AC is in emergency due to UNSAFE temperature

===========================================================================*/
boolean cmac_is_in_thermal_emergency(sys_modem_as_id_e_type asubs_id)
{
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL)
  {
    return FALSE;
  }

  if( ac_ptr->state == CMAC_STATE_EMERGENCY_ONLY      ||
      ac_ptr->state == CMAC_STATE_EMERGENCY_PENDING   ||
      ac_ptr->state == CMAC_STATE_3GPP_DETACH_PENDING ||
      ac_ptr->state == CMAC_STATE_WAIT_1X_DEREG_EMERG )
  {
    return (ac_ptr->reason_mask & BM(CM_AC_REASON_UNSAFE_TEMP));
  }

  return FALSE;

}
/*===========================================================================

FUNCTION cmac_is_me_blocked

DESCRIPTION
  Returns TRUE if CM AC is in emergency due to reject cause #6 from network

===========================================================================*/
boolean cmac_is_me_blocked(sys_modem_as_id_e_type asubs_id)
{
  cmac_s_type* ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL) {return FALSE;}

  if( ac_ptr->state == CMAC_STATE_EMERGENCY_ONLY      ||
      ac_ptr->state == CMAC_STATE_EMERGENCY_PENDING   ||
      ac_ptr->state == CMAC_STATE_3GPP_DETACH_PENDING ||
      ac_ptr->state == CMAC_STATE_WAIT_1X_DEREG_EMERG )
  {
    if (ac_ptr->reason_mask & BM(CM_AC_REASON_ME_BLOCKED))
    {
      return TRUE;
    }
  }

  return FALSE;

}

/*===========================================================================

FUNCTION cmac_is_in_sim_lock

DESCRIPTION
  Returns TRUE if CM AC is in emergency due to SIM Lock

===========================================================================*/
static boolean cmac_is_in_sim_lock(sys_modem_as_id_e_type asubs_id)
{
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL)
  {
    return FALSE;
  }

  if( ac_ptr->state == CMAC_STATE_EMERGENCY_ONLY      ||
      ac_ptr->state == CMAC_STATE_EMERGENCY_PENDING   ||
      ac_ptr->state == CMAC_STATE_3GPP_DETACH_PENDING ||
      ac_ptr->state == CMAC_STATE_WAIT_1X_DEREG_EMERG )
  {
    return (ac_ptr->reason_mask & BM(CM_AC_REASON_SIM_LOCK));
  }

  return FALSE;

}

/*===========================================================================

FUNCTION cmac_is_in_emergency_only

DESCRIPTION
  Returns TRUE if CM AC is in emergency only state due to any of the reasons

===========================================================================*/
boolean cmac_is_in_emergency_only(sys_modem_as_id_e_type asubs_id)
{
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL)
  {
    return FALSE;
  }

  return (ac_ptr->state == CMAC_STATE_EMERGENCY_ONLY );
}

/*===========================================================================

FUNCTION cmac_set_current_state

DESCRIPTION
  Set the current state of access control.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void cmac_set_current_state ( sys_modem_as_id_e_type asubs_id,
                                     cmac_state_e_type ac_state )
{
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL)
  {
    return;
  }

  if (ac_state >= CMAC_STATE_MAX || ac_state < CMAC_STATE_NORMAL ||
      !cmutil_is_as_id_valid(asubs_id))
  {
    CM_ERR_2("AC state changed failed due to invalid state: %d sub %d",ac_state,asubs_id);
    return;
  }

  if(ac_ptr->state != CMAC_STATE_NORMAL || ac_state != CMAC_STATE_NORMAL)
  {
    CM_MSG_HIGH_3("Set AC state: %d -> %d on sub %d",ac_ptr->state,ac_state,asubs_id);
  }
  ac_ptr->state = ac_state;

} /* cmac_set_current_state */

/*==========================================================================
FUNCTION cmac_is_1x_ppp_dereg_end

DESCRIPTION
  Return TRUE if 1X PPP clean up operation is ended and CM is restoring
  phone preference and forcing to SD

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

boolean cmac_is_1x_ppp_dereg_end(sys_modem_as_id_e_type asubs_id)
{
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL)
  {
    return FALSE;
  }

  return (ac_ptr->state == CMAC_STATE_1X_DEREG_END);
}

/*===========================================================================

FUNCTION cmac_client_cmd_err

DESCRIPTION
  Notify client access control command error.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void cmac_client_cmd_err
(
  cm_ac_cmd_s_type        *ac_cmd_ptr,
  cm_ac_cmd_err_e_type    cmd_err
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( ac_cmd_ptr != NULL );
  CM_ASSERT( ac_cmd_ptr->cmd_type == CM_CMD_TYPE_AC );
  CM_ASSERT( BETWEEN( cmd_err, CM_AC_CMD_ERR_NONE, CM_AC_CMD_ERR_MAX) );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Point at access control command component
  */

  /* If commanding client supplied a callback function,
  ** notify this client of command error status.
  */
  if( ac_cmd_ptr->cmd_cb_func != NULL )
  {
    ac_cmd_ptr->cmd_cb_func( ac_cmd_ptr->data_block_ptr,
                             ac_cmd_ptr->cmd,
                             cmd_err );
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If command error is OTHER than NO ERROR,
  ** notify the client list of command error, as well.
  */
  if( cmd_err != CM_AC_CMD_ERR_NOERR )
  {
    CM_MSG_HIGH_3("AC cmd err!, cmd=%d, err=%d, client=%ld",
                  ac_cmd_ptr->cmd, cmd_err, ac_cmd_ptr->client_id);

    cmclient_list_ac_cmd_err_ntfy( cmd_err, ac_cmd_ptr );
  }

} /* cmac_client_cmd_err() */

/*===========================================================================

FUNCTION cmac_client_cmd_check

DESCRIPTION
  Check for access control command parameter errors

DEPENDENCIES
  None.

RETURN VALUE
  CM_AC_CMD_ERR_NOERR if command is allowed in the current state

SIDE EFFECTS
  Might change one or more of the command parameters pointed by cmd_ptr.

===========================================================================*/

static cm_ac_cmd_err_e_type  cmac_client_cmd_check
(
  cm_ac_cmd_s_type *ac_cmd_ptr
)
{
  cm_ac_cmd_err_e_type    ac_cmd_err       = CM_AC_CMD_ERR_NOERR;
  cm_ac_cmd_info_s_type   *cmd_info_ptr    = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( ac_cmd_ptr != NULL );
  CM_ASSERT( ac_cmd_ptr->cmd_type == CM_CMD_TYPE_AC );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_LOW_1("START cmac_client_cmd_check(), cmd=%d",ac_cmd_ptr->cmd );

  cmd_info_ptr = CMD_INFO_PTR(ac_cmd_ptr);
  CM_ASSERT( cmd_info_ptr != NULL );

  /* check cmd is already in error or not */
  if ( ac_cmd_ptr->cmd_err != CM_AC_CMD_ERR_NOERR)
  {
    return ac_cmd_ptr->cmd_err;
  }

  if ( cmd_info_ptr->reason >= CM_AC_REASON_MAX ||
       cmd_info_ptr->reason <= CM_AC_REASON_NONE )
  {
    return CM_AC_CMD_ERR_INVALID_REASON;
  }

  if (cmph_is_ssim())
  {
    cmd_info_ptr->asubs_id = SYS_MODEM_AS_ID_1;
  }

  if (!cmutil_is_as_id_valid(cmd_info_ptr->asubs_id))
  {
    CM_MSG_HIGH_1("CMAC: cmd has invalid as_id %d",cmd_info_ptr->asubs_id);
    cmd_info_ptr->asubs_id = SYS_MODEM_AS_ID_1;
  }
  return ac_cmd_err;

} /* cmac_client_cmd_check() */


/*===========================================================================

FUNCTION cmac_send_notify_callback

DESCRIPTION
  Send event to clients through callbacks with reason as payload

===========================================================================*/

static void cmac_send_notify_callback(

  sys_modem_as_id_e_type asubs_id,

  cm_ac_event_e_type evt,
  /* CMAC evt that is sent to CM-clients through callbacks */

  cm_ac_reason_e_type reason
  /* Reason for generating this event */
)
{
  cm_ac_info_s_type evt_info;

  evt_info.asubs_id = asubs_id;
  evt_info.reason   = reason;

  cmclient_list_ac_event_ntfy(evt, &evt_info);

  if(reason == CM_AC_REASON_UNSAFE_TEMP)
  {
    cmph_event_per_subs(CM_PH_EVENT_THERMAL_EMERG_ENTER,asubs_id);
  }
}

/*===========================================================================

FUNCTION cmac_send_notify_ind

DESCRIPTION
  Send event to client through message router with reason as payload

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void cmac_send_notify_ind(
  msgr_umid_type          ac_evt_umid,
  cm_ac_reason_e_type     reason,
  void                   *user_data,
  sys_modem_as_id_e_type  asubs_id,
  boolean          is_ready_due_to_csfb
)
{
  errno_enum_type msgr_error;
  cm_ac_info_ind_s_type *ac_evt;

  ac_evt = (cm_ac_info_ind_s_type *)cm_mem_malloc(
             sizeof(cm_ac_info_ind_s_type));

  if(ac_evt == NULL)
  {
    sys_err_fatal_null_ptr_exception();
  }

  memset(ac_evt, 0, sizeof(cm_ac_info_ind_s_type));

  ac_evt->reason    = reason;
  ac_evt->user_data = user_data;
  ac_evt->asubs_id  = asubs_id;
  ac_evt->is_ready_due_to_csfb = is_ready_due_to_csfb;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_3("CM->MSGR: NOTIFY_IND, asubs_id=%d, reason=%d, is_ready_due_to_csfb =%d",
                ac_evt->asubs_id,
                reason,ac_evt->is_ready_due_to_csfb);

  msgr_error = cm_msgr_send( ac_evt_umid,
                             MSGR_CM_AC,
                             &(ac_evt->hdr),
                             sizeof(cm_ac_info_ind_s_type),
                             asubs_id);
  cm_mem_free(ac_evt);

} /* cmac_send_notify_ind */

/*===========================================================================

FUNCTION cmac_init

DESCRIPTION
  Access control initialize

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void cmac_init( void )
{
  uint8 i=0;

  for (i=0; i<cmpmprx_get_num_of_sims(); i++)
  {
    if (cmac_local[i] == NULL)  /* Should always evaluate TRUE */
    {
      cmac_local[i] = (cmac_s_type *)cm_mem_malloc(sizeof(cmac_s_type));
    }
    cmac_local[i]->reason         = CM_AC_REASON_NONE;
    cmac_local[i]->reason_mask    = 0;
    cmac_local[i]->state          = CMAC_STATE_NORMAL;
    cmac_local[i]->bound_timer    = 0;
    cmac_local[i]->cmd_type       = CM_AC_CMD_NONE;
    cmac_local[i]->data_block_ptr = NULL;
    cmac_local[i]->state_cb_func  = NULL;
    cmac_local[i]->cmd_cb_func    = NULL;
    cmac_local[i]->asubs_id       = i;
    cmac_local[i]->dereg_1x_timer = CMAC_UPTIME_NONE;
  }
} /* cmac_init() */

/*===========================================================================

FUNCTION cmac_send_mc_deregister

DESCRIPTION
  Ask 1xcp to deregister Network and waiting for confirmation

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - If successful
  FALSE - If failed

SIDE EFFECTS
  none

===========================================================================*/
void cmac_send_mc_deregister( sys_modem_as_id_e_type asubs_id )
{
#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900))

  mc_msg_type *mc_ptr = NULL;
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);



  if (ac_ptr == NULL) 
  {
    return;
  }

  mc_ptr = cm_mc_get_buf_else_err_fatal();
  
  mc_ptr->hdr.cmd = MC_DEREGISTER_F;
    cm_mc_send_cmd(asubs_id, mc_ptr);
  
  CM_MSG_HIGH_0("AC: CM->1x Send deregister to 1xCP");

  if(ac_ptr->reason == CM_AC_REASON_UNUSED_PPP)
  {
    cmac_set_current_state(asubs_id,CMAC_STATE_WAIT_1X_DEREG);
  }
  else
  {
    cmac_set_current_state(asubs_id,CMAC_STATE_WAIT_1X_DEREG_EMERG);
  }

#endif
} /* cmac_send_mc_deregister */


/*===========================================================================

FUNCTION cmac_clear_1xppp_context_proc

DESCRIPTION
  Change the mode pref to CDMA only with priority AC_EMERG. Nobody will change the pref
  except E911.

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmac_clear_1xppp_context_proc(cm_ac_cmd_info_s_type *cmd_info)
{
  sys_modem_as_id_e_type as_id = cmd_info->asubs_id;
  cmac_s_type           *ac_ptr = cmac_ptr(as_id);
  cm_pref_s_type        *pref_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(as_id);

  if(ph_sub_ptr == NULL || ac_ptr == NULL)
  {
    return;
  }

  if((pref_ptr = cm_pref_ptr()) == NULL)
  {
    return;
  }

  // Save current user mode pref
  ac_ptr->cur_mode_pref  = ph_sub_ptr->pref_info.mode_pref;

  // Notify DS with start clear indication
  cmac_send_notify_ind( MM_CM_AC_1XPPP_CLEAR_START_IND,
                        CM_AC_REASON_UNUSED_PPP,
                        NULL,
                        as_id,FALSE);

  // Start timer
  ac_ptr->bound_timer = time_get_uptime_secs() + 4;
  ac_ptr->reason = CM_AC_REASON_UNUSED_PPP;

  /* Change the mode pref to CDMA only with priority AC_EMERG. Nobody will change the pref
   * except E911.
   */
  memscpy(pref_ptr, sizeof(cm_pref_s_type),
          &(ph_sub_ptr->pref_info), sizeof(cm_pref_s_type));
  pref_ptr->pref_term       = CM_PREF_TERM_CM_1_CALL_PLUS;
  pref_ptr->prl_pref        = CM_PRL_PREF_NO_CHANGE;
  pref_ptr->roam_pref       = CM_ROAM_PREF_NO_CHANGE;
  pref_ptr->hybr_pref       = CM_HYBR_PREF_NO_CHANGE;
  pref_ptr->mode_pref       = CM_MODE_PREF_CDMA_ONLY;
  pref_ptr->band_pref       = CM_BAND_PREF_NO_CHANGE;
  pref_ptr->tds_band_pref   = CM_BAND_PREF_NO_CHANGE;
  pref_ptr->lte_band_pref   = SYS_LTE_BAND_MASK_CONST_NO_CHG;
  pref_ptr->srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;

  CM_MSG_HIGH_1("AC: >>PH FORCE_PREF: Forcing AC EMERG pref sub %d", as_id);

  (void)cmph_force_pref_on_the_fly_stack(
    cmph_map_sub_stk_to_id(as_id, MM_STACK_0),
    SD_SS_PREF_REAS_USER,
    CM_ACT_TYPE_AC_EMERG_MODE,
    pref_ptr,
    CM_OTASP_ACT_CODE_NONE,
    cmac_get_act_id(as_id),
    CM_ACT_UPDATE_REAS_ACT_START,
    TRUE,
    cmph_get_acq_pri_order_per_sub(as_id),
    CM_DEFAULT_VALUE);
  cm_mem_free (pref_ptr);

  // For sure the ph mode pref is restored
  ph_sub_ptr->pref_info.mode_pref = ac_ptr->cur_mode_pref;

  /* Before CM knows 1x is available, set the current AC state to CMAC_STATE_WAIT_1X_DEREG */
  cmac_set_current_state (as_id,CMAC_STATE_WAIT_1X_SRV);

}

/*===========================================================================

FUNCTION cmac_stop_mode_cnf_proc

DESCRIPTION
  As entering emergency happens in two stages, this function implements
  second stage after getting cnf from GWL deactivation.
  If there is no need to deactivate then this function is called directly
  at the end of first stage.
===========================================================================*/

static void cmac_stop_mode_cnf_proc(sys_modem_as_id_e_type as_id)
{
  cm_mode_pref_e_type prep_mode_pref = CM_MODE_PREF_NONE;
  cmac_s_type         *ac_ptr        = NULL;
  cm_mode_pref_e_type cur_mode_pref;
  sys_modem_as_id_e_type sub_idx = SYS_MODEM_AS_ID_1;
  cm_pref_s_type      *pref_info_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = NULL;

  /* Disable HDR in emergency state.
  */
  *cmph_hdr_kick_pending() = FALSE;

#ifdef CM_GW_SUPPORTED
  cmwcall_set_lmtd_srvc_req ();
#endif

  CM_MSG_HIGH_1("ME_BLOCKED: cmac_stop_mode_cnf_proc as_id %d",as_id);
  
  for(sub_idx=SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub_idx); sub_idx++)
  {
    ac_ptr         = cmac_ptr(sub_idx);
    ph_sub_ptr = cmph_get_sub_ptr(sub_idx);
    if (ph_sub_ptr == NULL || ac_ptr == NULL)
    {
      continue;
    }

    if ((ac_ptr->reason == CM_AC_REASON_NONE && ac_ptr->reason_mask == 0) ||
        ac_ptr->state == CMAC_STATE_WAIT_1X_DEREG_EMERG)
    { 
      continue; 
    }
    
    cur_mode_pref  = ph_sub_ptr->pref_info.mode_pref;

    CM_MSG_HIGH_2("CMAC: >>PH FORCE_PREF: Before forcing emergency pref, ph mode_pref 0x%x asubs_id %d",
                  cur_mode_pref, sub_idx);

    cmac_set_current_state(sub_idx, CMAC_STATE_EMERGENCY_ONLY);


    /* Disable HDR in emergency state */
    prep_mode_pref = cm_remove_mode_pref_components( cur_mode_pref,
                     1,
                     SD_SS_MODE_PREF_HDR);

#if (defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE)||defined(FEATURE_TDSCDMA))
    /* If LTE is active on stack 2, then LTE on stack 2 is disabled. Because
    ** we allow emergency traffic only on one stack in ETM state.
    */
    if(cmss_is_stack2_operational(sub_idx) &&
        cmregprx_get_state(cmph_map_sub_stk_to_id(sub_idx, MM_STACK_2)) ==  CMREGPRX_STATE_ACTIVE)
    {
      prep_mode_pref = cm_remove_mode_pref_components(prep_mode_pref,
                       1,
                       SD_SS_MODE_PREF_LTE);
    }
#endif

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    ph_sub_ptr->pref_info.mode_pref = prep_mode_pref;
    if ((pref_info_ptr = cm_pref_ptr()) == NULL) return;

    memscpy( pref_info_ptr,
             sizeof(cm_pref_s_type),
             &(ph_sub_ptr->pref_info),
             sizeof(cm_pref_s_type));

    pref_info_ptr->network_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_LIMITED_SRV;
    pref_info_ptr->hybr_pref = CM_HYBR_PREF_OFF;
    pref_info_ptr->srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;
    if(cmac_is_in_thermal_emergency(sub_idx) || (cmmsc_get_1x_sub() == sub_idx) || cmac_is_me_blocked(sub_idx))
    {
      cmph_force_pref_on_the_fly_stack( cmph_map_sub_stk_to_id(sub_idx, MM_STACK_ALL),
                                        SD_SS_PREF_REAS_USER,
                                        CM_ACT_TYPE_AC_EMERG_MODE,
                                        pref_info_ptr,
                                        CM_OTASP_ACT_CODE_NONE,
                                        cmac_get_act_id(sub_idx),
                                        CM_ACT_UPDATE_REAS_EMERG_ENTER,
                                        TRUE,
                                        cmph_get_acq_pri_order_per_sub(sub_idx),
                                        CM_DEFAULT_VALUE
                                      );
    }
    cm_mem_free (pref_info_ptr);
    
    ph_sub_ptr->pref_info.mode_pref = cur_mode_pref;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    if (cmmsc_get_1x_sub() == sub_idx)
    {
      cmph_update_ddtm_status(sub_idx);
    }
    cmac_send_notify_callback(sub_idx,CM_AC_EVENT_EMERGENCY_ENTER, ac_ptr->reason);

  }
}

/*===========================================================================

FUNCTION cmac_is_stop_mode_cnfs_pending_per_device

DESCRIPTION
  Return true if cmac_is_no_stop_mode_cnfs_pending_per_device is 0
  across all subs, false otherwise
===========================================================================*/
static boolean cmac_is_no_stop_mode_cnfs_pending_per_device()
{
  cmac_s_type         *ac_ptr = NULL;
  sys_modem_as_id_e_type  sub_idx = SYS_MODEM_AS_ID_1;

  for(sub_idx=SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub_idx); sub_idx++)
  {
    ac_ptr = cmac_ptr(sub_idx);
    if(ac_ptr == NULL)
    {
      continue;
    }
    if(ac_ptr->stop_mode_cnfs_pending != 0)
    {
      return FALSE;
    }

  }
  return TRUE;

}

/*===========================================================================

FUNCTION cmac_deregister_cnf_proc

DESCRIPTION
  As entering emergency happens in three stages, this function implements
  second stage after getting deregister cnf from 1X CP.
  If there is no need to deregister from 1X CP then this function is called directly
  at the end of first stage.
===========================================================================*/

static void cmac_deregister_cnf_proc(sys_modem_as_id_e_type asubs_id)
{
  cm_mode_pref_e_type prep_mode_pref;
  cm_mode_pref_e_type cur_mode_pref;
  uint8               sub_idx = 0;
  cmac_s_type         *ac_ptr = NULL;
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(ph_sub_ptr == NULL)
  {
    return;
  }
  cur_mode_pref = ph_sub_ptr->pref_info.mode_pref;
  ac_ptr = cmac_ptr(asubs_id);
  if (ac_ptr == NULL)
  {
    return;
  }

  ac_ptr->stop_mode_cnfs_pending = 0;
  ac_ptr->dereg_1x_timer         = CMAC_UPTIME_NONE;
  /* Remove GWL from mode pref to send STOP_MODE to GWL/TD. This is required
  ** to put GWL/TD on limited service.
  */
  prep_mode_pref = cm_remove_mode_pref_components( cur_mode_pref,
                   1,
                   SD_SS_MODE_PREF_TDS_GSM_WCDMA_LTE
                                                 );

  CM_MSG_HIGH_1("ME_BLOCKED: cmac_deregister_cnf_proc %d", asubs_id);
  
  cmac_set_current_state(asubs_id, CMAC_STATE_3GPP_DETACH_PENDING);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( prep_mode_pref != cur_mode_pref && ph_sub_ptr->is_3gpp_subs_avail)
  {
    ph_sub_ptr->pref_info.mode_pref = prep_mode_pref;

    /* If Regprx is active CM needs to wait for STOP_MODE_CNF before
    ** entering Emergency only mode.
    */
#ifdef CM_GW_SUPPORTED
    if(cmregprx_get_state(cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0))
        == CMREGPRX_STATE_ACTIVE )
    {
      cm_pref_s_type *pref_info_ptr = cm_pref_ptr();

      ac_ptr->stop_mode_cnfs_pending++;

      memscpy( pref_info_ptr,
               sizeof(cm_pref_s_type),
               &(ph_sub_ptr->pref_info),
               sizeof(cm_pref_s_type));
      pref_info_ptr->srv_domain_pref = CM_SRV_DOMAIN_PREF_CS_ONLY;
      cmph_force_pref_on_the_fly_stack( cmph_map_sub_stk_to_id(asubs_id, MM_STACK_0),
                                        SD_SS_PREF_REAS_USER,
                                        CM_ACT_TYPE_AC_EMERG_MODE,
                                        pref_info_ptr,
                                        CM_OTASP_ACT_CODE_NONE,
                                        cmac_get_act_id(asubs_id),
                                        CM_ACT_UPDATE_REAS_EMERG_ENTER,
                                        TRUE,
                                        cmph_get_acq_pri_order_per_sub(asubs_id),
                                        CM_DEFAULT_VALUE
                                      );
     cm_mem_free (pref_info_ptr);
    }
#endif
  }
  ph_sub_ptr->pref_info.mode_pref = cur_mode_pref;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(cmac_is_no_stop_mode_cnfs_pending_per_device())
  {
    CM_MSG_HIGH_1("ME_BLOCKED: no stop pending asubs_id %d",asubs_id);
    cmac_stop_mode_cnf_proc(asubs_id);
  }
}

/*===========================================================================

FUNCTION cmac_restrict_all_subs_to_emergency

DESCRIPTION
  Restricts all subs to emergency mode.
===========================================================================*/
void cmac_restrict_all_subs_to_emergency(

  cm_ac_reason_e_type reason
    /* Reason for putting modem in emergency mode */
)
{
  sys_modem_as_id_e_type asubs_id;

  for ( asubs_id = SYS_MODEM_AS_ID_1;
        cmutil_is_as_id_valid(asubs_id);
        asubs_id++ )
  {
    cmac_restrict_subs_to_emergency(reason, asubs_id);
  }

  return;
}

/*===========================================================================

FUNCTION cmac_restrict_subs_to_emergency

DESCRIPTION
  This function implements access control putting phone in emergency mode
  due to reasons in cm_ac_reason_e_type. This happens in two stages:
  1. If GWL protocols are active, then a deactivate is sent
  2. After recving conf for GWL activate, GWL protocols are put in limited
     service mode

  If at #1, GWL protocols are not active or in limited srv already, then #2
  is performed wihout delay.
===========================================================================*/
void cmac_restrict_subs_to_emergency(

  cm_ac_reason_e_type reason,
  /* Reason for putting modem in emergency mode */

  sys_modem_as_id_e_type asubs_id
)
{

  cmac_s_type         *ac_ptr = cmac_ptr(asubs_id);
  sys_modem_as_id_e_type subs;
  if (ac_ptr == NULL) {return;}

  CM_MSG_HIGH_4("CMAC sub %d is in state %d, emerg reason %d mask 0x%x",
                asubs_id, ac_ptr->state, reason, ac_ptr->reason_mask);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Add reason mask to access control requests */
  ac_ptr->reason_mask |= BM(reason);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If modem is already in emergency state, ACK calling CM-client */
  if( ac_ptr->state == CMAC_STATE_EMERGENCY_ONLY )
  {
    cmac_send_notify_callback(asubs_id, CM_AC_EVENT_EMERGENCY_ENTER, reason);
    return;
  }

  /* If already in any other AC state, then buffer this req */
  if( ac_ptr->state != CMAC_STATE_NORMAL && 
      ac_ptr->state != CMAC_STATE_THERMAL_EMERG_LEVEL_2 &&
      ac_ptr->state != CMAC_STATE_1X_DEREG_RETRY)
  {
    return;
  }

  /* If IMS deregistration/ DS PPP cleanup is pending on the subscription
  ** which is being put in emergency only mode, defer this request till the
  ** said operations complete.
  */
  if( cmclnup_is_ongoing_subsc(BM(asubs_id)) )
  {
    cmac_set_current_state(asubs_id, CMAC_STATE_WAIT_CLNUP );
    return;
  }
  
  #if defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
  cmwcall_send_pdp_abort_req(asubs_id);
  #endif
    if (reason == CM_AC_REASON_ME_BLOCKED)
    {
      // End the call only on the sub where 
    cmcall_endall_non_emerg_calls_per_subs_except_vowifi_gw_ps(CM_CALL_END_EMERGENCY_ONLY, FALSE, asubs_id);
    
    cmcall_cleanup_gw_ps_call_locally(asubs_id,CM_CALL_END_EMERGENCY_ONLY);
    }
    else if(reason == CM_AC_REASON_UNSAFE_TEMP)
  {
    cmcall_endall_non_emerg_calls_except_gw_ps(CM_CALL_END_THERMAL_EMERGENCY, FALSE);
    for ( subs = SYS_MODEM_AS_ID_1;
        cmutil_is_as_id_valid(subs);
        subs++ )
    {
       cmcall_cleanup_gw_ps_call_locally(subs,CM_CALL_END_THERMAL_EMERGENCY);
    }
  }
  else
  {
     cmcall_endall_non_emerg_calls_except_gw_ps(CM_CALL_END_EMERGENCY_ONLY, FALSE);
     for ( subs = SYS_MODEM_AS_ID_1;
        cmutil_is_as_id_valid(subs);
        subs++ )
    {
       cmcall_cleanup_gw_ps_call_locally(subs,CM_CALL_END_EMERGENCY_ONLY);
  }
}
  ac_ptr->reason = reason;

  if(cmcall_emergency_call_active() || cmph_is_in_emergency_cb())
  {
    cmac_set_current_state(asubs_id,CMAC_STATE_EMERGENCY_PENDING);
    return;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If CDMA defined send deregister to 1X, independent of whether actually in service
  ** 1X will respond with failure even when it is not active. Incase 1X is not defined
  ** directly go to handling of deregister cnf
  */
#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900))
  if(asubs_id == cmmsc_get_1x_sub())
  {
    cmac_send_mc_deregister(asubs_id);
  }
  else
#endif
  {
    cmac_deregister_cnf_proc(asubs_id);
  }

}

/*===========================================================================

FUNCTION cmac_enable_emergency_if_pending

DESCRIPTION
  This function enables emergency mode if it was deferred due to emergency
  call or callback mode.
===========================================================================*/
void cmac_enable_emergency_if_pending(void)
{
  sys_modem_as_id_e_type  asubs_id;
  cm_ac_reason_e_type     reason = CM_AC_REASON_NONE;
  
  for ( asubs_id = SYS_MODEM_AS_ID_1;
        cmutil_is_as_id_valid(asubs_id);
        asubs_id++ )
  {
    cmac_s_type  *ac_ptr = cmac_ptr(asubs_id);
    if (ac_ptr == NULL)
    {
      continue;
    }

    if( ac_ptr->state == CMAC_STATE_EMERGENCY_PENDING ||
        ac_ptr->state == CMAC_STATE_WAIT_CLNUP )
    {
      cmac_set_current_state(asubs_id, CMAC_STATE_NORMAL);
      reason = ac_ptr->reason;
    }

    if (reason == CM_AC_REASON_ME_BLOCKED && 
         (ac_ptr->state == CMAC_STATE_EMERGENCY_PENDING ||
          ac_ptr->state == CMAC_STATE_WAIT_CLNUP) )
    {
      cmac_restrict_subs_to_emergency(CM_AC_REASON_ME_BLOCKED, asubs_id);
    }
  }
  if( reason != CM_AC_REASON_NONE && reason != CM_AC_REASON_ME_BLOCKED)
  {
    cmac_restrict_all_subs_to_emergency(reason);
  }
  
}

/*===========================================================================

FUNCTION cmac_restore_all_subs_to_normal

DESCRIPTION
  Restore access control normal state where normal calls are enabled.
  Before call of this API, phone may be in emergency only mode due to
  reasons in cm_ac_reason_e_type.
===========================================================================*/

static void cmac_restore_to_normal(

  sys_modem_as_id_e_type asubs_id,

  cm_ac_reason_e_type reason
  /* Reason due to which normal opr being restored. */
)
{
  cmph_s_type       *ph_ptr = cmph_ptr();
  cm_orig_q_s_type  *ph_orig_ptr;
  cmac_s_type       *ac_ptr = cmac_ptr(asubs_id);
  sys_modem_as_id_e_type as_id_emg_cb = cmph_get_sub_in_emg_cb();

  uint16 updated_mask;

  if (ac_ptr == NULL)
  {
    return;
  }
  updated_mask = ac_ptr->reason_mask & ~BM(reason);
  if( updated_mask & ( BM(CM_AC_REASON_UNSAFE_TEMP) |
                       BM(CM_AC_REASON_MAND_PDN_DISABLED) |
                       BM(CM_AC_REASON_SIM_LOCK) |
                       BM(CM_AC_REASON_ME_BLOCKED) )
    )
  {
    /* Update the reason mask */
    ac_ptr->reason_mask = updated_mask;

    /* Notify the clients about EMEGENCY_EXIT_PEDNING because not
    ** not call reasons for EMERGENCY_ONLY operation has been
    ** revoked.
    ** Send out the reason due to which normal operation was requested.
    ** This would serve as an intermediate confirmation that the request
    ** has been processed by CM but device cannot be moved to NORMAL
    ** operation yet.
    */
    cmac_send_notify_callback( asubs_id, CM_AC_EVENT_EMERGENCY_EXIT_PENDING, reason );

    CM_MSG_HIGH_2("CMAC updated mask %d can't restore normal opr on sub %d",
                  updated_mask,asubs_id);
    return;
  }

  // Detach pending state is required in case if CFCM moves to level 0
  // before CM actually moves to level 3
  if( ac_ptr->state == CMAC_STATE_EMERGENCY_ONLY    ||
      ac_ptr->state == CMAC_STATE_EMERGENCY_PENDING ||
      ac_ptr->state == CMAC_STATE_3GPP_DETACH_PENDING )
  {
    mm_sub_stk_id_s_type local_mmid;

    CM_MSG_HIGH_0("Deleting AC object");

    ac_ptr->stop_mode_cnfs_pending = 0;
    ac_ptr->dereg_1x_timer         = CMAC_UPTIME_NONE;

    cmac_set_current_state(asubs_id, CMAC_STATE_NORMAL);

    local_mmid.asubs_id = asubs_id;

    for ( local_mmid.stk_id = MM_STACK_0;
          local_mmid.stk_id < cmtask_get_num_stacks(asubs_id);
          local_mmid.stk_id++)
    {
      if (asubs_id == SYS_MODEM_AS_ID_1)
      {
        cmph_delete_orig_param( cmac_get_act_id(local_mmid.asubs_id),local_mmid);
        cmph_remove_orig_mode_with_priority_end(local_mmid);
      }
      else
      {
        cmph_delete_update_orig_mode(local_mmid, cmac_get_act_id(local_mmid.asubs_id), TRUE);
      }
    }

    if ((as_id_emg_cb != SYS_MODEM_AS_ID_NONE && as_id_emg_cb == asubs_id) ||
        (as_id_emg_cb == SYS_MODEM_AS_ID_NONE && asubs_id == SYS_MODEM_AS_ID_1))
    {
      local_mmid.stk_id = MM_STACK_0;
      ph_orig_ptr = cmtask_orig_para_get_top(local_mmid);

      if(ph_orig_ptr != NULL &&
          ph_orig_ptr->act_type == CM_ACT_TYPE_PH_OBJ)
      {
        CM_MSG_HIGH_1("Forcing preferences after deleting AC object on sub %d  All stk",local_mmid.asubs_id);
        local_mmid.stk_id   = MM_STACK_ALL;
        cmph_force_orig_mode_on_the_stack(local_mmid);
      }
    }

    if (cmmsc_get_1x_sub() == asubs_id)
    {
      cmph_update_ddtm_status(local_mmid.asubs_id);
    }
  }

  /* Update CMAC internal state */
  cmac_set_current_state(asubs_id,CMAC_STATE_NORMAL);

  /* Update rason_mask*/
  ac_ptr->reason_mask = updated_mask;

  /* Notify clients that UE is resuming normal operation */
  cmac_send_notify_callback(asubs_id, CM_AC_EVENT_EMERGENCY_EXIT, reason);
}

/*===========================================================================

FUNCTION cmac_restore_all_subs_to_normal

DESCRIPTION
  Restore access control normal state where normal calls are enabled.
  Before call of this API, phone may be in emergency only mode due to
  reasons in cm_ac_reason_e_type.
===========================================================================*/

void cmac_restore_all_subs_to_normal(

  cm_ac_reason_e_type reason
  /* Reason due to which normal opr being restored. */
)
{
  sys_modem_as_id_e_type asubs_id;

  for (asubs_id = SYS_MODEM_AS_ID_1;
       cmutil_is_as_id_valid(asubs_id);
       asubs_id++)
  {
    cmac_restore_to_normal(asubs_id,reason);
  }
}

/*===========================================================================

FUNCTION cmac_mc_rpt_proc

DESCRIPTION
  Process ac reports specific to DS

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmac_mc_rpt_proc(

  const cm_hdr_type *rpt_ptr

)
{
#if defined (FEATURE_LTE) && (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900))

  cm_mc_rpt_type          *mc_rpt_ptr = (cm_mc_rpt_type *) rpt_ptr;
  sys_modem_as_id_e_type  asubs_id = SYS_MODEM_AS_ID_1;
  cmac_s_type             *ac_ptr;

  if (cmph_is_msim())
  {
    asubs_id = mc_rpt_ptr->hdr.asubs_id;
  }

  if ((ac_ptr = cmac_ptr(asubs_id)) == NULL)
  {
    CM_MSG_HIGH_1("ac_ptr null ptr sub %d", asubs_id);
    return;
  }

  CM_MSG_HIGH_2("1x->CM: asubs_id %d rpt %d",
                mc_rpt_ptr->hdr.asubs_id, mc_rpt_ptr->hdr.cmd);

  switch(mc_rpt_ptr->hdr.cmd)
  {
    /* Service Indicators information is changed.
    */
    case CM_SRV_SYS_INFO_F:
    {
      //process srv ind only if we're in CMAC_STATE_WAIT_1X_SRV
      //if we get srv, then cmac_send_mc_deregister
      //change state to CMAC_STATE_WAIT_1X_DEREG
      if (ac_ptr->state != CMAC_STATE_WAIT_1X_SRV)
        return;

      cmac_send_mc_deregister(asubs_id);
    }
    break;

    case CM_DEREGISTER_RSP_F:
    {

      /* CM gets confirmation from 1x */

      CM_MSG_HIGH_2("CMAC 1x De-registration cause: %d, AC state %d",
                    mc_rpt_ptr->mc_dereg_info.cause,ac_ptr->state);

      CM_MSG_HIGH_2("ME_BLOCKED: header asubs_id %d, changing to %d",asubs_id, cmmsc_get_1x_sub());
      /* process only if we're in some wait state */
      if (ac_ptr->state == CMAC_STATE_WAIT_1X_DEREG)
      {
        cm_ac_end_reason_e_type  end_reason;
        mm_sub_stk_id_s_type local_mm_id = cmph_map_sub_stk_to_id(asubs_id,MM_STACK_0);

        /* When receiving this confirmation, send MM_CM_AC_1XPPP_CLEAR_END_IND to anyone
        ** who is interested in this message.
        */
        cmac_send_notify_ind( MM_CM_AC_1XPPP_CLEAR_END_IND,
                              CM_AC_REASON_UNUSED_PPP,
                              NULL,
                              asubs_id,FALSE);

        switch(mc_rpt_ptr->mc_dereg_info.cause)
        {
          case CM_DEREG_FAIL_TIMER:
            end_reason = CM_AC_END_REASON_FAIL_TIMER;
            break;
          case CM_DEREG_FAIL_E911_CALL:
            end_reason = CM_AC_END_REASON_FAIL_E911;
            break;
          case CM_DEREG_FAIL_1X_INTERNAL:
            end_reason = CM_AC_END_REASON_FAIL_1X_INTERNAL;
            break;
          case CM_DEREG_SUCCESS:
            end_reason = CM_AC_END_REASON_SUCCESS;
            break;
          default:
            end_reason = CM_AC_END_REASON_NONE;
            break;
        }

        if (ac_ptr->state_cb_func != NULL)
        {
          ac_ptr->state_cb_func( ac_ptr->data_block_ptr,
                                 ac_ptr->cmd_type,
                                 CM_AC_STATE_END,
                                 end_reason);
        }
        else
        {
          CM_MSG_HIGH_0("state_cb_func NULL");
        }

        /* Set CMAC state to 1X_DEREG_END, so that CM forces preferences to
        ** SD with preference update reason 1X_DEREG_END.
        */
        cmac_set_current_state(asubs_id, CMAC_STATE_1X_DEREG_END);

        CM_MSG_HIGH_0("Restore mode pref");
        cmph_delete_update_orig_mode (local_mm_id, cmac_get_act_id(asubs_id), TRUE);

        /* To-Do: Put the phone into CMAC_STATE_NORMAL */
        cmac_set_current_state(asubs_id, CMAC_STATE_NORMAL);
      }
      else if(ac_ptr->state == CMAC_STATE_WAIT_1X_DEREG_EMERG )
      {
        /* We come here only when deregister is sent as part of
        ** entering emergency mode */

        if(mc_rpt_ptr->mc_dereg_info.cause == CM_DEREG_FAIL_E911_CALL || 
           mc_rpt_ptr->mc_dereg_info.cause == CM_DEREG_FAIL_1X_INTERNAL)
        {
          /* Deregister failed due to emergency call
          ** start whole process again */
          CM_MSG_HIGH_2("ME_BLOCKED: Retry after %d secs, curr_time %d",CMAC_1X_DEREG_TIMER, time_get_uptime_secs());
          cmac_set_current_state(asubs_id, CMAC_STATE_1X_DEREG_RETRY);
          ac_ptr->dereg_1x_timer = time_get_uptime_secs() + CMAC_1X_DEREG_TIMER;
        }
        else
        {
          
          /*First set DDTM to avoid registration again*/
          CM_MSG_HIGH_1("ME_BLOCKED: do not Retry 1x deregister subs %d",asubs_id);
          cmph_ddtm_disable_cdma_tx(asubs_id);
          {
            sys_modem_as_id_e_type sub_idx;
            for(sub_idx=SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub_idx); sub_idx++)
            {
              if ((ac_ptr->reason == CM_AC_REASON_ME_BLOCKED || ac_ptr->reason == CM_AC_REASON_MAND_PDN_DISABLED)
                    && asubs_id != sub_idx) 
              { 
                continue;
              }
              if (cmac_ptr(sub_idx) == NULL)
              {
                return;
              }
              cmac_deregister_cnf_proc(sub_idx);
            }
          }
        }
      }
    }
    break;

    default:
      break;
  }
#endif
}

/*===========================================================================

FUNCTION cmac_nas_rpt_proc

DESCRIPTION
  Process reports from NAS. In CMAC, we are looking at STOP_MODE_CNF

===========================================================================*/
void cmac_nas_rpt_proc(

  const cm_hdr_type   *rpt_ptr

)
{
  cm_rpt_type *cm_rpt_ptr = (cm_rpt_type *)rpt_ptr;
  cmac_s_type       *ac_ptr;
  if(cm_rpt_ptr == NULL) return;

  ac_ptr = cmac_ptr(cm_rpt_ptr->hdr.asubs_id);
  if(ac_ptr == NULL) return;

  switch(cm_rpt_ptr->hdr.cmd)
  {
    case CM_STOP_MODE_CNF:
      if(ac_ptr->state == CMAC_STATE_3GPP_DETACH_PENDING)
      {
        ac_ptr->stop_mode_cnfs_pending--;
        if(cmac_is_no_stop_mode_cnfs_pending_per_device())
        {
          cmac_stop_mode_cnf_proc(cm_rpt_ptr->hdr.asubs_id);
        }
      }
      break;

    default:
      /* AC isn't interested in other NAS reports */
      break;
  }
}

/*===========================================================================

FUNCTION cmac_timer_proc

DESCRIPTION
  Process timer events

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmac_timer_proc(

  cm_timer_event_type    timer_event
  /* Indicate specific timer event.
  */
)
{

  dword                uptime = time_get_uptime_secs();
  sys_modem_as_id_e_type asubs_id;

  for ( asubs_id = SYS_MODEM_AS_ID_1;
        cmutil_is_as_id_valid(asubs_id);
        asubs_id++ )
  {
    cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
    if (ac_ptr == NULL)
    {
      continue;
    }

    /* Check if maximum time expired.
    */
    if ( uptime > ac_ptr->bound_timer &&
         ( ac_ptr->state == CMAC_STATE_WAIT_1X_DEREG ||
           ac_ptr->state == CMAC_STATE_WAIT_1X_SRV) )
    {
      /* If timer expires and we're in wait state. then clear all state
      ** This is applicable only for 1XPPP clearing
      */
      mm_sub_stk_id_s_type local_mm_id = cmph_map_sub_stk_to_id(asubs_id,MM_STACK_0);

      /* When receiving this confirmation, send MM_CM_AC_1XPPP_CLEAR_END_IND to anyone
      ** who is interested in this message.
      */
      cmac_send_notify_ind( MM_CM_AC_1XPPP_CLEAR_END_IND,
                            CM_AC_REASON_UNUSED_PPP,
                            NULL,
                            asubs_id,FALSE);

      // Tell DS system time out
      if (ac_ptr->state_cb_func != NULL)
      {
        ac_ptr->state_cb_func( ac_ptr->data_block_ptr,
                               ac_ptr->cmd_type,
                               CM_AC_STATE_END,
                               CM_AC_END_REASON_FAIL_TIMER);
      }
      else
      {
        CM_MSG_HIGH_0("state_cb_func NULL");
      }

      /* Set CMAC state to 1X_DEREG_END, so that CM forces preferences to
      ** SD with preference update reason 1X_DEREG_END.
      */
      cmac_set_current_state(asubs_id,CMAC_STATE_1X_DEREG_END);
      CM_MSG_HIGH_0("Restor mode pref");
      cmph_delete_update_orig_mode(local_mm_id, cmac_get_act_id(asubs_id), TRUE);
      cmac_set_current_state(asubs_id, CMAC_STATE_NORMAL);
    }

    else if (ac_ptr->dereg_1x_timer <= uptime && ac_ptr->dereg_1x_timer != CMAC_UPTIME_NONE)
    {
      CM_MSG_HIGH_5("accessctrl_timer_proc as_id %d, reason %d, reason_mask %d, uptime %d, prev_state %d",
        asubs_id, ac_ptr->reason, ac_ptr->reason_mask, uptime, ac_ptr->state);

      cmac_restrict_subs_to_emergency(ac_ptr->reason, asubs_id);
      
      ac_ptr->dereg_1x_timer = CMAC_UPTIME_NONE;
    }
  }

} /* cmac_timer_proc() */

/*===========================================================================

FUNCTION cmac_client_cmd_proc

DESCRIPTION
  Process clients' commands

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmac_client_cmd_proc(

  cm_ac_cmd_s_type  *ac_cmd_ptr

)
{
  cm_ac_cmd_err_e_type    ac_cmd_err       = CM_AC_CMD_ERR_NOERR;

  cm_ac_cmd_info_s_type   *cmd_info_ptr    = NULL;
  cmac_s_type             *ac_ptr          = NULL;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Start by checking whether this is a valid command
  */
  ac_cmd_err = cmac_client_cmd_check (ac_cmd_ptr);

  cmac_client_cmd_err( ac_cmd_ptr, ac_cmd_err );
  /*
  ** If error found, Notify client and return.
  */
  if( ac_cmd_err != CM_AC_CMD_ERR_NOERR )
  {
    return;
  }

  if ((ac_ptr = cmac_ptr(ac_cmd_ptr->info.asubs_id)) == NULL)
  {
    CM_MSG_HIGH_1("ac_ptr null ptr sub %d", ac_cmd_ptr->info.asubs_id);
    return;
  }
  ac_ptr->cmd_cb_func    = ac_cmd_ptr->cmd_cb_func;
  ac_ptr->state_cb_func  = ac_cmd_ptr->state_cb_func;
  ac_ptr->data_block_ptr = ac_cmd_ptr->data_block_ptr;
  ac_ptr->cmd_type       = ac_cmd_ptr->cmd;

  cmd_info_ptr = CMD_INFO_PTR(ac_cmd_ptr);

  switch(ac_cmd_ptr->cmd)
  {
    case CM_AC_CMD_CLEAR_1XPPP:

      /* Notify client PPP clean is going to start.
       * Don't need end reason
       */
      if (ac_ptr->state_cb_func != NULL)
      {
        ac_ptr->state_cb_func( ac_cmd_ptr->data_block_ptr,
                               ac_ptr->cmd_type,
                               CM_AC_STATE_START,
                               CM_AC_END_REASON_NONE );
      }
      else
      {
        CM_MSG_HIGH_0("state_cb_func NULL");
      }

      cmac_clear_1xppp_context_proc(cmd_info_ptr);
      break;

    case CM_AC_CMD_NORMAL_OPERATION:
      cmac_restore_to_normal(ac_cmd_ptr->info.asubs_id, ac_cmd_ptr->info.reason);
      break;

    case CM_AC_CMD_EMERGENCY_ONLY:
      cmac_restrict_subs_to_emergency(ac_cmd_ptr->info.reason, ac_cmd_ptr->info.asubs_id);
      break;

    default:
      CM_MSG_HIGH_1("CM AC cmd %d not handled", ac_cmd_ptr->cmd);
  }
  return;
} /* cmac_client_cmd_proc() */

/*===========================================================================

FUNCTION cmac_call_cmd_check

DESCRIPTION
  Process clients' call commands

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cm_call_cmd_err_e_type cmac_call_cmd_check(
  cm_call_cmd_s_type       *call_cmd_ptr
)
{
  cmac_s_type             *ac_ptr;

  if(call_cmd_ptr == NULL) return CM_CALL_CMD_ERR_AC_REJECT;
  ac_ptr = cmac_ptr(call_cmd_ptr->cmd_subs);
  if(ac_ptr == NULL) return CM_CALL_CMD_ERR_AC_REJECT;

  CM_MSG_HIGH_2("cmac_call_cmd_check: AC state: %d, call_type %d", ac_ptr->state, call_cmd_ptr->info.call_type);


  /* If we're in normal state then, return CM_CALL_CMD_ERR_NOERR
  ** If we're not in normal state, then check if its emergency call.
  ** If yes, return CM_CALL_CMD_ERR_NOERR
  */
  if (ac_ptr->state == CMAC_STATE_NORMAL ||
      ac_ptr->state == CMAC_STATE_THERMAL_EMERG_LEVEL_2)
  {
    return CM_CALL_CMD_ERR_NOERR;
  }
  else
  {
    if (call_cmd_ptr->info.call_type == CM_CALL_TYPE_EMERGENCY)
    {
      mm_sub_stk_id_s_type local_mm_id;

      local_mm_id.asubs_id = call_cmd_ptr->cmd_subs;
      local_mm_id.stk_id = MM_STACK_0;

      // For E911 call, abort PPP clean. So we restore mode pref in CM and waiting
      // for 1x to send dereg failed with E911 reason.
      if(ac_ptr->state != CMAC_STATE_EMERGENCY_ONLY)
      {
        CM_MSG_HIGH_0("Restor mode pref");
        cmph_delete_update_orig_mode (local_mm_id, cmac_get_act_id(local_mm_id.asubs_id), TRUE);
      }

      return CM_CALL_CMD_ERR_NOERR;
    }
    else
    {
      return CM_CALL_CMD_ERR_AC_REJECT;
    }
  }
} /* cmac_call_cmd_check */

/*===========================================================================

FUNCTION cmac_ph_cmd_proc

DESCRIPTION
  Process clients' ph commands

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmac_ph_cmd_proc(

  const cm_ph_cmd_s_type     *ph_cmd_ptr

)
{
  const cm_ph_cmd_info_s_type  *cmd_info_ptr = CMD_INFO_PTR( ph_cmd_ptr );
  cmac_s_type *ac_cmd_ptr = cmac_ptr(cmd_info_ptr->cmd_subs);

  if(ac_cmd_ptr == NULL) return;

  if(ph_cmd_ptr->cmd == CM_PH_CMD_OPRT_MODE)
  {
    sys_modem_as_id_e_type local_subs;
    boolean flag = FALSE;
    CM_MSG_HIGH_1("ph_cmd oprt_mode %d",cmd_info_ptr->oprt_mode);
    switch(cmd_info_ptr->oprt_mode)
    {
      case SYS_OPRT_MODE_ONLINE:
        for(local_subs = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(local_subs); local_subs++)
        {
          cmac_s_type *ac_ptr = cmac_ptr(local_subs);
          if (ac_ptr == NULL)
          {
            continue;
          }

          if(ac_ptr->state == CMAC_STATE_EMERGENCY_PENDING)
          {
            cmac_set_current_state(local_subs, CMAC_STATE_NORMAL);
            flag = TRUE;
          }
        }
        if (flag)
        {
          cmac_restrict_all_subs_to_emergency(ac_cmd_ptr->reason);
        }

        break;

      case SYS_OPRT_MODE_LPM:
        for(local_subs = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(local_subs); local_subs++)
        {
          cmac_s_type *ac_ptr = cmac_ptr(local_subs);
          if (ac_ptr == NULL)
          {
            continue;
          }

          if(ac_ptr->state == CMAC_STATE_3GPP_DETACH_PENDING ||
              ac_ptr->state == CMAC_STATE_EMERGENCY_ONLY)
          {
            cmac_set_current_state(local_subs, CMAC_STATE_EMERGENCY_PENDING);
          }
        }

        break;

      default:
        break;
    }
  }
  else if(ph_cmd_ptr->cmd == CM_PH_CMD_SUBSCRIPTION_AVAILABLE ||
          ph_cmd_ptr->cmd == CM_PH_CMD_SUBSCRIPTION_NOT_AVAILABLE ||
          ph_cmd_ptr->cmd == CM_PH_CMD_SUBSCRIPTION_CHANGED)
  {
    CM_MSG_HIGH_1("ME_BLOCKED: cmd_subs %d",cmd_info_ptr->cmd_subs);
    if( !( cmac_is_in_thermal_emergency(cmd_info_ptr->cmd_subs) ||
           cmac_is_in_sim_lock(cmd_info_ptr->cmd_subs) ||
           cmac_is_me_blocked(cmd_info_ptr->cmd_subs) )
      )
    {
      CM_MSG_HIGH_2( "AC to normal due ph cmd %d cmac reason %d",
                     ph_cmd_ptr->cmd, ac_cmd_ptr->reason );
      cmac_restore_to_normal(cmd_info_ptr->cmd_subs, ac_cmd_ptr->reason );
    }
  }

} /* cmac_ph_cmd_proc */

/*===========================================================================

FUNCTION cmac_enter_thermal_emergency_level_2

DESCRIPTION
  Updates AC state to thermal emergency level 2
  Also ends any locally held calls
===========================================================================*/

void cmac_enter_thermal_emergency_level_2(void)
{
  sys_modem_as_id_e_type asubs_id;

  for ( asubs_id = SYS_MODEM_AS_ID_1;
        cmutil_is_as_id_valid(asubs_id);
        asubs_id++)
  {
    cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
    if (ac_ptr == NULL)
    {
      continue;
    }

    cmac_set_current_state(asubs_id,CMAC_STATE_THERMAL_EMERG_LEVEL_2);
    ac_ptr->reason = CM_AC_REASON_UNSAFE_TEMP;
  }

  cmcall_endall_non_emerg_calls_except_gw_ps(CM_CALL_END_THERMAL_EMERGENCY,TRUE);

}

/*===========================================================================

FUNCTION cmac_is_reason_mask_set

DESCRIPTION
  Checks if the reason_mask for AC state machine is set

RETURN VALUE
  FALSE: if reason_mask is NONE
  TRUE: otherwise
===========================================================================*/

boolean cmac_is_reason_mask_set(sys_modem_as_id_e_type asubs_id)
{
  cmac_s_type *ac_ptr = cmac_ptr(asubs_id);
  if(ac_ptr == NULL) return FALSE;
  return ( ac_ptr->reason_mask != 0);
}


