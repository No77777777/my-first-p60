/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              H D R   S R C H    M C P M    F U N C T I O N S

GENERAL DESCRIPTION
  This module acts as a thin layer between HDRSRCH and Modem Clock and Power
  Manager ( MCPM ).  It consolidates SRCH state information, peak UL/DL data
  rates and diversity information and conveys it to MCPM.

EXTERNALIZED FUNCTIONS
  hdrsrchmcpm_init
    Initializes SRCH-MCPM struct.
 
  hdrsrchmcpm_set_state
    Sets HDR state to be conveyed to MCPM.
 
  hdrsrchmcpm_set_diversity_status
    Sets diversity on/off status to be conveyed to MCPM.
 
  hdrsrchmcpm_set_equalizer_status
    Sets equalizer on/off status to be conveyed to MCPM.
 
  hdrsrchmcpm_set_subtype
    Sets peak UL/DL data rates to be conveyed to MCPM.
 
  hdrsrchmcpm_set_mcdo_status
    Sets BW info to be conveyed to MCPM.
 
  hdrsrchmcpm_set_enh_rates_status
    Sets DL rates to be conveyed to MCPM based on enhanced rates
    enable/disable status.
 
  hdrsrchmcpm_do2lte_measurement
    Sets DO to LTE measurement status to be conveyed to MCPM.
 
  Copyright (c) 2012 - 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*



============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchmcpm.c#6 $ $DateTime: 2020/04/03 06:32:33 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
04/02/20   vaa     Include CMGR scenario cfg API call under critical section
02/06/20   vaa     FR54762: HDR Concurrency Mgr changes
01/17/19   dmd     Use MCPM_MCVS_SILVER_CLK_UPDATE for VPE update
12/04/19   vaa     Changes to support clk plan V2.0 FR changes
06/05/14   rmv     Delay setting MCPM state to HDRSRCHMCPM_STOP_TX_REQ, until 
                   searcher traffic exit in MCDO case 
02/25/14   ljl     Requested MCPM_DO_PARMS_UPDATE_REQ instead of 
                   MCPM_DO_GO_TO_SLEEP_REQ when HDR was in sleep already.
                   Requested MCPM_DO_PARMS_UPDATE_REQ instead of MCPM_DO_NOP_REQ.
10/29/13   sat     Reduced F3 messages (or/and) ASSERTs
06/05/12   kss     Removed hdrsrchmcpm_reset_tx().
04/26/12   rmg     Added hdrsrchmcpm_reset_tx().
01/26/11   ljl     Updated MCPM with sleep info
11/14/11   rmg     Removed workaround of bypassing MCPM sleep state.
10/06/11   rmg     Prevented MCPM sleep state transition as a temporary 
                   workaround to corrupted cc pkts issue after DO sleep.
09/13/11   rmg     Fixed stop LTE measurement command.
08/05/11   rmg     Updated both MCDO data rate and BW to MCPM in the same call.
07/28/11   rmg     Added support for sleep to powerdown state transition.
07/06/11   rmg     Set MCPM state change req field to NOP for param updates.
05/19/11   rmg     Updated HDR MCPM interface.
04/27/11   rmg     Created.

==========================================================================*/

/*==========================================================================

                     IMPLEMENTATION DEPENDANCIES

==========================================================================*/

#include "comdef.h"       /* Needed for byte, uint16, uint32, INTLOCK(), etc. */
#include "amssassert.h"   /* Needed for ASSERT_FATAL */
#include "hdrsrchmcpm.h"  /* Needed for enums used by this module */
#include "hdrdebug.h"     /* Needed for HDR debug message macros */

/* Temporary: for setting DEMBACK_ENABLE */
#include "msm.h"
#include "cmgr_api.h"
#include "hdrsrchrf.h"
/* Flag to enable MCPM interaction */
volatile boolean hdrsrchmcpm_enable_mcpm_interaction = TRUE;

/* When set, data -> idle rx and idlx rx -> sleep state transitions are
   disallowed */
volatile boolean hdrsrchmcpm_bypass_down_state_transition = FALSE;

/* When set, idlx rx -> sleep state transition is disallowed */
volatile boolean hdrsrchmcpm_bypass_sleep_state_transition = FALSE;

/*==========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

==========================================================================*/

/* Struct type for parameters sent to MCPM */
typedef struct 
{
  mcpm_request_type       request;
  mcpm_request_parms_type params;

}hdrsrchmcpm_sent_to_mcpm_struct_type;


/* Struct type for history of states and state change requests sent 
   to MCPM */
typedef struct
{
  hdrsrchmcpm_state_type   curr_state;
  hdrsrchmcpm_request_type curr_req;

}hdrsrchmcpm_history_type;

/* Number of past MCPM requests to store */
#define HDRSRCHMCMP_HISTORY_SIZE 10

/* SRCH-MCPM struct type */
typedef struct 
{
  /* Last communication with MCPM */
  hdrsrchmcpm_sent_to_mcpm_struct_type sent_to_mcpm;

  /* Most recent SRCH-MCPM state */
  hdrsrchmcpm_state_type curr_state;

  /* Most recent request received by SRCH-MCPM */
  hdrsrchmcpm_request_type curr_req;

  /* History of SRCH-MCPM states and received requests.  This is a circular
     buffer.  The last stored request is at entry pointed by history_ptr */
  hdrsrchmcpm_history_type history[HDRSRCHMCMP_HISTORY_SIZE];

  /* This points to last stored state / received request in history buffer */
  uint8 history_ptr;

  /* TRUE if enhanced rates are enabled */
  boolean enhanced_rates_enabled;

  /* TRUE if MCDO status enabled */
  boolean mcdo_status;

  /* Last Transient and steady req sent to Concurrency Mgr */
  cmgr_params sent_to_cmgr_trans;
  cmgr_params sent_to_cmgr_steady;

  rex_crit_sect_type  crit_sect; 

}hdrsrchmcpm_struct_type;


/* SRCH-MCPM struct */
LOCAL hdrsrchmcpm_struct_type hdrsrchmcpm;


/* Look up table for SRCH-MCPM next state given the current state and the 
   state transition request */
LOCAL hdrsrchmcpm_state_type hdrsrchmcpm_next_state_lu_table
  [HDRSRCHMCPM_MAX_ST+1][HDRSRCHMCPM_MAX_REQ+1] = 
{ 
  /* Power down state */
  {
    HDRSRCHMCPM_IDLE_RX_ST,      /* Start   request     */
    HDRSRCHMCPM_POWERDOWN_ST,    /* Stop    request     */
    HDRSRCHMCPM_INVALID_ST,      /* Wake up request     */
    HDRSRCHMCPM_INVALID_ST,      /* Go to sleep request */
    HDRSRCHMCPM_INVALID_ST,      /* Start tx request    */
    HDRSRCHMCPM_INVALID_ST,      /* Stop tx request     */
    HDRSRCHMCPM_POWERDOWN_ST     /* Params Update request */
  },

  /* Idle Rx state */
  {
    HDRSRCHMCPM_INVALID_ST,      /* Start   request     */
    HDRSRCHMCPM_POWERDOWN_ST,    /* Stop    request     */
    HDRSRCHMCPM_INVALID_ST,      /* Wake up request     */
    HDRSRCHMCPM_SLEEP_ST,        /* Go to sleep request */
    HDRSRCHMCPM_DATA_ST,         /* Start tx request    */
    HDRSRCHMCPM_IDLE_RX_ST,      /* Stop tx request     */
    HDRSRCHMCPM_IDLE_RX_ST       /* Params Update request */
  },

  /* Sleep state */
  {
    HDRSRCHMCPM_INVALID_ST,      /* Start   request     */
    HDRSRCHMCPM_POWERDOWN_ST,    /* Stop    request     */
    HDRSRCHMCPM_IDLE_RX_ST,      /* Wake up request     */
    HDRSRCHMCPM_SLEEP_ST,        /* Go to sleep request */
    HDRSRCHMCPM_INVALID_ST,      /* Start tx request    */
    HDRSRCHMCPM_INVALID_ST,      /* Stop tx request     */
    HDRSRCHMCPM_SLEEP_ST         /* Params Update request */
  },

  /* Data state */
  {
    HDRSRCHMCPM_INVALID_ST,      /* Start   request     */
    HDRSRCHMCPM_INVALID_ST,      /* Stop    request     */
    HDRSRCHMCPM_INVALID_ST,      /* Wake up request     */
    HDRSRCHMCPM_INVALID_ST,      /* Go to sleep request */
    HDRSRCHMCPM_DATA_ST,         /* Start tx request    */
    HDRSRCHMCPM_IDLE_RX_ST,      /* Stop tx request     */
    HDRSRCHMCPM_DATA_ST          /* Params Update request */
  }
};


/* Look up table mapping request received by SRCH-MCPM to MCPM request */
LOCAL mcpm_request_type hdrsrchmcpm_mcpm_request_lu_table
  [HDRSRCHMCPM_MAX_REQ+1] = 
{
  MCPM_DO_START_REQ,             /* HDRSRCHMCPM_START_REQ        */
  MCPM_DO_STOP_REQ,              /* HDRSRCHMCPM_STOP_REQ         */
  MCPM_DO_WAKE_UP_REQ,           /* HDRSRCHMCPM_WAKE_UP_REQ      */
  MCPM_DO_GO_TO_SLEEP_REQ,       /* HDRSRCHMCPM_GO_TO_SLEEP_REQ  */
  MCPM_DO_START_DATA_TX_REQ,     /* HDRSRCHMCPM_START_TX_REQ     */
  MCPM_DO_STOP_DATA_TX_REQ,      /* HDRSRCHMCPM_STOP_TX_REQ      */
  MCPM_DO_PARMS_UPDATE_REQ       /* HDRSRCHMCPM_PARMS_UPDATE_REQ */
};

/* Look up table mapping request received by SRCH-MCPM to MCPM request */
LOCAL uint32 hdrsrchmcpm_cmgr_request_lu_table
  [HDRSRCHMCPM_MAX_REQ+1] = 
{
  CMGR_DO_START_REQ,             /* HDRSRCHMCPM_START_REQ        */
  CMGR_DO_STOP_REQ,              /* HDRSRCHMCPM_STOP_REQ         */
  CMGR_DO_WAKE_UP_REQ,           /* HDRSRCHMCPM_WAKE_UP_REQ      */
  CMGR_DO_GO_TO_SLEEP_REQ,       /* HDRSRCHMCPM_GO_TO_SLEEP_REQ  */
  CMGR_DO_START_DATA_TX_REQ,     /* HDRSRCHMCPM_START_TX_REQ     */
  CMGR_DO_STOP_DATA_TX_REQ,      /* HDRSRCHMCPM_STOP_TX_REQ      */
  CMGR_DO_PARMS_UPDATE_REQ       /* HDRSRCHMCPM_PARMS_UPDATE_REQ */
};


/*============================================================================

FUNCTION HDRSRCHMCPM_INIT

DESCRIPTION
  Initializes SRCH-MCPM structure.  To be done at power up.
 
PARAMETERS 
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_init( void )
{

  uint8 hist_idx;

/*--------------------------------------------------------------------------*/

  memset( &hdrsrchmcpm, 0, sizeof(hdrsrchmcpm) );

  /* Initialize stored state/request */
  hdrsrchmcpm.curr_state = HDRSRCHMCPM_POWERDOWN_ST;
  hdrsrchmcpm.curr_req   = HDRSRCHMCPM_INVALID_REQ;
  
  /* Clear history */
  for ( hist_idx = 0; hist_idx < HDRSRCHMCMP_HISTORY_SIZE; hist_idx++ )
  {
    hdrsrchmcpm.history[hist_idx].curr_state = HDRSRCHMCPM_INVALID_ST;
    hdrsrchmcpm.history[hist_idx].curr_req   = HDRSRCHMCPM_INVALID_REQ;
  }

  /* Reset history index/pointer */
  hdrsrchmcpm.history_ptr = 0;

  hdrsrchmcpm.history[hdrsrchmcpm.history_ptr].curr_state = 
    HDRSRCHMCPM_POWERDOWN_ST;

  /* Reset update info field */
  hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_NO_UPDATE;

  /* Set data rates and BW to Rel0 values */
  hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_2P4_MBPS;
  hdrsrchmcpm.sent_to_mcpm.params.ul_datarate = MCPM_DO_UL_153KBPS;
  hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth = MCPM_DO;

  /* Reset enhanced rates enabled flag */
  hdrsrchmcpm.enhanced_rates_enabled = FALSE;

  /* Reset MCDO status enabled flag */
  hdrsrchmcpm.mcdo_status = FALSE;
  
  //register cb function with conc_mgr
  cmgr_register_cb( CMGR_DO_TECH, hdrsrchmcpm_cmgr_cb);

  rex_init_crit_sect( &hdrsrchmcpm.crit_sect );

  return;

} /*  hdrsrchmcpm_init */



/*============================================================================

FUNCTION HDRSRCHMCPM_SET_STATE

DESCRIPTION
  Conveys DO state change request to MCPM and stores it internally.
 
PARAMETERS 
  req  - DO state change request to be stored and conveyed to MCPM module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/
void hdrsrchmcpm_set_state
(
  hdrsrchmcpm_request_type   req
    /* SRCH-MCPM state change request */
)
{
  /* HDRSRCH-MCPM next state once the received request is processed */
  hdrsrchmcpm_state_type next_state;

/*--------------------------------------------------------------------------*/

  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }

  if( ( hdrsrchmcpm_bypass_down_state_transition ) &&
    ( ( ( hdrsrchmcpm.curr_state == HDRSRCHMCPM_IDLE_RX_ST ) && 
    ( req == HDRSRCHMCPM_GO_TO_SLEEP_REQ ) ) ||
    ( hdrsrchmcpm.curr_state == HDRSRCHMCPM_DATA_ST ) ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
      "MCPM down state transitions bypassed.  Current state: %d",
      hdrsrchmcpm.curr_state );

    return;
  }
  else if ( ( hdrsrchmcpm_bypass_sleep_state_transition ) &&
    ( ( hdrsrchmcpm.curr_state == HDRSRCHMCPM_IDLE_RX_ST ) && 
    ( req == HDRSRCHMCPM_GO_TO_SLEEP_REQ ) ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
      "MCPM sleep state transitions bypassed.  Current state: %d",
      hdrsrchmcpm.curr_state );

    return;
  }
  
  ASSERT( req <= HDRSRCHMCPM_MAX_REQ );
    /* Error check. */

  /* Special case: wake up request in powerdown state */
  if ( ( req == HDRSRCHMCPM_WAKE_UP_REQ ) &&
       (hdrsrchmcpm.curr_state == HDRSRCHMCPM_POWERDOWN_ST) )
  {
    /* Traslate wake up request in powerdown state to start request */
    req = HDRSRCHMCPM_START_REQ;
  }

  /* Store the received request */
  hdrsrchmcpm.curr_req = req;

  /* Find the next state */

  ASSERT(( hdrsrchmcpm.curr_state <= HDRSRCHMCPM_MAX_ST ) &&
         ( hdrsrchmcpm.curr_req <= HDRSRCHMCPM_MAX_REQ ));

  next_state = hdrsrchmcpm_next_state_lu_table[hdrsrchmcpm.curr_state]
    [hdrsrchmcpm.curr_req];


  /* Pass the next state to MCPM if the received request is expected to 
     result in valid next state that is different from the current state */
  if ( next_state == HDRSRCHMCPM_INVALID_ST )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,
      "Invalid request %d received in state %d",
      hdrsrchmcpm.curr_req,
      hdrsrchmcpm.curr_state );
  }
  else if ( next_state == hdrsrchmcpm.curr_state )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_ERROR,
                    "MCPM request %d received in state %d, next state %d",
      hdrsrchmcpm.curr_req,
                    hdrsrchmcpm.curr_state,
                    next_state );

  }
  else
    {
    /* Prepare and send request to MCPM */
  
      hdrsrchmcpm.sent_to_mcpm.request = 
        hdrsrchmcpm_mcpm_request_lu_table[hdrsrchmcpm.curr_req];
      hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_NO_UPDATE;
      hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[hdrsrchmcpm.curr_req]);
      MCPM_Config_Modem( hdrsrchmcpm.sent_to_mcpm.request, 
        &hdrsrchmcpm.sent_to_mcpm.params );

    /* Store the state change that went through */
    hdrsrchmcpm.curr_state = next_state;


      /* Update state change request history */

      hdrsrchmcpm.history[hdrsrchmcpm.history_ptr].curr_req = 
        hdrsrchmcpm.curr_req;

      hdrsrchmcpm.history_ptr = 
        ( hdrsrchmcpm.history_ptr + 1 )%HDRSRCHMCMP_HISTORY_SIZE;

      hdrsrchmcpm.history[hdrsrchmcpm.history_ptr].curr_state = 
        hdrsrchmcpm.curr_state;

    }

  return;

} /*  hdrsrchmcpm_set_state */


/*============================================================================

FUNCTION HDRSRCHMCPM_EXTEND_SLEEP

DESCRIPTION
  Update MCPM that sleep yim has changed.
 
PARAMETERS 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_extend_sleep( void )
{

/*--------------------------------------------------------------------------*/

  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }

  if ( hdrsrchmcpm.curr_state == HDRSRCHMCPM_SLEEP_ST )
  {
    /* This is to notify MCPM that HDR reprogram sleepctl */

    hdrsrchmcpm.sent_to_mcpm.request = 
      hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ];
    hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_WKTIME_RSRC_UPDATE;

    hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
    MCPM_Config_Modem( hdrsrchmcpm.sent_to_mcpm.request, 
      &hdrsrchmcpm.sent_to_mcpm.params );

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
      "MCPM extend sleep request %d update info 0x%x sent to MCPM in state %d",
      hdrsrchmcpm.sent_to_mcpm.request,
      hdrsrchmcpm.sent_to_mcpm.params.update_info,
      hdrsrchmcpm.curr_state );

    /* Update state change request history */

    hdrsrchmcpm.history[hdrsrchmcpm.history_ptr].curr_req = 
      hdrsrchmcpm.curr_req;

    hdrsrchmcpm.history_ptr = 
      ( hdrsrchmcpm.history_ptr + 1 )%HDRSRCHMCMP_HISTORY_SIZE;

    hdrsrchmcpm.history[hdrsrchmcpm.history_ptr].curr_state = 
      hdrsrchmcpm.curr_state;
  }
  else
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,
      "MCPM extend sleep incorrect request/state request %d sent to MCPM in state %d",
      hdrsrchmcpm.sent_to_mcpm.request,
      hdrsrchmcpm.curr_state );
  }

  return;

} /*  hdrsrchmcpm_extend_sleep */


/*============================================================================

FUNCTION HDRSRCHMCPM_SET_DIVERSITY_STATUS

DESCRIPTION
  Conveys DO Rx diversity on/off status to MCPM and stores it for future MCPM
  communication.
 
PARAMETERS 
  div_status  -  TRUE  => Diversity ON
                 FALSE => Diversity OFF

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_set_diversity_status
(
  boolean        div_status
    /* Diversity status */
)
{

  uint32 receiver_config_new; /* New receiver configuration */

/*--------------------------------------------------------------------------*/

  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }


  /* Compute new receiver config with diversity status update */
  if ( div_status )
  {
    /* Diversity enabled */
    receiver_config_new = hdrsrchmcpm.sent_to_mcpm.params.receiver_config | 
      MCPM_RECEIVER_CONFIG_RXD;
  }
  else
  {
    /* Diversity disabled */
    receiver_config_new = hdrsrchmcpm.sent_to_mcpm.params.receiver_config & 
      ~MCPM_RECEIVER_CONFIG_RXD;
  }

  /* If there is a change in diversity status, store the update and convey
     it to MCPM. */
  if ( hdrsrchmcpm.sent_to_mcpm.params.receiver_config != receiver_config_new )
  {
    hdrsrchmcpm.sent_to_mcpm.params.receiver_config = receiver_config_new;
      /* Save new config. */
  
    hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_RECEIVER_CONFIG_UPDATE;

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
      "Config MCPM for div update info 0x%x rx config 0x%x",
      hdrsrchmcpm.sent_to_mcpm.params.update_info,
      hdrsrchmcpm.sent_to_mcpm.params.receiver_config );

    hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
    MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
      &hdrsrchmcpm.sent_to_mcpm.params );
      /* Convey the change in diversity status to MCPM. */
  }

  return;

} /*  hdrsrchmcpm_set_diversity_status */



/*============================================================================

FUNCTION HDRSRCHMCPM_SET_EQUALIZER_STATUS

DESCRIPTION
  Conveys equalizer on/off status to MCPM and stores it for future MCPM
  communication.
 
PARAMETERS 
  eq_status  -  TRUE  => Equalizer ON
                FALSE => Equalizer OFF

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_set_equalizer_status
(
  boolean        eq_status
    /* Equalizer status */
)
{

  uint32 receiver_config_new; /* New receiver configuration */

/*--------------------------------------------------------------------------*/

  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }

  /* Compute new receiver config with diversity status update */
  if ( eq_status )
  {
    /* Equalizer enabled */
    receiver_config_new = hdrsrchmcpm.sent_to_mcpm.params.receiver_config | 
      MCPM_RECEIVER_CONFIG_EQ;
  }
  else
  {
    /* Equalizer disabled */
    receiver_config_new = hdrsrchmcpm.sent_to_mcpm.params.receiver_config & 
      ~MCPM_RECEIVER_CONFIG_EQ;
  }

  /* If there is a change in equalizer status, store the update and convey
     it to MCPM. */
  if ( hdrsrchmcpm.sent_to_mcpm.params.receiver_config != receiver_config_new )
  {
    hdrsrchmcpm.sent_to_mcpm.params.receiver_config = receiver_config_new;
      /* Save new config. */
  
    hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_RECEIVER_CONFIG_UPDATE;

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
      "Config MCPM for eq update info 0x%x rx config 0x%x",
      hdrsrchmcpm.sent_to_mcpm.params.update_info,
      hdrsrchmcpm.sent_to_mcpm.params.receiver_config );
    hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
    MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
      &hdrsrchmcpm.sent_to_mcpm.params );
      /* Convey the change in equalizer status to MCPM. */
  }

  return;

} /*  hdrsrchmcpm_set_equalizer_status */



/*============================================================================

FUNCTION HDRSRCHMCPM_SET_SUBTYPE

DESCRIPTION
  Stores DO subtype and notifies peak up-link/down-link rates for the subtype
  to MCPM.
 
PARAMETERS 
  subtype  -  DO protocol subtype

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_set_subtype
(
  sys_active_prot_e_type subtype
    /* DO protocol subtype */
)
{

/*--------------------------------------------------------------------------*/

  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }

  switch ( subtype )
  {
    case SYS_ACTIVE_PROT_HDR_RELB:
      /* Rev B */

      /* Update data rate and bandwidth */
      if ( hdrsrchmcpm.enhanced_rates_enabled )
      {
        hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_14P7_MBPS;
      }
      else
      {
        hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_9P3_MBPS;
      }
      hdrsrchmcpm.sent_to_mcpm.params.ul_datarate = MCPM_DO_UL_5P4MBPS;
      hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth = MCPM_MCDO;
      hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_DL_DATA_RATE_UPDATE | 
        MCPM_UL_DATA_RATE_UPDATE | MCPM_RF_BANDWIDTH_UPDATE;

      break;

    case SYS_ACTIVE_PROT_HDR_RELA:
      /* Rev A */

      /* Update data rate and bandwidth */
      hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_3P1_MBPS;
      hdrsrchmcpm.sent_to_mcpm.params.ul_datarate = MCPM_DO_UL_1P8MBPS;
      hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth = MCPM_DO;
      hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_DL_DATA_RATE_UPDATE | 
        MCPM_UL_DATA_RATE_UPDATE | MCPM_RF_BANDWIDTH_UPDATE;

      break;

    default:
      /* Assume Rel0 */

      /* Update data rate and bandwidth */
      hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_2P4_MBPS;
      hdrsrchmcpm.sent_to_mcpm.params.ul_datarate = MCPM_DO_UL_153KBPS;
      hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth = MCPM_DO;
      hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_DL_DATA_RATE_UPDATE | 
        MCPM_UL_DATA_RATE_UPDATE | MCPM_RF_BANDWIDTH_UPDATE;

      break;
  }

  hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
  MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
    &hdrsrchmcpm.sent_to_mcpm.params );
    /* Convey the data rate and bandwidth update to MCPM. */

  return;

} /*  hdrsrchmcpm_set_subtype */



/*============================================================================

FUNCTION HDRSRCHMCPM_SET_MCDO_STATUS

DESCRIPTION
  Conveys if DO is operating in multicarrier mode to MCPM and stores it for
  future MCPM communication.
 
PARAMETERS 
  mcdo_status  -  TRUE  => DO is operating in multicarrier mode
                  FALSE => DO is operating in single carrier mode

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_set_mcdo_status
(
  boolean        mcdo_status
    /* TRUE if more than one carriers are assigned */
)
{

/*--------------------------------------------------------------------------*/
  hdrsrchmcpm.mcdo_status = mcdo_status;

  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }

  /* If there is a change in MCDO status, store the update and convey
     it to MCPM. */
  if ( mcdo_status && 
    ( hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth != MCPM_MCDO ) )
  {
    hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth = MCPM_MCDO;
      /* Save */

    /* Data rate entries in MCPM are coupled with bandwidth
       => Update data rate as well. */
    if ( hdrsrchmcpm.enhanced_rates_enabled )
    {
      hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_14P7_MBPS;
    }
    else
    {
      hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_9P3_MBPS;
    }

    hdrsrchmcpm.sent_to_mcpm.params.ul_datarate = MCPM_DO_UL_5P4MBPS;

    hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_DL_DATA_RATE_UPDATE | 
      MCPM_UL_DATA_RATE_UPDATE | MCPM_RF_BANDWIDTH_UPDATE;

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH,
                    "Config MCPM for mcdo update info 0x%x bandwidth 0x%x dl 0x%x ul 0x%x",
                    hdrsrchmcpm.sent_to_mcpm.params.update_info,
                    hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth,
                    hdrsrchmcpm.sent_to_mcpm.params.dl_datarate,
                    hdrsrchmcpm.sent_to_mcpm.params.ul_datarate );

    hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
    MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
      &hdrsrchmcpm.sent_to_mcpm.params );
      /* Convey the change in bandwidth to MCPM. */
  }
  else if ( (!mcdo_status) && 
    ( hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth != MCPM_DO ) )
  {
    hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth = MCPM_DO;
      /* Save */

    /* Data rate entries in MCPM are coupled with bandwidth
       => Update data rate as well. */
    hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_3P1_MBPS;
    hdrsrchmcpm.sent_to_mcpm.params.ul_datarate = MCPM_DO_UL_1P8MBPS;

    hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_DL_DATA_RATE_UPDATE | 
      MCPM_UL_DATA_RATE_UPDATE | MCPM_RF_BANDWIDTH_UPDATE;

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH,
                    "Config MCPM for scdo update info 0x%x bandwidth 0x%x dl 0x%x ul 0x%x",
                    hdrsrchmcpm.sent_to_mcpm.params.update_info,
                    hdrsrchmcpm.sent_to_mcpm.params.rf_bandwidth,
                    hdrsrchmcpm.sent_to_mcpm.params.dl_datarate,
                    hdrsrchmcpm.sent_to_mcpm.params.ul_datarate );
    hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
    MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
      &hdrsrchmcpm.sent_to_mcpm.params );
      /* Convey the change in bandwidth to MCPM. */
  }

  return;

} /*  hdrsrchmcpm_set_mcdo_status */

/*============================================================================

FUNCTION HDRSRCHMCPM_GET_MCDO_STATUS

DESCRIPTION
  Get if DO is operating in multicarrier mode.
 
PARAMETERS 
  None.

DEPENDENCIES
  None.

RETURN VALUE 
  TRUE  => DO is operating in multicarrier mode
  FALSE => DO is operating in single carrier mode 

SIDE EFFECTS
  None.

============================================================================*/

boolean hdrsrchmcpm_get_mcdo_status( void )
{
   return hdrsrchmcpm.mcdo_status;
} /* hdrsrchmcpm_get_mcdo_status */

/*============================================================================

FUNCTION HDRSRCHMCPM_SET_ENH_RATES_STATUS

DESCRIPTION
  Sets enhanced rates status and conveys updated FL data rates to MCPM.
 
PARAMETERS 
  enh_rates_enabled  -  TRUE  => Enhanced rates enabled
                        FALSE => Enhanced rates disabled

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_set_enh_rates_status
(
  boolean        enh_rates_enabled
    /* TRUE if enhanced rates are enabled */
)
{

/*--------------------------------------------------------------------------*/
  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }

  /* If there is a change in MCDO status, store the update and convey
     it to MCPM. */
  if ( enh_rates_enabled != hdrsrchmcpm.enhanced_rates_enabled )
  {
    hdrsrchmcpm.enhanced_rates_enabled = enh_rates_enabled;
      /* Save */

    if ( enh_rates_enabled && 
      ( hdrsrchmcpm.sent_to_mcpm.params.dl_datarate == MCPM_DO_DL_9P3_MBPS ) )
    {
      /* Enhanced rates enabled => increase supported DL rate */
      hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_14P7_MBPS;
      hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_DL_DATA_RATE_UPDATE;

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                    "Config MCPM for enh rate update info 0x%x dl 0x%x",
                    hdrsrchmcpm.sent_to_mcpm.params.update_info,
                    hdrsrchmcpm.sent_to_mcpm.params.dl_datarate );
      hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
      MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
        &hdrsrchmcpm.sent_to_mcpm.params );
        /* Convey the change in supported DL rate to MCPM. */

    }
    else if ( ( !enh_rates_enabled ) &&
      ( hdrsrchmcpm.sent_to_mcpm.params.dl_datarate == MCPM_DO_DL_14P7_MBPS ) )
    {
      /* Enhanced rates disabled => decrease supported DL rate */
      hdrsrchmcpm.sent_to_mcpm.params.dl_datarate = MCPM_DO_DL_9P3_MBPS;
      hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_DL_DATA_RATE_UPDATE;

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                    "Config MCPM for non-enh rate update info 0x%x dl 0x%x",
                    hdrsrchmcpm.sent_to_mcpm.params.update_info,
                    hdrsrchmcpm.sent_to_mcpm.params.dl_datarate );
      hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
      MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
        &hdrsrchmcpm.sent_to_mcpm.params );
        /* Convey the change in supported DL rate to MCPM. */

    }
  }

  return;

} /*  hdrsrchmcpm_set_enh_rates_status */



/*============================================================================

FUNCTION HDRSRCHMCPM_DO2LTE_MEASUREMENT

DESCRIPTION
  Conveys DO to LTE measurement start / stop status to MCPM and stores it
  for future MCPM communication.
 
PARAMETERS 
  status  -  TRUE  => DO to LTE measurement start
             FALSE => DO to LTE measurement stop

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_do2lte_measurement
(
  boolean        status
    /* DO to LTE measurement status */
)
{

  boolean config_changed = FALSE; /* Config changed */

/*--------------------------------------------------------------------------*/
  if(!hdrsrchmcpm_enable_mcpm_interaction)
  {
    return;
  }

  /* If there is a change in measurement status, store the update and convey
     it to MCPM. */
  if ( status && 
    ( hdrsrchmcpm.sent_to_mcpm.params.neighbor_meas != MCPM_DO2L_MEAS_START ) )
  {
    /* DO to LTE measurement start */
    hdrsrchmcpm.sent_to_mcpm.params.neighbor_meas = MCPM_DO2L_MEAS_START;
    config_changed = TRUE;
  }
  else if ( !status && 
    ( hdrsrchmcpm.sent_to_mcpm.params.neighbor_meas != MCPM_DO2L_MEAS_STOP ) )
  {
    /* DO to LTE measurement stop */
    hdrsrchmcpm.sent_to_mcpm.params.neighbor_meas = MCPM_DO2L_MEAS_STOP;
    config_changed = TRUE;
  }

  if ( config_changed )
  {
    hdrsrchmcpm.sent_to_mcpm.params.update_info = MCPM_NEIGHBOR_MEAS_UPDATE;

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "Config MCPM for D2L meas update info 0x%x",
                    hdrsrchmcpm.sent_to_mcpm.params.update_info );
    hdrsrchmcpm_cmgr_steady_req(hdrsrchmcpm_cmgr_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ]);
    MCPM_Config_Modem( hdrsrchmcpm_mcpm_request_lu_table[HDRSRCHMCPM_PARMS_UPDATE_REQ],
      &hdrsrchmcpm.sent_to_mcpm.params );
      /* Convey the change in measurement status to MCPM. */

  }

  return;

} /*  hdrsrchmcpm_do2lte_measurement */

/*============================================================================

FUNCTION HDRSRCHMCPM_MCVS_CONFIG_MODEM

DESCRIPTION
  Conveys scenario and related modem clk boost request information to MCPM 
 
PARAMETERS 
  scenario          => scenario to boost

  trigger           => Start or stop for scenario

  timeToApply       => After API call when to apply boost

  q6_clock_KHz      => Q6 clk boost frequency

  modem_vpe_KHz     => VPE clk boost frequency

  modem_clk_bus_KHz => modem clk bus boost frequency

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_mcvs_config_modem
(
  hdr_scenario_type         scenario,
    /* scenario to boost */

  boolean                   trigger,
    /* Scenario start or stop */

  uint64                    timeToApply,
    /* After API call when to apply boost */

  uint32                    q6_clock_KHz,
    /* Q6 clk boost frequency */

  uint32                    modem_vpe_KHz,
    /* VPE clk boost frequency */

  uint32                    modem_clk_bus_KHz
    /* modem clk bus boost frequency */
)
{
  mcpm_mcvsrequest_parms_type mcvs_parms;  /* Used to request clk_bus rate */
  hdrsrchmcpm_scenario_struct_type scenario_info = { 0, 0 };

  memset( &mcvs_parms,0,sizeof( mcpm_mcvsrequest_parms_type ) );

  mcvs_parms.mcvs_req_type = MCVS_FULL_REQUEST;

  mcvs_parms.scenario_info = (uint32 *) &scenario_info;

  if ( scenario < HDR_INVALID_SCENARIO )
  {
    mcvs_parms.mcvs_update_info |= MCPM_MCVS_SCENARIO_INFO_UPDATE;
      /* Indicate the scenario is shared */
    
    scenario_info.scenario = ( ( (uint32) scenario << HDRSRCHMCPM_SCENARIO_MASK_SHIFT ) | trigger ) 
       << HDRSRCHMCPM_SCENARIO_TRIG_MASK_SHIFT;
      /* Bit mask for scenario trigger */
      
    scenario_info.timeToApply = timeToApply;
  }

  if ( q6_clock_KHz != 0 )
  {
    mcvs_parms.mcvs_update_info |= MCPM_MCVS_Q6_CLK_UPDATE;
    mcvs_parms.q6_clock_KHz = q6_clock_KHz;
  }

  if ( modem_vpe_KHz != 0 )
  {
    mcvs_parms.mcvs_update_info |= MCPM_MCVS_SILVER_CLK_UPDATE;
    mcvs_parms.modem_vpe_KHz = modem_vpe_KHz;
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    " MCPM Config modem_vpe_KHz :%d",
                    mcvs_parms.modem_vpe_KHz);
  }

  if ( modem_clk_bus_KHz != 0 )
  {
    mcvs_parms.mcvs_update_info |= MCPM_MCVS_CLKBUS_UPDATE;
    mcvs_parms.modem_clk_bus_KHz = modem_clk_bus_KHz;
  }

  HDR_MSG_SRCH_6( 
    MSG_LEGACY_HIGH,
    "MCVS Config, scenario:%d, trigger:%d, scenario_mask:0x%x, q6_clock_KHz:%d, modem_vpe_KHz:%d, modem_clk_bus_KHz:%d",
    scenario, trigger, scenario_info.scenario, q6_clock_KHz, modem_vpe_KHz, modem_clk_bus_KHz);

  MCPM_MCVSConfig_Modem( MCPM_DO_TECH, &mcvs_parms );
}


/*============================================================================

FUNCTION HDRSRCHMCPM_SERVICE_BOOST

DESCRIPTION
  Conveys clk boost request information to MCPM 
 
PARAMETERS 
  scenario          => scenario to boost

  trigger           => Start or stop for scenario

  boost_mask        => type of boost

  boost_duration    => duration for which boost needs to be applied

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_service_boost
(
  hdr_scenario_type        scenario,
    /* scenario to boost */

  boolean                   trigger,
    /* Scenario start or stop */

  uint32                    boost_mask,
    /* VPE clk boost frequency */

  uint32                    boost_duration
    /* modem clk bus boost frequency */
)
{
  if ( scenario < HDR_INVALID_SCENARIO )
  {   
    boost_mask 
      |= ( ( (uint32) scenario << HDRSRCHMCPM_SCENARIO_MASK_SHIFT ) |  trigger ) 
        << HDRSRCHMCPM_SCENARIO_TRIG_MASK_SHIFT;
      /* Bit mask for scenario trigger */
  }

  HDR_MSG_SRCH_4( 
    MSG_LEGACY_HIGH,
    "MCPM_Service_Boost, scenario:%d, trigger:%d, boost_mask:0x%x, boost_duration:%d",
    scenario, trigger, boost_mask, boost_duration );

  MCPM_Service_Boost_Request(
    MCPM_DO_TECH, 
    boost_mask, NULL, NULL, boost_duration);
}

/*============================================================================

FUNCTION HDRSRCHMCPM_CMGR_TRANS_REQ

DESCRIPTION
  Conveys clk boost request information to Concurrency Mgr
 
PARAMETERS

  scenario          => Transient scenario to boost

  trigger           => Is it clock boost or deboost

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_cmgr_trans_req(uint32 scenario , boolean trigger)
{
  cmgr_params cmgr_req;
  cmgr_tech_type tech = CMGR_DO_TECH;
  memset(&cmgr_req,0,sizeof(cmgr_params));
  
  /* status returned by Concurrency Mgr for the req*/
  cmgr_scenario_status_e status = CMGR_SCENARIO_NOT_RECOGNIZED;

  cmgr_req.scenario_e = scenario;

  if(trigger ==TRUE)
  {
    cmgr_req.type = CMGR_TRANSIENT_START;
  }
  else
  {
    cmgr_req.type = CMGR_TRANSIENT_END;
  }
  cmgr_req.priority = cmgr_get_priority(CMGR_HDR,(cmgr_reason_enum_t)hdrsrchrf_get_reason(TRM_HDR));
  cmgr_req.flow_ctrl_time_msec = 0xFFFFFFFF;
  cmgr_req.duration_msec = 0;
  cmgr_req.start_timestamp = qurt_sysclock_get_hw_ticks(); 
  cmgr_req.return_info = NULL;

  /* If already a request was sent, update the scenario */
  if (((hdrsrchmcpm.sent_to_cmgr_trans.type == CMGR_TRANSIENT_START) ||
        (hdrsrchmcpm.sent_to_cmgr_trans.type == CMGR_TRANSIENT_UPDATE)) && 
      (trigger == TRUE))
  {
    if(hdrsrchmcpm.sent_to_cmgr_trans.type == CMGR_TRANSIENT_START)
    {
      cmgr_req.scenario_e = hdrsrchmcpm.sent_to_cmgr_trans.scenario_e;
    }
    else if(hdrsrchmcpm.sent_to_cmgr_trans.type == CMGR_TRANSIENT_UPDATE)
    {
      cmgr_req.scenario_e = hdrsrchmcpm.sent_to_cmgr_trans.update_scenario_e;
    }
    cmgr_req.update_scenario_e = scenario;
    cmgr_req.type = CMGR_TRANSIENT_UPDATE;
  }
  else 
  {
    cmgr_req.update_scenario_e = 0;
  }

  HDR_MSG_SRCH_5( 
    MSG_LEGACY_HIGH,
    "MCPM_CMGR,Trans scenario:%d, trigger:%d, update_scenario_e:%d, req type:%d, prio:%d",
    cmgr_req.scenario_e, trigger, cmgr_req.update_scenario_e, cmgr_req.type, cmgr_req.priority);
  hdrsrchmcpm.sent_to_cmgr_trans = cmgr_req;
  status = cmgr_scenario_config(tech,&cmgr_req);
  
  ASSERT( status == CMGR_ACCEPTED );
 
}

/*============================================================================

FUNCTION HDRSRCHMCPM_CMGR_STEADY_REQ

DESCRIPTION
  Conveys clk boost request information to Concurrency Mgr
 
PARAMETERS

  scenario          => Steady scenario to boost

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_cmgr_steady_req(uint32 scenario )
{
  cmgr_params cmgr_req;
  cmgr_tech_type tech = CMGR_DO_TECH;
  memset(&cmgr_req,0,sizeof(cmgr_params));
  
  /* status returned by Concurrency Mgr for the req*/
  cmgr_scenario_status_e status = CMGR_SCENARIO_NOT_RECOGNIZED;

  cmgr_req.scenario_e = scenario;
  cmgr_req.type = CMGR_STEADY_START;
  cmgr_req.priority = cmgr_get_priority(CMGR_HDR,(cmgr_reason_enum_t)hdrsrchrf_get_reason(TRM_HDR));
  cmgr_req.flow_ctrl_time_msec = 0xFFFFFFFF;
  cmgr_req.duration_msec = 0;
  cmgr_req.start_timestamp = qurt_sysclock_get_hw_ticks(); 
  cmgr_req.return_info = NULL;

  HDR_MSG_SRCH( MSG_LEGACY_LOW, "MCPM_CMGR, Entering critical section");  
  rex_enter_crit_sect(&hdrsrchmcpm.crit_sect);
  
  /* If already a request was sent, update the scenario */
  if ((hdrsrchmcpm.sent_to_cmgr_steady.type == CMGR_STEADY_START) ||
        (hdrsrchmcpm.sent_to_cmgr_steady.type == CMGR_STEADY_UPDATE))
  {
    if(hdrsrchmcpm.sent_to_cmgr_steady.type == CMGR_STEADY_START)
    {
      cmgr_req.scenario_e = hdrsrchmcpm.sent_to_cmgr_steady.scenario_e;
    }
    else if(hdrsrchmcpm.sent_to_cmgr_steady.type == CMGR_STEADY_UPDATE)
    {
      cmgr_req.scenario_e = hdrsrchmcpm.sent_to_cmgr_steady.update_scenario_e;
    }
    cmgr_req.update_scenario_e = scenario;
    cmgr_req.type = CMGR_STEADY_UPDATE;
  }
  else 
  {
    cmgr_req.update_scenario_e = 0;
  }

  hdrsrchmcpm.sent_to_cmgr_steady = cmgr_req;
  status = cmgr_scenario_config(tech,&cmgr_req);
  
  rex_leave_crit_sect( &hdrsrchmcpm.crit_sect );
  HDR_MSG_SRCH( MSG_LEGACY_LOW, "MCPM_CMGR, Leaving critical section");
  HDR_MSG_SRCH_4( 
      MSG_LEGACY_HIGH,
      "MCPM_CMGR, Steady scenario:%d, update_scenario_e:%d, req type:%d, prio:%d",
       cmgr_req.scenario_e, cmgr_req.update_scenario_e, cmgr_req.type, cmgr_req.priority);

  ASSERT( status == CMGR_ACCEPTED );
 
}

/*============================================================================

FUNCTION HDRSRCHMCPM_CMGR_CB

DESCRIPTION
  Conveys clk boost request information to MCPM 
 
PARAMETERS 
  tech              => Tech which requested the boost

  scenario          => scenario to boost

  status            => Is the request for boost accepted?

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

============================================================================*/

void hdrsrchmcpm_cmgr_cb
(
  cmgr_tech_type tech,
  uint32 scenario, 
  cmgr_scenario_status_e status,
  uint32 additional_info
)
{
  HDR_MSG_SRCH_2( 
    MSG_LEGACY_HIGH,
    "MCPM_CMGR_cb, scenario:%d, status:%d",
    scenario, status);
  ASSERT( status == CMGR_ACCEPTED );
}

