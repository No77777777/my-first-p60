/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
   
         H D R    S R C H    L T E    P I L O T   M E A S U R E M E N T 
   
                   Search LTE Pilot Measurement State Module
   
GENERAL DESCRIPTION 
  This file contains the HDR Searcher LTE Pilot Measurement state machine.
  LTE pilot measurements are requested by the HDR technology to enble reselection
  from HDR to LTE.  HDR->LTE reselection is specified in the 3GPP2 C.S0087 spec.
  The procedure executes while DO is sleeping at every SCI cycle and contains the
  following steps:
 
  - At the start of sleep state, DO decides to request an LTE measurement.
  - LTE tunes to the desired frequency and does the measurement.
  - DO runs the reselection criteria on the obtained LTE measurement.
  - If decision is made to reselect to LTE, LTE module tries to acquire on the chosen
    frequency.
  - If LTE measurement does not pass reselection criteria, DO performs measurement
    on the next LTE frequency in list.
  - Continue until list is exhausted without interfering with normal DO wake up.
 
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

  Copyright (c) 2010 - 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchltemeas.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
11/24/17   vlc     Added support for FR34448 NULL2L IRAT measurement.
08/08/17   svu     Fixed compiler error by including as_id for LTE API
08/03/17   vko     HDR to pass IRAT sub id to L during meas request,
                          to support L + L config
08/03/17   vko     MSGR Instance ID to Variant ID change
10/09/15   vlc     Added code to handle meas abort before meas request is sent.
06/26/15   vke     Removed handling HDRSRCH_BAND_GRANT_RCVD_CMD
05/28/15   vlc     Added support for DO->LTE redirection in connected state.
05/19/15   rmv     Called LTE/RF exit API to cover D2L abort scenarios
05/11/15   vlc     Replaced deprecated API trm_release() with trm_release_chain(). 
05/07/15   vlc     Added hdrsrchltemeas_abort_lte_reselection(). 
04/13/15   vlc     Added RF API changes for D2L. 
02/18/15   sat     Adding bring up changes for D2L measurement using RUMI
02/13/15   sat     Changes to cancel Band grant command in LTE meas state
09/23/14   vlc     Send TRM End_state and unlock IRATMan node when receiving 
                   INIT cnf in abort state. 
09/17/14   vlc     Added setting for TRM should_change_reason_to_irat flag. 
09/16/14   vlc     Send TRM End_state in case of Abort. 
09/16/14   vlc     Always use TRM set_client_state API for D2L meas. 
09/11/14   vlc     Added changes to pass pri/div device IDs to LTE. 
09/11/14   vlc     Changed criteria to put LTE freq in less preferred list. 
09/11/14   vlc     Fixed incorrect use of LTE band. Cleaned up F3 messages 
06/27/14   sat     Initializing IRATMan on every HDR Activation.
08/12/14   tnp     Fix compiler warnings
08/08/14   sat     Updating LTE measurement across Traffic state
08/07/14   vlc     Implemented changes for DR-DSDS. 
07/29/14   tnp     FR22375: Fix Medium compiler warnings in HDR
06/02/14   vlc     Changed TRM request sequence for D2L measurements. 
02/26/14   vko     FR 17879: Extended EARFCN
04/10/14   vlc     Do not send clean up message with Timed Search interface. 
                   Fixed typo in ASSERT. 
01/16/14   cnx     Read JCDMA mode from 1x JCDMA mode NV.
01/07/14   vlc     Moved IRATMan initialization call to the first time HDR 
                   enters LTE meas state.
11/29/13   sat     NV control for Lock Release Enhancement (LRE)
01/10/14   vlc     Added new settings for RF in hdrsrchltemeas_rfm_meas_exit().
01/03/14   sat     Fixed an instance of wrong memcpy usage 
12/20/13   vlc     Added support for Tbackoff timer optimization. 
12/19/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/18/13   vlc     Made minor changes and fixed up debug messages. 
10/29/13   sat     Reduced F3 messages (or/and) ASSERTs
10/11/13   vlc     Added support for new LTE TIMED_SEARCH_MEAS_REQ API.
08/01/13   smd     Used new secapi to get random number.
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
09/03/13   vlc     Removed TRM reservation for LTE by HDR.  Added new call to 
                   LTE function to reverve TRM before reselection. 
06/26/13   rkc     Handle TRM unlock immediate but not unlock by. 
06/26/13   rkc     Call TRM retain lock after entering state 39
05/21/13   cnx     Add deprioritized LTE freq to less preferred list when 
                   priority is not included.
03/31/13   cnx     Support LTE deprioritization.
02/25/13   vlc     Provide LTE additional RF device parameter. 
02/06/13   vlc     Changed TRM_UMTS to TRM_LTE. 
01/28/13   vlc     Adjusted QualLevMinEUTRAOffset range from 0 - 7 to 1 - 8.   
01/25/13   vlc     Modified randomized timer calculation to avoid overflow. 
09/24/12   vlc     Added abort handling when processing Deinit confirmation. 
09/25/12   rmg     Added support for including freq info in TRM interaction.
07/30/12   vlc     Moved call to send Deinit request prior to exiting LTE meas 
                   state. 
07/13/12   vlc     Removed LTE deinit functionality in DO Inactive state to 
                   allow for Hybrid LTE reselection.  Deinit LTE at end of
                   meas cycle.
07/09/12   dsp     Removed unnecessary F3s. 
05/21/12   ljl     Fixed compiler error. 
05/17/12   vlc     Added code to reset LTE meas parameters in function 
                   hdrsrchltemeas_send_deinit_req().
05/13/12   vlc     Adjusted RxLevMinEUTRAOffset range from 0 - 7 to 1 - 8.  
05/02/12   mbs/vlc Added D2L RSRQ parameters. 
05/02/12   vlc     Added code to provide thresh_serving to IRATMan.  Removed 
                   ASSERTs in hdrsrchltemeas_treselect_timer_exp() to work
                   with IRATMan interface. 
04/27/12   vlc     Added a check of the current state when exiting LTE Meas state.
04/16/12   vlc     Adjusted pe_max value range (dBm) from 0 - 63 to 
                   -30 - +33.
03/16/12   arm     Resolve TCB errors for Dime. 
03/05/12   vlc     Added IRATMan interface to D2L reselection feature.  
02/14/12   wsh     Added support for SVLTE+SHDR
12/22/11   vlc     Added code to call RF sleep after LTE meas for D2L. 
11/04/11   vlc     Added hdrsrchltemeas_lte_deinit_is_finished().
10/17/11   rmg     Removed inclusion of clkregim.h.
10/06/11   vlc     Saved Tx power from LTE Init cnf message
                   even if current LTE meas is being aborted.
10/06/11   vlc     Removed code to enable/disable FW app when entering and 
                   exiting LTE meas state.  RF does not require FW app to be
                   active for tuning.
09/29/11   vlc     Changed deinit request/confirmation handling to allow LTE
                   to shutdown LTE FW.  Added code to increment a counter in
                   LTE cell array handling.
09/09/11   vlc     Fixed typo in function 
                   hdrsrchltemeas_s_eutra_is_greater_than_threshx().
09/02/11   vlc     Moved 2 reselect cell buffers from control info struct to 
                   frequency info struct. 
07/18/11   vlc     Removed checking for LTE MEAS state to fix race condition. 
06/23/11   vlc     Added support for RF scripting and RXLM buffers. Fixed 
                   compiler warnings. 
05/09/11   kss     Fixed typo in previous check-in. 
05/06/11   vlc     Added code to use RF scripts for LTE measurements.  Call 
                   MCPM stop meas after RF tune back. 
04/27/11   rmg     Added HDR MCPM support changes.
03/29/11   vlc     Added code to maintain a list of cells for LTE reselection. 
03/23/11   vlc     Changed reset functions to accomodate different reset 
                   scenarios.  Cleared LTE meas state if Treselect timer expires
                   while DO is in traffic state.  Removed 1 unused function.
                   Removed obsolete test code.
03/17/11   vlc     Changed code to ignore Search confirmation and Measurement 
                   confirmation messages from LTE once an Abort request has
                   been sent.
03/15/11   vlc     Changed ThreshServing to units of .5dB.  Changed calculation 
                   of Rx Minimum level in reselection equation.
03/07/11   vlc     Removed code to reset then re-define Treselect timer and 
                   timer group.
03/02/11   vlc     Added hdrsrchltemeas_deinit_reset().  Fixed Treselect and 
                   Tbackoff calculation.  Reset measurement state after
                   receiving deinit confirmation.  Added debug messages.  
02/17/11   vlc     Changed deinit handling to be called from other HDR states. 
                   Added clean up code in search cnf processing in case of
                   abort.  Added TRM reservation for LTE prior to reselection.
01/27/11   vlc     Added code to delete saved commands/confirmations after 
                   coming back from DO->LTE reselection.  Also, need to re-
                   define Treselect timer.  Added function
                   hdrsrchltemeas_clear_reselect_status().
01/06/11   vlc     Switched messages from LTE for DO->LTE reselection to
                   low priority queue.
                   Removed references to hdrsrchmsg_rf_script_struct_type. 
11/09/10   vlc     Fixed 2nd measurement issue to get to reselection.  Handle 
                   case where RxMin Offset is not included.  Send clean up
                   request and deinit request before starting reselection.
                   Changed state before sending requests to LTE to avoid race
                   conditions.
10/27/10   vlc     Fixed bugs in building frequency lists and arrays.  Added 
                   code to turn on clocks before RF tune back, added more test 
                   capability, debug messages.  Added reselection call back
                   functionality.  Updated comments.
09/28/10   vlc     Added a test mode to LTE_MEAS state.  Added ability to
                   abort LTE meas while in MEAS sub-state.  Fixed bugs. 
09/15/10   vlc     Added RF tune back to hdrsrchltemeas_done() function. 
                   Added fixes to return to sleep gracefully.
08/16/10   vlc     Created file for DO->LTE pilot measurements/reselection. 

==========================================================================*/

/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"
#include "customer.h"
#ifdef FEATURE_HDR_TO_LTE
#include "hdrsrch.h"
#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchtask.h"
#include "hdrsrchcmd.h"
#include "hdrdebug.h"
#include "hdrutil.h"
#include "hdrsrchset.h"
#include "hdrsrchltemeas.h"
#include "hdrsrchmsg.h"
#include "hdrsrchrf.h"
#include "hdrsrchmcpm.h"

#include "ran.h"
#include "timetick.h"
#include "lte_cphy_irat_meas_msg.h"
#include "lte_ml1_irat_ext_api.h"
#include "trm.h"
#include "rxlm_intf.h"
#include "iratman.h"
#ifdef FEATURE_HDR_DIME_MODEM
#include "slpc.h"
#else
#include "sleepctl.h"
#endif /* FEATURE_HDR_DIME_MODEM */
#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */
#include "hdrmc_v.h"
#include "hdrmultirat.h"
#include "sys.h"
#include "lte_rrc_ext_api.h"
#include "lte_ml1_ext_api.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

#define HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_HIGH          96
  /* Per C.S0098 spec, the range for the RX_LEVEL_MIN_EUTRA is 0 - 96 */

#define HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_CEILING       -44
  /* Per C.S0098 spec, -44 - RxLevMinEUTRA  in dBm is equal to the minimum
     reference signal received power (RSRP) level of an EUTRA cell */

#define HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_OFFSET_LOW    1
#define HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_OFFSET_HIGH   8
  /* Per C.S0098 spec, the range for the RX_LEVEL_MIN_EUTRA is 1 - 8 */

#define HDRSRCHLTEMAS_PE_MAX_OFFSET                     -30
  /* Adjust pe_max from range of 0 to +63 dBm to range of -30 to +33 dBm
     (required because of different configuration on Network side.) */

#define HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_OFFSET_NONE   0
  /* The offset value is not included so set it to 0 (RX_LEV_MIN_EUTRA_OFFSET
     parameter is 0 when calculating S_EUTRA value for reselection) */

#define HDRSRCHLTEMEAS_QUAL_MIN_EUTRA_OFFSET_NONE       0
  /* The offset value is not included so set it to 0 (QUAL_MIN_EUTRA_OFFSET
     parameter is 0 when calculating RSRQ level of an EUTRA cell */

#define HDRSRCHLTEMEAS_T_RESELECT_TIMER_MS              5120 
  /* Treselect timer default value */

#define HDRSRCHLTEMEAS_CHIPS_PER_MS                     1229     
  /* How many chips per ms */

#define HDRSRCHLTEMEAS_T_RESELECT_TIMER_DISCARD_MS \
  ( HDRSRCHLTEMEAS_T_RESELECT_TIMER_MS * 2 )
  /* T-Reselect timer discard default value, used to guard against long
     Treselect timer once a new LTE neighbor list needs to be updated */

#define HDRSRCHLTEMEAS_T_BACKOFF_TIMER_MS \
  ( HDRSRCHLTEMEAS_T_RESELECT_TIMER_MS * 2 )
  /* Tbackoff timer default value */

#define HDRSRCHLTEMEAS_NULL_ENTRY                       0xFF
  /* Set the current array entry to this default value before the first measurement
     in LTE MEAS state.  This value indicates that measurements should start with
     the first entry in the array built for this measurement cycle. */

#define HDRSRCHLTEMEAS_MAX_MEAS_CELLS LTE_CPHY_IMEAS_MAX_MEAS_CELLS
  /* Maximum number of cells that can be measured in a single meas req */

#define HDRSRCHLTEMEAS_LTE_RF_STARTUP_BUFFER_INDEX      1
#define HDRSRCHLTEMEAS_DO_RF_CLEANUP_BUFFER_INDEX       0
  /* RF startup buffer index is 1 per instruction from LTE L1.
     RF cleanup buffer index should not be 1, and may not be used as DO RF support
     is not based on scripts. */

#define HDRSRCHLTEMEAS_IRATMAN_LTE_MAX_TRESELECT_DEFAULT \
  ( IRATMAN_LTE_MAX_TRESELECT_DEFAULT )
#define HDRSRCHLTEMEAS_IRATMAN_LTE_MIN_TBACKOFF_DEFAULT \
  ( IRATMAN_LTE_MIN_TBACKOFF_DEFAULT ) 
#define HDRSRCHLTEMEAS_IRATMAN_LTE_MAX_TBACKOFF_DEFAULT \
  ( IRATMAN_LTE_MAX_TBACKOFF_DEFAULT )

#define HDRSRCHLTEMEAS_MEAS_TIME_DELAY_MS               10
  /* Account for possible delay between TRM reservation and the first measurement.
     The delay is mainly due to RF prep for measurement, as well as LTE init request
     processing, especially for single frequency cases. */

#define HDRSRCHLTEMEAS_MEAS_TIME_TRM_REQ_DELAY_MS       2
  /* TRM may return a minimum value just a few sclks/ms less than Min duration for
     one LTE meas.  This is due to rounding error or elapsed time between the TRM
     call and when control is back to DO.  */

/* Request and command messages (via MSGR) from DO to LTE */
typedef enum
{
  HDRSRCHLTEMEAS_NULL_CMD,            /* 0 */
  HDRSRCHLTEMEAS_STARTUP_CMD,         /* 1 */
  HDRSRCHLTEMEAS_INIT_REQ,            /* 2 */
  HDRSRCHLTEMEAS_DEINIT_REQ,          /* 3 */
  HDRSRCHLTEMEAS_SEARCH_REQ,          /* 4 */
  HDRSRCHLTEMEAS_MEAS_REQ,            /* 5 */
  HDRSRCHLTEMEAS_CLEANUP_REQ,         /* 6 */
  HDRSRCHLTEMEAS_ABORT_REQ,           /* 7 */
  HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_REQ, /* 8 */
} 
hdrsrchltemeas_cmd_and_req_enum_type;

/* Confirmation messages (via MSGR) from LTE to DO */
typedef enum
{
  HDRSRCHLTEMEAS_NULL_CNF,            /* 0 */
  HDRSRCHLTEMEAS_INIT_CNF,            /* 1 */
  HDRSRCHLTEMEAS_DEINIT_CNF,          /* 2 */ 
  HDRSRCHLTEMEAS_SRCH_CNF,            /* 3 */ 
  HDRSRCHLTEMEAS_MEAS_CNF,            /* 4 */ 
  HDRSRCHLTEMEAS_CLEANUP_CNF,         /* 5 */ 
  HDRSRCHLTEMEAS_ABORT_CNF,           /* 6 */ 
  HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_CNF, /* 7 */ 
} 
hdrsrchmsg_lte_cnf_enum_type;

/* These states are tied directly to the LTE command/response
   interface for pilot measurements */
typedef enum
{
  HDRSRCHLTEMEAS_INACTIVE_STATE,          /* No activity           */
  HDRSRCHLTEMEAS_INIT_STATE,              /* Wait for Init cnf     */
  HDRSRCHLTEMEAS_SEARCH_STATE,            /* Wait for Search cnf   */
  HDRSRCHLTEMEAS_MEAS_STATE,              /* Wait for Meas cnf     */
  HDRSRCHLTEMEAS_CLEANUP_STATE,           /* Wait for Clean up cnf */
  HDRSRCHLTEMEAS_DEINIT_STATE,            /* Wait for Deinit cnf   */
  HDRSRCHLTEMEAS_ABORT_STATE,             /* Wait for Abort cnf    */
  HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_STATE,   /* Wait for Timed srch meas cnf    */
} 
hdrsrchltemeas_state_enum_type;

/* DO needs to know the states of the Init request to make sure
   LTE Init/Deinit processing is done correctly. */
typedef enum
{
  HDRSRCHLTEMEAS_LTEINIT_NONE,     /* Need to send LTE Init */
  HDRSRCHLTEMEAS_LTEINIT_SENT,     /* Wait for Init cnf     */
  HDRSRCHLTEMEAS_LTEINIT_RXED,     /* Init cnf received     */
  HDRSRCHLTEMEAS_LTEDEINIT_SENT,   /* Wait for Deinit cnf   */
} 
hdrsrchltemeas_lteinit_enum_type;

/* Used to process Treselect and Tbackoff timers.
   - When Treselect timer expires, one final pilot measurement is required for
   reselection.
   - When Tbackoff timer expires, the frequency can be measured again. */
typedef enum
{
  HDRSRCHLTEMEAS_TIMER_INACTIVE,   /* No timer is active for this frequency */
  HDRSRCHLTEMEAS_TIMER_ACTIVE,     /* Either Treselect or Tbackoff timer is active */
  HDRSRCHLTEMEAS_TIMER_EXPIRED,    /* Timer has expired, require special handling */
}
hdrsrchltemeas_timer_enum_type;

/* Used to differentiate multiple measurement report types to IRATMan */
typedef enum
{
  HDRSRCHLTEMEAS_MEAS_SUCCESS,
  HDRSRCHLTEMEAS_MEAS_ABORT,
  HDRSRCHLTEMEAS_MEAS_NO_CELL,
  HDRSRCHLTEMEAS_MEAS_INVALID
} hdrsrchltemeas_meas_report_enum_type;

/* Contains information on serving freq (DO freq) that is applicable to reselection */
typedef struct
{
  uint8                                        serving_priority;
    /* Priority of serving (DO) frequency */

  uint8                                        thresh_serving;
    /* Threshold of serving (DO) frequency */
}
hdrsrchltemeas_serving_freq_struct_type;

/* LTE frequency information */
typedef struct
{
  lte_earfcn_t                                 earfcn;
    /* Evolved Absolute Radio Frequency Channel Number (LTE freq number) */

  trm_frequency_info                           earfcn_trm_info;
    /* LTE band corresponding to EARFCN */

  boolean                                      lte_band_is_valid;
    /* TRUE if LTE band is valid */

  trm_set_client_state_input_type              trm_set_client_state_input;
  trm_set_client_state_output_data             trm_set_client_state_output;
    /* Save away parameters for TRM set_client_state API */
     
  uint8                                        earfcn_priority;
    /* Priority of the EARFCN of the neighboring EUTRA network */

  uint8                                        thresh_x;
    /* Minimum required signal quality threshold used for selecting an EARFCN */

  uint8                                        rx_lev_min_eutra;
    /* Minimum received RSRP level from the EUTRA cell */

  uint8                                        pe_max;
    /* Maximum TX power */

  uint8                                        rx_lev_min_eutra_offset;
    /* Offset to Minimum reference power level to select EUTRA */

  hdrsrchmsg_lte_bandwidth_enum_type           bandwidth;
    /* Measuring bandwidth */

  boolean                                      rsrq_parameters_incl;
  /* Should RSRQ  (Reference Signal  Received Quality)  be used  to
     measure LTE frequencies? */
   
  uint8                                        thresh_xq;
  /*  Minimum  required  RSRQ  signal  quality  threshold  used  for
      selecting an EARFCN */

  uint8                                        qual_min_eutra;
  /* Minimum received RSRQ  level from the EUTRA  cell. Value ranges
     from 0-31 where (-3 -  qual_min_eutra_common) in dB is equal to
     the minimum reference signal received quality (RSRQ) level of a
     EUTRA cell
   */
   
  uint8                                        qual_min_eutra_offset;
  /* Offset to minimum reference  quality level to select EUTRA. The
     range of the 22 common offset values allowed is 1 through 8
   */
}
hdrsrchltemeas_lte_freq_info_struct_type;

/* Struct type to save LTE cell IDs which pass reselection criteria */
typedef struct
{
  uint16                                       cell_id;
    /* Cell ID which passes the reselection criteria */

  int16                                        s_eutra_val;
    /* Q_measured as provided by LTE in LTE measurement confirmation message in
       dBm units, range is -144dBm to -44dBm */
}
hdrsrchltemeas_reselect_cell_struct_type;
              
/* Struct type of array storing LTE cell IDs which pass reselection criteria */
typedef struct
{
  hdrsrchltemeas_reselect_cell_struct_type     cell_arr[ HDRSRCHLTEMEAS_MAX_MEAS_CELLS ]; 
    /* Array containing LTE cells which pass the reselection criteria */

  uint8                                        cnt;
    /* Number of cells in the array */
}
hdrsrchltemeas_reselect_cell_arr_struct_type;

/* Struct type for individual LTE frequency entry in frequency list. Contains
   LTE frequency information received from Network.  In addition, also contain
   information to set Tbackoff and Treselect timer, as well as a next pointer
   to point to the next record in singly linked list. */
typedef struct hdrsrchltemeas_freq_list_entry_struct
{
  hdrsrchltemeas_lte_freq_info_struct_type     earfcn_to_meas;
    /* LTE frequency and frequency related parameters */

  hdrsrchltemeas_timer_enum_type               treselect_state;
    /* Set if measurement result exceeds threshold,
       one more measurement is required for reselection */

  timetick_type                                treselect_sclks;
    /* Treselect timer value in units of sclks */

  timetick_type                                treselect_exp_sclks;
    /* Treselect timer expiration time in units of sclks */

  hdrsrchltemeas_timer_enum_type               tbackoff_state;
    /* Set if measurement result is lower than threshold,
       freq is not measured again until timer expires */

  timetick_type                                tbackoff_sclks;
    /* Treselect timer value in units of sclks */

  timetick_type                                tbackoff_exp_sclks;
    /* Tbackoff timer expiration time in units of sclks */

  hdrsrchltemeas_reselect_cell_arr_struct_type reselect_cell_arr1;
  hdrsrchltemeas_reselect_cell_arr_struct_type reselect_cell_arr2;
    /* Save candidate LTE cell IDs in these arrays to pick the final candidate.
       Array 1 is used for the first measurement. 
       Array 2 is used for the second measurement. */

  struct hdrsrchltemeas_freq_list_entry_struct *next_ptr;
    /* Next ptr in list */

}
hdrsrchltemeas_freq_list_entry_struct_type;

/* Struct type for one LTE frequency list */
typedef struct
{
  hdrsrchltemeas_freq_list_entry_struct_type   *head_ptr;

  hdrsrchltemeas_freq_list_entry_struct_type   *tail_ptr;

  uint8                                        cnt;

}
hdrsrchltemeas_freq_list_struct_type;

/* Struct type for all LTE frequency lists */
typedef struct
{
  hdrsrchltemeas_freq_list_struct_type         free_list;
    /* Free buffer pool */

  hdrsrchltemeas_freq_list_struct_type         more_preferred;
    /* Contains all the frequencies to be measured every DO wake up
       period */

  hdrsrchltemeas_freq_list_struct_type         less_preferred;
    /* Contains all the frequencies with priority <= DO priority.
       These frequencies are measured if DO signal strength is lower
       than the serving frequency threshold specified in the Other IRAT neighbor
       list OTA message. */

}
hdrsrchltemeas_all_freq_lists_struct_type;

/* Struct type for “Current list to measure” array, containing addresses of all LTE
   frequency entries that will be measure during this particular SCI cycle.  This
   array is build every SCI cycle before LTE pilot measurements start.  The list
   includes eligible frequencies to be measured, i.e. frequencies which do not contain
   active Treselect or Tbackoff timers. */
typedef struct
{
  hdrsrchltemeas_freq_list_entry_struct_type   *freq_ptr[ HDRSRCH_LTE_FREQ_ARR_SIZE ]; 
    /* Array containing ptrs to each entry containing frequency information */

  uint8                                        curr_entry;
    /* Current entry containing LTE frequency to be measured */

  uint8                                        cnt;
    /* Number of frequencies in list */
                                       
}
hdrsrchltemeas_curr_freq_ptr_arr_struct_type;

/*  Contains all control information on pilot measurements that are not specific to
    any one LTE frequency, including pilot measurement state.  Some of the information
    are shared between all LTE frequencies.  Some are used as part of reselection
    criteria. */
typedef struct
{
  hdrsrchltemeas_lteinit_enum_type             init_cnf_rxed;
    /* Init confirmation has been received from LTE */

  boolean                                      update_list_ind;
    /* Indicate if the LTE frequency list needs to be updated */

  hdrsrchltemeas_cmd_and_req_enum_type         last_sent;
    /* Last message sent to LTE */

  hdrsrch_cmd_name_enum_type                   last_cmd;
    /* Last CMD received in hdrltemeas state */

  msgr_umid_type                               last_cnf;
    /* Last LTE confirmation message received in hdrltemeas state */

  hdrsrchltemeas_state_enum_type               meas_state;
    /* Current state of pilot measurement request */

  hdrsrchltemeas_serving_freq_struct_type      do_freq;
    /* Serving frequency information */

  uint8                                        num_eutra_freqs;
    /* Number of EUTRA frequencies to measure */

  boolean                                      priority_ind;
    /* Priority of serving freq and LTE freq is part of reselection criteria */

  boolean                                      max_reselect_timer_ind;
    /* Maximum value of Reselection Timer included */

  uint32                                       max_reselect_timer_ms;
    /* Maximum value of Reselection Timer (msecs) common to all LTE freqs */

  timer_type                                   reselect_timer;
    /* Used to arm HW timer for Treselect */

  timer_group_type                             reselect_timer_group;
    /* Timer requires a timer group if it's set up to run during sleep.
       Treselect timer belongs to this category. */

  uint32                                       ran_reselect_timer_ms;
    /* Randomized value of Reselection Timer (msecs) common to all LTE freqs */

  boolean                                      backoff_timer_ind;
    /* Use Back-off Timer max and min values below for all LTE freqs */

  uint32                                       min_backoff_timer_ms;
    /* Minimum value of measurement Back-off Timer (msecs) */

  uint32                                       max_backoff_timer_ms;
    /* Maximum value of measurement Back-off Timer (msecs) */

  uint32                                       ran_backoff_timer_ms;
    /* Randomized value of Back-off Timer (msecs) common to all LTE freqs */

  hdrsrchltemeas_freq_list_entry_struct_type   *treselect_freq_ptr;
    /* Address of frequency used to arm Treselect timer */

  uint8                                        reselect_timer_cnt;
    /* Number of active Treselect timers */

  int16                                        lte_max_tx_power;
    /* Mobile maximum TX power as reported by LTE */

  hdrsrch_lte_reselect_cb_type                 reselect_cb;
    /* Call back to notify upper layer of LTE reselection */

  lte_earfcn_t                                 reselect_freq_id;
    /* Winning EARFCN (LTE frequency) ID for reselection purpose */

  uint16                                       reselect_cell_id;
    /* Winning cell ID for reselection purpose */

  boolean                                      reselection_in_progress;
    /* LTE reselection attempt is in progress */

  hdrsrchltemeas_rxlm_struct_type              rxlm;
    /* RXLM buffers for RF tune scripts */

  lte_cphy_irat_meas_startup_rf_params_s       rf_params;
    /* Parameters for RF tune scripts */

  iratman_input_node_type                      iratman_input_node;
    /* LTE input params for IRATMan API */

  iratman_meas_node_type                       iratman_meas_node;
    /* LTE meas frequency information for IRATMan API */

  iratman_meas_rpt_type                        iratman_meas_rpt;
    /* LTE meas results for IRATMan API */

  iratman_meas_req_type                        iratman_meas_req;
    /* Struct to store signal strength converted to .5dB unit using
       lookup table ps2eng_table */

  iratman_meas_rat_reselect_type               iratman_resel_node;
    /* Winning EARFCN/Cell info for reselection purpose */

  uint32                                       orig_sleep_ms;
    /* The original sleep duration before TRM request for D2L extension */

  uint32                                       avail_sleep_ms;
    /* Sleep time available for LTE meas (in ms) */

  uint32                                       min_meas_ms;
    /* Length of time required to measure 1 LTE frequency */ 

  boolean                                      iratman_init_done;
    /* Set to TRUE after IRATMan init (1st entrance to LTE meas state */

  boolean                                      iratman_report_pending;
    /* Need to report node result to IRATMan to unlock the node */


  lte_ml1_irat_gap_schedule_gap_state_s        gap_state;
    /* Required by LTE, not used by DO */

  lte_ml1_irat_gap_scheduled_info_s            gap_sched;
    /* Required by LTE, not used by DO */

  hdraddr_type                                 sector_id;
    /* Current HDR sector ID */

  uint8                                        max_reselect_timer;
    /* Maximum value of Reselection Timer */

  uint8                                        min_backoff;
    /* Minimum value of measurement Back-off Timer */

  uint8                                        max_backoff;
    /* Maximum value of measurement Back-off Timer */

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
}
hdrsrchltemeas_ctrl_info_struct_type;

/* Struct type to save command parameters */
typedef struct
{
  hdrsrch_lte_nset_update_params_type          lte_nset_update_params_unsorted;
    /* Save a copy of the LTE NSET Update parameters */

  hdrsrch_lte_nset_update_params_type          lte_nset_update_params;
    /* Save a copy of the LTE NSET Update parameters, sorted by frequency priority */

  hdrsrch_lte_search_cnf_params_type           lte_search_cnf_params;
    /* Save a copy of the LTE Search Confirmation parameters (from LTE via MSGR) */

  hdrsrch_lte_meas_cnf_params_type             lte_meas_cnf_params;
    /* Save a copy of the LTE Meas Confirmation parameters (from LTE via MSGR) */

}
hdrsrchltemeas_saved_params_struct_type;

/* Look up table containing LTE bandwidth values as expected by the
   LTE module */
const hdrsrchmsg_lte_bandwidth_enum_type hdrsrchltemeas_freq_bw[ ] = 
{
  HDRSRCHMSG_BW_NRB_6,       /* entry 0, 1.4MHz bandwidth */
  HDRSRCHMSG_BW_NRB_15,      /* entry 1, 3MHz bandwidth */
  HDRSRCHMSG_BW_NRB_25,      /* entry 2, 5MHz bandwidth */
  HDRSRCHMSG_BW_NRB_50,      /* entry 3, 10MHz bandwidth */
  HDRSRCHMSG_BW_NRB_75,      /* entry 4, 15MHz bandwidth */
  HDRSRCHMSG_BW_NRB_100,     /* entry 5, 20MHz bandwidth */
};

hdrsrchltemeas_ctrl_info_struct_type           hdrsrchltemeas_ctrl_info;
  /* Information pertaining to pilot measurement request */

hdrsrchltemeas_freq_list_entry_struct_type     hdrsrchltemeas_freq_arr[ 
                                                 HDRSRCH_LTE_FREQ_ARR_SIZE ];
  /* Each entry contains information of LTE frequency to be measured */

hdrsrchltemeas_all_freq_lists_struct_type      hdrsrchltemeas_all_freq_lists;
  /* More preferred, Less preferred, and Free lists */

hdrsrchltemeas_curr_freq_ptr_arr_struct_type   hdrsrchltemeas_curr_freq_ptr_arr;
  /* Contains ptrs to all the frequencies to be measured during the
     current DO wake up period. */ 

hdrsrchltemeas_saved_params_struct_type        hdrsrchltemeas_saved;
  /* Saved copy of command parameters */

#ifdef FEATURE_HDR_RUMI
boolean                                      enable_d2l_hack = TRUE;
  /*Tabasco RUMI hack */
#endif /* FEATURE_HDR_RUMI */ 

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/

void hdrsrchltemeas_treselect_timer_set( void );
void hdrsrchltemeas_init( hdrsrch_state_enum_type last_state );
void hdrsrchltemeas_done( hdrsrch_state_enum_type last_state );
errno_enum_type hdrsrchltemeas_cmd
(
  const hdrsrch_cmd_type                       *cmd
    /* Internal HDR command to be processed */
);
void hdrsrchltemeas_process_low_pri_rsp_ind
(
  hdrsrchmsg_s_type                            *msg
);
void hdrsrchltemeas_reset_buffers( void );
void hdrsrchltemeas_lte_treselect_timer_exp( void );
LOCAL void hdrsrchltemeas_pri_chain_event( void );
void hdrsrchltemeas_enable_tune_aways( void );
boolean hdrsrchltemeas_iratman_wakeup_cb( void );
boolean hdrsrchltemeas_state_visit_request_trm ( void );
hdrsrchltemeas_freq_list_entry_struct_type *hdrsrchltemeas_get_next_lte_freq_trm 
( uint32 duration_ms );
void hdrsrchltemeas_end_lte_meas ( void );

#ifdef FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF
boolean hdrsrchltemeas_iratman_get_sector_cb
(
  iratman_cell_info_type *hdr_sector_info_ptr
);
#endif /* FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF */
void hdrsrchltemeas_iratman_report_meas_results
( 
  hdrsrchltemeas_meas_report_enum_type      meas_rpt
);

/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/

hdrsrch_state_struct_type hdrsrchltemeas_state =
{
  HDRSRCH_LTE_MEAS_IDLE_STATE,
  hdrsrchltemeas_init,
  hdrsrchltemeas_done,
  hdrsrchltemeas_cmd,
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
  hdrsrchltemeas_process_low_pri_rsp_ind
};

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_IRATMAN_ADD_NODE

DESCRIPTION 
  This function calls an IRATMan API to add a node to either the high or
  low priority list.
  The key goals for IRATMan are:
  - The ability to combine lists of iRAT nodes between different home RATs to
    avoid duplication of measurement activity.
  - Initial design is to be optimized for home RATs: CDMA1x and EVDO, to
    measure the LTE RAT.
  - Design the list manager to be expandable for possible use by other RATs.
  - The list manger is not to be responsible for the measurement, only the
    keeping of the lists, measurement criteria, and evaluation criteria.

DEPENDENCIES
  Runs during sleep state

PARAMETERS
  Priority of the node

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_iratman_add_node
( 
  hdrsrchltemeas_freq_list_entry_struct_type   *lte_buf_ptr,
    /* Ptr to buffer containing LTE frequency parameters are stored */

  iratman_lte_list_e_type                      priority
    /* Add 1 node to either high or low priority lists in IRATMan */
)
{

  boolean                                      ret_val = TRUE;
  iratman_rat_list_e_type                      list;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "D2L Add IRATMan node,earfcn=%d, lte list priority=%d",
                  lte_buf_ptr->earfcn_to_meas.earfcn,
                  priority );

  ASSERT( priority <= IRATMAN_LTE_NUM_LIST );
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.earfcn =
    lte_buf_ptr->earfcn_to_meas.earfcn;
#ifdef FEATURE_LTE_DEPRIORITIZATION
  if ( lte_buf_ptr->earfcn_to_meas.earfcn_priority
         == HDRSRCH_LTE_FREQ_LOWEST_PRIORITY )
  {
    hdrsrchltemeas_ctrl_info.iratman_input_node.lte.earfcn_priority = -1;
  }
  else
#endif /* FEATURE_LTE_DEPRIORITIZATION */
  {
    hdrsrchltemeas_ctrl_info.iratman_input_node.lte.earfcn_priority = 
      lte_buf_ptr->earfcn_to_meas.earfcn_priority;
  }
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.bandwidth =
    ( lte_bandwidth_e ) lte_buf_ptr->earfcn_to_meas.bandwidth;
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.thresh_serving =
    hdrsrchltemeas_ctrl_info.do_freq.thresh_serving;
#ifdef FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.max_tbackoff =
    hdrsrchltemeas_ctrl_info.max_backoff;
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.min_tbackoff =
    hdrsrchltemeas_ctrl_info.min_backoff;
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.max_treselect =
    hdrsrchltemeas_ctrl_info.max_reselect_timer;
#else
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.treselect_dur_ms =
    hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms;
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.tbackoff_dur_ms =
    hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms; 
#endif /* FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF */

  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.rx_lev_min_eutra =
    lte_buf_ptr->earfcn_to_meas.rx_lev_min_eutra;
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.rx_lev_min_eutra_offset =
    lte_buf_ptr->earfcn_to_meas.rx_lev_min_eutra_offset;
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.pe_max =
    lte_buf_ptr->earfcn_to_meas.pe_max;
  hdrsrchltemeas_ctrl_info.iratman_input_node.lte.thresh_x =
    lte_buf_ptr->earfcn_to_meas.thresh_x;

  if( lte_buf_ptr->earfcn_to_meas.rsrq_parameters_incl == TRUE )
  {
    hdrsrchltemeas_ctrl_info.iratman_input_node.lte.meas_comp_algo =
      IRATMAN_LTE_ALGO_RSRQ;
    hdrsrchltemeas_ctrl_info.iratman_input_node.lte.rsrq_thresh_x =
      lte_buf_ptr->earfcn_to_meas.thresh_xq;
    hdrsrchltemeas_ctrl_info.iratman_input_node.lte.rsrq_min_eutra =
      lte_buf_ptr->earfcn_to_meas.qual_min_eutra;
    hdrsrchltemeas_ctrl_info.iratman_input_node.lte.rsrq_min_eutra_offset =
      lte_buf_ptr->earfcn_to_meas.qual_min_eutra_offset;
  }
  else
  {
    hdrsrchltemeas_ctrl_info.iratman_input_node.lte.meas_comp_algo =
      IRATMAN_LTE_ALGO_RSRP;
  }

  list.lte = priority;

  ret_val = iratman_add_meas_node(
    RAT_HDR,
    RAT_LTE,
    &hdrsrchltemeas_ctrl_info.iratman_input_node,
    list );
      
  if( ret_val == FALSE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Failed to add node" );
  }
          
} /* hdrsrchltemeas_iratman_add_node */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_INIT_MEAS_STATE

DESCRIPTION
  This function causes the state machine to begin LTE Pilot Measurement
  state processing during the start of this sleep cycle. DO enters this state
  from Sleep state.  The procedure to perform in this state is as followed:
 
  - Procedure executes while DO is sleeping at every SCI cycle.
  - At the start of sleep state, DO decides to request an LTE measurement.
  - LTE tunes to the desired frequency and does the measurement.
  - DO runs the reselection criteria on the obtained LTE measurement.
  - If decision is made to reselect to LTE, LTE module tries to acquire on the
    chosen frequency.
  - If LTE measurement does not pass reselection criteria, DO performs
    measurement on the next LTE frequency in list.
  - Continue until list is exhausted without interfering with normal DO wake up.

DEPENDENCIES
  Runs during sleep state

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Transitions to the LTE Pilot Measurement state

===========================================================================*/

void hdrsrchltemeas_init_meas_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_change_state( &hdrsrchltemeas_state );
    /* Switch to LTE Pilot Measurement state processing */

} /* hdrsrchltemeas_init_meas_state */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RESET_LTEMEAS_STATE_MIN

DESCRIPTION 
  This function resets LTE measurement state.  It resets the minimum set of
  control information.  It is designed to work with other reset functions in
  pairs.  The other reset functions are more specific to the scenario which
  results in resetting LTE measurement state.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_reset_ltemeas_state_min( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Reset all LTE measurement information once LTE reselection occurs.
      If LTE fails to acquire, DO should start LTE measurements again from
      scratch, not from staled data. */

  hdrsrchltemeas_treselect_timer_clear();
    /* Clear outstanding Treselect timer */

  hdrsrchltemeas_reset_buffers();
    /* Clear all buffers and lists */

    /* Clear all control information except these fields:
       - Indicator for Init confirmation has been received 
       - Debug information such as last_sent/last_cmd received
       - Current state of pilot measurement request 
       - Treselect timer information as it is used by timer utility
       - LTE Max TX power.  Since it only comes in LTE Init confirmation,
         it only gets refreshed everytime LTE is re-init.
       - Treselect in progress indicator 
       - RF RXLM buffers
       - RF tune script parameters
       
     These fields may get reset separately, depending on the scenario
     requiring reset (traffic, hand off, inactive state, etc.*/

  hdrsrchltemeas_ctrl_info.update_list_ind                 = FALSE;
  hdrsrchltemeas_ctrl_info.do_freq.serving_priority        = 0;
  hdrsrchltemeas_ctrl_info.do_freq.thresh_serving          = 0;
  hdrsrchltemeas_ctrl_info.num_eutra_freqs                 = 0;
  hdrsrchltemeas_ctrl_info.priority_ind                    = FALSE;
  hdrsrchltemeas_ctrl_info.max_reselect_timer_ind          = FALSE;
  hdrsrchltemeas_ctrl_info.max_reselect_timer_ms           = 0;
  hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms           = 0;
  hdrsrchltemeas_ctrl_info.backoff_timer_ind               = FALSE;
  hdrsrchltemeas_ctrl_info.min_backoff_timer_ms            = 0;
  hdrsrchltemeas_ctrl_info.max_backoff_timer_ms            = 0;
  hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms            = 0;
  hdrsrchltemeas_ctrl_info.treselect_freq_ptr              = NULL;
  hdrsrchltemeas_ctrl_info.reselect_timer_cnt              = 0;
  hdrsrchltemeas_ctrl_info.reselect_cb                     = NULL;
  hdrsrchltemeas_ctrl_info.reselect_freq_id                = 0;
  hdrsrchltemeas_ctrl_info.reselect_cell_id                = 0;

  memset( &hdrsrchltemeas_ctrl_info.iratman_input_node,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_input_node ) );

  memset( &hdrsrchltemeas_ctrl_info.iratman_meas_node,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_meas_node ) );

  memset( &hdrsrchltemeas_ctrl_info.iratman_meas_rpt,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_meas_rpt ) );

  memset( &hdrsrchltemeas_ctrl_info.iratman_meas_req,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_meas_req ) );

  memset( &hdrsrchltemeas_ctrl_info.iratman_resel_node,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_resel_node ) );

  if( hdrsrchltemeas_ctrl_info.iratman_init_done == TRUE)
  {
  /* Delete all nodes added to IRATMan */
  iratman_del_all_nodes
    ( 
      RAT_HDR, 
      RAT_LTE 
    );
  }

} /* hdrsrchltemeas_reset_ltemeas_state_min */

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

void hdrsrchltemeas_reset_ltemeas_state( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_reset_ltemeas_state() is called, init_cnf=%d",
    hdrsrchltemeas_ctrl_info.init_cnf_rxed );

  hdrsrchltemeas_reset_ltemeas_state_min();
    /* Clear minimum set of LTE meas state control information */

  memset( &hdrsrchltemeas_saved, 0, sizeof( hdrsrchltemeas_saved ) );
    /* Force an update of ORAT neighbor list when protocol layer sends it by
       clearing the local copy of commands received.  This is necessary
       because a bit exact match between the new ORAT neighbor list update
       and the local saved copy causes the update to get ignored.  Since
       all LTE frequency information has been cleared after reselection, the
       update is necessary to get LTE freq info for future measurements. */

} /* hdrsrchltemeas_reset_ltemeas_state */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_EXIT_LTE_MEAS

DESCRIPTION
  This function exits the LTE Measurement state machine.  This state will
  only transition to Sleep state, since DO only gets to this state from
  Sleep.

DEPENDENCIES
  Runs while DO is sleeping

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_exit_lte_meas( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L Debug hdrsrchltemeas_exit_lte_meas() is called" );

  if( hdrsrchltemeas_ctrl_info.init_cnf_rxed == HDRSRCHLTEMEAS_LTEINIT_RXED )
    /* If DO has received confirmation of LTE Init request */
  {
    ( void )hdrsrchltemeas_send_deinit_req();
      /* Deinit LTE before leaving LTE meas state */

    return;
  }

  hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_INACTIVE_STATE;
    /* Reset sub state for LTE measurements */

  if( hdrsrch_state_id == HDRSRCH_LTE_MEAS_IDLE_STATE ) 
  {
    hdrsrchsleep_init_sleep_state();
      /* Done with LTE pilot measurement state, back to sleep */
  }

} /* hdrsrchltemeas_exit_lte_meas */

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

void hdrsrchltemeas_init_lists( void )

{
  int idx;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Link buffers in the list entry array, put all of them in the free list */
  hdrsrchltemeas_all_freq_lists.free_list.head_ptr = hdrsrchltemeas_freq_arr;
  hdrsrchltemeas_all_freq_lists.free_list.tail_ptr = 
    &hdrsrchltemeas_freq_arr[ HDRSRCH_LTE_FREQ_ARR_SIZE - 1 ];
  hdrsrchltemeas_all_freq_lists.free_list.cnt = HDRSRCH_LTE_FREQ_ARR_SIZE;
  for( idx = 0; idx < HDRSRCH_LTE_FREQ_ARR_SIZE; idx++ )
  {
    hdrsrchltemeas_freq_arr[ idx ].next_ptr = 
      &hdrsrchltemeas_freq_arr[ idx + 1 ];
  }
  hdrsrchltemeas_all_freq_lists.free_list.tail_ptr->next_ptr = NULL;

  /* Initialize the more preferred and less preferred empty lists */
  hdrsrchltemeas_all_freq_lists.more_preferred.cnt = 0;
  hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr = NULL;
  hdrsrchltemeas_all_freq_lists.more_preferred.tail_ptr = NULL;
    /* Contains all the frequencies to be measured every DO wake up
       period */

  hdrsrchltemeas_all_freq_lists.less_preferred.cnt = 0;
  hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr = NULL;
  hdrsrchltemeas_all_freq_lists.less_preferred.tail_ptr = NULL;
    /* Contains all the frequencies with priority <= DO priority.
       These frequencies are measured if DO signal strength is lower
       than the serving frequency threshold specified in the Other IRAT
       neighbor list OTA message. */

} /* hdrsrchltemeas_init_lists */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RESET_BUFFERS

DESCRIPTION 
  This function resets all freq parameters used in LTE pilot measurements.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_reset_buffers( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset( hdrsrchltemeas_freq_arr, 0, sizeof( hdrsrchltemeas_freq_arr ) ) ;
    /* Reset entries contains information of LTE frequency to be measured.
       This is an array so the & is left out.*/

  memset( &hdrsrchltemeas_curr_freq_ptr_arr, 0, 
          sizeof( hdrsrchltemeas_curr_freq_ptr_arr ) ) ;
    /* Reset LTE freq pointer array.  This is a structure so & is used. */

  hdrsrchltemeas_init_lists();
    /* Reset LTE neighbor lists */

} /* hdrsrchltemeas_reset_buffers */

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

void hdrsrchltemeas_reset_deinit( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L Debug hdrsrchltemeas_reset_deinit() is called" );

  /* Reset all LTE measurement information */

  hdrsrchltemeas_reset_ltemeas_state();
    /* Clear minimum set of LTE meas state control information */

  hdrsrchltemeas_ctrl_info.init_cnf_rxed = HDRSRCHLTEMEAS_LTEINIT_NONE;
    /* Clear Init confirmation received indicator, as DO just received
       LTE confirmation for Deinit request */

  hdrsrchltemeas_ctrl_info.meas_state    = HDRSRCHLTEMEAS_INACTIVE_STATE;
    /* Clear current state of pilot measurement request */

} /* hdrsrchltemeas_reset_deinit */

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

void hdrsrchltemeas_treselect_timer_clear( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) timer_clr( &hdrsrchltemeas_ctrl_info.reselect_timer, T_NONE );

} /* hdrsrchltemeas_treselect_timer_clear */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_EXPIRED_TRESELECT_TIMER_CNT

DESCRIPTION 
  This function returns the count of any frequency with expired Treselect
  timer in the following frequency lists:
   - The more preferred list contains all the frequencies to be measured every DO
     wake up period.
   - The less preferred list contains all the frequencies with priority <= DO
     priority.  These frequencies are measured if DO signal strength is lower
     than the serving frequency threshold specified in the Other IRAT neighbor
     list OTA message.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  Number of frequencies with expired Treselect timer

SIDE EFFECTS 
  None 

=========================================================================*/

uint8 hdrsrchltemeas_get_expired_treselect_cnt( void )

{
  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

  uint8                                        idx = 0;
    /* Loop index */

  uint8                                        num_expired = 0;
    /* Number of expired Treselect timers */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( 
    MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_get_expired_treselect_cnt() is called" );

  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr;
  while( ( freq_entry_ptr != NULL ) && ( idx < HDRSRCH_LTE_FREQ_ARR_SIZE ) )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_EXPIRED )
      /* Look for expired timers in more preferred list */
    {
      num_expired++;
    }
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr;
  while( ( freq_entry_ptr != NULL ) && ( idx < HDRSRCH_LTE_FREQ_ARR_SIZE ) )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_EXPIRED )
      /* Look for expired timers in less preferred list */
    {
      num_expired++;
    }
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  return( num_expired );

} /* hdrsrchltemeas_get_expired_treselect_cnt */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_ACTIVE_TRESELECT_TIMER_CNT

DESCRIPTION 
  This function returns the count of any frequency with active Treselect
  timer in the following frequency lists:
   - The more preferred list contains all the frequencies to be measured every DO
     wake up period.
   - The less preferred list contains all the frequencies with priority <= DO
     priority.  These frequencies are measured if DO signal strength is lower
     than the serving frequency threshold specified in the Other IRAT neighbor
     list OTA message.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  Number of frequencies with expired Treselect timer

SIDE EFFECTS 
  None 

=========================================================================*/

uint8 hdrsrchltemeas_get_active_treselect_cnt( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_get_active_treselect_cnt() is called" );

  return( hdrsrchltemeas_ctrl_info.reselect_timer_cnt );

} /* hdrsrchltemeas_get_active_treselect_cnt */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_BUFFER_GET

DESCRIPTION 
  This function removes a buffer from the head of a singly linked list.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  Ptr to buffer 

SIDE EFFECTS 
  None 

=========================================================================*/

hdrsrchltemeas_freq_list_entry_struct_type   *hdrsrchltemeas_buffer_get
(  
  hdrsrchltemeas_freq_list_struct_type       *list_ptr
    /* Ptr to list containing buffer to remove */
)

{
  hdrsrchltemeas_freq_list_entry_struct_type *buf_ptr = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT( list_ptr != NULL );
    /* Error is not expected, this function is only called from within this file */
  {
    if( list_ptr->cnt > 0 )
    {
      /* Get buffer from head of list */
      list_ptr->cnt--;
      buf_ptr = list_ptr->head_ptr;
      list_ptr->head_ptr = buf_ptr->next_ptr;
      buf_ptr->next_ptr = NULL;
    }
    else
    {
      buf_ptr = NULL;
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                    "D2L Ran out of buffer to store LTE freq parameters" );
    }
  }

  return buf_ptr;

} /* hdrsrchltemeas_buffer_get */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_BUFFER_PUT

DESCRIPTION 
  This function adds a buffer to the tail of a list.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  Ptr to buffer 

SIDE EFFECTS 
  None 

=========================================================================*/

void hdrsrchltemeas_buffer_put
(  
  hdrsrchltemeas_freq_list_struct_type       *list_ptr,
    /* Ptr to list containing buffer to add */

  hdrsrchltemeas_freq_list_entry_struct_type *buf_ptr
    /* Ptr to buffer to add */
)

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ASSERT(( list_ptr != NULL ) &&
         ( buf_ptr != NULL ));
    /* Error is not expected, this function is only called from within this file */

  if( list_ptr->cnt == 0 )
  {
    /* First buffer in list, head and tail pointers now points at the same buffer */
    list_ptr->head_ptr = buf_ptr;
    list_ptr->tail_ptr = buf_ptr;
  }
  else
  {
  /* Put buffer at tail of list */
    list_ptr->tail_ptr->next_ptr = buf_ptr;
    list_ptr->tail_ptr = buf_ptr;
  }
  buf_ptr->next_ptr = NULL;
  list_ptr->cnt++;

} /* hdrsrchltemeas_buffer_put */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_BUILD_LISTS

DESCRIPTION 
  This function builds the more preferred and less preferred LTE frequency
  lists.
  The more preferred list contains all the frequencies to be measured every
  DO wake up period.
  The less preferred list contains all the frequencies with priority <= DO
  priority.  These frequencies are measured if DO signal strength is lower
  than the serving frequency threshold specified in the Other IRAT neighbor
  list OTA message.
 
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

errno_enum_type hdrsrchltemeas_build_lists( void )

{
  errno_enum_type                              status = E_SUCCESS;
    /* Report error if any */

  hdrsrchltemeas_freq_list_entry_struct_type   *curr_buf_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

  hdrsrch_lte_nset_update_params_type          *params_ptr; 
    /* Ptr to LTE frequencies from LTE Nset Update command */

  uint8                                        idx;
    /* LTE buffer index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_build_lists() is called" );

  hdrsrchltemeas_reset_ltemeas_state_min();
    /* Clear minimum set of LTE meas state control information */

  hdrsrchltemeas_ctrl_info.reselection_in_progress = FALSE;
    /* Clear indicator for LTE reselection attempt in progress.  DO receives a new
       LTE neighbor list which does not match the old neighbor list, so clear out
       stale information. */

  /* Preserve the INIT confirmation received flag so DO does not need to send
     another INIT request if it is already sent */
  if( hdrsrchltemeas_ctrl_info.init_cnf_rxed == HDRSRCHLTEMEAS_LTEINIT_RXED )
  {
    hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_INIT_STATE;
      /* If DO already sent INIT request, avoid doing so again */
  }
  else if( hdrsrchltemeas_ctrl_info.init_cnf_rxed == HDRSRCHLTEMEAS_LTEINIT_NONE )
  {
    hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_INACTIVE_STATE;
      /* Require an INIT request before measurements can start */
  }
  else
  {
    HDR_MSG_SRCH_1( 
      MSG_LEGACY_ERROR, 
      "D2L Possible mismatch of LTE Deinit req %d when building new lists", 
      hdrsrchltemeas_ctrl_info.init_cnf_rxed );
  }

  /* If there is no LTE frequency to measure, return */
  params_ptr = &hdrsrchltemeas_saved.lte_nset_update_params;
  hdrsrchltemeas_ctrl_info.num_eutra_freqs = params_ptr->num_eutra_freqs; 
  if( params_ptr->num_eutra_freqs == 0 )
  {
    return status;
      /* Return after clearing the lists */
  }
  ASSERT( params_ptr->num_eutra_freqs <= HDRSRCH_LTE_FREQ_ARR_SIZE );
    /* Check for Max */

  /* Here if there is at least 1 LTE frequency to measure.
     Update serving frequency information */
  hdrsrchltemeas_ctrl_info.priority_ind = params_ptr->priority_incl;
  if( params_ptr->priority_incl == TRUE )
  {
    hdrsrchltemeas_ctrl_info.do_freq.serving_priority = 
      params_ptr->serving_priority;
      /* Need to check priority of LTE freq vs DO freq */
  }
  hdrsrchltemeas_ctrl_info.do_freq.thresh_serving = params_ptr->thresh_serving;

#ifdef FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF 
  /* Update reselect timer information */
  hdrsrchltemeas_ctrl_info.max_reselect_timer_ind = params_ptr->max_reselect_timer_incl;
  if( hdrsrchltemeas_ctrl_info.max_reselect_timer_ind == TRUE )
  {
    hdrsrchltemeas_ctrl_info.max_reselect_timer = params_ptr->max_reselect_timer;
      /* Recent API for IRATMan only requres timer from ORAT */
  }
  else
  {
    hdrsrchltemeas_ctrl_info.max_reselect_timer = 
      HDRSRCHLTEMEAS_IRATMAN_LTE_MAX_TRESELECT_DEFAULT;
      /* Default value */
  }

  /* Update backoff timer information */
  hdrsrchltemeas_ctrl_info.backoff_timer_ind = params_ptr->backoff_timer_incl;
  if( params_ptr->backoff_timer_incl == TRUE )
  {
    hdrsrchltemeas_ctrl_info.max_backoff = params_ptr->max_backoff;
    hdrsrchltemeas_ctrl_info.min_backoff = params_ptr->min_backoff;
  }
  else
  {
    hdrsrchltemeas_ctrl_info.max_backoff = 
      HDRSRCHLTEMEAS_IRATMAN_LTE_MAX_TBACKOFF_DEFAULT;
    hdrsrchltemeas_ctrl_info.min_backoff = 
      HDRSRCHLTEMEAS_IRATMAN_LTE_MIN_TBACKOFF_DEFAULT;
  }
#else
  /* Update reselect timer information */
  hdrsrchltemeas_ctrl_info.max_reselect_timer_ind = params_ptr->max_reselect_timer_incl;
  if( hdrsrchltemeas_ctrl_info.max_reselect_timer_ind == TRUE )
  {
    /* Randomize reselection timer up to max value specified and convert
       those values to ms units */
    hdrsrchltemeas_ctrl_info.max_reselect_timer_ms = 
      ( uint32 )( 1 << params_ptr->max_reselect_timer ) * 1000;
    hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms = 
      ( uint32 ) ran_dist( hdrutil_get_random(), 0, 
                           ( ( word ) ( 1 << params_ptr->max_reselect_timer ) + 1 ) );
    /* Convert to ms */
    hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms =
      hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms * 1000;
  }
  else
  {
    hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms = 
      ( uint32 )HDRSRCHLTEMEAS_T_RESELECT_TIMER_MS;
      /* Randomized Treselect timer is set to default value */
    hdrsrchltemeas_ctrl_info.max_reselect_timer_ms = 
      ( uint32 )HDRSRCHLTEMEAS_T_RESELECT_TIMER_DISCARD_MS;
      /* Max Treselect timer is also set to default value, required to validate
         timer value */
  }

  /* Update backoff timer information */
  hdrsrchltemeas_ctrl_info.backoff_timer_ind = params_ptr->backoff_timer_incl;
  if( params_ptr->backoff_timer_incl == TRUE )
  {
    /* Randomize backoff timer between max and min values and convert
       those values to ms units */
    hdrsrchltemeas_ctrl_info.max_backoff_timer_ms = 
      ( uint32 )( 1 << params_ptr->max_backoff ) * 1000;
    hdrsrchltemeas_ctrl_info.min_backoff_timer_ms = 
      ( uint32 )( 1 << params_ptr->min_backoff ) * 1000;
    hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms = 
      ( uint32 ) ran_dist( hdrutil_get_random(), 
                           ( ( word ) ( 1 << params_ptr->min_backoff ) ),
                           ( ( word ) ( 1 << params_ptr->max_backoff ) + 1 ) );

    /* Convert to ms */
    hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms = 
      hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms * 1000;
  }
  else
  {
    hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms = 
      ( uint32 )HDRSRCHLTEMEAS_T_BACKOFF_TIMER_MS;
      /* Randomized Tbackoff timer is set to default value */
    hdrsrchltemeas_ctrl_info.max_backoff_timer_ms = 
      ( uint32 )HDRSRCHLTEMEAS_T_BACKOFF_TIMER_MS;
      /* Max Tbackoff timer is also set to default value, required to validate
         timer value */
  }
#endif /* FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF */

  /* Build LTE More Preferred list and Less Preferred list using buffers from the
     free list */
  for( idx = 0; idx < params_ptr->num_eutra_freqs; idx++ )
  {
    curr_buf_ptr = 
      hdrsrchltemeas_buffer_get( &hdrsrchltemeas_all_freq_lists.free_list );

    /* Double check for completeness, should never get here */
    if( curr_buf_ptr == NULL )
    {
      hdrsrchltemeas_reset_buffers();
        /* Reset LTE neighbor lists */

      status = E_FAILURE;
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Ran out of free buffer to store LTE frequency" );
      return status;
    }

    /* Store frequency parameters, put frequency in either the More Preferred list or
       Less Preferred list.
       - If priority is not included, or if priority is included but DO priority is
         higher than LTE priority, the frequency goes in the More Preferred list.
       - Otherwise, the frequency goes in the Less Preferred list. */
    curr_buf_ptr->earfcn_to_meas.earfcn = params_ptr->earfcn_params_arr[ idx ].earfcn;
    curr_buf_ptr->earfcn_to_meas.thresh_x = params_ptr->earfcn_params_arr[ idx ].thresh_x;
    curr_buf_ptr->earfcn_to_meas.bandwidth = 
      hdrsrchltemeas_freq_bw[ params_ptr->earfcn_params_arr[ idx ].bandwidth ];
    
    /* Store RSRQ Parameters */
    if( params_ptr->rsrq_parameters_incl == TRUE )
    {
      curr_buf_ptr->earfcn_to_meas.thresh_xq =
        params_ptr->earfcn_params_arr[ idx ].thresh_xq;
      curr_buf_ptr->earfcn_to_meas.rsrq_parameters_incl = TRUE;
    }
    else
    {
      curr_buf_ptr->earfcn_to_meas.rsrq_parameters_incl = FALSE;
    }
    
    if( params_ptr->per_earfcn_params_incl == TRUE )
    {
      curr_buf_ptr->earfcn_to_meas.rx_lev_min_eutra = 
        params_ptr->earfcn_params_arr[ idx ].rx_lev_min_eutra;
      curr_buf_ptr->earfcn_to_meas.pe_max = 
        params_ptr->earfcn_params_arr[ idx ].pe_max;

      if( params_ptr->earfcn_params_arr[ idx ].rx_lev_min_eutra_offset_incl == TRUE )
      {  
        curr_buf_ptr->earfcn_to_meas.rx_lev_min_eutra_offset = 
          params_ptr->earfcn_params_arr[ idx ].rx_lev_min_eutra_offset + 1;
          /* Adjust value from 0 - 7 (3 bits) to 1 - 8 per spec */
      }
      else
      {
        curr_buf_ptr->earfcn_to_meas.rx_lev_min_eutra_offset = 
          HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_OFFSET_NONE;
      }

      if( params_ptr->rsrq_parameters_incl == TRUE )
      {
        curr_buf_ptr->earfcn_to_meas.qual_min_eutra = 
                  params_ptr->earfcn_params_arr[ idx ].qual_min_eutra;
        if( params_ptr->earfcn_params_arr[ idx ].qual_min_eutra_offset_incl ==
                                                                        TRUE )
        {
          curr_buf_ptr->earfcn_to_meas.qual_min_eutra_offset = 
            params_ptr->earfcn_params_arr[ idx ].qual_min_eutra_offset + 1;
            /* Adjust value from 0 - 7 (3 bits) to 1 - 8 per spec */
        }
        else
        {
          curr_buf_ptr->earfcn_to_meas.qual_min_eutra_offset = 
            HDRSRCHLTEMEAS_QUAL_MIN_EUTRA_OFFSET_NONE;
        } 
      }
    }
    else
    {
      curr_buf_ptr->earfcn_to_meas.rx_lev_min_eutra = params_ptr->rx_level_min_eutra_common;
      curr_buf_ptr->earfcn_to_meas.pe_max = params_ptr->pe_max_common;

      if( params_ptr->rx_lev_min_eutra_offset_common_incl == TRUE )
      {  
        curr_buf_ptr->earfcn_to_meas.rx_lev_min_eutra_offset = 
          params_ptr->rx_lev_min_eutra_offset_common + 1;
          /* Adjust value from 0 - 7 (3 bits) to 1 - 8 per spec */
      }
      else
      {
        curr_buf_ptr->earfcn_to_meas.rx_lev_min_eutra_offset = 
          HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_OFFSET_NONE;
      }

      if( params_ptr->rsrq_parameters_incl == TRUE )
      {
        curr_buf_ptr->earfcn_to_meas.qual_min_eutra = 
                                      params_ptr->qual_min_eutra_common;
                                    
        if ( params_ptr->qual_min_eutra_offset_common_incl == TRUE )
        {
          curr_buf_ptr->earfcn_to_meas.qual_min_eutra_offset = 
                            params_ptr->qual_min_eutra_offset_common;
        }
        else
        {
          curr_buf_ptr->earfcn_to_meas.qual_min_eutra_offset = 
            HDRSRCHLTEMEAS_QUAL_MIN_EUTRA_OFFSET_NONE;
        }
      }
    }

    /* Update priority information */
    if( params_ptr->priority_incl == TRUE )
    {
      /* Need to check priority of LTE freq vs DO freq */
      curr_buf_ptr->earfcn_to_meas.earfcn_priority = 
        params_ptr->earfcn_params_arr[ idx ].earfcn_priority;
      if( 
#ifdef FEATURE_LTE_DEPRIORITIZATION
          ( curr_buf_ptr->earfcn_to_meas.earfcn_priority 
              == HDRSRCH_LTE_FREQ_LOWEST_PRIORITY ) ||
#endif /* FEATURE_LTE_DEPRIORITIZATION */
          ( curr_buf_ptr->earfcn_to_meas.earfcn_priority <= 
            hdrsrchltemeas_ctrl_info.do_freq.serving_priority ) )
      {
        hdrsrchltemeas_buffer_put( &hdrsrchltemeas_all_freq_lists.less_preferred, 
                                   curr_buf_ptr );
        hdrsrchltemeas_iratman_add_node( curr_buf_ptr, IRATMAN_LTE_LP_LIST );
          /* Add a low priority node to IRATMan */
      }
      else
      {
        hdrsrchltemeas_buffer_put( &hdrsrchltemeas_all_freq_lists.more_preferred, 
                                   curr_buf_ptr );
        hdrsrchltemeas_iratman_add_node( curr_buf_ptr, IRATMAN_LTE_MP_LIST );
          /* Add a high priority node to IRATMan */
      }
    }
    else
    {
#ifdef FEATURE_LTE_DEPRIORITIZATION
      if ( params_ptr->earfcn_params_arr[ idx ].earfcn_priority 
             == HDRSRCH_LTE_FREQ_LOWEST_PRIORITY )
      {
        curr_buf_ptr->earfcn_to_meas.earfcn_priority = 
          params_ptr->earfcn_params_arr[ idx ].earfcn_priority;

        hdrsrchltemeas_buffer_put( &hdrsrchltemeas_all_freq_lists.less_preferred, 
                                   curr_buf_ptr );
        hdrsrchltemeas_iratman_add_node( curr_buf_ptr, IRATMAN_LTE_LP_LIST );
          /* Add a low priority node to IRATMan */
      }
      else
#endif /* FEATURE_LTE_DEPRIORITIZATION */
      {
        hdrsrchltemeas_buffer_put( &hdrsrchltemeas_all_freq_lists.more_preferred, 
                                   curr_buf_ptr );

        hdrsrchltemeas_iratman_add_node( curr_buf_ptr, IRATMAN_LTE_MP_LIST );
          /* Add a high priority node to IRATMan */
      }
    }
  } /* end for loop */

  hdrsrchltemeas_ctrl_info.reselect_cb = params_ptr->reselect_cb;
    /* Save call back function for reselection */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                  "D2L Treselect=%d,Tbackoff=%d",
                  hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms,
                  hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms );
  return status;

} /* hdrsrchltemeas_build_lists */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_SORT_LTE_NEIGHBOR_LIST

DESCRIPTION 
  This function sorts the LTE neighbor list in order of priority if the
  priority included field is set in the OTA message.  The LTE frequencies
  are sorted from highest priority to lowest. 
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

=========================================================================*/

void hdrsrchltemeas_sort_lte_neighbor_list( void )

{
  hdrsrch_lte_freq_params_type      tmp_entry;
    /* Temporary storage for use when swapping two entries */

  boolean                           swap;
    /* Flag indicating whether we swapped any on this pass */

  uint8                             idx;
    /* Index into the array */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrsrchltemeas_saved.lte_nset_update_params.priority_incl == FALSE )
  {
    return;
      /* Sorting is only required if the priority included field is set */
  }

  if( hdrsrchltemeas_saved.lte_nset_update_params.num_eutra_freqs < 2 )
  {
    return;
      /* Need to have at least 2 entries to sort them */
  }

  /* Bubble sort algorithm for sorting the LTE neighbor list */
  do
  {
    swap = FALSE;
      /* swap is set to TRUE if a swap occurs in the for loop below */

    for( idx = 0; 
         idx < hdrsrchltemeas_saved.lte_nset_update_params.num_eutra_freqs - 1; idx++ )
    {
      if( hdrsrchltemeas_saved.lte_nset_update_params.earfcn_params_arr[ idx ].earfcn_priority < 
          hdrsrchltemeas_saved.lte_nset_update_params.earfcn_params_arr[ idx + 1 ].earfcn_priority )
        /* If less than next, swap entries */
      {

        tmp_entry = hdrsrchltemeas_saved.lte_nset_update_params.earfcn_params_arr[ idx ];
        hdrsrchltemeas_saved.lte_nset_update_params.earfcn_params_arr[ idx ] = 
          hdrsrchltemeas_saved.lte_nset_update_params.earfcn_params_arr[ idx + 1 ];
        hdrsrchltemeas_saved.lte_nset_update_params.earfcn_params_arr[ idx + 1 ] = tmp_entry;
        swap = TRUE;     
          /* Note that we did a swap */

      } /* if priority of idx < ( idx + 1 ) */

    } /* for each entry */

  } while( swap );     /* Keep bubbling until no swap is required */

} /* hdrsrchltemeas_sort_lte_neighbor_list */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_DISCARD

DESCRIPTION
  This function is called when DO has a new LTE frequency list but there
  are active Treselect timers.  It is not desirable for DO to update the list
  if one LTE frequency is close to reselection (by having active Treselect
  timer.)
 
  There is one exception so DO does not hold on to the old LTE neighbor list
  for an unreasonably long time once a new list is available.  If there is a
  Treselect timer active, but the expiry is still a long way away (beyond the
  default delay time,) that Treselect timer is discarded.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_treselect_timer_discard( void )

{

  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */

  timetick_type                              treselect_guard_time_sclks;
    /* Max time within which Treselect timer is preserved, in sclks units */

  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_treselect_timer_discard() is called" );

  curr_time_sclks = timetick_get();
    /* Get current time in sclk units */

  treselect_guard_time_sclks = curr_time_sclks +
    TIMETICK_SCLK_FROM_MS( HDRSRCHLTEMEAS_T_RESELECT_TIMER_DISCARD_MS );
    /* Add Max time within which Treselect timer is preserved, in sclks units.
       Beyond this time, the Treselect timer is discarded so the new LTE
       frequency list can be put in use. */

  /* Look for frequencies in the MorePreferred list */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr;

  while( freq_entry_ptr != NULL )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
    {
      /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
       * Note: This only works for large unsigned integers where the delta
       * between time 1 < time 2 is smaller than half of the
       * number set.  These 2 time values are uint32 values. */
      if( TIME_OCCURS_FIRST( treselect_guard_time_sclks,
                             freq_entry_ptr->treselect_exp_sclks ) )
      {
        freq_entry_ptr->treselect_state = HDRSRCHLTEMEAS_TIMER_INACTIVE;
          /* Reset timer state */

        hdrsrchltemeas_ctrl_info.reselect_timer_cnt--;
          /* Account for 1 less Treselect timer */

        if( freq_entry_ptr == hdrsrchltemeas_ctrl_info.treselect_freq_ptr )
        {
          hdrsrchltemeas_treselect_timer_clear();
            /* Clear HW timer if it has been armed. */

          hdrsrchltemeas_ctrl_info.treselect_freq_ptr = NULL;
        }
      }
    }
      
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  /* Continue the search in the less preferred list */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr;

  while( freq_entry_ptr != NULL )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
    {
      /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
       * Note: This only works for large unsigned integers where the delta
       * between time 1 < time 2 is smaller than half of the
       * number set.  These 2 time values are uint32 values. */
      if( TIME_OCCURS_FIRST( treselect_guard_time_sclks,
                             freq_entry_ptr->treselect_exp_sclks ) )
      {
        freq_entry_ptr->treselect_state = HDRSRCHLTEMEAS_TIMER_INACTIVE;
          /* Reset timer state */

        hdrsrchltemeas_ctrl_info.reselect_timer_cnt--;
          /* Account for 1 less Treselect timer */

        if( freq_entry_ptr == hdrsrchltemeas_ctrl_info.treselect_freq_ptr )
        {
          hdrsrchltemeas_treselect_timer_clear();
            /* Clear HW timer if it has been armed. */

          hdrsrchltemeas_ctrl_info.treselect_freq_ptr = NULL;
        }
      }
    }
      
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

} /* hdrsrchltemeas_treselect_timer_discard */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_UPDATE_LTE_NEIGHBOR_LIST

DESCRIPTION 
  This function sorts the new LTE neighbor list in priority order then update
  the MorePreferred and LessPreferred lists.
 
  The Protocol layer sends the Searcher layer the LTE neighbor list after
  processing the following OTA messages:
 
  - Quick Config
  - Other RAT Neighbor list
 
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

errno_enum_type hdrsrchltemeas_update_lte_neighbor_list( void )

{
  errno_enum_type                       status = E_SUCCESS;
    /* Report error if any */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_RUMI
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.backoff_timer_incl = FALSE;
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.max_reselect_timer_incl = FALSE;
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.num_eutra_freqs = 1;
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.priority_incl = FALSE;
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.thresh_serving = 6;
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.per_earfcn_params_incl = FALSE;
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.rx_level_min_eutra_common = 56;
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.pe_max_common = 25;                            
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.rx_lev_min_eutra_offset_common_incl = FALSE;                           
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.earfcn_params_arr[0].bandwidth = 3;                              
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.earfcn_params_arr[0].earfcn = 2175;                              
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted.earfcn_params_arr[0].thresh_x = 2;                              
#endif /* FEATURE_HDR_RUMI */

  /* The list has been saved in an unsorted memory area.  Keep the unsorted
     copy, but make a duplicate and sort it in order of priority per
     requirement. */
#ifdef FEATURE_MEMCPY_REMOVAL
  hdrsrchltemeas_saved.lte_nset_update_params = 
                        hdrsrchltemeas_saved.lte_nset_update_params_unsorted;
#else
  memcpy( &hdrsrchltemeas_saved.lte_nset_update_params, 
          &hdrsrchltemeas_saved.lte_nset_update_params_unsorted,
          sizeof( hdrsrchltemeas_saved.lte_nset_update_params ) );
#endif /* FEATURE_MEMCPY_REMOVAL */

  hdrsrchltemeas_sort_lte_neighbor_list();
    /* LTE neighbor list is sorted in order of priority */
 
  status = hdrsrchltemeas_build_lists();
     /* Build more preferred and less preferred lists now */

  return status;

} /* hdrsrchltemeas_update_lte_neighbor_list */

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
 
  The Protocol layer sends the Searcher layer the LTE neighbor list after
  processing the following OTA messages:
 
  - Quick Config
  - Other RAT Neighbor list

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
  const hdrsrch_lte_nset_update_params_type   *lte_nlist_ptr
)

{
  errno_enum_type                       status = E_SUCCESS;
    /* Report error if any */

  int                                   memcmp_result = 0;
    /* Result of memcmp, between current list and new list */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( 
    MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_prep_to_update_lte_neighbor_list() is called" );

  /* Compare the new LTE neighbor list with the current one.  If they are identical,
     there is no need to update current lists, thus the backoff timers and reselect
     timers (if any) are preserved. */
  memcmp_result = 
    memcmp( &hdrsrchltemeas_saved.lte_nset_update_params_unsorted, lte_nlist_ptr, 
            sizeof( hdrsrchltemeas_saved.lte_nset_update_params_unsorted ) );
  if( memcmp_result == 0 )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "D2L Identical LTE neighbor list received, no update" );
    return status;
  }

#ifdef FEATURE_MEMCPY_REMOVAL
  hdrsrchltemeas_saved.lte_nset_update_params_unsorted = *lte_nlist_ptr;
#else
  memcpy( &hdrsrchltemeas_saved.lte_nset_update_params_unsorted, lte_nlist_ptr, 
          sizeof( hdrsrchltemeas_saved.lte_nset_update_params_unsorted ) );
#endif /* FEATURE_MEMCPY_REMOVAL */
    /* Always save a copy for future comparison */

  hdrsrchltemeas_ctrl_info.update_list_ind = TRUE;
    /* Remember that DO just got a new LTE frequency list */

  return status;

} /* hdrsrchltemeas_prep_to_update_lte_neighbor_list */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_BUILD_CURRENT_ARRAY

DESCRIPTION 
  This function determines which LTE frequencies to measure during this SCI
  cycle.  The sources of these LTE frequencies are:
 
   - The more preferred list contains all the frequencies to be measured
     every DO wake up period.
   - The less preferred list contains all the frequencies with priority <= DO
     priority.  These frequencies are measured if DO signal strength is lower
     than the serving frequency threshold specified in the Other IRAT neighbor
     list OTA message.
 
  The frequencies with expired Treselect timer will be measured first since they
  are candidate frequencies for reselection.
 
  The rest of the list contains frequencies with no active backoff timer and no
  active reselect timer.
 
  The output list of LTE frequencies is kept as a pointer array because this
  array contains LTE frequency information already held in MorePreferredEUTRAList
  and LessPreferredEUTRAList.  So only the address of these frequencies are added
  to the hdrsrchltemeas_curr_freq_ptr_arr array.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  Number of frequencies to measure this wake up period

SIDE EFFECTS 
  None 

=========================================================================*/

uint8 hdrsrchltemeas_build_current_array( void )

{
  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

  uint8                                        idx = 0;
    /* Number of LTE frequencies to measure */

  int16                                        ec_io_lin = 0;   
    /* HDR filtered signal strength in lin unit */

  uint16                                       ec_io_half_db = 0;   
    /* Signal strength converted to .5dB unit using lookup table
       ps2eng_table */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  memset( &hdrsrchltemeas_curr_freq_ptr_arr, 0, 
          sizeof( hdrsrchltemeas_curr_freq_ptr_arr ) ) ;
    /* Reset LTE freq pointer array */

  /* Build an array of pointers to keep track of which LTE frequencies to
     measure.  First, measure frequencies that have expired Treselect timer
     so look for them in both more preferred and less preferred lists. */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr;
  while( ( freq_entry_ptr != NULL ) && ( idx < HDRSRCH_LTE_FREQ_ARR_SIZE ) )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_EXPIRED )
      /* Look for expired timers in more preferred list */
    {
      hdrsrchltemeas_curr_freq_ptr_arr.freq_ptr[ idx++ ] = freq_entry_ptr;
      hdrsrchltemeas_curr_freq_ptr_arr.cnt++;
    }
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr;
  while( ( freq_entry_ptr != NULL ) && ( idx < HDRSRCH_LTE_FREQ_ARR_SIZE ) )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_EXPIRED )
      /* Look for expired timers in less preferred list */
    {
      hdrsrchltemeas_curr_freq_ptr_arr.freq_ptr[ idx++ ] = freq_entry_ptr;
      hdrsrchltemeas_curr_freq_ptr_arr.cnt++;
    }
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  /* Next add LTE frequencies in more preferred list with no active 
     backoff timer and no active reselect timer */
  if( hdrsrchltemeas_all_freq_lists.more_preferred.cnt > 0 ) 
  {
    freq_entry_ptr = hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr;
    while( ( freq_entry_ptr != NULL ) && ( idx < HDRSRCH_LTE_FREQ_ARR_SIZE ) )
    { 
      if( ( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_INACTIVE ) && 
          ( freq_entry_ptr->tbackoff_state == HDRSRCHLTEMEAS_TIMER_INACTIVE ) )
      {
        hdrsrchltemeas_curr_freq_ptr_arr.freq_ptr[ idx++ ] = freq_entry_ptr;
        hdrsrchltemeas_curr_freq_ptr_arr.cnt++;
      }
      freq_entry_ptr = freq_entry_ptr->next_ptr;
    }
  }

  /* Only consider adding LTE frequencies in less preferred list 
     if HDR signal strength is < ThreshServing threshold */
  if( hdrsrchltemeas_all_freq_lists.less_preferred.cnt > 0 ) 
  {
    hdrsrchutil_get_ecio_and_io( &ec_io_lin, NULL );
      /* Find the maximum ecio among all active set sectors (filtered value) */
    ec_io_half_db = ( uint16 )( hdrsrchset_eng_to_ecio( ec_io_lin ) );
      /* Convert to -.5dB units using lookup table ps2eng_table */

    /* ThreshServing description per SO87 spec:
     
       The access network shall set this field to [-2 * 10 * log10PS], where PS
       is the strength of the reference pilot of the serving Access network,
     
       Both EcIo and ThreshServing are represented as positive numbers even
       though they are negative numbers.  Thus, the "<" in the evaluation:
           RSRP of Serving C2K < ThreshServing
       is changed to ">" in the SW implementation to account for the correct
       negative values. */

    if( ec_io_half_db > ( uint16 )hdrsrchltemeas_ctrl_info.do_freq.thresh_serving )
    {
      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, 
        "D2L LessPref list:fr cnt=%d,lin=%d,ServingEng=%d,thresh=%d (-.5dB)", 
        hdrsrchltemeas_curr_freq_ptr_arr.cnt, 
        ec_io_lin,
        ec_io_half_db,
        hdrsrchltemeas_ctrl_info.do_freq.thresh_serving );

      /* Add LTE frequencies in less preferred list with no active 
         backoff timer and no active reselect timer */
      freq_entry_ptr = hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr;
      while( ( freq_entry_ptr != NULL ) && ( idx < HDRSRCH_LTE_FREQ_ARR_SIZE ) )
      { 
        if( ( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_INACTIVE ) && 
            ( freq_entry_ptr->tbackoff_state == HDRSRCHLTEMEAS_TIMER_INACTIVE ) )
        {
          hdrsrchltemeas_curr_freq_ptr_arr.freq_ptr[ idx ] = freq_entry_ptr;
          hdrsrchltemeas_curr_freq_ptr_arr.cnt++;
        }
        freq_entry_ptr = freq_entry_ptr->next_ptr;
      }
    }
    else
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
        "D2L Skip LessPreferred list, fr cnt=%d,ServingEng=%d,thresh=%d (-.5dB)", 
        hdrsrchltemeas_curr_freq_ptr_arr.cnt, 
        ec_io_half_db,
        hdrsrchltemeas_ctrl_info.do_freq.thresh_serving );
    }
  }
  
  /* Finish building the current list to search, reset current entry
     because DO has not started any measurement, and return */
  hdrsrchltemeas_curr_freq_ptr_arr.curr_entry = HDRSRCHLTEMEAS_NULL_ENTRY;
  
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "D2L Current list contains %d LTE freq(s)", 
                  hdrsrchltemeas_curr_freq_ptr_arr.cnt );
  
  return( hdrsrchltemeas_curr_freq_ptr_arr.cnt );

} /* hdrsrchltemeas_build_current_array */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_BUILD_RF_TUNEBACK_SCRIPT

DESCRIPTION 
  This function is just a wrapper for now since it's been decided that RF
  scripts are not used for DO->LTE reselection.
 
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

errno_enum_type hdrsrchltemeas_build_rf_tuneback_script( void ) 

{
  errno_enum_type                              status = E_SUCCESS;
    /* Report error if any */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Use default parameters for DO tune back scripts, as DO RF will be
     shut down to sleep */
  hdrsrchltemeas_ctrl_info.rf_params.startup_rf_idx = 
    HDRSRCHLTEMEAS_LTE_RF_STARTUP_BUFFER_INDEX;
  hdrsrchltemeas_ctrl_info.rf_params.cleanup_rf_index = 
    HDRSRCHLTEMEAS_DO_RF_CLEANUP_BUFFER_INDEX;
  hdrsrchltemeas_ctrl_info.rf_params.lte_rxlm_rx_buffer_ant0 =
    hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain0;
  hdrsrchltemeas_ctrl_info.rf_params.lte_rxlm_rx_buffer_ant1 = 
    hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain1;
  hdrsrchltemeas_ctrl_info.rf_params.source_rat_rxlm_rx_buffer_ant0 =
    hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0;
  hdrsrchltemeas_ctrl_info.rf_params.source_parameters.hdr_params.band =
    SYS_BAND_BC0;
  hdrsrchltemeas_ctrl_info.rf_params.source_parameters.hdr_params.chan = 500;
  hdrsrchltemeas_ctrl_info.rf_params.source_parameters.hdr_params.agc_mode = 
    ( lte_cphy_irat_meas_hdr_agc_mode_e ) HDRSRCHRF_NORMAL_AGC_ACQ_MODE;
  hdrsrchltemeas_ctrl_info.rf_params.source_parameters.hdr_params.agc_rtc_offset = 0;
  hdrsrchltemeas_ctrl_info.rf_params.source_parameters.hdr_params.prev_agc = 0;
  hdrsrchltemeas_ctrl_info.rf_params.source_parameters.hdr_params.device = 
    hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );

  return status;

} /* hdrsrchltemeas_build_rf_tuneback_script */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_NEXT_LTE_FREQ

DESCRIPTION 
  This function is called to get the next LTE frequency to measure.
 
DEPENDENCIES
  None

RETURN VALUE
  Pointer to next LTE frequency

SIDE EFFECTS
  None

===========================================================================*/

hdrsrchltemeas_freq_list_entry_struct_type   *hdrsrchltemeas_get_next_lte_freq( void )

{
  hdrsrchltemeas_freq_list_entry_struct_type *freq_to_meas_ptr = NULL;

  uint32                                     chips;
    /* Current time, in chips */

  uint32                                     chips_per_ms;
    /* Current time, in ms */

  uint32                                     remaining_time_ms;
    /* Remaining sleep time available for LTE meas */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Update remaining slots to request TRM for next wake up.  Some time
     has been used to measure LTE frequencies since DO starts sleeping. */
#ifdef FEATURE_HDR_DIME_MODEM
  chips = slpc_get_sysclk_count( SLPC_HDR );
    /* How long we've slept for, in chipx8 */
  chips = chips >> 3 ;
    /* in chips now */
#else
  chips = sleepctl_get_chip_cnt( SLEEPCTL_HDR );
    /* Determine how long we slept for, in chips */
#endif /* FEATURE_HDR_DIME_MODEM */

  chips_per_ms = chips / HDRSRCHLTEMEAS_CHIPS_PER_MS;
    /* And convert to ms */

  remaining_time_ms = hdrsrchltemeas_ctrl_info.avail_sleep_ms - chips_per_ms;
    /* Calculate the remaining time from the original available time */

  hdrsrchltemeas_ctrl_info.iratman_meas_node = 
    iratman_get_meas
      (
        RAT_HDR,                          
        RAT_LTE,                          
        remaining_time_ms,                
        &hdrsrchltemeas_ctrl_info.iratman_meas_req  
      );
    /* Get LTE frequency from IRATMan */

  if( hdrsrchltemeas_ctrl_info.iratman_meas_node.lte.valid == FALSE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L Ran out of frequency to measure" );
    freq_to_meas_ptr = NULL;
  }
  else 
  {
    hdrsrchltemeas_curr_freq_ptr_arr.cnt = 1;
    freq_to_meas_ptr = &hdrsrchltemeas_freq_arr[ 0 ];
    freq_to_meas_ptr->earfcn_to_meas.bandwidth = 
      ( hdrsrchmsg_lte_bandwidth_enum_type ) hdrsrchltemeas_ctrl_info.iratman_meas_node.lte.bandwidth;
    freq_to_meas_ptr->earfcn_to_meas.earfcn = 
      hdrsrchltemeas_ctrl_info.iratman_meas_node.lte.earfcn;
      /* Go measure this frequency */

    hdrsrchltemeas_ctrl_info.iratman_report_pending = TRUE;
      /* Need to report to IRATMan regardless of result to unlock this node */
  }

  return( freq_to_meas_ptr );

} /* hdrsrchltemeas_get_next_lte_freq */

#ifdef FEATURE_HDR_D2L_RF_MEAS
/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RFM_MEAS_ENTER

DESCRIPTION 
  This function provides LTE with RF parameters to enter LTE measurement mode.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_rfm_meas_enter( void )
{

  uint8                                   chan_cnt;
    /* The number of channels coverred by the current RF config */

  const hdrsrch_demod_idx_type            *demod_list_ptr;
    /* A pointer to the demod index assignment from hdrsrchrf. */

  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* LTE frequency to measure */

  lte_cphy_irat_meas_rf_param_s           *lte_ptr;
  rfm_meas_common_param_type              *rf_common_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Allocate RXLM buffers for DO. */
  hdrsrchltemeas_ctrl_info.rxlm.do_handle_valid = 
    hdrsrchrf_get_rxlm_buffer_handle( 
      HDRSRCHRF_PRI_CHAIN,
      &hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0 ) ;

  /* Allocate RXLM buffers for LTE RF tuning.  LTE always uses both chains. */
  if ( hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid != TRUE )
  {
    rxlm_allocate_buffer( RXLM_CHAIN_0, LM_LTE,
                          &hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain0 );
    rxlm_allocate_buffer( RXLM_CHAIN_1, LM_LTE,
                          &hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain1 );

    hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid = TRUE;
  }

  ASSERT ( hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid == TRUE );

  /* DO header parameters */
  rf_common_ptr = &( hdrsrchltemeas_ctrl_info.common_param );

  rfm_meas_common_init_params( rf_common_ptr );
    /* Initialize RF common parameters */

  rf_common_ptr->header.source_tech       = RFCOM_PARKED_MODE;
  rf_common_ptr->header.num_source_item   = 1;
  rf_common_ptr->header.source_param_v2[ 0 ].band = ( uint32 )
    hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].device =
    hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].lm_handle = 
    hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0;

  /* DO parameters */
  rf_common_ptr->source_param.hdr_params.band_chan.chan_num =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->chan_num;
  rf_common_ptr->source_param.hdr_params.band_chan.band =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;

  freq_entry_ptr                    = hdrsrchltemeas_freq_arr;   
    /* Ptr to LTE frequency */

  /* Script type */
  rf_common_ptr->script_type = RFM_MEAS_PROGRAM_NULL2X;

  /* LTE parameters */
  lte_ptr = &hdrsrchltemeas_ctrl_info.lte_param;

  lte_ptr->common = rf_common_ptr;
  lte_ptr->flags.is_gap_aborted = FALSE;
  lte_ptr->target.device = 
    freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_ptr->target.div_device =
    freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].diversity_device;
  lte_ptr->target.rxlm_buf_idx_ant0 = 
    hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain0;
  lte_ptr->target.rxlm_buf_idx_ant1 = 
    hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain1;
  lte_ptr->target.earfcn = freq_entry_ptr->earfcn_to_meas.earfcn;

  lte_ml1_irat_x2l_meas_enter ( 
#ifdef FEATURE_HDR_MSGR_VARIANT 
           hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
          lte_ptr );

  hdrsrchltemeas_ctrl_info.is_rfm_meas_entered = TRUE;
    /* Update flag to indicate rfm_meas_enter done. Check this flag and call
       rfm_meas_exit for all abort scenarios */

} /* hdrsrchltemeas_rfm_meas_enter */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_MEAS_NEXT_LTE_FREQ

DESCRIPTION
  This function initiates another LTE frequency measurement by sending the
  START UP command followed by the SEARCH request to LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/
#ifdef FEATURE_HDR_D2L_RF_MEAS
errno_enum_type hdrsrchltemeas_meas_next_lte_freq
( 
  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr, 
    /* Ptr to LTE frequency to measure */
  boolean                                      call_meas_enter
    /* Indicate if RF meas enter call is required */
)

{
  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT( freq_entry_ptr != NULL );

  /* Send timed search and meas req message, resume after confirmation is received. */
  hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_REQ;
  hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_STATE;
  
  if( call_meas_enter == TRUE )
  {
    hdrsrchltemeas_rfm_meas_enter();
      /* Allows LTE to set up Enter meas parameters */
  }

  status = hdrsrchmsg_d2l_send_timed_search_meas_req_msg
             ( freq_entry_ptr->earfcn_to_meas.earfcn,
               freq_entry_ptr->earfcn_to_meas.bandwidth,
               &(freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output),
               &hdrsrchltemeas_ctrl_info.lte_param );

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Timed_search_meas_req failed" );
    return status;
  }

  return status;

} /* hdrsrchltemeas_meas_next_lte_freq */
#else
errno_enum_type hdrsrchltemeas_meas_next_lte_freq
( 
  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr 
)

{
  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT( freq_entry_ptr != NULL );

  /* Send timed search and meas req message, resume after confirmation is received. */
  hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_REQ;
  hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_STATE;
  
  status = hdrsrchmsg_d2l_send_timed_search_meas_req_msg
             ( freq_entry_ptr->earfcn_to_meas.earfcn,
               freq_entry_ptr->earfcn_to_meas.bandwidth,
               &(freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output),
               &hdrsrchltemeas_ctrl_info.rf_params );

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Timed_search_meas_req failed" );
    return status;
  }

  return status;

} /* hdrsrchltemeas_meas_next_lte_freq */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_CHECK_TO_UPDATE_LTE_NEIGHBOR_LIST

DESCRIPTION 
  This function checks to see if the LTE neighbor lists can be updated in
  IRATMan.
 
  This function is called in the following events:
 
  - At the start of LTE MEAS state, there may be expired Treselect timers
  that need to be serviced.  If so, DO hangs on to the old list.
 
  - Before leaving LTE MEAS state, those expired Treselect timers have been
  taken care of.  So if a new list is pending, then DO should try to update
  it to be ready for the next cycle.  Note that the frequency lists are
  just updated but not serviced.  The new lists will be serviced the next time
  DO enters LTE MEAS state.
 
  - In the future, this function may be called when DO hands off to a different
  sector.
 
  The Protocol layer sends the Searcher layer the LTE neighbor list after
  processing the following OTA messages:
 
  - Quick Config
  - Other RAT Neighbor list

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
errno_enum_type hdrsrchltemeas_check_to_update_lte_neighbor_list( void )
{
  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

  boolean                                      meas_pending = FALSE;
    /* TRUE if there are measurements outstanding, do not update list
       FALSE if it's OK to update list now */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_RUMI
  if ( enable_d2l_hack == TRUE )
  {
    hdrsrchltemeas_ctrl_info.update_list_ind = TRUE;
    enable_d2l_hack = FALSE;
  }
#endif /*FEATURE_HDR_RUMI*/

  if( hdrsrchltemeas_ctrl_info.update_list_ind == TRUE ) 
  {
    /* Have a new LTE list, discard timers that are too far away so the list
       can be updated.  Once that is done, are there other meas outstanding? */
    meas_pending = 
      iratman_trim_nodes
        ( 
          RAT_HDR, 
          RAT_LTE,
          HDRSRCHLTEMEAS_T_RESELECT_TIMER_DISCARD_MS
        );

    if( meas_pending == FALSE )
    {
      status = hdrsrchltemeas_update_lte_neighbor_list();
        /* Build more preferred and less preferred lists now */
      
      if( status == E_FAILURE )
      {
        HDR_MSG_SRCH( 
          MSG_LEGACY_ERROR,  
          "D2L Failed to build new frequency lists, go back to sleep" );
      }
      else
      {
        hdrsrchltemeas_ctrl_info.update_list_ind = FALSE;
          /* Reset flag */
      }
    }
  }

  return status;

} /* hdrsrchltemeas_check_to_update_lte_neighbor_list */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RFM_MEAS_EXIT

DESCRIPTION 
  Before leaving LTE MEAS state, tune RF back to DO frequency.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/
#ifdef FEATURE_HDR_D2L_RF_MEAS
void hdrsrchltemeas_rfm_meas_exit( void )
{

  uint8                                   chan_cnt;
    /* The number of channels coverred by the current RF config */

  const hdrsrch_demod_idx_type            *demod_list_ptr;
    /* A pointer to the demod index assignment from hdrsrchrf. */

  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* LTE frequency to measure */

  lte_cphy_irat_meas_rf_param_s           *lte_ptr;
  rfm_meas_common_param_type              *rf_common_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ASSERT ( hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid == TRUE );

  /* DO header parameters */
  rf_common_ptr = &( hdrsrchltemeas_ctrl_info.common_param );

  rf_common_ptr->header.source_tech       = RFCOM_PARKED_MODE;
  rf_common_ptr->header.num_source_item   = 1;
  rf_common_ptr->header.source_param_v2[ 0 ].band = ( uint32 )
    hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].device =
    hdrsrchrf_get_rf_dev( HDRSRCHRF_PRI_CHAIN );
  rf_common_ptr->header.source_param_v2[ 0 ].rx_dev_handle[ 0 ].lm_handle = 
    hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0;

  /* DO parameters */
  rf_common_ptr->source_param.hdr_params.band_chan.chan_num =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->chan_num;
  rf_common_ptr->source_param.hdr_params.band_chan.band =
      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list_ptr )->band;

  /* Script type */
  rf_common_ptr->script_type = RFM_MEAS_PROGRAM_NULL2X;

  freq_entry_ptr                   = hdrsrchltemeas_freq_arr;
    /* Ptr to LTE frequency */

  /* LTE parameters */
  lte_ptr = &hdrsrchltemeas_ctrl_info.lte_param;

  lte_ptr->common = rf_common_ptr;
  lte_ptr->flags.is_gap_aborted = FALSE;
  lte_ptr->target.device = 
    freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_ptr->target.div_device =
    freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].diversity_device;
  lte_ptr->target.rxlm_buf_idx_ant0 = 
    hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain0;
  lte_ptr->target.rxlm_buf_idx_ant1 = 
    hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain1;
  lte_ptr->target.earfcn = freq_entry_ptr->earfcn_to_meas.earfcn;

  lte_ml1_irat_x2l_meas_exit ( 
#ifdef FEATURE_HDR_MSGR_VARIANT 
           hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
           lte_ptr );

  hdrsrchltemeas_ctrl_info.is_rfm_meas_entered = FALSE;
    /* Update flag to indicate rfm_meas_exit done */

} /* hdrsrchltemeas_rfm_meas_exit */
#else
void hdrsrchltemeas_rfm_meas_exit( void )
{

  rfm_meas_exit_param_type         rf_meas_exit;
    /* Parameter for RF to tune back */

  rfm_meas_result_type             rf_result;
    /* Tune back result */

  rfm_meas_lte_exit_param_type     lte_exit_param;
    /* LTE exit param */

  rfm_meas_hdr_exit_param_type     do_exit_param;
    /* DO exit param */

  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* LTE frequency to measure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  rf_meas_exit.header.source_tech  = RFCOM_1XEVDO_MODE;
  rf_meas_exit.header.target_tech  = RFCOM_LTE_MODE;

  lte_exit_param.bw                = RFCOM_BW_LTE_10MHz;
  freq_entry_ptr                   = hdrsrchltemeas_freq_arr;
  lte_exit_param.device            = 
    freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_exit_param.rx_channel        = hdrsrchltemeas_saved.lte_search_cnf_params.freq; 
  lte_exit_param.buffer_index      = HDRSRCHLTEMEAS_LTE_RF_STARTUP_BUFFER_INDEX;
    /* Use default values */

  do_exit_param.device             = hdrsrchrf_get_rf_dev( 
                                        HDRSRCHRF_PRI_CHAIN );
  do_exit_param.band_chan.band     = SYS_BAND_BC0;
  do_exit_param.band_chan.chan_num = 500;
    /* Use default values, RF will be disabled anyway */
  do_exit_param.rxlm_buf_index     = hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0 ;

  rf_meas_exit.source_param = &do_exit_param;
  rf_meas_exit.target_param = &lte_exit_param;
  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "D2L rfm_meas_exit,DO device=%d,LTE device=%d",
                  do_exit_param.device,
                  lte_exit_param.device );

  rf_result = rfm_meas_common_script_exit( &rf_meas_exit, NULL );
    /* Call RF tune back function after LTE measurements. */

  if (rf_result != RFM_MEAS_COMMON_SUCCESS)
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                  "D2L Failed rfm_meas_common_script_exit()" );
  }

} /* hdrsrchltemeas_rfm_meas_exit */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_DONE

DESCRIPTION 
  This function is called when exiting from LTE MEAS state.  The only valid
  next state from LTE MEAS is SLEEP state.
 
  Before leaving LTE MEAS state, tune RF back to DO frequency.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_done( hdrsrch_state_enum_type next_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_done() is called" );

  hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_INACTIVE_STATE;
    /* Reset sub state for LTE measurements */

#ifdef FEATURE_HDR_D2L_RF_MEAS
  if (hdrsrchltemeas_ctrl_info.is_rfm_meas_entered)
  {
     hdrsrchltemeas_rfm_meas_exit();
       /* Call RF tune back function after LTE measurements */
  }
#else /* FEATURE_HDR_D2L_RF_MEAS */
  hdrsrchltemeas_rfm_meas_exit();
    /* Call RF tune back function after LTE measurements */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

  /* Deallocate RXLM buffers for LTE.  LTE always uses both chains. */
  if ( hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid == TRUE )
  {
    rxlm_deallocate_buffer( hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain0 );
    rxlm_deallocate_buffer( hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain1 );

    hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid = FALSE;
  }

  hdrsrchmcpm_do2lte_measurement( FALSE );
    /* Notify DO to LTE measurement done status to MCPM */

  hdrsrch_default_done( next_state );
    /* Use default handling */

} /* hdrsrchltemeas_done */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_RFM_MEAS_ENTER

DESCRIPTION 
  This function requests RF to enter LTE measurement mode.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

#ifndef FEATURE_HDR_D2L_RF_MEAS
void hdrsrchltemeas_rfm_meas_enter( void )
{

  static rfm_meas_enter_param_type        rf_meas_enter;
    /* Parameter for RF to enter D2L measurement state */

  rfm_meas_result_type             rf_result;
    /* Status returned from RF */

  static rfm_meas_lte_enter_param_type    lte_enter_param;
    /* LTE enter param */

  static rfm_meas_hdr_enter_param_type    do_enter_param;
    /* DO enter param */

  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* LTE frequency to measure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchltemeas_ctrl_info.rxlm.do_handle_valid = 
    hdrsrchrf_get_rxlm_buffer_handle( 
      HDRSRCHRF_PRI_CHAIN,
      &hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0 ) ;

  rf_meas_enter.header.source_tech  = RFCOM_1XEVDO_MODE;
  rf_meas_enter.header.target_tech  = RFCOM_LTE_MODE;

  lte_enter_param.bw                = RFCOM_BW_LTE_10MHz;
  freq_entry_ptr                    = hdrsrchltemeas_freq_arr;   
  lte_enter_param.device            = 
    freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device;
  lte_enter_param.rx_channel        = 500;
    /* Use default values, similar to WCDMA */

  do_enter_param.device             = hdrsrchrf_get_rf_dev( 
                                        HDRSRCHRF_PRI_CHAIN );
    /* Possibily RF_MAX_DEVICE if HDR released TRM (in non-SVDO config) */
  do_enter_param.band_chan.band     = SYS_BAND_BC0;
  do_enter_param.band_chan.chan_num = 500;
  do_enter_param.rxlm_buf_index     = hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0 ;
    /* Use default values, RF will be disabled anyway */

  rf_meas_enter.source_param = &do_enter_param;
  rf_meas_enter.target_param = &lte_enter_param;

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "D2L rfm_meas_enter,DO device=%d,LTE device=%d",
                  do_enter_param.device,
                  lte_enter_param.device );

  rf_result = rfm_meas_common_script_enter( &rf_meas_enter, NULL );
    /* Notify RF of LTE measurement */

  if (rf_result != RFM_MEAS_COMMON_SUCCESS)
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                  "D2L Failed rfm_meas_common_script_enter()" );
  }

} /* hdrsrchltemeas_rfm_meas_enter */
#endif /* !FEATURE_HDR_D2L_RF_MEAS */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_ENTER_LTE_MEAS

DESCRIPTION 
  This function is called to enter LTE pilot measurement state.  This
  function should be called every wake up cycle during Idle state if the
  following conditions are met.
  - There is enough time to request LTE pilot measurements.
  - There is at least one LTE frequency to measure.
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchltemeas_enter_lte_meas( void )

{
  errno_enum_type                              status = E_SUCCESS;
    /* Report error if any */

  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_enter_lte_meas() is called" );

  hdrsrchmcpm_do2lte_measurement( TRUE );
    /* Notify DO to LTE measurement start status to MCPM */

  /* Allocate RXLM buffers for LTE RF tuning.  LTE always uses both chains. */
  if ( hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid != TRUE )
  {
    rxlm_allocate_buffer( RXLM_CHAIN_0, LM_LTE,
                          &hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain0 );
    rxlm_allocate_buffer( RXLM_CHAIN_1, LM_LTE,
                          &hdrsrchltemeas_ctrl_info.rxlm.lte_buf_chain1 );

    hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid = TRUE;
  }

  /* Get DO RXLM buffer handle */
  hdrsrchltemeas_ctrl_info.rxlm.do_handle_valid = 
    hdrsrchrf_get_rxlm_buffer_handle( 
      HDRSRCHRF_PRI_CHAIN,
      &hdrsrchltemeas_ctrl_info.rxlm.do_buf_chain0 ) ;

  ASSERT ( hdrsrchltemeas_ctrl_info.rxlm.lte_handle_valid == TRUE );

  /* Initialize data used for IRATMan API */
  memset( &hdrsrchltemeas_ctrl_info.iratman_meas_rpt,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_meas_rpt ) );

  memset( &hdrsrchltemeas_ctrl_info.iratman_resel_node,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_resel_node ) );


  status = hdrsrchltemeas_build_rf_tuneback_script();
    /* Build RF tune back script to tune back to DO after LTE pilot measurement. */
  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                  "D2L Start Up state RF tune back script failure" );
    return status;
  }

  if( hdrsrchltemeas_ctrl_info.init_cnf_rxed == HDRSRCHLTEMEAS_LTEINIT_RXED )
    /* Already processed INIT confirmation */
  {
    freq_entry_ptr = hdrsrchltemeas_get_next_lte_freq();
      /* Get the frequency information from current frequency in list */

    if( freq_entry_ptr != NULL )
    {
#ifdef FEATURE_HDR_D2L_RF_MEAS
      status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr, TRUE );
        /* Begin the process to measure the next frequency from list. */
#else
      status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr );
        /* Begin the process to measure the next frequency from list */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

      if( status == E_FAILURE )
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                      "D2L Start Up state failed to measure next freq" );
      }
    }
    else
    {
      hdrsrchltemeas_exit_lte_meas();
        /* Exit the LTE measurement state machine, back to sleep */
    }
  }
  else if( hdrsrchltemeas_ctrl_info.init_cnf_rxed == HDRSRCHLTEMEAS_LTEINIT_NONE )
  {

    lte_ml1_irat_gap_init(
#ifdef FEATURE_HDR_MSGR_VARIANT 
           hdrsrch_get_subscriber_id(),  
#endif /* FEATURE_HDR_MSGR_VARIANT */
           &hdrsrchltemeas_ctrl_info.gap_state);
      /* LTE requires this call before starting measurements */

    /* Send Init req message, resume after confirmation is received */
    hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_INIT_REQ;
    hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_INIT_STATE;
    hdrsrchmsg_d2l_send_init_req_msg();
    hdrsrchltemeas_ctrl_info.init_cnf_rxed = HDRSRCHLTEMEAS_LTEINIT_SENT;
  }
  else
  {
    HDR_MSG_SRCH_1( 
      MSG_LEGACY_ERROR, 
      "D2L Possible mismatch of LTE Deinit req %d when entering LTE state", 
      hdrsrchltemeas_ctrl_info.init_cnf_rxed );
  }

  return status;

} /* hdrsrchltemeas_enter_lte_meas */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_ENABLE_TUNE_AWAYS

DESCRIPTION
  Turns on the ability for tune aways to happen within the lte meas state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_enable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are now interested in primary chain events. */

  hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb );
    /* Register an unlock callback so that we may be interrupted. */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                "HDR LRE: lte meas enable tune_aways" );  /* REVISIT */

} /* hdrsrchsync_enable_tune_aways */


/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_INIT

DESCRIPTION 
  This function is called to prepare for LTE MEAS state.  The TRM lock has
  been extended for the measurements and there is at least one LTE frequency
  to measure.  DO will attempt to sleep after LTE measurements are done.

  DO enters this state to request LTE pilot measurements of neighboring LTE
  frequencies.  DO uses the the resulting measurements to decide whether or
  not to reselect to LTE.
 
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_init( hdrsrch_state_enum_type last_state )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L Debug hdrsrchltemeas_init() is called" );

  if ( hdrsrch_is_lre_enabled() )
  {
  hdrsrchltemeas_enable_tune_aways();
    /* enable TA and handle unlock callback */
  }

  hdrsrch_default_init( last_state );
    /* Use default handling between states */

  if( ( hdrsrchltemeas_ctrl_info.meas_state != HDRSRCHLTEMEAS_INACTIVE_STATE )  &&
      ( hdrsrchltemeas_ctrl_info.meas_state != HDRSRCHLTEMEAS_INIT_STATE ) )
  {
    
    HDR_MSG_SRCH_2( 
      MSG_LEGACY_ERROR, 
      "D2L Possible mismatch of LTE Init/Deinit req %d in meas substate %d", 
      hdrsrchltemeas_ctrl_info.init_cnf_rxed,
      hdrsrchltemeas_ctrl_info.meas_state );
  }

  if( hdrsrchltemeas_enter_lte_meas() == E_FAILURE )
    /* Start LTE pilot measurement process */
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                  "D2L Enter_lte_meas returns error, leave LTE Meas state" );

    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */
  }

} /* hdrsrchltemeas_init */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_INIT_CNF_MSG

DESCRIPTION
  This function processes the Init confirmation message received from LTE.
  DO can now move to the next state to continue with the LTE messaging
  session for pilot measurement.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_init_cnf_msg
(
  hdrsrch_lte_init_cnf_params_type         *init_cnf_ptr
)

{
  errno_enum_type                          status = E_SUCCESS;
    /* Command completion status */

  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr = NULL;
    /* Ptr to buffer where one LTE frequency parameters are stored */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_process_init_cnf_msg() is called" );

  if( hdrsrchltemeas_ctrl_info.init_cnf_rxed != HDRSRCHLTEMEAS_LTEINIT_SENT )
  {
    HDR_MSG_SRCH_1( 
      MSG_LEGACY_ERROR, 
      "D2L Possible mismatch of LTE Init req %d", 
      hdrsrchltemeas_ctrl_info.init_cnf_rxed );
  }

  hdrsrchltemeas_ctrl_info.init_cnf_rxed = HDRSRCHLTEMEAS_LTEINIT_RXED;
    /* Remember that DO received INIT confirmation from LTE */

  hdrsrchltemeas_ctrl_info.lte_max_tx_power = init_cnf_ptr->max_tx_pwr;
    /* Save Max TX power for S_EUTRA evaluation, a comparison between LTE
       signal strength and a threshold to decide if LTE signal strength is
       strong enough for reselection. */

  if( hdrsrchltemeas_ctrl_info.meas_state == HDRSRCHLTEMEAS_ABORT_STATE )
  {
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_ABORT );
      /* If abort happens before TIMED_SRCH request is sent, report to IRATMan to
         unlock the node. */

    hdrsrchltemeas_end_lte_meas();
      /* Need to wrap up the previous measurement */

    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */

    return status;
  }

  if( hdrsrchltemeas_curr_freq_ptr_arr.cnt > 0 )
  {
    freq_entry_ptr = &hdrsrchltemeas_freq_arr[ 0 ]; 
      /* Already have the IRATMan node as the band was needed to request
         TRM.  So just use this node. */
  }

  if( freq_entry_ptr != NULL )
  {
#ifdef FEATURE_HDR_D2L_RF_MEAS
    status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr, TRUE );
      /* Begin the process to measure the next frequency from list. */
#else
    status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr );
      /* Begin the process to measure the next frequency from list */
#endif /* FEATURE_HDR_D2L_RF_MEAS */

    if( status == E_FAILURE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                    "D2L Start Up state failed to measure next freq" );
    }
  }
  else
  {
    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */
  }

  return status;

} /* hdrsrchltemeas_process_init_cnf_msg */

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

errno_enum_type hdrsrchltemeas_process_deinit_cnf_msg( void )

{
  errno_enum_type                  status = E_SUCCESS;
    /* Command completion status */

  /* Save temp values because all LTE measurement information is reset     
     once LTE reselection occurs.  If LTE fails to acquire, DO should start
     LTE measurements again from scratch, not from staled data. */         
  lte_earfcn_t                     temp_reselect_freq_id = 0;
  uint16                           temp_reselect_cell_id = 0;
  hdrsrch_lte_reselect_cb_type     temp_reselect_cb = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_process_deinit_cnf_msg() is called,state=0x%x",
    hdrsrch_state_id );

  if( hdrsrchltemeas_ctrl_info.init_cnf_rxed != HDRSRCHLTEMEAS_LTEDEINIT_SENT )
  {
    HDR_MSG_SRCH_1( 
      MSG_LEGACY_ERROR, 
      "D2L Possible mismatch of LTE Deinit req %d", 
      hdrsrchltemeas_ctrl_info.init_cnf_rxed );
  }

  hdrsrchltemeas_ctrl_info.init_cnf_rxed = HDRSRCHLTEMEAS_LTEINIT_NONE;
    /* Make sure DO starts in the right state next time */

  if( hdrsrchltemeas_ctrl_info.meas_state == HDRSRCHLTEMEAS_ABORT_STATE ) 
  {
    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */

    return status;
  }

  if( hdrsrchltemeas_ctrl_info.reselection_in_progress == TRUE )
  {
    /* Is this the 2nd pilot measurement for this frequency?  The reselection
       measurement?  If so, then the LTE frequency is the candidate frequency
       for reselection.  First, save temp values for reselection. */

    temp_reselect_freq_id = hdrsrchltemeas_ctrl_info.reselect_freq_id;
    temp_reselect_cell_id = hdrsrchltemeas_ctrl_info.reselect_cell_id;
    temp_reselect_cb = hdrsrchltemeas_ctrl_info.reselect_cb;

    /* Reset all LTE measurement information once LTE reselection occurs.
       If LTE fails to acquire, DO should start LTE measurements again from
       scratch, not from staled data. */
    hdrsrchltemeas_reset_deinit();
  }
 
  if( hdrsrch_state_id == HDRSRCH_INACTIVE_STATE ) 
    /* If called when DO is trying to shutdown */
  {
    hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_INACTIVE_STATE;
      /* Reset sub state for LTE measurements */

    HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L hdrsrchmcpm_do2lte_measurement( FALSE )" );
    hdrsrchmcpm_do2lte_measurement( FALSE );
      /* DO is not in Meas state, MCPM has been turned on for LTE to send Deinit
         request.  So now DO needs to turn it off */
        
    HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L hdrsrchmcpm_set_state( HDRSRCHMCPM_STOP_REQ )" );
    hdrsrchmcpm_set_state( HDRSRCHMCPM_STOP_REQ );
      /* Tell MCPM to powerdown DO clocks */

    return status;
      /* Not in LTE MEAS state, so just return */
  }

  if( hdrsrchltemeas_ctrl_info.reselection_in_progress == TRUE )
  {
    if( temp_reselect_cb != NULL )
    {
      HDR_MSG_SRCH_2( 
        MSG_LEGACY_ERROR,
        "D2L **** RESELECT TO LTE **** earfcn:%d,cell:%d",
        temp_reselect_freq_id,
        temp_reselect_cell_id );
       
      /* 
         ****************************************************************
         ** Start the LTE reselection procedure.                       **
         ****************************************************************
      */  
      HDR_MSG_SRCH( 
        MSG_LEGACY_HIGH,
        "D2L Call lte_ml1_irat_request_chain() to reserve TRM for LTE" );

      if ( lte_ml1_irat_request_chain (
 #ifdef FEATURE_HDR_MSGR_VARIANT
                 hdrsrch_get_subscriber_id(),
#endif /* FEATURE_HDR_MSGR_VARIANT */ 
            temp_reselect_freq_id ) == TRUE)
      {
      temp_reselect_cb( temp_reselect_freq_id, temp_reselect_cell_id );
        /* Notify upper layer to start reselection */
      }
      else
      {
        HDR_MSG_SRCH_2( 
          MSG_LEGACY_ERROR,
          "D2L Unable to reselect LTE could not get TRM, earfcn:%d,cell:%d",
          temp_reselect_freq_id,   
          temp_reselect_cell_id ); 

        hdrsrchltemeas_ctrl_info.reselection_in_progress = FALSE;
      }
    }
    else
    {
      HDR_MSG_SRCH_2( 
        MSG_LEGACY_ERROR,
        "D2L Unable to reselect, Null LTE reselect cb, earfcn:%d,cell:%d",
        temp_reselect_freq_id,   
        temp_reselect_cell_id ); 

      hdrsrchltemeas_ctrl_info.reselection_in_progress = FALSE;
    }
  } /* end if( hdrsrchltemeas_ctrl_info.reselection_in_progress == TRUE ) */

  /* Done with LTE measurements, go back to sleep if DO is in LTEMEAS
     state */
  hdrsrchltemeas_exit_lte_meas();
  
  return status;

} /* hdrsrchltemeas_process_deinit_cnf_msg */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_CURR_LTE_FREQ

DESCRIPTION 
  This function is called to get the current LTE frequency in the wake up
  frequency list to measure.
 
DEPENDENCIES
  None

RETURN VALUE
  Pointer to current LTE frequency

SIDE EFFECTS
  None

===========================================================================*/

hdrsrchltemeas_freq_list_entry_struct_type *hdrsrchltemeas_get_curr_lte_freq( void )

{
  hdrsrchltemeas_freq_list_entry_struct_type *freq_to_meas_ptr = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_get_curr_lte_freq() is called" );

  /* Return the current frequency */
  if( hdrsrchltemeas_curr_freq_ptr_arr.curr_entry < HDRSRCH_LTE_FREQ_ARR_SIZE )
  {
    freq_to_meas_ptr = 
      hdrsrchltemeas_curr_freq_ptr_arr.freq_ptr[ hdrsrchltemeas_curr_freq_ptr_arr.curr_entry ];
  }

  return( freq_to_meas_ptr );

} /* hdrsrchltemeas_get_curr_lte_freq */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TBACKOFF_TIMER_SET

DESCRIPTION
  This function sets the back off timer for this frequency since it failed
  the S_EUTRA reselection comparison.  The back off timer is used to save
  battery life by avoiding continuous searches to LTE frequencies with
  low signal strength.  The back off timer does not require a fast wake up.
  Once the timer expires, it is handled the next time DO enters LTE MEAS state.
 
  Note that the name of this function may be misleading since the backoff timer
  does not require arming a HW timer.  This is done to keep the back off
  timer processing as similar to the reselect timer processing as possible so
  the code is easier to follow.
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_tbackoff_timer_set( void )

{
  hdrsrchltemeas_freq_list_entry_struct_type *curr_freq_ptr = NULL;
    /* Ptr to LTE frequency associated with the current measurement */

  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_tbackoff_timer_set() is called" );

  curr_freq_ptr = hdrsrchltemeas_get_curr_lte_freq();
  ASSERT( curr_freq_ptr != NULL );
    /* Should never happen by the time this function is called */

  curr_freq_ptr->tbackoff_state = HDRSRCHLTEMEAS_TIMER_ACTIVE;
    /* Indicate that Tbackoff timer is active for this frequency */

  curr_freq_ptr->treselect_state = HDRSRCHLTEMEAS_TIMER_INACTIVE;
    /* Make sure Treselect timer is reset */

  curr_freq_ptr->tbackoff_sclks = 
    TIMETICK_SCLK_FROM_MS( hdrsrchltemeas_ctrl_info.ran_backoff_timer_ms );
    /* Timer duration in sclks */

  curr_time_sclks = timetick_get();
    /* Get current time in sclk units */

  curr_freq_ptr->tbackoff_exp_sclks = 
    curr_time_sclks + curr_freq_ptr->tbackoff_sclks;
    /* Calculate time when timer expires, required to find the earliest timer.
       Don't need to worry about wrap-around since timetick_type is unsigned
       and several order magnitude larger than timeout values. */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "D2L Set Tbackoff timer to %d",
                  curr_freq_ptr->tbackoff_sclks );

} /* hdrsrchltemeas_tbackoff_timer_set */

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

boolean hdrsrchltemeas_iratman_wakeup_cb( void )
{
  boolean                         ret_value = FALSE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_iratman_wakeup_cb() is called" );

  if( hdrsrch_state_id != HDRSRCH_INACTIVE_STATE ) 
    /* If called when DO is not inactive */
  {
    hdrsrchltemeas_lte_treselect_timer_exp();
      /* Send a command to handle the rest of the expiry processing in DO task
         context */
    ret_value = TRUE;
  }

  return ret_value;

} /* hdrsrchltemeas_iratman_wakeup_cb */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_IRATMAN_REPORT_MEAS_RESULTS

DESCRIPTION
  This function reports measurement results to IRATMan.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_iratman_report_meas_results
( 
  hdrsrchltemeas_meas_report_enum_type      meas_rpt
)

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( hdrsrchltemeas_ctrl_info.iratman_meas_node.lte.valid == FALSE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Report received for invalid meas node" );
    return;
  }

  if( hdrsrchltemeas_ctrl_info.iratman_report_pending == FALSE )
  {
    return;
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "D2L Debug hdrsrchltemeas_iratman_report_meas_results(),meas_rpt=%d",
                  meas_rpt );

  memset( &hdrsrchltemeas_ctrl_info.iratman_meas_rpt,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_meas_rpt ) );
    /* Reset report */

  memset( &hdrsrchltemeas_ctrl_info.iratman_resel_node,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_resel_node ) );
    /* Reset reselect result */

  hdrsrchltemeas_ctrl_info.iratman_meas_rpt.lte.lte_max_tx_power = 
    hdrsrchltemeas_ctrl_info.lte_max_tx_power;
    /* Max TX power does not change */

  switch( meas_rpt )
  {
    case HDRSRCHLTEMEAS_MEAS_SUCCESS:
    case HDRSRCHLTEMEAS_MEAS_NO_CELL:
      hdrsrchltemeas_ctrl_info.iratman_meas_rpt.lte.lte_meas_cnf_params_ptr =
        &hdrsrchltemeas_saved.lte_meas_cnf_params;
        /* Fill out report */

      hdrsrchltemeas_ctrl_info.iratman_resel_node =
        iratman_report_meas
          (
            RAT_HDR,                                 
            RAT_LTE,                                 
            &hdrsrchltemeas_ctrl_info.iratman_meas_node,
            &hdrsrchltemeas_ctrl_info.iratman_meas_rpt 
          );
        /* Report LTE frequency measurement results to IRATMan */

      break;

    case HDRSRCHLTEMEAS_MEAS_INVALID: 
    case HDRSRCHLTEMEAS_MEAS_ABORT:  
      hdrsrchltemeas_ctrl_info.iratman_resel_node =
        iratman_abort_meas
          ( 
            RAT_HDR, 
            RAT_LTE,
            &hdrsrchltemeas_ctrl_info.iratman_meas_node
          );
        /* Send measurement abort to IRATMan */

      break;

    default:
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "D2L Unexpected report type %d",
                      meas_rpt );

      hdrsrchltemeas_ctrl_info.iratman_resel_node =
        iratman_abort_meas
          ( 
            RAT_HDR, 
            RAT_LTE,
            &hdrsrchltemeas_ctrl_info.iratman_meas_node
          );
        /* Send measurement abort to IRATMan */

      break;

  } /* end switch */

  hdrsrchltemeas_ctrl_info.iratman_report_pending = FALSE;
    /* Just sent report, no longer pending */

} /* hdrsrchltemeas_iratman_report_meas_results */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_SEARCH_CNF_MSG

DESCRIPTION
  This function processes the Search confirmation message received from LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_search_cnf_msg
( 
  hdrsrch_lte_search_cnf_params_type         *search_cnf_ptr
)

{
  errno_enum_type                            status = E_SUCCESS;
    /* Command completion status */

  uint8                                      i = 0;
    /* For for loop to store cell information from LTE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_process_search_cnf_msg() is called" );

  if( hdrsrch_state_id != HDRSRCH_LTE_MEAS_IDLE_STATE ) 
  {
    /* If this message is received in the wrong state, report to IRATMan then
       drop the message */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_INVALID );
    return status;
  }

  if( hdrsrchltemeas_ctrl_info.meas_state == HDRSRCHLTEMEAS_ABORT_STATE ) 
  {
    /* DO already sent an Abort request, so just ignore this Search confirmation.
       DO needs to wait for the Abort confirmation which should be arriving after
       this confirmation. */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_ABORT );
    return status;
  }

  if( search_cnf_ptr->num_detected_cells > LTE_CPHY_IMEAS_MAX_DETECTED_CELLS ) 
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                    "D2L Number of detected cells=%d exceeds Max",
                    search_cnf_ptr->num_detected_cells );

    /* Report to IRATMan then drop the message */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_INVALID );
    status = E_FAILURE;
    return status;
  }
  else if( search_cnf_ptr->num_detected_cells == 0 )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "D2L LTE detected no cells, send clean up request" );

    memset( &hdrsrchltemeas_saved.lte_search_cnf_params, 0,
            sizeof( hdrsrchltemeas_saved.lte_search_cnf_params ) );
      /* Clear search cnf storage area */

    hdrsrchltemeas_saved.lte_search_cnf_params.freq = 
      search_cnf_ptr->freq;
    hdrsrchltemeas_saved.lte_search_cnf_params.num_detected_cells = 0;
      /* Save search confirmation information */

    memset( &hdrsrchltemeas_saved.lte_meas_cnf_params, 0,
            sizeof( hdrsrchltemeas_saved.lte_meas_cnf_params ) );
      /* Clear meas cnf storage area */

    hdrsrchltemeas_saved.lte_meas_cnf_params.freq = 
      search_cnf_ptr->freq;
    hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells = 0;
      /* Save search confirmation information in meas area to report to IRATMan */

    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_NO_CELL );
      /* Found no cell, so immediately report meas results */

    /* Send Clean Up req message, resume after confirmation is received */
    hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_CLEANUP_REQ;
    hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_CLEANUP_STATE;
    hdrsrchmsg_d2l_send_cleanup_req_msg();

    return status;
  }

  /* Here if LTE detected good cells, continue on with measurement request */

  memset( &hdrsrchltemeas_saved.lte_search_cnf_params, 0,
          sizeof( hdrsrchltemeas_saved.lte_search_cnf_params ) );
    /* Clear storage area */

  hdrsrchltemeas_saved.lte_search_cnf_params.freq = 
    search_cnf_ptr->freq;
  hdrsrchltemeas_saved.lte_search_cnf_params.num_detected_cells =
    search_cnf_ptr->num_detected_cells;
    /* Save search confirmation information */

  for( i = 0; i < search_cnf_ptr->num_detected_cells; i++ ) 
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    hdrsrchltemeas_saved.lte_search_cnf_params.det_cells[ i ] = 
                                        search_cnf_ptr->det_cells[ i ];
#else
    memcpy( &hdrsrchltemeas_saved.lte_search_cnf_params.det_cells[ i ],
            &search_cnf_ptr->det_cells[ i ],
            sizeof( lte_cphy_irat_meas_detected_cell_s ) 
          );
#endif /* FEATURE_MEMCPY_REMOVAL */
    /* Save cell information */
  }

  /* Send Measurement req message, resume after confirmation is received.
     The payload consists of information LTE sent previously in the LTE
     Search confirmation message. */
  hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_MEAS_REQ;
  hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_MEAS_STATE;
  status = 
    hdrsrchmsg_d2l_send_meas_req_msg( &hdrsrchltemeas_saved.lte_search_cnf_params );

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Meas req failed" );
    return status;
  }

  return status;

} /* hdrsrchltemeas_process_search_cnf_msg */

/*=========================================================================

FUNCTION     : HDRSRCHLTEMEAS_LTE_TRESELECT_TIMER_EXP

DESCRIPTION  : This function is called when the LTE TReselect timer has expired. 
               At this time, DO requests one more LTE pilot measurement.  If this
               pilot measurement meets reselection criteria, Mobile will reselect
               to LTE.  All this is done in DO task context.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

=========================================================================*/

void hdrsrchltemeas_lte_treselect_timer_exp( void )
{

  hdrsrch_cmd_type  *cmd;
    /* Command structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_lte_treselect_timer_exp() is called" );

  cmd = hdrsrchcmd_new( HDRSRCH_LTE_TRESELECT_EXP_CMD );
  /* Command HDRSRCH_TASK to handle LTE Treselect timer expiry */

  if ( cmd != NULL )
  {
    hdrsrchcmd_post( cmd );
      /* Post the command */
  }

} /* hdrsrchltemeas_lte_treselect_timer_exp */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_EXP_CB

DESCRIPTION
  This function is called when the Treselect timer expires.  The LTE frequency
  with the expired timer is marked, and DO is woken up if it is currently in
  sleep state.  All this is done in DO task context, thus this function only
  sends a DO command here.
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_treselect_timer_exp_cb
(
  timer_cb_data_type              unused_parm
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)

{
  ( void )unused_parm;
    /* Unused parameters - avoid compiler warning */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_treselect_timer_exp_cb() is called" );

  ASSERT( hdrsrchltemeas_ctrl_info.treselect_freq_ptr != NULL );
    /* Make sure frequency information associated with the expired timer is
       available */

  hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_state = 
    HDRSRCHLTEMEAS_TIMER_EXPIRED;
    /* To avoid possible race conditions by adding extra delay, indicate that
       Treselect timer has expired for this frequency right now */

  hdrsrchltemeas_ctrl_info.reselect_timer_cnt--;
    /* Account for 1 less Treselect timer */

  hdrsrchltemeas_lte_treselect_timer_exp();
    /* Send a command to handle the rest of the expiry processing in DO task
       context */

} /* hdrsrchltemeas_treselect_timer_exp_cb */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_ACTIVE_TRESELECT_MIN

DESCRIPTION 
  This function finds the frequency with active Treselect
  timer and the shortest expiration time in the following frequency lists:
   - The more preferred list contains all the frequencies to be measured every DO
     wake up period.
   - The less preferred list contains all the frequencies with priority <= DO
     priority.  These frequencies are measured if DO signal strength is lower
     than the serving frequency threshold specified in the Other IRAT neighbor
     list OTA message.
 
DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  Ptr to frequency with active Treselect timer and the minimum expiry time.
  This function may return NULL if no timer is found.

SIDE EFFECTS 
  None 

=========================================================================*/
hdrsrchltemeas_freq_list_entry_struct_type *hdrsrchltemeas_get_active_treselect_min
( 
  timetick_type                              curr_time_in_sclks
    /* Current time read from Timer Services */
)

{
  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

  hdrsrchltemeas_freq_list_entry_struct_type *min_ptr = NULL;
    /* Ptr to buffer with shortest Treselect timer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_get_active_treselect_min() is called" );

  /* Look for frequency that has minimum Treselect timer 
     in the more preferred list */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr;
  while( freq_entry_ptr != NULL )
  { 
    if( min_ptr == NULL ) 
    {
      if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
      {
        min_ptr = freq_entry_ptr;
      }
    }
    else
    { 
      if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
      {
        /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
         * Note: This only works for large unsigned integers where the delta
         * between time 1 < time 2 is smaller than half of the
         * number set.  These 2 time values are uint32 values. */
        if( TIME_OCCURS_FIRST( freq_entry_ptr->treselect_exp_sclks, 
                               min_ptr->treselect_exp_sclks ) )
        {
          min_ptr = freq_entry_ptr;
        }
      }
    }    
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  /* Continue the search in the less preferred list */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr;
  while( freq_entry_ptr != NULL )
  { 
    if( min_ptr == NULL ) 
    {
      if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
      {
        min_ptr = freq_entry_ptr;
      }
    }
    else
    { 
      if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
      {
        /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
         * Note: This only works for large unsigned integers where the delta
         * between time 1 < time 2 is smaller than half of the
         * number set.  These 2 time values are uint32 values. */
        if( TIME_OCCURS_FIRST( freq_entry_ptr->treselect_exp_sclks, 
                               min_ptr->treselect_exp_sclks ) )
        {
          min_ptr = freq_entry_ptr;
        }
      }
    }
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  if( min_ptr != NULL ) 
  {
    min_ptr->treselect_sclks = min_ptr->treselect_exp_sclks - curr_time_in_sclks;
      /* Adjust the Treselect timer to account for elapsed time until now.  The
         number of sclks remaining will be used to arm the HW timer. */
  }

  return( min_ptr );

} /* hdrsrchltemeas_get_active_treselect_min */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_SET

DESCRIPTION
  This function sets the reselect timer for a frequency since it passed
  the S_EUTRA reselection comparison.  Since only 1 HW timer is used for
  multiple frequencies, the timer is set to expire for the frequency with the
  shortest expiry time from now.
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_treselect_timer_set( void )

{
  hdrsrchltemeas_freq_list_entry_struct_type *curr_freq_ptr = NULL;
    /* Ptr to LTE frequency associated with the current measurement */

  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_treselect_timer_set() is called" );

  curr_freq_ptr = hdrsrchltemeas_get_curr_lte_freq();
  ASSERT( curr_freq_ptr != NULL );
    /* Should never happen by the time this function is called */

  curr_freq_ptr->treselect_state = HDRSRCHLTEMEAS_TIMER_ACTIVE;
    /* Indicate that Treselect timer is active for this frequency */

  curr_freq_ptr->tbackoff_state = HDRSRCHLTEMEAS_TIMER_INACTIVE;
    /* Make sure Tbackoff timer is reset */

  hdrsrchltemeas_ctrl_info.reselect_timer_cnt++;
    /* Account for 1 additional Treselect timer */

  curr_time_sclks = timetick_get();
    /* Get current time in sclk units */

  curr_freq_ptr->treselect_sclks = 
    TIMETICK_SCLK_FROM_MS( hdrsrchltemeas_ctrl_info.ran_reselect_timer_ms );
    /* Timer duration in sclks, required to set timer */

  curr_freq_ptr->treselect_exp_sclks = 
    curr_time_sclks + curr_freq_ptr->treselect_sclks;
    /* Calculate time when timer expires, required to find the earliest timer.
       Don't need to worry about wrap-around since timetick_type is unsigned
       and several order magnitude larger than timeout values. */
 
  HDR_MSG_SRCH_3( 
    MSG_LEGACY_MED, 
    "D2L Treselect(sclks):curr=%d,duration=%d,exp=%d",
    curr_time_sclks,
    curr_freq_ptr->treselect_sclks, 
    curr_freq_ptr->treselect_exp_sclks );
 
  if( hdrsrchltemeas_ctrl_info.reselect_timer_cnt > 1 )
    /* If there are also other timers active, need to find the earliest timer
       to arm, using the system time.  This way, one HW timer can be used
       for multiple Treselect timers by setting up another one every time one
       expires */
  {
    hdrsrchltemeas_ctrl_info.treselect_freq_ptr = 
      hdrsrchltemeas_get_active_treselect_min( curr_time_sclks );
      /* If there are multiple active timers, find the Treselect timer with the
         shortest expiration from now */
  }
  else
  {
    hdrsrchltemeas_ctrl_info.treselect_freq_ptr = curr_freq_ptr;
      /* Only one timer is active, so remember it */
  }

  if( hdrsrchltemeas_ctrl_info.treselect_freq_ptr != NULL )
  {
    if( hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks == 0 )
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Treselect sclk cnt = 0, do not arm timer" );
      
      hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_state = 
        HDRSRCHLTEMEAS_TIMER_EXPIRED;
        /* Since sclk count is 0, indicate that Treselect timer has expired without
           arming HW timer.  The expired timer will be processed next wake up time.
           This should happen in extremely are cases where 2 treselect timers expires
           very close to one another. */

      hdrsrchltemeas_ctrl_info.treselect_freq_ptr->tbackoff_state = 
        HDRSRCHLTEMEAS_TIMER_INACTIVE;
        /* Make sure Tbackoff timer is reset */
    }
    else if( hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks > 
        ( TIMETICK_SCLK_FROM_MS( hdrsrchltemeas_ctrl_info.max_reselect_timer_ms ) ) )
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, 
                      "D2L Treselect timer %d exceeds max, reset to %d and set timer",
                      hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks,
                      hdrsrchltemeas_ctrl_info.max_reselect_timer_ms  );

      hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks =
          ( TIMETICK_SCLK_FROM_MS( hdrsrchltemeas_ctrl_info.max_reselect_timer_ms ) );
        /* Treselect timer exceeds max.  Something is wrong, report error and cap the
           reselect timer at max value */

      timer_set( &hdrsrchltemeas_ctrl_info.reselect_timer, 
                 hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks,
                 0,
                 T_SCLK );
      /* Set timer for the shortest duration in sclk units */
    }
    else 
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                      "D2L Set Treselect timer to %d",
                      hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks );

      timer_set( &hdrsrchltemeas_ctrl_info.reselect_timer, 
                 hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks,
                 0,
                 T_SCLK );
      /* Set timer for the shortest duration in sclk units */
    }
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                    "D2L Unexpected Treselect timer count = %d",
                    hdrsrchltemeas_ctrl_info.reselect_timer_cnt );
  }

} /* hdrsrchltemeas_treselect_timer_set */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_DEF

DESCRIPTION
  This function defines the Treselect timer for use in DO->LTE reselection.
  
DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchltemeas_treselect_timer_def( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_treselect_timer_def() is called" );

  timer_def( &hdrsrchltemeas_ctrl_info.reselect_timer, 
             &hdrsrchltemeas_ctrl_info.reselect_timer_group, 
             HDRSRCH_TCB,
             0, 
             hdrsrchltemeas_treselect_timer_exp_cb, 
             0 );
    /* Define the Treselect timer */

} /* hdrsrchltemeas_treselect_timer_def */

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
void hdrsrchltemeas_treselect_timer_exp( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_treselect_timer_exp() is called" );

  /* If DO is sleeping, perform fast wake up to handle this timer expiry.  If
     DO is awake, eventually it will go to sleep.  LTE measurements (including the
     Treselect measurement) are handled when DO starts sleeping.
   
     Since DO most likely will reselect to LTE, it is decided that DO will perform
     a normal wake up and go back to sleep as usual at which time it will handle
     the LTE timer expiry.  This eliminates a more risky approach of having another
     special state to handle Treselect expiry, which will be messy since DO still
     needs to request the TRM to take care of the LTE Treselect candidate frequency.
   
     If DO is in any traffic states, reset LTE measurement state since DO may stay
     in traffic for a long time.  It is not desirable to use an out dated measurement
     prior to traffic as the first measurement for reselection. */

  if( hdrsrch_state_id == HDRSRCH_SLEEP_STATE ) 
  {
    (void) hdrsrchsleep_fast_wakeup( );

  }
} /* hdrsrchltemeas_treselect_timer_exp */ 

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_S_EUTRA_IS_GREATER_THAN_THRESHX 
 
DESCRIPTION
  This function evaluates the LTE Pilot measurement results using the following
  equation:
 
  S_EUTRA = Q_measured - (Qrxlevmin+Qrxlevminoffset)  -  max(Pemax-UEmaxpower,0)
 
  where:
 
  Q_measured:      Returned from LTE in the measurement result confirmation message.
                   Each cell's Q_measured is stored in an array.
                   RSRP combined value combined across Tx-Rx pairs. In linear scale. 
                   Range -144dBm..-44dBm with resolution of 1 dBm.
 
  Qrxlevmin:       Minimum received RSRP level from the EUTRA cell in dBm.
                   Range 0 to 96, where -44 – Rxlevmin RxL is equal to the
                   minimum reference signal received power (RSRP) level of a EUTRA 
 
  Qrxlevminoffset: Offset to Minimum reference power level to select EUTRA.
                   The range of values allowed is 1 through 8.
 
  Pemax:           Maximum TX power level an UE may use when transmitting on the
                   uplink in E-UTRA (dBm.)
 
  UEmaxpower:      Maximum transmit power received from LTE L1 in Init confirmation
                   message in dBm.
 
  For all cells returned by LTE, the cell's S_EUTRA is compared
  against the threshold ThreshX which is part of the Other RAT Neighborlist OTA
  message.  The result of the comparison is returned to the calling function.
 
  ThreshX:         Minimum required signal quality threshold used for selecting an
                   EARFCN in the range 0 to 31. The actual threshold value in dB used
                   to compare against the measured reference signal receive power
                   (RSRP) is equal to ThreshX * 2.
 
DEPENDENCIES
  None

RETURN VALUE
  TRUE:  S_EUTRA greater than or equal to threshold
  FALSE:  S_EUTRA less than threshold

SIDE EFFECTS 
  None 

===========================================================================*/

boolean hdrsrchltemeas_s_eutra_is_greater_than_threshx
(
  hdrsrchltemeas_freq_list_entry_struct_type *curr_freq_ptr
    /* Ptr to LTE frequency associated with the current measurement */
)

{
  boolean                                    result = FALSE;
    /* Command completion status */

  uint8                                      idx = 0;
    /* For loop to process all cells */

  int16                                      s_eutra;
    /* Temp storage for S_EUTRA in the equation */

  int16                                      q_measured;
    /* Temp storage for Q_measured (LTE pilot measurement result) in the equation */

  int16                                      rxlevelmin;
    /* Temp storage for the RxLevelMin component of the equation */

  int16                                      txlevelmax;
    /* Temp storage for the TxLevelMax component of the equation */

  hdrsrchltemeas_reselect_cell_arr_struct_type  
                                             *curr_reselect_cell_arr_ptr = NULL;
    /* Ptr to either array 1 for the first measurement, or array 2 for the
       second measurement */

  uint8                                      cell_idx = 0;
    /* Index to store cells which pass reselection criteria */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_evaluate_s_eutra() is called" );

  ASSERT( curr_freq_ptr != NULL );
    /* Should never happen by the time this function is called */

  /* Perform range checking of cell array */
  if( hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells > 
      LTE_CPHY_IMEAS_MAX_MEAS_CELLS ) 
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                    "D2L Number of s_eutra meas cells=%d exceeds Max",
                    hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells );
    return( result );
  }

  curr_reselect_cell_arr_ptr = &curr_freq_ptr->reselect_cell_arr1;
    /* Start with cell array 1 */

  /* Perform range checking of RxLevelMin.  Sacrifice the long lines for clarity. */
  if( ( curr_freq_ptr->earfcn_to_meas.rx_lev_min_eutra > HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_HIGH ) ||
      ( curr_freq_ptr->earfcn_to_meas.rx_lev_min_eutra_offset > HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_OFFSET_HIGH ) 
    )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, 
                    "D2L rx_lev_min %d, or offset %d out of range",
                    curr_freq_ptr->earfcn_to_meas.rx_lev_min_eutra,
                    curr_freq_ptr->earfcn_to_meas.rx_lev_min_eutra_offset );
    return( result );
  }

  /* Determine if this is the first, or second measurement to save cell ID info
     in the appropriate array */
  if( ( curr_freq_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_EXPIRED ) &&
      ( curr_freq_ptr == hdrsrchltemeas_ctrl_info.treselect_freq_ptr ) )
  {
    curr_reselect_cell_arr_ptr = &curr_freq_ptr->reselect_cell_arr2;
    HDR_MSG_SRCH( MSG_LEGACY_MED, 
                  "D2L This is the 2nd measurement" );
  }

  /* This for loop calculates S_EUTRA then compares it to ThreshX.  If S_EUTRA is
     greater than ThreshX * 2, then the cell (and frequency) meets the reselection
     criteria.
   
     Note that S_EUTRA, per spec, is in dBm unit, while ThreshX is in dB unit.  There
     is an ambiguity as to how this comparison should be done.  For now, ThreshX
     is just multiplied by 2 and a direct comparison is made.  */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "D2L NumCells=%d", 
    hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells );

  for( idx = 0; idx < hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells; idx++ ) 
  {
    q_measured = hdrsrchltemeas_saved.lte_meas_cnf_params.cells[ idx ].rsrp;
      /* Calculate the Q_measured component of the equation.  LTE provides RSRP in
         dBm units, range is -144dBm to -44dBm */

    rxlevelmin = 
      ( int16 )( ( HDRSRCHLTEMEAS_RX_LEVEL_MIN_EUTRA_CEILING - 
                   curr_freq_ptr->earfcn_to_meas.rx_lev_min_eutra ) + 
                   curr_freq_ptr->earfcn_to_meas.rx_lev_min_eutra_offset ) ;
      /* Calculate the RxLevelMin component of the equation in dBm unit */

    txlevelmax = MAX( 
      ( ( ( int16 ) ( curr_freq_ptr->earfcn_to_meas.pe_max ) + HDRSRCHLTEMAS_PE_MAX_OFFSET ) -
        ( int16 ) ( hdrsrchltemeas_ctrl_info.lte_max_tx_power ) ),
      0 );
      /* Calculate the TxLevelMax component of the equation in dBm unit.
         Adjust pe_max from range of 0 to +63 dBm to range of -30 to +33 dBm
         (required because of different configuration on Network side.) */

    s_eutra = q_measured - rxlevelmin - txlevelmax;
      /* Calculate S_EUTRA per spec requirement */

    HDR_MSG_SRCH_6( 
      MSG_LEGACY_HIGH,
      "D2L CellID=%d: S_EUTRA(%d) = Qmeasured(%d) - RxLevelMin(%d) - TxLevelMax(%d)) > (ThreshX*2 (%d))", 
      hdrsrchltemeas_saved.lte_meas_cnf_params.cells[ idx ].cell_id,
      s_eutra, 
      q_measured, 
      rxlevelmin, 
      txlevelmax, 
      ( curr_freq_ptr->earfcn_to_meas.thresh_x * 2 ) );

    if( s_eutra > ( curr_freq_ptr->earfcn_to_meas.thresh_x * 2 ) )
    {
      /* One LTE cell in one LTE frequency passed the reselection criteria.
         Need to save frequency and cell information to activate reselection process
         with upper layer. */
      result = TRUE;
      hdrsrchltemeas_ctrl_info.reselect_freq_id = curr_freq_ptr->earfcn_to_meas.earfcn;
      curr_reselect_cell_arr_ptr->cell_arr[ cell_idx ].cell_id =
        hdrsrchltemeas_saved.lte_meas_cnf_params.cells[ idx ].cell_id;
      curr_reselect_cell_arr_ptr->cell_arr[ cell_idx ].s_eutra_val = s_eutra;
      cell_idx++;
    }
  } /* end for loop */

  if( result == TRUE )
    /* One or more cells passed reselection criteria and was stored in the
       appropriate cell array. Update array count. */
  {
    if( cell_idx > 0 )
    {
      curr_reselect_cell_arr_ptr->cnt = cell_idx;
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                      "D2L %d cell(s) exceed(s) ThreshX",
                      cell_idx );
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Cell count is zero, report no LTE cell" );
      curr_reselect_cell_arr_ptr->cnt = 0;
      result = FALSE;
    }
  }

  return result;

} /* hdrsrchltemeas_s_eutra_is_greater_than_threshx */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_SELECT_WINNING_CELL_2ND_TIME
 
DESCRIPTION 
  This function implements the following algorithm, as specified by System
  Engineeering:
 
    EUTRAReselect timer per cell:  Following algorithm will be implemented to
    ensure that the cell to which the UE is reselecting to is stable across
    two measurements.
    
    At time 0, when EUTRAReselect is started, DO L1 will maintain a list of
    cells that exceed ThreshX.
 
    At time EUTRAReselect, DO L1 will compare the new list of cells that exceed
    ThreshX with the stored list.  If there is an overlap between the two lists,
    reselection will be performed to the strongest cell (among those in the
    overlap list) during the 2nd measurement. 
 
DEPENDENCIES
  None

RETURN VALUE
  TRUE:  The strongest cell has been found
  FALSE:  No cell found, error condition 

SIDE EFFECTS 
  None 

===========================================================================*/

boolean hdrsrchltemeas_select_winning_cell_2nd_time
( 
  hdrsrchltemeas_freq_list_entry_struct_type *curr_freq_ptr
    /* Ptr to LTE frequency associated with the current measurement */
)

{
  uint8                                      idx1 = 0;
  uint8                                      idx2 = 0;
    /* Nested for loop indexes to process all cells */

  hdrsrchltemeas_reselect_cell_struct_type   max_cell;
    /* Cell with max level of energy */

  boolean                                    found_winning_cell = FALSE;
    /* Return whether or not the winning cell was found */

  hdrsrchltemeas_reselect_cell_arr_struct_type  
                                             *reselect_cell_arr1_ptr = NULL;
    /* Ptr to array 1, containing winning cell IDs in the 1st measurement */

  hdrsrchltemeas_reselect_cell_arr_struct_type  
                                             *reselect_cell_arr2_ptr = NULL;
    /* Ptr to array 2, containing winning cell IDs in the 2nd measurement */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_select_winning_cell_2nd_time() is called" );

  ASSERT( curr_freq_ptr != NULL );
    /* Should never happen by the time this function is called */

  reselect_cell_arr1_ptr = &curr_freq_ptr->reselect_cell_arr1;
    /* Ptr to array 1, containing winning cell IDs in the 1st measurement */

  reselect_cell_arr2_ptr = &curr_freq_ptr->reselect_cell_arr2;
    /* Ptr to array 2, containing winning cell IDs in the 2nd measurement */

  ASSERT(( ( reselect_cell_arr1_ptr->cnt > 0 ) &&
           ( reselect_cell_arr1_ptr->cnt <= HDRSRCHLTEMEAS_MAX_MEAS_CELLS )) &&
         ( ( reselect_cell_arr2_ptr->cnt > 0 ) &&
           ( reselect_cell_arr2_ptr->cnt <= HDRSRCHLTEMEAS_MAX_MEAS_CELLS ) ));
    /* Perform range checking of cell arrays */

  max_cell.s_eutra_val = 0;
  max_cell.cell_id = 0;
    /* Initialize max cell to zero */

  /* These nested for loops find a match between cell array 1 containing cell
     IDs of those which pass reselection criteria for the first measurement,
     and cell array 2 containing cell IDs of those which pass reselection
     criteria for the second measurement.  The matched cell ID with the
     greatest energy level is the winning cell ID. */

  for( idx1 = 0; idx1 < reselect_cell_arr1_ptr->cnt; idx1++ ) 
  {
    for( idx2 = 0; idx2 < reselect_cell_arr2_ptr->cnt; idx2++ )
    {
      if ( reselect_cell_arr1_ptr->cell_arr[ idx1 ].cell_id ==
           reselect_cell_arr2_ptr->cell_arr[ idx2 ].cell_id )
      {
        if( reselect_cell_arr2_ptr->cell_arr[ idx2 ].s_eutra_val > 
            max_cell.s_eutra_val )
        {
          max_cell = reselect_cell_arr2_ptr->cell_arr[ idx2 ];
          found_winning_cell = TRUE;
        }
      }
    }
  }

  HDR_MSG_SRCH_2( 
    MSG_LEGACY_HIGH, 
    "D2L Debug select_winning_cell_2nd_time() is called,cellID1=%d,cellID2=%d",
    reselect_cell_arr1_ptr->cell_arr[ 0 ].cell_id,
    reselect_cell_arr2_ptr->cell_arr[ 0 ].cell_id );

  /* If the winning cell is found, save the cell ID to give to upper layer in the
     reselection call back function. */

  if( found_winning_cell == TRUE )
  {
    hdrsrchltemeas_ctrl_info.reselect_cell_id = max_cell.cell_id;
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, 
                    "D2L Winning cell ID=%d, S_EUTRA=%d",
                    hdrsrchltemeas_ctrl_info.reselect_cell_id,
                    max_cell.s_eutra_val );
  }

  return found_winning_cell;

} /* hdrsrchltemeas_select_winning_cell_2nd_time */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_SELECT_WINNING_CELL_1ST_TIME
 
DESCRIPTION 
  If there is no match between the first and 2nd measurements, this function
  goes further to see if there is any cell in the 2nd measurement which exceeds
  the reselection criteria.  
 
DEPENDENCIES
  None

RETURN VALUE
  TRUE:  One or more cells in the 2nd measurement passed reselection criteria
  FALSE:  No cell passed in the 2nd measurement, error condition

SIDE EFFECTS 
  None 

===========================================================================*/

boolean hdrsrchltemeas_select_winning_cell_1st_time
( 
  hdrsrchltemeas_freq_list_entry_struct_type *curr_freq_ptr
    /* Ptr to LTE frequency associated with the current measurement */
)

{
  uint8                                      idx = 0;
    /* Loop index */

  boolean                                    found_winning_cell = FALSE;
    /* Should at least get 1 winning cell in the 2nd measurement */

  hdrsrchltemeas_reselect_cell_arr_struct_type  
                                             *reselect_cell_arr1_ptr = NULL;
    /* Ptr to array 1, containing winning cell IDs in the 1st measurement */

  hdrsrchltemeas_reselect_cell_arr_struct_type  
                                             *reselect_cell_arr2_ptr = NULL;
    /* Ptr to array 2, containing winning cell IDs in the 2nd measurement */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( 
    MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_select_winning_cell_1st_time() is called" );

  ASSERT( curr_freq_ptr != NULL );
    /* Should never happen by the time this function is called */

  reselect_cell_arr1_ptr = &curr_freq_ptr->reselect_cell_arr1;
    /* Ptr to array 1, containing winning cell IDs in the 1st measurement */

  reselect_cell_arr2_ptr = &curr_freq_ptr->reselect_cell_arr2;
    /* Ptr to array 2, containing winning cell IDs in the 2nd measurement */

  ASSERT( ( reselect_cell_arr2_ptr->cnt > 0 ) &&
          ( reselect_cell_arr2_ptr->cnt <= HDRSRCHLTEMEAS_MAX_MEAS_CELLS ) );
    /* Perform range checking of the 2nd measurement cell array */

  memset( reselect_cell_arr1_ptr, 0, 
          sizeof( curr_freq_ptr->reselect_cell_arr1 ) );
    /* Reset the cell array for the first measurement before updating it */

  /* Get here if there is no match between the 1st and 2nd measurement, but
     one or more of the cells in the 2nd measurement exceed reselection
     threshold.  So save the list as the 1st measurement. */

  for( idx = 0; idx < reselect_cell_arr2_ptr->cnt; idx++ )
  {
    reselect_cell_arr1_ptr->cell_arr[ idx ] = 
      reselect_cell_arr2_ptr->cell_arr[ idx ];
    reselect_cell_arr1_ptr->cnt++; 
    found_winning_cell = TRUE;
  }

  HDR_MSG_SRCH_1( 
    MSG_LEGACY_FATAL, 
    "D2L Debug select_winning_cell_1st_time() is called,cellID=%d",
    reselect_cell_arr1_ptr->cell_arr[ 0 ].cell_id );

  memset( reselect_cell_arr2_ptr, 0, 
          sizeof( curr_freq_ptr->reselect_cell_arr2 ) );
    /* Reset the cell array containing the 2nd measurement to get rid of stale
       data */

  return found_winning_cell;

} /* hdrsrchltemeas_select_winning_cell_1st_time */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_MEAS_RESULTS

DESCRIPTION
  This function processes the LTE Pilot measurement results.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_meas_results( void )

{
  errno_enum_type                            status = E_SUCCESS;
    /* Command completion status */

  hdrsrchltemeas_freq_list_entry_struct_type *curr_freq_ptr;
    /* Ptr to LTE frequency associated with the current measurement */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_process_meas_results() is called" );

  curr_freq_ptr = hdrsrchltemeas_get_curr_lte_freq();
  ASSERT( curr_freq_ptr != NULL );
    /* Should never happen by the time this function is called */

  /* Evaluate LTE signal strength (S_EUTRA) against the threshold given by base
     station.  Allow the creation of treselect timer in test mode. */

  if( hdrsrchltemeas_s_eutra_is_greater_than_threshx( curr_freq_ptr ) == TRUE )
  {
    /* Is this the 2nd pilot measurement for this frequency?  The reselection
       measurement?  If so, perform more checks. */
    if( ( curr_freq_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_EXPIRED ) &&
        ( curr_freq_ptr == hdrsrchltemeas_ctrl_info.treselect_freq_ptr ) )
    {
      if( hdrsrchltemeas_select_winning_cell_2nd_time( curr_freq_ptr ) == TRUE )
      {
        /* Start the reselection procedure here.  LTE freq passed reselection 
           criteria.  But first, need to send LTE the clean up request and deinit
           request. */
        hdrsrchltemeas_ctrl_info.reselection_in_progress = TRUE;
      }
      else if( hdrsrchltemeas_select_winning_cell_1st_time( curr_freq_ptr ) == TRUE )
      {
        /* Did not find a match between the 2 measurements, but if there is a
           cell that passes the reselection criteria in the 2nd measurement, it
           is saved in the list as the 1st measurement.  Now start the reselect
           timer. */
        hdrsrchltemeas_treselect_timer_set();
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                      "D2L Could not find winning cell ID, suspect error" );

        /* Set back off timer for this frequency.  This is an error
           condition, trying to recover. */
        hdrsrchltemeas_tbackoff_timer_set();
      }
    }
    else
    {
      /* Set reselect timer for this frequency.  This can be the candidate
         frequency for reselection.  One final measurement will be taken when 
         the reselect timer expires.  If the final measurement also passes the
         reselection criteria, the reselection process will start. */
      hdrsrchltemeas_treselect_timer_set();
    }
  }
  else
  {
    /* Set back off timer for this frequency since it failed
       the S_EUTRA reselection comparison.  The back off timer is used to save
       battery life by avoiding continuous searches to LTE frequencies with
       low signal strength. */
    hdrsrchltemeas_tbackoff_timer_set();
  }

  return status;

} /* hdrsrchltemeas_process_meas_results */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_MEAS_CNF_MSG

DESCRIPTION
  This function processes the Meas confirmation message received from LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_meas_cnf_msg
( 
  hdrsrch_lte_meas_cnf_params_type         *meas_cnf_ptr
)

{
  errno_enum_type                          status = E_SUCCESS;
    /* Command completion status */

  uint8                                    i = 0;
    /* For for loop to store cell information from LTE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_process_meas_cnf_msg() is called" );

  if( hdrsrch_state_id != HDRSRCH_LTE_MEAS_IDLE_STATE ) 
  {
    /* If this message is received in the wrong state, report to IRATMan then
       drop the message */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_INVALID );
    return status;
   }

  if( hdrsrchltemeas_ctrl_info.meas_state == HDRSRCHLTEMEAS_ABORT_STATE )
  {
    /* DO already sent an Abort request, so just ignore this Search confirmation.
       DO needs to wait for the Abort confirmation which should be arriving after
       this confirmation. */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_ABORT );
    return status;
  }

  if( 
      ( meas_cnf_ptr->num_cells > LTE_CPHY_IMEAS_MAX_MEAS_CELLS ) ||
      ( meas_cnf_ptr->num_cells == 0 )
    )
    /* Sanity check num_cells returned from LTE.
       Note:  Cannot set num_cells to zero here because of the
       constant parameter type. */
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "D2L Number of meas cells=%d is invalid",
                    meas_cnf_ptr->num_cells );

    /* Report to IRATMan then drop the message */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_INVALID );
    status = E_FAILURE;
    return status;
  }

  memset( &hdrsrchltemeas_saved.lte_meas_cnf_params, 0,
          sizeof( hdrsrchltemeas_saved.lte_meas_cnf_params ) );
    /* Clear storage area */

  hdrsrchltemeas_saved.lte_meas_cnf_params.freq = 
    meas_cnf_ptr->freq;
  hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells =
    meas_cnf_ptr->num_cells;
    /* Save meas confirmation information */

  for( i = 0; i < hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells; i++ ) 
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    hdrsrchltemeas_saved.lte_meas_cnf_params.cells[ i ] = 
                                           meas_cnf_ptr->cells[ i ];
#else
    memcpy( &hdrsrchltemeas_saved.lte_meas_cnf_params.cells[ i ],
            &meas_cnf_ptr->cells[ i ],
            sizeof( lte_cphy_irat_meas_meas_results_s ) 
          );
#endif /* FEATURE_MEMCPY_REMOVAL */
      /* Save cell information */
  }

  hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_SUCCESS );
    /* Got measurements back from LTE, give them to IRATMan for processing */
    /* Found no cell, so immediately report meas results */

  /* Check whether LTE has enough energy for reselection and set the flag
     appropriately */
  if( hdrsrchltemeas_ctrl_info.iratman_resel_node.lte.valid == TRUE )
  {
    hdrsrchltemeas_ctrl_info.reselection_in_progress = TRUE;
    hdrsrchltemeas_ctrl_info.reselect_cell_id = 
      hdrsrchltemeas_ctrl_info.iratman_resel_node.lte.cell_id;
    hdrsrchltemeas_ctrl_info.reselect_freq_id = 
      hdrsrchltemeas_ctrl_info.iratman_resel_node.lte.earfcn;
  }

  /* Send Clean Up req message, resume after confirmation is received */
  hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_CLEANUP_REQ;
  hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_CLEANUP_STATE;
  hdrsrchmsg_d2l_send_cleanup_req_msg();

  return status;

} /* hdrsrchltemeas_process_meas_cnf_msg */

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
)

{
  errno_enum_type                              status = E_FAILURE;
    /* Command completion status */
  lte_ml1_irat_gap_schedule_return_e           lte_ret_val;
    /* Return value from scheduling function */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchltemeas_ctrl_info.gap_sched.gap_length_config_for_x2l_in_us = 0;
  hdrsrchltemeas_ctrl_info.gap_sched.move_to_next_freq = FALSE;
  lte_ret_val = lte_ml1_irat_setup_schedule (
#ifdef FEATURE_HDR_MSGR_VARIANT
     hdrsrch_get_subscriber_id(),
#endif /* FEATURE_HDR_MSGR_VARIANT */
     LTE_ML1_IRAT_GAP_SCHEDULE_HINT_SEARCH_AND_MEAS,
     timed_srch_meas_ptr,
     &hdrsrchltemeas_ctrl_info.gap_state,
     &hdrsrchltemeas_ctrl_info.gap_sched,
     timed_srch_meas_ptr->freq );

  if( lte_ret_val == LTE_ML1_IRAT_GAP_SUCCESS )
  {
    status = E_SUCCESS;
  }

  return status;
   
} /* hdrsrchltemeas_schedule_timed_search */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_TIMED_SRCH_MEAS_CNF_MSG

DESCRIPTION
  This function processes the Meas confirmation message received from LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_timed_srch_meas_cnf_msg
( 
  hdrsrch_lte_timed_srch_meas_cnf_params_type  *timed_srch_meas_cnf_ptr
)

{
  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

  uint8                                        i = 0;
    /* For for loop to store cell information from LTE */

  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr = NULL;
    /* Ptr to buffer where one LTE frequency parameters are stored */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug timed_srch_meas_cnf_msg() is called" );

#ifdef FEATURE_HDR_D2L_RF_MEAS
  hdrsrchltemeas_rfm_meas_exit();
    /* Allows LTE to set up Exit meas parameters */
#endif /* FEATURE_HDR_D2L_RF_MEAS */
  lte_ml1_irat_gap_srch_meas_cnf_handling
  ( 
#ifdef FEATURE_HDR_MSGR_VARIANT
     hdrsrch_get_subscriber_id(), 
#endif /* FEATURE_HDR_MSGR_VARIANT */
     timed_srch_meas_cnf_ptr, 
    &hdrsrchltemeas_ctrl_info.gap_state );
    /* LTE requires this interface call, it's not used by DO */

  if( hdrsrch_state_id != HDRSRCH_LTE_MEAS_IDLE_STATE ) 
  {
    /* If this message is received in the wrong state, report to IRATMan then
       drop the message */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_INVALID );
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                    "D2L LTE returned timed_srch_cnf when DO is in wrong state" );
    return status;
   }

  if( hdrsrchltemeas_ctrl_info.meas_state == HDRSRCHLTEMEAS_ABORT_STATE )
  {
    /* DO already sent an Abort request, so just ignore this Search confirmation.
       DO needs to wait for the Abort confirmation which should be arriving after
       this confirmation. */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_ABORT );
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                    "D2L LTE returned timed_srch_cnf when DO is in abort state" );
    return status;
  }

  if( timed_srch_meas_cnf_ptr->search.search_performed == FALSE ) 
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                    "D2L LTE returned timed_srch_cnf no search performed" );

    /* Report to IRATMan then drop the message */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_INVALID );
    status = E_FAILURE;
    return status;
  }

  if( timed_srch_meas_cnf_ptr->meas.num_cells > LTE_CPHY_IMEAS_MAX_MEAS_CELLS )

  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                    "D2L Number of detected cells=%d exceeds Max",
                    timed_srch_meas_cnf_ptr->meas.num_cells );

    /* Report to IRATMan then drop the message */
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_INVALID );
    status = E_FAILURE;
    return status;
  }

  memset( &hdrsrchltemeas_saved.lte_meas_cnf_params, 0,
          sizeof( hdrsrchltemeas_saved.lte_meas_cnf_params ) );
    /* Clear meas cnf storage area */

  hdrsrchltemeas_saved.lte_meas_cnf_params.freq = 
    timed_srch_meas_cnf_ptr->freq;
  hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells =
    timed_srch_meas_cnf_ptr->meas.num_cells;
    /* Save meas confirmation information */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                  "D2L LTE num cells searched %d, num cells measured %d, num cells saved by DO %d",
                  timed_srch_meas_cnf_ptr->search.num_detected_cells, 
                  timed_srch_meas_cnf_ptr->meas.num_cells,
                  hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells
                );

  if( timed_srch_meas_cnf_ptr->meas.num_cells == 0 )
  {
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_NO_CELL );
      /* Found no cell, so immediately report meas results */
  }
  else
  {
    /* Get here if LTE returns non zero number of cells */
    for( i = 0; i < hdrsrchltemeas_saved.lte_meas_cnf_params.num_cells; i++ ) 
    {
#ifdef FEATURE_MEMCPY_REMOVAL
      hdrsrchltemeas_saved.lte_meas_cnf_params.cells[ i ] = 
                                     timed_srch_meas_cnf_ptr->meas.cells[ i ];
#else
      memcpy( &hdrsrchltemeas_saved.lte_meas_cnf_params.cells[ i ],
              &timed_srch_meas_cnf_ptr->meas.cells[ i ],
              sizeof( lte_cphy_irat_meas_meas_results_s ) 
            );
#endif /* FEATURE_MEMCPY_REMOVAL */
        /* Save cell information */
    }

    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_SUCCESS );
      /* Got measurements back from LTE, give them to IRATMan for processing */
  }

  hdrsrchltemeas_end_lte_meas();
    /* Need to wrap up the previous measurement */
  
  /* Check whether LTE has enough energy for reselection and set the flag
     appropriately */
  if( hdrsrchltemeas_ctrl_info.iratman_resel_node.lte.valid == TRUE )
  {
    hdrsrchltemeas_ctrl_info.reselection_in_progress = TRUE;
    hdrsrchltemeas_ctrl_info.reselect_cell_id = 
      hdrsrchltemeas_ctrl_info.iratman_resel_node.lte.cell_id;
    hdrsrchltemeas_ctrl_info.reselect_freq_id = 
      hdrsrchltemeas_ctrl_info.iratman_resel_node.lte.earfcn;

    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */
  }
  else
  {
    freq_entry_ptr = hdrsrchltemeas_get_next_lte_freq_trm
          ( hdrsrchltemeas_ctrl_info.min_meas_ms + HDRSRCHLTEMEAS_MEAS_TIME_TRM_REQ_DELAY_MS );
            /* Get the next LTE frequency then request TRM for the next measurement.
               TRM may return a minimum value just a few sclks/ms less than Min duration for
               one LTE meas.  This is due to rounding error or elapsed time between the TRM
               call and when control is back to DO.  */

    if( freq_entry_ptr != NULL )
    {
#ifdef FEATURE_HDR_D2L_RF_MEAS
      status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr, TRUE );
        /* Begin the process to measure the next frequency from list.
           RF meas_enter was called in Sleep state, so skip the call for this first measurement. */
#else
      status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr );
        /* Begin the process to measure the next frequency from list */
#endif /* FEATURE_HDR_D2L_RF_MEAS */
      if( status == E_FAILURE )
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                      "D2L Start Up state failed to measure next freq" );
        hdrsrchltemeas_exit_lte_meas();
          /* Exit the LTE measurement state machine, back to sleep */
      }
    }
    else
    {
      hdrsrchltemeas_exit_lte_meas();
        /* Exit the LTE measurement state machine, back to sleep */
    }
  }

  return status;
   
} /* hdrsrchltemeas_process_timed_srch_meas_cnf_msg */


/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_CLEANUP_CNF_MSG

DESCRIPTION
  This function processes the Clean Up confirmation message received from LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_cleanup_cnf_msg( void )

{
  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_process_cleanup_cnf_msg() is called" );

  if( hdrsrchltemeas_ctrl_info.meas_state == HDRSRCHLTEMEAS_ABORT_STATE ) 
  {
    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */

    return status;
  }

  if( hdrsrchltemeas_ctrl_info.reselection_in_progress == TRUE ) 
  {
      hdrsrchltemeas_exit_lte_meas();
        /* Exit the LTE measurement state machine, back to sleep */

    return status;
  }

  freq_entry_ptr = hdrsrchltemeas_get_next_lte_freq_trm
    ( hdrsrchltemeas_ctrl_info.min_meas_ms + HDRSRCHLTEMEAS_MEAS_TIME_TRM_REQ_DELAY_MS );
          /* Get the next LTE frequency then request TRM for the next measurement.
             TRM may return a minimum value just a few sclks/ms less than Min duration for
             one LTE meas.  This is due to rounding error or elapsed time between the TRM
             call and when control is back to DO.  */

  if( freq_entry_ptr != NULL )
  {
#ifdef FEATURE_HDR_D2L_RF_MEAS
    status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr, TRUE );
      /* Begin the process to measure the next frequency from list.
         RF meas_enter was called in Sleep state, so skip the call for this first measurement. */
#else
    status = hdrsrchltemeas_meas_next_lte_freq( freq_entry_ptr );
      /* Begin the process to measure the next frequency from list */
#endif /* FEATURE_HDR_D2L_RF_MEAS */
    if( status == E_FAILURE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                    "D2L Start Up state failed to measure next freq" );
      hdrsrchltemeas_exit_lte_meas();
        /* Exit the LTE measurement state machine, back to sleep */
    }
  }
  else
  {
      hdrsrchltemeas_exit_lte_meas();
        /* Exit the LTE measurement state machine, back to sleep */
  }

  return status;
   
} /* hdrsrchltemeas_process_cleanup_cnf_msg */

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

boolean hdrsrchltemeas_send_deinit_req( void )

{

  boolean                                      request_sent = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_send_deinit_req() is called,state=0x%x", 
    hdrsrch_state_id );

  if( hdrsrchltemeas_ctrl_info.init_cnf_rxed == HDRSRCHLTEMEAS_LTEINIT_RXED )
    /* If DO has received confirmation of LTE Init request */
  {

    if( hdrsrch_state_id == HDRSRCH_INACTIVE_STATE ) 
      /* If called from Inactive state, turn on MCPM clocks for LTE */
    {

      HDR_MSG_SRCH( MSG_LEGACY_MED, "D2L hdrsrchmcpm_do2lte_measurement( TRUE )" );
      hdrsrchmcpm_do2lte_measurement( TRUE );
        /* DO is not in Meas state, turn on MCPM clocks for LTE before sending
           Deinit request. */
    }

    /* Send Deinit req message. */
    hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_DEINIT_REQ;

    if( hdrsrchltemeas_ctrl_info.meas_state != HDRSRCHLTEMEAS_ABORT_STATE ) 
    {
      hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_DEINIT_STATE;
        /* Only switch meas state if we are not aborting.  Otherwise, leave
           the  abort state as is for the processing of the deinit confirmation
           message */
    }

    hdrsrchmsg_d2l_send_deinit_req_msg();
    hdrsrchltemeas_ctrl_info.init_cnf_rxed = HDRSRCHLTEMEAS_LTEDEINIT_SENT;
    request_sent = TRUE;
  }

  return request_sent;

} /* hdrsrchltemeas_send_deinit_req */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_ABORT_CNF_MSG

DESCRIPTION
  This function processes the Abort confirmation message received from LTE.
  
DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS command is successful
  E_FAILURE encounter error, return without executing command

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_process_abort_cnf_msg( void )

{
  errno_enum_type                  status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, 
                "D2L Debug hdrsrchltemeas_process_abort_cnf_msg() is called" );

  hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_ABORT );
    /* If abort happens before TIMED_SRCH request is sent, report to IRATMan to
       unlock the node. */

  hdrsrchltemeas_end_lte_meas();
    /* Need to wrap up the previous measurement */

  hdrsrchltemeas_exit_lte_meas();
    /* Exit the LTE measurement state machine, back to sleep */

  return status;

} /* hdrsrchltemeas_process_abort_cnf_msg */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_ABORT_LTE_MEAS

DESCRIPTION
  This function processes commands received from DO LMAC layer which required
  aborting LTE pilot measurement.
  Upon receipt of these commands, DO should abort the current LTE measurement
  and return to Sleep state.  These commands are deferred to be processed
  in Sleep state.
  Currently, 3 commands cause LTE measurement to be aborted:
    HDRSRCH_ACCESS_CMD
    HDRSRCH_DEACTIVATE_CMD      
    HDRSRCH_ADVISE_UNLOCK_RF_CMD
  
DEPENDENCIES
  None

RETURN VALUE
  E_AGAIN

SIDE EFFECTS 
  None 

===========================================================================*/

errno_enum_type hdrsrchltemeas_abort_lte_meas( void )

{
  errno_enum_type                  status = E_AGAIN;
    /* Command completion status, set to E_AGAIN to be processed in Sleep state */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( 
    MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_abort_lte_meas() is called,state=%d",
    hdrsrchltemeas_ctrl_info.meas_state );

  switch( hdrsrchltemeas_ctrl_info.meas_state )
  {
    case HDRSRCHLTEMEAS_SEARCH_STATE:
    case HDRSRCHLTEMEAS_MEAS_STATE:
    case HDRSRCHLTEMEAS_TIMED_SRCH_MEAS_STATE:

      /* DO is waiting for confirmation.  Since these operations can be aborted
         on LTE side, send Abort req message, resume after confirmation
         is received */
      hdrsrchltemeas_ctrl_info.last_sent = HDRSRCHLTEMEAS_ABORT_REQ;
      hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_ABORT_STATE;
        /* Treat Abort as a special case, remember that DO is trying to abort
           LTE measurements */

      hdrsrchmsg_d2l_send_abort_req_msg();

      break;

    case HDRSRCHLTEMEAS_INACTIVE_STATE:

      hdrsrchltemeas_exit_lte_meas();
        /* Exit the LTE measurement state machine, back to sleep */
      break;

    case HDRSRCHLTEMEAS_CLEANUP_STATE:
    case HDRSRCHLTEMEAS_ABORT_STATE:
    case HDRSRCHLTEMEAS_INIT_STATE:

      hdrsrchltemeas_ctrl_info.meas_state = HDRSRCHLTEMEAS_ABORT_STATE;
        /* Remember that DO is trying to abort LTE measurements but wait
           until the confirmation for these requests come back */
      HDR_MSG_SRCH( MSG_LEGACY_MED, 
                    "D2L DO waits for LTE confirmation in Abort state" );
      break;

    case HDRSRCHLTEMEAS_DEINIT_STATE:
      /* DO is already cleaning up LTE measurements, do not need to do
         anything here.  When the deinit confirmation comes back, DO will
         go back to sleep.*/
      HDR_MSG_SRCH( MSG_LEGACY_MED, 
                    "D2L DO waits for LTE deinit confirmation" );
      break;

  } /* end switch */

  hdrsrchltemeas_clear_reselect_status();
    /* Clear reselect status to make sure DO doesn't reselect to LTE in abort
       cases */

  if( ( hdrsrchltemeas_ctrl_info.meas_state == HDRSRCHLTEMEAS_ABORT_STATE ) &&
      ( hdrsrchltemeas_ctrl_info.last_sent == HDRSRCHLTEMEAS_INIT_REQ ) )
  {
    hdrsrchltemeas_ctrl_info.is_rfm_meas_entered = FALSE;
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

} /* hdrsrchltemeas_abort_lte_meas */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_CMD

DESCRIPTION
  This function processes HDR internal commands in LTE measurement state.
  Messages from LTE via the message router have been converted to HDR internal
  commands and will be processed here.

DEPENDENCIES
  None

RETURN VALUE
  E_SUCCESS       - command has been successfully executed
  E_FAILURE       - command has failed
  E_NOT_SUPPORTED - command is not support
  E_IN_PROGRESS   - command has been started, but has not completed
  E_AGAIN         - command cannot be started in present state, but is a
                    valid command.  Execute hdrsrch_retry_cmd( ) at a
                    later point in time to reattempt the command.
                    [hdrsrch_retry_cmd( ) is called at every state change]

SIDE EFFECTS
  If E_IN_PROGRESS or E_AGAIN is returned, command processing is disabled
  until the command buffer is passed to hdrsrch_complete_cmd( ).

===========================================================================*/

errno_enum_type hdrsrchltemeas_cmd
(
  const hdrsrch_cmd_type           *cmd
    /* Internal HDR command to be processed. */
)

{
  errno_enum_type                  status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "D2L Debug hdrsrchltemeas_cmd() is called,cmd=0x%x",
                  cmd->name );

  switch( cmd->name )
  {
    case HDRSRCH_ACCESS_CMD:
    case HDRSRCH_DEACTIVATE_CMD:
    case HDRSRCH_ADVISE_UNLOCK_RF_CMD:
      /* All these cases require DO to abort LTE measurements.  Status
         E_AGAIN is returned and these commands will be processed in
         sleep state, as the LTE Meas state is a sub-state of sleep. */
      status = hdrsrchltemeas_abort_lte_meas();

      break;

    default:
      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */
      break;

  } /* switch (cmd->name) */

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                    "D2L Failed to execute cmd %d, back to sleep",
                    cmd->name );
    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */
  }

  hdrsrchltemeas_ctrl_info.last_cmd = cmd->name;
    /* Remember the last command for debugging */

  return status;

} /* hdrsrchltemeas_cmd */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TRESELECT_TIMER_UPDATE

DESCRIPTION
  This function manages the Treselect timers of all LTE frequencies.
  It double checks for any expired timers which may have expired without being
  programmed via the HW timer.  This covers the case where an additional
  timer expires between the time Time Services module calls back to notify DO
  of the expiry, and when this function is called which is after DO has been awake
  and is now ready to manage the timers.  Once the expired timers are taken care
  of, this function checks for and programs the next Treselect timer if there are
  outstanding active timers.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_treselect_timer_update( void )
{

  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */

  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  curr_time_sclks = timetick_get();
    /* Get current time in sclk units */

  /* Look for frequencies that have timer which may have expired without being
     armed with a HW timer */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr;

  while( freq_entry_ptr != NULL )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
    {
      /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
       * Note: This only works for large unsigned integers where the delta
       * between time 1 < time 2 is smaller than half of the
       * number set.  These 2 time values are uint32 values. */
      if( TIME_OCCURS_FIRST( freq_entry_ptr->treselect_exp_sclks, 
                             curr_time_sclks ) )
      {
        freq_entry_ptr->treselect_state = HDRSRCHLTEMEAS_TIMER_EXPIRED;

        hdrsrchltemeas_ctrl_info.reselect_timer_cnt--;
          /* Account for 1 less Treselect timer */
      }
    }
      
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  /* Continue the search in the less preferred list */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr;

  while( freq_entry_ptr != NULL )
  { 
    if( freq_entry_ptr->treselect_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
    {
      /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
       * Note: This only works for large unsigned integers where the delta
       * between time 1 < time 2 is smaller than half of the
       * number set.  These 2 time values are uint32 values. */
      if( TIME_OCCURS_FIRST( freq_entry_ptr->treselect_exp_sclks, 
                             curr_time_sclks ) )
      {
        freq_entry_ptr->treselect_state = HDRSRCHLTEMEAS_TIMER_EXPIRED;

        hdrsrchltemeas_ctrl_info.reselect_timer_cnt--;
          /* Account for 1 less Treselect timer */
      }
    }
      
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  /* Expired Treselect timers have been taken care of above.  Now look for
     additional active Treselect timers. */
  if( hdrsrchltemeas_ctrl_info.reselect_timer_cnt > 0 )
  {
    hdrsrchltemeas_ctrl_info.treselect_freq_ptr = 
      hdrsrchltemeas_get_active_treselect_min( curr_time_sclks );
      /* If there are also other timers active, need to find the earliest timer
       to arm, using the system time.  This way, one HW timer can be used
       for multiple Treselect timers by setting up another one every time one
       expires. */
  
    if( hdrsrchltemeas_ctrl_info.treselect_freq_ptr != NULL )
    {
      if( hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks == 0 )
      {
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Treselect sclk cnt = 0, do not arm timer" );

        hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_state = 
          HDRSRCHLTEMEAS_TIMER_EXPIRED;
          /* Since sclk count is 0, indicate that Treselect timer has expired
             without arming HW timer.  The expired timer will be processed next wake up
             time.  This should happen in extremely rare cases where 2 treselect timers
             expires very close to one another. */

        hdrsrchltemeas_ctrl_info.treselect_freq_ptr->tbackoff_state = 
          HDRSRCHLTEMEAS_TIMER_INACTIVE;
          /* Make sure Tbackoff timer is reset */
      }
      else if( hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks > 
          ( TIMETICK_SCLK_FROM_MS( hdrsrchltemeas_ctrl_info.max_reselect_timer_ms ) ) )
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                        "D2L Treselect timer %d exceeds max, reset to MAX value",
                        hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks );

        hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks =
            ( TIMETICK_SCLK_FROM_MS( hdrsrchltemeas_ctrl_info.max_reselect_timer_ms ) );
          /* Treselect timer exceeds max.  Something is wrong, report error and cap
             the reselect timer at max value */
      }
      else 
      {
        timer_set( &hdrsrchltemeas_ctrl_info.reselect_timer, 
                   hdrsrchltemeas_ctrl_info.treselect_freq_ptr->treselect_sclks,
                   0,
                   T_SCLK );
        /* Set timer for the shortest duration in sclk units */
      }
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "D2L Unexpected Treselect timer count = %d",
                      hdrsrchltemeas_ctrl_info.reselect_timer_cnt );
    } /* end if( hdrsrchltemeas_ctrl_info.treselect_freq_ptr != NULL ) */

  } /* end if( hdrsrchltemeas_ctrl_info.reselect_timer_cnt > 0 ) */

} /* hdrsrchltemeas_treselect_timer_update */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_TBACKOFF_TIMER_UPDATE

DESCRIPTION
  This function manages the Tbackoff timers of all LTE frequencies.  These
  timers have to be updated so the number of LTE frequencies to be measured is
  valid.  If a timer has expired, clear the backoff state so it can be measured
  again.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_tbackoff_timer_update( void )
{

  timetick_type                              curr_time_sclks;
    /* Current time in sclks units */

  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr;
    /* Ptr to buffer where one LTE frequency parameters are stored */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  curr_time_sclks = timetick_get();
    /* Get current time in sclk units */

  /* Look for frequencies that have timer which may have expired.  Tbackoff
     timers are not serviced immediately upon expiry.  They are serviced the
     very next time DO enters LTE measurement state after timer expiry.  To
     simplify the timer processing, Tbackoff timers are not armed using a
     HW timer. */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.more_preferred.head_ptr;

  while( freq_entry_ptr != NULL )
  { 
    if( freq_entry_ptr->tbackoff_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
    {
     /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
      * Note: This only works for large unsigned integers where the delta
      * between time 1 < time 2 is smaller than half of the
      * number set.  These 2 time values are uint32 values. */
     if( TIME_OCCURS_FIRST( freq_entry_ptr->tbackoff_exp_sclks, curr_time_sclks ) )
      {
        freq_entry_ptr->tbackoff_state = HDRSRCHLTEMEAS_TIMER_INACTIVE;
          /* Tbackoff timer has expired and now the frequency can be measured
             again */
      }
    }
      
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

  /* Continue the search in the less preferred list */
  freq_entry_ptr = hdrsrchltemeas_all_freq_lists.less_preferred.head_ptr;

  while( freq_entry_ptr != NULL )
  { 
    if( freq_entry_ptr->tbackoff_state == HDRSRCHLTEMEAS_TIMER_ACTIVE ) 
    {
     /* TIME_OCCURS_FIRST macro returns TRUE if time 1 < time 2.
      * Note: This only works for large unsigned integers where the delta
      * between time 1 < time 2 is smaller than half of the
      * number set.  These 2 time values are uint32 values. */
     if( TIME_OCCURS_FIRST( freq_entry_ptr->tbackoff_exp_sclks, 
                            curr_time_sclks ) )
      {
        freq_entry_ptr->tbackoff_state = HDRSRCHLTEMEAS_TIMER_INACTIVE;
      }
    }
      
    freq_entry_ptr = freq_entry_ptr->next_ptr;
  }

} /* hdrsrchltemeas_tbackoff_timer_update */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_STATE_VISIT_REQUIRED

DESCRIPTION
  This function is called to determine if DO should do LTE Pilot measurements
  during the start of this sleep cycle.  It calls IRATMan function to get the
  projected time for LTE Meas.  This time may be shortened depending on RF
  availability.
 
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
    /* Sleep time or maximum time DO can allow for LTE meas before its
       next wake up */
)

{
  int16                                        ec_io_lin = 0;   
    /* HDR filtered signal strength in lin unit */

  errno_enum_type                              status = E_SUCCESS;
    /* Command completion status */

#ifdef FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF
  iratman_cb_struct                            cb_struct;
    /* Struct containing IRATMan callback functions */
#endif /* FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF */

  boolean                                      ret_val = FALSE;
     
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Move IRATMan init function to here from hdrsrchltemeas_powerup_init().
     The power up init function used to call iratman_initialization() when the
     HDR SRCH task starts.  The JCDMA NV read below is performed when HDR MC
     task starts, which is after HDR SRCH task so it does not reflect the
     correct JCDMA value.
   
     To keep the change local to HDR SRCH only, it's simplest to
     initialize IRATMan the first time we enter HDR LTE measurement
     state to make sure the JCDMA NV read has been done. */

  if( hdrsrchltemeas_ctrl_info.iratman_init_done == FALSE )
  {

#ifdef FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF
    cb_struct.wakeup_cb        = hdrsrchltemeas_iratman_wakeup_cb;
    cb_struct.get_cell_info_cb = hdrsrchltemeas_iratman_get_sector_cb;

    iratman_initialization 
      ( 
        RAT_HDR, 
        cb_struct, 
        hdrmc_feature_is_enabled ( HDRMC_FEATURE_HDR_JCDMA_NV )
          /* Let IRATMan know if this is a KDDI phone */ 
      );
      /* Initialize IRAT Manager for D2L LTE frequency measurements */
#else
    iratman_init();
  
    /* Register wakeup callback with IRATMan */
    iratman_register_wakeup_cb
      ( 
        RAT_HDR, 
        RAT_LTE, 
        hdrsrchltemeas_iratman_wakeup_cb 
    );
#endif /* FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF */
  
    hdrsrchltemeas_ctrl_info.iratman_init_done = TRUE;

  }

  status = hdrsrchltemeas_check_to_update_lte_neighbor_list();
    /* Check to see if DO should update LTE neighbor list */

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( 
      MSG_LEGACY_ERROR,  
      "D2L Failed to build new frequency lists, go back to sleep" );
    return ret_val;
  }

  hdrsrchutil_get_ecio_and_io( &ec_io_lin, NULL );
    /* Find the maximum ecio among all active set sectors (filtered value) */

  hdrsrchltemeas_ctrl_info.iratman_meas_req.lte.home_rat_ecio = 
    ( uint8 )( hdrsrchset_eng_to_ecio( ec_io_lin ) );
    /* Convert to -.5dB units using lookup table ps2eng_table */

  hdrsrchltemeas_ctrl_info.orig_sleep_ms = sleep_ms;
    /* Save the original sleep duration before TRM request for D2L extension */

  hdrsrchltemeas_ctrl_info.avail_sleep_ms = sleep_ms;
    /* Save the available sleep duration before TRM request for D2L extension */

  hdrsrchltemeas_ctrl_info.min_meas_ms = iratman_get_min_meas_dur_ms ( RAT_HDR, RAT_LTE );
    /* Get min duration of 1 LTE measurement from IRATMan */

  memset( &hdrsrchltemeas_ctrl_info.iratman_meas_node,
          0,
          sizeof( hdrsrchltemeas_ctrl_info.iratman_meas_node ) );
    /* Initialize data used for IRATMan API */

    ret_val = hdrsrchltemeas_state_visit_request_trm();
    /* Request TRM for D2L meas */

  return ret_val;

} /* hdrsrchltemeas_state_visit_required */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_END_LTE_MEAS 
 
DESCRIPTION
  Need to end an IRAT EARFCN meas session with TRM END_STATE before starting
  a new measurement.
 
DEPENDENCIES
  None
 
PARAMETERS
  None

RETURN VALUE
  Pointer to LTE frequency just measured

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_end_lte_meas( void ) 

{
  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr = NULL;
    /* The next LTE frequency to measure */

  static trm_set_client_state_input_type     trm_set_client_state_input;
  static trm_set_client_state_output_data    trm_set_client_state_output;
    /* Parameters for TRM set_client_state API */
     
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( hdrsrchltemeas_curr_freq_ptr_arr.cnt > 0 )
  {
    freq_entry_ptr = &hdrsrchltemeas_freq_arr[ 0 ]; 
      /* Entry 0 is the LTE frequency just measured */
  }

  /* Error checking */
  if( freq_entry_ptr == NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "D2L Cannot locate EARFCN just measured" );
    return;
  }

  /* Avoid calling TRM with TRM_END_STATE multiple times in abort scenarios */
  if ( freq_entry_ptr->earfcn_to_meas.trm_set_client_state_input.data.irat_data.operation == TRM_END_STATE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "D2L Already sent TRM_END_STATE" );
    return;
  }

  memset( &trm_set_client_state_input,0,sizeof ( trm_set_client_state_input ) );
  memset( &trm_set_client_state_output,0,sizeof ( trm_set_client_state_output ) );

  trm_set_client_state_input = freq_entry_ptr->earfcn_to_meas.trm_set_client_state_input;
    /* Copy over what was saved for TRM set_client_state API */
  
  trm_set_client_state_input.data.irat_data.operation = TRM_END_STATE;
  trm_set_client_state_input.data.irat_data.transaction_id = 
    freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output.transaction_id;
    /* Override operation with END_STATE and the transaction ID provided by TRM from the
       transaction start request */

#ifndef FEATURE_HDR_RUMI
  trm_set_client_state( &trm_set_client_state_input, &trm_set_client_state_output );
    /* Call trm_set_client_state */
#endif /* FEATURE_HDR_RUMI */

  freq_entry_ptr->earfcn_to_meas.trm_set_client_state_input = trm_set_client_state_input;
  freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output = trm_set_client_state_output;
    /* Save parameters for TRM set_client_state API */
  
  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "D2L TRM END_STATE for LTE band=%d/earfcn=%d,output.grant=%d",
                    freq_entry_ptr->earfcn_to_meas.earfcn_trm_info.band,
                    freq_entry_ptr->earfcn_to_meas.earfcn,
                    trm_set_client_state_output.grant
                );

} /* hdrsrchltemeas_end_lte_meas */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_NEXT_LTE_FREQ_TRM

DESCRIPTION
  The TRM request sequence is shown below:
  1.  Get Min duration from IRATMan for 1 LTE frequency.
  2.  Get LTE band associated with the ORAT EARFCN.
  3.  Call TRM set_client_state() API to request TRM extension

DEPENDENCIES
  None
 
PARAMETERS
  None

RETURN VALUE 
  Pointer to next LTE frequency

SIDE EFFECTS
  None

===========================================================================*/

hdrsrchltemeas_freq_list_entry_struct_type   *hdrsrchltemeas_get_next_lte_freq_trm 
(
  uint32                                     duration_ms
    /* TRM request duration */
)

{
  hdrsrchltemeas_freq_list_entry_struct_type *freq_entry_ptr = NULL;
    /* The next LTE frequency to measure */

  static trm_set_client_state_input_type     trm_set_client_state_input;
  static trm_set_client_state_output_data    trm_set_client_state_output;
    /* Parameters for TRM set_client_state API */
     
  sys_sband_lte_e_type                       lte_band;
    /* LTE band obtained from LTE */
     
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  freq_entry_ptr = hdrsrchltemeas_get_next_lte_freq();
    /* Get the frequency information from current frequency in list */

  if( freq_entry_ptr == NULL )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "D2L No LTE freq returned when querried,Sleep(ms)=%d",
                    hdrsrchltemeas_ctrl_info.avail_sleep_ms );
    return NULL;
  }

  lte_band = 
	  lte_rrc_get_band_from_dl_earfcn ( 
#ifdef FEATURE_HDR_MSGR_VARIANT
               hdrsrch_get_subscriber_id(), 
#endif  /* FEATURE_HDR_MSGR_VARIANT */
               freq_entry_ptr->earfcn_to_meas.earfcn, 
                                      &freq_entry_ptr->earfcn_to_meas.lte_band_is_valid);
      /* This LTE API provides LTE band for the given earfcn */

  if ( freq_entry_ptr->earfcn_to_meas.lte_band_is_valid == FALSE )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "D2L LTE BAND is invalid,band=%d",
                    lte_band );
    return NULL;
  }

  freq_entry_ptr->earfcn_to_meas.earfcn_trm_info.band = 
    lte_ml1_convert_lte_band_to_sys_band ( lte_band );
    /* Convert from LTE band to TRM band */

  memset( &trm_set_client_state_input,0,sizeof ( trm_set_client_state_input ) );
  memset( &trm_set_client_state_output,0,sizeof ( trm_set_client_state_output ) );

  trm_set_client_state_input.client_id = TRM_HDR;
  trm_set_client_state_input.client_state = TRM_IRAT_STATE;
  trm_set_client_state_input.data.irat_data.operation = TRM_START_STATE;
  trm_set_client_state_input.data.irat_data.should_change_reason_to_irat = TRUE;
  trm_set_client_state_input.data.irat_data.measured_client = TRM_LTE;
  trm_set_client_state_input.data.irat_data.freq_info.num_bands = 1;
  trm_set_client_state_input.data.irat_data.freq_info.bands[ 0 ].band = 
    freq_entry_ptr->earfcn_to_meas.earfcn_trm_info.band;
  trm_set_client_state_input.data.irat_data.transaction_id = 0;
  trm_set_client_state_input.data.irat_data.is_diversity_needed = TRUE;

  trm_set_client_state_input.data.irat_data.duration = 
    TIMETICK_SCLK_FROM_MS( duration_ms );
  trm_set_client_state_input.data.irat_data.sync_result = TRUE;

#ifdef FEATURE_HDR_RUMI
  trm_set_client_state_output.grant = TRM_GRANTED;
  trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].primary_device = 0;
  trm_set_client_state_output.result.irat_result.band_dev_info[ 0 ].diversity_device = 1;
  /* Frequency to measure and device ID - LTE always use primary and diversity devices */
#else  
  trm_set_client_state( &trm_set_client_state_input, &trm_set_client_state_output );
    /* Call trm_set_client_state. We should get immediate grant or denial. */
#endif /* FEATURE_HDR_RUMI */

  freq_entry_ptr->earfcn_to_meas.trm_set_client_state_input = trm_set_client_state_input;
  freq_entry_ptr->earfcn_to_meas.trm_set_client_state_output = trm_set_client_state_output;
    /* Save parameters for TRM set_client_state API */
  
  HDR_MSG_SRCH_6( MSG_LEGACY_HIGH, "D2L TRM START_STATE grant=%d LTE band=%d/earfcn=%d Avail_ms=%d,dur_ms=%d,dur_sclks=%d",
                    trm_set_client_state_output.grant,
                    freq_entry_ptr->earfcn_to_meas.earfcn_trm_info.band,
                    freq_entry_ptr->earfcn_to_meas.earfcn,
                    hdrsrchltemeas_ctrl_info.avail_sleep_ms,
                    duration_ms,
                    TIMETICK_SCLK_FROM_MS( duration_ms )
                   );

  if ( trm_set_client_state_output.grant != TRM_GRANTED )
  {
    hdrsrchltemeas_iratman_report_meas_results( HDRSRCHLTEMEAS_MEAS_ABORT );
      /* Report to IRATMan to unlock the node. */

    return NULL;
  }

  return freq_entry_ptr;

} /* hdrsrchltemeas_get_next_lte_freq_trm */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_STATE_VISIT_REQUEST_TRM

DESCRIPTION
  The TRM request sequence is shown below:
  1.  Get Min duration from IRATMan for 1 LTE frequency.
  2.  Get LTE band associated with the ORAT EARFCN.
  3.  Call TRM set_client_state() API to request TRM extension

DEPENDENCIES
  None
 
PARAMETERS
  None

RETURN VALUE
  TRUE:  TRM request was successful
  FALSE:  TRM request was denied

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchltemeas_state_visit_request_trm ( void )

{
  uint32                                       min_meas_ms = 0;
    /* Length of time required to measure 1 LTE frequency */ 

  uint32                                       max_meas_ms = 0;
    /* Length of time required to measure LTE frequencies in list
       within DO sleep duration */

  hdrsrchltemeas_freq_list_entry_struct_type   *freq_entry_ptr = NULL;
    /* The next LTE frequency to measure */

  boolean                                      ret_val = FALSE;
    /* Return value */
     
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  min_meas_ms = hdrsrchltemeas_ctrl_info.min_meas_ms + 
                HDRSRCHLTEMEAS_MEAS_TIME_DELAY_MS;
    /* Allow for delay between TRM reservation and getting a frequency from IRATMan.
       RF delay is only applicable to the first EARFCN in the measurement cycle as we only
       prep RF for LTE meas once in the measurement cycle. */

  if ( min_meas_ms > hdrsrchltemeas_ctrl_info.orig_sleep_ms )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "D2L Not enough time to measure 1 LTE frequency,Sleep(ms)=%d,min(ms)=%d",
                    hdrsrchltemeas_ctrl_info.orig_sleep_ms, min_meas_ms );
    return ret_val;
  }

  max_meas_ms = 
    iratman_meas_pending
      (
        RAT_HDR,  
        RAT_LTE,  
        hdrsrchltemeas_ctrl_info.orig_sleep_ms,
        &hdrsrchltemeas_ctrl_info.iratman_meas_req 
      );

  if ( max_meas_ms == 0 )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "D2L No LTE freq,Sleep(ms)=%d",
                    hdrsrchltemeas_ctrl_info.avail_sleep_ms );
    return ret_val;
  }

  freq_entry_ptr = hdrsrchltemeas_get_next_lte_freq_trm( min_meas_ms );
    /* Get the next LTE freq and TRM extension.  TRM is granted per LTE meas so
       always use minimum meas per frequency for the grant request. */

  if( freq_entry_ptr == NULL )
  {
    return ret_val;
  }

    ret_val = TRUE;
  return ret_val;

} /* hdrsrchltemeas_state_visit_request_trm */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_GET_RESELECT_STATUS

DESCRIPTION 
  This function returns the LTE reselection status.

DEPENDENCIES 
  None 

PARAMETERS 
  None

RETURN VALUE 
  TRUE:   LTE reselection is in progress
  FALSE:  No LTE reselection

SIDE EFFECTS 
  None 

=========================================================================*/

boolean hdrsrchltemeas_get_reselect_status( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( 
    MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_get_reselect_status() returns %d",
    hdrsrchltemeas_ctrl_info.reselection_in_progress );

  return hdrsrchltemeas_ctrl_info.reselection_in_progress;

} /* hdrsrchltemeas_get_reselect_status */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_CLEAR_RESELECT_STATUS

DESCRIPTION 
  This function clears the LTE reselect status.  It is called whenever DO
  L1 receives the ACQUIRE command.  Since DO is being re-activated, LTE
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

void hdrsrchltemeas_clear_reselect_status( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchltemeas_ctrl_info.reselection_in_progress = FALSE;

} /* hdrsrchltemeas_clear_reselect_status */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  This function processes messages from LTE via the message router.
  These messages are confirmation messages coming from LTE
  for DO->LTE reselection.  They need to be in a separate queue from
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

RETURN VALUE 
  None 

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchltemeas_process_low_pri_rsp_ind
(
  hdrsrchmsg_s_type                            *msg
    /* LTE confirmation messages to be processed. */
)

{

  errno_enum_type                  status = E_SUCCESS;
    /* Command completion status */

#ifdef FEATURE_HDR_MSGR_VARIANT
  sys_modem_as_id_e_type variant;
    /* Indicates which subscription should receive the message */
#endif	/* FEATURE_HDR_MSGR_VARIANT */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( 
    MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_process_low_pri_rsp_ind(),msg %s,umid 0x%8x",
    hdrsrchmsg_get_msg_name( msg->msg.hdr.id ),
    msg->msg.hdr.id );

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
      status = 
        hdrsrchltemeas_process_init_cnf_msg( &msg->msg.lte_cnf_msg.lte_init_cnf );
      break;                              

    case HDRSRCH_LTE_IRAT_MEAS_D2L_DEINIT_CNF: 
      /* Receive LTE Deinit confirmation message via message router */
      status = 
        hdrsrchltemeas_process_deinit_cnf_msg();
      break;

    case HDRSRCH_LTE_IRAT_MEAS_D2L_SEARCH_CNF: 
      /* Receive LTE Search confirmation message via message router */
      status = 
        hdrsrchltemeas_process_search_cnf_msg( &msg->msg.lte_cnf_msg.lte_search_cnf );
      break;

    case HDRSRCH_LTE_IRAT_MEAS_D2L_MEAS_CNF:   
      /* Receive LTE Measurement confirmation message via message router */
      status = 
        hdrsrchltemeas_process_meas_cnf_msg( &msg->msg.lte_cnf_msg.lte_meas_cnf );
      break;

    case HDRSRCH_LTE_IRAT_MEAS_D2L_CLEANUP_CNF:
      /* Receive LTE Clean Up confirmation message via message router */
      status = 
        hdrsrchltemeas_process_cleanup_cnf_msg();
      break;

    case HDRSRCH_LTE_IRAT_MEAS_D2L_ABORT_CNF:  
      /* Receive LTE Abort confirmation message via message router */
      status = 
        hdrsrchltemeas_process_abort_cnf_msg();
      break;

    case HDRSRCH_LTE_IRAT_MEAS_D2L_TIMED_SRCH_MEAS_CNF:  
       /* Receive LTE Timed Search Meas confirmation message via message router */
       status = 
         hdrsrchltemeas_process_timed_srch_meas_cnf_msg( &msg->msg.lte_cnf_msg.lte_timed_srch_meas_cnf );
       break;


    default:
      HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, 
                     "D2L Low pri Q msg %x ignored", msg->msg.hdr.id);
      break;

  } /* switch ( msg->msg.hdr.id ) */

  if( status == E_FAILURE )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                  "D2L Failed to execute LTE cnf msg, back to sleep" );

    hdrsrchltemeas_exit_lte_meas();
      /* Exit the LTE measurement state machine, back to sleep */
  }
  else
  {
    hdrsrchltemeas_ctrl_info.last_cnf = msg->msg.hdr.id;
      /* Remember the last confirmation message for debugging */
  }

} /* hdrsrchltemeas_process_low_pri_rsp_ind */

/*=========================================================================

FUNCTION HDRSRCHLTEMEAS_LTE_DEINIT_IS_FINISHED

DESCRIPTION 
  This function returns the status of the LTE Deinit operation.  The Deinit
  operation is simple to initiate on DO side but is time consuming on LTE side.
 
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

boolean hdrsrchltemeas_lte_deinit_is_finished( void )
{

  boolean                                    result = FALSE;
    /* Status of LTE deinit operation  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrsrchltemeas_ctrl_info.init_cnf_rxed == HDRSRCHLTEMEAS_LTEINIT_NONE )
  {
    result = TRUE;
  }

  HDR_MSG_SRCH_1( 
    MSG_LEGACY_MED, 
    "D2L hdrsrchltemeas_lte_deinit_is_finished() returns %d", result );

  return result;

} /* hdrsrchltemeas_lte_deinit_is_finished */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHLTEMEAS_PRI_CHAIN_EVENT

DESCRIPTION
  Handles unlock events due to TRM unlock callback.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchltemeas_pri_chain_event( void )
{
  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* ACQ state must already own RF lock here.  Handle unlock events. */

    hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
      /* Retrieve the retain lock state. */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "HDR LRE: Pri Chain Event: rf_unlock_event() event: %d", 
                    rl_state.event  );

    /* Must already own RF.  Check unlock events. */
    if ( ( rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY ) ||
         ( rl_state.event == HDRSRCHRF_UNLOCK_REQUIRED ) )
    {
      /* Unlock now, e.g., for 1x rude wakeup */
   
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                    "HDR LRE: Pri Chain Event: ACQ unlock immediately requested" );
   
      hdrsrch_advise_unlock_rf();
      /* Since LTE might be performing measurements or reselection, send LTEMEAS state
         an advise unlock RF cmd and go back to sleep state to release the chain.  */
    }
    else
    {

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                      "HDR LRE: Pri Chain Event: Process unlock event: %d ignored",
                      rl_state.event );
    }


} /* hdrsrchltemeas_pri_chain_event( ) */

/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_UPDATE_SECTOR

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

boolean hdrsrchltemeas_update_sector 
(
  hdraddr_type        hdr_sector_ptr
)
{
  boolean                                      ret_val = TRUE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdr_sector_ptr == NULL )
  {
    ret_val = FALSE;
  }
  else
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy( hdrsrchltemeas_ctrl_info.sector_id, sizeof ( hdrsrchltemeas_ctrl_info.sector_id ), 
             hdr_sector_ptr, sizeof ( hdrsrchltemeas_ctrl_info.sector_id ) );
#else
    memcpy( hdrsrchltemeas_ctrl_info.sector_id, hdr_sector_ptr,
            sizeof ( hdrsrchltemeas_ctrl_info.sector_id ) );
      /* Store sector ID (dest, source) */
#endif /* FEATURE_MEMCPY_REMOVAL */
  }

  return ret_val;

} /* hdrsrchltemeas_update_sector */

#ifdef FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF
/*===========================================================================

FUNCTION HDRSRCHLTEMEAS_IRATMAN_GET_SECTOR_CB

DESCRIPTION 
  This function provides the HDR sector ID to the caller function.

DEPENDENCIES
  Must be called when HDR is still in LTE meas state to avoid race condition
  in case sector ID is being changed.

PARAMETERS
  Pointer to memory for HDR to write the sector ID

RETURN VALUE
  TRUE:  Operation completes successfully
  FALSE:  Unexpected error, operation fails

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchltemeas_iratman_get_sector_cb
(
  iratman_cell_info_type *hdr_sector_info_ptr
)
{

  boolean                                      ret_val = TRUE;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdr_sector_info_ptr == NULL )
  {
    ret_val = FALSE;
  }
  else
  {
#ifdef FEATURE_MEMCPY_REMOVAL
    memscpy( hdr_sector_info_ptr->hdr_cell_info, sizeof ( hdr_sector_info_ptr->hdr_cell_info ), 
             hdrsrchltemeas_ctrl_info.sector_id, sizeof ( hdrsrchltemeas_ctrl_info.sector_id ) );
#else
    memcpy( hdr_sector_info_ptr->hdr_cell_info, hdrsrchltemeas_ctrl_info.sector_id, 
            sizeof ( hdr_sector_info_ptr->hdr_cell_info ));
      /* Provide sector ID (dest, source) */
#endif /* FEATURE_MEMCPY_REMOVAL */
  }

  return ret_val;

} /* hdrsrchltemeas_iratman_get_sector_cb */
#endif /* FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF */

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

void hdrsrchltemeas_powerup_init( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchltemeas_init_lists();
    /* Initialize LTE pilot measurement lists */

  hdrsrchltemeas_treselect_timer_def();
    /* Define Treselect timer */

} /* hdrsrchltemeas_powerup_init */

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

void hdrsrchltemeas_enter_inactive_state( void )

{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchltemeas_treselect_timer_clear();
    /* Clear pending Treselect timers */

  /* Reset all LTE measurement information once LTE reselection occurs.
     If LTE fails to acquire, DO should start LTE measurements again from
     scratch, not from staled data. */
  hdrsrchltemeas_reset_deinit();

 /* IRATMan is initialized after every deactivate and activate of HDR */
  hdrsrchltemeas_ctrl_info.iratman_init_done = FALSE;

#ifdef FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF 
  iratman_flush_persistent_data( RAT_HDR );
    /* Required call by IRATMan */
#endif /* FEATURE_MCS_IRATMAN_DYNAMIC_BACKOFF */

} /* hdrsrchltemeas_enter_inactive_state */

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

void hdrsrchltemeas_abort_lte_reselection ( void )

{

  trm_release_chain_input_info      client_info;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( 
    MSG_LEGACY_MED, 
    "D2L Debug hdrsrchltemeas_abort_lte_reselection() is called,state=%d",
    hdrsrchltemeas_ctrl_info.meas_state );

  if( hdrsrchltemeas_ctrl_info.reselection_in_progress == TRUE )
  {

    memset( &client_info,0,sizeof( client_info ) );
    client_info.client      = TRM_LTE; 
    trm_release_chain( &client_info );

    hdrsrchltemeas_clear_reselect_status();
      /* Clear reselect status to make sure DO doesn't reselect to LTE in abort
         cases */
  }

} /* hdrsrchltemeas_abort_lte_reselection */

#endif /* FEATURE_HDR_TO_LTE */

