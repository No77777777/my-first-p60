#ifndef WL1_X2W_SRHC_DRV_H
#define WL1_X2W_SRHC_DRV_H

/*===========================================================================
   FILE: wl1x2wsrchdrv.h

GENERAL DESCRIPTION
   This file contains all of the definitions necessary to interface between
   Searcher module and L1 mDSP Sync interface module or mDSP.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2002-2013 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1x2wsrchdrv.h#6 $
$DateTime: 2020/07/07 08:09:55 $
$Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/07/20   rs      Maintaining the clock speed if ABORT recived in the middle of NASST search in L2W connected mode measurement.
01/10/20   ac      Mini dump framework checkin
11/22/19   rs      FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios
11/13/19   hs     Added uarfcn TO 0x41D7, 0x41D8, 0x41D9 log packet
20/07/18   rs      Handling race between ABORT and SRCH_CNF
06/02/18   pk     Avoid stop command at the time of deinit in TA when already stop is send at the time of sleep.
03/28/17   rs      Assert if NASTT is issued before receiving last_step1 task in L2W connected mode cell id measurment   
02/11/17   rs      x2w in QTA support
04/24/17   rs      WFW GAP_END flag support
01/13/16   mk      Ignore one cmd done from FW  in  WL1_X2W_SLEEP_WFW_STATE state 
12/21/15   wz      send STOP cmd to WFW 
12/08/15   gn      X2W F3 cleanup
10/12/15   db      Correct the PN reconf energy threshold for L2W connected mode searches
10/02/15   gn      Added 0x41D7, 0x41D8, 0x41D9 log packet for X2W driver code
09/18/15   cc      Offload build G2W RF script to DPC
08/28/15   ks      Make sure abort confirmation happens after RF script building is done
08/11/15   abs     Changes to support X2W MEAS frequency reporting
07/28/15   cc      Add a msgr log array
07/09/15   cc      Delay sending search result till sleep WFW
07/07/15   ks      Store common RF pointer
06/29/15   cc      Do not send ABORT cmd is we are outside of the gap
06/29/15   cc      Tune RF for the fresh full win search only
06/22/15   cc      L2W MSGR support
06/04/15   cc      Port direct stage 2 change
04/10/15   cc      Add G2W MSGR support
01/22/15   mm      IRAT globals re-organization for W+W
06/24/14   cc      Moving G2W drivers to WL1 folders
17/12/13   cja     CR590767 Bolt G2W - add gap start time to g2w startup
02/12/14   js      CR595142 :  T+G compilation errors removed
01/08/13   cja     CR511042 Separate RF ID for active and CM.
31/07/13   cja     CR498800 Use TRM to get IRAT RF ID
11/07/13   cja     CR511042 Separate RF ID for active and CM.
20/06/13   cs      Major Triton TSTS Syncup
18/06/13   cja     CR498800 Use TRM to get IRAT RF ID
19/03/13   pg      CR463973 Ensure WFW START wait signal is set on receiving WFW response  
20/02/13   cja     CR454583 Correct g2w state for rex set on WFW start
31/10/12   pg      CR409233: updates for G2W on Dime modem 
16/10/12   mm      Added function to validate G2W state before issuing a search.
02/04/12   mm      CR 348067 Wait for GL1 abort after FW AGC tuning completes
03/15/12   mm      CR 344531 G2W drivers new design 
28/11/11   cja     CR321479 Do not sleep if G2W WFW active
10/11/11   cja     CR314495 Fix Klocwork errors
01/11/11   cja     CR306187 Remove unused functions
11/01/11   cgc     Fix compiler warnings removed PACKED from int array
21/09/10   cja     Fix Lint and KlocWork errors
10-04-01   tjw     Add customer.h and comdef.h to all SU API files
23/23/10   ws      Integrate G2W driver from MDM8200
30/03/09   ws      Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                   to replace FEATURE_GSM_GPRS_MUSTANG
16/03/09   ws      Added PACKED_POST for QDSP6 targets
19/02/09   scm     Increase number of dedicated results possible.
19/01/09   cs      Fix weird double spacing issues in previous change
02/01/09   nt      Change of MUSTANG featurisation
12/17/08   scm     Extern wcdma_srch_id_srch_parms.
12/15/08   scm     Extern boolean g2w_dedicated_mode_on.
12/12/08   scm     Implement idle and dedicated reconfirmation PN search.
12/10/08   scm     Move some defines from g2wsrchdrv.c to g2wsrchdrv.h.
12/10/08   scm     Extern data for g2w_store_stage1_result() support.
09/25/08   scm     More mods to support FEATURE_GSM_GPRS_MUSTANG feature.
09/22/08   scm     More G2W FEATURE_GSM_GPRS_MUSTANG mods.
09/16/08   scm     Firmware API mods inside FEATURE_GSM_GPRS_MUSTANG.
03/04/07   ws      Corrected Featurisation for GSM only builds
11/09/05   am      Added API for the gl1_wcdma_ecio_logarithmic_average
04/20/05   cpe     Updated STTD field for G2W searches
04/03/05   rc      Added function definition for gl1_srch_compute_rscp()
                   and thresholds for calculating CPICH RSCP.
02/04/05   gfr     Added prototypes for programming sample ram functions.
01/30/04   jc      Support for automatic Reconf list search after Stage2.
06/24/03   cpe     Added G2W dedicated mode prototype functions, structures
                   and externs.
02/25/03   cpe     Updated the sample RAM related variables.
01/31/03   cpe     Rolled back the sample RAM values to 0x3cca and 2492
01/31/03   cpe     Added two new sample ram related constants for step3
11/22/02   cpe     Changed a few parameters in structure
                   g2w_srch_step3_peak_struct_type and
                   gl1_wcdma_step3_peaks_type
07/23/02   cpe     Created file.
11/04/02   cpe     Added a new structure for returning EcIo, RSCP and RSSI
                   back to GL1 module.

===========================================================================*/

#ifndef CUSTOMER_H
  #include "customer.h"
#endif

#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "wl1x2wsrchmdspsync.h"
#include "wl1x2wsrchapi.h"

#include "rfmeas_types.h"
#include "rfm_meas.h"

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*===========================================================================
                        DATA DEFINITIONS
===========================================================================*/

/* Max number of tasks to be sent to stage2 */
#define WL1_X2W_SRCH_STAGE1_LIMIT_NUM  35

/*---------Parameters for Calulating CPICH RSCP------------------------------*/

/* Minimum CPICH RSCP, corresponding to -116dBm */
#define WL1_X2W_SRCH_MIN_CPICH_RSCP  232
  
/* Minimum Usable CPICH Ec/Io Value for CPICH RSCP calculation,
   Corresponding to -24.5 dB */
#define WL1_X2W_SRCH_MIN_CPICH_ECIO  49

/*---------------------START WFW timeout periods----------------------------*/

/* Definitions for START WFW timeout periods */
#define WL1_X2W_SRCH_START_WFW_WAIT_MSEC                50

#define WL1_X2W_SRCH_DEDIC_SRCH_START_WFW_WAIT_MSEC     10

#define WL1_X2W_SRCH_LTOW_IMMEDIATE_TUNE_TIME           0x7FFFFFFF

/* Maximum number of the MSGR logs */
#define WL1_X2W_SRCH_MSG_LOG_MAX 30

/* Used to limit the number of search result F3s */
#define WL1_X2W_NUM_RESULTS_PRINT_MAX 3

/*============================================================================ 
                      ENUMERATIONS
=============================================================================*/

/*============================================================================
 						  STRUCTS
=============================================================================*/

/* X2W Outer Main State machine */
typedef enum
{
  WL1_X2W_INACTIVE_STATE,
  WL1_X2W_START_WFW_STATE,
  WL1_X2W_CXM_CFG_STATE,
  WL1_X2W_MODE_RF_CFG_STATE,
  WL1_X2W_RF_STOPPED_STATE,
  WL1_X2W_SEARCH_STATE,
  WL1_X2W_SLEEP_WFW_STATE,
  WL1_X2W_ABORT_STATE,
  WL1_X2W_CLK_CFG_STATE,
  WL1_X2W_STOP_WFW_STATE,
  WL1_X2W_NUM_STATES
} wl1_x2w_srch_drv_state_enum_type;

/* Control Info struct for State machine */
typedef struct
{
  /* Search Mode -- Idle or Dedicated */
  wl1_x2w_srch_wfw_mode_enum_type         mode;
  /* Flag to indicate if the RF is switched on */
  boolean                                 srch_rf_on;
  /* Flag to indicate GL1 abort is delayed till AGC tuning 
  and SR done/RF ON ISR is complete */
  boolean                                 abort_delayed;
  /* State machine state */
  wl1_x2w_srch_drv_state_enum_type        driver_state;
  /* Search state in Idle or Dedicated */
  wl1_x2w_srch_drv_search_state_enum_type search_state;
  /* G2W or L2W srch */
  wl1_x2w_drv_type_enum_type              drv_type;
  /* indicate if a WFW START CMD has been issued and therefore a
     response is still pending */
  boolean                                 wfw_start_sent;
} wl1_x2w_srch_ctrl_info_struct_type;

typedef struct
{
  /* Flag for Dedicated mode search programmed and parameters, if TRUE */
  boolean                          ded_mode_srch_programmed;
  /* Flag for Ded mode Reconf search programmed and params, if TRUE */           
  boolean                          reconf_srch_programmed;
  /* num of cells included */
  uint8                            num_id_cells;
  /* how many cells to reconfirm */
  uint8                            num_reconf_cells;
  /* UARFCN */
  uint16                           uarfcn;
  /* Dedicated mode cells to be searched */
  uint16                           id_cell_pscs[WL1_X2W_SRCH_MAX_PSC_PER_FREQ];
  /* the list of cells to reconfirm */
  wl1_x2w_located_cell_struct_type reconf_cells[WL1_X2W_SRCH_MAX_PSC_PER_FREQ];
} wl1_x2w_srch_ded_mode_input_struct_type;

/* Message array and its index */
typedef struct 
{
  uint8 idx;
  msgr_umid_type ids[WL1_X2W_SRCH_MSG_LOG_MAX]; 
} wl1_x2w_srch_msg_log_struct_type;

/* X2W Outer Main State machine */
typedef enum
{
  WL1_X2W_STOP_CMD_NOT_SENT,
  WL1_X2W_STOP_CMD_SENT,
  WL1_X2W_STOP_CMD_RSP_RCVD
} wl1_x2w_stop_cmd_state;


typedef struct
{
  /* Flag for if WFW is started */
  boolean wl1_x2w_srch_wfw_is_running;
  /* Flag for if RF script built but WFW start is not ready */
  boolean wl1_x2w_srch_pre_srch_cnf_pending;
  /* Flag for if abort cnf needs to pend */
  boolean wl1_x2w_srch_abort_cnf_pending;
  /* Flag for if RF build scripts is ongoing */
  boolean wl1_x2w_srch_rf_script_building_ongoing;
  /* Flag for if deinit is pending */
  boolean wl1_x2w_srch_deinit_cnf_pending;
  /* Flag to know if source tech has opened a QTA gap */
  boolean wl1_x2w_srch_in_qta_gap;
  /* Flag to know if the stop command is send during sleep*/
  wl1_x2w_stop_cmd_state wl1_x2w_srch_stop_cmd_in_sleep;
  /* WCDMA frequency being searched in idle mode */
  uint16 wl1_x2w_srch_wcdma_freq;
  /* variables used to keep track of the preload offsets needed for FED */
  uint16 wl1_x2w_srch_nbr_preload_offset_time;
  uint16 wl1_x2w_srch_home_preload_offset_time;
  /* Drivers Main Control Structure */
  wl1_x2w_srch_ctrl_info_struct_type wl1_x2w_srch_drv_ctrl_info;
  /* Unique NAS-TT peaks */
  wl1_x2w_srch_nastt_result_struct_type wl1_x2w_srch_unique_nastt_result;
  /* Dedicated mode results output. This gets passed to GL1 via the callback handler */
  wl1_x2w_srch_measured_result_struct_type wl1_x2w_srch_ded_pn_srch_result;
  /* Input dedicated mode parameters */
  wl1_x2w_srch_ded_mode_input_struct_type wl1_x2w_srch_ded_mode_parms;
  /* List search parameter struct object */
  wl1_x2w_srch_mdsp_list_srch_q_parm_struct_type wl1_x2w_srch_reconf_list_srch_q_parms;
  /* PN after NAS-TT search parameters */
  wl1_x2w_srch_mdsp_list_srch_q_parm_struct_type wl1_x2w_srch_pn_after_nastt_srch_q_parms;
  /* Global variable to get the startup and cleanup time for from RF */
  wl1_x2w_rf_timing_params_struct_type wl1_x2w_srch_rf_timing_params;
  wl1_x2w_srch_msg_log_struct_type wl1_x2w_srch_msg_log;
  rfm_meas_header_type wl1_x2w_srch_rfm_meas_params_header;
  /* Used to detect WFW_START_WFW_CMD_ID timing out. */
  rex_timer_type wl1_x2w_srch_start_wfw_timer;
  #ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  wl1_x2w_srch_mdsp_start_mode_ue_type wl1_x2w_srch_wfw_multi_sim_mode;
  boolean wl1_x2w_srch_cxm_update_received;
  #endif
  wl1_irat_x2w_srch_cnf_struct_type wl1_x2w_srch_srch_done_msg;
  rfm_meas_common_param_type* wl1_x2w_common_rf_ptr;

  /* L2W globals */
  boolean wl1_x2w_srch_wfw_start_rf_script_cnf_pending;
  boolean wl1_x2w_srch_l2w_result_cnf_pending;
  boolean wl1_x2w_srch_gap_in_progress;
  boolean wl1_x2w_srch_x2w_inited_only;
  boolean wl1_x2w_srch_x2w_sleep_pending;
  boolean wl1_x2w_srch_x2w_gap_end;
  boolean wl1_x2w_srch_x2w_rf_script_built;

  /* Callback pointers from L2W driver into LTE L1. */
  wl1_l2w_nastt_srch_done_cb wl1_l2w_nastt_done_ptr;
  wl1_l2w_rf_script_build_done_cnf_cb wl1_l2w_rf_script_build_done_ptr;
  wl1_l2w_pn_srch_done_cb wl1_l2w_pn_srch_done_ptr;
  wl1_l2w_cleanup_done_cnf_cb wl1_l2w_cleanup_cnf_ptr;
  wl1_l2w_abort_srch_meas_cnf_cb wl1_l2w_abort_cnf_ptr;
  /* This callback should be called when WFW start completes
     for WFW start request triggered from wl1_l2w_initiate_conn_search()*/
  wl1_l2w_wfw_start_cnf_cb    wl1_l2w_wfw_start_cnf_ptr;
  wl1_l2w_srch_results_s wl1_l2w_nastt_peaks;
  wl1_l2w_meas_results_s wl1_l2w_pn_peaks;
  boolean wl1_l2w_pending_cleanup_cnf;
  lm_handle_type wl1_l2w_handle;
  boolean wl1_x2w_srch_l2w_ded_step1_in_progress;
  #if defined(FEATURE_WCDMA_JOLOKIA_MODEM) || defined(FEATURE_WCDMA_TABASCO_MODEM)
  /*Variable to keep track of of number of step1 searches requested in WL1_X2W_DEDICATED_ID_SRCH, this variable indicates it's a last step in step1 search*/
  boolean wl1_l2w_step1_last_srch_request_received;
  #endif
  uint8 counter_hack_rf_tune_skip;
  uint32 time_to_wait_x2w_hack;
} wl1x2wsrchdrv_int_cntrl_params_type;

/*=============================================================================
                          EXTERN VARIABLES 
=============================================================================*/

/*=============================================================================
                           FUNCTION PROTOTYPES
=============================================================================*/

extern void wl1x2wsrchdrv_int_cntrl_params_init( wsub_id_e_type wsub_id);
extern void wl1x2wsrchdrv_ext_cntrl_params_init();

/*===========================================================================

FUNCTION  WL1_X2W_SRCH_CREATE_MSG

DESCRIPTION: 
  This function creates the message with the appropriate header to be sent 
  to the G/L MSGR interface

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1_x2w_srch_create_msg(
  /* Message header */
  msgr_hdr_struct_type* hdr, 
  /* Header UMID */
  msgr_umid_type umid,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  WL1_X2W_SRCH_SEND_MSG

DESCRIPTION:
  This function is the wrapper function for msgr_send which sends the message to 
  the MSGR interface with the header and payload

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void wl1_x2w_srch_send_msg(
  /* Message to send */
  msgr_hdr_struct_type* hdr,
  /* Size of the message */
  uint32 msg_size,
  wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION      WL1_X2W_SRCH_INIT_CRIT_SECT

DESCRIPTION   This function initializes the search driver critical section.

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
==========================================================================*/
void wl1_x2w_srch_init_crit_sect(wsub_id_e_type wsub_id);

/*=========================================================================

FUNCTION      WL1_X2W_SRCH_DRV_INIT

DESCRIPTION   This function initializes the search driver structures and
              regsiters callback for FW-to-SW event done

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
==========================================================================*/
void wl1_x2w_srch_drv_init(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_DRV_VALIDATE_STATE

DESCRIPTION     This function does a validity check if the search is already aborted
                and state is ABORT_STATE.

DEPENDENCIES    None.

RETURN VALUE    TRUE, if state is valid
                FALSE, if state is ABORT_STATE

SIDE EFFECTS    None.

===========================================================================*/
boolean wl1_x2w_srch_drv_validate_state(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION        WL1_X2W_SRCH_ENG_SORT_STAGE1_PATHS

DESCRIPTION     This function sorts the paths list in the decreasing order of
                energies. It uses bubble sort algorithm to achieve its goal.
                This function operates on the wl1_x2w_srch_mdsp_sync_stage1_result[] data structure
                by default and will be kept so unless there is need to use
                this function on other path lists as well.

DEPENDENCIES    None

INPUT PARAMS    None

RETURN VALUE    None

SIDE EFFECTS    wl1_x2w_srch_mdsp_sync_stage1_result[] is sorted per energy of each path
===========================================================================*/
void wl1_x2w_srch_eng_sort_stage1_paths(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_PROCESS_DED_MODE_STAGE1_RESULTS

DESCRIPTION
  This function processes the results of a stage1 search

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void wl1_x2w_srch_process_ded_mode_stage1_results(uint16 uarfcn, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_PROCESS_DED_MODE_STAGE2_RESULTS

DESCRIPTION
  This function processes the results of a stage2 search.
  Note that this function is executed in interrupt context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void wl1_x2w_srch_process_ded_mode_stage2_results(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_PROCESS_RECONF_SEARCH_RESULT

DESCRIPTION
  This function processes the results of a Reconf search.
  Note that this function is executed in interrupt context

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void wl1_x2w_srch_process_reconf_search_result(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_DRV_PROGRAM_PN_AFTER_NASTT_SEARCH

DESCRIPTION
  This function programs PN after NAS-TT search in idle mode. It uses the
  peaks found in the NAS-TT search for the PN search. There is also a debug
  option in which the PN search is conducted on the whole frame instead of
  the PN window.

DEPENDENCIES
  NAS-TT search must have been completed.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void wl1_x2w_srch_drv_program_pn_after_nastt_search (wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_DRV_PROGRAM_STAGE1_SEARCH

DESCRIPTION
  This function sends stage1 search request to the mDSP after setting the
  stage1 search parameter values of ded_mode_stage1_srch_q_parms structure
  for the cells to be searched.

DEPENDENCIES

RETURN VALUE None

SIDE EFFECTS
  Stage 1 search in dedicated mode is started.

===========================================================================*/
void wl1_x2w_srch_drv_program_stage1_search (wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_DRV_PROGRAM_STAGE2_SEARCH

DESCRIPTION
  This function sends stage2 search request to the mDSP after setting the
  stage2 search parameter values of ded_mode_stage2_srch_q_parms structure
  for the cells to be searched.

DEPENDENCIES

RETURN VALUE
   TRUE, if the command is sent to mDSP
  FALSE, otherwise

SIDE EFFECTS
  Stage 2 search in dedicated mode is started.

===========================================================================*/
void wl1_x2w_srch_drv_program_stage2_search (wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WL1_X2W_SRCH_DRV_DED_MODE_PROGRAM_RECONFIRM_SRCH

DESCRIPTION     This function programs a reconfirmation list search.

DEPENDENCIES    None

RETURN VALUE    void

SIDE EFFECTS
===========================================================================*/
void wl1_x2w_srch_drv_program_reconfirm_srch(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_DRV_DO_IDLE_SEARCH

DESCRIPTION
  This function is the state machine function for Idle W searches.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if the command is sent to FW
  FALSE, otherwise

SIDE EFFECTS
  Search commands will be sent to FW

===========================================================================*/
void wl1_x2w_srch_drv_do_idle_search(
  wl1_x2w_srch_drv_search_state_enum_type srch_state,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_DRV_DO_DEDICATED_SEARCH

DESCRIPTION
  This function is the state machine function for Dedicated mode W searches.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if the command is sent to FW
  FALSE, otherwise

SIDE EFFECTS
  Search commands will be sent to FW

===========================================================================*/
void wl1_x2w_srch_drv_do_dedicated_search(
  wl1_x2w_srch_drv_search_state_enum_type srch_state,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION    WL1_X2W_SRCH_DRV_CONTINUE_SRCH

DESCRIPTION
  This function is the main state machine function. It is installed as
  a callback handler for all FW-to-SW ISR events. Based on the event triggered,
  the states would be moved in the state and the next cmd to WFW would be issued.
  This would not be called as cb from cmd done isr only when ABORT is the next cmd
  from the cmd queue that's being sent to FW.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Search commands will be sent to FW

===========================================================================*/
void wl1_x2w_srch_drv_continue_srch(
  wl1_x2w_srch_mdsp_event_type mdsp_sync_event,
  WfwSwCmdEnum cur_cmd,
  wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION     WL1_X2W_SRCH_GET_ECIO

DESCRIPTION  This function converts an energy into the 6-bit Ec/Io value required
             for pilot strength measurements. It works by doing a binary search on
             the ecio_tab to find the closest value (without going over), and returning
             the index in the ecio_tab.

DEPENDENCIES  None.

RETURN VALUE  6-bits Ec/Io value. If the energy is out of range,  the return value is
              clipped to 0 or 63.

SIDE EFFECTS

===========================================================================*/
uint16 wl1_x2w_srch_get_ecio(
  uint32 cell_eng,
  wsub_id_e_type wsub_id);

/*==========================================================================
FUNCTION       WL1_X2W_SRCH_COMPUTE_RSCP

DESCRIPTION    This function calculates the RSCP given the ecIo and the RxAGC. The
               calculated RSCP value will be -ve of the returned value.

DEPENDENCIES   The cell's ecio have been computed.

RETURN VALUE   uint16

SIDE EFFECTS   None

===========================================================================*/
uint16 wl1_x2w_srch_compute_rscp(
  uint16 ecio, 
  int16 rx_agc,
  wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION WL1_X2W_SRCH_GET_ECIO_RSCP_RSSI

DESCRIPTION
  This function converts an energy into the 6-bit Ec/Io value required
  for pilot strength measurements. It works by doing a binary search on
  the ecio_tab to find the closest value (without going over), and returning
  the index in the ecio_tab.

DEPENDENCIES
  None.

RETURN VALUE
  6-bits Ec/Io value. If the energy is out of range,  the return value is
  clipped to 0 or 63. The value returned is double that of the actual
  EcIo. For example a returned value of -13 corresponds to an actual
  EcIo of -6.5 dB.

  Similarly rscp value returned is also double the actual value.
  -200 returned corresponds to -100 dBm.

  RSSI value is returned in dBm units. A value of -90 represents an RxAGC
  value of -90 dBm.

SIDE EFFECTS

===========================================================================*/
void wl1_x2w_srch_get_ecio_rscp(
  int16                      rssi,
  wl1_x2w_measured_cell_type *cell_eng_ptr,
  wsub_id_e_type             wsub_id);

/*===========================================================================

FUNCTION  WL1_X2W_SRCH_MSG_HANDLER

DESCRIPTION:
  This function handles all the messages received from GSM/LTE. It will
  extract the results if needed and call the callback function,
  informing that a message was received.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_x2w_srch_msg_handler(
  /* Message received */
  msgr_hdr_struct_type* hdr,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_X2W_SRCH_DEINIT

DESCRIPTION  This function cleans up the measurements because source L1 is going 
             INACT and shutting down drivers. It disables WFW app if WFW is not
             running, else it ERROR FATALs because source L1 did not send an abort
             if trying to cleanup during a search. 

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  Searches will be terminated. ERR_FATALs if an abort was not 
              sent before if issued during a search. 
===========================================================================*/
extern void wl1_x2w_srch_deinit(
  wl1_irat_x2w_deinit_req_struct_type *deinit_req,
  wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
/*=========================================================================

FUNCTION      WL1_X2W_CXM_READY_CB

DESCRIPTION   This function is called when CxM channel IDs are returned

DEPENDENCIES   None.

RETURN VALUE   None.

SIDE EFFECTS   None.
==========================================================================*/
extern void wl1_x2w_srch_cxm_ready_cb(wsub_id_e_type wsub_id);
#endif

/*===========================================================================

  FUNCTION: wl1_x2w_get_drv_type

===========================================================================*/
/*!
    @brief
    This function returns the WL1 X2W driver mode

    @return
    WL1_G2W_SRCH or WL1_L2W_SRCH
*/
/*=========================================================================*/
extern wl1_x2w_drv_type_enum_type wl1_x2w_get_drv_type (wsub_id_e_type wsub_id);

/*===========================================================================

  FUNCTION: wl1_x2w_get_srch_in_qta_gap

===========================================================================*/
/*!
    @brief
    This function returns if WL1 X2W driver is doing meas in QTA gap

    @return
    TRUE or FALSE
*/
/*=========================================================================*/
extern boolean wl1_x2w_get_srch_in_qta_gap (wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_irat_wl1x2wsrchdrv

DESCRIPTION
  This is a minidump function for dumping the selected MCAL global variables for IRAT searches

DEPENDENCIES
  NONE

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_irat_wl1x2wsrchdrv(wsub_id_e_type wsub_id);
#endif

#endif /* WL1_X2W_SRCH_DRV_H */
