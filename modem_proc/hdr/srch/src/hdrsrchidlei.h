#ifndef HDRSRCHIDLEI_H
#define HDRSRCHIDLEI_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       H D R   S E A R C H   I D L E   U T I L I T Y   F U N C T I O N S

GENERAL DESCRIPTION
  This module provides general purpose routines used in hdr searcher idle
  state. This file has been created to support different substates of idle
  state operation including monitor, broadcast, access etc.


EXTERNALIZED FUNCTIONS (Global)


EXTERNALIZED FUNCTIONS (Regional)


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2004-2018 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchidlei.h#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     --------------------------------------------------------- 
01/24/2018   vke     Disallow ASdiv switch when connection close is inprogress
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
01/15/2015   arm     Increased access tuneaway overhead time by 3 ms, to 
                     account for delay in timer processing and div disable
08/01/2014   sat     Changes to ignore Fast connect TCA processing while TA
08/03/2014   arm     Changes to request RX_ANY chain in DR mode for idle. 
05/05/2014   ljl     Added hdrsrchidle_channel_change_and_nset_update_cmd().
01/22/2013   wsh     Delay tuneaway when chan_change is in progress
06/18/2013   arm     Reverted disabling access handoff fix.
05/30/2013   arm     Disabled access handoff after receiving ac ack.
08/15/2012   smd     Supported idle tune away. 
06/21/2012   mbs     Initialized monitor state when entering from reacq.
06/06/2012   grl     Return error value for hdrsrchidle_idle_mode_cmd
11/11/2011   arm     HDR idle and access state diversity implemetation. 
03/11/2010   ljl     Supported FEATURE_HDR_SUPPORTS_FW_TIMING_STATE.
11/10/2008   ljl     Supported HDRSRCH_ACMAC_DISABLE_TA_CMD.
12/26/2007   ljl     Added amac_ready flag.
10/03/2006   ljl     Added declaration for hdrsrchidle_acmac_ta_time_ok_cmd()
                     and hdrsrchidle_acmac_ready_cmd().
08/28/2006   sq      Added support for FEATURE_HDR_QSM
05/03/2006   ljl     Added support for hybrid access
09/01/2005   grl     Added support for lock releases during access attempts.
05/31/2005   grl     Added the sleep_time for when we next enter sleep.
01/13/2005   sq      Mainlined FEATURE_HDR_NO_HO_DURING_SCC
01/10/2005   sq      Removed function hdrsrchidle_access_end_cmd
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
09/10/2004   sq      Removed function hdrsrchidle_handle_idle_ho
07/30/2004   sq      Included more common idle state functions
05/11/2004   aaj     Created this file

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/


#include "hdr_variation.h"
#include "hdrsrchtypes.h"
#include "hdrsrchdef.h"
#include "hdrsrchi.h"

#include "hdrsrchcmd.h"
#include "hdrsrchsect.h"

#include "hdrsrchstate.h"
#include "hdrsrchstates.h"

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
#include "hdrlog.h"
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros items needed 
by this module.

==========================================================================*/

#define HDRSRCH_IDLE_SEARCH_INTERVAL_MS       30
  /* Time interval between two search cycles */


#define HDRSRCHIDLE_NO_HANDOFF_HSTR          504
  /* MSTR at which point Idle handoffs should be disabled */


#define HDRSRCH_ACCESS_HANDOFF_HOLDOFF_MS    300
  /* Time between two access handoff requests to the same pilot */

#define HDRSRCH_ACCESS_HANDOFF_HOLDOFF \
  ( HDRSRCH_ACCESS_HANDOFF_HOLDOFF_MS / HDRSRCH_IDLE_SEARCH_INTERVAL_MS )


#define HDRSRCHIDLE_RX_RPT_TIMER  &hdrsrch.gp_timer0
#define HDRSRCHIDLE_RX_RPT_SIG    HDRSRCH_GP_TMR0_SIG

#ifdef FEATURE_HDR_HYBRID_ACCESS
#define HDRSRCHIDLE_ACCESS_TA_TIMER         (&hdrsrch.gp_timer1)
#define HDRSRCHIDLE_ACCESS_TA_SIG           HDRSRCH_GP_TMR1_SIG
#endif  /* FEATURE_HDR_HYBRID_ACCESS */

#define HDRSRCHIDLE_RX_RPT_TIME_MS          1000
  /* Time interval between RX power reports, when Idle */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
#define HDRSRCHIDLE_MONITOR_TA_TIMER         (&hdrsrch.gp_timer1)
#define HDRSRCHIDLE_MONITOR_TA_SIG           HDRSRCH_GP_TMR1_SIG
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

/* EJECT */
/*==========================================================================

                     IDLE STATE PROTOCOL STATES

==========================================================================*/

typedef enum
{
  HDRSRCHIDLE_MONITOR_STATE,
  HDRSRCHIDLE_ACCESS_STATE,

  HDRSRCHIDLE_NUM_STATES
}
hdrsrchidle_state_enum_type;


/*==========================================================================

                     STRUCTURE TYPES FOR THIS MODULE

This section contains typesdefs and other items needed by this module.

==========================================================================*/

#ifdef FEATURE_HDR_HYBRID_ACCESS
/*-------------------------------------------------------------------------
  Tune away constants
 -------------------------------------------------------------------------*/

#ifdef FEATURE_HDR_SUPPORTS_FW_TIMING_STATE
#define HDRSRCHIDLE_OVERHEAD_MS              23
#else
#define HDRSRCHIDLE_OVERHEAD_MS              18
#endif /* FEATURE_HDR_SUPPORTS_FW_TIMING_STATE */
  /* Tune away overhead */

#define HDRSRCHIDLE_OVERHEAD_SCLK \
                              (TIMETICK_SCLK_FROM_MS(HDRSRCHIDLE_OVERHEAD_MS))
  /* Minimum time from tune away */

#define HDRSRCHIDLE_EXTRA_OVERHEAD_MS (HDRSRCHIDLE_OVERHEAD_MS + 10)
#define HDRSRCHIDLE_EXTRA_OVERHEAD_SCLK \
                              (TIMETICK_SCLK_FROM_MS(HDRSRCHIDLE_EXTRA_OVERHEAD_MS))
  /* Minimum time from tune away + additional 10 ms Buffer. */


/*--------------------------------------------------------------------------
  Tune away Enums
 --------------------------------------------------------------------------*/

typedef enum
{
  HDRSRCHIDLE_TUNE_AWAY_NO_CONFLICT,

  HDRSRCHIDLE_TUNE_AWAY_DISABLED,

  HDRSRCHIDLE_TUNE_AWAY_GONE,

  HDRSRCHIDLE_TUNE_AWAY_RETURNING
}
hdrsrchidle_access_tune_away_enum_type;

/*--------------------------------------------------------------------------
  Disable tune away Structure
 --------------------------------------------------------------------------*/

typedef struct
{
  boolean                                 ta_disabled;
    /* Whether ACMAC has disabled tune away after receiving ACACK */

  timetick_type                           start_time_sclks;
    /* Sclk timestamp of NOW */

  timetick_type                           end_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  uint16                                  no_ta_duration;
    /* The duration that tune away is disabled in ms 
       no_ta_duration = sclk_to_ms( end_time_sclks - start_time_sclks) */
}
hdrsrchidle_access_disable_ta_type;


/*--------------------------------------------------------------------------
  Tune away Structure
 --------------------------------------------------------------------------*/

typedef struct
{
  hdrsrchidle_access_tune_away_enum_type  state;
    /* Tune away state */

  boolean                                 amac_ready;
    /* Whether AMAC is ready */

  boolean                                 no_tune_away;
    /* Do we know when we are supposed to tune away yet? */

  timetick_type                           start_time_sclks;
    /* Sclk timestamp of NOW */

  timetick_type                           end_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  hdrsrchrf_retain_lock_state_t           rl_state;
    /* Copy of the state of retain lock. */

  hdrsrchidle_access_disable_ta_type      disable_ta;
    /* Structure for disable tune away info */
}
hdrsrchidle_access_tune_away_type;

#endif /* FEATURE_HDR_HYBRID_ACCESS */

/*---------------------------------------------------------------------
   Access State data structure
---------------------------------------------------------------------*/

typedef struct
{
  hdrsrch_suspend_ho_cb_type                  suspend_ho_cb;
    /* Access Mode Entry callback */

  hdrsrch_request_ho_cb_type                  request_ho_cb;
    /* Access Handoff Request callback */

  int                                         thresh_0p5_db;
    /* Access Handoff threshold, in 0.5 db */

  hdrsrch_sect_struct_type                  * handoff_sector;
    /* Link we've requested a handoff to */

  uint8                                       request_holdoff;
    /* Count to prevent nagging protocol about a handoff request */

  const hdrsrch_access_handoff_params_type *  handoff;
    /* Parameters for an access mode handoff - if one is in progress */

#ifdef FEATURE_HDR_HYBRID_ACCESS
  
  hdrsrch_acmac_ta_time_cb_type               ta_time_cb;
    /* Callback function to notify ACMAC tune away time */

  hdrsrchidle_access_tune_away_type           tune_away;
    /* Tune away structure */

  hdrsrch_acmac_stop_cb_type                  stop_cb;
    /* Callback function to stop ACMAC */

  boolean                                     waiting_chan_change;
#endif /* FEATURE_HDR_HYBRID_ACCESS */

}
hdrsrchidle_access_struct_type;


/*---------------------------------------------------------------------
   Idle Handoff structure
---------------------------------------------------------------------*/

typedef struct
{
  hdrsrch_idle_ho_cb_type                     report;
    /* Idle Handoff Report callback */

  boolean                                     allowed;
    /* Flag to indicate if Idle Handoffs are allowed */

  int2                                        last_hstr;
    /* HSTR of last dump */
}
hdrsrchidle_handoff_struct_type;

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
/*--------------------------------------------------------------------------
  Idle Tune away Structure
 --------------------------------------------------------------------------*/

typedef struct
{
  boolean                                 no_tune_away;
    /* Do we know when we are supposed to tune away yet? */

  timetick_type                           start_time_sclks;
    /* Sclk timestamp of NOW */

  timetick_type                           end_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  hdrsrchrf_retain_lock_state_t           rl_state;
    /* Copy of the state of retain lock. */

  boolean                                 wait_for_channel_change;
    /* Whether we need to wait for channel change is done */
}
hdrsrchidle_tune_away_type;
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

/*---------------------------------------------------------------------
   HDR Search Idle State Common info type
---------------------------------------------------------------------*/

typedef struct
{
  hdrsrchidle_handoff_struct_type             handoff;
    /* Idle Handoff control data */

  const hdrsrch_channel_change_params_type *  channel_change;
    /* Parameters for a channel change - if one is in progress */

  hdrsrchidle_access_struct_type              access;
    /* Sleep sub-state data */

  boolean                                     supervision_timer_active;
    /* Flag indicating the pilot supervision time is running */

  boolean                                     perform_triage;
    /* Flag indicating triage is not required */

  hdrsrchidle_state_enum_type                 monitor_substate;
    /* monitor state protocol state */

  uint16                                      sleep_time;
    /* Sleep time in slots for the next time we enter sleep. */

#ifdef FEATURE_HDR_HYBRID_ACCESS
  boolean                                     waiting_for_close_loop;
    /* Whether to wait for close loop */
#endif /* FEATURE_HDR_HYBRID_ACCESS */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
  hdrsrchidle_tune_away_type                  monitor_ta;
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

  boolean                                     is_tx_pending;
    /* Are we requesting TX capable chain? */
}
hdrsrchidle_common_struct_type;


extern hdrsrchidle_common_struct_type    hdrsrchidle;


typedef 
void (*hdrsrch_link_report_func_type)
(
  hdrsrch_link_id_type            link_id,
    /* Link Id of pilot the HDR Searcher just handed off to */

  const sys_link_type *           link_ptr
    /* Pilot the HDR Searcher just handed off to */
);


/*==========================================================================

                      FUNCTION  DEFINITIONS

==========================================================================*/


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ENABLE_TUNE_AWAYS

DESCRIPTION
  Turns on the ability for tune aways to happen within the idle state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_enable_tune_aways( void );


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_DISABLE_TUNE_AWAYS

DESCRIPTION
  Turns off the ability for tune aways to happen within the idle state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_disable_tune_aways( void );


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_RF_UNLOCK_EVENT

DESCRIPTION
  This function processes the unlock RF event which can be invoked after
  hdrsrchidle_enable_tune_aways is called.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_rf_unlock_event( void );


#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRSRCHIDLE_RESUME_IDLE_INIT

DESCRIPTION
  Idle State Resumption after tune-away

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  NOne

===========================================================================*/

void hdrsrchidle_resume_idle_init ( void );


/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_TA_TIMER_EVENT

DESCRIPTION
  This function processes the access tune away timer event.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_access_ta_timer_event( void );
#endif /* FEATURE_HDR_HYBRID_ACCESS */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
/*===========================================================================

FUNCTION HDRSRCHIDLE_TA_TIMER_EVENT

DESCRIPTION
  This function processes the tune away timer event.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_ta_timer_event( void );
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_RX_POWER_REPORT

DESCRIPTION
  This function reports the RX power, and resets the timer for the next
  RX power report.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_rx_power_report( void );


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_KILL_RX_POWER_REPORT

DESCRIPTION
  This function reports the RX power, and resets the timer for the next
  RX power report.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_kill_rx_power_report( void );


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_LINK_REPORT

DESCRIPTION
  This function generates a report to the upper protocol layer

DEPENDENCIES
  None

PARAMETERS
  sect          - Sector to report on
  report_func   - Link report function

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_link_report
(
  hdrsrch_sect_struct_type        *sect,
    /* Sector to report on */

  hdrsrch_link_report_func_type   report_func
    /* Link report function */
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_REQUEST_ACCESS_HANDOFF

DESCRIPTION
  This function generates an Access HO Request to the upper protocol layer.
  If a request has been recently made, the report will be supressed, to avoid
  "nagging" the upper layer.

DEPENDENCIES
  None

PARAMETERS
  sect          - Sector to request a handoff to

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_request_access_handoff
(
  hdrsrch_sect_struct_type  *sect
    /* Sector we desire to perform an access handoff to. */
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_PREPARE_MONITOR_STATE

DESCRIPTION
  This function changes the searcher to IDLE MONITOR state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Alters priority with SAM

===========================================================================*/

void hdrsrchidle_prepare_monitor_state( void );


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_INIT_MONITOR_STATE

DESCRIPTION
  This function changes the searcher to IDLE MONITOR state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Alters priority with SAM

===========================================================================*/

void hdrsrchidle_init_monitor_state( void );


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_IDLE_MODE_CMD

DESCRIPTION
  This function commands the searcher to transition to IDLE

DEPENDENCIES
  TBD

PARAMETERS
  params - IDLE_MODE parameters

RETURN VALUE
  E_SUCCESS     - if idle mode is immediately entered.
  E_IN_PROGRESS - if idle mode will be entered later.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_idle_mode_cmd
(
  const hdrsrch_idle_params_type *params
    /* Parameters for Idle Mode command */
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_START_SLEEP

DESCRIPTION
  This function begins sleep

DEPENDENCIES
  Must only be called when searcher is idle.
  hdrsrchidle_compute_next_cycle( ) must be called prior to this function.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_start_sleep( void );


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_NSET_UPDATE_CMD

DESCRIPTION
  This function adjusts the neighbour set

DEPENDENCIES
  None

PARAMETERS
  params   - New neighbour list message

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_nset_update_cmd
(
  const hdrsrch_nset_update_params_type *params
);


/* <EJECT> */
/*=========================================================================

FUNCTION HDRSRCHIDLE_TUNE

DESCRIPTION
  Tuning after Channel Change, or perhaps for O-Freq neighbour searches

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchidle_tune
( 
  hdrsrchrf_tune_status_enum_type status
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_CHANNEL_CHANGE

DESCRIPTION
  This function changes the current channel.

DEPENDENCIES
  Must only be called when searcher is idle.
  Ie, should be called from hdrsrch_on_searcher_idle( )

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_channel_change( void );


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_CHANNEL_CHANGE_CMD

DESCRIPTION
  This function switches the active channel

DEPENDENCIES
  None

PARAMETERS
  params        - New channel parameters

RETURN VALUE
  E_IN_PROGRESS - Command is not completed immediately.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_channel_change_cmd
(
  const hdrsrch_channel_change_params_type *params
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_MODE_CMD

DESCRIPTION
  This function commands the searcher to transition to ACCESS

DEPENDENCIES
  None

PARAMETERS
  params - ACCESS_MODE parameters

RETURN VALUE
  E_SUCCESS     - if access mode is immediately entered.
  E_IN_PROGRESS - if access mode will be entered when IDLE is entered.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_access_mode_cmd
(
  const hdrsrch_access_params_type *params
    /* Parameters for Idle Mode command */
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_HANDOFF_CMD

DESCRIPTION
  This function allows search to perform 1 handoff

DEPENDENCIES
  None

PARAMETERS
  params        - ACCESS_HANDOFF parameters

RETURN VALUE
  E_IN_PROGRESS - Access handoff is not immediately completed

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_access_handoff_cmd
(
  const hdrsrch_access_handoff_params_type *params
    /* Parameters for Idle Mode command */
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_TC_ASSIGN_CMD

DESCRIPTION
  This function transitions IDLE (Monitor, Access, Broadcast or Sleep) state 
  to the connected state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  FALSE - Transition to traffic channel will take several steps.  Command
          is not finished until all the steps are complete.

SIDE EFFECTS
  Transition to Connected (Traffic Channel) state.
  Command processor is suspended until the transition is complete.

===========================================================================*/

errno_enum_type hdrsrchidle_tc_assign_cmd( void );


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_PILOT_SUPERVISION

DESCRIPTION
  This function handles pilot supervision in the idle state.
  If all pilots drop below PilotDrop, a supervision timer starts running.
  If any pilot rises above PilotDrop, the timer is halted.
  [6.6.5.5.2]

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_pilot_supervision( void );


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_GET_HO_CANDIDATE

DESCRIPTION
  This function finds the best CSET pilot, subject to it being stronger than
  the current ASET pilot

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Strong CSET pilot or NULL

SIDE EFFECTS
  None.

===========================================================================*/

hdrsrch_sect_struct_type* hdrsrchidle_get_ho_candidate
(
  uint8                     ho_thresh
    /* threshold in 0.5dB units required to avoid ping-pong handoffs */
);


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_HANDOFF_LINK_REPORT

DESCRIPTION
  TBD

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_handoff_link_report
(
  hdrsrch_sect_struct_type  *sect
    /* sector to which handing off to */
);


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_SCHED

DESCRIPTION
  This function programs searches, and schedules the next search request.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_sched( void );


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_TIMEOUT

DESCRIPTION
  This function handles the Network Lost timer.
  If all pilots drop below PilotDrop, the TimeOut timer is started.
  If any pilot rises above PilotDrop before the timer expires, then
  the TimeOut timer is cleared.
  If the TimeOut timer expires, this function is called, and we must
  declare a Network Lost indication.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Transitions back to start state.

===========================================================================*/

void hdrsrchidle_timeout( void );


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_DISABLE_HO_DURING_SCC

DESCRIPTION
  This function decides whether HO should be disabled or not, depending on
  the current HSTR count.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_disable_ho_during_scc( void );


typedef 
void (*hdrsrchidle_dump_func_type)
(
  int              new_rpts
    /* Number of new pilot reports */
);


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_DUMP_PROC

DESCRIPTION
  This function handles the search dump signal. Different Idle states pass
  their own set management functions.  

DEPENDENCIES
  None

PARAMETERS
  Set management function pointer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_dump_proc
( 
  hdrsrchidle_dump_func_type 
);


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_HANDLE_ACCESS_HO

DESCRIPTION
  This function handles access handoff.

DEPENDENCIES
  None

PARAMETERS
  Pointer to a sector that we might handoff to.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_handle_access_ho
(
  hdrsrch_sect_struct_type  *
    /* handoff sector */
);


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_ASET_MGMT

DESCRIPTION
  This function handles aset management in monitor state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchidle_monitor_aset_mgmt
(
  int new_rpts
    /* Number of new pilot report events (R/N to C, C>A, Weak A) */
);


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_HANDLE_IDLE_HO

DESCRIPTION
  This function handles idle handoff in monitor state.

DEPENDENCIES
  None.

PARAMETERS
  Pointer to a sector that we might handoff to.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_monitor_handle_idle_ho
(
  hdrsrch_sect_struct_type      *sect
    /* hand-off candidate sector. NULL -> no handoff */
);


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_IS_TA_STARTED

DESCRIPTION
  This function returns tune away status in Idle. This will be helpful in 
  stopping Fast connect TCA processing while tuning away in Idle.
 
DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if tune away processing has started.
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchidle_is_ta_started( void );

/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_ASET_MGMT

DESCRIPTION
  This function handles aset management in access state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchidle_access_aset_mgmt
(
  int new_rpts
    /* Number of new pilot report events (R/N to C, C>A, Weak A */
);


#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACMAC_READY_CMD

DESCRIPTION
  This function processes acmac ready command. It calls retain lock 
  which triggers rf unlock event. RF unlock event causes tune away 
  time command sent to ACMAC.
  
DEPENDENCIES
  None

PARAMETERS
  params - Callback function to notify tune away time

RETURN VALUE
  E_SUCCESS     - Command succeeded.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_acmac_ready_cmd
(
  const hdrsrch_acmac_ready_params_type *params
    /* Parameters for acmac ready command */
);


/*===========================================================================

FUNCTION HDRSRCHIDLE_ACMAC_TA_TIME_OK_CMD

DESCRIPTION
  This function processes tune away time ok command. 

DEPENDENCIES
  None

PARAMETERS
  params - Callback function to notify tune away time

RETURN VALUE
  E_SUCCESS     - Command succeeded.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_acmac_ta_time_ok_cmd
(
  const hdrsrch_acmac_ta_time_ok_params_type *params
    /* Parameters for ta time ok command */
);


/*===========================================================================

FUNCTION HDRSRCHIDLE_ACMAC_DISABLE_TA_CMD

DESCRIPTION
  This function processes disable ta command. 

DEPENDENCIES
  None

PARAMETERS
  params - the duration to disable tune away.

RETURN VALUE
  E_SUCCESS     - Command succeeded.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_acmac_disable_ta_cmd
(
  const hdrsrch_acmac_disable_ta_params_type *params
    /* Parameters for disable ta command */
);
#endif /* FEATURE_HDR_HYBRID_ACCESS */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
/*===========================================================================

FUNCTION HDRSRCHIDLE_PROCESS_SCC_DECODE_IND

DESCRIPTION
  This function handles scc deocde indication from FW.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_process_scc_decode_ind( void );

/*===========================================================================

FUNCTION HDRFMAC_PROCESS_MDSP_LOGGING_EVENT   

DESCRIPTION 
  This function is the call back from mdsp logging event for PSINR 
  calculation.
 
 DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

int16 hdrsrchidle_get_prev_wakeup_psinr ( void );

/*===========================================================================

FUNCTION HDRSRCHIDLE_PROCESS_PSINR_MDSP_LOGGING_EVENT   

DESCRIPTION 
  This function is the call back from mdsp logging event for PSINR 
  calculation.
 
 DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchidle_process_psinr_mdsp_logging_event
( 
  hdrlog_mdsp_slot_logging_data_type *sl_params 
); /* hdrsrchlog_process_psinr_mdsp_logging_event */

#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_ENABLE_TUNE_AWAY

DESCRIPTION
  Retain rf lock and initialize tuneaway data structure

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_monitor_enable_tune_away( void );
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */


/*===========================================================================

FUNCTION HDRSRCHIDLE_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD

DESCRIPTION
  This function switches the active channel

DEPENDENCIES
  None

PARAMETERS
  params        - New channel and nset parameters

RETURN VALUE
  E_IN_PROGRESS - Command is not completed immediately.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_channel_change_and_nset_update_cmd
(
  const hdrsrch_channel_change_and_nset_update_params_type *params
    /* params */
);

/*===========================================================================

FUNCTION HDRSRCHIDLE_REQUEST_TX_CAPABLE_CHAIN

DESCRIPTION
  This function gives up current RX_ONLY chain by tranitioning to 3a,
  and requests for RXTX_BEST.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchidle_request_tx_capable_chain( void );

/*===========================================================================

FUNCTION HDRSRCHIDLE_IS_TX_PENDING

DESCRIPTION
  This function returns whether we are in tx pending state i.e
  either access of TCA is pending while we wait for RXTX_BEST
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE: yes access or TCA pending
  FALSE: no access or TCA pending

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchidle_is_tx_pending( void );

/*==========================================================================

FUNCTION        HDRSRCHIDLE_GET_ACCESS_TA_TIME

DESCRIPTION
  This function return the primary chain tune away time (in sclk) in access state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Primary chain tune away time (end_time_sclks)

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchidle_get_access_ta_time( void );

#endif /* HDRSRCHIDLEI_H */
