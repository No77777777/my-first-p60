/*===========================================================================
             R R C     S T A T E    C H A N G E    M A N A G E R

DESCRIPTION

  This module manages State Transitions of the RRC state machine.
  Since RRC has concurrent procedures, when one procedure wants
  to change RRC's state, other active procedures might be affected
  by the state change. This module provides a mechanism to notify
  procedures when a state transition occurs.

  Procedures can register with this module indicating that they
  need to be notified about a particular state transition. When the
  desired state transition occurs, this module sends an internal
  command to that procedure to notify it of the state change.

EXTERNALIZED FUNCTIONS

  tdsrrcscmgr_register_for_scn   Called by procedures to register to be
                              notified of specific state changes.

  tdsrrcscmgr_event_handler      Main function that handles commands sent
                              by the dispatcher to this module.

INITIALIZATION AND SEQUENCING REQUIREMENTS



Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcscmgr.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/13/11   mkh     Added log version to RRC log packets
09/08/11   mkh     Events renamed to TDS
07/07/11   mkh     Renamed log pkts to TDSxxx. 
03/10/11   hx      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "tdsrrcscmgr.h"
#include "comdef.h"
#include "err.h"
#include "msg.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "event.h"
#include "tdsrrclogging.h"
#include "tdsrrcccm.h"


#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
#include "tdsrrcmm_v.h"
#include "cmwll.h"
#endif

#include "tdsrrccspi.h"

#include "tdsrrcqsh.h"


#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif

#ifdef FEATURE_TDSCDMA_RRC_REPORT_STATE_MODE_TO_MCS
#include "wcn_coex_mgr.h"
#include "tdsrrcmcm.h"
#endif
#include "ds3gpp_api.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Structure to hold each state change notification that has been
registered with this module */
typedef struct
{
  tdsrrc_proc_e_type  procedure;   /* Procedure to be notified */
  tdsrrc_state_e_type from_state;  /* State from which transition occurs */
  tdsrrc_state_e_type to_state;    /* state to which transition occurs */
}tdsrrc_state_change_notification_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* Array to store all the registered state change notifications */
tdsrrc_state_change_notification_type tdsrrcscn[TDSRRC_TOT_SCN];


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* Counter to keep track of the number of registrations */
uint32 tdsrrcscn_reg_count = 0;

RRC_HOME_ZONE_IND_CB_TYPE *tdsrrc_home_zone_ind_cb_func;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* The Current RRC State */
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_state_e_type  tdsrrc_curr_state;

/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_state_e_type tdsrrc_prev_state;  /* To save previous state to be sent to
                                state change manager */

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */



/*===========================================================================

FUNCTION RRC_GET_STATE

DESCRIPTION
  This function returns the current state of RRC.

DEPENDENCIES
  None

RETURN VALUE
  Current state of RRC.

SIDE EFFECTS
  None

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

tdsrrc_state_e_type  tdsrrc_get_state( void )
{
  return( tdsrrc_curr_state );
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/



/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION RRC_INIT_STATE

DESCRIPTION

  This function initializes the default state of RRC at powerup.
  This function should be called only once, at powerup.

DEPENDENCIES
  None

RETURN VALUE
  Current state of RRC.

SIDE EFFECTS
  None

===========================================================================*/
void  tdsrrc_init_state( void )
{
  /* Se the default state of RRC at power-up as the disconnected state */
  tdsrrc_curr_state = TDSRRC_STATE_DISCONNECTED;


  /* Reset callback func pointer to NULL */
  tdsrrc_home_zone_ind_cb_func = NULL;



}



/*===========================================================================

FUNCTION RRC_INITIATE_STATE_CHANGE

DESCRIPTION

  This function initiates a state change in RRC by posting a
  TDSRRC_NEW_STATE_IND command with the given RRC state. When this
  command is processed, the RRC state will be changed to the
  new state and state change notifications will be sent if needed.

  The reason the new state is not set righ away is that we
  want to avoid any potential race conditions with commands
  that are already queued. When commands that are already
  queued are processed before the TDSRRC_NEW_STATE_IND command they
  may be calling tdsrrc_get_state to get the current state which
  may lead to some race conditions.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void  tdsrrc_initiate_state_change
(
  tdsrrc_state_e_type  state  /* New state */
)
{

  tdsrrc_cmd_type *cmd_ptr;        /* To send a command to state change manager */

  /* First make sure the new state is a valid state. */
  if( !(TDSVALID_RRC_STATE(state)) )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"New RRC state is invalid - %d",state);
    return;
  }

  /* Send a TDSRRC_NEW_STATE_IND command through the internal command queue. */
  if( (cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Initiating RRC State Change");
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_NEW_STATE_IND;
    cmd_ptr->cmd.new_state_ind.new_state = state;
    tdsrrc_put_int_cmd(cmd_ptr);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't send NEW_STATE_IND cmd - no memory");
  }
}

/*===========================================================================

FUNCTION RRC_GET_PREV_STATE

DESCRIPTION
  This function returns the previous state of RRC.

DEPENDENCIES
  None

RETURN VALUE
  Current state of RRC.

SIDE EFFECTS
  None

===========================================================================*/
tdsrrc_state_e_type  tdsrrc_get_prev_state( void )
{
  return( tdsrrc_prev_state );
}


/*===========================================================================

FUNCTION RRCSCMGR_REGISTER_FOR_SCN

DESCRIPTION

  This function registers State change notification requests from
  other procedures. When a procedure needs to be notified when a
  particular RRC state transition occurs, that procedure must register
  using this function. The from_state is the state from which the
  transition occurs and the to_state is the state to which the transition
  occurs. Wildcard may be used for both the from_state and to_state.
  The wildcard to be used is TDSRRC_STATE_WILDCARD. When a wildcard is
  used for both from and to states, transitions from any state to
  any other state will be notified.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcscmgr_register_for_scn
(
  tdsrrc_proc_e_type  procedure,   /* Procedure that's doing this registration */
  tdsrrc_state_e_type from_state,  /* State from which the transition occurs. This
                                can be "TDSRRC_STATE_WILDCARD" to indicate that the
                                from state can be any RRC state */
  tdsrrc_state_e_type to_state     /* State to which the transition occurs. This
                                can be "TDSRRC_STATE_WILDCARD" to indicate that the
                                to state can be any RRC state */
)
{

  uint32 localcount;

  /* Check if the given "from" state is either a valid state or a wildcard. */
  if( (!(TDSVALID_RRC_STATE(from_state))) && (from_state != TDSRRC_STATE_WILDCARD) )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid state registered %d",from_state);
    return;
  }

  /* Check if the given "to" state is either a valid state or a wildcard. */
  if( (!(TDSVALID_RRC_STATE(to_state))) && (to_state != TDSRRC_STATE_WILDCARD) )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid state registered %d",to_state);
    return;
  }

  /* Check if there is space available to register another state change */
  if( tdsrrcscn_reg_count >= (TDSRRC_TOT_SCN - 1) )
  {
    ERR_FATAL("No space for state change registration!",0,0,0);
  }

  /* check for the duplicate registration */
  for (localcount=0; localcount<tdsrrcscn_reg_count; localcount++)
  {
    if (procedure == tdsrrcscn[localcount].procedure)
    {
      if ((tdsrrcscn[localcount].from_state == TDSRRC_STATE_WILDCARD)
         || (tdsrrcscn[localcount].from_state == from_state))
      {
        if ((tdsrrcscn[localcount].to_state == TDSRRC_STATE_WILDCARD)
            || (tdsrrcscn[localcount].to_state == to_state))
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Duplicate SCM reg! proc:%d,from:%d,to:%d",
                      procedure, from_state, to_state);
          return;
        }
      }
    }
  }      /*end of for loop */

  /* Ok to register this. */
  tdsrrcscn[tdsrrcscn_reg_count].procedure = procedure;   /* Copy procedure */
  tdsrrcscn[tdsrrcscn_reg_count].from_state = from_state; /* Copy from state */
  tdsrrcscn[tdsrrcscn_reg_count].to_state = to_state;     /* Copy to_state */
  tdsrrcscn_reg_count++; /* Increment counter */

}

/*===========================================================================

FUNCTION tdsrrcscmgr_update_rrc_state_tds

DESCRIPTION

  This function updates the RRC state TDS UI event info and notifies CM

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcscmgr_update_rrc_state_tds
(
  tdsrrc_state_e_type new_state
)
{
  sys_wcdma_rrc_state_e_type new_sys_rrc_state = SYS_WCDMA_RRC_STATE_DISCONNECTED;

  switch(new_state)
  {
    case TDSRRC_STATE_DISCONNECTED:
    case TDSRRC_STATE_CONNECTING:
      new_sys_rrc_state = SYS_WCDMA_RRC_STATE_DISCONNECTED;
      break;
    case TDSRRC_STATE_CELL_DCH:
      new_sys_rrc_state = SYS_WCDMA_RRC_STATE_CELL_DCH;
      break;
    case TDSRRC_STATE_CELL_FACH:
      new_sys_rrc_state = SYS_WCDMA_RRC_STATE_CELL_FACH;
      break;
    case TDSRRC_STATE_CELL_PCH:
      new_sys_rrc_state = SYS_WCDMA_RRC_STATE_CELL_PCH;
      break;
    case TDSRRC_STATE_URA_PCH:
      new_sys_rrc_state = SYS_WCDMA_RRC_STATE_URA_PCH;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid state %d",new_state);
  }

#ifdef FEATURE_DUAL_SIM
  ds_3gpp_rrc_state_update
  ( 
    tdsrrc_get_as_id(),
    SYS_SYS_MODE_TDS,
    new_sys_rrc_state
  );
#else
  ds_3gpp_rrc_state_update
  ( 
    SYS_SYS_MODE_TDS,
    new_sys_rrc_state
  );
#endif

#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
  if(RRC_IS_TDS_UI_EVENT_ENABLED(SYS_WCDMA_RRC_STATE))
  {
    tds_UI_event_info.wcdma_event_info.wcdma_rrc_state = new_sys_rrc_state;
    tds_UI_event_info.bitmask |= SYS_WCDMA_RRC_STATE;
    /*Call the CM API to set the Signal*/
    cm_stats_set_modem_info_signal(SYS_MODEM_MODULE_WCDMA);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"FTD_DBG: RRC State change bit mask set,bit mask %x",tds_UI_event_info.bitmask);
  }
#endif
}

/*===========================================================================

FUNCTION RRCSCMGR_NOTIFY_STATE_CHANGE_TO_PROCS

DESCRIPTION

  This function looks through the stored registrations to see
  if the given state transition should be notified to any of
  the registered procedures. This is called by the event_handler
  when RRC moves to a new state.

  If it finds procedures that need to be notified, it sends
  an internal command TDSRRC_STATE_CHANGE_IND to the internal command
  queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcscmgr_notify_state_change_to_procs
(
  tdsrrc_state_e_type old_state,  /* RRC's previous state */
  tdsrrc_state_e_type new_state   /* RRC's new state      */
)
{
  uint32 count;
  tdsrrc_cmd_type *cmd_ptr;
  uint32 proc_count=0; /* Number of procedures that would
                       get the notification */
  tdsrrc_event_state_change_info_type  state_change_event;

  TDSRRC_MSG0(MSG_LEGACY_LOW,"Sending notification about state change");

  /* Get a command buffer */
  if( (cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
  {
    /* Fill up the command type, the old and new states  */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_STATE_CHANGE_IND;
    cmd_ptr->cmd.state_change_ind.new_state = new_state;
    cmd_ptr->cmd.state_change_ind.previous_state = old_state;

    /* Look at each of the registered entries to see if this
    state change should be notified to any of the procedures.
    if so fill up the procedure name in the command */
    for( count = 0; count < tdsrrcscn_reg_count; count++ )
    {
      /* The from state should either match the old state or be wildcard
      AND the to state should either match the new state or be wildcard */
      if( ((tdsrrcscn[count].from_state == old_state) ||
          (tdsrrcscn[count].from_state == TDSRRC_STATE_WILDCARD)) &&
          ((tdsrrcscn[count].to_state == new_state) ||
          (tdsrrcscn[count].to_state == TDSRRC_STATE_WILDCARD)) &&
          (tdsrrcscn[count].procedure != TDSRRC_PROCEDURE_NONE) )
      {
        /* We have a match. Add this procedure. */
        cmd_ptr->cmd.state_change_ind.procedure[proc_count]=
          tdsrrcscn[count].procedure;
        proc_count++;
      }
    }/* end for */
  }/* end if */
  else
  {
    /* Out of memory - declare an error and return */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't create state change ind - No memory!");
    return;
  }

  /* Always enqueue command even when count =0, since LLC has to get this ALWAYS */
  cmd_ptr->cmd.state_change_ind.num_procedures = proc_count;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending STATE_CHANGE_IND to %d procs",proc_count);

  state_change_event.current_rrc_state = (uint8)old_state;
  state_change_event.next_rrc_state = (uint8)new_state;
  state_change_event.new_rate = 0;

  event_report_payload(EVENT_TDSCDMA_RRC_STATE, /* event toindicate that RRC has changed the state */
                       sizeof(tdsrrc_event_state_change_info_type), /* size of the payload */
                       (void *)&state_change_event); /* The state change structure contains the
                                                         current state and the next state after transition*/
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Changed state from %d to %d ",state_change_event.current_rrc_state,state_change_event.next_rrc_state);

  /* Now, send the command. */
  tdsrrc_put_int_cmd( cmd_ptr );

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
   #error code not present
#endif


  /*Notify the state change indication to the registered callback */
  if (tdsrrc_home_zone_ind_cb_func)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Notifying hzi func");
    tdsrrc_trigger_home_zone_ind();
  }

  tdsrrcscmgr_update_rrc_state_tds(new_state);

  if((new_state == TDSRRC_STATE_CELL_PCH) ||
     (new_state == TDSRRC_STATE_URA_PCH))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: Release the lock,new state %d",new_state);
    tdsrrcwrm_release_lock();
  }

#ifdef FEATURE_TDSCDMA_RRC_REPORT_STATE_MODE_TO_MCS
  tdsrrc_send_rrc_mode_and_state_to_mcs();
#endif
}/* end function - tdsrrcscmgr_notify_state_change_to_procs */


/*===========================================================================

FUNCTION RRC_TRIGGER_HOME_ZONE_IND

DESCRIPTION

This function will call the register HZI cb func to notify the rrc state and
required location information

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrc_trigger_home_zone_ind
(
  void
)
{
  /* This stores state and location info needed by upper layers*/
  home_zone_ind_type hzi;
  /* This stores the LAI received in SIB1 */
  rrc_lai_type  lai;

  memset(&hzi,0,sizeof(home_zone_ind_type));
  memset(&lai,0,sizeof(rrc_lai_type));

  /* Populate all state and location information */
  hzi.prev_state = (uint32)tdsrrc_prev_state;
  hzi.curr_state = (uint32)tdsrrc_curr_state;
  if (TDSRRC_CELL_LOSS_NO_NEW_CELL == tdsrrcccm_get_serv_plmn_cellid(&hzi.cell_id, &hzi.plmn_id))
  {
    hzi.loc_info_invalid = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"HZI,cell loss");
  }

  if (SUCCESS == tdsrrc_get_lac_id(&lai))
  {
    hzi.lac[0] = lai.lac[0];
    hzi.lac[1] = lai.lac[1];
  }
  else
  {
    hzi.loc_info_invalid = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to get LAC for HZI");
  }
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Calling HZI callback func");
  tdsrrc_home_zone_ind_cb_func(hzi);
}





#if (!(defined(T_WINNT)))
/*===========================================================================

FUNCTION RRCSCMGR_LOG_STATE_CHANGE

DESCRIPTION

  This function sends a log indicating the new RRC state to QXDM.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcscmgr_log_state_change( void )
{
  tdscdma_rrc_modes_and_states_log_packet_type *log_ptr; /* Pointer to hold log packet */
  uint32 log_record_length;                 /* Length of the log packet   */

  /* Since there is a header included in the log packet, the length is
  calculated using the FPOS macro */
  log_record_length = (FPOS( tdscdma_rrc_modes_and_states_log_packet_type, rrc_state) +
                       sizeof(uint8));

  /* Allocate the packet memory */
  log_ptr = (tdscdma_rrc_modes_and_states_log_packet_type *)log_alloc( TDSCDMA_RRC_MODES_AND_STATES_LOG_PACKET, log_record_length );

  /* If the memory allocation is sucssful, fill in the value of the
  new state */
  if( log_ptr != NULL )
  {
    log_ptr->version   = TDSRRC_LOG_VERSION;
    log_ptr->rrc_state = (uint8) tdsrrc_curr_state;
    TDSRRC_MSG0(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_MODES_AND_STATES_LOG_PACKET");
    /*commit the log record */
    log_commit(log_ptr);
  }
  /* If the memory allocation failed, QXDM was not on, so we
  can ignore the logging of this event. */
}
#endif /* (!(defined(T_WINNT))) */

#ifdef FEATURE_TDSCDMA_RRC_REPORT_STATE_MODE_TO_MCS
/*===========================================================================

FUNCTION tdsrrc_send_rrc_mode_and_state_to_mcs

DESCRIPTION
  This function sends RRC mode and state to MCS .
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void tdsrrc_send_rrc_mode_and_state_to_mcs(void)
{
  cxm_coex_tech_l3_state_ind_s l3_state_ind;
  cxm_tech_l3_state_e *l3_state = &l3_state_ind.l3.state;
  cxm_tech_op_mode_e *l3_mode = &l3_state_ind.l3.mode; 

  memset(&l3_state_ind, 0, sizeof(cxm_coex_tech_l3_state_ind_s));

  l3_state_ind.tech = CXM_TECH_TDSCDMA;

  switch(tdsrrc_get_state())
  {
    case TDSRRC_STATE_DISCONNECTED:
      *l3_state = CXM_TECH_L3_IDLE;
      break;
      
    case TDSRRC_STATE_CONNECTING:
      *l3_state =  CXM_TECH_L3_CONNECTING;
      break;
      
    case TDSRRC_STATE_CELL_FACH:
      *l3_state =  CXM_TECH_L3_CELL_FACH;
      break;
      
    case TDSRRC_STATE_CELL_DCH:
      *l3_state =  CXM_TECH_L3_CELL_DCH;
      break;
      
    case TDSRRC_STATE_CELL_PCH:
      *l3_state =  CXM_TECH_L3_CELL_PCH;
      break;
      
    case TDSRRC_STATE_URA_PCH:
      *l3_state =  CXM_TECH_L3_URA_PCH;
      break;
      
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Current rrc state is invalid");
      *l3_state = CXM_TECH_L3_IDLE;
      break;
  }

  if(TDSRRC_MODE_INACTIVE == tdsrrcmcm_get_rrc_mode())
  {
    *l3_mode = CXM_TECH_INACTIVE;
    *l3_state = CXM_TECH_L3_INACTIVE;
  }
  else
  {
    *l3_mode = CXM_TECH_ACTIVE;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Report to MCS, mode: %d,state: %d",*l3_mode,*l3_state);
  
  msgr_init_hdr(&l3_state_ind.msg_hdr, MSGR_TDSCDMA_RRC, MCS_CXM_COEX_TECH_L3_STATE_IND);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&l3_state_ind.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&l3_state_ind.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  msgr_send(&l3_state_ind.msg_hdr, sizeof(cxm_coex_tech_l3_state_ind_s));
}
#endif

/*===========================================================================

FUNCTION RRCSCMGR_EVENT_HANDLER

DESCRIPTION

  This is the event handler for the state change manager module.
  Only one event is expected here and that is the internal command
  TDSRRC_NEW_STATE_IND indicating that RRC has moved to a new state.
  This function is called by the dispatcher.

  This function initiates the process of notifying procedures, if any,
  that need to be notified of the state change.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcscmgr_event_handler
(
  tdsrrc_cmd_type *cmd_ptr    /* Pointer to received command */
)
{

  /* We handle only one type of command - TDSRRC_NEW_STATE_IND to indicate
  that we have a new RRC state. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_NEW_STATE_IND:

      /* First save the previous state and set the new RRC state */
      tdsrrc_prev_state = tdsrrc_curr_state;
      tdsrrc_curr_state = cmd_ptr->cmd.new_state_ind.new_state;

      TDSRRC_MSG1(MSG_LEGACY_MED,"RRC State set to %d",tdsrrc_curr_state);

      /* Make sure the previous and current states are not the same. */
      if( tdsrrc_prev_state == tdsrrc_curr_state )
      {
        /* Bogus state change. No need to inform state change manager,
        just return */
        TDSRRC_MSG0(MSG_LEGACY_MED,"Prev state and current state are same!");
        return;
      }


      /* Notify all the procedures that registered to be notified
      about this state change */
      tdsrrcscmgr_notify_state_change_to_procs( tdsrrc_prev_state, tdsrrc_curr_state );


#if (!(defined(T_WINNT)))
      /* Log the state change with QXDM */
      tdsrrcscmgr_log_state_change();
#endif /* (!(defined(T_WINNT))) */

#ifdef FEATURE_QSH_EVENT_METRIC
      TDSRRC_QSH_RRC_STATE_UPDATE(tdsrrc_curr_state);
#endif

      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid cmd %d Rx'ed by SCMGR",cmd_ptr->cmd_hdr.cmd_id);
      break;
  }
}
