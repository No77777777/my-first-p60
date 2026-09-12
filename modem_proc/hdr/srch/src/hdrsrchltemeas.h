#ifndef HDRSRCHLTEMEAS_H
#define HDRSRCHLTEMEAS_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
       H D R    S R C H    L T E    P I L O T   M E A S U R E M E N T 

                 Search LTE Pilot Measurement State Header File
 
GENERAL DESCRIPTION 
  This file contains prototypes and declarations used in LTE Pilot Measurement
  State.  LTE pilot measurements are taken by the HDR technology to enble reselection
  from HDR to LTE.  HDR->LTE reselection is specified in the 3GPP2 C.S0087 spec.
     
EXTERNALIZED FUNCTIONS
  hdrsrchltemeas_init_lists 
    Initialize LTE frequency lists 
  hdrsrchltemeas_update_lte_neighbor_list 
    New LTE neighbor list has been received from base station
  hdrsrchltemeas_init_meas_state 
    Switch to LTE Pilot Measurement State
  hdrsrchltemeas_send_deinit_req 
    Send DEINIT Req to LTE 
  hdrsrchltemeas_state_visit_required
    Determine if DO should do LTE pilot measurements
  hdrsrchltemeas_treselect_timer_def
    Define Treselect timer
  hdrsrchltemeas_treselect_timer_exp
    Handle Treselect timer expiry
  hdrsrchltemeas_clear_reselect_status
    Indicate that LTE reselection is not happening
  hdrsrchltemeas_process_deinit_cnf_msg
    Process DEINIT_CNF message from LTE
  hdrsrchltemeas_treselect_timer_clear
    Clear Treselect timer
  hdrsrchltemeas_reset_ltemeas_state
    Reset LTE Measurement state
  hdrsrchltemeas_lte_deinit_is_finished
    Return LTE deinit status
  hdrsrchltemeas_rfm_meas_enter
    Request RF to enter MEAS
  hdrsrchltemeas_iratman_wakeup_cb
    DO is woken up by IRATMan to process Treselect timer expiry
  hdrsrchltemeas_reset_deinit
    Reset LTE Measurement state from Inactive state or no LTE coverage
  hdrsrchltemeas_powerup_init
    Perform D2L initialization at modem power up
  hdrsrchltemeas_enter_inactive_state
    Reselt D2L state before HDR is deactivated
  hdrsrchltemeas_get_sector
    Provide IRATMan with HDR sector ID
  hdrsrchltemeas_update_sector
    Get HDR sector ID from CP
 
REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2010 thru 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchltemeas.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/28/15   vlc     Added support for DO->LTE redirection in connected state.
05/07/15   vlc     Added hdrsrchltemeas_abort_lte_reselection(). 
06/02/14   vlc     Changed TRM request sequence for D2L measurements. 
12/20/13   vlc     Added support for Tbackoff timer optimization. 
10/11/13   vlc     Added support for new LTE TIMED_SEARCH_MEAS_REQ API.
07/13/12   vlc     Added prototype for hdrsrchltemeas_reset_deinit(). 
03/05/12   vlc     Added IRATMan interface to D2L reselection feature. 
12/22/11   vlc     Added code to call RF sleep after LTE meas for D2L. 
11/04/11   vlc     Added prototype for hdrsrchltemeas_lte_deinit_is_finished().
09/29/11   vlc     Changed hdrsrchltemeas_send_deinit_req() to return a
                   boolean value.
03/23/11   vlc     Added prototype for hdrsrchltemeas_reset_ltemeas_state(). 
03/02/11   vlc     Added prototype for hdrsrchltemeas_treselect_timer_clear().  
02/17/11   vlc     Added prototype for hdrsrchltemeas_process_deinit_cnf_msg(). 
01/27/11   vlc     Added prototype for hdrsrchltemeas_clear_reselect_status(). 
08/16/10   vlc     Created file for DO->LTE pilot measurements/reselection.

===========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"
#include "customer.h"
#ifdef FEATURE_HDR_TO_LTE
#include "hdraddr.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/* Struct type of RF parameters for LTE and DO to build RF scripts */
typedef struct
{
  lm_handle_type                               lte_buf_chain0;
    /* RXLM buffer allocation handle for RF chain 0 for LTE */

  lm_handle_type                               lte_buf_chain1;
    /* RXLM buffer allocation handle for RF chain 1 for LTE */

  boolean                                      lte_handle_valid;
    /* Validity of the stored handle locations */

  lm_handle_type                               do_buf_chain0;
    /* RXLM buffer allocation handle for RF chain 0 for DO */

  boolean                                      do_handle_valid;
    /* Validity of the stored handle location */

}
hdrsrchltemeas_rxlm_struct_type;

#define TIME_OCCURS_FIRST( time_a, time_b ) \
  ( ( ( time_a ) - ( time_b ) ) > ( ( time_b ) - ( time_a ) ) )
  /* Both time values are unsigned, find which time occurs first taking into account
     a rollover with a range much larger than the distance between time_a and time_b.
     Returns TRUE if time_a occurs before time_b. */

/*===========================================================================

                    EXTERNAL FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_INIT_LISTS

DESCRIPTION 
  This function initializes frequency lists used for LTE pilot measurements.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchltemeas_init_lists( void );

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_PREP_TO_UPDATE_LTE_NEIGHBOR_LIST

DESCRIPTION 
  This function is called every time DO gets a new LTE neighbor list.
  Since this function can be called in any state, it only saves the neighbor
  list and returns immediately.  The actual update happens when DO is
  ready to do LTE measurements.  This way, DO is not spending too much time
  updating the LTE frequency list in all states when it is not yet necessary
  to do so.  The information is not used until DO is ready to do LTE
  measurements anyway.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

=========================================================================*/
errno_enum_type hdrsrchltemeas_prep_to_update_lte_neighbor_list
( 
  const hdrsrch_lte_nset_update_params_type   *lte_nlist
);

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_IRATMAN_WAKEUP_CB

DESCRIPTION
  This function is called by IRATMan when the Treselect timer expires.
  The LTE frequency with the expired timer is marked, and DO is woken up if it
  is currently in sleep state.  All this is done in DO task context, thus this
  function only sends a DO command here.
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

boolean hdrsrchltemeas_iratman_wakeup_cb( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_STATE_VISIT_REQUIRED

DESCRIPTION
  This function is called to determine if DO should do LTE Pilot measurements
  during the start of this sleep cycle.  It calls IRATMan function to get the
  projected time for LTE Meas.  This time may be shortened depending on RF
  availability.
 
  The TRM request sequence is shown below:
  1.  Get Min duration from IRATMan for 1 LTE frequency.
      Also get Max duration from IRATMan for all LTE freqs which DO can
      measure within its sleep cycle.
  2.  Change duration using wrapper function hdrsrchrf_prep_for_irat_meas().
      It’s basically trm_change_duration().
      Provide TRM with Min duration, ignore return value because it’s still
      HDR_DEMOD_PAGE at this point
  3.  Change reason, again with a wrapper function.  If this call is successful,
      it means that DO at least have time to make 1 LTE measurement.
  4.  Change duration again, but this time provide the correct Min and Max values
      which IRATMan provided.  Save the duration returned by TRM.
  5.  Use the duration returned by TRM to begin LTE meas.
 

DEPENDENCIES
  None
 
PARAMETERS
  Sleep time (in ms), adjusted to not include min sleep required

RETURN VALUE
  TRUE:  Transition to LTE meas state
  FALSE:  No LTE meas this sleep cycle

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchltemeas_state_visit_required
( 
  uint32                                       sleep_ms
    /* Sleep time, adjusted to not include minimum sleep required */
 
);

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_INIT_MEAS_STATE

DESCRIPTION
  This function causes the state machine to begin LTE Pilot Measurement
  state processing.

DEPENDENCIES
  Runs during sleep state

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Transitions to the LTE Pilot Measurement state

===========================================================================*/

void hdrsrchltemeas_init_meas_state( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_SEND_DEINIT_REQ

DESCRIPTION
  This function sends the DEINIT request to LTE to reset LTE Pilot Measurement
  state.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS 
  None 

===========================================================================*/

boolean hdrsrchltemeas_send_deinit_req( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_DEF

DESCRIPTION
  This function defines the Trelect timer for use in DO->LTE reselection.
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_treselect_timer_def( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_EXP

DESCRIPTION
  This function is called when the Treselect timer expires.  The LTE frequency
  with the expired timer is marked, and DO is woken up if it is currently in
  sleep state.   
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchltemeas_treselect_timer_exp( void );

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_CLEAR_RESELECT_STATUS

DESCRIPTION 
  This function clears the LTE reselect status.  It is called whenever DO
  L1 receives the ACTIVATE command.  Since DO is being re-activated, LTE
  reselection must be over.

DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  None

SIDE EFFECTS 
  None 

=========================================================================*/

void hdrsrchltemeas_clear_reselect_status( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_DEINIT_CNF_MSG

DESCRIPTION
  This function processes the Deinit confirmation message received from LTE.
  The DEINIT request may not be sent in LTE MEAS state, so check first before
  attempting to go back to sleep.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_deinit_cnf_msg( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_CLEAR

DESCRIPTION
  This function is called to clear the LTE Treselect timer.
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_treselect_timer_clear( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RESET_LTEMEAS_STATE

DESCRIPTION 
  This function resets LTE measurement state without clearing LTE Init Rx'ed
  indicator.  It is desirable to send deinit request to LTE only in areas with
  no LTE neighbor list so this is a separate operation.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_reset_ltemeas_state( void );

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_LTE_DEINIT_IS_FINISHED

DESCRIPTION 
  This function returns the status of the LTE Deinit operation.  The Deinit
  operation is simple on DO side but is time consuming on LTE side.
 
  This function indicates whether or not DO should delay its processing until
  the LTE Deinit operation is complete.

DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  TRUE:   LTE Deinit operation is finished
  FALSE:  Operation is not finished, HDR should wait

SIDE EFFECTS 
  None 

=========================================================================*/

boolean hdrsrchltemeas_lte_deinit_is_finished( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RFM_MEAS_ENTER

DESCRIPTION 
  This function requests RF to enter LTE measurement mode.
 
DEPENDENCIES
  None

RETURN VALUE 
  None                                                                             d

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_rfm_meas_enter( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RESET_DEINIT

DESCRIPTION 
  This function resets LTE measurement state when LTE measurements are done
  and LTE deinit confirmation has been received by DO.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_reset_deinit( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_SCHEDULE_TIMED_SEARCH

DESCRIPTION
  LTE requires a call to schedule the timed search request
  before sending the request.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_schedule_timed_search
( 
  lte_cphy_irat_meas_timed_srch_meas_req_s  *timed_srch_meas_ptr
);

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_UPDATE_SECTOR

DESCRIPTION 
  This function gets the HDR sector ID from Protocol layer for the 
  calculation of the optimized Tbackoff timer.

DEPENDENCIES
  Must be called when HDR is still in LTE meas state to avoid race condition
  in case sector ID is being changed.

PARAMETERS
  Pointer to write the sector ID

RETURN VALUE 
  TRUE:  Operation completes successfully
  FALSE:  Unexpected error, operation fails

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchltemeas_update_sector ( hdraddr_type hdr_sector_ptr );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_SECTOR

DESCRIPTION 
  This function provides IRATMan with the HDR sector ID for the calculation of
  of the optimized Tbackoff timer.

DEPENDENCIES
  Must be called when HDR is still in LTE meas state to avoid race condition
  in case sector ID is being changed.

PARAMETERS
  Pointer to IRATMan memory for HDR to write the sector ID

RETURN VALUE 
  TRUE:  Operation completes successfully
  FALSE:  Unexpected error, operation fails

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchltemeas_get_sector( hdraddr_type sector_id );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_POWERUP_INIT

DESCRIPTION 
  This function performs D2L initialization at modem power up.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_powerup_init( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_ENTER_INACTIVE_STATE

DESCRIPTION 
  This function prepares D2L for inactive state.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_enter_inactive_state( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_ABORT_LTE_RESELECTION

DESCRIPTION
  This function aborts LTE reselection, if LTE reselection is in progress.
  This function is needed to release the pending TRM reservation DO L1 has made
  for LTE prior to initiating reselection procedure with DO CP.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_abort_lte_reselection ( void );

#endif /* FEATURE_HDR_TO_LTE  */
#endif /* HDRSRCHLTEMEAS_H */

