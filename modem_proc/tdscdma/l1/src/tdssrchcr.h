#ifndef SRCHCR_H
#define SRCHCR_H
/*===========================================================================
   S R C H     C E L L     R E S E L E C T I O N     H E A D E R    F I L E

DESCRIPTION
   This file contains procedures declarations pertinent to WCDMA
   cell reselection evaluation.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  tdssrchcr_init() should be called once we enter DRX mode.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2000-2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/19/14   zl      Force a T2L cell detection after T2L reselection failure recovery
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsl1m.h"

#include "tdssrchcmd.h"
#include "tdssrchset.h"

#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_S
#include "tdssrchgsm.h"
#endif

//#if defined(FEATURE_TDSCDMA_TO_LTE) 
#include "tdsl1rrcif.h"
//#endif
#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
#include "tdssrchlte.h"
#endif

/*===========================================================================

                        CONSTANT DECLARATIONS

===========================================================================*/

/*daisy :I was told  that WCDMA does cell evaluation once every 80 ms on all cells in CELL_FACH.  
   From your description, it is performed on each frequency after its measurement. 
   Could you confirm this implementation?
   I think we can decouple S and R computation and the maintenance of Treselection: 
   S &R computation could be done after each measurement and based on which srched_cell[] is formed. 
   Treselection can be started (for cells just placed in srched_cell[] ) and checked (for cells already have 
   Treselection running) at a fixed period like once every 80ms (configurable interval). This way if UE find 
   two strong cells on subsequent measurements, they will likely to have the same Treselection expiry time 
   so that the stronger cell will be the reselection candidate.
*/
 /*zy:Thanks for your response, I already checked W implementation, it is evaluated when every time nset is 
    scheduled, the scheduling period of nset cells is 80ms,so the reselection is evaluated every 80ms.
	  
    for TD, the scheduling period for different freq maybe different, we don't have a fixed 80ms.however 
    we can set a fixed period for this evaluation, we think maybe 200ms will be more suitable. one reason 
    is because W has only 2 inter freqs and TD will have more, we need more time to complete all freq's 
    measurement. the second reason is now remf scheduling period is 200ms,80ms maybe not make this 
    freq measured
*/
 /*daisy:Thanks for providing the confirmation. For the reasons that you stated, 
    I agree the longer evaluation period is required, Since 200ms is only the measurement period for 
    intraF cells and the measurement period for interF cells is 400ms, let's set this constant to 400ms 
    to start with. Again this is tunable for performance optimization.
 */
 
 /*In FACH mode, our design evaluation period is 400 ms, But GSM measurement period is 480ms, 
     can we enlarge the evaluation period to 480ms?
  daisy:Enlarging the evaluation period to 480ms is acceptable.*/
 /*80 subframes = 400ms*/
#define  TDSSRCHCR_RESEL_EVALUATION_PERIOD 96

/*SW triggers intraF/interF measurement when SIR=TS_RSP -RSSI (dB) <= SIRth1 (nominal -2dB)
   SW triggers immediate reselection when SIR=TS_RSP -RSSI (dB) <=SIRth2  (nominal -20dB)*/

#define TDSSRCHCR_TRIGGER_N_MEAS_SIR_THROD -2

#define TDSSRCHCR_TRIGGER_IMMED_RESEL_SIR_DB -20
/*10^(-2) = 0.01,mutiply 100*/
#define TDSSRCHCR_TRIGGER_IMMED_RESEL_SIR_LINEAR 1

#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL1_DB  3
/*x= 10log10(y)->y=10^(x/10),when x=3,y=10^(0.3) = 2*/
/*y=10^(0.3)*256 = 511*/
#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL1_LINEAR_X256  511

#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL2_DB  0

#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL2_LINEAR_X256  256

//#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL5_DB  -12
//#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL5_DB  -14
#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL5_DB  -16
 /*0.063*256*/
//#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL5_LINEAR 16
//#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL5_LINEAR 10 
#define TDSSRCHCR_TRIGGER_N_MEAS_S_QUAL5_LINEAR 6 


#define TDSSRCHCR_SNR_BIGGEST_VALUE_LINEAR       (127*256)
#define TDSSRCHCR_SNR_BIGGEST_VALUE_DB           127


/* Smin value to calculate an internal threshold value for Sintrasrch
 * when Sintrasrch is specified OTA. Tuneable.
 */
/*daisy said keep this value,but change this value to the min q_rxlevmin
    because in function tdssrchcr_hcs_check_meas_rules,
    still need use s_intrasrch_internal*/
#define TDSSRCHCR_SMIN_FOR_SINTRA -115

/* Number of frames in 30 seconds */
#define TDSSRCHCR_30_SEC_TIME_IN_FRAMES 3000

/* Full search timer when no measurement occasions are specified
 * or when no GSM measurements are performed (number of frames).
 */

/* -110 dBm as minimum average RSSI for a cell to have BSIC meas */
#define TDSSRCHCR_GSM_RSSI_THRESHOLD_FOR_BSIC  (-105*16)

/* Check if the cell meets S criteria */
#define TDSSRCHCR_CELL_MEETS_S_CRITERIA( cell_ptr )  (cell_ptr->s_rxlev > 0)
    
/* Maximum value of Ncr, used to define the array size for 
 * storing reselection times 
 */
#define TDSSRCHCR_L1_MAX_N_CR_VALUE  16

#ifdef FEATURE_TDSCDMA_HCS
/* Maximum value of PENALTY_TIMER_n that NW can set for a neighbor
 * cell in SIB11/12 (in seconds)
 */
#define TDSSRCHCR_L1_MAX_PENALTY_TIMER   60

/* Value mapping tmp_offset to infinity */
#define TDSSRCHCR_L1_TMP_OFFSET_INFINITY   50
#endif /* FEATURE_TDSCDMA_HCS */

#define TDSSRCHCR_NV_MAX_TX_PWR_INCREMENT 1

/* Macro to indicate current meas rules status */
#define TDSSRCHCR_MEAS_RULES_INDICATE_MEAS()       (tdssrchcr_current_meas_rules != 0)

#define TDSSRCHCR_MEAS_RULES_INDICATE_INTRA_MEAS() (tdssrchcr_current_meas_rules & TDSSRCHCR_MEAS_INTRA)
#define TDSSRCHCR_MEAS_RULES_INDICATE_INTER_MEAS() (tdssrchcr_current_meas_rules & TDSSRCHCR_MEAS_INTER)
#define TDSSRCHCR_MEAS_RULES_INDICATE_IRAT_MEAS()  (tdssrchcr_current_meas_rules & TDSSRCHCR_MEAS_IRAT)
#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
#define TDSSRCHCR_MEAS_RULES_INDICATE_LTE_MEAS()  (tdssrchcr_current_meas_rules & TDSSRCHCR_MEAS_LTE)
#endif
#define TDSSRCHCR_RESET_RESEL_EVAL_MEAS_PENDING(reset_meas)\
        tdssrchcr_res_eval_meas_pending = (tdssrchcr_res_eval_meas_type) ((uint16) tdssrchcr_res_eval_meas_pending & \
          (~(uint16)(reset_meas)));

/* Macro to indicate if inter freq resel is in progress */
#define TDSSRCHCR_INTERF_RESEL_IN_PROGRESS() (tdssrchcr_inter_f_cell_ptr != NULL)

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
/* Macro to check if this prior is higher then serv cell */
#define TDSSRCHCR_IS_PRIOR_HIGHER_THEN_SERVING_CELL_PRIOR(prior) (prior > tdssrch_aset_parms.prior_info.priority) 
#endif

/* upon sending reselection indication to L1, L1 needs to refresh the target cell's 
 * RSCP against the R-criterion to avoid using its obsolete ranking value. */
#define TDSSRCHCR_INTERF_MEAS_MAX_LIFE_CYCLE 2

#define TDSSRCHCR_REFRESH_MEAS_IN_PROGRESS() (tdssrchcr_idle_res_eval.tds_meas_pending != 0)

#ifdef FEATURE_TDSCDMA_T2G_CELL_RESEL_RECONFIRMATION
#define TDSSRCHCR_RECONFIRM_GSM_MEAS_IN_PROGRESS() (tdssrchcr_idle_res_eval.irat_meas_pending != 0)
#endif

#define TDSSRCHCR_REFRESH_LTE_MEAS_IN_PROGRESS() (tdssrchcr_idle_res_eval.lte_meas_pending != 0)
#define TDSSRCHCR_SERVING_CELL_RSCP_THRESHOLD_HIGH_COST    -90

#define TDSSRCHCR_SERVING_CELL_SIR_THRESHOLD_HIGH_COST    -6
/*===========================================================================

                        TYPE DECLARATIONS

===========================================================================*/

/* Neighbor cell type */
typedef enum
{
  INTRA_F_CELL,
  INTER_F_CELL,
  INTER_S_GSM_CELL,
  INTER_S_LTE_CELL,
  INVALID_CELL
} tdssrchcr_cell_type_enum_type;

#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_F
/* Frequency index */
typedef enum
{
  FREQ_0,
  FREQ_1,
  FREQ_2,
  FREQ_3,
  FREQ_4,
  FREQ_5,
  FREQ_6,
  FREQ_7,
  FREQ_NONE
} tdssrchcr_inter_f_idx_enum_type;
#endif /* FEATURE_TDSCDMA_CELL_RESELECT_INTER_F */


/* Structure used to store the time at which UE sucessfully reselects.
 * This information is needed to calculate if the UE is in normal state
 * or fast moving state
 * Note that both Tcrmax and Tcrmaxhyst are in seconds and hence as a 
 * first pass the reselection times are being stored at a second 
 * granularity to avoid qword computations. If needed it can be 
 * implemented at a ms granularity using qword data type
 */
typedef struct
{
  /* Index into the reselection times structure */
  uint16 res_time_index;

  /* Time in sec when UE completes a successful reselection.
   * This is one more than Max Ncr value since UE declares
   * fast moving when num reselection in time period Tcrmax
   * EXCEEDS Ncr
   */
  dword res_time[TDSSRCHCR_L1_MAX_N_CR_VALUE + 1];
} tdssrchcr_res_times_struct_type;
                           
/* Structure used to store UE's fast moving status and fast moving end
 * time
 */
typedef struct
{
  /* Flag to indicate that UE is in fast moving state */
  boolean fast_moving;

  /* Time in s since power up when UE exits fast moving status. This 
   * time includes Tcrmaxhyst time too
   */
  dword fast_moving_end_time;
}tdssrchcr_ue_fast_moving_struct_type;


/* Barred Cells list of this structure type. Same as CELL_BAR_CMD struct*/
typedef struct
{
  tdsl1_cell_bar_info_e_type l1_cell_bar_info;        /* GSM or FDD cell*/
  union
  {
    tdsl1_gsm_cell_identity_struct gsm;
    //tdsl1_fdd_cell_identity_struct fdd;
#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
    tdsl1_eutra_cell_info_type lte;
#endif

    tdsl1_tdd_cell_identity_struct tdd;
  }id;
  uint32 t_barred;            /* T barred timer value */
  boolean bar_for_resel_only; /* Unbar this barred cell info on emergency call*/
  boolean non_eq_plmn_barred; /* Unbar this on eq plmn change */
#ifdef FEATURE_UNBAR_NON_HIGHEST_RANKED_CELL
  boolean prev_freq_bar; /* Frequency barred initially from SIB.*/
#endif
} tdssrchcr_barred_cell_struct;

/*Flag inidicating whether serving is barred*/
extern boolean tdssrchcr_pending_immdt_resel;

/*Flag indication whether reacq has been failed atleast once due to
barred cell*/
extern boolean tdssrchcr_barred_cell_reacq_failed;  

/*Flag indication whether to ignore frequency bar or not*/
extern boolean tdssrchcr_ignore_barred_freqs;

/* Indicates cell bar req is waiting for FMO cleanup*/
extern boolean tdssrchcr_cell_bar_update;


//#ifdef FEATURE_ENHANCED_CELL_RESELECTION
/* Enum to indicate status of cell reselection evaluation */
typedef enum 
{
  TDSSRCHCR_RES_EVAL_DISABLED,                    /* Before 1s timer expiry */
  TDSSRCHCR_RES_EVAL_INACTIVE,                    /* Possible now (1s timer has expired)  */
  TDSSRCHCR_RES_EVAL_PENDING_MEAS_COMPLETE
} tdssrchcr_res_eval_status_type;

/* Enum to indicate eval meas type */
typedef enum {
  TDSSRCHCR_RES_EVAL_MEAS_NONE         = 0x0,
  TDSSRCHCR_RES_EVAL_MEAS_INTRA        = 0x1,
  TDSSRCHCR_RES_EVAL_MEAS_INTER        = 0x2,
  /*zy:there are at most 8 inter-f,so tds_meas_pending can be set to 0x100(1<<8)*/
  TDSSRCHCR_RES_EVAL_MEAS_IRAT_RSSI    = 0x200,
  TDSSRCHCR_RES_EVAL_MEAS_IRAT_BSIC_ID    = 0x400,
  TDSSRCHCR_RES_EVAL_MEAS_IRAT_BSIC_RC         = 0x800,
  #if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
  TDSSRCHCR_RES_EVAL_MEAS_LTE_CELL_DETECT = 0x1000,
  TDSSRCHCR_RES_EVAL_MEAS_LTE_CELL_MEAS = 0x2000,
  TDSSRCHCR_RES_EVAL_MEAS_LTE = 0x4000,
  TDSSRCHCR_RES_EVAL_MEAS_INTER_IRAT_LTE = 0x8000,
  TDSSRCHCR_RES_EVAL_MEAS_ALL       = 0xFFFF
  #else
  TDSSRCHCR_RES_EVAL_MEAS_ALL       = 0xFFFF
  #endif
} tdssrchcr_res_eval_meas_type;

/* Enum to indicate meas rules type */
typedef enum {
  TDSSRCHCR_MEAS_NONE    = 0x0,
  TDSSRCHCR_MEAS_INTRA   = 0x1,
  TDSSRCHCR_MEAS_INTER   = 0x2,
  TDSSRCHCR_MEAS_IRAT    = 0x4,
#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
  TDSSRCHCR_MEAS_LTE     = 0x8,    
  TDSSRCHCR_MEAS_ALL     = 0xF
#else
  TDSSRCHCR_MEAS_ALL     = 0xF
#endif
} tdssrchcr_meas_rules_type;

typedef struct
{
  /* Indicates if the cell is Intra-F, Inter-F or GSM */
  tdssrchcr_cell_type_enum_type    cell_type;

  /* flag to exclude cell from ranking */
  boolean exclude;

  union
  {
    /* Pointer to Intra-Freq cell information */
    tdssrchset_cell_struct_type_tmp      *tdscdma_intra;
#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_S
    /* Pointer to Inter-RAT GSM cell information */
    tdssrchset_gsm_cell_struct_type  *gsm;
#endif
#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_F
    /* Pointer to Intra-Freq cell information */
    tdssrchset_cell_struct_type_tmp *tdscdma_inter;
#endif
#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
    tdssrchlte_cell_struct_type  *lte;
#endif
  }u;

} tdssrchcr_rank_cell_struct_type;

#ifdef FEATURE_TDSCDMA_HCS
/* Enum to find out previous state and present state                  */
/* F O R M A T     ---  Previousstate_PresentState                    */
/* X - stands for               not measured state                          */
/* L - Low priority             cells measure state                          */
/* E - Equal priority           cells measure state                         */
/* H - High priority            cells measure state                         */
/*                    E X A M P L E                                                  */
/*   LEH_LEH ==> LowEqualHIgh_LowEqualHigh                        */
/* Previous state measured LEH Present state measures LEH     */

typedef enum
{
  LEH_XEH,
  LEH_LEH,
  LEH_LEX,
  LEX_XEH,
  LEX_LEH,
  LEX_LEX,
  XEH_XEH,
  XEH_LEH,
  XEH_LEX

} tdssrchcr_prio_gp_state;
#endif


//#endif /* FEATURE_ENHANCED_CELL_RESELECTION */

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
typedef struct
{

  /* 25.133 section 4.2.2.5a
     The total number of configured higher priority E-UTRA 
    and UTRA FDD carrier frequencies and is additionally 
    increased by one if one or more groups of GSM frequencies 
    is configured as a higher priority
    Thigher_priority_search = 60 * Nlayers */

  uint32 nlayers;

    /* 25.133 section 4.2.2.3
     The total number of carriers used for FDD cells.
     The UE shall measure CPICH Ec/Io and CPICH RSCP of detected
     Inter-Frequency cells at least every (Ncarrier-1) * TmeasureFDD */

  uint32 ncarrier;

  /*25.123 4.2.2 
    The total number of carriers used for LTE cells. */
  uint32 kcarrier;

  /* Indicates if priority based reselection is to be used
     for Inter-Freq layers */
  boolean interf_prior_info_present;

  /* Indicates if any of the Inter-Freq layers configured by
     n/w are higher priority then serving cell */
  boolean higher_prior_inter_freq_present;

  /* Indicates if priority based reselection is to be used
     for GSM layers */
  boolean gsm_prior_info_present;

  /* Indicates if any of the GSM Arfcns configured by
     n/w are higher priority then serving cell */
  boolean higher_prior_gsm_arfcn_present;

  boolean g_arfcn_found_in_high_prio_srch;

  #ifdef FEATURE_TDSCDMA_TO_LTE
  /* Indicates if LTE EARFCNs have been configured in the 
     Neighbor list */
  boolean lte_info_present;

  /* Indicates if any of the LTE EArfcns configured by
     n/w are higher priority then serving cell */
  boolean higher_prior_lte_earfcn_present;

  /* Count of the number of higher prior LTE Earfcns */
  uint16 num_higher_prior_lte_earfcn;
  #endif

}tdssrchcr_prior_info_for_rats_struct_type;

typedef enum
{
  /* In high priority scheduling mode, cells with a higher priority will only be considered during reselection */
  TDSSRCHCR_HIGH_PRIORITY_SCHEDULING_MODE,
  /* In all priority scheduling mode, all cells are considered regardless of priority */
  TDSSRCHCR_ALL_PRIORITY_SCHEDULING_MODE
} tdssrchcr_prior_scheduler_mode_enum_type;

#endif

typedef struct
{
  /* indicate reselection evaluation needs to be performed or not */
  tdssrchcr_res_eval_status_type status;

  /* inidcate which TDS freq meas are pending for reselection evaluation */
  uint32 tds_meas_pending;
  /* inidcate which irat meas are pending for reselection evaluation */
  uint16 irat_meas_pending;
  #ifdef FEATURE_TDSCDMA_TO_LTE
  /*indicate LTE measurement is pending*/
  uint16 lte_meas_pending;
  #endif

  /* indicate which meas have been completed for reselection evaluation */
  tdssrchcr_res_eval_meas_type meas_completed;
}tdssrchcr_res_eval_struct_type;

extern tdssrchcr_res_eval_struct_type  tdssrchcr_idle_res_eval;

#ifdef FEATURE_TDSCDMA_T2G_CELL_RESEL_RECONFIRMATION
typedef struct
{
  /* Flag to indicate that a reconfirm GSM RSSI measurement is needed */
  boolean reconfirm_need;

  /* the arfcn of the cell which need to do reconfirm measurement*/
  W_ARFCN_T arfcn;
}tdssrchcr_reconfirm_arfcn_struct_type;

extern tdssrchcr_reconfirm_arfcn_struct_type tdssrchcr_reconfirm_arfcn_info;
#endif

#define TDSSRCHCR_RESET_RESEL_EVAL_TDS_MEAS_PENDING(freq_idx)\
        (tdssrchcr_idle_res_eval.tds_meas_pending &= ~(1 << freq_idx))

/*===========================================================================

                   EXTERNAL VARIABLE DECLARATIONS

===========================================================================*/
/* Flag to indicate if cell reselection evaluation needs to be started
 * after DRX has completed downlink channel setup.
 */
extern boolean tdssrchcr_wait_drx_start_cell_resel;

/* Timer to keep track number of frames to send out of service ind */
/*neveruse*/
extern uint8 tdssrchcr_out_of_service_timer;

/* Flag to indicate that the out of service timer has expired */
extern boolean tdssrchcr_out_of_service_timer_expired;

/* Flag to indicate that W2G failed and out of service timer has expired */
extern boolean tdssrchcr_resume_after_w2g_fail_OOS_expired;

/* Flag to indicate to DRX to count 1 second before enabling cell reselection */
extern boolean tdssrchcr_start_1_sec_timer;

/* Flag to indicate 1s has elapsed since DRX to start cell reselection */
extern boolean tdssrchcr_start_cell_reselection;

/* Flag to indicate if cell reselection related processing
 * should be done.
 */
extern boolean tsdsrchcr_cell_reselect_enabled;

/* Flag to indicate if a cell reselection is pending */
extern boolean tdssrchcr_pending_cell_resel;

/* flag to indicate that a cell reselection evaluation is due */
extern boolean tdssrchcr_cell_resel_eval_due;

/* Cell pointer to the WCDMA neighbor cell to be reported to RRC via
 * TDSRRC_CPHY_CELL_RESELECTION_IND.
 */
extern tdssrchset_cell_struct_type_tmp *tdssrchcr_cr_cell_ptr;

/* Cell pointer to a WCDMA Intra-F neighbor cell which became stronger
 * during a intra-freq cell reselection. In case of a cell reselection failure
 * this pointer will be used to trigger fresh reselection.
 */
extern tdssrchset_cell_struct_type_tmp *tdssrchcr_strong_cell_ptr;

#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_F
/* Cell pointer to the WCDMA Inter-frequency neighbor cell to be
 * reported to RRC via TDSRRC_CPHY_CELL_RESELECTION_IND.
 */
//extern srch_interf_cell_struct_type* tdssrchcr_inter_f_cell_ptr;

extern tdssrchset_cell_struct_type_tmp *tdssrchcr_inter_f_cell_ptr;

#endif

#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_S
/* Cell pointer to the GSM neighbor cell to be reported to RRC via
 * TDSRRC_CPHY_CELL_RESELECTION_IND.
 */
extern tdssrchset_gsm_cell_struct_type* tdssrchcr_gsm_cell_ptr;

#endif

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
/* Cell pointer to the LTE neighbor cell to be reported to RRC via
 * TDSRRC_CPHY_CELL_RESELECTION_IND.
 */
extern tdssrchlte_cell_struct_type* tdssrchcr_lte_cell_ptr;
#endif

#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_S
/* Sorted list of GSM neighbor cells, sorted according to RSSI */
extern tdssrchset_gsm_cell_struct_type*
        tdssrchcr_sorted_gsm_nset_ptr[ TDSSRCHSET_GSM_NSET_MAX ];
#endif

/* flag to indicate that a BSIC verification is due for the GSM
 * neighbor before sending the reselection indication.
 */
//extern boolean tdssrchcr_pending_cell_resel_bsic_verif;

/* Flag to indicate that a RSSI scan is pending */
extern boolean tdssrchcr_t_meas_gsm_timer_expired;

/* Flag indicating that the RF is now available for returning */
extern boolean tdssrchcr_rf_free;

/* Flag to indicate if drx is waiting on srch ready to sleep */
extern boolean tdssrchcr_drx_waiting_on_srch_sleep;

/*variable holding the remaining OOS timer value used during state transition*/
extern rex_timer_cnt_type tdssrchcr_rem_oos_time;

/*variable holding the remaining Cell Resel timer value used 
  during state transition*/
extern rex_timer_cnt_type tdssrchcr_rem_CR_time_ms;

/*Flag indicating whether CR timer has expired and
  tdssrchcr_init() should be called in DRX*/ 
extern boolean tdssrchcr_start_CR;

//extern uint16 tdssrchcr_cell_resel_eval_timer;

/* Tresel value to be used for inter rat resel*/
extern uint16 tdssrchcr_tresel_irat;

/* Global to indicate reselection evaluation needs to be performed or not */
extern tdssrchcr_res_eval_status_type       tdssrchcr_res_eval_status;

/* Global to inidcate which meas are pending for reselection evaluation */
extern tdssrchcr_res_eval_meas_type         tdssrchcr_res_eval_meas_pending;

/* Global to indicate which meas have been completed for reselection evaluation */
extern tdssrchcr_res_eval_meas_type         tdssrchcr_res_eval_meas_completed;

/* Global to indicate current meas rules type */
extern tdssrchcr_meas_rules_type tdssrchcr_current_meas_rules;

/* Number of drx cycles the cell selection criteria has failed continously */
extern uint8 tdssrchcr_cell_select_failed_drx_cycles;

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
/*Number of DRX cycles the serving cell fails the S criteria continuously 
  Defined for LTE only
  */
extern uint8 tdssrchcr_cell_select_failed_drx_cycles_lte;  
#endif

/* Table of Nserv values indexed by K values. K = 0..2 is not valid */
extern uint8 tdssrchcr_cell_select_nserv_tab[10];

/* Table of TmeasureGSM values indexed by K values. K = 0..2 is not valid */
extern uint8 tdssrchcr_t_meas_gsm_tab[10];

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
/* Table of TmeasureFDD values indexed by K values. K = 0..2 is not valid */
extern uint8 tdssrchcr_t_meas_tdd_tab[10];

extern tdssrchcr_prior_info_for_rats_struct_type tdssrchcr_prior_info_for_rats;
extern tdssrchcr_prior_scheduler_mode_enum_type tdssrchcr_curr_prior_scheduling_mode;
#endif

extern tdssrchcr_res_eval_struct_type  tdssrchcr_idle_res_eval;

extern tdssrchcr_meas_rules_type tdssrchcr_meas_rules_scell_barred;

extern uint16 tdssrchcr_refresh_inter_freq;
extern uint32 tdssrchcr_refresh_lte_freq; 
extern uint8 tdssrchcr_t2g_resel_fail_num;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
/* ================================================================================= 
FUNCTION       tdssrchcr_is_in_cell_resel_panic_mode() 

DESCRIPTION
  This function checks whether UE is in cell reselection panic mode due to continously  
   low signal strength of the serving cell. 

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
=================================================================================== */
extern boolean tdssrchcr_is_in_cell_resel_panic_mode(void);
#endif

/*===========================================================================

FUNCTION       tdssrchcr_exclude_cell_from_ranking

DESCRIPTION
  This function iterates through the searched cells list and if it
  finds the cell pointed to by the passed pointer, will mark this
  cell to be excluded from ranking during the current evaluation.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Will set the exclude flag to true for the cell pointed
  to by cell_ptr.
===========================================================================*/
void tdssrchcr_exclude_cell_from_ranking( void* cell_ptr );

/*===========================================================================

FUNCTION       TDSSRCHCR_OUT_OF_SERVICE_AREA_CHECK 

DESCRIPTION
  This function is called anytime the serving cell fails the S criteria.
  If the serving cell fails consistently for 12s during PCH or for 4s
  during FACH, this function returns a boolean indicating whether an
  out of service area indication needs to be sent to RRC.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if out of service area indication has to be sent.
  FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdssrchcr_out_of_service_area_check( void );

/*===========================================================================

FUNCTION       TDSSRCHCR_RESET_OOS_TIMER

DESCRIPTION
  This function resets the OOS timer and clears the signal and other
  related flags.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdssrchcr_reset_oos_timer( void );

/*===========================================================================

FUNCTION        TDSSRCHCR_SET_OUT_OF_SERVICE_TIMER_FLAG

DESCRIPTION
  This function sets the tdssrchcr_out_of_service_timer_expired flag when
  the out of service timer expires.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchcr_out_of_service_timer_expired flag is set.

===========================================================================*/
extern void tdssrchcr_set_out_of_service_timer_flag( void );

/*===========================================================================

FUNCTION        TDSSRCHCR_START_CELL_RESELECTION_EVAL

DESCRIPTION
  This function is called by DRX after it completes setting up the
  downlink channels. This is done only when reacquisition attempt with
  list search passes at which time search will notify DRX to complete
  downlink channel setup. Cell Reselection evaluation will continue
  once this function is called by DRX.

DEPENDENCIES
  The Cell Reselection Evaluation is done only if cell reselection is
  enabled (i.e., the 1s timer has elapsed since camping on serving cell)
  and the searcher is waiting on DRX to start cell reselection evaluation.

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchcr_wait_drx_start_cell_resel is reset.

===========================================================================*/
extern void tdssrchcr_start_cell_reselection_eval(void);


/*===========================================================================

FUNCTION        TDSSRCHCR_INIT

DESCRIPTION
  This function initializes this module to start cell reselection
  evaluation. This is called 1 second after we have camped on a cell.
  Currently we enable cell reselection 1 second after we receive
  the DRX command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  srch_intra_cell_reselect_enabled is set to TRUE.
  tdssrchmeas_ignore_heartbeat_evt is set to TRUE.
  All the NSET neighbor cell cell reselection timers are initialized.

===========================================================================*/
extern void tdssrchcr_init( void );

/*===========================================================================

FUNCTION        TDSSRCHCR_RESET

DESCRIPTION
  This function resets the variables related to cell reselection. This is
  called while suspending or ending cell reselection evaluation. It is also
  called from drx_restart(), which is called on cell reselection failure.
  This gets DRX and cell reselection searches started up cleanly again.

DEPENDENCIES
  This function should be called in L1M task context.

RETURN VALUE
  None.

SIDE EFFECTS
  tsdsrchcr_cell_reselect_enabled is set to FALSE
  and other search related variables are reset.

===========================================================================*/
extern void tdssrchcr_reset( void );

/*===========================================================================

FUNCTION        TDSSRCHCR_CLEANUP

DESCRIPTION
  This function resets the variables related to cell reselection. This is
  called while suspending or ending cell reselection evaluation. DRX is
  notified if it is waiting on search ready for sleep.

DEPENDENCIES
  This function should be called in L1M task context.

RETURN VALUE
  None.

SIDE EFFECTS
  tsdsrchcr_cell_reselect_enabled is set to FALSE
  and other search related variables are reset.

===========================================================================*/
boolean tdssrchcr_cleanup( tdsl1m_state_enum_type next_state );
/*===========================================================================

FUNCTION     TDSSRCHCR_NSET_SRCH_DONE_CMD

DESCRIPTION
  This function is called when we first enter DRX and also after List search
  on SYNC_NSET and possibly a Step 1/2/3 search on ASYNC_NSET neighbor cells
  completes. This function will sort all the neighbor cells according to
  their energies and performs nset evaluation for cell reselection.

  If a cell was found better than the serving cell, send a
  TDSRRC_CPHY_CELL_RESELECTION_IND indication to RRC.

  If the serving cell became the most suitable cell, report reacquisition
  success to DRX module.

  If no best cell was found, it means we lost the serving cell and also
  could not find a better neighbor cell. Send reacquisition failed status
  to DRX module.

  If the DRX module is waiting for the reacquisition/cell reselection
  evaluation to complete, notify DRX module we are done.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchcr_nset_srch_done_cmd( boolean use_legacy);


#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_S
/*===========================================================================

FUNCTION      TDSSRCHCR_CHECK_GSM_CELL_BARRED

DESCRIPTION
    This function checks if a GSM cell is barred or not.
    Call when adding a cell to gsm nbr set.
DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the cell corresponding to the input is barred.
  FALSE otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean tdssrchcr_check_gsm_cell_barred ( tdssrchset_gsm_cell_struct_type*   cell_ptr);

#if 0
/*===========================================================================

FUNCTION      TDSSRCHCR_BSIC_IDENTIFY_DONE

DESCRIPTION
  This function performs the next step after a BSIC identification has
  been done. If a PCH cleanup is pending, a module cleanup done command
  will be sent to L1M.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdssrchcr_bsic_identify_done( tdscrgsm_local_cmd_type* cmd_ptr );
#endif

#endif

#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
/*===========================================================================

FUNCTION:  TDSSRCHCR_CHECK_LTE_FREQ_BARRED

DESCRIPTION: This function checks if the LTE frequency is barred  
  
DEPENDENCIES
  None.

RETURN VALUE
 	TRUE - If the LTE frequency is barred
 	FALSE - If the LTE frequency is not barred 

SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdssrchcr_check_lte_freq_barred(uint32 earfcn);  

#endif



#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_F

void tdssrchcr_nset_inter_f_cell_reselect_eval( void );
/*===========================================================================

FUNCTION       TDSSRCHCR_TUNE_RF

DESCRIPTION
  This function tunes RF to the specified frequency.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdssrchcr_tune_rf( uint16 freq );

#endif /* FEATURE_TDSCDMA_CELL_RESELECT_INTER_F */




/*===========================================================================

FUNCTION       TDSSRCHCR_ADD_CELL_TO_SRCHED_LIST

DESCRIPTION
  This function adds the specified cell to the list of neighbors that 
  were searched during this cell reselection search cycle.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void tdssrchcr_add_cell_to_srched_list
(
  /* Type of cell to be added */
  tdssrchcr_cell_type_enum_type    type,
 /* Ptr to Intra-F cell      */
  tdssrchset_cell_struct_type_tmp        *tdscdma_intra_ptr
#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_F
  /* Ptr to Inter-F cell      */
  ,tdssrchset_cell_struct_type_tmp *tdscdma_inter_ptr
#endif
#ifdef  FEATURE_TDSCDMA_CELL_RESELECT_INTER_S
  /* Ptr to GSM cell */
  ,tdssrchset_gsm_cell_struct_type    *gsm_ptr
#endif
#if defined(FEATURE_TDSCDMA_TO_LTE) && defined(FEATURE_TDSCDMA_ABSOLUTE_PRIORITY)
  /* Ptr to LTE cell */
  ,tdssrchlte_cell_struct_type    *lte_ptr
#endif
);

/*===========================================================================

FUNCTION       TDSSRCHCR_REMOVE_CELL_FROM_SRCHED_LIST

DESCRIPTION
  This function removes specified cell from the list of neighbors that 
  were searched during this cell reselection search cycle.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchcr_remove_cell_from_srched_list
(
  /* pointer to the cell structure */
  void *cell_ptr
);

/*===========================================================================

FUNCTION       TDSSRCHCR_SIB_MOD_UPDATE

DESCRIPTION
  This function performs SIB modification update if they are pending.
  In case a cphy setup is in progress, this will not clear the SRCH
  and DL module cleanup done flags, since cphy setup request has to
  continue. This function is called when either DL or SRCHFACH module
  completes cleaning up the current searches.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchcr_sib_mod_update( void );

/*===========================================================================

FUNCTION       TDSSRCHCR_FACH_EXCLUDE_CELLS_USING_MEAS_RULES

DESCRIPTION
  This function limits the candidates for cell reselection in FACH state
  based on the current meas rules

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchcr_srched_cells array of cells is updted
  
===========================================================================*/
void tdssrchcr_fach_exclude_cells_using_meas_rules (void);

#ifdef FEATURE_TDSCDMA_HCS
/*===========================================================================

FUNCTION       TDSSRCHCR_CALC_INTRA_NEIGHBOR_PENALTY_AND_H

DESCRIPTION
  This function calculates updates the penalty timer and calculates H
  for the neighbor cell

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchset_cell_struct_type for the neighbor is updated
  
===========================================================================*/
void tdssrchcr_calc_intra_neighbor_penalty_and_h(tdssrchset_cell_struct_type_tmp* n_ptr,
                                              uint16 time_elapsed,
                                              boolean apply_penalty);

/*===========================================================================

FUNCTION       TDSSRCHCR_CALC_INTER_NEIGHBOR_PENALTY_AND_H

DESCRIPTION
  This function calculates updates the penalty timer and calculates H
  for the neighbor cell

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchset_cell_struct_type for the neighbor is updated
  
===========================================================================*/
void tdssrchcr_calc_inter_neighbor_penalty_and_h(tdssrchset_cell_struct_type_tmp* n_ptr,
                                              uint16 time_elapsed,
                                              boolean apply_penalty);

/*===========================================================================

FUNCTION       TDSSRCHCR_CALC_GSM_NEIGHBOR_PENALTY_AND_H

DESCRIPTION
  This function calculates updates the penalty timer and calculates H
  for the gsm neighbor cell

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchset_cell_struct_type for the neighbor is updated
  
===========================================================================*/
void tdssrchcr_calc_gsm_neighbor_penalty_and_h(tdssrchset_gsm_cell_struct_type* n_ptr,
                                            uint16 time_elapsed);

/*===========================================================================

FUNCTION       TDSSRCHCR_EXCLUDE_CELLS_USING_H

DESCRIPTION
  This function limits the candidates for cell reselection based
  on H value

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell and 
  UE is not in fast moving state

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchcr_srched_cells array of cells is updted
  
===========================================================================*/
void tdssrchcr_exclude_cells_using_h (void);
      
/*===========================================================================

FUNCTION       TDSSRCHCR_FACH_EXCLUDE_CELLS_USING_MEAS_RULES_HCS

DESCRIPTION
  This function limits the candidates for cell reselection in FACH state
  based on the current HCS meas rules

DEPENDENCIES
  Can ONLY be called after the meas_rules function has correctly set the
  meas_detect flag for the cells.

RETURN VALUE
  None.

SIDE EFFECTS
  tdssrchcr_srched_cells array of cells is updted
  
===========================================================================*/
void tdssrchcr_fach_exclude_cells_using_meas_rules_hcs (void);

/*===========================================================================

FUNCTION       TDSSRCHCR_UPDATE_CELL_RES_TIME

DESCRIPTION
  This function udpate the cell reselection time in the res_time data str

DEPENDENCIES
  Can ONLY be called after a successful cell reselection

RETURN VALUE
  None.

SIDE EFFECTS
  res_time array is updated
  
===========================================================================*/
void tdssrchcr_update_cell_res_time(void);

/*===========================================================================

FUNCTION TDSSRCHCR_UPD_OTA_PARM

DESCRIPTION
  This function updates mobility information, stored in tdssrchcr_ue_fast_moving 
  structure,  based on information conveyed by TDSL1_CPHY_UPD_OTA_PARMS_IND. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdssrchcr_upd_ota_parm(tdsl1_upd_ota_parms_ind_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION       TDSSRCHCR_CHECK_FAST_MOVING_CONDITION

DESCRIPTION
  This function checks if the UE is in fast moving condition

DEPENDENCIES
  Can ONLY be called if HCS is enabled and immediately after a 
  CELL_PARM_REQ (SIB3) or a CELL_MEAS_REQ (SIB11/12)

RETURN VALUE
  None.

SIDE EFFECTS
  UE fast moving condition updated
  
===========================================================================*/
void tdssrchcr_check_fast_moving_condition(void);

/*======================================================================
FUNCTION  TDSSRCHCR_BACKUP_NEIGH_CELL_PENALTY_TIMER

DESCRIPTION
    Backups the penalty timers for all neighbor cells

RETURN VALUE
    void

SIDE EFFECTS
    backup store is filled in
=======================================================================*/
void tdssrchcr_backup_nbr_penalty_timer(void);

/*======================================================================
FUNCTION  TDSSRCHCR_RESTORE_NEIGH_CELL_PENALTY_TIMER

DESCRIPTION
    Restores the penalty timer info for all neighbor cells

RETURN VALUE
    void

SIDE EFFECTS
    Neighbor cell list penalty timers are updated from backup info
=======================================================================*/
void tdssrchcr_restore_nbr_penalty_timer(dword elapsed_time);
#endif

/*===========================================================================

FUNCTION       TDSSRCHCR_SEND_DL_WEAK_IND_TO_RRC

DESCRIPTION
  This function sends a CEL RESEL IND to RRC for the PSC passed in

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  CEL RESEL IND sent to RRC
  
===========================================================================*/
void tdssrchcr_send_dl_weak_ind_to_rrc(void);

/*===========================================================================

FUNCTION       TDSSRCHCR_INIT_ON_POWERUP

DESCRIPTION
  This function initializes cell reselection related data structures on 
  power up

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Cell reselection related globals initialized
  
===========================================================================*/
void tdssrchcr_init_on_powerup(void);

/*===========================================================================

FUNCTION       TDSSRCHCR_INIT_GSM_NSET

DESCRIPTION
  This function initializes gsm nset data 
  
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
void tdssrchcr_init_gsm_nset( void );
/*======================================================================
FUNCTION TDSSRCHCR_START_CR_TIMER

DESCRIPTION
  Start rex timer for cell reselection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  
=======================================================================*/
extern void tdssrchcr_start_CR_timer(void);

#ifdef FEATURE_IGNORE_TRESEL_ON_S_FAIL
/*===========================================================================

FUNCTION       TDSSRCHCR_CHECK_WEAK_RESEL_CONDITION

DESCRIPTION
  This function check if weak resel condition happened or not and update the flag accordingly.
  weak resel condition is when NCR=10 opt reselections (due to s-fail; bad agc; tresel short
  has been attempted in less than TCR = 60s. 
  This function should be called after receiving TRANSITON_REQ when transitioning to a new cell.
  This function is also called when there is a transition failure. It is based on the number of weak reselections
  attempted.
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
extern void tdssrchcr_check_weak_resel_condition(void);

/*===========================================================================

FUNCTION       TDSSRCHCR_UPDATE_WEAK_RESEL_CONDITION

DESCRIPTION
  This function checks if weak resel condtion's end time has been reached. If so update reset the weak resel flag.
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
extern void tdssrchcr_update_weak_resel_times(void);
#endif

/*======================================================================
FUNCTION  TDSSRCHCR_GET_NUM_FRAMES_SINCE_PREV_EVAL

DESCRIPTION
  Function to get num frames since previous eval

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
=======================================================================*/
extern uint16 tdssrchcr_get_num_frames_since_prev_eval(void);
  
/*======================================================================
FUNCTION TDSSRCHCR_SET_NUM_FRAMES_SINCE_PREV_EVAL

DESCRIPTION
  Function to set num frames since previous eval

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
=======================================================================*/
extern void tdssrchcr_set_num_frames_since_prev_eval(void);

/*===========================================================================
FUNCTION      TDSSRCHCR_UPDATE_MEAS_RULES

DESCRIPTION
  This function updates meas rules for all types of meas

DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Following globals updated to reflect the latest measurement rules
    tdssrchcr_current_meas_rules
    srchcr_current_meas_prio
===========================================================================*/
extern void tdssrchcr_update_meas_rules(void);

/*===========================================================================
FUNCTION       TDSSRCHCR_ASET_S_CRITERIA_PROCEDURES

DESCRIPTION
  This function performs following S criteria related procedures
  (a) Update aset S criteria
  (b) Check against immediate reselection at low EcIo threshold
  (c) Perform S criteria success or failure procedures

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Serving cell S criteria calculated, Tresel_fdd updated, S criteria
  success or failure procedures performed.
  
===========================================================================*/
extern void tdssrchcr_aset_s_criteria_procedures(void);

/*neveruse*/
/*===========================================================================

FUNCTION      TDSSRCHCR_PERFORM_CELL_RES_EVAL_IN_DRX

DESCRIPTION
  This function is called to perform cell reselection evaluation

  It first calls functions to calculate the ngbr cell rank and then 
  If there is no more searching to be done, send TDS_SRCHCR_NSET_SRCH_DONE_CMD
  local command to L1M. If this search was started when we first entered DRX,
  sort and return to L1M. If we are performing reacquisition after coming
  out of sleep, L1M will send command to sort the nset and continue with
  cell reselection evaluation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchcr_perform_cell_res_eval_in_drx(tdssrchcr_res_eval_meas_type res_eval_meas_comp);

extern void tdssrchcr_perform_cell_res_eval_in_fach(void);

/*===========================================================================
FUNCTION       tdssrchcr_perform_cell_res_eval_abs_prio ()

DESCRIPTION
  This function is used only for REL8 UE's when Absolute Priotity 
  is used for resel eval. 
  This is just a wrapper function to which calls the inter-f/gsm/LTE resel eval 
  functions based on the current scheduling mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchcr_perform_cell_res_eval_abs_prio(void);

/*===========================================================================
FUNCTION       TDSSRCHCR_SET_T_RESEL_FDD

DESCRIPTION
  This function set the Treslection value to be used for FDD neighbor cells
  based on serving cell S criteria and EcIo

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Treselection for FDD neighbors set as per optimizations
  
===========================================================================*/
extern void tdssrchcr_set_t_resel_tdd(void);
/*===========================================================================

FUNCTION       TDSSRCHCR_CALC_ASET_S_CRITERIA

DESCRIPTION
  This function calculates and updates the S criteria of the serving cell.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdssrchcr_calc_aset_s_criteria( void );

/*===========================================================================
FUNCTION      TDSSRCHCR_START_STEP3_SRCH

DESCRIPTION
  This function starts step-3 search in idle mode

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchcr_clear_srched_cells(void);
boolean tdssrchcr_aset_srxlev_meets_gsm_thresh( void );
tdssrchcr_rank_cell_struct_type* tdssrchcr_find_best_ecno_cell
(
  void* curr_best_cell_ptr
);
//void tdssrchcr_fach_nset_inter_f_cell_reselect_eval( void );
tdssrchcr_rank_cell_struct_type* tdssrchcr_find_best_gsm_cell
(
  tdssrchcr_rank_cell_struct_type* cell_ptr
);
tdssrchcr_rank_cell_struct_type* tdssrchcr_find_best_ranked
(
  /* pointer to the current best cell */
  void* curr_best_cell_ptr
);
boolean tdssrchcr_find_next_best_ranked
(
  /* pointer to the current best cell */
  void* curr_best_cell_ptr
);
//boolean tdssrchcr_check_gsm( void );
void tdssrchcr_nset_intra_f_cell_reselect_eval( void );
void tdssrchcr_nset_irat_cell_reselect_eval( void );
void tdssrchcr_wait_for_gtime_ready( void );
void tdssrchcr_init_intra_f_nset( void );
void tdssrchcr_init_inter_f_nset( void );
void tdssrchcr_init_timers( void );
void tdssrchcr_trigger_cell_resel_ind(void );
void TDSSRCHCR_PRINT_BARRED_CELLS(void);
void tdssrchcr_add_cell_to_barredlist (tdsl1_cell_bar_cmd_type *barred_cell);
//void tdssrchcr_handle_immediate_gsm_meas(boolean success);
//void tdssrchcr_do_immediate_gsm_meas(boolean success);
void tdssrchcr_gsm_cell_resel_ind( tdssrchset_gsm_cell_struct_type* new_cell_ptr );
void tdssrchcr_exclude_cells_using_prio( int16 included_priority);
void tdssrchcr_update_prio_state(boolean low_prio, boolean hi_prio, tdssrchcr_prio_gp_state *prio_state);
void tdssrchcr_qualify_intraf_cell( boolean low_prio, boolean eq_prio, boolean hi_prio);
boolean tdssrchcr_qualify_interf_cell( boolean low_prio, boolean eq_prio, boolean hi_prio);
void tdssrchcr_qualify_gsm_cell(boolean low_prio,boolean hi_prio);

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
/*============================================================================
FUNCTION: tdssrchcr_init_prior_info_for_all_rats()

DESCRIPTION:
  This fn initialize the global to indicate abscense of priority for all          
  RATs. This info is received in 3rd MEAS_REQ from RRC. We assume that         
  no priorities are configured in the system until the 3rd MEAS_REQ         
  containing prior info is received.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void tdssrchcr_init_prior_info_for_all_rats(void);

/*===========================================================================

FUNCTION       TDSSRCHCR_COMPUTE_SCHEDULING_MODE

DESCRIPTION
  This function checks the following for the serving cell and returns the scheduling
  mode to be used in the current drx cycle
 
  if (srxlev > s_prioritysearch1) && (Squal > s_prioritysearch2)
    Use High Prior Scheduling
  else
    Use All Prior Scheduling


DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdssrchcr_debug_priority_info(tdsl1_meas_priority_eutra_cell_list_info_type *priority_info);

/*============================================================================
FUNCTION: tdssrchcr_update_priority_and_lte_info()

DESCRIPTION:
  This fn processes the 3rd MEAS_REQ to update the priority information
  for all the RATs.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void tdssrchcr_update_priority_and_lte_info(
  tdsl1_meas_priority_eutra_cell_list_info_type *info);

/*===========================================================================

FUNCTION       SRCHCR_LTE_PROC_SRCH_DONE

DESCRIPTION
  This function 

DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrchcr_lte_proc_srch_done(tdssrchlte_local_cmd_type *cmd_ptr);

extern boolean tdssrchcr_check_high_prio_rssi_tmeasgsm_srch_due(void);

/*===========================================================================

FUNCTION     tdssrchcr_check_n_schedule_lte_cell_detect_on_t2l_fail

DESCRIPTION
  This function will check whether this L1M resume is due to T2L reselection failure recovery or not. 
  If yes, it will force a T2L cell detection on the failed frequency. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: do measurement again and refresh measurement result.
  FALSE: don't need refresh measurement result.

SIDE EFFECTS
  None.
===========================================================================*/
void tdssrchcr_check_n_schedule_lte_cell_detect_on_t2l_fail(void);


#endif /* FEATURE_TDSCDMA_ABSOLUTE_PRIORITY */

#endif /* TDSSRCHCR_H */
