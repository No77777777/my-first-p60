#ifndef TDSRRCCSPI_H
#define TDSRRCCSPI_H

/*===========================================================================
            R R C    C S P     I N T E R N A L     H E A D E R    F I L E
DESCRIPTION

   This file contains all internal prototypes used by Cell Selection Procedure.

EXTERNALIZED FUNCTIONS



INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccspi.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   zz     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "rex.h"
#include "msg.h"
#include "tdsuecomdef.h"

#include "tdsrrcmm_v.h"
#include "tdsrrcasn1.h"

#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrccspdb.h"

#include "tdsl1rrcif.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrccsp.h"
#include "sd.h"

#include "tdsrrcwrm.h"

#ifdef FEATURE_TDSCDMA_TO_WCDMA
#include "rrc_rr_types.h"
#endif
#include "tdsrrclogging.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define TDSRRC_MAX_NO_ABORT_TIME 1

/* Default Frquency for Initial testing */
#define  TDSDEFAULT_FREQ   10055

/* An Invalid value for frequency */
#define  TDSINVALID_FREQ   0xffff

/* Define a Scrambling Code for Initial testing */
/* Keep the FDD term but with TDD valid value */
#define  TDSDEFAULT_CPID  4


/* 25.304 5.1.2.2 cell high quality criterion for PLMN. Negative value */
#define TDSRRC_PCCPCH_RSCP_QUALITY_THRESHOLD_DBM   84

/* This Macro determines if the passed parameters satisfy the Suitable Cell
 * Selection criteria. It returns TRUE or FALSE based on the PLMN status and
 * Cell Status. A cell status of 0 indicates that cell is NOT barred and NOT in forbidden
 * registration area.
 */
#define  TDSRRC_CSP_IN_SUIT_SEL(plmn_status, cell_status)  \
(  \
 (plmn_status == TDSRRC_CSP_PLMN_DIFF_THAN_REQ) ? FALSE : \
   ((cell_status != 0) ? FALSE : TRUE) \
)


/* This macro picks the MM command between an RRC_SERVICE_IND and RRC_SERVICE_CNF
 * based on the last CSP event. If CSP camps on a cell and finds the last event
 * was RRC_CSP_SERVICE_REQ_RCVD, an RRC_SERVICE_CNF is sent to MM.
 * If the last event at the time of camping on a cell is RRC_CSP_CAMPED_ON_CELL_RCVD,
 * it implies the cell was lost after having camped on it last. This time an
 * RRC_SERVICE_IND is to be sent to MM.
 * TBD: A distinction has to be made if the last camped on cell and the newly
 * camped on cell are different. For the time being, RRC_SERVICE_IND is sent
 * irrespective of whether a new cell is selected or the last camped on cell
 * is selected again.
 */
#define   RRC_CSP_SELECT_MM_CMD(last_mm_primitive)    \
(                                                  \
  ((last_mm_primitive) == RRC_CSP_SERVICE_REQ_RCVD) ? RRC_SERVICE_CNF: RRC_SERVICE_IND \
)


/* This defines the max tries to acquire on a cell on a directed frequency */
#define  TDSRRC_CSP_MAX_ACQ_TRIES_ON_DIRECTED_FREQ  3

/* This defines the max number of cycles of tries on destination frequency and
 * acquisition database.
 */
#define  TDSRRC_CSP_MAX_NUM_ACQDB_DEST_FREQ_CYCLES  5

  /* This defines the max time to allow for cell selection 
   * when going from DCH to FACH due to reconfiguration message.
   * As per Spec OOS should be indicated after 4s of being 
   * out of service in CELL_FACH.
   */
  #define  TDSRRCTMR_DCH_FACH_CELL_SELECTION_TIMER_IN_MS  4000UL



#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* This defines the max time to allow for cell selection 
   * when going from FACH to FACH due to reconfiguration message.
   * As per Spec OOS should be indicated after 4s of being 
   * out of service in CELL_FACH.
   */
  #define  TDSRRCTMR_FACH_TO_FACH_CELL_SELECTION_TIMER_IN_MS  4000UL

  /* This defines the max time to allow for cell selection 
   * when going from FACH to PCH/URA_PCH due to reconfiguration message.
   * As per Spec OOS should be indicated after 12s of being 
   * out of service in CELL_FACH.
   */
  #define  TDSRRCTMR_FACH_TO_PCH_CELL_SELECTION_TIMER_IN_MS  12000UL
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

/* This defines the max time to allow for cell selection
 * when going from DCH to FACH due to RL failure or RLC unrecoverable error
 */
#define  TDSRRCTMR_DCH_FACH_DUE_TO_CU_CELL_SELECTION_TIMER_IN_MS  180000UL


  /* This defines the max time to allow for cell selection 
   * when going from DCH to FACH due to reconfiguration message.
   * As per Spec OOS should be indicated after 12s of being 
   * out of service in CELL_PCH/URA_PCH.
   */
  #define  TDSRRCTMR_DCH_PCH_CELL_SELECTION_TIMER_IN_MS  12000UL


/* This defines the max time (default - 30 mins) for which Frequency scan should not be initiated */
#define  TDSRRCTMR_FREQ_SCAN_TIMER_IN_MS  1800000UL


/* This defines the max time for which service requests should not be entertained on finding no service
 * for a previous service request in deep sleep and empty available plmns.
 */
#ifdef FEATURE_DISABLE_DEEP_SLEEP 
#define  TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER_IN_MS  0
#else
#define  TDSRRCTMR_DEEP_SLEEP_NO_SVC_TIMER_IN_MS  10000UL
#endif


/* This defines the max time (default - 15 mins) for which BPLMN Frequency scan should not be initiated */
#define  TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER_IN_MS  900000UL
/* The same definition as TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER_IN_MS, but extend to 30 mins for SGLTE mode */
#define  TDSRRCTMR_BPLMN_FREQ_SCAN_TIMER_IN_MS_FOR_SGLTE 1800000UL


/* This defines the max number of attempts for which full frequency scan should be done 
 * after UE enters deep sleep. Full Scan timer is started after these number of attempts
 * are completed.
 */
#define  TDSRRC_MAX_FULL_SCANS_IN_DEEP_SLEEP 3

#define TDSMAX_BPLMN_FREQ_SEL_ATTEMPTS 4

/* This defines the max time (default - 30 seconds) for which Specific 
 * PLMN search should be done on both RATs 
 */
#define  TDSRRCTMR_CONN_MODE_OOS_TIMER_IN_MS  30000

/* This defines the timer PLMN seach should be done in fgnd on LTE or GSM */
#define TDSRRCTMR_TD2X_FGND_PLMN_SRCH_TIMER_IN_MS 60000

/* Maximum time allowed for completing background PLMN search */
#define  TDSRRCTMR_BPLMN_GUARD_SRCH_TIMER_IN_MS  120000UL

/* Maximum time (7s) allowed for completing td2td manual background plmn search */
#define  TDSRRCTMR_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER_IN_MS  7000


#define  TDSRRCTMR_DUAL_SIM_BPLMN_GUARD_SRCH_TIMER_IN_MS  180000UL

#ifdef FEATURE_TDSCDMA_TO_LTE  
#define TDSRRCTMR_LTE_BPLMN_GUARD_SRCH_TIMER_IN_MS 150000UL
#endif

/* This is the time substracted from the DRX search timer received from L1
 * to give enough time to L1 to configure PICH on last camped frequency so
 * as to demodulate the PI bits in the paging occasion frame. This overhead
 * is for TD2TD BPLMN search. 150ms for single SIM and 100ms for Multi-SIM
 */
#define  TD2TD_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS \
  (  \
    (tds_is_ue_in_dual_standby) ? 100 : 150\
  )


/* This is the minimum time of DRX timer left to try next cell during T2T BPLMN. or else do not try next cell on the same frequency. 
 * RRC will release lock if DRX timeout
 * soon after collect SIB for next cell.  This will cause TL1 wait lock too long to go to sleep in time 
 */
#define  TD2TD_BPLMN_SEARCH_TRY_NEXT_CELL_SIB_OVERHEAD_TIMER_IN_MS   80
/* This is the overhead to stop X2TD BPLMN search ahead of Srch timer expired.
 * It's to avoid RRC send acquisition or frequency scan request to L1 at the
 * timer closed to Srch timer expired.
 */
#define  X2TD_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS   150

/*
  WL1 cannot acquire lock on GO_TO_SLEEP_IND and WL1 not going to sleep.
  So additional time is substracted from DRX timer to make sure WL1 go to sleep and
  acquire lock on wake up
*/
#define  TDSRRC_MIN_WAIT_TIME_TO_L1   20
/* Before start T2T BPLMN, start lock wait timer for the whole DRX cycle.
 * lock wait timer length = available DRX time - necessary_ahead.
 * If DRX timeout soon after sending FREQ_SCAN_REQ/ACQ_REQ after getting lock, FREQ_SCAN_REQ/ACQ_REQ
 * may takes too long time so that send GO_TO_SLEEP_IND late to TL1;  Here add necessary ahead time to 
 * make sure there is enough time left after getting lock
 */
#define  TD2TD_BPLMN_SEARCH_WAIT_LOCK_NECESSAEY_AHEAD_TIMER_IN_MS   TD2TD_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS
/* Necessary ahead time for X2T BPLMN search*/
#define  X2TD_BPLMN_SEARCH_WAIT_LOCK_NECESSAEY_AHEAD_TIMER_IN_MS    2


/* This is the time substracted from the DRX search timer received from L1
 * to give enough time to L1 to configure PICH on last camped frequency so
 * as to demodulate the PI bits in the paging occasion frame. This overhead
 * is for TD2G BPLMN search
 */
#define  TD2G_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS   150

/* TD2TD BPLMN search during the DRX time will be triggered iff the DRX time
 * is at least equal to this value
 */
#define  MIN_TD2TD_BPLMN_SRCH_TIMER_IN_MS   300

/* This is the upper bound on DRX search time. If DRX search time
 * received from L1 exceeds this value it implies that there is 
 * some error 
 */
#define  MAX_TD2TD_BPLMN_SRCH_TIMER_IN_MS   5120

#ifdef FEATURE_BPLMN_SEARCH_320MS
/* Upto 640ms there is no page miss due to BPLMN searches. Lesser than that
 * there will be page misses if we intend do BPLMN searches. 
 */
#define MIN_TD2TD_NORMAL_BPLMN_DRX_CYCLE_LEN 64
#endif

/* This is the minimum DRX timer for X2TD BPLMN search. Lesser than that
 * special guard time or ACQ mode will be adpoted */
#define MIN_X2TD_NORMAL_BPLMN_DRX_TIMER_IN_MS  300

/* This is the time substracted from the BPLMN search timer received from GSM
 *  This overhead is for G2TD BPLMN search.
 */
#define G2TD_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS   225

/* This is the time substracted from the BPLMN search timer received from LTE
 *  This overhead is for LTOT BPLMN search.
 *  This value may be different from G2TD since L1 is taking more than 300ms to 
 *  ACQ due to spurious LTE freqs.
 */
#define L2TD_BPLMN_SEARCH_OVERHEAD_TIMER_IN_MS   225

/* Used for mapping GSM ARFCNs to UMTS Channel numbers */



#define TDSINVALID_LIST_TRANS_ID 0xFF

#define TDSJAPAN_MCC1 440
#define TDSJAPAN_MCC2 441
#define TDSJAPAN_MCC3 442
#define TDSJAPAN_MCC4 443

#define TDSUSA_MCC_START_RANGE 310
#define TDSUSA_MCC_END_RANGE   316
#define TDSUSA_MCC1   332



  #define TDSDEEP_SLEEP_NO_AGC_THRESHOLD_FREQ1 4387
  #define TDSDEEP_SLEEP_NO_AGC_THRESHOLD_FREQ2 4412
  #define TDSDEEP_SLEEP_NO_AGC_THRESHOLD_FREQ3 4436
  #define TDSDEEP_SLEEP_NO_AGC_THRESHOLD_MCC1 505
  #define TDSDEEP_SLEEP_NO_AGC_THRESHOLD_MNC1 1

  /* Initial Tmins period during which UE searches for service 
   * continuously without going to deep sleep
   */
  #define  TDSRRCTMR_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_IN_MS 120000

  /* Determines the duration for which UE should remain in deep
   * sleep
   */
  #define  TDSRRCTMR_CONN_MODE_OOS_DS_TIMER_IN_MS 30000

  /* Determines the time duration for which frequency should be barred due to LA reject 
   * received for the registration from the N/W with the cause "forbidden LAs for roaming"
   */
  #define TDSFORBIDDEN_RESEL_FREQ_BAR_TIMER_IN_SEC 300

#define TDSRRC_RESEL_PENALTY_TIMER_IN_SEC 3

  #define  RRCTMR_G2TD_REDIRECTION_WAIT_TIMER_IN_MS 10000
  #define  RRCTMR_G2TD_REDIRECTION_LFS_TIMER_IN_MS 2000
  #define TDSRRCTMR_GTOW_REDIRECTION_LFS_TIMER_IN_MS 2000

#define TDSRRC_MAX_NUM_FREQ 20


#define MAX_NUM_OF_TDS_BANDS 10


/* TD2G abort is supposed to complete in 2sec*/
#define RRCTMR_TD2G_ABORT_GUARD_TIMER_IN_MS 2000

#define TDSRRC_QTA_START_GUARD_TIMER_IN_MS 200
#define TDSRRC_QTA_STOP_GUARD_TIMER_IN_MS 100
#define TDSRRC_CSP_MAX_PLMN_SRCH_RATS 3

#define TDSRRC_CSP_INVALID_CPID  128

#ifdef FEATURE_WRLF_SYSTEM_SEL
#define TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()  (tdsrrc_csp_int_data.ue_in_conn_oos)
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
#define TDSRRCCSP_L2TD_MAX_ACQ_TRIES_ON_DIRECTED_FREQ 3
#define TDSRRCTMR_LTE_TO_TDSCDMA_REDIR_SRCH_TIMER_IN_MS 5000
#endif
#define TDSRRCTMR_DEFAULT_SEARCH_TIME_IN_MS 120000
#define TDSRRCTMR_NEGLECTABLE_SEARCH_TIME_IN_MS 3

#define TDSRRC_CSP_MAX_FPR_FREQ 9
/*haizhou 2012/9/25 define the max num of cells in tdsrrc_csp_bplmn_sib_fail_cell_list*/
#define TDSRRC_CSP_MAX_BPLMN_SIB_FAIL_CELL 10
/*haizhou 2012/9/25 define the max retry of tdsrrc_csp_bplmn_sib_fail_cell_list*/
#define TDSRRC_CSP_MAX_BPLMN_SIB_FAIL_RETRY  2

#define TDSRRC_CSP_RESEL_S_THRSHLD   5

#define TDSRRC_CSP_TDS_BANDWITH_IN_100KHZ    (16)

#define TDSRRC_CSP_SEL_RESEL_TIME_INVALID    (0xFFFFFFFF) 


#define TDSMAX_CS_DOMAIN_MO_ERROR_COUNT 1
#define TDSMAX_CS_DOMAIN_MT_ERROR_COUNT 2
#define TDS_CS_DOMAIN_BAR_TIME_LENGTH 43200
extern boolean tdsrrccsp_permit_cs_resel_evaluate_by_nas;
extern boolean tdsrrccsp_permit_ps_resel_evaluate_by_nas;

extern uint16 tds_cs_domain_bar_timer_length;

#ifdef FEATURE_TDSCDMA_DEV_SWAP_VIA_INTERSIB_SLEEP
extern boolean tdsrrccsp_acquire_lock_at_sib_cnf;
#endif  

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* This enum lists the source of target_tdd_cell_list comes from*/
typedef enum
{
  TDSRRC_CSP_TARGET_CELL_SOURCE_NONE,
  TDSRRC_CSP_TARGET_CELL_SOURCE_ACQDB_HIGH_CNT,
  TDSRRC_CSP_TARGET_CELL_SOURCE_REDIR_TARGET
} tdsrrc_csp_target_cell_source_e_type;

/* This enum lists the possibilities for Cell barred status */
typedef enum
{
  /* This indicates that the cell is not barred and UE may select/reselect
   * in Idle or Connected mode.
   */
  TDSRRC_CSP_CELL_OK,

  /* This indicates that UE shall treat this cell as Barred and CAN NOT
   * select another cell on the same frequency.
   */
  TDSRRC_CSP_CELL_BAR_NO_INTRA_FREQ,

  /* This indicates that UE shall treat this cell as Barred and CAN
   * select another cell on the same frequency.
   */
  TDSRRC_CSP_CELL_BAR_INTRA_FREQ
}tdsrrc_csp_cell_bar_e_type;



/* This enum lists the possibilities for Cell Access restrictions */
typedef enum
{
  /* This indicates all kinds of Cell Access are allowed */
  TDSRRC_CSP_CELL_ACCESS_ALLOWED,
  /* This indicates that cell access is not allowed for any regular call. It
   * is however allowed for emergency calls.
   */
  TDSRRC_CSP_NO_REGULAR_CELL_ACCESS,
  /* This indicates Cell Access is NOT allowed for ANY call (Regular or
   * Emergency.
   */
  TDSRRC_CSP_NO_CELL_ACCESS,
  /* This indicates that cell access is allowed for regular calls but not
   * for emergency calls
   */
  TDSRRC_CSP_REGULAR_CELL_ACCESS_ONLY

}tdsrrc_csp_access_control_e_type;


/* This enum lists the possibilities of LAI forbidden status */
typedef enum
{
  /* This indicates that this is a Forbidden LAI */
  TDSRRC_CSP_FORBID_LAI,
  /* This indicates that LAI is NOT forbidden */
  TDSRRC_CSP_LAI_OK
}tdsrrc_csp_lai_status_e_type;


/* This enumerates all possible primitives exchanged between MM
 * and RRC Cell selection that can have a bearing on the next
 * RRC to MM Primitive.
 */
typedef enum
{
  TDSRRC_CSP_NO_EVENT_RCVD,
  TDSRRC_CSP_ACT_REQ_RCVD,
  TDSRRC_CSP_SRVC_REQ_RCVD,
  TDSRRC_CSP_SRVC_CNF_WITH_NO_SRVC_SENT,
  TDSRRC_CSP_SRVC_CNF_WITH_SRVC_SENT,
  TDSRRC_CSP_ACT_CNF_SENT,
  TDSRRC_CSP_SRVC_IND_WITH_NO_SRVC_SENT,
  TDSRRC_CSP_SRVC_IND_WITH_SRVC_SENT,
  TDSRRC_CSP_PLMN_LIST_REQ_RCVD
}tdsrrc_csp_mm_primitive_e_type;


/* This enumerates the possible frequency scan types */
typedef enum
{
  TDSRRC_CSP_NO_SCAN,
  TDSRRC_CSP_RAW_SCAN,
  TDSRRC_CSP_FINE_SCAN,
  TDSRRC_CSP_ACQ_SCAN,
  TDSRRC_CSP_LAST_ASET_SCAN,   /* Used for determining if acquisition should be attempted
                              on the best cell in the active set for DCH->Idle transition before
                              trying ACQ DB */
  TDSRRC_CSP_GSM_SCAN
  , TDSRRC_CSP_LIST_SCAN
}tdsrrc_csp_freq_scan_e_type;


/* Enumerates the various scan types */
typedef enum
{
  /* 0: Scan triggered by one of MM commands (Service Req/Act Req etc) */
  TDSRRC_CSP_CELL_SELECTION_SCAN,

  /* 1: Scan triggered by Reselection Indication */
  TDSRRC_CSP_CELL_RESELECTION_SCAN,

  /* 2: Scan triggered by Out of Service Indication */
  TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN,

  /* 3: Scan triggered by Out of Service Indication in Idle state */
  TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN,

  /* 4: Scan triggered by Out of Service Indication in Idle state */
  TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN,

  /* 5: Scan triggered when leaving CELL_DCH state for CELL_FACH state */
  TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN,

  /* 6: Scan triggered when leaving CELL_DCH state for CELL_PCH state */
  TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN,

  /* 7: Scan triggered when leaving CELL_DCH state for CELL_PCH state */
  TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN,

  /* 8: Scan triggered on leaving connected mode */
  TDSRRC_CSP_LEAVING_CONN_MODE_SCAN,

  /* 9: Scan triggered by GSM RR */
  RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN,

  /* 10: Scan to GSM triggered by TDS */
  RRC_CSP_TDS_TO_GSM_CELL_RESELECTION_SCAN,

  /* 11: Scan triggered by PLMN_LIST_REQ */
  TDSRRC_CSP_PLMN_LIST_SCAN,

  /* 12: Scan triggered by GSM RR for Packet Cell Change Order to UTRAN */
  RRC_CSP_G2TD_CC_ORDER_SCAN,

  /* 13: Scan triggered by GSM RR for Automatic Background PLMN search */
  RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN,

  /* 14: Scan triggered by GSM RR for Manual PLMN search */
  RRC_CSP_G2TD_BPLMN_MANUAL_SCAN,

  /* 15: Scan triggered by Inter Frequency Redirection request from RCE */
  TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN,

  /* 16: Scan triggered by TDS RRC for WTOG service redirection */
  TDSRRC_CSP_INTER_RAT_REDIRECTION_SCAN,

  /* 17: Scan triggered by MM for 3G->3G Automatic Background PLMN search */
  RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN,

  /* 18: Scan triggered by MM for 3G->3G Manual PLMN search */
  RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN,

  /* 19: Scan triggered by MM for 3G->2G Automatic Background PLMN search */
  RRC_CSP_TD2G_BPLMN_AUTOMATIC_SCAN,

  /* 20: Scan triggered by MM for 3G->2G Manual PLMN search */
  RRC_CSP_TD2G_BPLMN_MANUAL_SCAN,

  /* 21: Scan triggered when leaving CELL_FACH state for CELL_FACH state */
  TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN,

  /* 22: Scan triggered when leaving CELL_FACH state for CELL_PCH state */
  TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN,

  /* 23: Scan triggered when leaving CELL_FACH state for URA_PCH state */
  TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN,

  /* 24: Reselection to GAN Scan*/
  RRC_CSP_TDS_TO_GAN_CELL_RESELECTION_SCAN,

  /* 25: Scan triggered when leaving CELL_FACH state for URA_PCH state */
  RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN,

  /* 26: Scan triggered for reselection from GAN request*/
  RRC_CSP_GAN_TO_TDS_RESELECTION_SCAN,

  /* 27: Go to Idle Detached mode scan*/
  TDSRRC_CSP_GO_TO_IDLE_DETACHED_SCAN,

  /* 28: GAN ACT REQ Scan*/
  TDSRRC_CSP_GAN_ACT_REQ_SCAN,

  /* 29: Scan triggered by G2T CCO or L2T PSHO */
  RRC_CSP_GSM_TO_TDS_PSHO_SCAN,

  /* 30 UMTS UNLOCK BY Scan */
  TDSRRC_CSP_UNLOCK_BY_SCAN
}tdsrrc_csp_scan_e_type;

/* Enumerates various types of pending commands */
typedef enum
{
  TDSRRC_CSP_SERVICE_REQ_PENDING,
  TDSRRC_CSP_DCH_TO_FACH_TIMER_PENDING,
  TDSRRC_CSP_PLMN_LIST_REQ_PENDING,
  TDSRRC_CSP_STATE_CHANGE_IND_PENDING,
  TDSRRC_CSP_DCH_TO_PCH_TIMER_PENDING,
  TDSRRC_CSP_INITIATE_CELL_SEL_PENDING,
  TDSRRC_BPLMN_SRCH_TIMER_PENDING,
  TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ_PENDING,
  TDSRRC_FREQ_REDIRECT_WAIT_TIMER_PENDING,
  TDSRRC_REDIRECT_REQ_PENDING, 
  RRC_CSP_TD2TD_BPLMN_DRX_SRCH_TIMER_PENDING,
  TDSRRC_CSP_BPLMN_GUARD_SRCH_TIMER_PENDING,
  TDSRRC_CSP_BPLMN_SRCH_SUSPEND_REQ_PENDING,
  TDSRRC_CSP_BPLMN_SRCH_ABORT_REQ_PENDING,
  TDSRRC_CSP_FACH_TO_FACH_PCH_TIMER_PENDING,
  TDSRRC_CSP_PREEMPT_IND_PENDING,
  TDSRRC_CSP_MODE_CHANGE_REQ_PENDING,

  TDSRRC_CSP_NONE_PENDING
}tdsrrc_csp_pending_e_type;

/* Defines all Substates of Cell Selection Procedure. */

typedef enum{
  /* 0: No cell is selected */
  TDSRRCCSP_NO_CELL_SELECTED,
  /* 1: Waiting for frequency scan */
  TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF,
  /* 2: Waiting for frequency scan 
        while a Service Request is pending to be serviced */
  TDSRRCCSP_CELL_SEL_WT_FREQ_SCAN_CNF_PENDING,
  /* 3: Waiting for Acq. Confirm */
  TDSRRCCSP_CELL_SEL_WT_ACQ,
  /* 4: Waiting for Acq. Confirm
        while a Service Request is pending to be serviced */
  TDSRRCCSP_CELL_SEL_WT_ACQ_PENDING,
  /* 5: Waiting for SIB Confirm */
  TDSRRCCSP_CELL_SEL_WT_SIB,
  /* 6: Waiting for SIB Confirm */
  TDSRRCCSP_CELL_SEL_WT_SIB_PENDING,
  /* 7: Waiting for State Change Indication to IDLE */
  TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE,
  /* 8: Waiting for State Change Indication while a MM request is pending */
  TDSRRCCSP_CELL_SEL_WT_STATE_CHANGE_PENDING,
  /* 9: Suitable cell selection waiting for Serving cell "S values" */
  TDSRRCCSP_SUIT_CELL_SEL_WT_SERV_S,
  /* 10: Suitable cell selection waiting for Serving cell "S values"
         while a Service Request is pending to be serviced */
  TDSRRCCSP_SUIT_CELL_SEL_WT_SERV_S_PENDING,
  /* 11: Suitable cell selection waiting for Camping Confirm */
  TDSRRCCSP_SUIT_CELL_SEL_WT_CAMP,
  /* 12: Suitable cell selection waiting for Camping Confirm
         while a Service Request is pending to be serviced */
  TDSRRCCSP_SUIT_CELL_SEL_WT_CAMP_PENDING,
  /* 13: Camped on Suitable Cell */
  TDSRRCCSP_CAMPED_SUIT_CELL,
  /* 14: Acceptable cell selection waiting for "S" Value */
  TDSRRCCSP_ACCEPT_CELL_SEL_WT_S,
  /* 15: Acceptable cell selection waiting for "S" Value
         while a Service Request is pending to be serviced */
  TDSRRCCSP_ACCEPT_CELL_SEL_WT_S_PENDING,
  /* 16: Acceptable cell selection waiting for Camping Confirm */
  TDSRRCCSP_ACCEPT_CELL_SEL_WT_CAMP,
  /* 17: Acceptable cell selection waiting for Camping Confirm
         while a Service Request is pending to be serviced */
  TDSRRCCSP_ACCEPT_CELL_SEL_WT_CAMP_PENDING,
  /* 18: Camped on Acceptable Cell */
  TDSRRCCSP_CAMPED_ACCEPT_CELL,
  /* 19: Suitable cell reselection waiting for SIB Confirm */
  TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB,
  /* 20: Acceptable cell reselection waiting for SIB Confirm */
  TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB,
  /* 21: Suitable cell reacquisition */
  TDSRRCCSP_SUIT_CELL_REACQ,
  /* 22: Acceptable cell reacquisition */
  TDSRRCCSP_ACCEPT_CELL_REACQ,
  /* 23: Suitable cell reselection waiting for Camping Confirm */
  TDSRRCCSP_SUIT_CELL_RESELECT_WT_CAMP,
  /* 24: Acceptable cell reselection waiting for Camping Confirm */
  TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_CAMP,
  /* 25: Suitable cell reselection waiting for SIB Confirm
         while a Service Request is pending to be serviced */
  TDSRRCCSP_SUIT_CELL_RESELECT_WT_SIB_PENDING,
  /* 26: Suitable cell reselection waiting for Camping Confirm
         while a Service Request is pending to be serviced */
  TDSRRCCSP_SUIT_CELL_RESELECT_WT_CAMP_PENDING,
  /* 27: Acceptable cell reselection waiting for SIB Confirm
         while a Service Request is pending to be serviced */
  TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_SIB_PENDING,
  /* 28: Acceptable cell reselection waiting for Camping Confirm
         while a Service Request is pending to be serviced */
  TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_CAMP_PENDING,
  /* 29: Waiting for Suspend Mode Change Confirm */
  TDSRRCCSP_WT_SUSPEND_MODE_CHANGE_CNF,
  /* 30: Waiting for Resume Mode Change Confirm */
  TDSRRCCSP_WT_RESUME_MODE_CHANGE_CNF,
  /* 31: Waiting for Resume Mode Change Confirm in pending */
  TDSRRCCSP_WT_RESUME_MODE_CHANGE_CNF_PENDING,
  /* 32: Waiting for channel config confirm after W resume has succeeded */
  TDSRRCCSP_WT_FOR_CHAN_CFG_CNF,
  /* 33: Waiting for channel config confirm after W resume has succeeded 
   *     and TDSRRC_SERVICE_REQ or PLMN_LIST_REQ is received in CSP substate
   *     TDSRRCCSP_WT_FOR_CHAN_CFG_CNF */
  TDSRRCCSP_WT_FOR_CHAN_CFG_CNF_PENDING,
  /* 34: CSP waits for TDSRRC_CPHY_CELL_TRANSITION_CNF from L1 */
  TDSRRCCSP_SUIT_CELL_RESELECT_WT_TRANS_CNF,
  /* 35: CSP waits for TDSRRC_CPHY_CELL_TRANSITION_CNF from L1 for accept camp */
  TDSRRCCSP_ACCEPT_CELL_RESELECT_WT_TRANS_CNF,
  /* 36:  */
  TDSRRCCSP_WT_STOP_MODE_CHANGE_CNF,
  /* 37:  */
  TDSRRCCSP_SUIT_CELL_WT_ACTIVATION_RSP,
  /* 38:  */
  TDSRRCCSP_ACCEPT_CELL_WT_ACTIVATION_RSP,
  /* 39:  */
  TDSRRCCSP_CELL_SEL_WT_HANDOVER_ACQ,
  /* 40: CSP waits for TDSRRC_LOWER_LAYER_DEACT_CNF from LLC */
  TDSRRCCSP_WT_LOWER_LAYER_DEACT_CNF,
  /* 41: CSP waits for Redirection response from RR */
  TDSRRCCSP_WT_RESPONSE_FROM_RR,
  /* 42: CSP waits for abort cnf from RR for redirection or reselection */
  TDSRRCCSP_WT_RR_ABORT_CNF,
  /* 43:  */
  TDSRRCCSP_WT_IDLE_DETACHED_MODE_CHANGE_CNF,
  /* 44: Maximum number of substates of Cell selection procedure */
  TDSRRCCSP_CSP_MAX_SUBSTATES

}tdsrrc_csp_substate_e_type;


/* Used by RRC to send appropriate command to RR in response to 
 * reselections, aborts, HO, and CCO 
 */
typedef enum
{
  /* Used by RRC to send appropriate reselection reject command */
  SEND_G2TD_RESEL_REJECT,

  /* Used by RRC to send appropriate reselection abort confirmation command */
  SEND_G2TD_RESEL_ABORT_CNF

  /* Used by RRC to send appropriate redirection reject command */
  ,SEND_G2TD_REDIRECTION_REJECT

  /* Used by RRC to send appropriate redirection abort confirmation command */
  ,SEND_G2TD_REDIRECTION_ABORT_CNF
}tdsrrc_csp_rsp_to_rr_e_type;


/* Used for initializing the variables associated with 3G->3G,2G background
 * PLMN Search during the search.
 */
typedef enum
{
  /* Automatic PLMN LIST REQ received from NAS          */
  TD2TDSEARCH_START,    

  /* DRX search timer expired or some other procedure
   * wants to become active                             */
  TD2TD_SEARCH_SUSPEND,  

  /* Continue the search                                */
  TD2TD_SEARCH_RESUME,    

  /* PLMN LIST Search completed                         */
  TD2TD_SEARCH_STOP,     

  /* Guard timer expired or state changed to DCH or OOS */
  TD2TD_SEARCH_ABORT
  
#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY  
  /*Provide Partial PLMN Search Results to NAS*/
  ,TD2TD_SEARCH_PARTIAL
#endif  
} td2td_bplmn_srch_status_type;



/* This is used between CSP functions for passing SIB bit mask */
typedef  uint32  tdsrrc_csp_sib_mask_type;

typedef struct
{
  /*Number of RATS*/
  uint8 num_rats;
  sys_radio_access_tech_e_type rat[TDSRRC_CSP_MAX_PLMN_SRCH_RATS];
  int8 curr_rat_srch_idx;
}tdsrrc_csp_plmn_srch_rat_order_type;

typedef struct
{
  tdsrrc_freq_type  freq;
  tdsrrc_cpid_type   cpid;
  rrc_plmn_identity_type  plmn_id;
  uint8 failed_count;
  boolean is_MIB_recv;
}tdsrrc_csp_bplmn_sib_fail_cell_type;

typedef struct
{
  uint8 num_cell;
  tdsrrc_csp_bplmn_sib_fail_cell_type bplmn_sib_fail_cell[TDSRRC_CSP_MAX_BPLMN_SIB_FAIL_CELL];  
}tdsrrc_csp_bplmn_sib_fail_cell_list_type;

typedef struct
{
  /* Pseudo fast return feature is enable or not */
  boolean pseudo_fr_active;

  /* Indicate if PFR G2TD redirection is ongoing or not.
   * TRUE means RRC is handling G2TD_REDIRECTION_REQ with PFR info
   */
  boolean pfr_redirect_ongoing;

  /* The RSCP threshold of the cells to be viewed as valid for 
   * pseudo fast return
   */
  uint8 rscp_threshold;

  /* Number of configured freuency for FPR. Can be 1~9 */
  uint8 num_freq;

  uint16 freq_list[TDSRRC_CSP_MAX_FPR_FREQ];
}tdsrrc_csp_interrat_pseudo_fr_info_type;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
#ifdef FEATURE_USE_MM_MANUAL_SEARCH_TIMER_VALUE_TYPE
typedef mm_manual_search_timer_value_type tds_manual_search_timer_value_type;
#else
typedef sys_manual_search_timer_value_type tds_manual_search_timer_value_type;
#endif
#endif

/* This data structure stores all current global data used internally by Cell
 * Selection. Most of this data comes in commands from MM and from SIBs.
 */
typedef struct
{
  /* Last Cell selection event - determines next MM response */
  /* Note that this is updated at the time of sending an MM command and at
   * the time of receiving a command from MM.
   */
  tdsrrc_csp_mm_primitive_e_type    last_mm_primitive;

  /* The current scan type */
  /* Note that this is updated when a scan is triggered by an event.
   */
  tdsrrc_csp_scan_e_type          curr_scan;

  /* The Home PLMN for this UE */
  /* TBD RRC has to call an MM function that returns the Home PLMN.
   */
  rrc_plmn_identity_type    home_plmn_id;

  /* List of Forbidden LAIs */
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  rrc_lai_list_type         forbid_lai_list[TDSCDMA_NUM_SUBS];
#else
  rrc_lai_list_type         forbid_lai_list;
#endif

  /* List of Reject LAIs */
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  lai_reject_list_T         lai_reject_list;

  boolean                   sm_ok_to_tuneaway;
#endif

  /* Current PLMN requested by MM in Service Req. message */
  rrc_requested_plmn_type   curr_req_plmn;

  mm_as_service_state_T       curr_service_status;

  /* Current PLMN selected by RRC.
   * It is valid once UE camps on the cell or is unable to camp on any cell.
   */
  rrc_selected_plmn_type    curr_sel_plmn;

  /* Current type of acq.- This indicates the expected acq. type.
   */
  tdsrrc_csp_acq_e_type        curr_type_acq;

  /* Current type of acq. - calculated based on OTA SIBs */
  tdsrrc_csp_acq_e_type        curr_type_acq_ota;

  /* Current Acq entry - stores all data for the current acquisition.
   */
  tdsrrc_csp_acq_entry_type    curr_acq_entry;

  /* New Cell Acq entry - stores all data for the new cell to be reselected.
   * The cell is provided by L1 in the RESELECTION_IND
   */
  tdsrrc_csp_acq_entry_type    new_cell_acq_entry;

  /* This stores parameters for serving SCCPCH */
  tdsl1_dl_phycfg_pccpch_parms_struct_type   serving_pccpch_parms;

  /* This indicates the type of pending command */
  tdsrrc_csp_pending_e_type   pending_type;
  /* Service Request received and still pending */
  rrc_service_req_type     pending_srvc_req_data;

  rrc_plmn_list_req_type   pending_plmn_list_req_data;


  /* Avaliable PLMN list */
  rrc_avail_plmn_list_type  plmn_list;
  
  tdsrrc_csp_bplmn_sib_fail_cell_list_type  bplmn_sib_fail_cell_list;
  
  tdsrrc_cmd_type              saved_rrc_cmd;

  int16                        s_rxlev;


  /* Params needed to be set while sending a INTERRAT_RESELECT_REJ to RR or RESEL_FAILED_RSP to LTE RRC */

  /* Cause of InterRAT Reselection Failure */
  tdsrrc_interrat_reject_reason_T   interrat_reselect_failure_reason;

  /* Time ( in Sec ) when the same cell should be considered */
  uint32                         tds_cell_time_to_reselect;

  /* Thresholds to be applied to Ec/No measurements */
  int16                         tds_cell_squalmin; /* FDD only, not used by TDD */
  
  int16                         tds_cell_srxlevmin;

  rrc_lai_type                  forbidden_lai;

  /* indicates current l2t redirection is rel-9 or rel-10 (multi-freq) */
  boolean                       l2t_r9_redirect;
  boolean                        dest_freq_present;
  tdsrrc_freq_type                  dest_freq;
  boolean                        dest_cpid_present;
  tdsrrc_cpid_type              dest_cpid;

  /* PN position of the cell in chipx1 units. */
  uint32                   pn_pos;

  /* Number of other scrambling codes received in ACQ_CNF */
  uint8                    num_cpid_det;

  tdsl1_acq_scr_code_det      cpid_det[TDSL1_NUM_CPID_DETECTED_MAX];

  /* Current Acquisition count */
  uint8                    curr_acq_count;

  tdsl1_acq_mode_enum_type   acq_mode;
  /* Frequency scan status */
  tdsl1_acq_fs_enum_type acq_fs_status;

  /* The flag is to force use shallow mode in next CPHY_ACQ_REQ. It's set
   * when DL_WEAK_IND occurs while reading SIBs in cell selection proc
   * (not include BPLMN scenario).
   * This is to make sure L1 do acquisition instead of measurement in next ACQ
   * although it stays in SYNC state, because DL_WEAK_IND implies the 
   * timing in L1 is unreliable
   */
  boolean                  use_shallow_mode_in_next_acq;

  /* Indicates how a PLMN is to be selected. */
  sys_network_selection_mode_e_type network_select_mode;

  /* User band preference TDS band only */
  sys_band_mask_type       band_pref;

  /* User band preference for G and W (TDS and G/W use different 64-bit mask) */
  sys_band_mask_type       band_pref_gw;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  /* User band preference, used for BST support */
  sys_band_mask_type       prev_band_pref;
#endif

  /* Indicates if full frequency scan is needed or not */
  boolean                  full_scan_needed;
  /* boolean indicating if UE is in deep sleep or not */
  boolean                  ue_in_deep_sleep;

  /* boolean indicating if UE's deep sleep no service timer has expired or not */
  boolean                  deep_sleep_no_svc_tmr_expired;

  /* boolean used for state keeping for multiple calls to tdsrrccsp_get_match_acq_entry() function */
  boolean                  deep_sleep_acq_db_plmn_searchloop;

  /* boolean indicating if need to do additional Deep ACQ to specific band */
  boolean                  deep_acq_needed;

  /* if UE is in deep sleep then do full scan till the counter reaches a maximum value */
  uint8                    full_scan_in_deep_sleep_counter;
  /* When UE is in deep sleep and full scan timer expires then
   * at least 1 scan should be done on each supported band.
   * Since SD sends different bands as supported in different
   * service requests, this mask maintains a list of bands that
   * have been scanned thus guaranteeing 1 full scan on each supported
   * band on timer expiry.
   */
  tdsrrc_csp_band_class_type  deep_sleep_band_scanned_mask;
  
  /* This mask maintains the bands that have been serviced since
   * the start of No Service timer. The reason for introducing this
   * mask is that RRC may get service request for a band that is different
   * from the ones received earlier when the timer was started/active.
   * Such bands which have not been serviced since the start of no service
   * timer should be serviced and hence the need for this mask.
   */
  tdsrrc_csp_band_class_type  deep_sleep_no_svc_band_scanned_mask;

  /* Variable indicating if ACQ DB is empty - For specific PLMN request this
   * variable is set to TRUE if no matching entry was found. For ANY PLMN it is
   * set to TRUE if there are no entries in the database at all.
   */
  boolean                  acq_db_empty;

  /* boolean indicating if an INITIATE_CELL_SELECTION_IND (to disconnected)
   * while CSP was waiting for SUSPEND_CNF or RESUME_CNF
    * for a WTOInterRAT cell reselection.
   */
  boolean                  init_cell_sel_ind_during_wtointerrat;

  /* boolean indicating if an INITIATE_CELL_SELECTION_IND (to disconnected)
   * while CSP was waiting for STOP_MODE_CHANGE_CNF.
   */
  boolean                   init_cell_sel_ind_during_stop_mode_change;

  /* For TD-SCDMA multiband support, maintain a bit mask of supported
   * frequency bands
   */
  tdsrrc_csp_band_class_type  tds_band_mask;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  /* used for BST support */
  tdsrrc_csp_band_class_type  bplmn_band_mask;
  /* used for BST support */
  tdsrrc_csp_band_class_type  prev_tds_band_mask;
#endif

  /* This field is used for additional acquisitions on the last camped frequency
   * in CONNECTED mode during full frequency scan
   */
  tdsrrc_freq_type            last_camped_freq_in_connected_mode;

  /* This field is used for additional acquisitions on the last camped frequency
   * in IDLE mode during full frequency scan
   */
  tdsrrc_freq_type            last_camped_freq_in_idle_mode;

   /*-----------------------------*/
  /* BPLMN data 2G->3G searches  */
  /*-----------------------------*/
  
  /* determines if BPLMN search request is new or continued when bplmn search request
   * is received from RR. Default value TRUE 
   */
  boolean bplmn_new_srch;

  /* determines if BPLMN timer has expired or not. Default value TRUE */
  boolean bplmn_srch_tmr_expired;


  /* PLMN Ids to be searched on 3G with 0th element being
   * the highest priority PLMN.
   * Points to the PLMN that is currently being read from the BPLMN search
   * request list.
   */
   
  uint32    bplmn_req_list_rd_index;

  /* Used to determine if current_band has been initialized or not during BPLMN
   * search. If the current_band has been initialized then it will not be
   * set again when ACQ DB is scanned from the beginning multiple times bcoz
   * of multiple PLMN entries in the HPLMN list. This is done to ensure that
   * full freq scan is started on the most recent "band entry" in the ACQ DB.
   * This variable was added bcoz of CR 104768.
   *
   * FALSE: initial value. implies that current_band has NOT been initialized
   * TRUE:  otherwise. 
   */
  boolean   bplmn_current_band_initialized;

  /* Numbers of plmns requested by RR/NAS to do background search on 
   *   "0"  implies manual plmn search.
   * "> 0"  implies automatic plmn search.
   */
  uint32    bplmn_num_req;

  /* BPLMNs found are added to the list at the index given by the
   * this variable. The list is sent by RRC to RR.
   */
  uint32    bplmn_fnd_list_wr_index;

  /* Determines if acquisition is part of SIB procedure, i.e., intra-freq 
   * cell acquistion. If part of SIB proc then acq db read index is not
   * modified. Default value FALSE;
   */
  boolean bplmn_sib_try_next_best_cell;

  /* PLMNs requested by RR for background search */
  rrc_requested_plmn_type   bplmn_req_list[SYS_PRIORITY_PLMN_LIST_MAX_LENGTH];

  /* RATs associated with the PLMNs in the higher priority PLMN list received from NAS
   * are stored in this array
   */ 
  sys_radio_access_tech_e_type bplmn_rat_req_list[SYS_PRIORITY_PLMN_LIST_MAX_LENGTH];

#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
  /* Upper layers use scan_scope to indicate ACQ_DB only search */
  sys_scan_scope_e_type srvc_req_scan_scope;

  sys_scan_scope_e_type bplmn_scan_scope;
#endif

  /* Confirmation sent by RRC to RR in response to PLMN Search */
  tdsrrc_interrat_plmn_srch_cnf_type rrc_rr_plmn_srch_cnf;

  /* Determines if tdsrrccsp_send_bplmn_search_cnf_to_rr function should
   * be called or tdsrrccsp_send_bplmn_abort_cnf_to_rr should be called.
   * Initial value = TRUE implies function tdsrrccsp_send_bplmn_search_cnf_to_rr
   * should be called.
   */
  boolean bplmn_srch_cnf;

  /* Determines if acquisition on the freq. succeeded or not when waiting
   * for acquisition confirmation and the BPLMN timer expires.
   */
  boolean bplmn_acq_status;

  /* Determines if full freq scan is needed for PLMN srch. Initial value
   * is TRUE.
   */
  boolean plmn_list_freq_scan_needed;


  /* Determines when MIB should be read in foreground. If the value 
   * of the variable reaches (or exceeds) TDSMAX_BPLMN_FREQ_SEL_ATTEMPTS then MIB
   * should be read in foreground else in background.
   */
  uint8   bplmn_bgnd_mib_rd_cnt;

  /* The variable above is incremented over one frequency which is
   * stored in this variable. I.e., if consecutive attempts to read
   * MIB on the frequency stored in this variable fail 
   * TDSMAX_BPLMN_FREQ_SEL_ATTEMPTS number of times then MIB should be 
   * read in foreground.
   */
  tdsrrc_freq_type bplmn_bgnd_mib_rd_freq;
  
  /* ---------------------------------------------------------- */ 
  /* Variables declared for Inter-Frequency Redirection Feature */
  /* ---------------------------------------------------------- */

  /* Frequency camped on when redirect request was received by CSP from RCE */
  tdsrrc_freq_type  last_camped_freq;

  /* Designated frequency in rrc inter-frequency redirect request */
  tdsrrc_freq_type  redirected_freq;

  /* Designated cpid in rrc inter-frequency redirect request */
  tdsrrc_cpid_type redirected_cpid;

  /* Wait time in rrc redirect request */
  uint32  freq_redirect_wait_time;

  /* Boolean for wait timer expiry. 
   * TRUE (wait timer expired) & FALSE (otherwise)
   */
  boolean start_acq_on_last_camped_freq;

  /* Determines if acquisition attempt on redirected frequency is the first one.
   * TRUE (first acquisition on redirected frequency) & FALSE (otherwise)
   */
  boolean first_acq_on_redirected_freq;

  /* This is used to inform L1 if the GSM cell is barred, or TDS 
   * cell is barred, or TDS frequency is barred, or the active
   * set is barred
   */
  tdsrrc_csp_cell_bar_info_e_type cell_bar_info;

  /* Used by CSP to determine if interrat_reselection_rej or
   * interrat_abort_cnf should be sent to GSM-RR on receiving 
   * stop confirmation from MCM 
   */
  tdsrrc_csp_rsp_to_rr_e_type rsp_to_rr;

  /* These two variables contain the scrambling code and pn position of the best cell in the 
   * active set if Connection Release Complete was sent to the network. Otherwise, this variable
   * always contains an invalid value of TDSRRC_CSP_INVALID_CPID.
   */
  uint16 aset_cpid_code;
  uint32 aset_pn_pos;

  /* OOS Handling in Connected Mode */

  /* Used for 30s specific PLMN search OOS timer. TRUE value implies that 
   * the 30s timer has expired. FALSE value implies that the 30s timer has
   * not expired. Initial value is TRUE.
   */
  boolean oos_splmn_srch_tmr_expd;

  /* Determines if GSM RAT has been searched or not. TRUE value implies that
   * at least 1 search has been completed on GSM. FALSE value implies otherwise.
   * Initial value is FALSE.
   */
  boolean oos_gsm_srch_done;

  /* Contains the number of PLMNs found on GSM RAT when specific PLMN
   * search was requested on GSM. If the variable has a value > 0 and
   * 30s timer has expired, then search is continued on GSM for ANY PLMN
   */
  byte  oos_gsm_num_avail_plmns;

  /* stores current cell access status */
  mm_as_cell_access_status_T curr_cell_access_status;

  /* boolean that says current DSAC info is valid(or) not */
  boolean is_curr_dsac_info_valid;

  /* boolean varible that says prev DSAC info is valid(or) not */
  boolean is_prev_dsac_info_valid;

  /* boolean that says DSAC info is valid(or) not */
  boolean  is_dsac_valid;
  
  /* stores cs cell access based on DSAC info */
  mm_as_cell_access_status_T     curr_cs_cell_access;

  /* stores ps cell access based on DSAC info */
  mm_as_cell_access_status_T     curr_ps_cell_access;
  
  /* ------------------------------------------ */
  /* Optimizing service search on PCS/850 bands */
  /* ------------------------------------------ */
  boolean gsm_svc_srch_done;

  /* ------------------------------------------ */
  /* NV Frequency Scan                          */
  /* ------------------------------------------ */
  /* Read from NV. TRUE implies that frequency in NV
   * should be read */
  boolean       tds_special_freq_enabled;

  /* Read from NV. Frequency on which acquisition should
   * be attempted before doing ACQ-DB/Full-Scan */
  tdsrrc_freq_type tds_special_freq;
  
  /*--------------------------------------*/
  /* BPLMN variables for 3G->3G searches  */
  /*--------------------------------------*/

  /* Determines whether PLMN search should also be done on GSM
   * RAT or not.
   * TRUE: Do PLMN search on GSM RAT also
   * FALSE (default): otherwise.
   */
  boolean  bplmn_dual_mode_enabled;

  /* This flag is set to TRUE by default. It indicates whether
   * search has to be done on TDSCDMA RAT or not
   */
  boolean  bplmn_tdscdma_mode_enabled;

  /* Stores the nw select mode to determine if Automatic PLMN list
   * was sent by NAS or Manual PLMN list request 
   */
  sys_network_selection_mode_e_type bplmn_nw_sel_mode;

  /* stores the status of the BPLMN search variables */
  td2td_bplmn_srch_status_type bplmn_srch_vars_status;

  /* Determines if GUARD timer for 3G->3G BPLMN search has expired (TRUE) or not (FALSE).
   * Initial value is TRUE.
   */
  boolean             bplmn_guard_srch_tmr_expired;

  /* Determines if guard timer for 3G->3G manual bplmn search has expired (TRUE) or not (FALSE).
   * Initial value is TRUE.
   */
  boolean             td2td_manual_bplmn_guard_srch_tmr_expired;
  /* Remaining time value of td2td_manual_bplmn_guard_srch_tmr*/
  uint32  td2td_manual_bplmn_guard_srch_timer_remaining; 

  /* TDS band only */
  sys_band_mask_type    bplmn_band_pref;

  /* Band preference for G and W (TDS and G/W use different 64-bit mask) */
  sys_band_mask_type    bplmn_band_pref_gw;

  /* Stores the original CSP scan type before changing it to BPLMN search scan type */
  tdsrrc_csp_scan_e_type prev_curr_scan;

  /* Previous PLMN requested by MM in Service Req. message */
  rrc_requested_plmn_type   prev_curr_req_plmn;

  /* Stores the original CSP acq entry type before changing it to BPLMN search acq entry type */
  tdsrrc_csp_acq_entry_type    prev_curr_acq_entry;

  /* Stores the original type of acq before starting the BPLMN search */
  tdsrrc_csp_acq_e_type        prev_curr_type_acq;

  /* Indicates how a PLMN is to be selected - Manual or Automatic. */
  sys_network_selection_mode_e_type prev_network_select_mode;

  /* This variable is set to MM_AS_NO_SERVICE when BPLMN search is 
   * started. Hence the need to save the original value of this var
   */
  mm_as_service_state_T       prev_curr_service_status;

  /* This variable is set to TDSRRC_CSP_NO_EVENT_RCVD when BPLMN search is 
   * started. Hence the need to save the original value of this var
   */
  tdsrrc_csp_mm_primitive_e_type    prev_last_mm_primitive;

  /* Stores the identity of the procedure that sent the suspend request */
  tdsrrc_proc_e_type            bplmn_suspend_proc;

  /* Stores the suspend cause in suspend request */
  tdsrrc_bplmn_suspend_cause_e_type bplmn_suspend_cause;

  /* This variable is used by CSP to determine the exact BPLMN state - active,
   * passive, inactive, even when L1 is a higher priority task and can 
   * pre-empt RRC at any time. This variable allows CSP to determine if TDSL1_CPHY_GO_TO_SLEEP_IND 
   * should be sent to L1 or not; and also allows CSP to inform L1 if it should go to
   * sleep or not when WTOG BPLMN search becomes active. 
   * To achieve the above, this variable is set to TRUE before accessing the critical
   * section; and the value of this variable is checked by L1 before accessing the 
   * critical section. This variable is set to FALSE on exiting the critical section.
   * (It is important to reset the value of the variable on exiting the critical section
   *  as otherwise no BPLMN search will ever be performed).
   */
  boolean bplmn_crit_sec_flag;

#ifdef FEATURE_BPLMN_SEARCH_320MS
  uint16  bplmn_start_sfn;
#endif

  /* This variable stores the # of W PLMNs present in the HPLMN list from NAS
   */
  uint32    wtow_bplmn_num_req;

  /* This variable stores the PLMN ids of the W PLMNs present in the HPLMN list from NAS
   */
  rrc_requested_plmn_type   wtow_bplmn_req_list[SYS_PRIORITY_PLMN_LIST_MAX_LENGTH];

  uint8      bplmn_trans_id;

  /* Stores the UMTS channel numbers which are mapped from GSM ARFCNs */

#ifdef FEATURE_TDSCDMA_TO_LTE
  boolean bplmn_lte_mode_enabled;
#ifdef LTE_BAND_NUM
  sys_lte_band_mask_e_type    bplmn_lte_band_pref;
#else
  sys_band_mask_e_type    bplmn_lte_band_pref;
#endif
#endif

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  /* Search timers for background MPLMN Search indicated by NAS*/
  tds_manual_search_timer_value_type    bplmn_search_timer_value;  
  boolean  send_partial_results;
#endif

  /* For UOOS, this var has been added to inform NAS 
   * (using var ue_oos in RRC_SERVICE_CNF) that service is available. 
   * NAS looks at this var, ue_oos, to determine if limited service request
   * should be sent to RRC or not.
   * For non-UOOS, this var is used to NOT start the 10s No Service
   * timer which (without this fix) gets started when UE finds a PLMN
   * that belongs to forbidden LAI which is NOT correct because UE can
   * get Limited Service on this PLMN.
   */
  boolean    plmn_fnd_during_fgnd_svc_srch;

  
   /* A new variable is added that stores lai and PLMN information
     together */
   inter_task_lai_T curr_lai; 

  /* Used to determine if the initial T mins timer during which UE does 
   * not go to sleep has expired or not. Default value is TRUE.
   */
  boolean oos_initial_no_deep_sleep_tmr_expd;

  /* Used to determine if the 30s duration for which UE should remain in
   * deep sleep has expd or not. Default value is TRUE.
   */
  boolean oos_conn_mode_ds_tmr_expd;


  /* Used to determine if the DCH->FACH/PCH reconfiguration timer has expired
   * or not. Initial value is FALSE.
   */
  boolean            dch_to_fach_pch_tmr_expd;

  /* Stores the destination state as it will be used to configure appropriate
   * channels when UE camps on a suitable cell. The CSP scan type may no longer
   * suffice as it is changed to OOS_Scan from DCH_to_FACH scan for RL_failure
   */
  tdsrrc_state_e_type   dch_tx_dest_state;

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
  /* Used to determine if the FACH->FACH/PCH reconfiguration timer has expired
   * or not. Initial value is FALSE.
   */
  boolean            fach_to_fach_pch_tmr_expd;

  /* Stores the destination state as it will be used to configure appropriate
   * channels when UE camps on a suitable cell. The CSP scan type may no longer
   * suffice as it is changed to OOS_Scan from FACH_to_FACH/FACH_to_PCH scan
   * after the redirection timer expiry
   */
  tdsrrc_state_e_type   fach_tx_dest_state;
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */




#ifdef FEATURE_INTER_FREQ_EM_CALL_REDIRECTION
  /* TRUE implies that the "type-of-PLMN-selection" variables were updated
   * and need to be reverted back to their original values. The initial
   * value is FALSE
   */
  boolean em_call_acq_type_updated;

  /* Stores the original value of network_select_mode when the above variable
   * em_call_acq_type_updated is set to TRUE
   */
  sys_network_selection_mode_e_type ifreq_orig_nw_select_mode;

  /* Stores the id of the PLMN UE is camped on. This plmn id is used to
   * compare with the PLMN UE eventually camps on to determine if the
   * n/w sel mode needs to be set to Automatic or Limited Routine. If
   * UE camps on the same/equivalent plmn then the n/w sel mode is 
   * set to Automatic, otherwise it is set to Limited Routine.
   */
  rrc_plmn_identity_type ifreq_orig_plmn_id;
#endif

#ifdef FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
  /* This enum is used by RRC to determine if background search is in progress
   * for "full service search" or "plmn list search". This value is set based
   * on the value received from NAS in PLMN_LIST_REQ.
   * The initial value of this enum is SYS_SERVICE_SEARCH_NONE.
   */
  sys_service_search_e_type service_search_bgnd;
#endif

  /* Setting this variable to TRUE means that on a limited service request L1 has to lift freq
    * and CPID barring both which overrides t-bar feature characteristic that  says unbar
    * only the frequency in this case
   */
  boolean bar_for_resel_only;

  boolean non_eq_plmn_barred;


  /* Scratch pad for storing the current (may or may not be camped) cell network 
   * sharing details. Note that this will not give the network sharing details of the 
   * current camped cell. */
  tdsrrc_csp_nw_sharing_entry_type curr_nw_db_entry;

  /* scrambling code */
  uint8 prev_cpid;

#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
  boolean oosc_plmn_found;
  uint8   oosc_l1_rssi;

  /* This variable is used to identify the first cycle of every GSM search in
   * RRC connected mode OOS. UI is updated with service status after every 
   * first cycle of GSM search.
  */
  boolean is_first_gsm_search;
#endif

  tdsrrc_csp_target_cell_source_e_type  target_tdd_cell_source;
  /* Contains target TDD cell list given by network via GSM-RR */
  tdsrrc_target_info_type    target_tdd_cell_list;

  /* for rel-10 redirect target freqs */
  tds_rrc_lte_tds_neigh_list_type tdd_target_freq_list;

  /* for rel-9 redirection lte sib6 utran ngbr freqs */
  tds_rrc_lte_tds_neigh_list_type ngbr_tdd_freq_list_r9;

  /* for rel-10 redirection lte sib6 utran ngbr freqs */
  tds_rrc_lte_tds_neigh_list_type ngbr_tdd_freq_list;

  /* freqs from lte tds irat acq db entries above success cnt threshold set by NV */
  tds_rrc_lte_tds_neigh_list_type irat_acqdb_high_cnt_freq_list;

  /* freqs from lte tds irat acq db entries below success cnt threshold set by NV */
  tds_rrc_lte_tds_neigh_list_type irat_acqdb_low_cnt_freq_list;

  /* indicate irat acq db entries in use. So order of freqs shall be maintained by ACQ */
  boolean l2t_redirect_irat_acqdb_selected;

  /* Used to determine if the GtoW redirection timer has expired
   * or not. Initial value is TRUE.
   */
  boolean    gtow_redirection_timer_expired;
  boolean    gtow_redir_retry_lfs_scan;

  uint8 num_freq_in_list_db;
  uint8 curr_rd_idx;

  /* To maintain the flexible deep sleep behavior right from the power up and OOS situations  */
  boolean do_lfs_scan;

  /*Boolean to indicate that RRC has received CEL_RESEL_IND that is to be processed when waiting for TRANS CNF*/
  /*Default Value is FALSE*/
  boolean cel_resel_ind_rcvd_waiting_trans_cnf;

  /*Boolean to indicate that LLC received Cel Transition CNF from L1
     Default value is TRUE*/
  boolean cel_trans_cnf_recvd_at_llc;

  /*
    Skip GSM search sent in Service request
    when this is set, do not search G as part of 900/850/PCS Band optimizations
  */
  boolean skip_gsm_search;


  sys_stop_mode_reason_e_type mode_change_reason;

  /* This variable is set when stop TDS req at MCM is pending
   * to be handled after W2G procedure abort cnf
   */
  boolean stop_pending_for_wtog_abort;

  /*TDS is preempted, after IDLE CNF from L1 call the Re ACQ procedure*/
  boolean preemption_handling_in_progress;

  /*If this flag is set to TRUE, RRC is waiting for L1 to release lock.*/
  boolean csp_waiting_for_wl1_to_release_lock;
  boolean oos_due_to_no_resource;
  /*If this flag is set to TRUE, LLC will ask L1 to release lock when drop all. */
  boolean set_l1_to_release_lock_when_drop_all;

#ifdef FEATURE_RAT_PRIORITY_LIST
  sys_rat_pri_list_info_s_type rat_pri_list_info;
#endif

  tdsrrc_csp_interrat_rat_e_type interrat_type;

  tdsrrc_csp_interrat_rat_e_type bplmn_interrat_type;

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  detected_frequency_list_type detected_frequency_list;

  /* Flag which indicates pre-redirection info has to be included or not in connection request msg. */
  boolean exclude_preredir_info_in_conn_req_msg;
  boolean  cgi_info_included;
#endif

  tdsrrc_csp_plmn_srch_rat_order_type plmn_srch_order;

  rrc_mm_service_req_cause_e_type service_req_cause;

  /* For GTOW PLMN list search, this flag indicates if UE is camped suitably on G */
  boolean gsm_camped_suitably;

  /*Boolean to track if the cell is barred for operator use*/
  boolean cell_reserved_for_operator_use;

  /* Flag to mark RL failure was detected */
  boolean rlf_detected;

#ifdef FEATURE_WRLF_SYSTEM_SEL
  /* Flag used to determine if connected mode OOS search is in progress*/
  boolean ue_in_conn_oos;
  /* PLMN on which NAS went OOS in connected mode.*/
  rrc_plmn_identity_type  rlf_plmn_id;
  /*Flag used to determine if connecting state OOS search is in progress*/
  boolean ue_in_connecting_oos;
#endif
#ifdef FEATURE_3GPP_CSFB_TDS
  /*Indicated that SIB cmd is posted to SIB when CSFB is active and SIB has skipped read SIB11/12/11bis/19*/
  boolean sib_read_csfb_active; 
  /* indicate to include csfb_indicator in rrc connection request OTA msg */
  boolean include_csfb_ind;
#endif

  /* Added for TDD */
  /* SIB mask used for setting up P-CCPCH on detected cells */
  tdsrrc_csp_sib_mask_type sib_mask;

  /* Acquisition frequency list */
  struct
  {
    uint16  num_freq;
    int16   curr_freq_index;
    uint16  freq_list[TDSL1_MAX_SCAN_FREQ];
    rrc_plmn_identity_type plmn_id[TDSL1_MAX_SCAN_FREQ];
	/* add source[] to record the where the frequency from */
    tdsrrc_acq_freq_source_type    source[TDSL1_MAX_SCAN_FREQ];
  }acq_freq_list;  

  /* flag to indicate continuation of existing LFS 
   * instead of starting a new LFS
   */
  boolean tds_continue_lfs;

  uint16  num_tds_prim_freq;
  tdsrrc_freq_type tds_prim_freq_list[TDS_MAX_PRIM_FREQ_NBR];
  int8   tds_prim_freq_list_rd_index;
  /* for Auto BPLMN using primary freq list after searching ACQ DB entries,
     we don't want it to affect the band search order of following full FS, 
     if needed, which should be based on ACQ DB entries info
  */
  boolean tds_prim_freq_list_in_use;

  /* ---------------------------------------------------------- */ 
  /* Variables declared for TD-SCDMA <-> GERAN Redirection Feature */
  /* ---------------------------------------------------------- */
  tdsrrc_csp_interrat_pseudo_fr_info_type pseudo_fr_info;


  boolean est_req_for_emerg_call;


  boolean allow_lte_resel;

  boolean l2t_redi_freq_compl;
  /* Indicates the presence of primary PLMN ID in RRC dedicated OTA */
  boolean primary_plmn_Id_present;

  boolean use_special_freq_for_acq;
  /*Haizhou 2012/10/22 indicate whether G2T redirection target freq has tried out*/
  boolean g2t_redi_freq_compl;
  
  /* For CR412213 */
  boolean rlf_skip_first_svc_req;
  /*indicate whether mset try complete for RLf/OOS cell selection procedure*/
  boolean is_mset_compl_for_rlf_oos;
  // CR 430034 RAU Reject workaround
  tdsrrc_cmd_type stored_svc_req_for_delay_rlf_rpt;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
  /* Indicates whether partial results have to be sent due to guard timer expiry or not*/
  boolean send_prtl_results_from_lte_due_to_guard_timer_expiry;
#endif

#ifdef FEATURE_SGLTE
  sys_ue_mode_e_type ue_mode;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_ue_mode_e_type ue_mode_internal[TDSCDMA_NUM_SUBS];
#endif
#endif
  sys_modem_dual_standby_pref_e_type  dual_standby_mode_info;
  sys_modem_device_mode_e_type  device_mode;
  /* used in tdsrrcwrm_block_wait_to_acquire_lock_for_time to handle WRM_LOCK_TIMER expiry */  
  boolean wrm_lock_tmr_started;
  /* time that service req should be aborted due to no TRM resource */
  uint32 trm_timeout_value;
  /*indicate whether shallow search has complete*/
  boolean       is_shallow_search_complete;
  /* indicate whether prioritized_band is used for frequency scan*/
  boolean       is_prioritized_band_used_for_freq_scan;

  /* FR20971 RELEASE 11 NAS change (REL11 CR 0225) */
  boolean can_ehplmn_be_camped;

  /* CR795437 update ue_oos in SERVICE_CFN per S criteria */
  boolean continue_cell_selection_per_s;
  boolean  rrc_bst_band_pref_incl;
  /*indicate whether L1 has no trm lock for raw scan*/
  boolean is_no_trm_lock_for_raw_scan;

  /*indicate L2T CSFB call is ongoing or not*/
  boolean csfb_extended_status;

  /*indicate the DRX count in current T2T/X BPLMN search*/
  uint32 bplmn_drx_cnt;

  /*indicate to use high or low priority for X2T BPLMN search*/
  boolean is_high_priority_for_irat_bplmn_srch;

  /*indicate X2T plmn search is manual BPLMN or FPLMN*/
  boolean is_irat_manual_foreground_plmn_srch;

  /*indicate T2X plmn search is excuted or not in current DRX cycle*/
  boolean is_t2x_bplmn_srched_in_curr_drx;
}tdsrrc_csp_int_data_type;


/*--------------------------------------*/
/* BPLMN variables for 3G->2G searches  */
/*--------------------------------------*/
/* This data structure stores all global data used internally by Cell
 * Selection procedure related to WTOG BPLMN search. 
 */
typedef struct
{

  /* This variable stores the value returned from GSM-RR. If the value
   * is set to TRUE then it implies that the GSM search is done.
   */
  boolean wtog_srch_done;

  /* This variable stores the value returned from GSM-RR. If the value
   * is set to TRUE then it implies that the HPLMN was found on GSM.
   */
  boolean wtog_hplmn_found;

  /* This variable is used by CSP to determine if TD2G search is currently
   * active or not. The initial value of this variable is set to FALSE
   */
  boolean wtog_bplmn_srch_active;

  /* This variable is used by CSP to determine if TD2G search has already
   * been aborted or not. The initial value of this variable is set to FALSE
   */
  boolean wtog_bplmn_srch_aborted;

  /* This variable is used by CSP to determine if TD2G search has already
   * been suspended or not. The initial value of this variable is set to FALSE
   */
  boolean wtog_bplmn_srch_suspended;

  /* This variable stores the timer value that is sent from RRC to RR so that
   * GSM can perform BPLMN search during this time and return control to RRC
   * after the timer expiry. Notice that for TD2G BPLMN search, the DRX timer
   * is started by GSM
   */
  uint32  wtog_rem_drx_tmr_value;

  /* This variable stores the value returned from GSM-RR related to the
   * # of PLMNs found on GSM
   */
  uint32  wtog_gsm_plmns_found;

  /* This variable stores the PLMNs found by GSM during the BPLMN search
   */
  rrc_avail_plmn_list_type  plmn_list;

#ifdef FEATURE_TDSCDMA_TO_LTE
  /* This variable stores the value returned from LTE-RRC related to the
   * # of PLMNs found on GSM
   */
  uint32  td2l_plmns_found;

  /* This variable stores the PLMNs found by LTE during the BPLMN search
   */
  rrc_avail_plmn_list_type  lte_plmn_list;
#endif
  /* This variable is used to determine if Manual BPLMN search as a whole should 
   * be aborted or restarted after aborting TD2G BPLMN search. Initial
   * value is set to FALSE which implies that Manual BPLMN search should be aborted
   */
  boolean restart_bplmn_search;  

}rrc_csp_td2g_bplmn_int_data_type;


/* This data structure stores the current data associated with
 * Initial and Stored Selection Controller.
 */
typedef struct
{
  /* This is the current PLMN being searched for */
  rrc_requested_plmn_type req_plmn;

  /* This is the current Acquisition type */
  tdsrrc_csp_acq_e_type      acq_type;

  /* This is the original Acquisition type. It is saved before
   * starting the BPLMN search in background
   */
  tdsrrc_csp_acq_e_type      prev_acq_type;

  /* This flag indicates that the Acq. DB is being searched at present */
  boolean                 acq_db_in_use;

  /* This flag indicates that the Destination Freq is being searched at present */
  boolean                 dest_freq_in_use;
  
  /* This count indicates how many times acquisition is to be tried
   * on the destination frequency.
   */
  uint16                  num_tries_on_dest_freq;

  /* Overall count of number of cycles done on destination frequency and 
   * Acquisition database. Note that:
   * 1 count of num_acq_db_and_dest_freq_cycles = 
   *        TDSRRC_CSP_MAX_ACQ_TRIES_ON_DIRECTED_FREQ + 1;
   */    
  uint16                  num_acq_db_and_dest_freq_cycles;
  /* The following data is to be used for acquisition database based scan.
   * This is valid only when acq_db_in_use is TRUE and acq_type is valid.
   */
  struct
  {
    /* This flag indicates that the next search is to begin with the first entry
     * in Acq. DB.
     */
    boolean                 start_with_first;
  }acq_db_scan_info;

  /* The following data is to be used for frequency scan only. This is valid
   * only when acq_db_in_use is FALSE and acq_type is valid.
   */
  struct
  {
    tdsrrc_csp_freq_scan_e_type  type_freq_scan;
    boolean   raw_scan_completed;
    boolean   fine_scan_completed;

    /* List frequency scan data */
    struct
    {
      uint16   num_freq;
      int16   curr_freq_index;
      tdsl1_freq_scan_result_struct  freq_result[TDSL1_MAX_SCAN_FREQ];
      rrc_plmn_identity_type plmn_id[TDSL1_MAX_SCAN_FREQ];
	  /* add source[] to record the where the frequency from */
      tdsrrc_acq_freq_source_type    source[TDSL1_MAX_SCAN_FREQ];
    }list_scan;
    /* Raw frequency scan data */
    struct
    {
      uint16  num_freq;
      int16   curr_freq_index;
      tdsl1_freq_scan_result_struct  freq_result[TDSL1_MAX_SCAN_FREQ];
    }raw_scan;

    /* Fine frequency scan data */
    struct
    {
      uint16  num_freq;
      int16   curr_freq_index;
      tdsl1_freq_scan_result_struct  freq_result[TDSL1_MAX_SCAN_FREQ];
    }fine_scan;

  }scan_info;

  /* List of frequencies where service was available */
  tdsrrc_freq_type                   available_freq_list[TDSL1_MAX_SCAN_FREQ];

  
  uint32                          num_available_freq;

  /* Band that was most recently scanned */
  tdsrrc_csp_band_class_type         current_band;

  /* Band that was scanned first as part of the full frequency scan */
  tdsrrc_csp_band_class_type         first_band_to_be_scanned;

  /* Mask to help in determining the next band to be scanned */
  tdsrrc_csp_band_class_type         next_band_mask;
  /* Mask to record next_band_mask when do deep search*/
  tdsrrc_csp_band_class_type         next_band_mask_for_deep;
  
  /* Record current sub band index (start from 0) within the whole band A/F/E, 
   * the sub band size is specified by TDSRRC_CSP_SUB_BAND_GAP;
   * This is used to divide raw scan of the whole band into smaller pieces 
   * to save time, specifically in BPLMN scenario.
   */
  uint8                              current_sub_band;


  tdsrrc_csp_band_class_type         prioritized_bands[MAX_NUM_OF_TDS_BANDS];
  uint8         prioritized_bands_read_idx;
  uint8         num_of_prioritized_bands;


}tdsrrc_csp_select_ctrl_data_type;





typedef struct 
{
  /* is dsac valid (or) not */
  boolean      valid;
  /* stores cs access control */ 
  tdsrrc_csp_access_control_e_type cs_access_control;
  /* stores ps access control */ 
  tdsrrc_csp_access_control_e_type ps_access_control;
} tdsrrc_dsac_info_type;

/* Stores the DSAC restrictions of each of the shared PLMNs of the current cell */
typedef struct
{
  rrc_plmn_identity_type plmn_id;
  /* is dsac valid (or) not */
  boolean      valid;
  tdsrrc_DomainSpecificAccessRestrictionParam_v670ext dsac_info;
} tdsrrc_dsac_info_for_shared_plmns_type;

#ifdef FEATURE_3GPP_CSFB_TDS
typedef struct
{
  uint8 num_cont;
  uint8 cpid_list[TDSCDMA_RRC_MAX_UTRA_CONT];
  tdsrrc_System_Information_Container_type cont[TDSCDMA_RRC_MAX_UTRA_CONT];
} tdsrrc_System_Information_Container_list_type;
typedef struct
{
  uint8 mo_error_count;
  uint8 mt_error_count;
}tdsrrc_cs_domain_error_count_type;
#endif
/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* RRC CSP Internal data, visible within Cell Selection Procedure */
extern  tdsrrc_csp_int_data_type  tdsrrc_csp_int_data;

extern boolean tdsrrc_csp_bplmn_data_saved;

/* Stores the value of tdsrrc_csp_int_data.bplmn_new_srch while
 * moving to connected mode with BPLMN in progress
 */
extern boolean tdsrrc_csp_bplmn_new_srch;

extern int16 acq_freq_list_curr_freq_index;
extern int8 tdsrrccsp_stored_acq_db_curr_rd_index;
extern boolean tds_prim_freq_list_in_use;
extern int8 tds_prim_freq_list_rd_index;
/* This structure is used by CSP only for maintaining state related to 
 * WTOG BPLMN search
 */ 
extern  rrc_csp_td2g_bplmn_int_data_type  rrc_csp_td2g_bplmn_int_data;

/* This data structure stores the Selection data for Initial and Stored
 * Selection Controller.
 */
extern  tdsrrc_csp_select_ctrl_data_type  tdsrrc_csp_curr_select_data;

extern  tdsrrc_csp_select_ctrl_data_type  tdsrrc_csp_prev_select_data;

/* Used for initializing the variables associated with 3G->3G background
 * PLMN Search during the search.
 */
extern  td2td_bplmn_srch_status_type wtow_bplmn_srch_status;

#ifdef FEATURE_3GPP_CSFB_TDS
extern tdsrrc_System_Information_Container_list_type tdsrrc_SIB_container;
#endif

extern tdsrrc_csp_acq_db_type     tdsrrccsp_stored_acq_db;

typedef enum
{
  TDSRRC_CAUSE_NONE,
  TDSRRC_CAUSE_QTA,
  TDSRRC_CAUSE_TA,
  TDSRRC_CAUSE_IMMEDIATE
}tdsrrccsp_preemption_cause_e_type;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */



/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION          RRC_CSP_FIND_CELL_BAR_STATUS

DESCRIPTION       This function is responsible for finding the status of
                  a cell in terms of Cell Barred Status. The
                  Cell Access Restrictions due to Access Control are ignored
                  by this function.

DEPENDENCIES
                  - SIB procedure should have SIB 3 available for this cell.
                  - The UE Access Class should be known.

RETURN VALUE
                  FAILURE: If Cell Barred status couldn't be evaluated.
                  SUCCESS: Cell Barred status could be successfully evaluated.

SIDE EFFECTS
                  None

===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_find_cell_bar_status (rrc_plmn_identity_type  plmn_id,
                              tdsrrc_int_cell_id_type  cell_id,
                              tdsrrc_csp_cell_bar_e_type  *cell_bar
                              ,rrc_plmn_identity_type sib_idx_plmn_id
                              );


/*===========================================================================

FUNCTION          RRC_CSP_FIND_ACCESS_CONTROL

DESCRIPTION       This function is responsible for finding the status of
                  Access Control due to Access Class Restrictions.

                  Note that this function doesn't care about Cell Barred status
                  OR Cell Reservations.

DEPENDENCIES
                  - SIB procedure should have SIB 3 available for this cell.
                  - The UE Access Class should be known.

RETURN VALUE
                  SUCCESS: Cell Access restrictions for this cell could be
                           calculated.
                  FAILURE: Failure to find out Cell Access restrictions due
                           to multiple reasons.
SIDE EFFECTS
                  None.
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_find_access_control(tdsrrc_csp_access_control_e_type  *access_control,
                            tdsrrc_dsac_info_type *dsac_info,
                            rrc_plmn_identity_type  plmn_id,
                            tdsrrc_int_cell_id_type    cell_id
                            ,rrc_plmn_identity_type sib_idx_plmn_id
                            );

/*===========================================================================

FUNCTION          RRC_FILL_ACCESS_CLASS_INFO

DESCRIPTION       This function will fill the access class info based on the 
                  access control of the cell
DEPENDENCIES
                  - SIB procedure should have SIB 3 available for this cell.
                  - The UE Access Class should be known.

RETURN VALUE
                  NONE
SIDE EFFECTS
                  None.
===========================================================================*/
void tdsrrccsp_fill_access_class_info(tdsrrc_int_access_class_barred_list_type rrc_int_access_class_barred_list, 
	                                                   tdsrrc_csp_access_control_e_type *access_control,
	                                                   rrc_plmn_identity_type  plmn_id,
	                                                   tdsrrc_int_cell_id_type    cell_id);

/*===========================================================================

FUNCTION          RRC_CSP_INIT_INT_DATA

DESCRIPTION       This function is responsible for initializing the internal
                  data structure to default values.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_int_data(void);

/*===========================================================================

FUNCTION          RRCCSP_GET_NEXT_BAND_TO_BE_SCANNED

DESCRIPTION       This function returns the next frequency band to be scanned
                  based on the last scanned band.

DEPENDENCIES
                  
RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
                  
===========================================================================*/
tdsrrc_csp_band_class_type
tdsrrccsp_get_next_band_to_be_scanned
( 
  void
);

/*===========================================================================

FUNCTION          RRC_CSP_CHECK_FOR_LAI_FORBIDDEN

DESCRIPTION       This function is responsible for initializing the internal
                  data structure to default values.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
boolean  tdsrrccsp_check_for_lai_forbidden
(
  tdsrrc_SysInfoType1 *sib1_ptr,
  tdsrrc_MasterInformationBlock *mib_ptr
);

/*===========================================================================

FUNCTION          RRC_CSP_CONTINUE_INITIAL_STORED_SEL_CNTRL

DESCRIPTION       This function continues with the already started Initial
                  and Stored Cell Selection
                  It looks for a frequency for a "specific" PLMN or
                  "Any" PLMN. It saves its current search state in Current
                  Selection Controller Data.

DEPENDENCIES
                  This function is closely knit with
                  tdsrrccsp_compare_expected_cell() and
                  tdsrrccsp_get_match_acq_entry().
                  A change in logic in either must be reflected in others.

RETURN VALUE
                  SUCCESS: A frequency has been found where Acquisition
                           can be tried.
                  FAILURE: No frequency found (all exhausted). Time for a
                           deep sleep ?

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_continue_stored_initial_select_ctrl
(
  tdsrrc_csp_acq_e_type  *ret_type_acq_ptr,
  tdsrrc_csp_acq_entry_type  *ret_acq_entry_ptr,
  tdsrrc_csp_freq_scan_e_type *ret_type_freq_scan
);

#ifdef FEATURE_WRLF_SYSTEM_SEL
/*===========================================================================

FUNCTION          RRC_CSP_COMPARE_EXPECTED_CELL_FOR_CONN_OOS

DESCRIPTION       This function compares the expected acq. type with the 
                  actual Acquisition status mask and PLMN status mask and
                  declares the actual cell as Better than/Same as/Below
                  the expected cell, when UE is in connected mode out of service.
           
DEPENDENCIES
                  This function is closely knit with 
                  tdsrrccsp_continue_stored_initial_select_ctrl() and
                  tdsrrccsp_get_match_acq_entry().
                  The logic in this function to compare actual and expected 
                  cells is based on the logic in rrc_csp_continue_stored_
                  initial_select_ctrl() to search cells. A change in logic
                  in either must be reflected in others.

RETURN VALUE
                  TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED: Cell exceeds expectations.
                  TDSRRC_CSP_CELL_AS_EXPECTED: Cell meets expectations.
                  TDSRRC_CSP_CELL_BELOW_EXPECTED: Cell is below expectations.

SIDE EFFECTS
                  None.
===========================================================================*/
tdsrrc_csp_cell_actual_vs_expected_e_type 
tdsrrccsp_compare_expected_cell_for_conn_oos
(
  tdsrrc_csp_plmn_status_e_type cell_plmn_status, 
  tdsrrc_csp_plmn_status_e_type cell_plmn_status_wrt_rlf_plmn, 
  tdsrrc_csp_acq_status_type  acq_status_mask,
  tdsrrc_csp_acq_e_type type_acq_expect
);
#endif

/*===========================================================================

FUNCTION          RRC_CSP_COMPARE_EXPECTED_CELL

DESCRIPTION       This function compares the expected acq. type with the
                  actual Acquisition status mask and PLMN status mask and
                  declares the actual cell as Better than/Same as/Below
                  the expected cell.

DEPENDENCIES
                  This function is closely knit with
                  tdsrrccsp_continue_stored_initial_select_ctrl() and
                  tdsrrccsp_get_match_acq_entry().
                  The logic in this function to compare actual and expected
                  cells is based on the logic in rrc_csp_continue_stored_
                  initial_select_ctrl() to search cells. A change in logic
                  in either must be reflected in others.

RETURN VALUE
                  TDSRRC_CSP_CELL_BETTER_THAN_EXPECTED: Cell exceeds expectations.
                  TDSRRC_CSP_CELL_AS_EXPECTED: Cell meets expectations.
                  TDSRRC_CSP_CELL_BELOW_EXPECTED: Cell is below expectations.

SIDE EFFECTS
                  None.
===========================================================================*/
tdsrrc_csp_cell_actual_vs_expected_e_type
tdsrrccsp_compare_expected_cell(tdsrrc_csp_plmn_status_e_type cell_plmn_status,
                              tdsrrc_csp_acq_status_type  acq_status_mask,
                              tdsrrc_csp_acq_e_type type_acq_expect);

/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all L1 commands sent by CSP. This also allocates cmd
                  buffer and sends the command.
                  Since all L1 cmds go via LLC, the LLC command TDSRRC_LLC_CMD_REQ
                  actaully encapsules the real L1 command. This is the LLC
                  command funneling mechanism.

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  SUCCESS: Command was successfully sent to L1.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_l1_cmd(tdsrrc_proc_e_type  proc_id,
                    tdsl1_cmd_enum_type l1_cmd_id,
                    tdsrrc_csp_acq_entry_type *acq_entry_ptr);

/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_TDS_ACQ_REQ

DESCRIPTION       This function is responsible for filling up data structures
                  for all new CPHY_ACQ_REQ command defined in TDD sent by CSP. 
                  This also allocates cmd buffer and sends the command.
                  Since all L1 cmds go via LLC, the LLC command TDSRRC_LLC_CMD_REQ
                  actaully encapsules the real L1 command. This is the LLC
                  command funneling mechanism.

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  SUCCESS: Command was successfully sent to L1.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_l1_tds_acq_req(tdsrrc_proc_e_type  proc_id,
                               uint16 * freq_list_ptr,
                               int16 freq_index,
                               uint8 num_freqs,
                               boolean unbar);

/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_DRX_REQ

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_DRX_REQ
                  to L1.

DEPENDENCIES


RETURN VALUE
                  SUCCESS: Able to send TDSL1_CPHY_DRX_REQ successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_send_l1_drx_req(void);
/*===========================================================================

FUNCTION          RRC_CSP_SEND_L1_DRX_REQ_FOR_MULTIPLE_DRX

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_DRX_REQ
                  to L1 for multiple DRX support feature.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send TDSL1_CPHY_DRX_REQ successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_send_l1_drx_req_for_multiple_drx(void);

#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
/*===========================================================================

FUNCTION          tdsrrccsp_send_l1_bar_cmd_with_lte_list

DESCRIPTION       This function is responsible for sending LTE bar list to L1 to avoid ping pong T2L redirection.

DEPENDENCIES
                  The passed pointer should be pointing to a valid data.

RETURN VALUE
                  SUCCESS: Command was successfully sent to L1.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type 
tdsrrccsp_send_l1_bar_cmd_with_lte_list(lte_rrc_irat_to_lte_redir_forbidden_earfcn_info_s * redir_forbidden_list_ptr, 
                                        uint8 t_barred);
#endif/*FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT*/

/*===========================================================================

FUNCTION          RRC_CSP_FILL_ACQ_ENTRY_FROM_SIB

DESCRIPTION       This function is responsible for filling up all fields of
                  Acq Entry from SIB3 data and SIB Confirmation command data.
                  It also fills the Cell Barred status into the passed pointer
                  for usage by the calling function.

DEPENDENCIES
                  SIB Confirmation pointer should point to valid data.
                  SIB3 should have been received for this cell (identified
                  by PLMN ID, Cell ID combination).

RETURN VALUE
                  SUCCESS: Acq entry successfully filled.
                  FAILURE: Acq. entry could not be filled (multiple reasons)

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_fill_acq_entry_from_sib( tdsrrc_sib_event_cnf_type *sib_cnf,
                                 tdsrrc_csp_acq_entry_type *acq_entry_ptr,
                                 tdsrrc_csp_cell_bar_e_type  *cell_bar_status);


/*===========================================================================

FUNCTION          RRC_CSP_SEND_SIB_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all RRC Internal commands to SIB procedure sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  TBD: If the data going into commands is conditional on
                  something, a mechanism will be needed to pass that condition.
                  It could be thru' devicing slightly different names for
                  the same commands with different conditional data.

DEPENDENCIES
                  The pointer to Acquisition Entry should be valid.
RETURN VALUE
                  SUCCESS: RRC Internal command was successfully sent to SIB Proc..
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_sib_cmd(tdsrrc_cmd_e_type sib_cmd, uint32 sib_mask,
                     tdsrrc_csp_acq_entry_type *acq_entry_ptr,
                     boolean                for_serving_cell);


/*===========================================================================

FUNCTION          RRC_CSP_GEN_STATUS_MASK

DESCRIPTION       This function generates an Acq. status mask from Cell
                  Barred status and LAI Status. The Acq. Status mask is
                  understood by Acq. database whereas Cell Barred status and
                  LAI Status are understood by Cell Selection utility
                  functions and Cell Selection state machine functions.

                  This utility acts as an interpretor so that the status
                  returned by Cell selection utilities can be compared with
                  the one returned by Acq. Database access functions.

DEPENDENCIES
                  The calling function MUST decide and screen non-relevant
                  enums in tdsrrc_csp_cell_bar_e_type and
                  rrc_csp_lai_rai_status_e_type.

RETURN VALUE
                  A bit mask that represents Acq. status.

SIDE EFFECTS
                  None
===========================================================================*/
tdsrrc_csp_acq_status_type
tdsrrccsp_gen_status_mask(tdsrrc_csp_cell_bar_e_type  cell_bar_status,
                        tdsrrc_csp_lai_status_e_type  lai_status);


/*===========================================================================

FUNCTION          RRC_CSP_SEND_MM_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all commands to MM sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.
                  It updates the last primtive sent to MM in internal data.

DEPENDENCIES

RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_mm_cmd(rrc_to_mm_sink_e_type  cmd_id,
                    mm_as_service_state_T  service_status,
                    rrc_selected_plmn_type *sel_plmn_ptr,
                    tdsrrc_csp_acq_entry_type *acq_entry_ptr);

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
/*===========================================================================

FUNCTION          tdsrrccsp_send_mplmn_partial_report_to_mm

DESCRIPTION       This function is responsible for filling up data structures
                  for PLMN List Confirm command to MM sent by CSP.
                  This function is called when partial MPLMN results needs to be informed to NAS

DEPENDENCIES
                  None
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrccsp_send_mplmn_partial_report_to_mm(void);
#endif

/*===========================================================================

FUNCTION          RRC_CSP_BPLMN_SEND_MM_PLMN_LIST_CNF_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for PLMN List Confirm command to MM sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES

RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  For time being it always sends an PLMN_LIST_CNF with the
                  no PLMNs.
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_bplmn_send_mm_plmn_list_cnf_cmd(td2td_bplmn_srch_status_type status, uint8 trans_id);


/*===========================================================================

FUNCTION          RRC_CSP_SEND_MM_PLMN_LIST_CNF_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for PLMN List Confirm command to MM sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES

RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  For time being it always sends an PLMN_LIST_CNF with the
                  no PLMNs.
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_mm_plmn_list_cnf_cmd(boolean srch_done);


/*===========================================================================

FUNCTION          	TDSRRCCSP_HANDLE_BPLMN_SIB_FAIL_HIT

DESCRIPTION       This function is responsible for check and update BPLMN
                  result when the corresponding cell's SIB fail hit 
				  TDSRRC_CSP_MAX_BPLMN_SIB_FAIL_RETRY

DEPENDENCIES

RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  For time being it always sends an PLMN_LIST_CNF with the
                  no PLMNs.
===========================================================================*/
boolean
tdsrrccsp_handle_bplmn_sib_fail_hit(uint8 index);

/*===========================================================================

FUNCTION          tdsrrccsp_update_plmn_list

DESCRIPTION       This function reads the SIBs for a cell and checks if it is
                  barred or in the forbidden lai list. If not, it will try to
                  add it to the PLMN list to be returned to MM in the
                  SERVICE_CNF.

DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_plmn_list(rrc_plmn_identity_type plmn_id, tdsrrc_int_cell_id_type cell_id);


/*===========================================================================

FUNCTION          tdsrrccsp_update_background_plmn_list

DESCRIPTION       This function reads the SIBs for a cell and checks if it is
                  barred or in the forbidden lai list. If not, it will try to
                  add it to the background PLMN list to be returned to RR in the
                  TDSRRC_INTERRAT_PLMN_SRCH_CNF or TDSRRC_INTERRAT_PLMN_SRCH_ABORT_CNF.

DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_background_plmn_list(rrc_plmn_identity_type plmn_id, tdsrrc_int_cell_id_type cell_id);


/*===========================================================================

FUNCTION          RRC_CSP_SEND_CCM_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for Cell Change Request sent to CCM.
                  This also allocates cmd buffer and sends the command.

DEPENDENCIES

RETURN VALUE
                  SUCCESS: Command was successfully sent to CCM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_ccm_cmd(tdsrrc_cmd_e_type  ccm_cmd,
                     tdsrrc_new_cell_status_e_type  req_cell_status,
                     tdsrrc_csp_acq_entry_type  *acq_entry_ptr);

/*===========================================================================

FUNCTION          RRC_CSP_SEND_LLC_CMD

DESCRIPTION       This function is responsible for filling up data structures
                  for all LLC (non-funneling) commands sent by CSP.
                  This also allocates cmd buffer and sends the command.

DEPENDENCIES
                  None.

RETURN VALUE
                  SUCCESS: Command was successfully sent to LLC.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_llc_cmd(tdsrrc_cmd_e_type   llc_cmd_id);

/*===========================================================================

FUNCTION          RRC_CSP_INIT_FREQ_SCAN_DATA

DESCRIPTION       This function initializes the frequency scan data in
                  Current select data.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_freq_scan_data(void);

/*===========================================================================

FUNCTION          RRC_CSP_GET_NEXT_FREQ_SCAN_ENTRY

DESCRIPTION       This function is responsible for determining the next
                  frequency scan type and returning the appropriate
                  data.
DEPENDENCIES
                  None

RETURN VALUE
                  SUCCESS: Able to find a frequency where a Raw or Fine scan
                           or an Acquisition can be tried.
                  FAILURE: All frequencies exhausted.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_get_next_freq_scan_entry(tdsrrc_freq_type *ret_freq);


/*===========================================================================

FUNCTION          RRC_CSP_UPDATE_FREQ_SCAN_ENTRY

DESCRIPTION       This function is responsible for updating frequency scan
                  data in current selection data and list frequency scan
                  data. Note that only the raw frequency scan list, fine frequency 
                  scan list and list frequency scan list need to be updated.

DEPENDENCIES
                  The L1 command data has to be validated before calling
                  this function.

RETURN VALUE
                  SUCCESS: If received scan type matches with the expected
                           scan type.
                  FAILURE: Discrepency between received and expected scan 
                           types.
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_update_freq_scan_entry(tdsl1_freq_scan_cnf_type *freq_scan_cnf,tdsrrc_csp_freq_scan_e_type type_freq_scan);


/*===========================================================================

FUNCTION          RRC_CSP_HANDLE_NO_AVAILABLE_FREQ

DESCRIPTION       This function handles failure to find any frequency for
                  acquisition. Based on the current service status an dthe last
                  MM primitive exchanged, it determines if and which command
                  needs to be sent to MM.It then sends the command to MM.

DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_handle_no_available_freq(void);


/*===========================================================================

FUNCTION          RRC_CSP_SEND_NEXT_FREQ_TO_L1

DESCRIPTION       This function sends either an Acquisition Request or a
                  Frequency scan request to L1 (via LLC) based on the passed
                  type of frequency scan.
                  The parameters for TDSL1_CPHY_ACQ_REQ and TDSL1_CPHY_FREQ_SCAN_REQ are
                  passed in the acq_entry_ptr.

DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_send_next_freq_to_l1
(
 tdsrrc_csp_acq_entry_type       *acq_entry_ptr,
 tdsrrc_csp_freq_scan_e_type     type_freq_scan
);

/*===========================================================================

FUNCTION          RRC_CSP_SEND_CHAN_RELEASE

DESCRIPTION       This function sends a Channel Config Request to LLC to
                  release all channels.

DEPENDENCIES
                  None
RETURN VALUE
                  SUCCESS/FAILURE.

SIDE EFFECTS
                  None

===========================================================================*/
uecomdef_status_e_type  tdsrrccsp_send_chan_release(void);

/*===========================================================================

FUNCTION          tdsrrccsp_send_chan_reconfig

DESCRIPTION       This function sends a Channel Config Request to LLC to
                  release all channels.

DEPENDENCIES
                  None
RETURN VALUE
                  SUCCESS/FAILURE.

SIDE EFFECTS
                  None

===========================================================================*/
uecomdef_status_e_type  tdsrrccsp_send_chan_reconfig(void);

/*===========================================================================

FUNCTION          tdsrrccsp_send_tr_phy_chl_release

DESCRIPTION       This function sends a Channel Config Request to LLC to
                  release Transport and Physical channels.

DEPENDENCIES
                  None.
RETURN VALUE
                  SUCCESS/FAILURE.
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_send_tr_phy_chl_release(void);

/*===========================================================================

FUNCTION          RRC_CSP_CMD_TO_MM_NEEDED

DESCRIPTION       This function determines if there is a need to send a
                  command to MM. If a command is needed to be sent, then
                  which command is needed is also determined.
                  The determination is made based on the last primitive
                  exchanged between MM and CSP and the new service status.

DEPENDENCIES
                  None
RETURN VALUE
                  TRUE: Command needs to be sent.
                  FALSE: No command needed.

SIDE EFFECTS
                  None

===========================================================================*/
boolean  tdsrrccsp_cmd_to_mm_needed
(
  rrc_to_mm_sink_e_type  *mm_cmd_id,
  tdsrrc_csp_mm_primitive_e_type  last_mm_primitive,
  mm_as_service_state_T      new_service_status
);

/*===========================================================================

FUNCTION          tdsrrccsp_mm_primitive_sent

DESCRIPTION       This function determines the new MM primitive to be sent
                  based on the command for MM and service status.

DEPENDENCIES
                  None
RETURN VALUE
                  Last CSP-MM primitive.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_mm_primitive_e_type  tdsrrccsp_mm_primitive_sent
(
  rrc_to_mm_sink_e_type  last_mm_cmd_id,
  mm_as_service_state_T      last_service_status
);


/*===========================================================================

FUNCTION          tdsrrccsp_init_data_from_service_req

DESCRIPTION       This function stores the data passed in TDSRRC_SERVICE_REQ
                  and initializes RRC data and CSP local data with the passed
                  data.

DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_init_data_from_service_req(rrc_service_req_type  *rrc_service_req);

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_on_mm_req

DESCRIPTION       This function starts a cell selection in response to
                  a newly received service request from MM. The acquisition
                  type is initialized based on the requested PLMN type. The
                  current scan is set to Cell selection scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_on_mm_req(void);

/*===========================================================================

FUNCTION          tdsrrccsp_release_chls_and_start_sel_on_mm_req

DESCRIPTION       This function releases all physical, transport and logical
                  channels before starting a cell selection in response to
                  a newly received service request from MM.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_release_chls_and_start_sel_on_mm_req(void);

/*===========================================================================

FUNCTION          tdsrrccsp_continue_cell_selection

DESCRIPTION       This function continues with the existing cell selection
                  with the existing acquisition type. It continues from the
                  point in the frequency scan or in acquisition database where
                  we were last.
                  If all frequencies are exhausted, a cell selection is
                  re-started in an acquisition type based on the requested
                  PLMN type.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_continue_cell_selection(void);

/*===========================================================================

FUNCTION          tdsrrccsp_release_chls_and_continue_cell_selection

DESCRIPTION       This function releases all logical, transport and physical
                  channels before continuing with cell selection.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_release_chls_and_continue_cell_selection(void);

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_out_of_service_area

DESCRIPTION       This function starts a cell selection on an out of service
                  area condition.
                  The cell selection is started in the current acquisition
                  type. The current scan is set to Out of Service area scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_out_of_service_area(void);

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_leaving_conn_mode

DESCRIPTION       This function starts a cell selection on leaving connected
                  mode.
                  The cell selection is started in the current acquisition
                  type. The current scan is set to leaving Connected mode scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_leaving_conn_mode(void);

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_out_of_service_area_in_idle_state

DESCRIPTION       This function starts a cell selection on an out of service
                  area condition in Idle state.
                  All channels are released.
                  The cell selection is started in the current acquisition
                  type. The current scan is set to Out of Service area in
                  Idle state scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_out_of_service_area_in_idle_state(void);

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_out_of_service_area_in_connecting_state

DESCRIPTION       This function starts a cell selection on an out of service
                  area condition in Connecting state.
                  All channels are released. 
                  The cell selection is started in the current acquisition 
                  type. The current scan is set to Out of Service area in
                  Connecting state scan.
                  Based on the returned frequency scan type, a CSP substate
                  is returned.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_out_of_service_area_in_connecting_state(void);


/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_from_dch_to_fach

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_DCH to CELL_FACH state.
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_dch_to_fach(tdsrrc_cmd_type  *rrc_cmd_ptr);

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION          RRC_CSP_START_SEL_FROM_FACH_TO_FACH

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_FACH to CELL_FACH state with frequency redirection info.
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_fach_to_fach(tdsrrc_cmd_type  *rrc_cmd_ptr);

/*===========================================================================

FUNCTION          RRC_CSP_START_SEL_FROM_FACH_TO_PCH

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_FACH to CELL_PCH or URA_PCH state with frequency redirection info.
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_fach_to_pch(tdsrrc_cmd_type  *rrc_cmd_ptr);

#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

/*===========================================================================

FUNCTION   tdsrrccsp_restore_saved_bplmn_data

DESCRIPTION
  This function is used to restore the saved 
  data for BPLMN operation

DEPENDENCIES

  None.
 
RETURN VALUE

  None.
 
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_restore_saved_bplmn_data(void);

/*===========================================================================

FUNCTION          tdsrrccsp_continue_bplmn_in_frgrnd

DESCRIPTION       This function is used to continue ongoing MPLMN Search in background 
                  to foreground.
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP Substate
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_continue_bplmn_in_frgrnd(void);

/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_for_plmn_list

DESCRIPTION       This function starts scanning for all the available PLMNs
                  Based on the returned frequency scan type, a CSP substate
                  is returned.

DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None
===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_for_plmn_list(void);



/*===========================================================================

FUNCTION          RRC_CSP_SEND_MM_ACT_IND

DESCRIPTION       This function is responsible for filling up data structures
                  for Activation Indication to MM sent by CSP.
                  This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES

RETURN VALUE
                  SUCCESS: Command was successfully sent to MM.
                  FAILURE: Command could not be sent due to any one of multiple
                           reasons.

SIDE EFFECTS

===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_send_mm_act_ind( void );



/*===========================================================================

FUNCTION          tdsrrccsp_start_sel_from_dch_to_pch

DESCRIPTION       This function starts a cell selection on a transition from
                  CELL_DCH to CELL_PCH or URA_PCH state.                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_from_dch_to_pch(tdsrrc_cmd_type  *rrc_cmd_ptr);

/*===========================================================================

FUNCTION          tdsrrccsp_release_bch_and_continue_cell_selection

DESCRIPTION       This function releases BCCH/BCH/S_PCCPCH pipe before 
                  continuing with cell selection.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_release_bch_and_continue_cell_selection(void);

/*===========================================================================

FUNCTION          tdsrrccsp_init_bplmn_data

DESCRIPTION       This function initializes RRC data and CSP local data.

DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_init_bplmn_data(void);

/*===========================================================================

FUNCTION          tdsrrccsp_start_bplmn_srch

DESCRIPTION       This function starts a BPLMN search in response to
                  a BPLMN search request from RR (for GTOW searches) or 
                  NAS (for WTOW searches). 
                  
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_bplmn_srch(void);


/*===========================================================================

FUNCTION          RRCCSP_BPLMN_UPDATE_STORED_ACQ_DB_RD_INDEX

DESCRIPTION       This function is used by CSP to updated 
                  stored_acq_db.curr_rd_index if BPLMN srch timer expires
                  before SIBs could be read.
                  
DEPENDENCIES      NONE

RETURN VALUE
                  
SIDE EFFECTS

===========================================================================*/
void
tdsrrccsp_bplmn_update_stored_acq_db_rd_index ( void );


  
/*===========================================================================
FUNCTION        RRCCSP_SEND_MODE_CHANGE_REQ_TO_MCM

DESCRIPTION
                This function sends a TDSRRC_MODE_CHANGE_REQ (with the desired
                mode) to MCM.

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void 
tdsrrccsp_send_mode_change_req_to_mcm( tdsrrc_mode_e_type tdsrrc_mode);


/*===========================================================================

FUNCTION          RRC_CSP_INIT_SELECT_DATA

DESCRIPTION       This function is responsible for initializing the internal
                  Search data structure to default values based on the passed
                  PLMN Choice type.

DEPENDENCIES
                  None

RETURN VALUE
                  SUCCESS: Success initializing Select data based on passed
                           PLMN type.
                  FAILURE: Could not initialize Select data for the passed
                           PLMN type.

SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_init_select_data(rrc_requested_plmn_type  *req_plmn_ptr);


/*===========================================================================

FUNCTION          RRC_CSP_START_SEL_REDIRECT_FREQ_IN_CONNECTING_STATE

DESCRIPTION       This function starts cell selection on the designated
                  frequency received by RRC in inter-frequency redirection
                  request in connecting state.
                  Only transport and physical channels are released before
                  attempting acquisition on the designated frequency.
                  The cell selection is started in the current acquisition 
                  type. The current scan is set to inter frequency 
                  redirection scan.
                  CSP substate is returned as appropriate.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_start_sel_redirect_freq_in_connecting_state(void);


/*===========================================================================

FUNCTION          tdsrrccsp_send_bch_release

DESCRIPTION       This function sends a Channel Config Request to LLC to 
                  release BCCH/BCH/S_PCCPCH pipe.
           
DEPENDENCIES
                  None.
RETURN VALUE
                  SUCCESS/FAILURE.
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type 
tdsrrccsp_send_bch_release(boolean); 

/*===========================================================================

FUNCTION          RRCCSP_HANDLE_FREQ_REDIRECT_WAIT_TIMER_EXPIRY

DESCRIPTION       This function handles expiry of wait timer started due to 
                  inter frequency redirection request received from RCE. 
                  Till the expiry of the wait timer, RRC does not allow
                  cell selections/reselections to the last camped frequency.
                  
                  If timer expires when redirection is in progress, this
                  function determines if UE should continue camping on the 
                  current frequency which would be the case if cell selection
                  is being attempted on the redirected frequency for the first time;
                  or UE should go to pending substate and wait for the appropriate
                  confirmation and on receiving the confirmation start acquisition
                  on the last camped frequency. 
                  
DEPENDENCIES      NONE

RETURN VALUE
                  CSP substate
                  
SIDE EFFECTS

===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_handle_freq_redirect_wait_timer_expiry(tdsrrc_csp_substate_e_type curr_substate);

/*===========================================================================

FUNCTION          tdsrrccsp_init_inter_freq_redirection_data

DESCRIPTION       This function initializes appropriate data before attempting
                  acquisition on redirected frequency.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
    
SIDE EFFECTS
                  None

===========================================================================*/
void  tdsrrccsp_init_inter_freq_redirection_data(void);


/*===========================================================================

FUNCTION          tdsrrccsp_cleanup_inter_freq_redirection_state

DESCRIPTION       This function cleans up appropriate data on completing or
                  aborting inter frequency redirection scan.
                                               
DEPENDENCIES
                  None
RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_cleanup_inter_freq_redirection_state(void);


/*===========================================================================

FUNCTION          RRCCSP_SEND_RCE_REDIRECT_CNF

DESCRIPTION       This function sends REDIRECT_CNF to RCE with appropriate
                  cause for successful or unsuccessful redirection. 
                  The wait timer is stopped only in this function (if it
                  was active).
                                    
DEPENDENCIES      NONE

RETURN VALUE
                  NONE
SIDE EFFECTS

===========================================================================*/
void
tdsrrccsp_send_rce_redirect_cnf(tdsrrc_redirect_status_e_type redirect_status, 
                             tdsrrc_redirect_cause_e_type  redirect_cause);


/*===========================================================================

FUNCTION          RRCCSP_VALIDATE_REDIRECTION_REQ

DESCRIPTION       This function checks whether inter frequency redirection 
                  or inter-RAT redirection request received by CSP has correct 
                  arguments and that the request has been received in correct 
                  RRC state (which is connecting).
                  
                  Additionally, this function sends REDIRECT_CNF to RCE with
                  failure if redirection is not supported or if any of the above
                  checks fail.
                  
DEPENDENCIES      NONE

RETURN VALUE
                  TRUE:  Implies start acq on redirected freq.
                  FALSE: Implies ignore redirection request (because in this 
                         case redirect confirm has already been sent inside this 
                         function).
                  
SIDE EFFECTS

===========================================================================*/
boolean
tdsrrccsp_validate_redirection_req( tdsrrc_cmd_type *cmd_ptr );


/*===========================================================================

FUNCTION          RRCCSP_HANDLE_FAILURE_DURING_FREQ_REDIRECTION

DESCRIPTION       This function handles failure when inter frequency redirection
                  scan is in progress. The failure could be due to acquisition
                  failure, or cell may not be suitable/acceptable, or S-criteria
                  was not met, etc. 
                  
                  Either acquisition is attempted on next frequency or
                  REDIRECT_CNF is returned to RCE with appropriate
                  cause.
                  
DEPENDENCIES      NONE

RETURN VALUE
                  CSP substate which can only be one of the following:
                  TDSRRCCSP_CAMPED_SUIT_CELL, or TDSRRCCSP_CAMPED_ACCEPT_CELL, or
                  TDSRRCCSP_CELL_SEL_WT_ACQ.
                  
SIDE EFFECTS

===========================================================================*/
tdsrrc_csp_substate_e_type 
tdsrrccsp_handle_failure_during_freq_redirection(void);


/*===========================================================================

FUNCTION          RRCCSP_SEND_CELL_UPDATE_INITIATE_REQ

DESCRIPTION       This function sends cell update initiate request to CU procedure
                  when UE goes Out of Service Area in Connected State
                  
                  
DEPENDENCIES      NONE

RETURN VALUE
                  CSP substate
                  
SIDE EFFECTS          
                  NONE
===========================================================================*/
tdsrrc_csp_substate_e_type
tdsrrccsp_send_cell_update_initiate_req(tdsrrc_cell_update_cause_e_type cause);
  

/*===========================================================================

FUNCTION RRC_GET_CSP_SUBSTATE

DESCRIPTION
  This function returns the current substate of CSP.
  
DEPENDENCIES
  None

RETURN VALUE
  Current sub-state of CSP.

SIDE EFFECTS
  None

===========================================================================*/
tdsrrc_csp_substate_e_type  tdsrrccsp_get_csp_substate( void );


/*===========================================================================

FUNCTION          RRCCSP_UPDATE_BPLMN_SRCH_VARS

DESCRIPTION       This function is used by CSP to update/re-store appropriate 
                  variables during different stages of 3G->3G Background PLMN search.
                  
DEPENDENCIES      NONE

RETURN VALUE
                  NONE
                  
SIDE EFFECTS
                  NONE

===========================================================================*/
void tdsrrccsp_update_bplmn_srch_vars( 
  td2td_bplmn_srch_status_type       status, 
  sys_network_selection_mode_e_type nw_sel_mode);


/*===========================================================================

FUNCTION          RRCCSP_RESET_BPLMN_LIST_SEARCH_STATE

DESCRIPTION       This function sends PLMN_LIST_CNF to NAS (if need be),
                  updates the BPLMN database, and also sends sleep indication
                  to L1 (if need be).
                  
DEPENDENCIES      NONE

RETURN VALUE      NONE
                      
SIDE EFFECTS      NONE

===========================================================================*/
void tdsrrccsp_reset_bplmn_list_search_state(
    td2td_bplmn_srch_status_type status,    
    boolean                     rce_waiting, 
    boolean                     send_sleep_ind);


/*===========================================================================

FUNCTION          RRC_CSP_BPLMN_SRCH_IN_PROGRESS

DESCRIPTION       This function is called by procedures CSP, RCE, SIB, and CU
                  to determine if BPLMN search is in progress.
                  
DEPENDENCIES      NONE

RETURN VALUE      TRUE:  BPLMN search is in progress
                  FALSE: Otherwise
                  
SIDE EFFECTS      NONE
===========================================================================*/
td2td_bplmn_srch_status_type tdsrrccsp_bplmn_srch_in_progress(void);


/*===========================================================================

FUNCTION          RRCCSP_CLEANUP_BPLMN_STATE

DESCRIPTION       This function determines if BPLMN search is active or not.
                  If BPLMN search is active then PLMN_LIST_CNF is sent to NAS
                  with status ABORTED and # of PLMNs as 0 and it is also determined 
                  if TDSL1_CPHY_GO_TO_SLEEP_IND should be sent to L1 or not.
                  If WTOG search is in progress (actively or passively) then this
                  function sends INTERRAT_PLMN_SRCH_ABORT_REQ to GSM
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_cleanup_bplmn_state( void );


/*===========================================================================

FUNCTION          RRCCSP_CLEANUP_AND_RESTART_BPLMN

DESCRIPTION       This function is called when UE transitions from Idle ->
                  non-idle state and BPLMN n/w sel mode is MANUAL. If BPLMN search 
                  was in progress when this function is called then the BPLMN 
                  search is re-started by resetting appropriate variables. Notice
                  that the guard timer is also reset. This function was added to
                  allow UE to continue Manual search across LAU/RAU or MT calls when UE
                  returns to Idle/PCH state.                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_cleanup_and_restart_bplmn( void );


/*===========================================================================

FUNCTION   RRCCSP_SEND_BPLMN_SRCH_RESUME_IND

DESCRIPTION

  This function is used by CSP to send itself an internal command
  RRC_BPLMN_SEARCH_IND after L1 calls the function can_l1_go_to_sleep()
  when it determines that BPLMN search can be performed
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_send_bplmn_srch_resume_ind(void);


/*===========================================================================

FUNCTION          RRC_CSP_RESUME_WTOG_BPLMN_SRCH

DESCRIPTION       This function starts the WTOG BPLMN search by suspending W
                  and sending INTERRAT_PLMN_SRCH_REQ to GSM
                                               
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_resume_td2g_bplmn_srch(void) ;


/*===========================================================================

FUNCTION        RRCCSP_PLMN_LIST_REQ_IS_VALID

DESCRIPTION
                This function determines if PLMN_LIST_REQ (Automatic and Manual)
                from MM has been received in correct RRC state or not. If  
                PLMN_LIST_REQ is NOT valid then PLMN_LIST_CNF is sent to NAS 
                within this function itself.
                
DEPENDENCIES
                None

RETURN VALUE
                TRUE:  PLMN_LIST_REQ is valid
                FALSE: Otherwise

SIDE EFFECTS
                None

===========================================================================*/
boolean tdsrrccsp_plmn_list_req_is_valid(rrc_plmn_list_req_type  *plmn_list_req_ptr);


/*===========================================================================

FUNCTION        RRCCSP_PLMN_LIST_REQ_WHEN_RESUME_IN_PROG_IS_VALID

DESCRIPTION
                This function determines if PLMN_LIST_REQ (Automatic and Manual)
                from MM has been received in correct RRC state or not. If  
                PLMN_LIST_REQ is NOT valid then PLMN_LIST_CNF is sent to NAS 
                within this function itself.

                This function is different from tdsrrccsp_plmn_list_req_is_valid()
                in the sense that in this function it is not checked if UE is 
                camped or not. The reason being that CCM is always updated with
                cell loss when W is suspended and hence if PLMN List Request 
                is received when W is suspended then Plmn List Request may be
                incorrectly handled if function tdsrrccsp_plmn_list_req_is_valid()
                is called. 

                This function is called only when W is in the process of resuming.
                
DEPENDENCIES
                None

RETURN VALUE
                TRUE:  PLMN_LIST_REQ is valid
                FALSE: Otherwise

SIDE EFFECTS
                None

===========================================================================*/
boolean tdsrrccsp_plmn_list_req_when_resume_in_prog_is_valid(rrc_plmn_list_req_type  *plmn_list_req_ptr);


/*===========================================================================

FUNCTION   RRCCSP_GET_CURR_SCAN

DESCRIPTION

  This function returns the current scan type in CSP
  
  
DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_csp_scan_e_type
 
SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_scan_e_type tdsrrccsp_get_curr_scan(void);

/*===========================================================================

FUNCTION   tdsrrccsp_get_curr_interrat_type

DESCRIPTION

  This function returns the interrat type 
  
DEPENDENCIES 
  None.
  
RETURN VALUE 

  tdsrrc_csp_acq_e_type: TDSRRC_CSP_INTERRAT_NONE,TDSRRC_CSP_INTERRAT_LTE,TDSRRC_CSP_INTERRAT_GSM,
  TDSRRC_CSP_INTERRAT_LTE_CGI,TDSRRC_CSP_INTERRAT_MAX
 
SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_interrat_rat_e_type tdsrrccsp_get_curr_interrat_type(void);

/*===========================================================================

FUNCTION   RRCCSP_CHECK_IF_3G_BAND_PREF_CHANGED

DESCRIPTION

  This function returns TRUE if both CSP and NAS passed 3G preferences are same
  otherwise returns FALSE  
  
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE/FALSE
 
SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_check_if_3g_band_pref_changed(sys_band_mask_type nas_band_mask);


/*===========================================================================

FUNCTION        RRCCSP_HANDLE_PLMN_LIST_REQ_IN_BGND

DESCRIPTION
                This function processes PLMN_LIST_REQ from MM and determines
                if PLMN list search should be done in background or foreground.
                BPLMN state is updated and appropriate timers are started within
                this function to do PLMN search in background
                
DEPENDENCIES
                None

RETURN VALUE
                TRUE: PLMN_LIST_REQ should be handled in background
                FALSE: PLMN_LIST_REQ should be handled in foreground

SIDE EFFECTS
                None

===========================================================================*/
boolean tdsrrccsp_handle_plmn_list_req_in_bgnd(rrc_plmn_list_req_type  *plmn_list_req_ptr);


/*===========================================================================

FUNCTION        RRCCSP_HANDLE_PASSIVE_BPLMN_SUSPEND_REQ

DESCRIPTION
                This function sends TDSRRC_BPLMN_SUSPEND_CNF to the procedure
                that sent TDSRRC_BPLMN_SUSPEND_REQ and also determines if BPLMN
                search should be suspended or aborted based on the procedure
                that sent the suspend request.
                This function is only called when BPLMN search is passively 
                in progress

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrccsp_handle_passive_bplmn_suspend_req( tdsrrc_cmd_type *cmd_ptr );


/*===========================================================================

FUNCTION   RRC_CSP_SEND_BPLMN_SUSPEND_CNF

DESCRIPTION

  This function is called by CSP and it sends TDSRRC_BPLMN_SUSPEND_CNF to 
  the procedure that sent TDSRRC_BPLMN_SUSPEND_REQ to CSP
  
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.
 
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_send_bplmn_suspend_cnf(tdsrrc_proc_e_type proc, uecomdef_status_e_type status);


/*===========================================================================

FUNCTION          RRC_CSP_INIT_WTOG_PLMN_SRCH_INT_DATA

DESCRIPTION       This function is responsible for initializing the internal
                  data structure to default values for WTOG PLMN Search.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_td2g_plmn_srch_int_data( void );


/*===========================================================================

FUNCTION          RRCCSP_CHECK_AND_START_PLMN_LIST_FULL_FREQ_SCAN_TIMER

DESCRIPTION       CSP uses this function to start the PLMN full scan timer.
                  As long as the timer is active, no full scans are done during
                  2G->3G and 3G->3G PLMN searches. 
                  This timer is started when PLMN full (raw/fine) scan search 
                  completes (in background or in foreground). This timer is never stopped. 
                  Additional check has been added for the NV GCF flag. If this flag 
                  is set then the timer is not started. 

DEPENDENCIES      NONE

RETURN VALUE      NONE

SIDE EFFECTS      NONE

===========================================================================*/
void tdsrrccsp_check_and_start_plmn_list_full_freq_scan_timer( void );


/*===========================================================================

FUNCTION   RRCCSP_GET_CURR_TYPE_ACQ

DESCRIPTION

  This function returns the current acq type - TDSRRC_CSP_SPEC_PLMN or
  TDSRRC_CSP_ANY_PLMN      
  
DEPENDENCIES 
  None.
  
RETURN VALUE 

  tdsrrc_csp_acq_e_type: TDSRRC_CSP_SPEC_PLMN or TDSRRC_CSP_ANY_PLMN
 
SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_acq_e_type tdsrrccsp_get_curr_type_acq(void);

#ifdef FEATURE_DSAC
/*===========================================================================

FUNCTION          RRC_CSP_FIND_DSAC_RESTRICTIONS

DESCRIPTION       This function is responsible for finding the status of
                  Access Control due to Domain Specific Access Class Restrictions.

DEPENDENCIES
                  None.

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.
===========================================================================*/
void tdsrrccsp_find_dsac_restrictions
(
  tdsrrc_DomainSpecificAccessRestrictionParam_v670ext dsac_local,
  tdsrrc_dsac_info_type* rrc_dsac_info,
  tdsrrc_int_access_class_barred_list_type* rrc_int_access_class_barred_list,
  rrc_plmn_identity_type  plmn_id,
  tdsrrc_int_cell_id_type    cell_id
);

/*====================================================================================

FUNCTION          RRC_CSP_FILL_DSAC_INFO_FOR_SHARED_PLMNS

DESCRIPTION     This function fills the DSAC info for shared PLMNs from SIB3

DEPENDENCIES

RETURN VALUE
                  None

SIDE EFFECTS
                  None
======================================================================================*/
void tdsrrccsp_fill_dsac_info_for_shared_plmns
(
  uint8 number_of_plmns,
  tdsrrc_SysInfoType3   *sib3_ptr,
  tdsrrc_dsac_info_for_shared_plmns_type* dsac_info_for_shared_plmns
);

#endif /* FEATURE_DSAC */

/*===========================================================================

FUNCTION          RRCCSP_REL_CHLS_AND_GO_TO_IDLE

DESCRIPTION       This function sends a channel config request to release all
                  channels and take UE to idle. It also checks if the channel
                  release will result in a state change to DISCONNECTED from
                  a non-disconnected state so that CSP can wait for the state
                  change indication before sending NO_SERVICE to MM.
           
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate

SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type  tdsrrccsp_rel_chls_and_go_to_idle(void);

/*===========================================================================

FUNCTION   RRCCSP_IS_GTOW_BPLMN_SRCH_IN_PROGRESS

DESCRIPTION

  This function checks if there is GTOW BPLMN search is in progress (or) not
  
DEPENDENCIES

  None.
 
RETURN VALUE

  return TRUE if GTOW BPLMN search is in progress otherwise
         FALSE
 
SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_is_g2td_bplmn_srch_in_progress(void);


/*===========================================================================

FUNCTION        tdsrrccsp_suspend_td2td_bplmn_ahead_of_srch_timer

DESCRIPTION     Suspend TD2TD BPLMN search before timer expiry. It's invoked when
                the remaining timer is not sufficient to perform next step
                action.

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_suspend_td2td_bplmn_ahead_of_srch_timer(void);


/*===========================================================================

FUNCTION        tdsrrccsp_suspend_x2td_bplmn_ahead_of_srch_timer

DESCRIPTION     Suspend X2TD BPLMN search before timer expiry. It's invoked when
                the remaining timer is not sufficient to perform next step
                action.

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_suspend_x2td_bplmn_ahead_of_srch_timer(void);


/*===========================================================================

FUNCTION          RRC_CONVERT_BAND_TO_SYS_BAND

DESCRIPTION       This function takes the band used internally by RRC and 
                  converts it into sys band which is used by CM/SD.
                  
DEPENDENCIES      None

RETURN VALUE      sys band type used by CM/SD. 

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_e_type tdsrrc_convert_band_to_sys_band(tdsrrc_csp_band_class_type band);


#ifdef FEATURE_INTER_FREQ_EM_CALL_REDIRECTION
/*===========================================================================

FUNCTION          RRCCSP_UPDATE_ACQ_TYPE_FOR_EM_CALL_IFREQ_REDIRECTION

DESCRIPTION       Updates appropriate variables so that cell selection for
                  inter freq redirection is done for any plmn. Currently 
                  this function is called only when Emergency call is made 
                  and it is redirected to another frequency via RRC Connection 
                  Reject message.

DEPENDENCIES      NONE

RETURN VALUE      NONE

SIDE EFFECTS      NONE

===========================================================================*/
void tdsrrccsp_update_acq_type_for_em_call_ifreq_redirection(tdsrrc_csp_acq_e_type type_of_acq);

#endif



/*===========================================================================

FUNCTION          RRC_CSP_ADD_FREQ_TO_AVAILABLE_FREQ_LIST

DESCRIPTION       Adds a given frequency to avaialble frequency list. All the frequencies which are in the 
                  predefined range of these frequencies are skipped while scanning later.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_add_freq_to_available_freq_list(tdsrrc_freq_type freq);



#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
/*===========================================================================

FUNCTION: RRC_CSP_SEND_SVC_STATUS_TO_MM

DESCRIPTION:

DEPENDENCIES: NONE

RETURN VALUE: NONE

SIDE EFFECTS: NONE
=============================================================================*/
void tdsrrccsp_send_svc_status_to_mm(sys_srv_status_e_type svc_st,
                                   sys_radio_access_tech_e_type rat, 
                                   sys_signal_strength_type sig_st );
#endif


/*===========================================================================

FUNCTION           tdsrrccsp_prioritize_tds_bands_for_full_scan

DESCRIPTION       Constructs the order in which the TDS bands need to be searched while doing
                  full frequency scan.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void 
tdsrrccsp_prioritize_tds_bands_for_full_scan(void);

/*===========================================================================

FUNCTION          RRCCSP_IS_BAND_ALREADY_ADDED_TO_PRIORITIZED_LIST

DESCRIPTION       Determines whether a band given is already present in the prioritized band list.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
boolean 
tdsrrccsp_is_band_already_added_to_prioritized_list(
  tdsrrc_csp_band_class_type freq_band,
  uint8 num_of_bands
);



/*===========================================================================

FUNCTION          tdsrrccsp_start_cell_selection

DESCRIPTION       This function is responsible for starting a New Stored or
                  Initial Cell Selection. It initializes the Selection Data
                  based on the passed requested PLMN before calling 
                  tdsrrccsp_continue_stored_initial_select_ctrl which takes off
                  from there.

DEPENDENCIES
                  None

RETURN VALUE
                  SUCCESS: A frequency has been found where Acquisition 
                           can be tried.
                  FAILURE: No frequency found (all exhausted) or passed data not 
                           valid. Time for a deep sleep ?
SIDE EFFECTS
                  None
===========================================================================*/
uecomdef_status_e_type
tdsrrccsp_start_cell_selection
(
  rrc_requested_plmn_type  *req_plmn_ptr, 
  tdsrrc_csp_acq_e_type  *ret_type_acq_ptr,
  tdsrrc_csp_acq_entry_type  *ret_acq_entry_ptr,
  tdsrrc_csp_freq_scan_e_type *ret_type_freq_scan
);



/*===========================================================================

FUNCTION        RRCCSP_SEND_RR_RRC_STOP_GSM_REQ

DESCRIPTION     This function sends TDSRR_STOP_GSM_REQ to RR

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrccsp_send_rr_rrc_stop_gsm_req
(
  tdsrrc_rr_stop_tds_cause_e_type tdsstop_cause
);

/*===========================================================================

FUNCTION        RRCCSP_SEND_RRC_RR_STOP_TDS_CNF

DESCRIPTION     This function sends TDSRRC_STOP_TDS_CNF to RR

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrccsp_send_rrc_rr_stop_tds_cnf
(
  tdsrrc_rr_stop_tds_cause_e_type tdsstop_cause,
  sys_modem_as_id_e_type as_id
);

#ifdef FEATURE_TDSCDMA_TO_WCDMA
/*===========================================================================

FUNCTION        RRCCSP_SEND_WRRC_STOP_WCDMA_REQ

DESCRIPTION     This function sends RRC_TDSRRC_STOP_WCDMA_REQ to WRRC

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrccsp_send_wrrc_stop_wcdma_req
(
  rrc_rr_stop_wcdma_cause_e_type tdsstop_cause
);

/*===========================================================================

FUNCTION        RRCCSP_SEND_WRRC_STOP_TDS_CNF

DESCRIPTION     This function sends TDSRRC_RRC_STOP_TDS_CNF to WRRC

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrccsp_send_wrrc_stop_tds_cnf
(
  rrc_rr_stop_wcdma_cause_e_type tdsstop_cnf_cause
);
#endif /* #ifdef FEATURE_TDSCDMA_TO_WCDMA */


#ifdef FEATURE_OOSC_USER_ACTION
#ifdef FEATURE_TDSCDMA_OOSC_SERVICE_STATUS_UI_UPDATE
/*===========================================================================

FUNCTION        RRCCSP_CHECK_AND_SEND_UOOSC_END_IND

DESCRIPTION     This function is called when RRC receives STOP TDS request from NAS
                This function checks if UOOSC search was in progress and sends 
                RRC_CONNECTED_MODE_SERVICE_STATUS_IND with is_connected_mode_oos as FALSE

DEPENDENCIES
                None

RETURN VALUE
                None

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrccsp_check_and_send_uoosc_end_ind
(
  void
);
#endif
#endif


/*===========================================================================

FUNCTION          RRC_CSP_GET_LIST_DB_MATCH_ACQ_ENTRY

DESCRIPTION    This function will match the frequency and PLMN id of the entry received from L1 as part of the 
                         frequency scan confirmation against the ACQ DB entries. If the match is successful  pointer to the acq list
                         entry will be returned to the called function otherwise NULL will be returned. GSM scan will be initiated if the 
                         List DB exhausts and the criteria for GSM Scan satisifies..

DEPENDENCIES
                  None

RETURN VALUE
                  Pointer to acquisition entry. A NULL pointer means no 
                  acquisition entry was found that fulfilled selection criteria.

SIDE EFFECTS
                  None.

===========================================================================*/
boolean 
tdsrrccsp_get_list_db_match_acq_entry(void);
                                                                              

/*===========================================================================

FUNCTION        RRCCSP_CHECK_FOR_EPLMN

DESCRIPTION     This function checks the network select mode, and accordingly calls the NAS fucntion to check the
                EPLMN list

DEPENDENCIES
                None

RETURN VALUE    If the network select mode is
                >LIMITED     returns TRUE
                >Otherwise   return the return value of sys_eplmn_list_equivalent_plmn

SIDE EFFECTS
                None

===========================================================================*/
boolean tdsrrccsp_check_for_eplmn
(
  sys_plmn_id_s_type nas_plmn_id
);

/*===========================================================================

FUNCTION  tdsrrccsp_is_other_sub_in_dds

DESCRIPTION

  This function is used to check if other SUB is in DDS or not
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE - Other SUB is in DDS
  FALSE-Other SUB is not in DDS

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_is_other_sub_in_dds(void);

/*===========================================================================

FUNCTION  tdsrrccsp_do_opt_for_con_to_idle

DESCRIPTION

  This function is used to check if we need apply optimization from connection to idle
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE - Apply opt
  FALSE- Don't apply opt

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_do_opt_for_con_to_idle(void);

/*===========================================================================

FUNCTION  tdsrrccsp_handle_preemption_based_on_cs_other_sub

DESCRIPTION

  This function is used to block the preemption if other sub not CS activities.
  Condition 1: if it is connected to idle cell search, block the preempt if other sub is not CS activites.
  Condition 2: if it is a initial cell search , and FREQ SCAN not start , block the preempt if other sub is not CS activites. 
  Condition 3: If other sub want cs call during T CU procedure wait camp on, allow the preempt.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  void

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_handle_preemption_based_on_cs_other_sub(boolean *handle_preempt_ptr );


/*===========================================================================

FUNCTION  RRC_CHECK_CAN_PREEMPTION_BE_HANDLED

DESCRIPTION

  This function checks if preemption can be handled.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE If pre-emption can be handled.
  else
  FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_check_can_preemption_be_handled(void);

/*===========================================================================

FUNCTION  tdsrrc_check_can_preemption_be_handled_in_FACH_DCH

DESCRIPTION

  This function checks if preemption can be handled in FACH & DCH states
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE If pre-emption can be handled.
  else
  FALSE.

SIDE EFFECTS

  None.
===========================================================================*/

boolean tdsrrc_check_preemption_can_be_handled_in_fach_dch(void);

/*===========================================================================

FUNCTION  tdsrrc_check__can_rrc_handle_preemption_in_fach_dch

DESCRIPTION

  This function checks if RRC can handle preemption  in FACH & DCH states
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE If pre-emption can be handled.
  else
  FALSE.

SIDE EFFECTS

  None.
===========================================================================*/

boolean tdsrrc_check_can_rrc_handle_preemption_in_fach_dch(
    tdsrrccsp_preemption_cause_e_type cause_preemption
);


/*===========================================================================

FUNCTION  tdsrrc_check_can_l1_handle_preemption_in_fach_dch

DESCRIPTION

  This function checks if L1 can handle preemption  in FACH & DCH states
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE If pre-emption can be handled.
  else
  FALSE.

SIDE EFFECTS

  None.
===========================================================================*/

boolean tdsrrc_check_can_l1_handle_preemption_in_fach_dch(void);

/*===========================================================================

FUNCTION  tdsrrc_check_can_nas_handle_preemption_in_fach_dch

DESCRIPTION

  This function checks if NAS can handle preemption  in FACH & DCH states
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE If pre-emption can be handled.
  else
  FALSE.

SIDE EFFECTS

  None.
===========================================================================*/

boolean tdsrrc_check_can_nas_handle_preemption_in_fach_dch(void);


/*===========================================================================

FUNCTION  tdsrrc_csp_handle_preemption_in_FACH_DCH

DESCRIPTION

  This function determines the next sub state in FACH & DCH states
    
DEPENDENCIES

  None.
 
RETURN VALUE

Next sub state

SIDE EFFECTS

  None.
===========================================================================*/

tdsrrc_csp_substate_e_type tdsrrc_csp_handle_preemption_in_fach_dch(tdsrrc_cmd_type *);
/*===========================================================================

FUNCTION  tdsrrccsp_initiate_CU_for_immediate_preempt_in_FACH_DCH

DESCRIPTION

  This function iniiates Cell update req in FACH & DCH states
    
DEPENDENCIES

  None.
 
RETURN VALUE

Next sub state

SIDE EFFECTS

  None.
===========================================================================*/

tdsrrc_csp_substate_e_type tdsrrccsp_initiate_CU_for_immediate_preempt_in_fach_dch(void);


/*===========================================================================

FUNCTION  RRC_HANDLE_PREEMPTION_IND

DESCRIPTION

  This function handles pre-emption indication and aborts the ongoing procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Cell Selection Procedure substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_preemtpion_ind( tdsrrc_cmd_type *cmd_ptr );

/*===========================================================================

FUNCTION  RRC_SP_HANDLE_NO_RESOURCE_FROM_L1

DESCRIPTION

  This function handles no resource from L1 ,i.e the case when L1 is not able
  to acquire lock. RRC starts out of service search for such cases.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  CSP substate

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_no_resource_from_l1(void);

/*===========================================================================

FUNCTION  tdsrrc_wrm_scan_type_map_priority

DESCRIPTION

  This function is to map scan type to priority
    
DEPENDENCIES

  None.
 
RETURN VALUE

  trm_reason_enum_t

SIDE EFFECTS

  None.

===========================================================================*/
trm_reason_enum_t tdsrrc_wrm_scan_type_map_priority(tdsrrc_csp_freq_scan_e_type type_freq_scan);

/*===========================================================================

FUNCTION  RRC_CSP_WAIT_FOR_WRM_LOCK

DESCRIPTION

  This function calls another function to request WRM for lock and takes action
  according to the lock status returned.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  CSP substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_wait_for_wrm_lock(tdsrrc_csp_freq_scan_e_type *type_freq_scan);

/*===========================================================================

FUNCTION  RRC_CSP_CONTINUE_WAIT_FOR_WRM_LOCK

DESCRIPTION

  This function starts cells seletion afresh when RRC is not able to acquire lock
  long time.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  CSP substate

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_continue_wait_for_wrm_lock(tdsrrc_csp_freq_scan_e_type *type_freq_scan);


/*===========================================================================

FUNCTION  RRC_SEND_DS_STATUS_IND_TO_L1

DESCRIPTION

  This function sends dual standby status informed by NAS to L1.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_send_ds_status_ind_to_l1
(
  tdsrrc_cmd_type * cmd_ptr
);

/*====================================================================
FUNCTION: tdsrrc_send_no_resource_available_from_l1_to_csp()

DESCRIPTION:
  This function posts NO_RESOURCE_AVAILABLE_FROM_L1 command to 
  internal command queue.

DEPENDENCIES:
  None

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrc_send_no_resource_available_from_l1_to_csp(void);


/*===========================================================================

FUNCTION  RRC_CSP_REL_LOCK_AND_CONT_CELL_SEL_ON_PREV_PREEMPTED

DESCRIPTION

  This function is called after L1 has released lock due to preemption. This function then releases
  RRC lock and starts cell selection from the previous state.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  CSP substate

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_rel_lock_and_cont_cell_sel_on_prev_preempted(void);

/*===========================================================================

FUNCTION  RRC_CSP_WAITING_TO_RELEASE_LOCK

DESCRIPTION

  This function is called to check if RRC is waiting for L1 to release lock.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  TRUE: RRC is waiting for L1 to release lock.
  else
  FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_waiting_to_release_lock(void);
/*===========================================================================

FUNCTION  TDSRRCCSP_IS_PREEMPTION_HANDLED

DESCRIPTION

  This function checks the CSP substate and sends RESET_L1_REQ to L1.

DEPENDENCIES

  None.

RETURN VALUE

  boolean
  Indicates whether preemption handling is taken care or not

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrccsp_is_preemption_handled(tdsrrc_csp_substate_e_type *next_csp_sub_state);

/*===========================================================================

FUNCTION  RRC_CSP_HANDLE_GTOW_LOCK_FAILURE_OR_PREEMPTION

DESCRIPTION

  This function aborts the ongoing inter-RAT procedure in case of pre-emption
  or abort requests from RR.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  CSP substate.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_g2td_lock_failure_or_preemption
(
tdsrrc_out_of_lock_wait_e_type wrm_lock_status, 
boolean abort_due_to_preemption
);

/*===========================================================================

FUNCTION  tdsrrccsp_handle_g2td_abort_during_change_priority

DESCRIPTION

  This function aborts the ongoing inter-RAT procedure in case abort requests from RR during change priority.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_handle_g2td_abort_during_change_priority(tdsrrc_out_of_lock_wait_e_type wrm_lock_status);

/*===========================================================================

FUNCTION        tdsrrccsp_handle_td2td_bplmn_lock_failure

DESCRIPTION     This function will handle the case of lock not granted for WTOW BPLMN

DEPENDENCIES
                None

RETURN VALUE
                Next CSP substate

SIDE EFFECTS
                None

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_td2td_bplmn_lock_failure
(
  tdsrrc_out_of_lock_wait_e_type wtrm_lock_status /*For now no use of this, can plan to use this later*/
);

/*===========================================================================

FUNCTION          tdsrrc_reset_oos_due_to_no_resource_flag

DESCRIPTION       This function is called to reset the flag which track OOS due to 
                          no resource.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrc_reset_oos_due_to_no_resource_flag(void);

/*===========================================================================

FUNCTION          tdsrrc_set_oos_due_to_no_resource_flag

DESCRIPTION       This function is called to set the flag which track OOS due to 
                          no resource.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrc_set_oos_due_to_no_resource_flag(void);

/*===========================================================================

FUNCTION          tdsrrc_get_oos_due_to_no_resource_flag_status

DESCRIPTION       This function is called to get the flag status which tracks OOS due to 
                          no resource.

DEPENDENCIES      None

RETURN VALUE      boolean

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrc_get_oos_due_to_no_resource_flag_status(void);



/*===========================================================================

FUNCTION          tdsrrccsp_check_td2g_abort_in_progress

DESCRIPTION       This function checks if W2G procedure abort is in progress.
                  If so, STOP TDS should be pended till WTOG search abort is 
                  completed
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
boolean tdsrrccsp_check_td2g_abort_in_progress(
  rrc_stop_cause_e_type tdsstop_cause
);

#ifdef FEATURE_BPLMN_SEARCH_320MS
/*===========================================================================

FUNCTION          RRC_CSP_CHECK_320MS_BPLMN

DESCRIPTION       This functions checks if CSP is required to do 320ms lossy BPLMN

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_check_320ms_bplmn(void);

/*===========================================================================

FUNCTION          RRC_SUB_SFN

DESCRIPTION       This function subtract two system frame numbers.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
uint16 tdsrrccsp_sub_sfn(uint16 sfn, uint16 subtract_by);

#endif

/*===========================================================================

FUNCTION          RRC_CSP_CELL_TRANS_CNF_RCVD_AT_LLC

DESCRIPTION       This function is called by LLC on reception of cell_trans_cnf from L1

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_cell_trans_cnf_rcvd_at_llc
(
  void
);


/*===========================================================================

FUNCTION          RRC_CSP_PRINT_RAT_SEARCH_ORDER

DESCRIPTION       This function prints the RAT search order for PLMN_LIST_REQ

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_print_rat_search_order
(
  void
);

/*===========================================================================

FUNCTION          RRCCSP_GENERATE_RAT_ORDER_FOR_AUTO_PLMN_SRCH

DESCRIPTION       This function generates the RAT search order for Automatic PLMN search

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_rat_order_for_auto_plmn_srch
(
  void
);

#ifndef FEATURE_RAT_PRIORITY_LIST
/*===========================================================================

FUNCTION          RRCCSP_GENERATE_RAT_ORDER_FOR_MANUAL_PLMN_SRCH

DESCRIPTION       This function generates the RAT search order for Manual PLMN search

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_rat_order_for_manual_plmn_srch
(
  void
);
#else
/*===========================================================================

FUNCTION          tdsrrccsp_generate_rat_order_for_manual_plmn_srch_from_rat_pri_list

DESCRIPTION       This function generates the RAT search order for Manual PLMN search according to rat_pri_list

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_rat_order_for_manual_plmn_srch_from_rat_pri_list
(
  sys_rat_pri_list_info_s_type *rat_pri_list_info_ptr
);
#endif/*FEATURE_RAT_PRIORITY_LIST*/

/*===========================================================================

FUNCTION          RRCCSP_GENERATE_BPLMN_RAT_SRCH_ORDER_UPDATE_CURR_SRCH_RAT

DESCRIPTION       This function generates the RAT search order for either Manual of Automatic BPLMN search
                         based on n/W sel mode passeds
                         After generation of RAT order for search, will update the curr srch idx to first RAT

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_generate_bplmn_rat_srch_order_update_curr_srch_rat
(
  sys_network_selection_mode_e_type nw_sel_mode
);
/*===========================================================================

FUNCTION          RRC_CSP_INIT_WTOG_PLMN_SRCH_INT_DATA_ACROSS_RATS

DESCRIPTION       This function is responsible for initializing the internal data structure to default values for WTOInterrat search
                         without initalzing num of plmns

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void  tdsrrccsp_init_td2g_plmn_srch_int_data_across_rats
(
  void
);

/*===========================================================================

FUNCTION          tdsrrccsp_handle_bplmn_suspend_lock_rel_offset_tmr_running

DESCRIPTION       This function is called to put WL1 to sleep, stop the 10ms lock release offset timer and
                         stop BPLMN DRX timer
                         This function is called on handling BPLMN suspend and BPLMN cleanup

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_handle_bplmn_suspend_lock_rel_offset_tmr_running
(
  void
);

/*===========================================================================

FUNCTION          tdsrrccsp_check_sib1_validity

DESCRIPTION       This function calls NAS API to verify LAC info provided in SIB1

DEPENDENCIES      None

RETURN VALUE      TRUE: LAC info is valid.
                  FALSE: Otherwise. 

SIDE EFFECTS      None
===========================================================================*/
boolean tdsrrccsp_check_sib1_validity
(
  tdsrrc_SysInfoType1  *sib1
);

#ifdef FEATURE_3GPP_CSFB_TDS

#ifdef FEATURE_L2TD_REDIRECT_R10
/*===========================================================================

FUNCTION          tdsrrccsp_is_redirect_uarfcn_with_sib_container_from_lte

DESCRIPTION      This function returns idx to the uarfcn in the L2T redirected freq list

DEPENDENCIES      None

RETURN VALUE     Index to the uarfcn in the L2T redirected freq list if this uarfcn present and index to any cpid on this
                           uarfcn presents in the sib container list, MAX_UTRAN_TDS_FREQUNECIES Otherwise

SIDE EFFECTS      None
===========================================================================*/
uint8 tdsrrccsp_is_redirect_uarfcn_with_sib_container_from_lte
(
  uint16 uarfcn
);
#endif

/*===========================================================================

FUNCTION          tdsrrccsp_get_idx_sib_cont_list

DESCRIPTION       This function returns idx to the CPID in the SIB container list

DEPENDENCIES      None

RETURN VALUE      Index to the CPID in the SIB container list if present
                          TDSCDMA_RRC_MAX_UTRA_CONT Otherwise

SIDE EFFECTS      None
===========================================================================*/
uint8  tdsrrccsp_get_idx_sib_cont_list
(
  uint8 cpid
);

#ifdef FEATURE_L2TD_REDIRECT_R10
/*===========================================================================

FUNCTION          rrccsp_get_idx_sib_cont_list_r10

DESCRIPTION       This function returns idx to the PSC in the SIB container list

DEPENDENCIES      None

RETURN VALUE      Index to the PSC in the SIB container list if present
                  TDSCDMA_RRC_MAX_UTRA_CONT Otherwise

SIDE EFFECTS      None
===========================================================================*/
uint8  tdsrrccsp_get_idx_sib_cont_list_r10
(
  uint8 uarfcn_idx, uint8 cpid
);
#endif

/*===========================================================================

FUNCTION          tdsrrccsp_free_sib_cont_list

DESCRIPTION       This function will free the memory allocated for SIB cont list. 
                  This will be called on L2TD redirection initialization, successful 
                  camping for L2TD redirection, CSP int data init.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_free_sib_cont_list
(
  void
);

/*===========================================================================

FUNCTION          tdsrrccsp_post_update_sibs_ind

DESCRIPTION       This function will post update SIBS IND to SIB with cause received in arguments

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_post_update_sibs_ind
(
  tdsrrcsib_update_sibs_cause_e_type cause
);
#endif

/*===========================================================================

FUNCTION        TDSRRC_IS_S_CRITERIA_MET

DESCRIPTION
                This function is called during GTOW and WTOW Automatic BPLMN search 
                to determine if the cell satisfies S-criteria before adding the PLMN 
                to the available PLMN list. 

DEPENDENCIES
                None

RETURN VALUE
                TRUE:  S criteria met
                FALSE: S criteria not met

SIDE EFFECTS
                None

===========================================================================*/
boolean tdsrrccsp_is_s_criteria_met(rrc_plmn_identity_type plmn_id, tdsrrc_int_cell_id_type cell_id);

/*===========================================================================
FUNCTION          tdsrrccsp_get_sys_wcdma_band_mask

DESCRIPTION       This function will return supported TDS band mask

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_get_sys_wcdma_band_mask
(
  void
);

/*===========================================================================

FUNCTION          tdsrrccsp_get_sys_tdscdma_band_mask

DESCRIPTION       This function will return supported TDSCDMA band mask

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
sys_band_mask_type tdsrrccsp_get_sys_tdscdma_band_mask
(
  void
);
/*===========================================================================

FUNCTION          RRC_CSP_SEND_INTERRAT_CELL_CHANGE_IND

DESCRIPTION       This function is responsible for filling up data structures
                  for INTERRAT_CELL_CHANGE_IND to MM sent by CSP only for GTOW
                  PCCO. This also allocates cmd buffer and sends the command.
                  It also frees allocated command and embedded buffers
                  when failure is detected before sending the command.

DEPENDENCIES

RETURN VALUE
                  None.
                  
SIDE EFFECTS
                  None.
===========================================================================*/
void tdsrrccsp_send_interrat_cell_change_ind
(
void
);

/*===========================================================================

FUNCTION          RRC_TRANSLATE_ACCESS_CLASS_BARRED_LIST

DESCRIPTION       This function is responsible for translating the access 
                  class barred list from the ASN1 defined linked list format
                  to RRC internal array structure.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrc_translate_access_class_barred_list(tdsrrc_AccessClassBarredList 
                                             *sib_access_class_barred_list_ptr,
                                            tdsrrc_int_access_class_barred_list_type
                                             *access_class_barred_list_ptr);

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)

#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION          tdsrrccsp_reset_flag_for_excluding_pre_redir_info

DESCRIPTION       This function resets the flag to not include pre-redir info in 
                  connection request message.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_reset_flag_for_excluding_pre_redir_info(void);

/*===========================================================================

FUNCTION          tdsrrccsp_exclude_preredirection_info

DESCRIPTION       This function checks if pre-redirection info has to be excluded
                  from RRC connection request message.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
boolean tdsrrccsp_is_preredirection_info_to_be_excluded(void);

/*===========================================================================

FUNCTION          tdsrrccsp_lte_earfcn_broadcasted_in_sib19

DESCRIPTION       This function check if there is a valid SIB19 with valid 
                  LTE frequencies broadcasted in the current camped cell.

DEPENDENCIES
                  None.

RETURN VALUE
                  TRUE or FALSE

SIDE EFFECTS      None.
===========================================================================*/
boolean tdsrrccsp_lte_earfcn_broadcasted_in_sib19(tdsrrc_SysInfoType19 * sib19_ptr);

#endif


#ifdef FEATURE_LTE_TO_TDSCDMA
/*===========================================================================

FUNCTION          RRCCSP_HANDLE_RESELECTION_REQ_FROM_LTE

DESCRIPTION       This function process the reselection request from LTE

DEPENDENCIES      None

RETURN VALUE      Next CSP Substate

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_reselection_req_from_lte
(
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION          RRCCSP_HANDLE_PLMN_SRCH_REQ_FROM_LTE

DESCRIPTION       This function process the plmn search request from LTE

DEPENDENCIES      None

RETURN VALUE      Next CSP Substate

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_plmn_srch_req_from_lte
(
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION          RRCCSP_HANDLE_REDIRECTION_REQ_FROM_LTE

DESCRIPTION       This function process the redirection request from LTE

DEPENDENCIES      None

RETURN VALUE      Next CSP Substate

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_redirection_req_from_lte
(
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION          RRCCSP_HANDLE_CGI_REQ_FROM_LTE

DESCRIPTION       This function process the cgi request from LTE

DEPENDENCIES      None

RETURN VALUE      Next CSP Substate

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_handle_cgi_req_from_lte
(
  tdsrrc_cmd_type *cmd_ptr
);

#endif
#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION          TDSRRCCSP_UPDATE_BPLMN_LTE_INFO_FROM_RAT_PRI_LIST

DESCRIPTION       This function updates the LTE BPLMN variables in rrc_csp_int_data from rat pri list

DEPENDENCIES
                         None

RETURN VALUE
                         None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_update_bplmn_lte_info_from_rat_pri_list
(  
  sys_rat_pri_list_info_s_type *rat_pri_list,
  boolean                       tdd_band_only
);
#endif

/*===========================================================================

FUNCTION          TDSRRCCSP_UPDATE_IRAT_DETECTED_FREQUENCIES

DESCRIPTION       This function updates the detected frequencies in rrc_csp_int_data with detected frequency list
                          received from other RAT

DEPENDENCIES
                     None

RETURN VALUE
                     None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_update_irat_detected_frequencies(  detected_frequency_list_type * detected_frequency_list);


/*===========================================================================

FUNCTION          rrc_csp_add_frequency_to_detected_cell_list

DESCRIPTION       This function adds WCDMA frequency to detected frequency list

DEPENDENCIES
                  None.

RETURN VALUE
                  None.

SIDE EFFECTS      None.
===========================================================================*/
void tdsrrc_csp_add_frequency_to_detected_cell_list
(
  detected_frequency_type *detected_frequency
);

#endif /* #if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA) */

/*===========================================================================

FUNCTION          tdsrrccsp_init_first_band_to_be_scanned

DESCRIPTION       This function will init the first band to be scanned.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
void tdsrrccsp_init_first_band_to_be_scanned(void);

/*===========================================================================

FUNCTION          tdsrrccsp_restart_cell_selection

DESCRIPTION       This function restarts initial cell selection from 
                  reading ACQ DB.

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_restart_cell_selection
(
   tdsrrc_csp_substate_e_type csp_curr_state
);

/*=========================================================================

 FUNCTION     : tdsrrccsp_update_det_cells_with_jds_cells
 
 DESCRIPTION  : When any cell is barred, RRC gets JDS cells from L1 of the
   same frequency to replace the remaining detected cells, because JDS cell
   are more accurate. This function is handling the replacement.
              
 DEPENDENCIES : 

 INPUT PARAMETERS :
   tdsl1_jds_cell_list : number of JDS cells and their CPID.

 RETURN VALUE : 
   void.
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
void tdsrrccsp_update_det_cells_with_jds_cells
(
  tdsl1_jds_cell_list_type *tdsl1_jds_cell_list
);

void tdsrrccsp_send_reselection_failure
(
  void
);





/*===========================================================================

FUNCTION TDSRRC_REGISTER_SERVING_CELL_CHANGE_IND_CB_INT

DESCRIPTION

This function enables upper layer to register a callback function if notification
of the serving cell id is required.  Once the function is registered, it will be 
called whenever there is a serving cell change.


DEPENDENCIES

  None.
  
RETURN VALUE
  
  BOOLEAN:  TRUE signifies that registration has succeeded.  FALSE signifies
            that registration failed.

SIDE EFFECTS

  None.

===========================================================================*/

boolean tdsrrc_register_serving_cell_change_ind_cb_int
(
  TDSRRC_SERVING_CELL_CHANGE_IND_CB_FUNC_TYPE *cb_ptr
);


/*===========================================================================

FUNCTION RRC_DEREGISTER_SERCING_CELL_CHANGE_IND_CB_INT

DESCRIPTION

This function enables upper layer to deregister the callback function that was
registered for notification of serving cell id.


DEPENDENCIES

  None.
  
RETURN VALUE

  BOOLEAN:  TRUE signifies that deregistration has succeeded.  FALSE signifies
            that deregistration failed.  

SIDE EFFECTS

  None.

===========================================================================*/

boolean tdsrrc_deregister_serving_cell_change_ind_cb_int
(
   TDSRRC_SERVING_CELL_CHANGE_IND_CB_FUNC_TYPE *cb_ptr
);

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
/*===========================================================================

FUNCTION          tdsrrc_get_rr_inter_rat_bplmn_prtl_result

DESCRIPTION       TDSRRC Calls the respective RR API to get Partial PLMN results from RR during ongoing TtoG
                  Manual PLMN Search

DEPENDENCIES
                  None
                  
RETURN VALUE
                  Boolean

SIDE EFFECTS
                  None
===========================================================================*/
boolean tdsrrc_get_rr_inter_rat_bplmn_prtl_result(sys_detailed_plmn_list_s_type *available_plmn_list);
#endif

/*===========================================================================
FUNCTION          tdsl1_rrc_can_l1_go_to_sleep

DESCRIPTION       This function is called by L1 in every DRX cycle to determine
                  if L1 should go to sleep or not. This function call was 
                  introduced due to TD2TD and TD2G BPLMN search. If TD2TD search needs to be
                  done during the DRX sleep time, then L1 should NOT go to sleep. Else
                  if TD2G search needs to be done then L1 should go to sleep.

DEPENDENCIES      NONE

RETURN VALUE      
                  TRUE:  L1 can go to sleep.
                  FALSE: Otherwise
                                  
SIDE EFFECTS

===========================================================================*/
boolean tdsrrc_can_l1_go_to_sleep
(
  uint32 timer_val,
  boolean *wtog_bplmn_in_progress
  ,boolean *wtow_bplmn_in_progress
  ,boolean is_new_drx_cycle
);

void tdsrrccsp_send_reselection_failure
(
  void
);


/*=========================================================================

 FUNCTION     : tdsrrccsp_is_dummy_plmn_id
 
 DESCRIPTION  : This function checks the input PLMN to match dummy PLMN
                which NAS uses for special purpose.
              
 DEPENDENCIES : 

 INPUT PARAMETERS :
   req_plmn : input PLMN for checking.

 RETURN VALUE : 
   TRUE: it is dummy PLMN
   FALSE: it is not dummy PLMN
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
boolean tdsrrccsp_is_dummy_plmn_id
(
  rrc_plmn_identity_type req_plmn
);

/*=========================================================================

 FUNCTION     : tdsrrccsp_is_dummy_plmn_search
 
 DESCRIPTION  : This function checks the input PLMN to match dummy PLMN
                and the search for CR504264
              
 DEPENDENCIES : 

 INPUT PARAMETERS :
   req_plmn : input PLMN for checking.

 RETURN VALUE : 
   TRUE: it is dummy PLMN search
   FALSE: it is not dummy PLMN search
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
boolean tdsrrccsp_is_dummy_plmn_search
(
  rrc_plmn_identity_type req_plmn
);

/*===========================================================================

FUNCTION  TDSRRC_DOG_REPORT_AND_START_TIMER

DESCRIPTION
 
  This function reports dog and restarts the dog timer.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_dog_report_and_start_timer(void);

#ifdef FEATURE_SGLTE
/*=========================================================================

 FUNCTION     : tdsrrc_get_ue_mode
 
 DESCRIPTION  : This function returns UE mode understood by TL1, by mapping the UE mode from NAS, i.e.,
 
 SYS_UE_MODE_NONE = -1,
 SYS_UE_MODE_NORMAL,
 SYS_UE_MODE_SGLTE_HOME,
 SYS_UE_MODE_SGLTE_ROAM,
 SYS_UE_MODE_CSFB_ONLY,
 SYS_UE_MODE_SGLTE_TEST_ONLY
              
 DEPENDENCIES : 

 INPUT PARAMETERS :

 RETURN VALUE : 
 TDSL1_MEAS_MODE_NORMAL or TDSL1_MEAS_MODE_SGLTE.
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
tdsl1_meas_mode_enum_type tdsrrc_get_ue_mode
(
  void
);
/*=========================================================================

 FUNCTION     : tdsrrccsp_get_absolute_ue_mode
 
 DESCRIPTION  : This function returns UE mode for RRC, by mapping the UE mode from NAS, i.e.,
 

 DEPENDENCIES : 

 INPUT PARAMETERS :

 RETURN VALUE : 
   SYS_UE_MODE_NONE = -1,
   Ue Mode None - INTERNAL USE 

  SYS_UE_MODE_NORMAL,
   NORMAL is UE Mode used when UE is in legacy behavior.
   

  SYS_UE_MODE_SGLTE,
  SGLTE: mode for simultaneous voice and data. UE can function in either
   single stack or dual-stack.
   

  SYS_UE_MODE_CSFB,
   CSFB: mode for circuit-switched fallback.
   

  SYS_UE_MODE_CSFB_ONLY,
   CSFB_ONLY: This is CSFB mode, and UE is not allowed to move to SV_DATA.
   

  SYS_UE_MODE_SGLTE_ONLY,
   SGLTE_ONLY: this is used only for testing. It is SGLTE mode and UE
    will not move out of this mode.
   

  SYS_UE_MODE_SVLTE,
   SVLTE: mode for simultaneous voice and data. UE can function in either
    single stack or dual-stack.
   

  SYS_UE_MODE_SVLTE_ONLY,
   SVLTE_ONLY: this is used only for testing. It is SVLTE mode and UE
    will not move out of this mode.
   

  SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED,
    SYS_UE_MODE_1XSRLTE_CSFB_PREFERRED: Attempt to connect as CSFB, but fallback
     to 1XSRLTE if CSFB is not supported on the network.
   

  SYS_UE_MODE_GSMSRLTE_CSFB_PREFERRED,
    SYS_UE_MODE_GSMSRLTE_CSFB_PREFERRED: Attempt to connect as CSFB, but fallback
     to GSMSRLTE if CSFB is not supported on the network.
   

  SYS_UE_MODE_GSMSRLTE_ONLY,
   GSMSRLTE_ONLY: this is used only for testing. It is GSMSRLTE mode and UE
    will not move out of this mode.
   

  SYS_UE_MODE_1XSRLTE_ONLY
   1XSRLTE_ONLY: this is used only for testing. It is 1XSRLTE mode and UE
    will not move out of this mode.
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/
sys_ue_mode_e_type tdsrrccsp_get_absolute_ue_mode(void);
#endif
#ifdef FEATURE_WRLF_SYSTEM_SEL
/*=========================================================================

 FUNCTION     : tdsrrc_set_connecting_oos_flag
 
 DESCRIPTION  : This function set tdsrrc_csp_int_data.ue_in_connecting_oos flag,

              
 DEPENDENCIES : 

 INPUT PARAMETERS :is_connecting_oos

 RETURN VALUE : 
 void
       
 SIDE EFFECTS : 
   N/A
==========================================================================*/

void tdsrrc_set_connecting_oos_flag(boolean is_connecting_oos);


/*===========================================================================

FUNCTION          tdsrrccsp_is_suitable_srch_ended

DESCRIPTION       This function checks if suitable search has ended

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
boolean tdsrrccsp_is_suitable_srch_ended(void);

#endif

/*===========================================================================

FUNCTION        tdsrrc_csp_return_pending_type

DESCRIPTION
                This function returns the CSP pending command type.

DEPENDENCIES
                None

RETURN VALUE
                CSP Pending type.

SIDE EFFECTS
                None

===========================================================================*/

tdsrrc_csp_pending_e_type tdsrrc_csp_return_pending_type(void);

/*===========================================================================
FUNCTION          tdsrrccsp_reselection_in_progress

DESCRIPTION       This function is responsible for returning the current 
                  status of Reselection in progress.
DEPENDENCIES
                  None

RETURN VALUE
                  A status that indicates if a Reselection is in progress
                  or not.
                  
SIDE EFFECTS      None
===========================================================================*/
boolean  tdsrrccsp_reselection_in_progress(void);

/*=========================================================================

 FUNCTION     : tdsrrccsp_abort_cell_reselection
 
 DESCRIPTION  : This function aborts the cell reselection if any.
              
 DEPENDENCIES : None

 INPUT PARAMETERS : None

 RETURN VALUE : 
  TRUE - the reselection is absent or aborted successfully;
  FALSE - fail to abort the reselection;
       
 SIDE EFFECTS : 
   This function will update CSP substate and send commands to LLC.
==========================================================================*/
boolean tdsrrccsp_abort_cell_reselection(void);
/*===========================================================================

FUNCTION   tdsrrccsp_cleanup_needed_bplmn_data

DESCRIPTION

  This function cleans the needed BPLMN data after sending TDSRRC_BPLMN_SUSPEND_REQ to CSP
  to suspend the ongoing Manual or Automatic Background PLMN Search.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.
 
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccsp_cleanup_needed_bplmn_data(void);

/*===========================================================================

FUNCTION   TDSRRC_SEND_IRAT_ABORT_IND

DESCRIPTION

  This function sends a TDSRRC_IRAT_ABORT_IND to the corresponsing
  Interrat procedure to abort the ongoing Interrat operation 
  inorder to service a higher priority request from MM
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_send_irat_abort_ind(tdsrrc_proc_e_type tdsrrc_proc);


/*===========================================================================

FUNCTION          tdsrrccsp_set_to_suitable_or_acceptable_substate

DESCRIPTION       This function sets csp substate to suitable or acceptable.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  Next CSP substate.
SIDE EFFECTS
                  None

===========================================================================*/
tdsrrc_csp_substate_e_type tdsrrccsp_set_to_suitable_or_acceptable_substate(void);

/*===========================================================================

FUNCTION          tdsrrccsp_check_is_suitable_or_acceptable_substate

DESCRIPTION       This function checks that csp substate should be set to suitable or acceptable.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  TRUE for suitable substate and FALSE for acceptable state.
SIDE EFFECTS
                  None

===========================================================================*/
boolean tdsrrccsp_check_is_suitable_or_acceptable_substate();

/*===========================================================================

FUNCTION          tdsrrccsp_store_tdd_target_info_freqs

DESCRIPTION       This function stores the uarfcns from tdsrrc_csp_int_data.target_tdd_cell_list to csp internal data
                  structure, while processing L2T redirection req msg.
                  For R9 redirection, tdsrrc_csp_int_data.target_tdd_cell_list contains LTE sib6 ngbr freqs;
                  For R10 redirection, tdsrrc_csp_int_data.target_tdd_cell_list contains redirect target freqs.
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_store_tdd_target_info_freqs(void);

#ifdef FEATURE_L2TD_REDIRECT_OPT_IRAT_ACQ_DB
/*===========================================================================

FUNCTION          tdsrrccsp_store_lte_tds_irat_acq_db_freqs

DESCRIPTION       This function stores lte tds irat acq db freqs to csp internal data structure, while processing L2T
                  redirection req msg. It is agreed with LTE RRC that the irat acq db entries are sorted by LTE side
                  based on the success_cnt in descending order before sending to TDS RRC. 
                  For R10 redirect, the freqs are stored in high_cnt and low_cnt two lists per
                  threshold from NV. For R9 redirect, the freqs are all stored in low_cnt list.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_store_lte_tds_irat_acq_db_freqs
(
  tds_rrc_lte_redir_req_type *lte_redir_req
);

/*===========================================================================

FUNCTION          tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r9

DESCRIPTION       This function checks and removes dup uarfcns passed by LRRC in R9 L2T redirection req msg, in the
                  priority order of redirected freq, irat acq db freq, lte sib6 ngbr freqs.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r9(void);

/*===========================================================================

FUNCTION          tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r10

DESCRIPTION       This function checks and removes dup uarfcns passed by LRRC in R10 L2T redirection req msg, in the
                  priority order of high_cnt irat acq db freq, redirected target freq list, low_cnt irat acq db freq,
                  lte sib6 ngbr freqs.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_rmv_dup_freq_from_lte_redirect_req_r10(void);

/*===========================================================================

FUNCTION          tdsrrccsp_update_target_tdd_cell_list_freqs

DESCRIPTION       This function updates the uarfcns in tdsrrc_csp_int_data.target_tdd_cell_list to be used for cell selection
                  during L2T redirection. For R9 redirect, the priority order is: irat acq db freqs (stored in
                  low_cnt_irat_acqdb_freq_list), lte sib6 ngbr freqs. For R10 redirect, the priority order is: high cnt
                  irat acq db freq list, redirect target freq list, low cnt irat acq db freq list, lte sib6 ngbr freq list.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  None
SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrccsp_update_target_tdd_cell_list_freqs(void);
#endif

 /*===========================================================================

FUNCTION          tdsrrc_mt_call_is_there_call_on_other_sub

DESCRIPTION       This function checks whether MT call is on other sub.
                             
DEPENDENCIES
                  None
RETURN VALUE
                  TRUE for MT call on the other sub.
SIDE EFFECTS
                  None

===========================================================================*/
boolean tdsrrc_mt_call_is_there_call_on_other_sub();

/*===========================================================================

FUNCTION        tdsrrccsp_get_system_time

DESCRIPTION     This function will return the current system time

DEPENDENCIES
                None

RETURN VALUE
                uint32

SIDE EFFECTS
                None

===========================================================================*/
extern uint32 tdsrrccsp_get_system_time(void);
/*===========================================================================

FUNCTION          tdsrrccsp_bar_forbid_cells

DESCRIPTION       This function the forbidden cell status

DEPENDENCIES      None

RETURN VALUE      None

SIDE EFFECTS      None
===========================================================================*/

boolean tdsrrccsp_bar_forbid_cells(tdsrrc_SysInfoType3 *sib3_ptr,
                                      rrc_plmn_identity_type plmn_id, tdsrrc_MasterInformationBlock *mib_ptr,tdsrrc_SysInfoType1 *sib1_ptr);

/*===========================================================================

FUNCTION tdsrrccsp_compare_forbidden_cell

DESCRIPTION
  This  function is used to compare the cell whether it is forbidden cell form efs.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: it is Forbidden cell
  FAlsE:it is not Forbidden cell

===========================================================================*/
boolean tdsrrccsp_compare_forbidden_cell(sys_plmn_id_s_type sys_plmn_id,uint32 cell_id,uint32 lac);
#endif /* #ifndef TDSRRCCSPI_H */

