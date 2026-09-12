/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             H D R    S R C H    A R D    S T A T E    M A C H I N E

GENERAL DESCRIPTION
      This module contains functions for the HDR ARD State Machine

INITIALIZATION AND SEQUENCING REQUIREMENTS

    Copyright (c) 2009 - 2014 by Qualcomm Technologies, Inc.  All Rights Reserved.

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchardsm.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when         who     what, where, why 
----------   ---     ---------------------------------------------------------- 
08/27/2013   wsh     Fixed crash when doing 2nd chain test
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
06/27/2014   wsh     ARD/ASDiv incorrectly switching in RTAP mode  
08/08/2013   wsh     Fixed 2nd chain test not working in DIME
08/31/2012   wsh     Respect hard tuneaway rampdown
08/23/2012   smd     Added updates for Device only DTX and FlexCONN.
08/07/2012   wsh     Fixed crash due to ARD enabled in IDLE
07/27/2012   wsh     Fixed ARD STM Q full after back-to-back FL ACT ind.
06/20/2012   smd     Added Device only DTX and FlexCONN.
06/19/2012   smd     Fixed two bugs in ARD state machine
05/25/2012   wsh     Moved ARD logic into it's own state machine

===========================================================================*/ 

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "customer.h"

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
#include "stm.h"
#include "hdrsrchard.h"
#include "hdrsrchcom_sm.smh"
#include "hdrsrchsm.h"
#include "hdrsrchard_sm.smt"

#include "hdrsrch.h"
#include "hdrsrchstates.h"
#include "hdrdebug.h"
#include "hdrsrchlog.h"
#include "hdrsrchrfdiv.h"

#include "hdrfmac.h"
#include "hdrdec.h"
#include "hdrmdspmcr.h"

#include "hdrsrchlog.h"

#include "hdrsrchafc.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define HDRSRCHARD_FL_INACTIVE_TIME_MS  hdrsrchard_fl_inactive_time_ms
  /* Forward link inactive time threshold */

int32 hdrsrchard_fl_inactive_time_ms = 200;
  /* A tunable value for feature charatorization */

#define HDRSRCHARD_MAX_HSTR_COUNT                      0xFFFF
#define HDRSRCHARD_HSTR_TO_MS( hstr )                  ( (hstr) * 5 / 6 )

typedef           enum
{    
  HDRSRCHARD_DIV_FALLBACK_NO_OP,
    /* ignore the event */
  HDRSRCHARD_DIV_ALWAYS_PROCESS,
    /* always process even if ARD disabled/fixed DRC */
  HDRSRCHARD_DIV_FALLBACK_ENABLE,
    /* fallback to DIV enable */
  HDRSRCHARD_DIV_FALLBACK_DISABLE
    /* fallback to DIV disable */
} hdrsrchard_div_fallback_enum_type;
/* what to do when ARD is not enabled/fixed DRC */

typedef           struct
{
  hdrsrch_int_cmd_code_type           cmd;
  hdrsrchard_div_fallback_enum_type   fallback;
} hdrsrchard_stm_event_cmd_map_type;

typedef struct
{
  rex_timer_type                      inact_timer;
    /* FL inactivity timer */

  uint32                              time_left;
    /* time left if timer is suspended */

  boolean                             tap;
    /* whether ARD is disabled due to TAP call
       if true, DIV is left on when ARD shutsdown
       if false, DIV is off */

  /* The following fields are used for optimization/logging only */

  boolean                             div_requested;
    /* Whether DIV has already been requested */

  boolean                             suspended;
    /* Keep track if SRCH is in suspend */

  boolean                             runtime_disabled;
    /* In addition to the NV control, ARD maybe disabled
       (due to 2nd chain test call). */
} hdrsrchard_struct_type;

hdrsrchard_struct_type hdrsrchard;

/*===========================================================================

                     COMMAND  RELATED  FUNCTION  DEFINITIONS

===========================================================================*/

/*==========================================================================

FUNCTION  HDRSRCHARD_EVENT

DESCRIPTION
  This function reports an ARD event to the state machine. As a
  result, ARD state will be updated.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchard_event
( 
  hdrsrchard_event_enum_type event
)
{
  boolean           ard_enabled = HDRSRCH_FL_DATA_ADAPTIVE_RXD_NV_ENABLE;

  const hdrsrchard_stm_event_cmd_map_type ard_event_cmd_map[] = 
  {
    { HDRSRCHARD_ACTIVATE_CMD, HDRSRCHARD_DIV_FALLBACK_ENABLE },
    { HDRSRCHARD_DEACTIVATE_CMD, HDRSRCHARD_DIV_FALLBACK_DISABLE },
    { HDRSRCHARD_SUSPEND_CMD, HDRSRCHARD_DIV_FALLBACK_DISABLE },
    { HDRSRCHARD_RESUME_CMD, HDRSRCHARD_DIV_FALLBACK_ENABLE },
    { HDRSRCHARD_FL_ACTIVITY_CMD, HDRSRCHARD_DIV_FALLBACK_NO_OP },
    { HDRSRCHARD_TIMER_EXP_CMD, HDRSRCHARD_DIV_FALLBACK_NO_OP },
    { HDRSRCHARD_DRC_CHANGED_CMD, HDRSRCHARD_DIV_FALLBACK_NO_OP },
    { HDRSRCHARD_SET_TAP_CMD, HDRSRCHARD_DIV_FALLBACK_NO_OP },
    { HDRSRCHARD_SET_TAP_CMD, HDRSRCHARD_DIV_FALLBACK_NO_OP },
    { HDRSRCHARD_SET_TAP_CMD, HDRSRCHARD_DIV_FALLBACK_NO_OP },
  };
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "ARD: event:%d enabled:%d", 
                  event, ard_enabled );

  ASSERT( event < HDRSRCHARD_EVENT_ENUM_SIZE );

  if ( event == HDRSRCHARD_EVENT_RUNTIME_DISABLE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "runtime disabled" );
    hdrsrchard.runtime_disabled = TRUE;
  }
  else if ( event == HDRSRCHARD_EVENT_RUNTIME_ENABLE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "runtime enabled" );
    hdrsrchard.runtime_disabled = FALSE;
  }

  if ( ard_enabled )
  {
    HDRSRCH_STM_PUT_INTERNAL_CMD( &HDRSRCHARD_SM,
                                  &hdrsrchcom_stm_group,
                                  ard_event_cmd_map[event].cmd );

	if ( ard_event_cmd_map[event].cmd == HDRSRCHARD_SUSPEND_CMD ||
		 ard_event_cmd_map[event].cmd == HDRSRCHARD_DEACTIVATE_CMD
	   )
	{
	  (void) stm_process_group( &hdrsrchcom_stm_group);
		/* In case of disabling diversity, we want to synchronize the operation of disabling
		   diversity. In another word, we want to disable diversity before the state machine
		   process returns, since the next step may relies on diversity is off. Then HDRSRCH
		   task could continue its next step like rf_sleep().
		*/
	}
  }
  else if ( ard_event_cmd_map[event].fallback == HDRSRCHARD_DIV_FALLBACK_ENABLE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "hdrsrchrfdiv_diversity_ctrl TRUE" );

    hdrsrchrfdiv_diversity_ctrl( TRUE );
  }
  else if ( ard_event_cmd_map[event].fallback == HDRSRCHARD_DIV_FALLBACK_DISABLE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "hdrsrchrfdiv_diversity_ctrl FALSE" );

    hdrsrchrfdiv_diversity_ctrl( FALSE );
  }
} /* hdrsrchard_inact_timer_cb */

/*==========================================================================

FUNCTION  HDRSRCHTC_ARD_INACT_TIMER_EXP_CB

DESCRIPTION
  This function indicates FL active with packet arrived.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
static void hdrsrchard_inact_timer_cb
( 
  uint32 timer_id
)
{
  hdrsrchard_event( HDRSRCHARD_EVENT_FL_INACT_TIMER_EXP );
} /* hdrsrchard_inact_timer_cb */

/*==========================================================================

FUNCTION  HDRSRCHARD_ACT_CB

DESCRIPTION
  This function indicates FL active with packet arrived.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
static void hdrsrchard_act_cb( boolean fl_active )
{
  /*FMAC notifies SRCH if there is FL packet received. If the input parameter 
    is true, it means a FL packet has been received and FL is back to active.
    If the input parameters is false, it means FL has not received packets for
    a period of time and FL is idle.
    For ARD, FMAC does not track if FL is idle. It only informs SRCH when there
	is a FL packet being received. The input parameter must be TRUE.
  */

  hdrsrchard_event( HDRSRCHARD_EVENT_FL_ACTIVITY );
} /* hdrsrchtc_ard_act_cb */

/*===========================================================================

                     COMMON  HELPER  FUNCTION  DEFINITIONS

===========================================================================*/


/*=========================================================================

FUNCTION HDRSRCHARD_SET_DIVERSITY

DESCRIPTION 
  This function controls diversity enable/disable. It only passes the
  command to DIV SM if diversity desired state has changed.  

DEPENDENCIES
  Must be called from ARD state machine

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_set_diversity
(
  boolean div_desired,
    /* if diversity is desired in current state */
  boolean forceful
    /* if forceful, do not respect DIV rampdump process */
)
{
  hdrsrchrfdiv_ard_diversity_ctrl(div_desired, forceful);

  hdrsrchard.div_requested = div_desired;
}

/*=========================================================================

FUNCTION HDRSRCHARD_SET_FL_ACT_CB

DESCRIPTION 
  This function registers/unregisters FL activity cb. It will only
  passes the command to fmac if there is a change.

DEPENDENCIES
  Must be called from ARD state machine

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_set_fl_act_cb
(
  boolean cb_desired
    /* if cb is desired in current state */
)
{
  if ( cb_desired )
  {
    hdrfmac_reg_fl_active_cb( hdrsrchard_act_cb, HDRFMAC_FL_ACTIVE_CB_ARD,FALSE );
  }
  else
  {
    hdrfmac_dereg_fl_active_cb( HDRFMAC_FL_ACTIVE_CB_ARD, FALSE );
  }
}

/*==========================================================================

FUNCTION  HDRSRCHARD_GET_INACT_TIME_MS

DESCRIPTION
  This function returns the time since last FL activity in ms. Note
  the time left in the timer is not accurate it's not being updated
  when the timer is running.
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Time since last FL dec HSTR in ms  

SIDE EFFECTS
  None

==========================================================================*/
static uint32 hdrsrchard_get_inact_time_ms( void )
{
  uint16 cur_hstr;
  uint16 last_fl_act_hstr;
  uint16 inact_hstr;
  uint32 inact_ms;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cur_hstr = HDRHSTR_GET_COUNT(); 
  last_fl_act_hstr = hdrdec_get_last_ftc_pkt_hstr();

  inact_hstr = ( cur_hstr - last_fl_act_hstr ) &
               HDRSRCHARD_MAX_HSTR_COUNT;
  inact_ms = HDRSRCHARD_HSTR_TO_MS( inact_hstr );

  HDR_MSG_SRCH_4( MSG_LEGACY_MED, "ARD: inact time %d-%d=>%d=%d ms",
                  cur_hstr, last_fl_act_hstr,
                  inact_hstr, inact_ms );

  return inact_ms;
}

/*==========================================================================

FUNCTION  HDRSRCHARD_LOG_EVENT

DESCRIPTION
  This function logs the ARD related event and its states
    
DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Time since last FL dec HSTR in ms  

SIDE EFFECTS
  None

==========================================================================*/
static void hdrsrchard_log_event
(
  hdrsrchard_event_enum_type  event,
  stm_state_type              new_state,
  boolean                     div
)
{
  uint8 drc = hdrsrchlog_get_pred_drc_rate();
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_6( MSG_LEGACY_MED, 
                  "ARD: evt:%d new st:%d time_left:%d div?:%d drc:%d tap?:%d",
                  event,
                  new_state,
                  hdrsrchard.time_left,
                  div,
                  drc,
                  hdrsrchard.tap );

  hdrsrchlog_log_ard_event( event, 
                            new_state,
                            hdrsrchard.time_left,
                            div,
                            drc );

  /* Currently only uses F3. Log packet will be added in later phase */
}

/*===========================================================================

                  STATE  MACHINE  ENTRY/EXIT  FUNCTIONS

===========================================================================*/

/*=========================================================================

FUNCTION HDRSRCHARD_START_STM

DESCRIPTION 
  Start ARD stm.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
void hdrsrchard_start_stm( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  stm_activate( &HDRSRCHARD_SM, &hdrsrchcom_stm_group, TRUE );
} /* hdrsrchrfdiv_start_stm */

/*===========================================================================

FUNCTION HDRSRCHRFDIV_INIT

DESCRIPTION
  Initializes the diversity state machine.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void hdrsrchard_init
(
  stm_group_type *group
    /* The group this state machine belongs to. */
)
{
  static boolean initialized = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH(MSG_LEGACY_MED, "Init called");

  if ( !initialized )
  {
    rex_def_timer_ex( &hdrsrchard.inact_timer, 
                      hdrsrchard_inact_timer_cb,
                      0 );
  
    initialized = TRUE;
  }
}

/*=========================================================================

FUNCTION HDRSRCHARD_ENTER_INACTIVE

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_enter_inactive
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  rex_clr_timer( &hdrsrchard.inact_timer );

  hdrsrchard_set_fl_act_cb( FALSE );

  /* deactivated due to exiting TC, disable DIV */
  hdrsrchard_set_diversity( FALSE, TRUE );
}

/*=========================================================================

FUNCTION HDRSRCHARD_ENTER_FL_ACTIVE

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_enter_fl_active
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  rex_set_timer( &hdrsrchard.inact_timer,
                 hdrsrchard.time_left );  
  
  hdrsrchard_set_fl_act_cb( FALSE );

  hdrsrchard_set_diversity( TRUE, FALSE );
}

/*=========================================================================

FUNCTION HDRSRCHARD_ENTER_FL_INACTIVE_DRC_LOW

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_enter_fl_inactive_drc_low
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  rex_clr_timer( &hdrsrchard.inact_timer );
  
  hdrsrchard_set_fl_act_cb( TRUE );

  hdrsrchard_set_diversity( TRUE, FALSE );
}

/*=========================================================================

FUNCTION HDRSRCHARD_ENTER_FL_INACTIVE_DRC_HIGH

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_enter_fl_inactive_drc_high
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  /* should not enter this state if ARD disabled from NV */
  rex_clr_timer( &hdrsrchard.inact_timer );
    
  hdrsrchard_set_fl_act_cb( TRUE );

  hdrsrchard_set_diversity( FALSE, FALSE );
}

/*=========================================================================

FUNCTION HDRSRCHARD_ENTER_SUSPENDED

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_enter_suspended
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  rex_clr_timer( &hdrsrchard.inact_timer );
  
  hdrsrchard_set_fl_act_cb( FALSE );

  hdrsrchard_set_diversity( FALSE, TRUE );
}

/*=========================================================================

FUNCTION HDRSRCHARD_ENTER_TAP

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/
static void hdrsrchard_enter_tap
(
  void          *payload,                        
    /* command payload        */

  stm_state_type previous_state
    /* previous state         */
)
{
  rex_clr_timer( &hdrsrchard.inact_timer );
  
  hdrsrchard_set_fl_act_cb( FALSE );

  if (!hdrsrchard.suspended)
  {
    hdrsrchard_set_diversity( TRUE, FALSE );
      /* disable ARD, DIV is always ON when in TAP */
  }
}

/*===========================================================================

                  STATE  MACHINE  TRANSITION  FUNCTIONS

===========================================================================*/


/*=========================================================================

FUNCTION HDRSRCHARD_ACTIVATE_TF

DESCRIPTION 
  This function activates ARD state machine

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_FL_ACTIVE_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_activate_tf
( 
  void                       *payload
    /* Payload */
)
{ 
  stm_state_type             state;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  NOTUSED( payload );

  hdrsrchard.tap = hdrsrchafc_get_tap_status();

  if ( hdrsrchard.tap ||
       hdrsrchard.runtime_disabled )
  {
    state = HDRSRCHARD_TAP_STATE;
  }
  else
  {
    hdrsrchard.time_left = HDRSRCHARD_FL_INACTIVE_TIME_MS;
    state = HDRSRCHARD_FL_ACTIVE_STATE;
  }

  hdrsrchard.suspended = FALSE;

  hdrsrchard_log_event( HDRSRCHARD_EVENT_ENTER_TC, 
                        state,
                        TRUE );

  return state;

} /* hdrsrchard_activate_tf */

/*=========================================================================

FUNCTION HDRSRCHARD_DEACTIVATE_TF

DESCRIPTION 
  This function deactivates ARD state machine

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_INACTIVE_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_deactivate_tf
( 
  void                       *payload
    /* Payload */
)
{ 
  NOTUSED( payload );

  hdrsrchard.time_left = 0;

  hdrsrchard_log_event( HDRSRCHARD_EVENT_EXIT_TC, 
                        HDRSRCHARD_INACTIVE_STATE,
                        FALSE );

  return HDRSRCHARD_INACTIVE_STATE;
} /* hdrsrchard_activate_tf */

/*=========================================================================

FUNCTION HDRSRCHARD_SET_TAP_TF

DESCRIPTION 
  This function deactivates/activates ARD state machine due to set TAP
  command. When DRC is fixed, ARD state machine is disabled. DIV is
  controlled by DIV SM alone. When DRC is variable, ARD logic takes
  effect.

DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_INACTIVE_STATE if TAP
  STM_SAME_STATE            if not TAP

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_set_tap_tf
( 
  void                       *payload
    /* Payload */
)
{ 
  stm_state_type             state;
  boolean                    div;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  NOTUSED( payload );

  hdrsrchard.tap = hdrsrchafc_get_tap_status();

  if ( hdrsrchard.tap ||
       hdrsrchard.runtime_disabled )
  {
    /* Fixed DRC or runtime disabled */
    state = HDRSRCHARD_TAP_STATE;
    hdrsrchard.time_left = 0;
    div = FALSE;
  }
  else if ( hdrsrchard.suspended )
  {
    /* Since HDR is in tuneaway, we go to suspend state */
    state = HDRSRCHARD_SUSPENDED_STATE;
    hdrsrchard.time_left = HDRSRCHARD_FL_INACTIVE_TIME_MS;
    div = FALSE;
  }
  else 
  {
    /* Otherwise, go to FL active state */
    state = HDRSRCHARD_FL_ACTIVE_STATE;
    hdrsrchard.time_left = HDRSRCHARD_FL_INACTIVE_TIME_MS;   
    div = hdrsrchard.div_requested;
  }

  hdrsrchard_log_event( HDRSRCHARD_EVENT_SET_TAP, 
                        state,
                        div );

  return state;
} /* hdrsrchard_activate_tf */

/*=========================================================================

FUNCTION HDRSRCHARD_SUSPEND_TF

DESCRIPTION 
  This function suspends ARD state machine due to PRI chain tuneaway.
  The inactivity timer is reserved and will be restored after tuneback.
 
DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_SUSPEND_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_suspend_tf
( 
  void                       *payload
    /* Payload */
)
{
  uint32                inact_time = hdrsrchard_get_inact_time_ms();
  stm_state_type        state;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  NOTUSED( payload );

  if ( hdrsrchard.tap )
  {
    state = STM_SAME_STATE;
    hdrsrchrfdiv_diversity_ctrl( FALSE );
      /* pass thru to DIV SM */
  }
  else
  {
    hdrsrchard.time_left = ( HDRSRCHARD_FL_INACTIVE_TIME_MS > inact_time ) ?
                           ( HDRSRCHARD_FL_INACTIVE_TIME_MS - inact_time ) : 0;                        
    
    state = HDRSRCHARD_SUSPENDED_STATE;
  }

  hdrsrchard.suspended = TRUE;

  hdrsrchard_log_event( HDRSRCHARD_EVENT_SUSPEND, 
                        state,
                        FALSE );

  return state;
} /* hdrsrchard_activate_tf */

/*=========================================================================

FUNCTION HDRSRCHARD_TIMER_EXP_TF

DESCRIPTION 
  This function checks the FL inactivity time and updates the timer.
  Based on 1) FL inactivity time and 2) pred. DRC, ARD state machine
  may transition into FL_ACT/FL_INACT_DRC_HIGH/FL_INACT_DRC_LOW state.
 
DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_FL_ACTIVE_STATE or
  HDRSRCARD_FL_INACTIVE_DRC_HIGH_STATE or
  HDRSRCARD_FL_INACTIVE_DRC_LOW_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_timer_exp_tf
( 
  void                       *payload
    /* Payload */
)
{ 
  stm_state_type             state;
  boolean                    drc_low;
  uint32                     inact_time = hdrsrchard_get_inact_time_ms();
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  NOTUSED( payload );

  /* should not receive this command if ARD disabled from NV */

  if ( HDRSRCHARD_FL_INACTIVE_TIME_MS > inact_time )
  {
    hdrsrchard.time_left = HDRSRCHARD_FL_INACTIVE_TIME_MS - inact_time;

    rex_set_timer( &hdrsrchard.inact_timer,
                   hdrsrchard.time_left );

    state = HDRSRCHARD_FL_ACTIVE_STATE;
  }
  else
  {
    hdrsrchard.time_left = 0;

    drc_low = hdrsrchlog_is_drc_below_div_on_thres();
  
    state = drc_low ? HDRSRCHARD_FL_INACTIVE_DRC_LOW_STATE :
                      HDRSRCHARD_FL_INACTIVE_DRC_HIGH_STATE;
  }

  hdrsrchard_log_event( HDRSRCHARD_EVENT_FL_INACT_TIMER_EXP, 
                        state,
                        ( state != HDRSRCHARD_FL_INACTIVE_DRC_HIGH_STATE ) );

  return state;
} /* hdrsrchard_activate_tf */

/*=========================================================================

FUNCTION HDRSRCHARD_FL_ACTIVITY_TF

DESCRIPTION 
  This function starts FL activity timer and transition to FL_ACT state.
 
DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_FL_ACTIVE_STATE or
  HDRSRCARD_FL_INACTIVE_DRC_HIGH_STATE or
  HDRSRCARD_FL_INACTIVE_DRC_LOW_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_fl_activity_tf
( 
  void                       *payload
    /* Payload */
)
{
  NOTUSED( payload );

  /* should not receive this command if ARD disabled from NV */

  hdrsrchard.time_left = HDRSRCHARD_FL_INACTIVE_TIME_MS;

  hdrsrchard_log_event( HDRSRCHARD_EVENT_FL_ACTIVITY, 
                        HDRSRCHARD_FL_ACTIVE_STATE,
                        TRUE );

  return HDRSRCHARD_FL_ACTIVE_STATE;
} /* hdrsrchard_activate_tf */

/*=========================================================================

FUNCTION HDRSRCHARD_DRC_CHANGED_TF

DESCRIPTION 
  This function updates ARD state based on current DRC.
 
DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_INACTIVE_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_drc_changed_tf
( 
  void                       *payload
    /* Payload */
)
{
  stm_state_type             state;
  boolean                    drc_low;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* should not receive this command if ARD disabled from NV */

  drc_low = hdrsrchlog_is_drc_below_div_on_thres();

  state = drc_low ? HDRSRCHARD_FL_INACTIVE_DRC_LOW_STATE :
                    HDRSRCHARD_FL_INACTIVE_DRC_HIGH_STATE;

  hdrsrchard_log_event( HDRSRCHARD_EVENT_DRC_CHANGED, 
                        state,
                        ( state != HDRSRCHARD_FL_INACTIVE_DRC_HIGH_STATE ) );

  return state;
}

/*=========================================================================

FUNCTION HDRSRCHARD_RESUME_TF

DESCRIPTION 
  This function updates ARD state based on current DRC.
 
DEPENDENCIES
  None

RETURN VALUE
  HDRSRCHARD_INACTIVE_STATE

SIDE EFFECTS
  None

=========================================================================*/
static stm_state_type hdrsrchard_resume_tf
( 
  void                       *payload
    /* Payload */
)
{
  stm_state_type             state;
  boolean                    drc_low;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  NOTUSED( payload );

  if ( hdrsrchard.tap )
  {
    state = STM_SAME_STATE;
    hdrsrchrfdiv_diversity_ctrl( TRUE );
      /* pass thru to DIV SM */
  }  
  else if ( hdrsrchard.time_left > 0 )
  {
    state = HDRSRCHARD_FL_ACTIVE_STATE;
  }
  else
  {
    drc_low = hdrsrchlog_is_drc_below_div_on_thres();
  
    state = drc_low ? HDRSRCHARD_FL_INACTIVE_DRC_LOW_STATE :
                      HDRSRCHARD_FL_INACTIVE_DRC_HIGH_STATE;
  }

  hdrsrchard.suspended = FALSE;

  hdrsrchard_log_event( HDRSRCHARD_EVENT_RESUME,
                        state,
                        FALSE );

  return state;
}
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
