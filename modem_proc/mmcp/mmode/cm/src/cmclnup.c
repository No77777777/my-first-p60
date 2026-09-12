/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C A L L   M A N A G E R   C L E A N U P   M O D U L E

GENERAL DESCRIPTION
  This module handles the IMS deregistration and DS cleanup needed for
  graceful SIM refresh, operating mode change operations

  This module is responsible for:
  1. Maintain a State machine independent of CM command procesing for IMS
     deregistration and DS cleanup.
  2. Maintain a seperate Buffer queue to hold the CM commands which need the
     above mentioned operations to conclude successfully before proceeding.

      ALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  cm_clnup_init() must be called before invoking any other function in this
  module.

Copyright (c) 2014 by QUALCOMM TECHNOLOGIES INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmclnup.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/30/14   ak      Create file.

===========================================================================*/

#include "mmcp_variation.h"
#include "customer.h"       /* Customer configuration file */
#include "comdef.h"         /* Definition for basic types and macros */
#include "cmidbg.h"
#include "cmph.h"
#include "cmipappi.h"
#include "cmtaski.h"
#include "cmclnup.h"
#include "cmtaski.h"
#include "cmsds.h"
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "cmdbg_qsh.h"
#endif
#include "dsmgr_api.h" 
/* CM Cleanup state machine.
*/


static cm_cmd_q_type      cm_buff_cmd_q;
/* Queue to store clients commands. Clients command are temporarily
** being queued in this queue and then serviced in a FIFO order after
** cleanup is completed.
*/

static cm_clnup_state_s_type cmclnup_state_machine;
/* Cleanup state machine */

/*===========================================================================

FUNCTION cm_buff_cmd_q_init

DESCRIPTION
  Initialize the client command queue.

  This function must be called before the buffer command queue is used in
  any way, place, or form.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                   cm_buff_cmd_q_init( void )
{
  (void) q_init( &cm_buff_cmd_q );
}

/*===========================================================================

FUNCTION cm_buff_cmd_q_cnt

DESCRIPTION
  Determine if there are any commands in the buffer command queue.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  TRUE: Buffer Queue is empty
  FALSE: Buffer Queue contains commands.

SIDE EFFECTS
  none

===========================================================================*/
boolean                cmclnup_is_buff_q_empty( )
{
  return (q_cnt( &cm_buff_cmd_q ) == 0 );
}

/*===========================================================================

FUNCTION cm_buff_cmd_q_get

DESCRIPTION
  Dequeue a command buffer from the client command queue.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  A pointer to the dequeued command buffer.
  If the Q is empty A NULL is returned.

SIDE EFFECTS
  none

===========================================================================*/
cm_cmd_type           *cm_buff_cmd_q_get(  )
/*lint -esym(765,cm_buff_cmd_q_get)
** Can't be static, CM unit test uses it
*/
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If the CM client command queue is empty, return without doing anything.
  */
  if( cmclnup_is_buff_q_empty() )
  {
    return NULL;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Get next command from the CM client command queue.
  */
  return (cm_cmd_type *)q_get( &cm_buff_cmd_q );

}

/*===========================================================================

FUNCTION cm_buff_cmd_q_put

DESCRIPTION
  Enqueue a command buffer onto the client command queue.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                   cm_buff_cmd_q_put(

  cm_cmd_type    *cmd_ptr
  /* pointer to a command buffer to be enqueued */
)
{

  CM_MSG_LOW_1("START cm_buff_cmd_q_put(), cmd_type=%d",cmd_ptr->cmd_type );

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Enqueue the command buffer into the specified queue.
  */
  (void) q_link( cmd_ptr, &cmd_ptr->link);
  q_put( &cm_buff_cmd_q, &cmd_ptr->link );

}

/*===========================================================================

FUNCTION cmclnup_ptr

DESCRIPTION
  Return a pointer to the CM cleanup state machine.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
cm_clnup_state_s_type        *cmclnup_ptr( void )
{
  return &cmclnup_state_machine;
} /* cmclnup_ptr() */

/*===========================================================================

FUNCTION cmclnup_get_curr_uptime

DESCRIPTION
  Get the current CM CLNUP Uptimer

DEPENDENCIES
 

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/


dword cmclnup_get_curr_uptime( void )
{
  return cmclnup_ptr()->uptime;
}

/*===========================================================================

FUNCTION cmclnup_empty_buff_q

DESCRIPTION
  Move all the commands on the buffer command queue to client command queue.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                   cmclnup_empty_buff_q()
{
  cm_cmd_type           *cmd_ptr;

  CM_MSG_HIGH_1("CLNUP: cmclnup_empty_buff_q, buff queue cnt %d", cm_buff_cmd_q.cnt);

  /* Move all commands from buff_q to cmd_q*/
  rex_enter_crit_sect(cmtask_crit_sect_ptr());

  /* First move everything from client queue to buffer queue
  ** so that order is maintained
  */
  while( (cmd_ptr = cm_client_cmd_q_get()) != NULL )
  {
    cm_buff_cmd_q_put(cmd_ptr);
  }

  while( (cmd_ptr = cm_buff_cmd_q_get()) != NULL )
  {
    cm_client_cmd_q_put(cmd_ptr);
  }
  rex_leave_crit_sect(cmtask_crit_sect_ptr());

  cmclnup_ptr()->curr_clnup_stage = CM_CLNUP_STAGE_DONE;

  /* Put a new command on client cmd q which will reset the cleanup state
  ** machine completely.
  */
  if( !cm_ph_cmd_reset_clnup_st_mc() )
  {
    CM_MSG_HIGH_0("CLNUP: Unable to queue reset cmd");
  }
}

/*===========================================================================

FUNCTION cm_buff_cmd_queue

DESCRIPTION
  Queue a CM command in its appropriate CM command queue.

DEPENDENCIES
  All CM command queues must have already been initialized.

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                          cm_buff_cmd_queue(

  cm_ph_cmd_s_type const       *cmd_ptr,

  boolean                       is_cb_tobe_clrd
)
{


  CM_ASSERT(cmd_ptr != NULL);

  if( is_cb_tobe_clrd == TRUE )
  {
    /* This is the command initiating the clean up operation. For this command
    ** the calling module has already been notified with callback after the
    ** para_check. Need to clear the callback function pointer to avoid
    ** duplicate calls to the calling module.
    */
    cm_ph_cmd_s_type *ph_buff_cmd_ptr = cm_cmd_alloc_ph_init();

    *ph_buff_cmd_ptr = *cmd_ptr;

    ph_buff_cmd_ptr->cmd_cb_func     = NULL;
    ph_buff_cmd_ptr->data_block_ptr  = NULL;
    ph_buff_cmd_ptr->client_id       = CM_CLIENT_ID_ANONYMOUS;
    cm_buff_cmd_q_put( (cm_cmd_type *) ph_buff_cmd_ptr );
  }
  else
  {
    /* This command was just dequeued from the client_cmd_q. It has not been
    ** processed and thus its callback has not been invoked yet. Preserve the
    ** callback function pointer passed by the calling module.
    */
    ((cm_cmd_type *)cmd_ptr)->is_reused = TRUE;
    cm_buff_cmd_q_put( (cm_cmd_type *) cmd_ptr );
  }
  CM_MSG_HIGH_4("CLNUP: BUFFER_CMD: cmd %d, buff queue cnt %d, is_cb_tobe_clrd %d, is_reused %d",
                cmd_ptr->cmd,
                cm_buff_cmd_q.cnt,
                is_cb_tobe_clrd,
                cmd_ptr->is_reused);
}

/*===========================================================================

FUNCTION cmclnup_init

DESCRIPTION
  Initialize the Clean up state machine.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
static void                   cmclnup_init(
  cm_clnup_state_s_type  *clnup_ptr
)
{
  if( clnup_ptr == NULL )
  {
    return;
  }
  clnup_ptr->curr_clnup_stage = CM_CLNUP_STAGE_NONE;
  clnup_ptr->curr_clnup_oper  = CM_CLNUP_OPER_NONE;
  clnup_ptr->uptime           = 0;
  memset(&clnup_ptr->clnup_payload,0,sizeof(cm_clnup_curr_oper_payload_s_type));
  clnup_ptr->clnup_payload.oprt_mode = SYS_OPRT_MODE_NONE;
}

/*===========================================================================

FUNCTION cmclnup_init

DESCRIPTION
  Initialize the Clean up state machine.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void                          cmclnup_st_mc_init( )
{
  cmclnup_init(cmclnup_ptr());
  cm_buff_cmd_q_init();
}

/*===========================================================================

FUNCTION cmclnup_is_ongoing_oprt_mode

DESCRIPTION
  Let the calling entity know if cleanup is currently in progress for
  operating mode change.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: Clean up currently in progress.
  FALSE: Cleanup operation not currently underway.

SIDE EFFECTS
  none

===========================================================================*/
boolean                       cmclnup_is_ongoing_oprt_mode( void )
{
  cm_clnup_state_s_type *clnup_ptr = cmclnup_ptr();

  if( clnup_ptr == NULL )
  {
    CM_ERR_0("CLNUP: Error getting cleanup state machine.");
  }

  return (( clnup_ptr->curr_clnup_oper & BM(CM_CLNUP_OPER_OPRT_MODE) )
          &&
          ( clnup_ptr->curr_clnup_stage != CM_CLNUP_STAGE_DONE) );
}

/*===========================================================================

FUNCTION cmclnup_is_ongoing_subsc

DESCRIPTION
  Let the calling entity know if cleanup is currently in progress.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: Clean up currently in progress.
  FALSE: Cleanup operation not currently underway.

SIDE EFFECTS
  none

===========================================================================*/
boolean                       cmclnup_is_ongoing_subsc(

  uint8                  subs_bm

)
{

  if( cmclnup_is_ongoing_oprt_mode() )
  {
    return TRUE;
  }
  else
  {
    cm_clnup_state_s_type *clnup_ptr = cmclnup_ptr();

    if( clnup_ptr == NULL )
    {
      CM_ERR_0("CLNUP: Error getting cleanup state machine.");
    }

    return ( ( clnup_ptr->curr_clnup_oper & subs_bm )
             &&
             ( clnup_ptr->curr_clnup_stage != CM_CLNUP_STAGE_DONE) );
  }
}

/*===========================================================================

FUNCTION cmclnup_is_clnup_needed

DESCRIPTION
  If the device is currently in Online operating mode and has subscription
  available, cleanup operation might be needed.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: Clean up might be needed.
  FALSE: Cleanup operation wont be needed.

SIDE EFFECTS
  none

===========================================================================*/
boolean                       cmclnup_is_clnup_needed(
  uint8                                   sub_bitmask,
  cm_subscription_not_avail_cause_e_type  sub_not_avail_cause,
  sys_oprt_mode_e_type                    cmd_oprt_mode
)
{

  cmph_s_type *ph_ptr = cmph_ptr();
  sys_modem_as_id_e_type asubs_id;
  boolean      subs_avail = FALSE; 

  /* Cleanup already completed for that sub.*/
  if( cmclnup_ptr()->curr_clnup_stage == CM_CLNUP_STAGE_DONE
  	 && (sub_bitmask & cmclnup_get_subs_id_bitmask()
	     || cmclnup_ptr()->curr_clnup_oper & BM(CM_CLNUP_OPER_OPRT_MODE)))
  {
    CM_MSG_HIGH_0("NO_CLNUP: stage_done");
    return FALSE;
  }

#if defined(FEATURE_IP_CALL)
  /* If CM is processing subscription not available and IWLAN registered
  *  cleanup is needed.
  */
  if(sub_not_avail_cause == CM_SUBSCRIPTION_NOT_AVAIL_CAUSE_OTHERS)
  {
    return TRUE;
  }
#endif

   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   
  for( asubs_id = SYS_MODEM_AS_ID_1;
       cmutil_is_as_id_valid(asubs_id); asubs_id++ )
  {
    if( BM(asubs_id)&sub_bitmask
        && ph_ptr->sub_info[asubs_id] != NULL
        && ( ph_ptr->sub_info[asubs_id]->is_3gpp_subs_avail == TRUE
             || ph_ptr->sub_info[asubs_id]->is_3gpp2_subs_avail == TRUE )
      )
    {
      subs_avail = TRUE;
      break;
    }
  }

  /* If no subscription is available, no cleanup will be needed. */
  if(!subs_avail)
  {
    return FALSE;
  }

  /* Always process LPM Oprt Mode if state is not in CM_CLNUP_STAGE_DONE
   * to inform IMS and DS for cleanup incase of IMS over WiFi is active
  */
  if( ph_ptr->device_prop.ph_oprt_mode.oprt_mode == SYS_OPRT_MODE_LPM &&
      cmd_oprt_mode     == SYS_OPRT_MODE_LPM)
  {
    return TRUE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If operating mode is not Online, and above condition not met,
  *  no cleanup is needed.
  */
  if( ph_ptr->device_prop.ph_oprt_mode.oprt_mode != SYS_OPRT_MODE_ONLINE )
  {
    CM_MSG_HIGH_1("NO_CLNUP: oprt_mode %d not online",
                   ph_ptr->device_prop.ph_oprt_mode.oprt_mode);
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If we get back to back online operating mode,
  *  no cleanup is needed.
  */
  if( cmd_oprt_mode == SYS_OPRT_MODE_ONLINE )
  {
    CM_MSG_HIGH_1("NO_CLNUP: oprt_mode %d already online",
                   ph_ptr->device_prop.ph_oprt_mode.oprt_mode);
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* When subscirpiton loss is due to ILLEGAL SIM event , no cleanup will be needed
  */
  if (sub_not_avail_cause == CM_SUBSCRIPTION_NOT_AVAIL_CAUSE_ILLEGAL_SUB)
  {
    CM_MSG_HIGH_0("NO_CLNUP:   illegal sub");
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return TRUE;
}

/*===========================================================================

FUNCTION cmclnup_initiate_ds_clnup

DESCRIPTION
  Initiate the DS cleanup operation.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  TRUE: Timer started.
  FALSE: Timer not started.

SIDE EFFECTS
  none

===========================================================================*/
static boolean                   cmclnup_initiate_ds_clnup(
  cm_clnup_state_s_type    *clnup_ptr
)
{
  boolean                  is_wait_timer_needed = FALSE;
  uint8                                     idx = 0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( clnup_ptr == NULL )
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( clnup_ptr->curr_clnup_oper & BM( CM_CLNUP_OPER_OPRT_MODE ) )
  {
    is_wait_timer_needed = cmds_is_wait_on_oprt_mode(clnup_ptr->clnup_payload.oprt_mode);
  }
  else
  {
    is_wait_timer_needed = cmds_is_wait_on_subsc_cmd(clnup_ptr->clnup_payload.subs_id_bitmask);
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(is_wait_timer_needed)
  {
    for(idx = SYS_MODEM_AS_ID_1; idx < MAX_AS_IDS; idx++)
  {
        if( clnup_ptr->clnup_payload.sub_cleanup_reason[idx] == CM_SUB_CLNUP_REASON_SUBS_NOT_AVAIL_REFRESH)
        {
           clnup_ptr->uptime = time_get_uptime_secs() + CMPH_MAX_DS_TRAFFIC_DOWN_TIME_REFRESH;
           CM_MSG_HIGH_3("CLNUP: DS clnup,OTASN config is_wait %d, uptime %d, timer_exp %d",
                             is_wait_timer_needed, time_get_uptime_secs(),clnup_ptr->uptime);
            return is_wait_timer_needed;
        }
    }

    clnup_ptr->uptime = time_get_uptime_secs() + CMPH_MAX_DS_TRAFFIC_DOWN_TIME;
  }
  CM_MSG_HIGH_3("CLNUP: DS clnup, is_wait %d, uptime %d, timer_exp %d",
                is_wait_timer_needed, time_get_uptime_secs(),clnup_ptr->uptime);

  return is_wait_timer_needed;
}

/*===========================================================================

FUNCTION cmclnup_update_st_mc

DESCRIPTION
  Update the state machine.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
static void                   cmclnup_update_st_mc(
  cm_clnup_state_s_type *clnup_local
)
{
  cm_clnup_state_s_type   *clnup_ptr       = cmclnup_ptr();

  if( clnup_local == NULL ||
      clnup_ptr->curr_clnup_oper & BM(CM_CLNUP_OPER_OPRT_MODE) ||
      (clnup_local->clnup_payload.subs_id_bitmask>>1)>=MAX_SIMS)
  {
    return;
  }

  clnup_ptr->curr_clnup_oper |= clnup_local->curr_clnup_oper;
  clnup_ptr->curr_clnup_stage = clnup_local->curr_clnup_stage;
  clnup_ptr->uptime           = clnup_local->uptime;
  clnup_ptr->clnup_payload.oprt_mode    = clnup_local->clnup_payload.oprt_mode;
  clnup_ptr->clnup_payload.subs_id_bitmask |= clnup_local->clnup_payload.subs_id_bitmask;

  /* If an oprt_mode cmd is received after a subscription cmd, the sub_cleanup_reason
  ** mask should not be reset
  */
  if(!(clnup_local->curr_clnup_oper & BM(CM_CLNUP_OPER_OPRT_MODE)))
  {
    sys_modem_as_id_e_type  idx = SYS_MODEM_AS_ID_NONE;

    for(idx = SYS_MODEM_AS_ID_1; idx < MAX_AS_IDS; idx++)
    {
      if(BM(idx) & clnup_local->clnup_payload.subs_id_bitmask)
      {
        clnup_ptr->clnup_payload.sub_cleanup_reason[idx] =
          clnup_local->clnup_payload.sub_cleanup_reason[idx];
      }
    }
  }

  return;
}

/*===========================================================================

FUNCTION cmclnup_initiate_cleanup

DESCRIPTION
  Initiate the cleanup operation.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void                          cmclnup_initiate_cleanup(
  cm_ph_cmd_s_type const       *ph_cmd_ptr
)
{
  cm_clnup_state_s_type  clnup_local;
  boolean                is_wait_timer_needed = FALSE;
  cm_ph_cmd_info_s_type const *cmd_info_ptr    = NULL;
  sys_modem_as_id_e_type        sub_idx = SYS_MODEM_AS_ID_NONE;
  cmph_s_type *ph_ptr = cmph_ptr();
  cm_clnup_state_s_type *clnup_ptr = cmclnup_ptr();
  boolean                 is_ds_otasan_on = FALSE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If the command pointer is NULL or the command does not require
  ** any cleanup, exit the cleanup
  */
  if(ph_cmd_ptr == NULL ||
      ( ph_cmd_ptr->cmd != CM_PH_CMD_OPRT_MODE &&
        ph_cmd_ptr->cmd != CM_PH_CMD_SUBSCRIPTION_NOT_AVAILABLE &&
        ph_cmd_ptr->cmd != CM_PH_CMD_DEACTIVATE_SUBS  &&
        ph_cmd_ptr->cmd != CM_PH_CMD_DUAL_STANDBY_PREF)
    )
  {
    CM_MSG_HIGH_0("CLNUP: Error initiating cleanup");
    return;
  }

  if(clnup_ptr->curr_clnup_stage == CM_CLNUP_STAGE_DONE)
  {
    /* If previous clnup already in done stage, reset subs_id_bitmask so that we dont redo*/
    clnup_ptr->clnup_payload.subs_id_bitmask = 0;
  }

  cmd_info_ptr = CMD_INFO_PTR( ph_cmd_ptr );
  cmclnup_init(&clnup_local);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch( ph_cmd_ptr->cmd )
  {
    case CM_PH_CMD_OPRT_MODE:
      clnup_local.curr_clnup_oper |= BM( CM_CLNUP_OPER_OPRT_MODE );
      clnup_local.clnup_payload.oprt_mode = cmd_info_ptr->oprt_mode;
      break;

    case CM_PH_CMD_SUBSCRIPTION_NOT_AVAILABLE:
    {
      uint8 subs_bm = BM(cmd_info_ptr->cmd_subs);
      sys_modem_as_id_e_type  idx = SYS_MODEM_AS_ID_NONE;

      if( subs_bm == SYS_MODEM_AS_ID_NO_ACTIVE_MASK )
      {
        CM_ERR_1("CLNUP: No subs impacted, subs_bm %d",subs_bm);
        return;
      }

      /* If UE is already doing cleanup for oprt_mode, it will not initiate cleanup
      ** for subscription lost.
      ** If UE is already doing cleanup for subscription lost on one sub and then
      ** cleanup is needed for subscription lost on another sub, the subs_id_bitmask
      ** is aggregated in order to keep track of all the subs currently doing cleanup.
      */
      clnup_local.clnup_payload = cmclnup_ptr()->clnup_payload;
      clnup_local.curr_clnup_oper |= subs_bm;
      clnup_local.clnup_payload.subs_id_bitmask |= subs_bm;
      for(idx = SYS_MODEM_AS_ID_1; idx < MAX_AS_IDS; idx++)
	  {
        if(BM(idx) & subs_bm)
		  {
            is_ds_otasan_on = ds3g_is_otasn_feature_supported(cmd_info_ptr->cmd_subs);
            if((cmd_info_ptr->sub_not_avail_cause == CM_SUBSCRIPTION_NOT_AVAIL_CAUSE_REFRESH || 
                  cmd_info_ptr->sub_not_avail_cause == CM_SUBSCRIPTION_NOT_AVAIL_CAUSE_REFRESH_NO_STDBY ) &&
                  is_ds_otasan_on)
            {
              clnup_local.clnup_payload.sub_cleanup_reason[idx] = CM_SUB_CLNUP_REASON_SUBS_NOT_AVAIL_REFRESH;
              is_ds_otasan_on = FALSE;
            }
            else
            {
               clnup_local.clnup_payload.sub_cleanup_reason[idx] |= BM(CM_SUB_CLNUP_REASON_SUBS_NOT_AVAIL);
            }	  

			

			CM_MSG_MED_2("clnup_local.clnup_payload.sub_cleanup_reason %d for sub %d", clnup_local.clnup_payload.sub_cleanup_reason[idx], idx);
		}
	  }
	}
	break;

    case CM_PH_CMD_DEACTIVATE_SUBS:
    {
      sys_modem_as_id_e_type  idx = SYS_MODEM_AS_ID_NONE;

      /* If UE is already doing cleanup for oprt_mode, it will not initiate cleanup
      ** for subscription lost.
      ** If UE is already doing cleanup for subscription lost on one sub and then
      ** cleanup is needed for subscription lost on another sub, the subs_id_bitmask
      ** is aggregated in order to keep track of all the subs currently doing cleanup.
      */
      clnup_local.clnup_payload = cmclnup_ptr()->clnup_payload;
      clnup_local.curr_clnup_oper |= cmd_info_ptr->cmd_subs_mask;
      clnup_local.clnup_payload.subs_id_bitmask |= cmd_info_ptr->cmd_subs_mask;
      for(idx = SYS_MODEM_AS_ID_1; idx < MAX_AS_IDS; idx++)
      {
        if(BM(idx) & cmd_info_ptr->cmd_subs_mask)
        {
          clnup_local.clnup_payload.sub_cleanup_reason[idx] |= BM(CM_SUB_CLNUP_REASON_SUB_DEACTIVATED);
        }
      }
    }
    break;

    case CM_PH_CMD_DUAL_STANDBY_PREF:
    {
      uint8 deactivated_sub_mask = 0;
      sys_modem_as_id_e_type  idx = SYS_MODEM_AS_ID_NONE;

      /* If UE standby pref has changed, then calculate which subs will be
      ** deactivated and add them to the aggregated subs_id_bitmask.
      */
      cmph_find_standby_pref(ph_cmd_ptr->info, &deactivated_sub_mask);
      deactivated_sub_mask ^=  cmph_ptr()->device_prop.msim_prop.active_subs ;
      clnup_local.clnup_payload = cmclnup_ptr()->clnup_payload;
      clnup_local.curr_clnup_oper |= deactivated_sub_mask;
      clnup_local.clnup_payload.subs_id_bitmask |= deactivated_sub_mask;
      for(idx = SYS_MODEM_AS_ID_1; idx < MAX_AS_IDS; idx++)
      {
        if(BM(idx) & deactivated_sub_mask)
        {
          clnup_local.clnup_payload.sub_cleanup_reason[idx] |= BM(CM_SUB_CLNUP_REASON_STANDBY_PREF_CHANGE);
        }
      }
    }
    break;

    default:
      CM_ERR_1("CLNUP: Invalid phone command %d",ph_cmd_ptr->cmd);
      return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If UE is currently performing IMS dereg on 3rd Party IMS and a new cmd is
  ** being handled for oprt_mode change, just update the payload and curr_clnup_oper.
  ** There is no action on 3rd Party IMS for oprt_mode, so the subscription related
  ** dreg is allowed to complete. The updated payload and curr_clnup_oper will ensure
  ** that the subsequent DS cleanup will handle the oprt_mode change.
  */
  for(sub_idx = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub_idx); sub_idx++)
  {
    cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(sub_idx);

    if(cmph_is_ims_capable_on_sub(sub_idx) &&
        cmclnup_ptr()->curr_clnup_stage == CM_CLNUP_STAGE_IMS_DEREG &&
        ph_sub_ptr != NULL &&
        ph_sub_ptr->active_ims_stack == SYS_IMS_STACK_STATUS_ENABLED_THPY &&
        ph_cmd_ptr->cmd == CM_PH_CMD_OPRT_MODE)
    {
      cm_buff_cmd_queue( ph_cmd_ptr, TRUE );

      clnup_local.uptime           = cmclnup_ptr()->uptime;
      clnup_local.curr_clnup_stage = cmclnup_ptr()->curr_clnup_stage;
      cmclnup_update_st_mc(&clnup_local);

      return;
    }
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Inform IMS to perform deregistration
  */
  #ifdef FEATURE_IP_CALL
  if( clnup_local.curr_clnup_oper & BM( CM_CLNUP_OPER_OPRT_MODE ) )
  {
    /* For dual-LTE, cleanup needs to be sent to one sub at a time
    ** if both subs have registered apps with oprt mode cb.
    */
    if(cmph_is_dual_lte())
    {
      ph_ptr->device_prop.ims_dereg_subs = 0;

      for(sub_idx = SYS_MODEM_AS_ID_1; cmutil_is_as_id_valid(sub_idx); sub_idx++)
      {
        if(cmipapp_is_there_an_app_with_oprt_mode_cb_on_sub(sub_idx))
        {
          ph_ptr->device_prop.ims_dereg_subs |= BM(sub_idx);
        }
      }

      /* Send oprt mode to the DDS sub first.
      ** If there is only one sub for deregistration, send to that sub.
      */
      if(ph_ptr->device_prop.ims_dereg_subs == (SYS_MODEM_AS_ID_1_MASK | SYS_MODEM_AS_ID_2_MASK))
      {
        ph_ptr->device_prop.ims_dereg_subs &= ~(BM(ph_ptr->device_prop.msim_prop.default_data_subs));
        cmipapp_send_oprt_mode(clnup_local.clnup_payload.oprt_mode, TRUE, ph_ptr->device_prop.msim_prop.default_data_subs);
      }
      else
      {
        if(ph_ptr->device_prop.ims_dereg_subs == SYS_MODEM_AS_ID_2_MASK)
        {
          cmipapp_send_oprt_mode(clnup_local.clnup_payload.oprt_mode, TRUE, SYS_MODEM_AS_ID_2);
        }
        else if(ph_ptr->device_prop.ims_dereg_subs == SYS_MODEM_AS_ID_1_MASK)
        {
          cmipapp_send_oprt_mode(clnup_local.clnup_payload.oprt_mode, TRUE, SYS_MODEM_AS_ID_1);
        }

        ph_ptr->device_prop.ims_dereg_subs = 0;
      }
       
      is_wait_timer_needed = cmipapp_is_wait_on_oprt_mode_cnf(clnup_local.clnup_payload.oprt_mode);
    }
    else
    {
      if( cmipapp_send_oprt_mode (clnup_local.clnup_payload.oprt_mode, TRUE, SYS_MODEM_AS_ID_NONE))
      {
        is_wait_timer_needed = cmipapp_is_wait_on_oprt_mode_cnf(clnup_local.clnup_payload.oprt_mode);
      }
    }
  }
  else
  {
    if(cmipapp_send_subsc_cmd (ph_cmd_ptr->cmd,
                               clnup_local.clnup_payload.subs_id_bitmask,
                               clnup_local.clnup_payload.sub_cleanup_reason))
    {
      is_wait_timer_needed = cmipapp_is_wait_on_subsc_cmd_cnf(
                               ph_cmd_ptr->cmd,
                               ACTIVE_MASK_TO_AS_ID(clnup_local.clnup_payload.subs_id_bitmask));
    }
  }
  #endif

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If CM needs to wait for a confirmation from IMS, set the cleanup timer.
  ** If needed, put the command onto the buffer queue.
  */
  if( is_wait_timer_needed )
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    cmdbg_qsh_send_xxx_evts(CM_QSH_EVENT_IMS_CLEANUP_TIMER_START, ph_cmd_ptr->info.cmd_subs);
#endif
    if (ph_cmd_ptr->cmd == CM_PH_CMD_DEACTIVATE_SUBS )
    {
      cm_buff_cmd_queue( ph_cmd_ptr, FALSE );
    }
    else
    {
      cm_buff_cmd_queue( ph_cmd_ptr, TRUE );
    }

    clnup_local.curr_clnup_stage = CM_CLNUP_STAGE_IMS_DEREG;
    clnup_local.uptime = time_get_uptime_secs() + CMPH_MAX_IPAPP_DEREG_TIME;

    cmclnup_update_st_mc(&clnup_local);

    CM_MSG_HIGH_4("CLNUP: START: cmd %d, IMS dereg timer started uptime = %d, timer_exp = %d, curr_clnup_oper = %d",
                  ph_cmd_ptr->cmd, time_get_uptime_secs(),clnup_local.uptime,
                  clnup_local.curr_clnup_oper);

    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If we got here, CM does not need to wait for IMS cleanup confirmation, so
  ** trigger the DS cleanup right away. If this function returns before this point, DS cleanup
  ** will be triggered after the IMS cleanup is done when the confirmation is received from
  ** IMS.
  */
  if( cmclnup_initiate_ds_clnup(&clnup_local) )
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    cmdbg_qsh_send_xxx_evts(CM_QSH_EVENT_DS_CLEANUP_TIMER_START, ph_cmd_ptr->info.cmd_subs);
#endif
    if (ph_cmd_ptr->cmd == CM_PH_CMD_DEACTIVATE_SUBS )
    {
      cm_buff_cmd_queue( ph_cmd_ptr, FALSE );
    }
    else
    {
      cm_buff_cmd_queue( ph_cmd_ptr, TRUE );
    }

    clnup_local.curr_clnup_stage = CM_CLNUP_STAGE_DS_CLNUP;

    cmclnup_update_st_mc(&clnup_local);
	CM_MSG_HIGH_5("CLNUP:START: cmd=%d, DS clnup timer uptimer=%d, timer_exp=%d, curr_clnup_oper=%d, sub_not_avail=%d",
		ph_cmd_ptr->cmd,time_get_uptime_secs(),clnup_local.uptime,clnup_local.curr_clnup_oper,cmd_info_ptr->sub_not_avail_cause);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If we got here, even DS cleanup does not require waiting for confirmation.
  */
  return;
}

#ifdef FEATURE_IP_CALL
/*===========================================================================

FUNCTION cmclnup_ip_rpt_proc

DESCRIPTION
  Process the IMS deregistration confirmation.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void                          cmclnup_ip_rpt_proc(
  cm_name_type           rpt_name,
  uint8                  subs_bm
)
{
  cm_clnup_state_s_type *clnup_ptr = cmclnup_ptr();
  cmph_sub_info_s_type *ph_sub_ptr = cmph_get_sub_ptr(ACTIVE_MASK_TO_AS_ID(subs_bm));
  cmph_s_type *ph_ptr = cmph_ptr();

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( (rpt_name != CM_IP_APP_ALLAPPS_OPRT_CNF &&
       rpt_name != CM_IP_APP_ALLAPSS_SUBSC_CNF)
      ||
      clnup_ptr->curr_clnup_stage != CM_CLNUP_STAGE_IMS_DEREG
    )
  {
    CM_MSG_HIGH_6("CLNUP: Incorrect rpt %d or incorrect stage %d, curr_clnup_oper %d, subs_bm 0x%x, curr timer %d, uptime %d",
                  rpt_name, clnup_ptr->curr_clnup_stage,
                  clnup_ptr->curr_clnup_oper, subs_bm,
                  time_get_uptime_secs(), clnup_ptr->uptime);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( rpt_name == CM_IP_APP_ALLAPSS_SUBSC_CNF )
  {
    if (ph_sub_ptr)
    {
      if( clnup_ptr->curr_clnup_oper & BM( CM_CLNUP_OPER_OPRT_MODE ) &&
          ph_sub_ptr->active_ims_stack != SYS_IMS_STACK_STATUS_ENABLED_THPY)
      {
        CM_MSG_HIGH_0("CLNUP: Discard subscription cnf as oprt_mode clnup underway");
        return;
      }
    }

  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /* In dual-LTE when oprt mode needs to be sent to both subs, it will be
  ** sent one sub at a time. Here CM received ALLAPPS_OPRT_CNF for one sub,
  ** so send oprt mode to the other sub now if necessary.
  */
  if(rpt_name == CM_IP_APP_ALLAPPS_OPRT_CNF &&
    (clnup_ptr->curr_clnup_oper & BM(CM_CLNUP_OPER_OPRT_MODE)) &&
     cmph_is_dual_lte() &&
     ph_ptr->device_prop.ims_dereg_subs != 0)
  {
    if(cmipapp_send_oprt_mode(clnup_ptr->clnup_payload.oprt_mode, TRUE, ACTIVE_MASK_TO_AS_ID(ph_ptr->device_prop.ims_dereg_subs)) &&
       cmipapp_is_wait_on_oprt_mode_cnf(clnup_ptr->clnup_payload.oprt_mode))
    {
      clnup_ptr->uptime = time_get_uptime_secs() + CMPH_MAX_IPAPP_DEREG_TIME;
    
      CM_MSG_HIGH_5("CLNUP: CM->IMS: Sent oprt_mode %d to sub %d uptime %d timer_exp %d, DDS %d", 
                    clnup_ptr->clnup_payload.oprt_mode,
                    ACTIVE_MASK_TO_AS_ID(ph_ptr->device_prop.ims_dereg_subs),
                    time_get_uptime_secs(),
                    clnup_ptr->uptime,
                    ph_ptr->device_prop.msim_prop.default_data_subs);
      
      ph_ptr->device_prop.ims_dereg_subs = 0;
      return;
    }
    ph_ptr->device_prop.ims_dereg_subs = 0;
  }
  
  if(ph_ptr->sub_info[SYS_MODEM_AS_ID_1] != NULL &&
  	ph_ptr->sub_info[SYS_MODEM_AS_ID_1]->ip_call_end_rsp_pend == TRUE &&
  	(subs_bm & SYS_MODEM_AS_ID_1_MASK))
  {
    
    cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_NONE,
                                          SYS_MODEM_AS_ID_1,
                                          MM_STACK_0,
                                          TRUE);
  
  }	
  
  if(ph_ptr->sub_info[SYS_MODEM_AS_ID_2] != NULL &&
  	ph_ptr->sub_info[SYS_MODEM_AS_ID_2]->ip_call_end_rsp_pend == TRUE &&
  	(subs_bm & SYS_MODEM_AS_ID_2_MASK) )
  {
    
    cmph_send_trm_priority_req_proc_type(SYS_PROC_TYPE_NONE,
                                          SYS_MODEM_AS_ID_2,
                                          MM_STACK_0,
                                          TRUE);
  
 
  }	
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( !cmclnup_initiate_ds_clnup(clnup_ptr))
  {
    cmclnup_empty_buff_q();
  }
  else
  {
    clnup_ptr->curr_clnup_stage = CM_CLNUP_STAGE_DS_CLNUP;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return;
}
#endif

/*===========================================================================

FUNCTION cmclnup_ds_pkt_rpt_proc

DESCRIPTION
  Process the DS cleanup confirmation.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void cmclnup_ds_pkt_rpt_proc(
  uint8 subs_id_bitmask
)
{
  cm_clnup_state_s_type *clnup_ptr = cmclnup_ptr();
  uint8 ref_bitmask = 0;
  uint8 sub=0;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( clnup_ptr->curr_clnup_stage != CM_CLNUP_STAGE_DS_CLNUP )
  {
    CM_MSG_HIGH_1("CLNUP: DS Cleanup in incorrect stage %d",clnup_ptr->curr_clnup_stage);
    return;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ref_bitmask = ((clnup_ptr->curr_clnup_oper & BM( CM_CLNUP_OPER_OPRT_MODE )) ?
                 0xFF :
                 clnup_ptr->clnup_payload.subs_id_bitmask);

  if( ref_bitmask != subs_id_bitmask )
  {
    CM_MSG_HIGH_2("CLNUP: DS Cleanup incomplete, curr_clnup_oper %d, bitmask %d",
                  clnup_ptr->curr_clnup_oper,subs_id_bitmask);
    return;
  }

  /* reset subs specific reason only if there was no successive OPRT MODE change
  */
  if(ref_bitmask!= 0XFF)
  {
    for(sub=0; sub<MAX_AS_IDS; sub++)
    {
      if(ref_bitmask &  BM( sub ))
      {
        clnup_ptr->clnup_payload.sub_cleanup_reason[sub]=SUBS_CLEANUP_REASON_MASK_NONE;
	clnup_ptr->curr_clnup_oper &= (~BM(sub));
      }
    }
  }
  else
  {
    memset(clnup_ptr->clnup_payload.sub_cleanup_reason,0,sizeof(uint8)*MAX_AS_IDS);
  }

  CM_MSG_HIGH_1("CLNUP: DS Cleanup success, curr_clnup_stage %d", clnup_ptr->curr_clnup_stage);

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmclnup_empty_buff_q();
}

/*===========================================================================

FUNCTION cmclnup_timer_proc

DESCRIPTION
  Process the DS cleanup confirmation.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  None

SIDE EFFECTS
  none

===========================================================================*/
void                          cmclnup_timer_proc( void )
{
  cm_clnup_state_s_type *clnup_ptr = cmclnup_ptr();
  dword                  uptime    = time_get_uptime_secs();
  /* Get the current uptime */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If no timer is running or timer has not yet expired no action needed
  */
  if( !clnup_ptr->uptime ||
      clnup_ptr->uptime > uptime )
  {
    return;
  }

  CM_MSG_HIGH_6("CLNUP: Timer expiry in stage: %d, curr_clnup_oper 0x%x, curr time %d, uptime %d, oprt_mode %d, subs_bm 0x%x",
                clnup_ptr->curr_clnup_stage,
                clnup_ptr->curr_clnup_oper,
                time_get_uptime_secs(), clnup_ptr->uptime,
                clnup_ptr->clnup_payload.oprt_mode,
                clnup_ptr->clnup_payload.subs_id_bitmask);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If timer has expired, move on to the next stage of the cleanup
  */
  switch( clnup_ptr->curr_clnup_stage )
  {
#ifdef FEATURE_IP_CALL
    case CM_CLNUP_STAGE_IMS_DEREG:
      cmipapp_reset_trans_id ();
      if( clnup_ptr->curr_clnup_oper & BM(CM_CLNUP_OPER_OPRT_MODE) )
      {
        cmclnup_ip_rpt_proc( CM_IP_APP_ALLAPPS_OPRT_CNF, 0xFF);
      }
      else
      {
        cmclnup_ip_rpt_proc( CM_IP_APP_ALLAPSS_SUBSC_CNF,
                             clnup_ptr->clnup_payload.subs_id_bitmask );
      }
      return;
#endif

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    case CM_CLNUP_STAGE_DS_CLNUP:
    {
      uint8 bitmask = 0;
      if( clnup_ptr->curr_clnup_oper & BM(CM_CLNUP_OPER_OPRT_MODE) )
      {
        bitmask = 0xFF;
      }
      else
      {
        bitmask = clnup_ptr->clnup_payload.subs_id_bitmask;
      }
      cmclnup_ds_pkt_rpt_proc( bitmask );
    }
    return;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    default:
      CM_MSG_HIGH_0("CLNUP: Invalid timer expiry");
      break;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  clnup_ptr->uptime = 0;
  return;
}

/*===========================================================================

FUNCTION cmclnup_is_cmd_tobe_buff

DESCRIPTION
  Put the Phone command on to the buffer queue if the cleanup related to it is
  already underway.

DEPENDENCIES
  Buffer command queue must have already been initialized with
  cm_buff_cmd_q_init().

RETURN VALUE
  TRUE: Command was placed on the buffer queue.
  False: Command was not placed on the buffer queue.

SIDE EFFECTS
  none

===========================================================================*/
boolean                       cmclnup_is_cmd_tobe_buff(

  cm_ph_cmd_s_type const       *ph_cmd_ptr

)
{
  cm_ph_cmd_info_s_type const *cmd_info_ptr = CMD_INFO_PTR(ph_cmd_ptr);
  uint8 subs_id_bitmask = cmclnup_ptr()->clnup_payload.subs_id_bitmask;
  sys_modem_as_id_e_type sub_id = SYS_MODEM_AS_ID_NONE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( ph_cmd_ptr == NULL ||
      cmclnup_ptr()->curr_clnup_stage == CM_CLNUP_STAGE_DONE )
  {
    return FALSE;
  }

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* If cleanup is already underway,queue this command to buffer queue and end
  ** the processing for now.
  */
  switch( ph_cmd_ptr->cmd )
  {
    case CM_PH_CMD_OPRT_MODE:

      /* This subsc API checks for both oprt mode and subsc clnup ongoing */
      if( cmclnup_is_ongoing_subsc(SYS_MODEM_AS_ID_ALL_ACTIVE_MASK) )
      {
         /*For the case where clnup is ongoing for subsc and CM LPM command is received.
          if CM has started a 60s timer (CMPH_MAX_DS_TRAFFIC_DOWN_TIME_REFRESH), reduce it
          to 5s so that LPM cmd processing is not delayed beyond 5s.
        */
        if(((cmclnup_ptr()->uptime - time_get_uptime_secs())> CMPH_MAX_DS_TRAFFIC_DOWN_TIME) && 
                 cmclnup_ptr()->curr_clnup_stage == CM_CLNUP_STAGE_DS_CLNUP &&
                 (cmclnup_ptr()->curr_clnup_oper & SYS_MODEM_AS_ID_ALL_ACTIVE_MASK))
        {
           if((subs_id_bitmask & SYS_MODEM_AS_ID_MASK_1) == SYS_MODEM_AS_ID_MASK_1)
                 sub_id = SYS_MODEM_AS_ID_1;
          else if((subs_id_bitmask & SYS_MODEM_AS_ID_MASK_2) == SYS_MODEM_AS_ID_MASK_2)
                 sub_id = SYS_MODEM_AS_ID_2;
          else if((subs_id_bitmask & SYS_MODEM_AS_ID_MASK_3) == SYS_MODEM_AS_ID_MASK_3)
                          sub_id = SYS_MODEM_AS_ID_2;
          else 
               {
                CM_MSG_HIGH_0("CMCLNUP: Invalid sub_id, set to AS_ID 1 by default");
                sub_id = SYS_MODEM_AS_ID_1;
               }
           cmclnup_ptr()->uptime = time_get_uptime_secs() + CMPH_MAX_DS_TRAFFIC_DOWN_TIME;
           cmclnup_ptr()->clnup_payload.sub_cleanup_reason[sub_id] = CM_SUB_CLNUP_REASON_SUBS_NOT_AVAIL;
           CM_MSG_HIGH_2("CLNUP: OPRT_MODE cmd received while DS OTASN cleanup in progress.Reduce uptime to %d curr_time %d",cmclnup_ptr()->uptime, time_get_uptime_secs());
           if (cmds_is_wait_on_subsc_cmd(subs_id_bitmask))
           {
                CM_MSG_HIGH_2("CLNUP: DS subsc cmd ntfy sent with BM %d, reason %d",subs_id_bitmask,cmclnup_ptr()->clnup_payload.sub_cleanup_reason[sub_id]);
           }
        }

        cm_buff_cmd_queue( ph_cmd_ptr, FALSE );
        return TRUE;
      }
      break;
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_PH_CMD_SUBSCRIPTION_NOT_AVAILABLE:
    {
      if( cmclnup_is_ongoing_subsc(SYS_MODEM_AS_ID_ALL_ACTIVE_MASK) )
      {
        cm_buff_cmd_queue( ph_cmd_ptr, FALSE );
        return TRUE;
      }
    }
    break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    case CM_PH_CMD_SUBSCRIPTION_AVAILABLE:
    {
      if( cmclnup_is_ongoing_subsc(SYS_MODEM_AS_ID_ALL_ACTIVE_MASK) )
      {
        cm_buff_cmd_queue( ph_cmd_ptr, FALSE );
        return TRUE;
      }
    }
    break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    case CM_PH_CMD_SUBSCRIPTION_CHANGED:
    {
      if( cmclnup_is_ongoing_subsc(SYS_MODEM_AS_ID_ALL_ACTIVE_MASK) )
      {
        cm_buff_cmd_queue( ph_cmd_ptr, FALSE );
        return TRUE;
      }
    }
    break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    case CM_PH_CMD_DEACTIVATE_SUBS:
    case CM_PH_CMD_ACTIVATE_SUBS:
      if( cmclnup_is_ongoing_subsc(SYS_MODEM_AS_ID_ALL_ACTIVE_MASK) )
      {
        cm_buff_cmd_queue( ph_cmd_ptr, FALSE );
        return TRUE;
      }
      break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    default:
      break;
  }
  return FALSE;
}
/*===========================================================================

FUNCTION cmclnup_is_clnup_done

DESCRIPTION
  Let the calling entity know if cleanup is done.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: Clean up already done.
  FALSE: Cleanup operation not yet done.

SIDE EFFECTS
  none


===========================================================================*/
boolean                       cmclnup_is_clnup_done( void )
{
  cm_clnup_state_s_type *clnup_ptr = cmclnup_ptr();

  if( clnup_ptr == NULL )
  {
    CM_ERR_0("CLNUP: Error getting cleanup state machine.");
  }

  return  (clnup_ptr->curr_clnup_stage == CM_CLNUP_STAGE_DONE);
}

/*===========================================================================

FUNCTION cmclnup_get_sub_reason_mask

DESCRIPTION
  Get the cleanup reason mask

DEPENDENCIES
  none

RETURN VALUE
  cm_clnup_stage_e_type

SIDE EFFECTS
  none

===========================================================================*/
uint8 *cmclnup_get_sub_reason_mask( void )
{
  return (&cmclnup_ptr()->clnup_payload)->sub_cleanup_reason;
}

/*===========================================================================

FUNCTION cmclnup_get_subs_id_bitmask

DESCRIPTION
  Get the cleanup subs_id_bitmask

DEPENDENCIES
  none

RETURN VALUE
  uint8

SIDE EFFECTS
  none

===========================================================================*/
uint8 cmclnup_get_subs_id_bitmask( void )
{
  return cmclnup_ptr()->clnup_payload.subs_id_bitmask;
}

/*===========================================================================

FUNCTION cmclnup_get_current_stage

DESCRIPTION
  Get the stage of the current cleanup operation.

DEPENDENCIES
  none

RETURN VALUE
  cm_clnup_stage_e_type

SIDE EFFECTS
  none

===========================================================================*/
cm_clnup_stage_e_type         cmclnup_get_current_stage()
{
  return cmclnup_ptr()->curr_clnup_stage;
}

#ifdef CM_DEBUG
#error code not present
#endif

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION cmclnup_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for cmclnup global variables

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
void cmclnup_qsh_mdump_collect()
{
  qsh_mdump_collect_high(&cm_buff_cmd_q,sizeof(cm_cmd_q_type));
  qsh_mdump_collect_high(&cmclnup_state_machine,sizeof(cm_clnup_state_s_type));
}
#endif
#ifdef CM_DEBUG
#error code not present
#endif

