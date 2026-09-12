#ifndef TDSSRCHACQ_H
#define TDSSRCHACQ_H
/*===========================================================================

   T D S S R C H    B C H    S T A T E   R E L A T E D   D E C L A R A T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2001 - 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchacq.h#2 $ 
$DateTime: 2021/02/04 23:39:42 $ 
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdsl1rrcif.h"
#include "tfw_sw_intf_srch.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
#define TDSSRCHACQ_MAX_NUM_BEST_CELLS 2
#if 0
#define TDSSRCHACQ_HIGH_THRESH_SHALLOW -85
#define TDSSRCHACQ_LOW_THRESH_SHALLOW -105
#define TDSSRCHACQ_HIGH_THRESH_DEEP -90
#define TDSSRCHACQ_LOW_THRESH_DEEP -106
#endif
extern tdsl1_acq_cell_db_threshold_params_type tdsl1_acq_cell_db_threshold;
#define TDSSRCHACQ_HIGH_THRESH_SHALLOW  tdsl1_acq_cell_db_threshold.tdssrchacq_high_threshold_shallow
#define TDSSRCHACQ_LOW_THRESH_SHALLOW   tdsl1_acq_cell_db_threshold.tdssrchacq_low_threshold_shallow
#define TDSSRCHACQ_HIGH_THRESH_DEEP     tdsl1_acq_cell_db_threshold.tdssrchacq_high_threshold_deep
#define TDSSRCHACQ_LOW_THRESH_DEEP      tdsl1_acq_cell_db_threshold.tdssrchacq_low_threshold_deep

#endif

typedef enum
{
  TDSSRCHACQ_STATE_NONE,
  TDSSRCHACQ_STATE_SHALLOW,
  TDSSRCHACQ_STATE_DEEP
} tdssrchacq_state_enum_type;

typedef enum
{
  TDSSRCHACQ_STEP_INACTIVE,
  TDSSRCHACQ_STEP_STEP0,
  TDSSRCHACQ_STEP_STEP1,
  /*for G2T ACQ,step2 means blind PCCPCH or S1/S2+PCCPCH*/
  TDSSRCHACQ_STEP_STEP2
} tdssrchacq_step_enum_type;

#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
typedef struct
{
  uint16    num_valid_cpid;
  struct 
  {
    uint8 cpid;
    uint8 rf_chan_idx;
    uint16 freq;
    int16 rscp;
  } cell[TDSSRCHACQ_MAX_NUM_BEST_CELLS];
} tdssrchacq_best_cells_type;
#endif

typedef struct
{
  /* Acquisition mode specified by L3 */
  tdsl1_acq_mode_enum_type      mode;

  boolean lfs_done;

  /* When this parameter is TRUE, L1 should only verify if the cell
  ** freq_list[0] and cpi can be acquired. If it is FALSE, the cpi
  ** field is ignored by L1.
  */
  boolean                   single_cell;

  /* only used when single_cell is true */
  uint8                     cpid;

  /* The current acquisition mode, none/deep/shallow */
  tdssrchacq_state_enum_type    curr_state;

  /* The current acquisition stage, step 0 or step 1 */
  tdssrchacq_step_enum_type     curr_step;

  /* RF channel list */
  #ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
  uint16                        rf_chan_list[TDSL1_MAX_ACQ_CHANNEL_NUM + TDSSRCHACQ_MAX_NUM_BEST_CELLS];
  #else
  uint16                        rf_chan_list[TDSL1_MAX_ACQ_CHANNEL_NUM];
  #endif
  /* Number of RF channels in ACQ request */
  uint8                         num_rf_chan;
  /* The index to the current "active" RF channel */
  uint8                         rf_chan_idx;

  /* Step0 request and response */
  tfw_srch_gap_detect_cmd_t               step0_req;
  tfw_srch_gap_detect_result_t    step0_rsp;

  /* Step1 request and response */
  tfw_srch_syncdl_midamble_detect_cmd_t   step1_req;
  //tfw_srch_sync_mid_det_result_t  step1_rsp;
  tfw_srch_syncdl_midamble_detect_rsp_t step1_rsp;

  /* Flag indicates whether step0+step1 succeeds or not.     
     This variable is for debug/testing purposes    */
  boolean                       status;

  boolean                       acq_rpt_sent_to_RRC;

  boolean                       do_meas;

  boolean                       ignore_meas_rsp;

  boolean                       is_irat;

  /* Let [-Fmax, Fmax] be the frequency range to search over,     
     where Fmax is the only input of the binning selection algorithm  
     Note that Fmax is obtained as RelativeErrorMax * NominalCarrierFrequency.  
     For example, if RelativeErrorMax is 10ppm and NominalCarrierFrequency is 2GHz, we get Fmax=20kHz.  
   */    
  uint32                         fmax;
  uint32                         freq_offset_idx;
  uint8                          num_barred_cell;
  uint8                          lna_state;
#ifdef FEATURE_TDSCDMA_ANTENNA_SWITCH_DIVERSITY
//Indication of current Ant during S0
//FALSE ==> ANT 0 TRUE ==> ANT 1
tfw_srch_gap_detect_result_t   asd_acq_s0_result;  
tfw_srch_detected_cell_t	   asd_acq_s1_best_cell_result;
boolean 					   asd_acq_s0_only_one_ant_available;
#endif

boolean                      postpone_cleanup;

#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
tdssrchacq_best_cells_type best_cells;
boolean measurement_done;
boolean reconfirm_ongoing;
uint8   last_good_rf_chan_idx;
uint8   last_searched_rf_chan_idx;
uint8 recnf_start_chan_idx;
tdsl1_acq_fs_enum_type acq_fs_status;
#endif

}tdssrchacq_struct_type;

extern tdssrchacq_struct_type    tdssrchacq;

/*=========================================================================

 FUNCTION     :tdssrchacq_proc_reacq_rsp
 
 DESCRIPTION  : This function is used to handle TDSCDMA_FW_SRCH_REACQ_RSP from FW.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
   tfw_cell_info_t *cell_info
 
 RETURN VALUE : None. 
       
 SIDE EFFECTS : 
==========================================================================*/
void tdssrchacq_proc_reacq_rsp(tfw_srch_reacq_result_t *reacq_rsp);


/*===========================================================================
FUNCTION     tdssrchacq_is_deep_search

DESCRIPTION:
    
  
DEPENDENCIES
  None.

RETURN VALUE
 TRUE: deep search

SIDE EFFECTS
  None
===========================================================================*/
boolean tdssrchacq_is_deep_search(void);

/*===========================================================================

FUNCTION tdssrchacq_get_acq_mode

DESCRIPTION
  This function reports acqusition mode

DEPENDENCIES
  None.

RETURN VALUE
  tdsl1_acq_mode_enum_type:  ACQ_MODE
  

SIDE EFFECTS
  None.

===========================================================================*/
extern tdsl1_acq_mode_enum_type tdssrchacq_get_acq_mode(void);

#ifdef FEATURE_TDSCDMA_ACQ_ENHANCEMENT
void tdssrchacq_calculate_best_cells(uint8 freq_index);
void tdssrchacq_best_cells_init(void);
void tdssrchacq_reconfirm_acq(uint8 rf_chan_idx);
boolean tdssrchacq_check_good_cell_to_report(void);
void tdssrchacq_prune_acq_list(void);
void tdssrchacq_append_one_freq_to_acq(void);
boolean tdssrchacq_check_freq_exists_in_acq(uint16 freq);
void tdssrchacq_pop_cell_from_db(uint16 freq, boolean all_cell_in_freq);
#endif

#endif /* TDSSRCHACQ_H */
