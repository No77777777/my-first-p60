#ifndef SRCHDCH_H
#define SRCHDCH_H
/*===========================================================================

   S R C H    D C H    S T A T E   R E L A T E D   F U N C T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None


REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2002-2012 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchdch.h#4 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/03/20   hs      After SFN decode call resume suspend after cell decfg is done
12/11/19   ac      Searcher Mini Dump Change
12/29/17   nc      Whenever deact thresh for 1A reached, reset 1A related parameters
                   so that 1A eval starts afresh after 1A resumed again
08/18/17   nc      Added an API to check if any sfn decoding is ongoing
04/29/16   rn      Move the srch_evt_rpt_dbase inline with srch_intra_freq_meas_db.
04/22/16   rn      Doubling the DCH internal and external db
11/23/15   vn      Removing SET RXD state function 
06/29/15   jk      Apply threshold for E1A for X ms, in case a cell was removed from ASET 
                   due to early reporting of E1B using TPR enhancement conditions.
05/30/15   jk      FR 25383: Traffic-to-Pilot Ratio (TPR) Based Measurement Optimization.
01/20/15   jd      Global re-org for W + W
01/12/15   ag      Removed compilation warnings.
10/10/14   amj     Code Cleanup.
05/14/14   jd      Changes to add frame reservation in ACQ and BCH.
04/02/14   jk      Reset filter values, if WGW Tuneaway time is > 4 seconds. Back up Resuming_WCDMA
                   global var during DCH_INIT, since it is reset before PHY_CHAN_ESTABLISH.
03/07/14   jd      Removing usage of srch_backup_async_srch_timer.
01/22/14   jk      Min_timer on E1D_enhancement enablling will be based on flag 
                   instead of timer being set to value other than 0.
01/10/14   jd      Searcher clean-up
01/07/14   amj     Clean up old ASET Update code.
12/10/13   da      Fixed KW error related to search measurements
11/25/13   jk      FR 15721: HSPA+: Call performance improvement when selecting the best cell.
11/21/13   jd      FET 2.0 Searcher two queue approach.
11/18/13   rsr     Added QICE 3C Support.
11/06/13   gj      Added 3C searcher code
11/06/13   km      Refresh QICE srch cell lists before passing it to CME
10/17/13   bs      Preliminary 3C Support
09/12/13   jd      Searcher redesign
07/23/13   jd      parallel step1 changes
05/16/13   zr      Externing srchdch_aset_init_filt_for_sir_target for new ASET update procedure
11/28/12   amj     MCVS feature related chnages   
12/31/12   vn      Making srchdch_num_step1_pks_for_nastt extern 
                   so that it can be used in srchcpcdrx.c
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
04/18/12   zr      Added previous DCH substate variable for restoring/reverting
02/08/12   vs      Feature cleanup.
02/01/12   gnk     Nikel feature cleanup
11/12/11   sv      Improved 1A TTT change algo.
11/11/11   mm      W2G CM optimizations
11/06/11   sv      Added code for intraf cell crossing optimizations.
06/17/11   sa      Added code to support list search logging for all cells
                   after every 1 sec.
03/17/11   sv      Moved get aset rscp from srchdch.h to wsrchi.h
01/28/11   sv      Added API to return RSCP across ASET cells
11/24/10   sa      Added enum to calculate event 3a leaving criteria as 
                   per spec.
10/19/10   sv      Added changes to support full search for DC and QICE support.
05/03/10   sv      Fixed compiler warnings.
02/06/10   sv      Mainlined feature FEATURE_WCDMA_OFFLINE_SEARCHER.
01/25/10   sv      Extern the functions for reserving step1\NASTT searches during 
                   ASU to timing unknown ASET cells when CM active.
05/27/09   stk     Fixed Compiler Warning
03/12/09   rc      Added HS callback function to update change in CPICH STTD 
                   of HS cell.
10/03/08   hk      Bringing in 7k Mailine fixes
01/22/07   nd      event 1J, exported a function to be called from edl.c
11/01/06   cpe     Mainlined the following features:
                    -FEATURE_OPTIMIZE_SEARCH_FOR_HHO
06/05/06   sv      Added define for search EcIo noise floor, lint fixes.
03/20/06   sv      Added support for Rx diversity searcher changes under 
                   FEATURE_WCDMA_RX_DIVERSITY, Additional searcher changes to 
                   support FEATURE_MDSP_MULTI_RF_CHAIN_INTERFACE.
05/13/05   m       Added prototype for a new function
04/14/05   vb      Externed srchdch_step1_srch_cmd()
04/13/05   m       Added support for srch optimization for HHO
10/14/02   scm     Extern srchdch_substate for SUSPEND processing in srch.c.
09/26/02   djm     created file, export srchdch evt 3a eval function
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"

#include "l1rrcif.h"

#include "srchsched.h"

#include "wsrch.h"
#include "wsrchi.h"
#include "srchparm.h"
#include "srchsetutil.h"
#include "l1def.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define SRCH_STEP1_MAX_PEAK_CNT_C0 12
#define SRCH_STEP1_MAX_PEAK_CNT_WITH_CM_C0 8
#define SRCH_STEP1_MAX_PEAK_CNT_C1 8

/*--------------------------------------------------------------------------
                          DCH SEARCH SUBSTATES

--------------------------------------------------------------------------*/
typedef enum
{
  SRCHDCH_INIT,           /* Initialization */
  SRCHDCH_INACTIVE,       /* No search activity */
  SRCHDCH_ASET_CSET,      /* ASET and CSET Search */
  SRCHDCH_NSET_STEP1,     /* NSET Step 1 search */
  SRCHDCH_NSET_NASTT,     /* NSET NASTT search */
  SRCHDCH_HHO_NASTT,      /* NASTT search during HHO */
  SRCHDCH_NSET_USET,      /* NSET List search   */
  SRCHDCH_A_NO_PN_LIST,   /* List search   */
  SRCHDCH_DC_NSET_STEP1,  /* DC Step1 search */
  SRCHDCH_DC_NSET_NASTT,  /* DC NASTT search */
  SRCHDCH_DC_PN_SRCH,     /* DC PN search */
  SRCHDCH_ALL_CELL_SRCH,  /* Search to log all ACSET+ NSET cells */
  SRCHDCH_PARALLEL_NSET_STEP1,  /* Parallel step1 on C0 and C1 */
  SRCHDCH_PARALLEL_NSET_NASTT, /* Parallel nastt on C0 and C1 */
  SRCHDCH_STATE_MAX       /* Last item in enum. */
}
srchdch_substate_enum_type;


/* Enyum type for evaluating 3a event criteria on UMTS frequency */
typedef enum
{
  EVENT_3A_RESET_ON_USED_FREQ,
  EVENT_3A_SET_ON_USED_FREQ,  
  EVENT_3A_NO_CHANGE
}srchdch_event3a_crit_enum_type;




typedef void SRCHDCH_HS_TD_UPDATE_IND_CB_FUNC_TYPE( wsub_id_e_type wsub_id);



#define SRCHDCH_PATHLOSS_MIN_VAL (46)
#define SRCHDCH_PATHLOSS_MAX_VAL (158)

#define SRCH_SIRT_MAX_NUM_FILTERS   1
#define SRCH_SIRT_MAX_FILT_LEN      8
#define SRCH_SIRT_MIN_FILT_LEN      4

#define SRCHDCH_MIN_TTT_REDUC       10 /* 100 ms*/
#define SRCHDCH_MIN_TTT_FOR_OPT     10 /* 100 ms*/
#define SRCHDCH_MAX_TTT_FOR_OPT     20 /* 200 ms */
/* Minimum TTT value should be > 200 ms for 1A TTT reduction */
#define SRCH_MIN_TTT_FOR_REDUC      20

#define SRCHDCH_THRES_FOR_CELL_MOV  40 /* -20dB*/
#define EIGHT_FIVE_PERCENT          85
#define EIGHTY_PERCENT              80
#define ONE_EIGHTY_MS               18 
#define THREE_SIXTY_MS              36
#define SRCH_SIRT_FILT_THRESH                  28
#define SRCH_DCH_MAX_NUM_CARRIERS        3


typedef struct
{
  boolean enable;
  uint16 filt_len;
  uint16 filt_ecio;
  uint16 prev_filt_ecio_hi_res;
  uint16 grow_timer;
  boolean eval_win_started;
  uint16 evt1a_mrm_ts_cnt;
  
}srchdch_sirt_filter_stuct_type;

typedef struct
{
   uint8 num_filters;
   srchdch_sirt_filter_stuct_type  filt_params[SRCH_SIRT_MAX_NUM_FILTERS];
}srchdch_aset_sirt_filt_struct_type;

typedef struct 
{
    uint16 num_aset_cells;
    uint16 num_mset_cells;
    uint16 num_uset_cells;
} srchdch_cell_meas_cnt_struct_type;

typedef struct 
{
    uint16 num_cells ;

    boolean event_id;
} srchdch_intraf_meas_rpt_struct_type ;

typedef void SRCHDCH_FET_STATUS_CB_FUNC_TYPE(boolean, wsub_id_e_type wsub_id);

typedef struct
{
  srch_cell_struct_type *srchdch_unsorted_rpt_set_ptr[SRCHDCH_CELL_MEAS_RES_MAX];
  /* sorted ASET in DCH state */
  srch_cell_struct_type *srchdch_sort_aset_ptr[SRCHMEAS_MAX_NUM_CARR][SRCH_ASET_MAX_C0];
  srch_cell_struct_type *srchdch_sorted_rpt_set_ptr[SRCHDCH_CELL_MEAS_RES_MAX];

  /*keep info when 1A MRM was sent to RRC */
  srchdch_evt1a_info_type evt_1a;

  srch_sfn_decode_client_enum_type srch_sfn_decode_client[SRCHMEAS_MAX_NUM_CARR];
}srchdch_ext_cntrl_params_mdump_type;

typedef struct
{
  uint8 srch_prim_nbr_cnt;
  uint8 srch_pri_carr_qice_sorted_cnt;
  uint8 srch_sec_carr_qice_sorted_cnt[WL1_MAX_VALID_CARRIER_IDX];
  uint8 srchdch_last_antenna_searched;
  boolean srch_rxdiv_state;
  uint8 srch_dc_nbr_cnt[WL1_MAX_VALID_CARRIER_IDX];
  /* keep a snapshot of the time when the LPQ srch was issued*/
  uint16 srchdch_ts_cnt_at_lpq_issue;
  uint16 srchdch_cell_meas_result_cnt;
  uint8 srchdch_edch_aset_cnt[SRCHMEAS_MAX_NUM_CARR];

  /* Callback function definition */
  SRCHDCH_HS_TD_UPDATE_IND_CB_FUNC_TYPE *srch_dch_hs_td_upd_cb;
  /* FET status callback for overlapped searches */
  SRCHDCH_FET_STATUS_CB_FUNC_TYPE *srchdch_fet_status_cb;
  uint16 srchdch_full_search_timer[NUM_CARR];
  srchdch_aset_sirt_filt_struct_type srchdch_aset_sirt_filt;


  /* Data structure to store the sorted R99 neighbors for QICE */
  srch_cell_struct_type *srch_pri_carr_qice_sorted_ptr[SRCH_SYNC_NSET_MAX_C0];
  /* Data structure to store the sorted DC neighbors for QICE */
  srch_cell_struct_type *srch_sec_carr_qice_sorted_ptr[WL1_MAX_VALID_CARRIER_IDX][SRCH_SYNC_NSET_MAX_C1];
  srch_cell_struct_type *srch_prim_nbr_ptr[SRCH_PRI_NBRS_MAX];

  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchdch_ext_cntrl_params_mdump_type srchdch_ext_cntrl_params_mdump;
}srchdch_ext_cntrl_params_type;


extern srchdch_ext_cntrl_params_type srchdch_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  srch_cell_struct_type *srchdch_unsorted_evt_rpt_set_ptr[L1_MAX_CELL_MEAS];
  srch_cell_struct_type *srchdch_sorted_evt_rpt_set_ptr[L1_MAX_CELL_MEAS];
  int16 srchdch_1a_ttt_reduc_time;
  srchdch_cell_meas_cnt_struct_type srchdch_cell_meas_cnt_struct;
  srch_cell_struct_type *srchdch_evt_rpt_cell_ptr[SRCHMEAS_MAX_NUM_CARR][SRCH_SYNC_NSET_MAX_C0];
  srch_cell_struct_type *srchdch_best_cell_ptr[SRCHMEAS_MAX_NUM_CARR];
  /* The actual number of cells with event(s) to be reported */
  uint8 srchdch_evt_rpt_cell_cnt[SRCHMEAS_MAX_NUM_CARR];
  srchdch_intraf_meas_rpt_struct_type srchdch_evt_rpt_dbase[SRCHMEAS_MAX_NUM_CARR][SRCH_MAX_INTRAF_MEAS_ALLOWED];

}srchdch_int_cntrl_params_mdump_type;

typedef struct
{
  /* Debug variable to skip SFN decoding on PCCPCH */
  boolean srch_debug_skip_sfn_decode;
  boolean skip_e1d_initial;
  /* unsorted and sorted lists for event results */
  uint16 srchdch_cell_evt_res_cnt;
  uint32 debug_cnt;
  /*debug purposes */
  uint32 count;
  int16 srchdch_ecio_rpt_range_for_1a[SRCHMEAS_MAX_NUM_CARR];
  uint32 *srchdch_pathloss_linear_to_db_tab_ptr;
  /* Rx AGC value updated after every 20 ms HPQ search */
  int16 srchdch_hpq_rx_agc_val[SRCHMEAS_MAX_NUM_CARR];
  srch_cell_struct_type *tmp_srched_cell_array[SRCH_NORMAL_MODE_MAX_PN_TASKS];
  /* Data structure to store all R99 neighbors for QICE sorting */
  srch_cell_struct_type *srch_dc_nbr_ptr[WL1_MAX_VALID_CARRIER_IDX][SRCH_DC_NBRS_MAX];
  
  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchdch_int_cntrl_params_mdump_type srchdch_int_cntrl_params_mdump;
}srchdch_int_cntrl_params_type;



void srchdch_aset_cset_srch_done(srch_done_cmd_type *, wsub_id_e_type wsub_id);
void srchdch_nset_srch_done(srch_done_cmd_type *, wsub_id_e_type wsub_id);
void srchdch_step1_done(srchsched_step1_nastt_status_struct_type *step1_nastt_status, wsub_id_e_type wsub_id);
void srchdch_nastt_done (srchsched_step1_nastt_status_struct_type *results, wsub_id_e_type wsub_id);

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION SRCHDCH_EVT_3A_EVAL

DESCRIPTION
  This function performs UMTS Quality evaluation for the Event 3a handling
  code.

  Qutran - the quality of the UMTS Active set signals
  Mutran - the measured energy of the UMTS Active set as specified in
           the following formula.
  Mi     - the measured energy of the ith UMTS active set signal
  Mbest  - the measured energy of the best UMTS active set signal
            ( == srch_aset_ptr[0] )
  Na     - the number of active set cells
  W      - signaled parameter
  Tused  - signaled Threshold parameter
  Hyst3a - signaled hysteresis parameter

  Qutran = 10 * Log(Mutran) = W * 10 * Log(SUM[1..Na] Mi) + (1-W) * 10 * Log(Mbest)

  If the following formula is satisfied the UMTS quality is below the UTRAN specified
  threshold.  Event 3a reporting will take place iff an Inter-RAT signal is above
  the specified threshold.
  
  Qutran <= Tused - Hyst3a/2


DEPENDENCIES
  srch_aset_ptr[0] always points to the strongest cell in ASET. This is
  guranteed by function srchdch_proc_acset_srch() immediately after 
  each HPQ list search dump.
  
  The neighbor cell's and each ASET cell's ecio have been computed in 
  srchdch_proc_sync_nset_srch() and srchdch_proc_acset_srch().

RETURN VALUE
  if  ( Qutran <= Tused - Hyst3a/2 )
    return (TRUE);
  else
    return (FALSE);

SIDE EFFECTS
  None
===========================================================================*/
 srchdch_event3a_crit_enum_type srchdch_evt_3a_eval(

  l1_meas_quan_enum_type  meas_quan,
    /* EcIo or RSCP Measurement Quantity Specified */
  int32  W,
    /* weight specified by network */
  int32  Tused,
    /* Threshold to be used, specified by network */
  int32  Hyst3a
    /* hysterises specified by network */
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHDCH_STEP1_SRCH_CMD

DESCRIPTION
  This function issues a step1 search command in DCH state
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchdch_step1_srch_cmd( void);

extern void srchdch_reset_async_srch_parms( wsub_id_e_type wsub_id);

extern srch_cell_struct_type *srchdch_get_non_forbidden_cell(uint8 carr_idx,uint16 meas_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHDCH_UPDATE_EDCH_ASET

DESCRIPTION
  update EDCH active set from demod edch RL DB. 
  


DEPENDENCIES
  .EDCH active set PSCs and num of EDCH active set should be available before
  calling the function

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchdch_update_edch_aset( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchdch_register_hs_cpich_td_callback
         
DESCRIPTION
  This function registers the HS callback function.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void srchdch_register_hs_cpich_td_callback(
  /* Callback function pointer */
  SRCHDCH_HS_TD_UPDATE_IND_CB_FUNC_TYPE *hs_cb, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchdch_deregister_hs_cpich_td_callback
         
DESCRIPTION
  This function deregisters the HS callback function.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void srchdch_deregister_hs_cpich_td_callback( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHDCH_STEP3_RESULT_OUTSIDE_LIST_SRCH_WINDOW

DESCRIPTION
  This function checks whether a step3 peak returned is outside list 
  search window or not.
  
DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchdch_nastt_result_outside_pn_srch_window(srch_cell_struct_type *cell_ptr,
                                                        uint16 pilot_idx, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION srchdch_aset_filt_trig_sirt_boost

DESCRIPTION
  
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
extern void srchdch_aset_filt_trig_sirt_boost( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchdch_aset_init_filt_for_sir_target

DESCRIPTION
  initializing parameter for SIRT boost filter 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

extern void srchdch_aset_init_filt_for_sir_target( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_DC_HSDPA

/*===========================================================================

FUNCTION SRCHDCH_SORT_PRIM_NEIGHBORS_FOR_SRCH

DESCRIPTION
  This function sorts the prim carrier neighbors in descending order of energy.
  CME module requires this in order to work on only the topmost energied cells.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchdch_sort_prim_neighbors_for_qice( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHDCH_SORT_DC_NEIGHBORS_FOR_SRCH

DESCRIPTION
  This function sorts the DC neighbors in descending order of energy. CME module
  requires this in order to work on only the topmost energied cells.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchdch_sort_dc_neighbors_for_qice(uint8 carr_idx, wsub_id_e_type wsub_id);

#endif /*FEATURE_WCDMA_DC_HSDPA*/

/*===========================================================================

FUNCTION SRCHDCH_E1D_ENHANCEMENT_EVAL

DESCRIPTION
	Checks if the difference between target cell and serving cell is greater 
than the threshold (T2). If positive, activates the min_timer variable, if not
active already. If negative, deactivates the min_timer if active.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void srchdch_e1d_enhancement_eval(int16 tgt_cell_meas_val, int16 srv_cell_meas_val, srch_evt_ts_struct_type *evt_ts, wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION SRCHDCH_E1D_ENHANCEMENT_RESET

DESCRIPTION
	Resets the min_time to 0.  
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/
void srchdch_e1d_enhancement_reset(srch_evt_ts_struct_type *evt_ts);
/*===========================================================================

FUNCTION SRCHDCH_IS_E1D_ENHANCEMENT_NV_ENABLED

DESCRIPTION
  	Returns the NV enabled value.
  
DEPENDENCIES
  None.

RETURN VALUE
  NV_Enabled (boolean).

SIDE EFFECTS
  None.
  
===========================================================================*/
inline boolean srchdch_is_e1d_enhancement_nv_enabled( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION SRCHDCH_GET_E1D_ENHANCEMENT_THRESHOLD

DESCRIPTION
  Returns the threshold value set by the user through NV.
  
DEPENDENCIES
  None.

RETURN VALUE
  Threshold (uint16).

SIDE EFFECTS
  None.
  
===========================================================================*/
inline uint16 srchdch_get_e1d_enhancement_threshold( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION SRCHDCH_GET_E1D_ENHANCEMENT_NOMINAL_TIMER

DESCRIPTION
  Returns the e1d enhancement nominal timer.
  
DEPENDENCIES
  None.

RETURN VALUE
  Nominal timer (uint16).

SIDE EFFECTS
  None.
  
===========================================================================*/
inline uint16 srchdch_get_e1d_enhancement_nominal_timer( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION SRCHDCH_GET_E1D_ENHANCEMENT_MIN_TIMER

DESCRIPTION
  Returns the e1d enhancement min timer.
  
DEPENDENCIES
  None.

RETURN VALUE
  Min timer (uint16).

SIDE EFFECTS
  None.
  
===========================================================================*/
inline uint16 srchdch_get_e1d_enhancement_min_timer( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHDCH_GET_E1D_ENHANCEMENT_MIN_TIMER

DESCRIPTION
  Returns the e1d enhancement min timer.
  
DEPENDENCIES
  None.

RETURN VALUE
  Min timer (uint16).

SIDE EFFECTS
  None.
  
===========================================================================*/
boolean srchdch_is_e1d_enhancement_min_timer_enabled(srch_evt_ts_struct_type *evt_ts);



/*===========================================================================

FUNCTION SRCHDCH_IS_CARR_IDX_VALID

DESCRIPTION
  This function checks if carr_idx has a valid value. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: carr_idx is valid
  FALSE: carr_idx is invalid.

SIDE EFFECTS
  None.

===========================================================================*/

extern inline boolean srchdch_is_carr_idx_valid(uint8 carr_idx);

/*===========================================================================

FUNCTION SRCHDCH_SKIP_E1D_INITIAL

DESCRIPTION
  This function checks if carr_idx has a valid value. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: Skip E1d initial is set.
  FALSE: Skip E1d initial has been reset.

SIDE EFFECTS
  None.

===========================================================================*/

static inline boolean srchdch_skip_e1d_initial(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHDCH_SET_SKIP_E1D_INITIAL

DESCRIPTION
   This function sets srchdch_int_cntrl_params.skip_e1d_initial global var. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static inline void srchdch_set_skip_e1d_initial(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHDCH_RESET_SKIP_E1D_INITIAL

DESCRIPTION
  This function resets srchdch_int_cntrl_params.skip_e1d_initial global var. 

DEPENDENCIES
  None.

RETURN VALUE
  None. 

SIDE EFFECTS
  None.

===========================================================================*/

static inline void srchdch_reset_skip_e1d_initial(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchdch_ext_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize shared variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void srchdch_ext_cntrl_params_init( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION srchdch_int_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize exclusive variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void srchdch_int_cntrl_params_init( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION srchdch_check_any_sfn_decoding_ongoing

DESCRIPTION
    This function checks if any sfn decoding is ongoing
DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern inline boolean srchdch_check_any_sfn_decoding_ongoing( wsub_id_e_type wsub_id);



#ifdef FEATURE_WCDMA_TPR_BASED_MEASUREMENTS
#error code not present
#endif /* FEATURE_WCDMA_TPR_BASED_MEASUREMENTS */

/*===========================================================================
FUNCTION       SRCHDCH_UPDATE_EVT_RPT_DBASE_MEAS_IDX

DESCRIPTION
    On the event of measurement event movement inside the database, corresponding 
indices in the srchdch_evt_rpt_dbase indices has to be updated. This function takes
care of that.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/ 
extern void srchdch_update_evt_rpt_dbase_meas_idx(uint8 carr_idx,uint16 src_meas_idx,
                                     uint16 dst_meas_idx, wsub_id_e_type wsub_id);

/*===========================================================================

  FUNCTION 
  SRCHDCH_RESET_1A_MEAS_CELLS_WHEN_DEACT_THRES_REACHED

  DESCRIPTION 
  Due to active set update if the active set cell cnt becomes greater than the
  1A deactivation threshold then no neighbors are evaluated for 1A criteria. In that case it is 
  better to reset the 1A parameters for all the neighbors so that they are evaluated again for
  TTT period before being reported after deactivation thershold restriction is not there.

  DEPENDENCIES
  None

  RETURN VALUE
  None.

  SIDE EFFECTS
  None.

===========================================================================*/

void srchdch_reset_1a_meas_cells_when_deact_thres_reached(uint8 carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================

  FUNCTION 
  srchdch_cell_decfg_done_after_sfn_decode

  DESCRIPTION 
   Call back to call after cell decfg due to SFN decde done. 
   This will inform demod is suspen request is pending

  DEPENDENCIES
  None

  RETURN VALUE
  None.

  SIDE EFFECTS
  None.

===========================================================================*/

void srchdch_cell_decfg_done_after_sfn_decode (wsub_id_e_type wsub_id);


#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION     srchdch_qsh_mdump_collect

  DESCRIPTION
  This function dumps all the required parameters from srchdch internal and 
  external control parameter structures

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/
extern QSH_MDUMP_FN_ATTR void srchdch_qsh_mdump_collect(wsub_id_e_type wsub_id);
#endif


#endif /* SRCHDCH_H */
