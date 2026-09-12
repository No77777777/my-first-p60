/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            C A L L   M A N A G E R   I P  C A L L

                      M O D U L E

GENERAL DESCRIPTION

  This module implements the functionality of handling calls through
  IP. IP call reports are received and call object is updated. Call
  Origination/end functions are implemented to call, registered
  applications that work using IP.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  cmipcall_init() must be call to initialize this module before any other
  function declared in this module is being called.


Copyright (c) 2005 - 2015 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmipcall.c#10 $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/14   xs      Add per call hold/resume support
05/29/14   xs      VOLTE call handling during L->D IRAT
04/23/14   sm      Wait for IMS call cleanup during user ending
09/25/13   xs      Hold VOPS indication when moving from VOLTE area to non-VOLTE area
08/14/13   sm      End IMS call in ORIG upon normal call release
08/08/13   sm      SKT req FR changes
07/03/13   dk      KW error fixed in cmipcall_client_cmd_forward_ll()
06/24/13   xs      VT answered as voice
06/18/13   gm      FR 2617 - OMADM IMS and ADMIN PDN Disable Requirement
06/05/13   qf      Added support for IMS rat_change between LTE and WLAN
05/08/13   sm      Removed check for ss during mng_state_update
04/08/13   xs      Notify NAS of hold call end after srvcc
01/15/13   xs      Added video share support
01/04/13   th      Support error code CM_CALL_END_UPGRADE_DOWNGRADE_CANCELLED and
                   return error code type to clients.
01/04/13   th      Mapping new end cause CMIPAPP_END_CAUSE_RETRY_AFTER
11/02/12   xs      Fix VOLTE call alllow with modification issue
10/18/12   ak/xs   Fix not sending MNG_CNF issue
08/21/12   xs      Add debug msg
08/21/12   xs      Fix VT call waiting issue
07/31/12   xs      Send MNG_CNF without considering the report order
07/31/12   xs      Fix issue of VT call on hold
05/05/12   xs      Support sending 486 busy here end cause to client
04/25/12   gm      Refactor IPAPP callback interface
03/27/12   xs      Add call attribute info in connected_ind
03/27/12   xs      VoLTE Remove of user from Conf-call
03/08/12   xs      Added codec info support for VOLTE
02/13/12   xs      VT upgrade/downgrade support
02/07/12   xs      VOLTE silent redial support
01/11/12   xs      Send remote party retrieve event without checking call status
01/06/12   gm      Data synchronization changes for freefloating platform
01/05/12   xs      Added support of remote party hold notification
01/04/12   xs      Add new IMS call end error codes
12/21/11   xs      L->W SRVCC support
11/18/11   xs      Need to set VOLTE call flag for MT call as well
10/10/11   xs      Fix CM_CALL_SUPS_TYPE_HELD_CALL_REL_OR_UDUB reject issue
10/05/11   xs      Fix 2 sups call issue
09/23/11   xs      Change data block sending mechanism
09/19/11   xs      Fix invalid data block sent in call orig.
08/25/11   xs      Add user data block support for some APIs
08/05/11   xs      VOLTE phase 1 support
07/24/11   aj      SC SVLTE 1.0 merge to mainline
06/29/20   jqi     VoIP early media support
04/11/11   am      Cleaning stubs and runnning qtf SUTE
03/22/11   gm      Fix to waiting 4s after call end to force THERM EMERG
12/02/10   gm      Fix to compiler warning- naming conflicts with VC
11/02/10   am      Replacing FEATURE_MOB_MMODE_CUTE
09/23/10   xs      Superlint critical/high error fixed
08/17/10   gm      Fix to KW Error
06/10/10   ak      Mainlining memory optimization changes.
05/26/10   jqi     EHRPD VOIP silent redial support.
01/26/10   np      Added LTE IP Call sys mode
01/19/10   aj      Added VOIP support for emergency call
12/23/09   sg      Deallocate the memory after processing call info.
11/05/09   mp      Added support to remove the last party added for VOIP calls
10/14/09   sg      ZI memory reduction. Mmode should call new Heap Manager API to
                   allocate memory dynamically.
09/01/09   sv      Moving from clk API to time API
04/23/09   aj      Prevent referencing deallocated memory
12/03/08   cl      Added support for CM_CALL_SUPS_TYPE_ACTIVE_CALL_REL
10/30/08   jd      Added support for IMS registration
10/27/08   cl      Added support for KTFT Voip Call Waiting Feature
08/20/08   st      Klockwork Fixes
06/12/08   sv      Resolve Klocwork defects
01/25/08   vs      Added 0-send call reject support to VOIP call.
12/07/07   ra      Added support for IMS VCC in HOMER files
08/07/07   ic      Lint clean up
06/22/07   sk      Added support for ATOM.
06/11/07   pk      VoIP -> 1x hho enahncements / CR fixes
03/15/07   ka      Adding INFO_IP to CM_CALL_EVENT_MNG_CALLS_CONF.
01/03/07   pk      Added support for DO VOIP -> 1X Handoff
01/09/07   ka      Replacing call to cmipcall_end with cmcall_end to make
                   sure GW connection id is unreserved.
11/09/06   pk      Added support for HYBR_GW and UMTS -> 1X handover cont...
11/07/06   ka      Removing the check in Hold Call for call id passed to be
                   in CONV state.
10/03/06   ka      Adding support for IP call on WCDMA
09/18/06   ka      Moving data_block copy to within switch case block.
                   Primarily done for MT_INVITE but changed others to make
                   it uniform.
08/08/06   sk      Mapped all VoIP errors to CM_CALL_END_IP_FAIL.
                   Removed unnecessary call end event when processing IP call
                   end reports.
07/31/06   ka      Adding Explicit call transfer and Multiparty support for
                   VoIP.
07/14/06   ka      Fixing the issue where call cmd cb for call end api was
                   being called more than once.
06/27/06   ka      Adding feature flag FEATURE_IP_CALL
05/10/06   ka      mode_spec_end used to bunch calls of particular info_type
                   and then send them to wcall, ipcall or xcall. Modifying
                   wcall, ipcall and xcall to end calls of their typeonly.
05/07/06   ka      Adding support for activating/deactivating answer hold on
                   VoIP calls.
02/17/06   ka      Adding support for videoshare
01/12/06   ic      For specific calls / walks Lint warnings, replaced global
                   -fsp comment with -e{error#} comments where needed
01/06/06   ic      Lint cleanup
01/05/06   ka      Adding support for multi-call hold/activate.
12/02/05   ka      Fixing compilation warnings.
11/23/05   ka      Modification to allow hold/active for ip calls.
11/16/05   ka      User used to be alerted at the receipt of
                   CM_CALL_EVENT_SETUP_IND, this behavior is changed to wait
                   for seperate ringing indication from network to alert user.
11/08/05   sk      Separated CM reports and commands.
11/03/05   ic      Added CM_ASSERT() for ptr function parameters and lint
                   comments for 613 Lint error
10/21/05   sk      Removed all unnecessary checks on digit_mode.
10/08/05   ic      Added Header: field
08/16/05   pk      Added support for priority Queues for origination
                   parameters
08/16/05   ka      Added mapping for SYS_SYS_MODE_HDR to CMIPAPP enum.
08/02/05   ic      Print value that caused error in default switch case
04/11/05   ka      Changing return type for function
                   cmipcall_map_sys_mode_to_ip_sys_mode ()
04/06/05   ka      Passing system mode to call origination.
03/28/05   ka      Fixed Lint errors.
03/15/05   ka      Fixing RVCT compiler warning, conversion from CM call type
                   to IP call type
03/14/05   ka      cleaning up IP rpt and adding handling call cmd for IP
                   in forward_ll, reply_ll and cmd_check functions.
02/09/05   ic      Lint cleanup
02/03/05   ka      Lint fixes
02/02/05   dk/ka   Initial release
===========================================================================*/

/**--------------------------------------------------------------------------
** Includes
** --------------------------------------------------------------------------
*/
/*lint -esym(766,customer.h) */
#include "mmcp_variation.h"
#include "customer.h"  /* Customer configuration file */

#if defined(FEATURE_IP_CALL)

#include "cmipappi.h"   /* Includes definitions for VoIP app */
#include "cm.h"        /* External interface to cm.c */
#include "cmkpi.h"
#include "cm_v.h"
#include "cmi.h"       /* Internal interface to cm.c */
#include "cmdbg.h"     /* CM debug macros */
#include "cmcall.h"    /* Interface to CM call object */
#include "cmclient.h"  /* Interface to CM Client Interface Block */
#include "cmipcall.h"
#include "cmph.h"      /* Includes phone object */
#include "cmss.h"      /* Includes SS object */
#include "time_svc.h"
#include "cmtaski.h"
#include "cmxcall.h"
#include "cmcsfbcall.h"
#ifdef CM_GW_SUPPORTED
#include "cmwcall.h"
#endif
#include "cmll.h"
#ifdef CM_DEBUG
#error code not present
#endif
#include "cmsds.h"
#include "cmmsc.h"
#include "cmsimcoord.h"
#include "cmaccessctrl.h"
#include "cmdrvcc.h"
#include "sd_v.h"
#include "cmlog_v.h"
#include "cmsups.h"

typedef struct
{
  cm_call_sups_type_e_type ss_cmd;
  /* sups command type */
  cmipcall_sups_state_e_type cmipcall_mng_call_ids[CMCALL_MAX_CALL_OBJ];
  /* each call's state regarding the sups service */
} cmipcall_mng_call_s_type;

static cmipcall_mng_call_s_type cmipcall_mng_call_cnf;


/**--------------------------------------------------------------------------
** Forward declarations
** --------------------------------------------------------------------------
*/
static void cmipcall_send_mng_cnf(boolean call_ss_status, cm_ipapp_cause_e_type failure_cause,cm_call_id_type call_id);

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/**--------------------------------------------------------------------------
** Defines
** --------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
                         TUNABLE CALL PARAMETERS
---------------------------------------------------------------------------*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**--------------------------------------------------------------------------
** Datatypes
** --------------------------------------------------------------------------
*/


/**--------------------------------------------------------------------------
** Functions - internal:
** --------------------------------------------------------------------------
*/

static void cmipcall_mt_invite_update_call_composer(
  cmcall_s_type                    *call_ptr,
  const cm_ip_call_composer_info_s_type  *call_composer_info
);

#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE)

/*=========================================================================

FUNCTION
cmipcall_crit_sect_ptr

DESCRIPTION

Critical section to protect shared global data related to CM IP CALL.

DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static rex_crit_sect_type *cmipcall_crit_sect_ptr(void)
{
  /* For now we use the same lock, once have lock sequencing framework
  ** we can have separate lock for CMIP CALL */
  return cmtask_crit_sect_ptr();
}

/*=========================================================================

FUNCTION
cmipcall_set_mt_cs_reject_timer

DESCRIPTION

Start/Reset timer for rejection of MT CS Page during MT VoLTE call setup.

DEPENDENCIES
none

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmipcall_set_mt_cs_reject_timer( dword uptime,sys_modem_as_id_e_type asubs_id )
{
  cmph_sub_info_s_type *ph_sub_config_ptr = cmph_get_sub_ptr(asubs_id);

  if(ph_sub_config_ptr != NULL)
  {
    /* For now we use the same lock, once have lock sequencing framework
        ** we can have separate lock for CMIP CALL */
    CM_MSG_HIGH_2("set_mt_cs_reject_timer[%d] :%d",asubs_id,uptime);
    rex_enter_crit_sect(cmipcall_crit_sect_ptr());
    ph_sub_config_ptr->mt_cs_page_reject_uptime = uptime;
    rex_leave_crit_sect(cmipcall_crit_sect_ptr());
  }
}
#endif

/*===========================================================================

FUNCTION cmipcall_get_sr_backup_normal_call

DESCRIPTION
  Return SR backup for normal call taken before
  converting it to e911 call

DEPENDENCIES
  none

RETURN VALUE
  SR component backup

SIDE EFFECTS
  none

===========================================================================*/
static cm_sr_s_type* cmipcall_get_sr_backup_normal_call()
{  
  static cm_sr_s_type sr_backup_normal_call;

  return &sr_backup_normal_call;
}

/*===========================================================================

FUNCTION cmipcall_update_mng_state_per_call

DESCRIPTION
  Update mng_cnf state per call_id

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmipcall_update_mng_state_per_call(cm_call_id_type call_id,cmipcall_sups_state_e_type sup_state)
{
  CM_MSG_MED_2("mng state update, call=%d, state=%d", call_id, sup_state);
  cmipcall_mng_call_cnf.cmipcall_mng_call_ids[call_id] = sup_state;
  return;
}
#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
/*===========================================================================

FUNCTION cm_avoid_list_init

DESCRIPTION
  Initialize PLMN list to avoid.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void cm_avoid_list_init( sys_modem_as_id_e_type asubs_id)
{
  cmph_sub_info_s_type *ph_sub_info_ptr = cmph_get_sub_ptr(asubs_id);
  if(NULL == ph_sub_info_ptr)
  {
    CM_MSG_HIGH_0("ph_sub_info_ptr is NULL!");
	return;
  }
  if(ph_sub_info_ptr->avoid_plmn_list_info.is_initialized)
  {
    CM_MSG_HIGH_0("Avoid plmn list already initialized.");
    return;
  }

  ph_sub_info_ptr->avoid_plmn_list_info.is_initialized = TRUE;

  memset(&(ph_sub_info_ptr->avoid_plmn_list_info.plmn),0xFF,sizeof(sys_plmn_id_s_type)*SYS_AVOID_PLMN_LIST_MAX_LENGTH);
  CM_MSG_HIGH_0("Initializing avoid plmn list - success");

} /* cm_avoid_list_init() */
#endif
/*===========================================================================

FUNCTION cmipcall_end_cause_to_cmcall_end_status

DESCRIPTION
  Map app end status to CM call end enum.

DEPENDENCIES
  None

RETURN VALUE
  CM call end status that corresponds to App end status

SIDE EFFECTS
  none

===========================================================================*/
static cm_call_end_e_type cmipcall_end_cause_to_cmcall_end_status(
  cmipapp_end_cause_e_type  end_cause,
  /* ipcall cause that needs to be mapped to CM end cause */

  cm_call_type_e_type       call_type,

  mm_sub_stk_id_s_type               mm_id
)
{
  cm_call_end_e_type    call_end = CM_CALL_END_REL_NORMAL;
  cmph_sub_info_s_type *ph_sub_config_ptr = cmph_get_sub_ptr(mm_id.asubs_id);

  if(ph_sub_config_ptr == NULL)
  {
    CM_MSG_HIGH_1("cmph sub config pointer[%d] is NULL",mm_id.asubs_id);
    return CM_CALL_END_NONE;
  }

  switch (end_cause)
  {
    case CMIPAPP_END_CAUSE_NORMAL:

      call_end = CM_CALL_END_REL_NORMAL;
      break;

    case CMIPAPP_END_CAUSE_TEMPORARY_FAILURE:

      call_end = CM_CALL_END_IP_RETRY;
      break;

    case CMIPAPP_END_CAUSE_FALLBACK_TO_CS:
      /* these changes needs to be removed once
      IMS is ready with the changes of sending CMIPAPP_END_CAUSE_HARD_FAILURE
      instead of fallback_to_cs */
      if(call_type == CM_CALL_TYPE_EMERGENCY &&
          (ph_sub_config_ptr->emerg_call_prop.is_alternate_redial_algorithm || mm_id.stk_id == MM_STACK_2))
      {
        call_end = CM_CALL_END_IP_HARD_FAILURE;
      }
      else
      {
        /* for rest of the cases,
                  voice calls and E911 when alternate redial is OFF
                  use behavior similar to FALLBACK_TO_CS */
        call_end = CM_CALL_END_IP_FAIL;
      }
      break;

    case CMIPAPP_END_CAUSE_HARD_FAILURE:
      if(call_type == CM_CALL_TYPE_EMERGENCY &&
          ph_sub_config_ptr->emerg_call_prop.is_alternate_redial_algorithm)
      {
        call_end = CM_CALL_END_IP_HARD_FAILURE;
      }
      else
      {
        /* for rest of the cases,
                  voice calls and E911 when alternate redial is OFF
                  use behavior similar to FALLBACK_TO_CS */
        call_end = CM_CALL_END_IP_FAIL;
      }
      break;

    case CMIPAPP_END_CAUSE_PERMANENT_FAILURE:

      call_end = CM_CALL_END_IP_END;
      break;

    case CMIPAPP_END_CAUSE_RETRY_AFTER:

      call_end = CM_CALL_END_IP_RETRY_AFTER_FAIL;
      break;

    case CMIPAPP_END_CAUSE_IP_ORIGINATION_FAIL:

      call_end = CM_CALL_END_IP_FAIL;
      break;

    case CMIPAPP_END_CAUSE_BUSY_HERE:

      call_end = CM_CALL_END_SIP_486_BUSY_HERE;
      break;

    case CMIPAPP_END_CAUSE_SERVER_INTERNAL_ERROR:
    case CMIPAPP_END_CAUSE_REQUEST_TIMEOUT:

      call_end = CM_CALL_END_NETWORK_NO_RESP_TIME_OUT;
      break;

    case CMIPAPP_END_CAUSE_NOT_FOUND:

      call_end = CM_CALL_END_INVALID_REMOTE_URI;
      break;

    case CMIPAPP_END_CAUSE_DECLINE:

      call_end = CM_CALL_END_IP_USER_REJECT;
      break;

    case CMIPAPP_END_CAUSE_TEMPORARILY_UNAVAILABLE:
      call_end = CM_CALL_END_IP_LOW_BATTERY;
      break;

    case CMIPAPP_END_CAUSE_SRVCC_LTE_TO_CS:

      call_end = CM_CALL_END_IP_UNSUPPORTED_USER_TERMINAL;
      break;

    case CMIPAPP_END_CAUSE_SERVER_UNAVAILABLE:
      call_end = CM_CALL_END_SIP_503_SERVER_UNAVAILABLE;
      break;
    case CMIPAPP_END_CAUSE_FORBIDDEN:

      call_end = CM_CALL_END_SIP_403_FORBIDDEN;
      break;

    case CMIPAPP_END_CAUSE_ANSWERED_ELSEWHERE:
      call_end = CM_CALL_END_SIP_200_ANSWERED_ELSEWHERE;
      break;
    case CMIPAPP_END_CAUSE_BUSY_EVERYWHERE:
      call_end = CM_CALL_END_SIP_600_BUSY_EVERYWHERE;
      break;

    case CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL:
      call_end = CM_CALL_END_IP_FAIL;
      break;

    case CMIPAPP_END_CAUSE_HOLD_RESUME_FAILED:
      call_end = CM_CALL_END_IP_HOLD_RESUME_FAILED;
      break;

    case CMIPAPP_END_CAUSE_HOLD_RESUME_CANCELED:
      call_end = CM_CALL_END_IP_HOLD_RESUME_CANCELED;
      break;

    case CMIPAPP_END_CAUSE_REINVITE_COLLISION:
      call_end = CM_CALL_END_IP_REINVITE_COLLISION;
      break;

    case CMIPAPP_END_CAUSE_UNWANTED_CALL:
      call_end = CM_CALL_END_IP_UNWANTED_CALL;	 
      break;


    default:
      call_end = CM_CALL_END_IP_FAIL;
      break;
  }

  CM_MSG_HIGH_2("VOIP SR ipapp end cause %d call end %d", end_cause, call_end );

  return call_end;

} /* cmipcall_end_cause_to_cmcall_end_status () */


/*===========================================================================

FUNCTION cmipcall_should_call_redial_over_ip()

DESCRIPTION
  Checks if call will get redialed over IP

DEPENDENCIES
  None

RETURN VALUE
  TRUE if call will be redialed over IP

SIDE EFFECTS
  none

===========================================================================*/

boolean cmipcall_should_call_redial_over_ip
(
  cmcall_s_type   *call_ptr,
  cmipapp_end_cause_e_type ims_end_cause
)
{
  boolean ret_val = FALSE;

  switch(ims_end_cause)
  {
    case CMIPAPP_END_CAUSE_TEMPORARY_FAILURE:
    case CMIPAPP_END_CAUSE_LOOP_BACK:
    {
      ret_val = TRUE;
    }
    break;
    default:
    {
      if(cmipcall_end_cause_to_cmcall_end_status(
          ims_end_cause,
          call_ptr->call_type,
          call_ptr->mm_id) == CM_CALL_END_IP_FAIL &&
          cmcall_is_volte_restricted_call(call_ptr))
      {
        ret_val = TRUE;
      }
    }
    break;
  } /* ims_end_cause */

  return ret_val;
} /* cmipcall_should_call_redial_over_ip ()*/


/*===========================================================================

FUNCTION cmcall_end_cause_to_cmipcall_rej_status

DESCRIPTION
  Map CM call end enum to app incoming rej status

DEPENDENCIES
  None

RETURN VALUE
  APP end status

SIDE EFFECTS
  none

===========================================================================*/
static cmipapp_incom_rej_cause_e_type cmcall_end_cause_to_cmipcall_rej_status
(
  cm_call_end_e_type  end_cause
  /* CM end cause to be mapped to ipcall incoming rej */

)
{
  cmipapp_incom_rej_cause_e_type    incom_rej = CMIPAPP_INCOM_REJ_CAUSE_NONE;

  switch (end_cause)
  {
    case CM_CALL_END_SIP_486_BUSY_HERE:
      incom_rej = CMIPAPP_INCOM_REJ_CAUSE_BUSY;
      break;

    case CM_CALL_END_IP_USER_REJECT:
      incom_rej = CMIPAPP_INCOM_REJ_CAUSE_USER_REJECT;
      break;

    case CM_CALL_END_IP_LOW_BATTERY:
      incom_rej = CMIPAPP_INCOM_REJ_CAUSE_LOW_BATTERY;
      break;

    case CM_CALL_END_BLACKLISTED_CALL_ID:
      incom_rej = CMIPAPP_INCOM_REJ_CAUSE_BLACKLISTED_CALL_ID;
      break;

    case CM_CALL_END_IP_UNWANTED_CALL:
      incom_rej = CMIPAPP_INCOM_REJ_CAUSE_UNWANTED_CALL;
      break;


    default:
      incom_rej = CMIPAPP_INCOM_REJ_CAUSE_UNAVAILABLE;
      break;

  }

  CM_MSG_HIGH_2("VOIP SR ipapp rej cause %d client call end %d", incom_rej,
                end_cause );

  return incom_rej;

} /* cmcall_end_cause_to_cmipcall_rej_status () */


/*===========================================================================

FUNCTION cmipcall_map_client_end_status

DESCRIPTION
  Map app end status to CM call end enum.

DEPENDENCIES
  None

RETURN VALUE
  CM call end status that corresponds to App end status

SIDE EFFECTS
  none

===========================================================================*/
static cm_call_end_e_type cmipcall_map_client_end_status
(
  cmipapp_client_end_cause_e_type  end_cause
  /* ipcall cause that needs to be mapped to CM end cause */

)
{
  cm_call_end_e_type    call_end = CM_CALL_END_NONE;

  call_end = (cm_call_end_e_type)end_cause;
  if(!INRANGE(call_end,CM_CALL_END_NONE, (CM_CALL_END_MAX - 1)))
  {
    call_end = CM_CALL_END_NONE;
  }

  return call_end;

} /* cmipcall_end_cause_to_cmcall_end_status () */


/*===========================================================================

FUNCTION cmipcall_map_modify_cause_code

DESCRIPTION
  Map IP APP modify cause code to CM Modify cause code.

DEPENDENCIES
  None

RETURN VALUE
  CM modify cause code

SIDE EFFECTS
  none

===========================================================================*/
static cm_call_modify_cause_e_type cmipcall_map_modify_cause_code
(
  cmipapp_client_modify_cause_e_type  modify_cause_code
  /* ipcall cause that needs to be mapped to CM modify cause */
)
{
  cm_call_modify_cause_e_type    cm_modify_cause_code = CM_CALL_MODIFY_CAUSE_NONE;

  cm_modify_cause_code = (cm_call_modify_cause_e_type) modify_cause_code;
  if(!INRANGE(cm_modify_cause_code,CM_CALL_MODIFY_CAUSE_NONE, (CM_CALL_MODIFY_CAUSE_MAX - 1)))
  {
    cm_modify_cause_code = CM_CALL_MODIFY_CAUSE_NONE;
  }

  CM_MSG_HIGH_2("VOIP client modify_cause_code %d, cm_modify_cause_code %d",
                modify_cause_code, cm_modify_cause_code);

  return cm_modify_cause_code;
} /* cmipcall_map_modify_cause_code () */


/*===========================================================================

FUNCTION cmipcall_map_calltype_to_cm_calltype

DESCRIPTION
  Process app reports (i.e. notifications of call activity or
  replies to clients call commands).

DEPENDENCIES
  None.

RETURN VALUE
  CM call type that corresponds to app call type

SIDE EFFECTS
  none

===========================================================================*/
static cm_call_type_e_type cmipcall_map_calltype_to_cm_calltype(

  cmipapp_call_type_e_type      call_type
  /* call type */
)
{
  switch (call_type)
  {
    case CMIPAPP_CALL_TYPE_VOICE:
      return CM_CALL_TYPE_VOICE;

    case CMIPAPP_CALL_TYPE_VT:
      return CM_CALL_TYPE_VT;

    case CMIPAPP_CALL_TYPE_VS:
      return CM_CALL_TYPE_VS;

    case CMIPAPP_CALL_TYPE_SMS:
      return CM_CALL_TYPE_SMS;

    case CMIPAPP_CALL_TYPE_EMERGENCY:
      return CM_CALL_TYPE_EMERGENCY;

    default:
      CM_ERR_1( "CMIPAPP call type %d not mapped to CM call type !!!",call_type);
      CM_ASSERT(FALSE);
      return CM_CALL_TYPE_MAX;
  }

} /* cmipcall_map_calltype_to_cm_calltype () */


/*===========================================================================

FUNCTION cmipcall_map_cm_calltype_to_ip_calltype

DESCRIPTION
  Maps CM call type to CM app call type

DEPENDENCIES
  None.

RETURN VALUE
  app call type that corresponds to CM call type

SIDE EFFECTS
  none

===========================================================================*/
static cmipapp_call_type_e_type cmipcall_map_cm_calltype_to_ip_calltype(

  cm_call_type_e_type      call_type
  /* call type */
)
{
  switch (call_type)
  {
    case CM_CALL_TYPE_VOICE:
      return CMIPAPP_CALL_TYPE_VOICE;

    case CM_CALL_TYPE_EMERGENCY:
      return CMIPAPP_CALL_TYPE_EMERGENCY;

    case CM_CALL_TYPE_VT:
      return CMIPAPP_CALL_TYPE_VT;

    case CM_CALL_TYPE_VS:
      return CMIPAPP_CALL_TYPE_VS;

    case CM_CALL_TYPE_SMS:
      return CMIPAPP_CALL_TYPE_SMS;

    default:
      CM_ERR_1( "CM call type %d not mapped to CMIPAPP call type !!!", call_type);
      CM_ASSERT(FALSE);
      return CMIPAPP_CALL_TYPE_MAX; /*lint !e641 */
  }

} /* cmipcall_map_cm_calltype_to_ip_calltype () */


/*===========================================================================

FUNCTION cmipcall_map_sys_mode_to_ip_sys_mode

DESCRIPTION
  Maps system mode to IP system mode

DEPENDENCIES
  None.

RETURN VALUE
  sys mode that corresponds to ip system mode

SIDE EFFECTS
  none

===========================================================================*/
static cmipapp_sys_mode_e_type cmipcall_map_sys_mode_to_ip_sys_mode(

  sys_sys_mode_e_type  sys_mode
  /* call type */
)
{
  CM_ASSERT (BETWEEN(sys_mode, SYS_SYS_MODE_NONE, SYS_SYS_MODE_MAX));

  switch (sys_mode)
  {
    case SYS_SYS_MODE_NO_SRV:
      return CMIPAPP_SYS_MODE_NO_SRV;

    case SYS_SYS_MODE_CDMA:
      return CMIPAPP_SYS_MODE_CDMA;

    case SYS_SYS_MODE_GSM:
      return CMIPAPP_SYS_MODE_GSM;

    case SYS_SYS_MODE_WCDMA:
      return CMIPAPP_SYS_MODE_WCDMA;

    case SYS_SYS_MODE_HDR:
      return CMIPAPP_SYS_MODE_HDR;

    case SYS_SYS_MODE_LTE:
      return CMIPAPP_SYS_MODE_LTE;

    case SYS_SYS_MODE_WLAN:
      return CMIPAPP_SYS_MODE_WLAN;

    default:
      CM_ERR_1( "sys mode %d not supported in IP call !!!", sys_mode);
      CM_ASSERT(FALSE);
      return CMIPAPP_SYS_MODE_MAX;
  }

} /* cmipcall_map_sys_mode_to_ip_sys_mode () */


/*===========================================================================

FUNCTION cmipcall_reoriginate_call_as_emergency

DESCRIPTION
  Change ordinary VOLTE call into an emergency call

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_reoriginate_call_as_emergency
(
  cmcall_s_type           *call_ptr,
  /* call object
  */
  boolean fallback_to_cs
  /* if IMS has already send fallback to CS for this */
)
{
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);

  cmph_s_type              *ph_ptr            = cmph_ptr();
    /* Point at phone object */

  if (ph_sub_ptr == NULL)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* update call object - call type , srv type*/
  call_ptr->call_type = CM_CALL_TYPE_EMERGENCY;
  call_ptr->srv_type = CM_SRV_TYPE_AUTOMATIC;
  call_ptr->calling_num.pi = CM_PRESENTATION_ALLOWED;
  call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.is_ip_pi_valid = TRUE;

  /* update the volte indicator*/
  call_ptr->is_volte = FALSE;
  call_ptr->is_vowlan = FALSE;
  call_ptr->cmcall_mode_info.info_type = CM_CALL_MODE_INFO_NONE;

  /* Take backup of SR component for normal call before converting to emergency call */
  *cmipcall_get_sr_backup_normal_call() = call_ptr->sr;
  
  /* Reset the silent redial component.
  */
  if(ph_sub_ptr->emerg_call_prop.is_wlan_e911_call_supp)
  {
    dword curr_wlan_fallback_timer = call_ptr->sr.wlan_fallback_timer;
    cm_emerg_call_attempt_state_e_type curr_emerg_call_attempt_state = call_ptr->sr.emerg_call_attempt_state;
    sr_reset( &call_ptr->sr );
    call_ptr->sr.wlan_fallback_timer = curr_wlan_fallback_timer;
    call_ptr->sr.emerg_call_attempt_state = curr_emerg_call_attempt_state;
  }
  else
  {
    sr_reset(&call_ptr->sr);
  }

  /* update favored mode/hybr prefs for call*/
  cmcall_set_favored_mode_pref_for_origination (
    cmph_map_sub_stk_to_id(call_ptr->mm_id.asubs_id,MM_STACK_ALL ), // this param is not used for non DSDS
    call_ptr->call_type,
    call_ptr->srv_type,
    TRUE, //drs bit - not used
#ifdef FEATURE_IS2000
    call_ptr->cmcall_mode_info.info.cdma_info.srv_opt,//not used,
#else
    TRUE,
#endif
    &call_ptr->favored_mode_pref,
    &call_ptr->favored_hybr_pref );
  /* Find the SS to use for call origination */
  call_ptr->mm_id.stk_id = cmcall_determine_stk_for_origination(
                             call_ptr->favored_mode_pref,
                             call_ptr->favored_hybr_pref,
                             CM_CC_MODE_PREF_ANY,
                             call_ptr );
  CM_MSG_HIGH_4( "Using sub=%d stack=%d for emerg fav mode_pref %d,fav hybr_pref =%d",
                 call_ptr->mm_id.asubs_id,
                 call_ptr->mm_id.stk_id,
                 call_ptr->favored_mode_pref,
                 call_ptr->favored_hybr_pref );
  /*
  ** Force top pref is set to TRUE, if the current call is possible over IP
  ** If there is no ipapp (on HDR )for voice origination, remove HDR
  ** component from favored mode preference.
  ** Both these steps are not needed for emergency call over EHRPD case.
  ** Hybr pref will not be OFF during 911 call only for emerg call over ehrpd case.
  */
  if(! ( call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
         call_ptr->favored_hybr_pref != CM_HYBR_PREF_NONE)
    )
  {
    cmcall_process_ipcall_params (call_ptr);
  }
  
  /* Run domain selection to determine CSFB type for the E911 call 
  */
#if defined( FEATURE_3GPP_CSFB) || defined (FEATURE_LTE_TO_1X)  || \
      defined (FEATURE_DOMAIN_SELECTION)
  cmsds_emergency_call_dom_selection(&call_ptr->csfb_type, call_ptr->mm_id.asubs_id);
#endif

#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
  if(ph_sub_ptr->emerg_call_prop.wifi_e911_support && !fallback_to_cs)
  {
    cmcall_process_custom_wifi_settings(call_ptr);
  }
#endif

  /* update orig mode */
  (void)cmcall_process_orig_mode(CM_CC_MODE_PREF_ANY,call_ptr);

#if defined (FEATURE_CM_LTE) && defined(FEATURE_IP_CALL)
  /* End existing VOLTE call if any */
  cmcall_end_concurrent_volte_call(call_ptr->mm_id);
#endif

  /* Update Priority Queue with computed new preferences 
   ** if Priority Queue object corresponding to call_id is already present
   ** This might not get updated in cmcall_force_pref_on_fly 
   */
  if(BETWEEN(call_ptr->mm_id.stk_id, MM_STACK_NONE, MM_STACK_ALL))
  {
    cm_orig_q_s_type *computed_orig_para =  cmtask_orig_para_search_act_id  (
                                                         call_ptr->mm_id,
                                         (cm_act_id_type)call_ptr->call_id );

    if( computed_orig_para != NULL &&
        computed_orig_para->orig->orig_mode != SD_SS_ORIG_MODE_NONE )
    {

      /* Get the favored band preference to be used for this call-type and
           ** service-type.
           */
      computed_orig_para->orig->orig_mode_pref = call_ptr->favored_mode_pref;
      computed_orig_para->orig->orig_roam_pref = CM_ROAM_PREF_ANY;
      computed_orig_para->orig->orig_band_pref = cmcall_get_favored_band_pref( call_ptr->call_type,
                                                        call_ptr->srv_type,
                                                        CMPH_SS_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr)
                                                      );
    
      computed_orig_para->orig->orig_lte_band_pref = cmcall_get_favored_lte_band_pref(
                                                    call_ptr->call_type,
                                                    CMPH_SS_LTE_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr)
                                                    );
    
      computed_orig_para->orig->orig_tds_band_pref = cmcall_get_favored_tds_band_pref(
                                                    call_ptr->call_type,
                                                    CMPH_SS_TDS_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr)
                                                    );
    
    }

  }

  /* Update priority Q and send call orig event */
  cmcall_orig_start (call_ptr);
  return;

} /* cmipcall_reoriginate_call_as_emergency() */


/*===========================================================================

FUNCTION cmipcall_convert_emergency_to_voice

DESCRIPTION
  Change emergency call to ordinary VOLTE call

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_convert_emergency_to_voice
(
  cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);

  cmph_s_type              *ph_ptr            = cmph_ptr();
    /* Point at phone object */

  if (ph_sub_ptr == NULL)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Set call_type to original call type*/
  call_ptr->call_type = call_ptr->original_call_type;

  /* Restore backed up SR component */
  call_ptr->sr = *cmipcall_get_sr_backup_normal_call();

  /* update orig mdoe to normal for non emerg calls , it will be updated further
  ** after csfb support is concluded in force_non_ip */
  call_ptr->orig_mode = SD_SS_ORIG_MODE_NORMAL;
  
  /* update favored mode/hybr prefs for call*/
  cmcall_set_favored_mode_pref_for_origination (
    cmph_map_sub_stk_to_id(call_ptr->mm_id.asubs_id,MM_STACK_ALL ), // this param is not used for non DSDS
    call_ptr->call_type,
    call_ptr->srv_type,
    TRUE, //drs bit - not used
    #ifdef FEATURE_IS2000
    call_ptr->cmcall_mode_info.info.cdma_info.srv_opt,//not used,
    #else
    TRUE,
    #endif
    &call_ptr->favored_mode_pref,
    &call_ptr->favored_hybr_pref );

  /* Find the SS to use for call origination */
  call_ptr->mm_id.stk_id = cmcall_determine_stk_for_origination(
                             call_ptr->favored_mode_pref,
                             call_ptr->favored_hybr_pref,
                             CM_CC_MODE_PREF_MAX,
                             call_ptr );
  
  CM_MSG_HIGH_4( "Using sub=%d stack=%d for converted voice fav_mode_pref 0x%x,fav hybr_pref =%d",
                 call_ptr->mm_id.asubs_id,
                 call_ptr->mm_id.stk_id,
                 call_ptr->favored_mode_pref,
                 call_ptr->favored_hybr_pref );

  /* Update Priority Queue with computed new preferences 
   ** if Priority Queue object corresponding to call_id is already present
   ** This might not get updated in cmcall_force_pref_on_fly 
   */
  if(BETWEEN(call_ptr->mm_id.stk_id, MM_STACK_NONE, MM_STACK_ALL))
  {
    cm_orig_q_s_type *computed_orig_para =  cmtask_orig_para_search_act_id  (
                                                         call_ptr->mm_id,
                                         (cm_act_id_type)call_ptr->call_id );

    if( computed_orig_para != NULL &&
        computed_orig_para->orig->orig_mode != SD_SS_ORIG_MODE_NONE )
    {

      /* Get the favored band preference to be used for this call-type and
           ** service-type.
           */
      computed_orig_para->orig->orig_mode_pref = call_ptr->favored_mode_pref;
      computed_orig_para->orig->orig_band_pref = cmcall_get_favored_band_pref( call_ptr->call_type,
                                                        call_ptr->srv_type,
                                                        CMPH_SS_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr)
                                                      );
    
      computed_orig_para->orig->orig_lte_band_pref = cmcall_get_favored_lte_band_pref(
                                                    call_ptr->call_type,
                                                    CMPH_SS_LTE_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr)
                                                    );
    
      computed_orig_para->orig->orig_tds_band_pref = cmcall_get_favored_tds_band_pref(
                                                    call_ptr->call_type,
                                                    CMPH_SS_TDS_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr)
                                                    );
      computed_orig_para->orig->orig_roam_pref = CMPH_SS_ROAM_PREF(call_ptr->mm_id.asubs_id, ph_ptr);
      computed_orig_para->orig->orig_hybr_pref = call_ptr->favored_hybr_pref;
    }

  }

  return;

} /* cmipcall_convert_emergency_to_voice() */

/*===========================================================================

FUNCTION cmipcall_sups_cmd_check

DESCRIPTION
  Check the supplementary service command.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  CM_CALL_CMD_ERR_NOERR if no errors found or an error as appropriate

SIDE EFFECTS
  None

==========================================================================*/
static cm_call_cmd_err_e_type cmipcall_sups_cmd_check(

  cm_call_cmd_s_type *call_cmd_ptr
  /* SUPS command to be checked */
)
{

  cmcall_s_type              *call_ptr            = NULL;
  /* Pointer to call object */

  cm_call_cmd_err_e_type      cmd_err             = CM_CALL_CMD_ERR_NOERR;
  /* Call command error to return */

  cm_call_cmd_info_s_type    *cmd_info_ptr        = CMD_INFO_PTR( call_cmd_ptr );
  /* Pointer to call cmd info */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( call_cmd_ptr != NULL );
  CM_ASSERT( cmd_info_ptr != NULL );

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* We have to have a call up for all SUPS commands
  **
  ** - This also checks for Phone being online, since call obj
  **   does not get alloc without phone being online.
  **
  */
  call_ptr = cmcall_ptr( cmd_info_ptr->call_id );

  if(call_ptr == NULL)
  {
    return CM_CALL_CMD_ERR_CALL_ID_P;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Phone has to have service on the ss pointed by call OR
  ** It is a WLAN call and no service is req on MAIN
  */
  if(!call_ptr->is_vowlan &&
      !sys_srv_status_is_srv(cmss_get_info_srv_status(call_ptr->mm_id.asubs_id, call_ptr->mm_id.stk_id))
    )
  {
    CM_MSG_HIGH_2 ("No service on sub %d stack %d", call_ptr->mm_id.asubs_id, call_ptr->mm_id.stk_id);

    return (CM_CALL_CMD_ERR_SRV_STATE_S );
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch(cmd_info_ptr->sups_cmd_type)
  {

    /* Hold call is used either to
    ** - Hold the current active call
    ** - Hold current active call and accept incoming call.
    **  call id passed for HOLD_CALL can be in INCOM or CONV state.
    **  Reject hold operation when 1xSRVCC in progress
    */
    case CM_CALL_SUPS_TYPE_HOLD_CALL:

#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
      defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE) && defined(FEATURE_1X_SRVCC)

      if( call_ptr->srvcc_prog_state == CM_SRVCC_STATE_HO_EUTRA_RSP_SENT ||
          call_ptr->srvcc_prog_state == CM_SRVCC_STATE_HO_EUTRA_REQ_RCVD )
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
#endif
#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE)
      if(cmcall_drvcc_cache_get_drvcc_state() == CM_DRVCC_STATE_SETUP_IN_PROG)
      {
        CM_MSG_HIGH_0("DRVCC: drvcc is in progress, reject hold cmd");
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
#endif

      break;

    /* Operation of multiparty requires both calls
    ** to be connected. State of either call will
    ** be in conversation.
    */
    case CM_CALL_SUPS_TYPE_MULTIPARTY_CALL:
      if( call_ptr->call_state != CM_CALL_STATE_CONV )
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    /* ECT can be performed only when a call is
    ** in INCOM/CONV state.
    */
    case CM_CALL_SUPS_TYPE_EXPLICIT_CALL_TRANSFER:
      if( !( (call_ptr->call_state == CM_CALL_STATE_CONV) ||
             (call_ptr->call_state == CM_CALL_STATE_INCOM)
           )
        )
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;


    /* Allow Act Answer hold only when call is in incoming state.
    */
    case CM_CALL_SUPS_TYPE_ACT_ANSWER_HOLD:
      if ( ! ( ( call_ptr->call_state == CM_CALL_STATE_INCOM) &&
               (call_ptr->call_subst.incom ==
                CM_CALL_INCOM_SUBST_ALERTING)
             )
         )
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;


    /* Allow De act Answer hold only when call is in CONV state.
    */
    case CM_CALL_SUPS_TYPE_DEACT_ANSWER_HOLD:
      if( call_ptr->call_state != CM_CALL_STATE_CONV )
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    /* Allow UDUB (0-Send) only when call is in INCOM state */
    case CM_CALL_SUPS_TYPE_HELD_CALL_REL_OR_UDUB:
      if( call_ptr->call_state != CM_CALL_STATE_INCOM &&
          !(call_ptr->call_state == CM_CALL_STATE_CONV &&
            call_ptr->call_subst.conv == CM_CALL_CONV_CALL_ON_HOLD))
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    /* Allow only flash to go through if IP Call is in conversation */
    case CM_CALL_SUPS_TYPE_SIMPLE_FLASH:
      if (call_ptr->call_state != CM_CALL_STATE_CONV)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    /* Allow active Call Release only when Call is in conversation state */
    case CM_CALL_SUPS_TYPE_ACTIVE_CALL_REL:
      if (call_ptr->call_state != CM_CALL_STATE_CONV)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    case CM_CALL_SUPS_TYPE_RELEASE_CALL_X_FROM_CONFERENCE:
      if (call_ptr->call_state != CM_CALL_STATE_CONV)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    case CM_CALL_SUPS_TYPE_ADD_PARTICIPANT:
      if (call_ptr->call_state != CM_CALL_STATE_CONV)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    case CM_CALL_SUPS_TYPE_CALL_DEFLECTION:
      if (call_ptr->call_state != CM_CALL_STATE_INCOM)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    case CM_CALL_SUPS_TYPE_HOLD_PER_CALL:
      if(!( call_ptr->call_state == CM_CALL_STATE_CONV && call_ptr->call_subst.conv == CM_CALL_CONV_CALL_ACTIVE))
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    case CM_CALL_SUPS_TYPE_RESUME_PER_CALL:
      /* If the call is in conversation and is active, do not send error. */
      if(call_ptr->call_state == CM_CALL_STATE_CONV
         && call_ptr->call_subst.conv == CM_CALL_CONV_CALL_ACTIVE)
      {
        break;
      }
      if( !(call_ptr->call_state == CM_CALL_STATE_CONV && call_ptr->call_subst.conv == CM_CALL_CONV_CALL_ON_HOLD))
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      break;

    case CM_CALL_SUPS_TYPE_SWITCH_SPEECH_CODEC:
      /* no error checks required as of now */
      break;

  case CM_CALL_SUPS_TYPE_IP_EXPLICIT_CALL_TRANSFER:
  /*If IMS has not registered its callback for ECT, set err cmd.*/
  
	 if(!cmipapp_check_ims_reg_ect_cb(call_ptr->ipapp_id))
	  {
	  cmd_err = CM_CALL_CMD_ERR_CALL_SUPS_P;
	  }	  	
    break;
	  
    default:
      cmd_err = CM_CALL_CMD_ERR_CALL_SUPS_P;
      break;
  }

  return cmd_err;

} /* cmipcall_sups_cmd_check () */


/*===========================================================================

FUNCTION cmipcall_process_hold_call

DESCRIPTION
  Sends request to answer a waiting call or to put a conencted call
  on hold/active sub-state.

  Sequence to be followed
  -----------------------
  1> First put all active calls on hold.
  2> Make held calls active or accept an incoming call

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_process_hold_call
(
  const cmcall_s_type           *call_ptr,
  /* call object
  */
  cm_call_sups_type_e_type       sups_cmd
)
{
  cm_iterator_type       call_itr;
  /* iterates through call call objects in CM
  */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmcall_s_type           *call_ptr_tmp = NULL;
  cmcall_s_type           *call_ptr_tmp1 = NULL;

  cm_ip_call_sip_reject_code_s_type      sip_reject_code;
  /* SIP code sent for rejecting incoming call*/


  CM_ASSERT (call_ptr != NULL);

  memset( &sip_reject_code, 0, sizeof( sip_reject_code ) );
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(sups_cmd == CM_CALL_SUPS_TYPE_HOLD_CALL)
  {
    cmcall_obj_iterator_init(&call_itr);

    call_ptr_tmp = cmcall_obj_get_next(&call_itr);

    /* Active calls should be put on hold first
    */
    while(call_ptr_tmp != NULL)
    {

      if (call_ptr_tmp->call_state == CM_CALL_STATE_CONV &&
          call_ptr_tmp->call_subst.conv == CM_CALL_CONV_CALL_ACTIVE &&
          (call_ptr_tmp->call_type == CM_CALL_TYPE_VOICE || call_ptr_tmp->call_type == CM_CALL_TYPE_SUPS ||
           call_ptr_tmp->call_type == CM_CALL_TYPE_VT || call_ptr_tmp->call_type == CM_CALL_TYPE_VS ||
           call_ptr_tmp->call_type == CM_CALL_TYPE_EMERGENCY)
         )
      {
        if (!cmipapp_send_hold_cmd(call_ptr_tmp->ipapp_id, call_ptr_tmp->call_id, call_ptr_tmp->mm_id.asubs_id))
        {
          CM_ERR_1 ("Request to put call %d on hold failed",
                    call_ptr_tmp->call_id);
          return;
        }
        cmipcall_mng_call_cnf.ss_cmd = sups_cmd;
        cmipcall_update_mng_state_per_call(call_ptr_tmp->call_id, CMIPCALL_SUPS_ST_WAIT_HOLD_CNF);

      } /* if ((call_ptr->call_state == CM_CALL_STATE_CONV) &&
          (call_ptr->call_subst.conv == CM_CALL_CONV_CALL_ACTIVE))
        */
      call_ptr_tmp = cmcall_obj_get_next(&call_itr);

    } /* while(call_ptr != NULL) */
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Make held calls active or accept an incoming call
  */

  cmcall_obj_iterator_init(&call_itr);

  call_ptr_tmp1 = cmcall_obj_get_next(&call_itr);


  while(call_ptr_tmp1 != NULL)
  {
    switch (call_ptr_tmp1->call_state)
    {

      case CM_CALL_STATE_INCOM:

        CM_MSG_HIGH_4 ("Answering call %d, is_call_modified %d, call_type_mod_to %d rtt_mode %d", call_ptr_tmp1->call_id,
                       call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.is_call_modified,
                       call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.call_type_mod_to,
                       call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.rtt_mode);

        if((call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.is_call_modified) &&
            (call_ptr_tmp1->call_id == call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.waiting_call_id))
        {
          call_ptr_tmp1->call_type = call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.call_type_mod_to;
          call_ptr_tmp1->cmcall_mode_info.info.ip_info.ipcall_attrib_info.audio_attrib = call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.audio_attrib;
          call_ptr_tmp1->cmcall_mode_info.info.ip_info.ipcall_attrib_info.video_attrib = call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.video_attrib;
          call_ptr_tmp1->cmcall_mode_info.info.ip_info.ipcall_attrib_info.rtt_mode= call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param.rtt_mode;

        }

        /* Accept incoming call with TRUE
        */
        sip_reject_code.is_reject_code_valid = FALSE;
        cmipcall_answer (call_ptr_tmp1, TRUE, CMIPAPP_INCOM_REJ_CAUSE_NONE,sip_reject_code);
        cmipcall_mng_call_cnf.ss_cmd = sups_cmd;
        cmipcall_update_mng_state_per_call(call_ptr_tmp1->call_id, CMIPCALL_SUPS_ST_WAIT_ANS_CNF);
        return;


      case CM_CALL_STATE_CONV:

        /* If Call is currently on hold, invoke active callback
        */
        if (call_ptr_tmp1->call_subst.conv == CM_CALL_CONV_CALL_ON_HOLD)
        {
          call_ptr_tmp = call_ptr_tmp1;
        }

        break;

      case CM_CALL_STATE_NONE:
      case CM_CALL_STATE_MAX:
        CM_ERR_2("Invalid call state %d for call id %d",
                 call_ptr_tmp1->call_state,
                 call_ptr_tmp1->call_id);
        return;

      case CM_CALL_STATE_IDLE:
      case CM_CALL_STATE_ORIG:
      case CM_CALL_STATE_CC_IN_PROGRESS:
      case CM_CALL_STATE_RECALL_RSP_PEND:
      default:
        break;

    } /* switch (call_ptr->call_state) */

    call_ptr_tmp1 = cmcall_obj_get_next(&call_itr);

  } /* while(call_ptr != NULL) */

  if (call_ptr_tmp != NULL)
  {
    CM_MSG_HIGH_1 ("Requesting call %d to be made active",
                   call_ptr_tmp->call_id);

    if (!cmipapp_send_resume_cmd(call_ptr_tmp->ipapp_id, call_ptr_tmp->call_id, call_ptr_tmp->mm_id.asubs_id))
    {
      CM_ERR_0 ("Request to make call active failed");
      return;
    }
    cmipcall_mng_call_cnf.ss_cmd = sups_cmd;
    cmipcall_update_mng_state_per_call(call_ptr_tmp->call_id, CMIPCALL_SUPS_ST_WAIT_RETRIEVE_CNF);
  }

} /* cmipcall_process_hold_call () */


/*===========================================================================

FUNCTION cmipcall_process_act_answer_hold

DESCRIPTION
  Sends a request for activating answer hold.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_process_act_answer_hold
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  if (!cmipapp_send_activate_answer_hold_cmd(call_ptr->ipapp_id,
      call_ptr->call_id,
      call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Request to put call on answer-hold failed, call_id %d",
              call_ptr->call_id);
    return;
  }

} /* cmipcall_process_act_answer_hold () */



/*===========================================================================

FUNCTION cmipcall_process_deact_answer_hold

DESCRIPTION
  Sends a request for de-activating answer hold.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_process_deact_answer_hold
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  if (!cmipapp_send_deactivate_answer_hold_cmd(call_ptr->ipapp_id,
      call_ptr->call_id,
      call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Request to deact answer-hold failed, call_id %d",
              call_ptr->call_id);
    return;
  }

} /* cmipcall_process_act_answer_hold () */



/*===========================================================================

FUNCTION cmipcall_process_ect

DESCRIPTION
  Sends a request to perform explicit call transfer on the calls.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_process_ect
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  if (!cmipapp_send_ect_on_calls_cmd(call_ptr->ipapp_id, call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Request to perform ECT failed, call_id %d",
              call_ptr->call_id);
    return;
  }

} /* cmipcall_process_ect () */

cmipapp_ip_expicit_call_transfer_e_type cmipcall_map_ect_type_to_ip_ect_type (
  cm_call_ip_expicit_call_transfer_e_type ect_type
)
{
  cmipapp_ip_expicit_call_transfer_e_type ip_ect_type;
  switch(ect_type)
  {
    case CM_EXPLICIT_CALL_TRANSFER_TYPE_BLIND:
      ip_ect_type = CMIPAPP_EXPLICIT_CALL_TRANSFER_TYPE_BLIND;
      break;
    case CM_EXPLICIT_CALL_TRANSFER_TYPE_ASSURED:
      ip_ect_type = CMIPAPP_EXPLICIT_CALL_TRANSFER_TYPE_ASSURED;
      break;
    case CM_EXPLICIT_CALL_TRANSFER_TYPE_CONSULTATIVE:
      ip_ect_type = CMIPAPP_EXPLICIT_CALL_TRANSFER_TYPE_CONSULTATIVE;
      break;
    default:
      ip_ect_type = CMIPAPP_EXPLICIT_CALL_TRANSFER_TYPE_NONE;
      break;
  }
  return ip_ect_type;
}

/*===========================================================================

FUNCTION cmipcall_process_ip_explicit_call_transfer

DESCRIPTION
  Sends a request to perform explicit call transfer for IP Calls.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_process_ip_explicit_call_transfer
(
  const cmcall_s_type           *call_ptr,
  /* call object
  */
  const cm_call_cmd_info_s_type    *cmd_info_ptr
)
{
  CM_ASSERT (call_ptr != NULL);
  CM_ASSERT (cmd_info_ptr != NULL);

  CM_MSG_HIGH_1 ("Requesting ECT for Call id %d", call_ptr->call_id);

  if (!cmipapp_send_ip_explict_call_transfer_on_calls_cmd(
        call_ptr->ipapp_id,
        call_ptr->mm_id.asubs_id,
        call_ptr->call_id,
        cmipcall_map_ect_type_to_ip_ect_type(cmd_info_ptr->ip_call.ip_sups_ect_params.ect_type),
        cmd_info_ptr->ip_call.ip_sups_ect_params.transfer_target_call_num,
        cmd_info_ptr->ip_call.ip_sups_ect_params.transfer_target_call_id))
  {
    CM_ERR_1 ("Request to perform ECT failed, call_id %d", call_ptr->call_id);
    return;
  }

} /* cmipcall_process_ect () */

/*===========================================================================

FUNCTION cmipcall_process_mpty

DESCRIPTION
  Sends a request to perform multiparty on given calls.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_process_mpty
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  if (!cmipapp_send_mpty_cmd( call_ptr->ipapp_id, call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Request to perform MPTY failed, call_id %d",
              call_ptr->call_id);
    return;
  }

} /* cmipcall_process_mpty () */

/*===========================================================================

FUNCTION cmipcall_proc_sups_cmd_type

DESCRIPTION
  Branches sups request to hold, activ answer-hold or deact answer-hold
  functions.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
static void cmipcall_proc_sups_cmd_type
(
  const cm_call_cmd_info_s_type    *cmd_info_ptr,
  /* Pointer to cmd
  */

  cmcall_s_type              *call_ptr
  /* Pointer to a call object
  */
)
{

  cmipapp_incom_rej_cause_e_type   incoming_rej = CMIPAPP_INCOM_REJ_CAUSE_NONE;

  if( call_ptr == NULL )
  {
    CM_ERR_0 ("parameter call_ptr = NULL");
    return;
  }

  switch (cmd_info_ptr->sups_cmd_type)
  {
    case CM_CALL_SUPS_TYPE_HOLD_CALL:

      /* Call is put into hold or active depending on its current
      ** active status.
      **
      ** SUPS_TYPE_HOLD_CALL toggles a call between held and active
      ** states.
      */
      cmipcall_process_hold_call (call_ptr, CM_CALL_SUPS_TYPE_HOLD_CALL);

      break;

    case CM_CALL_SUPS_TYPE_ACT_ANSWER_HOLD:

      /* Call is being put on answer hold.
      */
      cmipcall_process_act_answer_hold (call_ptr);

      break;


    case CM_CALL_SUPS_TYPE_DEACT_ANSWER_HOLD:

      /* Call is being pulled out of answer hold.
      */
      cmipcall_process_deact_answer_hold (call_ptr);

      break;

    case CM_CALL_SUPS_TYPE_EXPLICIT_CALL_TRANSFER:

      /* Explicit call transfer request being processed
      */
      cmipcall_process_ect (call_ptr);

      break;


    case CM_CALL_SUPS_TYPE_MULTIPARTY_CALL:

      /* Request being sent to put call in MPTY
      */
      cmipcall_process_mpty (call_ptr);

      break;

    case CM_CALL_SUPS_TYPE_HELD_CALL_REL_OR_UDUB:

      /* Answer the call and send reject.
      */
      incoming_rej = cmcall_end_cause_to_cmipcall_rej_status(cmd_info_ptr->ip_call.call_reject_reason);

      if(call_ptr->call_state == CM_CALL_STATE_INCOM)
      {
        cmipcall_answer (call_ptr, FALSE, incoming_rej,cmd_info_ptr->ip_call.sip_reject_code);

        cmipcall_mng_call_cnf.ss_cmd = CM_CALL_SUPS_TYPE_HELD_CALL_REL_OR_UDUB;
        cmipcall_update_mng_state_per_call(call_ptr->call_id, CMIPCALL_SUPS_ST_WAIT_ANS_CNF);
      }
      /*Release all held calls
      */
      else
      {
        cm_iterator_type         call_itr;
        cmcall_s_type           *pcall;

        cmcall_obj_iterator_init(&call_itr);
        pcall = cmcall_obj_get_next(&call_itr);

        while(pcall != NULL)
        {

          if (pcall->call_state == CM_CALL_STATE_CONV &&
              pcall->call_subst.conv == CM_CALL_CONV_CALL_ON_HOLD
             )
          {
            cmipcall_send_end (pcall);
            cmipcall_mng_call_cnf.ss_cmd = CM_CALL_SUPS_TYPE_HELD_CALL_REL_OR_UDUB;
            cmipcall_update_mng_state_per_call(call_ptr->call_id, CMIPCALL_SUPS_ST_WAIT_END_CNF);
          }
          pcall = cmcall_obj_get_next(&call_itr);
        }
      }

      break;

    case CM_CALL_SUPS_TYPE_SIMPLE_FLASH:
      /* Answer the Call Waiting Indication/Delete last party added to the conference
      */
      cmipcall_simple_flash(call_ptr);
      break;

    case CM_CALL_SUPS_TYPE_ACTIVE_CALL_REL:

      /* Holded call should not be ended, End call only if active */
      if (call_ptr->call_state      == CM_CALL_STATE_CONV &&
          call_ptr->call_subst.conv != CM_CALL_CONV_CALL_ON_HOLD)
      {
        cmipcall_mng_call_cnf.ss_cmd = CM_CALL_SUPS_TYPE_ACTIVE_CALL_REL;
        cmipcall_send_end (call_ptr);
        cmipcall_update_mng_state_per_call(call_ptr->call_id, CMIPCALL_SUPS_ST_WAIT_END_CNF);
      }

      /* Do retreive only. If there is call on hold, automatically retrieve it.
      */
      cmipcall_process_hold_call (call_ptr, CM_CALL_SUPS_TYPE_ACTIVE_CALL_REL);
      break;

    case CM_CALL_SUPS_TYPE_RELEASE_CALL_X_FROM_CONFERENCE:
      cmipcall_remove_last_party(call_ptr);
      break;

    case CM_CALL_SUPS_TYPE_ADD_PARTICIPANT:
      cmipcall_add_new_party(call_ptr);
      break;

    case CM_CALL_SUPS_TYPE_CALL_DEFLECTION:
      cmipcall_mng_call_cnf.ss_cmd = CM_CALL_SUPS_TYPE_CALL_DEFLECTION;
      cmipcall_update_mng_state_per_call(call_ptr->call_id, CMIPCALL_SUPS_ST_WAIT_CALL_DEFLECTION_CNF);

      cmipcall_process_call_deflection(call_ptr, cmd_info_ptr);
      break;

    case CM_CALL_SUPS_TYPE_HOLD_PER_CALL:
      cmipcall_mng_call_cnf.ss_cmd = CM_CALL_SUPS_TYPE_HOLD_PER_CALL;
      cmipcall_update_mng_state_per_call(call_ptr->call_id, CMIPCALL_SUPS_ST_WAIT_HOLD_CNF);

      cmipapp_send_hold_cmd(call_ptr->ipapp_id, call_ptr->call_id, call_ptr->mm_id.asubs_id);
      break;

    case CM_CALL_SUPS_TYPE_RESUME_PER_CALL:
      cmipcall_mng_call_cnf.ss_cmd = CM_CALL_SUPS_TYPE_RESUME_PER_CALL;
      /* If the call is in conversation and is active, send all call status
      ** notification and send success response. 
      */
      if(call_ptr->call_state == CM_CALL_STATE_CONV
         && call_ptr->call_subst.conv == CM_CALL_CONV_CALL_ACTIVE)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_CALL_RETRIEVED);
        cmipcall_send_mng_cnf(TRUE, CM_IPAPP_CAUSE_NORMAL, call_ptr->call_id);
        break;
      }
      cmipcall_update_mng_state_per_call(call_ptr->call_id, CMIPCALL_SUPS_ST_WAIT_RETRIEVE_CNF);
      cmipapp_send_resume_cmd(call_ptr->ipapp_id, call_ptr->call_id, call_ptr->mm_id.asubs_id);
      break;

    case CM_CALL_SUPS_TYPE_SWITCH_SPEECH_CODEC:
    {
      if (!cmipapp_send_switch_speech_codec_cmd(
            call_ptr->ipapp_id,
            call_ptr->call_id,
            cmd_info_ptr->ip_call.ip_sups_param.switch_to_codec,
            cmd_info_ptr->ip_call.ip_sups_param.priority_codec,
            call_ptr->mm_id.asubs_id))
      {
        CM_ERR_1 ("Sending switch speech codec to app failed, call_id %d",
                  call_ptr->call_id);
        return;
      }
    }
    break;

    case CM_CALL_SUPS_TYPE_IP_EXPLICIT_CALL_TRANSFER:
    {
      /* IP Explicit call transfer request being processed */
      cmipcall_process_ip_explicit_call_transfer (call_ptr, cmd_info_ptr);
    }
    break;

    default:
      CM_ERR_1 ("Unknown command type received = %d",
                cmd_info_ptr->sups_cmd_type);
      break;
  }

} /* cmipcall_proc_sups_cmd_type () */

/**--------------------------------------------------------------------------
** Functions - external:
** --------------------------------------------------------------------------
*/

/*===========================================================================

FUNCTION cmipcall_reset_mng_call_cnf

DESCRIPTION
  reset MNG call cnf structure to default values

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmipcall_reset_mng_call_cnf(void)
{
  CM_MSG_MED_0("reset_mng_cnf");
  memset(&cmipcall_mng_call_cnf,0,sizeof(cmipcall_mng_call_s_type));
  cmipcall_mng_call_cnf.ss_cmd = CM_CALL_SUPS_TYPE_NONE;
}

/*===========================================================================

FUNCTION cmipcall_reset

DESCRIPTION
  Reset IP call fields to default values reflecting
  a IP call in idle state.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmipcall_reset(

  cmcall_s_type  *call_ptr
  /* pointer to a call object */
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( call_ptr != NULL )

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  call_ptr->ipapp_id          = CMIPAPP_ID_INVALID;

  call_ptr->ipapp_data_block  = 0;

  call_ptr->is_volte          = FALSE;

  call_ptr->end_cause_to_client = CM_CALL_END_NONE;

  call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type = CM_CALL_TYPE_NONE;

  call_ptr->cmcall_mode_info.info.ip_info.ims_end_reason = NULL;

  call_ptr->cmcall_mode_info.info.ip_info.sip_error_code = CM_IP_SIP_ERROR_CODE_NONE;

  call_ptr->cmcall_mode_info.info.ip_info.alternate_uri = NULL;

  call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.vs_ver = CM_IP_VS_RCSE;
  REF_CNT_OBJ_RELEASE_IF(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param);
  REF_CNT_OBJ_RELEASE_IF(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.picture_url);
  REF_CNT_OBJ_RELEASE_IF(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.subject);
  memset(&call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info,0, sizeof(cm_ip_call_attrib_info_s_type));
  memset(&call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib,0, sizeof(cm_ip_call_attrib_info_s_type));

#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE)
  call_ptr->is_srvcc_call                    = FALSE;
  call_ptr->is_int_ims                       = TRUE;
  call_ptr->mid_call_supported               = FALSE;
  call_ptr ->ims_call_type                   = CM_CALL_TYPE_NONE;
  call_ptr->parent_call_id                   = CM_CALL_ID_INVALID;
  call_ptr->alert_media_type                 = CM_SRVCC_ALERTING_NONE;
  call_ptr->is_conf_call                     = FALSE;
  call_ptr->srvcc_prog_state                 = CM_SRVCC_STATE_NONE;

  if(call_ptr->srvcc_caller_info.caller_name != NULL)
  {
    cm_mem_free(call_ptr->srvcc_caller_info.caller_name);
  }

  call_ptr->srvcc_caller_info.caller_name     = NULL;
  call_ptr->srvcc_caller_info.caller_len      = 0;
  call_ptr->srvcc_caller_info.caller_name_pi   = CM_PRESENTATION_ALLOWED;
  call_ptr->orm_sent_status = FALSE;
  call_ptr->drvcc_parent_call_id = CM_CALL_ID_INVALID;
#endif

}

/*===========================================================================

FUNCTION cmipcall_send_mng_cnf

DESCRIPTION
  send mng_cnf to client

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

static void cmipcall_send_mng_cnf(boolean call_ss_status, cm_ipapp_cause_e_type failure_cause,cm_call_id_type call_id)
{

  cmipapp_ind_s_type           ipapp_ind;
  uint8                        i;
  cmcall_s_type               *call_ptr = cmcall_ptr(call_id);
  sys_modem_as_id_e_type       call_sub = SYS_MODEM_AS_ID_1;

  if(cmipcall_mng_call_cnf.ss_cmd == CM_CALL_SUPS_TYPE_NONE)
  {
    return;
  }

  if(call_ptr != NULL)
  {
    call_sub = call_ptr->mm_id.asubs_id;
  }

  /* if there is no others calls waiting for cnf, send mng_cnf with ss_status. otherwise,
  ** continue waiting for cnf
  */
  for(i=0; i<CMCALL_MAX_CALL_OBJ; i++)
  {
    if(cmipcall_mng_call_cnf.cmipcall_mng_call_ids[i] != CMIPCALL_SUPS_ST_NONE)
    {
      CM_MSG_HIGH_3("Not send mng cnf, cmd=%d, call=%d, state=%d",
                    cmipcall_mng_call_cnf.ss_cmd,i,
                    cmipcall_mng_call_cnf.cmipcall_mng_call_ids[i]);
      return;
    }
  }

  /* send MNG_CNF to client */
  memset(&ipapp_ind,
         0,
         sizeof(cmipapp_ind_s_type));

  CM_MSG_HIGH_3("CM->CM: CM_IP_CALL_IND_SUPS_CONF, call_success %d, sups_type %d, as_id %d",
                call_ss_status,
                ipapp_ind.ind.sups_conf.sups_type,
                call_sub);

  ipapp_ind.ind.sups_conf.id = call_id;
  ipapp_ind.ind.sups_conf.call_success = call_ss_status;
  ipapp_ind.ind.sups_conf.sups_type= cmipcall_mng_call_cnf.ss_cmd;
  ipapp_ind.ind.sups_conf.failure_cause = failure_cause;
  ipapp_ind.hdr.asubs_id = call_sub;
  if(call_ptr != NULL )
  {
    ipapp_ind.ind.sups_conf.sip_error_code = call_ptr->cmcall_mode_info.info.ip_info.sip_error_code;
    ipapp_ind.ind.sups_conf.end_reason = call_ptr->cmcall_mode_info.info.ip_info.ims_end_reason;
  }

  cmipapp_rpt_ind (CM_IP_CALL_IND_SUPS_CONF, ipapp_ind, 0);

  cmipcall_reset_mng_call_cnf();
}
/*===========================================================================

FUNCTION cmipcall_mng_state_update

DESCRIPTION
  Update mng_call_cnf states

DEPENDENCIES

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/

void cmipcall_mng_state_update(
  cm_name_type ip_ind_type,
  boolean ss_status,
  cm_call_id_type call_id,
  cm_ipapp_cause_e_type failure_cause)
{

  cmcall_s_type *call_ptr = cmcall_ptr(call_id);
  if(call_ptr == NULL)
  {
    CM_MSG_HIGH_1( "Null call ptr for call id %d", call_id);
    return;
  }

  CM_MSG_MED_6_EXT("cmipcall_mng_state_update:ind_type %d, status %d, call_id %d call mng state %d, ss_cmd %d, failure_cause %d",
                   ip_ind_type, ss_status, call_id,
                   cmipcall_mng_call_cnf.cmipcall_mng_call_ids[call_id],cmipcall_mng_call_cnf.ss_cmd, failure_cause,
                   (call_ptr->mm_id.asubs_id+1));

  switch(ip_ind_type)
  {
    case CM_IP_CALL_IND_CALL_HOLD:

      /* check if call id is in valid state to receive call hold cnf */
      if(CMIPCALL_SUPS_ST_WAIT_HOLD_CNF == cmipcall_mng_call_cnf.cmipcall_mng_call_ids[call_id])
      {
        cmipcall_update_mng_state_per_call(call_id, CMIPCALL_SUPS_ST_NONE);
      }
      else
      {
        return;
      }
      break; //CM_IP_CALL_IND_CALL_HOLD

    case CM_IP_CALL_IND_CALL_DEFLECTION:

      /* check if call id is in valid state to receive call deflection cnf */
      if(CMIPCALL_SUPS_ST_WAIT_CALL_DEFLECTION_CNF == cmipcall_mng_call_cnf.cmipcall_mng_call_ids[call_id])
      {
        cmipcall_update_mng_state_per_call(call_id, CMIPCALL_SUPS_ST_NONE);
      }
      else
      {
        return;
      }
      break; //CM_IP_CALL_IND_CALL_DEFLECTION

    case CM_IP_CALL_IND_CONNECTED:
      /* check if call id is in valid state to receive call connected */
      if(CMIPCALL_SUPS_ST_WAIT_ANS_CNF == cmipcall_mng_call_cnf.cmipcall_mng_call_ids[call_id])
      {
        cmipcall_update_mng_state_per_call(call_id, CMIPCALL_SUPS_ST_NONE);
      }
      else
      {
        return;
      }
      break; //CM_IP_CALL_IND_CONNECTED

    case CM_IP_CALL_IND_CALL_RETRIEVE:
      if(CMIPCALL_SUPS_ST_WAIT_RETRIEVE_CNF == cmipcall_mng_call_cnf.cmipcall_mng_call_ids[call_id])
      {
        cmipcall_update_mng_state_per_call(call_id, CMIPCALL_SUPS_ST_NONE);
      }
      else
      {
        return;
      }
      break; //CM_IP_CALL_IND_CALL_RETRIEVE

    case CM_IP_CALL_IND_CALL_END:
    case CM_IP_CALL_IND_ANSWER_REJECT: // This is specific only to the case when INCOM call is rejected without processing by IMS
      cmipcall_update_mng_state_per_call(call_id, CMIPCALL_SUPS_ST_NONE);
      break; //CM_IP_CALL_IND_CALL_END | CM_IP_CALL_IND_ANSWER_REJECT

    default:
      CM_ERR_1( "wrong ip_ind_type =%d",
                ip_ind_type);
      break;
  }

  cmipcall_send_mng_cnf(ss_status, failure_cause,call_id);
}


/*===========================================================================

FUNCTION cmipcall_copy_error_info
DESCRIPTION
Copies Error text/sip code for call operation failures

DEPENDENCIES
Call object must have already been initialized with
cmcall_init().

RETURN VALUE
none

SIDE EFFECTS
none

===========================================================================*/
static void cmipcall_copy_error_info( cm_ip_call_end_reason_s_type *end_reason, uint16 sip_code, cmcall_s_type  *call_ptr)
{

  if(call_ptr == NULL)
  {
    CM_ERR_0 ( "ERROR: cmipcall_copy_error_info, call ptr NULL");
    return;
  }

  if(end_reason != NULL)
  {
    call_ptr->cmcall_mode_info.info.ip_info.ims_end_reason = end_reason;
    CM_MSG_HIGH_1("end reason len %d",  call_ptr->cmcall_mode_info.info.ip_info.ims_end_reason->end_reason_len);
  }
  call_ptr->cmcall_mode_info.info.ip_info.sip_error_code = sip_code;
}
/*===========================================================================

FUNCTION cmipcall_calculate_effective_call_type_special_emerg

DESCRIPTION
  Determine the effective call_type for special emeregncy call

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  The effective call_type for emeregncy

SIDE EFFECTS
  Nothing.

===========================================================================*/
cm_call_type_e_type cmipcall_calculate_effective_call_type_special_emerg
(
  cmcall_s_type 		  *call_ptr
  /* Call object for the originating call
	  */
)
{

  cm_call_type_e_type effective_call_type = call_ptr->call_type;
  sd_si_info_s_type *ss_true_stk_info_ptr = cmss_true_stack_info_ptr(call_ptr->mm_id);

  if(ss_true_stk_info_ptr == NULL)
  {
    sys_err_fatal_null_ptr_exception();
    return FALSE;
  }
  
  if(call_ptr->special_emergency)
  {
    /* Call type changed to voice to check if voice ipcall is possible over LTE */
    call_ptr->call_type = CM_CALL_TYPE_VOICE;
	

    if(call_ptr->is_vowlan  &&
       cmcall_is_ipcall_over_mode_poss(SYS_SYS_MODE_WLAN, call_ptr))
    {
      /* call type sent to IMS is VOICE */
      effective_call_type= CM_CALL_TYPE_VOICE;
    }

    else if(ss_true_stk_info_ptr->srv_status==SYS_SRV_STATUS_SRV &&
       ss_true_stk_info_ptr->roam==SYS_ROAM_STATUS_OFF &&
 cmcall_is_ipcall_over_mode_poss(SYS_SYS_MODE_LTE, call_ptr))
    {
      /* call type sent to IMS is VOICE */
      effective_call_type= CM_CALL_TYPE_VOICE;
    }

    /* Call type changed to back to emergency after checking if voice ipcall is possible over LTE */
    call_ptr->call_type = CM_CALL_TYPE_EMERGENCY;

    CM_MSG_HIGH_2("ipcall spl emergency: Current Service status: %d , Current Roam Status: %d",
                  ss_true_stk_info_ptr->srv_status, ss_true_stk_info_ptr->roam);
  }
  return effective_call_type;

}

/*===========================================================================

FUNCTION cmipcall_rpt_proc

DESCRIPTION
  Process app reports (i.e. notifications of call activity or
  replies to clients call commands).

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmipcall_rpt_proc(

  const cm_hdr_type   *rpt_ptr
  /* Pointer to a IP app report */
)
{

  const cmipapp_rpt_s_type  *cmipapp_rpt_ptr = (cmipapp_rpt_s_type *) rpt_ptr;
  /* Pointer to a IP app report */

  cmcall_s_type             *call_ptr = NULL;
  /* Point at call object */

  cm_mm_call_info_s_type    *call_info_ptr = NULL;
  /* Structure to send: call event */

  ip_cmcall_type            *ip_ptr = NULL;
  /* type to point to ipcall specific info */

  boolean                    ss_status = FALSE;
  /* supplement service status */

  cmph_s_type                *ph_ptr         = cmph_ptr();

  cm_call_id_type            call_id = CM_CALL_ID_INVALID;

  cmph_sub_info_s_type           *ph_sub_ptr;
  
  mm_sub_stk_id_s_type local_mm_id;
  dword                    uptime ;
  sys_mcc_type  current_mcc = SYS_WILDCARD_MCC;
  sys_mcc_type  home_mcc = SYS_WILDCARD_MCC;
  cm_mm_ss_info_sub_prop_s_type *ss_sub_ptr;
  cm_mm_ss_info_gen_stack_info_s_type *ss_stack_ptr;
  volte_sub_info_s_type *volte_ptr;
  cm_sr_s_type             *sr_ptr              = NULL;

  #if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
  sys_aplmn_list_s_type          *avoid_plmn_list = NULL;
  #endif
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( cmipapp_rpt_ptr != NULL );
  ph_sub_ptr = cmph_get_sub_ptr(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);
  CM_ASSERT( ph_sub_ptr != NULL );

  volte_ptr = cmph_get_volte_sub_info_ptr(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

  ss_sub_ptr = cmss_get_info_sub_prop(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);
  
  ss_stack_ptr = ss_sub_ptr->stack_info[MM_STACK_0].gen_info;
       
  sys_plmn_get_mcc(ss_stack_ptr->sys_id.id.plmn, &current_mcc);
  sys_plmn_get_mcc(ph_ptr->sub_info[cmipapp_rpt_ptr->ip_ind.hdr.asubs_id]->gwl_imsi_plmn, &home_mcc);
  
  /* Do command specific processing as follows:
  ** 1. Update appropriate call object fields.
  ** 2. Modify the call state if necessary.
  ** 3. Notifying clients of event if necessary.
  */
  switch( cmipapp_rpt_ptr->hdr.cmd )
  {
    case CM_IP_CALL_IND_MT_INVITE:
    {
       CM_MSG_HIGH_6("IP RXD: MT_INVITE sys_mode=%d, as_id=%d, call_id %d, call_type %d, is_ims_cap_on_sub=%d rtt_mode=%",
                    cmipapp_rpt_ptr->ip_ind.ind.mt_invite.sys_mode,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                    cmipapp_rpt_ptr->ip_ind.ind.mt_invite.id,
                    cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_type,
                    cmph_is_ims_capable_on_sub(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id),
                    cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.rtt_mode);

      /* Call id should be requested by IP app before sending any reports to CM
      */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.mt_invite.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT( call_ptr != NULL );
        CM_ERR_0 ( "ERROR: Unknown call ID in MT invite");
        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      /*------------------------------------------------------------------*/

      /* Record app that is placing the call
      */
      call_ptr->ipapp_id = cmipapp_rpt_ptr->ip_ind.ind.mt_invite.app_id;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /* Copy system mode for the call
      */
      call_ptr->sys_mode = cmipapp_map_sysmode_to_cm_sysmode(
                             cmipapp_rpt_ptr->ip_ind.ind.mt_invite.sys_mode);

      /*------------------------------------------------------------------*/

      /* Determine ss for this call
      */
      if(cmph_is_msim())
      {
        call_ptr->mm_id.asubs_id = cmipapp_rpt_ptr->ip_ind.hdr.asubs_id;
      }
      else
      {
        call_ptr->mm_id.asubs_id = SYS_MODEM_AS_ID_1;
      }
      call_ptr->mm_id.stk_id = cmcall_det_ss_from_sys_mode(call_ptr);

      CM_ASSERT(call_ptr->mm_id.stk_id != MM_STACK_NONE);

      if(call_ptr->mm_id.stk_id == MM_STACK_NONE)
      {
        CM_ERR_1("ss for incom call id %d returned NONE", call_ptr->call_id);
      }

      /*------------------------------------------------------------------*/

      /* Call type is carried by invite indication
      */
      call_ptr->call_type = cmipcall_map_calltype_to_cm_calltype(
                              cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_type);

      if((call_ptr->sys_mode == SYS_SYS_MODE_LTE) &&
          (call_ptr->call_type == CM_CALL_TYPE_VOICE ||
           call_ptr->call_type == CM_CALL_TYPE_VT ||
           call_ptr->call_type == CM_CALL_TYPE_VS))
      {
        call_ptr->is_volte = TRUE;
      }
      else if((call_ptr->sys_mode == SYS_SYS_MODE_WLAN) &&
              (call_ptr->call_type == CM_CALL_TYPE_VOICE ||
               call_ptr->call_type == CM_CALL_TYPE_VT))
      {
        call_ptr->is_vowlan = TRUE;
      }
      else
      {
        call_ptr->is_ip_call = TRUE;
      }

      /* Initialize number buffer in call object
      */
      memset((byte *)&call_ptr->num, 0, sizeof(call_ptr->num));

      if( cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num.len != 0 )
      {
        call_ptr->num.len = memscpy(call_ptr->num.buf,
                                    sizeof(call_ptr->num.buf),
                                    cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num.buf,
                                    cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num.len);
      }

      /* Initialize pi in call object
      */
      call_ptr->num.pi = cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num.pi;

      /*Intialize child nuber
      */
      memset((byte *)&call_ptr->child_num, 0, sizeof(call_ptr->child_num));

      if( cmipapp_rpt_ptr->ip_ind.ind.mt_invite.child_num.len != 0 )
      {
        call_ptr->child_num.len = memscpy(call_ptr->child_num.buf,
                                          sizeof(call_ptr->child_num.buf),
                                          cmipapp_rpt_ptr->ip_ind.ind.mt_invite.child_num.buf,
                                          cmipapp_rpt_ptr->ip_ind.ind.mt_invite.child_num.len);
      }

      /* Initialize redirecting number buffer in call object
      */
      memset((byte *)&call_ptr->redirecting_number.num, 0, sizeof(call_ptr->redirecting_number.num));

      if( cmipapp_rpt_ptr->ip_ind.ind.mt_invite.redirecting_number.len != 0 )
      {
        call_ptr->redirecting_number.num.len = memscpy(call_ptr->redirecting_number.num.buf,
                                               sizeof(call_ptr->redirecting_number.num.buf),
                                               cmipapp_rpt_ptr->ip_ind.ind.mt_invite.redirecting_number.buf,
                                               cmipapp_rpt_ptr->ip_ind.ind.mt_invite.redirecting_number.len);

        call_ptr->redirecting_number.num.pi = cmipapp_rpt_ptr->ip_ind.ind.mt_invite.redirecting_number.pi;
      }

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info =
        cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.history_info;

      /* For adversarial scenarios, logging history info len/text*/
      if(cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.history_info != NULL)
      {
        if((cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.history_info->hist_info_len == 0) ||
            (cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.history_info->hist_info_text == NULL))
        {
          CM_MSG_HIGH_2("hist_info_len %d and hist_info_text %d. Something Wrong!",
                        cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.history_info->hist_info_len,
                        cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.history_info->hist_info_text);
        }
      }

      /*logging alert info len/text*/

      if(cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.alert_info != NULL)
      {
        if((cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.alert_info->alert_info_len == 0) ||
            (cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.alert_info->alert_info_text == NULL))
        {
          CM_MSG_HIGH_2("alert_info_len %d and alert_info_text %d. Something Wrong!",
                        cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.alert_info->alert_info_len,
                        cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.alert_info->alert_info_text);
        }
        else
        {
          CM_MSG_HIGH_2("IP RXD: MT_INVITE, update alert_info for sys_mode=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.mt_invite.sys_mode,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);
          call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.alert_info =
          cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.alert_info;
        }
      }

      /* Update call composer info if avail */
      cmipcall_mt_invite_update_call_composer(
          call_ptr, 
          &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info);
          
      /*------------------------------------------------------------------*/

      /* Set up ind is not presented to the user and is only used by UI
      ** code to check if an incoming call can be allowed. User is alerted
      ** after CM_IP_CALL_IND_MT_RING_CNF is received.
      */
      call_ptr->call_state                     = CM_CALL_STATE_INCOM;

      call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_SETUP;

      /* Set call info type to IP
      */
      call_ptr->cmcall_mode_info.info_type = CM_CALL_MODE_INFO_IP;
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info;
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_peer_cap_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.mt_invite.peer_call_cap_attrib;
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
      call_ptr->mt_call_verstat.call_verstat = cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_verstat;
#endif
      CM_MSG_HIGH_6("is_secure_call %d, call_type %d, sys_mode %d, is_volte %d, is_vowlan %d, is_ip_call %d", 
                        cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.is_secure_call,
                        call_ptr->call_type,
                        call_ptr->sys_mode,
                        call_ptr->is_volte,
                        call_ptr->is_vowlan,
                        call_ptr->is_ip_call);
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
      CM_MSG_HIGH_2(" call_comp peer %d call_verstat %d",
                       cmipapp_rpt_ptr->ip_ind.ind.mt_invite.peer_call_cap_attrib.call_composer_supported, call_ptr->mt_call_verstat.call_verstat);
#else
      CM_MSG_HIGH_1(" call_comp peer %d",
                       cmipapp_rpt_ptr->ip_ind.ind.mt_invite.peer_call_cap_attrib.call_composer_supported);
#endif
      if(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param != NULL)
      {
        ref_cnt_obj_add_ref(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param);
      }

      if(cmipapp_rpt_ptr->ip_ind.ind.mt_invite.caller_info != NULL)
      {
        call_ptr->cmcall_mode_info.info.ip_info.caller_info = cmipapp_rpt_ptr->ip_ind.ind.mt_invite.caller_info;
        CM_MSG_HIGH_1("caller_info present, info_len %d", call_ptr->cmcall_mode_info.info.ip_info.caller_info->info_len);
      }

      cmcall_set_call_direction(call_ptr, CM_CALL_DIRECTION_MT);

	  
	  	        /* Check if voice call is allowed, if not then block it. there are
      ** no emergency MT calls the check added is a safety check, in case
      ** the MT call comes with call type emergency
      */
      if( (( CMPH_SS_DISABLE_CALL_TYPE_MASK(call_ptr->mm_id.asubs_id, ph_ptr) & CM_CALL_TYPE_VOICE_MASK ) &&
           ( call_ptr->call_type == CM_CALL_TYPE_VOICE ||
             call_ptr->call_type == CM_CALL_TYPE_EMERGENCY ))
          ||
          ( ( CMPH_SS_DISABLE_CALL_TYPE_MASK(call_ptr->mm_id.asubs_id, ph_ptr) & CM_CALL_TYPE_VT_MASK )&&
            ( call_ptr->call_type == CM_CALL_TYPE_VT ))
        )
      {

	    cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_CALL_TYPE_NOT_ALLOWED);

	    CM_MSG_HIGH_0( "Rejecting CM_MT_SETUP_IND as call type is disabled");
	  
	    call_ptr->call_subst.incom			   = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;


      }

      /* Perform error checking, and reject the call if necessary
      */
      else if(ph_ptr->device_prop.ui_ready_status == FALSE)
      {
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_BUSY);
        CM_MSG_HIGH_0("UI not ready, reject incoming call");

        cmcall_send_mt_call_end_auto_reject_ind(
            cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
            call_ptr->call_type,
            CM_IP_SIP_ERROR_CODE_NONE,
            CM_CALL_END_INCOM_REJ_CAUSE_UI_NOT_READY,
            cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,
            call_ptr->cmcall_mode_info.info_type,
            &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
             call_ptr->mt_call_verstat.call_verstat);

        call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      }

      /* If the UE is in an emergency call, any incoming call must be provided the busy treatment, irrespective of the CW setting.
      */
      else if(cmcall_is_there_a_call_type_no_call_id_per_sub(call_ptr->mm_id.asubs_id, CM_CALL_TYPE_EMERGENCY, call_ptr->call_id) != CM_CALL_ID_INVALID)
      {
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_BUSY);

		CM_MSG_HIGH_0("UE is in an emergency call, incoming call must be provided the busy treatment");
		
        cmcall_send_mt_call_end_auto_reject_ind(
            cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
            call_ptr->call_type,
            CM_IP_SIP_ERROR_CODE_NONE,
            CM_CALL_END_CAUSE_EMRG_CALL_ONGOING,
            cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,
            call_ptr->cmcall_mode_info.info_type,
            &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
             call_ptr->mt_call_verstat.call_verstat);

        call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      }

      /* If the UE is in a voice or VT call, any incoming call must be provided the busy treatment,
      ** if the CW is set to be UE based and it s disabled.
      */
      else if(cmph_get_volte_sub_info_ptr(call_ptr->mm_id.asubs_id)->ue_based_cw.use_ue_based_cw &&
              !cmph_get_volte_sub_info_ptr(call_ptr->mm_id.asubs_id)->ue_based_cw.is_enabled &&
              ((cmcall_is_there_a_call_type_no_call_id_per_sub(call_ptr->mm_id.asubs_id, CM_CALL_TYPE_VOICE, call_ptr->call_id) != CM_CALL_ID_INVALID) ||
               (cmcall_is_there_a_call_type_no_call_id_per_sub(call_ptr->mm_id.asubs_id, CM_CALL_TYPE_VT, call_ptr->call_id) != CM_CALL_ID_INVALID))
             )
      {
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_BUSY);
        CM_MSG_HIGH_0("ue_based-cw, reject incoming call");

        cmcall_send_mt_call_end_auto_reject_ind(
            cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
            call_ptr->call_type,
            CM_IP_SIP_ERROR_CODE_NONE,
            CM_CALL_END_INCOM_REJ_CAUSE_CALL_ONGOING_CW_DISABLED,
            cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,
            call_ptr->cmcall_mode_info.info_type,
            &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
             call_ptr->mt_call_verstat.call_verstat);

        call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      }

             
      /*If CB is set to UE_based and call_barring is active enabled*/
      else if( (volte_ptr->ue_based_sups[baic_idx].use_ue_based_sups == TRUE &&   //barringOfIncomingCalls and baic
                volte_ptr->ue_based_sups[baic_idx].is_enabled == TRUE )||
      
               (volte_ptr->ue_based_sups[bicRoam_idx].use_ue_based_sups == TRUE &&   //bicRoam
                volte_ptr->ue_based_sups[bicRoam_idx].is_enabled == TRUE &&
                !sys_mcc_same_country(home_mcc,current_mcc)))          
      {
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_CALL_BARRING);
        CM_MSG_HIGH_0("ue_based_cb enabled and call_barring is active, reject incoming call");
      
        cmcall_send_mt_call_end_auto_reject_ind(
            cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
            call_ptr->call_type,
            CM_IP_SIP_ERROR_CODE_NONE,
            CM_CALL_END_INCOM_REJ_CAUSE_CALL_ONGOING_CB_ENABLED,
            cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,
            call_ptr->cmcall_mode_info.info_type,
            &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
             call_ptr->mt_call_verstat.call_verstat);

        call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      }
      /*If CB is set to UE_based and call_barring is active enabled*/
      else if(volte_ptr->ue_based_sups[bain_idx].use_ue_based_sups && //bain
              volte_ptr->ue_based_sups[bain_idx].is_enabled &&
              cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.is_ip_pi_valid  == TRUE && 
              cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num.pi == TRUE)
      {
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_CALL_BARRING_ANONYMOUS);
        CM_MSG_HIGH_0("ue_based_anonymous_cb enabled and call_barring is active, reject incoming call");

        cmcall_send_mt_call_end_auto_reject_ind(
            cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
            call_ptr->call_type,
            CM_IP_SIP_ERROR_CODE_NONE,
            CM_CALL_END_INCOM_REJ_CAUSE_CALL_ONGOING_CB_ENABLED,
            cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,
            call_ptr->cmcall_mode_info.info_type,
            &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
            call_ptr->mt_call_verstat.call_verstat);

        call_ptr->call_subst.incom = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      } 
     
      /* Check if GW CS call is in origination state then reject the vowlan call
      ** This is required to handle the race cond where CM is processing the GW CS call(is in orig state)
      ** and IMS is not aware of this call . So if CM receives any MT call from IMS in this duration,
      ** it needs to be rejected.
      */
      else if(call_ptr->is_vowlan == TRUE && !cmcall_is_no_call_in_gw_cs_per_subs(call_ptr->mm_id.asubs_id))
      {
        CM_MSG_HIGH_0("GW CS voice call already exists.");
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_BUSY);

        cmcall_send_mt_call_end_auto_reject_ind(
            cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
            call_ptr->call_type,
            CM_IP_SIP_ERROR_CODE_NONE,
            CM_CALL_END_CAUSE_CS_CALL_ONGOING,
            cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,
            call_ptr->cmcall_mode_info.info_type,
            &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
             call_ptr->mt_call_verstat.call_verstat);
        
        call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      }

      /* Reject call depending on call activity on the other sub
      */
      #ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
      else if(cmcall_msim_is_cross_sub_concurrency_forbidden(call_ptr, FALSE))
      #else
      else if(!cmcall_is_cross_sub_concurrency_allowed(call_ptr->call_type, call_ptr->mm_id.asubs_id, FALSE))
      #endif
      {
        CM_MSG_HIGH_0("Reject incoming call due to call activity on the other sub");
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_CALL_ON_OTHER_SUB);
    	cmcall_send_mt_call_end_auto_reject_ind(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,\
                                                call_ptr->call_type,\
                                                CM_IP_SIP_ERROR_CODE_NONE,\
                                                CM_CALL_END_INCOM_REJ_CAUSE_CALL_ON_OTHER_SUB,\
                                                cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,\
                                                call_ptr->cmcall_mode_info.info_type,
                                                &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
                                                call_ptr->mt_call_verstat.call_verstat);

         call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      }

      /* If there is no existing CDMA voice/emerg call, accept the VoLTE MT call
      */
      else if((call_id = cmcall_find_active_cs_call_per_sub(call_ptr->mm_id.asubs_id)) == CM_CALL_ID_INVALID ||
              (cmcall_ptr(call_id) != NULL &&
               cmcall_ptr(call_id)->cmcall_mode_info.info_type != CM_CALL_MODE_INFO_CDMA))
      {
        /* Set up indication to UI followed by setup response from UI was
        ** designed to mirror GW way of handling incoming calls. With 2 clients
        ** ICM and ITelephone being able to handle setup indications it can
        ** happen that ITel sends setup response with FALSE even before ICM
        ** can respond with TRUE. Final Verdict is to remove sending INVITE
        ** to CM from ip app. Intermediate fix is to make CM send setup
        ** response.
        */

        /* cmcall_event( call_ptr, CM_CALL_EVENT_SETUP_IND ); */

        /* Call the IP call funtion for set up response
        */
        CM_MSG_HIGH_1("Accept incoming call, call_id %d", call_id);
        cmipcall_invite_res(call_ptr, TRUE, CMIPAPP_INCOM_REJ_CAUSE_NONE);

        /* Change incom substate to setup processed.
        */
        call_ptr->call_subst.incom = CM_CALL_INCOM_SUBST_SETUP_PROCESSED;

        #if defined(FEATURE_HA_MAINLINE_ONLY_ENABLE)
        if(cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info.info_included)
        {
          call_ptr->call_subst.incom = CM_CALL_INCOM_SUBST_PRE_RINGING;
          cmcall_event(call_ptr, CM_CALL_EVENT_PRE_INCOM);
        }
        #endif

        if(call_ptr->sys_mode == SYS_SYS_MODE_LTE)
        {
          cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_VOLTE_CALL,
                                                call_ptr->mm_id.asubs_id,
                                                call_ptr->mm_id.stk_id,
                                                TRUE);
        }
        cmcall_check_and_send_mmtel_status(call_ptr,TRUE);

        /* For MO/MT wifi call, start the special mapping on the call sub
        */
        if(call_ptr->sys_mode == SYS_SYS_MODE_WLAN)
        {
          cmph_update_sp_special_priority_mode(FALSE, TRUE, call_ptr->mm_id.asubs_id);
        }

#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
        {
          cmph_sub_info_s_type *ph_sub_config_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);
          /*Start MT CS Page Reject Timer*/
          if(ph_sub_config_ptr == NULL ||
              !ph_sub_config_ptr->mt_cs_page_reject_param.allow_cs_page_during_mt_volte_call ||
              (cmcall_is_ip_call_originated () != CM_CALL_ID_INVALID))
          {
            CM_MSG_HIGH_1("ERROR:mt_cs_page_reject timer not started for sub[%d]",call_ptr->mm_id.asubs_id);
          }
          /*MT INVITE while another MT INVITE in progress is not expected.
            MT INVITE while MO VOLTE call is getting connected is not expected.
            MT INVITE while MO VOLTE call is connected, follow legacy behavior*/
          else
          {
            cmipcall_set_mt_cs_reject_timer(time_get_uptime_secs()+ph_sub_config_ptr->mt_cs_page_reject_param.mt_cs_page_reject_duration
                                            ,call_ptr->mm_id.asubs_id);
            CM_MSG_HIGH_2("mt_cs_page_reject started. uptime:%d,mt_cs_page_reject_uptime:%d",
                          time_get_uptime_secs(),ph_sub_config_ptr->mt_cs_page_reject_uptime);
          }
        }
#endif
        /*MT invite notify cmkpi
        */
        if(call_ptr->call_type == CM_CALL_TYPE_VOICE || call_ptr->call_type == CM_CALL_TYPE_EMERGENCY)
        {
           cmkpi_qsh_event_handler(CM_CALL_EVENT_ORIG, call_ptr);
        }
      }
      else
      {
        /* 1x voice/emerg call existing. Reject this VoLTE MT
        */
        CM_MSG_HIGH_0("Reject incoming call, 1x voice/emerg ongoing");
        cmipcall_invite_res(call_ptr, FALSE, CMIPAPP_INCOM_REJ_CAUSE_1X_COLLISION);

    	cmcall_send_mt_call_end_auto_reject_ind(
           cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,\
           call_ptr->call_type,\
           CM_IP_SIP_ERROR_CODE_NONE,\
           CM_CALL_END_INCOM_REJ_CAUSE_1X_COLLISION,\
           cmipapp_rpt_ptr->ip_ind.ind.mt_invite.num,
           call_ptr->cmcall_mode_info.info_type,
           &cmipapp_rpt_ptr->ip_ind.ind.mt_invite.call_attrib_info.call_composer_info,
            call_ptr->mt_call_verstat.call_verstat);

        call_ptr->call_subst.incom               = CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT;
      }

      /* Release ref after sending auto reject or pre_incom event */
      REF_CNT_OBJ_RELEASE_IF(
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.picture_url);
      REF_CNT_OBJ_RELEASE_IF(
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.subject);
	  REF_CNT_OBJ_RELEASE_IF(
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.org_header);
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.subject 
        = NULL;
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.picture_url 
        = NULL;
	   call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.org_header = NULL;

      /* Clear call composer info from call ptr after sending auto reject or pre_incom event */
      memset(&call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info,
             0, sizeof(cm_ip_call_composer_info_s_type));

      /* Terminate periodic search for MT INVITE over LTE
      */
      if((cmutil_is_periodic_manual_search()) &&
         (call_ptr->sys_mode == SYS_SYS_MODE_LTE)&&
         (call_ptr->call_subst.incom != CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT))
      {
        CM_MSG_HIGH_0("GET_NET: PERIODIC: TERMINATE: MT invite over LTE");
        cmph_terminate_periodic_get_networks_req_only(call_ptr->mm_id.asubs_id);
      }
    }

    break;

    case CM_IP_CALL_IND_MT_RING_CNF:
    {

      /*
      ** Call id should be requested by IP app before sending any reports
      ** to CM.
      */
      cm_call_type_e_type new_call_type;
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.id );

      CM_MSG_HIGH_5("IP RXD: MT_RING_CNF, id=%d, sys_mode=%d, as_id=%d, MT_TN_verstat_info= %d, is_nw_support_unwanted_call= %d",
                    cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.id,
                    cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.sys_mode,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                    cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_verstat,
                    cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.is_nw_support_unwanted_call);

      if (call_ptr == NULL)
      {
        CM_ASSERT( call_ptr != NULL );
        CM_ERR_0 ( "ERROR: Unknown call ID in MT_RING_CNF");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      /*------------------------------------------------------------------*/

      /* Call state should be incoming and subst should be setup_processed
      ** for alerting to be processed.
      */
      if (call_ptr->call_state       == CM_CALL_STATE_INCOM
          &&
          (call_ptr->call_subst.incom == CM_CALL_INCOM_SUBST_SETUP_PROCESSED ||
           call_ptr->call_subst.incom == CM_CALL_INCOM_SUBST_PRE_RINGING)
        )
      {

        call_ptr->call_subst.incom = CM_CALL_INCOM_SUBST_ALERTING;

        call_ptr->sys_mode   = cmipapp_map_sysmode_to_cm_sysmode (
                                 cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.sys_mode);

#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
        {
          /*Stop MT CS Page Reject Timer*/
          cmph_sub_info_s_type *ph_sub_config_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);
          if(ph_sub_config_ptr != NULL)
          {
            cmipcall_set_mt_cs_reject_timer(0,call_ptr->mm_id.asubs_id);
            CM_MSG_HIGH_1("mt_cs_page_reject timer reset for sub[%d]",call_ptr->mm_id.asubs_id);
          }
        }
#endif

        cmph_call_start( call_ptr );

      }
      else if(call_ptr->call_subst.incom != CM_CALL_INCOM_SUBST_ALERTING)
      {

        CM_ERR_0( "Rcvd CM_IP_CALL_IND_MT_RING_CNF out of seq.");
        call_ptr->end_status = CM_CALL_END_INCOM_CALL;
        cmcall_end( call_ptr );
        return;

      }

      /*------------------------------------------------------------------*/

      /* Call type is carried by MT_RING_CNF as well.
         this is required as call_type may change between MT_INVITE and MT_RING_CNF
         It is IMS responsibility to send the correct call type always
      */
      new_call_type = cmipcall_map_calltype_to_cm_calltype (
                        cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_type);
      if(call_ptr->call_type != new_call_type)
      {
        CM_MSG_HIGH_2("call type varied between MT_INVITE %d and MT_RING_CNF %d",call_ptr->call_type, new_call_type);
        call_ptr->call_type  = new_call_type;
      }

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.is_secure_call =
        cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.is_secure_call;

      CM_MSG_HIGH_1("is_secure_call %d", cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.is_secure_call);

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

	  /* Copy the unwanted_robocall_call features from ll	 
	  */
	  call_ptr->mt_call_verstat.is_nw_support_unwanted_call = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.is_nw_support_unwanted_call;
	  call_ptr->mt_call_verstat.call_verstat = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_verstat;


      /*------------------------------------------------------------------*/

      if(call_ptr->is_vowlan == FALSE)
      {
        (void)cmph_update_orig_param_incoming_call(call_ptr);
      }

      call_ptr->tir_mode = cmipapp_get_TIR_mode(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.rtt_mode = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_attrib.rtt_mode;
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.audio_attrib = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_attrib.audio_attrib;
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.video_attrib = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_attrib.video_attrib;

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.crs_info.crs_type = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_attrib.crs_info.crs_type;
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.crs_info.original_call_type = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.call_attrib.crs_info.original_call_type;
      call_ptr->cmcall_mode_info.info.ip_info.silent_ui = cmipapp_rpt_ptr->ip_ind.ind.mt_ring_cnf.silent_ui;

      cmcall_event( call_ptr, CM_CALL_EVENT_INCOM );

      /* delete the reference which is stored at MT invite after sending INCOM event.
      ** QMI should add reference to the buf when receive INCOM event.
      ** answer cmd will pass in new ip_gen_param
      */
      REF_CNT_OBJ_RELEASE_IF(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param);

      /* delete the History Info reference which is stored at MT invite after sending INCOM event.
      ** QMI should add reference to the buf when receive INCOM event.
      */
      REF_CNT_OBJ_RELEASE_IF(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info);

      /* Since the Event has been sent, we are not storing the History Info in Call Object */
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info = NULL;

      /* delete the Alert Info reference which is stored at MT invite after sending INCOM event.
      ** QMI should add reference to the buf when receive INCOM event.
      */
      REF_CNT_OBJ_RELEASE_IF(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.alert_info);

      /* Since the Event has been sent, we are not storing the Alert Info in Call Object */
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.alert_info = NULL;

      CM_MSG_HIGH_2("IP RXD: MT_RING_CNF, free alert_info id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_update.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      if (call_ptr->cmcall_mode_info.info.ip_info.caller_info != NULL)
      {
        CM_MSG_HIGH_0("Released ref for caller_info");
        REF_CNT_OBJ_RELEASE_IF(call_ptr->cmcall_mode_info.info.ip_info.caller_info);
      }
    }

    break;

    case CM_IP_CALL_IND_CALL_UPDATE_INFO:

      CM_MSG_HIGH_2("IP RXD: CALL_UPDATE_INFO, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_update.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* get the call pointer from the call ID and check for its validity */
      call_ptr = cmcall_ptr(cmipapp_rpt_ptr->ip_ind.ind.call_update.id);

      if (call_ptr == NULL)
      {
        CM_ASSERT( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call update info");

        return;
      }

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.caller_name =
        cmipapp_rpt_ptr->ip_ind.ind.call_update.caller_name;

      /* For adversarial scenarios, logging caller name len/text*/
      if(cmipapp_rpt_ptr->ip_ind.ind.call_update.caller_name == NULL)
      {
        CM_MSG_HIGH_0("caller_name not allocated. Something Wrong!");
      }
      else
      {
        if((cmipapp_rpt_ptr->ip_ind.ind.call_update.caller_name->caller_name_len == 0) ||
            (cmipapp_rpt_ptr->ip_ind.ind.call_update.caller_name->caller_name_text == NULL))
        {
          CM_MSG_HIGH_2("caller_name_len %d and caller_name_text %d. Something Wrong!",
                        cmipapp_rpt_ptr->ip_ind.ind.call_update.caller_name->caller_name_len,
                        cmipapp_rpt_ptr->ip_ind.ind.call_update.caller_name->caller_name_text);
        }
      }

      /* Processing of Caller Info and then sending this Info to registered clients (e.g. QMI-V) */
      cmcall_event( call_ptr, CM_CALL_EVENT_CNAP_INFO_RECEIVED);

      /* Since the Event has been sent, we are not storing the Caller Name in Call Object */
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.caller_name = NULL;

      break;

    case CM_IP_CALL_IND_BARRING:

      CM_MSG_HIGH_2("IP RXD: BARRING_IND, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_barring.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* get the call pointer from the call ID and check for its validity */
      call_ptr = cmcall_ptr(cmipapp_rpt_ptr->ip_ind.ind.call_barring.id);

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in barring ind");

        return;
      }

      /* Processing of Caller Info and then sending this Info to registered clients (e.g. QMI-V) */
      cmcall_event( call_ptr, CM_CALL_EVENT_CALL_BARRED);

      break;

    case CM_IP_CALL_IND_FORWARD_INFO:

      CM_MSG_HIGH_2("IP RXD: FORWARD_INFO, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_forward.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* get the call pointer from the call ID and check for its validity */
      call_ptr = cmcall_ptr(cmipapp_rpt_ptr->ip_ind.ind.call_forward.id);

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in fwd info");

        return;
      }

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info =
        (cm_ip_history_info_s_type *)cmipapp_rpt_ptr->ip_ind.ind.call_forward.forward_info;

      /* For adversarial scenarios, logging caller name len/text*/
      if(cmipapp_rpt_ptr->ip_ind.ind.call_forward.forward_info == NULL)
      {
        CM_MSG_HIGH_0("forward_info not allocated. Something Wrong!");
      }
      else
      {
        if((cmipapp_rpt_ptr->ip_ind.ind.call_forward.forward_info->hist_info_len == 0) ||
            (cmipapp_rpt_ptr->ip_ind.ind.call_forward.forward_info->hist_info_text == NULL))
        {
          CM_MSG_HIGH_2("hist_info_len %d and hist_info_text %d. Something Wrong!",
                        cmipapp_rpt_ptr->ip_ind.ind.call_forward.forward_info->hist_info_len,
                        cmipapp_rpt_ptr->ip_ind.ind.call_forward.forward_info->hist_info_text);
        }
      }

      /* Sending this Forward Info to registered clients (e.g. QMI-V) */
      cmcall_event( call_ptr, CM_CALL_EVENT_CALL_BEING_FORWARDED);

      /* Since the Event has been sent, we are not storing the History Info in Call Object */
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info = NULL;

      break;

    case CM_IP_CALL_IND_MO_PROGR_INFO:

      CM_MSG_HIGH_2("IP RXD: MO_PROGR_INFO, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* get the call pointer from the call ID */
      call_ptr = cmcall_ptr (cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.id);

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in progr info");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);
      /*------------------------------------------------------------------*/

      /* Copy the number, call is being made from
      */
      if ( cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.num.buf[0] != 0 )
      {
        call_ptr->num.len = memscpy( call_ptr->num.buf,
                                     sizeof(call_ptr->num.buf),
                                     cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.num.buf,
                                     cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.num.len ) ;
      }

      /* Check for the call state to be in origination
      */

      if (call_ptr->call_state != CM_CALL_STATE_ORIG)
      {
        CM_MSG_HIGH_0 ("Progress info for call not in CM_CALL_STATE_ORIG");
        return;
      }
      else
      {
        if(call_ptr->is_vowlan)
        {
          CM_MSG_HIGH_2("sys_mode Change from %d to %d",call_ptr->sys_mode, SYS_SYS_MODE_WLAN);
          call_ptr->sys_mode = SYS_SYS_MODE_WLAN;
        }
      }

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.is_secure_call =
        cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.is_secure_call;

      CM_MSG_HIGH_3("is_secure_call %d is_mo_prog_ind_for_180_ringing %d peer_cap.rtt_mode %d",
                    cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.is_secure_call,
                    cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.is_mo_prog_ind_for_180_ringing,
                    cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.peer_call_cap_attrib.rtt_mode);

      if( cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.progr == CMIPAPP_PROG_SESSION ||
          cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.progr == CMIPAPP_PROG_MO_RINGBACK )
      {
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info =
          cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.history_info;

        /* For adversarial scenarios, logging history info len/text*/
        if(cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.history_info != NULL)
        {
          if((cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.history_info->hist_info_len == 0) ||
              (cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.history_info->hist_info_text == NULL))
          {
            CM_MSG_HIGH_2("hist_info_len %d and hist_info_text %d. Something Wrong!",
                          cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.history_info->hist_info_len,
                          cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.history_info->hist_info_text);
          }
        }
      }

      /*------------------------------------------------------------------*/
      cmkpi_qsh_event_handler(CM_CALL_EVENT_PROGRESS_INFO_IND, call_ptr);

     CM_MSG_HIGH_4("MO_PROGR_INFO, call_progress %d, call_comp peer %d, notif_type %d, notif_reason %d",
                   cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.progr,
                   cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.peer_call_cap_attrib.call_composer_supported,
                   cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.prog_info_notif.notif_type,
                   cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.prog_info_notif.notif_reason);

      switch (cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.progr)
      {
        case CMIPAPP_PROG_SESSION:
          ip_ptr                 = &call_ptr->cmcall_mode_info.info.ip_info;
          ip_ptr->call_prog_info = CM_CALL_PROG_INFO_LOCAL_ALERT;
          ip_ptr->ipcall_peer_cap_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.peer_call_cap_attrib;
          ip_ptr->ipcall_prog_info_notif
            = cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.prog_info_notif;
          cmcall_event( call_ptr, CM_CALL_EVENT_PROGRESS_EARLY_MEDIA );
          break;

        case CMIPAPP_PROG_FORWARD:
          cmcall_event( call_ptr, CM_CALL_EVENT_CALL_FORWARDED );
          break;

        case CMIPAPP_PROG_REDIRECT:
          cmcall_event( call_ptr, CM_CALL_EVENT_REDIRECTING_NUMBER );
          break;

        case CMIPAPP_PROG_MO_RINGBACK:
          ip_ptr                 = &call_ptr->cmcall_mode_info.info.ip_info;
          ip_ptr->call_prog_info = CM_CALL_PROG_INFO_REMOTE_ALERT;
          ip_ptr->ipcall_peer_cap_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.peer_call_cap_attrib;
          call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.rtt_mode = cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.call_attrib.rtt_mode;
          call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.audio_attrib= cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.call_attrib.audio_attrib;
          call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.video_attrib= cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.call_attrib.video_attrib;
          ip_ptr->ipcall_prog_info_notif = cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.prog_info_notif;
          call_ptr->call_subst.orig = CM_CALL_ORIG_SUBST_ALERTING;

          cmcall_event( call_ptr, CM_CALL_EVENT_PROGRESS_INFO_IND );
          break;

        case CMIPAPP_PROG_MO_CALL_WAITING:
          cmcall_event( call_ptr, CM_CALL_EVENT_CALL_IS_WAITING);
          break;

        default:
          CM_MSG_HIGH_1 ("mo_prog_info default case %d",
                         cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.progr);
          break;

      } /* switch */

      /* Since the Event has been sent, we are not storing the History Info in Call Object */
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info = NULL;

      if(cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.is_mo_prog_ind_for_180_ringing)
      {
        CM_MSG_HIGH_1 ("Rxd is_mo_prog_ind_for_180_ringing %d in MO_PROG_INFO_IND",
                       cmipapp_rpt_ptr->ip_ind.ind.mo_progr_info.is_mo_prog_ind_for_180_ringing);
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.is_180_ringing_rxd_in_mo_prog_ind = TRUE;

        cmcall_event( call_ptr, CM_CALL_EVENT_ABRV_ALERT);

        /* Need to inform RIL about call_alert indication only once, when 180 ring is receieved
           clearing call_ptr flag imemdiately to avoid sending the flag as TRUE in subsequent call events*/
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.is_180_ringing_rxd_in_mo_prog_ind = FALSE;
      }

      break;


    case CM_IP_CALL_IND_CONNECTED:
    {
      sys_modem_as_id_e_type rpt_asubs_id = SYS_MODEM_AS_ID_1;
      cmmsc_state_machine_s_type *state_machine;
      cmph_sub_info_s_type   *ph_sub_config_ptr;

      CM_MSG_HIGH_4_EXT("DS: SUB %d IP RXD: CONNECTED, id=%d, sys_mode=%d, as_id=%d",
                        cmipapp_rpt_ptr->ip_ind.hdr.asubs_id + 1,
                        cmipapp_rpt_ptr->ip_ind.ind.connected.id,
                        cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                        cmipapp_rpt_ptr->ip_ind.ind.connected.sys_mode,
                        (cmipapp_rpt_ptr->ip_ind.hdr.asubs_id+1));
      if (cmph_is_msim())
      {
        rpt_asubs_id = cmipapp_rpt_ptr->ip_ind.hdr.asubs_id;
      }
      state_machine = cmmsimc_state_machine_ptr(rpt_asubs_id);

      /* Get the call pointer from the call ID */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.connected.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in connected");

        return;
      }

      ph_sub_config_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);
      if(ph_sub_config_ptr == NULL)
      {
        CM_MSG_HIGH_1("cmph sub config pointer[%d] is NULL",call_ptr->mm_id.asubs_id);
        return;
      }

      /* sometimes MMODE might have switched from SRLTE -> SingleMode ater
      ** MT INVITE was sent and before CM_IP_CALL_IND_CONNECTED
      ** hence for such cases make sure we move the call to stack_0
      */
      if(cmph_is_subs_feature_mode_1x_sxlte(rpt_asubs_id) &&
          state_machine->stack_common_info.ue_mode == SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED &&
          cmss_is_stack2_operational(rpt_asubs_id) == FALSE)
      {
        call_ptr->mm_id.stk_id = cmcall_det_ss_from_sys_mode (call_ptr);
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      /* Get the system call is connected through
      */
      call_ptr->sys_mode =  cmipapp_map_sysmode_to_cm_sysmode (
                              cmipapp_rpt_ptr->ip_ind.ind.connected.sys_mode);



      CM_ASSERT( call_ptr != NULL );

      call_ptr->call_state        = CM_CALL_STATE_CONV;

      call_ptr->call_subst.conv   = CM_CALL_CONV_CALL_ACTIVE;

      call_ptr->call_connect_time = time_get_uptime_secs();

      if(!(call_ptr->special_emergency))
      {
        call_ptr->call_type = cmipcall_map_calltype_to_cm_calltype(cmipapp_rpt_ptr->ip_ind.ind.connected.call_type);
      }

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib;

      CM_MSG_HIGH_2("is_secure_call %d peer_cap.rtt_mode %d", cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.is_secure_call,
                    cmipapp_rpt_ptr->ip_ind.ind.connected.peer_call_cap_attrib.rtt_mode);

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_local_cap_attrib_info= cmipapp_rpt_ptr->ip_ind.ind.connected.local_call_cap_attrib;

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_peer_cap_attrib_info= cmipapp_rpt_ptr->ip_ind.ind.connected.peer_call_cap_attrib;

      CM_MSG_HIGH_4("is_secure_call %d peer_cap.rtt_mode %d, call_composer peer %d local %d", 
                     cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.is_secure_call,
                     cmipapp_rpt_ptr->ip_ind.ind.connected.peer_call_cap_attrib.rtt_mode,
                     cmipapp_rpt_ptr->ip_ind.ind.connected.peer_call_cap_attrib.call_composer_supported,
                     cmipapp_rpt_ptr->ip_ind.ind.connected.local_call_cap_attrib.call_composer_supported);

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info =
        cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.history_info;

      /* For adversarial scenarios, logging history info len/text*/
      if(cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.history_info != NULL)
      {
        if((cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.history_info->hist_info_len == 0) ||
            (cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.history_info->hist_info_text == NULL))
        {
          CM_MSG_HIGH_2("hist_info_len %d and hist_info_text %d. Something Wrong!",
                        cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.history_info->hist_info_len,
                        cmipapp_rpt_ptr->ip_ind.ind.connected.call_attrib.history_info->hist_info_text);
        }
      }

      call_ptr->cmcall_mode_info.info.ip_info.conf_is_enabled= cmipapp_rpt_ptr->ip_ind.ind.connected.conf_is_enabled;

      if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY)
      {
        // EMVT: Write a utility fn to convert and update is_em_vt;
        if(cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.em_type == CM_CALL_EM_VOICE)
        {
          call_ptr->is_em_vt = FALSE;
        }
        else if(cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.em_type == CM_CALL_EM_VT)
        {
          call_ptr->is_em_vt = TRUE;
        }
        else
        {
          CM_MSG_HIGH_1("Invalid em_type %d from IMS!",cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.em_type);
        }

        {
          mm_sub_stk_id_s_type local_mm_id;
          local_mm_id.asubs_id = call_ptr->mm_id.asubs_id;
          local_mm_id.stk_id = cmcall_det_ss_for_volte_call(call_ptr);
          sd_ss_ind_ims_emerg_call_state(local_mm_id,
                                         (sys_volte_call_state_e_type)CM_CALL_STATUS_CONNECTED);
        }
      }
      cmph_update_ddtm_status(call_ptr->mm_id.asubs_id);


      /*------------------------------------------------------------------*/

      cmcall_event( call_ptr, CM_CALL_EVENT_CONNECT );

      /* Since the Event has been sent, we are not storing the History Info in Call Object */
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.history_info = NULL;

      cmipcall_mng_state_update(CM_IP_CALL_IND_CONNECTED, TRUE, call_ptr->call_id, CM_IPAPP_CAUSE_NORMAL);

#ifdef  FEATURE_VOIP_E911_SUPPORT
      /* If the call is a mobile originated emergency call over EHRPD, queue an indication
      ** to CM to enter emergency callback mode
      */
      if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
          call_ptr->direction == CM_CALL_DIRECTION_MO &&
          call_ptr->sys_mode  == SYS_SYS_MODE_HDR)
      {
        emcb_s_type emcb_info;
        emcb_info.enter = TRUE;
        emcb_info.mm_id = call_ptr->mm_id;
        emerg_callbck_mode_cb(emcb_info);
      }
#endif

      /* After sending the CALL CONNECT event to clients
      ** if the call is a mobile originated emergency call over LTE IMS,
      ** enter emergency callback mode ( if Emergency call customization so indicates)
      */
      if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
          call_ptr->direction == CM_CALL_DIRECTION_MO &&
          call_ptr->sys_mode  == SYS_SYS_MODE_LTE &&
          ph_sub_config_ptr->emerg_call_prop.is_ecbm_required)
      {
        cmph_s_type   *ph_ptr = cmph_ptr();

        /* Store the RAT on which CM enters callback mode
        */
        ph_sub_config_ptr->emerg_call_prop.ecbm_rat = call_ptr->sys_mode;

        if (call_ptr->mm_id.stk_id == MM_STACK_ALL)
        {
          cmph_enter_emergency_cb(
            cmph_map_sub_stk_to_id(call_ptr->mm_id.asubs_id,MM_STACK_0), TRUE );
        }
        else
        {
          cmph_enter_emergency_cb(  call_ptr->mm_id, TRUE );
        }
        CM_MSG_HIGH_3_ASID("Enter ECBM: stk %d, LTE IMS rat%d ",
                           call_ptr->mm_id.asubs_id,
                           call_ptr->mm_id.stk_id,
                           ph_sub_config_ptr->emerg_call_prop.ecbm_rat);

      }

      /* WLAN ECBM */
      if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
          call_ptr->direction == CM_CALL_DIRECTION_MO &&
          ph_sub_config_ptr->emerg_call_prop.is_wlan_e911_call_supp &&
          call_ptr->sys_mode  == SYS_SYS_MODE_WLAN)
      {
        cmph_enter_emergency_cb_wlan(call_ptr->mm_id.asubs_id);
        CM_MSG_HIGH_2_ASID("Enter ECBM: stk %d, WLAN ECBM rat",
                           call_ptr->mm_id.asubs_id,
                           call_ptr->mm_id.stk_id);
      }
      /* - - - - - - - - - for WLAN <-> LTE HO - - - - - - - - - */
      /*If call is moving from WLAN -> LTE. set/reset corresponding flags*/
      if(call_ptr->is_vowlan && call_ptr->sys_mode == SYS_SYS_MODE_LTE)
      {
        call_ptr->is_vowlan = FALSE;
        switch(call_ptr->call_type)
        {
          case CM_CALL_TYPE_VOICE:
          case CM_CALL_TYPE_VT:
            call_ptr->is_volte = TRUE;
            break;
          default:
            break;
        }
        /* In case of Emergency call, if Stack maybe set to SS_MAX,
        ** Update call_ptr stack detail to latest LTE stack
        */
        call_ptr->mm_id.stk_id = cmph_determine_stk_per_rat(call_ptr->mm_id.asubs_id, SYS_SYS_MODE_LTE);
        cmph_update_orig_param_incoming_call(call_ptr);
      }


      /*
      ** New Emerg call object to be inserted into the pri-queue after the Emergency call over the WIFI is connected
      ** Force the incoming preferences only when the EFS "e911_wlan_to_lte_handover" is enabled. 
      ** This is needed so that Emergency scan can be started.  
      ** Updating the stack_id as the emergency call will have stack_id as max.  
      ** Notify, PM of the emergency call so that PM can be suspended
      */

    	
       if(
             
             cmph_is_e911_wlan_to_LTE_handover_enabled(call_ptr->mm_id.asubs_id) &&
             
         	 call_ptr->is_vowlan &&
          	 (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
        	 (call_ptr->call_state == CM_CALL_STATE_CONV) &&
        	 (call_ptr->sys_mode == SYS_SYS_MODE_WLAN))
         {
           call_ptr->mm_id.stk_id = cmph_determine_stk_per_rat(call_ptr->mm_id.asubs_id, SYS_SYS_MODE_LTE);
        
           cmpmprx_notify_emg_call_start();

           /*
           ** Updating the Proc type to VOLTE for the emerg call handover from WLAN to LTE
           */
           cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_VOLTE_CALL,
                                          call_ptr->mm_id.asubs_id,
                                          call_ptr->mm_id.stk_id,
                                          TRUE);

           cmph_update_orig_param_incoming_call(call_ptr);
         }


      /*If call is moving from LTE -> WLAN set/reset corresponding flags and run dom sel if there is pending one*/
      if((cmcall_is_there_volte_call_obj_per_sub(SYS_BM_64BIT(call_ptr->call_type), call_ptr->mm_id.asubs_id) != CM_CALL_ID_INVALID)&&
          call_ptr->sys_mode == SYS_SYS_MODE_WLAN)
      {
        cmsds_sub_prop_s_type   *sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(rpt_asubs_id);
        call_ptr->is_volte = FALSE;
        call_ptr->is_vowlan = TRUE;
#ifdef FEATURE_DOMAIN_SELECTION
        if(sds_sub_prop_ptr != NULL && sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr != NULL)
        {
          mm_sub_stk_id_s_type  mm_id;
          mm_id.asubs_id = rpt_asubs_id;
          mm_id.stk_id = cmph_determine_stk_per_rat(rpt_asubs_id, sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr->mode );
          cmsds_process_srv_ind(mm_id, sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr);
        }
#endif

        cmph_remove_act_id_and_force_orig_mode(call_ptr->mm_id,(cm_act_id_type)call_ptr->call_id, FALSE);
      }

    }
    break;


    case CM_IP_CALL_IND_CALL_END:
    {
      cmph_sub_info_s_type   *ph_sub_config_ptr;
      cmipapp_end_cause_e_type  ims_end_cause = cmipapp_rpt_ptr->ip_ind.ind.call_end.end_cause;
      cmsds_sub_prop_s_type *sds_sub_prop_ptr;
      volte_sub_info_s_type *volte_sub_info_ptr;
      #if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
      sd_ss_mode_pref_e_type  mode_pref_local =SD_SS_MODE_PREF_NONE;
      sd_ss_band_pref_e_type  band_pref_local = SD_SS_BAND_PREF_ANY;
      sys_lte_band_mask_e_type lte_band_pref_local = SYS_LTE_BAND_MASK_CONST_NONE;
      sd_ss_band_pref_e_type   tds_band_pref_local = SD_SS_BAND_PREF_ANY;
      sd_ss_mode_pref_e_type  supported_mode = SD_SS_MODE_PREF_NONE;
      cm_plmn_blocking_info_s_type  plmn_blocking_info;
      #endif
      boolean spl_emerg_hplmn = FALSE;
      mm_sub_stk_id_s_type    local_mm_id;


      /*to be used for calls that are re-dialed to IMS due to no CS coverage/domain
      ** present and to decide whether to end it or restrict to IMS */
      boolean is_end_call = FALSE;

      /* get the call pointer from the call ID */
      call_ptr = cmcall_ptr (cmipapp_rpt_ptr->ip_ind.ind.call_end.id);

      /*Call_end_indication with call composer is expected only when CM does not have call object
      */

      if (cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.info_included)
      {
        if(((cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.subject != NULL) ||
           (cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.picture_url != NULL)) &&
           (call_ptr != NULL)
          )
        {
          CM_ERR_FATAL_0("Unexpected Call composer info received during call end ind");
        }
      }

      if (call_ptr == NULL)
      {

        /* For MT call end, IMS will send the call_id as INVALID. 
        ** This is for the case in which IMS ends the call withouting sending invite to CM.
        */
        
        if(cmipapp_rpt_ptr->ip_ind.ind.call_end.id == CM_CALL_ID_INVALID)
        {
           /*
                ** Incoming call ended by IMS before reporting MT_INVITE to CM. 
                */
           CM_MSG_HIGH_2("IP RXD: CALL_END, no call_id exist, sub %d, call_composer incl %d",
                            cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                            cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.info_included);
       
           cmcall_send_mt_call_end_auto_reject_ind(
               cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
               cmipcall_map_calltype_to_cm_calltype
               (cmipapp_rpt_ptr->ip_ind.ind.call_end.call_type),
               cmipapp_rpt_ptr->ip_ind.ind.call_end.sip_error_code,
               cmipcall_map_client_end_status
               (cmipapp_rpt_ptr->ip_ind.ind.call_end.client_end_cause),
               cmipapp_rpt_ptr->ip_ind.ind.call_end.num,
               CM_CALL_MODE_INFO_IP,
               &cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info,
               cmipapp_rpt_ptr->ip_ind.ind.call_end.call_verstat);
        }
        else
        {

		/*If CM is waiting for IMS clnup confirmation for call end, CM will not lower the TRM as we may  hit CR2193788 issue again*/
           #ifdef FEATURE_MMODE_DUAL_SIM
           cmph_check_and_reduce_trm_priority(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);
	   #endif
          {
          CM_MSG_MED_0("call exists when receiving IMS END IND, ignore!");
           }
         }

       /* CM shall release reference for subject and picture URL after sending 
        ** MM_CM_MT_CALL_END_AUTO_REJECT_IND via MSGR.
        */
        if(cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.subject != NULL)
        {
          ref_cnt_obj_release(cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.subject);
        }
        
        if(cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.picture_url != NULL)
        {
          ref_cnt_obj_release(cmipapp_rpt_ptr->ip_ind.ind.call_end.call_composer_info.picture_url);
        }

        return;
      }

	  /* If N/W abnormally brings down voice/video bearers , call type can get changed in call end indication */
      /* If Call has been SRVCCed to CS Domain then the call end on IMS shouldnt update the call params*/
	 if(!call_ptr->special_emergency &&
	   	 (call_ptr->call_type == CM_CALL_TYPE_VOICE ||
	   	  call_ptr->call_type == CM_CALL_TYPE_VT) &&
	   	 (cmipapp_rpt_ptr->ip_ind.ind.call_end.call_type == CMIPAPP_CALL_TYPE_VOICE ||
          cmipapp_rpt_ptr->ip_ind.ind.call_end.call_type == CMIPAPP_CALL_TYPE_VT) &&
         (ims_end_cause != CMIPAPP_END_CAUSE_SRVCC_LTE_TO_CS)
         )
      {
		if(call_ptr->call_type != cmipcall_map_calltype_to_cm_calltype(cmipapp_rpt_ptr->ip_ind.ind.call_end.call_type))
		{
          CM_MSG_HIGH_4("CALL_END: call_type chgd: %d->%d, call_id %d, sub %d",
                         call_ptr->call_type, 
                         cmipcall_map_calltype_to_cm_calltype(cmipapp_rpt_ptr->ip_ind.ind.call_end.call_type),
                         call_ptr->call_id,
                         call_ptr->mm_id.asubs_id);

          call_ptr->call_type = cmipcall_map_calltype_to_cm_calltype(cmipapp_rpt_ptr->ip_ind.ind.call_end.call_type);
	    }
        
      }

      CM_MSG_HIGH_6_ASID_EXT("IP RXD: CALL_END, end_cause=%d, client_end_cause=%d, call_id=%d, call_state=%d, is_lte_hard_failure=%d",
                             call_ptr->mm_id.asubs_id,
                             cmipapp_rpt_ptr->ip_ind.ind.call_end.end_cause,
                             cmipapp_rpt_ptr->ip_ind.ind.call_end.client_end_cause,
                             cmipapp_rpt_ptr->ip_ind.ind.call_end.id,
                             call_ptr->call_state,
                             call_ptr->sr.voip_sr.is_lte_hard_failure,
                             (call_ptr->mm_id.asubs_id+1));
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
      CM_MSG_HIGH_4_ASID("IP RXD: CALL_END, is_ims_redial_allowed = %d, is_unwanted_call %d, call_verstat %d",
                         call_ptr->mm_id.asubs_id,
                         cmipapp_rpt_ptr->ip_ind.ind.call_end.is_ims_redial_allowed,
                         cmipapp_rpt_ptr->ip_ind.ind.call_end.is_unwanted_call,
                         cmipapp_rpt_ptr->ip_ind.ind.call_end.call_verstat);
#else
      CM_MSG_HIGH_3_ASID("IP RXD: CALL_END, is_ims_redial_allowed = %d, is_unwanted_call %d",
                         call_ptr->mm_id.asubs_id,
                         cmipapp_rpt_ptr->ip_ind.ind.call_end.is_ims_redial_allowed,
                         cmipapp_rpt_ptr->ip_ind.ind.call_end.is_unwanted_call);
#endif

      ph_sub_config_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);
      sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(call_ptr->mm_id.asubs_id);
      volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(call_ptr->mm_id.asubs_id);
      local_mm_id.asubs_id = call_ptr->mm_id.asubs_id;
      local_mm_id.stk_id = MM_STACK_0;


      if(ph_sub_config_ptr == NULL || volte_sub_info_ptr == NULL )
      {
        CM_MSG_HIGH_4_ASID("IP RXD: CALL_END, Null Ptr, ph_sub_config_ptr %d, sds_sub_prop_ptr %d volte_sub_info_ptr %d",
                           call_ptr->mm_id.asubs_id,
                           ph_sub_config_ptr,
                           sds_sub_prop_ptr,
                           volte_sub_info_ptr);
        return;
      }
      
      call_ptr->is_ip_orig_sent = FALSE;
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
      call_ptr->mt_call_verstat.call_verstat = cmipapp_rpt_ptr->ip_ind.ind.call_end.call_verstat;
#endif
      /* In case IMS ends IMS call and there is a buffered srv ind, reset ext_ims_call_active
      ** We will process buffered srv ind at call end, which might lead to SRLTE switch. To perform
      ** SRLTE switch, we need to reset ext_ims_call_active.
      */
#ifdef FEATURE_DOMAIN_SELECTION
      if(sds_sub_prop_ptr != NULL && sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr != NULL &&
          call_ptr->direction == CM_CALL_DIRECTION_MT &&
          sds_sub_prop_ptr->gen_sub_info.ext_ims_call_active == TRUE )
      {
        CM_MSG_HIGH_0("EXT_IMS_CALL: Reset ext_ims_call_active as IMS indicated call end for MT call");
        sds_sub_prop_ptr->gen_sub_info.ext_ims_call_active = FALSE ;
      }
#endif

     /* copy is_ims_redial_allowed value from IMS */
     call_ptr->sr.voip_sr.is_ims_redial_allowed = cmipapp_rpt_ptr->ip_ind.ind.call_end.is_ims_redial_allowed;

	 
#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)

     if(ims_end_cause == CMIPAPP_END_CAUSE_FALLBACK_TO_CS && sd_ss_is_emergency_psap_cb_mandatory(local_mm_id) &&
        call_ptr->call_type == CM_CALL_TYPE_EMERGENCY
     )
     {
       cmcall_set_lte_plmn_before_csfb_for_ims_state_deregistered(call_ptr,ims_end_cause);
     }
#endif
     /* If emergency call ended with reason CMIPAPP_END_CAUSE_ALTERNATE_EMERGENCY_CALL(381) :
        1. If UE is combined attach && if cs-ps redial hasn't happened or UE is not attached do CSFB
        2. if the CS domain is not available to the UE or the emergency call has already been attempted using the CS domain,
        then add the current PLMN to plmn avoid list, notify the list to LRRC and force LTE only mode to SD.
     */
     #if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)

     /* If the IMS End Cause is CMIPAPP_END_CAUSE_ALTERNATE_EMERGENCY_CALL, then we need to
      ensure that if we are having a special emergency resolved to be Normal Call, then we do not
      originate a CSFB Call , rather originate a emergency call */

     spl_emerg_hplmn = cmcall_check_spl_emergency_on_hplmn();
     
     if((CM_CALL_TYPE_EMERGENCY == call_ptr->call_type && CMIPAPP_END_CAUSE_ALTERNATE_EMERGENCY_CALL == ims_end_cause
        && !call_ptr->is_vowlan && CM_CALL_STATE_ORIG == call_ptr->call_state && !spl_emerg_hplmn))
     {
       if(cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG)
       {
         call_ptr->is_sip380_std_emc_categ = FALSE;
       }
       /* Do not copy NW reported emerg_srv_category if the category is NON_STANDARD and
       ** dialed number is UE detected number. Instead use original srv_category from call_control.
       */
       if(!((call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
             (cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG))
       )
       {
         call_ptr->num.emerg_srv_categ = cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ;
         call_ptr->is_sip380_std_emc_categ = TRUE;
       }

       mode_pref_local = cmph_map_cm_mode_pref_to_sd_mode_pref(call_ptr->favored_mode_pref);
       band_pref_local = cmutil_map_cm_band_pref_to_sd_band_pref(CMPH_SS_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr));
       lte_band_pref_local = CMPH_SS_LTE_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr);
       tds_band_pref_local = cmutil_map_cm_band_pref_to_sd_band_pref(CMPH_SS_TDS_BAND_PREF(call_ptr->mm_id.asubs_id, ph_ptr));
       supported_mode = sd_ss_get_supp_mode_pref(mode_pref_local, band_pref_local, lte_band_pref_local, tds_band_pref_local);

       if(!call_ptr->is_emerg_cs_attempted &&
          //check if target is capable of CS   
          (supported_mode & SD_SS_MODE_PREF_CDMA_GW_TDS) != SD_SS_MODE_PREF_NONE
         )
       {
         ims_end_cause = CMIPAPP_END_CAUSE_FALLBACK_TO_CS;

         /* If CSFB is not possible. avoid/block the current plmn */
         if(CM_CALL_CSFB_TYPE_NONE == cmsds_get_csfb_type_allowed(call_ptr->mm_id.asubs_id))
         {

           /* If emergency attach support over 380 PLMN is configured, send plmn block to NAS.
                        it will ensure, LRRC can still camp on 380 plmn and emergency attach is possible.
                        if MC sends avoid list to LRRC< on 380 plmn even emergency attach is not possible and will
                        result in emergency call failures when only 380 plmn is availble.
                    */
           if(ph_sub_config_ptr->emerg_call_prop.wifi_e911_wwan_full_srv)
           {
             plmn_blocking_info.asubs_id = call_ptr->mm_id.asubs_id;
             plmn_blocking_info.cause    = SYS_BLOCK_PLMN_CAUSE_EMC_IN_LSM;
             plmn_blocking_info.request_pref = CM_PLMN_BLOCKING_PREF_PLMN_BLOCKING_ONLY;
             plmn_blocking_info.blocking_interval = 0;

             plmn_blocking_info.plmn  = cmss_true_info_ptr()->sub_info[call_ptr->mm_id.asubs_id]->si_info[MM_STACK_0]->sid.plmn.plmn_id;
             (void)cm_ph_cmd_detach_and_plmn_blocking(
               NULL,
               NULL,
               CM_CLIENT_ID_ANONYMOUS,
               plmn_blocking_info);
           
             cmlog_plmn_block_req(sds_sub_prop_ptr->gen_sub_config.lte_disable_duration_nv,
                                  (plmn_blocking_info.blocking_interval/1000),
                                  cmsds_get_t3402_value(call_ptr->mm_id.asubs_id),
                                  call_ptr->mm_id.asubs_id);

           }
           else
           {
             cm_avoid_list_init(call_ptr->mm_id.asubs_id);
           
             /*Add current plmn into the avoid plmn list.*/
             if(cmph_add_avoid_plmn_to_list(call_ptr->mm_id.asubs_id,
                 cmss_true_info_ptr()->sub_info[call_ptr->mm_id.asubs_id]->si_info[MM_STACK_0]->sid.plmn.plmn_id))
             {
               /*Send plmn list to LRRC*/
               cmph_send_plmn_list_to_lrrc(call_ptr->mm_id.asubs_id);
             }
             sd_ss_set_avoid_plmn_list_flag(call_ptr->mm_id.asubs_id,TRUE);
           }
         }
         else
         {
           /* When updating LTE PLMN before CSFB, update is_redial to TRUE - to allow
                  ** redial back on IMS in case no CS service is found */
           call_ptr->lte_plmn_before_csfb = cmss_true_info_ptr()->sub_info[call_ptr->mm_id.asubs_id]->si_info[MM_STACK_0]->sid.plmn.plmn_id;
         }

         call_ptr->sr.voip_sr.is_ims_redial_allowed = TRUE;
         
         CM_MSG_HIGH_3("lte_plmn_before_csfb %d %d %d",call_ptr->lte_plmn_before_csfb.identity[0],call_ptr->lte_plmn_before_csfb.identity[1],
         call_ptr->lte_plmn_before_csfb.identity[2]);
       }

       else
       { 

         // Dectare a flag to mark its re originated over LTE because of 381
         call_ptr->is_381_for_emergency = TRUE;

       /* If emergency attach support over 380 PLMN is configured, send plmn block to NAS.
                it will ensure, LRRC can still camp on 380 plmn and emergency attach is possible.
                if MC sends avoid list to LRRC< on 380 plmn even emergency attach is not possible and will
                result in emergency call failures when only 380 plmn is availble.
                */
         if(ph_sub_config_ptr->emerg_call_prop.wifi_e911_wwan_full_srv)
         {         
           plmn_blocking_info.asubs_id = call_ptr->mm_id.asubs_id;
           plmn_blocking_info.cause    = SYS_BLOCK_PLMN_CAUSE_EMC_IN_LSM;
           plmn_blocking_info.request_pref = CM_PLMN_BLOCKING_PREF_PLMN_BLOCKING_ONLY;
           plmn_blocking_info.blocking_interval = 0;
  
           plmn_blocking_info.plmn  = cmss_true_info_ptr()->sub_info[call_ptr->mm_id.asubs_id]->si_info[MM_STACK_0]->sid.plmn.plmn_id;
           (void)cm_ph_cmd_detach_and_plmn_blocking(
             NULL,
             NULL,
             CM_CLIENT_ID_ANONYMOUS,
             plmn_blocking_info);
         
           cmlog_plmn_block_req(sds_sub_prop_ptr->gen_sub_config.lte_disable_duration_nv,
                                (plmn_blocking_info.blocking_interval/1000),
                                cmsds_get_t3402_value(call_ptr->mm_id.asubs_id),
                                call_ptr->mm_id.asubs_id);
  
         }
         /*Add current plmn into the avoid plmn list.*/
         else 
         {
           cm_avoid_list_init(call_ptr->mm_id.asubs_id);
           
           /*Send plmn list to LRRC*/
           if(cmph_add_avoid_plmn_to_list(call_ptr->mm_id.asubs_id,
                  cmss_true_info_ptr()->sub_info[call_ptr->mm_id.asubs_id]->si_info[MM_STACK_0]->sid.plmn.plmn_id))
           {
             cmph_send_plmn_list_to_lrrc(call_ptr->mm_id.asubs_id);
           }
         }
         
         call_ptr->end_status = CM_CALL_END_IP_RETRY;
         call_ptr->end_cause_to_client = CM_CALL_END_ALTERNATE_EMERGENCY_CALL;
         cmcall_orig_proc(call_ptr);
         break;
       }
     }
     #endif
#ifdef FEATURE_IP_CALL

      // If call is VoWLAN and if device is not in ONLINE mode,
      // then call should be ended insted of retrying over CSFB
      if (cmcall_vowlan_call_not_in_online_oprt_mode(call_ptr)         == TRUE &&
          cmipcall_should_call_redial_over_ip(call_ptr, ims_end_cause) == FALSE
         )
      {
        ims_end_cause = CMIPAPP_END_CAUSE_NORMAL;
        call_ptr->num.emerg_srv_categ = cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ;
      }

      if(ims_end_cause == CMIPAPP_END_CAUSE_FALLBACK_TO_CS)
      {
        call_ptr->sr.ignore_sr_max_uptime = TRUE;
        CM_MSG_HIGH_0("Set ignore_sr_max_uptime due to CSFB");
      }
#endif

      if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY)
      {
        mm_sub_stk_id_s_type local_mm_id;
        local_mm_id.asubs_id = call_ptr->mm_id.asubs_id;
        local_mm_id.stk_id = cmcall_det_ss_for_volte_call(call_ptr);
        sd_ss_ind_ims_emerg_call_state(local_mm_id,
                                       (sys_volte_call_state_e_type)CM_CALL_STATUS_END);

          CM_MSG_HIGH_2("IMS->CM: CALL_END: original_Call_type %d srv_Categ %d",call_ptr->original_call_type, cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ);
        /* 1.Current call type is emergency and original call type was VOICE/VT
        ** 2.Emerg_srv_categ is country specific that doesnt map to CS domain
        ** 3.End cause is fALLBACK_TO_CS 
        ** --> Change call type and restore SR component 
        */
        if((call_ptr->original_call_type == CM_CALL_TYPE_VOICE || call_ptr->original_call_type == CM_CALL_TYPE_VT) &&
           (ims_end_cause == CMIPAPP_END_CAUSE_FALLBACK_TO_CS) &&
            (cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG) &&
           !call_ptr->is_vowlan )
        {
          CM_MSG_HIGH_0("IMS->CM: CALL_END: Convert emergency call to voice call");
          cmipcall_convert_emergency_to_voice(call_ptr);
        }

      }

      /* If received IMS call end due to SRVCC drop the indication
      */
      if(ims_end_cause == CMIPAPP_END_CAUSE_SRVCC_LTE_TO_CS)
      {
        cmipapp_release_reference_end_reason(call_ptr);
        cmipapp_release_reference_alternate_uri(call_ptr);
        break;
      }

      /*Undetectable e911 call when getting 380, If 380 does not include Contact header, UE get CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL
      and attempt Normal call over CS domain. after Cs fails, redial over WLAN as emergency call if WLAN is available*/
      if((call_ptr->call_type == CM_CALL_TYPE_VOICE || call_ptr->call_type == CM_CALL_TYPE_VT) && 
         ims_end_cause == CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL)
      {
        call_ptr->is_convert_emerg_wlan = TRUE;
        CM_MSG_HIGH_0("Set is_convert_emerg_wlan to TRUE");
      }

      /* If received IMS call end due to CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL, change the call_type to VOICE
       **  or FALLBACK_TO_CS for call_type VT.
       */
      if((ims_end_cause == CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL ||
          ims_end_cause == CMIPAPP_END_CAUSE_FALLBACK_TO_CS) &&
           call_ptr->call_type == CM_CALL_TYPE_VT)
      {
        call_ptr->call_type = CM_CALL_TYPE_VOICE;
      }

      /* If received loopback end cause just send it back to IMS
      */
      if(ims_end_cause == CMIPAPP_END_CAUSE_LOOP_BACK)
      {
        uptime               = time_get_uptime_secs();
        sr_ptr               = &call_ptr->sr;

        cmipapp_release_reference_end_reason(call_ptr);
        cmipapp_release_reference_alternate_uri(call_ptr);

        CM_MSG_HIGH_0("CMIPAPP_END_CAUSE_LOOP_BACK");

        #if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
        CM_MSG_HIGH_6("WLAN_E911: call end status %d, orig_sent_on_mode %d, wifi_e911_support %d, wifi_e911_wwan_full_srv %d, uptime %d, fall_back_to_wifi_from_cs %d",
                  call_ptr->end_status,call_ptr->sr.orig_sent_on_mode,
                  ph_sub_config_ptr->emerg_call_prop.wifi_e911_support,
                  ph_sub_config_ptr->emerg_call_prop.wifi_e911_wwan_full_srv,
                  uptime,
                  sr_ptr->fall_back_to_wifi_from_cs);
    
        if(ph_sub_config_ptr->emerg_call_prop.wifi_e911_support &&
            call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
            cmcall_is_active_ip_app(SYS_SYS_MODE_WLAN, CM_CALL_TYPE_EMERGENCY, call_ptr->mm_id.asubs_id) &&
            (uptime >= sr_ptr->fall_back_to_wifi_from_cs) &&
            (call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_LTE)
          )
        {
          /*Timer has expired and call was previously attempted on LTE/NR5G
            (i)send to WIFI immediately if fallback timer expired
            (ii) or send on WIFI in case fallback timer expired while waiting for
                    ok to orig
          */
          CM_MSG_HIGH_0 ("convert to emerg call over WLAN as cellular redial timer expired.");
          call_ptr->call_type = CM_CALL_TYPE_EMERGENCY;
          sr_ptr->fall_back_to_wifi_from_cs = uptime;
          call_ptr->sr.orig_sent_on_mode = SYS_SYS_MODE_NONE;
          call_ptr->end_status = CM_CALL_END_NONE;
          call_ptr->sr.is_ok_to_orig = FALSE;
       
          cmcall_orig_proc(call_ptr);
          return;
        }
        else
        #endif
        {
          CM_MSG_HIGH_0("CMIPAPP_END_CAUSE_LOOP_BACK - ROUTING LTE/NR5G TO WLAN FAILED");
          cmipcall_send_orig(call_ptr);
        }

        break;
      }

      /* If received IMS call end due to it being identified as an emergency call - reoriginate the call
      */
      if(ims_end_cause == CMIPAPP_END_CAUSE_ALTERNATE_EMERGENCY_CALL)
      {
        /*
        ** Update the emerg_srv_categ as the call may originate as EMERG over CS.
        */
        if(call_ptr->call_type == CM_CALL_TYPE_VT)
        {
          CM_MSG_HIGH_0("Due to 380 rsp, set is_em_vt to TRUE!");
          call_ptr->is_em_vt = TRUE;
        }
        if(spl_emerg_hplmn )
        {
          call_ptr->special_emergency = FALSE;
        }
        if(cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG)
        {
          call_ptr->is_sip380_std_emc_categ = FALSE;
        }
        /* Do not copy NW reported emerg_srv_category if the category is NON_STANDARD and
        ** dialed number is UE detected number. Instead use original srv_category from call_control.
        */
        if(!((call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
              (cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG))
        )
        {
          call_ptr->num.emerg_srv_categ = cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ;
          call_ptr->is_sip380_std_emc_categ = TRUE;
        }
        cmipapp_release_reference_end_reason(call_ptr);
        cmipapp_release_reference_alternate_uri(call_ptr);
        cmipcall_reoriginate_call_as_emergency(call_ptr, FALSE);
        break;
      }

      /* 1) If emergency call originated over WLAN fails and IMS returns a FALLBACK_TO_CS end reason,
           the call is reattempted over CS domain
         2) If emergency call over LTE fails, and IMS returns FALLBACK_TO_CS end reason,
           the call needs to be reattempted over CS domain
         All the above when the emergency calling over wifi feature is ON
      */
#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
      if((ph_sub_config_ptr->emerg_call_prop.wifi_e911_support) && (!ph_sub_config_ptr->emerg_call_prop.wifi_e911_wwan_full_srv) &&
          (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
          (call_ptr->is_vowlan == TRUE) &&
          (call_ptr->call_state == CM_CALL_STATE_ORIG))
      {
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE

         call_ptr->sr.fall_back_to_wifi_from_cs = CM_INVALID_WWAN_TO_WLAN_FALLBACK_TIME;
         CM_MSG_HIGH_0("WLAN_E911:Reset fallback to wifi timer ");

#endif
        /*
        ** Update the emerg_srv_categ as the call may originate as EMERG over CS.
        */
	if(ims_end_cause == CMIPAPP_END_CAUSE_FALLBACK_TO_CS 
		 || (ims_end_cause == CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL
		 || ims_end_cause == CMIPAPP_END_CAUSE_PERMANENT_FAILURE))
		{

          if(cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG)
          {
            call_ptr->is_sip380_std_emc_categ = FALSE;
          }
           /* Do not copy NW reported emerg_srv_category if the category is NON_STANDARD and
          ** dialed number is UE detected number. Instead use original srv_category from call_control.
           */
          if(!((call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
                (cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG))
            )
            {
               call_ptr->num.emerg_srv_categ = cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ;
               call_ptr->is_sip380_std_emc_categ = TRUE;
            }

                  
            call_ptr->is_vowlan = FALSE;
            cmipapp_release_reference_end_reason(call_ptr);
            cmipapp_release_reference_alternate_uri(call_ptr);
            cmipcall_reoriginate_call_as_emergency(call_ptr, TRUE);
                  break;
          }
      }
#endif
      if((ph_sub_config_ptr->emerg_call_prop.is_wlan_e911_call_supp) &&
          (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
          (call_ptr->is_vowlan == TRUE) &&
          (call_ptr->call_state == CM_CALL_STATE_ORIG) &&
          (ims_end_cause == CMIPAPP_END_CAUSE_FALLBACK_TO_CS ||
           ims_end_cause == CMIPAPP_END_CAUSE_HARD_FAILURE ||
           ims_end_cause == CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL))
      {
        cm_call_end_e_type curr_end_status;
        if(cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG)
        {
          call_ptr->is_sip380_std_emc_categ = FALSE;
        }
         /* Do not copy NW reported emerg_srv_category if the category is NON_STANDARD and
        ** dialed number is UE detected number. Instead use original srv_category from call_control.
         */
        if(!((call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
              (cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ == CMIPAPP_COUNTRY_SPECIFIC_EMERG_SRV_CATEG))
          )
          {
             call_ptr->num.emerg_srv_categ = cmipapp_rpt_ptr->ip_ind.ind.call_end.emerg_srv_categ;
             call_ptr->is_sip380_std_emc_categ = TRUE;
          }

        call_ptr->is_vowlan = FALSE;
        cmipapp_release_reference_end_reason(call_ptr);
        cmipapp_release_reference_alternate_uri(call_ptr);
        curr_end_status = call_ptr->end_status;
        call_ptr->end_status = cmipcall_end_cause_to_cmcall_end_status
                               (ims_end_cause,
                                call_ptr->call_type,
                                call_ptr->mm_id);
        cmcall_check_and_update_wlan_e911_settings(call_ptr);
        call_ptr->end_status = curr_end_status;
        cmipcall_reoriginate_call_as_emergency(call_ptr, TRUE);
        break;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      call_ptr->end_cause_to_client = cmipcall_map_client_end_status
                                      (cmipapp_rpt_ptr->ip_ind.ind.call_end.client_end_cause);
      CM_MSG_HIGH_2_EXT("VOIP client end cause %d, call_end %d",
                        cmipapp_rpt_ptr->ip_ind.ind.call_end.client_end_cause,
                        call_ptr->end_cause_to_client,
                        (call_ptr->mm_id.asubs_id+1));

      if(cmipapp_rpt_ptr->ip_ind.ind.call_end.alternate_uri!= NULL)
      {
        call_ptr->cmcall_mode_info.info.ip_info.alternate_uri = cmipapp_rpt_ptr->ip_ind.ind.call_end.alternate_uri;
        CM_MSG_HIGH_1("alternate uri len %d", call_ptr->cmcall_mode_info.info.ip_info.alternate_uri->uri_len);
      }
      cmipcall_copy_error_info(cmipapp_rpt_ptr->ip_ind.ind.call_end.end_reason,
                               cmipapp_rpt_ptr->ip_ind.ind.call_end.sip_error_code,call_ptr);

      switch( call_ptr->call_state )
      {

        case CM_CALL_STATE_IDLE:
          /* Client released already */
          if ( call_ptr->call_subst.idle == CM_CALL_IDLE_SUBST_ENDWAIT )
          {
#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE)
            if(cmcall_drvcc_cache_get_drvcc_state() == CM_DRVCC_STATE_SETUP_IN_PROG)
            {
              cmcall_drvcc_check_and_abort_drvcc_setup(call_ptr);
            }
#endif /* defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE) */


            /* This check is added to unforce the emergency object from the pri-que whenever the VoWiFi is ended.
            ** This is done, since Emergency object is added to the Pri-Queue when call connects on Wifi.
            ** Now if the VoWifi call is ended before RAT change/or Handover then the Emerg objects needs to be removed
            */
            if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY && 
			   call_ptr->sys_mode == SYS_SYS_MODE_WLAN &&
			   (cmtask_orig_para_search_act_id(call_ptr->mm_id,(cm_act_id_type)call_ptr->call_id) != NULL))
            {
              cmph_remove_act_id_and_force_orig_mode(call_ptr->mm_id,(cm_act_id_type)call_ptr->call_id, TRUE);
                           
            }


            cmcall_end(call_ptr);
          }
          break;

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        case CM_CALL_STATE_ORIG:

          /* If we are trying to originate, indicate a RELEASE
          ** end status and call the origination function to decide
          ** whether to continue or abort the origination process.
          */

          if(call_ptr->sr.voip_sr.is_lte_hard_failure &&
		  	 !call_ptr->is_vowlan)
          {
            if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
                ph_sub_config_ptr->emerg_call_prop.is_alternate_redial_algorithm)
            {
              call_ptr->end_status = CM_CALL_END_IP_HARD_FAILURE;
            }
            else if(call_ptr->is_special_pref_volte_redial &&
               !volte_sub_info_ptr->volte_special_pref_setting)
            {
              /*If we see any LTE connection/drb failures when call is restricted
              ** to IMS , we should retry again on IMS and not fallback to cs
              */
              call_ptr->end_status = CM_CALL_END_IP_RETRY;    
            }
            else
            {
              call_ptr->end_status = CM_CALL_END_IP_FAIL;
            }
            call_ptr->sr.voip_sr.is_lte_hard_failure = FALSE;
            /*In case of LTE hard failure, mark ims redial allowed as TRUE by default */
            call_ptr->sr.voip_sr.is_ims_redial_allowed = TRUE;

          }
          else
          {
            call_ptr->end_status = cmipcall_end_cause_to_cmcall_end_status
                                   (ims_end_cause,
                                    call_ptr->call_type,
                                    call_ptr->mm_id);
            if((call_ptr->favored_mode_pref == CM_MODE_PREF_WLAN_ONLY) &&
                ((CMPH_SS_MODE_PREF(call_ptr->mm_id.asubs_id,cmph_ptr())) != CM_MODE_PREF_NONE))
            {
              call_ptr->favored_mode_pref = CMPH_SS_MODE_PREF(call_ptr->mm_id.asubs_id,cmph_ptr());
            }

            /* for calls that are redialled to IMS due to no CS coverage or due
            ** to no CS domain , check if we need to end the call or restrict it
            ** to IMS
            */
            if(call_ptr->is_special_pref_volte_redial)
            {
              /* If VOLTE_pref special setting is on , always end call after redial
              ** on IMS */
              if(volte_sub_info_ptr->volte_special_pref_setting)
              {
                is_end_call = TRUE;
              }
              /* If VOLTE_pref special setting is off, we should redial to IMS
              ** if IMS indicates redial is allowed or if not, end the call */
              else 
              {
                if(!call_ptr->sr.voip_sr.is_ims_redial_allowed)
                {
                  is_end_call = TRUE;
                }
                /* If we have performed cs to ps redial due to cs domain not available and
                ** redial to IMS is allowed , over-write end reason as IP_RETRY
                */
                else
                {
                  CM_MSG_HIGH_0("Force call end status to IP_RETRY ");
                  call_ptr->end_status = CM_CALL_END_IP_RETRY;
                }
              }
            }

            CM_MSG_HIGH_3 ("IMS: CALL_END : volte_special_pref_setting: %d, is_special_pref_volte_redial %d, is_end_call %d",
                   volte_sub_info_ptr->volte_special_pref_setting,
                   call_ptr->is_special_pref_volte_redial,
                   is_end_call);


            /* End call without silent Redial when IMS sends normal call release */
            if((call_ptr->end_status == CM_CALL_END_REL_NORMAL) ||(call_ptr->favored_mode_pref == CM_MODE_PREF_WLAN_ONLY) ||
                is_end_call )
            {
              #ifdef FEATURE_MMODE_DUAL_SIM
              if(call_ptr->is_emerg_on_other_sub && call_ptr->call_type == CM_CALL_TYPE_EMERGENCY )
              {
                call_ptr->end_status = CM_CALL_END_CAUSE_PERM_REDIAL_NOT_NEEDED;
                call_ptr->prot_end_status = CM_CALL_END_NONE;
                call_ptr->end_cause_to_client = CM_CALL_END_NONE;
                CM_MSG_HIGH_1("end_status being changed to %d",call_ptr->end_status);
              }
              #endif

              cmcall_end(call_ptr);
              break;
            }
          }

          /* For VoLTE/ViLTE MMODE call, if IMS retunrs CSFB, check with PM if we need to restrict 3GPP CS RATs in Silent Redial*/
          if((call_ptr->end_status == CM_CALL_END_IP_FAIL) && (call_ptr->call_type != CM_CALL_TYPE_EMERGENCY) &&
              (call_ptr->is_vowlan == FALSE) && !cmcall_is_volte_restricted_call(call_ptr))
          {
            boolean sr_restricted_on_gw = FALSE;

            sr_restricted_on_gw = policyman_get_subs_boolean_value("cm:silent_redial_restricted_on_GW",
                                  call_ptr->mm_id.asubs_id,&sr_restricted_on_gw) && sr_restricted_on_gw;

            /* For VzW NW, PM would return this TRUE*/
            if(sr_restricted_on_gw)
            {
              CM_MSG_HIGH_1("VzW SR: Remove GW from favored_mode_pref %d",call_ptr->favored_mode_pref);
              call_ptr->favored_mode_pref = cm_remove_mode_pref_components(
                                              call_ptr->favored_mode_pref,
                                              1,
                                              SD_SS_MODE_PREF_TDS_GSM_WCDMA_LTE);

              /* If mode pref is NONE, we can end the call here itself*/
              if(call_ptr->favored_mode_pref == CM_MODE_PREF_NONE)
              {
                cmcall_end(call_ptr);
                break;
              }
            }
          }


          /* IMS will send end reason string only for these two causes in ORIG, if IMS send endreason text
            for any other reason, release reference */
          if(ims_end_cause!= CMIPAPP_END_CAUSE_PERMANENT_FAILURE &&
              ims_end_cause!= CMIPAPP_END_CAUSE_NORMAL)
          {
            cmipapp_release_reference_end_reason(call_ptr);
          }

          /* IMS will send alternate uri string only for these four causes in ORIG, if IMS sends alternate uri
             for any other reason, release reference 
          */
          if(ims_end_cause != CMIPAPP_END_CAUSE_MULTIPLE_CHOICES &&
             ims_end_cause != CMIPAPP_END_CAUSE_MOVED_PERMANENTLY &&
             ims_end_cause != CMIPAPP_END_CAUSE_MOVED_TEMPORARILY &&
             ims_end_cause != CMIPAPP_END_CAUSE_USE_PROXY)
          {
            cmipapp_release_reference_alternate_uri(call_ptr);
          }
          /* Even when IMS sent normal call end release, cmph would not send LOW again */


          if(call_ptr->is_vowlan &&
              (ims_end_cause == CMIPAPP_END_CAUSE_FALLBACK_TO_CS ||
               ims_end_cause == CMIPAPP_END_CAUSE_ALTERNATE_VOICE_CALL)
            )
          {
            call_ptr->is_vowlan = FALSE;
          }

          if(call_ptr->sys_mode == SYS_SYS_MODE_LTE)
          {
            if(CM_CALL_ID_INVALID == cmcall_is_there_volte_call_sub_ignore(call_ptr->mm_id.asubs_id,
                                                                           call_ptr->call_id))
            {
              cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_NONE,
                                                    call_ptr->mm_id.asubs_id,
                                                    call_ptr->mm_id.stk_id,
                                                    TRUE);
            }
            cmcall_check_and_send_mmtel_status(call_ptr,FALSE);
          }

          if((call_ptr->is_em_vt) && ((call_ptr->end_status == CM_CALL_END_IP_FAIL) || (call_ptr->end_status == CM_CALL_END_IP_END)))
          {
            call_ptr->is_em_vt = FALSE;
            CM_MSG_HIGH_1("Reset is_em_vt to FALSE for end_cause %d",call_ptr->end_status);
          }
#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
          /* if wifi_e911_wwan_full_srv is enabled, and UE is in full service then all call end reasons are to be changed to
            CM_CALL_END_IP_FAIL to trigger forcing preferences to SD */
          if((ph_sub_config_ptr->emerg_call_prop.wifi_e911_wwan_full_srv == TRUE) &&
              (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
              cmss_is_ss_in_full_srv(call_ptr->mm_id))
          {
            call_ptr->end_status = CM_CALL_END_IP_FAIL;
          }
#endif

          /* If end status is still DRB REJ here, it should be mapped to
          ** IP FAIL to allow for redial.
          */
          if(call_ptr->end_status == CM_CALL_END_DRB_REJ)
          {
            call_ptr->end_status = CM_CALL_END_IP_FAIL;
          }
          
          cmcall_orig_proc(call_ptr);
          break;

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        case CM_CALL_STATE_INCOM:
          /* Call was never established -
          ** simply end the call.
          */
          /* This covers scenario where incoming call can be in SETUP/ ALERTING */
          if(ims_end_cause == CMIPAPP_END_CAUSE_ANSWERED_ELSEWHERE ||
              ims_end_cause == CMIPAPP_END_CAUSE_BUSY_EVERYWHERE )
          {
            call_ptr->end_status = cmipcall_end_cause_to_cmcall_end_status
                                   (ims_end_cause,
                                    call_ptr->call_type,
                                    call_ptr->mm_id);
          }
          else
          {
            call_ptr->end_status = CM_CALL_END_REL_NORMAL;
          }

          if(cmipapp_rpt_ptr->ip_ind.ind.call_end.is_unwanted_call)
	  {
            call_ptr->mt_call_verstat.call_verstat = CMIPAPP_TN_VALIDATION_FAIL; 
	  }
	  else 
	  {
	    call_ptr->mt_call_verstat.call_verstat = CMIPAPP_TN_VALIDATION_NONE; 
	  }


          cmcall_end( call_ptr );
          break;

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        case CM_CALL_STATE_CONV:
          if(ims_end_cause == CMIPAPP_END_CAUSE_DRVCC_WIFI_TO_CS)
          {
            /* call end due toi this cause are to be supressed */
            call_ptr->end_status = CM_CALL_END_DRVCC_WIFI_TO_CS;
            call_ptr->end_cause_to_client = CM_CALL_END_NONE;
          }
          else
          {
            call_ptr->end_status = CM_CALL_END_REL_NORMAL;
          }
#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE)
          if(cmcall_drvcc_cache_get_drvcc_state() == CM_DRVCC_STATE_SETUP_IN_PROG)
          {
            cmcall_drvcc_check_and_abort_drvcc_setup(call_ptr);
          }
#endif /* defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE) */
          cmcall_end(call_ptr);
          break;

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        default:
          CM_ERR_1( "Default switch in CALL_EVENT_END , call_state=%d",
                    call_ptr->call_state);

      }



      break;
    }
    /*------------------------------------------------------------------*/

    case CM_IP_CALL_IND_CALL_HOLD:

      CM_MSG_HIGH_2("IP RXD: CALL_HOLD, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.hold.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.hold.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call hold");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      /* If the call is put to hold by remote party, send indication to client without
      ** changing call status
      */

      call_ptr->cmcall_mode_info.info.ip_info.held_call_audio_attrib =
        cmipapp_rpt_ptr->ip_ind.ind.hold.held_call_audio_attrib;
      if(cmipapp_rpt_ptr->ip_ind.ind.hold.is_call_held_by_remote)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_CALL_ON_HOLD_BY_REMOTE);
        break;
      }

      /* Call has to satisfy following conditions to be put on hold
      ** Call type is not checked since multiple call types can use
      ** cmipcall
      **
      ** - Call state has to be in conversation.
      ** - Call substate has to be Active.
      */
      if ((call_ptr->call_state      != CM_CALL_STATE_CONV) ||
          (call_ptr->call_subst.conv != CM_CALL_CONV_CALL_ACTIVE)
         )
      {
        CM_ERR_1 ("Call %d not in right state for hold", call_ptr->call_id);
        break;
      }

      /*------------------------------------------------------------------*/

      /* call is not put to hold by remote party
      */
      CM_MSG_HIGH_2 ("Call hold status for call %d cause %d",
                     call_ptr->call_id, cmipapp_rpt_ptr->ip_ind.ind.hold.cause);
      if (cmipapp_rpt_ptr->ip_ind.ind.hold.cause == CM_IPAPP_CAUSE_NORMAL)
      {
        call_ptr->call_subst.conv = CM_CALL_CONV_CALL_ON_HOLD;
        ss_status = TRUE;
      }
      else
      {
        ss_status = FALSE;
      }

      cmipcall_copy_error_info(cmipapp_rpt_ptr->ip_ind.ind.hold.end_reason,
                               cmipapp_rpt_ptr->ip_ind.ind.hold.sip_error_code,call_ptr);

      /*------------------------------------------------------------------*/

      /*
      ** 10/31/05 - According to UI MANAGE_CALLS_CONF way of supplying
      ** active call list in a table is a legacy implementation. UI is
      ** more comfortable handling call hold/retrieve events on a per
      ** call basis.
      */
      if(ss_status)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_CALL_ON_HOLD);
      }

      cmipcall_mng_state_update(CM_IP_CALL_IND_CALL_HOLD, ss_status, call_ptr->call_id, cmipapp_rpt_ptr->ip_ind.ind.hold.cause);

      break;

    case CM_IP_CALL_IND_CALL_LOCAL_HOLD:
    {
      sys_modem_as_id_e_type rpt_asubs_id = cmipapp_rpt_ptr->ip_ind.hdr.asubs_id;
      sys_modem_as_id_e_type other_subs = (rpt_asubs_id == SYS_MODEM_AS_ID_1)?
                                          SYS_MODEM_AS_ID_2:SYS_MODEM_AS_ID_1;

      CM_MSG_HIGH_2("IP RXD: CALL_LOCAL_HOLD, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.local_hold.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.local_hold.id );

      /*------------------------------------------------------------------*/
      /* update phone voice sub to the other sub
      */
      if(ph_ptr->device_prop.msim_prop.curr_voice_subs == rpt_asubs_id)
      {
        ph_ptr->device_prop.msim_prop.curr_voice_subs = other_subs;
        cmph_sp_subs_info_update();
      }

      /* Copy the data block value from application */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/
      call_ptr->call_lch_state &= (~BM(CM_CALL_LCH_WAIT_IMS_RSP));
      if(call_ptr->call_lch_state == CM_CALL_LCH_STATE_NONE)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_LOCAL_HOLD);
      }

      break;
    }
    case CM_IP_CALL_IND_CALL_LOCAL_RETRIEVE:
    {
      sys_modem_as_id_e_type rpt_asubs_id = cmipapp_rpt_ptr->ip_ind.hdr.asubs_id;

      CM_MSG_HIGH_2("IP RXD: CALL_LOCAL_RETRIEVE, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.local_retrieve.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.local_retrieve.id );

      /*------------------------------------------------------------------*/
      /* update phone voice sub to the current sub
      */
      if(ph_ptr->device_prop.msim_prop.curr_voice_subs != rpt_asubs_id)
      {
        ph_ptr->device_prop.msim_prop.curr_voice_subs = rpt_asubs_id;
        cmph_sp_subs_info_update();
      }

      /* Copy the data block value from application */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/
      call_ptr->call_lch_state &= (~BM(CM_CALL_LCH_WAIT_IMS_RSP));
      if(call_ptr->call_lch_state == CM_CALL_LCH_STATE_NONE)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_LOCAL_HOLD);
      }

      break;
    }

    case CM_IP_CALL_IND_CALL_DEFLECTION:

      CM_MSG_HIGH_2("IP RXD: CALL_DEFLECTION, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_deflection.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID
      */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.call_deflection.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0("ERROR: Unknown call ID in call hold");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      CM_MSG_HIGH_2 ("Call deflection status for call %d cause %d",
                     call_ptr->call_id, cmipapp_rpt_ptr->ip_ind.ind.call_deflection.cause);

      if (cmipapp_rpt_ptr->ip_ind.ind.call_deflection.cause == CM_IPAPP_CAUSE_NORMAL)
      {
        ss_status = TRUE;
      }

      /* Copy the cause to call object.
      */
      call_ptr->end_params.end_params.ip_cause.cause =
        cmipapp_rpt_ptr->ip_ind.ind.call_deflection.cause;

      cmipcall_mng_state_update(CM_IP_CALL_IND_CALL_DEFLECTION, ss_status, call_ptr->call_id, CM_IPAPP_CAUSE_NORMAL);
      break;

    case CM_IP_CALL_IND_CALL_RETRIEVE:

      CM_MSG_HIGH_2("IP RXD: CALL_RETRIEVE, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.retrieve.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID
      */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.retrieve.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call hold");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /* If the call is retrieved by remote party, send indication to client without
      ** changing call status
      */
      if(cmipapp_rpt_ptr->ip_ind.ind.retrieve.is_call_retrieve_by_remote)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_CALL_RETRIEVE_BY_REMOTE);
        break;
      }
      /*------------------------------------------------------------------*/

      /* Call has to satisfy following conditions to be put on hold
      ** Call type is not checked since multiple call types can use
      ** cmipcall.
      **
      ** - Call state has to be in conversation.
      ** - Call substate has to be on hold.
      */
      if ((call_ptr->call_state      != CM_CALL_STATE_CONV) ||
          (call_ptr->call_subst.conv != CM_CALL_CONV_CALL_ON_HOLD)
         )
      {
        CM_ERR_1 ("Call %d not in right state for retrieve", call_ptr->call_id);
        break;
      }

      /*------------------------------------------------------------------*/
      CM_MSG_HIGH_2 ("Call retrieve status for  call %d cause %d",
                     call_ptr->call_id,
                     cmipapp_rpt_ptr->ip_ind.ind.retrieve.cause);

      if (cmipapp_rpt_ptr->ip_ind.ind.retrieve.cause == CM_IPAPP_CAUSE_NORMAL)
      {
        call_ptr->call_subst.conv = CM_CALL_CONV_CALL_ACTIVE;
        ss_status = TRUE;
      }
      else
      {
        ss_status = FALSE;
      }

      cmipcall_copy_error_info(cmipapp_rpt_ptr->ip_ind.ind.retrieve.end_reason,
                               cmipapp_rpt_ptr->ip_ind.ind.retrieve.sip_error_code,call_ptr);

      /*------------------------------------------------------------------*/

      /*
      ** 10/31/05 - According to UI MANAGE_CALLS_CONF way of supplying
      ** active call list in a table is a legacy implementation. UI is
      ** more comfortable handling call hold/retrieve events on a per
      ** call basis.
      */

      if(ss_status)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_CALL_RETRIEVED);
      }
      cmipcall_mng_state_update(CM_IP_CALL_IND_CALL_RETRIEVE, ss_status, call_ptr->call_id, cmipapp_rpt_ptr->ip_ind.ind.retrieve.cause);
      break;

    case CM_IP_CALL_IND_VS_STATUS:
      CM_MSG_HIGH_2("IP RXD: VS_STATUS, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.vs_status.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID
      */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.vs_status.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0 ("ERROR: Unknown call ID in call hold");

        return;
      }

      /* Voice call needs to be in conversation for videoshare to be possible
      */
      if( !((call_ptr->call_state == CM_CALL_STATE_CONV) &&
            (call_ptr->call_type == CM_CALL_TYPE_VOICE)
           )
        )
      {
        CM_ERR_1("Call %d in incorrect state for videoshare",
                 call_ptr->call_id );
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      /* Event sent to clients informing videoshare availability
      */
      CM_MSG_HIGH_2 ("Vidoeshare status on call %d status %d",
                     call_ptr->call_id, cmipapp_rpt_ptr->ip_ind.ind.vs_status.is_vs_poss );

      if (cmipapp_rpt_ptr->ip_ind.ind.vs_status.is_vs_poss == TRUE)
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_VS_AVAIL);
      }
      else
      {
        cmcall_event (call_ptr, CM_CALL_EVENT_VS_NOT_AVAIL);
      }

      break;

    case CM_IP_CALL_IND_SUPS_CONF:


      /* Call dependant supp service works on more
      ** than one call. Conf is not sent with respect
      ** to a particular call id.
      */

      /*------------------------------------------------------------------*/

      CM_MSG_HIGH_6("IP RXD: SUPS_CONF, sups_type=%d, num_part=%d, participant=0x%x id=%d, rpt asubs_id=%d, is_ims_cap_on_sub=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.sups_conf.sups_type,
                    cmipapp_rpt_ptr->ip_ind.ind.sups_conf.number_of_participants_add_removed,
                    cmipapp_rpt_ptr->ip_ind.ind.sups_conf.participants,
                    cmipapp_rpt_ptr->ip_ind.ind.sups_conf.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                    cmph_is_ims_capable_on_sub(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id));

      /* Allocate a call info buffer */
      call_info_ptr = cmcall_info_alloc();

      /* Call Id for sups */
      call_info_ptr->call_id = cmipapp_rpt_ptr->ip_ind.ind.sups_conf.id;

      call_info_ptr->asubs_id= cmipapp_rpt_ptr->ip_ind.hdr.asubs_id;

      local_mm_id.asubs_id = call_info_ptr->asubs_id;
      local_mm_id.stk_id =  MM_STACK_0;
	  
      call_info_ptr->mode_info.info.ip_call.modify_error_code =
        cmipcall_end_cause_to_cmcall_end_status((cmipapp_end_cause_e_type)cmipapp_rpt_ptr->ip_ind.ind.sups_conf.failure_cause,
            CM_CALL_TYPE_NONE, local_mm_id);

      /* Success of supp service req */
      call_info_ptr->mode_info.info.ip_call.call_ss_success =
        cmipapp_rpt_ptr->ip_ind.ind.sups_conf.call_success;

      /* Sups type that is generating ss_success */
      call_info_ptr->mode_info.info.ip_call.sups_type =
        cmipapp_rpt_ptr->ip_ind.ind.sups_conf.sups_type;

      call_info_ptr->mode_info.info.ip_call.number_of_participants_add_removed =
        cmipapp_rpt_ptr->ip_ind.ind.sups_conf.number_of_participants_add_removed;

      /* List of active calls is not sent as in GW because IP call
      ** sends individual hold/retrieve for each call.
      */

      /* Set info_type to IP
      */
      call_info_ptr->mode_info.info_type = CM_CALL_MODE_INFO_IP;

      /* send end reason string/text for sups failure like hold/retreive/conference failure  */
      call_ptr = cmcall_ptr(cmipapp_rpt_ptr->ip_ind.ind.sups_conf.id);

      if (call_ptr == NULL)
      {
        CM_MSG_HIGH_0  ("call_not_present for MNG_CALLS_CONF ");
      }
      else
      {

        cmipcall_copy_error_info(cmipapp_rpt_ptr->ip_ind.ind.sups_conf.end_reason,
                                 cmipapp_rpt_ptr->ip_ind.ind.sups_conf.sip_error_code,call_ptr);

        call_info_ptr->mode_info.info.ip_call.ims_end_reason = call_ptr->cmcall_mode_info.info.ip_info.ims_end_reason;

        call_info_ptr->mode_info.info.ip_call.sip_error_code = call_ptr->cmcall_mode_info.info.ip_info.sip_error_code;
      }

      /* CM pass the participant pointer to QMi directly without CM buffer allocation
      ** This buffer will be released by QMi after use
      */
      call_info_ptr->mode_info.info.ip_call.participant_uri = cmipapp_rpt_ptr->ip_ind.ind.sups_conf.participants;

      /* Notify clients list of call event. */
      cmclient_list_call_event_ntfy ( CM_CALL_EVENT_MNG_CALLS_CONF,
                                      call_info_ptr );


      if(cmipapp_rpt_ptr->ip_ind.ind.sups_conf.participants != NULL)
      {
        ref_cnt_obj_release(cmipapp_rpt_ptr->ip_ind.ind.sups_conf.participants);
      }

      cmipapp_release_reference_end_reason(call_ptr);

      /* Deallocate the memory after sending the call event. */
      cm_mem_free(call_info_ptr);

      break;


    case CM_IP_CALL_IND_TRANSFER:

      CM_MSG_HIGH_2("IP RXD: CM_IP_CALL_IND_TRANSFER, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_transfer.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID
      */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.call_transfer.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call hold");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      cmcall_event (call_ptr, CM_CALL_EVENT_TRANSFERRED_CALL);

      break;


    case CM_IP_CALL_IND_CONNECTED_NUM:

      /* Get the call pointer from the call ID
      */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.connected_num.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call hold");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      /* Copy only the new connected number buffer and PI from IP APP */
      CM_MSG_HIGH_2("RXD: CONNECTED_NUM: num.len = %d, num.pi = %d from IP APP",
                    cmipapp_rpt_ptr->ip_ind.ind.connected_num.num.len,
                    cmipapp_rpt_ptr->ip_ind.ind.connected_num.num.pi);

      if ( cmipapp_rpt_ptr->ip_ind.ind.connected_num.num.len != 0 )
      {
        call_ptr->num.len = memscpy( call_ptr->num.buf,
                                     sizeof(call_ptr->num.buf),
                                     cmipapp_rpt_ptr->ip_ind.ind.connected_num.num.buf,
                                     cmipapp_rpt_ptr->ip_ind.ind.connected_num.num.len ) ;
      }

      call_ptr->num.pi = cmipapp_rpt_ptr->ip_ind.ind.connected_num.num.pi;

      call_ptr->cmcall_mode_info.info.ip_info.connected_num_is_ect_trig =
        cmipapp_rpt_ptr->ip_ind.ind.connected_num.is_ect_triggered;
      /*------------------------------------------------------------------*/

      cmcall_event (call_ptr, CM_CALL_EVENT_CONNECTED_NUM);

      break;

    case CM_IP_CALL_IND_MODIFY_COMPLETE:

      CM_MSG_HIGH_4("IP RXD: MODIFY_COMPLETE, id=%d, as_id=%d, call_type=%d, is_secure_call = %d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_id,
                   cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                   cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_type,
                   cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.is_secure_call);

      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_id);

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call modify");
        return;
      }

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /* store the call modify status
      */
      call_ptr->cmcall_mode_info.info.ip_info.call_modif_err_status = cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.modify_error_status;
      call_ptr->cmcall_mode_info.info.ip_info.modify_error_code = cmipcall_map_client_end_status(cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.modify_error_code);
      CM_MSG_HIGH_2_EXT("VOIP client end cause %d, call_end %d",
                        cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.modify_error_code,
                        call_ptr->cmcall_mode_info.info.ip_info.modify_error_code,
                        (call_ptr->mm_id.asubs_id+1));
      call_ptr->cmcall_mode_info.info.ip_info.retry_timer_val = cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.retry_timer_val;
      call_ptr->cmcall_mode_info.info.ip_info.modify_cause_code     = cmipcall_map_modify_cause_code(cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.modify_cause_code);

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_local_cap_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.local_call_cap_attrib;
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_peer_cap_attrib_info  = cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.peer_call_cap_attrib;

      /* store call type/attrib in all cases:
      ** for error response, IMS will send current call type/attrib
      ** for success resposne, IMS will send new call type/attrib
      */
      if(!(call_ptr->special_emergency))
      {
        call_ptr->call_type = cmipcall_map_calltype_to_cm_calltype(cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_type);
      }
      if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY)
      {
        if(cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.em_type == CM_CALL_EM_VOICE)
        {
          call_ptr->is_em_vt = FALSE;
        }
        else if(cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.em_type == CM_CALL_EM_VT)
        {
          call_ptr->is_em_vt = TRUE;
        }
        else
        {
          CM_MSG_HIGH_1("Invalid em_type %d from IMS!",cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.em_type);
        }
      }

        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib;
        CM_MSG_HIGH_1("MODIFY_COMPLETE, is_secure_call=%d",cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.is_secure_call);
        
        
        CM_MSG_MED_6("Modify cnf with audio=%d, video = %d em_type %d rtt_mode %d peer_rtt_mode %d local_rtt_mode %d",
                     cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.audio_attrib,
                     cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.video_attrib,
                     cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.em_type,
                     cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.rtt_mode,
                     cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.peer_call_cap_attrib.rtt_mode,
                     cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.local_call_cap_attrib.rtt_mode);

      CM_MSG_MED_6("Modify cnf with err status = %d, err_code=%d call_type = %d is_secure_call %d, call_composer %d %d",
                   call_ptr->cmcall_mode_info.info.ip_info.call_modif_err_status,
                   call_ptr->cmcall_mode_info.info.ip_info.modify_error_code,
                   call_ptr->call_type,
                   cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.call_attrib.is_secure_call,
                   cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.peer_call_cap_attrib.call_composer_supported,
                   cmipapp_rpt_ptr->ip_ind.ind.call_modify_cnf.local_call_cap_attrib.call_composer_supported);

      cmcall_event(call_ptr, CM_CALL_EVENT_MODIFY_COMPLETE_CONF);

      /* Reset new call type field
      */
      call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type = CM_CALL_TYPE_NONE;
      memset(&(call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib),0,sizeof(cm_ip_call_attrib_info_s_type));
      break;

    case CM_IP_CALL_IND_MODIFY_IND:
      CM_MSG_HIGH_3("IP RXD: MODIFY_IND, id=%d, as_id=%d rtt_mode=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_modify_ind.call_id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                    cmipapp_rpt_ptr->ip_ind.ind.call_modify_ind.call_attrib.rtt_mode);

      call_ptr = cmcall_ptr(cmipapp_rpt_ptr->ip_ind.ind.call_modify_ind.call_id);
      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call modify");
        return;
      }

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info = cmipapp_rpt_ptr->ip_ind.ind.call_modify_ind.call_attrib;

      call_ptr->cmcall_mode_info.info.ip_info.modify_cause_code 
        = cmipcall_map_modify_cause_code(cmipapp_rpt_ptr->ip_ind.ind.call_modify_ind.modify_cause_code);

      CM_MSG_HIGH_2("IP RXD: MODIFY_IND, CM modify_cause_code %d IMS modify_cause_code %d", call_ptr->cmcall_mode_info.info.ip_info.modify_cause_code, cmipapp_rpt_ptr->ip_ind.ind.call_modify_ind.modify_cause_code);

      if(!(call_ptr->special_emergency))
      {
        call_ptr->call_type = cmipcall_map_calltype_to_cm_calltype(cmipapp_rpt_ptr->ip_ind.ind.call_modify_ind.req_call_type);
      }
      cmcall_event(call_ptr, CM_CALL_EVENT_MODIFY_IND);

      break;

    case CM_IP_APP_AUDIO_CODEC_INFO:
      /* Get the call pointer from the call ID
      */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.audio_codec.call_id);

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in audio codec info");

        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      call_ptr->speech_codec_info.network_mode = SYS_NETWORK_TYPE_LTE;

      if(call_ptr->is_vowlan)
      {
        CM_MSG_HIGH_0("Set network_mode as WLAN for Audio Codec Indo");
        call_ptr->speech_codec_info.network_mode = SYS_NETWORK_TYPE_WLAN;
      }

      // For normal VT calls over IMS, we need to make the change and we need to derive it based on sys_mode
      if(call_ptr->sys_mode == SYS_SYS_MODE_WCDMA)
      {
        CM_MSG_HIGH_0("Set network_mode as WCDMA for Audio Codec Indo");
        call_ptr->speech_codec_info.network_mode = SYS_NETWORK_TYPE_WCDMA;
      }

      call_ptr->speech_codec_info.speech_codec_type =
        cmipapp_rpt_ptr->ip_ind.ind.audio_codec.speech_codec_type;
      call_ptr->speech_codec_info.speech_enc_samp_freq =
        cmipapp_rpt_ptr->ip_ind.ind.audio_codec.speech_enc_samp_freq;

      cmcall_event (call_ptr, CM_CALL_EVENT_SPEECH_CODEC_INFO);

      break;

    case CM_IP_CALL_START_DTMF_CONF:
      cminband_ip_rpt_proc(CM_IP_CALL_START_DTMF_CONF,
                           cmipapp_rpt_ptr->ip_ind.ind.dtmf_start_conf.dtmf_success,
                           cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);
      break;

    case CM_IP_CALL_STOP_DTMF_CONF:
      cminband_ip_rpt_proc(CM_IP_CALL_STOP_DTMF_CONF,
                           cmipapp_rpt_ptr->ip_ind.ind.dtmf_stop_conf.dtmf_success,
                           cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);
      break;

    case CM_IP_CALL_START_DTMF_IND:
      cminband_ip_ind_proc(CM_IP_CALL_START_DTMF_IND,cmipapp_rpt_ptr->ip_ind.ind.start_dtmf_ind);
      break;

    case CM_IP_CALL_STOP_DTMF_IND:
      cminband_ip_ind_proc(CM_IP_CALL_STOP_DTMF_IND,cmipapp_rpt_ptr->ip_ind.ind.stop_dtmf_ind);
      break;

    case CM_IP_CALL_JOIN_CONF_CALL:

      CM_MSG_MED_4("IP RXD: JOIN_CONF, id=%d, paticipant_uri=0x%x rpt asubs_id=%d, is_ims_cap_on_sub=%d",
                   cmipapp_rpt_ptr->ip_ind.ind.join_conf_call.id,
                   cmipapp_rpt_ptr->ip_ind.ind.join_conf_call.participant_uri,
                   cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                   cmph_is_ims_capable_on_sub(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id));

      /* Allocate a call info buffer */
      call_info_ptr = cmcall_info_alloc();
      call_info_ptr->call_id = cmipapp_rpt_ptr->ip_ind.ind.join_conf_call.id;
      call_info_ptr->asubs_id= cmipapp_rpt_ptr->ip_ind.hdr.asubs_id;

      call_info_ptr->mode_info.info_type = CM_CALL_MODE_INFO_IP;
      call_info_ptr->mode_info.info.ip_call.participant_uri = cmipapp_rpt_ptr->ip_ind.ind.join_conf_call.participant_uri;

      /* Notify clients list of call event. */
      cmclient_list_call_event_ntfy ( CM_CALL_EVENT_JOIN_CONFERENCE,
                                      call_info_ptr );

      if(cmipapp_rpt_ptr->ip_ind.ind.join_conf_call.participant_uri != NULL)
      {
        ref_cnt_obj_release(cmipapp_rpt_ptr->ip_ind.ind.join_conf_call.participant_uri);
      }

      /* Deallocate the memory after sending the call event. */
      cm_mem_free(call_info_ptr);

      break;

    case CM_IP_CALL_CONF_INFO:
      CM_MSG_HIGH_3("IP RXD: CONF_INFO, id=%d, rpt asubs_id=%d, is_ims_cap_on_sub=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.conf_info.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,
                    cmph_is_ims_capable_on_sub(cmipapp_rpt_ptr->ip_ind.hdr.asubs_id));

      /* Allocate a call info buffer */
      call_info_ptr = cmcall_info_alloc();
      call_info_ptr->call_id = cmipapp_rpt_ptr->ip_ind.ind.conf_info.id;
      call_info_ptr->asubs_id= cmipapp_rpt_ptr->ip_ind.hdr.asubs_id;

      call_info_ptr->mode_info.info_type = CM_CALL_MODE_INFO_IP;

      call_info_ptr->mode_info.info.ip_call.conference_info = cmipapp_rpt_ptr->ip_ind.ind.conf_info.conf;

      /* Notify clients list of call event. */
      cmclient_list_call_event_ntfy ( CM_CALL_EVENT_CONFERENCE_INFO,
                                      call_info_ptr );

      if(cmipapp_rpt_ptr->ip_ind.ind.conf_info.conf != NULL)
      {
        ref_cnt_obj_release(cmipapp_rpt_ptr->ip_ind.ind.conf_info.conf);
      }

      /* Deallocate the memory after sending the call event. */
      cm_mem_free(call_info_ptr);

      break;

    /* Receive rat change from IMS
      */
    case CM_IP_IND_CALL_RAT_CHANGE:
      cmipcall_rat_change_rpt_proc(cmipapp_rpt_ptr);
      break;

    case CM_IP_IND_EPDN_RAT_CHANGE:
	  cmipcall_epdn_rat_change_rpt_proc(cmipapp_rpt_ptr);
	  break;
	  
    /* ===This indication is obsoleted for now. No one use it
    ** CM_CALL_EVENT_CALL_IS_WAITING will be sent when is_alert_info_present is set in progress info ind
    ** It is used to notify calling party call is waiting. For called party, QMI send call waiting upon
    ** CM_CALL_EVENT_INCOMING  ===*/
    case CM_IP_CALL_IND_CALL_WAIT:
#ifdef FEATURE_IP_CALL

      CM_MSG_HIGH_2("IP RXD: CALL_WAIT, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_waiting.call_id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.call_waiting.call_id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0    ("ERROR: Unknown call ID in call hold");
        return;
      }

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /* Copy the new waiting num from ip */
      call_ptr->waiting_num = cmipapp_rpt_ptr->ip_ind.ind.call_waiting.num;

      /* Notify clients list of call event CM_CALL_EVENT_CALL_IS_WAITING. */
      cmcall_event ( call_ptr, CM_CALL_EVENT_CALL_IS_WAITING );
      break;
#endif

    case CM_IP_CALL_IND_ANSWER_REJECT:

      CM_MSG_HIGH_2("IP RXD: ANSWER_REJECT, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.answer_reject.id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.answer_reject.id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0 ("ERROR: Unknown call ID in ANSWER reject");
        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      CM_MSG_HIGH_1("ANSWER reject from IMS for INCOM, cause %d", cmipapp_rpt_ptr->ip_ind.ind.answer_reject.cause);

      cmipcall_mng_state_update(CM_IP_CALL_IND_ANSWER_REJECT, ss_status, call_ptr->call_id, CM_IPAPP_CAUSE_NORMAL);
      break;

    case CM_IP_APP_CALL_SWITCH_CODEC_CNF:
    {

      CM_MSG_HIGH_2("IP RXD: SWITCH_CODEC_CNF, id=%d, as_id=%d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_switch_codec_cnf.call_id,
                    cmipapp_rpt_ptr->ip_ind.hdr.asubs_id);

      /* Get the call pointer from the call ID */
      call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.call_switch_codec_cnf.call_id );

      if (call_ptr == NULL)
      {
        CM_ASSERT ( call_ptr != NULL );
        CM_ERR_0 ("ERROR: Unknown call ID in switch codec cnf");
        return;
      }

      /*------------------------------------------------------------------*/

      /* Copy the data block value from application
      */
      call_ptr->ipapp_data_block = cmipapp_rpt_ptr->data_block;

      cmipapp_set_user_cb_data(call_ptr->ipapp_id,
                               cmipapp_rpt_ptr->data_block);

      /*------------------------------------------------------------------*/

      CM_MSG_HIGH_2("switch codec cnf from IMS for call id %d, status %d",
                    cmipapp_rpt_ptr->ip_ind.ind.call_switch_codec_cnf.call_id,
                    cmipapp_rpt_ptr->ip_ind.ind.call_switch_codec_cnf.status);

      /* Copy the status of switch codec cnf in call_ptr */
      call_ptr->cmcall_mode_info.info.ip_info.switch_codec_cmd_status = cmipapp_rpt_ptr->ip_ind.ind.call_switch_codec_cnf.status;

      /* Notify clients list of call event CM_CALL_EVENT_SWITCH_CODEC_CNF. */
      cmcall_event ( call_ptr, CM_CALL_EVENT_SWITCH_CODEC_CNF );
      break;
    }

    case CM_IP_CALL_IND_CANCEL_IMS_CALL_RSP:
    {   
        CM_MSG_HIGH_4("IP RXD: CM_IP_CALL_IND_CANCEL_IMS_CALL_RSP, as_id=%d, id=%d, cancel_result=%d,cancel_type=%d",
                        cmipapp_rpt_ptr->ip_ind.hdr.asubs_id,   
                        cmipapp_rpt_ptr->ip_ind.ind.cancel_ims_call_ind.call_id,
                        cmipapp_rpt_ptr->ip_ind.ind.cancel_ims_call_ind.cancel_call_result,
                        cmipapp_rpt_ptr->ip_ind.ind.cancel_ims_call_ind.cancel_ims_call_req_type);
	    /* Get the call pointer from the call ID */
        call_ptr = cmcall_ptr ( cmipapp_rpt_ptr->ip_ind.ind.cancel_ims_call_ind.call_id);

        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.cancel_ims_call_req = 
                cmipapp_rpt_ptr->ip_ind.ind.cancel_ims_call_ind.cancel_ims_call_req_type;
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.cancel_ims_call_req_result =  
                cmipapp_rpt_ptr->ip_ind.ind.cancel_ims_call_ind.cancel_call_result;
        call_ptr->call_id = cmipapp_rpt_ptr->ip_ind.ind.cancel_ims_call_ind.call_id;

        cmcall_event(call_ptr,CM_CALL_EVENT_CANCEL_IMS_CALL_RSP);

        break;
    }

    case CM_IP_CALL_SIP_DTMF_IND:
      cminband_mt_sip_dtmf_ind_proc(cmipapp_rpt_ptr->ip_ind.ind.sip_dtmf_ind);
      break;

    default:
      CM_MSG_MED_1 ("Unkwn report from IP app %d", cmipapp_rpt_ptr->hdr.cmd);
      break;

  } /* switch (cmipapp_rpt_ptr->hdr.cmd) */


} /* cmipcall_rpt_proc () */


/*===========================================================================

FUNCTION  cmipcall_client_cmd_check

DESCRIPTION
  Check for call command parameter errors and whether a specified call command
  is allowed in the current state of the call/phone.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  CM_CALL_CMD_ERR_NOERR if command is allowed in the current state
  of the call/phone, specific cm_call_cmd_err_e_type otherwise.

SIDE EFFECTS
  None

===========================================================================*/
cm_call_cmd_err_e_type  cmipcall_client_cmd_check(

  cm_call_cmd_s_type          *call_cmd_ptr
  /* Pointer to a CM command */
)
{

  cmcall_s_type               *call_ptr      = NULL;
  /* Pointer to the call object */

  cm_call_cmd_err_e_type       cmd_err       = CM_CALL_CMD_ERR_NOERR;
  /* Initialize command error to NO-ERROR */

  int                          i;
  /* For loop counter */
  cmph_s_type           *ph_ptr = cmph_ptr();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( call_cmd_ptr != NULL );
  CM_ASSERT( call_cmd_ptr->cmd_type == CM_CMD_TYPE_CALL );

  CM_MSG_HIGH_1( "IPCALL cmd check %d",call_cmd_ptr->cmd_type);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* check cmd is already in error or not
  */
  if ( call_cmd_ptr->info.call_type != CM_CALL_TYPE_PS_DATA )
  {
    if (call_cmd_ptr->info.cmd_err != CM_CALL_CMD_ERR_NOERR)
    {
      return call_cmd_ptr->info.cmd_err;
    }

  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



  /* Check whether command is possible.
  */
  switch( call_cmd_ptr->cmd )
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*---------------------------*/
    /* Response to a setup message */
    /*---------------------------*/

    case CM_CALL_CMD_SETUP_RES:


      call_ptr = cmcall_ptr ( call_cmd_ptr->info.call_id );

      if(call_ptr == NULL)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_ID_P;
      }
      else if ( call_ptr->call_state != CM_CALL_STATE_INCOM )
      {
        /* Makes sense only in incoming state
        */
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }
      /*lint -restore */
      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_CALL_CMD_ANSWER:

      /* check command parameter errors */
      if((cmd_err = cmcall_call_cmd_answer_para_check(call_cmd_ptr)) != CM_CALL_CMD_ERR_NOERR)
      {
        /* stop further processing if any errors found in the command */
        break;
      }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      call_ptr = cmcall_ptr ( call_cmd_ptr->info.call_id );
      if(call_ptr==NULL)
      {
        CM_MSG_HIGH_0( "call_ptr_NULL");
        break;
      }

      if (call_ptr->call_state != CM_CALL_STATE_INCOM)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }

      /*lint -restore */
      break; /* CM_CALL_CMD_ANSWER */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_CALL_CMD_END:
    {
#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
      cmph_sub_info_s_type *ph_sub_ptr = NULL;
#endif

      /* CALL_CMD_END can carry calls in multiple technologies. Earlier it
      ** was either xcall_end, wcall_end or ipcall_end being called. Currently
      ** all three get called one after the other and overall call state can
      ** go to IDLE in any of these calls. So not returning error any more.
      */
      if( (cmcall_overall_call_state() == CM_OVERALL_CALL_STATE_IDLE ) )
      {
        CM_MSG_MED_0 ("overall call state in idle");
      }


      /* check command parameter errors */
      /* none */
      for ( i = 0; i < call_cmd_ptr->info.num_end_call_ids; i++ )
      {

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /* cmipcall should end calls of info_type IP only
        */
        if (cmcall_call_id_mode (call_cmd_ptr->info.end_params[i].call_id)
            != CM_CALL_MODE_INFO_IP)
        {
          continue;
        }

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        call_ptr = cmcall_ptr ( call_cmd_ptr->info.end_params[i].call_id );

        if (call_ptr == NULL )
        {
          cmd_err = CM_CALL_CMD_ERR_CALL_ID_P;
          break;
        }
#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
        if ((ph_sub_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id)) == NULL)
        {
          cmd_err = CM_CALL_CMD_ERR_OP_NOT_SUPPORTED_ON_SUBS;
          break;
        }
#endif

        if((ph_ptr->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE)
#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
            && !(ph_sub_ptr->voice_in_wifi_only && call_ptr->is_vowlan)
#endif
          )
        {
          cmd_err = CM_CALL_CMD_ERR_OFFLINE_S;
          break;
        }

        if ( call_ptr->call_state == CM_CALL_STATE_IDLE )
        {
          /* In idle this command makes no sense.
          */
          cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
          break;
        }

        /* Need not check for call info type, since CM internally
        ** determines from call object
        */
      }

      break;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


    case CM_CALL_CMD_SUPS:

      /* Check cmd parameters for call related sups
      */
      cmd_err = cmipcall_sups_cmd_check (call_cmd_ptr);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_CALL_CMD_MO_MODIFY_REQ:
    case CM_CALL_CMD_MODIFY_RES:

      call_ptr = cmcall_ptr ( call_cmd_ptr->info.call_id );
      if(call_ptr == NULL)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_ID_P;
        CM_ASSERT(call_ptr != NULL);
        break;
      }

      if(call_ptr->call_state != CM_CALL_STATE_CONV)
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_STATE_S;
      }

      /* Call modification is possible on only VOIP, VS, VT and Emergency Calls
      */
      if ( !((call_ptr->call_type == CM_CALL_TYPE_VOICE) ||
             (call_ptr->call_type == CM_CALL_TYPE_VT) ||
             (call_ptr->call_type == CM_CALL_TYPE_VS) ||
             (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY)
            )
         )
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_TYPE_S;
      }

      /* Call can be modified to only VOIP, VS, VT calls
      */
      if ( !((call_cmd_ptr->info.ip_call.new_call_type_req == CM_CALL_TYPE_VOICE) ||
             (call_cmd_ptr->info.ip_call.new_call_type_req == CM_CALL_TYPE_VT) ||
             (call_cmd_ptr->info.ip_call.new_call_type_req == CM_CALL_TYPE_VS) ||
             (call_cmd_ptr->info.ip_call.new_call_type_req == CM_CALL_TYPE_EMERGENCY)
            )
         )
      {
        cmd_err = CM_CALL_CMD_ERR_CALL_TYPE_P;
      }

      break;

    case CM_CALL_CMD_CANCEL_IMS_CALL_REQ:
     {
        call_ptr = cmcall_ptr(call_cmd_ptr->info.call_id);
        if(call_ptr == NULL)
        {
           cmd_err = CM_CALL_CMD_ERR_CALL_ID_P;
           CM_ASSERT(call_ptr != NULL);
           break;
        }   
        
        /*CM_CALL_CMD_CANCEL_IMS_CALL_REQ can only be sent for VOICE,VT/VS call type
        */

        if ( !((call_ptr->call_type == CM_CALL_TYPE_VOICE) ||
               (call_ptr->call_type == CM_CALL_TYPE_VT) ||
               (call_ptr->call_type == CM_CALL_TYPE_VS)
               ))
          {
              cmd_err = CM_CALL_CMD_ERR_CALL_TYPE_S;
          }
		break;
      }

    default:

      cmd_err = CM_CALL_CMD_ERR_OTHER;
      CM_ERR_1( "Bad call command %d", call_cmd_ptr->cmd);

  }

  return cmd_err;
} /* cmipcall_client_cmd_check */


/*===========================================================================

FUNCTION cmipcall_end_req

DESCRIPTION
  Does call end necessary activities.
  Sends end_req event.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void cmipcall_end_req(

  cmcall_s_type  *call_ptr
  /* pointer to a call object */
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_MSG_HIGH_0_EXT( "cmipcall_end_req()", (call_ptr->mm_id.asubs_id+1));
  CM_ASSERT( call_ptr != NULL );

  /* call state IDLE means call end request already sent
     just return. no need for another call end request event*/
  if(call_ptr->call_state      == CM_CALL_STATE_IDLE)
  {
    return;
  }

  /* save the previous call state */
  call_ptr->prev_call_state = call_ptr->call_state;

  /* Change the call state to idle.*/
  call_ptr->call_state      = CM_CALL_STATE_IDLE;
  call_ptr->call_subst.idle = CM_CALL_IDLE_SUBST_ENDWAIT;

  if(call_ptr->cc_object_ptr != NULL)
  {
    return;
  }
  /* Notify clients of call end req event.*/
  cmcall_update_call_end_info( call_ptr );
  cmcall_event( call_ptr, CM_CALL_EVENT_END_REQ );

  /*
  ** Tell phone that Call End procedure has been
  ** started.
  */
  cmph_call_end_req(call_ptr);


}


/*===========================================================================

FUNCTION cmipcall_client_cmd_forward_ll

DESCRIPTION
  Forward a client call commands to Lower Layer.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  TRUE if CM needs to wait for a reply from IP App before continuing
  processing this command, FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmipcall_client_cmd_forward_ll(

  const cm_call_cmd_s_type    *call_cmd_ptr
  /* Pointer to a CM command */
)
{
  const cm_call_cmd_info_s_type    *cmd_info_ptr     = NULL;
  /* Pointer to call cmd info */

  cmipapp_incom_rej_cause_e_type    incom_rej        = CMIPAPP_INCOM_REJ_CAUSE_NONE;
  /* Reject reason for call set up and call answer */

  const cm_call_cmd_info_ip_s_type  *ip_cmd_info_ptr = NULL;


  cmcall_s_type                     *call_ptr        = NULL;
  /* Pointer to a call object */

  int                                i;

#ifdef  FEATURE_VOIP_E911_SUPPORT
  cmph_s_type                       *ph_ptr          = cmph_ptr();
#endif

#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
       defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE) && defined(FEATURE_1X_SRVCC)
  mc_msg_type             *mc_ptr;
#endif

  cm_ip_call_sip_reject_code_s_type      sip_reject_code ;
  /* Reject reason for call set up and call answer */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( call_cmd_ptr != NULL );
  CM_ASSERT( call_cmd_ptr->cmd_type == CM_CMD_TYPE_CALL );

  memset( &sip_reject_code, 0, sizeof( sip_reject_code ) );
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ip_cmd_info_ptr = IPCALL_CMD_INFO_PTR(call_cmd_ptr);
  cmd_info_ptr    = CMD_INFO_PTR(call_cmd_ptr);

  CM_MSG_HIGH_3("IP CALLCMD: cmd=%d, as_id=%d, is_ims_cap_on_sub=%d",
                call_cmd_ptr->cmd,
                call_cmd_ptr->cmd_subs,
                cmph_is_ims_capable_on_sub(call_cmd_ptr->cmd_subs));

  switch( call_cmd_ptr->cmd )
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*---------------------------*/
    /* Response to a setup message*/
    /*---------------------------*/

    case CM_CALL_CMD_SETUP_RES:

      call_ptr = cmcall_ptr (call_cmd_ptr->info.call_id);

      if (ip_cmd_info_ptr->accept)
      {
        incom_rej = CMIPAPP_INCOM_REJ_CAUSE_NONE;
      }
      else
      {
        incom_rej = CMIPAPP_INCOM_REJ_CAUSE_BUSY;
      }

      if(call_ptr == NULL)
      {
        CM_ERR_1( "Invalid callid: %d", call_cmd_ptr->info.call_id);
      }
      else
      {
        /* Call the IP call funtion for set up response
        */
        cmipcall_invite_res (call_ptr, ip_cmd_info_ptr->accept, incom_rej);
      }

      /* CM_CALL_EVENT_INCOM gets sent at the receipt of a seperate
      ** indication.
      */

      break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /* Answer an incoming call */
    /*-------------------------*/

    case CM_CALL_CMD_ANSWER:

      call_ptr = cmcall_ptr (call_cmd_ptr->info.call_id);

      if(call_ptr == NULL)
      {
        CM_MSG_HIGH_0( "call_ptr_NULL");
        break;
      }

      if(ip_cmd_info_ptr->accept)
      {

        incom_rej = CMIPAPP_INCOM_REJ_CAUSE_NONE;
        /* update call type with user accepted call type if ims_call_type is not VT. If it is VT it means
           SRVCC is going on and cache is with CM and VT needs to be converted to Voice
        */
#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE)
        if (call_ptr->ims_call_type == CM_CALL_TYPE_VT)
        {
          call_ptr->call_type = CM_CALL_TYPE_VOICE;
        }
        else
        {
          call_ptr->call_type = call_cmd_ptr->info.call_type;
        }
#else
        call_ptr->call_type = call_cmd_ptr->info.call_type;
#endif
      }
      else
      {
        incom_rej = cmcall_end_cause_to_cmipcall_rej_status(ip_cmd_info_ptr->call_reject_reason);
        sip_reject_code = ip_cmd_info_ptr->sip_reject_code;
      }

      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info = cmd_info_ptr->ip_call.ipcall_attrib_info;

	  if(incom_rej == CMIPAPP_INCOM_REJ_CAUSE_UNWANTED_CALL)
	  {
		call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.is_unwanted_call = TRUE;
	  }
	  else
	  {
		call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.is_unwanted_call = FALSE;
	  }

   
      if(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param != NULL)
      {
        ref_cnt_obj_add_ref(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param);
      }
      /* Update answer number PI with value received in ANSWER command */
      call_ptr->answer_num_pi = call_cmd_ptr->info.ip_call.pi;

      cmipcall_answer (call_ptr, ip_cmd_info_ptr->accept, incom_rej, sip_reject_code);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*---------------------------*/
    /* Ending a call              */
    /*--------------------------- */

    case CM_CALL_CMD_END:
    {
#if defined FEATURE_VOIP_E911_SUPPORT
      cmph_sub_info_s_type *ph_sub_ptr = NULL;
      cmph_sub_info_s_type *ph_cmd_sub_ptr = NULL;
#endif

      for ( i = 0; i < call_cmd_ptr->info.num_end_call_ids; i++ )
      {

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /* cmipcall should end calls of info_type IP only
        */
        if (cmcall_call_id_mode (call_cmd_ptr->info.end_params[i].call_id)
            != CM_CALL_MODE_INFO_IP)
        {
          continue;
        }
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        call_ptr = cmcall_ptr( call_cmd_ptr->info.end_params[i].call_id);

        if (call_ptr == NULL)
        {
          CM_ERR_1("Invalid call_ptr from call_id %d",
                   call_cmd_ptr->info.end_params[i].call_id);

          continue;
        }

        /* If the origination has not been sent to the lower layer,
        ** do not send a call end request to the lower layer.
        */
        if ((call_ptr->call_state == CM_CALL_STATE_ORIG) &&
            (call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_NONE))
        {
          #if defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)
          sys_modem_as_id_e_type esr_sub = SYS_MODEM_AS_ID_1;
          
          if (call_ptr->mm_id.asubs_id == SYS_MODEM_AS_ID_1)
          {
            esr_sub = SYS_MODEM_AS_ID_2;
          }
          else
          {
            esr_sub = SYS_MODEM_AS_ID_1;
          }
          if(((call_ptr->call_type == CM_CALL_TYPE_VOICE) ||
             (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY)) &&
             cmph_is_msim_standby_esr_enabled() &&
             (call_ptr->is_waiting_on_esr_rsp == TRUE) &&
             (cmmsimc_get_esr_state(esr_sub) == CMSIMC_ESR_RSP_PENDING)
            )
          {
            cmcsfbcall_send_csfb_abort_req(call_ptr->call_id, esr_sub);
            cmmsimc_set_esr_state(esr_sub, CMSIMC_ESR_NONE);
            call_ptr->is_waiting_on_esr_rsp = FALSE;
          }
          #endif

          break;
        }
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef  FEATURE_VOIP_E911_SUPPORT
        ph_sub_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);
        ph_cmd_sub_ptr = cmph_get_sub_ptr(call_cmd_ptr->cmd_subs);

        /* If the call is an emergency call in origination state
        ** start the cancel retry timer
        */
        if (ph_sub_ptr != NULL && ph_cmd_sub_ptr != NULL)
        {
          uint8 voip_cancel_retry_period = ph_sub_ptr->ip_pref.voip_cancel_retry_period;
          if (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
              call_ptr->call_state == CM_CALL_STATE_ORIG    &&
              voip_cancel_retry_period != 0 )
          {
            ph_cmd_sub_ptr->ip_pref.voip_cancel_retry_uptime = time_get_uptime_secs() +
                voip_cancel_retry_period;
            CM_MSG_MED_2("voip911: retry period %d, retry timer expiry %d",
                         voip_cancel_retry_period,
                         ph_cmd_sub_ptr->ip_pref.voip_cancel_retry_uptime);
          }
        }
#endif
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        /* End the call
        */

#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900)) && \
       defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE) && defined(FEATURE_1X_SRVCC)

        /* handle call end after sending HO_EUTRA RSP to 1x */
        if(call_ptr->srvcc_prog_state == CM_SRVCC_STATE_HO_EUTRA_RSP_SENT)
        {
          mc_ptr  = cm_mc_get_buf_else_err_fatal();

          mc_ptr->hdr.cmd = MC_SRVCC_CALL_END_F;
          mc_ptr->srvcc_call_end.call_id = call_ptr->call_id;
          mc_ptr->srvcc_call_end.end_reason = (byte)(call_ptr->end_params.end_params.ip_cause.cause);

          cm_mc_send_cmd( call_ptr->mm_id.asubs_id,mc_ptr );

        }
#endif

        /* To CM cleints, the end_reason would still go as CLIENT_END */
        call_ptr->end_status = (cm_call_end_e_type) call_cmd_ptr->info.end_params[i].end_params.ip_cause.cause;

        /* Inform IMS to end the Call */
        cmipcall_send_end (call_ptr);
      }

      break;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /*---------------------------*/
    /*   Call related SUPS CMD   */
    /*---------------------------*/

    case CM_CALL_CMD_SUPS:

      call_ptr = cmcall_ptr (cmd_info_ptr->call_id);
      if(call_ptr == NULL)
      {
        CM_MSG_HIGH_0( "cmd sups: call ptr null");
        break;
      }

      /* ASSERT ON call_ptr not required because cmipcall_sups_cmd_check()
      ** already does the checking !!!!!
      */

      if(cmd_info_ptr->sups_cmd_type == CM_CALL_SUPS_TYPE_RELEASE_CALL_X_FROM_CONFERENCE
          || cmd_info_ptr->sups_cmd_type == CM_CALL_SUPS_TYPE_ADD_PARTICIPANT)
      {
        *(&(call_ptr->cmcall_mode_info.info.ip_info.num)) = *(&(cmd_info_ptr->num));
      }

      if(((cmd_info_ptr->sups_cmd_type == CM_CALL_SUPS_TYPE_HOLD_CALL) ||
          (cmd_info_ptr->sups_cmd_type == CM_CALL_SUPS_TYPE_ACTIVE_CALL_REL)) &&
          cmd_info_ptr->is_ip_call_info_present)
      {
        call_ptr->cmcall_mode_info.info.ip_info.ip_sups_param = cmd_info_ptr->ip_call.ip_sups_param;
      }
      cmipcall_proc_sups_cmd_type (cmd_info_ptr, call_ptr);

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_CALL_CMD_MO_MODIFY_REQ:
      call_ptr = cmcall_ptr (cmd_info_ptr->call_id);
      if(call_ptr == NULL)
      {
        CM_MSG_HIGH_0( "modify req: call ptr null");
        break;
      }
      call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib = cmd_info_ptr->ip_call.ipcall_attrib_info;
      call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type = cmd_info_ptr->ip_call.new_call_type_req;
      cmipcall_modify_req( call_ptr);
      break;

    case CM_CALL_CMD_MODIFY_RES:
      call_ptr = cmcall_ptr (cmd_info_ptr->call_id);
      if(call_ptr == NULL)
      {
        CM_MSG_HIGH_0( "modify rsp: call ptr null");
        break;
      }
      call_ptr->cmcall_mode_info.info.ip_info.call_modify_response = cmd_info_ptr->ip_call.modify_response;
      call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib = cmd_info_ptr->ip_call.ipcall_attrib_info;
      call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type = cmd_info_ptr->ip_call.new_call_type_req;
      if(cmd_info_ptr->ip_call.modify_response == FALSE)
      {
        call_ptr->call_type = cmd_info_ptr->ip_call.new_call_type_req;
      }
      cmipcall_modify_rsp( call_ptr);
      break;


      case CM_CALL_CMD_CANCEL_IMS_CALL_REQ:
        {
            call_ptr = cmcall_ptr(cmd_info_ptr->call_id);
            if(call_ptr == NULL)
            {
              CM_MSG_HIGH_0("cancel ims call req:call_ptr null");
              break;
            }
                
            call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.cancel_ims_call_req =
                 cmd_info_ptr->ip_call.ipcall_attrib_info.cancel_ims_call_req;
            cmipcall_cancel_ims_call_req(call_ptr);
            break;
        }

    default:

      CM_ERR_1( "Bad call command %d", call_cmd_ptr->cmd);


  }

  return FALSE;
} /* cmipcall_client_cmd_forward_ll () */



/*===========================================================================

FUNCTION cmipcall_client_cmd_ll_reply

DESCRIPTION
  Forward a client call commands to Lower Layer.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void cmipcall_client_cmd_ll_reply(

  const cm_call_cmd_s_type    *call_cmd_ptr
  /* Pointer to a CM command */
)
{
  cmcall_s_type                        *call_ptr      = NULL;
  /* Pointer to a call object */

  const cm_call_cmd_info_s_type    *cmd_info_ptr     = NULL;
  /* Pointer to call cmd info */

  int                                   i             = 0;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  CM_ASSERT( call_cmd_ptr != NULL );
  CM_ASSERT( call_cmd_ptr->cmd_type == CM_CMD_TYPE_CALL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd_info_ptr  = CMD_INFO_PTR(call_cmd_ptr);

  if (call_cmd_ptr->cmd != CM_CALL_CMD_END)
  {
    if ((call_ptr = cmcall_ptr ( call_cmd_ptr->info.call_id)) == NULL)
    {
      sys_err_fatal_null_ptr_exception();
      /*
      ** 527: Unreachable
      ** This is because for some targets _ERR_FATAL may evaluate with sth
      ** with exit(1) in it but on others error fatals may be turned off
      */
      /*lint -save -e527 */
      return;
      /*lint -restore */
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch( call_cmd_ptr->cmd )
  {
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*---------------------------*/
    /* Response to a setup message*/
    /*---------------------------*/

    case CM_CALL_CMD_SETUP_RES:
      /* No fields to copy into call object as of now
      */

      /* if setup was rejected, send end to clients else no event is sent
      */
      if (call_cmd_ptr->info.end_status == CM_CALL_END_SETUP_REJ)
      {

        /* Send call end request for the call
        */
        cmipcall_send_end (call_ptr);
      }

      /* Change incom substate to setup processed.
      */
      call_ptr->call_subst.incom =  CM_CALL_INCOM_SUBST_SETUP_PROCESSED;

      cmcall_event (call_ptr, CM_CALL_EVENT_SETUP_RES);
      break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /* Answer an incoming call */
    /*-------------------------*/

    case CM_CALL_CMD_ANSWER:

      /* Change incom substate to answer processed
      */
      call_ptr->call_subst.incom =  CM_CALL_INCOM_SUBST_ANSWERED;


      /* No fields to copy into call object as of now
      */

      if ((call_cmd_ptr->info.end_status == CM_CALL_END_INCOM_REJ) 
|| (call_cmd_ptr->info.end_status == CM_CALL_END_IP_UNWANTED_CALL)
 
)
      {

        /* Send call end request for the call
        */
        cmipcall_send_end (call_ptr);

      }
      else
      {
        cmcall_event( call_ptr, CM_CALL_EVENT_ANSWER );
      }

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*-------------------------*/
    /* End the call            */
    /*-------------------------*/

    case CM_CALL_CMD_END:

      for ( i=0; i < call_cmd_ptr->info.num_end_call_ids; i++ )
      {
        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /* cmipcall should end calls of info_type IP only
        */
        if (cmcall_call_id_mode (call_cmd_ptr->info.end_params[i].call_id)
            != CM_CALL_MODE_INFO_IP)
        {
          continue;
        }

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        call_ptr = cmcall_ptr(call_cmd_ptr->info.end_params[i].call_id);

        if (call_ptr == NULL)
        {
          CM_MSG_HIGH_0( "call_ptr_NULL");
        }
        else
        {

          call_ptr->end_status = CM_CALL_END_CLIENT_END;

          *(&(call_ptr->end_params)) = *(&(call_cmd_ptr->info.end_params[i]));

          if( (call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_NONE) &&
              (call_ptr->call_state == CM_CALL_STATE_ORIG))
          {
            cmipcall_end_req( call_ptr );
            cmcall_end( call_ptr );
          }
          else
          {
            cmipcall_end_req(call_ptr);
          }
        }

      }

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_CALL_CMD_SUPS:

      call_ptr = cmcall_ptr (call_cmd_ptr->info.call_id);
      if(call_ptr==NULL)
      {
        CM_MSG_HIGH_0( "call_ptr_NULL");
        break;
      }

      switch (cmd_info_ptr->sups_cmd_type)
      {

        case CM_CALL_SUPS_TYPE_ACT_ANSWER_HOLD:

          /* Change incom substate to answered
          */
          call_ptr->call_subst.incom =  CM_CALL_INCOM_SUBST_ANSWERED;

          cmcall_event( call_ptr, CM_CALL_EVENT_ANSWER );

          break;


        default:
          CM_MSG_MED_1("Unknown command type received = %d",
                       cmd_info_ptr->sups_cmd_type );
          break;
      }

      /* Update call fields by copying relevant
      ** command fields into call struct.
      */
      cmcall_cmd_copy_fields( call_ptr, call_cmd_ptr );

      /* Send CM_CALL_EVENT_SUPS to clients
      ** IP call specific fields have not been added to
      ** call object. sups_type and sups_cmd_params
      ** for ip call are not sent to clients in
      ** CALL_EVENT_SUPS.
      */
      cmcall_event( call_ptr, CM_CALL_EVENT_SUPS );

      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_CALL_CMD_MO_MODIFY_REQ:

      cmcall_event( call_ptr, CM_CALL_EVENT_MODIFY_REQ);
      break;

    case CM_CALL_CMD_MODIFY_RES:

      cmcall_event (call_ptr, CM_CALL_EVENT_MODIFY_RES);
      break;
	
	case CM_CALL_CMD_CANCEL_IMS_CALL_REQ:
  	  call_ptr = cmcall_ptr (call_cmd_ptr->info.call_id);
      if(call_ptr==NULL)
      {
        CM_MSG_HIGH_0( "call_ptr_NULL");
      }
	  break;
	
    default:

      CM_ERR_1( "Bad call command %d", call_cmd_ptr->cmd);



  } /* switch () */
} /* cmipcall_client_cmd_ll_reply () */


/*===========================================================================

FUNCTION cmipcall_client_cmd_proc

DESCRIPTION
 Handles call commands for CM IP call interface

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmipcall_client_cmd_proc
(
  cm_call_cmd_s_type  *call_cmd_ptr
  /* pointer to a CM command */

)
{
  cm_call_cmd_err_e_type    call_cmd_err;

  boolean                   wait_for_reply;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  CM_ASSERT( call_cmd_ptr != NULL );

  CM_ASSERT( call_cmd_ptr->cmd_type == CM_CMD_TYPE_CALL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Start by checking whether this is a valid command
  ** relative to the current state of the call/phone.
  */
  call_cmd_err = cmipcall_client_cmd_check( call_cmd_ptr );

  /*
  ** Notify clients of command processing status.
  ** If error found, return.
  */
  cmcall_client_cmd_err( call_cmd_ptr, call_cmd_err );

  if( call_cmd_err != CM_CALL_CMD_ERR_NOERR )
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If we got here, no errors were found,
  ** so forward the client command to Lower Layer.
  */
  wait_for_reply = cmipcall_client_cmd_forward_ll( call_cmd_ptr );


  /* If NO need to wait for reply from IP App,
  ** call the reply function right a way.
  */
  if( ! wait_for_reply )
  {
    /* Comes here only when cmd_err is NO_ERR, so need not send
    ** cmd_err as an argument
    */
    cmipcall_client_cmd_ll_reply(call_cmd_ptr);

  }

} /* cmipcall_client_cmd_proc () */

/*===========================================================================

FUNCTION cmipcall_send_orig

DESCRIPTION
  Directs a call origination to a VoIP app after matching an app with the
  right system mode and capability,

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  TRUE : If origination command was succesfully sent to IP APP.
  FALSE: If origination command was not sent to IP APP.

SIDE EFFECTS
  Nothing.

===========================================================================*/
boolean cmipcall_send_orig
(
  cmcall_s_type           *call_ptr
  /* Call object for the originating call
  */
)
{
  cmipapp_id_type app_id = CMIPAPP_ID_INVALID;
  cm_num_s_type   num;
  cm_cc_result_e_type cc_result;
  cm_call_type_e_type effective_call_type;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  CM_ASSERT (call_ptr != NULL);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  
  effective_call_type = cmipcall_calculate_effective_call_type_special_emerg(call_ptr);
   
  app_id = cmipapp_find_active_app(call_ptr->sr.orig_sent_on_mode,
                                   effective_call_type,
                                   call_ptr->mm_id.asubs_id);
  if(app_id == CMIPAPP_ID_INVALID)
  {
    CM_MSG_HIGH_2("CMIPAPP: No app matching call %d sys_mode %d",
                  effective_call_type,
                  call_ptr->sr.orig_sent_on_mode);
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if(call_ptr->is_em_vt)
  {
    CM_MSG_HIGH_1("For call_type %d, set em_type to VT",call_ptr->call_type);
    call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.em_type = CM_CALL_EM_VT;
  }


  if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY )
  {
    mm_sub_stk_id_s_type local_mm_id;
    local_mm_id.asubs_id = call_ptr->mm_id.asubs_id;
    local_mm_id.stk_id = cmcall_det_ss_for_volte_call(call_ptr);
    sd_ss_ind_ims_emerg_call_state(local_mm_id,
                                   (sys_volte_call_state_e_type)CM_CALL_STATUS_START);
  }

  /*(a)set is_emerg_ps_redial to TRUE to remember that call has been sent to PS once
  ** and further  PS attempts are not allowed (from cs to ps redial handling).
  ** (b) If this call origination is being sent as part of redialling E911 call from CS to PS,
  **     Set is_emerg_ps_redial_attempt as FALSE so after call fails on IMS, it should be able to 
  **     go over CSFB again.  
  */
  if(call_ptr->sys_mode == SYS_SYS_MODE_LTE)
  {
    CM_MSG_HIGH_0("Set is_ps_attempt_done to TRUE");
    call_ptr->is_ps_attempt_done = TRUE;
  }
  
  if(call_ptr->is_emerg_ps_redial_attempt)
  {
    CM_MSG_HIGH_0("Reset is_emerg_ps_redial_attempt");
    call_ptr->is_emerg_ps_redial_attempt = FALSE;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  num = call_ptr->num;

#ifdef CM_GW_SUPPORTED
  cc_result = call_ptr->gw_cc_modified_info.call_control_result;
#else
  cc_result = call_ptr->cdma_cc_modified_info.call_control_result;
#endif

  if ( cc_result != CM_CC_RESULT_ALLOWED_NO_MODIFICATIONS )
  {
#ifdef CM_GW_SUPPORTED
    num = call_ptr->gw_cc_modified_info.num;
#else
    num = call_ptr->cdma_cc_modified_info.num;
#endif
  }

#if defined(FEATURE_HA_MAINLINE_ONLY_ENABLE)
  /* For emergency call, if MCC is USCC in Home or domestic roaming,
  ** emergency category is set to none. CM will query into PM for the MCC
  ** information. The PM API returns the third argument (in this case FALSE)
  ** if the tag is not present in PM XML. If the tag is found, the PM API
  ** will return TRUE if the MCC is USCC in home or domestic roaming network
  ** and FALSE otherwise.
  */
  if(effective_call_type == CM_CALL_TYPE_EMERGENCY &&
     policyman_get_subs_boolean_with_default("cm:e911_urn_subcategory",
       call_ptr->mm_id.asubs_id, FALSE))
  {
    CM_MSG_HIGH_2("CMIPAPP: sub %d, setting emerg_srv_categ to none as per uscc requirement: %d->0",
                  call_ptr->mm_id.asubs_id,
                  num.emerg_srv_categ);
    num.emerg_srv_categ = CM_EMERG_NUM_TYPE_NONE;
  }
#endif

  CM_MSG_MED_2("CMIPAPP: cc_result %d, app_id %d",cc_result,app_id);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Give the origination to an active app
  */
  if(!cmipapp_send_orig_cmd(app_id,
                            num,
                            call_ptr->call_id,
                            cmipcall_map_cm_calltype_to_ip_calltype(effective_call_type),
                            cmipcall_map_sys_mode_to_ip_sys_mode(call_ptr->sr.orig_sent_on_mode),
                            &(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info),
                            call_ptr->calling_num.pi,
                            call_ptr->mm_id.asubs_id))
  {
    CM_ERR_0 ("Sending origination to app failed");
    return FALSE;
  }

  /*VOWIFI MO orig, notify cmkpi for KPI measurements.
  */
  if((call_ptr->call_type == CM_CALL_TYPE_VOICE || call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
    call_ptr->is_vowlan)
  {
    cmkpi_qsh_event_handler(CM_CALL_EVENT_ORIG, call_ptr);
  }

  /* If we are here then call has been sent to active app on the sys_mode selected. 
  ** We will set is_wlan_attempted to TRUE, if the selected sys_mode is WLAN. 
  */
  
  if(call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_WLAN)
  {
    call_ptr->is_wlan_attempted = TRUE;
  }

  cmcall_check_and_send_mmtel_status(call_ptr,TRUE);

  if(call_ptr->sys_mode == SYS_SYS_MODE_LTE)
  {
    cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_VOLTE_CALL,
                                          call_ptr->mm_id.asubs_id,
                                          call_ptr->mm_id.stk_id,
                                          TRUE);
  }

  /* For MO/MT wifi call, start the special mapping on the call sub
  */
  if(call_ptr->sys_mode == SYS_SYS_MODE_WLAN)
  {
    cmph_update_sp_special_priority_mode(FALSE, TRUE, call_ptr->mm_id.asubs_id);
  }

  call_ptr->ipapp_id = app_id;

  /* Set flag indicating orig was sent for IP call 
  */
  if(call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_IP &&
     (((call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_LTE ||
        call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_WLAN) &&
        call_ptr->sr.is_ok_to_orig) ||
      (call_ptr->sr.orig_sent_on_mode == SYS_SYS_MODE_HDR &&
       call_ptr->sr.hdr_is_ok_to_orig)))
  {
    CM_MSG_HIGH_1("Set is_ip_orig_sent to TRUE for call id %d", call_ptr->call_id);
    call_ptr->is_ip_orig_sent = TRUE;
  }

  return TRUE;

} /* cmipcall_send_orig () */


/*===========================================================================

FUNCTION cmipcall_invite_res

DESCRIPTION
  Sends client response to invite received from network.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_invite_res
(
  const cmcall_s_type            *call_ptr,
  /* call object
  */

  boolean                         is_accept,
  /* TRUE or FALSE depending on if call is accepted or rejected
  */

  cmipapp_incom_rej_cause_e_type  cause
  /* causes for rejecting an incoming call. Can be set to
  ** CMIPAPP_INCOM_REJ_CAUSE_NONE for is_accept = TRUE
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(!cmipapp_send_invite_response(call_ptr->ipapp_id, call_ptr->call_id,
                                   is_accept, cause, call_ptr->mm_id.asubs_id))
  {
    CM_ERR_0 ("Sending invite response to app failed");
    return;
  }

} /* cmipcall_invite_res () */


/*===========================================================================

FUNCTION cmipcall_answer

DESCRIPTION
  Sends user response to an incoming call

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_answer
(
  cmcall_s_type                   *call_ptr,
  /* call object
  */

  boolean                         is_accept,
  /* TRUE or FALSE depending on if call is accepted or rejected
  */

  cmipapp_incom_rej_cause_e_type  cause,
  /* causes for rejecting an incoming call. Can be set to
  ** CMIPAPP_INCOM_REJ_CAUSE_NONE for is_accept = TRUE
  */

  cm_ip_call_sip_reject_code_s_type      sip_reject_code
  /* SIP code sent for rejecting incoming call*/
)
{

  cm_ue_based_sups_config_s_type     cm_ue_based_sups_config;

    volte_sub_info_s_type *volte_sub_info_ptr;

  CM_ASSERT (call_ptr != NULL);

    volte_sub_info_ptr = cmph_get_volte_sub_info_ptr(call_ptr->mm_id.asubs_id);


  cm_ue_based_sups_config = cmsups_map_ss_code_to_sups_config(colr,call_ptr->mm_id.asubs_id);

  /* If user answers the call with default PI value set the corresponding PI value based on the status of TIR, 
  ** if TIR mode is Temporary.
  */
  if(cm_ue_based_sups_config.use_ue_based_sups == TRUE && volte_sub_info_ptr->sups_mode[cmsups_map_code_to_index(colr)] == CM_SUPS_TEMPORARY_MODE)
  {
  if(call_ptr->answer_num_pi == 0xFF)
    {
       /* TIR enabled means, restriction is in place */
      if(cm_ue_based_sups_config.is_enabled)
        {
        call_ptr->answer_num_pi = CM_PRESENTATION_RESTRICTED;
        }
        else
        {
        call_ptr->answer_num_pi = CM_PRESENTATION_ALLOWED;
        }
    }
    CM_MSG_HIGH_1("PI value = %d", call_ptr->answer_num_pi);
}

  /* Send the user response about answering the call
  */
  if(!cmipapp_send_answer_cmd(
        call_ptr->ipapp_id, call_ptr->call_id, is_accept, cause,
        cmipcall_map_cm_calltype_to_ip_calltype(call_ptr->call_type),
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info,
        call_ptr->answer_num_pi,sip_reject_code,
        call_ptr->mm_id.asubs_id))

  {
    CM_ERR_2 ("Sending call answer to app failed, call_id %d, cm_call_type=%d",
              call_ptr->call_id, call_ptr->call_type);
    return;
  }

  /* If sending answer cmd to IMS success process answer cmd on ims Cache.
  */
#if defined(FEATURE_IP_CALL) && defined(FEATURE_CM_LTE)
  CM_MSG_HIGH_0("Process answer cmd on ims cache");
  cm_process_answer_cmd_on_ims_cache();
#endif

} /* cmipcall_answer () */


/*===========================================================================

FUNCTION cmipcall_simple_flash

DESCRIPTION
  Sends user response to an call waiting indication or request by the user to
  remove the last party added.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_simple_flash
(
  const cmcall_s_type                   *call_ptr
  /* call object
  */
)
{
  cm_mm_call_info_s_type     *call_info_ptr = NULL;
  /* Call information pointer */

  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the user response about answering the call waiting or
   * user request to remove the last party added.
   */

  if (!cmipapp_send_answer_call_waiting_response(call_ptr->ipapp_id,
      call_ptr->call_id,
      call_ptr->mm_id.asubs_id))
  {
    CM_MSG_MED_0 ("Sending call wait answer to app failed");

    call_info_ptr = cmcall_info_alloc();

    (void) cmcall_info_get( call_ptr, call_info_ptr );

    if (!cmipapp_send_simple_flash_cmd(call_ptr->ipapp_id, call_ptr->call_id,
                                       call_info_ptr, call_ptr->mm_id.asubs_id))
    {
      CM_ERR_0 ("Answer to Call wait or remove last party to app failed");
    }

    /* Deallocate the memory after processing call info. */
    cm_mem_free(call_info_ptr);
  }

} /* cmipcall_simple_flash () */


/*===========================================================================

FUNCTION cmipcall_send_end

DESCRIPTION
  Ends a call

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_send_end
(
  cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Request to end the call */
  if(!cmipapp_send_call_end_cmd(call_ptr->ipapp_id,
                                call_ptr->call_id,
                                call_ptr->end_status,
                                call_ptr->mm_id.asubs_id))
  {
    CM_MSG_MED_1 ("Call end request failed, call_id %d", call_ptr->call_id);
    return;
  }

  call_ptr->is_ip_orig_sent = FALSE;

} /* cmipcall_send_end () */


/*===========================================================================

FUNCTION cmipcall_send_abort

DESCRIPTION
  Ends a call

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_send_abort
(
  cmcall_s_type                  *call_ptr,
  /* call object
  */

  const cmipapp_abort_cause_e_type     abort_cause
  /* The call abort cause enum type.
  */
)
{

  CM_ASSERT (call_ptr != NULL);

  /* Request to abort the call
  */
  if(call_ptr->sys_mode == SYS_SYS_MODE_LTE)
  {
    if(CM_CALL_ID_INVALID == cmcall_is_there_volte_call_sub_ignore(call_ptr->mm_id.asubs_id,
                                                                   call_ptr->call_id))
    {
      cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_NONE,
                                            call_ptr->mm_id.asubs_id,
                                            call_ptr->mm_id.stk_id,
                                            TRUE);
    }
  }

  if (!cmipapp_send_abort_call_cmd(call_ptr->ipapp_id, call_ptr->call_id,
                                   abort_cause, call_ptr->mm_id.asubs_id))
  {
    CM_MSG_MED_1 ("Call abort request failed, call_id %d", call_ptr->call_id);
    return;
  }
  
  call_ptr->is_ip_orig_sent = FALSE;

} /* cmipcall_send_abort() */


/*===========================================================================

FUNCTION cmipcall_end

DESCRIPTION
  Deallocates call object and informs phone of a call end

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Call object gets deallocated.

===========================================================================*/
void cmipcall_end
(
  cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  cm_act_update_reas_e_type update_reason = CM_ACT_UPDATE_REAS_ACT_END;
  boolean mt_call_in_setup = FALSE;
  mm_sub_stk_id_s_type mm_id = call_ptr->mm_id;
  cmsds_sub_prop_s_type *sds_sub_prop_ptr;
  cmph_s_type* ph_ptr = cmph_ptr();
  sd_si_info_s_type *ss_true_stk_info_ptr=NULL;
  cm_call_state_e_type prev_call_state = CM_CALL_STATE_NONE;
  cm_call_incom_subst_e_type prev_call_subst_incom = CM_CALL_INCOM_SUBST_NONE;

  CM_ASSERT (call_ptr != NULL);

  if((mm_id.asubs_id < SYS_MODEM_AS_ID_1) || (mm_id.asubs_id >= MAX_SIMS) )
  {
    return;
  }

  if(ph_ptr->sub_info[mm_id.asubs_id] == NULL)
  {

    return;
  	
  }

  /* IP call ended before receiving ring cnf */
  if (call_ptr->call_state == CM_CALL_STATE_INCOM 
     &&
       (call_ptr->call_subst.incom == CM_CALL_INCOM_SUBST_SETUP ||
      call_ptr->call_subst.incom == CM_CALL_INCOM_SUBST_SETUP_PROCESSED ||
      call_ptr->call_subst.incom == CM_CALL_INCOM_SUBST_PRE_RINGING) 
  )
  {
    mt_call_in_setup = TRUE;
    prev_call_state = call_ptr->call_state;
    prev_call_subst_incom = call_ptr->call_subst.incom;
  }

  cmcall_check_and_send_mmtel_status(call_ptr,FALSE);

  /* Change the call state to idle.
  */
  call_ptr->call_state       = CM_CALL_STATE_IDLE;
  call_ptr->call_subst.idle  = CM_CALL_IDLE_SUBST_NONE;

  /*Update the end status for CMKPI to use*/
  call_ptr->end_params.end_params.ip_cause.end_status_for_kpi = call_ptr->end_status;

  /* We need to clear the state to CMIPCALL_SUPS_ST_NONE for the call to be ended.
  */
  cmipcall_mng_state_update(CM_IP_CALL_IND_CALL_END, TRUE, call_ptr->call_id, CM_IPAPP_CAUSE_NORMAL);

  /* Tell phone that call is over.
     Number of allocated calls equal to 1 means that this is the
     last call that is being ended, so we need to notify the phone now.
  */

  if(call_ptr->end_status == CM_CALL_END_EMERGENCY_ONLY ||
      call_ptr->end_status == CM_CALL_END_THERMAL_EMERGENCY)
  {
    update_reason = CM_ACT_UPDATE_REAS_EMERG_ENTER;
  }

  if(call_ptr->end_cause_to_client != CM_CALL_END_NONE)
  {
    CM_MSG_MED_4("CALL_END: CM->IMS: CMIPAPP: end_status %d, end_cause_to_client %d, call_id %d, sub %d",
                    call_ptr->end_status,
                    call_ptr->end_cause_to_client,
                    call_ptr->call_id,
                    call_ptr->mm_id.asubs_id);
    call_ptr->end_status = call_ptr->end_cause_to_client;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the queues for this activity
  ** serach the Queues and if the activity is found, update its uptime a
  ** and the update reason
  */
  {
    mm_sub_stk_id_s_type local_mm_id = mm_id;
    for( local_mm_id.stk_id = MM_STACK_0;
         local_mm_id.stk_id < cmtask_get_num_stacks(local_mm_id.asubs_id);
         local_mm_id.stk_id++)
    {
      (void) cmph_update_orig_param((cm_act_id_type) call_ptr->call_id,
                                    local_mm_id,
                                    update_reason );
    }
  }

  cmph_call_end_process(call_ptr);

  if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY)
  {
    cmac_enable_emergency_if_pending();
  }
  /* Notify clients of call end event.
  */
  cmcall_update_call_end_info( call_ptr );

  /* Donot send end event, when call waiting for reorig phase2
  ** Also, if we are in INCOM; SETUP/SETUP_PROCESSED substate then we should not send END EVENT
  ** as MT_INCOM event is also not sent
  */
  if(call_ptr->cc_object_ptr == NULL && !mt_call_in_setup &&
     call_ptr->call_subst.incom != CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT)
     /*for INVIT RSP reject no need to send call end event as there 
        was no call setup*/
  {
    cmcall_event( call_ptr, CM_CALL_EVENT_END );
  }
  else
  {
     cmkpi_qsh_event_handler(CM_CALL_EVENT_END, call_ptr);
     #if defined(FEATURE_QSH_EVENT_METRIC)
  /* update and send call end metrics */
     cmdbg_chk_and_update_call_end_info_metrics(CM_CALL_EVENT_END, call_ptr);
  /** Notify QSH of call start/end/drop/setup failure.
  */
     cmdbg_qsh_send_call_evts(CM_CALL_EVENT_END,call_ptr,NULL);
  #endif
     if( call_ptr->direction == CM_CALL_DIRECTION_MT &&
         call_ptr->call_subst.incom != CM_CALL_INCOM_SUBST_INVITE_RSP_REJECT )
       /* for invite rsp reject, QMI reject would have send 
           in MT invite processing itself no need to send from here*/
     {
      /* As CM is not sending the call end event to QMI, we will need to send the 
      ** MM_CM_MT_CALL_END_AUTO_REJECT_IND to QMI.
      ** No need to pass call composer info as it has been updated to QMI earlier when cm
      ** sends CM_CALL_EVENT_PRE_INCOM.
      */
      cmcall_send_mt_call_end_auto_reject_ind(
            call_ptr->mm_id.asubs_id,
            call_ptr->call_type,
            call_ptr->cmcall_mode_info.info.ip_info.sip_error_code,
            call_ptr->end_cause_to_client,
            call_ptr->num,
            call_ptr->cmcall_mode_info.info_type,
            NULL,
            call_ptr->mt_call_verstat.call_verstat);
    }

    /* Send call end so that QMI can clean up the call */
    if(prev_call_state == CM_CALL_STATE_INCOM &&
       prev_call_subst_incom == CM_CALL_INCOM_SUBST_PRE_RINGING)
    {
      cmcall_event( call_ptr, CM_CALL_EVENT_END );
     }
  }

  cmipapp_release_reference_end_reason(call_ptr);
  cmipapp_release_reference_alternate_uri(call_ptr);
  sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(mm_id.asubs_id);
  cm_call_id_deallocate( cmcall_call_id_get( call_ptr ) );

  /* update DDTM status */
  cmph_update_ddtm_status(mm_id.asubs_id);

  if(CM_CALL_ID_INVALID == cmcall_is_there_volte_call_sub(mm_id.asubs_id) &&
  	ph_ptr->sub_info[mm_id.asubs_id]->ip_call_end_rsp_pend  == FALSE)
  {
    cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_NONE,
                                          call_ptr->mm_id.asubs_id,
                                          call_ptr->mm_id.stk_id,
                                          TRUE);
  }

  /* For MO/MT wifi call end, end the special mapping on the call sub
  */
  if(CM_CALL_ID_INVALID == cmcall_is_there_vowlan_call_sub(mm_id.asubs_id))
  {
    cmph_update_sp_special_priority_mode(FALSE, FALSE, mm_id.asubs_id);
  }

  /* if ther is no call over IMS then update gsdi about traffic */
  if(CM_CALL_ID_INVALID == cmcall_is_there_volte_call_sub(mm_id.asubs_id) &&
     CM_CALL_ID_INVALID == cmcall_is_there_vowlan_call_sub(mm_id.asubs_id))
  {
    cmcall_update_gsdi_and_log_call_state(mm_id);
  }


#if defined (FEATURE_IP_CALL) && defined (FEATURE_CM_LTE)
  {
    /*Stop MT CS Page Reject Timer*/
    cmph_sub_info_s_type   *ph_sub_config_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);
    if(ph_sub_config_ptr != NULL)
    {
      cmipcall_set_mt_cs_reject_timer(0,call_ptr->mm_id.asubs_id);
      CM_MSG_HIGH_1("mt_cs_page_reject timer reset for sub[%d]",call_ptr->mm_id.asubs_id);
    }
  }
#endif

  /* Process buffered VOPS ind if any */
#ifdef FEATURE_DOMAIN_SELECTION
  if(sds_sub_prop_ptr != NULL && sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr != NULL)
  {
    mm_id.stk_id = cmph_determine_stk_per_rat(mm_id.asubs_id,
                   sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr->mode);
    ss_true_stk_info_ptr=cmss_true_stack_info_ptr(mm_id);
    if( ss_true_stk_info_ptr!= NULL && ss_true_stk_info_ptr->srv_status ==SYS_SRV_STATUS_SRV &&
        ss_true_stk_info_ptr->is_stable_in_svc==TRUE)
    {
      CM_MSG_MED_2("cmipcall_end,Process buffered srv ind,sub %d stk %d",mm_id.asubs_id, mm_id.stk_id);
      cmsds_process_srv_ind(mm_id, sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr);
    }
    else
    {
      /*clear the stale srv ind*/
      cmsds_reset_srv_ind_buf_ptr(mm_id.asubs_id);
    }
  }
#endif

} /* cmipcall_end */

/*===========================================================================

FUNCTION cmipcall_hold

DESCRIPTION
  Request to put call on hold

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_hold
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Request to put the call on hold
  */
  if (!cmipapp_send_hold_cmd(call_ptr->ipapp_id, call_ptr->call_id, call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Call hold request failed, call_id %d",
              call_ptr->call_id);
    return;
  }

} /* cmipcall_hold () */


/*===========================================================================

FUNCTION cmipcall_active

DESCRIPTION
  Request to put the call active

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_active
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Request to make the call active
  */

  if (!cmipapp_send_resume_cmd(call_ptr->ipapp_id, call_ptr->call_id, call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Call active request failed, call_id %d", call_ptr->call_id);
  }

  return;

} /* cmipcall_active () */

/*===========================================================================

FUNCTION cmipcall_update_calls_on_srv_lost

DESCRIPTION
  Update the call object on service lost

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void   cmipcall_update_calls_on_srv_lost(
  mm_sub_stk_id_s_type  mm_id
)
{
  cm_iterator_type    call_itr;
  /* Call Object Iterator */

  cmcall_s_type       *call_ptr;
  /* Point at call object */

  cmcall_obj_iterator_init(&call_itr);

  call_ptr = cmcall_obj_get_next(&call_itr);

  while(call_ptr != NULL)
  {

    if(( call_ptr->call_state == CM_CALL_STATE_ORIG ) &&
        (call_ptr->cmcall_mode_info.info_type == CM_CALL_MODE_INFO_IP) &&
        (call_ptr->is_vowlan != TRUE))
    {
      if(call_ptr->mm_id.asubs_id == mm_id.asubs_id &&
          (call_ptr->mm_id.stk_id == mm_id.stk_id ||
           call_ptr->mm_id.stk_id == MM_STACK_ALL))
      {
        call_ptr->sr.is_ok_to_orig = FALSE;
      }
    }

    call_ptr = cmcall_obj_get_next(&call_itr);
  }

  return;
}


/*===========================================================================

FUNCTION cmipcall_start_cont_dtmf

DESCRIPTION
  Sends the dtmf for the requested digit

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_start_cont_dtmf
(
  const cmcall_s_type           *call_ptr,
  /* call object
  */

  uint8                    digit
  /* DTMF digit
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Request to make the call active
  */
  if(!cmipapp_send_start_inband_dtmf_cmd(call_ptr->ipapp_id,
                                         call_ptr->call_id, digit,
                                         call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("start dtmf request failed, call_id %d, data_block %d",
              call_ptr->call_id );
  }

  return;

} /* cmipcall_start_cont_dtmf () */

/*===========================================================================

FUNCTION cmipcall_stop_cont_dtmf

DESCRIPTION
  Sends request to stop dtmf operation for the digit

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_stop_cont_dtmf
(
  const cmcall_s_type           *call_ptr,
  /* call object
  */

  uint16                         duration
  /* DTMF digit duration
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Request to stop dtmf.
  */
  if(!cmipapp_send_stop_inband_dtmf_cmd(call_ptr->ipapp_id, call_ptr->call_id,
                                        duration, call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("stop dtmf request failed, call_id %d",
              call_ptr->call_id );
  }

  return;

} /* cmipcall_stop_cont_dtmf () */


/*===========================================================================

FUNCTION cmipcall_cmd_copy_fields

DESCRIPTION
  Copy the active IP fields of a call command into a call object.

DEPENDENCIES
  Call object must have already been initialized with
  cmcall_init().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmipcall_cmd_copy_fields(

  cmcall_s_type               *call_ptr,
  /* Pointer to a call object */

  const cm_call_cmd_s_type    *cmd_ptr
  /* Pointer to a call command */

)
{
  /* Copy the presentation indicator value */
  call_ptr->calling_num.pi = cmd_ptr->info.calling_num.pi;

  call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info = cmd_ptr->info.ip_call.ipcall_attrib_info;
  if(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param != NULL)
  {
    ref_cnt_obj_add_ref(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.ip_gen_param);
  }

  if(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.subject != NULL)
  {
    ref_cnt_obj_add_ref(
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.subject);
  }

  if(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.picture_url != NULL)
  {
    ref_cnt_obj_add_ref(
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.picture_url);
  }
    if(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.org_header!= NULL)
  {
    ref_cnt_obj_add_ref(
      call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.org_header);
  }
}


/*===========================================================================

FUNCTION cmipcall_modify_req

DESCRIPTION
  send call modify request to IMS

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_modify_req
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the user response about answering the call
  */
  if (!cmipapp_send_modify_call_cmd(
        call_ptr->ipapp_id, call_ptr->call_id,
        cmipcall_map_cm_calltype_to_ip_calltype(
          call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type),
        call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib,
        call_ptr->mm_id.asubs_id))
  {
    CM_ERR_2 ("Sending call modify req to app failed, call_id %d, cm_call_type=%d",
              call_ptr->call_id,
              call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type);
    return;
  }

  CM_MSG_HIGH_5 ("Sending IP modify req, call_id  %d, req_call_type %d,\
                current_call_type = %d rtt_mode %d, upgrade_to_secure_call %d", call_ptr->call_id,
                 call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type,
                 call_ptr->call_type,
                 call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib.rtt_mode,
                 call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib.is_secure_call);

} /* cmipcall_modify_req () */



/*===========================================================================

FUNCTION cmipcall_modify_rsp

DESCRIPTION
  send call modify response to IMS

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_modify_rsp
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the user response about answering the call
  */
  if ( !cmipapp_send_modify_response(
         call_ptr->ipapp_id, call_ptr->call_id,
         call_ptr->cmcall_mode_info.info.ip_info.call_modify_response,
         cmipcall_map_cm_calltype_to_ip_calltype(
           call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_type),
         call_ptr->cmcall_mode_info.info.ip_info.req_modify_call_attrib,
         call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1("Sending call modify rsp to app failed, call_id %d",
             call_ptr->call_id);
    return;
  }

} /* cmipcall_modify_rsp () */

/*===========================================================================

FUNCTION cmipcall_cancel_ims_call_req

DESCRIPTION
  send request to IMS to cancel call upgrade/downgrade/cancelHold/cancelResume

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/

void cmipcall_cancel_ims_call_req(

  const cmcall_s_type           *call_ptr
        /* call object
        */
)
{
    
    CM_ASSERT(call_ptr != NULL);
    
    /* Send the cancel request to IMS  */
      if (!cmipapp_send_cancel_ims_call_req_cmd(
             call_ptr->ipapp_id, call_ptr->call_id,
             call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.cancel_ims_call_req,
             call_ptr->mm_id.asubs_id))
      {
        CM_ERR_2 ("Sending ims call cancel req to app failed, call_id %d, cancel_call_req=%d",
                  call_ptr->call_id,
                  call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.cancel_ims_call_req);
        return;
      }
    
      CM_MSG_HIGH_3("Sending IP cancel req, call_id  %d, cancel_ims_call_req %d,\
                    current_call_type = %d ", call_ptr->call_id,
          call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.cancel_ims_call_req,
                     call_ptr->call_type);

}

/*===========================================================================

FUNCTION cmipcall_remove_last_party

DESCRIPTION
  send request to IMS to remove last party from conference

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_remove_last_party
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the user response about answering the call
  */
  if (!cmipapp_send_remove_last_party_cmd(
        call_ptr->ipapp_id,
        call_ptr->cmcall_mode_info.info.ip_info.num,
        call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1("Sending remove last party to app failed, call_id %d",
             call_ptr->call_id);
    return;
  }

} /* cmipcall_remove_last_party () */


/*===========================================================================

FUNCTION cmipcall_add_new_party

DESCRIPTION
  send request to IMS to add a new party to conference

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_add_new_party
(
  const cmcall_s_type           *call_ptr
  /* call object
  */
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send the client cmd to add a new party
  */
  if (!cmipapp_send_add_new_party_cmd(
        call_ptr->ipapp_id,
        call_ptr->call_id,
        call_ptr->cmcall_mode_info.info.ip_info.num,
        call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Sending add new party to app failed, call_id %d",
              call_ptr->call_id);
    return;
  }

} /* cmipcall_add_new_party () */

/*===========================================================================

FUNCTION cmipcall_epdn_rat_change_rpt_proc

DESCRIPTION
  Process rat change indication from IMS and notify clients.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void cmipcall_epdn_rat_change_rpt_proc(
  const cmipapp_rpt_s_type  *rpt_ptr
)
{
  cmph_sub_info_s_type	 *ph_sub_config_ptr;

  CM_ASSERT( rpt_ptr != NULL );
  if (rpt_ptr == NULL)
  {
	CM_ERR_0("rpt_ptr null");
	return;
  }
  
  ph_sub_config_ptr = cmph_get_sub_ptr(rpt_ptr->ip_ind.hdr.asubs_id);

  if (ph_sub_config_ptr == NULL)
  {
	CM_ERR_1("ERROR: invalid ph_sub ptr, id %d",
			   rpt_ptr->ip_ind.ind.call_rat_change.id);
	return;
  }

if(!(cmcall_emergency_call_active() && cmph_is_in_emergency_cb()))
{
  CM_MSG_HIGH_2_EXT("IP RXD: EPDN_RAT_CHANGE, new sys_mode = %d, as_id=%d",
			   rpt_ptr->ip_ind.ind.epdn_rat_change.sys_mode,
			   rpt_ptr->ip_ind.hdr.asubs_id,
			   rpt_ptr->ip_ind.hdr.asubs_id);

  if((ph_sub_config_ptr->emerg_call_prop.ecbm_rat == SYS_SYS_MODE_LTE) && 
  	(rpt_ptr->ip_ind.ind.epdn_rat_change.sys_mode == CMIPAPP_SYS_MODE_WLAN))

  	{
  	  ph_sub_config_ptr->emerg_call_prop.ecbm_rat_change_to_wlan = TRUE;
  	}

  if((ph_sub_config_ptr->emerg_call_prop.ecbm_rat == SYS_SYS_MODE_WLAN) && 
  	(rpt_ptr->ip_ind.ind.epdn_rat_change.sys_mode == CMIPAPP_SYS_MODE_LTE))

  	{
  	  ph_sub_config_ptr->emerg_call_prop.ecbm_rat_change_to_wlan = FALSE;
  	}
  
  ph_sub_config_ptr->emerg_call_prop.ecbm_rat = cmipapp_map_sysmode_to_cm_sysmode(rpt_ptr->ip_ind.ind.epdn_rat_change.sys_mode);
  

  CM_MSG_HIGH_1("ecbm_rat %d",
				 ph_sub_config_ptr->emerg_call_prop.ecbm_rat);

}
  return;
}

/*===========================================================================

FUNCTION cmipcall_rat_change_rpt_proc

DESCRIPTION
  Process rat change indication from IMS and notify clients.

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void cmipcall_rat_change_rpt_proc(
  const cmipapp_rpt_s_type  *rpt_ptr
)
{
  cmcall_s_type *call_ptr = NULL;
  sys_modem_as_id_e_type ps_data_suspend_sub_id = SYS_MODEM_AS_ID_NONE;
  sys_sys_mode_e_type curr_sys_mode;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr;
  cmph_sub_info_s_type   *ph_sub_config_ptr;

  CM_ASSERT( rpt_ptr != NULL );
  if (rpt_ptr == NULL)
  {
    CM_ERR_0("rpt_ptr null");
    return;
  }

  CM_MSG_MED_3("IP RXD: RAT_CHANGE id=%d, sys_mode=%d, as_id=%d",
               rpt_ptr->ip_ind.ind.call_rat_change.id,
               rpt_ptr->ip_ind.ind.call_rat_change.sys_mode,
               rpt_ptr->ip_ind.hdr.asubs_id);

  /* Get the call pointer from the call ID
   */
  call_ptr = cmcall_ptr(rpt_ptr->ip_ind.ind.call_rat_change.id);

  if (call_ptr == NULL)
  {
    CM_ERR_1("ERROR: Unknown call ID in call_rat_change or invalid ph_sub ptr, id %d",
               rpt_ptr->ip_ind.ind.call_rat_change.id);
    CM_ASSERT( call_ptr != NULL );
    return;
  }

  ph_sub_config_ptr = cmph_get_sub_ptr(call_ptr->mm_id.asubs_id);

  if (ph_sub_config_ptr == NULL)
  {
    CM_ERR_1("ERROR: invalid ph_sub ptr, id %d",
               rpt_ptr->ip_ind.ind.call_rat_change.id);
    return;
  }

  CM_MSG_HIGH_1("ecbm_rat %d",
                 ph_sub_config_ptr->emerg_call_prop.ecbm_rat);

  /* Indicate call end in previous sys mode first */
  cmcall_check_and_send_mmtel_status(call_ptr,FALSE);

  /* Update sys_mode based on the rpt
  */
  curr_sys_mode = call_ptr->sys_mode ;

  call_ptr->sys_mode= cmipapp_map_sysmode_to_cm_sysmode(
                        rpt_ptr->ip_ind.ind.call_rat_change.sys_mode);

  if(call_ptr->call_type == CM_CALL_TYPE_EMERGENCY &&
     call_ptr->direction == CM_CALL_DIRECTION_MO &&
     ph_sub_config_ptr->emerg_call_prop.is_wlan_e911_call_supp)
     {
       ph_sub_config_ptr->emerg_call_prop.ecbm_rat = call_ptr->sys_mode;
     }

  /* Update VOWLAN flag when LTE to WLAN HO happens */
  if(curr_sys_mode == SYS_SYS_MODE_LTE && call_ptr->sys_mode == SYS_SYS_MODE_WLAN)
  {
    cmsds_sub_prop_s_type *sds_sub_prop_ptr = cmsds_get_sub_prop_ptr(rpt_ptr->ip_ind.hdr.asubs_id);

    if(call_ptr->call_state == CM_CALL_STATE_ORIG)
    {
      call_ptr->sr.orig_sent_on_mode = SYS_SYS_MODE_WLAN;
    }

    call_ptr->is_vowlan = TRUE;
    call_ptr->is_volte = FALSE;


    if(
       cmph_is_e911_wlan_to_LTE_handover_enabled(call_ptr->mm_id.asubs_id) &&
       
       (call_ptr->call_type == CM_CALL_TYPE_EMERGENCY) &&
       (call_ptr->call_state == CM_CALL_STATE_CONV))
    {
      call_ptr->mm_id.stk_id = cmph_determine_stk_per_rat(call_ptr->mm_id.asubs_id, SYS_SYS_MODE_LTE);
 
      cmph_update_orig_param_incoming_call(call_ptr);

      cmph_update_sp_special_priority_mode(FALSE, TRUE, call_ptr->mm_id.asubs_id);


   	  /* If VOLTE call HO to WLAN, no need to buffer VOPS. kick start domain sel */
#ifdef FEATURE_DOMAIN_SELECTION
	  if(sds_sub_prop_ptr != NULL && sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr != NULL)
	  {
	    mm_sub_stk_id_s_type	local_mm_id;
	
		local_mm_id.asubs_id = rpt_ptr->ip_ind.hdr.asubs_id;
		local_mm_id.stk_id = cmph_determine_stk_per_rat(rpt_ptr->ip_ind.hdr.asubs_id,
								 sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr->mode );
		cmsds_process_srv_ind(local_mm_id, sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr);
	  }
#endif

     
    }

    else
	{

    if(CM_CALL_ID_INVALID == cmcall_is_there_volte_call_sub_ignore(call_ptr->mm_id.asubs_id,
                                                                   call_ptr->call_id))
    {
      cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_NONE,
                                            call_ptr->mm_id.asubs_id,
                                            call_ptr->mm_id.stk_id,
                                            TRUE);
    }

    /* For MO/MT wifi call, start the special mapping on the call sub
    */
    cmph_update_sp_special_priority_mode(FALSE, TRUE, call_ptr->mm_id.asubs_id);

    /* Upon IRAT from LTE to WLAN, remove the call Activity from Priority_Q and
    ** force new orig mode
    */
    cmph_remove_act_id_and_force_orig_mode(call_ptr->mm_id,
                                           (cm_act_id_type)call_ptr->call_id, FALSE);

  
    /*The call is being moved from LTE to WLAN. 
      Resume the data if suspended, meaning set the ps_data_suspend flag to FALSE and trigger SS EVENT.
	  Data is resumed on the other sub. 
    */
    ps_data_suspend_sub_id = cmph_get_other_sub(rpt_ptr->ip_ind.hdr.asubs_id);
    if (ps_data_suspend_sub_id != SYS_MODEM_AS_ID_NONE) 
    {
       cmss_resume_data(TRUE, ps_data_suspend_sub_id);
    }

    /* If VOLTE call HO to WLAN, no need to buffer VOPS. kick start domain sel */
#ifdef FEATURE_DOMAIN_SELECTION
    if(sds_sub_prop_ptr != NULL && sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr != NULL)
    {
      mm_sub_stk_id_s_type  local_mm_id;

      local_mm_id.asubs_id = rpt_ptr->ip_ind.hdr.asubs_id;
      local_mm_id.stk_id = cmph_determine_stk_per_rat(rpt_ptr->ip_ind.hdr.asubs_id,
                             sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr->mode );
      cmsds_process_srv_ind(local_mm_id, sds_sub_prop_ptr->gen_sub_info.srv_ind_buf_ptr);
    }
#endif

  }

  }

  /* WLAN to LTE HO, update volte flag */
  else if (curr_sys_mode == SYS_SYS_MODE_WLAN && call_ptr->sys_mode == SYS_SYS_MODE_LTE)
  {
    if(call_ptr->call_state == CM_CALL_STATE_ORIG)
    {
      call_ptr->sr.orig_sent_on_mode = SYS_SYS_MODE_LTE;
    }

    call_ptr->is_vowlan = FALSE;
    call_ptr->is_volte = TRUE;
    cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_VOLTE_CALL,
                                          call_ptr->mm_id.asubs_id,
                                          call_ptr->mm_id.stk_id,
                                          TRUE);

    /* For MO/MT wifi call end, end the special mapping on the call sub
    */
    if(CM_CALL_ID_INVALID == cmcall_is_there_vowlan_call_sub(call_ptr->mm_id.asubs_id))
    {
      cmph_update_sp_special_priority_mode(FALSE, FALSE, call_ptr->mm_id.asubs_id);
    }

    /* IRAT from WLAN to LTE can be treated as new Incoming call. Just need to
    ** update call object in Priority_Q without sending any SYS_SEL_PREF
    */

    /* In case of Emergency call, if Stack maybe set to SS_MAX,
    ** Update call_ptr stack detail to latest LTE stack
    */
    call_ptr->mm_id.stk_id = cmph_determine_stk_per_rat(call_ptr->mm_id.asubs_id, SYS_SYS_MODE_LTE);
    
     /*The call is being moved from WLAN to LTE. 
      Suspend the data , meaning set the ps_data_suspend flag to TRUE and trigger SS Event.
	  Data is suspended on the othe SUB. 
    */
    ps_data_suspend_sub_id = cmph_get_other_sub(rpt_ptr->ip_ind.hdr.asubs_id);
    if (ps_data_suspend_sub_id != SYS_MODEM_AS_ID_NONE) 
    {
       cmss_process_data_suspend_rpt(
       TRUE, 
       cmph_map_sub_stk_to_id(
       ps_data_suspend_sub_id,MM_STACK_0));

      ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(ps_data_suspend_sub_id);
      if(ss_misc_sub_ptr != NULL)
      {
        ss_misc_sub_ptr->sub_info.gen_sub_info.ue_init_ps_data_suspend = TRUE;
        ss_misc_sub_ptr->sub_info.gen_sub_info.ue_init_ps_data_suspend_cause = CM_SMREG_DUAL_STANDBY_CS_MO_CALL;
      }
    }

    cmph_update_orig_param_incoming_call(call_ptr);
  }

  /* Now, indicate call start on the latest handed over sys mode */
  cmcall_check_and_send_mmtel_status(call_ptr,TRUE);

  CM_MSG_MED_4("call id %d moved from %d to %d, orig_sent_on_mode -> %d",
               call_ptr->call_id, curr_sys_mode, call_ptr->sys_mode, call_ptr->sr.orig_sent_on_mode);


  /* Notify clients
  */
  cmcall_event(call_ptr, CM_CALL_EVENT_RAT_CHANGE);

  return;
}

/*===========================================================================

FUNCTION cmipcall_process_call_deflection

DESCRIPTION
  send request to IMS to handle communication deflection

DEPENDENCIES
  Call object must have been initialized with cmcall_init.

RETURN VALUE
  None.

SIDE EFFECTS
  Nothing.

===========================================================================*/
void cmipcall_process_call_deflection
(
  const cmcall_s_type           *call_ptr,
  /* call object
  */
  const cm_call_cmd_info_s_type    *cmd_info_ptr
)
{
  CM_ASSERT (call_ptr != NULL);
  CM_ASSERT (cmd_info_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Send command to IMS to handle communication deflection on
  ** incoming call
  */
  if (!cmipapp_send_call_deflection_cmd(
        call_ptr->ipapp_id,
        call_ptr->call_id,
        cmd_info_ptr->num,
        call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1 ("Sending communication deflection to app failed, call_id %d",
              call_ptr->call_id);
    return;
  }

} /* cmipcall_process_call_deflection () */

/*===========================================================================

FUNCTION cmipcall_mt_invite_update_call_composer

DESCRIPTION
  Update call composer information from MT invite to call ptr.

RETURN VALUE
  None.

===========================================================================*/
static void cmipcall_mt_invite_update_call_composer(
  cmcall_s_type                    *call_ptr,
  const cm_ip_call_composer_info_s_type  *call_composer_info
)
{
  if (call_composer_info->info_included)
  {
    CM_MSG_HIGH_5("call_composer: MT_INVITE: importance %d, subject 0x%x, url 0x%x, loc incl %d, shape %d",
                     call_composer_info->importance,
                     call_composer_info->subject,
                     call_composer_info->picture_url,
                     call_composer_info->geo_location.location_included,
                     call_composer_info->geo_location.geo_shape);

    call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info
      = *call_composer_info;
  
    if (call_composer_info->subject != NULL)
    {
      CM_MSG_HIGH_2("call_composer: MT_INVITE: subject_len %d, subject_text 0x%x",
                        call_composer_info->subject->subject_length,
                        call_composer_info->subject->subject_text);
      ref_cnt_obj_add_ref(
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.subject);
    }
    if (call_composer_info->picture_url != NULL)
    {
      CM_MSG_HIGH_2("call_composer: MT_INVITE: pic_url 0x%x, pic_url_len %d",
                    call_composer_info->picture_url->pic_url,
                    strlen(call_composer_info->picture_url->pic_url));
      ref_cnt_obj_add_ref(
        call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.picture_url);
    }
	 if (call_composer_info->org_header != NULL)
    {
      CM_MSG_HIGH_2("call_composer: MT_INVITE: org_header_len %d, org_header_text 0x%x",
                        call_composer_info->org_header->org_header_length,
                        call_composer_info->org_header->org_header_text);
      ref_cnt_obj_add_ref(call_ptr->cmcall_mode_info.info.ip_info.ipcall_attrib_info.call_composer_info.org_header);
    }
  }
}


/*===========================================================================

FUNCTION  cmipcall2_sip_dtmf_string_check

DESCRIPTION
  Check whether the SIP DTMF digits are in valid range

RETURN VALUE
  CM_INBAND_CMD_ERR_NONE if no digit related errors,
  otherwise CM_INBAND_CMD_ERR_DIGIT_P is returned

===========================================================================*/
cm_inband_cmd_err_e_type  cmipcall2_sip_dtmf_string_check(
  const char *sip_dtmf_str
)
{
  cm_inband_cmd_err_e_type cmd_err = CM_INBAND_CMD_ERR_NOERR;
  uint8 i = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT (sip_dtmf_str != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for (i=0; i<MIN(strlen(sip_dtmf_str), CM_SIP_DTMF_MAX_LEN); i++)
  {
    if ((sip_dtmf_str+i) != NULL)
    {
      char c = *(sip_dtmf_str+i);

      if (!DECCHK(c) &&
          c != '*' &&
          c != '#'
#ifdef CM_GW_SUPPORTED
          && !(c >= 'A' && c <= 'Z')
          && !(c >= 'a' && c <= 'z')
#endif
         )
      {
        CM_MSG_HIGH_1("SIP_DTMF: CM_INBAND_CMD_ERR_DIGIT_P invalid char %c", c);
        cmd_err = CM_INBAND_CMD_ERR_DIGIT_P;
        break;
      }
    }
  }
  return cmd_err;
}

/*===========================================================================

FUNCTION cmipcall2_cmd_sip_dtmf_para_check

DESCRIPTION
  Check parameter errors for the SIP DTMF command

RETURN VALUE
  CM_INBAND_CMD_ERR_NONE if no parameter related errors,
  otherwise specific cm_inband_cmd_err_e_type.

===========================================================================*/
cm_inband_cmd_err_e_type  cmipcall2_cmd_sip_dtmf_para_check(
  cm_inband_cmd_s_type *inband_cmd_ptr
)
{
  cm_inband_cmd_info_s_type  *cmd_info_ptr =  NULL;
  cm_inband_cmd_err_e_type    cmd_err =  CM_INBAND_CMD_ERR_NOERR;
  cm_call_mode_info_e_type    call_mode;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(inband_cmd_ptr != NULL);

  if (inband_cmd_ptr->info.sip_dtmf_info == NULL ||
      inband_cmd_ptr->info.sip_dtmf_info->dtmf_string == NULL)
  {
    CM_MSG_HIGH_0("SIP_DTMF: CM_INBAND_CMD_ERR_NULL_BUF_P");
    return CM_INBAND_CMD_ERR_NULL_BUF_P;
  }

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_info_ptr =  CMD_INFO_PTR( inband_cmd_ptr );
  call_mode = cmcall_call_id_mode(inband_cmd_ptr->info.call_id );

  /* Check if the digits in the incoming DTMF string are valid 
  */
  cmd_err = cmipcall2_sip_dtmf_string_check(
              inband_cmd_ptr->info.sip_dtmf_info->dtmf_string);

  if (cmd_err != CM_INBAND_CMD_ERR_NOERR )
  {
    return cmd_err;
  }

  /* Accept SIP DTMF for IP call only 
  */
  if (call_mode != CM_CALL_MODE_INFO_IP)
  {
    cmd_err = CM_INBAND_CMD_ERR_OTHER;
  }

  return cmd_err;
}

/*===========================================================================

FUNCTION cmipcall2_mo_sip_dtmf_proc

DESCRIPTION
  Process SIP DTMF received from user

RETURN VALUE
  None

===========================================================================*/
void cmipcall2_mo_sip_dtmf_proc
(
  const cmcall_s_type           *call_ptr,
  cm_ip_sip_dtmf_s_type   *sip_dtmf_info
)
{
  CM_ASSERT (call_ptr != NULL);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Request to send SIP DTMF
  */
  if(!cmipapp_send_sip_dtmf_cmd(call_ptr->ipapp_id,
                                call_ptr->call_id,
                                sip_dtmf_info,
                                call_ptr->mm_id.asubs_id))
  {
    CM_ERR_1("SIP_DTMF: request failed, call_id %d",
             call_ptr->call_id );
  }
}

#else /* FEATURE_IP_CALL */

/*===========================================================================
FUNCTION cmipcall_dummy

DESCRIPTION

  Dummy function, defined to get rid of
  'Warning: C3040E: no external declaration in translation unit'
  ADS 1.2 compiler warning

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmipcall_dummy( void )
/*lint -esym(714,cmipcall_dummy) */
/*lint -esym(765,cmipcall_dummy)
** Can't be static as it would result in compiler warnings
*/
{
}

#endif /* FEATURE_IP_CALL */
