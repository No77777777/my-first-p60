#ifndef SRCHINTERF_H
#define SRCHINTERF_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            S R C H I N T E R F . H                  

GENERAL DESCRIPTION
  This module handles processing of measurement requests information from RRC
  for inter-frequency neighbors.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2003-2006 by Qualcomm Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchinterf.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/02/10   stk     Added support for W2LTE measurements and reselection under 
                   FEATURE_TDSCDMA_ABSOLUTE_PRIORITY & FEATURE_TDSCDMA_TO_LTE
05/27/10   sa      Modified the scope of variables to support feature
                   FEATURE_TDSCDMA_MODEM_STATISTICS.
06/26/09   sa      Change the TDSSRCHINTERF_MAX_INTERF_MEAS from 7 to 8.
04/22/09   stk     Compiler warning fixes	
10/03/08   hk      Bringing in 7k Mailine fixes
08/23/06   rc      Added support for cells for measurement and virtual active
                   set under FEATURE_WCDMA_VASET_SUPPORT.
08/22/06   rc      Added declaration for tdssrchinterf_update_cell_list_in_dch_init().
11/11/05   sk      Introduced interf saved set to hold sync information 
                   across states
08/26/05   nd      added a flag "stopped" in the meas db to indicate stopped 
                   measurements after inter freq HHO
07/22/05   vp      Changed the FACH state k-value for inter-freq filter
                   from 2 to 1. This should converge the filtered energy
                   faster to the last measured energy
06/20/06   sk      Added cell bar flag to interf cell structure
06/14/05   rc      Added support for multiple 2A events.
06/10/05   vp      Code modifications to filter inter-freq measurements
04/06/05   ub/ba   HCS based cell reselection feature (FEATURE_TDSCDMA_HCS)
01/10/05   src     Correction of a potential feature skew.
12/03/04   cpe     Added a couple of fields to srch_interf_cell_struct_type 
                   for supporting pathloss feature
11/01/04   vb      Extern'ed the function tdssrch_interf_find_cell
07/28/04   rc      Added declaration for tdssrchinterf_fach_add_cells() function
04/28/04   sh      Added field rl_set_idx to srch_interf_cell_struct_type.
04/22/04   cpe     Modified tdssrchinterf_meas_parms_struct_type to account
                   for multiple non used frequencies 
04/06/04   cpe     Exported a couple of functions for usage in other files
12/10/03   cpe     Added a prototype for tdssrch_interf_delete_cell_from_set
12/05/03   cpe     Added a prototype for tdssrchinterf_meas_sib_update
12/01/03   cpe     Added a prototype for tdssrchinterf_remove_all_interf_cells()
11/13/02   src     Removed extra hash-endif to fix compilation problem.
11/12/03   sh      Added some fields for inter-f cell structure for cell
                   reselection.
11/12/03   cpe     Extern'ed a few functions
11/05/03   cpe/src Changed a hash-ifdef to consider both inter-frequency cell
                   re-selection as well as inter-frequency compressed-mode
                   measurements.
10/24/03   cpe     Expanded the support for inter frequency measurements,
                   set maintenance structures, function prototypes etc.
01/21/03   ddh     Created file.

===========================================================================*/

#include "tdscdma_variation.h"

#include "tdssrchgsm.h"
             
/***************************************************************************

    D A T A     S T R U C T U R E S 
    
 ***************************************************************************/
/* Make the number of interf measurement from 7 to 8 because it was required
by some some n/w configurations */
#define TDSSRCHINTERF_MAX_INTERF_MEAS 8
#define TDSSRCHINTERF_NO_INTERF_MEAS_ID (0xFF)
/*--------------------------------------------------------------------------
                       Inter-Frequency MACROS 
--------------------------------------------------------------------------*/

#define TDSSRCHINTERF_DEFAULT_K_DCH 0 /* No filtering*/
#define TDSSRCHINTERF_DEFAULT_K_FACH 1
#define TDSSRCHINTERF_INVALID_K_VAL 0
#define TDSSRCHINTERF_MEAS_PRD_MSECS 480L 
#define TDSSRCHINTERF_MAX_K_VALUE 19
#define TDSSRCHINTERF_VALUES_STORED_PER_K 2
#define TDSSRCHINTERF_INVALID_ALPHA -1
#define TDSSRCHINTERF_INVALID_BETA -1
#define TDSSRCHINTERF_ALPHA_INDEX 0
#define TDSSRCHINTERF_BETA_INDEX 1
#define TDSSRCHINTERF_ALPHA_MULT_FACTOR_16384 16384
#define TDSSRCHINTERF_BETA_MULT_FACTOR_128 128
#define TDSSRCHINTERF_LOG_INTERMEDIATE_MAX 62
#define TDSSRCHINTERF_SCALE_UNIT_1 1
#define TDSSRCHINTERF_LOG_FINAL_MAX 44

#define TDSSRCHINTERF_FILTER_COEFF_VALIDATE(k) \
            ((k>TDSSRCHINTERF_MAX_K_VALUE)||(tdssrchinterf_filter_coef_tab[k][TDSSRCHINTERF_ALPHA_INDEX]==\
            TDSSRCHINTERF_INVALID_ALPHA)) ? FALSE:TRUE
#define TDSSRCHINTERF_FILTER_CALC_NUME(time_since_last_meas) \
            (time_since_last_meas*(int64)TDSSRCHINTERF_BETA_MULT_FACTOR_128)
#define TDSSRCHINTERF_FILTER_CALC_DENO(filt_len_param, time_since_last_meas) \
            ((time_since_last_meas*(int64)TDSSRCHINTERF_BETA_MULT_FACTOR_128)+\
            (uint32)tdssrchinterf_filter_coef_tab[filt_len_param][TDSSRCHINTERF_BETA_INDEX]*\
            (int64)TDSSRCHINTERF_MEAS_PRD_MSECS)
            


#define  TDSSRCHINTERF_OFREQ_FREQ_MAX  11
  /* The max number of TDS inter-freuency channels the mobile searches */

#define  TDSSRCHINTERF_OFREQ_CELL_MAX  32
  /* The max number of cells specified for an inter-frequency WCDMA channel */
  
   

 /*
 * Inter freq Set Management Strategy Explained...
 *
 * -E- All cells begin in the TDSSRCHINTERF_OFREQ_SET_E (Empty) set.
 *     After Allocation they may be removed, at which time they
 *     will return to the empty set.
 * -SYNC_N- Cells are added to the TDSSRCHINTERF_OFREQ_SET_N (Neighbor) when 
 *     position is known.
 *     When monitoring should be performed this is directed
 *     in the Cells for Measurement/meas_cell_list[]
 *     parameter of the measurement control message. 
 * -VA- Cells which are in virtual active set. This is applicable
 *      only in DCH state. Cells are used for frequency quality estimate.
 * -MONITORED- Cells that are in Cells for Measurement or Cell Info List 
 *             are in this Set. These are passed to the drivers for measurement.
 *             VA Set cells are selected from Monitored Set.
 *             Applicable in DCH only.
 * -MEAS_N- All the other frequency cells. Applicable in DCH state. 
 *          Monitored set cells are selected from MEAS_N. 
 */
typedef enum
{
    /* Cell belongs to Inter freq  sync Neighbor Set */
  TDSSRCHINTERF_OFREQ_SET_SYNC_N, 
    /* Cell belongs to Inter freq  async Neighbor Set */
  TDSSRCHINTERF_OFREQ_SET_ASYNC_N,
    /* Cell belongs to Inter freq Virtual Active Set. Applicable in DCH */
  TDSSRCHINTERF_OFREQ_SET_VA, 
   /* Cell belongs to Inter freq  Saved Set. During reselection sync cells saved*/
  TDSSRCHINTERF_OFREQ_SET_SAVED,
    /* Cell belongs to Empty Set */
  TDSSRCHINTERF_OFREQ_SET_E
}
tdssrchinterf_set_enum_type;


/*--------------------------------------------------------------------------
                        INTER FREQ SEARCH CELL STRUCTURE
--------------------------------------------------------------------------*/
typedef struct
{
  /* WCDMA Cell Scrambling Code */
  uint16  scr_code;
    
  /* WCDMA frequency assignment for the cell */
  uint16  freq;
  
  /* TRUE if the CPICH of this cell uses STTD */
  boolean sttd;
          
  /* Which Set the Cell currently belongs to */
  tdssrchinterf_set_enum_type  set;
    
  /* The total energy and Ec/Io in the best paths found in a sweep */
  uint32  tot_eng;
  uint16  ecio;

  uint16  filt_mult_factor;
  /* Filtered energy and Ec/Io and Filter length used currently */
  uint16  path_thresh;
  /* path threshold for this cell */

   uint32  filt_eng;
  uint16  filt_ecio;
  uint16  filt_len;

  /* CPICH RSCP and filtered RSCP */
  uint16  rscp;
    /* It is RxAGC + Filtered Ec/Io. The actual CPICH RSCP is "-rscp" */
  uint16  filt_rscp;
  int16   rssi_dbm;
  /* Flag indicating if the cell has been evaluated */
  boolean evaled;

  /* the index of the radio link set that this cell belongs to */
  uint8 rl_set_idx;

  /* positions of the maximum energy paths in 1/8 chip units. */
  uint32  max_pos[ TDSSRCH_MAX_PILOT_PEAKS ];
    
  /* Energies of the maximum energy paths */
  uint32  max_eng[ TDSSRCH_MAX_PILOT_PEAKS ];

#ifdef FEATURE_TDSCDMA_OFFLINE_SEARCHER
  /* positions of the maximum energy paths in 1/8 chip units. */
  uint32  max_pos_div[ TDSSRCH_MAX_PILOT_PEAKS ];

  /* Energies of the maximum energy paths */
  uint32  max_eng_div[ TDSSRCH_MAX_PILOT_PEAKS ];
 
  /* variable to tell  if it can reliably use results from primary
   * or diversity antenna
   */
 // boolean  srch_result_valid[MAX_NUM_ANTENNA];
#endif
  /* The position of the path used for centering searching window. Normally,
     it is the earliest usable (or above threshold SRCH_PATH_THRESHOLD) path.
     
     Valid value ranges (0..307200) in CHIPx8. 0xFFFFFFFF (or SRCH_NO_REF_POS)
     implies that ref_pos is not being set to a valid value.
   */
  uint32  ref_pos;

  /* The age of the above reference position, in units of search period. 
     
     At time ref_pos is updated with a new search result, ref_age is set to 0.
   */
  int16   ref_age; 
 
  /* Reference Time difference to Serving cell, in chips. 
     0xFFFF indicates the field is not included. This field is 
     either obtained from a SIB message or via mobile measurement.
   */
  uint16  time_diff;
    
  /* Neighbor Cell timing accuracy relative to the serving cell.
     This will be translated to "search window size" for the neighbor.
   */
  tdsl1_cell_time_acc_enum_type time_acc;

  /* flag indicating if the cell's SFN decoding is requested for 
     measurement.
   */
  boolean read_sfn;

  /* Quality offsets to serving cell for cell reselection */
  int16  q_offset1;
  int16  q_offset2;

  /* Minimum qualities */
  int16  qual_min;
  int16  rxlev_min;
  int16  max_tx_pwr;

  /* Cell selection values evaluated by SRCH and to be reported to RRC */
  int16    s_qual;
  int16    s_rxlev;

  int16 cell_offset;
  /*  cell individual offset Refer to 10.3.7.2 and 14.1.5.3 
      for an explanation of this parameter. 
      Used to set invidual threshold for triggering reports 
      Range -20 to +20 in steps of 1 */

  int16 cpich_tx_pwr;
  uint16 pathloss;
    /*  Primary CPICH Tx power. Needed if calculating patchloss
        Units: Int  -10... +50 dBm */

  /* CELL_DCH Intra-frequency Event time stamps */
  tdssrchset_evt_ts_union_type   u_ts;
 
    /* CELL_DCH event rpt forbidden for the cell */
  tdssrchset_evt_barred_union_type  u_bar;

  /* CELL_DCH Intra-freqency event reporting status flag */
  tdssrchset_evt_rpt_union_type  u_rpt;

  /* SFN-CFN time difference structure type. */ 
  tdssrchset_sfn_cfn_tm_diff_struct_type  sfn_cfn_tm_diff;

  /* Search window size for this cell */
  uint32  win_size;

  /* Cell Reselection sorted index */
  uint16   cr_sort_inx;

  /* Cell Reselection search required flag, for weighting searches */
  boolean   cr_srch_reqd;

  /* Cell Reselection time stamp. Enabled when the neighbor cell is
     significantly stronger than the ASET cell.
   */
  tdssrchset_evt_ts_struct_type  evt_cr_ts;

  /* Rank of this cell for cell reselection */
  int16 rank;

#ifdef FEATURE_TDSCDMA_HCS
  /* HCS related info for the cell */
  tdssrchset_cell_hcs_info_struct_type  cell_hcs_info;
#endif

  /* Flag indicating whether the cell is barred*/
  boolean cell_barred;

}
tdssrchinterf_cell_struct_type;


#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
/* The following information is the inter frequency priority information
   that was revieved from RRC via the 3rd MEAS_REQ/SIB19 */
typedef struct
{
  int8 priority;
  uint8 threshx_high; 
  uint8 threshx_low; 
  int16 q_qualmin_fdd; 
  int16 q_rxlevmin_fdd;
} tdssrchinterf_freq_priority_info_type;
#endif

typedef struct
{
  uint16  freq;
    /* Frequency specification */
  uint8   cell_cnt;

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
  boolean prior_info_present;
  /* Indicates if priority information has been provided for this frequency */

  tdssrchinterf_freq_priority_info_type prior_info;
  /* Priority information for this frequency */
#endif
    /* Number of cells listed on this frequency */
  tdssrchinterf_cell_struct_type *cell_ptr[TDSSRCHINTERF_OFREQ_CELL_MAX];
    /* Pointers to cell table entries */
}
tdssrchinterf_ofreq_cell_list_struct_type;


typedef enum
{
    eIF_TTT_DISABLED,
    eIF_TTT_ENABLED,
    eIF_TTT_REPORTED
} tdssrchinterf_evt_ttt_enum_type;

    
/* For event 2A, we need to maintain best freq */
typedef struct {
      /* Frequency */
      uint16 freq;
      /* Cell Parameter id */
      uint16  cpi;
      /* Measured value */
      int16 meas_val;

} tdssrchinterf_evt_2a_type;
    

typedef struct {
        /* Event id */
        tdsl1_inter_freq_event_enum_type evt_id ;

        /* Threshold used for event 2B, 2D or 2F, in dB */
        int16  used_thresh;
 
        /* Hysteresis. 0..15 (0..7.5dB) by step of 0.5 dB */
        uint16 hyst;

        /* Weight for even 2A,2B,2D and 2F. 0..20 by step of 0.1. 
           set to 0xFFFF for all other events. */
        uint16 w;

        /* Time-to-trigger in 10ms units */
        uint16 trig_timer;

        /* Non-used freq parameters */

        /* Threshold non-used */
        int16 non_used_thresh;

        /* W non-used in steps of *10 */
        uint16 w_non_thresh;

} tdssrchinterf_evt_trig_struct_type ; 

typedef struct {
    /* measurement id */
    uint16 meas_id;
    /* reporting mode */
    tdsl1_inter_freq_meas_report_mode_enum_type rpt_mode;

    /* layer 3 filter_coeff  */
    uint16 layer3_filter_coeff;

    /* filter_idx- Can be 0,1 or 2 */
    uint16 filt_idx;

    boolean meas_quan_incl;

    /* Quantity Measured. Note that, for WCDMA FDD cells, we currently support
         Ec/Io or RSCP Measurement Quantity for DCH Measurement report. */
    tdsl1_meas_quan_enum_type   intraf_quan_type;

    tdsl1_meas_quan_enum_type   interf_quan_type;

    /* Xiaoning add */
    uint8         intraf_meas_quan_list_no;
    tdsl1_meas_quan_enum_type   intraf_meas_quan[L1_MAX_MEAS_LIST_NO]; 

    uint8         interf_meas_quan_list_no;
    tdsl1_meas_quan_enum_type   interf_meas_quan[L1_MAX_MEAS_LIST_NO];  


    /*  Following variable indicates the current Best Frequency among the
   serving frequency and (upto) two non-used frequencies.
   Upon entering the DCH state, this value is initialized with the
   current serving cell */
    tdssrchinterf_evt_2a_type freqBEST_FREQUENCY_2A_EVENT;

  /* If, during the evaluation of event 2A, it is determined that the 
   best frequency has changed then the interim value is stored in the
   following variable which will be moved to the above variable after
   a measurement report has been sent */
    tdssrchinterf_evt_2a_type changedBEST_FREQUENCY_2A_EVENT;


    struct {
      /* Number of reports currently sent */
      uint16 periodic_rpt_cnt;

      /* timestamp in units of 10 ms. Used for determining TTT */
      uint16 ts_cnt;
      /* Number of reports requested */
      uint16 num_reports_req;
      /* Reporting interval, in multiples of 10 ms */
      uint16 reporting_interval;
    } periodic_s;

    /* Even triggered structure */
    tdssrchinterf_evt_trig_struct_type  evt_trig_s;

    /* Tracks the current state of the measurement */
    tdssrchinterf_evt_ttt_enum_type evt_ttt[TDSSRCHINTERF_OFREQ_FREQ_MAX+1];
    
    /* For event triggered reporting */
    boolean meas_reported[TDSSRCHINTERF_OFREQ_FREQ_MAX+1] ;

    uint16 ts_cnt[TDSSRCHINTERF_OFREQ_FREQ_MAX+1];

    /* common to both evt trig and periodic meas reporting */
    boolean rpt_cell_status_incl;
    tdsl1_meas_rpt_cell_status_struct_type  rpt_cell_status;

    boolean rpt_quan_incl;

    /* Reporting quantities */
    tdsl1_inter_freq_rpt_quan_struct_type    rpt_quan;
    
    /* Measuremnt Report transfer criteria to UTRAN */
    tdsl1_meas_report_trans_mode_enum_type  rpt_trans_mode;

    /* For event triggered, maintain a TRIGGERED_EVENT sub structure.
       This is required so that we can report all the cells in here
       every time a cell is reported.
        */
    uint16 num_freq_triggered;
    /* scr code */
    uint16 freq_triggered[TDSSRCHINTERF_OFREQ_FREQ_MAX+1];
    boolean stopped;

} tdssrchinterf_meas_parms_struct_type;

extern tdssrchinterf_meas_parms_struct_type 
                                tdssrchinterf_inter_freq_meas_db[TDSSRCHINTERF_MAX_INTERF_MEAS];
                                
extern uint16 tdssrchinterf_curr_freq_for_evaluation;

/*=========================================================================

    V A R I A B L E     D E C L A R A T I O N S                                                                             
  
 ===========================================================================*/
#if (defined (FEATURE_TDSCDMA_PLI_NMR_UTRAN) || defined(FEATURE_TDSCDMA_MODEM_STATISTICS))
extern uint16 tdssrchinterf_num_non_used_freq;
//extern uint16 tdssrchinterf_non_used_freq_array[TDSL1_MAX_NON_USED_FREQ];
#else
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
extern uint16 tdssrchinterf_num_non_used_freq;
#endif
#endif

extern boolean tdssrchinterf_interf_periodic_rpts_requested;
extern boolean tdssrchinterf_interf_evt_trig_rpts_requested;
extern uint16 tdssrchinterf_num_interf_meas_in_progress;

extern boolean tdssrchinterf_inter_freq_meas_performed;

extern boolean tdssrchinterf_reset_interf_meas_during_ho;

/*=========================================================================

    F U N C T I O N     D E C L A R A T I O N S                                                                             
  
 ===========================================================================*/

/*======================================================================
FUNCTION SRCH_INTERF_PROCESS_MEAS_CTRL_MSG

DESCRIPTION
    Currently we use this in DCH state to obtain a new inter_f 
    measurement infor.
     

RETURN VALUE
    boolean

=======================================================================*/
extern boolean tdssrchinterf_process_meas_ctrl_msg (tdsl1_meas_cmd_type *cmd_ptr);

/*======================================================================
FUNCTION SRCH_INTERF_RESET_MEAS_DBASE

DESCRIPTION
    Resets the existing interf freq measurement dbase
    In particular the following data structures are impacted
        
        tdssrchinterf_inter_freq_meas_db
     
RETURN VALUE
    void

=======================================================================*/
extern void tdssrchinterf_reset_meas_dbase(void);

/*======================================================================
FUNCTION SRCH_MEAS_INTER_F_SIB_UPDATE

DESCRIPTION
    Used to update the inter-frequency SIB information received in 
    a TDSL1_CPHY_MEASUREMENT_REQ command message.

RETURN VALUE
    boolean

=======================================================================*/
extern void tdssrchinterf_meas_sib_update(tdsl1_meas_cmd_type *cmd_ptr);

/*======================================================================
FUNCTION SRCHMEAS_REMOVE_INTERFREQ_MEASUREMENTS

DESCRIPTION
    Currently we use this in DCH state to release inter freq measurement
     

RETURN VALUE
    boolean

=======================================================================*/ 
extern boolean tdssrchinterf_remove_interfreq_measurements(uint16 meas_id);
 
/*======================================================================
FUNCTION SRCH_INTERF_GET_MEAS_IDX_BASED_ON_ID

DESCRIPTION
    Returns the meas index into the measurements DB table based on
    the measurement id
RETURN VALUE
    uint16

=======================================================================*/
extern uint16 tdssrchinterf_get_meas_idx_based_on_id(uint16 meas_id);
/*======================================================================
FUNCTION SRCH_INTERF_UPDATE_INTERF_RPT_TRANS_MODE_INFO

DESCRIPTION
    Used to update the report trans mode
     

RETURN VALUE
    void

=======================================================================*/
extern void tdssrchinterf_update_interf_rpt_trans_mode_info(uint16 meas_id,
                 tdsl1_meas_report_trans_mode_enum_type rpt_trans_mode);
/*===========================================================================
FUNCTION        tdssrchinterf_evaluate_events_2d_2f
    

DESCRIPTION     This function evaluates the event triggered inter freq
                measurements 2D and 2F.
                
DEPENDENCIES    

RETURN VALUE    
            None
        
SIDE EFFECTS    
===========================================================================*/
extern void tdssrchinterf_evaluate_events_2d_2f(void);

/*===========================================================================
FUNCTION        SRCH_INTERF_SEND_PERIODIC_MEAS_REPORT
    

DESCRIPTION     This function prepares and sends the periodic measurement 
                report for inter freq measurements to RRC
                
DEPENDENCIES    

RETURN VALUE    
            None
        
SIDE EFFECTS    
===========================================================================*/
extern void tdssrchinterf_send_periodic_meas_report(uint16 meas_id);
/*======================================================================
FUNCTION SRCH_INTERF_DCH_INIT

DESCRIPTION
    Resets all the DS upon entering DCH state
         
RETURN VALUE
    void

=======================================================================*/
extern void tdssrchinterf_dch_init(void);


extern void tdssrchinterf_reset_mrm_history(void);

#endif /* SRCHINTERF_H */
