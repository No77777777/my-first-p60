#ifndef WL1DRXENTITY_H
#define WL1DRXENTITY_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               WL1 DRX Entity

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2007 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1drxentity.h#3 $
  $DateTime: 2020/01/22 08:56:27 $
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
01/31/18   sch     Allow meas gap only if it can accommodate first search
06/05/18   am      Clear PICH timer when DRX_REQ is received.
03/26/18   sch     Changing entity back off time and maximum time to block sleep in min sleep opts
11/11/16   rs      Don't trigger evals when CTCH is waking up before PICH due to collision
07/15/16   kr      Fix issues when inINT column
                   set rf_fw_mdsp flag
                   increase threshold to trigger MIN sleep time OPTs
                   DOnt trigger MIN sleep time Opts if R99 clients are active.
07/1/16    kr      Dont consider CTCH entity if its already blocked by SRCH/BPLMN
06/24/16   kr      Special case to handle when CTCH +Inint column+Srches to happen
06/16/16   skk     F3 Reduction.
05/24/16   skk     Disable Outerloop if PICH and other entity is waking up with in 10ms to avoid PO overran. 
05/13/16   skk     Handling FW/RF resources when sleep duration is less.
01/21/16   skk     Dont handle BPLMN_SLEEP_ABORT_IND from RRC if page wakeup is near by.
12/18/15   sl      Perform BCCH entity schedule update after EDRX de-configuration
11/05/15   kr      Increase the time for checking Collision between entities
10/07/15   rs      CTCH lock management for DSDS
10/07/15   vs      BSIC Id Reconf optimizations for X+W QTA scenarios.
09/28/15   skk     Clear wsleep_open_meas_gap flag while putting Split srch to asleep indef.
08/24/15   sks     Add the 6 ms buffer for wakeup for CTCH/BCCH entities in wakeup cancel
08/03/15   skk     Reduce min duration to trigger entity sleep to 28 msec.
07/18/15   kr      Clear BCH timer in BCH to FACJ transition
07/02/15   sks     Fix a featurization bug wrt BOLT and TH modems.
06/17/15   skk     Trigger sleep through regular code path at the end of BPLMN.
06/15/15   rs      BOLT W+W changes
06/02/15   pv      Remove unused API. 
06/02/15   sks     define entity timer at entity registration and undefine at deregsitration
05/25/15   skk     Check only time to next PO while releasing lock as a part of sending BPLMN BCCH drop CNF to RRC. 
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
05/01/15   kr      FR 26325: CB/BMC support for W/G/L+G MSIM DSDS
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
02/27/15   ap      Initial check-in for WL1 QSH framework 
02/24/15   pv      DRX/WSLEEP code reorg for W+W
02/20/15   rs      Added Mutex protection to avoid race condition when handling deregister-register local cmds 
                   for the same handle
02/06/15   rs      Code changes for requesting lock with new proc during BCH decode
02/06/15   psr     Compiler error fixes for Dual sim
01/19/15   sks     Global re-org for W+W
10/30/14   skk     Reset entity_delayed flag if wakeup is cancelled.
11/21/14   gsk     SRCHZZ wakeup cleanup changes
10/28/14   kr      externing a function
09/05/14   rs      Block PICH when Inter freq reselection is triggered 
09/03/14   skk     Unfreze sample server if PICH doesn't have valid schedule while some other entity is going to sleep.
05/08/14   rs/gsk  code changes to handle CTCH-BPLMN collisions
03/06/14   gsk     Ignore PICH wakeups during interf reselection
01/16/14   srk     Delay cphy_setup_cmd when sccpch drop is in progress.
01/07/14   gsk     Externing API to query if CTCH is blocked by search
01/03/14   rs      Returning the correct value of next entity waking up
01/03/13   gsk     Reverting the changes to delay cphy setup until L1M wakeup
12/16/13   as      Porting G2W TA from Triton to Dime.
08/22/13   kr      changing return type for delayed entity function
09/12/13   jd      Searcher redesign
12/19/13   rs      drx_entity_next_wakeup_time() function declaration added to get 
                   the next entity wakeup time.
08/23/13   dm      Change minimum sleep duration for CTCH to account for 
                   PICH/CTCH collision.
06/29/13   pv      API for resetting the DPC sync counter.
02/11/13   gsk/sr  EDRX Full Sleep initial check in.
09/08/12   pv      Extern for the RPUSH wrapper.
08/29/12   pv      New APIs for PICH timeline opts.
07/16/12   pv      Externed a variable.
07/13/11   pv      Offline PICH/Parallel Reacq fixes.
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
07/05/12   pv      New API needed for Offline PICH/Parallel Reacq.
06/11/12   vs      MBMS feature cleanup.
06/08/12   gv      Feature HS RACH Initial code check in
03/01/11   scm     Give SRCH ability to force CTCH to stay sleeping indefinitely.
08/11/10   scm     drx_entity_wake_any_delayed_entities() returns boolean.
04/08/09   scm     Handle waking an entity right after waking from sleep.
10/03/08   scm     Add ability to drop CTCH because of a sleep abort.
10/03/08   hk      Bringing in 7k Mailine fixes
06/11/08   scm     Extern drx_entity_is_pich_suspended_or_inactive().
05/07/08   scm     Featurize MICH code with FEATURE_MODEM_MBMS.
04/24/08   scm     Eliminate FEATURE_WCDMA_DRX_ENTITY_USE_REX_TIMER define.
04/08/08   scm     PICH/MICH "smallest circle" enhancements.
03/11/08   scm     Move ALL_ENTITIES_ASLEEP define from wl1drxentity.c to .h.
08/17/07   scm     Extern drx_entity_get_cell_info() utility function.
06/13/07   scm     Tweak drx_entity_time_to_next_wakeup_usec() prototype.
                   Extern drx_entity_wake_any_delayed_entities().
04/04/07   scm     Extern drx_entity_time_to_next_wakeup_usec().
03/31/07   scm     Switch some srchzz_xxx API's to srch_xxx.
03/28/07   scm     Support new entity suspend and resume commands.
03/23/07   scm     Remove extern to drx_entity_find_psc_handle().
03/19/07   scm     Implement drx_manager_get_current_entity_mask().
03/03/07   scm     Add extern to drx_entity_get_sleep_info().
02/22/07   scm     Remove extern to ext_wl1drxmanager_cntrl->drx_timeout_timer.
02/15/07   scm     Modify drx_srch_wakeup_done() parameter list.
01/24/07   scm     Initial support of PICH with new DRX Manager.
01/15/07   scm     Support schedule update indication from Channel Manager.
11/16/06   scm     Additional DRX manager functionality.
11/10/06   scm     Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "wcdma_variation.h"
#include "comdef.h"
#include "wl1drxmanager.h"
#include "wsleeptimelinemgr.h"
#include "wl1sleep.h"

#define DRX_ENTITY_REGISTER_MUTEXLOCK() REX_ISR_LOCK(&wl1drxentity_ext_cntrl_params[wsub_id].drx_entity_register_mutex_lock);
#define DRX_ENTITY_REGISTER_MUTEXFREE() REX_ISR_UNLOCK(&wl1drxentity_ext_cntrl_params[wsub_id].drx_entity_register_mutex_lock);

/* The different commands processed by the channel state machines. */
typedef enum
{
  DRX_ENTITY_REGISTER_CMD = 0,
  DRX_ENTITY_DEREGISTER_CMD,
  DRX_ENTITY_SRCH_WAKEUP_RSP,
  DRX_ENTITY_SRCH_TO_SLEEP_RSP,
  DRX_ENTITY_DEMOD_WAKEUP_RSP,
  DRX_ENTITY_DEMOD_TO_SLEEP_RSP,
  DRX_ENTITY_TIMER_EXPIRED_CMD,
  DRX_ENTITY_SCHEDULE_UPDATE_CMD,
  DRX_ENTITY_SUSPEND_CMD,
  DRX_ENTITY_RESUME_CMD,
  DRX_ENTITY_NUM_CMDS
} drx_entity_cmd_id_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;

  boolean                 channel_awake;
} drx_register_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;
} drx_deregister_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type        local_cmd_hdr;

  drx_entity_handle_type       handle;
  drx_entity_cmd_id_type       cmd_id;

  srch_wakeup_set_struct_type  psc_woken_up;
  
} drx_srch_awake_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;

  boolean                 status;
} drx_entity_srch_to_sleep_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;

  boolean                 status;
} drx_demod_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;
} drx_timer_expired_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;
} drx_schedule_update_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;
} drx_suspend_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  l1_local_cmd_hdr_type   local_cmd_hdr;

  drx_entity_handle_type  handle;
  drx_entity_cmd_id_type  cmd_id;
} drx_resume_cmd_type;


typedef union
{
  struct
  {
    /* Need this to put command on l1_local_cmd_q. */
    l1_local_cmd_hdr_type            local_cmd_hdr;

    drx_entity_handle_type           handle;
    drx_entity_cmd_id_type           cmd_id;
  } header;

  drx_register_cmd_type              register_cmd;
  drx_deregister_cmd_type            deregister_cmd;
  drx_srch_awake_cmd_type            srch_awake_cmd;
  drx_entity_srch_to_sleep_cmd_type  srch_to_sleep_cmd;
  drx_demod_cmd_type                 demod_cmd;
  drx_timer_expired_cmd_type         timer_expired_cmd;
  drx_schedule_update_cmd_type       schedule_update_cmd;
  drx_suspend_cmd_type               suspend_cmd;
  drx_resume_cmd_type                resume_cmd;
} drx_entity_cmd_type;

/* Whenever an entity handle is in a state other than INACTIVE or ASLEEP, the
** corresponding bit in this mask shall be set to one.  In other words, if
** awake, set the bit, otherwise clear the bit.  So when this mask is 0x00,
** all channels are asleep, and we may put the phone to sleep. */
#define ALL_ENTITIES_ASLEEP  0x00

#define DRX_MINIMUM_UE_SLEEP_TIME_USEC            30000
#define DRX_UNOPT_MINIMUM_UE_SLEEP_TIME_USEC      (DRX_MINIMUM_UE_SLEEP_TIME_USEC + 20000)

/*Add an extra buffer of 3ms to take care of the delay from DL to Sleep point to gts.*/
#define DRX_MINIMUM_UE_SLEEP_TIME_USEC_WITH_BUFF(handle_waking_up)  \
            (DRX_MINIMUM_UE_SLEEP_TIME_USEC + ((RF_WAKE_UP_TIMER_IN_MS(handle_waking_up)) * 1000) + 3000)

/*
** This is the minimum value to put RF/FW to sleep increasing from 15 to 17 to account TRM apis also.
** Applicable only for TA
*/
#define DRX_MINIMUM_RF_FW_SLEEP_TIME_USEC       18000

/*At the end of BPLMN as a part of BCCH drop lock will be released but if enough time
  is not available to sleep then we will not request for lock again. So dont release
  lock in first place if enough time is not available.*/
#define DRX_SLEEP_THRESHOLD_AT_BPLMN_END                (DRX_MINIMUM_UE_SLEEP_TIME_USEC + 4000)

/*RRC will send BPLMN_SLEEP_ABORT_IND when L1 is in sleep to make l1 call can_l1_go_to_sleep
  and start BPLMN. As this ext cmd is no lock cmd, if wakeup is near by, sleep will be blocked
  after handling this cmd and we will miss page. So ignore the cmd if PICH is near by. After
  PICH done can_l1_go_to_sleep is anyway called. From the time we get this cmd we should do 
  sleep abort, wakeup and then trigger sleep again. So all the below macros.*/
#define DRX_SLEEP_THRESHOLD_FOR_BPLMN_SLEEP_ABORT_IND   (DRX_MINIMUM_UE_SLEEP_TIME_USEC + MIN_TIME_TO_WAKEUP_FOR_ABORT + RPM_DELAY_MS)

/*Lock reserve reason at sleep is decided based on entity timers but at wakeup lock 
  request happens at MWU ISR which can happen 20ms on either side of PICH timer expiry
  because of outer loop adjustment. If MWU ISR is programmed after PICH timer expiry
  then at wakeup a collision might be detected and lock can be requested with a diff
  priority.So increase the collision window considering outerloop adjustment.*/
#define DRX_ENTITY_COLLISION_WINDOW  DRX_MINIMUM_UE_SLEEP_TIME_USEC + (WSLEEP_TIMELINE_MAX_ADJ_SLP_LNGR_MS * 1000)

/* If PICH is waking first, and there's another entity waking this close behind
** it, do not optimize the timeline for PICH.  With another entity this close
** to PICH, we must loosen the timeline a bit to wake both entities in time. */
#define DRX_ENTITY_MIN_ENTITY_DIFF_USEC  10000

/* Reduce messaging between RF wakeup and PICH setup.
   SCCPCH state transitions will be printed only if UE is paged.
   1) Avoid PICH entity state change prints while waking up.
   2) Avoid printing SCCPCH state changes if UE is not paged.
   3) Print only -> AWAKE or -> ASLEEP transitions in DCH and FACH for any entity.
   4) Print all state transitions for EDRX.  */
#define WL1_DRX_STATE_CHANGE_MSG3(str, a, b, c) \
  if (((!((a == (drx_entity_handle_type)DRX_ENTITY_PICH) && \
          (c != DRX_ENTITY_INACTIVE)                     && \
          DRX_ENTITY_IS_WAKING_UP()) \
       ) && \
       (!((a == (drx_entity_handle_type)DRX_ENTITY_PCCH) && \
          (ext_drx_cntrl->drx_rxd_pch.page_received_this_drx == FALSE)) \
       ) && \
       (!(((WL1_GET_L1M_STATE() == L1M_DCH) || (WL1_GET_L1M_STATE() == L1M_FACH)) && \
          (c != DRX_ENTITY_ASLEEP) && (c != DRX_ENTITY_AWAKE)) \
       ) \
      ) || \
      (a == (drx_entity_handle_type)DRX_ENTITY_EDRX) \
     ) \
  { \
    WL1_MSG3(ERROR,str,a,b,c); \
  } \

/* SRCH can block CTCH activity if it wants to, like for inter-RAT searches.
   Similarly BPLMN start blocks CTCH and BPLMN stop unblocks CTCH 
** Maintain that state here. */
typedef enum
{
  DRX_ENTITY_CTCH_NORMAL_BEHAVIOR    = 0,
  DRX_ENTITY_CTCH_DROPPING_FOR_SRCH  = 0x1,
  DRX_ENTITY_CTCH_DISABLED_BY_SRCH   = 0x2,
  DRX_ENTITY_CTCH_DROPPING_FOR_BPLMN = 0x4,
  DRX_ENTITY_CTCH_DISABLED_BY_BPLMN  = 0x8        
} drx_entity_ctch_disabled_type;

typedef enum
{
  ENTITY_COLLISION_WITHIN_WINDOW,
  ENTITY_WAKEUP_CHANGED_DUE_TO_COLLISION,
}drx_entity_collision_type;

typedef struct
{
  #ifdef FEATURE_DUAL_SIM
  boolean widle_g2w_ta_rel_res_aftr_resel_fail;
  #endif
  boolean wl1drxentity_modify_activity_fail_at_wakeup;
  drx_entity_ctch_disabled_type drx_entity_disable_ctch_for_other_activities;
  boolean wl1drxentity_bypass_qxt_notf_nxt_cycle;
  /* Variable tracking if DRX manager pends srch entity drop 
** due to ongoing parallel reacq.*/
  boolean wl1drxentity_pended_srch_pich_entity_drop;
  /* Whenever an entity handle is in a state other than INACTIVE or ASLEEP, the
** corresponding bit in this mask shall be set to one.  In other words, if
** awake, set the bit, otherwise clear the bit.  So when this mask is 0x00,
** all channels are asleep, and we may put the phone to sleep. */
  uint32 drx_handles_awake_mask;
  uint32 drx_previous_handles_awake_mask; 
  /* Mutex to prevent clearing of entity db when a back to back deregister - register
  local cmd is posted */
  rex_crit_sect_type drx_entity_register_mutex_lock;

  rex_timer_type drx_entity_meas_gap_lock_guard_timer;
}wl1drxentity_ext_cntrl_params_type;

extern wl1drxentity_ext_cntrl_params_type wl1drxentity_ext_cntrl_params[WCDMA_NUM_SUBS];

#define DRX_FORCE_ENTITY_TO_SLEEP()         (wl1drxentity_ext_cntrl_params[wsub_id].wl1drxentity_modify_activity_fail_at_wakeup)
#define DRX_SET_FORCE_ENTITY_TO_SLEEP(val)  (wl1drxentity_ext_cntrl_params[wsub_id].wl1drxentity_modify_activity_fail_at_wakeup = val)
/*===========================================================================
FUNCTION     DRX_ENTITY_TIME_TO_NEXT_WAKEUP_USEC

DESCRIPTION
  This function is called when all channels are sleeping or inactive.  It
  returns the time in microseconds until the next entity wakes up.  It also
  returns the handle of that entity, so the caller can know why we are
  waking up.

DEPENDENCIES
  None.

PARAMETERS
  handle_waking_up - Parameter used to return the entity handle we will
                     be waking up for.

RETURN VALUE
  int32 - Microseconds until next wakeup.
===========================================================================*/
extern int32 drx_entity_time_to_next_wakeup_usec
(
  drx_entity_handle_type *handle_waking_up
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_PROCESS_CMD

DESCRIPTION
  This function routes incoming commands to the proper logical entity state
  machine for processing and possible state change.  There are 3 main types
  of commands handled here:
  (1) DEMOD commands and responses
  (2) SRCH responses
  (3) Activity or inactivity timer expiration.

DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr - Command to this logical entity's state machine.

RETURN VALUE
  TRUE on success.  FALSE on failure.
===========================================================================*/
extern boolean drx_entity_process_cmd(const drx_entity_cmd_type  *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_ALLOCATE_HANDLE

DESCRIPTION
  This function attempts to allocate a handle for a new logical entity.  If
  it successfully allocates a handle, it initializes the data associated
  with that handle.

DEPENDENCIES
  None.

PARAMETERS
  logical_entity_type - Channel type (PICH, MTCH, etc.) identifying this channel.
  dl_uarfcn           - DL channel number of cell carrying this channel
                        (maps to DL frequency of this cell).
  psc                 - Primary scrambling code of cell carrying this channel.
  cb_functions        - Pointer to structure of callback functions.

RETURN VALUE
  handle - Uniquely identifies this channel (or logical entity).  Returns
           DRX_ENTITY_HANDLE_INVALID on failure.
===========================================================================*/
extern drx_entity_handle_type drx_entity_allocate_handle
(
  drx_entity_type          entity_type,
  uint16                   dl_uarfcn,
  uint16                   psc,
  const channel_cb_struct_type  *cb_functions
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_WAKE_ANY_DELAYED_ENTITIES

DESCRIPTION
  There are currently two ways an entity that wants to wake up is not
  allowed to wake from sleep state:

  (1) A Background PLMN search is active.  It may be on a different frequency,
      so we can't allow the sleeping entity to wake on the serving frequency.
  (2) mDSP and RF are not yet active.  We power down during true sleep.  If an
      entity wants to wake while we are in true sleep, it must wait a bit.

  This function is called to kick any delayed entities back to life because
  WL1 is now in a state to support entities going active.  If no entities have
  been delayed, then this function takes no action.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
 Mask having all the entities delayed 
===========================================================================*/
extern uint32 drx_entity_wake_any_delayed_entities( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_CHECK_FOR_CTCH_DROP

DESCRIPTION
  This function is called during a sleep abort caused by RRC command reception
  in WL1.  With FEATURE_WCDMA_NO_CTCH_IF_ANOTHER_ENTITY_ACTIVE feature, CTCH
  cannot be active if any other entity is active.  So this function will
  initiate the CTCH drop caused by a sleep abort.

  Without FEATURE_WCDMA_NO_CTCH_IF_ANOTHER_ENTITY_ACTIVE feature, this
  function does nothing.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean - TRUE if we have started CTCH drop for sleep abort.
          \ FALSE if we have not.
===========================================================================*/
extern boolean drx_entity_check_for_ctch_drop(void);

/*===========================================================================
FUNCTION     DRX_ENTITY_SET_TARGET_ENTITY

DESCRIPTION
  When DRX Manager puts the UE to sleep, it calls this function to set the
  entity handle that we will wake up for in the future.  This would be the
  first entity to wake up of all sleeping entities.  Of course, usually
  it's just PICH.

DEPENDENCIES
  None.

PARAMETERS
  drx_entity_handle_type handle_waking_up - Entity waking next.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_set_target_entity(drx_entity_handle_type handle_waking_up, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_GET_TARGET_ENTITY

DESCRIPTION
  This function returns the next entity waking  up.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  drx_entity_handle_type handle_waking_up - Entity waking next.
===========================================================================*/
extern drx_entity_handle_type drx_entity_get_target_entity( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_WAKE_TARGET_ENTITY

DESCRIPTION
  UE has just woken up from true sleep.  Since RF is on now, etc., we can
  actually wake up channel(s).  This function wakes the entity/channel that
  wants to be awake next.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_wake_target_entity( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_FIND_TYPE_HANDLE

DESCRIPTION
  This function returns the entity handle based on its type.  This function
  should only be used on entities that can only have a single instance, like
  PICH, MCCH, etc.  It should not be used on MTCH for example.

DEPENDENCIES
  None.

PARAMETERS
  entity_type - i.e. DRX_ENTITY_PICH, DRX_ENTITY_CTCH, etc.

RETURN VALUE
  drx_entity_handle_type - Handle identifying this logical entity.
===========================================================================*/
extern drx_entity_handle_type drx_entity_find_type_handle
(
  drx_entity_type entity_type
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_WHICH_TYPE

DESCRIPTION
  This function returns the entity type based on its handle.

DEPENDENCIES
  None.

PARAMETERS
  handle - Handle identifying this logical entity.

RETURN VALUE
  drx_entity_type - i.e. DRX_ENTITY_PICH, DRX_ENTITY_CTCH, etc.
===========================================================================*/
extern drx_entity_type drx_entity_which_type(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_GET_SLEEP_INFO

DESCRIPTION
  This function returns a pointer to the sleep information structure
  corresponding to the passed-in logical entity handle.

  Note that it doesn't check if this handle has actually been allocated.
  That is, only use this function on handles returned by
  drx_entity_allocate_handle();

DEPENDENCIES
  None.

PARAMETERS
  handle - Handle identifying this logical entity.

RETURN VALUE
  drx_manager_sleep_struct_type* - Pointer to sleep info.
===========================================================================*/
extern drx_manager_sleep_struct_type *drx_entity_get_sleep_info
(
  drx_entity_handle_type handle
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_GET_SLEEP_INFO

DESCRIPTION
  This function returns a pointer to the cell information structure
  corresponding to the passed-in logical entity handle.  This structure is
  filled with the primary scrambling code and frequency associated with the
  passed-in logical entity handle.

DEPENDENCIES
  None.

PARAMETERS
  handle - Handle identifying this logical entity.

RETURN VALUE
  *cell_info_ptr - Pointer to cell info.
===========================================================================*/
extern boolean drx_entity_get_cell_info
(
  drx_entity_handle_type     handle,
  tlm_cell_info_struct_type *cell_info_ptr
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_IS_PICH_SUSPENDED_OR_INACTIVE

DESCRIPTION
  This function pretty much just returns whether or not PICH is operational
  in WL1.  If DRX_ENTITY_PICH is suspended or not registered with DRX, this
  function returns TRUE.  Otherwise it returns FALSE.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE if PICH is suspended or inactive.
  FALSE otherwise, i.e. PICH is awake or asleep.
===========================================================================*/
extern boolean drx_entity_is_pich_suspended_or_inactive( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION     DRX_ENTITY_IS_CTCH_UP

DESCRIPTION
  SRCH calls this function to check if CTCH is awake.  If so, we must drop it
  before SRCH can do stuff like inter-RAT neighbor searches.  So this function
  starts CTCH sleep and tells SRCH to wait for completion before moving on.

  If CTCH not currently awake, set flag so it won't wake up later.  Also tell
  SRCH that CTCH not awake.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - CTCH currently awake, and sleep procedure started.
  FALSE - CTCH not currently awake.
===========================================================================*/
extern boolean drx_entity_is_ctch_up( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_UNBLOCK_CTCH

DESCRIPTION
  SRCH no longer needs CTCH to stay sleeping, so resume normal CTCH behavior.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_unblock_ctch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_RESET_ALL_ENTITIES

DESCRIPTION
  Called when WCDMA L1 is shutting down.  Makes sure all timers are stopped,
  and logical entity database gets reset.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_reset_all_entities( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_INIT

DESCRIPTION
  Logical entity database initializations performed whenever WCDMA is
  activated.  Resets all database entries and allocates a timer for each
  entry in the database.

  Must be called ONE TIME, at power-up only.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     drx_entity_is_entity_only_active

DESCRIPTION
  This function returns the number of active entities.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
boolean drx_entity_is_entity_only_active(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     drx_entity_is_entity_awake

DESCRIPTION
  This function is used to verify if entity is in 'AWAKE' state

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern boolean drx_entity_is_entity_awake(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     drx_return_drx_entity_type_for_nsch

DESCRIPTION
  Returns the DRX entity type corresponding to the Phy chan type passed as argument.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  DRX entity type
===========================================================================*/
extern drx_entity_type drx_return_drx_entity_type_for_nsch(l1_dl_phychan_enum_type phch_type);

/*===========================================================================
FUNCTION     DRX_PICH_OPT_ENTITY_OPS

DESCRIPTION
  This function performs the operations for PICH opts.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_pich_opt_entity_ops( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PROCESS_DPC_SYNC_PROC_OPS

DESCRIPTION
  This function performs synchronization between processing
  in different HW threads.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_process_dpc_sync_proc_ops( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     GET_PICH_FREQ

DESCRIPTION
  This function gets the frequency for the registered PICH entity.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern uint16 get_pich_freq( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     GET_PICH_PSC

DESCRIPTION
  This function gets the psc for the registered PICH entity.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern uint16 get_pich_psc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1DRXENTITY_PERF_RFS_UPDATE_RPUSH_WRAPPER

DESCRIPTION
  This function queues up RGS update and RPUSH in DPC task.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/

extern void wl1drxentity_perf_rgs_update_rpush_wrapper( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_RESET_DPC_SYNC_COUNTER

DESCRIPTION
  This function resets the DPC sync counter used to synchronize parallel threads 
  during wakeup.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_reset_dpc_sync_counter( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_HS_FACH_DRX
/*===========================================================================
FUNCTION     DRX_ENTITY_CLR_TIMER_FOR_EDRX

DESCRIPTION
  Just a wrapper function to the proper subsystem when clearing and stopping
  the timer associated with a logical entity.

DEPENDENCIES
  None.

PARAMETERS
  Handle - Logical entity whose timer to clear/stop.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_clr_timer_for_edrx(drx_entity_handle_type handle, wsub_id_e_type wsub_id);
#endif /*FEATURE_WCDMA_HS_FACH_DRX*/

/*===========================================================================
FUNCTION     DRX_ENTITY_TO_BE_AWAKE

DESCRIPTION
  Just a wrapper function to to know which Handle/entity to be awake.

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
Handle to be awakew next
===========================================================================*/
extern uint8 drx_entity_to_be_awake( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_VERIFY_TARGET_ENTITY_HANDLE

DESCRIPTION
  The function checks if the entity handle passed into it is the target entity 
  waking up.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE/FALSS.
===========================================================================*/

extern boolean drx_entity_verify_target_entity_handle(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_RETURN_CTCH_MIN_SLEEP_DURATION

DESCRIPTION
  API which returns the minimum sleep duration for CTCH. 
  It is max(4, 2 x CTCH/PICH collision time).

DEPENDENCIES
  None.

PARAMETERS
 None

RETURN VALUE
max(4, 2 x CTCH/PICH collision time)
===========================================================================*/
extern uint8 drx_return_ctch_min_sleep_duration( wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION     DRX_ENTITY_PUT_ENTITY_TO_SLEEP

DESCRIPTION
  API puts the entity to sleep by changing entity
  state to DRX_ENTITY_ASLEEP.

DEPENDENCIES
  None.

PARAMETERS
drx_entity_handle_type handle

RETURN VALUE
TRUE - entity put to sleep successfully
FALSE - entity not put to sleep.
===========================================================================*/
extern boolean drx_entity_put_entity_to_sleep(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_GET_SLEEP_TYPE

DESCRIPTION
  API will return the current sleep type of the handle.

DEPENDENCIES
  None.

PARAMETERS
 drx_entity_handle_type

RETURN VALUE
drx_manager_sleep_type
===========================================================================*/
extern drx_manager_sleep_type drx_entity_get_sleep_type(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_CHECK_ENTITY_ASLEEP

DESCRIPTION
  API checks whether the entity is asleep or not.

DEPENDENCIES
  None.

PARAMETERS
 drx_entity_handle_type

RETURN VALUE
TRUE - entity state is asleep
FALSE - entity state is not asleep
===========================================================================*/
extern boolean drx_entity_check_entity_asleep(drx_entity_handle_type handle, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION     DRX_ENTITY_NEXT_WAKEUP_TIME

DESCRIPTION
  This function is called by SRCH to ask DRX Manager when one or more channels
  are going to wake up next.  SRCH passes in a bitmask of channels for DRX
  Manager to check.  This function will return the wakeup time of the channel
  that wants to wake up the soonest.

  Some bitmasks are DRX_ENTITY_MASK_PICH, DRX_ENTITY_MASK_MTCH, etc.

  Also the macro DRX_ENTITY_TYPE_TO_MASK(type) can be used to convert the
  enumerated type "drx_entity_type" into a bitmask.

  At the moment, this function only supports serving cell channels.
  TODO - add support for non serving cell channels.

DEPENDENCIES
  None.

PARAMETERS
  entity_bit_mask                - Bitmask of channels to check.
  next_wakeup (output paramater) - Absolute time (SFN/chipx8) of next wakeup.

RETURN VALUE
  int32 - Duration until next wakeup in microseconds.
===========================================================================*/
extern int32 drx_entity_next_wakeup_time
(
  uint32                          entity_bit_mask,
  drx_manager_sleep_struct_type  *next_wakeup,
  int16 *entity_wakingup_soonest
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_CHECK_IF_CTCH_BLOCKED_BY_SRCH

DESCRIPTION 
  API checks if CTCH is blocked by search. 

DEPENDENCIES
  None.

PARAMETERS 
  None 

RETURN VALUE 
  TRUE -- If CTCH is blocked by search
  FALSE -- otherwise

===========================================================================*/
extern boolean drx_entity_check_if_ctch_blocked_by_srch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     drx_is_entity_awake

DESCRIPTION
  API which checks wl1drxentity_ext_cntrl_params.drx_handles_awake_mask and returns if an entity is awake

DEPENDENCIES
  None.

PARAMETERS
  Entity in handle mask format

RETURN VALUE
  TRUE  -- Entity is not asleep
  FALSE -- Entity is asleep
===========================================================================*/
extern boolean drx_is_entity_awake(uint32 handle_mask, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_ADJUST_ENTITY_SCHEDULE

DESCRIPTION
  API avoids the next wakeup of PICH/SPLIT SRCH entities if its not needed

DEPENDENCIES
  None.

PARAMETERS
  Entity in handle mask format

RETURN VALUE
  TRUE  -- Entity is not asleep
  FALSE -- Entity is asleep
===========================================================================*/
void drx_entity_adjust_entity_schedule(drx_entity_handle_type *next_wkup_entity_handle, int32 *time_to_next_wakeup_usec, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_POST_ENTITY_PROCESS_CMD

DESCRIPTION
  API is used by drx manager to post an entity process command to drx entity module

DEPENDENCIES
  None.

PARAMETERS
  Command info

RETURN VALUE
  None
===========================================================================*/
extern void drx_entity_post_entity_process_cmd(drx_entity_cmd_type *cmd_ptr,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_CLR_PICH_ENTITY_TMR_IF_RESEL_IN_PROG

DESCRIPTION
  API used to clear the PICH entity timer when Inter Freq reselection is
  triggered 

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None
===========================================================================*/
void drx_clr_pich_entity_tmr_if_resel_in_prog( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_RESET_ENTITY_DELAYED

DESCRIPTION
  This function is used to handle special cases where the entity_delayed flag
  for an entity needs to be cleared in an asynchronous manner.

DEPENDENCIES
  None.

PARAMETERS
  handle - Uniquely identifies channel shutting down.

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_reset_entity_delayed(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_IS_PO_FAR_AWAY

DESCRIPTION
  API will check if a Paging Occasion is nearby.

DEPENDENCIES
  None.

PARAMETERS
  uint16 usec_to_wakeup - threshold to check for wakeup.

RETURN VALUE
  FALSE - Paging Occasion is near by
  TRUE  - Paging Occasion is far away.
===========================================================================*/
extern boolean drx_entity_is_po_far_away(uint16 usec_to_wakeup, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     drx_entity_chk_entity_collision

DESCRIPTION
  This function checks if two handle wakeups are within collision window

DEPENDENCIES
  None

PARAMETERS
  handles whole collision have to be checked,sub id


RETURN VALUE
  TRUE/FALSE
===========================================================================*/
extern boolean drx_entity_chk_entity_collision(drx_entity_handle_type base_handle, drx_entity_handle_type target_handle , 
                                               uint16 entity_tti_info, drx_entity_collision_type collision_type,
                                               int32 *next_entity_wakeup_usec, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     drx_entity_get_target_sfn

DESCRIPTION
  This function provides the target SFN for CTCH


DEPENDENCIES
None

PARAMETERS
  handle waking up(here CTCH),sub id

RETURN VALUE
  Target SFN
===========================================================================*/
extern uint16 drx_entity_get_target_sfn(drx_entity_handle_type entity_wants_wakeup ,wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     DRX_ENTITY_SET_ENTITY_TIMER

DESCRIPTION
  This API is used to set PICH timer in wakeup cancel scenarios.

DEPENDENCIES
  None.

PARAMETERS
  uint32 duration

RETURN VALUE
  None
===========================================================================*/
extern void drx_entity_set_entity_timer(drx_entity_handle_type handle, uint32 sleep_duration_us, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_HANDLE_WAKEUP_CANCEL

DESCRIPTION
  Will cancel the current entity wakeup, computes the wakeup time for
  next entity and programs sleep controller and reserves TRM lock.
 
DEPENDENCIES
  None.

PARAMETERS
   drx_entity_handle_type wakeup_cancel_handle     -  Current entity for whose wakeup is cancelled.
   int32 *time_to_next_wakeup_usec                 -  Time to next wakeup in usec.
   uint32 sfn_now                                  -  Current SFN.
   wsub_id_e_type wsub_id                          -  sub id

RETURN VALUE
  TRUE/FALSE
===========================================================================*/
extern drx_entity_handle_type drx_entity_handle_wakeup_cancel
(
   drx_entity_handle_type wakeup_cancel_handle,
   int32 *time_to_next_wakeup_usec,
   uint32 sfn_now,
   wsub_id_e_type wsub_id
);

extern void wl1drxentity_int_cntrl_params_init( wsub_id_e_type wsub_id);
extern void wl1drxentity_ext_cntrl_params_init( wsub_id_e_type wsub_id);
extern void wl1drxentity_init_target_entity_handle(wsub_id_e_type wsub_id);

extern void wl1drxentity_global_params_init(void);

#ifdef FEATURE_WCDMA_QSH_DUMP
extern void wl1_drx_entity_get_debug_info(wl1_drx_sleep_mini_debug_info_struct_type* obj);
#endif

/*===================================================================================
FUNCTION     DRX_VERIFY_MEAS_NOT_CLOSE_TO_PICH

DESCRIPTION
  check if Meas reserve/req and notify can be made. 
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE - PICH is waking up next and we are more than 180 ms from it.
  FALSE- either PICH not waking up next, or waking up too close. So do not try to open a gap for MEAS.
======================================================================================*/
extern boolean drx_entity_verify_meas_not_close_to_pich(wsub_id_e_type wsub_id);

/*===================================================================================
FUNCTION     DRX_ENTITY_CLR_SPLIT_SEARCH_TIMER 

DESCRIPTION
  Clears SPL-S entity timer and puts it to indefinite sleep. has to be done when:
    there are pending bands to be measured while going to sleep and the other tech is in idle, so a req and notify has to be done.
  
DEPENDENCIES
  None.

PARAMETERS
  boolean lock_req_not_pending - TRUE always except in idle+idle
                                 when req_notify needs to be done for split srch wakeup.

RETURN VALUE
  No  
 
======================================================================================*/
extern void drx_entity_force_split_srch_to_sleep(boolean lock_req_not_pending, wsub_id_e_type wsub_id);

/*===================================================================================
FUNCTION     DRX_ENTITY_GET_TIME_LEFT_FOR_WAKEUP 

DESCRIPTION
  Returns the time left for ENTITY to wake up.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  PICH entity timer time in usec.  
 
======================================================================================*/
extern int32 drx_entity_get_time_left_for_wakeup(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===================================================================================
FUNCTION     DRX_ENTITY_GENERATE_COLLISION_MASK_FOR_SPLS 

DESCRIPTION
  Generates a mask of PICH, BCCH and CTCH entities. 
  Used in deciding to open a meas gap.
  
DEPENDENCIES
  None.

PARAMETERS
  uint32 Entity mask.

RETURN VALUE
  None.  
 
======================================================================================*/

extern void drx_entity_generate_collision_mask_for_spls(uint32 *mask);

#if (defined (FEATURE_WCDMA_BOLT_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)) && !(defined(FEATURE_WCDMA_THOR_MODEM))
/*===========================================================================
FUNCTION     DRX_ENTITY_DEFINE_ENTITY_TIMER

DESCRIPTION
  Defines a timer. will be called whenever an entity is registered.
 
DEPENDENCIES
  None.

PARAMETERS
   entity handle

RETURN VALUE
  none
===========================================================================*/

extern void drx_entity_define_entity_timer(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_DUNEFINE_ENTITY_TIMER

DESCRIPTION
  Undefines a timer. will be called whenever an entity is deregistered.
 
DEPENDENCIES
  None.

PARAMETERS
   entity handle

RETURN VALUE
  none
===========================================================================*/
extern void drx_entity_undefine_entity_timer(drx_entity_handle_type handle, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION     DRX_ENTITY_CHECK_FOR_SLEEP

DESCRIPTION
  This function is called when a channel goes to sleep or is deactivated.
  It checks to see if all channels are now sleeping or inactive.  If so, the
  UE may be put to sleep to save power.

DEPENDENCIES
  None.

PARAMETERS
  is_r99_handle - TRUE if the channel just put to sleep is R99.
                \ FALSE if channel is MBMS (MCCH or MTCH).
RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_check_for_sleep( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_TRIGGER_SLEEP

DESCRIPTION
  This function is called when a channel goes to sleep or is deactivated.
  It checks to see if all channels are now sleeping or inactive.  If so, the
  UE may be put to sleep to save power.

DEPENDENCIES
  None.

PARAMETERS
  is_r99_handle - TRUE if the channel just put to sleep is R99.
                \ FALSE if channel is MBMS (MCCH or MTCH).
RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_trigger_sleep( int32  time, drx_entity_handle_type handle,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     drx_entity_force_schedule_update

DESCRIPTION
  This function is called when we have BCH-->FACH transition or EDRX de-configuration
  So as to make sure BCH timer is cleared before entering FACH
[as sometime it may happen in FACH inside QTA BCH timer expires].

DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.
===========================================================================*/
extern void drx_entity_force_schedule_update(drx_entity_handle_type handle,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1DRXENTITY_RETURN_ENTITY_BUFFR_TIME

DESCRIPTION
  Returns entity specific wakeup-buffer time in us for resource warmup.
 
DEPENDENCIES
  None.

PARAMETERS
   entity handle

RETURN VALUE
   backoff timer in us.
===========================================================================*/
extern uint16 wl1drxentity_return_entity_buffer_time(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_ENTITY_IS_ENTITY_AWAKE

DESCRIPTION
  Function used to check if the entity corresponding to the handle passed
  is already awake with DRX

DEPENDENCIES
  None.

PARAMETERS
  Handle

RETURN VALUE
  TRUE   - If entity is awake
  FASLE  - otherwise
===========================================================================*/
extern boolean drx_entity_is_entity_awake(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1_GET_ENTITY_LOCK_DURATION_IN_FRAMES

DESCRIPTION
    This function returns the approx duration in frames for which the
    entity handle passed to it will be active

PARAMETERS
     Entity handle and SUB ID

DEPENDENCIES
     None

RETURN VALUE
     reserve/request duration in frames
=========================================================================*/
extern uint16 wl1_get_entity_lock_duration_in_frames(drx_entity_handle_type handle,wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    drx_entity_adjust_ctch_schedule

DESCRIPTION
   This function updates CTCH schedule and clears timer if blocked by SRCH/BPLMN

PARAMETERS
     Entity handle and SUB ID

DEPENDENCIES
     None

RETURN VALUE
     reserve/request duration in frames
=========================================================================*/
extern void drx_entity_adjust_ctch_schedule(wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     DRX_ENTITY_CHECK_IF_ENTITY_HAS_SCHEDULE

DESCRIPTION
  This helper function checks to see if the entity handle passed to this function
  has any shedule with the DRX manager. 
 
DEPENDENCIES
  None.

PARAMETERS
   none

RETURN VALUE
   TRUE  - Entity passed has a schedule with DRX manager
   FALSE - Entity passed has no schedule
===========================================================================*/
extern boolean drx_entity_check_if_entity_has_schedule(drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_CLR_PICH_TMR

DESCRIPTION
  API used to clear the PICH entity timer.
 
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None
===========================================================================*/

extern void drx_clr_pich_tmr( wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_wl1drxentity_params

DESCRIPTION
  Wrapper

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_wl1drxentity_params(wsub_id_e_type wsub_id);
#endif
#endif  /* #ifndef WL1DRXENTITY_H */
