/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            CM Logging Facilities

GENERAL DESCRIPTION
   This module contains functions for logging CM packets.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002 - 2015 by Qualcomm Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmlog.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/22/13   jvo     cmlog_get_call_state() returns none when called by external
                   API before cm_init()
10/21/13   jvo     Modify cmlog_ssac_timer_event_handler to use unaligned
                   timer value in event payload
08/06/13   xs      Add function cmlog_csfb_svlte_switch() to log switch between
                   SVLTE and CSFB
04/12/12   fj      Separate call_orig and call_connect log structures for DSDS
                   and non-DSDS.
01/06/12   gm      Data synchronization changes for freefloating platform
12/13/11   sg      Updated LOG_CM_PH_EVENT for TDS
11/28/11   jh      Print out gw stack log for NIKEL along with DUAL SIM
07/24/11   aj      SC SVLTE 1.0 merge to mainline
05/19/11   rm      Adding req/resp packet for CDMA info
04/21/11   sv      Correct Lint Errors
04/11/11   am      Cleaning stubs and runnning qtf SUTE
02/17/11   cl      Add support FEATURE_MMODE_REMOVE_1X for FUSION-MDM
12/23/10   sv      Integrate DSDS feature
11/02/10   am      Replacing FEATURE_MOB_MMODE_CUTE
07/14/10   mh      Fixed compile warnings
07/12/10   mj      LTE log packet updates
07/09/10   nm      Corrected event reports for OPERATIONAL_MODE and SYSTEM_MODE
06/10/10   ak      Mainlining memory optimization changes.
06/08/10   gm      Klocwork/Lint error fixes
05/06/10   rm      Make srv_status sent to Diag consistent with what is sent
                   in ss event
11/16/09   ak      Rectified compilation error.
11/04/09   ak      Added support for Ecall commands.
10/14/09   sg      ZI memory reduction. Mmode should call new Heap Manager API to
                   allocate memory dynamically.
10/05/09   fj      In cm_ph_info_s_type, change is_gwl_subscription_available
                   back to is_gw_subscription_available.
07/01/09   fj      Changes due to LTE merge.
08/07/09   rm      CMI phase-2: Removing featurisation from /api files
05/27/09   aj      Adding support for updating clients with cdma local time
05/27/09   aj      Adding support for passing otasp status to clients
05/15/09   sg      Update SS EVENT log packet with new SS events added in CM
04/24/09   rm      Including cmlog_v.h
02/23/09   rm      Removing cm_log_v.h
02/11/09   sv      Updated cmlog_get_max to cmlog_get_max_buff_size
01/29/09   fj      Remove FEATURE_HDR_HYBRID for call events
11/26/08   ks      Reuse static phone info to avoid overflow of CM stack
11/24/08   cl      Limit maximum payload of the event to 32 bytes.
09/26/08   aj      Fix compiler warning
09/18/08   aj      support for UMA GAN
08/20/08   st      Resolve Klockwork defects
06/12/08   sv      Resolve Klocwork defects
03/17/08   am      Added two diag commands to support
                   commands for CM_CALL_CMD_MODIFY_REQ / RSP
01/30/08   vs      Removing unnecessary code that was added under
                   FEATURE_HDR_HYBRID_DISABLE
01/03/08   ka      Adding handset based plus code dialing feature.
01/02/08   jqi/vs  Added support to temporary change the system preference
                   when FEATURE_HDR_HYBRID_DISABLE is enabled.
08/07/07   ic      Lint clean up
06/05/07   ic/cl   Change MSG_HIGH() to CM_MSG_HIGH() in the effort of merging
                   CL# 436262. This CL however has been previously merged.
05/02/07   rp      Adding FEATURE_CM_LCS
04/15/07   rp      Removing FEATURE_MMGPS
02/23/07   ic      In cmlog_wcdma_call_origination() do not send as many
                   origination commands as there are digits in called number
                   (fixed bug introduced on CM mainline in CL 271665 )
11/17/06   jqi     Added support for HDR active protocol revision information.
11/09/06   rp      Added support for EVENT_CM_CELL_SRV_IND and
                   EVENT_CM_CELL_SRV_IND
07/10/06   ic      Fixed Klockworks errors (added missing assertions)
07/03/06   ic      Cleaned up leftovers from mainlining FEATURE_CM
                   Cleaned up leftovers from mainlining FEATURE_SUPS
04/20/06   jqi     Lint fixes.
04/19/06   jqi     Added diag subsystem command(select system preference
                   command2) to process 64-bit band preference.
04/10/06   pk      Lint fixes
03/05/06   jqi     Merged the WCDMA BC3,4,8,9 support.
01/12/06   ic      For specific calls / walks Lint warnings, replaced global
                   -fsp comment with -e{error#} comments where needed
01/09/06   ic      Adjusted for cm_num_init() / cm_num_copy() changes in cm.h
01/06/06   ic      Lint cleanup
12/13/05   ic      Fixed compiler errors "..\..\services\cm\cmlog.c",
                   Error: C3028E: '=': implicit cast of pointer to non-equal
                   pointer
12/10/05   ic      Lint cleanup
11/21/05   ic      Send EVENT_CM_OPERATIONAL_MODE on transition from
                   SYS_OPRT_MODE_OFFLINE_CDMA to SYS_OPRT_MODE_RESET.
                   Fix the functionality that postpones sending of
                   EVENT_CM_OPERATIONAL_MODE for SYS_OPRT_MODE_OFFLINE_CDMA
                   until after TASK_OFFLINE_SIG from TMC comes (CR73050).
11/03/05   ic      Added CM_ASSERT() for ptr function parameters and lint
                   comments for 613 Lint error
10/28/05   pk      Modified cm_mm_client_ss_reg to register for all SS events
10/27/05   pk      Added support for SS log packet and
                   EVENT_CM_CALL_EVENT_ORIG_THR
08/15/05   pk      Added support for priority Queues for origination
                   parameters
08/02/05   ic      Print value that caused error in default switch case
07/27/05   ic      Log call events sent via cmxcall_event_generic()
07/25/05   sk      Replaced (for fixing CR 67893) the following:
                   - cm_util_def_alphabet_to_ascii()
                   - cm_util_ascii_to_def_alphabet()
                   with:
                   - cm_util_gsm_alphabet_to_ascii()
                   - cm_util_ascii_to_gsm_alphabet()
07/11/05   pk      Modified cm_ss_cmd_orig_thr_update_tbl() added packet_zone
                   as a additional parameter
06/21/05   ic      Renamed DIAG_MAX_GSM_DIGITS to CM_DIAG_MAX_GSM_DIGITS and
06/08/05   sk      Rearranged the featurization for
                   cmlog_cm_get_call_information_list() to be minimal.
                   Added cmlog_cm_send_call_information_log().
                   Moved user defined data types from here to cmlog.h.
06/06/05   pk      Added diag command to reset the call throttle table
                   CM_ORIG_THR_TBL_RESET -- SUBCOMMAND 15
05/25/05   ic      Mainlined FEATURE_CM
05/05/05   pk      Corrected copying of ddtm_srv_opt_list in the log packet
02/09/05   ic      Lint cleanup
02/01/05   dk      Added WLAN Support.
01/24/05   ic      Fixed copying of arrays for call/phone event logging
01/06/05   ic      Mainlined FEATURE_CM_CALL_EVENT_LOGGING and
                   FEATURE_CM_PH_EVENT_LOGGING
01/05/05   ic      Log packet version for call and phone event packets
12/20/04   ic      Fixed ADS 1.2 compiler errors
12/19/04   ic      Added support for sending LOG_CM_CALL_EVENT_C under
                   FEATURE_CM_CALL_EVENT_LOGGING
12/10/04   ic      Refinments for CR: 48569 - HDR-KDDI: Request for new EVENTs
                   for DDTM mode (LOG_CM_PH_EVENT_C)
12/05/04   ic      Added support for sending LOG_CM_PH_EVENT_C under
                   FEATURE_CM_PH_EVENT_LOGGING
11/15/04   ka      Fixed copying calling number from diag request in
                   cmlog_cm_call_orig().
11/11/04   ic      Lint cleanup
10/28/04   ka      get_call_information_list modified to return call info
                   list syncronously, added ps call dormant status to list of
                   call details.
10/01/04   ic      Removed unnecessary lint -e787 comment (switch statement
                   now has default case and -e788 is globally suppressed)
09/30/04   ka      Changed CM_ERR to MSG_LOW
09/24/04   ka      Adding support for Location based service request, service
                   complete response and event for conf from lower layers.
09/22/04   ka      Fixed a issue where allocated log buffer size was being
                   over run.
08/30/04   ka      Added support for srv_status in CM_STATE_INFO response
08/27/04   jqi     Added ps dormant optimization.
08/20/04   sj      Added support for PRL Preference in sys_sel_pref().
08/18/04   ic      Fixed RVCT 2.1 compiler warning thrown on Saber build
                   (M6250CTMQG4106):
                   Warning:  #144-D: a value of type "__packed void *" cannot
                   be used to initialize an entity of type
                   "DIAG_SUBSYS_CM_CM_WCDMA_CALL_ORIGINATION_req_type *"
                   DIAG_SUBSYS_CM_CM_WCDMA_CALL_ORIGINATION_req_type *req_ptr =
                   req_pkt;
                   Warning:  #144-D: a value of type "__packed void *" cannot
                   be used to initialize an entity of type
                   "DIAG_SUBSYS_CM_CM_GSM_CALL_ORIGINATION_req_type *"
                   DIAG_SUBSYS_CM_CM_GSM_CALL_ORIGINATION_req_type *req_ptr =
                   req_pkt;
08/12/04   ws      Remove cm20 support.
08/10/04   ic      Removed cmlog_get_operating_mode() and cmlog_system_mode()
                   as they became obsolete.
                   In cmlog_ph_event_handler() and cmlog_ss_event_handler()
                   call event_report_payload() with sizeof(value) instead of
                   sizeof(int)
08/03/04   ic      CM Lint clean up - cmlog.c with Lint supression verified
                   and free of Lint issues in all CUTE configurations.
07/30/04   ws      Add redirecting num call event as a valid event with
                   unassigned call id.
07/26/04   ic      Changed include file order to customer.h, then cm.h then
                   cmi.h
07/07/04   ka      Fixed Lint errors
07/01/04   ka      Increased array length for call_state_list, corrected length
                   checking for CM_SEND_USSD_REPLY. log_ptr allocation failure
                   no more stops USSD events from being reported.
06/30/04   ic      Fixed obvious RVCT 2.1 compiler warnings (Jaguar build)
06/30/04   dk      Added GET_CCM_INFO command for CR 44830.
06/23/04   ic      Fixed RVCT 2.1 compiler error #158: (expression must be an
                   lvalue or a function designator) / Lint error #50:
                   Attempted to take the address of a non-lvalue
06/22/04   ic      Fixed obvious RVCT 2.1 compiler warnings
06/14/04   ws      Fixed featurization problem with CM_CALL_END.
05/14/04   ka      Fixed Lint errors.
05/14/04   ka      Changing system selection preference to accept 0xFFFFFFFF for
                   no change
04/05/04   ka      Changing system selection preference to be written with CM_PREF_TERM_PERMANENT.
03/23/04   ws      Merged code from 6300.
03/19/04   ka      Added Fix to change phone to OFFLINE_CDMA only after
                   cmlog_cm_offline_signal_recvd is set to true.
03/18/04   sj      Fixed featurization problems on CDMA only target.
03/16/04   ka      Merged code from 6250.
                   Added new events for PS Data continuity.
                   Fixed Lint errors.
03/02/04   ic      Fixed featurization compile errors
02/25/04   ic      Changes to make the code pass unit test cases and at the
                   same time make calls in Saber build
02/06/04   ws      Initial jaguar/mainline merge.
01/26/04   ka      Added routine cmlog_check_request_length () to check request length.
                   Swapped Id's for get and set closed user group preference.
01/21/04   ka      Corrected CM_CALL_ID_MAX to CM_DIAG_CALL_ID_MAX
01/13/04   ka      Removed include file uissups.h. Defined constants locally.
12/15/03   ka      Added function for getting call info list, entering password and
                   sending reply to USSD events. Added events for password prompting,
                   USSD request and notify indication
10/17/03   ka      Diag commands for getting/setting closed user group
                   preferences. CUG parameters used for call origination.
10/13/03   ka      Changed wcdma, gsm call origination function names.
09/29/03   vt      Added orig & end commands.
07/10/03   prk     Modified cmlog_cm_system_selection to reverse the byte
                   order of the PLMN id.  Previously, the user was being
                   forced to enter the PLMN id in little endian notation.
05/21/03   vt      Added new call events.
04/28/03   AT      Lint cleanup.
03/21/03   ATM     Added event logging for svc_cnf and nw_list_cnf
                   And commands for initiating service/nw_list
                   And mainlined DEBUG_CM_EVENT_REPORTING
02/26/03   prk     Added include of diagcmd.h.
02/12/03   atm     Added pref logging and moved subsys command from UMTS to
                   CM.
01/21/03   ws      Updated copyright information for 2003
09/03/02   atm     Added mapping from CM enum to logging enum to sync up to
                   existing event database.
05/06/02   PRK     Renamed DEBUG_CM_NO_EVENT_REPORTING to
                   DEBUG_CM_EVENT_REPORTING.  The flag needs to be defined
                   to perform event reporting.
04/30/02   atm     Added flag to turn off functionality:
                    DEBUG_CM_NO_EVENT_REPORTING (by default, code is on)
04/30/02   atm     Event codes now defined in event_defs.h, -r1.14.
04/26/02   atm     Initial Revision.

===========================================================================*/

#include "mmcp_variation.h"
#if defined __cplusplus
extern "C" {
#endif

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "customer.h"
#include "cmlog.h"          /* This module's header file */
#include "cmlog_v.h"
#include "event.h"          /* For logging STATE events and event codes */
#include "cmcall.h"         /* For cmcall_overall_call_state() */
#include "cmss.h"           /* For pointer to SS object */
#include "sys.h"            /* For SYSMODE and OPMODE enums */
#include "sys_v.h"
#include "cmclient.h"       /* for event notifications     */
#include "cmtaski.h"         /* Interface to CM task services */
#include "cmpmprx.h"
#ifdef FEATURE_MMODE_DUAL_SIM
#include "cm_dualsim.h"      /* For Subscription id and standy pref enums */
#endif
#include "cmdiag.h"         /* For diag related constants  */
#include "diag.h"

#include "cmdbg.h"
#include <string.h>
#include "msg.h"            /* Interface to F3 messages services */
#include "cmutil.h"         /* Declaring cm_util_ascii_to_def_alphabet () */
#include "event_defs.h"

#ifdef CM_GW_SUPPORTED
#include "log_codes_umts.h" /* Defines LOG_UMTS_RESPONSE_NOTIFY */
#endif
#include "cmtask.h"

#ifdef CM_DEBUG
#error code not present
#endif

#ifdef FEATURE_ECALL_APP
#include "ecall_diag.h"
#endif /* FEATURE_ECALL_APP */

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "mm.h"

static cmlog_client_s_type diag_client;

static cmlog_call_state_s_type call_state_list[CMCALL_MAX_CALL_OBJ];

#define EVENT_CM_RPT_EXT_VERSION DIAG_EXT_API_VERSION_1

static uint32 cmlog_sys_as_id_to_uint32(sys_modem_as_id_e_type asubs_id);

/*===========================================================================
                         FUNCTION DECLARATIONS
===========================================================================*/
/*===========================================================================
FUNCTION cmlog_call_orig_start

DESCRIPTION
   This function logs the call orig start event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_call_orig_start
(
  const cmcall_s_type  *call_ptr
  /*  pointer for the call object */
)
{
  uint8 payload[6];
  uint8 *payload_ptr;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr;
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr;
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_ptr;
  multimode_stack_e_type stack_id;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(call_ptr->mm_id.asubs_id);;
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(call_ptr != NULL);

  ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(call_ptr->mm_id.asubs_id);
  ss_info_sub_prop_ptr = cmss_get_info_sub_prop(call_ptr->mm_id.asubs_id);
  if(ss_misc_sub_ptr == NULL || ss_info_sub_prop_ptr == NULL ||
      !cmutil_is_mm_id_valid(call_ptr->mm_id))
  {
    return;
  }
  stack_id = call_ptr->mm_id.stk_id;
  if(stack_id == MM_STACK_ALL)
  {
    stack_id = MM_STACK_0;
  }
  ss_info_all_stack_ptr = cmss_get_info_stk_ptr(call_ptr->mm_id.asubs_id, stack_id);
  if(ss_info_all_stack_ptr == NULL)
  {
    return;
  }
  /* report EVENT_CM_CALL_ORIG_START_P1  */
  payload_ptr = payload;/* set payload pointer = &payload[0] */
  *payload_ptr = (uint8)call_ptr->call_type;
  payload_ptr += sizeof(uint8);
  *payload_ptr = (uint8)call_ptr->srv_type;
  payload_ptr += sizeof(uint8);
#if (defined (FEATURE_MMODE_CDMA_800) || defined (FEATURE_MMODE_CDMA_1900))
  *((uint16 *)payload_ptr)
    = (uint16)call_ptr->cmcall_mode_info.info.cdma_info.srv_opt;
  payload_ptr += sizeof(uint16);
  *payload_ptr = (uint8)call_ptr->cmcall_mode_info.info.cdma_info.drs_bit;
#endif
#ifdef FEATURE_HDR_HANDOFF
  payload_ptr += sizeof(uint8);
  *payload_ptr = (uint8)call_ptr->last_act_data_net;
#endif

#ifdef FEATURE_HDR_HANDOFF
  event_report_payload_ext(EVENT_CM_CALL_ORIG_START_P1,6,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);
#else
  event_report_payload_ext(EVENT_CM_CALL_ORIG_START_P1,5,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);
#endif

  /* report EVENT_CM_CALL_ORIG_START_P2 */
  payload_ptr = payload; /* set payload pointer = &payload[0] */
  *payload_ptr = (uint8)call_ptr->favored_mode_pref;
  payload_ptr += sizeof(uint8);
  *payload_ptr = (uint8)call_ptr->favored_hybr_pref;
  event_report_payload_ext(EVENT_CM_CALL_ORIG_START_P2,2,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);

  /* report EVENT_CM_CALL_ORIG_START_P3 */
  payload_ptr = payload; /* set payload pointer = &payload[0] */
  *payload_ptr = (uint8)call_ptr->mm_id.stk_id;
  payload_ptr += sizeof(uint8);
  *payload_ptr = (uint8)ss_info_all_stack_ptr->sys_mode;
  payload_ptr += sizeof(uint8);
  *payload_ptr = (uint8)ss_misc_sub_ptr->sub_info.gpp2_sub_info.is_colloc;
  event_report_payload_ext(EVENT_CM_CALL_ORIG_START_P3,3,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);

}


/*===========================================================================
FUNCTION  cmlog_call_orig_redial

DESCRIPTION
   This function logs the call orig redial event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_call_orig_redial(
  cm_call_end_e_type    end_status,
  /* the call end status */
  cmcall_act_e_type     redial_act,
  /* the redial action */
  dword                 sr_left,
  /* number of silent redial left */
  unsigned int          sr_max_redial,
  /* maximun number of silent redial attempts */
  sys_modem_as_id_e_type asubs_id
  /* Subscription ID */
)
{

  uint8 payload[5];
  uint8 *payload_ptr;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*
  ** 571: Suspicious cast
  ** To be investigated why we cast it as uint16 vs uint8
  */
  /*lint -save -e571 */

  /* Report EVENT_CM_CALL_ORIG_REDIAL
  */

  /* Set payload pointer = &payload[0]
  */
  payload_ptr = payload;
  *((uint16 *)payload_ptr) = (uint16)end_status;
  payload_ptr += sizeof(uint16);
  *payload_ptr = (uint8)redial_act;
  payload_ptr += sizeof(uint8);
  *payload_ptr = (uint8)sr_left;
  *payload_ptr = (uint8)(sr_max_redial - sr_left);
  event_report_payload_ext(EVENT_CM_CALL_ORIG_REDIAL,5, payload, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
  /*lint -restore */
}


/*===========================================================================
FUNCTION  cmlog_call_orig_send_hdr_orig

DESCRIPTION
   This function logs the orig sent to HDR event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_call_orig_send_hdr_orig(
  sys_modem_as_id_e_type asubs_id
    /* Subscription ID */
)
{
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  event_report_payload_ext(EVENT_CM_CALL_ORIG_SEND_HDR_ORIG,0,NULL,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);
}


/*===========================================================================
FUNCTION  cmlog_call_orig_send_mc_orig

DESCRIPTION
   This function logs the orig sent to MC event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_call_orig_send_mc_orig(
  sys_modem_as_id_e_type     asubs_id
    /* Subscription ID */
)
{
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  event_report_payload_ext(EVENT_CM_CALL_ORIG_SEND_MC_ORIG,0,NULL,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);
}


/*===========================================================================
FUNCTION  cmlog_call_orig_end

DESCRIPTION
   This function logs the call orig end event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_call_orig_end(
  cm_call_end_e_type         end_status,
  /* call end status */
  sys_modem_as_id_e_type     asubs_id
  /*Subscription ID*/
)
{
  uint8 payload[2];
  uint8 *payload_ptr;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*
  ** 571: Suspicious cast
  ** It's OK here, want to supress it
  */
  /*lint -save -e571 */
  /* report EVENT_CM_CALL_ORIG_END */
  payload_ptr = payload;/* set payload pointer = &payload[0] */
  *(uint16 *)payload_ptr = (uint16)end_status;
  event_report_payload_ext(EVENT_CM_CALL_ORIG_END,2,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);
  /*lint -restore */
}


/*===========================================================================
FUNCTION  cmlog_call_orig_connected

DESCRIPTION
   This function logs the call orig connected event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_call_orig_connected(
  cm_call_type_e_type         call_type,
  /* origination call type */
  cm_srv_type_e_type          srv_type,
  /* origination srv type */
  word                        srv_opt,
  /* origination srv opt */
  sys_modem_as_id_e_type      asubs_id
  /* Subscription ID */
)
{
  uint8 payload[4];
  uint8 *payload_ptr;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* report EVENT_CM_CALL_ORIG_CONNECTED */
  payload_ptr = payload;/* set payload pointer = &payload[0] */
  *payload_ptr = (uint8)call_type;
  payload_ptr += sizeof(uint8);
  *payload_ptr = (uint8)srv_type;
  payload_ptr += sizeof(uint8);
  *(uint16 *)payload_ptr =(uint16)srv_opt;
  event_report_payload_ext(EVENT_CM_CALL_ORIG_CONNECTED,4,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);
}

#if (defined(FEATURE_LTE_TO_1X) && defined(FEATURE_MMODE_SC_SVLTE)) || defined(FEATURE_MMODE_DYNAMIC_SV_OPERATION_SWITCH)
/*===========================================================================
FUNCTION  cmlog_csfb_svlte_switch

DESCRIPTION
   This function logs the dynamic switch event between CSFB and SVLTE.
   Event name is EVENT_CM_PH_DYN_SWITCH defined in event_defs.h

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void cmlog_csfb_svlte_switch(
  cmsoa_sv_switch_action_e_type dyn_sw,
  /*dynamic switch action type*/
  sys_modem_as_id_e_type        asubs_id
  /* Subscription ID */
)
{
  uint8 payload;
  uint8 *payload_ptr;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  payload_ptr = &payload;
  *payload_ptr = (uint8)dyn_sw;

  /*event ID: 0x9D3, defined in event_defs.h*/
  event_report_payload_ext( EVENT_CM_PH_DYN_SWITCH, 1, payload_ptr, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);

}
#endif

/*===========================================================================
FUNCTION  cmlog_enter_emergency_cb
===========================================================================*/
void cmlog_enter_emergency_cb (
  sys_modem_as_id_e_type        asubs_id
    /* Subscription ID */
)
{
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  event_report_payload_ext( EVENT_CM_ENTER_EMERGENCY_CB, 0, NULL, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
}

/*===========================================================================
FUNCTION  cmlog_enter_emergency_cb
===========================================================================*/
void cmlog_exit_emergency_cb (
  sys_modem_as_id_e_type        asubs_id
    /* Subscription ID */
)
{
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  event_report_payload_ext( EVENT_CM_EXIT_EMERGENCY_CB, 0, NULL, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
}

/*===========================================================================
FUNCTION  cmlog_call_state
===========================================================================*/
void cmlog_call_state (
  sys_modem_as_id_e_type  asubs_id
)
{
  uint8         state[2];
  uint8         *state_ptr;
  uint32        evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
#ifdef FEATURE_MMODE_DUAL_SIM
  cmph_s_type   *ph_ptr = cmph_ptr();
#endif

  state_ptr = state;

#ifdef FEATURE_MMODE_DUAL_SIM
  if( ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#if defined(FEATURE_MMODE_TRIPLE_SIM)
      || ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY
#endif
    )
  {
    /* Set the subscription-id
    */
    *state_ptr = (uint8) asubs_id;
    state_ptr++;
  }
#endif

  /* Set the call state
  */
  *state_ptr = (byte)cmcall_overall_call_state_per_subs(asubs_id);

#ifdef FEATURE_MMODE_DUAL_SIM
  if( ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#if defined(FEATURE_MMODE_TRIPLE_SIM)
      ||   (ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY)
#endif
    )
  {
    event_report_payload_ext( EVENT_CM_DS_CALL_STATE, 2, state, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
  }
  else
#endif
  {
    event_report_payload_ext( EVENT_CM_CALL_STATE, 1, state, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
  }
}

/*===========================================================================
FUNCTION  cmlog_get_call_state

DESCRIPTION
  This function should be called to retrieve the call state.
  It can be used by DIAG, for example, to fill status packet responses.

RETURN VALUE
  byte -  The call state

SIDE EFFECTS
  None

===========================================================================*/
byte cmlog_get_call_state ()
{
  if (cmtask_is_init_complete())
  {
    return (byte)cmcall_overall_call_state();
  }
  else
  {
    CM_MSG_HIGH_0("cmtask init not complete, return NONE");
    return CM_OVERALL_CALL_STATE_NONE;
  }
}

/*===========================================================================
FUNCTION  cmlog_get_call_state_list

DESCRIPTION
  Returns pointer to call_state_list static data. This data needs to be used
  in cmdiag module, so the global datum is converted to static and provided
  is through an API.

RETURN VALUE
  Pointer to type cmlog_call_state_s_type

SIDE EFFECTS
  None

===========================================================================*/
cmlog_call_state_s_type *cmlog_get_call_state_list( void )
{
  return call_state_list;
}

/*===========================================================================
FUNCTION  cmlog_get_diag_client

DESCRIPTION
  Returns pointer to diag client global static data defined in cmlog.c

RETURN VALUE
  Pointer to type cmlog_client_s_type

SIDE EFFECTS
  None

===========================================================================*/
cmlog_client_s_type *cmlog_get_diag_client( void )
{
  return &diag_client;
}

/*===========================================================================
FUNCTION  cmlog_get_operating_mode
===========================================================================*/
byte cmlog_get_operating_mode ( )
{
  return (byte)diag_client.oprt_mode;
}

/*===========================================================================
FUNCTION  cmlog_get_system_mode
===========================================================================*/
byte cmlog_get_system_mode()
{
  return (byte)diag_client.sub_info[SYS_MODEM_AS_ID_1].sys_mode;
}

#ifdef FEATURE_MMODE_DUAL_SIM
/*===========================================================================
FUNCTION  cmlog_get_system_mode_per_subs
===========================================================================*/
byte cmlog_get_system_mode_per_subs(

  sys_modem_as_id_e_type      asubs_id

)
{
  if(!cmutil_is_as_id_valid(asubs_id))
  {
    asubs_id = SYS_MODEM_AS_ID_1;
  }

  return (byte)diag_client.sub_info[asubs_id].sys_mode;
}

/*===========================================================================
FUNCTION  cmlog_get_call_state_per_subs
===========================================================================*/
byte cmlog_get_call_state_per_subs (

  sys_modem_as_id_e_type   asubs_id

)
{
  return (byte)cmcall_overall_call_state_per_subs(asubs_id);
}
#endif /* FEATURE_MMODE_DUAL_SIM */

/*===========================================================================
FUNCTION  cmlog_get_srv_status
===========================================================================*/
byte cmlog_get_srv_status ( )
{
  return (byte)diag_client.sub_info[SYS_MODEM_AS_ID_1].srv_status;
}

/*===========================================================================
FUNCTION  cmlog_get_call_state_list_index

Description:

  Loops through call_state_list to return index of the array entry
  holding requested call_id
===========================================================================*/
static int cmlog_get_call_state_list_index (cm_call_id_type call_id)
{
  int loop = 0;

  for (loop = 0; loop < CMCALL_MAX_CALL_OBJ; ++loop)
  {
    if (call_state_list[loop].call_id == call_id)
      return loop;
  }

  return (CMCALL_MAX_CALL_OBJ);
}


/*===========================================================================
FUNCTION  cmlog_set_call_state_list_entry

Description:

       Loops through call_state_list to find an entry for storing call_id
       and returns the index
===========================================================================*/
static int cmlog_set_call_state_list_entry (cm_call_id_type call_id)
{
  int loop = 0;

  for (loop = 0; loop < CMCALL_MAX_CALL_OBJ; ++loop)
  {
    if (call_state_list[loop].call_id == CM_CALL_ID_INVALID)
    {
      call_state_list[loop].call_id = call_id;
      return loop;
    }
  }

  return (CMCALL_MAX_CALL_OBJ);
}

/*===========================================================================
FUNCTION  cmph_log_handle_oprt_mode_event

Description:

  This function handles CM_PH_EVENT_OPRT_MODE
  We want to send DIAG EVENT_CM_OPERATIONAL_MODE only after TASK_OFFLINE_SIG
  was sent by TMC and received by CM

===========================================================================*/
static void cmph_log_handle_oprt_mode_event(

  cm_ph_event_e_type         ph_event,
  /* phone event to log */

  const cm_ph_info_s_type   *ph_info_ptr
  /* phone info structure with event info */
)
/*lint -esym(715, ph_event) */
{
  boolean send_oprt_mode_event;
  uint8     value;
  uint32    evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(ph_info_ptr->asubs_id);

  if (diag_client.oprt_mode != ph_info_ptr->oprt_mode)
  {
    /* Most of the time we'll send the DIAG event
    */
    send_oprt_mode_event = TRUE;

    /* Field diag_client.offline_signal_recvd records TASK_OFFLINE_SIG
    ** sent by TMC to CM.
    ** This signal is the closest indicator about all the tasks in the system
    ** going offline.
    ** Send EVENT_CM_OPERATIONAL_MODE for SYS_OPRT_MODE_OFFLINE_CDMA
    ** only after CM got the signal from TMC
    */
    if(ph_info_ptr->oprt_mode == SYS_OPRT_MODE_OFFLINE_CDMA)
    {
      if(diag_client.offline_signal_recvd == TRUE)
      {
        /* Reset the flag, send the event
        */
        diag_client.offline_signal_recvd = FALSE;
      }
      else
      {
        /* Defer sending the event until after TASK_OFFLINE_SIG comes
        ** It will be sent from cmlog_set_cm_offline_signal_recvd()
        */
        send_oprt_mode_event = FALSE;
        CM_MSG_HIGH_0("No TASK_OFFLINE_SIG, defer EVENT_CM_OPERATIONAL_MODE DIAG event");
      }
    }

    if(send_oprt_mode_event)
    {
      value = (uint8)(diag_client.oprt_mode = ph_info_ptr->oprt_mode);
      event_report_payload_ext( EVENT_CM_OPERATIONAL_MODE, sizeof(value), &value, 
                                EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
    }
  }
}
/*lint +esym(715, ph_event) */

/*===========================================================================
FUNCTION  cmlog_ph_event_handler

Description:

       This function checks for changes to preferences and logs
       them accordingly.

===========================================================================*/
/*lint -esym(765, cmlog_ph_event_handler)
** Can't be static, registered to handle events for DIAG client
*/
void cmlog_ph_event_handler(

  cm_ph_event_e_type         ph_event,
  /* phone event to log */

  const cm_ph_info_s_type   *ph_info_ptr
  /* phone info structure with event info */
)
{

  int                     value;

  unsigned int            i;
  /* Loop counter
  */

  unsigned int            log_pkt_len = 0;
  /* Length of packet to be logged.
  */

  LOG_CM_PH_EVENT_C_type  *log_ptr     = NULL;
  /* Log pointer to the log record type.
  */
  uint8                   pref[5];
  uint8                   *pref_ptr = pref;
  cmph_s_type             *ph_ptr = cmph_ptr();
  sys_modem_as_id_e_type  asubs_id = SYS_MODEM_AS_ID_1;
  uint32                  evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(ph_info_ptr->asubs_id);
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(!BETWEEN(CM_PH_EVENT_NONE, ph_event, CM_PH_EVENT_MAX));
  CM_ASSERT(ph_info_ptr != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch ( ph_event )
  {
    case CM_PH_EVENT_INFO_AVAIL:
    /* Phone information is now available. Automatically generated
    ** after a client does activation */

    case CM_PH_EVENT_OPRT_MODE:
    {
      /* Operating mode was changed */
      sys_oprt_mode_e_type prev_diag_oprt_mode = diag_client.oprt_mode;

      cmph_log_handle_oprt_mode_event(ph_event, ph_info_ptr);

      /* In case oprt mode changes to ONLINE, update DIAG client as well with
      ** latest values (simialr to sys_sel_pref), else break
      */
      if(!(prev_diag_oprt_mode != SYS_OPRT_MODE_ONLINE &&
           ph_info_ptr->oprt_mode == SYS_OPRT_MODE_ONLINE ))
      {
        break;
      }

    }
    case CM_PH_EVENT_SYS_SEL_PREF:
    case CM_PH_EVENT_NAM_SEL:
      /* NAM dependent variables changed, such as
      ** the mode preference and the system preference
      ** in accordance with the new NAM selection.
      */
      *pref_ptr = (uint8)ph_info_ptr->asubs_id;
      pref_ptr++;
      if(cmutil_is_as_id_valid(ph_info_ptr->asubs_id))
      {
        asubs_id = ph_info_ptr->asubs_id;
      }
      if ( diag_client.sub_info[asubs_id].mode_pref != ph_info_ptr->mode_pref )
      {
        value = (int)(diag_client.sub_info[asubs_id].mode_pref = ph_info_ptr->mode_pref);
        memscpy( &pref[1], ((ARR_SIZE(pref) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_MODE_PREF, sizeof(pref), pref, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      if ( diag_client.sub_info[asubs_id].acq_order_pref != ph_info_ptr->acq_order_pref )
      {
        value = (int)(diag_client.sub_info[asubs_id].acq_order_pref = ph_info_ptr->acq_order_pref);
        memscpy( &pref[1], ((ARR_SIZE(pref) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_GW_ACQ_ORDER_PREF, sizeof(pref), pref, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      if ( diag_client.sub_info[asubs_id].srv_domain_pref != ph_info_ptr->srv_domain_pref )
      {
        value = (int)(diag_client.sub_info[asubs_id].srv_domain_pref = ph_info_ptr->srv_domain_pref);
        memscpy( &pref[1], ((ARR_SIZE(pref) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_SRV_DOMAIN_PREF, sizeof(pref), pref, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      if ( diag_client.sub_info[asubs_id].band_pref != ph_info_ptr->band_pref )
      {
        value = (int)(diag_client.sub_info[asubs_id].band_pref = ph_info_ptr->band_pref);
        memscpy( &pref[1], ((ARR_SIZE(pref) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_BAND_PREF, sizeof(pref), pref, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      if ( diag_client.sub_info[asubs_id].roam_pref != ph_info_ptr->roam_pref )
      {
        value = (int)(diag_client.sub_info[asubs_id].roam_pref = ph_info_ptr->roam_pref);
        memscpy( &pref[1], ((ARR_SIZE(pref) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_ROAM_PREF, sizeof(pref), pref, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      if ( diag_client.sub_info[asubs_id].hybr_pref != ph_info_ptr->hybr_pref )
      {
        value = (int)(diag_client.sub_info[asubs_id].hybr_pref = ph_info_ptr->hybr_pref);
        memscpy( &pref[1], ((ARR_SIZE(pref) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_HYBRID_PREF, sizeof(pref), pref, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
       }

      if ( diag_client.sub_info[asubs_id].network_sel_mode_pref != ph_info_ptr->network_sel_mode_pref )
      {
        value = (int)(diag_client.sub_info[asubs_id].network_sel_mode_pref
                      = ph_info_ptr->network_sel_mode_pref);
        memscpy( &pref[1], ((ARR_SIZE(pref) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_NETWORK_SEL_MODE_PREF, sizeof(pref), pref, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      /* System Selection pref was changed - SD20*/
      if ( diag_client.oprt_mode != ph_info_ptr->oprt_mode )
      {
        value = (int)(diag_client.oprt_mode = ph_info_ptr->oprt_mode);
        event_report_payload_ext( EVENT_CM_OPERATIONAL_MODE, sizeof(value), &value, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    default:
      break;
  } /* switch */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*
  ** The below are log packets for all PH events
  */

  /*
  ** Compute log packet length
  */
  log_pkt_len = sizeof(LOG_CM_PH_EVENT_C_type);

  /* Allocate a log packet.
  */
  log_ptr = (LOG_CM_PH_EVENT_C_type *) log_alloc_ex( LOG_CM_PH_EVENT_C,
            log_pkt_len );
  /*
  ** If allocation failed, just return.
  ** It means that either logging of LOG_CM_PH_EVENT_C is off
  ** or DIAG is truly out of memory (rare)
  */
  if ( log_ptr == NULL )
  {
    CM_MSG_LOW_0("Log packet buffer allocation failed!");
    return;
  }

  /*lint -save -e641 Converting enum to int */

  /*
  ** Write log packet version
  ** This is always set to CM_LOG_PH_EVENT_LOG_PACKET_VERSION
  */
  log_ptr->version               = CM_LOG_PH_EVENT_LOG_PACKET_VERSION;

  /* Copy field by field from ph_info_ptr
  */
  log_ptr->ph_event              = ph_event;
  log_ptr->is_in_use             = ph_info_ptr->is_in_use;
  log_ptr->oprt_mode             = ph_info_ptr->oprt_mode;
  log_ptr->test_control_type     = ph_info_ptr->test_control_type;
  log_ptr->cdma_lock_mode        = ph_info_ptr->cdma_lock_mode;
  log_ptr->answer_voice          = ph_info_ptr->answer_voice;
  log_ptr->mode_pref             = ph_info_ptr->mode_pref;
  log_ptr->pref_term             = ph_info_ptr->pref_term;
  log_ptr->acq_order_pref        = ph_info_ptr->acq_order_pref;
  log_ptr->srv_domain_pref       = ph_info_ptr->srv_domain_pref;
  log_ptr->band_pref             = ph_info_ptr->band_pref; /*lint !e713 */
  log_ptr->lte_band_pref         = sys_map_lte_band_mask_to_band_mask(ph_info_ptr->lte_band_pref); /*lint !e713 */
#if(LTE_BAND_NUM == 256)
  log_ptr->lte_band_pref_65_128  = ph_info_ptr->lte_band_pref.bits_65_128; /*lint !e713 */
  log_ptr->lte_band_pref_129_192 = ph_info_ptr->lte_band_pref.bits_129_192; /*lint !e713 */
  log_ptr->lte_band_pref_193_256 = ph_info_ptr->lte_band_pref.bits_193_256; /*lint !e713 */
#else
  log_ptr->lte_band_pref_65_128  = 0; /*lint !e713 */
  log_ptr->lte_band_pref_129_192 = 0; /*lint !e713 */
  log_ptr->lte_band_pref_193_256 = 0; /*lint !e713 */
#endif
  log_ptr->tds_band_pref         = ph_info_ptr->tds_band_pref; /*lint !e713 */
  log_ptr->roam_pref             = ph_info_ptr->roam_pref;
  log_ptr->nam_sel               = ph_info_ptr->nam_sel;
  log_ptr->curr_nam              = ph_info_ptr->curr_nam;
  log_ptr->packet_state          = ph_info_ptr->packet_state;
  log_ptr->hybr_pref             = ph_info_ptr->hybr_pref;
  log_ptr->network_sel_mode_pref = ph_info_ptr->network_sel_mode_pref;

  log_ptr->plmn.identity[0] = ph_info_ptr->plmn.identity[0];
  log_ptr->plmn.identity[1] = ph_info_ptr->plmn.identity[1];
  log_ptr->plmn.identity[2] = ph_info_ptr->plmn.identity[2];

  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->user_pref_networks.length = MIN(ph_info_ptr->user_pref_networks.length,
                                       ARR_SIZE(log_ptr->user_pref_networks.info));


  for(i = 0; i < log_ptr->user_pref_networks.length; i++)
  {
    log_ptr->user_pref_networks.info[i].plmn.identity[0] =
      ph_info_ptr->user_pref_networks.info[i].plmn.identity[0];
    log_ptr->user_pref_networks.info[i].plmn.identity[1] =
      ph_info_ptr->user_pref_networks.info[i].plmn.identity[1];
    log_ptr->user_pref_networks.info[i].plmn.identity[2] =
      ph_info_ptr->user_pref_networks.info[i].plmn.identity[2];

    log_ptr->user_pref_networks.info[i].access_tech =
      ph_info_ptr->user_pref_networks.info[i].access_tech;
  }

  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->available_networks.length = MIN(ph_info_ptr->available_networks.length,
                                       ARR_SIZE(log_ptr->available_networks.info));

  for(i = 0; i < log_ptr->available_networks.length; i++)
  {

    log_ptr->available_networks.info[i].plmn.identity[0] =
      ph_info_ptr->available_networks.info[i].plmn.identity[0];
    log_ptr->available_networks.info[i].plmn.identity[1] =
      ph_info_ptr->available_networks.info[i].plmn.identity[1];
    log_ptr->available_networks.info[i].plmn.identity[2] =
      ph_info_ptr->available_networks.info[i].plmn.identity[2];

    log_ptr->available_networks.info[i].rat =
      ph_info_ptr->available_networks.info[i].rat;

    log_ptr->available_networks.info[i].plmn_service_capability =
      ph_info_ptr->available_networks.info[i].plmn_service_capability;

    log_ptr->available_networks.info[i].plmn_forbidden =
      ph_info_ptr->available_networks.info[i].plmn_forbidden;

    log_ptr->available_networks.info[i].list_category =
      ph_info_ptr->available_networks.info[i].list_category;

    log_ptr->available_networks.info[i].signal_quality =
      ph_info_ptr->available_networks.info[i].signal_quality;

    log_ptr->available_networks.info[i].signal_strength =
      ph_info_ptr->available_networks.info[i].signal_strength;

  }

  log_ptr->is_cdma_subscription_available =
    ph_info_ptr->is_cdma_subscription_available;
  log_ptr->is_gwl_subscription_available   =
    ph_info_ptr->is_gw_subscription_available;
  log_ptr->rtre_config                    = ph_info_ptr->rtre_config;
  log_ptr->rtre_control                   = ph_info_ptr->rtre_control;
  log_ptr->orig_srv_domain_pref
    = cmtask_compute_srv_domain_pref(  cmph_map_sub_stk_to_id(SYS_MODEM_AS_ID_1, MM_STACK_0));
  log_ptr->ddtm_pref                      = ph_info_ptr->ddtm_pref;
  log_ptr->cur_ddtm_status                = ph_info_ptr->cur_ddtm_status;
  log_ptr->ddtm_act_mask                  = ph_info_ptr->ddtm_act_mask;
  log_ptr->orig_ddtm_act_mask             = ph_info_ptr->orig_ddtm_act_mask;


  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->ddtm_num_srv_opt               = MIN(ph_info_ptr->ddtm_num_srv_opt,
      ARR_SIZE(log_ptr->ddtm_srv_opt_list));
  for ( i = 0; i < log_ptr->ddtm_num_srv_opt; i++ )
  {
    log_ptr->ddtm_srv_opt_list[i] = ph_info_ptr->ddtm_srv_opt_list[i];
  }

  log_ptr->cphs_allowed         = ph_info_ptr->cphs_allowed;
  log_ptr->als_allowed          = ph_info_ptr->als_allowed;
  log_ptr->line                 = ph_info_ptr->line;
  log_ptr->line_switching       = ph_info_ptr->line_switching;

  log_ptr->requesting_client_id = ph_info_ptr->requesting_client_id;


#if (defined(FEATURE_UMTS_1X_HANDOVER_1XMSM) || \
      defined(FEATURE_UMTS_1X_HANDOVER_UMTSMSM))
#error code not present
#else

  /* Fill with N/A if those features are off
  */

  log_ptr->cdma_capability.type        = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  log_ptr->cdma_capability.payload_len = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;

  for ( i = 0; i < ARR_SIZE(log_ptr->cdma_capability.payload); i++ )
  {
    log_ptr->cdma_capability.payload[i] = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  }

  log_ptr->msm_id = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;

#endif

#ifdef FEATURE_MMODE_DUAL_SIM
  log_ptr->asubs_id                = (uint8)ph_info_ptr->asubs_id;
  log_ptr->standby_pref            = ph_info_ptr->standby_pref;
  log_ptr->active_subs             = (uint8)ph_info_ptr->active_subs;
  log_ptr->default_voice_subs      = (uint8)ph_info_ptr->default_voice_subs;
  log_ptr->default_data_subs       = (uint8)ph_info_ptr->default_data_subs;
  log_ptr->priority_subs           = (uint8)ph_info_ptr->priority_subs;
#else
  log_ptr->asubs_id                = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  log_ptr->is_hybr_gw_subscription_available
    = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
#if defined(FEATURE_MMODE_TRIPLE_SIM)
  log_ptr->is_hybr_gw3_subscription_available
    = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
#endif
  log_ptr->standby_pref            = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  log_ptr->active_subs             = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  log_ptr->default_voice_subs      = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  log_ptr->default_data_subs       = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  log_ptr->priority_subs           = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
#endif /* FEATURE_MMODE_DUAL_SIM */

  /* Submit the log to DIAG.
  */
#ifdef FEATURE_MMODE_DUAL_SIM
  log_util_log_packet_commit(log_ptr->asubs_id, (void *) log_ptr );
#else
  log_util_log_packet_commit(SYS_MODEM_AS_ID_1, (void *) log_ptr );
#endif

  /*lint -restore */

}

/*===========================================================================
FUNCTION  cmlog_set_cm_offline_signal_recvd

Description:

  CM calls this after receiving TASK_OFFLINE_SIG signal from TMC.
  This happens some time after cm_ph_cmd_oprt_mode() with
  SYS_OPRT_MODE_OFFLINE_CDMA had been processed by CM.
  Call on log object oprt mode event handler to force sending of DIAG
  EVENT_CM_OPERATIONAL_MODE event now that we got TASK_OFFLINE_SIG
  from TMC

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_set_cm_offline_signal_recvd(void)
{
  /* get a buffer for phone info */
  cm_ph_info_s_type *ph_info_ptr = cmph_info_alloc();;

  /* Set the flag to indicate that we got TASK_OFFLINE_SIG
  */
  diag_client.offline_signal_recvd = TRUE;

  /* Copy phone state information into phone info buffer.
  */
  (void)cmph_info_get( ph_info_ptr, SYS_MODEM_AS_ID_1, CM_PH_EVENT_OPRT_MODE);

  /* ... it'd better be SYS_OPRT_MODE_OFFLINE_CDMA
  */
  CM_ASSERT(ph_info_ptr->oprt_mode == SYS_OPRT_MODE_OFFLINE_CDMA);

  /* Call on log object oprt mode event handler to force sending of DIAG
  ** EVENT_CM_OPERATIONAL_MODE event now that we got TASK_OFFLINE_SIG
  ** from TMC
  */
  cmph_log_handle_oprt_mode_event(CM_PH_EVENT_OPRT_MODE,
                                  ph_info_ptr);

  /* Deallocate the memory after sending the ph event. */
  cm_mem_free(ph_info_ptr);

}

/*===========================================================================
FUNCTION  cmlog_ss_event_handler

Description:

       This function checks for changes to preferences and logs
       them accordingly.

===========================================================================*/
/*lint -esym(765, cmlog_ss_event_handler)
** Can't be static, registered to handle SS events for DIAG client
*/
void cmlog_ss_event_handler_msim(

  cm_ss_event_e_type           ss_event,

  const cm_mm_msim_ss_info_s_type  *ss_info_ptr

)
{
  int                     value,i;

  unsigned int            log_pkt_len = 0;
  /* Length of packet to be logged.
  */
  int index_val = 0 ;
  LOG_CM_SS_MSIM_EVENT_C_type  *log_ptr     =  NULL;
  /* Log pointer to the log record type.
  */

  uint8                   srv[5];
  uint8                   *srv_ptr    = srv;
  uint8                   cap_index=0;
  sys_modem_as_id_e_type  asubs_id = SYS_MODEM_AS_ID_1;
  uint32                  evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(ss_info_ptr->asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(cmutil_is_as_id_valid(ss_info_ptr->asubs_id))
  {
    asubs_id = ss_info_ptr->asubs_id;
  }

  switch ( ss_event )
  {
    case CM_SS_EVENT_SRV_CHANGED:
      /* Serving system information changed */

      *srv_ptr = (uint8)ss_info_ptr->asubs_id ;
      srv_ptr++;

      if ( diag_client.sub_info[asubs_id].sys_mode != ss_info_ptr->stack_info[0].sys_mode )
      {
        value = (int)(diag_client.sub_info[asubs_id].sys_mode = ss_info_ptr->stack_info[0].sys_mode);
        memscpy( &srv[1], ((ARR_SIZE(srv) - 1)*sizeof(uint8)), (void*)&(value), sizeof(value) );
        event_report_payload_ext( EVENT_CM_DS_SYSTEM_MODE, sizeof(srv), srv, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
       }

      if((ss_info_ptr->changed_fields_subs & CM_SS_EVT_PS_DATA_SUSPEND_MASK) != 0)
      {
        uint8 result[3];
        result[0] = (uint8)ss_info_ptr->asubs_id;
        result[1] = (uint8)ss_info_ptr->ps_data_suspend;
        result[2] = (uint8)ss_info_ptr->stack_info[0].sys_mode;

        event_report_payload_ext( EVENT_CM_DS_DS_INTERRAT_STATE, sizeof(result), result, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      if(((ss_info_ptr->stack_info[0].changed_fields & CM_SS_EVT_SRV_IND_MASK) !=0) &&
          ((diag_client.sub_info[asubs_id].cell_srv_ind.hs_ind != ss_info_ptr->stack_info[0].cell_srv_ind.hs_ind) ||
           (diag_client.sub_info[asubs_id].cell_srv_ind.dtm_supp != ss_info_ptr->stack_info[0].cell_srv_ind.dtm_supp)))
      {

        uint8 result[6];
        diag_client.sub_info[asubs_id].cell_srv_ind.hs_ind = ss_info_ptr->stack_info[0].cell_srv_ind.hs_ind;
        diag_client.sub_info[asubs_id].cell_srv_ind.dtm_supp = ss_info_ptr->stack_info[0].cell_srv_ind.dtm_supp;
        result[0] = (uint8)ss_info_ptr->asubs_id ;
        result[1] = (uint8)ss_info_ptr->stack_info[0].cell_srv_ind.hs_ind;
        result[2] = (uint8)ss_info_ptr->stack_info[0].cell_srv_ind.dtm_supp;

        /* Setting unused bytes to zero */
        result[3]=result[4]=result[5]= (uint8)0;

        event_report_payload_ext( EVENT_CM_DS_CELL_SRV_IND, sizeof(result), result, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }

      /* Store the current service information in diag client */
      diag_client.sub_info[asubs_id].srv_status = ss_info_ptr->stack_info[0].srv_status;

      break;

#if (defined(FEATURE_GSM) || defined(FEATURE_WCDMA))
    case CM_SS_EVENT_PS_DATA_AVAIL:
    {
      uint8 result[2];
      result[0] = (uint8)ss_info_ptr->asubs_id;
      result[1] = (uint8)ss_info_ptr->stack_info[0].sys_mode;
      event_report_payload_ext( EVENT_CM_DS_DATA_AVAILABLE, sizeof(result), result, EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
    }
    break;
#endif /* FEATURE_PWR_DORMANT_PWR_SAVE */

    default:
      break;
  } /* switch */

  /*
  ** Generate log packet for SS event
  */
  /*
  ** Compute log packet length
  */
  log_pkt_len = sizeof(LOG_CM_SS_MSIM_EVENT_C_type);
  /* Allocate a log packet.
  */
  log_ptr = (LOG_CM_SS_MSIM_EVENT_C_type *) log_alloc_ex( LOG_CM_SS_MSIM_EVENT_C,
            log_pkt_len );
  /*
  ** If allocation failed, just return.
  ** It means that either logging of LOG_CM_SS_EVENT_C is off
  ** or DIAG is truly out of memory (rare)
  */
  if ( log_ptr == NULL )
  {
    return;
  }

  /*lint -save -e641 Converting enum to int */
  /*
  ** Write log packet version
  ** This is always set to CM_LOG_SS_EVENT_MSIM_LOG_PACKET_VERSION
  */

  log_ptr->version                = CM_LOG_SS_EVENT_MSIM_LOG_PACKET_VERSION;
  log_ptr->ss_event               = ss_event;
  log_ptr->asubs_id               = ss_info_ptr->asubs_id ;
  log_ptr->number_of_stacks       = ss_info_ptr->number_of_stacks ;
  for(index_val = 0 ; index_val<= 1 ; index_val++)
  {
    log_ptr->stack_info[index_val].changed_fields         = ss_info_ptr->stack_info[index_val].changed_fields;
    log_ptr->stack_info[index_val].changed_fields2        = ss_info_ptr->stack_info[index_val].changed_fields2;
    log_ptr->stack_info[index_val].signal_strength_changed_fields        = ss_info_ptr->stack_info[index_val].signal_strength_changed_fields;
    log_ptr->stack_info[index_val].is_operational         = ss_info_ptr->stack_info[index_val].is_operational ;
    log_ptr->stack_info[index_val].true_srv_status           = ss_info_ptr->stack_info[index_val].true_srv_status;

    log_ptr->stack_info[index_val].srv_status             = ss_info_ptr->stack_info[index_val].srv_status;

    log_ptr->stack_info[index_val].srv_domain             = ss_info_ptr->stack_info[index_val].srv_domain;
    log_ptr->stack_info[index_val].srv_capability         = ss_info_ptr->stack_info[index_val].srv_capability;
    log_ptr->stack_info[index_val].sys_mode               = ss_info_ptr->stack_info[index_val].sys_mode;
    log_ptr->stack_info[index_val].roam_status            = ss_info_ptr->stack_info[index_val].roam_status;

    log_ptr->stack_info[index_val].sys_id.id_type         = ss_info_ptr->stack_info[index_val].sys_id.id_type;

    switch( ss_info_ptr->stack_info[index_val].sys_id.id_type )
    {

      case SYS_SYS_ID_TYPE_IS95:

        log_ptr->stack_info[index_val].sys_id.id.is95.sid = ss_info_ptr->stack_info[index_val].sys_id.id.is95.sid;
        log_ptr->stack_info[index_val].sys_id.id.is95.nid = ss_info_ptr->stack_info[index_val].sys_id.id.is95.nid;
        break;

      case SYS_SYS_ID_TYPE_IS856:

        for( i=0; i<16; i++ )
        {
          log_ptr->stack_info[index_val].sys_id.id.is856[i] = ss_info_ptr->stack_info[index_val].sys_id.id.is856[i];
        }
        break;


      case SYS_SYS_ID_TYPE_UMTS:

        log_ptr->stack_info[index_val].sys_id.id.plmn_lac.lac = ss_info_ptr->stack_info[index_val].sys_id.id.plmn_lac.lac;

        for( i=0; i<3; i++ )
        {
          log_ptr->stack_info[index_val].sys_id.id.plmn_lac.plmn.identity[i] =
            ss_info_ptr->stack_info[index_val].sys_id.id.plmn_lac.plmn.identity[i];
          log_ptr->stack_info[index_val].sys_id.id.plmn.identity[i] =
            ss_info_ptr->stack_info[index_val].sys_id.id.plmn.identity[i];

        }
        break;

      case SYS_SYS_ID_TYPE_UNDEFINED:

        memset((void *) &log_ptr->stack_info[index_val].sys_id.id, CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
               sizeof(log_ptr->stack_info[index_val].sys_id.id));
        break;

      default:

        memset((void *) &log_ptr->stack_info[index_val].sys_id, CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
               sizeof(log_ptr->stack_info[index_val].sys_id));
        break;
    } /* switch( ss_info_ptr->sys_id.id_type ) */

    log_ptr->stack_info[index_val].is_sys_forbidden       = ss_info_ptr->stack_info[index_val].is_sys_forbidden;
    if(ss_info_ptr->stack_info[index_val].sys_mode == SYS_SYS_MODE_HDR)
    {
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.is_colloc              = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.is_colloc;
    }
    else
    {
      log_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.is_colloc              = ss_info_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.is_colloc;
    }

    log_ptr->stack_info[index_val].rssi                   = ss_info_ptr->stack_info[index_val].rssi;
    log_ptr->stack_info[index_val].ecio                   = ss_info_ptr->stack_info[index_val].ecio;
    log_ptr->stack_info[index_val].io                     = ss_info_ptr->stack_info[index_val].io;
    log_ptr->stack_info[index_val].sinr                   = ss_info_ptr->stack_info[index_val].sinr;
    log_ptr->stack_info[index_val].psist                  = ss_info_ptr->stack_info[index_val].psist;
    log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_hybrid             = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_hybrid;
    log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_active_prot        = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_active_prot;
    log_ptr->stack_info[index_val].prev_idle_digital_mode = ss_info_ptr->stack_info[index_val].prev_idle_digital_mode;
    log_ptr->stack_info[index_val].cur_idle_digital_mode  = ss_info_ptr->stack_info[index_val].cur_idle_digital_mode;

    switch ( ss_info_ptr->stack_info[index_val].sys_mode )
    {
      case SYS_SYS_MODE_NONE:
      case SYS_SYS_MODE_GPS:
      case SYS_SYS_MODE_NO_SRV:

        memset((void *) &log_ptr->stack_info[index_val].mode_info, CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
               sizeof(log_ptr->stack_info[index_val].mode_info));
        break;


      case SYS_SYS_MODE_GSM:
      case SYS_SYS_MODE_WCDMA:
      case SYS_SYS_MODE_LTE:
      case SYS_SYS_MODE_TDS:

        /* Copy GW info here
        */
        log_ptr->stack_info[index_val].mode_info.gw_info.bit_err_rate   = ss_info_ptr->stack_info[index_val].mode_info.gw_info.bit_err_rate;
        log_ptr->stack_info[index_val].mode_info.gw_info.sim_state      = ss_info_ptr->stack_info[MM_STACK_0].mode_info.gw_info.sim_state;
        log_ptr->stack_info[index_val].mode_info.gw_info.plmn_forbidden = ss_info_ptr->stack_info[index_val].mode_info.gw_info.plmn_forbidden;

        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.plmn_avail                =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.plmn_avail;
        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name_avail           =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name_avail;
        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name_avail          =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name_avail;
        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone_avail =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone_avail;
        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.time_zone_avail           =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.time_zone_avail;
        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity_avail        =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity_avail;
        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.daylight_saving_adj_avail =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.daylight_saving_adj_avail;

        if( log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.plmn_avail )
        {
          for( i=0; i<3; i++ )
          {
            log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.plmn.identity[i] =
              ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.plmn.identity[i];
          }

        }
        else
        {

          memset((void *) &log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.plmn,
                 CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
                 sizeof(log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.plmn));
        }


        if( log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name_avail )
        {
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.length        =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.length;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.coding_scheme =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.coding_scheme;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.add_country_initials =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.add_country_initials;

          for( i=0; i<log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.length; i++)
          {
            log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.name[i] =
              ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name.name[i];
          }
        }
        else
        {

          memset((void *) &log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name,
                 CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
                 sizeof(log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.full_name));
        }


        if( log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name_avail )
        {
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.length        =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.length;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.coding_scheme =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.coding_scheme;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.add_country_initials =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.add_country_initials;

          for( i=0; i<log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.length; i++)
          {
            log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.name[i] =
              ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name.name[i];
          }
        }
        else
        {

          memset((void *) &log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name,
                 CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
                 sizeof(log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.short_name));
        }

        if( log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone_avail )
        {
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.year      =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.year;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.month     =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.month;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.day       =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.day;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.hour      =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.hour;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.minute    =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.minute;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.second    =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.second;
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.time_zone =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone.time_zone;

        }
        else
        {

          memset((void *) &log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone,
                 CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
                 sizeof(log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.univ_time_and_time_zone));
        }

        if( log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity_avail )
        {
          log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity.type =
            ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity.type;

          for(i=0; i<3; i++ )
          {
            log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity.identity[i] =
              ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity.identity[i];
          }
        }
        else
        {

          memset((void *) &log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity,
                 CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
                 sizeof(log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.lsa_identity));
        }

        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.time_zone           =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.time_zone;
        log_ptr->stack_info[index_val].mode_info.gw_info.mm_information.daylight_saving_adj =
          ss_info_ptr->stack_info[index_val].mode_info.gw_info.mm_information.daylight_saving_adj;
        break;

      default:

        /* Copy CDMA info here
        */

        log_ptr->stack_info[index_val].mode_info.cdma_info.band_class      =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.band_class;
        log_ptr->stack_info[index_val].mode_info.cdma_info.block_or_system =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.block_or_system;
        log_ptr->stack_info[index_val].mode_info.cdma_info.reg_zone        =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.reg_zone;
        log_ptr->stack_info[index_val].mode_info.cdma_info.packet_zone     =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.packet_zone;
        log_ptr->stack_info[index_val].mode_info.cdma_info.bs_p_rev        =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.bs_p_rev;
        log_ptr->stack_info[index_val].mode_info.cdma_info.p_rev_in_use    =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.p_rev_in_use;
        log_ptr->stack_info[index_val].mode_info.cdma_info.is_registered   =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.is_registered;
        log_ptr->stack_info[index_val].mode_info.cdma_info.ccs_supported   =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.ccs_supported;
        log_ptr->stack_info[index_val].mode_info.cdma_info.uz_id           =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.uz_id;

        log_ptr->stack_info[index_val].mode_info.cdma_info.uz_name.uz_name_encoding =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.uz_name.uz_name_encoding;
        log_ptr->stack_info[index_val].mode_info.cdma_info.uz_name.uz_name_n_octets =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.uz_name.uz_name_n_octets;

        for( i=0; i<SYS_PUZL_MAX_UZ_NAME_LEN; i++)
        {
          log_ptr->stack_info[index_val].mode_info.cdma_info.uz_name.uz_name[i] =
            ss_info_ptr->stack_info[index_val].mode_info.cdma_info.uz_name.uz_name[i];
        }

        log_ptr->stack_info[index_val].mode_info.cdma_info.srch_win_n =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.srch_win_n;
        log_ptr->stack_info[index_val].mode_info.cdma_info.base_lat   =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.base_lat;
        log_ptr->stack_info[index_val].mode_info.cdma_info.base_long  =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.base_long;
        log_ptr->stack_info[index_val].mode_info.cdma_info.base_id    =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.base_id;

        /*Copy CDMA time info into log packet*/
        log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.sys_mode =
          ss_info_ptr->stack_info[index_val].mode_info.cdma_info.time_info.sys_mode;
        if(log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.sys_mode == SYS_SYS_MODE_CDMA)
        {
          log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.cdma_time.lp_sec =
            ss_info_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.cdma_time.lp_sec;
          log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.cdma_time.ltm_offset =
            ss_info_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.cdma_time.ltm_offset;
          log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.cdma_time.daylt_savings =
            ss_info_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.cdma_time.daylt_savings;
        }
        if(log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.sys_mode == SYS_SYS_MODE_HDR)
        {
          log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.hdr_time.lp_sec =
            ss_info_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.hdr_time.lp_sec;
          log_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.hdr_time.ltm_offset =
            ss_info_ptr->stack_info[index_val].mode_info.cdma_info.time_info.time.hdr_time.ltm_offset;
        }
        break;

    } /* switch (ss_info_ptr->sys_mode) */


    log_ptr->stack_info[cmss_map_stack_id_to_client_stk(MM_STACK_1)].bcmcs_srv_supported = ss_info_ptr->stack_info[cmss_map_stack_id_to_client_stk(MM_STACK_1)].bcmcs_srv_supported;
    log_ptr->stack_info[index_val].bcmcs_srv_supported  = ss_info_ptr->stack_info[index_val].bcmcs_srv_supported;

    if(ss_info_ptr->stack_info[index_val].sys_mode == SYS_SYS_MODE_GSM)
    {
      log_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.lac = ss_info_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.lac;
      log_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.rac_or_mme_code = ss_info_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.rac_or_mme_code;
      log_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.csg_info.csg_id = ss_info_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.csg_info.csg_id ;
      log_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.csg_info.hybrid_cell = ss_info_ptr->stack_info[index_val].rat_mode_info.gsm_mode_info.csg_info.hybrid_cell ;
    }
    else if(ss_info_ptr->stack_info[index_val].sys_mode == SYS_SYS_MODE_CDMA)
    {
      log_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.cdma_reg_prd = ss_info_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.cdma_reg_prd;
      log_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.frame_err_rate = ss_info_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.frame_err_rate;
      log_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.is_colloc = ss_info_ptr->stack_info[index_val].rat_mode_info.cdma_mode_info.is_colloc;
    }
    else if(ss_info_ptr->stack_info[index_val].sys_mode == SYS_SYS_MODE_HDR)
    {
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_active_prot = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_active_prot;
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.subnet_mask_len = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.subnet_mask_len;
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_ln_status = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_ln_status;
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_packet_err_rate = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_packet_err_rate;
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_personality = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_personality;
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.is_colloc = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.is_colloc;
      log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_hybrid = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_hybrid;
    }
    else if(ss_info_ptr->stack_info[index_val].sys_mode == SYS_SYS_MODE_WCDMA)
    {
      log_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.lac = ss_info_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.lac;
      log_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.rac_or_mme_code = ss_info_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.rac_or_mme_code;
      log_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.csg_info.csg_id = ss_info_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.csg_info.csg_id;
      log_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.csg_info.hybrid_cell = ss_info_ptr->stack_info[index_val].rat_mode_info.wcdma_mode_info.csg_info.hybrid_cell;
    }
    else if(ss_info_ptr->stack_info[index_val].sys_mode == SYS_SYS_MODE_LTE)
    {
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.rsrp = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.rsrp;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.rsrq = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.rsrq;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.lac  = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.lac;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.tac  = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.tac;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.rac_or_mme_code = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.rac_or_mme_code;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.embms_info.embms_coverage_status = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.embms_info.embms_coverage_status;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.embms_info.trace_id = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.embms_info.trace_id;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.csg_info.csg_id  = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.csg_info.csg_id ;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.csg_info.hybrid_cell = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.csg_info.hybrid_cell;
      log_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.voice_support_on_lte = ss_info_ptr->stack_info[index_val].rat_mode_info.lte_mode_info.voice_support_on_lte;
    }
    else if(ss_info_ptr->stack_info[index_val].sys_mode == SYS_SYS_MODE_TDS)
    {
      log_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.lac = ss_info_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.lac;
      log_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.rac_or_mme_code = ss_info_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.rac_or_mme_code;
      log_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.csg_info.csg_id = ss_info_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.csg_info.csg_id;
      log_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.csg_info.hybrid_cell = ss_info_ptr->stack_info[index_val].rat_mode_info.tds_mode_info.csg_info.hybrid_cell;
    }





#if (defined(FEATURE_UMTS_1X_HANDOVER_1XMSM) || \
       defined(FEATURE_UMTS_1X_HANDOVER_UMTSMSM))
    #error code not present
#else
    log_ptr->msm_id = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
#endif

    log_ptr->stack_info[index_val].cipher_domain = ss_info_ptr->stack_info[index_val].cipher_domain;

    log_ptr->stack_info[index_val].thr_status.ss_thr_status = ss_info_ptr->stack_info[index_val].thr_status.ss_thr_status;
    log_ptr->stack_info[index_val].thr_status.thr_uptime    = ss_info_ptr->stack_info[index_val].thr_status.thr_uptime;
    log_ptr->stack_info[index_val].thr_status.idx           = ss_info_ptr->stack_info[index_val].thr_status.idx;
    log_ptr->stack_info[index_val].thr_status.call_status   = ss_info_ptr->stack_info[index_val].thr_status.call_status;



    /* Below fields added for version = 5 and upwards */

    log_ptr->stack_info[index_val].true_srv_status                 = ss_info_ptr->stack_info[index_val].true_srv_status;
    log_ptr->stack_info[index_val].signal_strength_changed_fields
      = ss_info_ptr->stack_info[index_val].signal_strength_changed_fields;
    log_ptr->stack_info[index_val].pathloss                        = ss_info_ptr->stack_info[index_val].pathloss;
    log_ptr->stack_info[index_val].sir                             = ss_info_ptr->stack_info[index_val].sir;
    log_ptr->stack_info[index_val].rssi2                           = ss_info_ptr->stack_info[index_val].rssi2;
    log_ptr->stack_info[index_val].rscp                            = ss_info_ptr->stack_info[index_val].rscp;
    log_ptr->stack_info[index_val].ecio_for_display                = ss_info_ptr->stack_info[index_val].ecio_for_display;

    log_ptr->stack_info[index_val].ecio            = ss_info_ptr->stack_info[index_val].ecio;

    log_ptr->stack_info[index_val].cell_info.cell_id = ss_info_ptr->stack_info[index_val].cell_info.cell_id;

    log_ptr->stack_info[index_val].reg_reject_info.reject_srv_domain
      = ss_info_ptr->stack_info[index_val].reg_reject_info.reject_srv_domain;
    log_ptr->stack_info[index_val].reg_reject_info.reject_cause
      = ss_info_ptr->stack_info[index_val].reg_reject_info.reject_cause;

    log_ptr->gan_info.gan_state = ss_info_ptr->gan_info.gan_state;

    log_ptr->gan_info.gan_conn_status = ss_info_ptr->gan_info.gan_conn_status;

    log_ptr->gan_info.gan_sig_chan_status =
      ss_info_ptr->gan_info.gan_sig_chan_status;

    log_ptr->gan_info.gan_mode_display.is_mode_display_on     =
      ss_info_ptr->gan_info.gan_mode_display.is_mode_display_on;

    log_ptr->gan_info.gan_mode_display.gan_usz_icon_indicator =
      ss_info_ptr->gan_info.gan_mode_display.gan_usz_icon_indicator;

    log_ptr->gan_info.gan_mode_display.gan_ms_location_status =
      ss_info_ptr->gan_info.gan_mode_display.gan_ms_location_status;

    memscpy( (void *)log_ptr->gan_info.gan_mode_display.usz_str,
             ARR_SIZE(log_ptr->gan_info.gan_mode_display.usz_str),
             ss_info_ptr->gan_info.gan_mode_display.usz_str,
             sizeof(ss_info_ptr->gan_info.gan_mode_display.usz_str));

    log_ptr->gan_info.gan_error.gan_error_category =
      ss_info_ptr->gan_info.gan_error.gan_error_category;

    log_ptr->gan_info.gan_error.gan_error_codepoint =
      ss_info_ptr->gan_info.gan_error.gan_error_codepoint;


    /* Below fields added for version = 6 and upwards */

    log_ptr->stack_info[index_val].cell_srv_ind.hs_ind     = ss_info_ptr->stack_info[index_val].cell_srv_ind.hs_ind;
    log_ptr->stack_info[index_val].cell_srv_ind.dtm_supp   = ss_info_ptr->stack_info[index_val].cell_srv_ind.dtm_supp;
    log_ptr->stack_info[index_val].cell_srv_ind.egprs_supp = ss_info_ptr->stack_info[index_val].cell_srv_ind.egprs_supp;
    log_ptr->stack_info[index_val].cell_srv_ind.hs_call_status
      = ss_info_ptr->stack_info[index_val].cell_srv_ind.hs_call_status;

    log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_active_prot           = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_active_prot;
    log_ptr->stack_info[index_val].bcmcs_srv_status = ss_info_ptr->stack_info[index_val].bcmcs_srv_status;

    log_ptr->ipapp_info.reg_status       = ss_info_ptr->ipapp_info.reg_status;
    log_ptr->ipapp_info.cause            = ss_info_ptr->ipapp_info.cause;

    log_ptr->ipapp_info.warn_info.present
      = ss_info_ptr->ipapp_info.warn_info.present;
    log_ptr->ipapp_info.warn_info.cause
      = ss_info_ptr->ipapp_info.warn_info.cause;

    memscpy( (void *)log_ptr->ipapp_info.warn_info.hostname,
             sizeof(log_ptr->ipapp_info.warn_info.hostname),
             ss_info_ptr->ipapp_info.warn_info.hostname,
             sizeof(ss_info_ptr->ipapp_info.warn_info.hostname));

    memscpy( (void *)log_ptr->ipapp_info.warn_info.description,
             sizeof(log_ptr->ipapp_info.warn_info.description),
             ss_info_ptr->ipapp_info.warn_info.description,
             sizeof(ss_info_ptr->ipapp_info.warn_info.description));

    for(cap_index=0; cap_index<CMIPAPP_CAP_ARR_MAX; cap_index++)
    {
      log_ptr->ipapp_info.call_type_mask[cap_index] = ss_info_ptr->ipapp_info.call_type_mask[cap_index];
      log_ptr->ipapp_info.sys_mode[cap_index]       = ss_info_ptr->ipapp_info.sys_mode[cap_index];
    }

    log_ptr->stack_info[index_val].active_band               = ss_info_ptr->stack_info[index_val].active_band;
    log_ptr->stack_info[index_val].active_channel            = ss_info_ptr->stack_info[index_val].active_channel;

    log_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_ln_status = ss_info_ptr->stack_info[index_val].rat_mode_info.hdr_mode_info.hdr_ln_status;


  }
  /* Below fields added for version = 7 and upwards */
  log_ptr->curr_uptime        = ss_info_ptr->curr_uptime;
  log_ptr->current_mcc = ss_info_ptr->current_mcc;
  log_ptr->home_mcc    = ss_info_ptr->home_mcc;
  log_ptr->plus_dial_setting = ss_info_ptr->plus_dial_setting;
  log_ptr->def_roam_ind      = ss_info_ptr->def_roam_ind;
  log_ptr->changed_fields_subs = ss_info_ptr->changed_fields_subs ;
  log_ptr->is_csfb_call_active = ss_info_ptr->is_csfb_call_active ;
  log_ptr->bsr_in_progress = ss_info_ptr->bsr_in_progress ;
  log_ptr->eutra_detect_status = ss_info_ptr->eutra_detect_status ;
  log_ptr->requesting_client_id         = ss_info_ptr->requesting_client_id;
  log_ptr->target_rat = ss_info_ptr->target_rat ;
  log_ptr->irat_ctxt_target_rat = ss_info_ptr->irat_ctxt_target_rat ;
  log_ptr->gwl_irat_asubs_id = ss_info_ptr->gwl_irat_asubs_id ;
  log_ptr->voice_domain = ss_info_ptr->voice_domain ;
  log_ptr->sms_domain = ss_info_ptr->sms_domain ;
  log_ptr->div_duplex = ss_info_ptr->div_duplex ;
  log_ptr->def_roam_ind = ss_info_ptr->def_roam_ind ;
  log_ptr->ps_data_suspend        = ss_info_ptr->ps_data_suspend;
  log_ptr->sys_sel_pref_req_id = ss_info_ptr->sys_sel_pref_req_id ;


  /* Update CDMA parameters for Diag */
  diag_client.sub_info[asubs_id].active_channel   =  ss_info_ptr->stack_info[0].active_channel;
  diag_client.sub_info[asubs_id].band_class       =  ss_info_ptr->stack_info[0].mode_info.cdma_info.band_class;
  diag_client.sub_info[asubs_id].packet_zone      =  ss_info_ptr->stack_info[0].mode_info.cdma_info.packet_zone;
  diag_client.sub_info[asubs_id].roam_status      =  ss_info_ptr->stack_info[0].roam_status;
  diag_client.sub_info[asubs_id].latitude         =  ss_info_ptr->stack_info[0].mode_info.cdma_info.base_lat;
  diag_client.sub_info[asubs_id].longitude        =  ss_info_ptr->stack_info[0].mode_info.cdma_info.base_long;
  diag_client.sub_info[asubs_id].mcc              =  ss_info_ptr->current_mcc;


  /* Submit the log to DIAG.
  */
  log_util_log_packet_commit( asubs_id, (void *) log_ptr );
} /* cmlog_ss_event_handler */

/*===========================================================================

FUNCTION  cmlog_is_dual_standby

DESCRIPTION
 The function checks if standby pref is dual standby vs. single standby.

DEPENDENCIES
 none

RETURN VALUE
  TRUE if standby pref is dual standby.
  FALSE otherwise.

SIDE EFFECTS
  none

===========================================================================*/
static boolean cmlog_is_dual_standby (

  sys_modem_dual_standby_pref_e_type  standby_pref
)
{
  return ( standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY ||
           standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY_NO_TUNEAWAY);
}

/*===========================================================================
FUNCTION  cmlog_call_event_report_payload

Description:

       Generate DIAG events corresponding to given call event

===========================================================================*/
void cmlog_call_event_report_payload(

  cm_call_event_e_type          call_event,

  const cm_mm_call_info_s_type  *call_info_ptr
)
{
  cm_call_id_type         call_id;
  /* call id
  */

  int                     call_state_list_index;
  /* Code to log call id, call state and call type of all calls
  */

  uint32                  evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(call_info_ptr->asubs_id);

#ifdef FEATURE_MMODE_DUAL_SIM
  cmph_s_type             *ph_ptr = cmph_ptr();
#endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(call_info_ptr != NULL);
  CM_ASSERT(!BETWEEN(CM_CALL_EVENT_NONE, call_event, CM_CALL_EVENT_MAX));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if (defined (FEATURE_MMODE_CDMA_800) || defined (FEATURE_MMODE_CDMA_1900))
  /*
  **  Ignore these events if they arrived with CM_CALL_ID_UNASSIGNED
  **  ie. were sent via cmxcall_event_generic()
  */
  if ((call_event == CM_CALL_EVENT_SIGNAL)             ||
      (call_event == CM_CALL_EVENT_ABRV_ALERT)         ||
      (call_event == CM_CALL_EVENT_DISPLAY)            ||
      (call_event == CM_CALL_EVENT_EXT_DISP)           ||
      (call_event == CM_CALL_EVENT_EXIT_TC)            ||
      (call_event == CM_CALL_EVENT_REDIRECTING_NUMBER) ||
      (call_event == CM_CALL_EVENT_CONNECTED_NUM)      ||
      (call_event == CM_CALL_EVENT_CALLER_ID)          ||
      (call_event == CM_CALL_EVENT_CALLED_PARTY)       ||
      (call_event == CM_CALL_EVENT_EXT_BRST_INTL)
     )
  {
    if(call_info_ptr->call_id == CM_CALL_ID_UNASSIGNED)
    {
      CM_MSG_HIGH_1("CM_CALL_ID_UNASSIGNED for call event %d",call_event);
      return;
    }
  }
#endif   /* FEATURE_MMODE_CDMA_800 || FEATURE_MMODE_CDMA_1900 */

  if(call_event == CM_CALL_EVENT_CONFERENCE_INFO || call_event == CM_CALL_EVENT_JOIN_CONFERENCE ||
      (call_event == CM_CALL_EVENT_MNG_CALLS_CONF &&
       (call_info_ptr->mode_info.info.ip_call.sups_type ==CM_CALL_SUPS_TYPE_MULTIPARTY_CALL  ||
        call_info_ptr->mode_info.info.ip_call.sups_type ==CM_CALL_SUPS_TYPE_HOLD_CALL ||
        call_info_ptr->mode_info.info.ip_call.sups_type == CM_CALL_SUPS_TYPE_HOLD_PER_CALL ||
        call_info_ptr->mode_info.info.ip_call.sups_type == CM_CALL_SUPS_TYPE_RESUME_PER_CALL)))
  {
    CM_MSG_HIGH_0("ignore updating call state for conference success ");
    return;
  }


  call_id = call_info_ptr->call_id;


  if (call_id >= CMCALL_MAX_CALL_OBJ)
  {
    CM_MSG_HIGH_0 ("Not enough buff to update call_state_list[] \n");
    return;
  }

  /* Check if we have previous information about call_id provided by CM */
  call_state_list_index = cmlog_get_call_state_list_index (call_id);

  if (call_state_list_index >= CMCALL_MAX_CALL_OBJ)
  {
    /* Need to create an entry for new call */
    call_state_list_index = cmlog_set_call_state_list_entry (call_id);

    if (call_state_list_index < CMCALL_MAX_CALL_OBJ)
    {
      call_state_list[call_state_list_index].call_state =
        call_info_ptr->call_state;

      call_state_list[call_state_list_index].call_type  =
        call_info_ptr->call_type;

      call_state_list[call_state_list_index].asubs_id = call_info_ptr->asubs_id;
    }
    else
    {
      CM_ERR_0 ("Not enough buff to update call_state_list[] \n");
    }

  }
  else /* Already existing call id */
  {
    if (call_event == CM_CALL_EVENT_END)
    {
      call_state_list[call_state_list_index].call_id = CM_CALL_ID_INVALID;
    }
    else
    {
      call_state_list[call_state_list_index].call_state =
        call_info_ptr->call_state;

      call_state_list[call_state_list_index].call_type  =
        call_info_ptr->call_type;

      call_state_list[call_state_list_index].asubs_id = call_info_ptr->asubs_id;
    }
  } /* if (call_state_list_index >= CM_CALL_ID_MAX) */

  /* Code to log calls ends */

  switch ( call_event )
  {
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CM_CALL_EVENT_ORIG:
    {
#ifdef FEATURE_MMODE_DUAL_SIM
      cmlog_call_orig_dsds_s_type call_orig;
#else
      cmlog_call_orig_s_type call_orig;
#endif
      cmlog_call_orig_2_s_type call_orig_client_id;

      cmcall_s_type *call_ptr = cmcall_ptr(call_info_ptr->call_id);

      if(call_ptr == NULL)
      {
        CM_ERR_1( "Invalid Callid: %d", call_info_ptr->call_id);
        break;
      }

      call_orig.call_id = (uint8) call_info_ptr->call_id;
      call_orig.call_type = (uint8) call_info_ptr->call_type;
      call_orig.srv_type = (uint8) call_ptr->srv_type;

      call_orig_client_id.call_id = (uint8) call_info_ptr->call_id;
      call_orig_client_id.call_type = (uint8) call_info_ptr->call_type;
      call_orig_client_id.srv_type = (uint8) call_ptr->srv_type;
      call_orig_client_id.client_id = (uint8) call_info_ptr->call_client_id;
      call_orig_client_id.subs_id  = (uint8)call_ptr->mm_id.asubs_id;

#ifdef FEATURE_MMODE_DUAL_SIM
      call_orig.asubs_id = (uint8) call_ptr->mm_id.asubs_id;

      if (ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#if defined(FEATURE_MMODE_TRIPLE_SIM)
          || (ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY )
#endif
         )
      {
        event_report_payload_ext( EVENT_CM_DS_CALL_EVENT_ORIG,
                                  sizeof(cmlog_call_orig_dsds_s_type), &call_orig,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      }
      else
      {
        event_report_payload_ext( EVENT_CM_CALL_EVENT_ORIG,
                                  sizeof(cmlog_call_orig_dsds_s_type)-1, ((uint8*)&call_orig)+1,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      }
#else
      {

          event_report_payload_ext( EVENT_CM_CALL_EVENT_ORIG,
                                    sizeof(cmlog_call_orig_s_type), &call_orig,
                                    EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
        }
        #endif
        event_report_payload_ext( EVENT_CM_CALL_EVENT_ORIG_2,
                                  sizeof(cmlog_call_orig_2_s_type), &call_orig_client_id,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
    }
    break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CM_CALL_EVENT_CONNECT:
    {
#ifdef FEATURE_MMODE_DUAL_SIM
      cmlog_call_connect_dsds_s_type call_connect;
#else
      cmlog_call_connect_s_type call_connect;
#endif
      cmlog_call_connect_2_s_type call_connect_client_id;

      call_connect.call_id = (uint8) call_info_ptr->call_id;
      call_connect.call_type = (uint8) call_info_ptr->call_type;
      call_connect.sys_mode = (uint8) call_info_ptr->sys_mode;


      call_connect_client_id.call_id = (uint8) call_info_ptr->call_id;
      call_connect_client_id.call_type = (uint8) call_info_ptr->call_type;
      call_connect_client_id.sys_mode = (uint8) call_info_ptr->sys_mode;
      call_connect_client_id.client_id = (uint8)call_info_ptr->call_client_id;
      call_connect_client_id.subs_id = (uint8)call_info_ptr->asubs_id;

#ifdef FEATURE_MMODE_DUAL_SIM
      if (ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#if defined(FEATURE_MMODE_TRIPLE_SIM)
          || (ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY )
#endif
         )
      {
        event_report_payload_ext( EVENT_CM_DS_CALL_EVENT_CONNECT,
                                  sizeof(cmlog_call_connect_dsds_s_type), &call_connect,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      }
      else
      {
        event_report_payload_ext( EVENT_CM_CALL_EVENT_CONNECT,
                                  sizeof(cmlog_call_connect_dsds_s_type)-1, ((uint8 *)&call_connect)+1,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }
#else
      {

        event_report_payload_ext( EVENT_CM_CALL_EVENT_CONNECT,
                                  sizeof(cmlog_call_connect_s_type), &call_connect,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      }
#endif

      event_report_payload_ext( EVENT_CM_CALL_EVENT_CONNECT_2,
                                sizeof(cmlog_call_connect_2_s_type), &call_connect_client_id,
                                EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      CM_MSG_HIGH_1_EXT("EVENT_CM_CALL_EVENT_CONNECT_2 - asubid %d",
                        call_connect_client_id.subs_id, (call_connect_client_id.subs_id+1));
    }
    break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CM_CALL_EVENT_END:
    {
      cmlog_call_end_s_type call_end;
      cmlog_call_end_2_s_type call_end_client_id;

      call_end.num_calls = 1;
      call_end.call_id = (uint8) call_info_ptr->call_id;

      call_end_client_id.num_calls = 1;
      call_end_client_id.call_id = (uint8) call_info_ptr->call_id;
      call_end_client_id.client_id = (uint8)call_info_ptr->call_client_id;
      call_end_client_id.subs_id = (uint8)call_info_ptr->asubs_id;
      event_report_payload_ext( EVENT_CM_CALL_EVENT_END,
                                sizeof(cmlog_call_end_s_type), &call_end,
                                EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      event_report_payload_ext( EVENT_CM_CALL_EVENT_END_2,
                                sizeof(cmlog_call_end_2_s_type), &call_end_client_id,
                                EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      CM_MSG_HIGH_1_EXT("EVENT_CM_CALL_EVENT_END_2 - asubid %d",call_end_client_id.subs_id,(call_end_client_id.subs_id+1));
    }
    break;

      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if !(defined(FEATURE_GSM) || defined(FEATURE_WCDMA))
    case CM_CALL_EVENT_PS_DATA_AVAILABLE:
    {
      uint8 evt_data[2];

      evt_data[1] = (uint8) cmss_get_info_sys_mode(call_info_ptr->asubs_id, MM_STACK_0);

#ifdef FEATURE_MMODE_DUAL_SIM
      if ((ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY )
#ifdef FEATURE_MMODE_TRIPLE_SIM
          || (ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY )
#endif
         )
      {
        evt_data[0] = call_info_ptr->asubs_id;

        event_report_payload_ext( EVENT_CM_DS_DATA_AVAILABLE,
                                  sizeof(evt_data), &evt_data, 
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      }
      else
#endif
      {
        event_report_payload_ext( EVENT_CM_DATA_AVAILABLE,
                                  1, &evt_data[1], 
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
      }
    }
    break;
#endif /* FEATURE_PS_DORMANT_PWR_SAVE */

      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if defined(FEATURE_WCDMA)
    case CM_CALL_EVENT_RAB_REL_IND:
    {
      if (call_state_list_index < CMCALL_MAX_CALL_OBJ)
      {
        call_state_list[call_state_list_index].is_ps_call_dormant = TRUE;
      }

      CM_MSG_HIGH_1 ("RAB rel for call %d", call_info_ptr->call_id);
    }
    break;

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CM_CALL_EVENT_RAB_REESTAB_IND:
    {
      if (call_state_list_index < CMCALL_MAX_CALL_OBJ)
      {
        call_state_list[call_state_list_index].is_ps_call_dormant = FALSE;
      }

      CM_MSG_HIGH_4_EXT ("RAB reestab for call_id %d, call_state %d, sys_mode %d, sub %d",
                         call_info_ptr->call_id,
                         call_info_ptr->call_state,
                         call_info_ptr->sys_mode,
                         call_info_ptr->asubs_id,
                         (call_info_ptr->asubs_id+1));
    }
    break;

#endif /* FEATURE_WCDMA */

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    case CM_CALL_EVENT_CALL_ORIG_THR:
    {
      cmlog_call_orig_s_type call_orig;
      cmcall_s_type *call_ptr = cmcall_ptr(call_info_ptr->call_id);
      if(call_ptr == NULL)
      {
        CM_MSG_HIGH_0( "call_ptr_NULL");
        break;
      }
      call_orig.call_id = (uint8) call_info_ptr->call_id;
      call_orig.call_type = (uint8) call_info_ptr->call_type;
      call_orig.srv_type = (uint8) call_ptr->srv_type;
      event_report_payload_ext( EVENT_CM_CALL_EVENT_ORIG_THR,
                                sizeof(cmlog_call_orig_s_type), &call_orig,
                                EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id );
    }
    break;
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    default:
      break;
  } /* switch */
}

/*===========================================================================
FUNCTION  cmlog_call_event_handler

Description:

       Generate log packet for given call event.

===========================================================================*/
/*lint -esym(765, cmlog_call_event_handler)
** Can't be static, registered to be called by CM for call event handling
*/
void cmlog_call_event_handler(

  cm_call_event_e_type          call_event,

  const cm_mm_call_info_s_type *call_info_ptr

)
{

  int                      i            = 0;
  /* Loop index */

  unsigned int             log_pkt_len  = 0;
  /* Length of packet to be logged.
  */

  LOG_CM_CALL_EVENT_C_type *log_ptr     = NULL;
  /* Log pointer to the log record type.
  */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(call_info_ptr != NULL);
  CM_ASSERT(!BETWEEN(CM_CALL_EVENT_NONE, call_event, CM_CALL_EVENT_MAX));

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Generate DIAG events for call event as appropriate
  */
  cmlog_call_event_report_payload(call_event, call_info_ptr);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*
  ** Generate log packet for call event
  */

  /*
  ** Compute log packet length
  */
  log_pkt_len = sizeof(LOG_CM_CALL_EVENT_C_type);

  /* Allocate a log packet.
  */
  log_ptr = (LOG_CM_CALL_EVENT_C_type *) log_alloc_ex( LOG_CM_CALL_EVENT_C,
            log_pkt_len );
  /*
  ** If allocation failed, just return.
  ** It means that either logging of LOG_CM_CALL_EVENT_C is off
  ** or DIAG is truly out of memory (rare)
  */
  if ( log_ptr == NULL )
  {
    CM_MSG_LOW_0("Log packet buffer allocation failed!");
    return;
  }

  /*lint -save -e641 Converting enum to int */

  /*
  ** Write log packet version
  ** This is always set to CM_LOG_CALL_EVENT_LOG_PACKET_VERSION
  */
  log_ptr->version        = CM_LOG_CALL_EVENT_LOG_PACKET_VERSION;

  /*
  ** Copy field by field from call_info_ptr
  ** Cannot do memcpy() etc because we're copying from something that
  ** is padded into something that is packed
  */

  log_ptr->call_event     = call_event;
  log_ptr->call_client_id = call_info_ptr->call_client_id;
  log_ptr->call_id        = call_info_ptr->call_id;
  log_ptr->call_state     = call_info_ptr->call_state;
  log_ptr->call_type      = call_info_ptr->call_type;


  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->num.len        = MIN(call_info_ptr->num.len,
                                ARR_SIZE(log_ptr->num.buf));

  for(i = 0; i < log_ptr->num.len; i++)
  {
    log_ptr->num.buf[i]   = call_info_ptr->num.buf[i];
  }

  log_ptr->num.digit_mode              = call_info_ptr->num.digit_mode;
  log_ptr->num.number_type             = call_info_ptr->num.number_type;


  log_ptr->num.number_plan             = call_info_ptr->num.number_plan;
  log_ptr->num.pi                      = call_info_ptr->num.pi;
  log_ptr->num.si                      = call_info_ptr->num.si;


  log_ptr->num.subaddr.extension_bit
    = call_info_ptr->num.subaddr.extension_bit;
  log_ptr->num.subaddr.subadd_type  = call_info_ptr->num.subaddr.subadd_type;
  log_ptr->num.subaddr.odd_even_ind
    = call_info_ptr->num.subaddr.odd_even_ind;

  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->num.subaddr.num_char  = MIN(call_info_ptr->num.subaddr.num_char,
                                       ARR_SIZE(log_ptr->num.subaddr.chari));

  for(i = 0; i < log_ptr->num.subaddr.num_char; i++)
  {
    log_ptr->num.subaddr.chari[i]      = call_info_ptr->num.subaddr.chari[i];
  }

  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->redirecting_number.num.len =
    MIN(call_info_ptr->redirecting_number.num.len,
        ARR_SIZE(log_ptr->redirecting_number.num.buf));

  for(i = 0; i < log_ptr->redirecting_number.num.len; i++)
  {
    log_ptr->redirecting_number.num.buf[i]
      = call_info_ptr->redirecting_number.num.buf[i];
  }

  log_ptr->redirecting_number.num.digit_mode
    = call_info_ptr->redirecting_number.num.digit_mode;
  log_ptr->redirecting_number.num.number_type
    = call_info_ptr->redirecting_number.num.number_type;
  log_ptr->redirecting_number.num.number_plan
    = call_info_ptr->redirecting_number.num.number_plan;
  log_ptr->redirecting_number.num.pi
    = call_info_ptr->redirecting_number.num.pi;
  log_ptr->redirecting_number.num.si
    = call_info_ptr->redirecting_number.num.si;
  log_ptr->redirecting_number.num.subaddr.extension_bit
    = call_info_ptr->redirecting_number.num.subaddr.extension_bit;


  log_ptr->redirecting_number.num.subaddr.subadd_type
    = call_info_ptr->redirecting_number.num.subaddr.subadd_type;
  log_ptr->redirecting_number.num.subaddr.odd_even_ind
    = call_info_ptr->redirecting_number.num.subaddr.odd_even_ind;

  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->redirecting_number.num.subaddr.num_char
    = MIN(call_info_ptr->redirecting_number.num.subaddr.num_char,
          ARR_SIZE(log_ptr->redirecting_number.num.subaddr.chari));

  for(i = 0; i < log_ptr->redirecting_number.num.subaddr.num_char; i++)
  {
    log_ptr->redirecting_number.num.subaddr.chari[i]
      = call_info_ptr->redirecting_number.num.subaddr.chari[i];
  }
  log_ptr->redirecting_number.redirecting_reason =
    call_info_ptr->redirecting_number.redirecting_reason;

  /* Copy MIN() to not overflow log packet buffer
  */
  log_ptr->alpha.len        = MIN(call_info_ptr->alpha.len,
                                  ARR_SIZE(log_ptr->alpha.buf));

  for(i = 0; i < log_ptr->alpha.len; i++)
  {
    log_ptr->alpha.buf[i]        = call_info_ptr->alpha.buf[i];
  }

  log_ptr->signal.is_signal_info_avail =
    call_info_ptr->signal.is_signal_info_avail;

  if(log_ptr->signal.is_signal_info_avail)
  {
    log_ptr->signal.signal_type = call_info_ptr->signal.signal_type;
    log_ptr->signal.alert_pitch = call_info_ptr->signal.alert_pitch;
    log_ptr->signal.signal = call_info_ptr->signal.signal;
  }
  else
  {
    log_ptr->signal.signal_type = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
    log_ptr->signal.alert_pitch = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
    log_ptr->signal.signal      = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  }

  log_ptr->end_status = call_info_ptr->end_status;
  log_ptr->num_call_ids_ended = call_info_ptr->num_call_ids_ended;

  log_ptr->end_params.call_id = call_info_ptr->end_params.call_id;
  log_ptr->end_params.info_type = call_info_ptr->end_params.info_type;

  if(log_ptr->end_params.info_type == CM_CALL_MODE_INFO_GW_CS)
  {
    log_ptr->end_params.end_params.gw_cs_end.reserved =
      call_info_ptr->end_params.end_params.gw_cs_end.reserved;
  }
  else if(log_ptr->end_params.info_type == CM_CALL_MODE_INFO_GW_PS)
  {
#ifdef CM_GW_SUPPORTED
    log_ptr->end_params.end_params.gw_ps_end.nsapi.valid =
      call_info_ptr->end_params.end_params.gw_ps_end.nsapi.valid;
    if(log_ptr->end_params.end_params.gw_ps_end.nsapi.valid)
    {
      log_ptr->end_params.end_params.gw_ps_end.nsapi.nsapi =
        call_info_ptr->end_params.end_params.gw_ps_end.nsapi.nsapi;
    }
    else
    {
      log_ptr->end_params.end_params.gw_ps_end.nsapi.nsapi =
        CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
    }

    log_ptr->end_params.end_params.gw_ps_end.tear_down_ind.valid =
      call_info_ptr->end_params.end_params.gw_ps_end.tear_down_ind.valid;
    if(log_ptr->end_params.end_params.gw_ps_end.tear_down_ind.valid)
    {
      log_ptr->end_params.end_params.gw_ps_end.tear_down_ind.ti_flag =
        call_info_ptr->end_params.end_params.gw_ps_end.tear_down_ind.ti_flag;
    }
    else
    {
      log_ptr->end_params.end_params.gw_ps_end.tear_down_ind.ti_flag =
        CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
    }
    log_ptr->end_params.end_params.gw_ps_end.cause =
      call_info_ptr->end_params.end_params.gw_ps_end.cause;
#endif
  }
  else if(log_ptr->end_params.info_type == CM_CALL_MODE_INFO_CDMA)
  {

#if (defined (FEATURE_MMODE_CDMA_800) || defined (FEATURE_MMODE_CDMA_1900))
    log_ptr->end_params.end_params.cdma_end.end_reason_included =
      call_info_ptr->end_params.end_params.cdma_end.end_reason_included;

    if(log_ptr->end_params.end_params.cdma_end.end_reason_included)
    {
      log_ptr->end_params.end_params.cdma_end.end_reason =
        call_info_ptr->end_params.end_params.cdma_end.end_reason;
    }
    else
    {
      log_ptr->end_params.end_params.cdma_end.end_reason =
        CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
    }
#endif

  }
  else if(log_ptr->end_params.info_type == CM_CALL_MODE_INFO_NONE)
  {
    /* Fill with N/A
    */
    memset((void *) &log_ptr->end_params.end_params,
           CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
           sizeof(log_ptr->end_params.end_params));
  }
  else
  {
    CM_MSG_HIGH_0("info_type not set!");

    /* Fill with N/A
    */
    memset((void *) &log_ptr->end_params.end_params,
           CM_LOG_PACKET_CONTENTS_NOT_DEFINED,
           sizeof(log_ptr->end_params.end_params));
  }

  log_ptr->sys_mode            = call_info_ptr->sys_mode;
  log_ptr->call_connect_time   = call_info_ptr->call_connect_time;
  log_ptr->call_end_time       = call_info_ptr->call_end_time;
  log_ptr->bytes_received      = call_info_ptr->bytes_received;
  log_ptr->bytes_transmitted   = call_info_ptr->bytes_transmitted;

  log_ptr->line   = call_info_ptr->line;
  log_ptr->requesting_client_id   = call_info_ptr->requesting_client_id;

#if defined(FEATURE_UMTS_1X_HANDOVER_1XMSM) || \
      defined(FEATURE_UMTS_1X_HANDOVER_UMTSMSM)
  #error code not present
#else
  log_ptr->msm_id   = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
#endif

  log_ptr->direction   = call_info_ptr->direction;
  if(call_event == CM_CALL_EVENT_OTASP_STATUS)
  {
    log_ptr->otasp_status = call_info_ptr->mode_info.info.cdma_call.otasp_status;
  }
  else
  {
    log_ptr->otasp_status = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
  }

#ifdef FEATURE_MMODE_DUAL_SIM
  log_ptr->asubs_id       = (uint8)call_info_ptr->asubs_id;
#else
  log_ptr->asubs_id       = CM_LOG_PACKET_CONTENTS_NOT_DEFINED;
#endif

  /* Copy active call info
  */
  log_ptr->active_call_info.info_type       =
    call_info_ptr->mode_info.info_type;
  log_ptr->active_call_info.call_ss_success =
    call_info_ptr->mode_info.info.gw_cs_call.call_ss_success;
  /* Copy MIN() to not overflow log packet buffer
   */
  log_ptr->active_call_info.size        = MIN(call_info_ptr->mode_info.info.gw_cs_call.active_calls_list.size,
                                          ARR_SIZE(log_ptr->active_call_info.table));
  for (i = 0; i < log_ptr->active_call_info.size; i++)
  {
    log_ptr->active_call_info.table[i]      =
      call_info_ptr->mode_info.info.gw_cs_call.active_calls_list.table[i];
  }


  /* Submit the log to DIAG.
  */
  log_util_log_packet_commit( (sys_modem_as_id_e_type)log_ptr->asubs_id, (void *) log_ptr );

  /*lint -restore 641 Converting enum to int */

}

/*===========================================================================
Function: cmlog_plmn_list_event
Description:

       This function sends a diag event when the PLMN NETWORK LIST arrives.

============================================================================*/
void  cmlog_plmn_list_event
(
  sys_detailed_plmn_list_s_type *plmn_list_ptr,
  /* Pointer to a structure containing the list of available PLMNs.
  */
  sys_modem_as_id_e_type         asubs_id
  /* Subscription ID 
  */
)
{
  uint32 i;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(plmn_list_ptr != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for( i=0; i< plmn_list_ptr->length; ++i)
  {
    event_report_payload_ext( EVENT_CM_PLMN_FOUND,
                              sizeof(sys_detailed_plmn_list_info_s_type),
                              &plmn_list_ptr->info[i],
                              EVENT_CM_RPT_EXT_VERSION,
                              &evnt_rpt_subs_id );
  }
}
/*===========================================================================
FUNCTION  cmlog_cmstats_event_handler

Description:

       Generate log packet for given stats event.

===========================================================================*/

void cmlog_cmstats_event_handler
(
  cm_stats_event_e_type evt,
  const sys_modem_stats_info_s_type *stats_info_ptr
)
{

  int                      i            = 0;
  /* Loop index */

  int                      j            = 0;
  /* Loop index */

  unsigned int             log_pkt_len  = 0;
  /* Length of packet to be logged.
  */

  LOG_CM_STATS_EVENT_C_type  *log_ptr     =  NULL;

  log_pkt_len = sizeof(LOG_CM_STATS_EVENT_C_type);

  /* Allocate a log packet.
  */
  log_ptr = (LOG_CM_STATS_EVENT_C_type *) log_alloc_ex( LOG_CM_STATS_EVENT_C,
            log_pkt_len );

  /*
  ** If allocation failed, just return.
  ** It means that either logging of LOG_CM_STATS_EVENT_C is off
  ** or DIAG is truly out of memory (rare)
  */
  if ( log_ptr == NULL )
  {
    CM_MSG_LOW_0("Log packet buffer allocation failed!");
    return;
  }

  log_ptr->version                = CM_LOG_STATS_EVENT_LOG_PACKET_VERSION;

  log_ptr->stats_event            = evt;

  log_ptr->asubs_id               = stats_info_ptr->asubs_id;

  log_ptr->type                   = stats_info_ptr->type;

  switch(stats_info_ptr->type)
  {
    case SYS_MODEM_MODULE_NAS:
      log_ptr->prm.nas_info.changed_bitmask = stats_info_ptr->prm.nas_info.changed_bitmask;
      log_ptr->prm.nas_info.attach_type     = stats_info_ptr->prm.nas_info.attach_type;
      log_ptr->prm.nas_info.cs_reject_cause = stats_info_ptr->prm.nas_info.cs_reject_cause;
      log_ptr->prm.nas_info.ps_reject_cause = stats_info_ptr->prm.nas_info.ps_reject_cause;

      for(i = 0; i < SYS_TMSI_SIZE; i++)
      {
        log_ptr->prm.nas_info.reg_nw_info.tmsi[i]    = stats_info_ptr->prm.nas_info.reg_nw_info.tmsi[i];
        log_ptr->prm.nas_info.reg_nw_info.ptmsi[i]    = stats_info_ptr->prm.nas_info.reg_nw_info.ptmsi[i];
      }

      log_ptr->prm.nas_info.reg_nw_info.gprs_attach_status    = stats_info_ptr->prm.nas_info.reg_nw_info.gprs_attach_status;

      for(i = 0; i < 3; i++)
      {
        log_ptr->prm.nas_info.reg_nw_info.rplmn.identity[i]    = stats_info_ptr->prm.nas_info.reg_nw_info.rplmn.identity[i];
      }


      log_ptr->prm.nas_info.periodic_timers.T3212_value = stats_info_ptr->prm.nas_info.periodic_timers.T3212_value;
      log_ptr->prm.nas_info.periodic_timers.T3312_value = stats_info_ptr->prm.nas_info.periodic_timers.T3312_value;

      log_ptr->prm.nas_info.ps_call_info.pdp_address.valid = stats_info_ptr->prm.nas_info.ps_call_info.pdp_address.valid;
      log_ptr->prm.nas_info.ps_call_info.pdp_address.pdp_type_org = stats_info_ptr->prm.nas_info.ps_call_info.pdp_address.pdp_type_org;
      log_ptr->prm.nas_info.ps_call_info.pdp_address.pdp_type_num = stats_info_ptr->prm.nas_info.ps_call_info.pdp_address.pdp_type_num;
      log_ptr->prm.nas_info.ps_call_info.pdp_address.pdp_addr_len = stats_info_ptr->prm.nas_info.ps_call_info.pdp_address.pdp_addr_len;

      for(i = 0; i < log_ptr->prm.nas_info.ps_call_info.pdp_address.pdp_addr_len; i++)
      {
        log_ptr->prm.nas_info.ps_call_info.pdp_address.address[i] = stats_info_ptr->prm.nas_info.ps_call_info.pdp_address.address[i];
      }

      log_ptr->prm.nas_info.ps_call_info.qos.valid = stats_info_ptr->prm.nas_info.ps_call_info.qos.valid;
      log_ptr->prm.nas_info.ps_call_info.qos.delay_class = stats_info_ptr->prm.nas_info.ps_call_info.qos.delay_class;
      log_ptr->prm.nas_info.ps_call_info.qos.reliability_class = stats_info_ptr->prm.nas_info.ps_call_info.qos.reliability_class;
      log_ptr->prm.nas_info.ps_call_info.qos.peak_throughput = stats_info_ptr->prm.nas_info.ps_call_info.qos.peak_throughput;
      log_ptr->prm.nas_info.ps_call_info.qos.precedence_class = stats_info_ptr->prm.nas_info.ps_call_info.qos.precedence_class;
      log_ptr->prm.nas_info.ps_call_info.qos.mean_throughput = stats_info_ptr->prm.nas_info.ps_call_info.qos.mean_throughput;
      log_ptr->prm.nas_info.ps_call_info.qos.traffic_class = stats_info_ptr->prm.nas_info.ps_call_info.qos.traffic_class;
      log_ptr->prm.nas_info.ps_call_info.qos.deliver_order = stats_info_ptr->prm.nas_info.ps_call_info.qos.deliver_order;
      log_ptr->prm.nas_info.ps_call_info.qos.deliver_err_sdu = stats_info_ptr->prm.nas_info.ps_call_info.qos.deliver_err_sdu;
      log_ptr->prm.nas_info.ps_call_info.qos.max_sdu_size = stats_info_ptr->prm.nas_info.ps_call_info.qos.max_sdu_size;
      log_ptr->prm.nas_info.ps_call_info.qos.max_bit_rare_uplink = stats_info_ptr->prm.nas_info.ps_call_info.qos.max_bit_rare_uplink;
      log_ptr->prm.nas_info.ps_call_info.qos.max_bit_rare_downlink = stats_info_ptr->prm.nas_info.ps_call_info.qos.max_bit_rare_downlink;
      log_ptr->prm.nas_info.ps_call_info.qos.residual_ber = stats_info_ptr->prm.nas_info.ps_call_info.qos.residual_ber;
      log_ptr->prm.nas_info.ps_call_info.qos.sdu_err_ratio = stats_info_ptr->prm.nas_info.ps_call_info.qos.sdu_err_ratio;
      log_ptr->prm.nas_info.ps_call_info.qos.tranfer_delay = stats_info_ptr->prm.nas_info.ps_call_info.qos.tranfer_delay;
      log_ptr->prm.nas_info.ps_call_info.qos.traffic_priority = stats_info_ptr->prm.nas_info.ps_call_info.qos.traffic_priority;
      log_ptr->prm.nas_info.ps_call_info.qos.guarantee_ber_uplink = stats_info_ptr->prm.nas_info.ps_call_info.qos.guarantee_ber_uplink;
      log_ptr->prm.nas_info.ps_call_info.qos.guarantee_ber_dwnlink = stats_info_ptr->prm.nas_info.ps_call_info.qos.guarantee_ber_dwnlink;
      log_ptr->prm.nas_info.ps_call_info.qos.sig_ind = stats_info_ptr->prm.nas_info.ps_call_info.qos.sig_ind;
      log_ptr->prm.nas_info.ps_call_info.qos.src_stat_desc = stats_info_ptr->prm.nas_info.ps_call_info.qos.src_stat_desc;
      log_ptr->prm.nas_info.ps_call_info.qos.ext_max_bit_rate_downlink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext_max_bit_rate_downlink;
      log_ptr->prm.nas_info.ps_call_info.qos.ext_gtd_bit_rate_downlink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext_gtd_bit_rate_downlink;
      log_ptr->prm.nas_info.ps_call_info.qos.ext_max_bit_rate_uplink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext_max_bit_rate_uplink;
      log_ptr->prm.nas_info.ps_call_info.qos.ext_gtd_bit_rate_uplink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext_gtd_bit_rate_uplink;
      log_ptr->prm.nas_info.ps_call_info.qos.ext2_max_bit_rate_downlink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext2_max_bit_rate_downlink;
      log_ptr->prm.nas_info.ps_call_info.qos.ext2_gtd_bit_rate_downlink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext2_gtd_bit_rate_downlink;
      log_ptr->prm.nas_info.ps_call_info.qos.ext2_max_bit_rate_uplink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext2_max_bit_rate_uplink;
      log_ptr->prm.nas_info.ps_call_info.qos.ext2_gtd_bit_rate_uplink = stats_info_ptr->prm.nas_info.ps_call_info.qos.ext2_gtd_bit_rate_uplink;

      log_ptr->prm.nas_info.ps_call_info.apn.valid = stats_info_ptr->prm.nas_info.ps_call_info.apn.valid;
      log_ptr->prm.nas_info.ps_call_info.apn.apn_addr_len = stats_info_ptr->prm.nas_info.ps_call_info.apn.apn_addr_len;

      for(i = 0; i < log_ptr->prm.nas_info.ps_call_info.apn.apn_addr_len; i++)
      {
        log_ptr->prm.nas_info.ps_call_info.apn.address[i] = stats_info_ptr->prm.nas_info.ps_call_info.apn.address[i];
      }

      log_ptr->prm.nas_info.sys_mode     = stats_info_ptr->prm.nas_info.sys_mode;

      log_ptr->prm.nas_info.emm_state     = stats_info_ptr->prm.nas_info.emm_state;
      log_ptr->prm.nas_info.emm_connection_state     = stats_info_ptr->prm.nas_info.emm_connection_state;
      break;

    case SYS_MODEM_MODULE_GSM:
      log_ptr->prm.gsm_info.changed_bitmask = stats_info_ptr->prm.gsm_info.changed_bitmask;
      log_ptr->prm.gsm_info.timing_advance_value = stats_info_ptr->prm.gsm_info.timing_advance_value;

      log_ptr->prm.gsm_info.nmr_info.num_of_items= stats_info_ptr->prm.gsm_info.nmr_info.num_of_items;

      for(i = 0; i < log_ptr->prm.gsm_info.nmr_info.num_of_items; i++)
      {
        log_ptr->prm.gsm_info.nmr_info.nmr[i].arfcn=stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].arfcn;
        log_ptr->prm.gsm_info.nmr_info.nmr[i].bsic =stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].bsic;
        log_ptr->prm.gsm_info.nmr_info.nmr[i].rxlev=stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].rxlev;

        log_ptr->prm.gsm_info.nmr_info.nmr[i].ncell_id.cell_id =stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].ncell_id.cell_id;
        log_ptr->prm.gsm_info.nmr_info.nmr[i].ncell_id.lac =stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].ncell_id.lac;

        for(j = 0; j < 3; j++)
        {
          log_ptr->prm.gsm_info.nmr_info.nmr[i].ncell_id.plmn.identity[j] =stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].ncell_id.plmn.identity[j];
        }


        log_ptr->prm.gsm_info.nmr_info.nmr[i].C1 =stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].C1;
        log_ptr->prm.gsm_info.nmr_info.nmr[i].C2 =stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].C2;
        log_ptr->prm.gsm_info.nmr_info.nmr[i].C31 =stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].C31;
        log_ptr->prm.gsm_info.nmr_info.nmr[i].C32=stats_info_ptr->prm.gsm_info.nmr_info.nmr[i].C32;
      }

      //log_ptr->prm.gsm_info.nmr_info.time_stamp = stats_info_ptr->prm.gsm_info.nmr_info.time_stamp;

      log_ptr->prm.gsm_info.rx_qual = stats_info_ptr->prm.gsm_info.rx_qual;

      for(i = 0; i < SYS_ENG_MODE_TX_PWR_NUM_TIMESLOTS; i++)
      {
        log_ptr->prm.gsm_info.tx_pwr_in_use[i]=stats_info_ptr->prm.gsm_info.tx_pwr_in_use[i];
      }

      log_ptr->prm.gsm_info.scell_info.cell_id = stats_info_ptr->prm.gsm_info.scell_info.cell_id;
      log_ptr->prm.gsm_info.scell_info.lac = stats_info_ptr->prm.gsm_info.scell_info.lac;

      for(i = 0; i < 3; i++)
      {
        log_ptr->prm.gsm_info.scell_info.plmn.identity[i] = stats_info_ptr->prm.gsm_info.scell_info.plmn.identity[i];
      }
      log_ptr->prm.gsm_info.scell_info.arfcn = stats_info_ptr->prm.gsm_info.scell_info.arfcn;
      log_ptr->prm.gsm_info.scell_info.bsic = stats_info_ptr->prm.gsm_info.scell_info.bsic;
      log_ptr->prm.gsm_info.scell_info.cell_info_cause = stats_info_ptr->prm.gsm_info.scell_info.cell_info_cause;
      log_ptr->prm.gsm_info.scell_info.rac = stats_info_ptr->prm.gsm_info.scell_info.rac;

      log_ptr->prm.gsm_info.scell_measurements.C1 = stats_info_ptr->prm.gsm_info.scell_measurements.C1;
      log_ptr->prm.gsm_info.scell_measurements.C2 = stats_info_ptr->prm.gsm_info.scell_measurements.C2;
      log_ptr->prm.gsm_info.scell_measurements.rx_lev = stats_info_ptr->prm.gsm_info.scell_measurements.rx_lev;
      log_ptr->prm.gsm_info.scell_measurements.rx_qual_sub = stats_info_ptr->prm.gsm_info.scell_measurements.rx_qual_sub;
      log_ptr->prm.gsm_info.scell_measurements.channel_type = stats_info_ptr->prm.gsm_info.scell_measurements.channel_type;

      log_ptr->prm.gsm_info.amr_info.ul_chan_mode = stats_info_ptr->prm.gsm_info.amr_info.ul_chan_mode;
      log_ptr->prm.gsm_info.amr_info.dl_chan_mode = stats_info_ptr->prm.gsm_info.amr_info.dl_chan_mode;

      for(i = 0; i < 4; i++)
      {
        log_ptr->prm.gsm_info.amr_info.ul_acs[i] = stats_info_ptr->prm.gsm_info.amr_info.ul_acs[i];
        log_ptr->prm.gsm_info.amr_info.dl_acs[i] = stats_info_ptr->prm.gsm_info.amr_info.dl_acs[i];
      }
      log_ptr->prm.gsm_info.amr_info.dl_dtx = stats_info_ptr->prm.gsm_info.amr_info.dl_dtx;
      log_ptr->prm.gsm_info.amr_info.dl_c_over_i = stats_info_ptr->prm.gsm_info.amr_info.dl_c_over_i;

      log_ptr->prm.gsm_info.dtx_ul_status = stats_info_ptr->prm.gsm_info.dtx_ul_status;

      log_ptr->prm.gsm_info.tbf_info.mac_mode = stats_info_ptr->prm.gsm_info.tbf_info.mac_mode;
      log_ptr->prm.gsm_info.tbf_info.tbf_mode = stats_info_ptr->prm.gsm_info.tbf_info.tbf_mode;

      log_ptr->prm.gsm_info.nw_params.nc_mode = stats_info_ptr->prm.gsm_info.nw_params.nc_mode;
      log_ptr->prm.gsm_info.nw_params.nmo = stats_info_ptr->prm.gsm_info.nw_params.nmo;

      log_ptr->prm.gsm_info.gprs_ul_cs = stats_info_ptr->prm.gsm_info.gprs_ul_cs;
      log_ptr->prm.gsm_info.gprs_dl_cs = stats_info_ptr->prm.gsm_info.gprs_dl_cs;
      log_ptr->prm.gsm_info.egprs_ul_mcs = stats_info_ptr->prm.gsm_info.egprs_ul_mcs;
      log_ptr->prm.gsm_info.egprs_dl_mcs = stats_info_ptr->prm.gsm_info.egprs_dl_mcs;

      log_ptr->prm.gsm_info.edge_meas.lq_meas_mode = stats_info_ptr->prm.gsm_info.edge_meas.lq_meas_mode;
      log_ptr->prm.gsm_info.edge_meas.bep_period = stats_info_ptr->prm.gsm_info.edge_meas.bep_period;

      log_ptr->prm.gsm_info.scell_config.pbcch_present = stats_info_ptr->prm.gsm_info.scell_config.pbcch_present;
      log_ptr->prm.gsm_info.scell_config.gprs_rxlev_access_min = stats_info_ptr->prm.gsm_info.scell_config.gprs_rxlev_access_min;
      log_ptr->prm.gsm_info.scell_config.gprs_ms_txpwr_max_cch = stats_info_ptr->prm.gsm_info.scell_config.gprs_ms_txpwr_max_cch;

      log_ptr->prm.gsm_info.hopping_status.hopping_status = stats_info_ptr->prm.gsm_info.hopping_status.hopping_status;

      log_ptr->prm.gsm_info.g_bcch = stats_info_ptr->prm.gsm_info.g_bcch;
      log_ptr->prm.gsm_info.g_ta = stats_info_ptr->prm.gsm_info.g_ta;

      log_ptr->prm.gsm_info.current_timeslot.timeslot = stats_info_ptr->prm.gsm_info.current_timeslot.timeslot;
      break;


    case SYS_MODEM_MODULE_WCDMA:
      log_ptr->prm.wcdma_info.changed_bitmask = stats_info_ptr->prm.wcdma_info.changed_bitmask;

      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.uarfcn = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.uarfcn;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.psc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.psc;

      for(i = 0; i < 3; i++)
      {
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.plmn_id.identity[i] = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.plmn_id.identity[i];
      }

      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.lac_id = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.lac_id;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.rac_id = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.rac_id;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.ura = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.ura;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.cell_id = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.cell_id;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.nw_operation_mode = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_info.nw_operation_mode;


      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.psc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.psc;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.rscp = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.rscp;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.ecio = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.ecio;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.squal = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.squal;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.srxlev = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_serving_cell_power_info.srxlev;

      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.uarfcn = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.uarfcn;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.psc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.psc;
      for(i = 0; i < 3; i++)
      {
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.plmn_id.identity[i] = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.plmn_id.identity[i];
      }
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.lac_id = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.lac_id;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.rac_id = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.rac_id;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.cell_id = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_reference_rl_info.cell_id;

      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.num_rls = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.num_rls;

      for(i = 0; i < log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.num_rls; i++)
      {
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.psc[i] = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.psc[i];
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.cell_id[i] = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.cell_id[i];
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.rscp[i] = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.rscp[i];
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.ecio[i] = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.ecio[i];
      }
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.sf = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.sf;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.phy_chan_type = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.phy_chan_type;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.slot_format = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.slot_format;
      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.is_compressed_mode_on = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_active_set_info.is_compressed_mode_on;


      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_of_freqs = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_of_freqs;

      for(i = 0; i < log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_of_freqs; i++)
      {
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].uarfcn = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].uarfcn;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].num_of_cells = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].num_of_cells;

        for(j = 0; j < log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].num_of_cells; j++)
        {
          log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].psc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].psc;
          log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].rscp = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].rscp;
          log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].ecio = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].ecio;
          log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].squal = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].squal;
          log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].srxlev = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].srxlev;
          log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].rank = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].rank;
          log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].set = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.freq[i].cell[j].set;
        }
      }

      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_gsm_cells = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_gsm_cells;

      for(i = 0; i < log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_gsm_cells; i++)
      {
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].arfcn = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].arfcn;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].bsic_ncc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].bsic_ncc;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].bsic_bcc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].bsic_bcc;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].rssi = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].rssi;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].bcch = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].bcch;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].rank = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.gsm_cell[i].rank;
      }

      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_lte_cells = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_lte_cells;

      for(i = 0; i < log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.num_lte_cells; i++)
      {
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].earfcn = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].earfcn;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].cell_id = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].cell_id;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].rsrp = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].rsrp;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].rsrq = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].rsrq;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].s_rxlev = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].s_rxlev;
        log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].is_tdd_freq = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_monitored_set_info.lte_cell[i].is_tdd_freq;
      }

      log_ptr->prm.wcdma_info.wcdma_event_info.wcdma_rrc_state = stats_info_ptr->prm.wcdma_info.wcdma_event_info.wcdma_rrc_state;
      log_ptr->prm.wcdma_info.wcdma_event_info.gw_dl_bler = stats_info_ptr->prm.wcdma_info.wcdma_event_info.gw_dl_bler;
      log_ptr->prm.wcdma_info.wcdma_event_info.w_agc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.w_agc;
      log_ptr->prm.wcdma_info.wcdma_event_info.w_txagc = stats_info_ptr->prm.wcdma_info.wcdma_event_info.w_txagc;
      break;

    case SYS_MODEM_MODULE_CDMA:
      log_ptr->prm.cdma_info.changed_bitmask = stats_info_ptr->prm.cdma_info.changed_bitmask;

      log_ptr->prm.cdma_info.state_info.cdma_prot_state = stats_info_ptr->prm.cdma_info.state_info.cdma_prot_state;
      log_ptr->prm.cdma_info.state_info.band_class = stats_info_ptr->prm.cdma_info.state_info.band_class;
      log_ptr->prm.cdma_info.state_info.slotted_mode = stats_info_ptr->prm.cdma_info.state_info.slotted_mode;
      log_ptr->prm.cdma_info.state_info.pch_rate = stats_info_ptr->prm.cdma_info.state_info.pch_rate;
      log_ptr->prm.cdma_info.state_info.active_so = stats_info_ptr->prm.cdma_info.state_info.active_so;

      log_ptr->prm.cdma_info.state_info.min.min1 = stats_info_ptr->prm.cdma_info.state_info.min.min1;
      log_ptr->prm.cdma_info.state_info.min.min2 = stats_info_ptr->prm.cdma_info.state_info.min.min2;

      log_ptr->prm.cdma_info.state_info.ms_pref_sci = stats_info_ptr->prm.cdma_info.state_info.ms_pref_sci;
      log_ptr->prm.cdma_info.state_info.qpch_status = stats_info_ptr->prm.cdma_info.state_info.qpch_status;
      log_ptr->prm.cdma_info.state_info.num_active_pilots = stats_info_ptr->prm.cdma_info.state_info.num_active_pilots;

      log_ptr->prm.cdma_info.meas_info.rx_pwr = stats_info_ptr->prm.cdma_info.meas_info.rx_pwr;
      log_ptr->prm.cdma_info.meas_info.tx_pwr = stats_info_ptr->prm.cdma_info.meas_info.tx_pwr;
      log_ptr->prm.cdma_info.meas_info.pch_loss_count = stats_info_ptr->prm.cdma_info.meas_info.pch_loss_count;
      log_ptr->prm.cdma_info.meas_info.tch_loss_count = stats_info_ptr->prm.cdma_info.meas_info.tch_loss_count;
      log_ptr->prm.cdma_info.meas_info.idle_ho_count = stats_info_ptr->prm.cdma_info.meas_info.idle_ho_count;
      log_ptr->prm.cdma_info.meas_info.acc_req_count = stats_info_ptr->prm.cdma_info.meas_info.acc_req_count;
      log_ptr->prm.cdma_info.meas_info.acc_rsp_count = stats_info_ptr->prm.cdma_info.meas_info.acc_rsp_count;
      log_ptr->prm.cdma_info.meas_info.failed_acc_att_count = stats_info_ptr->prm.cdma_info.meas_info.failed_acc_att_count;
      log_ptr->prm.cdma_info.meas_info.t41_expiry_count = stats_info_ptr->prm.cdma_info.meas_info.t41_expiry_count;

      for(i = 0; i < SYS_CDMA_MAX_ASET_PILOTS; i++)
      {
        log_ptr->prm.cdma_info.active_set_info.pilot_info[i].pn = stats_info_ptr->prm.cdma_info.active_set_info.pilot_info[i].pn;
        log_ptr->prm.cdma_info.active_set_info.pilot_info[i].ecio = stats_info_ptr->prm.cdma_info.active_set_info.pilot_info[i].ecio;
      }

      log_ptr->prm.cdma_info.serv_sys_info.pilot_pn_offset = stats_info_ptr->prm.cdma_info.serv_sys_info.pilot_pn_offset;
      log_ptr->prm.cdma_info.serv_sys_info.channel = stats_info_ptr->prm.cdma_info.serv_sys_info.channel;
      log_ptr->prm.cdma_info.serv_sys_info.pagech = stats_info_ptr->prm.cdma_info.serv_sys_info.pagech;
      log_ptr->prm.cdma_info.serv_sys_info.base_id = stats_info_ptr->prm.cdma_info.serv_sys_info.base_id;
      log_ptr->prm.cdma_info.serv_sys_info.sid = stats_info_ptr->prm.cdma_info.serv_sys_info.sid;
      log_ptr->prm.cdma_info.serv_sys_info.nid = stats_info_ptr->prm.cdma_info.serv_sys_info.nid;
      log_ptr->prm.cdma_info.serv_sys_info.packet_zone_id = stats_info_ptr->prm.cdma_info.serv_sys_info.packet_zone_id;
      log_ptr->prm.cdma_info.serv_sys_info.p_rev = stats_info_ptr->prm.cdma_info.serv_sys_info.p_rev;
      log_ptr->prm.cdma_info.serv_sys_info.min_p_rev = stats_info_ptr->prm.cdma_info.serv_sys_info.min_p_rev;
      log_ptr->prm.cdma_info.serv_sys_info.nw_pref_sci = stats_info_ptr->prm.cdma_info.serv_sys_info.nw_pref_sci;

      log_ptr->prm.cdma_info.channel_config_info.fsch0_mux = stats_info_ptr->prm.cdma_info.channel_config_info.fsch0_mux;
      log_ptr->prm.cdma_info.channel_config_info.fsch0_rc = stats_info_ptr->prm.cdma_info.channel_config_info.fsch0_rc;
      log_ptr->prm.cdma_info.channel_config_info.fsch0_turbo_supported = stats_info_ptr->prm.cdma_info.channel_config_info.fsch0_turbo_supported;
      log_ptr->prm.cdma_info.channel_config_info.rsch0_mux = stats_info_ptr->prm.cdma_info.channel_config_info.rsch0_mux;
      log_ptr->prm.cdma_info.channel_config_info.rsch0_rc= stats_info_ptr->prm.cdma_info.channel_config_info.rsch0_rc;
      log_ptr->prm.cdma_info.channel_config_info.rsch0_turbo_supported = stats_info_ptr->prm.cdma_info.channel_config_info.rsch0_turbo_supported;
      log_ptr->prm.cdma_info.channel_config_info.fsch0_active = stats_info_ptr->prm.cdma_info.channel_config_info.fsch0_active;
      log_ptr->prm.cdma_info.channel_config_info.fsch0_duration = stats_info_ptr->prm.cdma_info.channel_config_info.fsch0_duration;
      log_ptr->prm.cdma_info.channel_config_info.fsch0_sccl_index = stats_info_ptr->prm.cdma_info.channel_config_info.fsch0_sccl_index;
      log_ptr->prm.cdma_info.channel_config_info.fsch0_num_pilots = stats_info_ptr->prm.cdma_info.channel_config_info.fsch0_num_pilots;
      log_ptr->prm.cdma_info.channel_config_info.rsch0_active = stats_info_ptr->prm.cdma_info.channel_config_info.rsch0_active;
      log_ptr->prm.cdma_info.channel_config_info.rsch0_duration = stats_info_ptr->prm.cdma_info.channel_config_info.rsch0_duration;
      log_ptr->prm.cdma_info.channel_config_info.scrm_seq_num = stats_info_ptr->prm.cdma_info.channel_config_info.scrm_seq_num;

      log_ptr->prm.cdma_info.call_stat.orig_acc_att_count = stats_info_ptr->prm.cdma_info.call_stat.orig_acc_att_count;
      log_ptr->prm.cdma_info.call_stat.page_rsp_acc_att_count = stats_info_ptr->prm.cdma_info.call_stat.page_rsp_acc_att_count;

      log_ptr->prm.cdma_info.reg_stat.powerdown_reg_count = stats_info_ptr->prm.cdma_info.reg_stat.powerdown_reg_count;
      log_ptr->prm.cdma_info.reg_stat.timer_reg_count = stats_info_ptr->prm.cdma_info.reg_stat.timer_reg_count;
      log_ptr->prm.cdma_info.reg_stat.parameter_reg_count = stats_info_ptr->prm.cdma_info.reg_stat.parameter_reg_count;
      log_ptr->prm.cdma_info.reg_stat.powerup_reg_count = stats_info_ptr->prm.cdma_info.reg_stat.powerup_reg_count;
      log_ptr->prm.cdma_info.reg_stat.ordered_reg_count = stats_info_ptr->prm.cdma_info.reg_stat.ordered_reg_count;
      log_ptr->prm.cdma_info.reg_stat.zone_reg_count = stats_info_ptr->prm.cdma_info.reg_stat.zone_reg_count;
      log_ptr->prm.cdma_info.reg_stat.distance_reg_count = stats_info_ptr->prm.cdma_info.reg_stat.distance_reg_count;

      log_ptr->prm.cdma_info.cdma_1x_rx0_agc = stats_info_ptr->prm.cdma_info.cdma_1x_rx0_agc;
      log_ptr->prm.cdma_info.cdma_1x_rx1_agc = stats_info_ptr->prm.cdma_info.cdma_1x_rx1_agc;
      break;

    case SYS_MODEM_MODULE_HDR:
      log_ptr->prm.hdr_info.changed_bitmask = stats_info_ptr->prm.hdr_info.changed_bitmask;
      log_ptr->prm.hdr_info.hdr_prot_state = stats_info_ptr->prm.hdr_info.hdr_prot_state;

      log_ptr->prm.hdr_info.channel.band = stats_info_ptr->prm.hdr_info.channel.band;
      log_ptr->prm.hdr_info.channel.chan_num = stats_info_ptr->prm.hdr_info.channel.chan_num;

      log_ptr->prm.hdr_info.c2i = stats_info_ptr->prm.hdr_info.c2i;
      log_ptr->prm.hdr_info.rx_dbm = stats_info_ptr->prm.hdr_info.rx_dbm;
      log_ptr->prm.hdr_info.tx_dbm = stats_info_ptr->prm.hdr_info.tx_dbm;
      log_ptr->prm.hdr_info.ecio = stats_info_ptr->prm.hdr_info.ecio;
      log_ptr->prm.hdr_info.uati_24 = stats_info_ptr->prm.hdr_info.uati_24;
      log_ptr->prm.hdr_info.color_code = stats_info_ptr->prm.hdr_info.color_code;
      log_ptr->prm.hdr_info.subnet_mask = stats_info_ptr->prm.hdr_info.subnet_mask;
      log_ptr->prm.hdr_info.mac_index = stats_info_ptr->prm.hdr_info.mac_index;
      log_ptr->prm.hdr_info.cdma_evdo_rx0_agc = stats_info_ptr->prm.hdr_info.cdma_evdo_rx0_agc;
      log_ptr->prm.hdr_info.cdma_evdo_rx1_agc = stats_info_ptr->prm.hdr_info.cdma_evdo_rx1_agc;
      break;

    case SYS_MODEM_MODULE_CM:
      log_ptr->prm.cm_info.changed_bitmask = stats_info_ptr->prm.cm_info.changed_bitmask;

      log_ptr->prm.cm_info.ph_info.pref_sys = stats_info_ptr->prm.cm_info.ph_info.pref_sys;
      log_ptr->prm.cm_info.ph_info.mode_pref = stats_info_ptr->prm.cm_info.ph_info.mode_pref;
      log_ptr->prm.cm_info.ph_info.band_cap = stats_info_ptr->prm.cm_info.ph_info.band_cap;

      log_ptr->prm.cm_info.sys_sel_info.sel_sys.id_type = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id_type;

      switch(log_ptr->prm.cm_info.sys_sel_info.sel_sys.id_type)
      {
        case SYS_SYS_ID_TYPE_UMTS:
          for(i = 0; i < 3; i++)
          {
            log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.plmn.identity[i] = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.plmn.identity[i];
            log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.plmn_lac.plmn.identity[i] = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.plmn_lac.plmn.identity[i];
          }
          log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.plmn_lac.lac = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.plmn_lac.lac;

          break;

        case SYS_SYS_ID_TYPE_IS95:
          log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.sid = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.sid;
          log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.nid = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.nid;
          log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.mcc = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.mcc;
          log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.imsi_11_12 = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is95.imsi_11_12;

          break;

        case SYS_SYS_ID_TYPE_IS856:
          for(i = 0; i < 16; i++)
          {
            log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is856[i] = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.is856[i];
          }

          break;

        case SYS_SYS_ID_TYPE_WLAN:
          log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.ssid.len = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.ssid.len;

          for(i = 0; i < log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.ssid.len; i++)
          {
            log_ptr->prm.cm_info.sys_sel_info.sel_sys.id.ssid.ssid[i] = stats_info_ptr->prm.cm_info.sys_sel_info.sel_sys.id.ssid.ssid[i];
          }

          break;

        default:
          break;

      }


      log_ptr->prm.cm_info.sys_sel_info.roam_ind = stats_info_ptr->prm.cm_info.sys_sel_info.roam_ind;
      log_ptr->prm.cm_info.prl_ver = stats_info_ptr->prm.cm_info.prl_ver;
      log_ptr->prm.cm_info.send_key_orig_count = stats_info_ptr->prm.cm_info.send_key_orig_count;
      break;

    case SYS_MODEM_MODULE_DS:
      log_ptr->prm.ds_info.changed_bitmask = stats_info_ptr->prm.ds_info.changed_bitmask;
      log_ptr->prm.ds_info.ds_hdr_link_info.hdr_fwd_data_rate = stats_info_ptr->prm.ds_info.ds_hdr_link_info.hdr_fwd_data_rate;
      log_ptr->prm.ds_info.ds_hdr_link_info.hdr_rev_data_rate = stats_info_ptr->prm.ds_info.ds_hdr_link_info.hdr_rev_data_rate;
      break;

    case SYS_MODEM_MODULE_LTE:
      log_ptr->prm.lte_info.changed_bitmask = stats_info_ptr->prm.lte_info.changed_bitmask;

      log_ptr->prm.lte_info.lte_meas_info.is_service_available = stats_info_ptr->prm.lte_info.lte_meas_info.is_service_available;
      log_ptr->prm.lte_info.lte_meas_info.sys_mode = stats_info_ptr->prm.lte_info.lte_meas_info.sys_mode;

      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.valid = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.valid;
      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.idle = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.idle;

      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.earfcn = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.earfcn;
      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.serving_cell_id = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.serving_cell_id;
      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.num_lte_cells = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.num_lte_cells;


      for(i = 0; i < log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.num_lte_cells; i++)
      {
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].pci = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].pci;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].rsrq = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].rsrq;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].rsrp = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].rsrp;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].rssi = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].rssi;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].idle.srxlev = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.cell[i].idle.srxlev;
      }

      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.cell_resel_priority = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.cell_resel_priority;
      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.s_non_intra_search = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.s_non_intra_search;
      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.thresh_serving_low = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.thresh_serving_low;
      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.s_intra_search = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_intra.idle.s_intra_search;


      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.num_freqs = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.num_freqs;

	for(i = 0; i < log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.num_freqs; i++)
      {

        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].earfcn = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].earfcn;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].num_lte_cells = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].num_lte_cells;


        for(j = 0; j < log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].num_lte_cells; j++)
        {
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].pci = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].pci;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].rsrq = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].rsrq;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].rsrp = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].rsrp;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].rssi = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].rssi;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].idle.srxlev = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].cell[j].idle.srxlev;
        }

        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].idle.threshX_low = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].idle.threshX_low;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].idle.threshX_high = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].idle.threshX_high;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].idle.cell_resel_priority = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.lte_inter.freqs[i].idle.cell_resel_priority;

      }



      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.num_freq_groups = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.num_freq_groups;

      for(i = 0; i < log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.num_freq_groups; i++)
      {
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].num_gsm_arfcn = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].num_gsm_arfcn;

        for(j = 0; j < log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].num_gsm_arfcn; j++)
        {
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].arfcn = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].arfcn;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].band_1900 = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].band_1900;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].cell_id_valid = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].cell_id_valid;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].bsic_id = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].bsic_id;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].rssi = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].rssi;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].idle.srxlev = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].arfcn[j].idle.srxlev;
        }

        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.cell_resel_priority = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.cell_resel_priority;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.thresh_gsm_high = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.thresh_gsm_high;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.thresh_gsm_low = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.thresh_gsm_low;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.ncc_permitted = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.gsm.freq_group[i].idle.ncc_permitted;

      }

      log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.num_wcdma_freqs = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.num_wcdma_freqs;

      for(i = 0; i < log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.num_wcdma_freqs; i++)
      {
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].uarfcn = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].uarfcn;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].num_wcdma_cells = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].num_wcdma_cells;

        for(j = 0; j < log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].num_wcdma_cells; j++)
        {
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].psc = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].psc;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].cpich_rscp = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].cpich_rscp;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].cpich_ecno = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].cpich_ecno;
          log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].idle.srxlev = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].cell[j].idle.srxlev;
        }

        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].idle.cell_resel_priority = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].idle.cell_resel_priority;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].idle.thresh_Xhigh = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].idle.thresh_Xhigh;
        log_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].idle.thresh_Xlow = stats_info_ptr->prm.lte_info.lte_meas_info.lte_nbr_info.wcdma.freq[i].idle.thresh_Xlow;

      }
      break;

    default:
      break;

  }

  /* Submit the log to DIAG.
  */
  log_util_log_packet_commit( (sys_modem_as_id_e_type)log_ptr->asubs_id, (void *) log_ptr );

}

#ifdef FEATURE_MMODE_DUAL_SIM
/*===========================================================================
Function: cmlog_plmn_list_event_per_subs
Description:

       This function sends a diag event when the PLMN NETWORK LIST arrives.

============================================================================*/
void  cmlog_plmn_list_event_per_subs
(
  const sys_detailed_plmn_list_s_type *plmn_list_ptr,
  /* Pointer to a structure containing the list of available PLMNs.
  */

  sys_modem_as_id_e_type          as_id
)
{
  uint32        i;
  uint8         *plmn;
  size_t        size_plmn_arr;
  uint32        evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(as_id);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(plmn_list_ptr != NULL);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  size_plmn_arr = (sizeof(sys_detailed_plmn_list_info_s_type) + sizeof(uint8));
  plmn = cm_mem_malloc(size_plmn_arr);

  CM_ASSERT(NULL != plmn);
  if (plmn == NULL) return;

  *plmn = (uint8)as_id;

  for( i=0; i< plmn_list_ptr->length; ++i)
  {
    /* First byte of plmn is used for as_id, hence 1 is subtracted from destination size  */
    memscpy( plmn+1, size_plmn_arr-1, &plmn_list_ptr->info[i], sizeof(sys_detailed_plmn_list_info_s_type) );

    event_report_payload_ext( EVENT_CM_DS_PLMN_FOUND,
                              sizeof(sys_detailed_plmn_list_info_s_type) + sizeof(uint8),
                              plmn,
                              EVENT_CM_RPT_EXT_VERSION, 
                              &evnt_rpt_subs_id );
  }

  if(NULL != plmn)
  {
    cm_mem_free(plmn);
    plmn = NULL;
  }
}

/*===========================================================================
Function: cmlog_get_max_buff_size
Description:

       This function returns the max allowed buffer size based on the size of
       the payload (which is passed as argument)

============================================================================*/
static byte cmlog_get_max_buff_size(byte a)
{
  return ( (a > 32) ? (32) : (a) );
}

#endif /* FEATURE_MMODE_DUAL_SIM */
/*===========================================================================
Function: cmlog_service_cnf_event
Description:

       This function sends a diag event when a SERVICE CONFIRMATION arrives.

============================================================================*/
void  cmlog_service_cnf_event
(
  sys_plmn_service_state_s_type   *srv_info_ptr,
  /* Pointer to the service state info received in the report.
  */

  sys_modem_as_id_e_type           as_id
  /* Sub on which the service confirmation is recieved
  */
)
{
  byte                        payload_size;

#ifdef FEATURE_MMODE_DUAL_SIM
  uint8  srv_info[13];

  cmph_s_type                 *ph_ptr   = cmph_ptr();
#endif

  uint32                      evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(as_id);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  CM_ASSERT(srv_info_ptr != NULL);
  if(!cmutil_is_as_id_valid(as_id))
  {
    as_id = SYS_MODEM_AS_ID_1;
  }
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Store the current service information in diag client */
  diag_client.sub_info[as_id].srv_status = srv_info_ptr->service_status;

#ifdef FEATURE_MMODE_DUAL_SIM
  if( ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_DUAL_STANDBY
#ifdef FEATURE_MMODE_TRIPLE_SIM
      || ( ph_ptr->device_prop.msim_prop.internal_standby_pref == SYS_MODEM_DS_PREF_TRIPLE_STANDBY)
#endif
    )
  {
    srv_info[0] = (uint8)as_id;
    srv_info[1] = (uint8)srv_info_ptr->sim_state;
    memscpy( (void *)&(srv_info[2]), ((ARR_SIZE(srv_info) - 2)*sizeof(srv_info[0])), (void *)srv_info_ptr->plmn.identity, 3 );
    srv_info[5] = (uint8)srv_info_ptr->plmn_service_capability;
    srv_info[6] = srv_info_ptr->plmn_forbidden;
    memscpy( (void *)&(srv_info[7]), ((ARR_SIZE(srv_info) - 7)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->roaming_ind), 2 );
    srv_info[9] = (uint8)srv_info_ptr->service_status;
    srv_info[10] = (uint8)srv_info_ptr->reg_service_domain;
    srv_info[11] = (uint8)srv_info_ptr->active_rat;
    srv_info[12] = srv_info_ptr->ps_data_suspend;

    payload_size = cmlog_get_max_buff_size(sizeof(srv_info));

    event_report_payload_ext( EVENT_CM_DS_SERVICE_CONFIRMED,
                              payload_size,
                              srv_info,
                              EVENT_CM_RPT_EXT_VERSION,
                              &evnt_rpt_subs_id );
  }
  else
#endif
  {
    uint8  srv_info[100] = {0};

    srv_info[0] = (uint8)srv_info_ptr->sim_state;
    memscpy( (void *)&(srv_info[1]), ((ARR_SIZE(srv_info) - 1)*sizeof(srv_info[0])), (void *)srv_info_ptr->plmn.identity, 3 );
    srv_info[4] = (uint8)srv_info_ptr->plmn_service_capability;
    srv_info[5] = srv_info_ptr->plmn_forbidden;
    memscpy( (void *)&(srv_info[6]), ((ARR_SIZE(srv_info) - 6)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->roaming_ind), 2 );
    srv_info[8] = (uint8)srv_info_ptr->service_status;
    srv_info[9] = (uint8)srv_info_ptr->reg_service_domain;
    srv_info[10] = (uint8)srv_info_ptr->active_rat;
    memscpy( (void *)&(srv_info[11]), ((ARR_SIZE(srv_info) - 11)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->active_band), 8 );
    memscpy( (void *)&(srv_info[19]), ((ARR_SIZE(srv_info) - 19)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->lac), 2 );
    memscpy( (void *)&(srv_info[21]), ((ARR_SIZE(srv_info) - 21)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->tac), 2 );
    srv_info[23] = (uint8)srv_info_ptr->ps_data_suspend;

    memscpy( (void *)&(srv_info[24]), ((ARR_SIZE(srv_info) - 24)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->csg_info.csg_id), 4 );
    srv_info[28] = (uint8)srv_info_ptr->csg_info.hnb_name.length;
    memscpy( (void *)&(srv_info[29]), ((ARR_SIZE(srv_info) - 29)*sizeof(srv_info[0])), (void *)srv_info_ptr->csg_info.hnb_name.name, 48 );
    srv_info[77] = (uint8)srv_info_ptr->csg_in_white_list;

    if(srv_info_ptr->csg_info.csg_id != SYS_CSG_ID_INVALID)
    {
      CM_MSG_HIGH_2 ("csg_id(%d),csg_in_white_list(%d)", srv_info_ptr->csg_info.csg_id,
                     srv_info_ptr->csg_in_white_list);
    }

    srv_info[78] = (uint8)srv_info_ptr->cs_service_status;
    srv_info[79] = (uint8)srv_info_ptr->no_service_cause;
    memscpy( (void *)&(srv_info[80]), ((ARR_SIZE(srv_info) - 80)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->suitable_search_time ), 4 );
    srv_info[84] = (uint8)srv_info_ptr->rac_or_mme_code;
    srv_info[85] = (uint8)srv_info_ptr->sib8_available;
    srv_info[86] = (uint8)srv_info_ptr->lte_cs_capability;
    srv_info[87] = (uint8)srv_info_ptr->acq_status;
    memscpy( (void *)&(srv_info[88]), ((ARR_SIZE(srv_info) - 88)*sizeof(srv_info[0])), (void *)&(srv_info_ptr->eps_nw_feature_support ), 4 );
    srv_info[92] = (uint8)srv_info_ptr->extend_srv_info;
    srv_info[93] = (uint8)srv_info_ptr->plmn_reg_type;
    memscpy( (void *)&(srv_info[94]), ((ARR_SIZE(srv_info) - 94)*sizeof(srv_info[0])), (void *)srv_info_ptr->registered_plmn.identity, 3 );
    srv_info[97] = (uint8)srv_info_ptr->emc_attached;

    payload_size = sizeof(srv_info);
    event_report_payload_ext( EVENT_CM_SERVICE_CONFIRMED,
                              payload_size,
                              srv_info,
                              EVENT_CM_RPT_EXT_VERSION,
                              &evnt_rpt_subs_id );
  }

}

#ifdef CM_GW_SUPPORTED

/*===========================================================================
Function: cmlog_cm_send_call_information_log
Description:

       This function sends log packets containing call info.
============================================================================*/
void cmlog_cm_send_call_information_log(void)
{
  uint8 loop = 0;
  uint8 log_ptr_index = 0; /* indexes into log_ptr array */
  uint8 log_size = 0;
  byte *log_ptr = NULL;
  uint8 call_cnt = 0;

  /* Send a log packet that contains details of all the call ids */

  /* Log packet carries
  * number_of_calls (1 byte)
  * and call_id (1 byte), call_state (1 byte), call_type (1 byte) for each call
  */
  log_size = sizeof(log_hdr_type) + sizeof(uint8) + CM_DIAG_CALL_ID_MAX *
             (sizeof(uint8) + sizeof(uint8) + sizeof(uint8));

  /* Associated data is sent as log record */
  log_ptr = (byte *) log_alloc (LOG_GET_CALL_INFO, log_size);

  if (log_ptr == NULL)
  {
    CM_MSG_HIGH_0 ("log_alloc failed \n");
  }

  log_ptr_index = sizeof(log_hdr_type) + 1;

  /* Fill in the call information */
  for (loop=0; loop<CMCALL_MAX_CALL_OBJ; ++loop)
  {
    if (call_state_list[loop].call_id != CM_CALL_ID_INVALID)
    {
      call_cnt++;

      if ((call_cnt <= CM_DIAG_CALL_ID_MAX) && (log_ptr != NULL))
      {
        /* call id */
        log_ptr[log_ptr_index] = call_state_list[loop].call_id;

        /* call type */
        log_ptr[log_ptr_index + 1] = (byte) call_state_list[loop].call_type;

        /* call state */
        log_ptr[log_ptr_index + 2] = (byte) call_state_list[loop].call_state;

        log_ptr_index = log_ptr_index + 3;
      }
    }
  } /* for */

  if (log_ptr != NULL)
  {
    if (call_cnt > CM_DIAG_CALL_ID_MAX)
    {
      log_ptr [sizeof(log_hdr_type)] = CM_DIAG_CALL_ID_MAX;

      CM_MSG_HIGH_2 (" Reporting only %d out of %d calls in LOG_GET_CALL_INFO",
                     CM_DIAG_CALL_ID_MAX, call_cnt);
    }
    else
    {
      log_ptr [sizeof(log_hdr_type)] = call_cnt;
    }

    log_util_log_packet_commit (SYS_MODEM_AS_ID_NONE, log_ptr);
  }

}

/*===========================================================================
Function: cmlog_cm_sups_event_cb
Description:
       SUPS event callback function, called by CM.
============================================================================*/
/*lint -esym(765, cmlog_cm_sups_event_cb)
** Can't be static, it's a callback
*/
void cmlog_cm_sups_event_cb(

  cm_sups_event_e_type          sups_event,

  const cm_sups_info_s_type     *sups_info_ptr
)
{
  uint8       event_result[4];
  byte        num_chars = 0;
  byte        uss_ascii_text[MAX_USS_CHAR_ASCII +1]; /* Holds USS data converted to ascii */
  byte        *log_ptr = NULL;
  int         log_size;
  uint32      evnt_rpt_subs_id;

#ifdef FEATURE_MMODE_DUAL_SIM
  cmph_s_type *ph_ptr = cmph_ptr();
#endif

  /* verify that there is an associated event information */
  if (sups_info_ptr == NULL)
  {
    CM_ERR_0 ("sups_info is NULL \n");
    return;
  }

  evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(sups_info_ptr->asubs_id);
  CM_MSG_HIGH_1 ("SUPS event received by diag function (%d)\n", sups_event);

  /* Check for events we are interested in */
  switch (sups_event)
  {
#ifdef CM_GW_SUPPORTED
    /* Event to show password prompt from the network */
    case CM_SUPS_EVENT_GET_PASSWORD_IND:
#ifdef FEATURE_MMODE_DUAL_SIM
      if( cmlog_is_dual_standby(ph_ptr->device_prop.msim_prop.standby_pref ) )
      {
        event_result[0] = (uint8)sups_info_ptr->asubs_id;
        event_result[1] = sups_info_ptr->invoke_id;
        event_result[2] = (uint8)(sups_info_ptr->guidance_info);

        CM_MSG_HIGH_2 ("Get password event invoke_id=%d, guidance_info=%d\n",
                       sups_info_ptr->invoke_id, sups_info_ptr->guidance_info);

        event_report_payload_ext (EVENT_CM_DS_GET_PASSWORD_IND, 3, event_result,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }
      else
#endif /* FEATURE_MMODE_DUAL_SIM */
      {
        event_result[0] = sups_info_ptr->invoke_id;
        event_result[1] = (uint8)(sups_info_ptr->guidance_info);

        CM_MSG_HIGH_2 ("Get password event invoke_id=%d, guidance_info=%d\n",
                       sups_info_ptr->invoke_id, sups_info_ptr->guidance_info);

        event_report_payload_ext (EVENT_CM_GET_PASSWORD_IND, 2, event_result,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }
      break;

    /* Events that may carry password confirmation from the network */
    case CM_SUPS_EVENT_REG_PASSWORD_CONF:
    case CM_SUPS_EVENT_INTERROGATE_CONF:
    case CM_SUPS_EVENT_REGISTER_CONF:
    case CM_SUPS_EVENT_ACTIVATE_CONF:
    case CM_SUPS_EVENT_ERASE_CONF:
    case CM_SUPS_EVENT_DEACTIVATE_CONF:
    case CM_SUPS_EVENT_RELEASE_USS_IND:
      if ( sups_info_ptr->ss_success &&
           sups_info_ptr->conf_type == SS_ERROR_INFO &&
           sups_info_ptr->ss_error.error_code_tag == ERROR_CODE_TAG &&
           sups_info_ptr->ss_error.error_code == negativePasswordCheck
         )
      {
#ifdef FEATURE_MMODE_DUAL_SIM
        if( cmlog_is_dual_standby(ph_ptr->device_prop.msim_prop.standby_pref ))
        {
          event_result[0] = (uint8)sups_info_ptr->asubs_id;
          event_result[1] = sups_info_ptr->invoke_id;
          event_result[2] = 0; /* Authentication failed */

          CM_MSG_HIGH_1 ("Negative password event invoke_id = %d\n",
                         sups_info_ptr->invoke_id);
          event_report_payload_ext (EVENT_CM_DS_PASSWORD_AUTHENTICATION_STATUS, 3, event_result,
                                    EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
        }
        else
#endif /* FEATURE_MMODE_DUAL_SIM */
        {
          event_result[0] = sups_info_ptr->invoke_id;
          event_result[1] = 0; /* Authentication failed */

          CM_MSG_HIGH_1 ("Negative password event invoke_id = %d\n", sups_info_ptr->invoke_id);
          event_report_payload_ext (EVENT_CM_PASSWORD_AUTHENTICATION_STATUS, 2, event_result,
                                    EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
        }
      } /* if (sups_info_ptr->ss_success) */

      break;

    /* Received USS notify indication, send an event to diag */
    case CM_SUPS_EVENT_USS_NOTIFY_IND:
    /* Received USS indication requesting for response , send an event to diag */
    case CM_SUPS_EVENT_USS_IND:
    {
      int i   = 0;
      size_t num_converted = 0;
#ifdef FEATURE_MMODE_DUAL_SIM
      if( cmlog_is_dual_standby(ph_ptr->device_prop.msim_prop.standby_pref ) )
      {
        event_result[i] = (uint8)sups_info_ptr->asubs_id;
        i++;
      }
#endif

      event_result[i++] = sups_info_ptr->invoke_id;

      /* Check if data coding scheme is the default one*/
      if ((sups_info_ptr->uss_data.uss_data_coding_scheme & ALPHABET_MASK ) ==
          DEFAULT_ALPHABET)
      {
        /* data coding scheme is default_alphabet
        */
        event_result [i++] = 1;

        (void) cm_util_gsm7_to_iso8859(
          (byte *) sups_info_ptr->uss_data.ussData,
          (byte) sups_info_ptr->uss_data.size,
          FALSE,
          uss_ascii_text,
          sizeof(uss_ascii_text),
          &num_converted
        );

        num_chars = (byte)num_converted;

        if (num_chars > MAX_DIAG_USS_CHAR)
        {
          CM_ERR_0 (" num_chars exceeds MAX_DIAG_USS_CHAR \n");
          return;
        }

        /* Size of log packet to be allocated */
        log_size = sizeof(log_hdr_type) + CM_SIZE_RESPONSE_NOTIFY_HEADER + num_chars;

        /* Associated data is sent as log record */
        log_ptr = (byte *) log_alloc (LOG_UMTS_RESPONSE_NOTIFY,
                                      (unsigned int)log_size);

        if (log_ptr != NULL)
        {
          log_ptr [CM_LOG_DATA_OFFSET] = 1; /* 1 == USS Notify indication */
          log_ptr [CM_RESPONSE_NOTIFY_DATA_LENGTH_OFFSET] = num_chars;

          if (num_chars <= MAX_DIAG_USS_CHAR)
          {
            memscpy( &log_ptr[CM_RESPONSE_NOTIFY_DATA_OFFSET],
                     (size_t)(log_size - (CM_RESPONSE_NOTIFY_DATA_OFFSET * sizeof(log_ptr[0]))),
                     uss_ascii_text,
                     num_chars );
          }
          else
          {
            CM_MSG_HIGH_1 ("num_chars for USSD data exceeds %d \n",
                           MAX_DIAG_USS_CHAR);
          }
        }
        else
        {
          CM_MSG_HIGH_0 ("log_alloc failed \n");
        }

        if (log_ptr != NULL)
        {
          CM_MSG_HIGH_0 ("Committing log for USSD notify \n");
#ifdef FEATURE_MMODE_DUAL_SIM
          log_util_log_packet_commit ((sys_modem_as_id_e_type)sups_info_ptr->asubs_id, log_ptr);
#else
          log_util_log_packet_commit (SYS_MODEM_AS_ID_1, log_ptr);
#endif
        }
      }
      else
      {
        /* data coding scheme is not understood
        */
        event_result[i++] = 0;
      }
      event_result[i++] = 1; /* 1 denotes USS Notify indication */

#ifdef FEATURE_MMODE_DUAL_SIM
      if( cmlog_is_dual_standby(ph_ptr->device_prop.msim_prop.standby_pref ) )
      {
        CM_MSG_HIGH_2 ("USS notify indication invoke_id = %d, ussd_type = %d \n",
                       event_result[1], event_result[3]);
        event_report_payload_ext (EVENT_CM_DS_USS_RESPONSE_NOTIFY_IND, 4, event_result,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }
      else
#endif /* FEATURE_MMODE_DUAL_SIM */
      {
        CM_MSG_HIGH_2 ("USS notify indication invoke_id = %d, ussd_type = %d \n",
                       event_result[0], event_result[2]);
        event_report_payload_ext (EVENT_CM_USS_RESPONSE_NOTIFY_IND, 3, event_result,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }
    }
    break;
#endif /* (defined(FEATURE_WCDMA) || defined(FEATURE_GSM)) */


    case CM_SUPS_EVENT_LCS_MOLR_CONF:
#ifdef FEATURE_MMODE_DUAL_SIM
      if( cmlog_is_dual_standby(ph_ptr->device_prop.msim_prop.standby_pref ) )
      {
        event_result[0] = (uint8)sups_info_ptr->asubs_id;
        event_result[1] = sups_info_ptr->ss_ref;
        CM_MSG_HIGH_1 ("lcs_molr_conf event ss_ref = %d\n",
                       sups_info_ptr->ss_ref);

        event_report_payload_ext (EVENT_CM_DS_LCS_MOLR_CONF, 2, event_result,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }
      else
#endif /* FEATURE_MMODE_DUAL_SIM */
      {
        event_result[0] = sups_info_ptr->ss_ref;
        CM_MSG_HIGH_1 ("lcs_molr_conf event ss_ref = %d\n",
                       sups_info_ptr->ss_ref);

        event_report_payload_ext (EVENT_CM_LCS_MOLR_CONF, 1, event_result,
                                  EVENT_CM_RPT_EXT_VERSION, &evnt_rpt_subs_id);
      }
      break;


    default:
      CM_MSG_LOW_1 ("cmlog_cm_sups_event_cb = %d", sups_event);
      break;

  } /* switch (sups_event) */

} /* cmlog_cm_sups_event_cb () */

#endif /* #if ((FEATURE_WCDMA) || (FEATURE_GSM) || (FEATURE_MMGPS)) */


void cmlog_init (void)
{
  int loop;

  /* 1) Initialize registered client. */
  diag_client.oprt_mode             = SYS_OPRT_MODE_NONE;
  diag_client.offline_signal_recvd  = FALSE;
  for(loop=0; cmutil_is_as_id_valid(loop); loop++)
  {
    diag_client.sub_info[loop].mode_pref             = CM_MODE_PREF_NONE;
    diag_client.sub_info[loop].acq_order_pref        = CM_GW_ACQ_ORDER_PREF_NONE;
    diag_client.sub_info[loop].srv_domain_pref       = CM_SRV_DOMAIN_PREF_NONE;
    diag_client.sub_info[loop].band_pref             = CM_BAND_PREF_NONE;
    diag_client.sub_info[loop].lte_band_pref         = CM_BAND_PREF_NONE;
    diag_client.sub_info[loop].tds_band_pref         = CM_BAND_PREF_NONE;
    diag_client.sub_info[loop].roam_pref             = CM_ROAM_PREF_NONE;
    diag_client.sub_info[loop].hybr_pref             = CM_HYBR_PREF_NONE;
    diag_client.sub_info[loop].network_sel_mode_pref = CM_NETWORK_SEL_MODE_PREF_NONE;
    diag_client.sub_info[loop].srv_status            = SYS_SRV_STATUS_NONE;
    diag_client.sub_info[loop].sys_mode              = SYS_SYS_MODE_NONE;
    diag_client.sub_info[loop].active_channel        = 0;
    diag_client.sub_info[loop].band_class            = SYS_BAND_CLASS_NONE;
    diag_client.sub_info[loop].packet_zone           = CM_SS_PACKET_ZONE_NONE;
    diag_client.sub_info[loop].roam_status           = SYS_ROAM_STATUS_NONE;
    diag_client.sub_info[loop].latitude              = 0;
    diag_client.sub_info[loop].longitude             = 0;
    diag_client.sub_info[loop].mcc                   = CM_INVALID_MOBILE_COUNTRY_CODE;
  }

  (void) cm_client_init( CM_CLIENT_TYPE_DIAG, &diag_client.client );

  /* Invalidate all entries in cmlog_call_info_list */
  for (loop=0; loop<CMCALL_MAX_CALL_OBJ; ++loop)
  {
    call_state_list[loop].call_id = CM_CALL_ID_INVALID;
  }

  /* 2) Register for desired CM phone events. */
  (void) cm_client_ph_reg(
    diag_client.client,
    cmlog_ph_event_handler,
    CM_CLIENT_EVENT_REG,
    (cm_ph_event_e_type) (CM_PH_EVENT_NONE+1),
    (cm_ph_event_e_type) (CM_PH_EVENT_MAX-1),
    NULL                );


  (void) cm_mm_client_ss_reg_msim(
    diag_client.client,
    cmlog_ss_event_handler_msim,
    CM_CLIENT_EVENT_REG,
    (cm_ss_event_e_type) (CM_SS_EVENT_NONE+1),
    (cm_ss_event_e_type) (CM_SS_EVENT_MAX-1),
    NULL,
    SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_2_MASK
#ifdef FEATURE_MMODE_TRIPLE_SIM
    | SYS_MODEM_AS_ID_3_MASK
#endif
  );


  /* Register for supplementary service events */
#ifdef CM_GW_SUPPORTED
  {
    cm_client_status_e_type return_status;
    return_status = cm_client_sups_reg(
                      diag_client.client,
                      cmlog_cm_sups_event_cb,
                      CM_CLIENT_EVENT_REG,
                      CM_SUPS_EVENT_REGISTER,
                      CM_SUPS_EVENT_GET_PASSWORD_RES,
                      NULL );

    if (return_status != CM_CLIENT_OK )
    {
      sys_err_fatal_invalid_value_exception(return_status);
    }
  }
#endif

  /* Register for supplementary service events */
#ifdef CM_GW_SUPPORTED
  {
    cm_client_status_e_type return_status;
    return_status = cm_client_sups_reg(
                      diag_client.client,
                      cmlog_cm_sups_event_cb,
                      CM_CLIENT_EVENT_REG,
                      CM_SUPS_EVENT_LCS_LOCATION_NOTIFICATION_IND,
                      CM_SUPS_EVENT_LCS_MOLR_CONF,
                      NULL );

    if (return_status != CM_CLIENT_OK )
    {
      sys_err_fatal_invalid_value_exception(return_status);
    }
  }
#endif /* #if (defined(FEATURE_WCDMA) || defined(FEATURE_GSM) ) */

  /* register for call events for diag */
  (void) cm_mm_client_call_reg(
    diag_client.client,
    cmlog_call_event_handler,
    CM_CLIENT_EVENT_REG,
    (cm_call_event_e_type) (CM_CALL_EVENT_NONE+1),
    (cm_call_event_e_type) (CM_CALL_EVENT_MAX-1),
    NULL );

  (void) cm_client_stats_reg( diag_client.client,
                              cmlog_cmstats_event_handler,
                              CM_CLIENT_EVENT_REG,
                              (CM_STATS_EVENT_NONE+1),
                              (CM_STATS_EVENT_MAX-1),
                              NULL );

  /* 3) Activate client. */
  (void) cm_client_act( diag_client.client );
  /*lint -restore */
}

#if defined FEATURE_HDR_HYBRID && defined FEATURE_GPSONE
/*===========================================================================
FUNCTION  cmlog_block_hdr_orig_during_gps

DESCRIPTION
   This function logs the block HDR orig during GPS event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_block_hdr_orig_during_gps(
  sys_modem_as_id_e_type    asubs_id
    /* Subscription ID */
)
{
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  event_report_payload_ext( EVENT_CM_BLOCK_HDR_ORIG_DURING_GPS,0,NULL,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id );

}
/*===========================================================================
FUNCTION  cmlog_allow_hdr_orig_during_gps

DESCRIPTION
   This function logs the allow HDR orig during GPS event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_allow_hdr_orig_during_gps(
  sys_modem_as_id_e_type    asubs_id
    /* Subscription ID */
)
{
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  event_report_payload_ext( EVENT_CM_ALLOW_HDR_ORIG_DURING_GPS,0,NULL,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id );
}
#endif


/*===========================================================================
FUNCTION  cmlog_ssac_call_event_handler

DESCRIPTION
   This function logs the ssac call event when voice/video over LTE call is made.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_ssac_call_event_handler(

  boolean                     ssac_call_result,
  /* ssac result - accept or reject the call */

  cmcall_ssac_mmtel_e_type    mmtel_type,
  /* voice/video over LTE */

  cmcall_ssac_sub_info_s_type          *ssac_info,
  /* containing cm ssac info, ie. ssac params, uptime, etc */

  sys_modem_as_id_e_type      asubs_id
  /* Subscription ID */

)
{
  cmlog_event_ssac_call_s_type event_ssac_call_payload;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);
  /* Return if null ptr
   */
  if (ssac_info == NULL)
  {
    CM_ERR_0("SSAC: ssac info ptr null");
    return;
  }

  /* Populate the event payload
   */
  switch (mmtel_type)
  {
    case SSAC_MMTEL_TYPE_VOICE:
      event_ssac_call_payload.ssac_param_barring_factor
        = (uint8)(ssac_info->ssac_params.barring_factor_mmtel_voice);
      event_ssac_call_payload.ssac_param_barring_time
        = (uint16)(ssac_info->ssac_params.barring_time_mmtel_voice);
      break;

    case SSAC_MMTEL_TYPE_VIDEO:
      event_ssac_call_payload.ssac_param_barring_factor
        = (uint8)(ssac_info->ssac_params.barring_factor_mmtel_video);
      event_ssac_call_payload.ssac_param_barring_time
        = (uint16)(ssac_info->ssac_params.barring_time_mmtel_video);
      break;

    /* Return if timer_action is invalid
      */
    default:
      CM_ERR_1("SSAC: invalid mmtel type %d",mmtel_type);
      return;
  }

  event_ssac_call_payload.ssac_result = (uint8)ssac_call_result;
  event_ssac_call_payload.mmtel_type = (uint8)mmtel_type;

  /* Send qxdm event
   */
  event_report_payload_ext( EVENT_CM_SSAC_CALL,
                            sizeof(event_ssac_call_payload),
                            &event_ssac_call_payload,
                            EVENT_CM_RPT_EXT_VERSION,
                            &evnt_rpt_subs_id );

}

/*===========================================================================
FUNCTION  cmlog_ssac_timer_event_handler

DESCRIPTION
   This function logs the ssac timer event such as start/stop/reset ssac timer.
   CANCEL action will always be sent with type VOICE_VIDEO

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_ssac_timer_event_handler(

  cmcall_ssac_timer_action_e_type    timer_action,
  /* start/stop/cancel timer */

  cmcall_ssac_mmtel_e_type           mmtel_type,
  /* voice/video over LTE */

  uint32                             timer,
  /* backoff timer if timer_action == start.  0 for all the other timer_action values */

  sys_modem_as_id_e_type             asubs_id
  /* Subscription ID */

)
{
  cmlog_event_ssac_timer_s_type event_ssac_timer_payload;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);

  /* Return if timer_action is invalid
   */
  if (timer_action != SSAC_TIMER_ACTION_START &&
      timer_action != SSAC_TIMER_ACTION_STOP  &&
      timer_action != SSAC_TIMER_ACTION_CANCEL)
  {
    CM_ERR_1("SSAC: invalid timer action %d",timer_action);
    return;
  }

  /* Return  mmtel_type is invalid
  1. If action is CANCEL and type is VOICE_VIDEO
  2. if type is not VOICE or VIDEO.
  */
  if (! (mmtel_type == SSAC_MMTEL_TYPE_VOICE ||
         mmtel_type == SSAC_MMTEL_TYPE_VIDEO ||
         (mmtel_type == SSAC_MMTEL_TYPE_VOICE_VIDEO &&
          timer_action == SSAC_TIMER_ACTION_CANCEL)))
  {
    CM_ERR_1("SSAC: invalid mmtel type %d",mmtel_type);
    return;
  }

  /* timer value is only for starting timer */
  if (timer_action != SSAC_TIMER_ACTION_START)
  {
    timer = 0;
  }

  /* Populate the event payload
   */
  event_ssac_timer_payload.timer_action = (uint8)timer_action;
  event_ssac_timer_payload.mmtel_type = (uint8)mmtel_type;

  /* Copy the backoff timer value manually because it is unaligned */
  memscpy(&(event_ssac_timer_payload.backoff_timer_byte_0),
          sizeof(uint32),
          &timer,
          sizeof(uint32));

  /* Send qxdm event
   */
  event_report_payload_ext( EVENT_CM_SSAC_TIME,
                            sizeof(event_ssac_timer_payload),
                            &event_ssac_timer_payload,
                            EVENT_CM_RPT_EXT_VERSION,
                            &evnt_rpt_subs_id );

}

/*===========================================================================
FUNCTION  cmlog_msim_info

DESCRIPTION
   This function logs any changes in MSIM information

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_msim_info(

  sys_modem_dual_standby_pref_e_type  user_standby_pref,

  sys_modem_dual_standby_pref_e_type  standby_pref,

  uint8                               active_subs,

  sys_modem_as_id_e_type              default_data_subs,

  sys_modem_as_id_e_type              priority_subs,

  cm_dds_cause_e_type                 dds_reason,

  sys_modem_device_mode_e_type        device_mode

)
{
  uint8 payload[7];
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(SYS_MODEM_AS_ID_1);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  payload[0] = (uint8)user_standby_pref;
  payload[1] = (uint8)standby_pref;
  payload[2] = (uint8)active_subs;
  payload[3] = (uint8)default_data_subs;
  payload[4] = (uint8)priority_subs;
  payload[5] = (uint8)dds_reason;
  payload[6] = (uint8)device_mode;
  
  event_report_payload_ext(EVENT_CM_MSIM_INFO_CHG,7,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);

}

/*===========================================================================
FUNCTION  cmlog_plmn_block_req

DESCRIPTION
   This function logs the Event in case the Domain Selection based PLMN Blocking
   for Voice Centric Devices gets triggered

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void cmlog_plmn_block_req(
  uint32         lte_disable_duration_nv,
  /* Value of lte_disable_duration as read from NV */
  uint32         plmn_block_interval,
  /* actual interval for which plmn is blocked */
  dword          t3402_value,
  /* Value of T3402*/
  sys_modem_as_id_e_type asubs_id
  /* Subscription ID */
)
{
  uint32 payload[3];
  uint32 *payload_ptr;
  uint32 evnt_rpt_subs_id = cmlog_sys_as_id_to_uint32(asubs_id);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* report EVENT_CM_PLMN_BLOCK_REQ */
  payload_ptr = payload;/* set payload pointer = &payload[0] */
  *payload_ptr = lte_disable_duration_nv;
  payload_ptr += sizeof(uint8);
  *payload_ptr = plmn_block_interval;
  payload_ptr += sizeof(uint8);
  *payload_ptr =(uint32)t3402_value;

  event_report_payload_ext(EVENT_CM_PLMN_BLOCK_REQ,12,payload,EVENT_CM_RPT_EXT_VERSION,&evnt_rpt_subs_id);
}

/*===========================================================================
FUNCTION  cmlog_sys_as_id_to_uint32

DESCRIPTION
   This function increments the subscription ID by 1 and converts it from
   sys_modem_as_id_e_type to uint32.
   Example:
   SYS_MODEM_AS_ID_1 -> 1
   SYS_MODEM_AS_ID_2 -> 2

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
static uint32 cmlog_sys_as_id_to_uint32(
  sys_modem_as_id_e_type         asubs_id
  /* Subscription ID */
)
{
  uint32 converted_subid = 1;

  if(asubs_id > SYS_MODEM_AS_ID_NONE &&
     asubs_id < MAX_SIMS &&
     (uint8)asubs_id < cmpmprx_get_num_of_sims())
  {
    converted_subid = (uint32)(asubs_id + 1);
  }
  return converted_subid;
}

#if defined __cplusplus
}
#endif
