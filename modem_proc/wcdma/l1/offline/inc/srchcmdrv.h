#ifndef SRCHCMDRV_H
#define SRCHCMDRV_H

/*============================================================================
                            S R C H C M D R V . H
DESCRIPTION

Copyright (c) 2000-2011 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/srchcmdrv.h#4 $
$DateTime: 2020/01/24 00:26:38 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------
01/23/20   hs      Assigning proper value to SRCHCM_MAX_NUM_PSC_ACQ_SRCH_PER_GAP
06/18/15   gn      UT compilation fix for wcdma.mpss.7.2.0
06/11/15   mk      Updated thresholds for IRAT searches (Applicable only to Jolokia/tobasco modem) 
12/05/14   cc      Reduce array size of unused elements
01/22/15   mm      IRAT globals re-organization for W+W
01/07/15   db      Unification of W<->G IRAT driver for WCDMA PCH, DCH and FACH states
08/04/14   rs      Fixed compiler warnings
06/19/14   cc      IRAT DR-DSDS check in
05/30/14   dp      W2W restructure with mult step1 peak search in pn-acq gaps
05/29/14   mm      Update IRAT/CM driver initialization and RxLM buffer allocation call flow
04/07/14   cc      Protecting shared global variables
04/02/14   mm      3C IRAT/CM support
12/20/13   dp      Allow partial task completion from FW for DSDA
12/11/13   mm      3C CM/IRAT support
10/15/13   mm      Initial FE-FACH check-in
09/30/13   cc      DBDC IRAT support check in
09/12/13   jd      Searcher redesign
12/17/12   mm      Interface changes for passing AS ID to GL1 functions
12/11/12   cc      0x417A log pacekt improvement
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
07/05/12   dp      Made it so W2W cleanup waits for FW searches to finish
12/08/11   dp      Change search tasks scheduled in CM with different
                   offline clock speeds.  Also fixed tabs.
11/11/11   yh      Added more GSM search states to allow concurrent searches in CM
10/26/11   yh      Fix calculation of available search window in a gap
09/12/11   yh      Add function to calculate number of pwr meas
09/02/11   dp      Fixed FW task math macros so that math is done in int32 type
                   instead of uint32 type, which was causing a bug.
05/26/11   dp      Added changes to make tuning time value dynamic
04/14/10   vsr     Enhancements to reduce IF false alarms
03/30/10   rvs     IRAT sample-server config changes for Chetak and
                   general clean-up.
08/31/09   yh      Change step1 thres for NN 30 to -23.7db (70 linear)
08/26/09   yh      Support for Interf searches when in low offline clock
06/23/09   yh      Make interF NN to be 30 instead of 45.
05/13/09   yh      Change step1 threshold to -24.5db because NN is 45, not 30.
04/20/09   yh      Compiler warning fixes.
03/27/09   yh      Modify SRCHCM_PN_TASKS_PER_SLOT to accommodate change 
                   in wcdma offline clk for cx28
01/21/09   sv      Fixed compile error.
01/20/09   sv      Updated PN search thresholds for CM.
11/13/08   yh      Add CM support
10/03/08   hk      Bringing in 7k Mailine fixes
06/08/07   yh      Set step1 results to saturation if exceeds 16 bits.
10/11/06   yh      Added Interf. rescheduling when DM is delayed
09/19/06   yh      Change PSC ACQ early abort threshold to -20dB.
08/02/06   yh      Added DM delay check.
04/05/06   bbd     Added frequency offset to BSIC id parm and BSIC reconf struct
01/16/06   gs      Updated RFAPI calls from common header file
12/05/05   bbd     Added support for CM STTD searches.
04/12/05   rc      Reverted back the SRCHCM_LIST_SRCH_EGY_THRESH and 
                   SRCHCM_LIST_SRCH_EARLY_ABORT_EGY_THRESH values. 
01/27/05   cpe     Updated a few search parameters based on latest memo
10/06/04   gs/bbd  Make SRCHCM_LIST_SRCH_WINSIZE dependent on length of gap.
                   Shorten SRCHCM_INTERF_TUNE_TIME_CX8 to 29184 (950 micro sec).
                   Modify SRCHCM_LIST_SRCH_MAX_TASK_1_NN to accommodate more
                   list search tasks per gap.
                   Modify srchcm_list_srch_set_srch_params() and 
                   srchcm_list_srch_for_psc_peaks_set_srch_params() to accept 
                   gap length in slots.
09/20/04   gs      Disabled code under FEATURE_SRCHCM_NO_SRCH_TILL_RESULT_PENDING
09/13/04   gs      Removed var srchcm_interf_step1_log_struct_in_use
                   Changed step 1 buffer to array that is accessed using queue
                   index of step 1 search used.
                   Function srchcm_step1_cmd now returns queue index used.
07/21/04   gs      Enabled code under FEATURE_SRCHCM_NO_SRCH_TILL_RESULT_PENDING
05/10/04   gs      Redid the macro definition SRCHCM_LIST_SRCH_MAX_TASK_1_NN.
                   With existing definition it was using worst case F/W trigger
                   time (assuming all tasks are triggered) to compute the number
                   of tasks doable. With number of max task increased to 16 it
                   was resulting in no search schedulable in gap of 7 slots.
04/08/04   gs      Moved macro SRCHCM_GSM_SCH_DEC_CMD_MAX_PER_GAP from file
                   srchcmmeas.h to this file.
                   Added structure member extra_qs_uncertainty_win to BSIC
                   id/reconf SCH decode parameter structure.
                   Changed prototype of function
                   srchcm_update_gsm_bsic_id_sch_dec_cmd_params
02/19/04   gs      Removed reference to FEATURE_SRCH_BITWIDTH_MULTI_FACTOR
                   and code under that.
01/16/04   gs      Changed PSC peak separation to 4 Cx1 (32 Cx8)
12/16/03   gs      Support for GSM Quadband
11/11/03   yus     Replace FEATURE_6250_COMPILE with FEATURE_SRCH_BITWIDTH_MULTI_FACTOR
11/06/03   gs      Removed duplicate declaration of 2 functions.
10/24/03   gs      Updated functionality for compressed mode inter-freq
                   search under FEATURE_CM_FOR_INTER_FREQUENCY_MEASUREMENTS
07/24/03   yus     Add support for MSM6250.
08/09/02   gs      Created this file
===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "comdef.h"
#include "mdspsync.h"
#include "wl1rfmif.h"
#include "srchparm.h"
#include "srchgsmdrv.h"
#include "srchcmmeasparms.h"
#include "l1mathutil.h"
#include "mcalwcdma_srch.h"
#include "l1interflog.h"
#include "mcalwcdma_cm.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* WCDMA inter frequency search related macro */
/* ------------------------------------------ */
  /* CM search related parameters */
  /* ---------------------------- */
/* Theoretical num of Max tasks that can be scheduled for each search 
 * using a lowest 72MHz clock rate 
 */
#define SRCHCM_PN_SRCH_MAX_TASK 64
/* Max mum of PSC that can be scheduled per gap */
#define SRCHCM_MAX_NUM_PSC_ACQ_SRCH_PER_GAP 32
/* Number of PN search peaks to be reported by mDSP */
#define SRCHCM_NUM_PN_PEAKS_PER_TASK 6
/* Non coherent integration length to cover in step 1 substaks */
#define SRCHCM_STEP1_SRCH_NON_COH_INT_LEN 30
#ifdef FEATURE_WCDMA_TABASCO_MODEM
/* Step1 energy threshold -25.0  for SRCHCM_STEP1_SRCH_NON_COH_INT_LEN = 30 */
#define SRCHCM_STEP1_EGY_THRESH 47
/* Step1 energy threshold -10.5 dB for SRCHCM_STEP1_SRCH_NON_COH_INT_LEN = 30 */
#define SRCHCM_STEP1_PEAK_ENERGY_THRESH_MED 796
/* Step1 energy threshold -8.5 dB for SRCHCM_STEP1_SRCH_NON_COH_INT_LEN = 30 */
#define SRCHCM_STEP1_PEAK_ENERGY_THRESH_HIGH 1260
#else
/* Step1 energy threshold -23.7 for SRCHCM_STEP1_SRCH_NON_COH_INT_LEN = 30 */
#define SRCHCM_STEP1_EGY_THRESH 70
/* Step1 energy threshold -10 dB for SRCHCM_STEP1_SRCH_NON_COH_INT_LEN = 30 */
#define SRCHCM_STEP1_PEAK_ENERGY_THRESH_MED 894
/* Step1 energy threshold -8 dB for SRCHCM_STEP1_SRCH_NON_COH_INT_LEN = 30 */
#define SRCHCM_STEP1_PEAK_ENERGY_THRESH_HIGH 1397
#endif

/* minimum step 1 PSC peak to LIST search PN pos PSC position seperation required
   This is refered to as D-chips in systems memo */
#define SRCHCM_STEP1_LIST_SRCH_PEAK_PSC_PN_SEP_CX8 32

/* CM PN search parameters */
/* coherent int len (31 + 1) * 64 = 2048 Cx1 */
#define SRCHCM_PN_SRCH_NC               31
#define SRCHCM_PN_SRCH_NN               0
#define SRCHCM_RECONF_SRCH_NN           1

/* Index into table to determine no TD threshold. Idx 44 means -22db */
#define SRCHCM_PN_SRCH_NO_TD_THRESH_IDX 44
/* Index into table to determine TD threshold. Idx 42 means -21db */
#define SRCHCM_PN_SRCH_TD_THRESH_IDX    42
/* Look up table */
#define SRCHCM_PN_SRCH_EGY_THRESH(sttd) ((sttd) ? SRCH_GET_PN_ENG(SRCHCM_PN_SRCH_TD_THRESH_IDX): \
                                                  SRCH_GET_PN_ENG(SRCHCM_PN_SRCH_NO_TD_THRESH_IDX))
                                                  
/* Early abort threshold for no TD search is 40 or -20db */
#define SRCHCM_PN_SRCH_EARLY_ABRT_NO_TD_THRESH_IDX 40
/* Early abort threshold for TD search is 39 or -19.5 db */
#define SRCHCM_PN_SRCH_EARLY_ABRT_TD_THRESH_IDX 39
/* Look up table */
#define SRCHCM_PN_SRCH_EARLY_ABRT_EGY_THRES(sttd) ((sttd) ? SRCH_GET_PN_ENG(SRCHCM_PN_SRCH_EARLY_ABRT_TD_THRESH_IDX): \
                                                            SRCH_GET_PN_ENG(SRCHCM_PN_SRCH_EARLY_ABRT_NO_TD_THRESH_IDX))

/* Threshold for detected pn noise floor (Reconfirmation) */
/* Index into table to determine TD threshold. Idx 44 means -22db */
#define SRCHCM_STTD_DETECTED_PN_SRCH_NOISE_FLOOR_IDX_LOW 44
/* Index into table to determine TD threshold. Idx 42 means -21db */
#define SRCHCM_STTD_DETECTED_PN_SRCH_NOISE_FLOOR_IDX_MED 42
/* Index into table to determine TD threshold. Idx 40 means -20db */
#define SRCHCM_STTD_DETECTED_PN_SRCH_NOISE_FLOOR_IDX_HIGH 40
/* Index into table to determine Non TD threshold. Idx 46 means -23db */
#define SRCHCM_NOTD_DETECTED_PN_SRCH_NOISE_FLOOR_IDX 46
/* Look up table */
#define SRCHCM_DETECTED_PN_SRCH_NOISE_FLOOR(sttd, idx) SRCH_GET_ENG_FRM_ECIO_LUT(sttd,idx)

  /* CM search state machine related macros */
  /* -------------------------------------- */
/* |----------------|_________________//________________|----------------
                    <---> 1024 Chips Tx continue
                                                    <---> 1024 chips possible Rx start
                         <---> Interf RF Tune in
                              <---> Sample collection
                                   <---------------> offline searches
                                               <---> Interf RF Tune out
                                               (tuning can occur in parallel with offline searches)
                              <======//=======> Usable interf search gap */
#define SRCHCM_INTERF_USABLE_GAP_CX8(num_gap_slots) \
  (((num_gap_slots) * CHIP_PER_SLOT * 8) - (1024 * 8 * 2) \
   - ((int32)mcalwcdma_cm_ext_cntrl_params[wsub_id].mcalwcdma_cm_info.w2w_tune_away_time_cx8)  \
   - ((int32)mcalwcdma_cm_ext_cntrl_params[wsub_id].mcalwcdma_cm_info.w2w_samples_collection_time_cx8))
   /* note: 1) casted as int32 so arithmetic happens as integers, not unsigned
            2) Interf RF Tune out is not considered. Instead, WFW needs time
               to collect the samples. Samples processing can be used when
               RF is tuned back.
   */

/* Number of slots usable for an interf measurement */
#define SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots) \
  (SRCHCM_INTERF_USABLE_GAP_CX8(num_gap_slots) / (8 * CHIP_PER_SLOT))

#define SRCHCM_STEP1_NN_IN_GAP(num_gap_slots) (SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots))

/* Number of PCS peaks to process in list serch tasks */
#define SRCHCM_MAX_STEP1_PEAKS_TO_PROC 32

/* How much stuff can be computed in one slot's worth of time at a given clock 
   speed. Higher clock speeds give higher numbers because more stuff can be 
   accomplished in one slot's worth of time).  Not sure what the units are. 
   Used for calculating number of tasks we can schedule per slot.*/
#define SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_L_CLK  1440
#define SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_M_CLK  2160
#define SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_H_CLK  2700

/* PN SEARCH TASK SCHEDULING DEFINES AND MACROS */
#define SRCHCM_PN_SRCH_WINSIZE_CX1 (192)
#define SRCHCM_COST_PER_WFW_PN_SRCH_TASK (223)
#define SRCHCM_PN_SRCH_MAX_TASK_1_NN_LOW_CLK(num_gap_slots) \
  (SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots) * \
  (FLOOR(SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_L_CLK, SRCHCM_COST_PER_WFW_PN_SRCH_TASK)))
#define SRCHCM_PN_SRCH_MAX_TASK_1_NN_MED_CLK(num_gap_slots) \
  (SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots) * \
  (FLOOR(SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_M_CLK, SRCHCM_COST_PER_WFW_PN_SRCH_TASK)))
#define SRCHCM_PN_SRCH_MAX_TASK_1_NN_HIGH_CLK(num_gap_slots) \
  (SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots) * \
  (FLOOR(SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_H_CLK, SRCHCM_COST_PER_WFW_PN_SRCH_TASK)))

/* RECONF SEARCH TASK SCHEDULING DEFINES AND MACROS */
#define SRCHCM_RECONF_GAP_SIZE_CUTTOFF_FOR_WINSIZE 7  /* in units of slots */
#define SRCHCM_RECONF_NORMAL_WINSIZE_CX1 (192)
#define SRCHCM_RECONF_SMALL_WINSIZE_CX1 (128)
#define SRCHCM_RECONF_SRCH_WINSIZE_CX1(num_gap_slots) \
  (((num_gap_slots) <= SRCHCM_RECONF_GAP_SIZE_CUTTOFF_FOR_WINSIZE) ? \
   SRCHCM_RECONF_SMALL_WINSIZE_CX1 : SRCHCM_RECONF_NORMAL_WINSIZE_CX1)
#define SRCHCM_COST_PER_WFW_RECONF_SRCH_TASK(winsize) (((winsize) == SRCHCM_RECONF_NORMAL_WINSIZE_CX1) ? (415) : (282))
#define SRCHCM_RECONF_SRCH_MAX_TASK_2_NN_LOW_CLK(num_gap_slots) \
  (SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots - 1) * \
  (FLOOR(SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_L_CLK, SRCHCM_COST_PER_WFW_RECONF_SRCH_TASK(SRCHCM_RECONF_SRCH_WINSIZE_CX1(num_gap_slots)))))
#define SRCHCM_RECONF_SRCH_MAX_TASK_2_NN_MED_CLK(num_gap_slots) \
  (SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots - 1) * \
  (FLOOR(SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_M_CLK, SRCHCM_COST_PER_WFW_RECONF_SRCH_TASK(SRCHCM_RECONF_SRCH_WINSIZE_CX1(num_gap_slots)))))
#define SRCHCM_RECONF_SRCH_MAX_TASK_2_NN_HIGH_CLK(num_gap_slots) \
  (SRCHCM_INTERF_USABLE_GAP_SLOTS(num_gap_slots - 1) * \
  (FLOOR(SRCHCM_COMP_UNITS_AVAIL_PER_SLOT_H_CLK, SRCHCM_COST_PER_WFW_RECONF_SRCH_TASK(SRCHCM_RECONF_SRCH_WINSIZE_CX1(num_gap_slots)))))

/* The list search non coherent length */
#define SRCHCM_PN_SRCH_NON_COH_INT_LEN ((SRCHCM_PN_SRCH_NN) + 1)

/* The Reconf search non coherent length */
#define SRCHCM_RECONF_SRCH_NON_COH_INT_LEN ((SRCHCM_RECONF_SRCH_NN) + 1)

/* Number of PCS hypothesis to cover for list search. This is the number
   of slots in a frame */
#define SRCHCM_LIST_SRCH_MAX_PSC_HYPO 15

/* Num of PN peaks per PSC. each PSC has a max 15 tasks, each task has 6 peaks */
#define SRCHCM_NUM_PN_PEAKS_PER_PSC (SRCHCM_LIST_SRCH_MAX_PSC_HYPO) * (SRCHCM_NUM_PN_PEAKS_PER_TASK)

  /* CM search driver related macros */
  /* ------------------------------- */
/* Non coherent search state for a search sub-task. */
#define SRCHCM_SRCH_NON_COH_INT_START 0x01
#define SRCHCM_SRCH_NON_COH_INT_CONT  0x02
#define SRCHCM_SRCH_NON_COH_INT_END   0x04

/* Maximum STEP 1 search peaks reported */
#define SRCHCM_STEP1_MAX_PEAKS SRCH_MAX_NUM_STEP1_PEAKS
  
#ifndef MAX_NUM_ANTENNA
#define MAX_NUM_ANTENNA 2
#endif

#define SRCHCM_MAX_PILOT_PEAKS SRCH_PN_NUM_RESULT_PER_TASK

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* CM search state machine related enum definitions */
/* ------------------------------------------------ */
typedef enum
{
  /* "0x00": Peak Detection enabled. */
  SRCHCM_PD_NORMAL,
  /* "0x01": Peak Detection Disabled. */
  SRCHCM_PD_BYPASS,
  /* "0x02": used for DRX mode search. */
  SRCHCM_PD_SHOULDER
} srchcm_pd_enum_type;

/* Alignment for coherent integration */
typedef enum
{
  /* "0x00": No alignment needed */
  SRCHCM_ALIGN_NONE,
  /* "0x01": 256-chip alignment */
  SRCHCM_ALIGN_256,
  /* "0x02": 512-chip alignment */
  SRCHCM_ALIGN_512,
  /* "0x03": 1024-chip alignment */
  SRCHCM_ALIGN_1024
} srchcm_align_enum_type;

typedef void (*SRCHCM_SRCH_DONE_CB_FUNC_TYPE)(uint8, wsub_id_e_type);

typedef void SRCHCM_CLEANUP_CB_FUNC_TYPE(void);


/* CM search driver related structure definitions */
/* ---------------------------------------------- */
/* This structure defines the PCS peak parameters */
typedef struct
{
  /* Step 1 PCS prak energy */
  uint16 egy;
  /* step 1 PCS peak position 0..2560x8 */
  uint16 pcs_pos_cx8;
  /* indicate if this peak has got the cell identified in list search */
  boolean cell_exists;
} srchcm_step1_srch_peak_struct_type;

/* This structure defines the Step 1 peak result */
typedef struct
{
  /* the number of non coh int length performed in the step1 search */
  uint8 num_nn_completed;
  /* number of peaks reported by step1 result */
  uint8 num_peaks;
  /* number of un identified peaks */
  uint8 num_peaks_no_cell_exists;
  /* Each step 1 peak result */
  srchcm_step1_srch_peak_struct_type peak[SRCHCM_STEP1_MAX_PEAKS];
  /* RRSI at step 1 done */
  int16 rssi_at_step1_dbmx10;
} srchcm_step1_srch_result_struct_type;

/* This structure defines the list search peak result parameters */
typedef struct
{
  /* PSC index scheduled to the search to determine the PSC of the each peak */
  uint8 psc_index;
  /* pn search peak energy */
  uint16 egy;
  /* PN position for pn search task */
  uint32 pn_pos_cx8;
} srchcm_pn_srch_peak_struct_type;

/* This structure defines all PN search peak result */
typedef struct
{
  /* number of the tasks finished in the PN search */
  uint8 num_tasks;
  /* result of peak parameter for all tasks. Each tasks report the
     number of peaks as per the async parameter programed in to mDSP */
  srchcm_pn_srch_peak_struct_type peak[SRCHCM_PN_SRCH_MAX_TASK][SRCHCM_MAX_PILOT_PEAKS];
  srchcm_pn_srch_peak_struct_type peak_div[SRCHCM_PN_SRCH_MAX_TASK][SRCHCM_MAX_PILOT_PEAKS];
  boolean  srch_result_valid[MAX_NUM_ANTENNA];
} srchcm_pn_srch_result_struct_type;

typedef struct
{
  /* Result from the CM list search (PSC_ACQ/RECONF LIST SRCH)  */
  srchcm_pn_srch_result_struct_type srchcm_pn_srch_result;
  /* CM step 1 search results pointer. Pointer must be set to valid
   result buffer for a given frequency before starting step 1 search */
  srchcm_step1_srch_result_struct_type *srchcm_step1_srch_result;
  /* Save GSM band info to be searched for WRM to query RF resource */
  sys_band_class_e_type srchcm_gsm_band_info_list[MCALWCDMA_MAX_NUM_BAND];
  /* Call back function for CM step 1 result processing. This can
   be set to NULL if no processing is required */
  SRCHCM_SRCH_DONE_CB_FUNC_TYPE srch_cm_step1_done_cb_func[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  /* List search buffer index for each Q. Given Q index it stores the
   buffer index in list search log packet array */
  uint8 srchcm_interf_list_srch_log_pkt_buf_idx[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  /* List search log packet pointer for each command/result Q0 and Q1 */
  L1_WCDMA_INTERF_LIST_SRCH_LOG_PKT_type *srchcm_interf_list_srch_log_pkt[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  /* This is the call back function for list search result processing.
   This can be set to NULL if not processing is required */
  SRCHCM_SRCH_DONE_CB_FUNC_TYPE srch_cm_list_srch_done_cb_func[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  /* The cb srchcmdrv should call in cleanup scenarios where srchcmmeas is 
   waiting for all configured/ongoing searches in FW to be done.  Whether this
   is NULL or not also determines whether srchcmmeas is waiting.  (This is set 
   directly by srchcmmeas. */
  SRCHCM_CLEANUP_CB_FUNC_TYPE *srchcm_cleanup_pending_cb;
  /* List search buffer in use flag */
  boolean srchcm_interf_srch_log_pkt_in_use[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  /* There are 2 list search log packets for compressed mode Q0 and Q1.
   This points to current list search log packet while sending commands
   to mDSP. */
  L1_WCDMA_INTERF_LIST_SRCH_LOG_PKT_type *srchcm_interf_cur_list_srch_log_pkt;
  /* Changes for memory reduction using ZI->MemHeap */
  L1_WCDMA_INTERF_LIST_SRCH_LOG_PKT_type *srchcm_interf_list_srch_log_pkt_buf_ptr[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  /* number of peaks configured per task for the PN srch.
   for pd=normal --->  6 peaks (non RxD,combined)  12 peaks (RxD separate)
   for pd=shoulder --->  6*3 peaks (non RxD,combined)  12*3 peaks (RxD separate)    
*/
  uint16 srchcm_pn_peaks_per_task;
  /* step 1 log packet */
  L1_WCDMA_INTERF_STEP1_SRCH_LOG_PKT_type srchcm_interf_step1_log_pkt[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
}srchcmdrv_ext_cntrl_params_type;

extern srchcmdrv_ext_cntrl_params_type srchcmdrv_ext_cntrl_params[WCDMA_NUM_SUBS];


typedef struct
{
  timer_type srchcm_step1_debug_timer[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  mcalwcdma_srch_cm_step1_sync_parms_struct_type srchcm_step1_sync_parms;
  mcalwcdma_srch_cm_pn_result_buffer_type srchcm_pn_result_buf;
  timer_type srchcm_pn_debug_timer[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  mcalwcdma_srch_cm_pn_sync_parms_struct_type srchcm_pn_sync_parms;
  /*--------------------------------------------------------------------------
              The mDSP async search parameter blocks
--------------------------------------------------------------------------*/
  mcalwcdma_srch_cm_step1_async_parms_struct_type srchcm_step1_async_parms;
  mcalwcdma_srch_cm_pn_async_parms_struct_type srchcm_pn_async_parms;
  mcalwcdma_srch_cm_step1_result_buffer_type srchcm_step1_result_buf;
  /* This variable indicate the number of list search task current
   programmed via last CM list search command */
  uint8 srchcm_num_pn_srch_task[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  boolean srch_cm_list_srch_in_progress[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
  /* variables used to track whether there are individual gap searches still in 
   progress.  This information is useful at cleanup-time as we can't claim W2W
   searches are cleaned up until FW finishes processing all ongoing searches. */
  boolean srch_cm_step1_srch_in_progress[MCALWCDMA_SRCH_CM_MAX_NUM_CMQ];
}srchcmdrv_int_cntrl_params_type;

/* =======================================================================
**                        Function Declarations
** ======================================================================= */

extern void srchcmdrv_int_cntrl_params_init();
extern void srchcmdrv_ext_cntrl_params_init();
 
/*===========================================================================
FUNCTION srchcm_state_init

DESCRIPTION
  This function initializes the CM state machine state related variables.
  For inter frequency search it does the following:
  - Clear the search paneind flag.
  - Set the non coherent length for step 1 done to 0
  - Set the step 1 and list search done call back function to NULL
  - Set the inter frequency search state to IDLE
  - Initialize the mDSP async writes for step 1 search energy threshold
    and list search peaks.

  Note that list search related variables are initialized when inter frequency
  search state transitions from STEP1_DONE to LIST SEARCH
  This function should be called at the begining of DCH state.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcm_drv_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_async_parm_init

DESCRIPTION
  This function initializes the async parameters for step1/pn search
  to be issued for inter freq search i.e using CM queues 0 and 1

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcm_async_parm_init( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_step1_cmd

DESCRIPTION
  This function writes the asynchronous variables to mDSP for CM step 1 search.
  It also sets the Step 1 done call back function and then sends the CM step 1
  command to mdsp sync interface.

DEPENDENCIES
  None

RETURN VALUE
  Q index used for step 1

SIDE EFFECTS
  None
===========================================================================*/
extern uint8 srchcm_step1_cmd(
  /* non coherent int length */
  uint8 non_coh_int_len,
  /* non coherent int state */
  uint8 non_coh_int_state_bf,
  /* call back function for step 1 done */
  SRCHCM_SRCH_DONE_CB_FUNC_TYPE step1_done_handler, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_list_srch_set_srch_params

DESCRIPTION
  This function sets various list search parameters based on the input
  parameters passed in function argument. This is for list search peaks.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void srchcm_pn_srch_set_srch_params(
  /* The search window size of the task, in chipx1 */
  uint16 search_winsize_cx1,
  /* number of list search task */
  uint8 num_pn_search_task,
  /* cell scrambling code list */
  uint16 *scr_code,
  /* PCS position from step1 result */
  uint32 *pn_pos_cx8,
  /* list search STTD enable */
  boolean *pn_srch_sttd,
  /* non coherent accumulation start.end state bit field */
  uint8 pn_srch_non_coh_int_state_bf,
  /* Non Coherent Integ length */
  uint16 nn_int_len, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_pn_srch_cmd

DESCRIPTION
  This function writes the list search parameter to mDSP. To do this it gets
  the free Q for list seqrch parameters. then it writes the parameters to this
  Q and then sends the CM list search command to mdsp sync interface.

DEPENDENCIES
  None

RETURN VALUE
  Q index used for list search

SIDE EFFECTS
  None
===========================================================================*/
extern uint8 srchcm_pn_srch_cmd(
  /* list search done call back function */
  SRCHCM_SRCH_DONE_CB_FUNC_TYPE pn_srch_done_handler, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_get_step1_srch_result

DESCRIPTION
  This function gets the CM step 1 results from the result Q buffer passed as
  function argument and save them in global variable srchcm_step1_srch_result.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcm_get_step1_srch_result(
  /* CM result Q index */
  uint8 step1_queue_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_get_list_srch_result

DESCRIPTION
  This function gets the CM list search results from the result Q buffer passed
  as function argument and save them in global variable srchcm_list_srch_result.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcm_get_pn_srch_result(
  /* CM result Q index */
  uint8 pn_queue_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCM_MCAL_WHL1_STEP1_SRCH_STATUS_CB

DESCRIPTION:
  - callback function registered with MCAL and will be called by MCAL
     - on CMD_DONE interrupt
     -on SRCHCM_DONE interrupt


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcm_mcal_whl1_step1_srch_status_cb(
  /* Status returned by mDSP */
  mcalwcdma_srch_status_type status,
  /* Queue in which this callback is coming for */
  uint8 queue_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_MCAL_WHL1_PN_SRCH_STATUS_CB

DESCRIPTION:
  - callback function registered with MCAL and will be called by MCAL
     - on CMD_DONE interrupt
     -on SRCHCM_DONE interrupt


DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void srchcm_mcal_whl1_pn_srch_status_cb(
  /* Status returned by mDSP */
  mcalwcdma_srch_status_type status, 
  /* Queue in which this callback is coming for */
  uint8 queue_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_interf_get_rssi_dbmx10

DESCRIPTION
  This functin calculates the RSSI inside the CM Gap in dBm10 for Taxis.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern int16 srchcm_interf_get_rssi_dbmx10( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION srchcm_interf_query_cfgd_srchs_still_ongoing

DESCRIPTION
  This function indicates whether individual (per-gap) searches are still 
  configured and ongoing in FW.  Currently it's intended for use during cleanup
  of W2W module, as a way to tell when it's safe to say W2W is cleaned up.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicating whether configured searches are still in progress

SIDE EFFECTS
  None
===========================================================================*/
extern boolean srchcm_interf_query_cfgd_srchs_still_ongoing(wsub_id_e_type wsub_id);

#ifdef FEATURE_CM_UNIT_TEST
#error code not present
#endif

#endif /* SRCHCMDRV_H */

