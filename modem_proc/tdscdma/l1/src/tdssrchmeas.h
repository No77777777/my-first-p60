#ifndef SRCHMEAS_H
#define SRCHMEAS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            S R C H M E A S . H                  

GENERAL DESCRIPTION
  This module handles periodic measurement requests from RRC.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2009 by Qualcomm Technologies Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/tdssrchmeas.h_v   1.0   11 Mar 2002 16:57:06   cemani  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchmeas.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/10/14   zli     Handle race condition between ERUCCH and T2L
05/03/10   sv      Fixed compiler warnings.
09/09/09   rvs     Added function tdssrchmeas_get_cme_filt_idx() to get filter
                   index of Event 1A for CME if available.
06/26/09   sa      Added MCM failure cause enum data type.
04/22/09   stk     Compiler warning fixes	
10/03/08   hk      Bringing in 7k Mailine fixes
03/17/07   sv      Externed the "srchmeas_print_cell_info" function. 
11/29/06   nd      Added multiple layer3 filter support for intra freq measurement
                   under FEATURE_TDSCDMA_MULTIPLE_L3_FILTERS_SUPPORTED
11/01/06   cpe     Mainlined the following features:
                    -FEATURE_STTD_CAPABLE
08/23/06   rc      Added support for inter-freq virtual active set under 
                   FEATURE_WCDMA_VASET_SUPPORT.
06/05/06   sv      Added prototype for update cells with meas info function.
01/10/06   ub      Added prototypes for cell info copy functions
08/05/05   ub      Added function prototypes for initializing meas filter
04/11/05   cpe     Code addition for resuming to old config upon HHO failure. 
05/26/04   cpe     Added an extern
12/09/03   yshi    Support for GSM Quadband
11/13/03   src     Added declaration for tdssrchmeas_intra_freq_meas_performed
                   to track measurements when HHOs are performed.
11/12/03   cpe     Extern'ed a few functions.
11/04/03   cpe/src Added declaration for tdssrchmeas_determine_sttd_behavior.
10/31/03   cpe     Removed the FEATURE_WCDMA_MULTIPLE_INTRA_MEAS_SUPPORTED 
                   feature which is always present
05/23/03   cpe     Added support for multiple intra freq measurements
03/06/03   djm     addition of tdssrchmeas_inter_s_find_cell, a find
                   of GSM cells based on ARFCN, BSIC.
10/01/02   ddh     Added changes for InterRat processing
09/11/01   cpe     Created file.

===========================================================================*/
#include "tdscdma_variation.h"
#include "customer.h"
#include "tdsl1rrcif.h"
#include "tdssrchset.h"
#include "tdssrchinterf.h"
#include "tdsmathutil.h"
#include "tfw_const.h"
#ifdef FEATURE_TDSCDMA_TDS_TO_1X_QBTA
#include "tdssrchcr.h"
#endif


/***************************************************************************

    D A T A     S T R U C T U R E S 
    
 ***************************************************************************/

/* the max buf idx RF support is 10 */
#define TDSSRCHMEAS_MAX_IFREQ_RF_BUF_IDX  TFW_RX_TX_MAX_NUM_INTER_FREQ_BUFFERS

/*RSCP Threshold to use longer measurement period for PF */
#define TDSSRCHMEAS_LONG_PF_MIN_PERIOD_RSCP_TH 46 /* -70 dBm */
/*10ms*/
#define TDSSRCHMEAS_PF_MIN_PERIOD 1
/*20ms*/
#define TDSSRCHMEAS_PF_MIN_PERIOD_L 4
/*40ms*/
#define TDSSRCHMEAS_PF_MAX_PERIOD 8
/*40ms*/
#define TDSSRCHMEAS_CF_MIN_PERIOD 8
/*60ms*/
#define TDSSRCHMEAS_CF_MAX_PERIOD 12
/*80ms*/
#define TDSSRCHMEAS_REMF_MIN_PERIOD 8
/*200ms*/
#define TDSSRCHMEAS_REMF_MAX_PERIOD 40

#define TDSSRCHMEAS_ISCP_MAX_PERIOD 40
#define TDSSRCHMEAS_ISCP_MIN_PERIOD 40

/*used for GSM RSSI*/
#define TDSSRCHMEAS_MIN_SAMPLE_RSSI   8
/*all gsm cells are measured one time---160ms,3 samples need 480ms
#define TDSSRCHMEAS_GSM_RSSI_MAX_PERIOD(cell_num) \
	     MIN(96,FLOOR((96*TDSSRCHMEAS_MIN_SAMPLE_RSSI),(3*cell_num)))*/

#define TDSSRCHMEAS_GSM_RSSI_MAX_PERIOD(cell_num) 32 //160 ms

/**/
#define TDSSRCHMEAS_GSM_RSSI_MIN_PERIOD 4 // 20 ms, in case small number of GSM nbrs are configured, we don't wait too long to do RSSI measurement

#define TDSSRCHMEAS_INVALID_U8  0xFF
#define TDSSRCHMEAS_INVALID_U16  0xFFFF

#define TDSSRCHMEAS_RSCP_OFFSET 116
#define TDSSRCHMEAS_ISCP_OFFSET 116


#define TDSSRCHMEAS_RSSI_OFFSET 100

/*for int16,0x8000 is -32768,it is corresponding to -128 (-32768/256)*/
#define TDSSRCHMEAS_RSCP_X256_INVALID -32768

#define TDSSRCHMEAS_RSCP_Q16_INVALID -8388608	// (-8388608/256/256) = -128
#define TDSSRCHMEAS_TXPWR_X16_INVALID 0x7FFF

#define TDSSRCHMEAS_RSSI_X256_INVALID -32768

#define TDSSRCHMEAS_SNR_X256_INVALID -32768

/*according to chunxia,it is -15*1024*/
#define TDSSRCHMEAS_SINR_X1024_MIN -15360
#define TDSSRCHMEAS_SINR_X1024_DEFAULT 0x1400

/*rscp invalid for reporting,it is corresponding to -128+116*/
#define TDSSRCHMEAS_RSCP_INVALID  -12
/* Min RSCP */
#define TDSSRCHMEAS_RSCP_X256_MIN -32767

#define TDSSRCHMEAS_METRICM_SW_MAX 65534

#define TDSSRCHMEAS_METRICM_FW_MAX 65535


#define TDSSRCHMEAS_RSCP_REPORT_TH (-108+116)

#define TDSSRCHMEAS_IRAT_TIMING_DRIFT_TH  64
#ifdef FEATURE_TDSCDMA_NFTL
#define TDSSRCHMEAS_FILT_SLOT_SNR_INVALID 0
#define TDSSRCHMEAS_FILT_FREQ_ERROR_INVALID 0
#define TDSSRCHMEAS_FILT_SNR_FOR_FTL 128  /*3db ->Q6*/
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) || defined(FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT)
/*The base time is 500ms. The timer is counted from the heartbeat evt*/
#define TDSSRCHMEAS_TWAIT_BASE_TIME_COUNTER     500
#endif
extern boolean tdssrchmeas_wait_isr_occur;
extern uint8 tdssrchmeas_scch_config_ts;
extern uint8 tdssrchmeas_scch_decode_ts;



/*timeslot's state*/
typedef enum
{
  TDSSRCH_RSCP_MEAS_DONE = 0,
  TDSSRCH_PROC_TS0_ISCP_RSP,
  TDSSRCH_PROC_NON_TS0_ISCP_RSP,
  TDSSRCH_PROC_ISCP_RSP,
  TDSSRCH_DO_RSCP_FILTERING,
  TDSSRCH_DO_ISCP_FILTERING,
  TDSSRCHDCH_INIT_MULTI_FILT_ENG_FILT_LEN,
  TDSSRCHDCH_FILT_IDX_FOR_INTERF_IRAT_MEAS,
  TDSSRCHDCH_RESET_FILT_PARAMS_ALL_TIMING_KNOWN_CELLS_TEMP,
  TDSSRCHMEAS_PRINT_INTRAF_MEAS_DB_INFO,
  TDSSRCHMEAS_RESTORE_MEASUREMENTS_UPON_SUCCESSFUL_HO,
  TDSSRCHMEAS_ADD_NEW_FILTER,
  TDSSRCHMEAS_DELETE_FROM_FILTER_DB,
  TDSSRCHMEAS_CHECK_IF_FILTER_EXISTS,
  TDSSRCHMEAS_DELETE_FILT_IF_NOT_USED,
  TDSSRCHMEAS_VALIDATE_FILTER_COEFF,
  TDSSRCHMEAS_GET_FILT_IDX = 16,
  TDSSRCHMEAS_FILT_FUNCTION_UNKNOWN
}tdssrchmeas_filt_mutex_function_name_enum_type;


typedef struct
{
uint32 lock_holder;
uint32 unlock_holder;
uint8 lock_counter;
uint8 unlock_counter;
}tdssrchmeas_filt_mutex_info_struct_type;



typedef struct
{
  /*the index of tdssrchset_freq_set*/
  uint8 freq_index;
 /*it records the time when this freq's rscp is scheduled*/
 /*when initilize,it will be set to invalid value*/
  uint16 time_stamp;
 /*it records the time difference from last rscp scheduling to now*/
  uint16 time_diff;
 /*The index point to rset,it points out which cell will be scheduled next time*/
  uint8 scan_rset_index;
}tdssrchmeas_rscp_freq_sche_struct_type;

typedef struct
{
  /*in FACH and DCH state,rscp_max_period indicates 
  the max RSCP sampling period in unit of subframes*/
  uint8 rscp_max_period;  
 /*in FACH and DCH state,rscp_min_period indicates 
  the min RSCP sampling period in unit of subframes*/
  uint8 rscp_min_period;
  /*the number of frequencies in this set,it decides 
  the number of freq_index and scan_cell_index*/
  uint8 freq_num;  
  /*The index to freq_index,it points out which frequency 
  will be scheduled next time*/
  uint8 next_sche_index;  
  /*The index to srch_freq_nset[TDSSRCHSET_FREQ_MAX]*/
  tdssrchmeas_rscp_freq_sche_struct_type freq_sche_info[TDSSRCHSET_FREQ_MAX+TDSSRCHSET_FREQ_MAX_ADD];
}tdssrchmeas_rscp_sche_strcut_type;

extern tdssrchmeas_rscp_freq_sche_struct_type tdssrchmeas_pf_rscp_meas_parms;
extern tdssrchmeas_rscp_sche_strcut_type tdssrchmeas_cf_rscp_meas_parms;
extern tdssrchmeas_rscp_sche_strcut_type tdssrchmeas_remf_rscp_meas_parms;


typedef struct
{ 
  /*ts number,0--TS0,1--TS1,2--TS2 and so on*/
  uint8 ts_id;
  /*cell number which need iscp*/
  uint8 cell_num;
  /*cell ptr which will measure iscp based on this ts */
  #ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT
  tdssrchset_cell_struct_type_tmp *cell_ptr[TDSSRCHSET_ISCP_MAX_CELL_1SLOT];
  tdssrchset_cell_struct_type_tmp *ramained_cell_ptr[TDSSRCHSET_ISCP_REMAINED_CELL_1SLOT];
  #else
  tdssrchset_cell_struct_type_tmp *cell_ptr[TDSSRCHSET_ISCP_PAIR_MAX];
  #endif
  /*it records the time when this timeslot's iscp is scheduled*/
  /*when initilize,it will be set to invalid value*/
  uint16 time_stamp;
  /*it records the time difference from last iscp scheduling to current*/
  uint16 time_diff;
}tdssrchmeas_iscp_ts_struct_type;

typedef struct
{
  /*in FACH and DCH state,rscp_max_period indicates 
  the max RSCP sampling period in unit of subframes*/
  uint8 iscp_max_period;
  /*in FACH and DCH state,rscp_min_period indicates
  the min RSCP sampling period in unit of subframes*/
  uint8 iscp_min_period;
  /*the number of frequencies in this set,it decides 
  the number of freq_index and scan_cell_index*/
  uint8 iscp_ts_num;
  /*The index to srch_freq_nset[TDSSRCHSET_FREQ_MAX]*/
  tdssrchmeas_iscp_ts_struct_type  iscp_ts_config[TDSSRCHSET_ISCP_PAIR_MAX];
  /*it indicates the timeslot bitmap which will be 
  used for next time iscp scheduling*/  
  uint8 next_ts_bitmap;
}tdssrchmeas_iscp_sche_strcut_type;

extern tdssrchmeas_iscp_sche_strcut_type tdssrchmeas_sche_iscp_meas_parms;

/*GSM RSSI measurement parameters */
typedef struct
{
  boolean rssi_valid;
  /* Next GSM cell index in tdssrchset_gsm_nset_ptr[] */       
  uint8  next_cell_idx;
  
  uint16 time_diff;
  
  uint16 time_stamp;  

} tdssrchmeas_gsm_rssi_parms_struct_type;

extern tdssrchmeas_gsm_rssi_parms_struct_type tdssrchmeas_gsm_rssi_parms;


/*it will replace tdssrch_ops_ctl_parm_struct_type*/
typedef struct
{ /*pf rscp*/
  boolean pf_srch_flag;
  /*cf rscp*/
  boolean cf_srch_flag;
  /*remf rscp*/
  boolean remf_srch_flag;
  /*intra iscp*/
  boolean iscp_srch_flag;
}tdssrchmeas_ops_parm_struct_type;

extern tdssrchmeas_ops_parm_struct_type  tdssrchmeas_ops_parms_tmp;

/*timeslot's state*/
typedef enum
{
  TDSSRCH_IDLE,
  TDSSRCH_WF_RX,
  TDSSRCH_PF_RX,
  TDSSRCH_TX, 
  TDSSRCH_BUSY,
  TDSSRCH_UNKNOWN
}tdssrchmeas_ts_usage_state_enum_type;

typedef enum
{
  TDSSRCH_TDS_MEAS,
  TDSSRCH_GSM_MEAS,
  TDSSRCH_ALL_MEAS,
  #if defined(FEATURE_TDSCDMA_TO_LTE)
  TDSSRCH_LTE_MEAS,
  #endif
  TDSSRCH_MEAS_MAX
  
}tdssrchmeas_result_enum_type;


#define TDSSRCH_SFN_UnitWithUs    5000
#define TDSSRCH_SFN_UnitWithEigthChip 51200
#define TDSSRCH_TS_UnitWithEigthChip 6912
/*96 chip DwPTS + 96 chip gap +160 chip UPPTS*/
#define TDSSRCH_SPEC_TS_UnitWithEigthChip 2816
#define TDSSRCH_UPPTS_UnitWithEigthChip 1280
#define TDSSRCH_MAX_TS_NUM 7
#define TDSSRCH_MAX_GAP_NUM_ONE_SUBFRAME 3
#define TDSSRCH_MAX_GAP_NUM_ONE_FRAME 4
#define TDSSRCH_SFN_NUM_FOR_BSIC     13
#define TDSSRCH_SUBFN_UnitWithUs    5000
#define TDSSRCH_SUBFN_UnitWithEigthChip 51200



/*
typedef enum
{   
  TS0,      
  TS1,
  TS2,
  TS3,
  TS4,
  TS5,
  TS6,
  INVALID_TS
}tdssrch_timeslot_type;
*/

extern uint8 tdssrchmeas_interval_length;
extern tdssrchmeas_ts_usage_state_enum_type tdssrchmeas_subframe1[TDSSRCH_MAX_TS_NUM+1];
extern tdssrchmeas_ts_usage_state_enum_type tdssrchmeas_subframe2[TDSSRCH_MAX_TS_NUM+1];

extern uint16 tdssrch_ts_begin_position[TDSSRCH_MAX_TS_NUM];

extern boolean tdssrchmeas_tds_meas_not_done;

extern boolean tdssrchmeas_gsm_meas_not_done;

extern boolean tdssrchmeas_gap_change;

extern boolean tdssrchmeas_start_interf_scheduling;
extern boolean tdssrchmeas_rf_interf_scripts_pending;
extern boolean tdssrchmeas_rf_interf_scripts_updating;
extern boolean tdssrchmeas_rf_interf_scripts_pending_by_demo;
extern volatile boolean tdssrchmeas_start_gsm_scheduling;
extern uint8 tdssrchmeas_start_gsm_countdown;
extern boolean tdssrchmeas_rf_gsm_scripts_pending;

extern boolean tdssrchmeas_rf_interf_scripts_pending_at_wakeup;

typedef struct
{
  boolean need_ta_flag;
  uint16 sfn;
  uint16 gap_start;
  uint32 gap_duration;
}tdssrchmeas_gap_parms_struct_type;


typedef struct
{
  uint16 gsm_start_sfn;
  uint16 gsm_start_offset;
  uint16 gsm_end_sfn;
  uint16 gsm_end_offset;
}tdssrchmeas_gap_mapped_parms_struct_type;


typedef struct
{
  uint16 event_start_sfn;
  uint16 event_start_offset;
}tdssrchmeas_eve_trig_parms_struct_type;


typedef struct
{
  uint8 gap_num_current;
  uint8 gap_num_total;
  tdssrchmeas_gap_parms_struct_type gap_parms[TDSSRCH_MAX_GAP_NUM_ONE_FRAME];
  tdssrchgsmdrv_gap_param_gsm_struct_type gap_mapped_parms[TDSSRCH_MAX_GAP_NUM_ONE_FRAME];
}tdssrchmeas_meas_gap_parms_struct_type;

extern tdssrchmeas_meas_gap_parms_struct_type  tdssrchmeas_meas_gap_info_parms;

typedef enum
{
  /* no scanning activity going on*/ 
  TDSSRCH_GSM_RSSI_SCAN,
  /* Ready to scan next frequency on command*/
  TDSSRCH_GSM_BSIC_IDENTIFY,
  /* send FS_CMD to mdsp and waiting for the result from mdsp*/
  TDSSRCH_GSM_BSIC_RECONF
}tdssrchmeas_gsm_meas_purpose_enum_type;


typedef struct
{
  /*measure popuse--RSSI/BSIC ID/BSIC RC*/
  tdssrchgsm_meas_purpose_enum_type meas_purpose;
  /*gap parms---in TD timeline*/
  tdssrchmeas_gap_parms_struct_type gap_parms;
  /*gap parms---in GSM timeline*/
  //tdssrchmeas_gap_mapped_parms_struct_type gap_mapped_parms;
  /*event tigger time*/
  tdssrchmeas_eve_trig_parms_struct_type event_trig_parms;
}tdssrchmeas_gsm_gap_info_struct_type;

typedef struct
{
  /*the gap number which is assigned to gsm*/
  uint8 assigned_gap_num;
  /*used for gsm event set*/
  uint8 event_set_gap_idx;
  /*gap parms--suppose max 2 gaps are supported in one subframe*/
  tdssrchmeas_gsm_gap_info_struct_type gap_info[TDSSRCH_MAX_GAP_NUM_ONE_SUBFRAME];
}tdssrchmeas_gsm_ctl_parm_struct_type;

extern tdssrchmeas_gsm_ctl_parm_struct_type tdssrchmeas_gsm_ctl_parms_before;

extern tdssrchmeas_gsm_ctl_parm_struct_type tdssrchmeas_gsm_ctl_parms_current;

#ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT
/*CR 383425 ISCP Optimixation*/
extern int16 tdssrchmeas_remained_cell_iscp;
extern int16 tdssrchmeas_remained_cell_filted_iscp;
/*CR 383425 ISCP Optimixation*/
#endif

typedef enum
{
  TDS_NONE_PERIOD,
  /*rscp/iscp min period*/
  TDS_MIN_PERIOD,
  /*rscp/iscp max period*/
  TDS_MAX_PERIOD
} tdssrchmeas_sche_period_enum_type;
 
 /* This will check the cause of the failure of MCM in l1 */
 typedef enum 
 {  
   TDSSRCHMEAS_MCM_FAILURE_CAUSE_NO_FAILURE,
   TDSSRCHMEAS_MCM_FAILURE_CAUSE_NUM_MEAS_EXCEED
 } tdssrchmeas_mcm_failure_cause_enum_type;
 
 extern tdssrchmeas_mcm_failure_cause_enum_type tdssrchmeas_mcm_failure_cause;

  //Xiaoning changed
#define TDSSRCHMEAS_PDSCH_TS_2ND_SUBFN 0x0FFC
#define TDSSRCHMEAS_EPUCH_TS_2ND_SUBFN 0x3FFC
#define TDSSRCHMEAS_CONTINUOUS_CONFIG_REP_PERIOD_IN_FRAME 1
#define TDSSRCHMEAS_DEFAULT_REP_LENGTH_FOR_CONTINUOUS_CONFIG_IN_SUBFRAME 2
#define TDSSRCHMEAS_PCCPCH_ON_BITMAP 0x8000
#define TDSSRCHMEAS_PCCPCH_OFF_BITMAP 0x0000

typedef enum
{
  TDSSRCHMEAS_IDLE_SLOT,
  TDSSRCHMEAS_TX_SLOT,
  TDSSRCHMEAS_RX_SLOT,
  TDSSRCHMEAS_UNKNOWN_SLOT
}tdssrchmeas_ts_usage_enum_type;

/*--------------------------------------------------------------------------------
          Structure of the ts usage for the current and next subframe 
---------------------------------------------------------------------------------*/
typedef struct
{
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn; 
}tdssrchmeas_ts_usage_struct_type;
 
/*------------------------------------------------------------------------------
          Structure of measurement ts info for dpch ul,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_dpchul_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for dpch dl,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint8 repetition_period;
  uint64 chanPresentMask;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
  uint16 idleslot_config_time_1st_subfn;
  uint16 idleslot_config_time_2nd_subfn;
  uint8 repetition_period_1st_subfn;
  uint8 repetition_period_2nd_subfn;
  uint64 chanPresentMask_1st_subfn;
  uint64 chanPresentMask_2nd_subfn;
}tdssrchmeas_dpchdl_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for sccpch,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint8 repetition_period;
  uint64 chanPresentMask;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
  uint16 idleslot_config_time_1st_subfn;
  uint16 idleslot_config_time_2nd_subfn;
  uint8 repetition_period_1st_subfn;
  uint8 repetition_period_2nd_subfn;
  uint64 chanPresentMask_1st_subfn;
  uint64 chanPresentMask_2nd_subfn;
}tdssrchmeas_sccpch_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for pich,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_pich_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for hs-scch,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_hsscch_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for eagch,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_eagch_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts config info for uppch&fpach,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  /*solve fpach conflict with inter-f meas*/
  boolean valid;
  uint16 upsync_start_time;
  uint16 uppch_ts_bitmap;
  uint16 fpach_ts_bitmap; 
  uint16 fpach_wt;
  uint8 seq_no;
}tdssrchmeas_uppch_fpach_ts_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for uppch&fpach,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  boolean index;
  tdssrchmeas_uppch_fpach_ts_struct_type config_info[2];
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_uppch_fpach_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for prach,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_prach_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for pdsch,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_pdsch_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for epuch,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
  /*this eagch decoding result is for the current measurement period or not*/
  boolean latest_eagch_for_current;
}tdssrchmeas_epuch_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for non scheduled epuch,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_nonsche_epuch_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for ehich,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
}tdssrchmeas_ehich_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for pccpch,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint16 ts_bitmap;
  uint8 repetition_period;
  uint64 chanPresentMask;
  uint16 ts_1st_subfn;
  uint16 ts_2nd_subfn;
  uint16 idleslot_config_time_1st_subfn;
  uint16 idleslot_config_time_2nd_subfn;
  uint8 repetition_period_1st_subfn;
  uint8 repetition_period_2nd_subfn;
  uint64 chanPresentMask_1st_subfn;
  uint64 chanPresentMask_2nd_subfn;
}tdssrchmeas_pccpch_struct_type;

/*------------------------------------------------------------------------------
          Structure of measurement ts info for fmo,
          channel config info are all subframe based
-------------------------------------------------------------------------------*/ 
typedef struct
{
  uint16 idleslot_config_time;
  uint8 n_tti;
  uint32 t_meas_rep;
  boolean fmo_on_1st_subfn;
  boolean fmo_on_2nd_subfn;
  boolean fmo_start;
}tdssrchmeas_fmo_struct_type;

/*------------------------------------------------------------------------------
          Structure for measurement ts usage database
-------------------------------------------------------------------------------*/

typedef struct
{
  /*DL on next TS0?*/  
  boolean dl_on_ts0_2nd_subframe;

  /*any DL on next non-TS0 slot? */
  boolean dl_on_non_ts0_2nd_subframe;

  /*the used slot from the scheduling point of last subframe*/
  uint16 scheduled_ts_usage;   

  /*the current ts usage for the coming measurement period*/
  tdssrchmeas_ts_usage_struct_type ts_usage_in_2_subfn;

  /*new config is available for dpch ul?*/
  boolean dpchul_new_config_avail; 

  #if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
  boolean dpchul_setuped;
  #endif

  tdssrchmeas_dpchul_struct_type dpchul_ts;

  /*new config is available for dpch dl?*/
  boolean dpchdl_new_config_avail;

  #if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
  boolean dpchdl_setuped;
  #endif

  tdssrchmeas_dpchdl_struct_type dpchdl_ts;

  /*new config is available for sccpch?*/
  boolean sccpch_new_config_avail;

  tdssrchmeas_sccpch_struct_type sccpch_ts;

  /*new config is available for pich?*/
  boolean pich_new_config_avail;

  tdssrchmeas_pich_struct_type pich_ts;

  /*new config is available for hs-scch?*/
  boolean hsscch_new_config_avail;

  tdssrchmeas_hsscch_struct_type hsscch_ts;

  /*new config is available for eagch?*/
  boolean eagch_new_config_avail;

  tdssrchmeas_eagch_struct_type eagch_ts;

  /*uppch fpach start?*/
  boolean start_uppch_fpach;

  #if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
  boolean uppch_setuped[2];
  #endif

  tdssrchmeas_uppch_fpach_struct_type uppch_fpach_ts;

  /*new config is available for dpch ul?*/
  boolean prach_new_config_avail;

  #if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
  boolean prach_setuped;
  #endif 

  tdssrchmeas_prach_struct_type prach_ts;

  /*new hs-scch decoding interrupt for pdsch?*/
  boolean pdsch_new_config_avail;

  tdssrchmeas_pdsch_struct_type pdsch_ts;

  /*new eagch decoding interrupt for epuch?*/
  boolean epuch_new_config_avail;
  tdssrchmeas_epuch_struct_type epuch_ts;

  /*new config for nonscheduled epuch?*/
  boolean epuch_nonsche_new_config_avail;
  tdssrchmeas_nonsche_epuch_struct_type epuch_nonsche_ts;

  /*new config for for ehich?*/
  boolean ehich_new_config_avail;
  tdssrchmeas_ehich_struct_type ehich_ts;

  /*new config for nonscheduled ehich?*/
  boolean ehich_nonsche_new_config_avail;
  tdssrchmeas_ehich_struct_type ehich_nonsche_ts;

  /*new config is available for pccpch?*/
  boolean pccpch_new_config_avail;
  tdssrchmeas_pccpch_struct_type pccpch_ts;

  /*new config is available for fmo?*/
  boolean fmo_new_config_avail;
  tdssrchmeas_fmo_struct_type fmo_ts; 
}tdssrchmeas_ts_struct_type;

extern tdssrchmeas_ts_struct_type  tdssrchmeas_ts_info;

#ifdef FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT
/*------------------------------------------------------------------------------
          Structure for 60ms gap for GSM BSIC optimization
-------------------------------------------------------------------------------*/ 
typedef struct
{
  boolean gap_needed;
  boolean gap_start;
  boolean gap_tried;
  boolean meas_sched;
  boolean stop_tds_meas;
  boolean sch_detected;
  boolean gap_booked;
  boolean first_enter;
  uint16 start_subfn;
  uint16 forbidden_start_subfn;
  uint16 msg_id;
  tdssrchset_gsm_cell_struct_type *target_cell_ptr;
}tdssrchmeas_60ms_gap_struct_type;

extern tdssrchmeas_60ms_gap_struct_type tdssrchmeas_60ms_gap;
#endif

/* 'Ping-Pong' preventing timer
 * To prevent 'ping-pang' HO, a timer will be start after a HO complete. Before the timer expire, 
 * no HO event should be triggered. The duration of the time is the same as time to trigger. 
 * This guarantee the interval between 2 HO are longer than Time to trigger.
 */
extern uint16 tdssrchmeas_1g_fast_ho_timer;
extern uint16 tdssrchmeas_2a_fast_ho_timer;
extern uint16 tdssrchmeas_3a_fast_ho_timer;
extern boolean tdssrchmeas_1g_bypass_fast_ho_timer;
extern boolean tdssrchmeas_2a_bypass_fast_ho_timer;
extern boolean tdssrchmeas_ho_flag;


       
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) || defined(FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT)

typedef enum 
 {  
   TDSSRCHMEAS_CM_MEAS_HIGH_PRIORITY_L,
   TDSSRCHMEAS_CM_MEAS_HIGH_PRIORITY_G,
   TDSSRCHMEAS_CM_MEAS_HIGH_PRIORITY_T
 } tdssrchmeas_cm_measurement_priority_enum_type;

/* The CM measurement priority */
typedef struct
{
  /*T2G has the higher priority*/
  boolean sched_for_G;
  /*T2L will be scheduled at the next subframe boundary (5ms later) */
  boolean sched_for_L;
  /*the subframe index of the G gap start*/
  uint8 g_gap_subfm_start_idx;
    /*the subframe index of the G gap start*/
  uint8 g_gap_subfm_end_idx;
  /*gap time slot start index of the G gap start*/
  uint8 g_gap_ts_start_idx;
  /*gap time slot end index of the G gap start*/
  uint8 g_gap_ts_end_idx; 
  /*g gap len*/
  uint8 g_gap_len;  
  /* priority flag to show which meaurement has a higher priority */
  tdssrchmeas_cm_measurement_priority_enum_type priority_flag;
} tdssrchmeas_cm_meas_sched_priority_struct_type;

extern tdssrchmeas_cm_meas_sched_priority_struct_type tdssrchmeas_cm_meas_sched_priority;

/*call type bitmap from CPHY_SETUP*/
/* RRC will set the bit based on what type of call is active and 
** what type of call is being setup.
** bit7(MSB) bit6 bit5 .........bit0(LSB)
** bit0 ...................1 Voice call  
** bit1 ...................1 Release 99 PS data call
** bit2 ...................1 HS call 
** bit3 ...................1 MBMS channel
** bit4-7 ...................reserved 
*/
extern uint8 tdssrchmeas_cm_meas_call_type_mask;
#endif
extern boolean tdssrchmeas_enable_blind_ho_flag;
extern boolean tdssrchgsm_blind_ho_flag;

#ifdef FEATURE_TDSCDMA_REPEAT_TO_SECOND_BEST 
/*index maps to meas_id*/
extern uint8 tdssrchdch_1g_repeat_cell_num[TDSSRCHDCH_MAX_INTRAF_EVT];
extern uint8 tdssrchdch_2a_repeat_cell_num[TDSSRCHDCH_MAX_INTRAF_EVT];
#endif

//extern boolean tdssrchmeas_t2g_adjust_gap_in_DRX_mode;


       
/*=========================================================================

    V A R I A B L E     D E C L A R A T I O N S


 ===========================================================================*/
#define TDSSRCHMEAS_STATISTICS_MAX_SIZE  4
extern uint8 tdssrchmeas_statistics_idx;
extern uint8 tdssrchmeas_statistics_meas_cells_num[TDSSRCHMEAS_STATISTICS_MAX_SIZE];

#ifdef FEATURE_TDSCDMA_MULTIPLE_L3_FILTERS_SUPPORTED
/* This global will keep track of the situation when there are no NW configured
filter. In that case the default filter will be used. Later when NW configures a filter
this flag will be turned false */
extern boolean tdssrchmeas_no_filt_configured_frm_nw;
#endif

#ifdef FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT
extern uint16 tdssrchmeas_reconfig_act_time_query(uint16 act_time);
extern tdsl1_tti_enum_type tdsulutil_get_longest_tti(void);
extern tdsl1_tti_enum_type tdsdlcctrch_get_longest_tti_for_srch(void);

/*===========================================================================
FUNCTION  TDSSRCHMEAS_ERUCCH_ongoing

DESCRIPTION:
  This function to see if E-RUCCH is ongoing. 

DEPENDENCIES
  None.

RETURN VALUE
                TRUE:   E-RUCCH is going on.
                 FALSE:  No E-RUCCH is going on. 

SIDE EFFECTS
  None
===========================================================================*/
boolean tdssrchmeas_check_erucch_ongoing_for_60ms_gap(void);

/*===========================================================================
FUNCTION        tdssrchmeas_60ms_gap_reconfig_act_time

DESCRIPTION     L1M to adjust reconfigure activation time of the 13G frame gap with SRCH

DEPENDENCIES  only applicable for DCH state currently

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/

uint16 tdssrchmeas_60ms_gap_reconfig_act_time(uint16 act_time);


/*===========================================================================
FUNCTION        tdssrchmeas_adjust_ts_usage_for_13GSM

DESCRIPTION     This function will adjust the ts_usage for LTE measurement if it is enabled. 

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS:

===========================================================================*/
void tdssrchmeas_adjust_ts_usage_for_13GSM(void);
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) || defined(FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT)
/*===========================================================================
FUNCTION  tdssrchmeas_check_subfn_within_interval

DESCRIPTION:
  This function will check whether one given subfn is within the interval defined by start_subfn and end_subfn or not.  

DEPENDENCIES
  None.

RETURN VALUE
                TRUE:    given subfn is within the interval
                 FALSE:  No. 

SIDE EFFECTS
  None
===========================================================================*/
boolean tdssrchmeas_check_subfn_within_interval
(
  uint16 checked_subfn, 
  uint16 start_subfn, 
  uint16 end_subfn
);


/*===========================================================================
FUNCTION  tdssrchmeas_reserve_erucch_start_subfn

DESCRIPTION:
  This function will try to check conflicts with T2L scheduler and reserve ERUCCH start time when there is no conflict between 
    ERUCCH and T2L gap. 

  If there is no T2L gap on-going right now, or the scheduled T2L gap doesn't conflict with the proposed ERUCCH start time, 
           this function will return true and set the ERUCCH start subfn in Gap Manager. 
  Otherwise, it will return FALSE and the current T2L gap start and end subframe numbers.
  
  Please note that gap_sfm_start, gap_sfm_start+1, gap_sfm_end-1 will be 
    In the LTE measurement gap. 

DEPENDENCIES
  None.

RETURN VALUE
                TRUE:    One LTE measurement is going on. The valid gap information is returned.
                 FALSE:  No LTE measurement is going on. The gap information should be ignored. 

SIDE EFFECTS
  None
===========================================================================*/
boolean tdssrchmeas_reserve_erucch_start_subfn(
  /*planed uppch start subframe number*/
  uint16 uppch_start_subfn, 
  /* current gap start subframe */
  uint16 * gap_sfm_start, 
  /* current gap end subframe */
  uint16 * gap_sfm_end);

/*===========================================================================
FUNCTION  TDSSRCHMEAS_CM_GAP_INFO_FOR_UL

DESCRIPTION:
  This function gives UL the gap information for T2L CM measumrent 
  
  Please note that gap_sfm_start, gap_sfm_start+1, gap_sfm_end-1 will be 
    In the LTE measurement gap. 

DEPENDENCIES
  None.

RETURN VALUE
                TRUE:    One LTE measurement is going on. The valid gap information is returned.
                 FALSE:  No LTE measurement is going on. The gap information should be ignored. 

SIDE EFFECTS
  None
===========================================================================*/
boolean tdssrchmeas_cm_gap_info_for_UL(
/*subframe number of the gap start*/
uint16* gap_sfm_start, 
/*subframe number of the gap end which is included in the gap
  so, gap_sfm_end will has no LTE measurement*/
uint16* gap_sfm_end 
);

/*===========================================================================
FUNCTION        tdssrchmeas_reconfig_act_time_query

DESCRIPTION    API function for L1M to adjust reconfigure activation time with SRCH

DEPENDENCIES

RETURN VALUE    srch adjusted activation time in CFN

SIDE EFFECTS
===========================================================================*/
uint16 tdssrchmeas_reconfig_act_time_query(uint16 act_time);

#endif


extern void tdssrchmeas_heartbeat_event_handler(void);

extern void tdssrchmeas_internal_meas_heartbeat_event(void );

/*======================================================================
FUNCTION tdssrchmeas_determine_sttd_behavior

DESCRIPTION
    This function is used to determine STTD behavior     

RETURN VALUE
    void

=======================================================================*/
extern void tdssrchmeas_determine_sttd_behavior(void);

/*===========================================================================
FUNCTION        tdssrchmeas_suspend_measurements_for_ho

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
===========================================================================*/
extern void tdssrchmeas_suspend_measurements_for_ho(void);

/*===========================================================================
FUNCTION        tdssrchmeas_restore_measurements_upon_resumption_on_orig_cell

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
===========================================================================*/
extern void tdssrchmeas_restore_measurements_upon_resumption_on_orig_cell(void);

/*===========================================================================
FUNCTION        tdssrchmeas_restore_measurements_upon_successful_ho

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
  void.

SIDE EFFECTS    
===========================================================================*/
extern void tdssrchmeas_restore_measurements_upon_successful_ho(void);

/*===========================================================================

FUNCTION tdssrchmeas_cleanup_intraf_cells_triggered_history

DESCRIPTION:
  Reset the cells riggered in intra-frequency measurement database.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrchmeas_cleanup_intraf_cells_triggered_history(void);

//extern void tdssrchmeas_update_cells_with_cells_for_meas_info(uint16 meas_id);

extern boolean tdssrchmeas_get_cell_for_meas_info(tdssrchset_cell_struct_type_tmp *cell_ptr, uint16 meas_idx);

#ifdef FEATURE_TDSCDMA_MULTIPLE_L3_FILTERS_SUPPORTED
extern uint16 tdssrchmeas_get_filt_idx(void);

#endif

extern void tdssrchmeas_update_interf_rf_scripts( void );

extern void tdssrchmeas_update_rscp_sche_parms( void );

extern void  tdssrchmeas_update_cfs_and_remf_parms(void);

extern void  tdssrchmeas_update_cfs_and_remf_period(void);

extern void tdssrchmeas_update_iscp_sche_parms( void );

extern uint8 tdssrchmeas_ts_position_in_iscp_config(uint8 ts_id);

extern boolean tdssrchmeas_freq_exist_in_sche_set
(uint8 freq_index,
 tdssrchmeas_rscp_sche_strcut_type *meas_sche_set_ptr);
 
extern void  tdssrchmeas_move_freq_between_sche_set
(uint8 freq_index,
 tdssrchmeas_rscp_sche_strcut_type *orig_set,tdssrchmeas_rscp_sche_strcut_type *targ_set);
 
extern void tdssrchmeas_map_time_tds_to_gsm(void);

extern void tdssrchmeas_cal_gsm_trigger_time(void);

extern boolean tdssrchmeas_cur_ts_is_used_for_gsm(void);

extern void tdssrchmeas_update_ts_state
(tdssrchmeas_gap_parms_struct_type *used_gap_info_ptr);

extern void tdssrchmeas_set_ts_state
(uint16 sfn,uint8 ts_start,uint8 ts_num,tdssrchmeas_ts_usage_state_enum_type state);

extern boolean tdssrchgsm_gap_enough_for_gsm_meas
(uint32 gap_duration, 
 tdssrchgsm_meas_purpose_enum_type meas_type);

boolean tdssrchgsm_gap_enough_for_gsm_meas
(uint32 gap_duration, 
 tdssrchgsm_meas_purpose_enum_type meas_type);

extern void  tdssrchmeas_init_meas_parms(void);

extern void  tdssrchmeas_ignore_rscp_result(uint16 freq);
extern void  tdssrchmeas_ignore_all_rscp_result(void);

extern uint8 tdssrchmeas_get_snpl_index(void);

void tdssrchmeas_sche_gsm_meas_in_idle_timeslot
(tdssrchgsm_meas_purpose_enum_type meas_purpose);

boolean tdssrchmeas_sche_gsm_meas_in_idle_subframe
(tdssrchgsm_meas_purpose_enum_type meas_purpose);

extern boolean tdssrchmeas_measure_results_received
(tdssrchmeas_result_enum_type meas_type);

extern void tdssrchmeas_set_event_for_gsm_meas(void);

extern void tdssrchmeas_send_periodic_meas_report(uint16 meas_id);

extern void tdssrchmeas_add_cell_to_measured_results_temp(
        tdsl1_intra_meas_rpt_cell_info_struct_type *addl_cell_ptr,
        tdssrchset_cell_struct_type_tmp *cell_ptr, uint16 meas_idx);

extern boolean tdssrchmeas_dl_on_ts0(void);

extern boolean tdssrchmeas_dl_on_non_ts0(void);

extern boolean tdssrchgsm_sche_appoint_gsm_cell_bsic
(uint8 cell_index,
 tdssrchgsmdrv_gap_param_gsm_struct_type *gsm_gap_params_ptr,
 tdssrchmeas_gap_parms_struct_type *gap_params_ptr,
 tdssrchgsm_meas_purpose_enum_type meas_purpose);

/*===========================================================================
FUNCTION     tdssrchdch_proc_srch_ops_tmp

DESCRIPTION:
    handle srch_ops
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern void tdssrchdch_proc_srch_ops_tmp(tdssrchmeas_ops_parm_struct_type *ops_ptr);


//extern void tdssrch_add_freq_to_meas_sche(uint8 freq_index);

//extern void tdssrch_delete_freq_from_meas_sche(uint8 freq_index);


extern void tdssrchmeas_add_cell_to_measured_results(
            tdsl1_intra_meas_rpt_cell_info_struct_type *addl_cell_ptr,
            tdssrchset_cell_struct_type_tmp *cell_ptr, uint16 meas_idx);

extern void tdssrchmeas_calc_snpl_index(tdsl1_eul_snpl_measure_type_enum_type SnplRptType);

/*===========================================================================
FUNCTION        TDSSRCHMEAS_SCCH_DONE_ISR_HANDLING

DESCRIPTION     This function handles the search heartbeat event. 
                The function will be registered with L1 Sequencer when
                Search Module starts its heartbeat.

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS:
when HSDPA,if SCCH is in TS6,then this function will be called in scch decode
 done ISR(when HSDPA configured,if TS6,then disable heartbeat event,and set a flg,
 then in scch SIR,judge this flag,if it is true,then call this function).
 if if SCCH is in TS6,this function will be an event every 5ms.
===========================================================================*/
extern void tdssrchmeas_scch_done_isr_handling(void);

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
/*===========================================================================
FUNCTION        tdssrchmeas_is_lte_meas_on_going

DESCRIPTION     This function returns the LTE measurement status. 

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS:

===========================================================================*/
boolean tdssrchmeas_is_lte_meas_on_going(void);
#endif 

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*===========================================================================

FUNCTION     TDSSRCHMEAS_DEFERRED_SIB_MEAS_REQ_UPDATE

DESCRIPTION
    This function handle the deferred SIB meas request from RRC and update the parameters.

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  boolean - TRUE - if it successful, FALSE otherwise

SIDE EFFECTS
  
===========================================================================*/
boolean tdssrchmeas_deferred_sib_meas_req_update(tdsl1_meas_cmd_type *cmd_ptr);
#endif

/*=========================================================================

 FUNCTION     :tdssrchmeas_walltimeofs_drift_too_much
 
 DESCRIPTION  : This function is used to check whether walltime offset drift.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
   uint32 walltimeofs
 
 RETURN VALUE : None. 
       
 SIDE EFFECTS : 
==========================================================================*/
extern boolean tdssrchmeas_walltimeofs_drift_too_much(void);

//time profiling
#ifdef TDSL1_IRAT_TIME_PROFILING
typedef struct
{
  uint32 num_of_update;
  uint16 sfn;
  uint16 heartbeat_gsm_start_offset;  
  uint16 heartbeat_gsm_finish_offset;
  uint16 heartbeat_gsm_duration_max;
  uint16 heartbeat_gsm_duration_max_at_sfn;
  uint16 heartbeat_gsm_duration_max_at_update;
  uint16 heartbeat_gsm_duration_min;
  uint16 heartbeat_gsm_duration_min_at_sfn;
  uint16 heartbeat_gsm_duration_min_at_update;
  //max total time of heartbeat scheduling
  uint16 duration_max_total_scheduling_time[9];
  //max time of power meas cmds API including startup/cleanup
  uint16 duration_max_rssi_scheduling_cmds[9];
  //num of times we schedule power measurement per #rssi
  uint32 num_of_update_power_meas[9];
  //avg time of power meas cmd API
  double duration_avg_power_meas_cmd[9];
  //avg time of power meas cmds API including startup/cleanup
  double duration_avg_rssi_scheduling_cmds[9];
  //avg total time of heartbeat scheduling
  double duration_avg_total_scheduling_time[9];
  //number of rssi scheduled this time
  uint16 num_rssi;
       
  uint16 gsm_startup_cmd_duration_max;
  uint16 gsm_cleanup_cmd_duration_max; 
  uint16 gsm_acq_start_cmd_duration_max;
  uint16 gsm_acq_stop_cmd_duration_max;
  uint16 gsm_sch_decode_cmd_duration_max;
  uint16 gsm_margin;

} tdssrchmeas_gsm_time_profiling_parms_struct_type;

extern tdssrchmeas_gsm_time_profiling_parms_struct_type tdssrchmeas_gsm_time_profiling_parms;
#endif 
extern boolean tdssrchmeas_gap_sch_contain_three_or_more_timeslot( void);


void tdssrchmeas_init_non_ts0_cmd_buffer(uint8 msg_id);

#ifdef FEATURE_TDSCDMA_REPEAT_TO_SECOND_BEST
extern void tdssrchdch_1g2a_reset_all_cells(boolean reset_intra, boolean reset_inter);
#endif

#ifdef FEATURE_TDSCDMA_WLAN_COEX
extern tdsl1_gtl_snapshot_type tdssrchmeas_snapshot;
#endif

/*===========================================================================
FUNCTION        tdssrchmeas_reset_ts_sccpch_info

DESCRIPTION     

DEPENDENCIES    this function is used to reset meas ts sccpch info.

RETURN VALUE    None
===========================================================================*/
extern void tdssrchmeas_reset_ts_sccpch_info(void);

#ifdef FEATURE_TDSCDMA_MOTION_SENSOR_AID_MEAS_SUPPRESS

typedef enum
{
    TDS_MOTION_SENSOR_MODE_MODEM_DISABLE = 0, /*Modem state no need the sensor info*/
	TDS_MOTION_SENSOR_MODE_DISABLE, /*mode side can levage sensor mode*/
    TDS_MOTION_SENSOR_MODE_ENABLE,
    TDS_MOTION_SENSOR_MODE_NUM_TYPES

} tdssrchmeas_motion_mode_type;

typedef enum
{
#if 0 /*not consider dsds mode, as this bridge only get the sensor state*/
		TDS_MOTION_SENSOR_MODE_EVTS_MODEM_NONE_DS_MODE,  /*modem in none dsds mode*/
		TDS_MOTION_SENSOR_MODE_EVTS_MODEM_IN_DS_MODE,
#endif
    /*Modem side condition check*/
	TDS_MOTION_SENSOR_MODE_EVTS_MODEM_QUALITY_MEET = 0, /*modem channel quality meet*/
	TDS_MOTION_SENSOR_MODE_EVTS_MODEM_QUALITY_DISABLE,
    TDS_MOTION_SENSOR_MODE_EVTS_MODEM_STATE,         /*within cell reselect state DRX/FACH*/
    TDS_MOTION_SENSOR_MODE_EVTS_MODEM_STATE_DISABLE,
    /*Sensor side start stop events */
    TDS_MOTION_SENSOR_MODE_EVTS_START_SENSOR, /*4 modem side start levage sensor mode*/
    TDS_MOTION_SENSOR_MODE_EVTS_STOP_SENSOR,  /*5 modem stop take sensor state */
    TDS_MOTION_SENSOR_MODE_EVTS_NV_ENABLE,
	TDS_MOTION_SENSOR_MODE_EVTS_NUM_TYPES

} tdssrchmeas_motion_mode_evts_type;

/*===========================================================================

FUNCTION     

DESCRIPTION
    .

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  

SIDE EFFECTS
  
===========================================================================*/

void tdssrchmeas_motion_sensor_mode_updt(tdssrchmeas_motion_mode_evts_type trig_evt);

/*===========================================================================

FUNCTION     

DESCRIPTION
    .

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  

SIDE EFFECTS
  
===========================================================================*/

void tdssrchmeas_motion_sensor_mode_init(void);


/*===========================================================================

FUNCTION     

DESCRIPTION
    .

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  

SIDE EFFECTS
  
===========================================================================*/


void tdssrchmeas_motion_sensor_local_cmd_process(boolean action);

#endif /*FEATURE_TDSCDMA_MOTION_SENSOR_AID_MEAS_SUPPRESS */


#ifdef TDSL1_IRAT_TIME_PROFILING
 //time profiling
void tdssrchmeas_cal_gsm_scheduling_time_profiling(void);
#endif

#ifdef FEATURE_TDSCDMA_TDS_TO_1X_QBTA
/*===========================================================================

FUNCTION     tdssrchmeas_update_meas_rules_in_qta

DESCRIPTION
    called by tdsl1dsds to disable IRAT measurement in QBTA.

DEPENDENCIES
  None.

PARAMETERS

 
RETURN VALUE
  

SIDE EFFECTS
  
===========================================================================*/
void tdssrchmeas_update_meas_rules_in_qta (tdssrchcr_meas_rules_type meas_rules);
#endif
/*=========================================================================
 
 FUNCTION     : tdssrchmeas_pending_cleanup_process
 
 DESCRIPTION  : send a local cmd to L1M to process pending cleanup in HB
              
 DEPENDENCIES : 
 
 RETURN VALUE : 

 SIDE EFFECTS : 
 
 =========================================================================*/
extern void tdssrchmeas_send_pending_cleanup_proc_cmd(void);
/*=========================================================================
 
 FUNCTION     : tdssrchmeas_pending_cleanup_process
 
 DESCRIPTION  : process pending cleanup in heartbeat
              
 DEPENDENCIES : 
 
 RETURN VALUE : 

 SIDE EFFECTS : 
 
 =========================================================================*/
extern void tdssrchmeas_pending_cleanup_process(void);
#endif /* SRCHMEAS_H */

