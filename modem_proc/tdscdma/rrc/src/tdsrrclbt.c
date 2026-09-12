/*===========================================================================
         R R C   L O O P B A C K   T E S T   M O D U L E 

DESCRIPTION

  This module performs the actions related to the loopback test. It 
  maintanences the loopback test mode and test type information. It processes 
  the messages from and to the TC. It also performs the channel reconfiguration
  to support the loopback test based on the test type. However, the RB setup 
  and release for the loopback test is done through the RRC RB Setup procedure.
  
  If there is a failure this module sends an Active Set Update failure
  message with the appropriate failure cause.
  
EXTERNALIZED FUNCTIONS

  tdsrrclbt_procedure_event_handler   Event handler for this procedure
  tdsrrclbt_init_procedure            Power-up initialization of the procedure

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Procedure must be initialized at power-up by calling tdsrrcasu_init_procedure. 
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrclbt.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "err.h"
#include "msg.h"
#include "memheap.h"
#include "tdsrrclbt.h"
#include "tdsrrcsend.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd.h"
#include "tdsrrcintif.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcccm.h"
#include "ms.h"
#ifdef FEATURE_TC
#include "tctask.h"
#ifdef FEATURE_CMI
#include "tctask_v.h"
#endif
#endif
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"
#include "tdsrrcdataext.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
/* if TC is enabled, the following constants will be included in tc_rrc.h */
#ifndef FEATURE_TC
#define RRCTC_NORMAL_MODE       0x21
#define RRCTC_RB_TEST_MODE      0x22

#define RRCTC_LB_MODE1          0x41
#define RRCTC_LB_MODE2          0x42

#endif

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
/* Maximum number of transactions accepted for the ASU message             */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* RRC Test Mode                                                           */
byte tdsrrc_test_mode;

/* Loopback Test Type                                                      */
byte tdsrrc_lb_type;

/* Loopback 2 dummy signaling test mode */
boolean tdsrrc_dummy_signaling_test;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
/* LB substate */
tdsrrclbt_substates_e_type tdslbt_substate;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */


/*===========================================================================

FUNCTION  RRCLBT_INIT_DATA

DESCRIPTION

  This function clears all the procedure variables and puts the initial
  values wherever needed.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_init_data( void )
{                   

  /* Reset variables for each transaction */
  tdslbt_substate = TDSRRCLBT_INITIAL;
  tdsrrc_test_mode = RRCTC_NORMAL_MODE;
  tdsrrc_dummy_signaling_test = FALSE;
}



/*===========================================================================

FUNCTION  RRCLBT_IS_LB_TEST

DESCRIPTION

  This function returns True if RRC is in the loopback test mode, otherwise,
  returns False.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrclbt_is_lb_on( void )
{                   

  /* Check if RRC is in the Loopback test */
  if ( tdsrrc_test_mode == RRCTC_RB_TEST_MODE ) {
    return ( TRUE );
  }
  else
  {
    return ( FALSE );
  }
}

/*===========================================================================

FUNCTION  RRCLBT_GET_LB_MODE_AND_DUMMY_SIG_ENABLED

DESCRIPTION

  This functions returns TRUE and the LB mode and signalling enabled status.
  If LB is not active, returns false
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrclbt_get_lb_mode_and_dummy_sig_enabled( tdsrrc_lb_test_mode_e_type *lb_mode,
                                                  boolean  *dummy_signaling_enabled)
{                   
  boolean ans = TRUE;
  /* Check if RRC is in the Loopback test */
  if ((tdslbt_substate == TDSRRCLBT_LB_IN_PROGRESS) &&
      (tdsrrc_test_mode == RRCTC_RB_TEST_MODE)) 
  { 
    /* Set the LP test mode */
    if (tdsrrc_lb_type == RRCTC_LB_MODE1)

    {
      *lb_mode = TDSLB_TEST_MODE_1;
    }
    else if (tdsrrc_lb_type == RRCTC_LB_MODE2)
    {
      *lb_mode = TDSLB_TEST_MODE_2;
    }
    else
    {
      ans = FALSE;
    }
    *dummy_signaling_enabled = tdsrrc_dummy_signaling_test;
  }
  else
  {
    ans = FALSE;
  }

  return ans;
}
/*===========================================================================

FUNCTION   RRCLBT_REQUEST_CHANNEL_RECONFIG

DESCRIPTION

  This function sends the TDSRRC_CHANNEL_CONFIG_REQ command to LLC
  to request a channel reconfiguration for loopback test.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_request_channel_reconfig( void )
{
  tdsrrc_cmd_type *cmd_ptr;

  /* Get the command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL)
  {
    /* Fill in the command id  */
    cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;

    /* Fill in procedure id    */
    cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_LBT;

    /* Indicate that state change is not required,
    since we will be in CELL_DCH only */
    cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
    
    /* Indicate that we would like to get a confirmation 
    back from LLC */
    cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

    /* set the number of RB to config to 0 */
    cmd_ptr->cmd.chan_config_req.num_rb_to_config = 0;

    /* Set the LP test mode */
    if (tdsrrc_lb_type == RRCTC_LB_MODE1)

    {
      cmd_ptr->cmd.chan_config_req.lb_test_mode = TDSLB_TEST_MODE_1;
    }
    else 
    {
      cmd_ptr->cmd.chan_config_req.lb_test_mode = TDSLB_TEST_MODE_2;
    }

    /* Set Dummy signaling test mode */
    cmd_ptr->cmd.chan_config_req.dummy_signaling_enabled = 
                                 tdsrrc_dummy_signaling_test;

    TDSRRC_MSG0(MSG_LEGACY_HIGH," Sending CHAN_CONFIG_REQ for LBT");
    tdsrrc_put_int_cmd( cmd_ptr );

  }
  else
  {
    /* Out of memory!!  */
    ERR_FATAL("Can't send channel config! No memory!",0,0,0);
  }
}/* end function tdsrrclbt_request_channel_reconfig */




/*===========================================================================

FUNCTION  RRCLBT_SEND_LB_CNF

DESCRIPTION

  This function sends a RRCTC_TEST_LOOPBACK_CNF message to TC to indicate 
  LB has successfully been configured.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_send_lb_cnf( void )
{                   
#ifdef FEATURE_TC
  tc_cmd_type *cmd_ptr;

  /* get a command TC command buffer */
  if ( (cmd_ptr = tc_get_cmd_buf()) != NULL )
  {
    /* Send LB Request to TC */
    cmd_ptr->header.message_set = MS_RRC_TC;
    cmd_ptr->header.cmd_id = RRCTC_TEST_LOOPBACK_CNF;

#if defined(FEATURE_DUAL_WCDMA)&& defined(FEATURE_DUAL_SIM)
    cmd_ptr->cmd.rrc_test_loopback_cnf.as_id = tdsrrc_get_as_id();
#endif

    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_tc_put_cmd(cmd_ptr);
    }
    tc_put_cmd(cmd_ptr);
  }
  else
  {
    /* Out of memory!!  */
    ERR_FATAL("Can't allocate LB CNF buf!",0,0,0);
  }
#endif
  return;
}



/*===========================================================================

FUNCTION  RRCLBT_SEND_LB_REJ

DESCRIPTION

  This function sends a RRCTC_TEST_LOOPBACK_REJ message to TC to indicate 
  LB can not be configured.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_send_lb_rej( void )
{                   
#ifdef FEATURE_TC
  tc_cmd_type *cmd_ptr;

  /* get a command TC command buffer */
  if ( (cmd_ptr = tc_get_cmd_buf()) != NULL )
  {
    /* Send LB Request to TC */
    cmd_ptr->header.message_set = MS_RRC_TC;
    cmd_ptr->header.cmd_id = RRCTC_TEST_LOOPBACK_REJ;

#if defined(FEATURE_DUAL_WCDMA)&& defined(FEATURE_DUAL_SIM)
    cmd_ptr->cmd.rrc_test_loopback_rej.as_id = tdsrrc_get_as_id();
#endif

    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_tc_put_cmd(cmd_ptr);
    }
    tc_put_cmd(cmd_ptr);
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"LOOPBACK REJ is sent to TC");
  }
  else
  {
    /* Out of memory!!  */
    ERR_FATAL("Can't allocate LB REJ buf!",0,0,0);
  }
#endif
  return;
}



/*===========================================================================

FUNCTION   RRCLBT_PROCESS_CHANNEL_CONFIG_CNF

DESCRIPTION

  This function processes the received Channel Config Confirmation
  from LLC. 
          
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_process_channel_config_cnf
(
  tdsrrc_channel_config_cnf_type *ptr  /* Pointer to the Channel cfg cnf msg */
)
{
  if( ptr->chan_cfg_status != TDSRRCLLC_CHAN_CFG_SUCCESS )
  { 
    /* Channel reconfig failed, clear the LB test */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Channel config failed for LBT");
    tdsrrclbt_send_lb_rej();
    tdsrrclbt_init_data();
  }
  else
  {
    /* Inform TC that channel reconfig completed */
    tdsrrclbt_send_lb_cnf();

    /* Change LBT state to LP_IN_PROGRESS */
    tdslbt_substate = TDSRRCLBT_LB_IN_PROGRESS;
  }
} /* end of tdsrrclbt_process_channel_config_cnf */



/*===========================================================================

FUNCTION   RRCLBT_INITIAL_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the LB is in the TDSRRCLBT_INITIAL substate. This function
  looks at the received event and does the appropriate processing
  of the event.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_initial_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr /* Received command                   */
)
{

  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_TC_MODE_CHANGE:
      
      /* Set RRC Test Mode according received command */
#ifdef FEATURE_TC
      tdsrrc_test_mode=cmd_ptr->cmd.mode_change.mode_type;
#endif
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Mode Changed: %d",tdsrrc_test_mode);
      break;

    case TDSRRC_STATE_CHANGE_IND:

      /* If LP test enabled, clear the state */
      if (tdsrrc_test_mode == RRCTC_RB_TEST_MODE) {
        tdsrrclbt_send_lb_rej();
        tdsrrclbt_init_data();
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"State to Idle,LP stopped");
      }
      break;

    case TDSRRC_TC_TEST_LOOPBACK_IND:

      /* Store LB test type and perform the channel re-configuration */
      if (tdsrrc_test_mode != RRCTC_RB_TEST_MODE) {
        tdsrrc_test_mode = RRCTC_RB_TEST_MODE;
      }
#ifdef FEATURE_TC
      tdsrrc_lb_type = cmd_ptr->cmd.test_loopback_ind.lb_mode_type;
      tdsrrc_dummy_signaling_test = 
        cmd_ptr->cmd.test_loopback_ind.dummy_signaling_enabled;
#endif

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Mode:%d,Dummy Signaling:%d",tdsrrc_lb_type,tdsrrc_dummy_signaling_test);

      /* Request LLC to perform channel reconfiguration */
      if (tdsrrc_lb_type == RRCTC_LB_MODE1 || 
          tdsrrc_lb_type == RRCTC_LB_MODE2)
      {
        tdsrrclbt_request_channel_reconfig(); 

        /* Change the substate */
        tdslbt_substate = TDSRRCLBT_WAIT_FOR_CHAN_CFG_CNF;
      }
      #ifdef FEATURE_MBMS
      #error code not present
#endif
      else
      {
        /* send LB reject to TC */
        tdsrrclbt_send_lb_rej();
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid LB Mode: %d",tdsrrc_lb_type);
        tdsrrclbt_init_data();
      }

      break;

    default:

      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring LB event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdslbt_substate);
      break;
  }
}



/*===========================================================================

FUNCTION   RRCLBT_WFCHCNF_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the LBT procedure is in the TDSRRCLBT_WAIT_FOR_CHAN_CFG_CNF substate.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_wfchcnf_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr /* Received command                   */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_CHANNEL_CONFIG_CNF:
      
      /* Process the channel config confirm command */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing CHANNEL_CONFIG_CNF cmd");
      tdsrrclbt_process_channel_config_cnf
        ( &(cmd_ptr->cmd.chan_config_cnf) );  
      break;

    case TDSRRC_TC_MODE_CHANGE:
      
      /* Check the new test mode in the command */
#ifdef FEATURE_TC
      if ( cmd_ptr->cmd.mode_change.mode_type == RRCTC_NORMAL_MODE ) {
        /* LB is terminated */
        tdsrrclbt_init_data();
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LP is stopped by TC");
      }
#endif
      break;

    case TDSRRC_STATE_CHANGE_IND:

      /* LB is terminated due to state transition to Idle */
      tdsrrclbt_init_data();
      tdsrrclbt_send_lb_rej();
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"State to Idle,LP stopped");
      break;

    default:

      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring LB event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdslbt_substate);
      break;
  }
}




/*===========================================================================

FUNCTION   RRCLBT_LBINP_SUBSTATE_EVENT_HANDLER

DESCRIPTION

  This function handles the events received by this module when
  the procedure is in the LP Test in progress substate.
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrclbt_lbinp_substate_event_handler
(
  tdsrrc_cmd_type *cmd_ptr  /* Received command                   */
)
{
  /* Switch on the received command. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_TC_MODE_CHANGE:
#ifdef FEATURE_TC

       TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_TC_MODE_CHANGE %d",cmd_ptr->cmd.mode_change.mode_type);
      /* If Test mode is normal, test is terminated */
      if (cmd_ptr->cmd.mode_change.mode_type == RRCTC_NORMAL_MODE)
      {
        tdsrrclbt_init_data();
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LB is completed %d");
      }
#endif
      break;

    case TDSRRC_TC_TEST_LOOPBACK_IND:

      /* Store LB test type and perform the channel re-configuration */
      if (tdsrrc_test_mode != RRCTC_RB_TEST_MODE) {
        tdsrrc_test_mode = RRCTC_RB_TEST_MODE;
      }
#ifdef FEATURE_TC

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_TC_TEST_LOOPBACK_IND %d",cmd_ptr->cmd.test_loopback_ind.lb_mode_type);
      /* Store LB test type and perform the channel re-configuration */
      if (tdsrrc_lb_type == cmd_ptr->cmd.test_loopback_ind.lb_mode_type &&
          tdsrrc_dummy_signaling_test == cmd_ptr->cmd.test_loopback_ind.dummy_signaling_enabled)
      {
        /* no need to perform the re-configuration */
        tdsrrclbt_send_lb_cnf();
        
      }
      else
      {
        /* Request LLC to perform channel reconfiguration */
        if (cmd_ptr->cmd.test_loopback_ind.lb_mode_type == RRCTC_LB_MODE1 ||
            cmd_ptr->cmd.test_loopback_ind.lb_mode_type == RRCTC_LB_MODE2)
        {
          tdsrrc_lb_type = cmd_ptr->cmd.test_loopback_ind.lb_mode_type;
          tdsrrc_dummy_signaling_test = cmd_ptr->cmd.test_loopback_ind.dummy_signaling_enabled;
 
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"LB Mode:%d,Dummy Signaling:%d",tdsrrc_lb_type,tdsrrc_dummy_signaling_test);

          tdsrrclbt_request_channel_reconfig();

          /* Change the substate */
          tdslbt_substate = TDSRRCLBT_WAIT_FOR_CHAN_CFG_CNF;
        }
        #ifdef FEATURE_MBMS
        #error code not present
#endif
        else
        {
          /* send LB reject to TC */
          tdsrrclbt_send_lb_rej();
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid LB Mode: %d",cmd_ptr->cmd.test_loopback_ind.lb_mode_type);
        }
      }
#endif
      break;

    case TDSRRC_STATE_CHANGE_IND:

      /* LP test is terminated due to the RRC state change */
      tdsrrclbt_send_lb_rej();
      tdsrrclbt_init_data();
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LP stopped due to state change");
      break;
      
    default:

      /* No other events are normal in this
      substate */
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring LB event %x in substate %d",cmd_ptr->cmd_hdr.cmd_id,tdslbt_substate);
      break;
  }
}




/*===========================================================================

FUNCTION RRCLBT_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Active Set Update procedure. This function is called by the
  dispatcher whenever an event is received for this module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrclbt_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
)
{

  /* Check the procedure substate and procedure the event accordingly */  
  switch( tdslbt_substate )
  {
    case TDSRRCLBT_INITIAL:
      /* Call the event handler for Initial substate */
      tdsrrclbt_initial_substate_event_handler( cmd_ptr );
      break;

    case TDSRRCLBT_WAIT_FOR_CHAN_CFG_CNF:
      /* Call the event handler for waiting for chan cfg cnf substate */
      tdsrrclbt_wfchcnf_substate_event_handler( cmd_ptr );
      break;
  
    case TDSRRCLBT_LB_IN_PROGRESS:
      /* Call the event handler for LB in progress substate */
      tdsrrclbt_lbinp_substate_event_handler( cmd_ptr );
      break;

    default:
      /* Invalid substate - should never happen. Clear
         all procedure vars */
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid LB Substate %d",tdslbt_substate);
      tdsrrclbt_init_data();
      break;
  }/* end switch */
}





/*===========================================================================

FUNCTION  RRCLBT_INIT_PROCEDURE

DESCRIPTION

  This function initializes the LBT procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrclbt_init_procedure( void )
{
  tdsrrclbt_init_data(); /* Initialize all the procedure variables */

  /* Register for state change  - When state changes from DCH
  to any other state we want to be notified since this procedure
  is active only in the DCH state. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_LBT,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_DISCONNECTED  /* To State       */
                           );
}
