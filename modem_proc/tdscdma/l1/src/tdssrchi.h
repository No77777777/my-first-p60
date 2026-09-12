#ifndef TDSSRCHI_H
#define TDSSRCHI_H

/*===========================================================================
   FILE: srchi.h

GENERAL DESCRIPTION:
      This file contains definitions used internally by the Search module.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright(c) 2000 to 2010 by Qualcomm Technologies Incorporated. All Rights Reserved.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchi.h_v   1.43   11 Jul 2002 21:25:56   halbhavi  $
$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $


when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/



/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsl1m.h"
#include "tdssrch.h"
#include "tdssrchparm.h"
#include "tdssrchcmd.h"
#include "tdssrchset.h"
#include "tdsl1rrcif.h"
#include "tdssrchinterf.h"
#include "tfw_sw_intf_common.h"
#include "tdsl1m_rcinit.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define TDSSRCH_NO_PCCPCH_TX_PWR  0xFF

#define TDSSRCH_NO_REF_POS  0xFFFFFFFF
   /* No Reference postion for window centering */

#define TDSSRCH_NO_INTRAF_MEAS_ID (0xff)
#define TDSSRCH_NO_MEAS_INDEX     (0xff)
/*   Max num of reported cells in cell measured results */
#define TDSSRCHDCH_CELL_MEAS_RES_MAX 12

/*delete PN from name*/
#define TDSSRCH_BCH_CELL_SELECT_INCLUDED  0x0400

#define TDSSRCH_Q_DEFAULT_CELL_SELECTION_MIN (-105)
#define TDSSRCH_CELL_SELECTION_MAX_CANDIDATE_CELL_NUM   3

/*it will replace TDSSRCH_PN_SYNC_NSET_INCLUDED and so on*/
#define TDSSRCH_PF_RSCP_NSET_INCLUDED        0x0002
#define TDSSRCH_CF_RSCP_NSET_INCLUDED      0x0004
#define TDSSRCH_REMF_RSCP_NSET_INCLUDED    0x0008
#define TDSSRCH_ISCP_CELL_INCLUDED    0x0010

#define TDSSRCH_DSET_RSCP_DIFF_SCELL  15
#define TDSSRCH_SCELL_SIR_TARGET_LIMIT  7

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
/* Nc=2048, Nn=1 PN, Nc=2048, Nn=2 NASTT*/
extern uint32 *tdssrch_nastt_pn_ecio_tab_ptr ; 

/* Size of translation tables from IS-95 Ec/Io to anything. This is just the
 * number of possible Ec/Io values 
 */
#define  TDS_ECIO_TAB_SIZ       64

#define TDSSRCH_GET_PN_ENG(idx)                      (tdssrch_nastt_pn_ecio_tab_ptr[idx])
//#define TDSSRCH_CONVERT_ENG_TO_ECIO(eng,sttd)        tdssrch_eng_to_ecio(eng, tdssrch_nastt_pn_ecio_tab_ptr)
#define TDSSRCH_CONVERT_ECIO_TO_ENG(ecio,sttd)       ((ecio >= TDS_ECIO_TAB_SIZ) ? TDSSRCH_GET_PN_ENG(TDS_ECIO_TAB_SIZ-1): TDSSRCH_GET_PN_ENG(ecio))
#endif 
/*--------------------------------------------------------------------------
                         Function-like Macros
--------------------------------------------------------------------------*/

#ifdef FEATURE_TDSCDMA_SRCHACQ_RETRY
/* number of retries when ACQ fails */
#define TDSSRCHACQ_MAX_RETRY_CNT 3
#endif

/*the value is TBD*/
#define TDSSRCH_NO_MEAS_VALUE 0x7FFF

#ifdef FEATURE_TDSCDMA_L1_NEW_GAP_DETECT_API
#define TH_MAX_Q_DIFF 0x0a00
#define TH_MIN_Q 0x6400
#define TH_NARROW_GAP 0x011f 
#endif
/*--------------------------------------------------------------------------
                            SEARCH MODULE STATES

--------------------------------------------------------------------------*/
typedef enum
{
  TDSSRCH_IDLE_STATE,    /* Idle state */
  TDSSRCH_FS_STATE,      /* WCDMA Carrier Frequency Scan */
  TDSSRCH_ACQ_STATE,     /* WCDMA Cell acquisition */
  TDSSRCH_BCH_STATE,     /* Reception of a BCH channel on Serving Cell */
  //TDSSRCH_PCH_STATE,     /* Reception of a PCH channel on Serving Cell */
  TDSSRCH_FACH_STATE,    /* Reception of a FACH channel on Serving Cell */
  TDSSRCH_DCH_STATE,     /* Reception of a DCH channel on Serving Cell */
  TDSSRCH_DRX_STATE,     /* DRX in Idle Mode or Connected Mode */
  TDSSRCH_DZ_STATE,      /* Deep Sleep Mode */
  TDSSRCH_BPLMN_STATE,
	TDSSRCH_SYNC_STATE,			/* SYNC state */
  TDSSRCH_MAX_STATE      /* Must remain last item in enum. */
} tdssrch_state_enum_type;

/*--------------------------------------------------------------------------
                Search Operation Control Structure Type
--------------------------------------------------------------------------*/

typedef struct
{
  uint16 periodic_meas_rpt_cnt;
  uint16 meas_id;
}
tdssrch_ops_ctl_parm_struct_type;

extern tdssrch_ops_ctl_parm_struct_type  tdssrchmeas_ops_ctl_parms;


#define TDSSRCH_MAX_RSCP_CMD_NUM   2

#define TDSSRCH_MAX_ISCP_CMD_NUM   2

#define TDSSRCH_ISCP_PAIR_MAX 5

#ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT
#define TDSSRCH_ISCP_PAIR_MAX_PER_SLOT  4 //5
#define TDSSRCH_ISCP_PAIR_MAX_NON_TS0   20 //25
#endif

#ifdef FEATURE_JDS_OPT

#define FEATURE_TDSCDMA_JDS_DEBUG
#define FEATURE_JDS_DOUBLE_CELL_CMD_BUFF
//#define FEATURE_TDSCDMA_RESEL_OPT

#ifndef FEATURE_JDS_DOUBLE_CELL_CMD_BUFF
#define TDSSRCH_MAX_TS0_CMD_NUM   2
#define TDSSRCH_MAX_NON_TS0_CMD_NUM   3
#else /* FEATURE_JDS_DOUBLE_CELL_CMD_BUFF */
#define TDSSRCH_MAX_TS0_CMD_NUM   4
#define TDSSRCH_MAX_NON_TS0_CMD_NUM   6
#endif /* FEATURE_JDS_DOUBLE_CELL_CMD_BUFF */

#define TDSSRCH_MAX_ASET_UPDATE_CMD_NUM  4
#endif /* FEATURE_JDS_OPT */

/*the input para will be negative*/
/*for example,if input -20.6*256,then we will get -21,
   if input -20.4*256,we will get -20*/
/*changed it from round to floor,i.e, for both -20.1 and -20.6,we will get -21*/
#define  TDSSRCH_X256_TO_DBM(filt_val_x256) \
    FLOOR(filt_val_x256,256)
    //CEIL((filt_val_x256-128) ,256)

/*the following are all TBD*/
#define TIME_FOR_ONE_RSSI_MEAS    100   /**/

#define RF_TUNE_TIME_G2T                300   /*us*/
#define RF_TUNE_TIME_T2G                300

/*including RF tuning time*/
#define TIME_DURATION_FOR_GSM_RSSI          13824       /*2 TS(1/8 chip) --TBD*/

#ifdef FEATURE_TRITON_MODEM
#define TIME_DURATION_FOR_GSM_BSIC_ID     20735 //13824      /*3 TS --TBD*/
#define TIME_DURATION_FOR_GSM_BSIC_RC     20735 //13824      /*3 TS --TBD*/
#else // DIME
#define TIME_DURATION_FOR_GSM_BSIC_ID     20735      /*3 TS --TBD*/
#define TIME_DURATION_FOR_GSM_BSIC_RC     20735      /*3 TS --TBD*/
#endif

#define TDSSRCH_LEN_TD_TS_EIGCHIP  6912   /*(1/8 chip)*/
#define TDSSRCH_LEN_TD_SUBFRAME_EIGCHIP  51200 /*(1/8 chip)*/

/*this const is from system,refer to the description of tdssrch_filter_meas_results*/
#define TDSSRCH_TL1_FILT_CONST  133


#define TDSSRCH_TL1_M_FILT_alpha 10
/*0.25*256*256*/
#define TDSSRCH_TL1_M_FILT_TH    16384

/*0.7*256*256*/
#define TDSSRCH_TL1_M_FILT_TH_IN_DRX    45875

#define TDSSRCH_TL1_RSCP_FLOOR   -114
#define TDSSRCH_TL1_RSCP_FLOOR_IDX   2

/* While the wallTimeOffset field is too old,
 * the wallTimeOffset value has to be overwritten with the serving cell's value.*/
#define TDSSRCH_WALL_TIME_OFFSET_MAX_AGE 256

/* if TS-RSP - RSCP > The threshold, we need reset the cell's wall time offset */
#define TDSSRCH_RESET_WALL_TIME_OFFSET_TH 127*256


extern uint16 tdssrch_last_req_to_fw_subsfn;
extern uint16 tdssrch_g2t_remaining_lock_time;
//extern boolean tdssrch_l1_request_lock_for_split;
extern boolean tdssrch_split_between_step1_and_step2;
extern boolean tdssrch_postphone_report_cnf;


/*DS request the lock time when GSM is in PS*/
#define TDSSRCH_G2T_REQ_LOCK_TIME_MS        130 

#define TDSSRCH_STATE_CFG_MS         5
#define TDSSRCH_FS_STEP_MS      5 
#define TDSSRCH_ACQ_STEP0_MS        10 
#define TDSSRCH_ACQ_STEP1_MS        40
#define TDSSRCH_ACQ_STEP2_MS        40
#define TDSSRCH_ACQ_STEP_MEAS_MS    5


/**/
#define TDSSRCH_G2T_FREQ_SCAN_MS        (TDSSRCH_STATE_CFG_MS+TDSSRCH_FS_STEP_MS)
/*including state change to ACQ(5ms)+step0(10ms)+step1(40ms)*/
#define TDSSRCH_G2T_ACQ_STEP0_STEP1_MS        (TDSSRCH_STATE_CFG_MS+TDSSRCH_ACQ_STEP0_MS+TDSSRCH_ACQ_STEP1_MS)
/*including state change to tracking(5ms)+step2(40ms)+one time RSCP measurement(5ms)*/
#define TDSSRCH_G2T_ACQ_STEP2_RSCP_MS        (TDSSRCH_STATE_CFG_MS+TDSSRCH_ACQ_STEP2_MS+TDSSRCH_ACQ_STEP_MEAS_MS)


#define TDSSRCH_CAL_G2T_REM_LOCK_TIME(X,Y) \
	    (X >= Y)? (X-Y) : 0



#ifdef FEATURE_TDSCDMA_DSDA
#define TDSSRCH_RX_POWER_REPORT_TIMEING 4 //20ms
#endif

typedef struct
{
	int16 number_of_retries;
}
tdssrchacq_parameters_type;

/*--------------------------------------------------------------------------
          Structure for Serving Cell Search Operation Parameters
--------------------------------------------------------------------------*/

typedef struct
{
  boolean  map_info_included;
    /* Flag indicating if mapping functionality is needed.
       FALSE indicates no further mapping functionality is needed, or using
       implicit mapping: Q_map = Q_meas_LEV. See section 7 of TS25.304.
     */
   /*zy--this field will be delteted later*/
  tdsl1_mapping_info_struct_type  meas_map_info;
    /* Measurement mapping information. It is used when the above flag is
       TRUE
     */
  tdsl1_mapping_LCR_struct_type  mapping_lcr_info;

  tdsl1_meas_quan_enum_type   quan_type;
    /* Measurement quantity -- Ec/Io or RSCP. Note that, for Cell Selection
       and Reselection, we only support Ec/Io Measurement Quantity on FDD cells,
       as required by TS25.304.
     */

  boolean thresh_intra_f_incl;
  int16   thresh_intra_f;
    /* Intra-frequency measurement threshold. Value (in dB) ranges (-32..20)
       by step of 2. 0xFFFF indicates the parameter is not applicable.
     */

#ifdef FEATURE_TDSCDMA_CELL_RESELECT_INTER_F
  boolean thresh_inter_f_incl;
  int16   thresh_inter_f;
    /* Inter-frequency measurement threshold. Value (in dB) ranges (-32..20)
       by step of 2. 0xFFFF indicates the parameter is not applicable.
     */
#endif

#if defined(FEATURE_TDSCDMA_CELL_RESELECT_INTER_S) 
  boolean thresh_interrat_incl;
    /* If Sinterrat,m not included, do measurements always */

  uint16  num_rats;
    /* Number of additional RATs included */
  tdsl1_interrat_resel_info_type_tdstemp   inter_s_info[TDSL1_MAX_CELL_MEAS];
    /* Inter-RAT Reselection Information */
#endif

  uint16   hyst1;
    /* Hysteresis value of the serving cell for RSCP (?), It is used for
       calculating cell-ranking R in HCS. Value ranges (0..40) by step of 2.
     */
   /*zy--this field will be delte later*/
  uint16   hyst2;
    /* Hysteresis value of the serving cell for Ec/Io (?), It is used for
       calculating cell-ranking R in HCS. Value ranges (0..40) by step of 2.
       Default value is qhyst1_s.
     */

  uint16   t_resel_frames;
    /* Cell reselection timer, in 5ms unit */

  int16   max_tx_pwr;
    /* Maximum allowed UL TX power, UE_TXPER_MAX_RACH (dBm) */
  /*zy--this field will be delte later*/
  int16   q_qualmin;
    /* Minimum required quality level for Ec/Io measurement (in dB).
       Value ranges (-20..0)
     */
  int16   q_rxlevmin;
    /* Minimum required received level for RSCP measurement (in dBm).
       Value ranges (-115..-25) by step of 2.
     */
  uint16	 q_rxlevmin_offset;
	/* Offset to the signalled Qrxlevmin taken into account in the Srxlev */
	/* evaluation as a result of a periodic search for a higher priority	*/
	/* PLMN while camped normally in a VPLMN. SIB3 only in v770 Ext.		*/
	/* Value ranges (2..16) by step of 2. Default to 0 */

  int16 q_selectmin;
      /* The default value of Qselect_min is -105 dBm; The default value is */
      /* useful in the iRAT redirection case where UE has never camped on */
      /* any TDS cell; Every time the UE camps successfully on a TDS cell,    */
      /* Qselect_min is updated with the minimum of                                  */
      /* '(Qrxlevmin + QrxlevminOffset) - Pcompensation' of UE camped     */
      /* cells in the past. */
      
#ifdef FEATURE_TDSCDMA_HCS
  boolean hcs_used;
    /*Flag indicating if HCS is enabled on the current cell or not */

  boolean hcs_info_incl;
    /* Flag indicating if Serving cell HCS information included */
  tdsl1_serv_cell_hcs_info_struct_type hcs_info;
    /* Serving cell HCS information */
#endif

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
  boolean prior_used;
    /*Flag indicating if priority information is provided by network or not */

  //TO DO : Not needed, clean it up
  //  boolean prior_info_incl;
    /* Flag indicating if Serving cell Priority information included */

  tdsl1_serving_cell_priority_info_type prior_info;
    /* Serving cell Priority information */
#endif

  boolean thresh_hcs_incl;
  int16   thresh_hcs;
    /* HCS measurement threshold. Value (in dB) ranges (-105..91)
       by step of 2. 0xFFFF indicates the parameter is not applicable.
     */
    
    /* Part of REL5 CR-136 R1*/

    /* This specifies the maximum number of cell reselections in 
       case of non-HCS usage.
    */
  uint16 nhcs_n_cr;
    /* This specifies the duration for evaluating allowed amount of 
       cell reselection(s) in case of non-HCS usage.
    */
  tdsl1_t_cr_max_enum_type nhcs_t_crmax;
    /* This specifies the additional time period before the UE can 
       revert to low-mobility measurements in case of non-HCS usage.
    */
  tdsl1_t_cr_max_hyst_enum_type nhcs_t_crmaxhyst;

    /* Speed dependent ScalingFactor for Treselection */
  uint16 tresel_scale;

    /* Inter-frequency ScalingFactor for Treselection */ 
  uint16 tresel_interf_scale;

    /* Inter-RAT ScalingFactor for Treselection */
  uint16 tresel_irat_scale;

}
tdssrch_aset_parms_struct_type;


/*-----------------------------------------------------------------------*
 *       Non-cell-specific Intra-Frequency measurement Parameters        *
 *-----------------------------------------------------------------------*/
#define TDSSRCH_MAX_INTRAF_MEAS_ALLOWED 8
#define TDSSRCH_INCLUDE_ALL_CELLS_FOR_MEAS 255

typedef struct 
{
   struct 
   {
     /* This field will indicate if this filtering is currently used or not */
     boolean in_use;
     /* filter coeff value sent in higher layer msg */
     uint16 l3_filt_val;
     /* filter max length obtained from l3_filt_val */
     uint16 aset_max_filt_len;
     uint16 nset_max_filt_len;
     uint16 aset_rxdiv_filt_len;
     /* best aset cell when sorted w.r.t filt eng corresponding to this filtering */
     tdssrchset_cell_struct_type * best_aset_ptr;
     tdssrchset_cell_struct_type_tmp * best_intraset_ptr;  
   } filt_db[TDSSRCH_TDSCDMA_MAX_FILT_SIZE];
   
   uint16  num_filt_idx;
  
}  tdssrch_intraf_filt_idx_struct_type;



typedef struct 
{
   struct 
   {
     /* This field will indicate if this filtering is currently used or not */
     boolean in_use;
     /* filter coeff value sent in higher layer msg */
     uint16 l3_filt_val;
       
   } filt_db[TDSSRCH_TDSCDMA_MAX_FILT_SIZE];
   
   uint16  num_filt_idx;
  
}  tdssrch_interf_filt_idx_struct_type;



#define TDSSRCH_DEFAULT_FILTER_IDX 0

extern tdssrch_intraf_filt_idx_struct_type tdssrchmeas_intraf_filt_idx_s;
extern uint16 tdssrchmeas_intraf_filt_coeff_val ;
extern int8 tdssrchinterf_meas_k_val;

extern boolean tdssrchmeas_intra_freq_meas_performed;
#ifdef FEATURE_TDSCDMA_MOTION_SENSOR_AID_MEAS_SUPPRESS
extern boolean tdssrchmeas_stop_inter_irat_meas;
#endif

typedef struct {
    /* Flag indicating if the following two fields, rach_rpt and rach_rpt_max,
       are included.     */
    boolean  rach_rpt_info_included;
    /* Intra-frequency reporting quantity for RACH reporting as in 10.3.7.42 */
    tdsl1_intra_freq_rach_rpt_quan_struct_type  rach_rpt;
    /* Maximum number of reported cells on RACH as in 10.3.7.68 */
    uint16  rach_rpt_max;
} tdssrch_intraf_rach_rpt_info_struct_type;

extern tdssrch_intraf_rach_rpt_info_struct_type tdssrchmeas_intraf_rach_rpt_info_s;

#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
typedef struct 
{
  boolean interf_rach_rpt_configured;
  tdsl1_meas_rach_interf_struct_type interf_rach_rpt;
}  tdssrch_interf_rach_rpt_info_struct_type;

extern tdssrch_interf_rach_rpt_info_struct_type tdssrchmeas_interf_rach_rpt_info_s;
#endif /* FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING */

typedef struct {
        tdsl1_intra_freq_event_enum_type evt_id ;
        uint16 r;
          /* 0..29 (0..14.5dB) by step of 0.5dB for event 1A and 1B.
             Set to 0xFFFF for all other events.
           */
        uint16 hyst;
          /* Hysteresis. 0..15 (0..7.5dB) by step of 0.5 dB */
        uint16 w;
          /* Weight for even 1A and 1B. 0..20 by step of 0.1. set to 0xFFFF
             for all other events.
           */
        uint16 trig_timer;
          /* Time-to-trigger in 10ms units */

        uint16 rpt_deact_thresh;
          /* Event 1A report deactivation threshold */
        uint16 replace_act_thresh;
          /* Replacement activation threshold for Event 1C */
        int16  thresh;
          /* Threshold used for event 1E and 1F, in dB */

        uint16 num_of_reports ;
        /* How many reports need to be there */

        uint16 reporting_interval ;
        /* The time interval between reports */

        uint16 num_cells_forbidden;
        /* num of cells forbidden for this event */

        uint16 forbidden_cell_scr_code[TDSL1_MAX_CELL_MEAS];
        /* The Prmiary scrambling codes for those cells */
    } tdssrchmeas_evt_trig_struct_type ;

typedef struct {

    uint16 meas_id;

    tdsl1_meas_report_mode_enum_type rpt_mode;

    /* layer 3 filter_coeff  */
    uint16 layer3_filter_coeff;
    /* filter_idx- Can be 0,1 or 2 */
    uint16 filt_idx;

    boolean meas_quan_incl;
    /* Quantity Measured. Note that, for WCDMA FDD cells, we currently support
         Ec/Io or RSCP Measurement Quantity for DCH Measurement report. */
    tdsl1_meas_quan_enum_type   quan_type;

      //Xiaoning modified
    uint8         meas_quan_list_no;
    tdsl1_meas_quan_enum_type   meas_quan[L1_MAX_MEAS_LIST_NO];

    struct {
      uint16 periodic_rpt_cnt;

      uint16 ts_cnt;

      uint16 num_reports_req;

      uint16 reporting_interval;
    } periodic_s;

    tdssrchmeas_evt_trig_struct_type  evt_trig_s;
    /* common to both evt trig and periodic meas reporting */
    boolean rpt_cell_status_incl;
    tdsl1_meas_rpt_cell_status_struct_type  rpt_cell_status;

    tdsl1_intra_dch_rpt_quan_struct_type    aset_quan;
      /* Rpt quantities for active set  */
    tdsl1_intra_dch_rpt_quan_struct_type    mset_quan;
      /* Rpt quantities for monitored set  */
    boolean       dset_quan_incl;
    tdsl1_intra_dch_rpt_quan_struct_type    dset_quan;
      /* Rpt quantity for detected set  */

    tdsl1_meas_report_trans_mode_enum_type  rpt_trans_mode;
      /* Measuremnt Report transfer criteria to UTRAN */

    /* If this is event 1D, this points to the best cell */
    //tdssrchset_cell_struct_type *cTRIGGERED_1D_EVENT;
    //uint16 filt_ecio_best_cell ;

     /* If this is event 1G, this points to the best cell */
    tdssrchset_cell_struct_type_tmp *cTRIGGERED_1G_EVENT;
    uint16 filt_rscp_best_cell ;

    /* For event triggered, maintain a TRIGGERED_EVENT sub structure.
       This is required so that we can report all the cells in here
       every time a cell is reported.
        */
    uint16 num_cells_triggered;
    /* scr code */
    uint16 cells_triggered[TDSL1_MAX_CELL_MEAS];

    uint16 num_cells_for_measurement;
    uint16 cells_for_measurement[TDSL1_MAX_CELL_MEAS];
    boolean stopped;
    #ifdef FEATURE_TDSCDMA_L1_INTRAF_MRM_OPT
    /* Indicate if the peridical reporting is active for this measurement*/
    boolean periodic_rpting_active;
    /*periodic timer for this measurement. All cells when reported in periodic evt
    trigger mode ,this timer is used to set the periodic rpting timer for all the
    cells so that in periodic reports all the cells are reported at the same time */
    uint16 periodic_rpting_timer;
    #endif
} tdssrch_intra_freq_meas_parms_struct_type;

extern tdssrch_intra_freq_meas_parms_struct_type
                      tdssrchmeas_intra_freq_meas_db[TDSSRCH_MAX_INTRAF_MEAS_ALLOWED];
extern uint16 tdssrchmeas_num_intraf_meas_in_progress ;

extern boolean tdssrch_meas_db_init;

/* This flag indicates if the inter-freq meas need to be stored across state transitions */
extern boolean tdssrch_interf_store_meas_db;

extern boolean tdssrchmeas_intraf_periodic_rpts_requested ;
extern boolean tdssrchmeas_intraf_evt_trig_rpts_requested ;


extern boolean tdssrchmeas_no_inter_freq_meas_as_ho_in_progress;
extern boolean tdssrchmeas_no_inter_rat_meas_as_ho_in_progress ;

/*--------------------------------------------------------------------------
               Define srch_cell_resel_status_enum_type
--------------------------------------------------------------------------*/
typedef tdsl1_cell_resel_status_enum_type tdssrch_cell_resel_status_enum_type;

/*===========================================================================

        Exported variables and functions from file srch_evt.c

===========================================================================*/

extern boolean tdssrchmeas_ignore_heartbeat_evt;

extern tdssrchset_cell_struct_type_tmp *tdssrchdch_unsorted_rpt_set_ptr_temp[TDSSRCHDCH_CELL_MEAS_RES_MAX];
extern tdssrchset_cell_struct_type_tmp *tdssrchdch_sorted_rpt_set_ptr_temp[TDSSRCHDCH_CELL_MEAS_RES_MAX];

extern tdssrchset_cell_struct_type *tdssrchdch_unsorted_rpt_set_ptr[TDSSRCHDCH_CELL_MEAS_RES_MAX];
//extern tdssrchset_cell_struct_type *tdssrchdch_sorted_rpt_set_ptr[TDSSRCHDCH_CELL_MEAS_RES_MAX];
/*===========================================================================

        Exported variables and functions from file srch.c

===========================================================================*/

extern tdssrch_state_enum_type  tds_search_state;

extern uint16 tdssrch_ts_cnt;

extern uint16  tdssrch_cur_freq;

extern tdssrch_aset_parms_struct_type  tdssrch_aset_parms;

/* Saved TDSL1_CPHY_CELL_PARM_REQ, used during SIB modification update */
extern tdsl1_cell_parm_cmd_type tdssrch_saved_cell_parms_cmd;

/* Flag to indicate cell parm update due to SIB modification */
extern boolean tdssrch_sib_mod_cell_parm_update;

/* Saved TDSL1_CPHY_MEASUREMENT_REQ, used during SIB modification update */
extern tdsl1_meas_cmd_type tdssrchmeas_saved_meas_req_cmd;
extern tdsl1_meas_cmd_type tdssrchmeas_saved_inter_f_meas_req_cmd;
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
extern tdsl1_meas_cmd_type tdssrchmeas_saved_prior_lte_meas_req_cmd;
#endif

/* Flag to indicate cell parm update due to SIB modification */
extern boolean tdssrchmeas_sib_mod_meas_req_update;
extern boolean tdssrchmeas_sib_mod_inter_f_meas_req_update;
#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
extern boolean tdssrchmeas_sib_mod_prior_lte_meas_req_update;
#endif


extern uint8 tdssrch_ts0_cmd_num;
extern uint8 tdssrch_non_ts0_cmd_num;


typedef struct
{
  uint16 sfn;

  /*if need ignore rscp results*/
  boolean ignore_result;

  /*if this freq's rscp  is scheduling*/
  boolean is_scheduling;
  	
  uint8 freq_index;
  /*\brief Number of TSO JDS cells for the measured frequency
   Max number of TS0 JDS cells = 4*/
  uint8 ts0JdcsNum;  
  /*freq which freq_index points to*/
  uint16 freq;
 /*Rx diversity flag 
   0: One rx antenna
   1: Two rx antennas
   If set to 1, RxAntenna filed ignored*/
 boolean rxDiv;
  /*0: Antenna 0
   1: Antenna 1 */ 
 boolean rxAntenna;
 /* ant sel */
 tfw_ant_sel_e antSel;
 /*0: Intra freq
     1: Inter freq  */
  boolean interFreq; 
  /*If interFreq is 0, ignored
   There are multiple RF scripts buffers for working freq tuning, measurement
   freq tuning etc.
   Configuration TBD   */
  uint8 interFreqRfBufIdx;
   /*0: measured freq is not the same primary freq
   1: measured freq is the same as the primary freq
   Placeholder, to be used if DwPTS based FTL/TTL loops runs when measurement
   freq is the same as the active cell primary frequency*/
  boolean activePrimaryFreq;
  /* measured frequency (Hz), Used by FW for Frequency Tracking Loop (FTL) */
  uint32 measuredFreq;
  
  uint8 mon_det_num;
  tdssrchset_cell_struct_type_tmp *tdssrch_rscp_nset_ptr[TDSSRCHSET_FREQ_CELL_MAX];   
  uint8 rem_cell_num;
  tdssrchset_rset_cell_struct_type  tdssrch_rscp_rset_cell[TDSSRCHSET_MEAS_REM_CELL_MAX];  

  /* SW help FW to store LNA info for inter freq measurement */ 
  #ifdef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
  tfw_rx_lna_t lnaStateInfo;
  #else
  tfw_inter_freq_lna_t inter_lna_info;  
  #endif


}
tdssrch_rscp_cmd_struct_type;

extern tdssrch_rscp_cmd_struct_type tdssrch_send_rscp_cmd[TDSSRCH_MAX_RSCP_CMD_NUM];

typedef struct
{
  //uint8 ts_index;/*it point to the position in tdssrchmeas_sche_iscp_meas_parms.iscp_ts_config*/
  uint8 ts_id;
  tdssrchset_cell_struct_type_tmp *cell_ptr;/*it points to the position in intra freq of tdssrchset_freq_set*/
  //uint8 cpi;/*TBD*/
}
tdssrch_iscp_pair_config_struct_type;

typedef struct
{
  uint16 sfn;
  /*if need ignore iscp results*/
  boolean ignore_result;
  /*if this freq's iscp is scheduling*/
  boolean is_scheduling;
  
  uint8 pair_num;

  #ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT
  tdssrch_iscp_pair_config_struct_type       pair_config[TDSSRCH_ISCP_PAIR_MAX_NON_TS0];/*TBD*/
  #else
  tdssrch_iscp_pair_config_struct_type       pair_config[TDSSRCH_ISCP_PAIR_MAX];/*TBD*/
  #endif
}
tdssrch_iscp_cmd_struct_type;

extern tdssrch_iscp_cmd_struct_type tdssrch_send_iscp_cmd[TDSSRCH_MAX_ISCP_CMD_NUM];


#ifdef FEATURE_JDS_OPT

typedef struct
{ 
  /*number of monitor cells+detected cells*/
  uint8 mon_det_num;
  /*cell info*/
  tdssrchset_cell_struct_type_tmp *tdssrch_rscp_nset_ptr[TDSSRCHSET_FREQ_CELL_MAX];	

}tdssrch_mset_dset_rscp_cmd_struct_type;

typedef struct
{
  /*number of reminder cells*/
  uint8 rem_cell_num;
  /*cell info*/
  tdssrchset_rset_cell_struct_type  tdssrch_rscp_rset_cell[TDSSRCHSET_FREQ_CELL_MAX];  
}
tdssrch_rset_rscp_cmd_struct_type;

typedef struct
{
  uint8 cpid;
  
  tdssrchset_ts0_cell_info_type ts0_demod_cell;
}
tdssrch_ts0_cell_info_type;

typedef struct
{
  /*number of JDCS cells for TS0-demod */
  uint8 interactive_cell_num;	
  /*ts0 demod info */
  uint8 demod_cell_cpid[TDSSRCHSET_TS0_DEMOD_CELL_SIZE];
}
tdssrch_ts0_demod_cmd_struct_type;

typedef struct
{
  /*number of iscp pair*/
  uint8 pair_num;
  /*iscp pair info*/
  #ifdef TDSCDMA_ISCP_OPTIMIZATION_8CELLS_1SLOT
  tdssrch_iscp_pair_config_struct_type       pair_config[TDSSRCH_ISCP_PAIR_MAX_NON_TS0];
  #else
  tdssrch_iscp_pair_config_struct_type       pair_config[TDSSRCH_ISCP_PAIR_MAX];
  #endif
}
tdssrch_iscp_cmd_struct_type_tmp;

typedef struct
{  
  uint16 subframe;
  /*if need ignore rscp results*/
  boolean ignore_result;
  /*if this freq's rscp  is scheduling*/
  boolean is_scheduling;
  
  uint8 freq_index; 
  /*freq which freq_index points to*/
  uint16 freq;  
  /*Rx diversity flag 
   0: One rx antenna
   1: Two rx antennas
   If set to 1, RxAntenna filed ignored*/
  boolean rxDiv;
  /*0: Antenna 0
   1: Antenna 1 */ 
  boolean rxAntenna;
  /* ant sel field */  
  tfw_ant_sel_e antSel;
  /*0: Intra freq
     1: Inter freq  */
  boolean interFreq; 
  /*If interFreq is 0, ignored
   There are multiple RF scripts buffers for working freq tuning, measurement
   freq tuning etc.
   Configuration TBD   */
  uint8 interFreqRfBufIdx;
   /*0: measured freq is not the same primary freq
   1: measured freq is the same as the primary freq
   Placeholder, to be used if DwPTS based FTL/TTL loops runs when measurement
   freq is the same as the active cell primary frequency*/
  boolean activePrimaryFreq;
  /* measured frequency (Hz), Used by FW for Frequency Tracking Loop (FTL) */
  uint32 measuredFreq; 
  /* SW help FW to store LNA info for inter freq measurement */ 
  #ifdef FEATURE_TDSCDMA_FW_NEW_LNA_INTF
  boolean isLnaIfreqValid;
  tfw_rx_lna_t lnaStateInfo;
  #else
  tfw_inter_freq_lna_t inter_lna_info; 
  #endif

  
}
tdssrch_ts0_common_struct_type;

typedef struct
{
  boolean rset_flag;
  uint8 cell_idx;
  
}tdssrch_cell_record_info_struct_type;

typedef struct
{  
#ifdef FEATURE_JDS_DOUBLE_CELL_CMD_BUFF
  boolean occupied;
#endif

  /*if it is true,which means following field have been filled*/
  boolean ts0_com_flag;
  tdssrch_ts0_common_struct_type ts0_com_field;
  boolean rscp_cmd_flag;
  boolean iscp_cmd_flag;
  boolean ts0_demod_cmd_flag;

  tdssrch_cell_record_info_struct_type ts0_cell_record_info[40];
  
  tdssrch_mset_dset_rscp_cmd_struct_type mon_det_rscp_cmd;
  tdssrch_rset_rscp_cmd_struct_type rset_rscp_cmd;
  tdssrch_iscp_cmd_struct_type_tmp iscp_cmd;
  tdssrch_ts0_demod_cmd_struct_type ts0_demod_cmd;
  
  uint8 interactive_cell_num; 
  uint8 total_cell_num; 
}
tdssrch_ts0_cell_cmd_struct_type;

extern tdssrch_ts0_cell_cmd_struct_type tdssrch_ts0_cell_cmd_buffer[TDSSRCH_MAX_TS0_CMD_NUM];

typedef struct
{ /*number of JDCS cells for NonTS0-demod */
  uint8 interactive_cell_num;	
  /*NonTS0-demod cell info*/
  uint8 demod_cell_cpid[TDSSRCHSET_NON_TS0_DEMOD_CELL_SIZE];
}
tdssrch_non_ts0_demod_cmd_struct_type;


typedef struct
{  
  uint16 subframe;

  /*if need ignore rscp results*/
  boolean ignore_result;

  /*if this freq's rscp  is scheduling*/
  boolean is_scheduling;
  	
  uint8 freq_index;
  
  /*freq which freq_index points to*/
  uint16 freq;  
}
tdssrch_non_ts0_common_struct_type;


typedef struct
{
#ifdef FEATURE_JDS_DOUBLE_CELL_CMD_BUFF
  boolean occupied;
#endif
  /*if it is true,which means non_ts0_com_field have been filled*/
  boolean non_ts0_com_flag;
  tdssrch_non_ts0_common_struct_type non_ts0_com_field;
  boolean iscp_cmd_flag;
  boolean non_ts0_demod_cmd_flag;
  	
  tdssrch_iscp_cmd_struct_type_tmp iscp_cmd;
  tdssrch_non_ts0_demod_cmd_struct_type non_ts0_demod_cmd;
  
  uint8 interactive_cell_num; 
  uint8 total_cell_num;  
}
tdssrch_non_ts0_cell_cmd_struct_type;

extern tdssrch_non_ts0_cell_cmd_struct_type tdssrch_non_ts0_cell_cmd_buffer[TDSSRCH_MAX_NON_TS0_CMD_NUM];

extern uint8 tdssrch_aset_update_cmd_num;
extern tdssrchcmd_aset_update_cmd_type tdssrch_aset_update_cmd_buffer[TDSSRCH_MAX_ASET_UPDATE_CMD_NUM];

#ifdef FEATURE_JDS_DOUBLE_CELL_CMD_BUFF
extern uint8 tdssrch_current_ts0_cell_buff_idx;
extern uint8 tdssrch_current_nonts0_cell_buff_idx;
extern uint16 tdssrch_last_send_ts0_cell_cmd_subframe;
extern uint16 tdssrch_last_send_nonts0_cell_cmd_subframe;
#endif /* FEATURE_JDS_DOUBLE_CELL_CMD_BUFF */

#endif /* FEATURE_JDS_OPT */

extern boolean tdssrch_rssi_valid_for_6e;

extern tdssrchset_cell_struct_type_tmp *tdssrch_camp_cell_ptr;

#ifdef FEATURE_UMTS_SIR_PATHLOSS
/* Pathloss value for UI bars display */
extern uint16 tdssrch_pathloss_ui_display;
#endif

/*===========================================================================

        Exported variables and functions from file srchacq.c

===========================================================================*/

#ifdef FEATURE_UMTS_UNIFORM_OOS_HANDLING_P2
extern boolean tdssrchacq_acq_rpt_sent_to_RRC;
#endif

/* In ACQ state, TS0_CELL_UPDT cmd only can be sent once,we use the global variable to control the logic.*/
extern boolean tdssrchacq_ts0_cell_updt_cmd_is_sent;

extern boolean tdssrch_revd_rrc_msg_to_stop_split;

/*--------------------------------------------------------------------------
       Macro to convert scrambling code to primary scrambling code
--------------------------------------------------------------------------*/
#define TDSSCR_CODE_TO_PRI_SCR_CODE(scr) ((scr) >> 4)

extern void tdssrchmeas_evt_ctl_reset(void);

extern uint16 tdssrchmeas_get_meas_idx_based_on_evt_id(
                tdsl1_intra_freq_event_enum_type evt_id);

extern void tdssrchmeas_get_cells_for_measurement_info(
      tdsl1_intra_freq_cell_list_struct_type *cell_list_ptr, uint16 meas_id);

#define TDSSRCHMEAS_INTRA_FREQ_DB_INTLOCK()  REX_ISR_LOCK(&tdssrch_intra_freq_db_crit_sect)
#define TDSSRCHMEAS_INTRA_FREQ_DB_INTFREE()  REX_ISR_UNLOCK(&tdssrch_intra_freq_db_crit_sect)
#define TDSSRCHMEAS_INTER_FREQ_DB_INTLOCK()  REX_ISR_LOCK(&tdssrch_inter_freq_db_crit_sect)
#define TDSSRCHMEAS_INTER_FREQ_DB_INTFREE()  REX_ISR_UNLOCK(&tdssrch_inter_freq_db_crit_sect)
#define TDSSRCHFS_SUBSTATE_INTLOCK()  REX_ISR_LOCK(&tdssrchfs_scan_substate_crit_sect)
#define TDSSRCHFS_SUBSTATE_INTFREE()  REX_ISR_UNLOCK(&tdssrchfs_scan_substate_crit_sect)

#define TDSSRCH_INTERNAL_MEAS_DB_INTLOCK()  REX_ISR_LOCK(&tdssrch_internal_meas_db_crit_sect)
#define TDSSRCH_INTERNAL_MEAS_DB_INTFREE()  REX_ISR_UNLOCK(&tdssrch_internal_meas_db_crit_sect)
#define TDSSRCHTSCNT_INTLOCK()  REX_ISR_LOCK(&tdssrch_ts_cnt_crit_sect)
#define TDSSRCHTSCNT_INTFREE()  REX_ISR_UNLOCK(&tdssrch_ts_cnt_crit_sect)

#define TDSSRCH_OPS_SANITY_CHK_LOCK()  REX_ISR_LOCK(&tdssrch_ops_sanity_chk_crit_sect)
#define TDSSRCH_OPS_SANITY_CHK_FREE()  REX_ISR_UNLOCK(&tdssrch_ops_sanity_chk_crit_sect)


#define TDSSRCHTBAR_INTLOCK()  REX_ISR_LOCK(&tdssrch_tbar_elapsed_cnt_crit_sect)
#define TDSSRCHTBAR_INTFREE()  REX_ISR_UNLOCK(&tdssrch_tbar_elapsed_cnt_crit_sect)

#define TDSSRCH_ASET_UPDTAE_INTLOCK()  REX_ISR_LOCK(&tdssrch_aset_update_cmd_buffer_crit_sect)
#define TDSSRCH_ASET_UPDTAE_INTFREE()  REX_ISR_UNLOCK(&tdssrch_aset_update_cmd_buffer_crit_sect)

extern uint32 tdsschset_blind_ho_in_succession_cnt;

#ifdef FEATURE_JDS_OPT
extern void tdssrch_meas_appointed_freq(uint8 freq_index);
extern boolean tdssrch_check_all_rscp_received(boolean handle_is_on,uint8 msg_id);

#else
extern void tdssrch_meas_appointed_freq(uint8 freq_index,uint8 *rem_cell_index_ptr);
#endif

extern boolean  tdssrch_need_wait_ts0_demod_rsp;

/*===========================================================================

FUNCTION tdssrch_event_timer_exp

DESCRIPTION
  This function check if a event has met time-to-trigger criteria.

DEPENDENCIES
  Time-to-trigger value must be less that (65535 x 10)ms.

RETURN VALUE
  TRUE: if Time-to-trigger criteria is met.
  FALSE: otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean tdssrch_event_timer_exp (uint16 start_ts, uint16 time_to_trig);

/*===========================================================================

        Exported variables and functions from file srchacq.c

===========================================================================*/

/*===========================================================================
FUNCTION     tdssrchacq_init_vctcxo_and_rotator

DESCRIPTION
  This function is invoked at WCDMA activation and before acquisition
  attempts to freeze VCTCXO and optionally set a rotator frequency hypothesis
  to use when attempting to acquire.  In addition, the trk_lo_adj PDM driving
  VCTCXO is initialized to a best estimate based upon what it was in the
  recent past, like when GSM was active for example.

  This entire process is only necessary when WCDMA is first entered.
  Subsequent acquisitions in WCDMA mode leave VCTCXO alone because it is
  probably pretty accurate since it was tracking at some point in the past.

DEPENDENCIES
  None.

PARAMETERS
  read_rot_from_tcxo_mgr - TRUE if we should initialize the rotator frequency
    hypothesis based upon value read from the TCXO MGR.  FASLE otherwise.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchacq_init_vctcxo_and_rotator(boolean read_rot_from_tcxo_mgr);

/*===========================================================================

FUNCTION SRCHDCH_PROC_SRCH_OPS

DESCRIPTION
  This function processes "srch_ops_cmd".

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchdch_proc_srch_ops(tdssrchcmd_ops_cmd_type  *cmd_ptr);

extern void tdssrchmeas_reset_intraf_meas_dbase(void);

#ifdef FEATURE_TDSCDMA_MULTIPLE_L3_FILTERS_SUPPORTED
extern void tdssrchmeas_initialise_default_filter(void);

#define TDSSRCH_INVALID_FILT_IDX 0xFF
/* return the filter to be used when comparing filt_eng > SFN dec thresh before
   SFN decode in case of multiple filters*/
#define TDS_GET_FILT_IDX_FOR_SFN_DEC() (tdssrchdch_filt_idx_for_interf_irat_meas(TDSSRCH_DEFAULT_FILTER_IDX))
#endif

/*===========================================================================

        Exported variables and functions from file srchzz.c

===========================================================================*/

extern boolean TDSSRCHDCH_IS_IN_CELLS_FOR_MEAS ( void *ptr , uint16 meas_idx);

/*===========================================================================

FUNCTION  SRCHDCH_REMOVE_CELL_FROM_TRIGGERED_CELLS

DESCRIPTION


RETURN VALUE
    void

 ============================================================================*/
extern void tdssrchdch_remove_cell_from_triggered_cells (tdssrchset_cell_struct_type_tmp *cell_ptr,
                                                                uint16 meas_idx);

/*===========================================================================

FUNCTION SRCHDCH_RESET_EVT_RPT_DBASE

DESCRIPTION
  This function resets the event reporting database .

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchdch_reset_evt_rpt_dbase(void);

/*===========================================================================

FUNCTION SRCHDCH_REMOVE_FROM_EVT_RPT_CELL_LIST

DESCRIPTION
  This function removes the given cell from the evt_rpt_cell_list.
  This function gets called when this cell's SFN fails to be
  decoded (for some reason).

  The cell will continue to be searched and when

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchdch_remove_from_evt_rpt_cell_list(tdssrchset_cell_struct_type_tmp *cell_ptr);


/* Prototypes of event evaluation functions */

extern boolean tdssrchdch_evt_1g_eval(tdssrchset_cell_struct_type_tmp *cell_ptr, uint16 meas_idx);
extern boolean tdssrchdch_evt_1h_eval(tdssrchset_cell_struct_type_tmp *cell_ptr, uint16 meas_idx);
extern boolean tdssrchdch_evt_1i_eval(tdssrchset_cell_struct_type_tmp *cell_ptr, uint16 meas_idx);
/*===========================================================================

FUNCTION SRCHDCH_FILT_IDX_FOR_INTERF_IRAT_MEAS

DESCRIPTION: This function will return the filt_idx to be used for aset quality estimate
used in inter freq (2d/2f) and IRAT evt evaluations.
   a) check if the filter configured by inter-freq/IRAT meas also available in intraf filt db. 
       if not go to step 2
   2) check if the default fitler is active. if yes use it else go to 3)
   3) if there are only 1 non default filter, use it.But if there are 2 non default
   filter then use the one which has lower filt_eng

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS


===========================================================================*/
uint8 tdssrchdch_filt_idx_for_interf_irat_meas(uint16 filter_coeff);

#ifdef FEATURE_TDSCDMA_MULTIPLE_L3_FILTERS_SUPPORTED
/*===========================================================================

FUNCTION     SRCHDCH_RESET_FILT_PARAMS_ALL_TIMING_KNOWN_CELLS

DESCRIPTION
  reset the filtered parameters for all the cells for the particular filter
  indicated by filt_idx. This will be done when a new filter is added using MCM
  If we dont reset e.g filt_ecio_hi_res then the filtering process will assume that
  its not a new filter eng and clip it to +/-3dB of the stale filt eng already there

DEPENDENCIES
  None.

PARAMETERS
  
 
RETURN VALUE
  none

SIDE EFFECTS
  i
===========================================================================*/
void tdssrchdch_reset_filt_params_all_timing_known_cells_temp(uint16 filt_idx);

#endif /* FEATURE_TDSCDMA_MULTIPLE_L3_FILTERS_SUPPORTED */

#ifdef FEATURE_TDSCDMA_L1_INTRAF_MRM_OPT
/*======================================================================
FUNCTION tdssrchmeas_initialize_cells_for_meas_idx

DESCRIPTION
Used to initialize meas related info for all the cells for the given
index


RETURN VALUE
void

=======================================================================*/
extern void tdssrchmeas_initialize_cells_for_meas_idx(uint16 meas_idx);

#endif
 /*=========================================================================
 
 FUNCTION     : SRCH_SEND_SRCH_CLEANUP_DONEb
 
 DESCRIPTION  : This function sends a srch cleanup done to wl1m.
              
 DEPENDENCIES : 
 
 RETURN VALUE : 
       
 SIDE EFFECTS : 
 
 =========================================================================*/
extern boolean tdssrch_send_srch_cleanup_done(void);

#if 0
extern void tdssrch_bplmn_set_samp_serv_freeze(tdssrch_step_enum_type srch_step);

extern void tdssrchzz_interf_update_eng_params_post_full_srch
(
  /* Cell pointer to inter-frequency cell */
  tdssrchinterf_cell_struct_type *cell_ptr,
  uint16 pilot_idx
);
#endif

#if 0
#ifdef FEATURE_WCDMA_SEND_SS_FREEZE_UNFREEZE_TO_FW
/*===========================================================================
FUNCTION tdssrch_send_sample_server_cmd

DESCRIPTION:
  This function sends the sample server (un)freeze command to the MDSP sync i\f.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrch_send_sample_server_cmd
(
  /* Indicates whether to freeze SS */
  boolean freeze,
  /* Indicates whether to unfreeze SS */
  boolean unfreeze,
  /* Indicates whether to apply freeze/unfreeze action to chain 0 */
  boolean chain0,
  /* Indicates whether to apply freeze/unfreeze action to chain 1 */
  boolean chain1
);
#endif /* FEATURE_WCDMA_SEND_SS_FREEZE_UNFREEZE_TO_FW */

#endif
 
/*=========================================================================
 
 FUNCTION     : mcaltds_srch_current_fw_serving_cell_cpid
 
 DESCRIPTION  : 

              
 DEPENDENCIES : 
 
 RETURN VALUE : 
                
 SIDE EFFECTS : 
 
 =========================================================================*/
extern uint16 mcaltds_srch_current_fw_serving_cell_cpid(void);

/*===========================================================================
FUNCTION     tdssrchacq_proc_rscp_done

DESCRIPTION:
    handle rscp_rsp in ACQ state
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrchacq_proc_rscp_done(uint8 message_id);

/*=========================================================================

 FUNCTION     :TDSSRCH_GET_DPCH_COMPUTE_TX_CHAN_POWER
 
 DESCRIPTION  : get the current DPCH TX POWER.
              
 DEPENDENCIES : 
 
 RETURN VALUE : DPCH TX POWER. 
       
 SIDE EFFECTS : 
==========================================================================*/
extern int16 tdssrch_get_dpch_compute_tx_chan_power(void);

#ifdef FEATURE_JDS_OPT
/*===========================================================================
FUNCTION     tdssrch_reset_ts0_cmd_buffer

DESCRIPTION:
  - reset the some fields of tdssrch_ts0_cell_cmd_buffer
     it will be called after receiveing a TS0_RSP

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrch_reset_ts0_cmd_buffer(uint8 msg_id);
/*===========================================================================
FUNCTION     tdssrch_reset_non_ts0_cmd_buffer

DESCRIPTION:
  - reset the some fields of tdssrch_non_ts0_cell_cmd_buffer
     it will be called after receiveing a Non_TS0_RSP

DEPENDENCIES
  None.

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrch_reset_non_ts0_cmd_buffer(uint8 msg_id);
/*===========================================================================
FUNCTION     tdssrch_check_all_iscp_received

DESCRIPTION:
  - this fucntion will judge if all iscp rsp have been received
  

DEPENDENCIES
  None.

RETURN VALUE
  TRUE--all iscp rsp(including TS0 and non-TS0 ISCP) have been received by SW
  FALSE--vice visa


SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdssrch_check_all_iscp_received(boolean ts0_handle_on,boolean non_ts0_handle_on,uint8 msg_id);


/*=========================================================================

 FUNCTION     :TDSSRCH_PROC_NON_TS0_DEMOD_RSP
 
 DESCRIPTION  : This function handle non-TS0 TS-RSP measurement results and update non-TS0 table and select 
   new iterative cells by latest filt TS-RSP values.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
      uint8 msg_id, 
      tfw_nonTs0_cell_rsp_t *msg_non_ts0_cell_rsp:

 RETURN VALUE : . 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdssrch_proc_non_ts0_demod_rsp(uint8 msg_id, tfw_nonts0_cell_report_t *msg_non_ts0_cell_rsp);

/*=========================================================================

 FUNCTION     :TDSSRCH_PROC_TS0_DEMOD_RSP
 
 DESCRIPTION  : This function handle TS0 TS-RSP measurement results and update TS0 table and select 
   new iterative cells by latest filt TS-RSP values.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
      uint8 msg_id, 
      tfw_ts0_cell_rsp_t *msg_ts0_cell_rsp:

 RETURN VALUE : . 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdssrch_proc_ts0_demod_rsp(uint8 msg_id, tfw_ts0_cell_report_t *msg_ts0_cell_rsp);

/*=========================================================================

 FUNCTION     :TDSSRCH_COMBINE_AND_SEND_NON_TS0_CELL_CMD
 
 DESCRIPTION  : This function combine non-ts0 demod command and non-ts0 iscp command, then send it to FW..
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : . 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdssrch_combine_and_send_non_ts0_cell_cmd(void);

/*=========================================================================

 FUNCTION     :TDSSRCH_COMBINE_AND_SEND_TS0_CELL_CMD
 
 DESCRIPTION  : This function combine rscp command and ts0 demod command and ts0 iscp command.
   then send it to FW.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : . 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdssrch_combine_and_send_ts0_cell_cmd(void);

/*=========================================================================

 FUNCTION     :TDSSRCH_FILL_NON_TS0_DEMOD_CMD
 
 DESCRIPTION  : This function fill non-TS0 demod command in tdssrch_non_ts0_cell_cmd_buffer.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : . 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdssrch_fill_non_ts0_demod_cmd(void);

/*=========================================================================

 FUNCTION     :TDSSRCH_FILL_TS0_DEMOD_CMD
 
 DESCRIPTION  : This function fill TS0 demod command in tdssrch_ts0_cell_cmd_buffer.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
 
 RETURN VALUE : . 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdssrch_fill_ts0_demod_cmd(void);

/*=========================================================================

 FUNCTION     :TDSSRCH_GET_SERVING_CELL_WALL_TIME_OFFSET
 
 DESCRIPTION  : The function will return current wall time offset of serving cell.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : . 
       uint32:
       
 SIDE EFFECTS : 
==========================================================================*/
extern uint32 tdssrch_get_serving_cell_wall_time_offset(void);

#ifdef FEATURE_JDS_DOUBLE_CELL_CMD_BUFF
/*=========================================================================

 FUNCTION     :TDSSRCH_UPDATE_CURRENT_CELL_CMD_BUFF_IDX
 
 DESCRIPTION  : The function will update TS0 and non-TS0 cell buffer index  
  in the current sub-frame, which is used to save TS0 cmd and non-TS0 cmd
  this sub-frame.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdssrch_update_current_cell_cmd_buff_idx(void);

/*===========================================================================
FUNCTION     tdssrch_check_interf_demo_received

DESCRIPTION:
  - this fucntion will judge if interf demo rsp have been received
  

DEPENDENCIES
  None.

RETURN VALUE
  TRUE--interf demo rsp have been received by SW
  FALSE--vice visa
  
SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdssrch_check_interf_demo_received(void);
#endif /* FEATURE_JDS_DOUBLE_CELL_CMD_BUFF */

/*=========================================================================
 
 FUNCTION     : mcaltds_srch_send_serving_cell_cmd
 
 DESCRIPTION  : 

              
 DEPENDENCIES : 
 
 RETURN VALUE : 
                
 SIDE EFFECTS : 
 
 =========================================================================*/
extern void mcaltds_srch_send_serving_cell_cmd(tdssrchset_active_cell_info_struct_type *active_cell_ptr);

#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
extern uint16 tdssrchlog_meas_accumulate_rscp_timer;
#endif
extern void tdssrch_submit_accumulate_rscp_pkt(void);

#ifdef FEATURE_TDSCDMA_ANTENNA_SWITCH_DIVERSITY
extern boolean tdsasdiv_switch_antenna(void);
extern boolean tdsasdiv_switch_antenna_type1_algo(void);
#endif
#endif /* #ifdef FEATURE_JDS_OPT */

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
extern boolean tdssrchacq_is_deep_search(void);

extern void tdssrch_stop_g2t_split_action(void);
/*=========================================================================

 FUNCTION     :TDSSRCH_need_WAIT_DEMOD_rsp
 
 DESCRIPTION  : This is a function for DL to ask SRCH to wait all demod
                rsps before sending cleanup done to L1M.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
extern boolean tdssrch_need_wait_demod_rsp(void);

#endif /* TDSSRCHI_H */


