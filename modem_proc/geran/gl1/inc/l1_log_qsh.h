#ifndef L1_QSH_LOG_H
#define L1_QSH_LOG_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
            G E R A N  QSH LOG

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2014 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================
 when       who       what, where, why
 --------   --------  ----------------------------------------------------------
27/01/20   rks        CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
16/08/17    km        CR2089992 DTF handler to abort G2L IRAT search in idle mode
27/12/16    sm        CR1105461 Resolve Compilation error when QHS flags are enabled.Compilation error is because of CR1089963.
15/11/16    sm        CR1089963 Added state transition events and BCCH abort in W2G stare handler, to reproduce issue CR/1082619
16/03/17    mn        CR2020307 FR40846:Packet transfer Error Recovery mechanism.
21/07/16    sm        CR1044487 Added DTF handler to trigger panic reset.
30/06/16    sm        CR1035750 Added common event to logging all failure events.
13/06/16    nm        CR1024602 PACKET Transfer DTF Events/Handlers Phase2
09/06/16    mn        CR1026351 Resolve linking error in QTF mob build
31/05/16    mn        CR1022253 Idle DTF/Handlers
26/05/16    sk        CR1020757 Add DTF events for G2X IRAT scenarios
18/5/16     sm        CR1017901 Added GL1 idle mode QSH events 
18/5/16     sn        CR1007565 PACKET Transfer DTF Events/Handlers Phase1
02/05/16    mk        CR1010068 Add DTF events in dedicated mode
13/05/16    km        CR1012528 DTF sleep scenario "Forced error Recovery"
12/05/16    akm       CR1010520 DTF sleep scenario "PCH and NCELL activity cancellation"
09/05/16    sm        CR1002861 Added QSH GL1 handler : PCH collision with Ncell activity,TRM updation(reserve/cancel)and continuous Ncell activity failure  .  
11/12/15    snjv      CR950902 QSH: Get recent Tx metric on Timer expiry after state change
14/10/15    snjv      CR919955 FR38046:QSH Call End Data Collection
30/09/15    sk        CR916227 Disable frequent QSH events and add PCH rsv handler
10/09/15    sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
11/05/15    pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
19/02/15    ab        Initial Revision.
20/02/15    ab        CR773948 GL1 Support for provding Dumps and Metrics for QSH

 ===========================================================================*/
                     
/*===========================================================================
                                         INCLUDE FILES FOR MODULE
===========================================================================*/
#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "l1i.h"
#include "gl1_msg_g.h" // for gl1_msg_rx_rpt & gl1_msg_chan_type enum
#include "l1_fm.h"
#include "gsmdiag.h"
#include "gsmdiag_v.h"

#include "qsh.h"
#include "qsh_util.h"
#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined(FEATURE_QSH_MDUMP)

#include "gl1_qsh_ext.h"
#endif

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined ( FEATURE_QSH_EVENT_NOTIFY_TO_QSH ) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER ) || defined(FEATURE_QSH_MDUMP)


void gl1_qsh_register(void);
void gl1_gfw_qsh_register(void);
int64 gl1_get_rem_time_in_ms(uint32 rem_sclk);
void gl1_qsh_notify_state_transition(gas_id_t gas_id ,uint16 newState);
#ifdef FEATURE_QSH_EVENT_METRIC
void gl1_process_qsh_metric_ind
(
  l1_qsh_metric_ind_T *msg_ptr,
  gas_id_t            gas_id
);

typedef struct{
  uint8                           *start_addr;
  gas_id_t                        gas_id;
  qsh_client_metric_context_id_t  metric_context_id;
}gl1_qsh_metric_bufffer_t;

void gl1_qsh_write_tx_metric_on_reset(int32 metric, gas_id_t gas_id);

void gl1_qsh_cache_tch_rx_params (int32 pri_rxlev_full, 
                                  int32 pri_rxlev_sub,
                                  int32 sec_rxlev_full, 
                                  int32 sec_rxlev_sub,
                                  uint8 rxqual_full, 
                                  uint8 rxqual_sub,
                                  gas_id_t gas_id);

#endif /* FEATURE_QSH_EVENT_METRIC */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
typedef enum
{
  GL1_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE = 0x01, // Use for Add flush only 
  GL1_QSH_EVENT_OOS,
  GL1_QSH_EVENT_RESEL,
  GL1_QSH_EVENT_PAGE_WUP,
  GL1_QSH_EVENT_ACQ_PSCAN,
  GL1_QSH_EVENT_ACQ_LIST_DECODE,
  GL1_QSH_EVENT_EARLY_CAMP_BCCH_DECODE,
  GL1_QSH_EVENT_IRAT_MEAS_GAP,
  GL1_QSH_EVENT_SLEEP_EARLY_CAMP,
  GL1_QSH_EVENT_IRAT_RESEL,
  GL1_QSH_EVENT_IRAT_CCO,
  GL1_QSH_EVENT_IRAT_HO,
  GL1_QSH_EVENT_RACH,
  GL1_QSH_EVENT_GBTA,
  GL1_QSH_EVENT_QTA,
  GL1_QSH_EVENT_G2W_HO_FAILURE,
  GL1_QSH_EVENT_RESEL_FAILURE,
  GL1_QSH_EVENT_CON_REL_L2_RESET,
  GL1_QSH_EVENT_PAGE_DEC_FAIL,
  GL1_QSH_EVENT_PAGE_MISS,
  GL1_QSH_EVENT_EMPTY_QTA_GAP,
  GL1_QSH_EVENT_ASDIV_SWITCH,
  GL1_QSH_EVENT_TRM_LOCK_WAIT_TIMEOUT,
  GL1_QSH_EVENT_RX_HIGH_TIMING_ERROR,
  GL1_QSH_EVENT_RX_HIGH_FREQ_ERROR,
  GL1_QSH_EVENT_HIGH_RXLEV_DIFF_PRI_DIV,
  GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_RXPWR,
  GL1_QSH_EVENT_CRC_FAIL_BAD_SNR_GOOD_RXPWR,
  GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_BAD_RXPWR,
  GL1_QSH_EVENT_HRXQUAL_DROP_GOOD_RXPWR,
  GL1_QSH_EVENT_BEFORE_PARAM_VALID,
  GL1_QSH_EVENT_AFTER_PARAM_VALID,
  GL1_QSH_EVENT_TX_HOMING_SEQ_VOC_NOT_READY,
  GL1_QSH_EVENT_TX_HOMING_SEQ_VOC_READY,
  GL1_QSH_EVENT_DTX_SID_FIRST_TX,
  GL1_QSH_EVENT_DTX_SID_UPDATE_TX,
  GL1_QSH_EVENT_DTX_NO_DATA_TX,
  GL1_QSH_EVENT_DTX_SID_FIRST_RX,
  GL1_QSH_EVENT_DTX_SID_UPDATE_RX,
  GL1_QSH_EVENT_DTX_NO_DATA_RX,
  GL1_QSH_EVENT_HR_VALID_SID_RCVD,
  GL1_QSH_EVENT_FACCH_FILLER_RXED,
  GL1_QSH_EVENT_FACCH_TX,
  GL1_QSH_EVENT_VALID_FACCH_RX,
  GL1_QSH_EVENT_BFI_RCVD,
  GL1_QSH_EVENT_VOCODER_READY,
  /* QSH PKT TRANSFER */
  GL1_QSH_EVENT_TBF_CONNECT,
  GL1_QSH_EVENT_TBF_RECONFIG, /* RECONFIG received */
  GL1_QSH_EVENT_TBF_RELEASE,  /* RELEASE received */
  GL1_QSH_EVENT_TBF_SUSPEND,
  GL1_QSH_EVENT_TBF_RESUME,
  GL1_QSH_EVENT_LOW_TS_CHHANGE,
  GL1_QSH_EVENT_GBTA_NONE,
  GL1_QSH_EVENT_GBTA_ALLOWED,
  GL1_QSH_EVENT_GBTA_ACTIVE,
  GL1_QSH_EVENT_GBTA_BLOCKED,
  GL1_QSH_EVENT_SC_ACTIVITY_START,
  GL1_QSH_EVENT_SC_ACTIVITY_STOP,
  GL1_QSH_EVENT_IDLE_FRAME_IN_PTM,
  GL1_QSH_EVENT_PTCCH_FRAME_IN_PTM,
  GL1_QSH_EVENT_DS_ABORT,
  GL1_QSH_EVENT_2PHASE_ACESS,
  GL1_QSH_EVENT_TA_IN_RACH_STATE,
  GL1_QSH_EVENT_TA_IN_2PHASE_ACCESS,
  GL1_QSH_EVENT_HIGH_BLER,
  GL1_QSH_EVENT_EXTENDED_PAGING,
  GL1_QSH_EVENT_PAGING_REORGANIZATION,
  GL1_QSH_EVENT_NON_DRX_START_IND,
  GL1_QSH_EVENT_NON_DRX_STOP_IND,
  GL1_QSH_EVENT_FIND_BCCH_STATE_TRANSITION,
  GL1_QSH_EVENT_BPLMN_SRCH_START,
  GL1_QSH_EVENT_BPLMN_SRCH_ABORT,
  GL1_QSH_EVENT_BPLMN_SRCH_SUSPEND,
  GL1_QSH_EVENT_BPLMN_SRCH_RESUME,
  GL1_QSH_EVENT_W2G_BPLMN_SRCH_START,
  GL1_QSH_EVENT_W2G_BPLMN_SRCH_END,
  GL1_QSH_EVENT_W2G_BPLMN_SRCH_SUSPEND,
  GL1_QSH_EVENT_W2G_BPLMN_SRCH_RESUME,
  GL1_QSH_EVENT_G2W_BPLMN_SRCH_START,
  GL1_QSH_EVENT_G2W_BPLMN_SRCH_END,
  GL1_QSH_EVENT_G2W_BPLMN_SRCH_SUSPEND,
  GL1_QSH_EVENT_G2W_BPLMN_SRCH_RESUME,
/* QSH PKT TRANSFER */
/* QSH IRAT*/
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_START_ID,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_START_RECONFIRM,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_START_DED,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_SDCCH_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_SDCCH_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_DTM_TCH_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_DTM_TCH_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_GPRSTRANS_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_GPRSTRANS_RECONFIRM,
  GL1_QSH_EVENT_G2L_IRAT_SEARCH_START_DED,
  GL1_QSH_EVENT_G2L_IRAT_SEARCH_START_IDLE,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_ID_REPORT,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_RECONFIRM_REPORT,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_ID_ABORT,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_RECONFIRM_ABORT,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_ID_TBF_ABORT,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_DED_ID,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_DED_ABORT,
  GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_DED_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_SDCCH_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_DTM_TCH_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_GPRSTRANS_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_IDLE_ID,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_SDCCH_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_DTM_TCH_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_GPRSTRANS_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_IDLE_RECONFIRM,
  GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_ABORT,
  GL1_QSH_EVENT_G2L_IRAT_SEARCH_END_IDLE,
  GL1_QSH_EVENT_G2L_IRAT_SEARCH_END_DED,
/* QSH IRAT*/
/* QSH IDLE*/
 GL1_QSH_EVENT_EXTENDED_PAGE_REM_TIME,
 GL1_QSH_EVENT_SCH_ACQ_SUCCESS,
 /* QSH IDLE*/
 /*State transistion */
  GL1_QSH_EVENT_INACTIVE_STATE_TRANSITION,
  GL1_QSH_EVENT_NULL_STATE_TRANSITION,
  GL1_QSH_EVENT_SUSPEND_STATE_TRANSITION,  
  GL1_QSH_EVENT_RESET_STATE_TRANSITION, 
  GL1_QSH_EVENT_WTOG_BPLMN_STATE_TRANSITION,  
  GL1_QSH_EVENT_FIND_SCH_STATE_TRANSITION, 
  GL1_QSH_EVENT_IDLE_STATE_TRANSITION,
  GL1_QSH_EVENT_GPRS_IDLE_STATE_TRANSITION,  
  GL1_QSH_EVENT_RANDOM_ACCESS_STATE_TRANSITION,
  GL1_QSH_EVENT_PACKET_ACCESS_STATE_TRANSITION,
  GL1_QSH_EVENT_DEDICATED_STATE_TRANSITION, 
  GL1_QSH_EVENT_TRANSFER_STATE_TRANSITION,  
  GL1_QSH_EVENT_DTM_STATE_TRANSITION, 
  GL1_QSH_EVENT_MSIM_DUMMY_STATE_TRANSITION,
  GL1_QSH_EVENT_MSIM_DECOUPLED_STATE_TRANSITION,
 /*State transastion*/
  GL1_QSH_EVENT_INVALID  = 0xff
}gl1_qsh_dtf_event_id_enum_T;

typedef struct
{
  uint64 gl1_qsh_dtf_event_id_mask[4];
}gl1_qsh_dtf_event_data_t;

#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif


#if defined ( FEATURE_QSH_EVENT_NOTIFY_TO_QSH ) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
void gl1_qsh_configure_event_data( qsh_client_cfg_s cfg);
void gl1_qsh_event_notify(gas_id_t gas_id, gl1_qsh_dtf_event_id_enum_T event_id);
boolean gl1_qsh_check_failure_event(gl1_qsh_dtf_event_id_enum_T event_id,uint8 gas_id);
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif


#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

#endif /* L1_QSH_LOG_H */
