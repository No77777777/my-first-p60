#ifndef L1_SC_IRAT_H
#define L1_SC_IRAT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        INTERRAT (WCDMA) SURROUND CELL

GENERAL DESCRIPTION
   This module contains routines specific to performing WCDMA interRAT
   measurements while in GSM.

EXTERNALIZED FUNCTIONS
   l1_sc_wcdma_update_list
   l1_sc_ded_wcdma_update_list
   l1_sc_irat_check_active
   l1_sc_wcdma_get_best_six
   l1_sc_wcdma_set_paging_multiframes
   l1_sc_wcdma_init
   l1_sc_wcdma_enter_idle
   l1_sc_wcdma_leave_idle
   l1_sc_wcdma_enter_xfer
   l1_sc_wcdma_enter_dedi
   l1_sc_check_priority_threshold
   l1_sc_wcdma_ded_pending
   l1_sc_wcdma_ded_ready
   l1_sc_wcdma_receive
   l1_sc_wcdma_srch_active
   l1_sci_tick_wcdma
   l1_sci_wcdma_abort_reconfirm
   l1_sci_wcdma_paging_block_tick
   l1_sc_lte_process_timed_search_results
   l1_sc_populate_cm_g2l
   l1_sc_cm_g2l_state
   l1_sc_wcdma_ded_tick

INITIALIZATION AND SEQUENCING REQUIREMENTS


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/l1_sc_irat.h#5 $
$DateTime: 2020/06/07 04:13:53 $ $Author: pwbldsvc $


when       who       what, where, why
--------   --------  ---------------------------------------------------------- 
05/06/20   sc        CR2703314 TDS Removal on Agatti- Moving G2T/T2G code under TDS feature
06/08/19   sal       CR2488908 If Cleanup cmd is not immediate to GFW then wait one frame to let cleanup complete. 
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
12/07/17   ng        CR2074605 Do not leave idle mode until G2X init confirm is received
05/10/16   ng        CR1093500 Irat Init/Deinit Code Change
22/06/16   ng        CR1026643 Booking 3 frames for G2L ded mode search in PTM may lead to conflict so increasing the number of frames needed
05/12/17   km        CR987605 G2X immediate cleanup
13/07/17   ng        CR2075022 Reset G2W ID search scheduling cycle only for a particular freq when reconf attempt on cell belonging to that freq fails
29/10/17   km        CR2133917 GL1 support for TA.3.0 L+L
19/09/16   dg        CR1066652 Make MCVS request of 384 mhz 1 frame in advance before initiating G2L search in dedicated mode
09/05/16   dg        CR1012577 Set g2x init flag to TRUE on receiving a new irat list if target tech is not initialized
20/10/15   snjv      CR927604 Update duration for DR Band registration in G2W Ded search
01/09/15   cgc       CR897441 renamed and moved MAX_FRAMES_FOR_IRAT_TRANSFER_MODE to MAX_FRAMES_FOR_IRAT_CONNECTED_MODE
17/08/15   cja       CR890396 In G2T dedicated abort do cleanup before setting state to NULL
28/07/15   aga       CR854929 Reflect the real frames required for W ID search with TRM
15/07/15   aga/zf    CR865337: NULL2T IRAT support.
08/07/15   df        CR815683 Avoid error recovery when G2W scripts are built during wakeup  
05/05/15   dp        CR830787 ensure srch handlers complete before SCE cleans up
24/04/15   cja       CR800657 QTA gap for G2W IRAT
21/04/15   cja       CR826384 For reconf do not use idle monitors in progress flag
03/02/15   cjl       CR790433 Create separate TRM activity for WCDMA reconfirm when needed
14/04/15   aga       CR799530 Reserve W/TD/L IRAT ID search with TRM in advance
09/04/15   cgc       CR819956 Thor 2.x RF API change, G2W use MSGR
30/03/15   cja       CR800650 Optimise irat state machine
06/03/15   cgc       CR794323 FR25799 support x8 LTE with fast idle search.
20/02/15   jj        CR 798186  IRAT enum duplication clean up
09/10/14   cgc       CR734430 G2L pruning algorithm to consider detection only scenarios
09/09/14   cgc       CR721718 FEATURE_GSM_TO_LTE changes to fix compile errors for non-lte build
07/08/14   cgc       CR700929 G2L update ded_tick after measurement completed
04/08/14   cja       CR703742 Check IRAT state machine is NULL before isr inact is complete

11/03/02   tb        Initial version.

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "err.h"

#ifdef  FEATURE_GSM_TO_LTE

#include "rfm_meas.h"

#include "lte_as.h"
#include "lte_l1_types.h"
#include "lte_cphy_irat_meas_msg.h"
#include "lte_ml1_irat_ext_api.h"


#include "msgr.h"
#include "msgr_geran.h"

#include "rr_l1.h"

#endif

#include "l1_sc.h"
#include "l1i.h"
#include "l1_utils.h"
#include "gl1_sys_params.h"
#include "msg.h"
#include "gl1_msg_wcdma_meas.h"
#include "l1_drx.h"
#include "l1_fm.h"

#include "log.h"            /* For log_commit() and log_alloc() and log codes */
#include "log_codes_gsm.h"  /* For WCDMA log codes */
#include "log_codes_gsm_v.h"
#include "l1_task.h"

#include "l1_isr.h"
#include "gpl1_gprs_isr.h"  /* For GPRS_NORMAL_PAGING enum */
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"

#include "l1_log.h"

#include "gl1_hw_clk_ctl_g.h"
#include "mcpm_api.h"
#include "gl1_hw_sleep_ctl.h"
#include "gl1_hwi.h"

#ifdef FEATURE_GSM_TDS
#include "tdsiratif.h"
#endif

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT*/
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
extern uint16 mdsp_ftsm (gas_id_t gas_id);

/************************** defines ***************************************/

#define MIN_LVL_dBm_x16              (-1024*16)  /* -1024 dBm (minimum sint15) */

#define MIN_EcNo_TO_BE_VALID_CELL    (-31*2)

#define MAX_EXPD_WCDMA_CELLS_SEARCH   2
#define MAX_PG_MFRMS                  9
#define MAX_ID_SEARCH_FRAMES         22
#define MAX_WCDMA_RECONF_FRAMES       6

/* Arbitrarily chosen to allow XRAT some traffic possibility between page and Wreconf */
#define PCH_TO_WCDMA_RECONF_GAP      20
#define WCDMA_RECONF_TO_PCH_GAP      20

#define MAX_LTE_SEARCH_FRAMES         15
/*
** FR25799, idle fast search window duration
*/
#define MAX_LTE_FAST_SEARCH_FRAMES    60        /* 8*7 , TDD is taking 7 frames per search, increase for testing from 45 */

#define MAX_SEARCH_CYCLE_WCDMA_FREQS  (3)
#ifdef FEATURE_GSM_TDS

/*This value needs to be refined*/
#define MAX_TDS_SEARCH_FRAMES         15

/*This value is the number of FREQs that we will cap to
  in order to get required time between G2T ID searchs -
  in this case the fastest, which is ~30s*/
#define MAX_FREQ_FASTEST_ACQ_TIME     1

#endif

/* used for TRM band registration */
#define MAX_FRAMES_FOR_IRAT_CONNECTED_MODE    (4)
/*In Dedicated Mode, GL1 has to wait till WFW is done with its Offline searches as well 
  A G2W Gap with a uarfcn will have 4 PSCs attempts in 1 Gap, for which WFW can take 4 * 15 msec
  So, Setting Max G frames to 15 (69 msec when 60 msec is required) 
  (Allowing 2 frames as Guard for further delays in WL1<->GL1 communication */
#define MAX_FRAMES_FOR_G2W_DED_SEARCH     (15)

/*
* idle frame gap to next idle frame for GSM activity
*/
#define NUM_IDLE_FRAMES_PER_DED_SRCH      4
#define NUM_IDLE_FRAMES_PER_DED_SRCH_G2L  (NUM_IDLE_FRAMES_PER_DED_SRCH-1)

#define MAX_SAVED_MEAS               (MAX_WCDMA_CELLS_PER_UARFCN/2)
#define MIN_PWR_TO_BE_VALID_CELL     (-31*2)

/* This is the number of times the WCDMA ID search can be aborted before
 * getting abandoned.
 */
#define WCDMA_MAX_TRIES              3

#define RAT_IS_WCDMA     (1)
#define RAT_IS_LTE_FDD   (2)
#define RAT_IS_LTE_TDD   (3)

/* 100 is choosen so that this will never get executed, and can be enabled later on */
#define MAX_RSCP_FLUCTUATION 100

#define LOG_MAX_KNOWN_CELL_LIST_SIZE  MAX_WCDMA_CELLS_PER_FREQ

#define L1_ISR_FEMTO_TIMEOUT  300000  /* ms = 5 mins */
#define L1_ISR_FEMTO_RUNNING  20000   /* ms = 20 seconds */

#define GSM_L1_NUM_PRIORITY_SEARCH_THRESHOLDS  16


#ifdef FEATURE_GSM_TDS

/* return the slot before the lowest downlink slot in xfer*/
#define LOWEST_DL_SLOT(s) ((s > 1) ? (s - 1):(s))

#endif

/*
** currently 16 to include both L+W entries , may increase for TDS
*/
#define MAX_L1_PRIORITY_ENTRIES        MAX_RR_L1_PRIORITY_ENTRIES
#define G2L_5_GAPS_USED                (5)

#define WCDMA_IDSEARCH_ADVANCE_NOTICE 20
#define TDS_IDSEARCH_ADVANCE_NOTICE 20
#define LTE_IDSEARCH_ADVANCE_NOTICE 20

#define G2X_MAX_OFFSET_FOR_PROCESS_CLEANUP 4850

#define MODE_CHANGE_REQ_RECEIVED  1
#define UPDATE_LIST_REQ_RECEIVED  2
#define ENBALE_G2X_IRAT_CLOCKS    3
/************************** enums / structures ***************************************/

#ifdef FEATURE_GSM_TO_LTE
typedef lte_cphy_irat_meas_meas_results_s  gl1_lte_cell_meas_type;
#endif



/* -----------KNOWN CELL LIST---------------- */
typedef PACKED struct PACKED_POST
{
    uint16 UARFCN;
    uint16 scr_code;   /* Scrambling code (0..511) */
    uint16  pn_pos;    /* PN position */
    boolean diversity; /* cell diversity */
    boolean ignore;    /* on ignore list */
    boolean thresholds_valid; /* thresholding used */

    int8 sQual;
    int8 sRxLev;
    int8 RSCP;  /* Received signal strength (in dBm) */
    int8 EcNo;  /* Energy value returned from firmware */

} l1_log_wcdma_known_list_info_T;

LOG_RECORD_DEFINE(LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C)   /*lint -esym(754, LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C_tag::hdr) suppress 'LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C_tag::hdr' not accessed*/
  uint32  frame_number;
  uint8   granted;
  uint16  UARFCN;
  uint8   num_cells;
  uint16  scr_code[MAX_WCDMA_CELLS_PER_FREQ];
LOG_RECORD_END

#define LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C_LEN(n_cell)  (\
    FPOS(LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C_type, scr_code) + \
      (n_cell*sizeof(uint16))   \
    )

LOG_RECORD_DEFINE(LOG_GSM_WCDMA_DED_CELL_LIST_C)   /*lint -esym(754, LOG_GSM_WCDMA_DED_CELL_LIST_C_tag::hdr) suppress 'LOG_GSM_WCDMA_DED_CELL_LIST_C_tag::hdr' not accessed*/
  uint8   qSearch_C;
  uint8   search_Prio;
  uint8   num_freqs;
  l1_log_wcdma_freq_info_T freq_info[MAX_WCDMA_FREQS];
LOG_RECORD_END

#define LOG_GSM_WCDMA_DED_CELL_LIST_C_LEN(n_freq, n_cell)  (\
    FPOS(LOG_GSM_WCDMA_DED_CELL_LIST_C_type, freq_info) + \
      (n_freq* (sizeof(uint16) + sizeof(uint8))) + \
      (n_cell* (sizeof(uint16) + sizeof(uint8)))   \
    )
LOG_RECORD_DEFINE(LOG_GSM_WCDMA_KNOWN_LIST_C) /*lint -esym(754, LOG_GSM_WCDMA_KNOWN_LIST_C_tag::hdr) suppress 'LOG_GSM_WCDMA_KNOWN_LIST_C_tag::hdr' not accessed*/
  uint8          Cell_Count;
  l1_log_wcdma_known_list_info_T Cell[LOG_MAX_KNOWN_CELL_LIST_SIZE];
LOG_RECORD_END


typedef struct
{
  uint32 frame_number;
  uint16 qs_offset;
} l1_gl1_time_s;


typedef struct
{
  l1_gl1_time_s start;
  l1_gl1_time_s end;
  l1_gl1_time_s clock;

} l1_rat_time_s;

/*
** for feature FEATURE_SGLTE_G2X_CONN_MODE_RESEL we can do priority searches in dedicated
*/
enum
{
  G2X_PRIORITY_FALSE,
  G2X_IDLE_PRIORITY,
  G2X_DEDICATED_PRIORITY
} g2x_search_priority;

#ifdef  FEATURE_GSM_TO_LTE

#define INVALID_FN               (0xFFFF)
#define G2L_GAP_START_OFFSET       (3750)
#define G2L_GAP_END_OFFSET          (325)
#define G2L_GAP_CLOCK_OFFSET          (0)   /* not used by LTE  */

#define G2L_MDSP_START_OFFSET      (3125)   /* offset wrt FNmod26=24 idle-1 */
#define G2L_MDSP_END_OFFSET        (4500)   /* offset wrt FNmod26=25 idle   */

typedef enum {
   G2L_START_CMD_IDX,
   G2L_CLEANUP_CMD_IDX,
   G2L_END_CMD_IDX
} g2l_mdsp_cmd_idx;

void   l1_sc_send_lte_cell_search_results_to_rr( uint16 freq_index, gas_id_t gas_id);

#endif

/************************** local function definitions ***************************************/
boolean g2l_cm_xfer_state(gas_id_t gas_id);
void l1_sc_set_gtow_srch_state_null (gas_id_t gas_id);
void l1_sc_irat_fm_clear(rr_l1_irat_rat_e l_rat, e_wcdma_srch_type srch_type, gas_id_t gas_id);
void l1_sc_set_gtow_srch_type (e_wcdma_srch_type new_srch_type, gas_id_t gas_id);
boolean l1_sc_check_priority_threshold(uint16 freq_idx, gas_id_t gas_id);

void  l1_sc_restart_ded_cell_id_srch( gas_id_t gas_id );
void  l1_sc_restart_ded_specific_cell_id_srch(uint16 freq_idx,  gas_id_t gas_id );

void  l1_sc_forced_meas_event(uint16 freq_idx , g2x_forced_meas_e measEvent, gas_id_t gas_id);

void l1_isr_femto_timer_start (void);
void l1_isr_femto_timer_stop( void );

void l1_sc_g2t_wait_state(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id);
void l1_sc_g2t_schedule(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id);
void l1_sc_g2w_schedule(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id);
void l1_sc_g2w_id_wait_state(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id);
void l1_sc_g2w_reconf_wait_state(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id);

uint16   l1_sc_wcdma_get_ded_next_srch_freq( boolean inhibit_update, gas_id_t gas_id );
void     update_stored_measurements_for_top_8( uint16 freq_idx, gas_id_t gas_id);
boolean  l1_sc_frames_available( l1_fm_priority_T pri, uint32 frames_required,
                                       void ( *abort_callback )( void*,gas_id_t ),
                                       void* abort_data,
                                       gas_id_t gas_id );
void l1_sci_wcdma_aborted( void* dummy, gas_id_t gas_id );
void l1_wcdma_abort_scheduled_reconfirm( void* dummy, gas_id_t gas_id );

boolean l1_sc_irat_state_machine_active(gas_id_t gas_id);

void  l1_sc_restart_cell_id_srch( gas_id_t gas_id );
void  l1_sc_restart_specific_cell_id_srch( uint16 freq_idx, gas_id_t gas_id );

void  gsm_l1_update_rr_report( const t_stored_meas      stored_meas[MAX_WCDMA_FREQS],
                               const t_rr_ignore_list*  rr_ignore_list_ptr,
                               const t_rr_params*       rr_params_ptr,
                               t_rr_report*             rr_report_ptr,
                               gas_id_t                 gas_id
                              );
void l1_sc_wcdma_send_rr_report(void* unused, gas_id_t gas_id);

#ifdef  FEATURE_INCREASED_PERIOD_FOR_CELL_ID_CELL_RECONF_SRCHS
void  l1_sc_send_cell_id_srch_to_rr(void* freq_index, gas_id_t gas_id);
#endif

void  l1_sc_wcdma_log_known_list( gas_id_t gas_id );
void  update_stored_measurements( uint8           num_new_freqs,
                                         t_stored_meas   stored_meas[MAX_WCDMA_FREQS],
                                         t_rr_params*    rr_params_ptr,
                                         gas_id_t        gas_id
                                       );

void  update_stored_measurements_from_ded_reconf( gl1_wcdma_cell_ded_reconf_rpt*  rpt,
                                                         uint16                          freq_idx,
                                                         gas_id_t                        gas_id);

boolean  get_stored_meas_cell_entry( uint32                                    UARFCN,
                                     uint16                                    scr_code,
                                     uint8                                     rat,
                                     gsm_l1_lte_wcdma_cell_params_type**       cell_params,
                                     t_gsm_l1_lte_wcdma_cell_energy_details**  cell_energy,
                                     gas_id_t                                  gas_id
                                    );

void remove_stored_meas_cell_entry( uint32  UARFCN,
                                    uint16  scr_code,
                                    uint8   rat,
                                    gas_id_t gas_id);

void  update_stored_meas_cell_entry( uint32                           UARFCN,
                                     const  gl1_wcdma_cell_meas_type* cell_data_ptr,
                                     uint8                            rat,
                                     boolean rscp_valid,
                                     gas_id_t gas_id
                                   );

void  add_stored_meas_cell_entry( uint32                           UARFCN,
                                  const gl1_wcdma_cell_meas_type*  cell_data_ptr,
                                  uint8   rat,
                                  boolean rscp_valid, gas_id_t gas_id
                                );

boolean l1_sc_wcdma_ded_ready_tds(boolean tds_perform_acq, uint16 f_off, uint16  ded_meas_count, gas_id_t gas_id);

void  id_report_cb (gl1_wcdma_cell_meas_rpt *rpt, gas_id_t gas_id);
void  id_abort_cb (gas_id_t gas_id);
void  reconfirm_abort_cb (gas_id_t gas_id);
void  reconfirm_report_cb (gl1_wcdma_cell_meas_rpt *rpt, gas_id_t gas_id);
void id_in_tbf_report_cb (gl1_wcdma_cell_meas_rpt *rpt, gas_id_t gas_id);
void  id_in_tbf_abort_cb (gas_id_t gas_id);
void  ded_srch_id_cb (gl1_wcdma_cell_ded_rpt *rpt, gas_id_t gas_id);
void  ded_srch_abort_cb ( boolean w_aborted_gap, gas_id_t gas_id);
void  ded_srch_reconf_cb (gl1_wcdma_cell_ded_reconf_rpt *rpt, gas_id_t gas_id);
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
void  srch_handler_done_cb(gas_id_t gas_id);
extern boolean gl1_check_if_g2w_init_pend(gas_id_t gas_id);
#endif

boolean gl1_check_if_g2w_init_deinit_pend(gas_id_t gas_id);

#ifdef FEATURE_GSM_TO_LTE
extern boolean gl1_check_if_g2l_init_pend(gas_id_t gas_id);
#endif

#ifdef FEATURE_GSM_TDS
extern boolean gl1_check_if_g2t_init_pend(gas_id_t gas_id);
void g2t_irat_exit_rf (tdsirat_meas_rf_param_type *g2t_enter_ptr,                 gas_id_t gas_id );

void  l1_sc_tds_report(gas_id_t gas_id);
#endif

void l1_g2t_gap_end(gas_id_t gas_id);

void l1_sc_g2x_pending_shutdown(gas_id_t gas_id);

void  l1_sc_g2x_filter_treatment(t_gsm_l1_lte_wcdma_cell_energy_details**  cell_energy,
                                     int16 power, uint16 freq_idx, uint16 meas_idx, rr_l1_irat_rat_e rat_type, gas_id_t gas_id);

void g2x_rfm_meas_common_init_params( rfm_meas_common_param_type *g2x_common_rf_ptr, gas_id_t gas_id);
boolean l1_sc_g2x_check_for_extend_srch_period( gas_id_t gas_id );

/************************** external function definitions ***************************************/


/************************** global data ***************************************/
extern t_stored_meas     stored_meas[MAX_WCDMA_FREQS];

/* CR355881: Hack should be removed once Abort is implemented*/
extern boolean l1_send_tds_irat_build_rfscripts[NUM_GERAN_DATA_SPACES];

extern boolean g2x_clk_boost_done[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_TDS
extern volatile boolean  gl1_TDS_IRAT_INIT_CNF_pending;
#endif

#ifdef FEATURE_GSM_TO_LTE
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
/*Track cleanUp command sent to GFW is Immediate command*/
extern boolean cleanup_cmd_sent[NUM_GERAN_DATA_SPACES];
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
#endif


extern const dBx16_T  gsm_l1_priority_search_threshold[ GSM_L1_NUM_PRIORITY_SEARCH_THRESHOLDS ];

extern boolean nv_for_search_all_w_cell ;

/* Bit mapped Flag to check if INIT_REQ is yet to be issued to the other RAT */
extern uint8  check_irat_active[];
/* Definition of the FEMTO state machine states.
This state machine is the callback when the timer expires*/
typedef enum
{
  FEMTO_SEARCH_NULL,
  FEMTO_SEARCH_TIMER_RUNNING,
  FEMTO_SEARCH_IN_PROGRESS
} femto_search_state_enum;

extern femto_search_state_enum femto_search_state;

extern boolean l1_sc_g2x_timeline_opt_enable[];

extern boolean mdsp_decrement_startup_time_offset_flag[];
extern boolean mdsp_decrement_cleanup_time_offset_flag[];
extern uint8   l1_g2t_delay_for_fw_startup;

/************************** function prototypes ***************************************/
extern void l1_sc_init_femto_timer (void);
extern void l1_sc_init_femto_timer_delete (void);

/* 
** new RF api
*/
void  g2x_set_common_rf_enter( rfm_meas_common_param_type *g2x_common_rf_ptr, gas_id_t gas_id);

#ifdef FEATURE_GSM_TDS
void  g2t_populate_startup_params(tdsirat_startup_param_type  *ptr_rf_startup, gas_id_t gas_id);

tdsirat_meas_rf_param_type        g2t_enter_rf_param;
#endif
rfm_meas_common_param_type*       gl1_get_irat_rf_structure_ptr(gas_id_t gas_id);
boolean l1_sc_x2g_ta_irat_gap(gas_id_t gas_id);
boolean l1_sc_is_reconf_next_pch_possible(gas_id_t gas_id);

#endif /*L1_SC_IRAT_H*/
