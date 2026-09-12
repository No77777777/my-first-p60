#ifndef SRCH_BPLMN_H
#define SRCH_BPLMN_H
/*===========================================================================

   S R C H    B P L M N    S T A T E   R E L A T E D   D E C L A R A T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


REFERENCES
  WCDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2000 to 2009 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchbplmn.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/20   ac      Mini dump framework checkin
12/11/19   ac      Searcher Mini Dump Change
06/10/16   vn      Added a new macro WSRCH_GET_BPLMN_FREQ to return BPLMN freq
11/20/15   ad      Changes to get cell info from backed up db when in pch bplmn
11/02/15   sl      Remove un-used global variable in SRCH_BPLMN database
01/20/15   jd      Global re-org for W + W
09/08/14   bj      DRX_TIMER_EXPIRY cmd handling in ACQ state for spllit acq case. 
08/23/14   bj      Added split acq across DRX support in BPLMN state.
08/21/14   da      Fixed compiler warnings
04/16/14   bj      BPLMN split ACQ code changes
09/12/13   jd      Searcher redesign
09/19/11   kcm     Added a postcall back function 
                   srchbplmn_resume_on_serving_cell_cb
03/13/09   sv      Corrected a typo. 
12/09/08   sv      Mainlined FEATURE_WCDMA_OFFLINE_SEARCHER, FEATURE_RUMI_BRINGUP 
                   and FEATURE_WCDMA_DL_ENHANCED.
10/26/07   sv      Added searcher support for FEATURE_WCDMA_DL_ENHANCED.
11/24/05   nd/gv   Code cleanup
11/08.05   nd/gv   Checkin the code onto mainline.
11/07/05   nd      initial creation for the feature W to W BPLMN srch

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/*--------------------------------------------------------------------------
                          BPLMN SEARCH SUBSTATES

--------------------------------------------------------------------------*/

typedef enum
{
  SRCH_BPLMN_INACTIVE,
  SRCH_BPLMN_ACQ_STEP1,
  SRCH_BPLMN_ACQ_STEP_NASTT,
  SRCH_BPLMN_ACQ_STEP_PN,
  SRCH_BPLMN_ACQ_KNOWN_POS_PN,
  SRCH_BPLMN_ACQ_PULLIN,
  SRCH_BPLMN_ACQ_STTD,
  SRCH_BPLMN_NUM_STATES
}srch_bplmn_state_enum_type;



/* Structure to save any serving cell related information for restoring once BPLMN
   search is over */
typedef struct
{
  /* serving cell frequecny */
  uint16 serv_cell_freq;
  /* detected set count for the serving cell */
  uint8 serv_freq_uset_cnt;
  /*save the SYNC_N set information...count*/
  uint8 serv_freq_sync_nset_cnt;
}srchbplmn_saved_info_struct_type;

/* step3 peaks from BPLMN searches are stored in this structure*/
typedef struct 
{
  uint32  max_eng;
  uint32  max_pos;
}srchbplmn_cell_peak_struct_type;

#ifdef FEATURE_WCDMA_L1_SPLIT_ACQ_ACROSS_BPLMN_CYCLES

/* Structure to save NASTT detected pilot in case of split acq bplmn, 
    we would use this info in next drx cycle*/
typedef struct
{
  /* acquistion request frequecny */
  uint16 saved_acq_req_freq;
  uint16  saved_nastt_pilot_cnt;
  srch_pilot_peak_struct_type  saved_nastt_pilots[SRCH_MAX_NASTT_PILOTS];
  timetick_type sp_acq_res_start_timeticks;

}srchbplmn_split_acq_resume_struct_type;

typedef struct
{
  uint32 bplmn_sp_acq_cnt;
  uint32 bplmn_sp_acq_resume_cnt;
  uint32 bplmn_sp_acq_resume_pass_cnt;
  uint32 bplmn_sp_acq_resume_pn_fail_cnt;
  uint32 bplmn_sp_acq_resume_pull_fail_cnt;
  boolean next_acq;
  uint32 next_acq_pass_cnt;
  uint32 next_acq_fail_cnt; 

}srchbplmn_split_acq_resume_stat_struct_type;
#endif

typedef struct
{
#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /* Indicates different searcher substates within a BPLMN search */
  srch_bplmn_state_enum_type srchbplmn_substate;
#endif

#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /* allow to check if an acquisition was done before setting up the PCCPCH in this BPLMN cycle */
  boolean srchbplmn_acq_status;
#endif

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /*Now that BPLMN ACQ search is split into segments and we release TRM. So its uncertain whether the DRX time is 
suitable for performing the full ACQ procedure. So RRC will indicate DRX timer expiry to wl1 at which point ACQ
will be aborted if not at the last segment */
  boolean srch_drx_tmr_expired;
  #endif
}srchbplmn_ext_cntrl_params_mdump_type;


typedef struct
{

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  #ifdef FEATURE_WCDMA_L1_SPLIT_ACQ_ACROSS_BPLMN_CYCLES
  /*flag  to capture split acq stats*/
  boolean srchbplmn_split_acq_capture_stats;
  #endif

  #endif

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  #ifdef FEATURE_WCDMA_L1_SPLIT_ACQ_ACROSS_BPLMN_CYCLES
  srchbplmn_split_acq_resume_stat_struct_type split_acq_resume_stat;
  #endif

  #endif

#ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchbplmn_ext_cntrl_params_mdump_type srchbplmn_ext_cntrl_params_mdump;
#endif

}srchbplmn_ext_cntrl_params_type;


extern srchbplmn_ext_cntrl_params_type srchbplmn_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef struct
{
  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /* This is a database of different parameters that need to be saved when we go for BPLMN srch
  when we finish BPLMN we need to restore these parameters*/
  srchbplmn_saved_info_struct_type srchbplmn_saved_db;
  #endif

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /* We can have upto 24 paths for 6 cells */
  srchbplmn_cell_peak_struct_type srchbplmn_cell_peak_set[SRCH_MAX_PILOT_PEAKS_FOR_SIX_CELLS];
  #endif

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /* Array of scrambling codes detected after pullin
 * (max value clipped to L1_WCDMA_ACQ_DET_LIST_MAX).
 */
  l1_acq_scr_code_det srchbplmn_scr_code_det[L1_WCDMA_ACQ_DET_LIST_MAX];
  #endif

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  #ifdef FEATURE_WCDMA_L1_SPLIT_ACQ_ACROSS_BPLMN_CYCLES
  srchbplmn_split_acq_resume_struct_type srchbplmn_split_acq_resume_db;
  #endif
  #endif

}srchbplmn_int_cntrl_params_mdump_type;


typedef struct
{
  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /* Number of scrambling codes detected after pullin
 * (max value clipped to L1_WCDMA_ACQ_LIST_MAX).
 */
  uint8 srchbplmn_num_scr_code_det;
  #endif

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /* store the ACQ mechanism if FREQ or FREQ_SCR_POS */
  l1_wcdma_acq_mech_type srchbplmn_acq_mech;
  #endif

  #ifdef FEATURE_WTOW_BACKGROUND_PLMN_SEARCH
  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchbplmn_int_cntrl_params_mdump_type srchbplmn_int_cntrl_params_mdump;
#endif

}srchbplmn_int_cntrl_params_type;



/* Macro to return BPLMN freq. If any module is expecting BPLMN freq, 
they need to use this macro and not srch_get_curr_freq() as it
is updated to return serving cell freq always, even in BPLMN state*/
#define WSRCH_GET_BPLMN_FREQ() ((WL1_GET_L1M_STATE() == L1M_PCH_BPLMN) ? (l1msetup_ext_cntrl_params[wsub_id].l1_pending_acq_cmd.freq): NO_FREQUENCY )


/*This global variable is defined to check whether the BPLMN acq 
  was successful or not. This flag is checked when trying to establish 
  physical channel to demodulate SIBs. If this is not set to TRUE then 
  a CNF is sent to RRC with a failure for the setup request.*/


/*===========================================================================

FUNCTION        SRCHBPLMN_FS_REQ_CMD

DESCRIPTION
  Freq scan for BPLMN searches

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void srchbplmn_fs_req_cmd(l1_freq_scan_cmd_type *cmd, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCHBPLMN_ACQ_REQ_CMD

DESCRIPTION
  This function first check if there is any step of neighbor search command
  to be issued following an ASET (HPQ) search done. if yes, the function
  issues the search command to DSP. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchbplmn_acq_req_cmd(l1_acq_cmd_type *cmd, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCHBPLMN_SAVE_SERVING_CELL_INFO

DESCRIPTION
  save the serving cell parameters before starting the BPLMN searches

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchbplmn_save_serving_cell_info( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchbplmn_get_saved_freq

DESCRIPTION
    This function is used to get frequency which was saved while entering into BPLMN

DEPENDENCIES
    None.

RETURN VALUE
    freq before entering into BPLMN.

SIDE EFFECTS
    None.
===========================================================================*/
extern uint16 srchbplmn_get_saved_freq(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHBPLMN_RESTORE_SERVING_CELL_INFO

DESCRIPTION
  restore the serving cell parameters after BPLMN searching is over.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchbplmn_restore_serving_cell_info( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION SRCHBPLMN_ACQ_PULLIN_DONE

DESCRIPTION
  This function is called by Demod module once pull-in operation is finished.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchbplmn_acq_pullin_done( dl_acq_pullin_done_cmd_type* cmd , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHBPLMN_PROC_SRCH_OPS

DESCRIPTION
  This function processes "srch_ops_cmd" in BPLMN state

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void srchbplmn_proc_srch_ops(srch_ops_cmd_type  *cmd_ptr);

/*===========================================================================

FUNCTION       SRCHBPLMN_PROC_SRCH_DONE

DESCRIPTION
  This function starts the processing of step1/2/3 or list search .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchbplmn_proc_srch_done( srch_done_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION      SRCHBPLMN_ADD_PCCPCH_N

DESCRIPTION
  This function is called to set up the PCCPCH on the BPLMN cell just acquired. The 
  PCCPCH is setup to resolve TTI ambiguity first and then reading SIBs for upper layers.
  It will be setup using concurrent PCCPCH_N with reason SFN_SFN_diff. The reason to use
  the SFN_SFN_DIFF as the reason to set up the neighbor PCCPCH channel was because
  it will allow to use the existing code without much modification.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean indicating if the channel was added successfully or not.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchbplmn_add_pccpch_n
(
  dl_phychan_add_type* phch_cmd,
  /* Freq of the cell on which the PCCPCH will be setup*/
  uint16 freq,
  /*the cell on which the PCCPCH will be setup*/
  srch_cell_struct_type* nghbr_cell_ptr
);

extern void srchbplmn_dereg_aset_postproc_resume_go_to_sleep( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION srchbplmn_resume_on_serving_cell_cb

DESCRIPTION
  This post call back function is executed to tune the RF to the serving cell 
  frequency and inform DRX that the BPLMN searching is done.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchbplmn_resume_on_serving_cell_cb( wsub_id_e_type wsub_id);

void srchbplmn_handle_step1_nastt_done(srchsched_step1_nastt_status_struct_type* results, wsub_id_e_type wsub_id);
void srchbplmn_handle_pn_done(srch_done_cmd_type* srch_cmd, wsub_id_e_type wsub_id);
void srchbplmn_handle_acq_pn_done(void);
extern void srch_handle_drx_expiry_cmd( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_L1_SPLIT_ACQ_ACROSS_BPLMN_CYCLES

extern void srchbplmn_reset_split_acq_resume_db( wsub_id_e_type wsub_id);

#endif

/*===========================================================================

FUNCTION SRCHBPLMN_ACQ_RPT

DESCRIPTION
  This function report acqusition result to RRC. acq_status "TRUE" indicates
  that acquisition succeeded; and acq_status "FALSE" indicates the acqusition
  failed.

DEPENDENCIES
  assumed that .FEATURE_BARRED_CELL_DURING_CELL_SELECTION is always defined

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchbplmn_acq_rpt (boolean acq_status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchbplmn_ext_cntrl_params_init

DESCRIPTION
    This function is called at wl1 task init to initialize shared variables.

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.
===========================================================================*/
extern void srchbplmn_ext_cntrl_params_init( wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP

/*===========================================================================
  FUNCTION     srchbplmn_qsh_mdump_collect

  DESCRIPTION
  This function dumps all the required parameters from srchbplmn internal and 
  external control parameter structures

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/

extern QSH_MDUMP_FN_ATTR void srchbplmn_qsh_mdump_collect(wsub_id_e_type wsub_id);

#endif

#endif
