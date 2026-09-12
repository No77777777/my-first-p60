#ifndef TDS_THERMBACKOFF_H
#define TDS_THERMBACKOFF_H

/*===========================================================================
                              TDS_THERMBACKOFF . H

GENERAL DESCRIPTION
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright (c) 2000 - 2011 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsthermbackoff.h#1 $ 
  $DateTime: 2019/08/19 10:51:23 $ 
  $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
02/23/12    hz      File created.
                    
===========================================================================*/

#include "tdscdma_variation.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "rex.h"

#ifdef FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF

/*-------------------------------------------------------------------------*/
/*                                                                                                                                */
/*                         Data Definitions                                                                                */
/*                                                                                                                                */
/*-------------------------------------------------------------------------*/

/* Possible states for PA Sensor device monitored
 * through NPA */
typedef enum
{
  TDSTHERMBACKOFF_NPA_STATE_OKAY,
  TDSTHERMBACKOFF_NPA_STATE_WARNING,
  TDSTHERMBACKOFF_NPA_STATE_WARNING_EX,
  TDSTHERMBACKOFF_NPA_STATE_CRITICAL
}tdsthermbackoff_npa_state_enum;


/* Possible states in MTPL Thermal Backoff Algorithm */
typedef enum
{
  TDSTHERMBACKOFF_START, /* PA Sensor Sate is WARNING_EX and UE in DCH */
  TDSTHERMBACKOFF_PAUSE, /* PA Sensor state is WARNING_EX and UE not in DCH */
  TDSTHERMBACKOFF_STOP,  /* PA Sensor state is not WARNING_EX */
  TDSTHERMBACKOFF_NUM_STATES 
}tdsthermbackoff_mtpl_algo_state;


/* thermal backoff global variables */
typedef struct {
  uint8 mtpl_init_val_db;     

  /* Toggle switch for duty cycle. Initialized to UP */ 
  uint8 dutycycle_switch;      

  /* dB value by which MTPL needs to be reduced */
  uint8 mtpl_reduce_by_db;    

  /* Max dB value that MTPL can reduce  */
  uint8 mtpl_max_reduce_by_db;
  
  /* Counter for the step timer. Used in calculating new MTPL backoff */
  uint8 step_timer_cnt;       
 
  /* Flag to indicate if PA SENSOR STATE is in WARNING_EX */
  boolean pa_sensor_state_warning_ex;         

  /* MTPL BACKOFF Duty Cyle and Step Timer Periods */
  uint32  dutycycle_tup_period_ms;
  uint32  dutycycle_tdown_period_ms;
  uint32  step_timer_period_ms;
  rex_timer_type dutycycle_timer;
  rex_timer_type step_timer;

  /* Initialize backoff algorithm state to STOP state */
  tdsthermbackoff_mtpl_algo_state curr_state;

  /* Boolean flag to determine if the currently active call is a voice only call 
    * Voice Only call = TRUE 
    * Voice + Data = FALSE 
    * Data only call = FALSE 
    * IDLE = FALSE */
  boolean is_voice_only_call;
  boolean is_e911_call_active;
} tdsthermbackoff_global_type;

extern tdsthermbackoff_global_type tdsthermbackoff_global;


/*=========================================================================
FUNCTION TDSTHERMBACKOFF_INIT

DESCRIPTION
  This routine initialize global variables by setting default values

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void tdsthermbackoff_init(void);

/*=========================================================================
FUNCTION TDSTHERMBACKOFF_IS_VOICE_ONLY_CALL_SET

DESCRIPTION
  This routine set the variable is_voice_only_call

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  variable is_voice_only_call is updated.
=========================================================================*/
void tdsthermbackoff_is_voice_only_call_set(boolean is_voice_only_call);

/*=========================================================================
FUNCTION TDSTHERMBACKOFF_IS_VOICE_ONLY_CALL_GET

DESCRIPTION
  This routine retrieves the variable is_voice_only_call

DEPENDENCIES

RETURN VALUE
  value of is_voice_only_call

SIDE EFFECTS
  None
=========================================================================*/
boolean tdsthermbackoff_is_voice_only_call_get(void);


/*=========================================================================
FUNCTION TDSTHERMBACKOFF_IS_E911_CALL_ACTIVE_SET

DESCRIPTION
  This routine set the variable is_e911_call_active

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  update is_e911_call_active
=========================================================================*/
void tdsthermbackoff_is_e911_call_active_set(boolean is_e911_call_active);


/*=========================================================================
FUNCTION TDSTHERMBACKOFF_IS_E911_CALL_ACTIVE_GET

DESCRIPTION
  This routine retrieves the variable is_e911_call_active

DEPENDENCIES

RETURN VALUE
  value of is_e911_call_active

SIDE EFFECTS
  None
=========================================================================*/
boolean tdsthermbackoff_is_e911_call_active_get(void);


/*=========================================================================
FUNCTION TDSTHERMBACKOFF_EVT_REG

DESCRIPTION This routine is called during WL1 Init to register with NPA for events
            on PA Sensor Device

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void tdsthermbackoff_npa_evt_reg(void);



/*===========================================================================
FUNCTION        TDSTHERMBACKOFF_FOR_VOICE_E911_CALL

DESCRIPTION     This function checks the current state of MTPL_BACKOFF state machine and 
                also checks whether any voice call or Emergency call is active and alters
                the state of MTPL_BACKOFF if required.
                If MTPL_BACKOFF is STARTED and if its a voice only call or E911 call , then PAUSE
                if MTPL_BACKOFF already Paused and if it is not a voice only call then START
 
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsthermbackoff_for_voice_e911_call(void);

/*===========================================================================
FUNCTION        tdsthermbackoff_leave_dch

DESCRIPTION     When leaving DPCH_TX,  PAUSE thermal MTPL backoff if it is already started
 
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsthermbackoff_leave_dch(void);

/*=========================================================================
FUNCTION TDSTHERMBACKOFF_DUTYCYCLE_SIG_HANDLER

DESCRIPTION
  This routine handles the duty cycle timer. Based on the Up/Down switch
  appropriate MTPL value is set and timers are restarted.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void tdsthermbackoff_dutycycle_sig_handler(void);


/*=========================================================================
FUNCTION TDSTHERMBACKOFF_STEP_SIG_HANDLER

DESCRIPTION  
This routine handles the duty cycle timer. Based on the timer count,
new backoff value is calculated by which to reduce MTPL
  
DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.
=========================================================================*/
void tdsthermbackoff_step_sig_handler(void);


#endif /* FEATURE_TDSCDMA_MTPL_THERMAL_BACKOFF */


#endif  /* TDS_THERMBACKOFF_H */
