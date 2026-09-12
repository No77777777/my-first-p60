#ifndef HDRSRCHTCLTE_H
#define HDRSRCHTCLTE_H

/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
   
         H D R    S R C H    L T E    P I L O T   M E A S U R E M E N T
 
                         A N D     R E D I R E C T I O N    
   
                      Searcher LTE Redirection State Module
 
GENERAL DESCRIPTION 
  This file contains the HDR Searcher LTE Pilot Measurement for redirection state
  machine.
  LTE pilot measurements are requested by the HDR technology to enble redirection 
  from HDR to LTE during HDR connected state.  HDR->LTE redirection is specified in
  the following spec.  The appropriate sections are listed.
  3GPP2 C.S0087-A v3.0
  E-UTRAN – cdma2000 HRPD Connectivity and Interworking Air Interface Specification

  • 5.7.9.1 InterRATRedirect
  • 5.7.9.2 OtherRATMeasurementRequest
  • 5.7.9.3 OtherRATMeasurementReport

  The procedure executes while DO is in connected state and contains the following
  steps:
 
  - HDR indicates that AT is capable of LTE redirection during session negotiation.
  - AN sends OtherRatRedirectionRequest message during HDR connected state.
  - At the appropriate time interval, HDR requests LTE measurements.
  - HDR stays in suspend state similar to long tune away while LTE makes measurements.
  - HDR compares measurement results from LTE and decides when to report the
    results in the OtherRATMeasurementReport.
  - AN sends the InterRATRedirect message to redirect AT to LTE.
 
EXTERNALIZED FUNCTIONS
          
REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2015 - 2020 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchtclte.h#2 $ $DateTime: 2020/04/14 08:32:30 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
04/13/20   svu     FR 55482: Extension of elevator mode to C2K RAT
07/08/15   vlc     Added function hdrsrchtclte_powerup_init(). 
05/28/15   vlc     Created file for DO->LTE pilot measurements/redirection.

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/*===========================================================================

                    FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================

FUNCTION HDRSRCHTCLTE_INIT_TCLTE_STATE

DESCRIPTION
  This function is called to prepare for LTE MEAS state.  The TRM lock has
  been extended for the measurements and there is at least one LTE frequency
  to measure.  

  DO enters this state to request LTE pilot measurements of neighboring LTE
  frequencies.  The resulting measurements are used to decide whether or
  not to redirect to LTE from DO connected state.

DEPENDENCIES
  None

PARAMETERS 
  None

RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_init_tclte_state( void );

/*=========================================================================

FUNCTION HDRSRCHTCLTE_PROCESS_LTE_NSET_CMD

DESCRIPTION 
  The Protocol layer sends the Searcher layer the LTE neighbor list after
  processing the Other RAT Measurement request OTA message from Network.  This
  command may also arrive as part of UE initated IRAT, in which case the
  LTE neighbor list comes from UE database, not Network.

DEPENDENCIES 
  None 

PARAMETERS 
  Pointer to command parameters

RETURN VALUE 
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

=========================================================================*/
errno_enum_type hdrsrchtclte_process_lte_nset_cmd
( 
  const hdrsrch_lte_nset_update_redir_params_type   *lte_nset_redir_ptr
);

/*===========================================================================

FUNCTION HDRSRCHTCLTE_REQUEST_TRM_GRANT

DESCRIPTION 
  This function is called to decide if DO should enter LTE meas state from
  Connected state to make LTE measurements for the purpose of redirection.
 
DEPENDENCIES
  TRM must be granted for the measurement
 
PARAMETERS 
  Available time for measurement
  Additional required DO rampdown time 
 
RETURN VALUE 
  TRUE:  LTE measurement should be performed
  FALSE:  No LTE measurement

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchtclte_request_trm_grant
(  
  uint16                                            avail_ms,
  uint16                                            rampdown_ms
);

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SCHEDULE_TIMED_SEARCH

DESCRIPTION
  LTE requires a call to schedule the timed search request
  before sending the request.
  
DEPENDENCIES
  None

PARAMETERS
  Ptr to Timed Search Meas request which DO is going to send to LTE.
 
RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/
errno_enum_type hdrsrchtclte_schedule_timed_search
( 
  lte_cphy_irat_meas_timed_srch_meas_req_s          *timed_srch_meas_ptr
);

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SEND_DEINIT_REQ

DESCRIPTION 
  This function sends the DEINIT request to LTE.  This is part of the LTE
  IRAT API.  LTE resets its state upon receving this request.
 
DEPENDENCIES
  None
 
PARAMETERS 
  None
 
RETURN VALUE 
  TRUE:  DEINIT request is sent to LTE
  FALSE:  It is not necessary to send DEINIT request to LTE

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchtclte_send_deinit_req( void );

/*===========================================================================

FUNCTION HDRSRCHTCLTE_LTE_DEINIT_IS_FINISHED

DESCRIPTION
  This function returns a boolean value indicating if LTE deinit is required.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE:  DO needs to send LTE Deinit request
  FALSE:  DO does not need to send LTE Deinit request

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchtclte_lte_deinit_is_finished( void );

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SERVING_THRESHOLD_IS_TRIGGERED

DESCRIPTION
  Per spec requirement, DO only makes LTE measurements if DO signal strength
  is below the Serving threshold.  This function determines if that is the case.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE:  DO should perform LTE measurements
  FALSE:  DO signal strength is good, do not perform LTE measurements

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchtclte_serving_threshold_is_triggered( void );

/*===========================================================================

FUNCTION HDRSRCHTCLTE_MAX_REPORT_COUNT_IS_TRIGGERED
 
DESCRIPTION
  If the maximum number of report has been sent, return because we do not
  want to make anymore measurement.  This limitation helps DO performance
  during Connected state.
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  TRUE:  DO has met the maximum report it can send, do not perform measurement
         until another LTE NSET command is received.
  FALSE: Max report count not met, perform measurement.

SIDE EFFECTS 
  None 

===========================================================================*/
boolean hdrsrchtclte_max_report_count_is_triggered( void );

/*===========================================================================

FUNCTION HDRSRCHTCLTE_MIN_RPT_INTERVAL_IS_TRIGGERED

DESCRIPTION
  Per spec requirement, DO only makes LTE measurements once the minimum report
  interval has been exceeded (from the previous report.)
  This function determines if that is the case.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE:  DO should perform LTE measurements
  FALSE:  Still within minimum report interval, do not perform LTE measurements

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchtclte_min_rpt_interval_is_triggered( void );

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_DEINIT_CNF_MSG

DESCRIPTION
  This function processes the Deinit confirmation message received from LTE.
  The DEINIT request may arrive outside LTE MEAS state.  If it arrives while
  DO is in INACTIVE state, turn off DO clocks.  DO clocks are required for the
  Deinit operation, when done it needs to be turned off.
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS
  Shuts down DO clocks in Inactive state

===========================================================================*/
errno_enum_type hdrsrchtclte_process_deinit_cnf_msg( void );

/*===========================================================================

FUNCTION HDRSRCHTCLTE_RELEASE_TRM_GRANT

DESCRIPTION
  In the case of an abort or when LTE meas is somehow interrupted, this function
  ensures that DO requests TRM to end the transaction.  This is only done if
  the TRM grant is still outstanding.  

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_release_trm_grant( void );

/*===========================================================================

FUNCTION HDRSRCHTCLTE_POWERUP_INIT

DESCRIPTION
  Initializes parameters for D2L measurement and redirection during DO traffic.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_powerup_init( void );
/*===========================================================================

FUNCTION HDRSRCHTCLTE_IS_IN_FAST_LTE_SRCH_MODE

DESCRIPTION
 This API returns whether DO is in fast LTE SRCH mode
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE  : if the Flag is set
  FALSE : otherwise
SIDE EFFECTS
  None

===========================================================================*/
boolean hdrsrchtclte_is_in_fast_lte_srch_mode( void );
#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */
#endif /* HDRSRCHTCLTE_H */


