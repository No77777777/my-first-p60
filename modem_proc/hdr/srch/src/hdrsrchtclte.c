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

  Copyright (c) 2015 - 2021 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchtclte.c#3 $ $DateTime: 2021/05/18 05:54:37 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
05/18/21   vaa     Fix for compilation errors in MSIM disable build flavor
04/13/20   svu     FR 55482: Extension of elevator mode to C2K RAT
05/07/18   vke     Changes to not to enable tune away in 4d state
02/19/18   wsh     Fixed KW warnings 
02/07/17   vke     FR36756 Changes to update TRM for APS state change
01/30/18   vko/vlc Calling deactivate done cb to almp only after receiving 
                   the DE_INIT_CNF REQ from LTE0
1/08/18   pga      Fixed compiler warnings
10/30/17   vko     FR 29153: Support HDR/CDMA on the 2nd sub 
10/16/17   vko     Use new scenario type for D2L connected measurement
08/03/16   vko     HDR to pass IRAT sub id to L during meas request,
                          to support L + L config
08/03/16   vlc/vko MSGR Instance ID to Variant ID change
01/06/16   vko     DO to send LTE_DEINIT_REQ before sending redirection request to LTE
11/04/15   vlc     Do not set TBACKOFF for EARFCNs already included in report. 
10/30/15   vlc     Change TRM duration to sclk unit.  
10/27/15   vlc     Avoid consecutive INIT requests.
10/27/15   vlc     Correct RSRP calculation.  
10/27/15   vlc     Allow for larger TBACKOFF value.
                   Make sure all frequencies with active timers are rejected. 
10/12/15   vlc     Added fixes to avoid providing report with too many entries. 
                   Excluded rampdown duration from measurement duration.
10/09/15   vlc     Added code to handle meas abort before meas request is sent.
10/05/15   vlc     Added debug statement in hdrsrchtclte_sanity_timer_clear(). 
10/01/15   vlc     Reset local copy of TRM grant before Traffic/Idle transition.
09/10/15   vlc     Changed the handling of TBACKOFF and TRESELECT default values. 
                   Added range checks to avoid overflow, added more debug messages. 
07/28/15   vlc     Fixed klockwork issues
07/08/15   vlc     Created supervisory timer for this state. 
06/18/15   vlc     Changed sort parameter to RSRP in report.  Fixed debug messages.
05/28/15   vlc     Created file for DO->LTE pilot measurements/redirection.

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"
#include "customer.h"
#include "hdrutil.h"

#ifdef FEATURE_HDR_TO_LTE_REDIRECTION
#include "hdrsrchdef.h"
#include "hdrsrch.h"
#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchcmd.h"
#include "hdrdebug.h"
#include "hdrsrchltemeas.h"
#include "hdrsrchmsg.h"
#include "hdrsrchrf.h"
#include "hdrsrchmcpm.h"
#include "hdrsrchtclte.h"
#include "hdrsrchset.h"
#include "hdrsrchrftxd.h"
#include "hdrmdsp.h"
#include "fs_public.h"
#include "timetick.h"
#include "lte_cphy_irat_meas_msg.h"
#include "lte_ml1_irat_ext_api.h"
#include "trm.h"
#include "rxlm_intf.h"
#include "sys.h"
#include "lte_rrc_ext_api.h"
#include "lte_ml1_ext_api.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

#define HDRSRCHTCLTE_DURATION_PER_EARFCN_MS        40
  /* Allow 40ms per measurement of 1 LTE EARFCN
    LTE meas overhead is 7ms for RF (included below)    
    LTE meas 13ms to 15ms for FDD
    LTE meas 23ms to 40ms for TDD
  */
#define HDRSRCHTCLTE_MEAS_CYCLE_MAX_DURATION_MS    100
  /* Each meas cycle should be no longer than Max value */
#define HDRSRCHTCLTE_RAMPDOWN_MAX_DURATION_MS      60
  /* Rampdown time not expected to exeed Max value */

#define HDRSRCHTCLTE_LTE_RF_STARTUP_BUFFER_INDEX   1
#define HDRSRCHTCLTE_DO_RF_CLEANUP_BUFFER_INDEX    0
  /* RF startup buffer index is 1 per instruction from LTE L1.
     RF cleanup buffer index should not be 1, and may not be used as DO RF support
     is not based on scripts. */

#define HDRSRCHTCLTE_MEAS_DISCARD_AGE_SEC          12
  /* Once the measurement is older than 12 seconds, it is discarded as it may
     be stale. */

#define HDRSRCHTCLTE_MAX_MIN_RPT_INT_MINUTES       15
#define HDRSRCHTCLTE_DEFAULT_MIN_RPT_INT_MINUTES   1
  /* Default value for MinOtherRATMeasurementReportInterval in unit of minutes
     <snippet from spec>
     If OtherRATRedirectionSupported = 0x02, the AT shall configure a value for
     MinOtherRATMeasurementReportInterval to a value between 0x01 and 0x0F in units
     of minutes. */

#define HDRSRCHTCLTE_MAX_OF_MAX_RPT_CNT            3
  /* Default value for MAX report count
     <snippet from spec>
     If AutonomousReportEnabled is set to '1', the access network shall set
     this field to (MaxReportCount + 1) numbers of autonomous
     OtherRATMeasurementReport message Access Terminal can send;
     otherwise the access network shall omit this field. */

#define HDRSRCHTCLTE_DEFAULT_THRESH_SRV            5
  /* Default value for ServingThresholdTrigger
     <snippet from spec>
     If ServingThresholdTirggerIncluded is set to ‘1’, the AN shall set this field to
     -2*10*Log10PS, where PS is the strength of the reference pilot of the serving
     AN; otherwise, the access network shall omit this field. */

#define HDRSRCHTCLTE_THRESHX_MASK                  0x1F
  /* Mask for Threshx and Threshxq */

#define HDRSRCHTCLTE_THRESH_SERVING_MASK           0x3F
  /* Mask for Thresh Serving */

#define HDRSRCHTCLTE_RPT_LTE_RSRP_MIN              ( -141 )
#define HDRSRCHTCLTE_RPT_RSRP_00                   0
  /* The defines above correspond to RSRP_00 in mapping table for measurement reporting */

#define HDRSRCHTCLTE_RPT_LTE_RSRP_MAX              ( -44 )
#define HDRSRCHTCLTE_RPT_RSRP_97                   97
  /* The defines above correspond to RSRP_97 in mapping table for measurement reporting */
  
#define HDRSRCHTCLTE_RPT_LTE_RSRQ_MIN              ( -40 )
#define HDRSRCHTCLTE_RPT_RSRQ_00                    0
  /* The defines above correspond to RSRQ_00 in mapping table for measurement reporting */

#define HDRSRCHTCLTE_RPT_LTE_RSRQ_MAX              ( -6 )
#define HDRSRCHTCLTE_RPT_RSRQ_34                   34
  /* The defines above correspond to RSRQ_34 in mapping table for measurement reporting */

#define HDRSRCHTCLTE_TRESELECT_DEFAULT_SEC         1
#define HDRSRCHTCLTE_TRESELECT_DEFAULT_MAX_SEC     12
  /* Wait duration between meas if the meas is good, max value provided by system engineering */
          
#define HDRSRCHTCLTE_TBACKOFF_DEFAULT_SEC          60
#define HDRSRCHTCLTE_TBACKOFF_DEFAULT_MAX_SEC      600 
  /* Wait duration between meas if the meas is bad, max value provided by system engineering */

#define HDRSRCHTCLTE_DEFAULT_MEAS_CYCLE_CNT        2
  /* How many measurements DO will make before redirection */

#define HDRSRCHTCLTE_TRESELECT_SEC   "/nv/item_files/modem/hdr/srch/tclte/treselect"
#define HDRSRCHTCLTE_TBACKOFF_SEC    "/nv/item_files/modem/hdr/srch/tclte/tbackoff"
#define HDRSRCHTCLTE_MEAS_CYCLE_CNT "/nv/item_files/modem/hdr/srch/tclte/meascycle"
  /* EFS control information */

#define TIMETICK_MS_FROM_SCLK(sclk) ((sclk)*1000/TIMETICK_NOMINAL_FREQ_HZ)
  /* Conversion macro for time in "ms" to "sclks" */

#define HDRSRCHTCLTE_TRM_NO_GRANT                  0xFF
  /* Indicate DO does not have TRM grant, required because TRM still set output.grant
     to TRM_GRANTED even after DO sends END_STATE to TRM. */

#define HDRSRCHTCLTE_DURATION_LTE_DEINIT_SANITY_MS    50
/* Sanity timer of 50ms for LTE DEINIT confirmation in INACTIVE state
   */

/* Request and command messages (via MSGR) from DO to LTE */
typedef enum
{
  HDRSRCHTCLTE_NULL_CMD,            /* 0 */
  HDRSRCHTCLTE_INIT_REQ,            /* 1 */
  HDRSRCHTCLTE_TIMED_SRCH_MEAS_REQ, /* 2 */
  HDRSRCHTCLTE_DEINIT_REQ,          /* 3 */
  HDRSRCHTCLTE_ABORT_REQ,           /* 4 */
} 
hdrsrchtclte_req_enum_type;

/* Confirmation messages (via MSGR) from LTE to DO */
typedef enum
{
  HDRSRCHTCLTE_NULL_CNF,            /* 0 */
  HDRSRCHTCLTE_INIT_CNF,            /* 1 */
  HDRSRCHTCLTE_TIMED_SRCH_MEAS_CNF, /* 2 */ 
  HDRSRCHTCLTE_DEINIT_CNF,          /* 3 */ 
  HDRSRCHTCLTE_ABORT_CNF,           /* 4 */ 
} 
hdrsrchmsg_lte_cnf_enum_type;

/* These states are tied directly to the LTE command/response
   interface for pilot measurements. */
typedef enum
{
  HDRSRCHTCLTE_INACTIVE_STATE,          /* 0 No activity                  */
  HDRSRCHTCLTE_INIT_STATE,              /* 1 Wait for Init cnf            */
  HDRSRCHTCLTE_TIMED_SRCH_MEAS_STATE,   /* 2 Wait for Timed srch meas cnf */
  HDRSRCHTCLTE_DEINIT_STATE,            /* 3 Wait for Deinit cnf          */
  HDRSRCHTCLTE_ABORT_STATE,             /* 4 Wait for Abort cnf           */
} 
hdrsrchtclte_state_enum_type;

/* DO needs to know the states of the Init request to make sure
   LTE Init/Deinit processing is done correctly. */
typedef enum
{
  HDRSRCHTCLTE_LTEINIT_NONE,     /* 0 Need to send LTE Init */
  HDRSRCHTCLTE_LTEINIT_SENT,     /* 1 Wait for Init cnf     */
  HDRSRCHTCLTE_LTEINIT_RXED,     /* 2 Init cnf received     */
  HDRSRCHTCLTE_LTEDEINIT_SENT,   /* 3 Wait for Deinit cnf   */
} 
hdrsrchtclte_lteinit_enum_type;

/* Indicates which stage of measurements we are in. */
typedef enum
{
  HDRSRCHTCLTE_GOOD_MEAS_NONE,   /* No measurement exceeds threshold */
  HDRSRCHTCLTE_GOOD_MEAS_1ST,    /* 1st measurement exceeds threshold */
  HDRSRCHTCLTE_GOOD_MEAS_2ND,    /* 2nd measurement exceed threshold, start attempt to redirect */
} 
hdrsrchtclte_good_meas_enum_type;

/* Used to process Treselect and Tbackoff timers.
   - When Treselect timer expires, one additional pilot measurement is required 
     to send report to network.
   - When Tbackoff timer expires, the frequency can be measured again. */
typedef enum
{
  HDRSRCHTCLTE_TIMER_INACTIVE,   /* No timer is active for this frequency */
  HDRSRCHTCLTE_TIMER_ACTIVE,     /* Either Treselect or Tbackoff timer is active */
}
hdrsrchtclte_timer_enum_type;

/* Struct type to save command parameters */
typedef struct
{
  hdrsrch_lte_nset_update_redir_params_type    redir_params;
    /* Save a copy of the LTE NSET Update Redirection parameters as received by Protocol layer */

  hdrsrch_lte_timed_srch_meas_cnf_params_type  timed_srch_meas_cnf_params;
    /* Save a copy of the LTE Meas Confirmation parameters from LTE ML1 layer */
}
hdrsrchtclte_saved_params_struct_type;

/* Struct type to store each LTE frequency */
typedef struct
{
  lte_earfcn_t                                 earfcn;
    /* LTE frequency  */

  uint8                                        backoff_cnt;
    /* Backoff count to avoid searching for LTE frequency that has no energy  */

  trm_frequency_info                           earfcn_trm_info;
    /* LTE band corresponding to EARFCN */

  boolean                                      lte_band_is_valid;
    /* TRUE if LTE band is valid */

  trm_set_client_state_input_type              trm_set_client_state_input;
  trm_set_client_state_output_data             trm_set_client_state_output;
    /* Save away parameters for TRM set_client_state API */

  hdrsrchtclte_good_meas_enum_type             good_meas_val;
    /* Indicate if any measurement passes the threshold test */

  timetick_type                                good_meas_timestamp_sclks;
    /* Age of the good measurement in sclks units */

  hdrsrchtclte_timer_enum_type                 treselect_state;
    /* Set if measurement result exceeds threshold,
       one more measurement is required to send report */

  timetick_type                                treselect_exp_sclks;
    /* Treselect timer expiry value in units of sclks for the 2nd measurement */

  hdrsrchtclte_timer_enum_type                 tbackoff_state;
    /* Set if measurement result is lower than threshold,
       freq is not measured again until timer expires */

  timetick_type                                tbackoff_exp_sclks;
    /* Treselect timer expiry time in units of sclks, when the EARFCN can be measured again */
  uint16                                       tbackoff_duration_sec;
    /*Backoff Duration in sec per Earfcn*/
}
hdrsrchtclte_earfcn_struct_type;

/* Struct type to store LTE frequency array.  This data comes from L3 in
   HDRSRCH_LTE_NSET_UPDATE_REDIR_CMD. */
typedef struct
{
  hdrsrchtclte_earfcn_struct_type              arr[ HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ];
    /* Array of LTE frequency  */

  uint8                                        arr_cnt;
    /* Array count */

  uint8                                        arr_idx ;
    /* Current entry index containing LTE frequency to be measured */
}
hdrsrchtclte_earfcn_arr_struct_type;

/* Struct type to store actual reporting parameters */
typedef struct
{
  uint8                                        max_rpt_cnt;
    /* Maximum report count */

  uint16                                       thresh_serving;
    /* DO serving threshold */

  int16                                        threshxq;
    /* LTE pilot measurement threshold (RSRQ) */

  int16                                        threshx;
    /* LTE pilot measurement threshold (RSRP) */

  uint8                                        min_rpt_int_minutes;
    /* Minimum report interval in unit of minutes */

}
hdrsrchtclte_actual_rpt_params_struct_type;

/* Struct type to store information for the redirection report */
typedef struct
{
  uint8                                        rpt_cnt;
    /* Current report count value */

  boolean                                      prep_to_send_rpt;
    /* Set to true if measurements pass threshold set for redirection */

  hdrsrchtclte_rpt_struct_type                 redirect_rpt;
    /* Redirection report */

  timetick_type                                rpt_timestamp_sclks;
    /* Age of the last report in sclks units */

}
hdrsrchtclte_rpt_info_struct_type;

/* Struct type to store LTE redirection control information */
typedef struct
{
  hdrsrchtclte_earfcn_arr_struct_type          earfcn_arr;
    /* LTE neighbor information */

  hdrsrchtclte_lteinit_enum_type               init_cnf_ind;
    /* Status of LTE INIT/DEINIT */

  hdrsrchtclte_state_enum_type                 redir_state;
    /* Current state of pilot measurement request */

  hdrsrchltemeas_rxlm_struct_type              rxlm;
    /* RXLM buffers for RF tune scripts, shared with D2L reselection */

  lte_cphy_irat_meas_startup_rf_params_s       rf_params;
    /* Parameters for RF tune scripts */

  lte_ml1_irat_gap_schedule_gap_state_s        gap_state;
    /* Required by LTE, not used by DO */

  lte_ml1_irat_gap_scheduled_info_s            gap_sched;
    /* Required by LTE, not used by DO */

  hdrsrchtclte_req_enum_type                   last_sent;
    /* Last message sent to LTE */

  hdrsrch_cmd_name_enum_type                   last_cmd;
    /* Last CMD received in LTE meas state */

  hdrsrchmsg_lte_cnf_enum_type                 last_cnf;
    /* Last LTE confirmation message received in LTE meas state */

  hdrsrchtclte_earfcn_struct_type              *curr_freq_entry_ptr;
    /* The LTE frequency to measure */

  hdrsrchtclte_earfcn_struct_type              *prev_freq_entry_ptr;
    /* The LTE frequency measured in the previous meas cycle */

  hdrsrchtclte_earfcn_struct_type              *rfm_meas_ptr;
    /* To use in rfm_meas_enter and exit.  If we call rfm_meas_enter,
       must use the same ptr for rfm_meas_exit.  This may be different
       than curr_freq_entry_ptr.  When we switch to the new RFM meas
       interface, this may not be needed. */

  hdrsrchtclte_actual_rpt_params_struct_type   actual_rpt_params;
    /* Actual reporting parameters */

  hdrsrch_lte_redirect_cb_type                 redirect_cb;
    /* Call back to notify upper layer of LTE reselection */

  hdrsrchtclte_rpt_info_struct_type            rpt;
    /* Information on redirection report */

  uint16                                       avail_ms;
    /* Time available for LTE meas (in ms) */

  uint16                                       rampdown_ms;
    /* Overhead time for HDR ramp down (in ms) */

  timetick_type                                meas_end_time_sclks;
    /* End of LTE meas time (in sclks) */

  boolean                                      found_good_cell;
    /* Found at least 1 good LTE cell */

  boolean                                      update_list_ind;
    /* Indicate if the LTE frequency list needs to be updated */

  uint8                                        treselect_duration_sec;
    /* Duration of treselect timer in seconds */

  uint16                                       tbackoff_duration_sec;
    /* Duration of tbackoff timer in seconds */

  uint8                                        meas_cycle_cnt;
    /* How many measurement cycles to go through before redirection */

#ifdef FEATURE_HDR_D2L_RF_MEAS
  lte_cphy_irat_meas_rf_param_s                lte_param;
    /* RF parameters */

  rfm_meas_common_param_type                   common_param;
    /* Common RF parameters among all technologies */

  rfm_meas_hdr_params_type                     hdr_param;
    /* HDR RF parameters */

  boolean                                      is_rfm_meas_entered;
    /* Flag indicating whether rfm_meas_enter is completed */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

  timer_type                                   sanity_timer;
    /* Define the D2L sanity timer to expire if LTE takes longer than expected.
       This way, DO traffic can resume. */

  uint8                                        earfcn_meas_this_cycle_cnt;
    /* Count of how many EARFCNs DO measure this cycle */
}
hdrsrchtclte_struct_type;

hdrsrchtclte_struct_type                       hdrsrchtclte;
  /* LTE redirection control information */

hdrsrchtclte_saved_params_struct_type          hdrsrchtclte_saved_params;
  /* Saved copy for debugging purposes */

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

void hdrsrchtclte_init( hdrsrch_state_enum_type last_state );
void hdrsrchtclte_done( hdrsrch_state_enum_type next_state );
errno_enum_type hdrsrchtclte_cmd
(
  const hdrsrch_cmd_type                *cmd
);
void hdrsrchtclte_process_low_pri_rsp_ind
(
  hdrsrchmsg_s_type                     *msg
);
void hdrsrchtclte_exit_tclte_state( void );
void hdrsrchtclte_get_lte_freq_trm
( 
  boolean                                      first_meas_this_cycle
    /* First measurement in meas cycle */
);
errno_enum_type hdrsrchtclte_abort_lte_meas( void );
void hdrsrchtclte_lte_nset_cmd_update ( void );

/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/

hdrsrch_state_struct_type hdrsrchtclte_state =
{
  HDRSRCH_LTE_MEAS_TC_STATE,
  hdrsrchtclte_init,
  hdrsrchtclte_done,
  hdrsrchtclte_cmd,
  hdrsrch_default_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrch_default_pri_chain_event,
  hdrsrch_process_int_cmd_default,
  hdrsrchtclte_process_low_pri_rsp_ind
};

/*===========================================================================

                    FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SANITY_TIMER_SET

DESCRIPTION 
  This function sets the sanity timer.
  The sanity timer is used to keep track of time availale for       
  measurement. If the measurements are still going on when measurement time
  is used up, then abort and return.
 
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_sanity_timer_set
(
  uint32   duration_ms /* timer duration */
)                                 

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "DLR:sanity_timer_set() duration_ms=%d",
                  duration_ms );

  /* Set timer for the LTE meas duration in ms */
  ( void ) timer_set( &hdrsrchtclte.sanity_timer,
                      ( timetick_type ) duration_ms,
                      0,
                      T_MSEC );
                     
} /* hdrsrchtclte_sanity_timer_set */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SANITY_TIMER_CLEAR

DESCRIPTION 
  This function clears the sanity timer.
  The sanity timer is used to keep track of time availale for       
  measurement. If the measurements are still going on when measurement time
  is used up, then abort and return.
 
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_sanity_timer_clear ( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "DLR:sanity_timer_clear(), state=%x",
                  hdrsrch_state_id );

  /* Clear timer */
  ( void ) timer_clr( &hdrsrchtclte.sanity_timer, T_NONE );

} /* hdrsrchtclte_sanity_timer_clear */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SANITY_TIMER_EXP_CB
 
DESCRIPTION
  This function is called when the sanity timer expires.            
  The sanity timer is used to keep track of time availale for       
  measurement. If the measurements are still going on at this point 
  of time, then abort and return.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_sanity_timer_exp_cb
(
  timer_cb_data_type              unused_parm
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ( void )unused_parm;
    /* Unused parameters - avoid compiler warning */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "DLR:sanity_timer_exp_cb(), state=%d",
                  hdrsrch_state_id );

  /* Sanity timer expires, abort to resume DO traffic.
     NOTE: If we are already in ABORT / DEINIT states, ignore the timeout, this is
     taken care of in the abort function. */
  if ( hdrsrch_state_id == HDRSRCH_LTE_MEAS_TC_STATE ) 
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"DLR:!!!D2LR Sanity timer expires,abort LTE meas!!!" );
    ( void ) hdrsrchtclte_abort_lte_meas();
  }
  else if ( hdrsrch_state_id == HDRSRCH_INACTIVE_STATE )
  {
	HDR_MSG_SRCH( MSG_LEGACY_ERROR,
	  	"DLR:!!!D2LR Sanity timer expired, after sending DEINIT REQUEST to LTE !!!" );

    ERR_FATAL( "D2LR Sanity timer expired: D2L REDIR state %d",
                hdrsrchtclte.redir_state,
                0,
                0 );	
  }

} /* hdrsrchtclte_sanity_timer_exp_cb */

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
void hdrsrchtclte_powerup_init( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ( void ) timer_def( &hdrsrchtclte.sanity_timer, 
                      NULL,
                      HDRSRCH_TCB,
                      0, 
                      hdrsrchtclte_sanity_timer_exp_cb, 
                      0 );
    /* Define the D2L sanity timer to expire if LTE meas takes longer than expected.
       This way, meas can be aborted and DO traffic can resume. */

} /* hdrsrchtclte_powerup_init */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_RFM_MEAS_ENTER 
 
DESCRIPTION 
  Prepare RF configuration to start LTE measurement. This function is called
  before LTE measurement.  It requires DO and LTE RXLM buffers.

DEPENDENCIES
  Each Enter_meas must be terminated by an Exit_meas after the measurement is complete.
 
PARAMETERS
  None
 
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_HDR_D2L_RF_MEAS
void hdrsrchtclte_rfm_meas_enter( void )
{
  uint8                                   chan_cnt;
    /* The number of channels coverred by the current RF config */

  const hdrsrch_demod_idx_type            *demod_list_ptr;
    /* A pointer to the demod index assignment from hdrsrchrf. */

  hdrsrchtclte_earfcn_struct_type         *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */

  lte_cphy_irat_meas_rf_param_s           *lte_ptr;
  rfm_meas_common_param_type              *rf_common_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rf_common_ptr = &( hdrsrchtclte.common_param );
  rfm_meas_common_init_params( rf_common_ptr );
    /* Initialize RF common parameters */

  /* Allocate RXLM buffers for DO. */
  hdrsrchtclte.rxlm.do_handle_valid = 
    hdrsrchrf_get_rxlm_buffer_handle( 
      HDRSRCHRF_PRI_CHAIN,
      &hdrsrchtclte.rxlm.do_buf_chain0 ) ;

  /* Allocate RXLM buffers for LTE RF tuning.  LTE always uses both chains. */
  if ( hdrsrchtclte.rxlm.lte_handle_valid != TRUE )
  {
    rxlm_allocate_buffer( RXLM_CHAIN_0, LM_LTE,
                          &hdrsrchtclte.rxlm.lte_buf_chain0 );
    rxlm_allocate_buffer( RXLM_CHAIN_1, LM_LTE,
                          &hdrsrchtclte.rxlm.lte_buf_chain1 );

    hdrsrchtclte.rxlm.lte_handle_valid = TRUE;
  }

  HDR_ASSERT ( ( hdrsrchtclte.rxlm.do_handle_valid == TRUE ) &&
          ( hdrsrchtclte.rxlm.lte_handle_valid == TRUE ));

  /* DO header parameters */
  rf_common_ptr->header.source_tech       = RFCOM_1XEVDO_MODE;
  rf_common_ptr->header.num_source_item   = 1;
  rf_common_ptr->header.source_param_v2[ 0 ].band = ( uint32 )
    hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].device =
    hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].lm_handle = 
    hdrsrchtclte.rxlm.do_buf_chain0;
     /* Scenario Type */
  rf_common_ptr->scenario_type = RFM_MEAS_SCENARIO_DO2L_CONNECTED;
  /* DO parameters */
  rf_common_ptr->source_param.hdr_params.band_chan.chan_num =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->chan_num;
  rf_common_ptr->source_param.hdr_params.band_chan.band =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;

  freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Temp pointer for convenience */

  /* LTE parameters */
  lte_ptr = &hdrsrchtclte.lte_param;

  lte_ptr->common = rf_common_ptr;
  lte_ptr->flags.is_gap_aborted = FALSE;
  lte_ptr->target.device = 
    freq_entry_ptr->trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_ptr->target.div_device =
    freq_entry_ptr->trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].diversity_device;
  lte_ptr->target.rxlm_buf_idx_ant0 = 
    hdrsrchtclte.rxlm.lte_buf_chain0;
  lte_ptr->target.rxlm_buf_idx_ant1 = 
    hdrsrchtclte.rxlm.lte_buf_chain1;
  lte_ptr->target.earfcn = freq_entry_ptr->earfcn;

  lte_ml1_irat_x2l_meas_enter (
#ifdef FEATURE_HDR_MSGR_VARIANT 
     hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
     lte_ptr );

  hdrsrchtclte.is_rfm_meas_entered = TRUE;
  /* Update flag to indicate rfm_meas_enter done. Check this flag and call
     rfm_meas_exit for all abort scenarios */  
  
} /* hdrsrchtclte_rfm_meas_enter */
#else
errno_enum_type hdrsrchtclte_rfm_meas_enter( void )
{

  static rfm_meas_enter_param_type        rf_meas_enter;
    /* Parameter for RF to enter LTE measurement state */
  rfm_meas_result_type                    rf_result;
    /* Status returned from RF */
  static rfm_meas_lte_enter_param_type    lte_enter_param;
    /* LTE enter param */
  static rfm_meas_hdr_enter_param_type    do_enter_param;
    /* DO enter param */
  errno_enum_type                         status = E_SUCCESS;
    /* Report error if any */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchtclte.curr_freq_entry_ptr == NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"DLR:Null curr_freq_entry_ptr, return" );
    status = E_FAILURE;
    return status;
      /* Unexpected condition */
  }

  hdrsrchtclte.rfm_meas_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Save current ptr to use for rfm_meas_enter and rfm_meas_exit */

  /* Allocate RXLM buffers for LTE RF tuning.  LTE always uses both chains. */
  if ( hdrsrchtclte.rxlm.lte_handle_valid != TRUE )
  {
    rxlm_allocate_buffer( RXLM_CHAIN_0, LM_LTE,
                          &hdrsrchtclte.rxlm.lte_buf_chain0 );
    rxlm_allocate_buffer( RXLM_CHAIN_1, LM_LTE,
                          &hdrsrchtclte.rxlm.lte_buf_chain1 );
    hdrsrchtclte.rxlm.lte_handle_valid = TRUE;
  }

  hdrsrchtclte.rxlm.do_handle_valid = 
    hdrsrchrf_get_rxlm_buffer_handle( HDRSRCHRF_PRI_CHAIN, &hdrsrchtclte.rxlm.do_buf_chain0 ) ;
    /* Get DO RXLM buffer handle, which should have been allocated at this
       point */

  rf_meas_enter.header.source_tech  = RFCOM_1XEVDO_MODE;
  rf_meas_enter.header.target_tech  = RFCOM_LTE_MODE;

  lte_enter_param.bw                = RFCOM_BW_LTE_10MHz;
  lte_enter_param.device            = 
    hdrsrchtclte.rfm_meas_ptr->trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_enter_param.rx_channel        = hdrsrchtclte.rfm_meas_ptr->earfcn; 

  do_enter_param.device             = hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );
  do_enter_param.band_chan.band     = SYS_BAND_BC0;
  do_enter_param.band_chan.chan_num = 690;
  do_enter_param.rxlm_buf_index     = hdrsrchtclte.rxlm.do_buf_chain0 ;
    /* Use default values for DO params, except for device */

  rf_meas_enter.source_param        = &do_enter_param;
  rf_meas_enter.target_param        = &lte_enter_param;

  HDR_MSG_SRCH_5( MSG_LEGACY_MED, "DLR:rfm_meas_enter,DO device=%d,band=%d ch=%d|LTE device=%d,EARFCN=%d",
                  do_enter_param.device, 
                  do_enter_param.band_chan.band,
                  do_enter_param.band_chan.chan_num,
                  lte_enter_param.device,
                  hdrsrchtclte.rfm_meas_ptr->earfcn
                );

  rf_result = rfm_meas_common_script_enter( &rf_meas_enter, NULL );
    /* Notify RF of LTE measurement */

  if (rf_result != RFM_MEAS_COMMON_SUCCESS)
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Failed rfm_meas_common_script_enter(),result=%d",
                    rf_result );
    status = E_FAILURE;
  }

  return status;

} /* hdrsrchtclte_rfm_meas_enter */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_RFM_MEAS_EXIT 
 
DESCRIPTION 
  This function is called after LTE measurement is finished.  It resets RF
  state back to non-IRAT measurement configuration.

DEPENDENCIES
  Each Enter_meas must be terminated by an Exit_meas after the measurement is complete.
 
PARAMETERS
  None
 
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_HDR_D2L_RF_MEAS
void hdrsrchtclte_rfm_meas_exit( void )
{

  uint8                                   chan_cnt;
    /* The number of channels coverred by the current RF config */

  const hdrsrch_demod_idx_type            *demod_list_ptr;
    /* A pointer to the demod index assignment from hdrsrchrf. */

  hdrsrchtclte_earfcn_struct_type         *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */

  lte_cphy_irat_meas_rf_param_s           *lte_ptr;
  rfm_meas_common_param_type              *rf_common_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_ASSERT ( ( hdrsrchtclte.rxlm.do_handle_valid == TRUE ) &&
          ( hdrsrchtclte.rxlm.lte_handle_valid == TRUE ));

  /* DO header parameters */
  rf_common_ptr = &( hdrsrchtclte.common_param );

  rf_common_ptr->header.source_tech       = RFCOM_1XEVDO_MODE;
  rf_common_ptr->header.num_source_item   = 1;
  rf_common_ptr->header.source_param_v2[ 0 ].band = ( uint32 )
    hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].device =
    hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].lm_handle = 
    hdrsrchtclte.rxlm.do_buf_chain0;

  /* DO parameters */
  rf_common_ptr->source_param.hdr_params.band_chan.chan_num =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->chan_num;
  rf_common_ptr->source_param.hdr_params.band_chan.band =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;

  freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Temp pointer for convenience */

  /* LTE parameters */
  lte_ptr = &hdrsrchtclte.lte_param;

  lte_ptr->common = rf_common_ptr;
  lte_ptr->flags.is_gap_aborted = FALSE;
  lte_ptr->target.device = 
    freq_entry_ptr->trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_ptr->target.div_device =
    freq_entry_ptr->trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].diversity_device;
  lte_ptr->target.rxlm_buf_idx_ant0 = 
    hdrsrchtclte.rxlm.lte_buf_chain0;
  lte_ptr->target.rxlm_buf_idx_ant1 = 
    hdrsrchtclte.rxlm.lte_buf_chain1;
  lte_ptr->target.earfcn = freq_entry_ptr->earfcn;

  lte_ml1_irat_x2l_meas_exit ( 
#ifdef FEATURE_HDR_MSGR_VARIANT 
     hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
     lte_ptr );

  hdrsrchtclte.is_rfm_meas_entered = FALSE;
    /* Update flag to indicate rfm_meas_exit done */

} /* hdrsrchtclte_rfm_meas_exit */
#else
void hdrsrchtclte_rfm_meas_exit( void )
{

  rfm_meas_exit_param_type         rf_meas_exit;
    /* Parameter for RF to tune back */
  rfm_meas_result_type             rf_result;
    /* Tune back result */
  rfm_meas_lte_exit_param_type     lte_exit_param;
    /* LTE exit param */
  rfm_meas_hdr_exit_param_type     do_exit_param;
    /* DO exit param */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchtclte.rfm_meas_ptr == NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"DLR:Null rfm_meas_ptr, return" );
    return;
      /* Unexpected condition */
  }

  rf_meas_exit.header.source_tech   = RFCOM_1XEVDO_MODE;
  rf_meas_exit.header.target_tech   = RFCOM_LTE_MODE;

  lte_exit_param.bw                 = RFCOM_BW_LTE_10MHz;
  lte_exit_param.device             = 
    hdrsrchtclte.rfm_meas_ptr->trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_exit_param.rx_channel         = hdrsrchtclte.rfm_meas_ptr->earfcn; 
  lte_exit_param.buffer_index       = HDRSRCHTCLTE_LTE_RF_STARTUP_BUFFER_INDEX;
  lte_exit_param.tgt_rxlm_handles.drx_rxlm_handle = 0;
  lte_exit_param.tgt_rxlm_handles.prx_rxlm_handle = 0;
    /* Use default values for LTE params, except for device */

  do_exit_param.device              = hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );
  do_exit_param.band_chan.band      = SYS_BAND_BC0;
  do_exit_param.band_chan.chan_num  = 690;
  do_exit_param.rxlm_buf_index      = hdrsrchtclte.rxlm.do_buf_chain0 ;
    /* Use default values for DO params, except for device */

  rf_meas_exit.source_param         = &do_exit_param;
  rf_meas_exit.target_param         = &lte_exit_param;

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR:rfm_meas_exit(), DO device=%d,LTE device=%d",
                  do_exit_param.device,
                  lte_exit_param.device );

  rf_result = rfm_meas_common_script_exit( &rf_meas_exit, NULL );
    /* Call RF tune back function after LTE measurements. */

  if (rf_result != RFM_MEAS_COMMON_SUCCESS)
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Failed rfm_meas_common_script_exit(),result=%d",
                    rf_result );
  }

  hdrsrchtclte.rfm_meas_ptr = NULL;
    /* Reset, it will be set to !=NULL once rfm_meas_enter() is called */

} /* hdrsrchtclte_rfm_meas_exit */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_RESET_STATE 
 
DESCRIPTION 
  This function resets LTE measurement state when LTE measurements are done
  and LTE deinit confirmation has been received by DO.

DEPENDENCIES
  None
 
PARAMETERS
  None
 
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_reset_state( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset( &hdrsrchtclte, 0, sizeof( hdrsrchtclte ) );
    /* Initialize LTE redirection control information */

  memset( &hdrsrchtclte_saved_params, 0, sizeof( hdrsrchtclte_saved_params ) );
    /* Force an update of the LTE NSET update command when protocol layer sends it by
       clearing the local copy of the command.  This is necessary
       because a bit exact match between the new command
       and the local saved copy causes the update to get ignored. */
  
} /* hdrsrchtclte_reset_state */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SEND_TIMED_SRCH_MEAS_REQ

DESCRIPTION
  This function initiates another LTE frequency measurement by sending the
  TIMED_SRCH_MEAS_REQ message to LTE.  This LTE request combines both LTE
  SEARCH and LTE MEAS operations.  DO always requests these operations together.
  
DEPENDENCIES
  None

PARAMETERS
  None
 
RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/
errno_enum_type hdrsrchtclte_send_timed_srch_meas_req( void ) 

{

  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:send_timed_srch_meas_req(),meas_cnt=%d",
                  hdrsrchtclte.earfcn_meas_this_cycle_cnt );

#ifdef FEATURE_HDR_D2L_RF_MEAS
  hdrsrchtclte_rfm_meas_enter();
      /* Allows LTE to set up Enter meas parameters */

  /* Send timed search and meas req message, resume after confirmation is received. */
  status = hdrsrchmsg_d2l_send_timed_search_meas_req_msg
             ( hdrsrchtclte.curr_freq_entry_ptr->earfcn,
               HDRSRCHMSG_BW_NRB_50, /* Default value is 10MHz, parameter not given in OTA message */
               &( hdrsrchtclte.curr_freq_entry_ptr->trm_set_client_state_output ),
               &( hdrsrchtclte.lte_param ) );
#else
  /* Send timed search and meas req message, resume after confirmation is received. */
  status = hdrsrchmsg_d2l_send_timed_search_meas_req_msg
             ( hdrsrchtclte.curr_freq_entry_ptr->earfcn,
               HDRSRCHMSG_BW_NRB_50, /* Default value is 10MHz, parameter not given in OTA message */
               &( hdrsrchtclte.curr_freq_entry_ptr->trm_set_client_state_output ),
               &( hdrsrchtclte.rf_params ) );
#endif

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "DLR:Timed_search_meas_req failed" );
    return status;
  }

  hdrsrchtclte.last_sent = HDRSRCHTCLTE_TIMED_SRCH_MEAS_REQ;
  hdrsrchtclte.redir_state = HDRSRCHTCLTE_TIMED_SRCH_MEAS_STATE;

  /* Reset backoff and reselect states as we are measuring this EARFCN to remove stale information. */
  hdrsrchtclte.curr_freq_entry_ptr->tbackoff_state  = HDRSRCHTCLTE_TIMER_INACTIVE;
  hdrsrchtclte.curr_freq_entry_ptr->treselect_state = HDRSRCHTCLTE_TIMER_INACTIVE;

  return status;

} /* hdrsrchtclte_send_timed_srch_meas_req */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_START_LTE_MEAS

DESCRIPTION
  This function initiates the first LTE frequency measurement after DO
  enters LTE meas state.  TRM extension and LTE frequency information are
  expected to be available at this time.
  
DEPENDENCIES
  TRM extension and LTE frequency information are available.

PARAMETERS
  None
 
RETURN VALUE
  None

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_start_lte_meas( void ) 

{

  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Error checking */
  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN to start meas" );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
    return;
  }

  /* Send timed search and meas req message, resume after confirmation is received. */
  status = hdrsrchtclte_send_timed_srch_meas_req();

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "DLR:Timed_search_meas_req failed" );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
  }

} /* hdrsrchtclte_start_lte_meas */

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
  lte_cphy_irat_meas_timed_srch_meas_req_s     *timed_srch_meas_ptr
)
{

  errno_enum_type                              status = E_FAILURE;
    /* Command completion status */
  lte_ml1_irat_gap_schedule_return_e           lte_ret_val;
    /* Return value from scheduling function */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchtclte.gap_sched.gap_length_config_for_x2l_in_us = 0;
  hdrsrchtclte.gap_sched.move_to_next_freq = FALSE;

  lte_ret_val = lte_ml1_irat_setup_schedule (
#ifdef FEATURE_HDR_MSGR_VARIANT 
     hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
     LTE_ML1_IRAT_GAP_SCHEDULE_HINT_SEARCH_AND_MEAS,
     timed_srch_meas_ptr,
     &hdrsrchtclte.gap_state,
     &hdrsrchtclte.gap_sched,
     timed_srch_meas_ptr->freq );

  if( lte_ret_val == LTE_ML1_IRAT_GAP_SUCCESS )
  {
    status = E_SUCCESS;
  }

  return status;

} /* hdrsrchtclte_schedule_timed_search */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_BUILD_RF_TUNEBACK_SCRIPT

DESCRIPTION 
  This function adheres to the convention of using RF Scripts for D2L measurements.
  This function provides DO RF parameters for the tune back portion.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_build_rf_tuneback_script( void ) 

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Use default parameters for DO tune back scripts, as DO RF does not use
     tune back script */
  hdrsrchtclte.rf_params.startup_rf_idx = 
    HDRSRCHTCLTE_LTE_RF_STARTUP_BUFFER_INDEX;
  hdrsrchtclte.rf_params.cleanup_rf_index = 
    HDRSRCHTCLTE_DO_RF_CLEANUP_BUFFER_INDEX;
  hdrsrchtclte.rf_params.lte_rxlm_rx_buffer_ant0 =
    hdrsrchtclte.rxlm.lte_buf_chain0;
  hdrsrchtclte.rf_params.lte_rxlm_rx_buffer_ant1 = 
    hdrsrchtclte.rxlm.lte_buf_chain1;
  hdrsrchtclte.rf_params.source_rat_rxlm_rx_buffer_ant0 =
    hdrsrchtclte.rxlm.do_buf_chain0;
  hdrsrchtclte.rf_params.source_parameters.hdr_params.band =
    SYS_BAND_BC0;
  hdrsrchtclte.rf_params.source_parameters.hdr_params.chan = 690;
  hdrsrchtclte.rf_params.source_parameters.hdr_params.agc_mode = 
    ( lte_cphy_irat_meas_hdr_agc_mode_e ) HDRSRCHRF_NORMAL_AGC_ACQ_MODE;
  hdrsrchtclte.rf_params.source_parameters.hdr_params.agc_rtc_offset = 0;
  hdrsrchtclte.rf_params.source_parameters.hdr_params.prev_agc = 0;
  hdrsrchtclte.rf_params.source_parameters.hdr_params.device = 
    hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );

} /* hdrsrchtclte_build_rf_tuneback_script */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_END_LTE_MEAS

DESCRIPTION 
  This function cleans up after every LTE measurement request, including
  TRM notification of the completed measurement.  This function is called
  per LTE frequency. 
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_end_lte_meas( void ) 

{
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  static trm_set_client_state_input_type     trm_set_client_state_input;
  static trm_set_client_state_output_data    trm_set_client_state_output;
    /* Parameters for TRM set_client_state API */
     
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Error checking */
  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    return;
  }

  /* Avoid calling TRM with TRM_END_STATE if request was not granted */
  if ( hdrsrchtclte.curr_freq_entry_ptr->trm_set_client_state_output.grant != TRM_GRANTED )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Not granted, do not send TRM_END_STATE" );
    return;
  }

  /* Avoid calling TRM with TRM_END_STATE multiple times in abort scenarios */
  if ( hdrsrchtclte.curr_freq_entry_ptr->trm_set_client_state_input.data.irat_data.operation == TRM_END_STATE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Already sent TRM_END_STATE" );
    return;
  }

  freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Temp pointer for convenience */

  memset( &trm_set_client_state_input,0,sizeof ( trm_set_client_state_input ) );
  memset( &trm_set_client_state_output,0,sizeof ( trm_set_client_state_output ) );

  trm_set_client_state_input = freq_entry_ptr->trm_set_client_state_input;
    /* Copy over what was saved for TRM set_client_state API */
  
  trm_set_client_state_input.data.irat_data.operation = TRM_END_STATE;
  trm_set_client_state_input.data.irat_data.transaction_id = 
    freq_entry_ptr->trm_set_client_state_output.transaction_id;
    /* Override operation with END_STATE and the transaction ID provided by TRM from the
       transaction start request */

  trm_set_client_state( &trm_set_client_state_input, &trm_set_client_state_output );
    /* Call trm_set_client_state */

  freq_entry_ptr->trm_set_client_state_input = trm_set_client_state_input;
  freq_entry_ptr->trm_set_client_state_output = trm_set_client_state_output;
    /* Save parameters for TRM set_client_state API */
  
  HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "DLR:TRM END_STATE for LTE band=%d/earfcn=%d,output.grant=%d,input.tID=%d,output.tID=%d",
                  freq_entry_ptr->earfcn_trm_info.band,
                  freq_entry_ptr->earfcn,
                  trm_set_client_state_output.grant,
                  trm_set_client_state_input.data.irat_data.transaction_id,
                  trm_set_client_state_output.transaction_id
                );

  freq_entry_ptr->trm_set_client_state_output.grant = HDRSRCHTCLTE_TRM_NO_GRANT;
    /* Even after sending TRM_END_STATE, TRM returns output.grant == TRM_GRANTED.
       Make sure to reset DO's local copy to reflect output.grant not valid */

  HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "DLR:local TRM END_STATE for LTE band=%d/earfcn=%d,output.grant=%d,input.tID=%d,output.tID=%d",
                  freq_entry_ptr->earfcn_trm_info.band,
                  freq_entry_ptr->earfcn,
                  freq_entry_ptr->trm_set_client_state_output.grant,
                  freq_entry_ptr->trm_set_client_state_input.data.irat_data.transaction_id,
                  freq_entry_ptr->trm_set_client_state_output.transaction_id
                );

} /* hdrsrchtclte_end_lte_meas */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_DONE

DESCRIPTION 
  This function is called when exiting from LTE MEAS state.   
  Before leaving LTE MEAS state, turn off RF.
 
DEPENDENCIES 
  None 

PARAMETERS 
  Next DO state

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_done
( 
  hdrsrch_state_enum_type next_state 
)

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:done(),call RF functions sleep_rx() then disable_rx()" );

  hdrsrchrf_prep_to_sleep();
    /* Prepare the RF for sleep. The AGC is stopped and the 
       DC offsets are saved. */

  hdrsrchrf_disable_rx( HDRSRCHRF_PRI_CHAIN );
  /* Disable rx */

  hdrsrch_default_done( next_state );
    /* Use default handling */

  /* If current state is 4C and next state is not any of the traffic states 
  then update APS state information */
  if( next_state == HDRSRCH_INACTIVE_STATE )
  {
#ifdef FEATURE_HDR_QTA
    hdrsrchrf_set_aps_client_state( HDRSRCHRF_TRM_END_STATE );
      /* Inform TRM for APS state end */   
#endif /* FEATURE_HDR_QTA */
  }
} /* hdrsrchtclte_done */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_CMD

DESCRIPTION
  This function processes HDR internal commands in LTE measurement state.
  Messages from LTE via the message router have been converted to HDR internal
  commands and will be processed here.

DEPENDENCIES
  None

PARAMETERS 
  Pointer to command

RETURN VALUE
  E_SUCCESS       - command has been successfully executed
  E_FAILURE       - command has failed
  E_NOT_SUPPORTED - command is not support
  E_IN_PROGRESS   - command has been started, but has not completed
  E_AGAIN         - command cannot be started in present state, but is a
                    valid command.
 
SIDE EFFECTS
  If E_IN_PROGRESS or E_AGAIN is returned, command processing is disabled
  until the command buffer is passed to hdrsrch_complete_cmd().

===========================================================================*/
errno_enum_type hdrsrchtclte_cmd
(
  const hdrsrch_cmd_type                *cmd
    /* Internal HDR command to be processed. */
)

{

  errno_enum_type                       status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch( cmd->name )
  {

    /* Indicate the reason for the abort for debugging purpose*/

    case HDRSRCH_DEACTIVATE_CMD:
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:cmd(),HDRSRCH_DEACTIVATE_CMD" );
      status = hdrsrchtclte_abort_lte_meas();
        /* Abort LTE meas, return to traffic state */
      break;

    case HDRSRCH_ADVISE_UNLOCK_RF_CMD:
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:cmd(),HDRSRCH_ADVISE_UNLOCK_RF_CMD" );
      status = hdrsrchtclte_abort_lte_meas();
        /* Abort LTE meas, return to traffic state */
      break;

    case HDRSRCH_IDLE_CMD:
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:cmd(),HDRSRCH_IDLE_CMD" );
      status = hdrsrchtclte_abort_lte_meas();
        /* Abort LTE meas, return to traffic state */
      break;

    default:
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:cmd(),process state-independent cmd %d",
                      cmd->name );

      status = hdrsrchtc_default_traffic_cmd( cmd );
        /* Handle state-independent commands */
      break;

  } /* switch (cmd->name) */

  hdrsrchtclte.last_cmd = cmd->name;
    /* Remember the last command for debugging */

  return status;

} /* hdrsrchtclte_cmd */

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
void hdrsrchtclte_init_tclte_state( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_change_state( &hdrsrchtclte_state );
    /* Switch to LTE measurement for redirection state processing */

} /* hdrsrchtclte_init_meas_state */

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
)

{
  errno_enum_type                       status = E_SUCCESS;
    /* Command completion status */
  int                                   memcmp_result = 0;
    /* Result of memcmp, between the saved command and new command */
  uint8                                 idx = 0;
    /* Loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:process_lte_nset_cmd()" );

  /* Compare the new LTE nset command with the saved command.  If they are identical,
     there is no need to perform update. */
  memcmp_result = memcmp( &hdrsrchtclte_saved_params.redir_params, lte_nset_redir_ptr, 
                          sizeof( hdrsrchtclte_saved_params.redir_params ) );
  if( memcmp_result == 0 )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                 "DLR:Identical LTE NSET update command received, no update" );
    return status;
  }

  hdrsrchtclte_saved_params.redir_params = *lte_nset_redir_ptr;
    /* Save a copy of the command */

  hdrsrchtclte.update_list_ind = TRUE;
    /* Indicate that DO needs to update the LTE NSET */

  if ( ( hdrsrchtclte.curr_freq_entry_ptr != NULL ) &&
       ( hdrsrchtclte.curr_freq_entry_ptr->trm_set_client_state_output.grant == TRM_GRANTED ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:LTE nset update command received while TRM grant is active, delay processing" );
  }
  else 
  {
    hdrsrchtclte_lte_nset_cmd_update();
      /* Can update the LTE neighbors now */
  }

  return status;

} /* hdrsrchtclte_process_lte_nset_cmd */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_PROCESS_INIT_CNF_MSG

DESCRIPTION
  This function processes the Init confirmation message received from LTE.
  DO can now move to the next state to continue with the LTE messaging
  session for pilot measurement.
  
DEPENDENCIES
  None

PARAMETERS 
  Pointer to Init cnf message
 
RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_process_init_cnf_msg
(
  hdrsrch_lte_init_cnf_params_type         *init_cnf_ptr
)

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "DLR:process_init_cnf_msg(),init_cnf_ind=%d,tx=%ddBm",
                  hdrsrchtclte.init_cnf_ind, init_cnf_ptr->max_tx_pwr ) ;

  if( hdrsrchtclte.init_cnf_ind != HDRSRCHTCLTE_LTEINIT_SENT )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Possible mismatch of LTE Init req %d", 
                    hdrsrchtclte.init_cnf_ind );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
    return;
  }

  hdrsrchtclte.init_cnf_ind = HDRSRCHTCLTE_LTEINIT_RXED;
    /* Remember that DO received INIT confirmation from LTE */

  if( hdrsrchtclte.redir_state == HDRSRCHTCLTE_ABORT_STATE )
  {
    hdrsrchtclte_end_lte_meas();
      /* Need to wrap up the previous measurement */

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
    return;

  }

  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN to start measurement" );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
    return;
  }

  hdrsrchtclte_start_lte_meas();
    /* Begin the process to measure the first lte frequency this cycle */

} /* hdrsrchtclte_process_init_cnf_msg */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_TBACKOFF_TIMER_SET

DESCRIPTION
  This function sets the back off timer for a LTE frequency when it fails
  the redirection threshold comparison.  The back off timer is used to save
  battery life by avoiding continuous searches of LTE frequencies with
  low signal strength.  
 
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_tbackoff_timer_set( void )

{
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */  

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error checking */
  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN" );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
    return;
  }

  curr_time_sclks = timetick_get();
    /* Get current time in unit of sclks */

  freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Temp location */

  freq_entry_ptr->tbackoff_state = HDRSRCHTCLTE_TIMER_ACTIVE;
    /* Indicate that Tbackoff timer is active for this frequency */

  freq_entry_ptr->treselect_state = HDRSRCHTCLTE_TIMER_INACTIVE;
    /* Make sure Treselect timer is reset */
  if(hdrsrchtclte_is_in_fast_lte_srch_mode() == TRUE)
  {
      freq_entry_ptr->tbackoff_exp_sclks = curr_time_sclks + 
             ( freq_entry_ptr->tbackoff_duration_sec * TIMETICK_NOMINAL_FREQ_HZ );

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,"FAST_LTE_SRCH: Setting Tbackoff %d sec for EARFCN %d",
                  freq_entry_ptr->tbackoff_duration_sec,
                   freq_entry_ptr->earfcn);
  }
  else
  {
  freq_entry_ptr->tbackoff_exp_sclks = curr_time_sclks + 
    ( hdrsrchtclte.tbackoff_duration_sec * TIMETICK_NOMINAL_FREQ_HZ );

       HDR_MSG_SRCH_2( MSG_LEGACY_HIGH," Setting Tbackoff %d sec for EARFCN %d",
                    hdrsrchtclte.tbackoff_duration_sec,
                    freq_entry_ptr->earfcn);
  }

    /* Calculate time when timer expires */

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                  "DLR:Set Tbackoff timer for EARFCN %d until %u(sclks),curr(sclks)=%u, Default Tbackoff_sec=%d",
                  freq_entry_ptr->earfcn,
                  freq_entry_ptr->tbackoff_exp_sclks,
                  curr_time_sclks,
                  hdrsrchtclte.tbackoff_duration_sec
                );

} /* hdrsrchtclte_tbackoff_timer_set */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_TRESELECT_TIMER_SET

DESCRIPTION
  This function sets the reselect timer for a lte frequency since it passes
  the redirection threshold comparison.  When the reselect timer expires,
  a second measurement is initiated.  If both measurements yield good results,
  DO intiates the LTE redirection process.
 
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_treselect_timer_set( void )

{
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Error checking */
  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN" );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
    return;
  }

  freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Temp location */

  freq_entry_ptr->treselect_state = HDRSRCHTCLTE_TIMER_ACTIVE;
    /* Indicate that Tbackoff timer is active for this frequency */

  freq_entry_ptr->tbackoff_state = HDRSRCHTCLTE_TIMER_INACTIVE;
    /* Make sure Treselect timer is reset */

  curr_time_sclks = timetick_get();
    /* Get current time in sclk units */

  freq_entry_ptr->treselect_exp_sclks = curr_time_sclks + 
    ( hdrsrchtclte.treselect_duration_sec * TIMETICK_NOMINAL_FREQ_HZ );
    /* Calculate time when timer expires */

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
                  "DLR:Set Treselect timer for EARFCN %d until %d(sclks),curr(sclks)=%d,Treselect_sec=%d",
                  freq_entry_ptr->earfcn,
                  freq_entry_ptr->treselect_exp_sclks,
                  curr_time_sclks,
                  hdrsrchtclte.treselect_duration_sec 
                );

} /* hdrsrchtclte_treselect_timer_set */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_PROCESS_MEAS_RESULTS

DESCRIPTION
  This function processes the LTE frequency measurement results to decide if
  LTE signal strength is strong enough for redirection.
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  TRUE:  LTE meas meets redirection criteria
  FALSE:  LTE meas fails redirection criteria

SIDE EFFECTS 
  None 

===========================================================================*/
boolean hdrsrchtclte_process_meas_results ( void )

{

  boolean                                    good_signal = FALSE;
    /* LTE signal is good for reporting purpose */
  uint8                                      idx = 0;
    /* For loop to process all cells */
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  timetick_type                              prev_meas_age_sclks;
    /* Age of previous measurement in unit of sclks */
  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */
  uint8                                      rpt_idx = 0;
    /* Index into the report array */
  boolean                                    pass_threshx = FALSE;
  boolean                                    pass_threshxq = FALSE;
    /* Threshx and Threshxq indicators */
  int16                                      rsrp_map = 0;
  int16                                      rsrq_map = 0;
    /* RSRP and RSRQ mapping */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:process_meas_results()" );

    /* Error checking */
  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN to process meas result" );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */

    return good_signal;
  }
 
  freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Temp pointer for convenience */

  if( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.num_cells == 0  )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "DLR:LTE found no cell for EARFCN=%d",
                    hdrsrchtclte.curr_freq_entry_ptr->earfcn );

    freq_entry_ptr->good_meas_val = HDRSRCHTCLTE_GOOD_MEAS_NONE;
      /* Reset Good measurement value */

    hdrsrchtclte_tbackoff_timer_set();
      /* This EARFCN did not pass threshold test so set the tbackoff timer */

    return good_signal;
  }

  /* Range check report entries */
  if ( hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell >= 
       ( HDRCOM_SRCH_LTE_CELL_ARR_SIZE * ( HDRCOM_SRCH_LTE_FREQ_ARR_SIZE - 1 ) ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Rpt size range checking tripped %d",
                    hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell );

    return good_signal;
  }

  /* Should only get here if the report still has room for the measurements of 1 additional
     LTE EARFCN. */
  curr_time_sclks = timetick_get();
    /* Get current time in unit of sclks */

  /* Check if this frequency has been measured once already.  If it has, then make sure
     the first measurement is not stale.  If it is (the first measurement is too old to
     be considered for redirection) then discard it.  Otherwise, the first measurement
     can be used. */
  if ( freq_entry_ptr->good_meas_val != HDRSRCHTCLTE_GOOD_MEAS_NONE )
  {
    prev_meas_age_sclks = freq_entry_ptr->good_meas_timestamp_sclks +
      ( HDRSRCHTCLTE_MEAS_DISCARD_AGE_SEC * TIMETICK_NOMINAL_FREQ_HZ );
      /* Calculate age of the previous measurement in unit of sclks */
   
    if( TIME_OCCURS_FIRST( prev_meas_age_sclks,
                           curr_time_sclks ) )
    {
      HDR_MSG_SRCH_4( MSG_LEGACY_MED, "DLR:previous meas of EARFCN %d is available but discarded due to age,good_meas=%d,prev=%d,curr=%d",
                      freq_entry_ptr->earfcn,
                      freq_entry_ptr->good_meas_val,
                      prev_meas_age_sclks,
                      curr_time_sclks );

      freq_entry_ptr->good_meas_val = HDRSRCHTCLTE_GOOD_MEAS_NONE;
        /* Age of previous measurement is too old, so discard it. */
    }
  }

  /* Prepare to write the winning cell information directly to the report array. 
     Fill out the report, starting at the last entry of the report. */
  for( idx = 0; 
       ( ( idx < hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.num_cells ) &&
         ( idx < HDRCOM_SRCH_LTE_CELL_ARR_SIZE ) ); 
       idx++ ) 
  {
    pass_threshx = FALSE; 
    pass_threshxq = FALSE;
      /* Reset at start of for loop */

    /* There is a layer of translation between reported values from LTE and what goes in the report to
       network */
    if ( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrp < HDRSRCHTCLTE_RPT_LTE_RSRP_MIN )
    {
      rsrp_map = HDRSRCHTCLTE_RPT_RSRP_00;
    }
    else if ( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrp >= HDRSRCHTCLTE_RPT_LTE_RSRP_MAX )
    {
      rsrp_map = HDRSRCHTCLTE_RPT_RSRP_97; 
    }
    else
    {
      rsrp_map = hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrp - 
        HDRSRCHTCLTE_RPT_LTE_RSRP_MIN;
    }

    /* Threshx comparison, instead of multiplying threshx by 2, divide RSRP map by 2 for greater accuracy. */
    if ( ( rsrp_map / 2 ) >= hdrsrchtclte.actual_rpt_params.threshx ) 
    {
      pass_threshx = TRUE;
    }

    HDR_MSG_SRCH_7( 
      MSG_LEGACY_MED, "DLR:CompareRSRP:EARFCN=%d,CellID=%d,RSRP=%d,MapRSRP=%d,(MapRSRP/2)=%d,THRESHX=%d,passed=%d",
      freq_entry_ptr->earfcn,
      hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].cell_id,
      hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrp,
      rsrp_map,
      ( rsrp_map /2 ),
      hdrsrchtclte.actual_rpt_params.threshx,
      pass_threshx );

    /* There is a layer of translation between reported values from LTE and what goes in the report to
       network */
    if ( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrq < HDRSRCHTCLTE_RPT_LTE_RSRQ_MIN )
    {
      rsrq_map = HDRSRCHTCLTE_RPT_RSRQ_00;
    }
    else if ( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrq >= HDRSRCHTCLTE_RPT_LTE_RSRQ_MAX )
    {
      rsrq_map = HDRSRCHTCLTE_RPT_RSRQ_34; 
    }
    else
    {
      rsrq_map = 
        hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrq - 
        HDRSRCHTCLTE_RPT_LTE_RSRQ_MIN;
    }

    /* Threshxq comparison */
    if ( rsrq_map >= hdrsrchtclte.actual_rpt_params.threshxq ) 
    {
      pass_threshxq = TRUE;
    }

    HDR_MSG_SRCH_6( 
      MSG_LEGACY_MED, "DLR:CompareRSRQ:EARFCN=%d,CellID=%d,RSRQ=%d,MapRSRQ=%d,THRESHXQ=%d,passed=%d",
      freq_entry_ptr->earfcn,
      hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].cell_id,
      hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrq,
      rsrq_map,
      hdrsrchtclte.actual_rpt_params.threshxq,
      pass_threshxq );
  
    if ( ( pass_threshx == TRUE ) && ( pass_threshxq == TRUE ) )
    {
      if( freq_entry_ptr->good_meas_val == HDRSRCHTCLTE_GOOD_MEAS_NONE )
      {
        freq_entry_ptr->good_meas_val = HDRSRCHTCLTE_GOOD_MEAS_1ST;
  
        freq_entry_ptr->good_meas_timestamp_sclks = curr_time_sclks;
          /* Save the timestamp for this measurement */
  
        hdrsrchtclte_treselect_timer_set();
          /* This EARFCN passes threshold test so set the treselect timer */
        
        break;
          /* Break from for loop.  First measurement so we don't need to keep
             track of multiple cells.
             Note:  without the break, we count the 2nd cell good result as the
             2nd measurement, which is not correct.  */
      }
      else 
      {
        /* Measurement exceeds thresholds for the 2nd time.
           Save winning parameters, update the good measurement value */
        freq_entry_ptr->good_meas_val = HDRSRCHTCLTE_GOOD_MEAS_2ND;
  
        hdrsrchtclte.rpt.redirect_rpt.cell[ hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell ].earfcn = 
          freq_entry_ptr->earfcn;
        hdrsrchtclte.rpt.redirect_rpt.cell[ hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell ].cell_id =
          hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].cell_id;

        /* RSRP */
        hdrsrchtclte.rpt.redirect_rpt.cell[ hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell ].rsrp_incl = TRUE;

        /* There is a layer of translation between reported values from LTE and what goes in the report to
           network */
        hdrsrchtclte.rpt.redirect_rpt.cell[ hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell ].rsrp_val = rsrp_map;

        /* RSRQ */
        hdrsrchtclte.rpt.redirect_rpt.cell[ hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell ].rsrq_incl = TRUE;

        /* There is a layer of translation between reported values from LTE and what goes in the report to
           network */
        hdrsrchtclte.rpt.redirect_rpt.cell[ hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell ].rsrq_val = rsrq_map;

        hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell++;
          /* Go to next report entry */
  
        freq_entry_ptr->good_meas_timestamp_sclks = curr_time_sclks;
        /* Save the timestamp for this measurement */
  
        good_signal = TRUE;
        /* Set to TRUE, measurements pass threshold set to send report */

        if( hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell >= 
            ( HDRCOM_SRCH_LTE_CELL_ARR_SIZE * HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ) )
        {
          break;
            /* Report is full, break out of for loop */
        }
      }
    } 
    else
    {
      if( freq_entry_ptr->good_meas_val == HDRSRCHTCLTE_GOOD_MEAS_2ND )
      {
        break;
          /* The array contains stronger cell(s) and we are going to redirect.
             So just break out of the loop without setting the Tbackoff timer. */
      }

      freq_entry_ptr->good_meas_val = HDRSRCHTCLTE_GOOD_MEAS_NONE;
        /* Reset value */
  
      hdrsrchtclte_tbackoff_timer_set();
        /* This EARFCN did not pass threshold test so set the tbackoff timer */
  
      break;
        /* Since the signal strength of the cells are sorted, and the current cell did not pass
           threshold tests, do not need to continue */
    }

  } /* end for loop */

  return good_signal;

} /* hdrsrchtclte_process_meas_results */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SORT_MEAS_RESULTS 
 
DESCRIPTION
  This function sorts the the LTE frequency measurement results in order of
  signal strength to make sure that the strongest cells are at the top of the list.
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  None

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_sort_meas_results( void )

{

  lte_cphy_irat_meas_meas_results_s          tmp_entry;
    /* Temporary storage for use when swapping two entries */
  boolean                                    swap;
    /* Flag indicating whether we swapped any on this pass */
  uint8                                      idx = 0;
    /* Index into the array */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH(MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN to sort measurement");

    hdrsrchtclte_exit_tclte_state();
    /* Exit LTE measurement state machine, back to traffic */
    return;
  }

  if( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.num_cells < 2 )
  {
    HDR_MSG_SRCH_5( MSG_LEGACY_MED, "DLR:measarr[%d],EARFCN=%d,PCI=%d,rsrp=%d,rsrq=%d",
                    idx,
                    hdrsrchtclte.curr_freq_entry_ptr->earfcn,
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[idx].cell_id,
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[idx].rsrp,
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[idx].rsrq );

    return;
    /* Need to have at least 2 entries to sort them */
  }

  if( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.num_cells > LTE_CPHY_IMEAS_MAX_MEAS_CELLS )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                    "DLR:Number of meas cells=%d exceeds Max, drop message",
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.num_cells );
      /* Error, drop message */

    return;
  }

  /* Bubble sort algorithm for sorting the LTE measurement results */
  do
  {
    swap = FALSE;
    /* swap is set to TRUE if a swap occurs in the for loop below */

    for( idx = 0; idx < ( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.num_cells - 1 );
         idx++ )
    {
      if( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ].rsrp <
          hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx + 1 ].rsrp )
           
      /* Use RSRP value (either RSRP or RSRQ can be used for sorting) to sort the array.
         If less than next, swap entries. */
      {

        tmp_entry = hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ];
        hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx ] =
          hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx + 1 ];
        hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[ idx + 1 ] = tmp_entry;

        swap = TRUE;
        /* Note that we did a swap */

      } /* if RSRQ value of idx < ( idx + 1 ) */

    } /* for each entry */

  }
  while( swap );     /* Keep bubbling until no swap is required */

  for( idx = 0; idx < ( hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.num_cells );
       idx++ )
  {
    HDR_MSG_SRCH_5( MSG_LEGACY_MED, "DLR:measarr[%d],EARFCN=%d,PCI=%d,rsrp=%d,rsrq=%d",
                    idx,
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.meas_freq,
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[idx].cell_id,
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[idx].rsrp,
                    hdrsrchtclte_saved_params.timed_srch_meas_cnf_params.meas.cells[idx].rsrq );

  } /* for each entry */

} /* hdrsrchtclte_sort_meas_results */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_SORT_RPT_ARR
 
DESCRIPTION
  Before sending the measurement report to upper layer, this function sorts the
  LTE frequency measurement results in order of signal strength.  The strongest
  cells are on top of the report.
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  None

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_sort_rpt_arr( void )

{

  uint8                                      sort_idx = 0;
    /* Index to sort report array */
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  hdrsrchtclte_rpt_cell_struct_type          tmp_entry;
    /* Temporary storage for use when swapping two entries */
  boolean                                    swap;
    /* Flag indicating whether we swapped any on this pass */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell < 2 )
  {
      HDR_MSG_SRCH_5( MSG_LEGACY_MED, "DLR:rptentry[%d],PCI=%d,EARFCN=%d,RSRP=%d,RSRQ=%d",
                      sort_idx,
                      hdrsrchtclte.rpt.redirect_rpt.cell->cell_id,
                      hdrsrchtclte.rpt.redirect_rpt.cell->earfcn,
                      hdrsrchtclte.rpt.redirect_rpt.cell->rsrp_val,
                      hdrsrchtclte.rpt.redirect_rpt.cell->rsrq_val
                    );

    /* There is only 1 entry in the array, no need to sort it. */
    return;
  }

  /* Sort the array and report the top cells */
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:report array cnt=%d", 
                  hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell );

  HDR_ASSERT ( hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell < 
           ( HDRCOM_SRCH_LTE_CELL_ARR_SIZE * HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ) );

  /* Bubble sort algorithm for sorting the LTE measurement report */
  do
  {
    swap = FALSE;
    /* swap is set to TRUE if a swap occurs in the for loop below */

    for( sort_idx = 0; sort_idx < ( hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell - 1 );
         sort_idx++ )
    {
      if( hdrsrchtclte.rpt.redirect_rpt.cell [ sort_idx ].rsrp_val <
          hdrsrchtclte.rpt.redirect_rpt.cell [ sort_idx + 1 ].rsrp_val )
      /* Use RSRP value (either RSRP or RSRQ can be used for sorting) to sort the array.
         If less than next, swap entries. */
      {

        tmp_entry = hdrsrchtclte.rpt.redirect_rpt.cell [ sort_idx ];
        hdrsrchtclte.rpt.redirect_rpt.cell [ sort_idx ] =
          hdrsrchtclte.rpt.redirect_rpt.cell [ sort_idx + 1 ];
        hdrsrchtclte.rpt.redirect_rpt.cell [ sort_idx + 1 ] = tmp_entry;

        swap = TRUE;
        /* Note that we did a swap */

      } /* if RSRQ value of sort_idx < ( sort_idx + 1 ) */

    } /* for each entry */

  }
  while( swap );     /* Keep bubbling until no swap is required */

  /* Only report top cells, limit by the size of the report. */
  if( hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell > HDRCOM_SRCH_LTE_CELL_ARR_SIZE )
  {
    hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell = HDRCOM_SRCH_LTE_CELL_ARR_SIZE;
  }

  for( sort_idx = 0; sort_idx < ( hdrsrchtclte.rpt.redirect_rpt.num_eutra_cell );
       sort_idx++ )
  {
    HDR_MSG_SRCH_5( MSG_LEGACY_MED, "DLR:rptentry[%d],PCI=%d,EARFCN=%d,RSRP=%d,RSRQ=%d",
                    sort_idx,
                    hdrsrchtclte.rpt.redirect_rpt.cell[ sort_idx ].cell_id,
                    hdrsrchtclte.rpt.redirect_rpt.cell[ sort_idx ].earfcn,
                    hdrsrchtclte.rpt.redirect_rpt.cell[ sort_idx ].rsrp_val,
                    hdrsrchtclte.rpt.redirect_rpt.cell[ sort_idx ].rsrq_val
                  );

  } /* for each entry */

} /* hdrsrchtclte_sort_rpt_arr */

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
boolean hdrsrchtclte_max_report_count_is_triggered( void )
{
  boolean                                    result = TRUE;
    /* Return value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If the maximum number of report has been sent, return because we do not
     want to make anymore measurement.
     <snippet from spec>
     MaxReportCount
     If AutonomousReportEnabled is set to '1', the access network shall set
     this field to (MaxReportCount + 1) numbers of autonomous
     OtherRATMeasurementReport message Access Terminal can send;
  */

  if( ( hdrsrchtclte.actual_rpt_params.max_rpt_cnt != 0) &&
      ( hdrsrchtclte.rpt.rpt_cnt < hdrsrchtclte.actual_rpt_params.max_rpt_cnt + 1) )
  {
    result = FALSE;
  }

  HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "DLR:max_report_count_is_triggered() returns:%d,curr rpt_cnt=%d,max=%d,max+1=%d",
                 result,
                 hdrsrchtclte.rpt.rpt_cnt,
                 hdrsrchtclte.actual_rpt_params.max_rpt_cnt,
                 ( hdrsrchtclte.actual_rpt_params.max_rpt_cnt + 1 ) );
  return result;

} /* hdrsrchtclte_max_report_count_is_triggered */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_PREPARE_RPT
 
DESCRIPTION
  If measurements are favorable, and if we are not within the minimum report interval,
  sort the measurements by the signal strength of the LTE cells, prepare to report the
  measurements to DO Protocol layer.  Put a time stamp on the report to fulfill the
  minimum report interval requirement for the next report.
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  None

SIDE EFFECTS 
  None 

===========================================================================*/
void hdrsrchtclte_prepare_rpt( void )
{
  timetick_type                                curr_time_sclks;
    /* Current time in sclks units */
  hdrsrchtclte_earfcn_struct_type              *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  uint8                                        idx = 0;
    /* Loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH(MSG_LEGACY_MED, "DLR:prepare_rpt()");

  /* If measurements are favorable, and if we are not within the minimum report interval,
     sort the measurements by the signal strength of the LTE cells, prepare to report the
     measurements to L3. */

  if ( hdrsrchtclte_min_rpt_interval_is_triggered() == TRUE )
  {
    HDR_MSG_SRCH(MSG_LEGACY_HIGH, "DLR:Drop report as DO is in min report interval window" );
    return;
  }

  hdrsrchtclte_sort_rpt_arr();
    /* Sort and select best cells in the array of measurements for the report */

	  
  ( void ) hdrsrchtclte_send_deinit_req();
    /*  Make sure DO sends Deinit request to LTE *before* initiating redirection.
	     So sends Deinit to LTE and afterwards notify CP to start redirection via the following
            call back function. */

  hdrsrchtclte.redirect_cb(&(hdrsrchtclte.rpt.redirect_rpt));
      /* Call back to CP to report list to AN */

  hdrsrchtclte.rpt.rpt_cnt++;
    /* Increase the report count, as the report is ready to be sent to AN */

  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "DLR:Request Protocol layer to send report,rpt_cnt=%d",
                 hdrsrchtclte.rpt.rpt_cnt);

  curr_time_sclks = timetick_get();
    /* Get current time in unit of sclks */

  hdrsrchtclte.rpt.rpt_timestamp_sclks = curr_time_sclks;
    /* Save report timestamp */

  memset( &hdrsrchtclte.rpt.redirect_rpt, 0, sizeof( hdrsrchtclte.rpt.redirect_rpt ) );
    /* Reset LTE report after reporting to Protocol layer */

  /* Clear out all previous measurement status so we can start fresh with future measurements. */
  freq_entry_ptr = hdrsrchtclte.earfcn_arr.arr;

  for( idx = 0; 
       ( idx < hdrsrchtclte.earfcn_arr.arr_cnt ) &&
       ( idx < HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ); 
       idx++ )
  {
    freq_entry_ptr->good_meas_timestamp_sclks = 0;
    freq_entry_ptr->good_meas_val = HDRSRCHTCLTE_GOOD_MEAS_NONE;
    freq_entry_ptr++;

  } /* end for loop */

} /* hdrsrchtclte_prepare_rpt */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_PROCESS_TIMED_SRCH_MEAS_CNF_MSG

DESCRIPTION
  This function processes the Timed Search and Meas confirmation message received
  from LTE. Once DO sends the Timed Search and Meas request message to LTE,
  DO processing is suspended until this confirmation is received.
  
DEPENDENCIES
  None

PARAMETERS 
  Pointer to confirmation message
 
RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/
errno_enum_type hdrsrchtclte_process_timed_srch_meas_cnf_msg
(
   hdrsrch_lte_timed_srch_meas_cnf_params_type  *timed_srch_meas_cnf_ptr
)

{
  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */
  boolean                                      good_signal = FALSE;
    /* LTE signal is good for reporting purpose */
  timetick_type                                curr_time_sclks;
    /* Current time in sclks units */
  timetick_type                                prev_rpt_age_sclks;
    /* Age of previous report in unit of sclks */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( hdrsrchtclte.curr_freq_entry_ptr != NULL ) 
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_MED, 
                   "DLR:timed_srch_meas_cnf_msg(),EARFCN=%d",hdrsrchtclte.curr_freq_entry_ptr->earfcn );
  }
  else
  {
    HDR_MSG_SRCH(MSG_LEGACY_MED, "DLR:timed_srch_meas_cnf_msg()");
  }

#ifdef FEATURE_HDR_D2L_RF_MEAS
  hdrsrchtclte_rfm_meas_exit();
    /* Allows LTE to set up Exit meas parameters */
#endif /* FEATURE_HDR_D2L_RF_MEAS */  

  if( timed_srch_meas_cnf_ptr == NULL )
  {
    /* If the message ptr is Null, drop the message */
    return status;
  }

  lte_ml1_irat_gap_srch_meas_cnf_handling
	  (
#ifdef FEATURE_HDR_MSGR_VARIANT 
           hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
           timed_srch_meas_cnf_ptr, 
	  &hdrsrchtclte.gap_state);

  /* LTE requires this interface call, it's not used by DO */

  if( hdrsrch_state_id != HDRSRCH_LTE_MEAS_TC_STATE )
  {
    HDR_MSG_SRCH(MSG_LEGACY_MED, "DLR:timed_srch_cnf_msg received while not in traffic");
      /* If this message is received in the wrong state, drop the message */
    return status;
  }

  if( hdrsrchtclte.redir_state == HDRSRCHTCLTE_ABORT_STATE )
  {
    HDR_MSG_SRCH(MSG_LEGACY_MED,
                 "DLR:LTE returned timed_srch_cnf, DO in abort state");
      /* DO already sent an Abort request, so just ignore this Meas confirmation.
         DO needs to wait for the Abort confirmation which should be arriving after
         this confirmation. */
    return status;
  }

  hdrsrchtclte_saved_params.timed_srch_meas_cnf_params = *timed_srch_meas_cnf_ptr;
    /* Save a copy */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                  "DLR:search_performed=%d,num cells searched=%d,num cells measured=%d",
                  timed_srch_meas_cnf_ptr->search.search_performed,
                  timed_srch_meas_cnf_ptr->search.num_detected_cells,
                  timed_srch_meas_cnf_ptr->meas.num_cells
                );

  if( timed_srch_meas_cnf_ptr->meas.num_cells > LTE_CPHY_IMEAS_MAX_MEAS_CELLS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR,
                    "DLR:Number of meas cells=%d exceeds Max, drop message",
                   timed_srch_meas_cnf_ptr->meas.num_cells );
      /* Error, drop message */
    return status;
  }

  if ( timed_srch_meas_cnf_ptr->search.search_performed == TRUE )
  {
    curr_time_sclks = timetick_get();
    /* Get current time in unit of sclks */

    hdrsrchtclte_sort_meas_results();
    /* Sort meas result to be more sufficient.  If the strongest cell does not pass threshold,
       do not continue with weaker cells */

    good_signal = hdrsrchtclte_process_meas_results();
    /* Got measurements back from LTE, process the results */

    if( good_signal == TRUE )
    {
      hdrsrchtclte.found_good_cell = TRUE;
      /* Looks like at least 1 LTE cell passes threshold, continue on until we are done
         with this measurement pass. */
    }
  }

  hdrsrchtclte_end_lte_meas();
  /* Need to wrap up the previous measurement */

  hdrsrchtclte.prev_freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
    /* Finish measurement, get ready for the next freq */

  hdrsrchtclte_get_lte_freq_trm(FALSE);
  /* Get the next LTE freq and TRM extension.  TRM is granted per LTE frequency.
     Indicate that this is not the first measurement for this cycle */

  /* If we are done with this measurement cycle, and there are good LTE cells, prepare the
     report and notify L3. */
  if( hdrsrchtclte.curr_freq_entry_ptr == NULL )
  {
    if( hdrsrchtclte.found_good_cell == TRUE )
    {
      hdrsrchtclte_prepare_rpt();
        /* Looks like we found at least 1 good LTE EARFCN, prepare report */

      hdrsrchtclte.found_good_cell = FALSE;
        /* Reset flag after sending report */
    }

    hdrsrchtclte_exit_tclte_state();
    /* Exit LTE measurement state machine, back to traffic */

    return status;
  }

  hdrsrchtclte.earfcn_meas_this_cycle_cnt++;
    /* For debugging purposes, keep tract of how many EARFCN DO measures */

  if ( hdrsrchtclte.earfcn_meas_this_cycle_cnt > hdrsrchtclte.earfcn_arr.arr_cnt )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"DLR:!!!Measurement count exceed array count, double check!!!" );
  }

  /* Send timed search and meas req message, resume after confirmation is received. */
  status = hdrsrchtclte_send_timed_srch_meas_req();

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "DLR:Timed_search_meas_req failed" );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */

  }
  return status;

} /* hdrsrchtclte_process_timed_srch_meas_cnf_msg */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_PROCESS_DEINIT_CNF_MSG

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
errno_enum_type hdrsrchtclte_process_deinit_cnf_msg( void )

{
  errno_enum_type                  status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "DLR:process_deinit_cnf_msg(),state=0x%x",
                 hdrsrch_state_id);

  if( hdrsrchtclte.init_cnf_ind != HDRSRCHTCLTE_LTEDEINIT_SENT )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "DLR:Possible mismatch of LTE Deinit req %d",
                   hdrsrchtclte.init_cnf_ind);
  }

  if(hdrsrch_state_id == HDRSRCH_INACTIVE_STATE )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_MED, "DLR:Clearing Sanity timer,state=0x%x",
					 hdrsrch_state_id);
    hdrsrchtclte_sanity_timer_clear();
  }

  /* Reset all LTE measurement information. */
  hdrsrchtclte_reset_state();

  if( ( hdrsrch_state_id == HDRSRCH_INACTIVE_STATE ) &&
      ( hdrmdsp_fws_app_is_enabled() == FALSE ) )
  /* If called when DO is trying to shutdown, check if FW has been disabled.
     If FW has been disabled, then shut down clocks.
     The FW check is necessary in the case we have just entered INACTIVE state, but we are
     still waiting for Searcher idle to disable FW.  If we get the deinit CNF within this
     window, we need to wait for hdrsrchst_powerdown() to disable FW AND DO clocks in that
     order. */
  {
    HDR_MSG_SRCH(MSG_LEGACY_MED, "DLR:hdrsrchmcpm_do2lte_measurement( FALSE )");
    hdrsrchmcpm_do2lte_measurement(FALSE);
    /* DO is not in Meas state, MCPM has been turned on for LTE to send Deinit
       request.  So now DO needs to turn it off */

    hdrsrchst_deactivate_done_notify();
 	/* Notify L3 that L1 deactivation is done */
    HDR_MSG_SRCH(MSG_LEGACY_MED, "DLR:hdrsrchmcpm_set_state( HDRSRCHMCPM_STOP_REQ )");
    hdrsrchmcpm_set_state(HDRSRCHMCPM_STOP_REQ);
    /* Tell MCPM to powerdown DO clocks */
  }
  return status;

} /* hdrsrchtclte_process_deinit_cnf_msg */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_PROCESS_ABORT_CNF_MSG

DESCRIPTION
  This function processes the Abort confirmation message receives from LTE.
  DO processing is suspended until this confirmation is received.
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/
errno_enum_type hdrsrchtclte_process_abort_cnf_msg( void )

{
  errno_enum_type                  status = E_SUCCESS;
  /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH(MSG_LEGACY_MED, "DLR:process_abort_cnf_msg()");

  hdrsrchtclte_end_lte_meas();
    /* Need to wrap up the previous measurement */

  hdrsrchtclte_exit_tclte_state();
    /* Exit LTE measurement state machine, back to traffic */
  return status;

} /* hdrsrchtclte_process_abort_cnf_msg */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_ABORT_LTE_MEAS

DESCRIPTION
  This function sends the Abort request message to LTE upon receiving commands 
  from DO LMAC layer which require aborting LTE pilot measurement.  The following
  commands initiate abort processing:
    HDRSRCH_IDLE_CMD
    HDRSRCH_DEACTIVATE_CMD      
    HDRSRCH_ADVISE_UNLOCK_RF_CMD
  
DEPENDENCIES
  None

PARAMETERS 
  None
 
RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/
errno_enum_type hdrsrchtclte_abort_lte_meas( void )

{
  errno_enum_type                  status = E_AGAIN;
    /* Command completion status, set to E_AGAIN to be processed in traffic state */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "DLR:abort_lte_meas(),state=%d",
                 hdrsrchtclte.redir_state);

  switch( hdrsrchtclte.redir_state )
  {
     case HDRSRCHTCLTE_INIT_STATE:
       hdrsrchtclte.redir_state = HDRSRCHTCLTE_ABORT_STATE;
       /* Remember that DO is trying to abort LTE measurements but wait
          until the confirmation for these requests come back */
       break;

     case HDRSRCHTCLTE_TIMED_SRCH_MEAS_STATE:
       /* DO is waiting for confirmation.  Since these operations can be aborted
          on LTE side, send Abort req message, resume after confirmation
          is received */
       hdrsrchmsg_d2l_send_abort_req_msg();
       hdrsrchtclte.last_sent = HDRSRCHTCLTE_ABORT_REQ;
       hdrsrchtclte.redir_state = HDRSRCHTCLTE_ABORT_STATE;
       /* Treat Abort as a special case, remember that DO is trying to abort
          LTE measurements */
       break;

     case HDRSRCHTCLTE_DEINIT_STATE:
       break;

     case HDRSRCHTCLTE_INACTIVE_STATE:
     case HDRSRCHTCLTE_ABORT_STATE:
     default:
       break;

  } /* end switch */

  if( ( hdrsrchtclte.redir_state == HDRSRCHTCLTE_ABORT_STATE ) &&
      ( hdrsrchtclte.last_sent == HDRSRCHTCLTE_INIT_REQ ) )
  {
    hdrsrchtclte.is_rfm_meas_entered = FALSE;
    /* If DO must abort the D2L meas before sending TIMED_SRCH_REQ,
       do not call LTE exit function.
       This is a new requirement set forth by RF team.
     
       Excerpt from RF team:
       All the power saving features are based on the assumption that
       vote on (build script) and vote off(meas exit) will happen in pair.
     
       state == ABORT if DO already got the abort command
       last_sent == INIT_REQ if DO is still waiting for INIT_CNF, so DO has not sent MEAS_REQ */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Abort before TIMED_SRCH_REQ, no meas_exit" );
  }

  return status;

} /* hdrsrchtclte_abort_lte_meas */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  This function processes messages from LTE via the message router.
  These messages are confirmation messages coming from LTE
  for DO->LTE redirection.  They need to be in a separate queue from
  the command queue.  This eliminate the race condition when DO receives
  commands (such as the Deactivate command) which require the LTE measurements
  to be aborted.  DO still needs to finish the LTE message sequence and
  receives the LTE confirmations.
 
  Without having a separate queue for the LTE confirmation messages, the pending
  Deactivate command would block DO L1 from processing the LTE confirmations,
  creating a dead lock situation.
 
  It was decided that the low priority queue is a good queue for these LTE
  confirmation messages. 

DEPENDENCIES
  None

PARAMETERS 
  Ptr to LTE confirmation message to be processed
 
RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_process_low_pri_rsp_ind
(
   hdrsrchmsg_s_type                     *msg
     /* LTE confirmation messages to be processed. */
)

{

  errno_enum_type                       status = E_SUCCESS;
  /* Command completion status */

#ifdef FEATURE_HDR_MSGR_VARIANT 
  sys_modem_as_id_e_type variant;
  /* Indicates which subscription should receive the message */
#endif  /* FEATURE_HDR_MSGR_VARIANT */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2(MSG_LEGACY_MED, "DLR:process_low_pri_rsp_ind(),msg=%s,umid=0x%8x",
                 hdrsrchmsg_get_msg_name(msg->msg.hdr.id),
                 msg->msg.hdr.id);

#ifdef FEATURE_HDR_MSGR_VARIANT 
  variant = MSGR_GET_VARIANT(&msg->msg.hdr);
  
  MSGR_CLEAR_VARIANT( msg->msg.hdr.id);
  
  /* Only process messages meant for the current subscription */
  if(variant != hdrsrch_get_subscriber_id())
  {
      HDR_MSG_SRCH_3 (MSG_LEGACY_ERROR,
                      "====================== Unexpected msg %d received on %d variant curr_sub_id %d ===========",
      msg->msg.hdr.id,variant,hdrsrch_get_subscriber_id() );
      return;
  }
#endif /* FEATURE_HDR_MSGR_VARIANT */

  switch( msg->msg.hdr.id )
  {
     case HDRSRCH_LTE_IRAT_MEAS_D2L_INIT_CNF:
       /* Receive LTE Init confirmation message via message router */

       hdrsrchtclte_process_init_cnf_msg(&msg->msg.lte_cnf_msg.lte_init_cnf);
       break;

     case HDRSRCH_LTE_IRAT_MEAS_D2L_TIMED_SRCH_MEAS_CNF:
       /* Receive LTE Timed Search Meas confirmation message via message router */
       status =
          hdrsrchtclte_process_timed_srch_meas_cnf_msg(&msg->msg.lte_cnf_msg.lte_timed_srch_meas_cnf);
       break;

     case HDRSRCH_LTE_IRAT_MEAS_D2L_DEINIT_CNF:
       /* Receive LTE Deinit confirmation message via message router */
       status = hdrsrchtclte_process_deinit_cnf_msg();
       break;

     case HDRSRCH_LTE_IRAT_MEAS_D2L_ABORT_CNF:
       /* Receive LTE Abort confirmation message via message router */
       status = hdrsrchtclte_process_abort_cnf_msg();
       break;

     default:
       HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "DLR:Low priority queue msg %x ignored",
                      msg->msg.hdr.id);
       break;

  } /* switch( msg->msg.hdr.id ) */

  hdrsrchtclte.last_cnf = msg->msg.hdr.id;
  /* Remember the last command for debugging */

} /* hdrsrchtclte_process_low_pri_rsp_ind */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_GET_LTE_FREQ

DESCRIPTION 
  This function is called to get the next LTE frequency to measure.
 
DEPENDENCIES
  None

PARAMETERS 
  TRUE:  This is the first measurement in the measurement cycle, TRM extension is already performed
  FALSE:  This is a follow on measurement, TRM extension still needs to be performed
 
RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_get_lte_freq
(
   boolean                                   first_meas_this_cycle
     /* First measurement in meas cycle */
)

{
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  boolean                                    done = FALSE;
    /* Done traversing array */
  uint16                                     cnt = 0;
    /* For loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( hdrsrchtclte.earfcn_arr.arr_cnt == 0 )
  {
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
    hdrsrchtclte.prev_freq_entry_ptr = NULL;

      /* Reset current frequency ptr, at this point there is nothing to measure */
    HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:get_lte_freq()= 0 Nothing in array" );
    return;
  }

  /* If there is no previous measurement, then measure the 1st frequency
     in the array if timers are not active and if we have not measured it this cycle.
     If we have measured it this cycle and it passes measurement, the 3rd check will trip
     and neither timer will be set in this case. */
  if( hdrsrchtclte.prev_freq_entry_ptr == NULL ) 
  {
    if ( ( hdrsrchtclte.earfcn_arr.arr[ 0 ].treselect_state == HDRSRCHTCLTE_TIMER_INACTIVE ) &&
         ( hdrsrchtclte.earfcn_arr.arr[ 0 ].tbackoff_state  == HDRSRCHTCLTE_TIMER_INACTIVE ) && 
         ( hdrsrchtclte.earfcn_arr.arr[ 0 ].good_meas_val != HDRSRCHTCLTE_GOOD_MEAS_2ND ) )
      /* No active timers and not yet selected for redirection */
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR:get_lte_freq() meas EARFCN=%d (out of %d,no prev meas)",
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].earfcn,
                      hdrsrchtclte.earfcn_arr.arr_cnt );
      hdrsrchtclte.curr_freq_entry_ptr = &hdrsrchtclte.earfcn_arr.arr[ 0 ];
    }
    else
    {
      HDR_MSG_SRCH_5( MSG_LEGACY_MED, "DLR:get_lte_freq() reject EARFCN=%d (out of %d,no prev meas),treselect=%d,tbackoff=%d,goodmeas=%d", 
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].earfcn,
                      hdrsrchtclte.earfcn_arr.arr_cnt,
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].treselect_state,
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].tbackoff_state,
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].good_meas_val );
      hdrsrchtclte.curr_freq_entry_ptr = NULL;
        /* Print EARFCN before setting ptr to NULL */
    }
    return;
  }

  /* Getting here implies that we have made measurement in the prior meas cycle.  If this is the first meas
     this cycle, and there is only 1 EARFCN, then we haven't measured it yet.  Do the same checks as above
     to make sure this EARFCN is eligible for measurement. */
  if( ( first_meas_this_cycle == TRUE ) && ( hdrsrchtclte.earfcn_arr.arr_cnt == 1 ) )
  {
    if ( ( hdrsrchtclte.earfcn_arr.arr[ 0 ].treselect_state == HDRSRCHTCLTE_TIMER_INACTIVE ) &&
         ( hdrsrchtclte.earfcn_arr.arr[ 0 ].tbackoff_state  == HDRSRCHTCLTE_TIMER_INACTIVE ) &&
         ( hdrsrchtclte.earfcn_arr.arr[ 0 ].good_meas_val != HDRSRCHTCLTE_GOOD_MEAS_2ND ) )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:get_lte_freq() meas EARFCN=%d (out of 1,prev meas,1st meas)",hdrsrchtclte.earfcn_arr.arr[ 0 ].earfcn );
      hdrsrchtclte.curr_freq_entry_ptr = &hdrsrchtclte.earfcn_arr.arr[ 0 ];
    }
    else
    {
      HDR_MSG_SRCH_4( MSG_LEGACY_MED, "DLR:get_lte_freq() reject EARFCN=%d (out of 1,no prev meas,1st meas),treselect=%d,tbackoff=%d,goodmeas=%d", 
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].earfcn,
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].treselect_state,
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].tbackoff_state,
                      hdrsrchtclte.earfcn_arr.arr[ 0 ].good_meas_val );
      hdrsrchtclte.curr_freq_entry_ptr = NULL;
        /* Print EARFCN before setting ptr to NULL */
    }
    return;

  }

  /* If the array only has 1 frequency, then measure the 1st frequency in the array if timers are not active.
     The F3 message distinguishes this case from the case above for debugging purposes. */
  if( hdrsrchtclte.earfcn_arr.arr_cnt == 1 )
  {
    HDR_MSG_SRCH_4( MSG_LEGACY_MED, "DLR:get_lte_freq() reject EARFCN=%d (out of 1,already meas),tbackoff=%d,treselect=%d,goodmeas=%d", 
                    hdrsrchtclte.earfcn_arr.arr[ 0 ].earfcn,
                    hdrsrchtclte.earfcn_arr.arr[ 0 ].tbackoff_state, 
                    hdrsrchtclte.earfcn_arr.arr[ 0 ].treselect_state,
                    hdrsrchtclte.earfcn_arr.arr[ 0 ].good_meas_val 
                  );
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
      /* Print EARFCN before setting ptr to NULL */
    return;
  }

  /* Here we have multiple frequencies and we have made a previous measurement, so we need to go to the
     next frequency, with wrap around.  Do the same checks as above
     to make sure this EARFCN is eligible for measurement. */
  hdrsrchtclte.curr_freq_entry_ptr = NULL;
  freq_entry_ptr = hdrsrchtclte.prev_freq_entry_ptr;
  freq_entry_ptr++;
  if( ( freq_entry_ptr > &( hdrsrchtclte.earfcn_arr.arr[ hdrsrchtclte.earfcn_arr.arr_cnt - 1 ] ) ) ||
      ( freq_entry_ptr > &( hdrsrchtclte.earfcn_arr.arr[ HDRCOM_SRCH_LTE_FREQ_ARR_SIZE - 1 ] ) ) )
  {
    freq_entry_ptr = &hdrsrchtclte.earfcn_arr.arr[ 0 ];
  }

  /* Go through all entries in array */
  for ( cnt = 0;  
        ( ( cnt < hdrsrchtclte.earfcn_arr.arr_cnt ) &&
          ( cnt < HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ) );
        cnt++ )
  {
    if ( ( freq_entry_ptr->treselect_state == HDRSRCHTCLTE_TIMER_INACTIVE ) &&
         ( freq_entry_ptr->tbackoff_state  == HDRSRCHTCLTE_TIMER_INACTIVE ) &&
         ( freq_entry_ptr->good_meas_val != HDRSRCHTCLTE_GOOD_MEAS_2ND ) )
    {
      hdrsrchtclte.curr_freq_entry_ptr = freq_entry_ptr;
      HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR:get_lte_freq() meas EARFCN=%d (out of %d)", 
                      freq_entry_ptr->earfcn,
                      hdrsrchtclte.earfcn_arr.arr_cnt );
      break;
    }
    else
    {
      HDR_MSG_SRCH_5( MSG_LEGACY_MED, "DLR:get_lte_freq() reject EARFCN=%d (out of %d),treselect=%d,tbackoff=%d,goodmeas=%d", 
                      freq_entry_ptr->earfcn,
                      hdrsrchtclte.earfcn_arr.arr_cnt,
                      freq_entry_ptr->treselect_state,
                      freq_entry_ptr->tbackoff_state,
                      freq_entry_ptr->good_meas_val
                    );
      freq_entry_ptr++;
      if( ( freq_entry_ptr > &( hdrsrchtclte.earfcn_arr.arr[ hdrsrchtclte.earfcn_arr.arr_cnt - 1 ] ) ) ||
          ( freq_entry_ptr > &( hdrsrchtclte.earfcn_arr.arr[ HDRCOM_SRCH_LTE_FREQ_ARR_SIZE - 1 ] ) ) )
      {
        freq_entry_ptr = &hdrsrchtclte.earfcn_arr.arr[ 0 ];
      }
    }
  }

} /* hdrsrchtclte_get_lte_freq */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_GET_LTE_FREQ_TRM

DESCRIPTION 
  This function is called to request TRM to measure a LTE frequency.
  The TRM request sequence is shown below:
  1.  Get duration for 1 LTE frequency measurement
  2.  Get LTE band associated with the LTE frequency (EARFCN)
  3.  Call TRM set_client_state() API to request TRM extension
 
DEPENDENCIES
  If TRM is granted, TRM requires that the set_client_state() API is called
  after measurement is completed to end the transaction.

PARAMETERS 
  TRUE:  This is the first measurement in the measurement cycle, need to add
         ramp down time to measurement time.
  FALSE:  This is a follow on measurement, only measurement time is required.
 
RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_get_lte_freq_trm
(
   boolean                                      first_meas_this_cycle
     /* First measurement in meas cycle */
)

{
  hdrsrchtclte_earfcn_struct_type              *freq_entry_ptr = NULL;
  /* Temp pointer for convenience */
  static trm_set_client_state_input_type       trm_set_client_state_input;
  static trm_set_client_state_output_data      trm_set_client_state_output;
  /* Parameters for TRM set_client_state API */
  sys_sband_lte_e_type                         lte_band;
  /* LTE band obtained from LTE */
  timetick_type                                curr_time_sclks;
  /* Current time in sclks units */
  uint16                                       meas_time_ms;
  /* Measurement time may include DRC ramp down time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "DLR:get_lte_freq_trm(),first_meas=%d", first_meas_this_cycle);

  if( hdrsrchtclte.earfcn_arr.arr_cnt == 0 )
  {
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
    /* Reset current frequency ptr, at this point there is nothing to measure */

    HDR_MSG_SRCH(MSG_LEGACY_HIGH, "DLR:current array count is 0, nothing to measure");
    return;
  }

  /* Measurement time may include DRC ramp down time */
  if( first_meas_this_cycle == TRUE )
  {
    meas_time_ms = hdrsrchtclte.rampdown_ms + HDRSRCHTCLTE_DURATION_PER_EARFCN_MS;
  } 
  else
  {
    meas_time_ms = HDRSRCHTCLTE_DURATION_PER_EARFCN_MS;
  }

  curr_time_sclks = timetick_get();
  /* Get current time in unit of sclks */

  HDR_MSG_SRCH_5(MSG_LEGACY_HIGH, "DLR:got current time,curr=%u,dur=%u,end=%u,avail_ms=%d,avail_sclks=%u",
                 curr_time_sclks,
                 TIMETICK_SCLK_FROM_MS(meas_time_ms),
                 hdrsrchtclte.meas_end_time_sclks,
                 hdrsrchtclte.avail_ms,
                 TIMETICK_SCLK_FROM_MS(hdrsrchtclte.avail_ms) 
                );

  /* Check when HDR must be finished with LTE meas, do we have enough time for one more meas? */
  if( TIME_OCCURS_FIRST(hdrsrchtclte.meas_end_time_sclks,
                        curr_time_sclks + TIMETICK_SCLK_FROM_MS(meas_time_ms)) )
  {
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
    /* Reset current frequency ptr, at this point there is nothing to measure */

    HDR_MSG_SRCH_3(MSG_LEGACY_HIGH, "DLR:run out of time,curr=%u,dur=%u,end=%u",
                   curr_time_sclks,
                   TIMETICK_SCLK_FROM_MS(meas_time_ms),
                   hdrsrchtclte.meas_end_time_sclks);
    return;
    /* There is not enough time to measure, return */
  }

  hdrsrchtclte_get_lte_freq( first_meas_this_cycle );
    /* Get a LTE frequency to measure */

  if ( hdrsrchtclte.curr_freq_entry_ptr == NULL )
  {
    return;
    /* There is nothing to measure, return */
  }

  freq_entry_ptr = hdrsrchtclte.curr_freq_entry_ptr;
  /* Temp location */

  /* There is a LTE frequency which HDR can measure, so try to get TRM grant */
  lte_band =
	  lte_rrc_get_band_from_dl_earfcn(
#ifdef FEATURE_HDR_MSGR_VARIANT 
           hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
           freq_entry_ptr->earfcn,
                                     &freq_entry_ptr->lte_band_is_valid);
  /* Get LTE band for the given earfcn */

  if( freq_entry_ptr->lte_band_is_valid == FALSE )
  {
    HDR_MSG_SRCH_2(MSG_LEGACY_ERROR, "DLR:LTE BAND is invalid,earfcn=%d,band=%d",
                   freq_entry_ptr->earfcn, lte_band);
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
    /* Error condition, return */
    return;
  }

  freq_entry_ptr->earfcn_trm_info.band = lte_ml1_convert_lte_band_to_sys_band(lte_band);
  /* Convert from LTE band to TRM band */

  memset(&trm_set_client_state_input, 0, sizeof(trm_set_client_state_input));
  memset(&trm_set_client_state_output, 0, sizeof(trm_set_client_state_output));

  /* Propagate information for the TRM set client state request */
  trm_set_client_state_input.client_id = TRM_HDR;
  trm_set_client_state_input.client_state = TRM_IRAT_STATE;
  trm_set_client_state_input.data.irat_data.operation = TRM_START_STATE;
  trm_set_client_state_input.data.irat_data.should_change_reason_to_irat = TRUE;
  trm_set_client_state_input.data.irat_data.measured_client = TRM_LTE;
  trm_set_client_state_input.data.irat_data.freq_info.num_bands = 1;
  trm_set_client_state_input.data.irat_data.freq_info.bands[0].band =
     freq_entry_ptr->earfcn_trm_info.band;
  trm_set_client_state_input.data.irat_data.transaction_id = 0;
  trm_set_client_state_input.data.irat_data.is_diversity_needed = TRUE;
  trm_set_client_state_input.data.irat_data.sync_result = TRUE;
  trm_set_client_state_input.data.irat_data.duration = 
    TIMETICK_SCLK_FROM_MS( meas_time_ms );
  trm_set_client_state(&trm_set_client_state_input, &trm_set_client_state_output);
  /* Call trm_set_client_state. We should get immediate grant or denial. */

  HDR_MSG_SRCH_4(MSG_LEGACY_HIGH, "DLR:TRM START_STATE grant=%d,LTE band=%d/earfcn=%d,duration(sclks)=%d",
                 trm_set_client_state_output.grant,
                 freq_entry_ptr->earfcn_trm_info.band,
                 freq_entry_ptr->earfcn,
                 trm_set_client_state_input.data.irat_data.duration 
                 );

  if( trm_set_client_state_output.grant != TRM_GRANTED )
  {
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
    return;
    /* TRM was not granted */
  }

  HDR_MSG_SRCH_4(MSG_LEGACY_HIGH, "DLR:TRM START_STATE granted,LTE pridev=%d,LTE divdev=%d,input.tID=%d,output.tID=%d",
                 trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device,
                 trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].diversity_device,
                 trm_set_client_state_input.data.irat_data.transaction_id,
                 trm_set_client_state_output.transaction_id
                );

  freq_entry_ptr->trm_set_client_state_input = trm_set_client_state_input;
  freq_entry_ptr->trm_set_client_state_output = trm_set_client_state_output;
  /* Save parameters for TRM set_client_state API */

  return;

} /* hdrsrchtclte_get_lte_freq_trm */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_TRESELECT_TIMER_UPDATE

DESCRIPTION 
  This function manages the Treselect timer of the LTE frequency.  If a timer
  is active and the time duration is exceeded, the timer is reset.
 
DEPENDENCIES
  None
 
PARAMETERS 
  None 
 
RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_treselect_timer_update( void )
{

  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  uint8                                      idx = 0;
    /* Loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( hdrsrchtclte.earfcn_arr.arr_cnt == 0 )
  {
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
      /* Reset current frequency ptr, at this point there is nothing to measure */
    return;
  }

  curr_time_sclks = timetick_get();
  /* Get current time in sclk units */

  /* Look for frequencies that have treselect timer which may have expired. */
  freq_entry_ptr = hdrsrchtclte.earfcn_arr.arr;

  for( idx = 0; 
       ( idx < hdrsrchtclte.earfcn_arr.arr_cnt ) &&
       ( idx < HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ); 
       idx++ )
  {
    if( freq_entry_ptr->treselect_state == HDRSRCHTCLTE_TIMER_ACTIVE )
    {
      /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
         Note: This only works for large unsigned integers where the delta
         between time 1 < time 2 is smaller than half of the
         number set.  These 2 time values are uint32 values. */
      if( TIME_OCCURS_FIRST(freq_entry_ptr->treselect_exp_sclks, curr_time_sclks) )
      {
        HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "DLR:treselect_timer_update(),EARFCN=%d timer expired",
                       freq_entry_ptr->earfcn);
        freq_entry_ptr->treselect_state = HDRSRCHTCLTE_TIMER_INACTIVE;
        /* Treselect timer has expired, the frequency can be measured
           again. */
      }
      else
      {
        HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "DLR:treselect_timer_update(),EARFCN=%d timer active",
                       freq_entry_ptr->earfcn);
      }
    }

    freq_entry_ptr++;
  } /* end for loop */

} /* hdrsrchtclte_treselect_timer_update */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_TBACKOFF_TIMER_UPDATE

DESCRIPTION 
  This function manages the Tbackoff timer of the LTE frequency.  If a timer
  is active and the time duration is exceeded, the timer is reset.
 
DEPENDENCIES
  None
 
PARAMETERS 
  None 
 
RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_tbackoff_timer_update( void )
{

  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */
  hdrsrchtclte_earfcn_struct_type            *freq_entry_ptr = NULL;
    /* Temp pointer for convenience */
  uint8                                      idx = 0;
    /* Loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( hdrsrchtclte.earfcn_arr.arr_cnt == 0 )
  {
    hdrsrchtclte.curr_freq_entry_ptr = NULL;
      /* Reset current frequency ptr, at this point there is nothing to measure */
    return;
  }

  curr_time_sclks = timetick_get();
  /* Get current time in sclk units */

  /* Look for frequencies that have tbackoff timer which may have expired. */
  freq_entry_ptr = hdrsrchtclte.earfcn_arr.arr;

  for( idx = 0; 
       ( idx < hdrsrchtclte.earfcn_arr.arr_cnt ) &&
       ( idx < HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ); 
       idx++ )
  {
    if( freq_entry_ptr->tbackoff_state == HDRSRCHTCLTE_TIMER_ACTIVE )
    {
      /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
         Note: This only works for large unsigned integers where the delta
         between time 1 < time 2 is smaller than half of the
         number set.  These 2 time values are uint32 values. */
      if( TIME_OCCURS_FIRST(freq_entry_ptr->tbackoff_exp_sclks, curr_time_sclks) )
      {
        HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "DLR:tbackoff_timer_update(),EARFCN=%d timer expired",
                     freq_entry_ptr->earfcn);
        freq_entry_ptr->tbackoff_state = HDRSRCHTCLTE_TIMER_INACTIVE;
        /* Treselect timer has expired, the frequency can be measured
           again. */
      }
      else
      {
        HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "DLR:tbackoff_timer_update(),EARFCN=%d timer active",
                       freq_entry_ptr->earfcn);
      }
    }

    freq_entry_ptr++;
  } /* end for loop */

} /* hdrsrchtclte_tbackoff_timer_update */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_LTE_NSET_CMD_UPDATE

DESCRIPTION 
  This function updates the LTE frequency list and associated parameters.
 
DEPENDENCIES
  None
 
PARAMETERS 
  None 
 
RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_lte_nset_cmd_update( void )

{
  uint8                                       idx = 0;
    /* Loop counter */
  hdrsrch_lte_nset_update_redir_params_type   *lte_nset_redir_ptr;
    /* Ptr to command */
  hdrsrchtclte_lteinit_enum_type              saved_init_cnf_ind;
    /* Saved copy of LTE INIT/DEINIT */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:update_lte_nset()" );

  if( hdrsrchtclte.update_list_ind == FALSE )
  {
     return;
       /* Nothing to update */
  }

  hdrsrchtclte.update_list_ind = FALSE;
    /* Updating now */

  lte_nset_redir_ptr = &hdrsrchtclte_saved_params.redir_params;
    /* Already saved the parameter list, now read from it */

  /* Sanity check array boundaries */
  if ( ( lte_nset_redir_ptr->earfcn_cnt == 0 ) ||
       ( lte_nset_redir_ptr->earfcn_cnt > HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Invalid EARFCN count=%d", lte_nset_redir_ptr->earfcn_cnt );
    return;
  }
  
  saved_init_cnf_ind = hdrsrchtclte.init_cnf_ind;
    /* Save away the INIT indicator as it should be transparent to command updates */

  memset( &hdrsrchtclte, 0, sizeof( hdrsrchtclte ) );
    /* Initialize LTE redirection control information */

  hdrsrchtclte.init_cnf_ind = saved_init_cnf_ind;
    /* Restore indicator */

  /* Range checks */
  if( lte_nset_redir_ptr->min_rpt_int > HDRSRCHTCLTE_MAX_MIN_RPT_INT_MINUTES )
  {   
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Invalid min report interval, set to %d",
                    HDRSRCHTCLTE_DEFAULT_MIN_RPT_INT_MINUTES );
    hdrsrchtclte.actual_rpt_params.min_rpt_int_minutes = HDRSRCHTCLTE_DEFAULT_MIN_RPT_INT_MINUTES;
  }
  else
  {
  hdrsrchtclte.actual_rpt_params.min_rpt_int_minutes = lte_nset_redir_ptr->min_rpt_int;
  }

  if( lte_nset_redir_ptr->max_rpt_cnt > HDRSRCHTCLTE_MAX_OF_MAX_RPT_CNT )
  {   
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Invalid max report count, set to %d",
                    HDRSRCHTCLTE_MAX_OF_MAX_RPT_CNT );
    hdrsrchtclte.actual_rpt_params.max_rpt_cnt = HDRSRCHTCLTE_MAX_OF_MAX_RPT_CNT;
  }
  else
  {
  hdrsrchtclte.actual_rpt_params.max_rpt_cnt = lte_nset_redir_ptr->max_rpt_cnt;
  }

  hdrsrchtclte.actual_rpt_params.thresh_serving = lte_nset_redir_ptr->serving_thresh_trig & HDRSRCHTCLTE_THRESH_SERVING_MASK;
  hdrsrchtclte.actual_rpt_params.threshx        = ( int16 ) ( lte_nset_redir_ptr->threshx_trig & HDRSRCHTCLTE_THRESHX_MASK );
  hdrsrchtclte.actual_rpt_params.threshxq       = ( int16 ) ( lte_nset_redir_ptr->threshxq_trig & HDRSRCHTCLTE_THRESHX_MASK );

  HDR_MSG_SRCH_5( MSG_LEGACY_MED, 
                  "DLR:Redirect parms from CP:MinRptInt=%d,MaxCnt=%d,SrvThresh=%d,ThreshX=%d,ThreshXQ=%d",
                  lte_nset_redir_ptr->min_rpt_int,   
                  lte_nset_redir_ptr->max_rpt_cnt,   
                  lte_nset_redir_ptr->serving_thresh_trig,
                  lte_nset_redir_ptr->threshx_trig,       
                  lte_nset_redir_ptr->threshxq_trig );     

  HDR_MSG_SRCH_5( MSG_LEGACY_MED, 
                  "DLR:Actual redirect parms used:MinRptInt=%d,MaxCnt=%d,SrvThresh=%d,ThreshX=%d,ThreshXQ=%d",
                    hdrsrchtclte.actual_rpt_params.min_rpt_int_minutes,   
                    hdrsrchtclte.actual_rpt_params.max_rpt_cnt,   
                    hdrsrchtclte.actual_rpt_params.thresh_serving,
                    hdrsrchtclte.actual_rpt_params.threshx,       
                    hdrsrchtclte.actual_rpt_params.threshxq );     

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:EARFCN count = %d", lte_nset_redir_ptr->earfcn_cnt );
  hdrsrchtclte.earfcn_arr.arr_cnt = lte_nset_redir_ptr->earfcn_cnt;
    /* Save number of LTE EARFCNs */

  for( idx = 0; 
       ( idx < hdrsrchtclte.earfcn_arr.arr_cnt ) &&
       ( idx < HDRCOM_SRCH_LTE_FREQ_ARR_SIZE ); 
       idx++ )
  {
    hdrsrchtclte.earfcn_arr.arr[ idx ].earfcn = lte_nset_redir_ptr->earfcn_arr[ idx ];

    if(hdrsrchtclte_is_in_fast_lte_srch_mode() == TRUE)
    {
      hdrsrchtclte.earfcn_arr.arr[ idx ].tbackoff_duration_sec = lte_nset_redir_ptr->tbackoff_earfcn_arr[ idx ];

      if(lte_nset_redir_ptr->tbackoff_reset_required[idx])
      {
        if(hdrsrchtclte.earfcn_arr.arr[ idx ].tbackoff_state == HDRSRCHTCLTE_TIMER_ACTIVE)
        {
           HDR_MSG_SRCH_1( MSG_LEGACY_MED,"Resetting Tbackoff for EARFCN %d from ACTIVE to INACTIVE",
     	                  hdrsrchtclte.earfcn_arr.arr[ idx ].earfcn);
           hdrsrchtclte.earfcn_arr.arr[ idx ].tbackoff_state = HDRSRCHTCLTE_TIMER_INACTIVE;
        }
      }
	  
      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "FAST_LTE_SRCH DLR:EARFCN=%d T_Backoff: %d sec T_backoff State %d", 
                     hdrsrchtclte.earfcn_arr.arr[ idx ].earfcn,
                     hdrsrchtclte.earfcn_arr.arr[ idx ].tbackoff_duration_sec,
                     hdrsrchtclte.earfcn_arr.arr[ idx ].tbackoff_state);
    }
    else
    {
    
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:EARFCN=%d", hdrsrchtclte.earfcn_arr.arr[ idx ].earfcn );
  }
  }

  if ( hdrutil_fs_get( HDRSRCHTCLTE_TRESELECT_SEC,
                &hdrsrchtclte.treselect_duration_sec, 
                sizeof( hdrsrchtclte.treselect_duration_sec ) ) == 
                MCFG_FS_STATUS_OK )

  {
    /* If EFS file is available, perform range check to avoid overflow */
    if ( hdrsrchtclte.treselect_duration_sec > HDRSRCHTCLTE_TRESELECT_DEFAULT_MAX_SEC )
    {
      hdrsrchtclte.treselect_duration_sec = HDRSRCHTCLTE_TRESELECT_DEFAULT_MAX_SEC;
    }
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR:EFS Treselect=%d,capped at %d", hdrsrchtclte.treselect_duration_sec,
                    HDRSRCHTCLTE_TRESELECT_DEFAULT_MAX_SEC );
  }
  else
  {
    hdrsrchtclte.treselect_duration_sec = HDRSRCHTCLTE_TRESELECT_DEFAULT_SEC;
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR:Default Treselect=%d,capped at %d", hdrsrchtclte.treselect_duration_sec,
                    HDRSRCHTCLTE_TRESELECT_DEFAULT_MAX_SEC );
  }

  if ( hdrutil_fs_get( HDRSRCHTCLTE_TBACKOFF_SEC,
                &hdrsrchtclte.tbackoff_duration_sec, 
                sizeof( hdrsrchtclte.tbackoff_duration_sec ) ) == 
                MCFG_FS_STATUS_OK )

  {
    /* If EFS file is available, perform range check to avoid overflow */
    if ( hdrsrchtclte.tbackoff_duration_sec > HDRSRCHTCLTE_TBACKOFF_DEFAULT_MAX_SEC )
    {
      hdrsrchtclte.tbackoff_duration_sec = HDRSRCHTCLTE_TBACKOFF_DEFAULT_MAX_SEC;
    }
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR:EFS Tbackoff=%d, capped at %d", hdrsrchtclte.tbackoff_duration_sec,
                    HDRSRCHTCLTE_TBACKOFF_DEFAULT_MAX_SEC );
  }
  else
  {
    hdrsrchtclte.tbackoff_duration_sec = HDRSRCHTCLTE_TBACKOFF_DEFAULT_SEC;
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DLR:Default Tbackoff=%d, capped at %d", hdrsrchtclte.tbackoff_duration_sec,
                    HDRSRCHTCLTE_TBACKOFF_DEFAULT_MAX_SEC );
  }

  if ( hdrutil_fs_get( HDRSRCHTCLTE_MEAS_CYCLE_CNT,
                &hdrsrchtclte.meas_cycle_cnt, 
                sizeof( hdrsrchtclte.meas_cycle_cnt ) ) == 
                MCFG_FS_STATUS_OK )

  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:EFS MeasCycleCnt=%d", hdrsrchtclte.meas_cycle_cnt );
  }
  else
  {
    hdrsrchtclte.meas_cycle_cnt = HDRSRCHTCLTE_DEFAULT_MEAS_CYCLE_CNT;
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:Default MeasCycleCnt=%d", hdrsrchtclte.meas_cycle_cnt );
  }

  hdrsrchtclte.redirect_cb = lte_nset_redir_ptr->redirect_cb;
    /* Save call back function for redirection reporting */

  return;

} /* hdrsrchtclte_lte_nset_cmd_update */

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
  uint16                                       avail_ms,
  uint16                                       rampdown_ms
)

{
  boolean                                      ret_val = FALSE; 
  timetick_type                                curr_time_sclks;
    /* Current time in sclks units */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "DLR:request_trm_grant(),avail_ms=%d,rampdown_ms=%d", avail_ms,rampdown_ms );

  HDR_ASSERT( avail_ms <= HDRSRCHTCLTE_MEAS_CYCLE_MAX_DURATION_MS );
  HDR_ASSERT( rampdown_ms <= HDRSRCHTCLTE_RAMPDOWN_MAX_DURATION_MS );
    /* Sanity check max duration for LTE measurement */

  if( ( hdrsrchtclte.curr_freq_entry_ptr != NULL ) &&
      ( hdrsrchtclte.curr_freq_entry_ptr->trm_set_client_state_output.grant ==
                                                                TRM_GRANTED ) )
  {
    ret_val = TRUE;
    return ret_val;
    /* If we already have the TRM grant, indicate so and return. */
  }

  hdrsrchtclte.curr_freq_entry_ptr = NULL;
    /* Reset current frequency ptr, at this point there is nothing to measure */

  if( ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) && 
      ( hdrsrchtclte.update_list_ind == FALSE ) )
  {
    return ret_val;
  }

  
#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( !hdrsrchrftxd_suspend_asdiv ( TRUE, TRM_ASDIV_STATE_UPDATE_REASON_IRAT ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "DLR:AS Div cannot be suspended, do not measure LTE" );
    return ret_val;
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrchtclte_lte_nset_cmd_update();
    /* Received new LTE NSET command? */

  hdrsrchtclte_treselect_timer_update();
    /* Check for Treselect timer expiry, the frequency with expired
       Treselect timer requires special handling */

  hdrsrchtclte_tbackoff_timer_update();
    /* Check for Tbackoff timer expiry, the frequency with expired
       Tbackoff timer is eligible for pilot measurements */

  hdrsrchtclte.avail_ms = avail_ms;
  hdrsrchtclte.rampdown_ms = rampdown_ms;
    /* Save time intervals for TRM reservations */

  curr_time_sclks = timetick_get();
    /* Get current time in unit of sclks */

  hdrsrchtclte.meas_end_time_sclks = curr_time_sclks + 
    TIMETICK_SCLK_FROM_MS ( hdrsrchtclte.avail_ms ) + TIMETICK_SCLK_FROM_MS ( hdrsrchtclte.rampdown_ms );
    /* Store the time when HDR must be finished with LTE meas */

  hdrsrchtclte_get_lte_freq_trm( TRUE );
    /* Get the LTE freq and TRM extension.  TRM is granted per LTE frequency.
       Indicate that this is the first measurement for this cycle */

  if( hdrsrchtclte.curr_freq_entry_ptr != NULL )
  {
    ret_val = TRUE;
      /* A LTE frequency is ready to be measured */

  }
  else 
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "DLR:No LTE meas, set SUSPEND ASDIV to FALSE" );
    /* At this point, ASDIV has been disabled for LTE meas.
       Since we do not perform LTE meas, immediately reenable ASDIV.
      (If we do perform LTE meas, ASDIV is re-enabled after LTE meas is done.) */
    
    (void) hdrsrchrftxd_suspend_asdiv( FALSE, TRM_ASDIV_STATE_UPDATE_REASON_IRAT );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  }

  if ( ret_val == TRUE )
  {
    hdrsrchtclte_sanity_timer_set ( avail_ms );
      /* Start sanity timer to abort measurement if LTE takes too long.  Always do this
         last. */
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DLR:request_trm_grant() returns:%d",ret_val );

  return ret_val;

} /* hdrsrchtclte_request_trm_grant */


/*===========================================================================

FUNCTION HDRSRCHTCLTE_ENTER_TCLTE_STATE

DESCRIPTION 
  This function is called upon entering LTE meas state.  This function prepares
  DO for LTE measurements, including RF preparation as well as sending LTE
  Init request message if DO has not done so.
 
DEPENDENCIES
  None
 
PARAMETERS 
  None
 
RETURN VALUE 
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_enter_tclte_state( void )

{
  errno_enum_type                              status = E_SUCCESS;
    /* Check for error, if any */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:enter_tclte_state()" );

  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN to start measurement" );

    hdrsrchtc_init_connected_state();
      /* Done with LTE pilot measurement state, back to traffic */
    return;
  }

  hdrsrchtclte.earfcn_meas_this_cycle_cnt = 1;
    /* For debugging purposes, keep tract of how many EARFCN DO measures */

  /**************************************/
  /* Do NOT change order of calls below */
  /**************************************/

  /* Allocate RXLM buffers for LTE RF tuning.  LTE always uses both chains. */
  if ( hdrsrchtclte.rxlm.lte_handle_valid != TRUE )
  {
    rxlm_allocate_buffer( RXLM_CHAIN_0, LM_LTE,
                          &hdrsrchtclte.rxlm.lte_buf_chain0 );
    rxlm_allocate_buffer( RXLM_CHAIN_1, LM_LTE,
                          &hdrsrchtclte.rxlm.lte_buf_chain1 );
    hdrsrchtclte.rxlm.lte_handle_valid = TRUE;
  }

  /* Get DO RXLM buffer handle, which should have been allocated at this
     point */
  hdrsrchtclte.rxlm.do_handle_valid = 
    hdrsrchrf_get_rxlm_buffer_handle( HDRSRCHRF_PRI_CHAIN, &hdrsrchtclte.rxlm.do_buf_chain0 ) ;

  HDR_ASSERT ( ( hdrsrchtclte.rxlm.do_handle_valid == TRUE ) &&
           ( hdrsrchtclte.rxlm.lte_handle_valid == TRUE ) );

#ifndef FEATURE_HDR_D2L_RF_MEAS
  status = hdrsrchtclte_rfm_meas_enter();
    /* Notify RF to enter LTE Pilot measurement state. */
#endif /* !FEATURE_HDR_D2L_RF_MEAS */

  if( status != E_SUCCESS )
  {
    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
    return;
  }

  hdrsrchmcpm_do2lte_measurement( TRUE );
    /* Notify DO to LTE measurement START status to MCPM */

  hdrsrchtclte_build_rf_tuneback_script();
    /* Build RF tune back script to tune back to DO after LTE pilot measurement */

  if( hdrsrchtclte.init_cnf_ind == HDRSRCHTCLTE_LTEINIT_RXED )
    /* Already processed INIT confirmation */
  {
    hdrsrchtclte_start_lte_meas();
      /* Begin the process to measure the first lte frequency this cycle */
  }
  else if( hdrsrchtclte.init_cnf_ind == HDRSRCHTCLTE_LTEINIT_NONE )
  {
    lte_ml1_irat_gap_init( 
#ifdef FEATURE_HDR_MSGR_VARIANT 
           hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
           &hdrsrchtclte.gap_state );
      /* LTE requires this call before starting measurements */

    /* Send Init req message, resume after confirmation is received */
    hdrsrchmsg_d2l_send_init_req_msg();
    hdrsrchtclte.last_sent = HDRSRCHTCLTE_INIT_REQ;
    hdrsrchtclte.redir_state = HDRSRCHTCLTE_INIT_STATE;
    hdrsrchtclte.init_cnf_ind = HDRSRCHTCLTE_LTEINIT_SENT;
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "DLR:Possible mismatch of LTE Init,cnf_rxed=%d", 
                    hdrsrchtclte.init_cnf_ind );

    hdrsrchtclte_exit_tclte_state();
      /* Exit LTE measurement state machine, back to traffic */
  }

} /* hdrsrchtclte_enter_tclte_state */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_INIT

DESCRIPTION 
  This function is called to transition to LTE meas state.  When done, DO 
  always returns to Traffic (Connected) state.  This function may be called in
  Traffic Suspend or Traffic state.
 
DEPENDENCIES
  None
 
PARAMETERS 
  Last state, which may be Traffic or Traffic Suspend state
 
RETURN VALUE 
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_init 
( 
  hdrsrch_state_enum_type               last_state
)

{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch_default_init( last_state );
    /* Use default handling between states */

  if( ( hdrsrchtclte.curr_freq_entry_ptr == NULL ) ||
      ( hdrsrchtclte.earfcn_arr.arr_cnt == 0 ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Cannot locate EARFCN to start measurement" );

    hdrsrchtc_init_connected_state();
      /* Done with LTE pilot measurement state, back to traffic */
    return;
  }

  hdrsrchtclte_enter_tclte_state();
    /* Enter state to make LTE measurements */
  
} /* hdrsrchtclte_init */

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
boolean hdrsrchtclte_send_deinit_req( void )

{

  boolean                                      request_sent = FALSE;
    /* Indicate whether or not the request is sent */
    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrsrchtclte.init_cnf_ind == HDRSRCHTCLTE_LTEINIT_RXED )
    /* If DO has received confirmation of LTE Init request */
  {
    if( hdrsrch_state_id == HDRSRCH_INACTIVE_STATE ) 
      /* If called from Inactive state, turn on MCPM clocks for LTE */
    {

      HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:hdrsrchmcpm_do2lte_measurement( TRUE )" );
      hdrsrchmcpm_do2lte_measurement( TRUE );
        /* DO is not in Meas state, turn on MCPM clocks for LTE before sending
           Deinit request. */
    }

    /* Send Deinit req message. */
    hdrsrchtclte.last_sent = HDRSRCHTCLTE_DEINIT_REQ;

    if( hdrsrchtclte.redir_state != HDRSRCHTCLTE_ABORT_STATE ) 
    {
      hdrsrchtclte.redir_state = HDRSRCHTCLTE_DEINIT_STATE;
        /* Only switch meas state if we are not aborting.  Otherwise, leave
           the  abort state as is for the processing of the deinit confirmation
           message */
    }

    hdrsrchmsg_d2l_send_deinit_req_msg();

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Sent DLR:hdrsrchmsg_d2l_send_deinit_req_msg state %d",hdrsrch_state_id );
	  
    if( hdrsrch_state_id == HDRSRCH_INACTIVE_STATE ) 
      /* If called from Inactive state, only then start timer for 50ms */
    {
      hdrsrchtclte_sanity_timer_set(HDRSRCHTCLTE_DURATION_LTE_DEINIT_SANITY_MS);
    }
    hdrsrchtclte.init_cnf_ind = HDRSRCHTCLTE_LTEDEINIT_SENT;
    request_sent = TRUE;
  }

  return request_sent;

} /* hdrsrchtclte_send_deinit_req */

/*===========================================================================

FUNCTION HDRSRCHTCLTE_EXIT_TCLTE_STATE

DESCRIPTION
  This function exits the LTE Measurement state machine.  This state will
  only transition to Traffic state, even though DO gets to this state from
  either Traffic or Traffic Suspend state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrsrchtclte_exit_tclte_state( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "DLR:exit_tclte_state()" );

  hdrsrchtclte.redir_state = HDRSRCHTCLTE_INACTIVE_STATE;
    /* Reset sub state for LTE measurements */

  hdrsrchtclte_sanity_timer_clear();
    /* Clear sanity timer as we are exiting TCLTE state */

#ifdef FEATURE_HDR_D2L_RF_MEAS
  if (hdrsrchtclte.is_rfm_meas_entered)
  {
    hdrsrchtclte_rfm_meas_exit();
      /* Call RF tune back function after LTE measurements */
  }
#else /* FEATURE_HDR_D2L_RF_MEAS */
  hdrsrchtclte_rfm_meas_exit();
    /* Call RF tune back function after LTE measurements */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

  hdrsrchtclte_end_lte_meas();
    /* Release the TRM grant provided for DLR measurement, if it has not been released */

  /* Deallocate RXLM buffers for LTE.  LTE always uses both chains. */
  if ( hdrsrchtclte.rxlm.lte_handle_valid == TRUE )
  {
    rxlm_deallocate_buffer ( hdrsrchtclte.rxlm.lte_buf_chain0 );
    rxlm_deallocate_buffer ( hdrsrchtclte.rxlm.lte_buf_chain1 );

    hdrsrchtclte.rxlm.lte_handle_valid = FALSE;
  }

  hdrsrchmcpm_do2lte_measurement( FALSE );
    /* Notify DO to LTE measurement done status to MCPM */

  hdrsrchtc_init_connected_state();
    /* Done with LTE pilot measurement state, back to traffic */

} /* hdrsrchtclte_exit_tclte_state */

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
boolean hdrsrchtclte_lte_deinit_is_finished( void )
{

  boolean                                    result = FALSE;
    /* Status of LTE deinit operation  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrsrchtclte.init_cnf_ind == HDRSRCHTCLTE_LTEINIT_NONE )
  {
    result = TRUE;
  }

  return result;

} /* hdrsrchtclte_lte_deinit_is_finished */

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
boolean hdrsrchtclte_serving_threshold_is_triggered( void )
{

  boolean                                    result = FALSE;
    /* Return value  */
  int16                                      ec_io_lin = 0;   
    /* HDR filtered signal strength in lin unit */
  uint16                                     ec_io_half_db = 0;   
    /* Signal strength converted to .5dB unit using lookup table
       ps2eng_table */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Only consider LTE measurement if HDR signal strength is <
     ThreshServing threshold */

  hdrsrchutil_get_ecio_and_io( &ec_io_lin, NULL );
    /* Find the maximum ecio among all active set sectors (filtered value) */

  ec_io_half_db = ( uint16 )( hdrsrchset_eng_to_ecio( ec_io_lin ) );
    /* Convert to -.5dB units using lookup table ps2eng_table */

  /* ThreshServing description per 3GPP2 C.S0087 spec:

     The access network shall set this field to [-2 * 10 * log10PS], where PS
     is the strength of the reference pilot of the serving Access network.

     Both EcIo and ThreshServing are represented as positive numbers even
     though they are negative numbers.  Thus, the "<" in the evaluation:
     RSRP of Serving C2K < ThreshServing
     is changed to ">" in the SW implementation. */

  if( ec_io_half_db > ( uint16 )hdrsrchtclte.actual_rpt_params.thresh_serving )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
      "DLR:Do LTE meas, Serving < ThreshServing, lin=%d | ServingEng=%d,thresh=%d (in -.5dB unit)", 
      ec_io_lin,
      ec_io_half_db,
      hdrsrchtclte.actual_rpt_params.thresh_serving );
    result = TRUE;
  }
  else
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
      "DLR:No LTE meas, Serving >= ThreshServing, lin=%d,ServingEng=%d,thresh=%d (in -.5dB unit)", 
      ec_io_lin,
      ec_io_half_db,
      hdrsrchtclte.actual_rpt_params.thresh_serving );
  }

  return result;

} /* hdrsrchtclte_serving_threshold_is_triggered */

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
boolean hdrsrchtclte_min_rpt_interval_is_triggered( void )
{
  boolean                                    result = TRUE;
    /* Return value */
  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */
  timetick_type                              prev_rpt_age_sclks = 0;
    /* Age of previous report in unit of sclks */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  curr_time_sclks = timetick_get();
    /* Get current time in unit of sclks */

  if( hdrsrchtclte.rpt.rpt_cnt == 0 )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,"DLR:there is no past report on record" );
    hdrsrchtclte.rpt.rpt_timestamp_sclks = 0;
      /* There is no report yet */
    result = FALSE;
  }
  else
  {
      /* Calculate age of the previous measurement in unit of sclks */
    prev_rpt_age_sclks = hdrsrchtclte.rpt.rpt_timestamp_sclks + 
                         ( ( hdrsrchtclte.actual_rpt_params.min_rpt_int_minutes * 60 ) * TIMETICK_NOMINAL_FREQ_HZ );

    if( TIME_OCCURS_FIRST( prev_rpt_age_sclks, curr_time_sclks ) )
    {
      result = FALSE;
    }
  }

  HDR_MSG_SRCH_5( MSG_LEGACY_HIGH,"DLR:min_rpt_interval_is_triggered returns:%d,rpt_cnt=%d,min_rpt_int_minutes=%d,prev(sclks)=%u,curr=%u",
                  result,
                  hdrsrchtclte.rpt.rpt_cnt,
                  hdrsrchtclte.actual_rpt_params.min_rpt_int_minutes,
                  prev_rpt_age_sclks,
                  curr_time_sclks );
  return result;
  
} /* hdrsrchtclte_min_rpt_interval_is_triggered */

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
void hdrsrchtclte_release_trm_grant( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Null ptr check */
  if( hdrsrchtclte.curr_freq_entry_ptr == NULL )
  {
    return;
  }

  /* Avoid calling TRM with TRM_END_STATE if request was not granted */
  if ( hdrsrchtclte.curr_freq_entry_ptr->trm_set_client_state_output.grant == TRM_GRANTED )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DLR:Make sure TRM GRANTED is reset at end of call" );
  }

  hdrsrchtclte_end_lte_meas();
    /* Release the TRM grant provided for DLR measurement,
       as we are not going to make measurement. */

} /* hdrsrchtclte_release_trm_grant */
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
boolean hdrsrchtclte_is_in_fast_lte_srch_mode( void )
{

   HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,"DO in FAST LTE SRCH MODE %d",
                  hdrsrchtclte_saved_params.redir_params.is_fast_lte_search);

  return hdrsrchtclte_saved_params.redir_params.is_fast_lte_search;

}

#endif /* FEATURE_HDR_TO_LTE_REDIRECTION */

