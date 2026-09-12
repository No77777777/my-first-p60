/*====*====*===*====*====*====*====*====*====*====*====*====*====*====*====*
                             C M    I D M

                 I M P L E M E N T A T I O N    F I L E

GENERAL DESCRIPTION
  This file is the main implementation for the IDM module. This module will
  control the state transitions for Idle Digital Mode and control the
  handdown and handup mechanisms. This module will function in associaton
  with the CMSS module.

EXTERNALIZED FUNCTIONS
  Functional Interface:
    cmssidm_proc_rpt_srv_ind_ss_main
    cmssidm_proc_rpt_srv_ind_ss_hdr
    cmssidm_proc_hdr_reselect
    cmssidm_proc_cmd_call_connected
    cmssidm_proc_cmd_call_ended
    cmssidm_proc_timer
    cmssidm_get_curr_idm
    cmssidm_get_prev_idm
    cmssidm_init

REGIONAL FUNCTIONS
  None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The cmssidm_init() should be called before any APIs are called.


Copyright (c) 2003 - 2006 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmssidm.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/25/12   qf      Add a timer to restore HDR back to phone mode mask
06/14/11   fj      Add support to suspend and resume IDM timer.
04/11/11   am      Cleaning stubs and runnning qtf SUTE
01/19/11   fj      Use the IDM timer value which is read from EFS.
11/17/10   aj      Update IDM state machine to ignore 3GPP service
11/02/10   am      Replacing FEATURE_MOB_MMODE_CUTE
11/25/09   mh      Modified debug messsage in cmssidm_update_state()
09/01/09   sv      Moving from clk API to time API
08/07/07   ic      Lint clean up
06/06/07   pk/cl   Added support for LN status notification from HDR
11/07/06   pk      Added support for HYBR_GW and UMTS -> 1X handover
09/08/06   pk      Removed more preferred acquisiton check from IDM timer
                   processing
01/12/06   ic      For specific calls / walks Lint warnings, replaced global
                   -fsp comment with -e{error#} comments where needed
01/06/06   ic      Lint cleanup
12/09/05   ic      Changes to cmssidm_estimate_base_station_idm()
                   declaration so cmcall.c lints clean
11/03/05   ic      Added CM_ASSERT() for ptr function parameters and lint
                   comments for 613 Lint error
10/20/05   sk      Added concept of base station IDM and functions to access
                   and update it.
05/23/05   ic      Do not change IDM while SMS is in progress
08/04/04   ic      CM Lint clean up - cmssidm.c with Lint supression verified
                   and free of Lint issues in all CUTE configurations.
07/26/04   ic      Added cmssidm_dummy() to get rid of ADS 1.2 compiler
                   warning
07/07/04   sj      Fixed issue if HDR is acquired in STATE_MAIN and timer is
                   running.
06/28/04   sj      Lint cleanup.
05/28/04   ic      Modified cmssidm_proc_call_connected() to send IDM event
                   on voice call connect if SVD is supported
05/20/04   sj      Delay handdown to 1x, if HDR is lost and 1x is acquired.
02/25/04   ic      Changes to make the code pass unit test cases and at the
                   same time make calls in Saber build
02/12/04   ic      Merged in changes from MSMSHARED_CM.01.00.10.00.20 to
                   MSMSHARED_CM.01.00.10.00.23
02/06/04   ws      Initial jaguar/mainline merge.
12/11/03   ic      Replaced is_hybr_redialled with is_hybr_redialed
12/03/03   ic      Added CM SS IDM state machine diagram.
                   In cmssidm_proc_rpt_srv_ind_ss_hdr() fixed state machine
                   bug for transition #10 (as HDR became unusable, we were
                   using wrong field to update IDM state and we were not
                   starting the avoid hand up timer).
11/21/03   ic      Fixed FEATURE_HDR_HANDOFF featurization bug.
11/19/03   ic      Featurized SS IDM object's functionality under
                   FEATURE_HDR_HANDOFF.
                   Comment clean up.
                   State machine fixes.
11/06/03   SJ      Initial release.

===========================================================================*/
/*

                          CMSSIDM STATE MACHINE
                          =====================

    State transitions are numbered to identify them.
    Numbering does not imply order in which transitions take place.


                                      Transition #1
                                      Acq_main & usable,
                                      colloc HDR,
                                      phone not in use
                                      - start hand down timer
                                    -------------------------------
                                    |                             |
                                    |                             |
                                    |                             |
                                    V                             |
                        |----------------------------|            |
  [ Power up / init ]   |                            |            |
  --------------------->|     CMSSIDM_STATE_NONE     |-------------
                        |                            |-------------------------|
                        |----------------------------|-----                    |
                                            |             |                    |
                                            |             |                    |
          Transition #2      Transition #3  |           Transition #4          |
          PS data call       Hand down timer time out,  Acq 1X & usable        |
          connected on 1X,   1X available & usable,     no colloc HDR,         |
          no HDR attempt     phone not in use           phone not in use       |
          - IDM update       - IDM update   |           - IDM update           |
                |                           |             |                    |
Transition #5   |                           |             |   Transition #7    |
PS data call    |                           |             |   Acq HDR & usable,|
connected on 1X,|                           |             |   phone not in use |
HDR attempted   |                           |             |   - IDM update     |
- start avoid   |                           |             |                    |
  hand up timer |                           |             |                    |
- IDM update    |     Transition #6         |             |                    |
  |             |     Acq 1X & it's         |             |                    |
  |             |     usable,               |             |                    |
  |             |     no colloc HDR,        |             |                    |
  |             |     sys mode changed,     |             |                    |
  |             |     phone not in use      |             |                    |
  |             |     - IDM update          |             |                    |
  |             |        ------------       |             |                    |
  |             |        |          |       |             |                    |
  |             |        |          |       |             |                    |
  |             |        |          |       |             |                    |
  |             |        |          V       V             |                    |
  |             |       |----------------------------|    |                    |
  |             |------>|                            |<----                    |
  |-------------------->|     CMSSIDM_STATE_MAIN     |-----------              |
  |                ---->|                            |          |              |
  |                |    |----------------------------|          |              |
  |                |              ^          |                  |              |
  |                |              |          |                Transition #12   |
  |                |              |          |                Acq HDR & usable,|
Transition #8   Transition #9  Transition #10|                1X not usable,   |
Acq 1X          Hand down      HDR unusable, |                phone not in use |
& usable,       timer time     1X available, |                OR               |
hand down       out,           phone not     |                Lost 1X,         |
timer expired,  HDR not        in use        |                HDR acq & usable,|
phone not in    available      - start      Transition #11    phone not in use |
use, no colloc  1X available     avoid      Avoid hand up   - stop timers      |
- start avoid  - start avoid     handup     timer time out  - IDM update       |
hand up timer    hand up timer   timer      or timer not        |              |
- IDM update   - IDM update    - IDM update active, HDR         |              |
  |                       |       |         available &         |              |
  |                       |       |         usable, phone       |              |
  |                       |       |         not in use          |              |
  |                       |       |         - IDM update        |              |
  |                       |       |          |                  |              |
  |                       |       |          |                  |              |
  |                       |       |          |                  |              |
  |                       |       |          |                  |              |
  |                       |       |          |                  |              |
  |                       |       |          V                  |              |
  |                     |----------------------------|          |              |
  |---------------------|                            |<---------|              |
                        |   CMSSIDM_STATE_HYBR_HDR   |<------------------------|
  |-------------------->|                            |<--------|
  |                     |----------------------------|         |
  |                                      |                     |
 Transition #13                          |                     |
 PS call connected                       |---------------------|
 on HDR                                      Transition #14
                                             Lost HDR
                                             - start hand down timer
                                             or
                                             Acquired usable 1x & no timer active,
                                             having colloc,
                                             - start hand down timer

Notes:
------
1. End of every call, start 5 second timer or max
   if timer running, to possibly handup on HDR.
2. Any timeout processing
   if HDR avl && usable
     IDM = HDR
   else if main_avl && usable
     IDM = MAIN
3 If timer expires when the phone is in use, postpone processing
  the timer expiry until the phone is not in use

*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/*lint -esym(766,customer.h) */
#include "mmcp_variation.h"
#include "customer.h" /* Customer configuration file */

#ifdef FEATURE_HDR_HANDOFF
#include "comdef.h"   /* Definition for basic types and macros */
#include "msg.h"      /* Message output services */
#include "err.h"      /* Error and error fatal services */
#include "cmd.h"      /* Command header for inter-task communication */

#include "sys.h"      /* Common types. */
#include "sys_v.h"
#include "sd.h"       /* Interface for System Determination services */
#include "sd_v.h"
#include "cmmsc.h"
#include "cmsimcoord.h"

#include "cmss.h"     /* Interface to CM Serving system object. */
#include "cmdbg.h"    /* Interface to CM debug macros.  */
#include "cmssidm.h"  /* Interface to CM IDM extern fns. */
#include "cmssidmi.h" /* Interface to CM IDM internal types/fns. */
#include "cmcall.h"   /* Interface to CM call object. */
#include "cmxcall.h"
#include "cmxsms.h"
#include "cmxdbm.h"
#include "time_svc.h"
#include "cmtaski.h"

#ifdef CM_DEBUG
#error code not present
#endif

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/


/* Definition for IDM's info.
*/
static  cmssidm_info_s_type             cmssidm_info;

/*===========================================================================
=============================================================================
=============================================================================
==================== IDM IMPLEMENTATION RELATED HELPER FUNCTIONS ============
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION cmssidm_get_info_ptr

DESCRIPTION
  This function will fetch the pointer to the IDM's info.

DEPENDENCIES
  None.

RETURN VALUE
  cmssidm_info_s_type* - Pointer to IDM's info.
  Note! Cannot be NULL

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN cmssidm_info_s_type            *cmssidm_get_info_ptr
(

  void

)
{
  /* Return the pointer to the local definition of IDM's info.
  */
  return &cmssidm_info;

} /* cmssidm_get_info_ptr() */

/*===========================================================================

FUNCTION cmssidm_get_sub_info_ptr

DESCRIPTION
  This function will fetch the sub info pointer to the IDM's info.

DEPENDENCIES
  None.

RETURN VALUE
  cmssidm_info_s_type* - Pointer to IDM's info.
  Note! Cannot be NULL

SIDE EFFECTS
  None.
===========================================================================*/
cmssidm_sub_info_s_type *cmssidm_get_sub_info_ptr
(

  sys_modem_as_id_e_type  asubs_id

)
{
  if(!cmutil_is_as_id_valid(asubs_id))
  {
    asubs_id = SYS_MODEM_AS_ID_1;
  }
  if(cmssidm_get_info_ptr()->sub_info[asubs_id] == NULL)
  {
    CM_MSG_HIGH_1("ssidm sub [%d] info is NULL", asubs_id);
  }
  /* Return the pointer to the local definition of IDM's info.
  */
  return cmssidm_get_info_ptr()->sub_info[asubs_id];

} /* cmssidm_get_info_ptr() */



/*===========================================================================

FUNCTION cmssidm_update_state

DESCRIPTION
  This function sets the IDM's state.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                            cmssidm_update_state
(

  cmssidm_state_e_type            new_state,
  /* The new state of the IDM.
  */
  sys_modem_as_id_e_type  asubs_id
)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  CM_ASSERT_ENUM_IS_INRANGE(new_state, CMSSIDM_STATE_MAX);

  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }
  CM_MSG_HIGH_2("update_cmssidm_state(),state %d, new %d",
                ssidm_sub_info_ptr->state,
                new_state);


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the state.
  */
  ssidm_sub_info_ptr->state = new_state;


} /* cmssidm_update_state() */

/*===========================================================================

FUNCTION cmssidm_start_restore_hdr_timer

DESCRIPTION
  This function starts the restore_hdr_uptime timer.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void      cmssidm_start_restore_hdr_timer
(

  dword                           timeout,
  /* The new timeout value.
  */
  sys_modem_as_id_e_type  asubs_id

)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  dword curr_uptime  = time_get_uptime_secs();


  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }


  /* reselect_enable_hybr_hdr_uptime, start it.
  */
  if ( ssidm_sub_info_ptr->restore_hdr_uptime == CMSSIDM_UPTIME_NONE )
  {
    /* Timer not running, start it.
    */
    ssidm_sub_info_ptr->restore_hdr_uptime = curr_uptime + timeout;
  }
  else
  {
    /* Timer running, choose the max of current timeout and what the new timeout
    ** will be.
    */
    ssidm_sub_info_ptr->restore_hdr_uptime =
      MAX(ssidm_sub_info_ptr->restore_hdr_uptime,
          curr_uptime + timeout);
  }

  CM_MSG_HIGH_2("restore_hdr_uptime: curr %d timeout %d",
                curr_uptime,
                ssidm_sub_info_ptr->restore_hdr_uptime);

}


/*===========================================================================

FUNCTION cmssidm_reset_restore_hdr_timer

DESCRIPTION
  This function stops the restore_hdr timer.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void       cmssidm_reset_restore_hdr_timer
(
  sys_modem_as_id_e_type  asubs_id
)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }


  /* Stop the generic timer if not runing.
  */
  if ( ssidm_sub_info_ptr->restore_hdr_uptime!= CMSSIDM_UPTIME_NONE )
  {
    /* Timer running, stop it.
    */
    ssidm_sub_info_ptr->restore_hdr_uptime = CMSSIDM_UPTIME_NONE;

    CM_MSG_HIGH_0("stopped restore_hdr timer");
  }
  else
  {
    /* Handup timer not running.
    */
    CM_MSG_HIGH_0("restore_hdr timer not running state %d");
  }


} /* cmssidm_reset_idm_timer() */


/*===========================================================================

FUNCTION cmssidm_update_suspend_state

DESCRIPTION
  This function sets the IDM's suspended state.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN void                            cmssidm_update_suspend_state
(

  boolean            is_idm_timer_suspended,
  /* The suspended state of the IDM.
  */
  sys_modem_as_id_e_type  asubs_id
)
{

  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  dword                curr_uptime       = 0;

  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }

  if (ssidm_sub_info_ptr->is_idm_timer_suspended != is_idm_timer_suspended)
  {
    CM_MSG_HIGH_2("cmssidm_update_suspend_state(),suspended %d, new %d",
                  ssidm_sub_info_ptr->is_idm_timer_suspended,
                  is_idm_timer_suspended);
  }


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Resume the IDM timer.
  */
  if((ssidm_sub_info_ptr->is_idm_timer_suspended) &&
      (!is_idm_timer_suspended))
  {

    /* If the IDM itmer is not OFF, resume the IDM timer.
    */
    if ( ssidm_sub_info_ptr->idm_timer_uptime != CMSSIDM_UPTIME_NONE )
    {
      curr_uptime  = time_get_uptime_secs();

      /* Update the timeout for the IDM timer. */
      ssidm_sub_info_ptr->idm_timer_uptime += curr_uptime;

      CM_MSG_HIGH_1("Resume IDM timer, timeout %d",
                    ssidm_sub_info_ptr->idm_timer_uptime);
    }
    else
    {
      CM_MSG_HIGH_0("Don't resume IDM timer, since IDM timer is off");
    }

  }

  /* Suspend the IDM timer.
  */
  if((!(ssidm_sub_info_ptr->is_idm_timer_suspended)) &&
      is_idm_timer_suspended)
  {

    /* If the IDM timer is not OFF, suspend the IDM timer.
    */
    if ( ssidm_sub_info_ptr->idm_timer_uptime != CMSSIDM_UPTIME_NONE )
    {
      curr_uptime = time_get_uptime_secs();

      /* Calculate and save the remaing time */
      ssidm_sub_info_ptr->idm_timer_uptime -= curr_uptime;

      CM_MSG_HIGH_1("Suspend IDM timer, remaining time %d",
                    ssidm_sub_info_ptr->idm_timer_uptime);
    }
    else
    {
      CM_MSG_HIGH_0("Don't suspend IDM timer, since IDM timer is off");
    }

  }

  /* Update the suspended state.
  */
  ssidm_sub_info_ptr->is_idm_timer_suspended = is_idm_timer_suspended;


} /* cmssidm_update_suspend_state() */



/*===========================================================================

FUNCTION cmssidm_start_idm_timer

DESCRIPTION
  This function starts the idm timer.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                            cmssidm_start_idm_timer
(

  dword                           timeout,
  /* The new timeout value.
  */
  sys_modem_as_id_e_type  asubs_id

)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  dword curr_uptime  = time_get_uptime_secs();


  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }


  /* IDM timer not running, start it.
  */
  if ( ssidm_sub_info_ptr->idm_timer_uptime == CMSSIDM_UPTIME_NONE )
  {
    /* Timer not running, start it.
    */
    ssidm_sub_info_ptr->idm_timer_uptime = curr_uptime + timeout;
  }
  else
  {
    /* Timer running, choose the max of current timeout and what the new timeout
    ** will be.
    */
    ssidm_sub_info_ptr->idm_timer_uptime = MAX(ssidm_sub_info_ptr->idm_timer_uptime,
                                               curr_uptime + timeout
                                              );
  }

  CM_MSG_HIGH_3("Set idm timer state %d, curr %d timeout %d",
                ssidm_sub_info_ptr->state,
                curr_uptime,
                ssidm_sub_info_ptr->idm_timer_uptime);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifndef FEATURE_MMODE_DUAL_SIM
  /* If REGPRX is active, is_idm_timer_suspended is set to TRUE.
  ** calculate and save the remaining time of the IDM timer as to suspend it.
  */
  if ( ssidm_sub_info_ptr->is_idm_timer_suspended )
  {
    ssidm_sub_info_ptr->idm_timer_uptime -= curr_uptime;
  }
#endif

} /* cmssidm_start_idm_timer() */



/*===========================================================================

FUNCTION cmssidm_stop_idm_timer

DESCRIPTION
  This function stops the generic idm timer.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                            cmssidm_reset_idm_timer
(
  sys_modem_as_id_e_type  asubs_id
)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);

  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }

  /* Stop the generic timer if not runing.
  */
  if ( ssidm_sub_info_ptr->idm_timer_uptime != CMSSIDM_UPTIME_NONE )
  {
    /* Timer running, stop it.
    */
    ssidm_sub_info_ptr->idm_timer_uptime = CMSSIDM_UPTIME_NONE;

    CM_MSG_HIGH_1("stopped timer state %d",
                  ssidm_sub_info_ptr->state);
  }


} /* cmssidm_reset_idm_timer() */



/*===========================================================================
FUNCTION cmssidm_set_idle_digital_mode

DESCRIPTION
  This function sets the idm without checking if phone is in use or timer
  is active.

DEPENDENCIES
  Phone object must be initialized

RETURNS
  None

SIDE EFFECTS
  none

===========================================================================*/
static boolean                         cmssidm_set_idle_digital_mode
(

  sys_sys_mode_e_type             new_idm,
  /* new Idle digital mode.
  */
  sys_modem_as_id_e_type  asubs_id
)
{

  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);


  if(ssidm_sub_info_ptr == NULL)
  {
    return FALSE;
  }

  /* Check if IDM really changed.
  */
  if ( ssidm_sub_info_ptr->curr_idm == new_idm )
  {
    CM_MSG_HIGH_1("No change in IDM %d", new_idm);
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Update the new IDM.
  */
  CM_MSG_HIGH_3("Set IDM new %d cur %d prev %d",
                new_idm,
                ssidm_sub_info_ptr->curr_idm,
                ssidm_sub_info_ptr->prev_idm);
  ssidm_sub_info_ptr->prev_idm  = ssidm_sub_info_ptr->curr_idm;
  ssidm_sub_info_ptr->curr_idm  = new_idm;

  return TRUE;

} /* cmssidm_set_idle_digital_mode() */



/*===========================================================================
FUNCTION cmssidm_update_idle_digital_mode

DESCRIPTION
  This function update the idle digital mode if the phone is not in use
  and timer is not active.

DEPENDENCIES
  Phone object must be initialized

RETURNS
  None

SIDE EFFECTS
  none

===========================================================================*/
static void                         cmssidm_update_idle_digital_mode
(

  sys_sys_mode_e_type             new_idm,
  /* new idle digital mode.
  */

  cmssidm_state_e_type            new_state,
  /* new IDM state
  */

  sys_modem_as_id_e_type          asubs_id
)
{

  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  cmph_s_type         *ph_ptr           =  cmph_ptr();

  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }

  CM_ASSERT( BETWEEN(new_state, CMSSIDM_STATE_NONE, CMSSIDM_STATE_MAX ));

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if IDM can be updated.
  */
  if( cmph_get_is_in_use(asubs_id)||
      ssidm_sub_info_ptr->idm_timer_uptime != CMSSIDM_UPTIME_NONE ||
      cmxsms_is_mo_sms_in_progress(asubs_id) ||
      cmxdbm_is_mo_dbm_in_progress(asubs_id)
    )
  {
    /* Not idle or need to wait or SMS or DBM in progress - do not update
    ** the idle digital mode.
    */
    CM_MSG_HIGH_1("Ignore new IDM=%d",new_idm);
    return;

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set the IDM and update state
  */
  (void)cmssidm_set_idle_digital_mode( new_idm, asubs_id );
  cmssidm_update_state( new_state, asubs_id );

} /* cmssidm_update_idle_digital_mode() */

#if defined FEATURE_CM_LTE

/*===========================================================================
FUNCTION cmssidm_reevaluate_IDM

DESCRIPTION
  This function re-evaluates the IDM based on serving status and cmssidm state
  is active.

DEPENDENCIES
  Phone object must be initialized

RETURNS
  None

SIDE EFFECTS
  none

===========================================================================*/
/*lint --e{528} suppress cm_cmd_alloc_mbms_init not referenced
** warning
*/

static void  cmssidm_reevaluate_IDM
(
  sys_modem_as_id_e_type  asubs_id
)
{

  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  /* Pointer to serving system object.
  */
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_0_ptr, *ss_info_all_stack_1_ptr;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(asubs_id);
  cmss_misc_gen_stack_info_s_type *ss_misc_stk0_ptr, *ss_misc_stk1_ptr;


  if(ss_info_sub_prop_ptr == NULL || !cmutil_is_as_id_valid(asubs_id) ||
      ss_misc_sub_ptr == NULL || ssidm_sub_info_ptr == NULL)
  {
    return;
  }
  ss_info_all_stack_0_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;
  ss_info_all_stack_1_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_1].gen_info;
  ss_misc_stk0_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_0].gen_info;
  ss_misc_stk1_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_1].gen_info;

  CM_MSG_HIGH_1("cmssidm state is  %d", ssidm_sub_info_ptr->state);

  switch ( ssidm_sub_info_ptr->state )
  {
    case CMSSIDM_STATE_NONE:
      /* Check if we are in hybrid operation and if colloc is avail.
        */
      if ( cmss_is_stack1_operational(asubs_id) &&
           ss_misc_sub_ptr->sub_info.gpp2_sub_info.is_colloc
         )
      {
        /* We are in hybrid and colloc is avail
        */
        if (ss_misc_stk1_ptr != NULL &&
            ss_misc_stk1_ptr->ss_usable == SD_SYS_USABLE_OK &&
            ss_info_all_stack_1_ptr != NULL &&
            sys_srv_status_is_srv(ss_info_all_stack_1_ptr->srv_status))
        {
          CM_MSG_HIGH_0("resetting IDM timer and set HDR as IDM");
          cmssidm_reset_idm_timer(asubs_id);
          cmssidm_update_idle_digital_mode(SYS_SYS_MODE_HDR,CMSSIDM_STATE_HYBR_HDR, asubs_id);
        }
        else if(ss_info_all_stack_0_ptr != NULL && ss_misc_stk0_ptr != NULL &&
                sys_srv_status_is_srv( ss_info_all_stack_0_ptr->srv_status ) &&
                ss_misc_stk0_ptr->ss_usable == SD_SYS_USABLE_OK)
        {
          cmssidm_update_idle_digital_mode
          (
            SYS_SYS_MODE_CDMA,
            CMSSIDM_STATE_MAIN,
            asubs_id
          );
        }
      }
      else if(ss_info_all_stack_0_ptr != NULL && ss_misc_stk0_ptr != NULL &&
              sys_srv_status_is_srv( ss_info_all_stack_0_ptr->srv_status ) &&
              ss_misc_stk0_ptr->ss_usable == SD_SYS_USABLE_OK)
      {
        /* We are either not in hybrid or no colloc sys available.
        */
        cmssidm_update_idle_digital_mode(SYS_SYS_MODE_CDMA,CMSSIDM_STATE_MAIN, asubs_id);
      }
      break;


    case CMSSIDM_STATE_MAIN:

      /* If Main service is unusable, update the IDM as HDR
      ** else start handup timer.
      */
      if ( (ss_misc_stk0_ptr != NULL && ss_misc_stk0_ptr->ss_usable != SD_SYS_USABLE_OK) ||
           ( ss_misc_stk1_ptr != NULL && ss_misc_stk1_ptr->ss_usable == SD_SYS_USABLE_OK &&
             ss_info_all_stack_1_ptr != NULL && sys_srv_status_is_srv(ss_info_all_stack_1_ptr->srv_status))
         )
      {
        /* Transition to IDM = HDR state.
        */
        CM_MSG_HIGH_0("resetting IDM timer and set HDR as IDM");
        cmssidm_reset_idm_timer(asubs_id);
        cmssidm_update_idle_digital_mode(SYS_SYS_MODE_HDR,CMSSIDM_STATE_HYBR_HDR, asubs_id);
      }
      break;


    case CMSSIDM_STATE_HYBR_HDR:
    case CMSSIDM_STATE_MAX:
    default:
      break;

  } /* switch ( cmssidm_info_ptr->state ) */

}

#endif

/*===========================================================================
=============================================================================
=============================================================================
==================== SERVICE INDICATION EVENT HANDLER ========================
=============================================================================
=============================================================================
===========================================================================*/



/*===========================================================================

FUNCTION cmssidm_proc_rpt_srv_ind_ss_main

DESCRIPTION
  This function should be called to process the CM_SRV_IND_INFO_F report
  for SS-Main.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_proc_rpt_srv_ind_ss_main
(

  const sd_si_info_s_type        *si_info_ptr,
  /* Pointer to buffer containing new service info information.
  */
  sys_modem_as_id_e_type  asubs_id

)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_1_ptr;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(asubs_id);
  cmss_misc_gen_stack_info_s_type *ss_misc_stk0_ptr, *ss_misc_stk1_ptr;


  if(ss_info_sub_prop_ptr == NULL || !cmutil_is_as_id_valid(asubs_id) ||
      ss_misc_sub_ptr == NULL || ssidm_sub_info_ptr == NULL)
  {
    return;
  }
  ss_info_all_stack_1_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_1].gen_info;
  ss_misc_stk0_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_0].gen_info;
  ss_misc_stk1_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_1].gen_info;

  CM_ASSERT( si_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If there is service on main stack which is other than 3GPP2, do not
  ** update IDM and reset any timers that are running.
  */
  if ( sys_srv_status_is_srv( si_info_ptr->srv_status  ) &&
       ! ( si_info_ptr->mode == SYS_SYS_MODE_CDMA ||
           si_info_ptr->mode == SYS_SYS_MODE_HDR )
     )
  {
    cmssidm_reset_idm_timer(asubs_id);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process event based on the current state.
  */
  switch ( ssidm_sub_info_ptr->state )
  {
    case CMSSIDM_STATE_NONE:
      /* check if SS-Main was acquired.
      */
      if( sys_srv_status_is_srv( si_info_ptr->srv_status ) &&
          !si_info_ptr->is_pref_srv_acq &&
          ss_misc_stk0_ptr != NULL &&
          ss_misc_stk0_ptr->ss_usable == SD_SYS_USABLE_OK
        )

      {
        /* Check if we are in hybrid operation and if colloc is avail.
        */
        if ( cmss_is_stack1_operational(asubs_id) &&
             si_info_ptr->is_colloc
           )
        {
          /* We are in hybrid and colloc is avail, start handdown timer
          ** if timer not running already.
          */
          if (ssidm_sub_info_ptr->idm_timer_uptime == CMSSIDM_UPTIME_NONE )
          {
#ifdef FEATURE_CM_LTE
            cmssidm_start_idm_timer( ss_misc_sub_ptr->sub_info.gpp2_sub_info.idm_timer_value, asubs_id );
#else
            cmssidm_start_idm_timer( CMSSIDM_HANDDOWN_TIME, asubs_id );
#endif
          }

        }
        else
        {
          /* We are either not in hybrid or no colloc sys available.
          */
          cmssidm_update_idle_digital_mode
          (
            cmss_map_sd_mode_to_cm_mode(si_info_ptr->mode),
            CMSSIDM_STATE_MAIN,
            asubs_id
          );
        }
        break;
      }
      break;

    case CMSSIDM_STATE_MAIN:
      /* Check if main service was lost.
      */
      if ( !sys_srv_status_is_srv( si_info_ptr->srv_status ) )
      {
        /* Lost main service, transition to HDR if service usable.
        */
        if ( ss_info_all_stack_1_ptr != NULL &&
             sys_srv_status_is_srv(ss_info_all_stack_1_ptr->srv_status) &&
             ss_misc_stk1_ptr != NULL &&
             !ss_misc_stk1_ptr->is_pref_srv_acq &&
             ss_misc_stk1_ptr->ss_usable == SD_SYS_USABLE_OK
           )
        {
          cmssidm_reset_idm_timer(asubs_id);
          cmssidm_update_idle_digital_mode(
            SYS_SYS_MODE_HDR,
            CMSSIDM_STATE_HYBR_HDR,
            asubs_id
          );
        }
        break;
      }

      /* Check if main service changed wrt to mode.
      */
      if ( sys_srv_status_is_srv( si_info_ptr->srv_status ) &&
           !si_info_ptr->is_pref_srv_acq &&
           cmss_map_sd_mode_to_cm_mode( si_info_ptr->mode )
             != ss_info_sub_prop_ptr->sub_info.gpp2_sub_info.cur_idle_digital_mode
         )
      {
        /* Just update the new IDM, no state change.
        */
        cmssidm_update_idle_digital_mode
        (
          cmss_map_sd_mode_to_cm_mode( si_info_ptr->mode ),
          ssidm_sub_info_ptr->state,
          asubs_id
        );
        break;
      }
      break;

    case CMSSIDM_STATE_HYBR_HDR:

      /* Check if HDR is ok.
      */
      if ( ss_info_all_stack_1_ptr != NULL &&
           sys_srv_status_is_srv(ss_info_all_stack_1_ptr->srv_status ) &&
           ss_misc_stk1_ptr != NULL &&
           ss_misc_stk1_ptr->ss_usable == SD_SYS_USABLE_OK
         )
      {
        /* Nothing to do.
        */
        break;
      }

      /* HDR is not acquired or unusable,
      ** check if SS-Main was acquired.
      */
      if( sys_srv_status_is_srv( si_info_ptr->srv_status ) &&
          !si_info_ptr->is_pref_srv_acq  &&
          si_info_ptr->usable_status ==  SD_SYS_USABLE_OK &&
          ssidm_sub_info_ptr->idm_timer_uptime == CMSSIDM_UPTIME_NONE
        )
      {
        if ( cmss_is_stack1_operational(asubs_id) &&
             si_info_ptr->is_colloc
           )
        {
#ifdef FEATURE_CM_LTE
          cmssidm_start_idm_timer( ss_misc_sub_ptr->sub_info.gpp2_sub_info.idm_timer_value, asubs_id );
#else
          cmssidm_start_idm_timer( CMSSIDM_HANDDOWN_TIME, asubs_id );
#endif
        }
        else
        {
          cmssidm_update_idle_digital_mode
          (
            cmss_map_sd_mode_to_cm_mode( si_info_ptr->mode ),
            CMSSIDM_STATE_MAIN,
            asubs_id
          );
        }
        break;
      }
      break;

    case CMSSIDM_STATE_MAX:
    default:
      break;

  } /* switch ( cmssidm_info_ptr->state ) */


} /* cmssidm_proc_rpt_srv_ind_ss_main() */



/*===========================================================================

FUNCTION cmssidm_proc_rpt_srv_ind_ss_hdr

DESCRIPTION
  This function should be called to process the CM_SRV_IND_INFO_F report
  for SS-HDR.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_proc_rpt_srv_ind_ss_hdr
(
  const sd_si_info_s_type        *si_info_ptr,
  /* Pointer to buffer containing new service info information.
  */
  sys_modem_as_id_e_type  asubs_id,

  boolean                        is_hdr_no_srv_timer_expire

)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);

  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_0_ptr, *ss_info_all_stack_1_ptr;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(asubs_id);
  cmss_misc_gen_stack_info_s_type *ss_misc_stk0_ptr;


  if(ss_info_sub_prop_ptr == NULL || !cmutil_is_as_id_valid(asubs_id) ||
      ss_misc_sub_ptr == NULL || ssidm_sub_info_ptr == NULL)
  {
    return;
  }
  ss_info_all_stack_0_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;
  ss_info_all_stack_1_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_1].gen_info;
  ss_misc_stk0_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_0].gen_info;

  CM_ASSERT( si_info_ptr != NULL );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( si_info_ptr->mode != SD_MODE_HDR )
  {
    return;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Process event based on the current state.
  */
  switch ( ssidm_sub_info_ptr->state )
  {
    case CMSSIDM_STATE_NONE:
      /* check if Hybrid instance was acquired.
      */
      if( sys_srv_status_is_srv( si_info_ptr->srv_status ) &&
          !si_info_ptr->is_pref_srv_acq &&
          si_info_ptr->usable_status == SD_SYS_USABLE_OK
        )
      {
        /* Stop the timer and Update the IDM.
        */
        cmssidm_reset_idm_timer(asubs_id);
        cmssidm_update_idle_digital_mode
        (
          cmss_map_sd_mode_to_cm_mode(si_info_ptr->mode),
          CMSSIDM_STATE_HYBR_HDR,
          asubs_id
        );
        break;
      }
      break;


    case CMSSIDM_STATE_MAIN:
      /* check if Hybrid HDR instance acquired.
      */
      if( sys_srv_status_is_srv( si_info_ptr->srv_status ) &&
          !si_info_ptr->is_pref_srv_acq &&
          si_info_ptr->usable_status == SD_SYS_USABLE_OK
        )
      {
        /* If Main service is unusable, update the IDM as HDR
        ** else start handup timer.
        */
        if ( ss_misc_stk0_ptr != NULL && ss_misc_stk0_ptr->ss_usable != SD_SYS_USABLE_OK )
        {
          /* Transition to IDM = HDR state.
          */
          cmssidm_reset_idm_timer(asubs_id);
          cmssidm_update_idle_digital_mode
          (
            cmss_map_sd_mode_to_cm_mode(si_info_ptr->mode),
            CMSSIDM_STATE_HYBR_HDR,
            asubs_id
          );
        }
        else if ( ssidm_sub_info_ptr->idm_timer_uptime == CMSSIDM_UPTIME_NONE )
        {
          /* Main is available, if timer is not running update to HDR.
          */
          cmssidm_update_idle_digital_mode
          (
            cmss_map_sd_mode_to_cm_mode(si_info_ptr->mode),
            CMSSIDM_STATE_HYBR_HDR,
            asubs_id
          );
        }
        break;
      }
      break;


    case CMSSIDM_STATE_HYBR_HDR:
      /* check if Hybrid instance is unusable
      ** OR
      ** if HDR no service timer expired and IDM needs to be updated
      */
      if(( sys_srv_status_is_srv( si_info_ptr->srv_status ) &&
           si_info_ptr->usable_status == SD_SYS_USABLE_NOT_OK
         ) ||
         (is_hdr_no_srv_timer_expire == TRUE &&
           ssidm_sub_info_ptr->idm_timer_uptime == CMSSIDM_UPTIME_NONE
         ))
      {
        /* Hybrid instance is unusable, check if Main service is available.
        */
        if ( ss_info_all_stack_0_ptr != NULL &&
             sys_srv_status_is_srv( ss_info_all_stack_0_ptr->srv_status ) &&
             ss_misc_stk0_ptr != NULL &&
             ss_misc_stk0_ptr->ss_usable == SD_SYS_USABLE_OK
           )
        {
          /* Main service is available, Update the IDM.
          */
          cmssidm_reset_idm_timer(asubs_id);
          cmssidm_update_idle_digital_mode
          (
            ss_info_all_stack_0_ptr->sys_mode,
            CMSSIDM_STATE_MAIN,
            asubs_id
          );
          cmssidm_start_idm_timer( CMSSIDM_AVOID_HANDUP_TIME, asubs_id );
        }
        break;
      }

      /* Check if hybrid service was lost.
      */
      if ( !sys_srv_status_is_srv( si_info_ptr->srv_status ) &&
           ss_info_all_stack_1_ptr != NULL &&
           sys_srv_status_is_srv( ss_info_all_stack_1_ptr->srv_status )
         )
      {
        /* Lost hybrid service, start handdown timer.
        */
#ifdef FEATURE_CM_LTE
        cmssidm_start_idm_timer( ss_misc_sub_ptr->sub_info.gpp2_sub_info.idm_timer_value, asubs_id );
#else
        cmssidm_start_idm_timer( CMSSIDM_HANDDOWN_TIME, asubs_id );
#endif
        break;
      }
      break;

    case CMSSIDM_STATE_MAX:
    default:
      break;

  } /* switch ( cmssidm_info_ptr->state ) */


} /* cmssidm_proc_rpt_srv_ind_ss_hdr() */



/*===========================================================================
=============================================================================
=============================================================================
==================== HDR LN STATUS EVENT HANDLER ============================
=============================================================================
=============================================================================
===========================================================================*/


/*===========================================================================

FUNCTION cmssidm_proc_rpt_hdr_ln_status

DESCRIPTION
  This function should be called to process the
  CM_HDR_LOCATION_NOTIFICATION_STATUS_F report

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_proc_rpt_hdr_ln_status
(
  sys_hdr_location_notification_status_e_type hdr_ln_status,
  /* The LN status
  */
  sys_modem_as_id_e_type  asubs_id
)
{
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_0_ptr, *ss_info_all_stack_1_ptr;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(asubs_id);
  cmss_misc_gen_stack_info_s_type *ss_misc_stk0_ptr, *ss_misc_stk1_ptr;


  if(ss_info_sub_prop_ptr == NULL || !cmutil_is_as_id_valid(asubs_id) ||
      ss_misc_sub_ptr == NULL)
  {
    return;
  }
  ss_info_all_stack_0_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;
  ss_info_all_stack_1_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_1].gen_info;
  ss_misc_stk0_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_0].gen_info;
  ss_misc_stk1_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_1].gen_info;


  switch( hdr_ln_status )
  {
    case SYS_HDR_LOCATION_NOTIFICATION_STATUS_SUCCESSFUL:
      if ( ss_info_all_stack_1_ptr != NULL && ss_misc_stk1_ptr != NULL &&
           sys_srv_status_is_srv( ss_info_all_stack_1_ptr->srv_status ) &&
           !ss_misc_stk1_ptr->is_pref_srv_acq &&
           ss_misc_stk1_ptr->ss_usable == SD_SYS_USABLE_OK
         )
      {

        /* Stop the IDM timer and change IDM to HDR
        */
        cmssidm_reset_idm_timer(asubs_id);
        (void)cmssidm_set_idle_digital_mode( SYS_SYS_MODE_HDR, asubs_id );
        cmssidm_update_state( CMSSIDM_STATE_HYBR_HDR, asubs_id );
        break;

      }

      break;

    case SYS_HDR_LOCATION_NOTIFICATION_STATUS_FAILED:

      /* Stop the IDM timer and change IDM to MAIN or No sevrvice
      */
      if ( ss_info_all_stack_0_ptr != NULL && ss_misc_stk0_ptr != NULL &&
           sys_srv_status_is_srv( ss_info_all_stack_0_ptr->srv_status ) &&
           ss_misc_stk0_ptr->ss_usable == SD_SYS_USABLE_OK
         )
      {
        /* Main service is available, Update the IDM.
        */
        cmssidm_reset_idm_timer(asubs_id);
        (void)cmssidm_set_idle_digital_mode( ss_info_all_stack_0_ptr->sys_mode, asubs_id );
        cmssidm_update_state( CMSSIDM_STATE_MAIN, asubs_id );
        break;
      }
      else
      {
        cmssidm_reset_idm_timer(asubs_id);
        (void)cmssidm_set_idle_digital_mode( SYS_SYS_MODE_NO_SRV, asubs_id );
        cmssidm_update_state( CMSSIDM_STATE_MAIN, asubs_id );

      }

      break;


    case SYS_HDR_LOCATION_NOTIFICATION_STATUS_NONE:
    case SYS_HDR_LOCATION_NOTIFICATION_STATUS_MAX:
    default:

      CM_ERR_1(" Invalid ln status %d", hdr_ln_status);
      break; /*lint !e527 */


  }

  return;

} /* cmssidm_proc_rpt_hdr_ln_status() */



/*===========================================================================
=============================================================================
=============================================================================
===================== HDR RESELECT EVENT HANDLER ============================
=============================================================================
=============================================================================
===========================================================================*/



/*===========================================================================

FUNCTION cmssidm_proc_hdr_reselect

DESCRIPTION
  This function should be called when the HDRMC reports Hdr reselect or not
  good for traffic.
  Implemented only for backwards compatability.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_proc_hdr_reselect
(
  sys_modem_as_id_e_type  asubs_id
)
{

  /* Exit HDR and stay in 1x only mode.
  */
  CM_MSG_HIGH_0("Reselect - got CM_HDR_RESELECT_EXIT_HDR_F");

  cmmsimc_proc_disable_hybr_hdr(asubs_id);

  /* Set the IDM as CDMA and generate the client event immediately.
  */
  if ( cmss_is_stack1_operational(asubs_id) &&
       cmssidm_set_idle_digital_mode( SYS_SYS_MODE_CDMA, asubs_id )
     )
  {
    /* IDM has changed, generated its event.
    */
    cmssidm_reset_idm_timer(asubs_id);
    cmss_send_idle_digital_mode_event(asubs_id);
  }

  /* Start restore_hdr timer.  Upon timer expiration, cm should restore hdr back to mode pref */
  cmssidm_start_restore_hdr_timer(CMSSIDM_RESTORE_HDR_TIME, asubs_id);

} /* cmssidm_proc_hdr_reselect() */



/*===========================================================================
=============================================================================
=============================================================================
==================== TIMER EVENT HANDLER ====================================
=============================================================================
=============================================================================
===========================================================================*/



/*===========================================================================

FUNCTION cmssidm_proc_timer

DESCRIPTION
  This function should be called to process the CM's 1 second timer tick.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_proc_timer
(

  sys_modem_as_id_e_type  asubs_id
)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_0_ptr, *ss_info_all_stack_1_ptr;
  cmss_misc_sub_prop_s_type *ss_misc_sub_ptr = cmss_get_misc_sub_prop_ptr(asubs_id);
  cmss_misc_gen_stack_info_s_type *ss_misc_stk0_ptr, *ss_misc_stk1_ptr;


  if(ss_info_sub_prop_ptr == NULL || !cmutil_is_as_id_valid(asubs_id) ||
      ss_misc_sub_ptr == NULL || ssidm_sub_info_ptr == NULL)
  {
    return;
  }
  ss_info_all_stack_0_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;
  ss_info_all_stack_1_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_1].gen_info;
  ss_misc_stk0_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_0].gen_info;
  ss_misc_stk1_ptr = ss_misc_sub_ptr->stack_info[MM_STACK_1].gen_info;

  /* If restore_hdr timer expires, reset the timer, and restore hdr
  */
  if( ssidm_sub_info_ptr->restore_hdr_uptime<= time_get_uptime_secs())
  {
    cmssidm_reset_restore_hdr_timer(asubs_id);
    cmmsimc_proc_enable_hybr_hdr(asubs_id, TRUE);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (ssidm_sub_info_ptr->is_idm_timer_suspended)
  {
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If there is service on main stack which is other than 3GPP2, do not
  ** update IDM
  */
  if ( ss_info_all_stack_0_ptr != NULL &&
       sys_srv_status_is_srv( ss_info_all_stack_0_ptr->srv_status ) &&
       ! ( ss_info_all_stack_0_ptr->sys_mode == SYS_SYS_MODE_CDMA ||
           ss_info_all_stack_0_ptr->sys_mode == SYS_SYS_MODE_HDR )
     )
  {
    return;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Check if "idm_timer" expired and the phone is not in use.
  */
  if( ssidm_sub_info_ptr->idm_timer_uptime <= time_get_uptime_secs() &&
      !cmph_get_is_in_use(asubs_id)
    )

  {
    CM_MSG_HIGH_0("IDM timer expired & not in use");

    /* reset the expired timer.
    */
    cmssidm_reset_idm_timer(asubs_id);


    /* Check if Hybrid HDR has service.
    */
    if ( ss_info_all_stack_1_ptr != NULL && ss_misc_stk1_ptr != NULL &&
         sys_srv_status_is_srv( ss_info_all_stack_1_ptr->srv_status ) &&
         ss_misc_stk1_ptr->ss_usable == SD_SYS_USABLE_OK
       )
    {
      /* Hybrid HDR has service, Update the IDM.
      */
      cmssidm_update_idle_digital_mode(SYS_SYS_MODE_HDR,
                                       CMSSIDM_STATE_HYBR_HDR,
                                       asubs_id);

    }
    else if ( ss_info_all_stack_0_ptr != NULL && ss_misc_stk0_ptr != NULL &&
              sys_srv_status_is_srv( ss_info_all_stack_0_ptr->srv_status) &&
              ss_misc_stk0_ptr->ss_usable == SD_SYS_USABLE_OK
            )
    {

      /* Update the IDM's state, start the avoid timer if we were
      ** in HDR.
      */
      cmssidm_state_e_type prev_SS_IDM_state = ssidm_sub_info_ptr->state;

      cmssidm_update_idle_digital_mode(ss_info_all_stack_0_ptr->sys_mode,
                                       CMSSIDM_STATE_MAIN,
                                       asubs_id);

      if ( prev_SS_IDM_state == CMSSIDM_STATE_HYBR_HDR &&
           cmss_is_stack1_operational(asubs_id)
         )
      {
        cmssidm_start_idm_timer( CMSSIDM_AVOID_HANDUP_TIME, asubs_id );
      }

    }

  } /* if( cmssidm_info_ptr->idm_timer_uptime <= time_get_uptime_secs() ) */


} /* cmssidm_proc_timer() */


/*===========================================================================
=============================================================================
=============================================================================
==================== CALL CONNECTED EVENT HANDLER ===========================
=============================================================================
=============================================================================
===========================================================================*/



/*===========================================================================

FUNCTION cmssidm_proc_call_connected

DESCRIPTION
  This function should be called when the call enters conversation.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_proc_call_connected
(
  const cmcall_s_type            *call_ptr
  /* Pointer to call object which entered conversation.
  */
)
{
  boolean call_is_pkt_data = FALSE;
  boolean call_is_voice    = FALSE;
  boolean svd_is_supported = FALSE;
  cmss_s_type      *ss_ptr = cmss_ptr();
  sys_modem_as_id_e_type  asubs_id = call_ptr->mm_id.asubs_id;
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_ptr;

  if(ss_info_sub_prop_ptr == NULL ||
      ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info == NULL)
  {
    return;
  }
  ss_info_all_stack_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If there is service on main stack which is other than 3GPP2, do not
  ** update IDM
  */
  if ( sys_srv_status_is_srv( ss_info_all_stack_ptr->srv_status  ) &&
       ! ( ss_info_all_stack_ptr->sys_mode == SYS_SYS_MODE_CDMA ||
           ss_info_all_stack_ptr->sys_mode == SYS_SYS_MODE_HDR )
     )
  {
    return;
  }
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  call_is_pkt_data =   cmcall_is_pkt_data_call(call_ptr);
  call_is_voice    = ( call_ptr->call_type == CM_CALL_TYPE_VOICE);

  svd_is_supported = cmss_is_cdma_svd_supported(asubs_id);

  /* Check if it is Hybrid HDR PS data call.
  */
  if ( call_is_pkt_data &&
       cmss_is_stack1_operational(asubs_id) &&
       call_ptr->sys_mode == SYS_SYS_MODE_HDR
     )
  {
    (void)cmssidm_set_idle_digital_mode( SYS_SYS_MODE_HDR, asubs_id );
    cmssidm_update_state( CMSSIDM_STATE_HYBR_HDR, asubs_id );
    return;
  }

  /* Check if it is PS data call or
     if it's voice call and SVD is supported
  */
  if( call_is_pkt_data ||
      (call_is_voice && svd_is_supported))
  {

    (void)cmssidm_set_idle_digital_mode( call_ptr->sys_mode, asubs_id );
    cmssidm_update_state( CMSSIDM_STATE_MAIN, asubs_id );

    /* If we attempted previously on Hybrid HDR and failed,
    ** update the IDM as CDMA and avoid HDR for sometime.
    */
    if ( call_ptr->sr.is_hybr_redialed )
    {
      cmssidm_start_idm_timer( CMSSIDM_AVOID_HANDUP_TIME, asubs_id );
    }
  } /* else of if ( call_ptr->sys_mode) */
} /* cmssidm_proc_call_connected() */



/*===========================================================================
=============================================================================
=============================================================================
==================== CALL ENDED EVENT HANDLER ===============================
=============================================================================
=============================================================================
===========================================================================*/



/*===========================================================================

FUNCTION cmssidm_proc_call_ended

DESCRIPTION
  This function should be called when the call ends.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_proc_call_ended
(
  sys_modem_as_id_e_type  asubs_id
)
{
  cm_mm_ss_info_sub_prop_s_type *ss_info_sub_prop_ptr = cmss_get_info_sub_prop(asubs_id);
  cm_mm_ss_info_gen_stack_info_s_type *ss_info_all_stack_ptr;

  if(ss_info_sub_prop_ptr == NULL ||
      ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info == NULL)
  {
    return;
  }
  ss_info_all_stack_ptr = ss_info_sub_prop_ptr->stack_info[MM_STACK_0].gen_info;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If there is service on main stack which is other than 3GPP2, do not
  ** update IDM
  */
  if ( sys_srv_status_is_srv(ss_info_all_stack_ptr->srv_status ) &&
       ! ( ss_info_all_stack_ptr->sys_mode == SYS_SYS_MODE_CDMA ||
           ss_info_all_stack_ptr->sys_mode == SYS_SYS_MODE_HDR )
     )
  {
    return;
  }
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Start timer to reevaluate the IDM.
  */
#ifdef FEATURE_CM_LTE
  //cmssidm_start_idm_timer( ss_ptr->idm_timer_value );
  cmssidm_reevaluate_IDM(asubs_id);
#else
  cmssidm_start_idm_timer( CMSSIDM_HANDDOWN_TIME, asubs_id );
#endif

} /* cmssidm_proc_call_ended() */


/*===========================================================================

FUNCTION cmssidm_estimate_base_station_idm

DESCRIPTION
  This function should be called when the a data call ends. This function
  does a best effort estimation of the Base Station's IDM. There are some
  cases where the estimation might be wrong, but those cases are limited by
  the lack of service.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_estimate_base_station_idm
(
  const cmcall_s_type  *call_ptr
)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(call_ptr->mm_id.asubs_id);
  sys_sys_mode_e_type call_mode;

  CM_ASSERT( call_ptr != NULL );

  call_mode = call_ptr->sr.orig_sent_on_mode;
  if(ssidm_sub_info_ptr == NULL)
  {
    return;
  }

  /* If the call is a data call, and the call mode is HDR or CDMA then
  ** process the event.
  */
  if( call_ptr->call_type == CM_CALL_TYPE_PS_DATA &&
      ( call_mode == SYS_SYS_MODE_HDR ||
        call_mode == SYS_SYS_MODE_CDMA ) )
  {

    /* See if the data call ended in Conversation state or, if it's a call with
    ** DRS=FALSE that ended with status Release Normal. When these conditions
    ** are met, the Base station would have moved it's IDM to the protocol in
    ** which this call was originated.
    */
    if( call_ptr->call_state == CM_CALL_STATE_CONV ||
        ( call_ptr->end_status == CM_CALL_END_REL_NORMAL &&
          call_ptr->cmcall_mode_info.info.cdma_info.drs_bit == FALSE ) )
    {
      ssidm_sub_info_ptr->cmssidm_bs = call_mode;
    }
  }

  return;
} /* cmssidm_proc_call_ended() */



/*===========================================================================
=============================================================================
=============================================================================
==================== MISC EXTERN FUNCTIONS ==================================
=============================================================================
=============================================================================
===========================================================================*/



/*===========================================================================

FUNCTION cmssidm_get_curr_idm

DESCRIPTION
  This function should be called to get the curr_idm.

DEPENDENCIES
  None.

RETURN VALUE
  sys_sys_mode_e_type - current Idle digital mode.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  sys_sys_mode_e_type            cmssidm_get_curr_idm
(
  sys_modem_as_id_e_type  asubs_id

)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  if(ssidm_sub_info_ptr == NULL)
  {
    return SYS_SYS_MODE_NONE;
  }

  return ssidm_sub_info_ptr->curr_idm;

} /* cmssidm_get_curr_idm() */



/*===========================================================================

FUNCTION cmssidm_get_prev_idm

DESCRIPTION
  This function should be called to get the previous IDM.

DEPENDENCIES
  None.

RETURN VALUE
  sys_sys_mode_e_type - Previous Idle digital mode.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  sys_sys_mode_e_type            cmssidm_get_prev_idm
(
  sys_modem_as_id_e_type  asubs_id
)
{

  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  if(ssidm_sub_info_ptr == NULL)
  {
    return SYS_SYS_MODE_NONE;
  }

  return ssidm_sub_info_ptr->prev_idm;

} /* cmssidm_get_prev_idm() */
/*  NONE
*/

/*===========================================================================

FUNCTION cmssidm_get_base_station_idm

DESCRIPTION
  This function returns CM's best estimate of the base station's IDM.

DEPENDENCIES
  cmssidm_init() should have been called previously.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  sys_sys_mode_e_type            cmssidm_get_base_station_idm
(
  sys_modem_as_id_e_type  asubs_id
)
{
  cmssidm_sub_info_s_type *ssidm_sub_info_ptr = cmssidm_get_sub_info_ptr(asubs_id);
  if(ssidm_sub_info_ptr == NULL)
  {
    return SYS_SYS_MODE_NONE;
  }

  return ssidm_sub_info_ptr->cmssidm_bs;
}



/*===========================================================================
=============================================================================
=============================================================================
==================== INIT FUNCTIONS =========================================
=============================================================================
=============================================================================
===========================================================================*/
#ifdef CM_DEBUG
#error code not present
#endif
/*===========================================================================

FUNCTION cmssidm_sub_info_alloc

DESCRIPTION
  This function should be called to allocate the IDM module.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_sub_info_alloc
(
  void
)
{
  uint8                      i;
  cmssidm_info_s_type *cmssidm_info_ptr   = cmssidm_get_info_ptr();
  uint8                      num_sub = MIN(MAX_SIMS, cmpmprx_get_num_of_sims());

  cmssidm_info_ptr->count = num_sub;

  for(i=0; i<num_sub; i++)
  {
    if(cmssidm_info_ptr->sub_info[i] == NULL)
    {
      cmssidm_info_ptr->sub_info[i] = (cmssidm_sub_info_s_type *)cm_mem_malloc
                                      (sizeof(cmssidm_sub_info_s_type));
    }
  }

}

/*===========================================================================

FUNCTION cmssidm_init

DESCRIPTION
  This function should be called to initialize the IDM module.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                           cmssidm_init
(
  void

)
{

  cmssidm_info_s_type *cmssidm_info_ptr   = cmssidm_get_info_ptr();
  cmssidm_sub_info_s_type *ss_idm_sub_info_ptr;
  uint8 i;

  for(i=0; i<cmssidm_info_ptr->count; i++)
  {
    ss_idm_sub_info_ptr = cmssidm_info_ptr->sub_info[i];
    /* Initialize CMSSIDM state variables.
    */
    ss_idm_sub_info_ptr->state                 = CMSSIDM_STATE_NONE;
    ss_idm_sub_info_ptr->idm_timer_uptime      = CMSSIDM_UPTIME_NONE;
    ss_idm_sub_info_ptr->restore_hdr_uptime    = CMSSIDM_UPTIME_NONE;
    ss_idm_sub_info_ptr->curr_idm              = SYS_SYS_MODE_NO_SRV;
    ss_idm_sub_info_ptr->prev_idm              = SYS_SYS_MODE_NO_SRV;
    ss_idm_sub_info_ptr->is_idm_timer_suspended = FALSE;

    /* Initialize the base station's view of the IDM.
    */
    ss_idm_sub_info_ptr->cmssidm_bs            = SYS_SYS_MODE_NO_SRV;
  }

} /* cmssidm_init() */

#else /* FEATURE_HDR_HANDOFF */

/*===========================================================================
FUNCTION cmssidm_dummy

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
void cmssidm_dummy( void )
/*lint -esym(714,cmssidm_dummy) */
/*lint -esym(765,cmssidm_dummy)
** Can't be static as it results in more compiler warnings
*/
{
}

#endif /* FEATURE_HDR_HANDOFF */
