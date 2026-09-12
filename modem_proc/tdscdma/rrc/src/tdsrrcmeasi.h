#ifndef TDSRRCMEASI_H
#define TDSRRCMEASI_H
/*===========================================================================
        R R C   M E A S U R E M E N T S   H E A D E R   F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  Measurement Control and Report Procedure.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/



/*===========================================================================

          EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcmeasi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/11   yuh   Merged LTE code
03/10/11   zwj   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/17/11   zwj  3GPP-10.3.7.70. Added TVM on E-DCH info for HSUPA.  
11/15/10   bj    Made changes to IE "Measured results on RACH"(FDD->TDD).
11/08/10   zwj   Modify intra event 14.1.3 for TDD.
11/06/10   zwj   Delete compress mode CCSA 8.4.1.3 for TDD.

===========================================================================*/



/*===========================================================================
** Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
** Include Files
**-------------------------------------------------------------------------*/

#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsl1rrcif.h"
#include "tdsrrcasn1.h"
#include "tdsrrcmm_v.h"
#include "tdsrrccommon.h"
#include "err.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include "tdsrrcccm.h"
#include "tdsrrccsp.h"

#if defined (FEATURE_CGPS)
  #include "tdsrrclsmif.h"


#endif /* FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP_TDSCDMA */

#include "mm_umts.h"
#include "sys_eplmn_list.h"
#include "geran_grr_types.h"
/*===========================================================================
**                  STRUCTURE TYPES
**=========================================================================*/

/* Stores RRC transaction Identifier and Error Cause to be used by Meas
 * Control Failure Message */
typedef struct
{
  uint8 rrc_transaction_id;
  uint32 error_choice;
  tdsrrc_ProtocolErrorCause error_cause;
}tdsrrcmeas_mcf_struct_type;

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING

/* Enumerated type of the different substates for RRC GPS  procedure           */
typedef enum
{
  TDSRRCGPS_INITIAL,                   /* Initial Substate                    */
  TDSRRCGPS_WAIT_FOR_SIB_CNF    /* Waiting for confirmation of Cell id from SIB                     */                       
}tdsrrcgps_substates_e_type;

#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/
/*===========================================================================
**                  ENUMERATED TYPES
**=========================================================================*/


/* Indicates if RACH report should be appended */
typedef enum
{
  TDSNO_RRC_MEAS_RACH_REPORT,
  TDSAPPEND_RRC_MEAS_RACH_REPORT
} tdsrach_report_crit_e_type;




/* Indicates the RRC Measurement Procedure Current sub-state */
typedef enum
{
  TDSRRCMEAS_INITIAL,
  TDSRRCMEAS_IDLE,
  TDSRRCMEAS_CELL_DCH,
  TDSRRCMEAS_CELL_FACH,
  TDSRRCMEAS_PCH
}tdsrrcmeas_substate_e_type;


/* Indicates that if SIB12 is present in the cell and has been stored
 * as specified by SIB11. This helps in reading meas directly from SIB12 ON
 * TRANSITION from IDLE-->DCH
 */
typedef enum
{
  TDSSIB12_ABSENT,
  TDSSIB12_PRESENT
} tdssib12_present_e_type;

/* Indicates if a cell is occupied or vacant in CELL_INFO_LIST array */
typedef enum
{
  TDSVACANT,
  TDSOCCUPIED
} tdscell_position_e_type;

/* Indicates if cpi is present or absent */
typedef enum
{
  TDSCPI_ABSENT,
  TDSCPI_PRESENT
} tdscpi_e_type;

typedef enum
{
  TDSRRCMEAS_FAIL,           /* Successful Status */
  TDSRRCMEAS_SUCCESS,         /* Failure status */
  TDSSFN_FAIL,
  TDSRRCMEAS_UNSUPPORTED      /* if band unsupported */
}tdsrrcmeas_status_e_type;

typedef enum
{
  TDSL1_MEAS,
  TDSMAC_MEAS,
  TDSINTRA_FREQ,
  TDSINTER_FREQ,
  TDSINTER_SYS,
  TDSINTERNAL_MEAS,
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
  TDSPOSITION_MEAS,
#endif
  TDSQUALITY_MEAS,
  TDSUNKNOWN_MEAS,
  TDSNOT_EXIST
}tdsrrcmeas_layer_e_type;


typedef enum
{
  TDSTVM_FAIL,           /* Successful Status */
  TDSTVM_SUCCESS         /* Failure status */
}tdsrrctvm_status_e_type;

typedef enum
{
  TDSSIB11,
  TDSSIB12
}tdssib_choice_e_type;


/*
 * 10.3.7.51  - Measurement Validity
 */
typedef enum
{
  TDSALL_STATES,
  TDSALL_STATES_BUT_DCH,
  TDSSTATE_DCH,
  TDSNOT_PRESENT
} tdsmeas_validity_enum_type;



/* Accepted transaction Table */
typedef struct
{
  uint8 transaction_id;
  /*
   boolean to track if the received trans id is latest among the same
   transaction ids 
  */
  boolean is_latest_trans;
} tdsrrcmeas_accepted_transaction_list;

/* Store Reporting Criteria in Internal Format */
typedef enum
{
  TDSPERIODIC,
  TDSEVENT,
  TDSADDTL,
  TDSNOT_AVAILABLE
} tdsrpt_crit_enum_type;


/*===================================================================
                        Defines
====================================================================*/

/* Max no of meas supported by L1. At present it is 1 */
// #define MAX_NUMBER_OF_MEAS_SUPPORTED_BY_L1  1

/* Max no of monitored cells sent by L1 in RACH report to UTRAN
 * Actually the total no is 7 with first being the active cell.
 * RRCMEAS procedure build the list for 7 cells. L1 sends list of 6
 * monitored cells. To read 6 monitored cells from L1, it is defined
 */
#define TDSMAX_NO_MONITORED_CELLS  6

#define TDSMEASUREMENT_IDENTITY_MIN 1
#define TDSMEASUREMENT_IDENTITY_MAX 16
/* Maximum allowed UE transmit power */
#define TDSUE_TXPWR_AMX_RACH 33

/* Quality params */
#define TDSQQUALMIN  -20

#define TDSQRXLEVMIN -115

#define TDSQOFFSET1 0

#define TDSQOFFSET2 0

/* L1 wants these 2 params initialized when they are not present */
#define TDSNO_FORBIDDEN_CELLS  0xFFFF

#define TDSNO_W  0xFFFF

/* Initializes transaction id to default ERROR value */
#define TDSINVALID_TRANSACTION_ID  55
#define TDSMAX_REL99_TRANSACTION_ID 3


/* To convert UTRAN values of reference time difference to L1 format */
#define TDSREF_TIME_DIFF_ACCURACY_40   40
#define TDSREF_TIME_DIFF_ACCURACY_256    256
#define TDSREF_TIME_DIFF_ACCURACY_2560   2560

#define TDSMAX_NO_OF_CELLS 32

#define TDSAVAILABLE_CELL_POSITION 100

#define TDSINVALID_CPI 1000


#define TDSINVALID_MEAS_ID 200

/* Following are used to check params in Meas Report */
#define TDSTM_MIN 0

#define TDSTM_MAX 38399

#define TDSOFF_MIN 0

#define TDSOFF_MAX 255

#define TDSMAXIMUM_TRANSACTIONS 16

/* MAX No of Inter-Freq Measurements */
#define TDSMAX_INTER_FREQ_MEAS 7


/* MAX No of Inter-RAT Measurements */
#define TDSMAX_INTER_RAT_MEAS 4

/* Invalid Cell Id for Inter-RAT Measurements */
/*Moved the definiton of TDSINVALID_CELL_ID to tdsrrcdata.h*/

/* TDS-1173 increase pending ack to 20 from 12 
 * to satisfy test equipment limitation
 * 4 sec no ACK from SS
 */
#define TDSMAX_PEND_MEAS_RPT 20

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
#define TDSMAX_POS_MEAS 2
#endif
/* As per specs, Max Intra Freq Meas at a time is 8 */
#define TDSMAX_INTRA_FREQ_MEAS 8

/* No scenario seen as yet of more than 1. */
#define TDSMAX_ADDITIONAL_MEAS 4
/* 25.123-9.1.1.3, reporting range for Timeslot ISCP */
#define TDSISCP_MIN_VALUE  0
#define TDSISCP_MAX_VALUE  91
#define TDSRSCP_MIN_VALUE  0
#define TDSRSCP_MAX_VALUE  91

#define TDSDELTA_RSCP_MAX -1
#define TDSDELTA_RSCP_MIN -5

#define TDSPATH_LOSS_MIN_VALUE    46
#define TDSPATH_LOSS_MAX_VALUE    158
#define TDSSFN_SFN_TYPE1_MIN_VALUE 0
#define TDSSFN_SFN_TYPE1_MAX_VALUE 3276799
#define TDSSFN_SFN_TYPE2_MIN_VALUE 0
#define TDSSFN_SFN_TYPE2_MAX_VALUE 27649
#define TDSMAX_CPI 127
#define TDSREMOVE_ALL_INTRA_FREQ_CELLS 0xff

#define TDSINTER_FREQ_RSSI 76

#define TDSMIN_CPICH_TX_POWER_RANGE -10
#define TDSMAX_CPICH_TX_POWER_RANGE 50

#define TDSRET_INTER_RAT_BAND(band) \
( \
(band == tdsrrc_Frequency_Band_dcs1800BandUsed) ? TDSL1_DCS_1800_BAND : TDSL1_PCS_1900_BAND \
)

/* Converts meas quantity from ASN format to L1 format */
/* After discussed with systems team, consider dummy as RSCP */
#define  TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN(intra_freq_meas_quan)   \
(  \
 (intra_freq_meas_quan == tdsrrc_IntraFreqMeasQuantity_TDD_pathloss) ? TDSL1_PATHLOSS : \
  ((intra_freq_meas_quan == tdsrrc_IntraFreqMeasQuantity_TDD_timeslotISCP) ? TDSL1_ISCP : \
    TDSL1_RSCP  \
  )  \
)
/*
#define  TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN(intra_freq_meas_quan)   \
(  \
 (intra_freq_meas_quan == tdsrrc_IntraFreqMeasQuantity_TDD_primaryCCPCH_RSCP) ? TDSL1_RSCP : \
  ((intra_freq_meas_quan == tdsrrc_IntraFreqMeasQuantity_TDD_pathloss) ? TDSL1_PATHLOSS : \
    ((intra_freq_meas_quan == tdsrrc_IntraFreqMeasQuantity_TDD_timeslotISCP) ? TDSL1_ISCP : \
        TDSL1_MEAS_NONE\
    )  \
  )  \
)
*/


/* Converts sfn sfn observed difference from ASN format to L1 format */
#define  TDSRRCMEAS_RET_INTRA_FREQ_SFN_DELTA(sfn_sfn_otd_type) \
(     \
 (sfn_sfn_otd_type == tdsrrc_SFN_SFN_OTD_Type_noReport) ? TDSL1_NO_REPORT :     \
  ((sfn_sfn_otd_type == tdsrrc_SFN_SFN_OTD_Type_type1) ? TDSL1_REPORT_TYPE_1 : TDSL1_REPORT_TYPE_2  \
  )   \
)

/* Converts sfn sfn observed difference from l1 FORMAT TO ASN format */
#define  TDSRRCMEAS_RET_INTRA_FREQ_SFN_SFN(tdsl1_sfn_report_enum_type) \
(     \
  (tdsl1_sfn_report_enum_type == TDSL1_REPORT_TYPE_1) ? T_tdsrrc_SFN_SFN_ObsTimeDifference_type1: \
   T_tdsrrc_SFN_SFN_ObsTimeDifference_type2 \
)




/* Converts the Intra Freq RACH Reporting Quantity for FDD
 * from ASN1 format to L1 format
 */
#define  TDSRRCMEAS_RET_INTRA_FREQ_RACH_REP_QUAN(intra_freq_rach_rep_quan)  \
 (  \
  (intra_freq_rach_rep_quan == tdsrrc_IntraFreqRepQuantityRACH_TDD_timeslotISCP) ? TDSL1_ISCP : \
   ((intra_freq_rach_rep_quan == tdsrrc_IntraFreqRepQuantityRACH_TDD_primaryCCPCH_RSCP) ? TDSL1_RSCP : \
     TDSL1_MEAS_NONE  \
   )  \
 )

#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
#define  TDSRRCMEAS_RET_INTER_FREQ_RACH_REP_QUAN(inter_freq_rach_rep_quan)  \
(  \
 (inter_freq_rach_rep_quan == tdsrrc_InterFreqRepQuantityRACH_TDD_primaryCCPCH_RSCP) ? TDSL1_RSCP : \
     TDSL1_MEAS_NONE  \
)
#endif

/* Converts triggering condition for FDD from ASN1 format to L1
 * format used by event 1a
 */
#define  TDSRRCMEAS_RET_TRIGGERING_CONDITION_2(triggering_condition)  \
(  \
 (triggering_condition == tdsrrc_TriggeringCondition2_activeSetCellsOnly) ? 0 : \
  ((triggering_condition == tdsrrc_TriggeringCondition2_monitoredSetCellsOnly) ? 1 : \
  ((triggering_condition == tdsrrc_TriggeringCondition2_activeSetAndMonitoredSetCells) ? 2 :\
  (triggering_condition == tdsrrc_TriggeringCondition2_detectedSetCellsOnly) ? 3 : 4 \
   ) \
  ) \
)


/* Converts triggering condition for FDD from ASN1 format to L1
 * format Used by event 1b
 */
#define  TDSRRCMEAS_RET_TRIGGERING_CONDITION_1(triggering_condition)  \
(  \
 (triggering_condition == tdsrrc_TriggeringCondition1_activeSetCellsOnly) ? 0 : \
  ((triggering_condition == tdsrrc_TriggeringCondition1_monitoredSetCellsOnly) ? 1 : 2 \
  ) \
)


/* Converts report deactivation threshold for FDD from ASN1 format
 * to L1 format
 */
#define  TDSRRCMEAS_RET_DEACTIVATION_THRESHOLD(deactivation_threshold)  \
(  \
 (deactivation_threshold == tdsrrc_ReportDeactivationThreshold_notApplicable) ? 0 : \
  ((deactivation_threshold == tdsrrc_ReportDeactivationThreshold_t1) ? 1 : \
  ((deactivation_threshold == tdsrrc_ReportDeactivationThreshold_t2) ? 2 :\
  ((deactivation_threshold == tdsrrc_ReportDeactivationThreshold_t3) ? 3 :\
  ((deactivation_threshold == tdsrrc_ReportDeactivationThreshold_t4) ? 4 :\
  ((deactivation_threshold == tdsrrc_ReportDeactivationThreshold_t5) ? 5 :\
  (deactivation_threshold == tdsrrc_ReportDeactivationThreshold_t6) ? 6 : 7 \
   ) \
  ) \
  ) \
  ) \
  ) \
)


/* Converts report deactivation threshold for FDD from ASN1 format to
 * L1 format
 */
#define  TDSRRCMEAS_RET_REPORTING_AMOUNT(reporting_amount)  \
(  \
 ((reporting_amount == tdsrrc_ReportingAmount_ra1) ? 1 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra2) ? 2 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra4) ? 4 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra8) ? 8 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra16) ? 16 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra32) ? 32 :\
  (reporting_amount == tdsrrc_ReportingAmount_ra64) ? 64 : 0 \
  )))))) \
)


/* Converts replacement activation threshold for FDD from ASN1 format
 * to L1 format
 */
#define  TDSRRCMEAS_RET_REPLACEMENT_ACTIVATION_THRESHOLD(replacement_activation_threshold)  \
(  \
 (replacement_activation_threshold == tdsrrc_ReplacementActivationThreshold_notApplicable) ? 0 : \
  ((replacement_activation_threshold == tdsrrc_ReplacementActivationThreshold_t1) ? 1 : \
  ((replacement_activation_threshold == tdsrrc_ReplacementActivationThreshold_t2) ? 2 :\
  ((replacement_activation_threshold == tdsrrc_ReplacementActivationThreshold_t3) ? 3 :\
  ((replacement_activation_threshold == tdsrrc_ReplacementActivationThreshold_t4) ? 4 :\
  ((replacement_activation_threshold == tdsrrc_ReplacementActivationThreshold_t5) ? 5 :\
  (replacement_activation_threshold == tdsrrc_ReplacementActivationThreshold_t6) ? 6 : 7 \
  ))))) \
)


/* Converts replacement activation threshold for FDD from ASN1
 * format to L1 format
 */
#define  TDSRRCMEAS_RET_REPORTING_INTERVAL(reporting_interval)  \
(  \
 (reporting_interval == tdsrrc_ReportingInterval_noPeriodicalreporting) ? 0 : \
  ((reporting_interval == tdsrrc_ReportingInterval_ri0_25) ? 250 : \
  ((reporting_interval == tdsrrc_ReportingInterval_ri0_5) ? 500 :\
  ((reporting_interval == tdsrrc_ReportingInterval_ri1) ? 1000 :\
  ((reporting_interval == tdsrrc_ReportingInterval_ri2) ? 2000 :\
  ((reporting_interval == tdsrrc_ReportingInterval_ri4) ? 4000 :\
  (reporting_interval == tdsrrc_ReportingInterval_ri8) ? 8000 : 16000 \
  ))))) \
)


/* Converts time to trigger for FDD from ASN1 format to L1 format */
#define  TDSRRCMEAS_RET_TIME_TO_TRIGGER(time_to_trigger)  \
(  \
 (time_to_trigger == tdsrrc_TimeToTrigger_ttt0) ? 0 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt10) ? 10 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt20) ? 20 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt40) ? 40 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt60) ? 60 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt80) ? 80 : \
  ((time_to_trigger ==tdsrrc_TimeToTrigger_ttt100) ? 100 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt120) ? 120 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt160) ? 160 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt200) ? 200 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt240) ? 240 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_tt320) ? 320 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt640) ? 640 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt1280) ? 1280 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt2560) ? 2560 : 5000 \
  )))))))))))))) \
)

/* Converts meas quantity from L1 format to ASN format */

#define  TDSRRCMEAS_RET_ASN_MEAS_QUANTITY(meas_quantity_type)  \
(  \
 (meas_quantity_type == TDSL1_EC_NO) ? rrc_cpich_Ec_N0_present : \
 ((meas_quantity_type == TDSL1_RSCP) ? rrc_cpich_RSCP_present : \
  rrc_fdd_pathloss_present  \
 ) \
)


/* Converts meas reporting mode for FDD from ASN1 format to L1 format */
#define  TDSRRCMEAS_RET_MEAS_REPORTING_MODE(periodic_or_event_trigger)  \
(  \
 (periodic_or_event_trigger == tdsrrc_PeriodicalOrEventTrigger_periodical) ? TDSL1_PERIODIC_RPT : \
 ((periodic_or_event_trigger == tdsrrc_PeriodicalOrEventTrigger_eventTrigger) ? TDSL1_EVENT_TRIGGER_RPT : TDSL1_NO_CHANGE \
 ) \
)


/* Converts meas report transfer mode for RLC for a meas report from ASN1 format to Local format */
#define  TDSRRCMEAS_RET_MEAS_RLC_TRANSFER_MODE(tdsrrc_TransferMode)  \
(  \
 (tdsrrc_TransferMode == tdsrrc_TransferMode_acknowledgedModeRLC) ? TDSL1_ACKNOWLEDGED_MODE : TDSL1_UNACKNOWLEDGED_MODE \
)

/* Converts intra-freq meas quantity filter coefficient for FDD from ASN1
 * format to L1 format
 */

#define  TDSRRCMEAS_RET_INTRA_FREQ_MEAS_QUAN_FILTER_COEFFICIENT(fc)  \
(  \
   (fc == tdsrrc_FilterCoefficient_fc0) ? TDSL1_TDS_FILTER_COEF_0 : \
  ((fc == tdsrrc_FilterCoefficient_fc1) ? TDSL1_TDS_FILTER_COEF_1 :\
    ((fc == tdsrrc_FilterCoefficient_fc2) ? TDSL1_TDS_FILTER_COEF_2 :\
  ((fc == tdsrrc_FilterCoefficient_fc3) ? TDSL1_TDS_FILTER_COEF_3 :\
    ((fc == tdsrrc_FilterCoefficient_fc4) ? TDSL1_TDS_FILTER_COEF_4 :\
  ((fc == tdsrrc_FilterCoefficient_fc5) ? TDSL1_TDS_FILTER_COEF_5 :\
  ((fc == tdsrrc_FilterCoefficient_fc6) ? TDSL1_TDS_FILTER_COEF_6 :\
  ((fc == tdsrrc_FilterCoefficient_fc7) ? TDSL1_TDS_FILTER_COEF_7 :\
    ((fc == tdsrrc_FilterCoefficient_fc8) ? TDSL1_TDS_FILTER_COEF_8 :\
  ((fc == tdsrrc_FilterCoefficient_fc9) ? TDSL1_TDS_FILTER_COEF_9 :\
  ((fc == tdsrrc_FilterCoefficient_fc11) ? TDSL1_TDS_FILTER_COEF_11 :\
  ((fc == tdsrrc_FilterCoefficient_fc13) ? TDSL1_TDS_FILTER_COEF_13 :\
  ((fc == tdsrrc_FilterCoefficient_fc15) ? TDSL1_TDS_FILTER_COEF_15 :\
  ((fc == tdsrrc_FilterCoefficient_fc17) ? TDSL1_TDS_FILTER_COEF_17 :\
  (fc == tdsrrc_FilterCoefficient_fc19) ? TDSL1_TDS_FILTER_COEF_19 : TDSL1_TDS_FILTER_COEF_0 \
  ))))))))))))) \
)

/* Converts max no of reported cells on RACH from ASN1
 * format to L1 format
 */
#define  TDSRRCMEAS_RET_RACH_REPORTED_CELLS(max_no_of_reported_cells)  \
(  \
 (max_no_of_reported_cells == tdsrrc_MaxReportedCellsOnRACH_noReport) ? TDSL1_RACH_RPT_NONE : \
  ((max_no_of_reported_cells == tdsrrc_MaxReportedCellsOnRACH_currentCell) ? TDSL1_RACH_RPT_0 : \
  ((max_no_of_reported_cells == tdsrrc_MaxReportedCellsOnRACH_currentAnd_1_BestNeighbour) ? TDSL1_RACH_RPT_1 :\
  ((max_no_of_reported_cells == tdsrrc_MaxReportedCellsOnRACH_currentAnd_2_BestNeighbour) ? TDSL1_RACH_RPT_2 :\
  ((max_no_of_reported_cells == tdsrrc_MaxReportedCellsOnRACH_currentAnd_3_BestNeighbour) ? TDSL1_RACH_RPT_3 :\
  ((max_no_of_reported_cells == tdsrrc_MaxReportedCellsOnRACH_currentAnd_4_BestNeighbour) ? TDSL1_RACH_RPT_4 :\
  (max_no_of_reported_cells == tdsrrc_MaxReportedCellsOnRACH_currentAnd_5_BestNeighbour) ? TDSL1_RACH_RPT_5 : TDSL1_RACH_RPT_6 \
  ))))) \
)

/* Converts periodic reporting interval for FDD from ASN1
 * format to L1 format
 */
#define  TDSRRCMEAS_RET_PERIODIC_REPORTING_INTERVAL(ri)  \
(  \
 (ri == tdsrrc_ReportingIntervalLong_ril0) ? 0 : \
  ((ri == tdsrrc_ReportingIntervalLong_ril0_25) ? 250 : \
  ((ri == tdsrrc_ReportingIntervalLong_ril0_5) ? 500 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril1) ? 1000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril2) ? 2000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril3) ? 3000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril4) ? 4000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril6) ? 6000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril8) ? 8000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril12) ? 12000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril16) ? 16000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril20) ? 20000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril24) ? 24000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril28) ? 28000 :\
  (ri == tdsrrc_ReportingIntervalLong_ril32) ? 32000 : 64000 \
  ))))))))))))) \
)

/* Converts meas quantity from L1 format to RACH ASN format */
#define  TDSRRCMEAS_RET_RACH_MEAS_QUANTITY(meas_quantity_type)  \
(  \
 (meas_quantity_type == TDSL1_EC_NO) ? T_rrc_measurementQuantity_cpich_Ec_N0 : \
 ((meas_quantity_type == TDSL1_RSCP) ? T_rrc_measurementQuantity_cpich_RSCP : \
  rrc_pathloss  \
 ) \
)



#define TDSMEAS_QUANTITY_INTERVAL_OFFSET 20

#define  TDSTVM_RET_REPORTING_AMOUNT(reporting_amount)  \
(  \
 ((reporting_amount == tdsrrc_ReportingAmount_ra1) ? 1 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra2) ? 2 : \
  ((reporting_amount == tdsrrc_ReportingAmount_ra4) ? 4 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra8) ? 8 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra16) ? 16 :\
  ((reporting_amount == tdsrrc_ReportingAmount_ra32) ? 32 :\
  (reporting_amount == tdsrrc_ReportingAmount_ra64) ? 64 : 0xff \
  )))))) \
)

#define  TDSTVM_RET_PERIODIC_REPORTING_INTERVAL(ri)  \
(  \
 (ri == tdsrrc_ReportingIntervalLong_ril0) ? 10 : \
  ((ri == tdsrrc_ReportingIntervalLong_ril0_25) ? 250 : \
  ((ri == tdsrrc_ReportingIntervalLong_ril0_5) ? 500 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril1) ? 1000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril2) ? 2000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril3) ? 3000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril4) ? 4000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril6) ? 6000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril8) ? 8000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril12) ? 12000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril16) ? 16000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril20) ? 20000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril24) ? 24000 :\
  ((ri == tdsrrc_ReportingIntervalLong_ril28) ? 28000 :\
  (ri == tdsrrc_ReportingIntervalLong_ril32) ? 32000 : 64000 \
  ))))))))))))) \
)


#define TDSTVM_RET_EVENT_ID(tdsevent_id) \
( \
(tdsevent_id == tdsrrc_TrafficVolumeEventType_e4a) ? TDSEVENT_4A :TDSEVENT_4B \
)


#define  TDSTVM_RET_THRESHOLD_BYTES(threshold)  \
(  \
 (threshold == tdsrrc_TrafficVolumeThreshold_th8) ? 8 : \
  ((threshold == tdsrrc_TrafficVolumeThreshold_th16) ? 16 : \
  ((threshold == tdsrrc_TrafficVolumeThreshold_th32) ? 32 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th64) ? 64 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th128) ? 128 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th256) ? 256 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th512) ? 512 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th1024) ? 1024 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th2k) ? 2048 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th3k) ? 3072 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th4k) ? 4096 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th6k) ? 6144 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th8k) ? 8192 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th12k) ? 12288 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th16k) ? 16384 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th24k) ? 24576 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th32k) ? 32678 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th48k) ? 49152 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th64k) ? 65536 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th96k) ? 98304 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th128k) ? 131072 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th192k) ? 196608 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th256k) ? 262144 :\
  ((threshold == tdsrrc_TrafficVolumeThreshold_th384k) ? 393216 :\
  (threshold == tdsrrc_TrafficVolumeThreshold_th512k) ? 524288 : 786432 \
  ))))))))))))))))))))))) \
)


#define  TDSTVM_RET_RLC_BUFFER_PAYLOAD(payload)  \
(  \
 (payload == 0) ? tdsrrc_RLC_BuffersPayload_pl0 : \
  (((payload > 0) && (payload <= 4)) ? tdsrrc_RLC_BuffersPayload_pl4 : \
  (((payload > 4) && (payload <= 8)) ? tdsrrc_RLC_BuffersPayload_pl8 :\
  (((payload > 8) && (payload <= 16)) ? tdsrrc_RLC_BuffersPayload_pl16 :\
  (((payload > 16) && (payload <= 32)) ? tdsrrc_RLC_BuffersPayload_pl32 :\
  (((payload > 32) && (payload <= 64)) ? tdsrrc_RLC_BuffersPayload_pl64 :\
  (((payload > 64) && (payload <= 128)) ? tdsrrc_RLC_BuffersPayload_pl128 :\
  (((payload > 128) && (payload <= 256)) ? tdsrrc_RLC_BuffersPayload_pl256 :\
  (((payload > 256) && (payload <= 512)) ? tdsrrc_RLC_BuffersPayload_pl512 :\
  (((payload > 512) && (payload <= 1024)) ? tdsrrc_RLC_BuffersPayload_pl1024 :\
  (((payload > 1024) && (payload <= 2048)) ? tdsrrc_RLC_BuffersPayload_pl2k :\
  (((payload > 2048) && (payload <= 4096)) ? tdsrrc_RLC_BuffersPayload_pl4k :\
  (((payload > 4096) && (payload <= 8192)) ? tdsrrc_RLC_BuffersPayload_pl8k :\
  (((payload > 8192) && (payload <= 16384)) ? tdsrrc_RLC_BuffersPayload_pl16k :\
  (((payload > 16384) && (payload <= 32768)) ? tdsrrc_RLC_BuffersPayload_pl32k :\
  (((payload > 32768) && (payload <= 65536)) ? tdsrrc_RLC_BuffersPayload_pl64k :\
  (((payload > 65536) && (payload <= 131702)) ? tdsrrc_RLC_BuffersPayload_pl128k :\
  (((payload > 131702) && (payload <= 262144)) ? tdsrrc_RLC_BuffersPayload_pl256k :\
  ((payload > 262144) && (payload <= 524288)) ? tdsrrc_RLC_BuffersPayload_pl512 : tdsrrc_RLC_BuffersPayload_pl1024 \
  ))))))))))))))))) \
)


#define  TDSTVM_RET_AVG_BUFFER_PAYLOAD(payload)  \
(  \
 (payload == 0) ? tdsrrc_AverageRLC_BufferPayload_pla0 : \
  (((payload > 0) && (payload <= 4)) ? tdsrrc_AverageRLC_BufferPayload_pla4 : \
  (((payload > 4) && (payload <= 8)) ? tdsrrc_AverageRLC_BufferPayload_pla8 :\
  (((payload > 8) && (payload <= 16)) ? tdsrrc_AverageRLC_BufferPayload_pla16 :\
  (((payload > 16) && (payload <= 32)) ? tdsrrc_AverageRLC_BufferPayload_pla32 :\
  (((payload > 32) && (payload <= 64)) ? tdsrrc_AverageRLC_BufferPayload_pla64 :\
  (((payload > 64) && (payload <= 128)) ? tdsrrc_AverageRLC_BufferPayload_pla128 :\
  (((payload > 128) && (payload <= 256)) ? tdsrrc_AverageRLC_BufferPayload_pla256 :\
  (((payload > 256) && (payload <= 512)) ? tdsrrc_AverageRLC_BufferPayload_pla512 :\
  (((payload > 512) && (payload <= 1024)) ? tdsrrc_AverageRLC_BufferPayload_pla1024 :\
  (((payload > 1024) && (payload <= 2048)) ? tdsrrc_AverageRLC_BufferPayload_pla2k :\
  (((payload > 2048) && (payload <= 4096)) ? tdsrrc_AverageRLC_BufferPayload_pla4k :\
  (((payload > 4096) && (payload <= 8192)) ? tdsrrc_AverageRLC_BufferPayload_pla8k :\
  (((payload > 8192) && (payload <= 16384)) ? tdsrrc_AverageRLC_BufferPayload_pla16k :\
  (((payload > 16384) && (payload <= 32768)) ? tdsrrc_AverageRLC_BufferPayload_pla32k :\
  (((payload > 32768) && (payload <= 65536)) ? tdsrrc_AverageRLC_BufferPayload_pla64k :\
  (((payload > 65536) && (payload <= 13102)) ? tdsrrc_AverageRLC_BufferPayload_pla128k :\
  (((payload > 13102) && (payload <= 262144)) ? tdsrrc_AverageRLC_BufferPayload_pla256k :\
  ((payload > 262144) && (payload <= 524288)) ? tdsrrc_AverageRLC_BufferPayload_pla512 : tdsrrc_AverageRLC_BufferPayload_pla1024 \
  ))))))))))))))))) \
)



#define  TDSTVM_RET_VAR_BUFFER_PAYLOAD(payload)  \
(  \
 (payload == 0) ? tdsrrc_VarianceOfRLC_BufferPayload_plv0 : \
  (((payload > 0) && (payload <= 4)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv4 : \
  (((payload > 4) && (payload <= 8)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv8 :\
  (((payload > 8) && (payload <= 16)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv16 :\
  (((payload > 16) && (payload <= 32)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv32 :\
  (((payload > 32) && (payload <= 64)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv64 :\
  (((payload > 64) && (payload <= 128)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv128 :\
  (((payload > 128) && (payload <= 256)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv256 :\
  (((payload > 256) && (payload <= 512)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv512 :\
  (((payload > 512) && (payload <= 1024)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv1024 :\
  (((payload > 1024) && (payload <= 2048)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv2k :\
  (((payload > 2048) && (payload <= 4096)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv4k :\
  ((payload > 262144) && (payload <= 524288)) ? tdsrrc_VarianceOfRLC_BufferPayload_plv8k : tdsrrc_VarianceOfRLC_BufferPayload_plv16k \
  ))))))))))) \
)

#define  TDSTVM_RET_TIME_TO_TRIGGER(time_to_trigger)  \
(  \
 (time_to_trigger == tdsrrc_TimeToTrigger_ttt0) ? 0 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt10) ? 10 : \
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt20) ? 20 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt40) ? 40 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt60) ? 60 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt80) ? 80 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt100) ? 100 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt120) ? 120 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt160) ? 160 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt200) ? 200 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt240) ? 240 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_tt320) ? 320 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt640) ? 640 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt1280) ? 1280 :\
  ((time_to_trigger == tdsrrc_TimeToTrigger_ttt2560) ? 2560 : 5000\
  )))))))))))))) \
)

#define  TDSTVM_RET_PENDING_TIME_AFTER_TRIGGER(pending_time_after_trigger)  \
(  \
 (pending_time_after_trigger == tdsrrc_PendingTimeAfterTrigger_ptat0_25) ? 250 : \
  ((pending_time_after_trigger == tdsrrc_PendingTimeAfterTrigger_ptat0_5) ? 500 : \
  ((pending_time_after_trigger == tdsrrc_PendingTimeAfterTrigger_ptat1) ? 1000 :\
  ((pending_time_after_trigger == tdsrrc_PendingTimeAfterTrigger_ptat2) ? 2000 :\
  ((pending_time_after_trigger == tdsrrc_PendingTimeAfterTrigger_ptat4) ? 4000 :\
  ((pending_time_after_trigger == tdsrrc_PendingTimeAfterTrigger_ptat8) ? 8000 : 16000 \
  ))))) \
)


#define  TDSTVM_RET_INTPT_TIME_AFTER_TRIGGER(intpt_time_after_trigger)  \
(  \
 (intpt_time_after_trigger == tdsrrc_TX_InterruptionAfterTrigger_txiat0_25) ? 250 : \
  ((intpt_time_after_trigger == tdsrrc_TX_InterruptionAfterTrigger_txiat0_5) ? 500 : \
  ((intpt_time_after_trigger == tdsrrc_TX_InterruptionAfterTrigger_txiat1) ? 1000 :\
  ((intpt_time_after_trigger == tdsrrc_TX_InterruptionAfterTrigger_txiat2) ? 2000 :\
  ((intpt_time_after_trigger == tdsrrc_TX_InterruptionAfterTrigger_txiat4) ? 4000 :\
  ((intpt_time_after_trigger == tdsrrc_TX_InterruptionAfterTrigger_txiat8) ? 8000 : 16000 \
  ))))) \
)




#define TDSTVM_RET_REPORTING_MODE(mode) \
(\
(mode == tdsrrc_TransferMode_acknowledgedModeRLC) ? TDSACK_MODE : TDSUNACK_MODE \
)



#define TDSTVM_RET_TRCH_TYPE(channel) \
(\
  (channel == TDSDCH) ? T_tdsrrc_UL_TrCH_Identity_dch : \
   ((channel == TDSRACH) ? T_tdsrrc_UL_TrCH_Identity_rachorcpch : T_tdsrrc_UL_TrCH_Identity_usch \
  ) \
)


/* Used for storing Reporting Criteria in RRC */
#define TDSMEAS_RPT_CRIT(mode) \
(  \
 (mode == TDSL1_PERIODIC_RPT) ? TDSPERIODIC : \
  ((mode == TDSL1_EVENT_TRIGGER_RPT) ? TDSEVENT : \
  ((mode == TDSL1_NO_RPT) ? TDSADDTL : TDSNOT_AVAILABLE \
  ) \
 ) \
)

/* Default Traffic Vol Meas Identity  */
#define TDSDEFAULT_TRAFFIC_VOL_MEAS_IDENITY  4
#define TDSQUANTITY_NOT_PRESENT 0

#define TDSMAX_TRAFFIC_VOLUME_MEASUREMENTS 4



#define  TDSTVM_RET_MEAS_VALIDITY(validity)  \
(  \
 (validity == tdsrrc_MeasurementValidity_ue_State_all_States) ? TDSALL_STATES  : \
  ((validity == tdsrrc_MeasurementValidity_ue_State_all_But_Cell_DCH) ? TDSALL_STATES_BUT_DCH : TDSSTATE_DCH \
  ) \
)


/* Used for storing TVM Reporting Criteria in RRC */
#define TDSTVM_RPT_CRIT(mode) \
(  \
 (mode == TDSNO_REPORTING) ? TDSADDTL : \
  ((mode == TDSPERIODIC_MODE) ? TDSPERIODIC : \
  ((mode == TDSEVENT_TRIGGERED_MODE) ? TDSEVENT : TDSNOT_AVAILABLE \
  ) \
 ) \
)

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)


/* Used for storing Reporting Criteria in RRC */
#define TDSPOS_RPT_CRIT(mode) \
(  \
 (mode == TDSRRC_PERIODIC_RPT) ? TDSPERIODIC : \
  ((mode == TDSRRC_EVENT_TRIGGER_RPT) ? TDSEVENT : TDSADDTL ) \
)

#endif

/* Converts hcs tmp offset 1 for FDD from ASN1 format to L1 format */
#define  TDSRRCMEAS_RET_HCS_TMP_OFFSET1(t_off1)  \
(  \
 (t_off1 == tdsrrc_TemporaryOffset1_to3) ? 3 : \
  ((t_off1 == tdsrrc_TemporaryOffset1_to6) ? 6 : \
  ((t_off1 == tdsrrc_TemporaryOffset1_to9) ? 9 :\
  ((t_off1 == tdsrrc_TemporaryOffset1_to12) ? 12 :\
  ((t_off1 == tdsrrc_TemporaryOffset1_to15) ? 15 :\
  ((t_off1 == tdsrrc_TemporaryOffset1_to18) ? 18 :\
  (t_off1 == tdsrrc_TemporaryOffset1_to21) ? 21 : 50 \
  ))))) \
)

/* Converts hcs tmp offset 2 for FDD from ASN1 format to L1 format */
#define  TDSRRCMEAS_RET_HCS_TMP_OFFSET2(t_off2)  \
(  \
 (t_off2 ==  tdsrrc_TemporaryOffset2_to2) ? 2 : \
  ((t_off2 == tdsrrc_TemporaryOffset2_to3) ? 3 : \
  ((t_off2 == tdsrrc_TemporaryOffset2_to4) ? 4 :\
  ((t_off2 == tdsrrc_TemporaryOffset2_to6) ? 6 :\
  ((t_off2 == tdsrrc_TemporaryOffset2_to8) ? 8 :\
  ((t_off2 == tdsrrc_TemporaryOffset2_to10) ? 10 :\
  (t_off2 == tdsrrc_TemporaryOffset2_to12) ? 12 : 50 \
  ))))) \
)

/* INTRA_FREQ_CELL_INFO_LIST variable that stores the cell info */
typedef struct
{
  uint16 cpi;
  tdscell_position_e_type cell_position;  /* Vacant or Occupied */
} tdsintra_freq_cell_list_struct_type;


typedef struct
{
  uint16 cell_position;
  uint16 cpi;
} tdscells_for_meas_struct_type;



/* MEASUREMENT_IDENTITY variable. Required to trash measurements*/
typedef struct
{
  uint16 meas_id;
  tdsl1_meas_type_enum_type meas_type;
  tdsmeas_validity_enum_type meas_validity;
  tdsrpt_crit_enum_type rpt_crit;
  /* If MCM owns this measurement */
  boolean mcm_owned;
  boolean meas_can_be_reported;

  uint8 additional_meas[TDSMAX_ADDITIONAL_MEAS];

} tdsintra_freq_meas_id_list_struct_type;



/* INTER_RAT_CELL_INFO_LIST variable that stores the cell info */
typedef struct
{
  uint16 cell_id;
  tdscell_position_e_type cell_position;  /* Vacant or Occupied */
  uint16 arfcn;
} tdsinter_rat_cell_list_struct_type;

typedef enum
{
  TDSRRCMEAS_IRAT_GSM
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
  ,TDSRRCMEAS_IRAT_EUTRA
#endif
} rrcmeas_interrat_type;

/* INTER_RAT_MEAS_ID_LIST VARIABLE */
typedef struct
{
  uint16 meas_id;
  tdsmeas_validity_enum_type meas_validity;
  tdsrpt_crit_enum_type rpt_crit;
  uint8 additional_meas[TDSMAX_ADDITIONAL_MEAS];
  

  /*InterRAT Type, If it is GSM/EUTRA etc*/
  rrcmeas_interrat_type interrat_type;

  boolean rpt_bsic_verified;
} tdsinter_rat_meas_id_list_struct_type;



#define TDSMAX_QUALITY_MEAS 2

/* QUALITY_MEAS_ID_LIST VARIABLE */
typedef struct
{
  uint16 meas_id;
  tdsmeas_validity_enum_type meas_validity;
  tdsrpt_crit_enum_type rpt_crit;
  uint8 additional_meas[TDSMAX_ADDITIONAL_MEAS];
} tdsquality_meas_id_list_struct_type;


/*-------------------------------------------------------------------
STRUCTURE: tdstvm_meas_id_list_struct_type
Stores the Traffic Volume Meas Id's
--------------------------------------------------------------------*/
/* TRAFFIC VOLUME MEASUREMENT_IDENTITY variable. Required to trash measurements*/
typedef struct
{
  uint16 meas_id;
  tdsmeas_validity_enum_type meas_validity;
  boolean meas_ctrl_msg;
  boolean block_rach_incl;
  tdsrpt_crit_enum_type rpt_crit;
  uint8 additional_meas[TDSMAX_ADDITIONAL_MEAS];
} tdstvm_meas_id_list_struct_type;





/* INTER_FREQ_MEAS_ID_LIST VARIABLE */
typedef struct
{
  uint16 meas_id;
  tdsmeas_validity_enum_type meas_validity;
  tdsrpt_crit_enum_type rpt_crit;
  /* If MCM owns this measurement */
  boolean mcm_owned;
  uint8 additional_meas[TDSMAX_ADDITIONAL_MEAS];
} tdsinter_freq_meas_id_list_struct_type;


#define TDS_ARFCN 16383
#define TDSINVALID_INTER_FREQ 25000

/*
Dl Freq 2110 - 2170 MHz, then the default duplex distance is 190 MHz;
DL frequency 1930 - 1990 MHz,then the default duplex distance is 80 MHz;
*/

#define TDSDL_BAND1_MIN 1930
#define TDSDL_BAND1_MAX 1990
#define TDSDL_BAND2_MIN 2110
#define TDSDL_BAND2_MAX 2170


/* Used for storing Reporting Criteria in RRC */
#define TDSRET_INTER_FREQ_RPT_CRIT(mode) \
(  \
 (mode == TDSL1_INTER_FREQ_PERIODIC) ? TDSPERIODIC : \
  ((mode == TDSL1_INTER_FREQ_EVT_TRIG) ? TDSEVENT : \
  ((mode == TDSL1_INTRA_FREQ_EVT_TRIG) ? TDSEVENT : TDSADDTL \
  ) \
 ) \
)

/* Inter Freq CELL_INFO_LIST */
typedef struct
{
  tdscell_position_e_type cell_position;
  uint16 cpi;
  uint16 dl_freq;
  /* If no IE "Intra-SecondaryFrequencyIndicator", will set FALSE*/
  boolean sec_freq_ind_incl;
  /*10.3.7.13, REL7 add new IE "Intra-SecondaryFrequencyIndicator".
    TRUE means the inter frequencyneighbour cell has a secondary frequency
    which is the same as the current working frequency of UE. Before REL7
    it always FALSE.*/
  boolean sec_freq_ind;
} tdsinter_freq_cell_list_struct_type;


typedef struct /* for each removed cell   */
{
  /* Freq and Scr code to remove */
  uint16    freq;
  uint16    cpi;
} tdsinter_freq_removed_cell_struct_type;


/* Max No of Internal Measurements */
#define TDSMAX_INTERNAL_MEAS 8

/* UEIM_MEAS_ID_LIST VARIABLE */
typedef struct
{
  uint16 meas_id;
  tdsmeas_validity_enum_type meas_validity;
  tdsrpt_crit_enum_type rpt_crit;
  uint8 additional_meas[TDSMAX_ADDITIONAL_MEAS];
} tdsueim_meas_id_list_struct_type;

typedef struct
{
  boolean interRAT_cell_info_present;
  uint8 interRAT_cell_info_ind;
} tdsinterRAT_cell_info_status_type;


#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)

/* Positioning MEAS_ID_LIST VARIABLE */
typedef struct
{
  uint8 meas_id;
  tdsmeas_validity_enum_type meas_validity;
  tdsrpt_crit_enum_type rpt_crit;
  tdsrrc_positioning_method_enum_type pos_method_type;
  uint8 additional_meas[TDSMAX_ADDITIONAL_MEAS];
} tdspos_meas_id_list_struct_type;

/* Declared to find out the owner of positioning module */
typedef struct
{
  uint8 meas_id;
  tdsrrc_positioning_method_enum_type pos_method_type;
  tdsrrc_meas_cmd_enum_type pos_meas_cmd;
} tdspos_info_struct_type;

#define  TDSRET_POS_METHOD(pos_method)  \
(  \
 (pos_method == tdsrrc_PositioningMethod_otdoa) ? TDSRRC_OTDOA : \
 ((pos_method == tdsrrc_PositioningMethod_gps) ? TDSRRC_GPS : \
 ((pos_method == tdsrrc_PositioningMethod_otdoaOrGPS) ? TDSRRC_OTDOAorGPS : \
  TDSRRC_CELLID  \
 )) \
)


#define  TDSRET_POS_METHOD_TYPE(pos_method_type)  \
(  \
 (pos_method_type == tdsrrc_UE_Positioning_MethodType_ue_Assisted) ? TDSRRC_UE_ASSISTED : \
 ((pos_method_type == tdsrrc_UE_Positioning_MethodType_ue_Based) ? TDSRRC_UE_BASED : \
 ((pos_method_type == tdsrrc_UE_Positioning_MethodType_ue_BasedPreferred) ? TDSRRC_UE_BASED_PREFERED : \
  TDSRRC_UE_ASSISTED_PREFERED  \
 )) \
)

#define  TDSRET_POS_RPT_TYPE(pos_rpt_type)  \
( \
  (pos_rpt_type == tdsrrc_TransferMode_acknowledgedModeRLC) ? TDSMODE_ACKNOWLEDGED : TDSMODE_UNACKNOWLEDGED \
)

#define TDSRET_POS_ERR(pos_error)  \
(  \
 (pos_error == TDSRRC_NOT_ENOUGH_OTDOA_CELLS) ? rrc_notEnoughOTDOA_Cells : \
 ((pos_error == TDSRRC_NOT_ENOUGH_GPS_SATELLITES) ? rrc_notEnoughGPS_Satellites : \
 ((pos_error == TDSRRC_ASSISTANCE_DATA_MISSING) ? rrc_assistanceDataMissing : \
 ((pos_error == TDSRRC_METHOD_NOT_SUPPORTED) ? rrc_notAccomplishedGPS_TimingOfCellFrames : \
 ((pos_error == TDSRRC_UNDEFINED_ERROR) ? rrc_undefinedError : \
 ((pos_error == TDSRRC_REQUEST_DENIED_BY_USER) ? rrc_requestDeniedByUser : \
 ((pos_error == TDSRRC_NOT_PROCESSED_AND_TIMEOUT) ? rrc_notProcessedAndTimeout : \
  rrc_referenceCellNotServingCell \
 )))))) \
)

/*
#define  RET_GET_POS_METHOD(meas_id)  \
((tdspos_meas_id_list[0] == meas_id) ? tdspos_meas_id_list[0].pos_method_type: \
tdspos_meas_id_list[1].pos_method_type)
*/


#endif
#ifdef  FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
typedef struct 
{
  boolean inter_freq_rach_info_incl;/* If yes, next field is valid */ 
  tdsl1_meas_rach_interf_struct_type interf_rach_rpt_inf;
} tdsl1_inter_freq_rach_rpt_type;
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/

#ifdef FEATURE_LTE_TO_TDSCDMA
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_DEPRIORITIZATION
typedef struct
{
  boolean is_deprio_db_valid;
  /*! Indicates if all EUTRA neighbor freqs should be deprioritized */
  boolean  depri_all_freq;
 
  /*! Number of freqs being deprioritized; Will be set to a valid value
    only if the depri_all_freq boolean is set to FALSE
    If num_depri_freq = 0 and depri_all_freq = FALSE, this implies
    LTE deprioritization is cleared*/
  uint8   num_depri_freq;
  
  /*! Set of deprioritized EUTRA frequencies. This will be valid only
    if the depri_all_freq boolean is set to FALSE */
  uint32 depri_freqs[TDSCDMA_RRC_MAX_LTE_DEPRI_FREQ];
}tdsrrc_deprio_db_type;
#endif
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
typedef struct
{
  uint32 earfcn;
}rrcirat_eutra_freq_type;

#define RRCMEAS_GET_EUTRA_MEAS_BANDWIDTH(bandwidth)\
    ((bandwidth == rrc_EUTRA_MeasurementBandwidth_mbw6) ? l1_mbw6 :\
      ((bandwidth == rrc_EUTRA_MeasurementBandwidth_mbw15) ? l1_mbw15 :\
        ((bandwidth == rrc_EUTRA_MeasurementBandwidth_mbw25) ? l1_mbw25: \
          ((bandwidth == rrc_EUTRA_MeasurementBandwidth_mbw50) ? l1_mbw50: \
            (bandwidth == rrc_EUTRA_MeasurementBandwidth_mbw75) ? l1_mbw75 : l1_mbw100\
            )\
          )\
        )\
      )\


#endif
/*===================================================================
                      DATA STRUCTURES Type
====================================================================*/


/*===========================================================================
                      FUNCTION DECLARATIONS
===========================================================================*/


/*===========================================================================

FUNCTION     RRCMEAS_PROCESS_INITIAL_STATE

DESCRIPTION

  Processes TDSRRC_CELL_SELECTED_MEAS_REQ received in
  TDSRRCMEAS_INITIAL state of RRC Measurements procedure.
  TDSRRC_CELL_SELECTED_MEAS_REQ from cell selection procedure
  would come in this state which would cause RRC Measurement
  to transition from RRCMEAS INITIAL state to RRCMEAS IDLE
  state and give indication to L1 to start meas based on SIB11.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcmeas_process_initial_state
(
tdsrrc_cmd_type*  cmd_ptr
);


/*===========================================================================

FUNCTION     RRCMEAS_PROCESS_IDLE_STATE

DESCRIPTION

  Processes all the messages received in TDSRRCMEAS_IDLE state
  of RRC Measurements procedure.

DEPENDENCIES

    None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcmeas_process_idle_state
(
tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_CELL_DCH_STATE

DESCRIPTION

  Processes all events that are dispatched to RRC Measurement
  Establishment procedure in RRCMEAS DCH state.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_cell_dch_state
(
tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION     RRCMEAS_PROCESS_CELL_FACH_STATE

DESCRIPTION

  Processes all the messages received in RRCMEAS FACH state
  of RRC Measurements procedure.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_cell_fach_state
(
tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION    RRCMEAS_PROCESS_PCH_STATE

DESCRIPTION

  Processes all the messages received in RRCMEAS PCH state
  of RRC Measurements procedure.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_pch_state
(
tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION tdsrrcmeas_read_sib11

DESCRIPTION

  Reads SIB11 from the current active serving cell for all the measurements.
  If SIB11 information is incorrect, meas setup command is not send to L1/MAC
  and UE does not do any measurements..

DEPENDENCIES

  SIB must have been stored by the SIB procedure

RETURN VALUE

  Success if SIB11 has been correctly read
  Failure if SIB11 has not been correctly read

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_read_sib11
(
void
);


/*===========================================================================

FUNCTION tdsrrcmeas_read_sib12

DESCRIPTION

  Caller must check the tdssib12_present before calling this fn.
  At present, this fn is only called on state transition
  from Idle to connected mode (CELL_DCH). It build up the
  new intra freq cell list from SIB11 cell list stored
  in tdsrrcmeas_sib11 struct and from the cell info
  given. L1 would perform meas based on the new constructed cell
  list

DEPENDENCIES

  SIB12 must have been received by the camping cell.

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void  tdsrrcmeas_read_sib12
(
  tdsrrc_SysInfoType12 *sib12_ptr,
  tdsl1_meas_sib_parm_struct_type* rrcmeas_sib12_ptr
);

/*===========================================================================

FUNCTION          RRCMEAS_SEND_L1_CMD

DESCRIPTION

  This function sends measurement setup requests to L1.
  Since all L1 cmds go via LLC, the LLC command
  TDSRRC_LLC_CMD_REQ actaully encapsules the real L1
  command. This is the LLC command funneling mechanism.
  It frees allocated command and embedded buffers when failure
  is detected before sending the command.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcmeas_send_l1_cmd
(
tdsl1_meas_sib_parm_struct_type* rrcmeas_sib_ptr,
tdsl1_meas_ctrl_parm_struct_type* rrcmeas_mcm_ptr,
tdsl1_meas_trans_type* rrcmeas_trans_ptr,
tdsl1_meas_ctrl_enum_type  meas_choice
);

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*===========================================================================

FUNCTION          RRCMEAS_SEND_DEFERRED_MEAS_REQ

DESCRIPTION

 This function sends TDSL1_CPHY_MEASUREMENT_REQ to L1 with parm type as
 L1_DEFERRED_MEASUREMENT_FROM_SIB. This parm type is chosen when 
 SIB11 hasn't been read. Depending on the state transition, meas identity 
 and cell info list are to be cleared. Also, meas quan should be chosen appropriately
 depending on whether SIB3 has been read or not.
 
 Since all L1 cmds go via LLC, the LLC command
 TDSRRC_LLC_CMD_REQ actaully encapsules the real L1
 command. This is the LLC command funneling mechanism.
 It frees allocated command and embedded buffers when failure
 is detected before sending the command.

DEPENDENCIES

 None.

RETURN VALUE

 None.

SIDE EFFECTS

 None.

===========================================================================*/

void tdsrrcmeas_send_deferred_meas_req
(
  uint8  l1_deferred_meas_from_sib_action
);
#endif

/*===========================================================================

FUNCTION     RRCMEAS_SEND_INTRA_FREQ_MEAS_REPORT_TO_UTRAN

DESCRIPTION

  Converts the intra-freq measurement report received
  From L1 into the measurement report structure format
  expected by UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  void

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_send_intra_freq_meas_report_to_utran
(
  tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
);


/*===========================================================================

FUNCTION  RRCMEAS_PROCESS_CELL_SELECTED_MEAS_REQ_FOR_IDLE_STATE

DESCRIPTION

  Processes new cell req from CSP in RRCMEAS Idle state.
  Reads new SIB paramteres and sends new measurement
  request to L1 based on the new SIB's. L1 on
  receiving new SIB's deletes meas based on old SIB's

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_cell_selected_meas_req_for_idle_state
(
void
);



/*===========================================================================

FUNCTION    RRCMEAS_PROCESS_STATE_CHANGE_IND_IN_IDLE_STATE

DESCRIPTION

  Processes state change indication received in idle mode.

DEPENDENCIES

  None.

RETURN VALUE

  void

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_state_change_ind_in_idle_state
(
tdsrrc_cmd_type* cmd_ptr
);


/*===========================================================================

FUNCTION     RRCMEAS_PROCESS_STATE_CHANGE_IND_IN_DCH_STATE

DESCRIPTION

  Processes state change indication message received in CELL_DCH
  Also changes the RRCMEAS procedure state from DCH to the
  desired state.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_state_change_ind_in_dch_state
(
tdsrrc_cmd_type* cmd_ptr
);


/*===========================================================================

FUNCTION    RRCMEAS_PROCESS_CPHY_MEAS_IND_IN_DCH_STATE

DESCRIPTION

  Processes measurement indication received from L1
  in CELL_DCH state.

DEPENDENCIES

  None.

RETURN VALUE

  void

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_cphy_meas_ind_in_dch_state
(
tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION    RRCMEAS_PROCESS_DOWNLINK_L2_ACK

DESCRIPTION

  Processes ack received from layer2 of the meas report transmitted to UTRAN.

DEPENDENCIES

  None.

RETURN VALUE

  void

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_downlink_l2_ack
(
tdsrrc_cmd_type *cmd_ptr
);



/*===========================================================================

FUNCTION  RRCMEAS_FILL_RSCP_CELL_INFO

DESCRIPTION

  This function fills in Cell Info for a Non-HCS cell when
  Cell Selection Reselection Quality Measure is RSCP.
  The function processes Cell Info in ASN1 format and
  translates into the L1 RRC Interface format.

DEPENDENCIES

  Pointer to tdsrrc_CellInfoSI_RSCP must be valid.

RETURN VALUE

  FAILURE: If passed pointer is NULL.
  SUCCESS: If able to analyze and fill Cell Info.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_rscp_cell_info
(
tdsrrc_CellInfoSI_RSCP_LCR_r4 *sib_cell_info,
tdsl1_cell_info_struct_type *l1_cell_info
,tdsrrc_CellSelectReselectInfo_v590ext * rel5_ext_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_FILL_ECN0_CELL_INFO

DESCRIPTION

  This function fills in Cell Info for a Non-HCS cell when
  Cell Selection Reselection Quality Measure is ECN0.
  The function processes Cell Info in ASN1 format and
  translates into the L1 RRC Interface format.

DEPENDENCIES

  Pointer to tdsrrc_CellInfoSI_ECN0 must be valid.

RETURN VALUE

  FAILURE: If passed pointer is NULL.
  SUCCESS: If able to analyze and fill Cell Info.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_ecn0_cell_info
(
tdsrrc_CellInfoSI_ECN0_LCR_r4  *sib_cell_info,
tdsl1_cell_info_struct_type *l1_cell_info
,tdsrrc_CellSelectReselectInfo_v590ext * rel5_ext_ptr
);


/*===========================================================================

FUNCTION          RRCMEAS_FILL_INTRA_FREQ_EVENT_CRITERIA

DESCRIPTION

  This function fills in intra-freq event criteria for a Non-HCS cell when
  Cell Selection Reselection Quality Measure is RCSP.
  The function processes intra-ftreq event criteria Info in ASN1 format
  and translates into the L1 RRC Interface format.

DEPENDENCIES

  The passed pointers must be valid.

RETURN VALUE

  No of events.

SIDE EFFECTS

  None

===========================================================================*/

uint16 tdsrrcmeas_fill_intra_freq_event_criteria
(
tdsrrc_IntraFreqReportingCriteria_LCR_r4*rrc_intra_freq_reporting_criteria_ptr,
tdsl1_intra_freq_event_crit_struct_type *l1_intra_freq_event_crit_ptr
, tdsrrc_MeasurementControl_v590ext_IEs *rel5_ext_ptr
, tdsrrc_SysInfoType11_v590ext_IEs *sib11_rel5_ext_ptr
, tdsrrc_SysInfoType12_v590ext_IEs *sib12_rel5_ext_ptr
);


/*===========================================================================

FUNCTION      RRCMEAS_FILL_NO_OF_REPORTING_CELLS

DESCRIPTION
  This function fills no of reporting cells for a Non-HCS cell
  for an event triggered criteria for RCSP or ECNO.
  The function processes this Info in ASN1 format and
  translates into the L1 RRC Interface format.

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If unable to fill no of reporting cells correctly
  SUCCESS: If no of reporting cells filled correctly

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_no_of_reporting_cells
(
tdsrrc_ReportingCellStatus  *rrc_rpt_cell_status_ptr,
tdsl1_meas_rpt_cell_status_struct_type  *l1_rpt_cell_status_ptr
);


/*===========================================================================

FUNCTION          RRCMEAS_FILL_DCH_STATE_INFO

DESCRIPTION

  This function fills DCH state info for a Non-HCS cell
  for RCSP/ECNO. The function processes this Info in ASN1 format
  and translates into the L1 RRC Interface format.

DEPENDENCIES

  None.

RETURN VALUE

  Success if DCH state info filled correctly
  Failure if unable to fill DCH state info correctly

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_dch_state_info
(
tdsrrc_ReportingInfoForCellDCH_LCR_r4  * rrc_reporting_info_for_cell_dch_ptr,
tdsl1_intra_freq_dch_rpt_info_struct_type* l1_dch_rpt_info_ptr
, tdsrrc_SysInfoType11_v590ext_IEs *sib11_rel5_ext_ptr
, tdsrrc_SysInfoType12_v590ext_IEs *sib12_rel5_ext_ptr
);


/*===========================================================================

FUNCTION     RRCMEAS_PROCESS_REMOVED_INTRA_FREQ_CELLS

DESCRIPTION

 This function fills the removed intra-freq cells for HCS Non-HCS cells
 for RSCP and ECN0. The function processes this Info in ASN1 format and
  translates into the L1 RRC Interface format.

DEPENDENCIES

  None

RETURN VALUE

 FAILURE: If unable to fill removed intra freq cell info correctly
  SUCCESS: If removed intra freq cell info filled correctly

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_process_removed_intra_freq_cells
(
tdsrrc_RemovedIntraFreqCellList *removed_intra_freq_cell_list_ptr,
tdsl1_intra_freq_cell_list_struct_type *l1_intra_freq_cell_ptr
);


/*===========================================================================

FUNCTION    RRCMEAS_SET_INTRA_FREQ_EVENT_ID

DESCRIPTION

  Sets the event id in measurement report from L1 format to ASN1 format

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE if event no is incorrect
  SUCCESS if event no is correct

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_set_intra_freq_event_id
(
uint16 tdsevent_id,
tdsrrc_EventIDIntraFreq* rrcmeas_intra_freq_id_results_ptr
);

/*===========================================================================

FUNCTION    CHECK_FOR_RACH_REPORTING

DESCRIPTION

  Determines if RACH report can be appended to RACH messages.
  The calling function should call rrc_meas_append_meas_rpt_on_rach
  only it has received TDSAPPEND_RRC_MEAS_RACH_REPORT value.

DEPENDENCIES

  None.

RETURN VALUE

  TDSAPPEND_RRC_MEAS_RACH_REPORT if RACH report can be appended
  TDSNO_RRC_MEAS_RACH_REPORT if RACH report cannot be appended

SIDE EFFECTS

  None

===========================================================================*/

tdsrach_report_crit_e_type tdsrrcmeas_check_for_rach_reporting
(
void
);


/*===========================================================================

FUNCTION    RRCMEAS_CLEAR_SIB11

DESCRIPTION

  Initialises SIB11 stored variable.

DEPENDENCIES

  None.

RETURN VALUE

  void

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_clear_sib11
(
void
);


/*===========================================================================

FUNCTION    RRCMEAS_BUILD_SIB12_INTRA_FREQ_LIST_FROM_SIB11

DESCRIPTION

  Builds neighbouring cell list from SIB11 for SIB12.

DEPENDENCIES

  None.

RETURN VALUE

  void

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_build_sib12_intra_freq_list_from_sib11
(
tdsrrc_RemovedIntraFreqCellList *removedIntraFreqCellList,
tdsl1_intra_freq_cell_list_struct_type* sib11_cell_list_ptr,
tdsl1_intra_freq_cell_list_struct_type* sib12_cell_list_ptr,
tdsintra_freq_cell_list_struct_type  *tmp_intra_freq_cell_list
);

/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_INTRA_FREQ_PERIODIC_MEAS_RESULTS

DESCRIPTION

  Processes intra-freq periodic measurements and appends
  to Measured Results in Measurement Report

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcmeas_append_intra_freq_periodic_meas_results
(
tdsl1_intra_freq_meas_rpt_struct_type* l1_intra_freq_periodic_meas_ptr,
tdsrrc_MeasurementReport *meas_rpt_ptr
,tdsrrc_IntraFreqMeasuredResultsList **rrc_intra_freq_meas_results_ptr
, boolean addtl_meas
);


/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_MEAS_CTRL_MSG

DESCRIPTION

  Processes meas control message in DCH state and directs L1 to setup/modify/
  release measurements accordingly. If the message determined is incorrect,
  send a measurement control failure message to UTRAN

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcmeas_process_meas_ctrl_msg
(
tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_READ_MCM_MEASUREMENTS

DESCRIPTION

  Reads different measurements, validate the paramters of each meas and sends
  a measurement control message to L1 to do meas based on new parameters.
  Send a measurement control failure message to UTRAN on finding an incorrect/
  unsupported measurement

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcmeas_read_mcm_measurements
(
tdsrrc_MeasurementControl_r3_IEs* mcm_ptr
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
, void* decoded_msg_ptr
#endif
,tdsrrc_DL_DCCH_Message* dcch_msg_ptr
,uint8 meas_ctrl_tr_id
);


/*===========================================================================

FUNCTION  RRCMEAS_FILL_MCM_CELL_INFO

DESCRIPTION

  This function fills in Cell Info received from a
  Measurement Control Message. The function processes
  Cell Info in ASN1 format and translates into the L1
  RRC Interface format.

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If unable to fill cell info correctly
  SUCCESS: If cell info is filled correctly

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_mcm_cell_info
(
tdsrrc_CellInfo* mcm_cell_info_ptr,
tdsl1_cell_info_struct_type* l1_cell_info_ptr
);

/*===========================================================================

FUNCTION  RRCMEAS_FILL_REFERENCE_TIME_DIFFERENCE

DESCRIPTION

  This function fills in Cell Info received from a
  Measurement Control Message. The function processes
  Cell Info in ASN1 format and translates into the L1
  RRC Interface format.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcmeas_fill_reference_time_difference
(
tdsrrc_ReferenceTimeDifferenceToCell* time_diff_ptr,
tdsl1_cell_info_struct_type* l1_cell_ptr
);


/*===========================================================================

FUNCTION  RRCMEAS_FILL_COMMON_MCM_INTRA_F_INFO

DESCRIPTION

  This function fills in common Cell Info for meas setup and
  meas modify command type in a Measurement Control Message.
  The function processes Cell Info in ASN1 format and translates
  into the L1RRC Interface format.

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If unable to fill common mcm info correctly
  SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

  None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_fill_common_mcm_intra_f_info
(
tdsrrc_IntraFrequencyMeasurement* intra_f_meas_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_f_mcm_ptr
, tdsrrc_MeasurementControl_v590ext_IEs *rel5_ext_ptr
);


/*===========================================================================

FUNCTION          RRCMEAS_FILL_INTRA_FREQ_REP_QUAN

DESCRIPTION

  Fills intra freq reporting info obtained from MCM.
  The function processes this Info in ASN1 format and translates
  into the L1 RRC Interface format.

DEPENDENCIES

  None.

RETURN VALUE

  Success if intra freq reporting quantity filled correctly
  Failure if unable to fill intra freq reporting quantity

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_intra_freq_rep_quan
(
tdsrrc_IntraFreqReportingQuantity* rrc_intra_freq_reporting_quan_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_freq_mcm_ptr
);


/*===========================================================================

FUNCTION     RRCMEAS_SEND_MEAS_CTRL_FAILURE_MSG

DESCRIPTION

  Sends meas control failure message to UTRAN

DEPENDENCIES

  None.

RETURN VALUE

  void


SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcmeas_send_meas_ctrl_failure_msg
(
tdsrrcmeas_mcf_struct_type* mcf_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_L1_ACK

DESCRIPTION

  Processes meas control message in DCH state and directs L1 to setup/modify/
  release measurements accordingly. If the message determined is incorrect,
  send a measurement control failure message to UTRAN

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_l1_ack
(
tdsrrc_cmd_type* cmd_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_FIND_CPI

DESCRIPTION

  Finds the primary scrambling code of the intra freq cell id

DEPENDENCIES

  None

RETURN VALUE

  TDSCPI_ABSENT if cpi absent
  TDSCPI_PRESENT if cpi present

SIDE EFFECTS

  None
===========================================================================*/

tdscpi_e_type tdsrrcmeas_find_cpi
(
uint32* intra_freq_cell_id_ptr,
uint16* cpi_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_UPDATE_INTRA_FREQ_CELL_LIST

DESCRIPTION

  Updates the intra freq cell list with new values of intra freq
  cell id. This is done on receving SIB system info or on
  receiving Meas Ctrl Message. The calling function gives
  position_no_ptr indicating where to store the cpi. If it
  points to TDSAVAILABLE_CELL_POSITION, then store cpi in the first
  available position in intra_freq_cell_list and and mark that
  position as occupied. If position_no_ptr points to some other
  value then store cpi at the position given by that value


DEPENDENCIES

  None

RETURN VALUE

  RRCMEAS_FAILURE if intra freq cell list cannot be updated due to incorrect
  value of the intra freq cell id
  TDSRRCMEAS_SUCCESS if intra freq cell list can be successfully updated

SIDE EFFECTS

  None
===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_update_intra_freq_cell_list
(
uint32* position_no_ptr,
uint16* cpi_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_REMOVE_CELLID

DESCRIPTION

  Delete intra-freq cell id from the intra freq cell list

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrcmeas_remove_cellid
(
uint32* intra_freq_cell_id_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_UPDATE_INTRA_FREQ_CELL_LIST_MCM

DESCRIPTION

  Updates the intra freq cell list with new values of intra freq
  cell id. This is done on receving Meas Ctrl Message. The calling
  function givesposition_no_ptr indicating where to store the cpi. If it
  points to TDSAVAILABLE_CELL_POSITION, then store cpi in the first
  available position in intra_freq_cell_list and and mark that
  position as occupied. If position_no_ptr points to some other
  value then store cpi at the position given by that value

DEPENDENCIES

  None

RETURN VALUE

  RRCMEAS_FAILURE if intra freq cell list cannot be updated due to incorrect
  value of the intra freq cell id
  TDSRRCMEAS_SUCCESS if intra freq cell list can be successfully updated

SIDE EFFECTS

  None
===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_update_intra_freq_cell_list_mcm
(
uint32* position_no_ptr,
uint16* cpi_ptr,
uint16* cpi_to_remove_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_SET_INTRA_FREQ_MEAS_IDENTITY

DESCRIPTION

  Set intra freq meas identity variable from SIB information.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrcmeas_set_intra_freq_meas_identity
(
tdsl1_intra_freq_sib_struct_type* l1_intra_freq_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_RETRIEVE_MEAS_OWNER

DESCRIPTION

  Finds the owner of measurement identity given in a MCM.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

tdsrrcmeas_layer_e_type tdsrrcmeas_retrieve_meas_owner
(
 tdsrrc_MeasurementCommand* mcm_ptr,
 uint32* meas_id_ptr
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
 ,tdspos_info_struct_type* pos_info_ptr
#endif
);



/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO

DESCRIPTION

    Process traffic volume information in a Meas
    Ctrl Message. Also sends a Meas Ctrl Failure
    Message in case of an error


DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/

tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info
(
  tdsrrc_MeasurementControl_r3_IEs* rrc_mcm_ptr
, uint32 meas_ctrl_tr_id
);


/*===========================================================================

FUNCTION  RRCMEAS_PROCESS_MAC_ACK

DESCRIPTION

  Processes ack from MAC

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrcmeas_process_mac_ack
(
 tdsrrc_cmd_type* cmd_ptr
);



/*===========================================================================

FUNCTION     RRCTVM_PROCESS_SIB_INFO

DESCRIPTION

  Process traffic volume information from SIB11/12.
  Send a Measurement command to MAC to start
  doing traffic volume Measurements


DEPENDENCIES

  None.

RETURN VALUE

  tdsrrctvm_status_e_type

SIDE EFFECTS

  None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_sib_info
(
  tdsrrc_MeasurementControlSysInfo* sib_meas_ptr
);


/*===========================================================================

FUNCTION     RRCTVM_READ_TRAFFIC_VOL_PARAMS

DESCRIPTION

  Read traffic volume parameters in a Meas
  Ctrl Message. Also sends a Meas Ctrl Failure
  Message in case of an error


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_read_traffic_vol_params
(
  tdsrrc_TrafficVolumeMeasurement* rrc_mcm_ptr,
  tdsmac_ul_traffic_meas_config_type*  mac_config_ptr
);


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_EVENT_CRITERIA

DESCRIPTION

  Sets the traffic volume event criteria for MAC.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

tdsrrctvm_status_e_type tdsrrctvm_process_event_criteria
(
  tdsrrc_TransChCriteriaList * event_crit_ptr,
  tdsmac_ul_traffic_event_trigger_cfg_type* mac_event_crit_ptr
);


/*===========================================================================

FUNCTION   RRCTVM_SEND_TRAFFIC_VOL_MEAS_REPORT

DESCRIPTION

  Sends the traffic volume Measurement Report

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrctvm_send_traffic_vol_meas_report
(
  tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION          RRCTVM_SEND_MAC_CMD

DESCRIPTION

  This function sends measurement requests to MAC.
  Since all MAC cmds go via LLC, the LLC command
  TDSRRC_LLC_CMD_REQ actaully encapsules the real L1
  command. This is the LLC command funneling mechanism.
  It frees allocated command and embedded buffers when failure
  is detected before sending the command.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrctvm_send_mac_cmd
(
tdsmac_ul_traffic_meas_config_type* traffic_vol_params_ptr
);



/*===========================================================================

FUNCTION   RRCTVM_SET_MEAS_IDENTITY

DESCRIPTION

  Set traffic volume meas identity variable from SIB information.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrctvm_set_meas_identity
(
  uint16* meas_id_ptr,
  tdsrrc_TrafficVolumeMeasSysInfo* sib_meas_ptr
);


/*===========================================================================

FUNCTION   RRCTVM_INITIALIZE

DESCRIPTION

  Initializes traffic volume configuration parameters.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrctvm_initialize
(
  void
);



/*===========================================================================

FUNCTION     RRCMEAS_PROCESS_STATE_CHANGE_IND_IN_FACH_STATE

DESCRIPTION

  Processes state change indication message received in CELL_FACH
  Also changes the RRCMEAS procedure state from FACH to the
  desired state.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_state_change_ind_in_fach_state
(
tdsrrc_cmd_type* cmd_ptr
);



/*===========================================================================

FUNCTION   RRCMEAS_CONFIG_L1_MEAS_ON_TRANSITION_BY_OTHER_MSG

DESCRIPTION

  Processes the secnario when the transition from DCH-->FACH is
  as a result of radio link failure and the default intra-freq
  measurement needs to be deleted. CELL_INFO_LIST variable
  is cleared. The function tries to read
  SIB11 of the new cell and if SIB11 is valid, it updates the
  MEASUREMENT_IDENTIY variable and also re-configures L1 with
  with Stop & Overwrite command. CELL_INFO_LIST is also updated
  with the new SIB11.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  If SIB11 of new cell is not valid, then no intra-freq measurements would be done.

===========================================================================*/

void tdsrrcmeas_config_l1_meas_on_transition_by_other_msg
(
 void
);


/*===========================================================================

FUNCTION   RRCMEAS_SAVE_L1_MEAS

DESCRIPTION

  Processes the secnario when the transition from DCH-->FACH
  transition determines that intra-freq measurement has to be
  saved and no measurement reporting for this measurement has to be done.
  Since the move may result in selection of a new cell, CELL_INFO_LIST
  has to be configured again. This requires reading cells from SIB11
  again and updating the CELL_INFO_LIST

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  If SIB11 is not valid, then no intra-freq measurements would be done.

===========================================================================*/

void tdsrrcmeas_save_l1_meas
(
 void
);


/*===========================================================================

FUNCTION   RRCTVM_GET_SIBS_IN_CONNECTED_STATE

DESCRIPTION

  Processes the scenario when the transition from DCH-->FACH
  transition determines that traffic volume measurement for the
  default measurement has to be overwritten by new SIB's from
  the new cell. So read the new SIB's for traffic volume measurements


DEPENDENCIES

  None

RETURN VALUE

  TDSTVM_SUCCESS if SIB 11/12 is successfully read for Traffic Volume
  TDSTVM_FAIL if SIB 11/12 is successfully read for Traffic Volume

SIDE EFFECTS

  If SIB11 is not valid, then  traffic volume measurements may not be done.

===========================================================================*/

tdsrrctvm_status_e_type tdsrrctvm_get_sibs_in_connected_state
(
  void
);


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_DCH_TO_FACH

DESCRIPTION

  The main handler function that handles the processing of
  traffic volume measurements when RRC transitions from
  DCH-->FACH. Check the MEASUREMENT_IDENTITY variable and
  determine for each measurement what has to be done as per
  its validity. The function accordingly updates the
  MEASUREMENT_IDENTITY variable and sends a command to
  MAC to reconfigure the traffic volume measurements.
  It also processes the scenario when no traffic volume
  measurement has been assigned to the UE with a MEASUREMENT
  CONTROL message that is valid in CELL_FACH state (stored
  in the variable tdstvm_meas_id_list), that has the same identity
  as the one indicated in the "Traffic volume measurement
  system information


DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  If SIB11 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_process_dch_to_fach
(
 void
);


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_FACH_TO_DCH

DESCRIPTION

  The main handler function that handles the processing of
  traffic volume measurements when RRC transitions from
  FACH-->DCH. Check the MEASUREMENT_IDENTITY variable and
  determine for each measurement what has to be done as per
  its validity. The function accordingly updates the
  MEASUREMENT_IDENTITY variable and sends a command to
  MAC to reconfigure the traffic volume measurements.


DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  If SIB11 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_process_fach_to_dch
(
  void
);


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_TVM_SIBS_IN_CONNECTED_STATE

DESCRIPTION

  Invoked when cell reselection is done in CELL_FACH state
  and traffic volume measurements need to be updated as
  per the new SIB's. It reads the new traffic volume
  measurememt parameters from SIB11/12 and stores it in an
  internal data structure

DEPENDENCIES

  None

RETURN VALUE

  Returns TDSTVM_SUCCESS if SIB11/12 successfully read.
  Returns TDSTVM_FAIL if SIB11/12 successfully read.

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

tdsrrctvm_status_e_type tdsrrctvm_process_tvm_sibs_in_connected_state
(
  void
);


/*===========================================================================

FUNCTION   RRCTVM_GET_MEAS_ID_FROM_SIBS

DESCRIPTION

  Gets the default traffic volume measurement identity by peeking
  into SIB11/12 of SIB database.

DEPENDENCIES

  None

RETURN VALUE

  Returns TDSDEFAULT_TRAFFIC_VOL_MEAS_IDENITY if SIB11/12 does not specify
  a measurement identity or return the measurememt identity specified
  in SIB11/12

  Returns TDSINVALID_MEAS_ID if traffic volume system information is not present

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

uint16 tdsrrctvm_get_meas_id_from_sibs
(
  void
);


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_TVM_ON_CELL_RESELECTION

DESCRIPTION

  Main handler that processes traffic volume measurements on cell
  reselection in connected mode. Checks the MEASUREMENT_IDENTITY
  variable for any measurement that is owned by UTRAN.
  It saves that measurement and tells MAC to store it. If it does
  not find any measurement in the database that corresponds to the
  default measurement, it reads SIB11/12 again to get the default
  measurement.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_process_tvm_on_cell_reselection
(
  void
);


/*===========================================================================

FUNCTION   RRCTVM_INITIALIZE_MEASUREMENT_IDENTITY

DESCRIPTION

  Handles initialization of MEASUREMNT_IDENTIY variable
  for Traffic Volume Measurements.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  If SIB11/12 is not valid, then traffic volume measurements may not be done.

===========================================================================*/

void tdsrrctvm_initialize_measurement_identity
(
 void
);


/*===========================================================================

FUNCTION   RRCMEAS_INITIALIZE_CELL_LIST

DESCRIPTION

  Handles initialization of CELL_INFO_LIST variable
  for Intra Freq Measurements.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcmeas_initialize_cell_list
(
 void
);


/*===========================================================================

FUNCTION   RRCMEAS_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

  Handles initialization of MEASUREMNT_IDENTIY variable
  for Intra Freq Measurements.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcmeas_initialize_meas_id_list
(
 void
);

/*===========================================================================

  FUNCTION     RRCMEAS_VALIDATE_INTRA_FREQ_REPORT

DESCRIPTION

  Processes the Intra-freq Measurement Report from L1 and checks if
  the parameters sent in the report are valid. If the parameter/s are
  found to be invalid, the measurement report is trashed.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  Trashing Measurement Report may result in Soft Hand Off not working properly

===========================================================================*/
tdsrrcmeas_status_e_type tdsrrcmeas_validate_intra_freq_report
(
 tdsl1_meas_rpt_struct_type* l1_rpt_ptr
);



/*===========================================================================

FUNCTION   IRAT_READ_SIB11

DESCRIPTION

  This function reads Inter RAT Cell List from SIB 11 and
  stores it in tdsinter_rat_meas_id_list. This Inter RAT
  Cell List is used for cell reselection. When UTRAN sends a meas
  in connected DCH mode, this cell list is overwritten.

DEPENDENCIES

  Pointer to tdsrrc_InterRATCellInfoList_B must be valid.

RETURN VALUE

  FAILURE: If cell list is not in order
  SUCCESS: If able to analyze and fill Cell Info.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_read_sib11
(
  tdsl1_meas_sib_parm_struct_type* rrc_inter_rat_sib_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_CHECK_INTER_RAT_CELL_ORDER

DESCRIPTION

  This function checks cell order received

DEPENDENCIES

  Pointer to tdsrrc_InterRATCellInfoList_B must be valid.

RETURN VALUE

  FAILURE: If cell list is not in order
  SUCCESS: If cell list is in order.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_check_inter_rat_cell_order
(
  tdsrrc_InterRATCellInfoList_B* rrc_cell_info_list_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_UPDATE_INTER_RAT_CELL_LIST

DESCRIPTION

  This function updates the inter rat cell list received from SIB11

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If cell list is not in order
  SUCCESS: If cell list is in order.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_update_inter_rat_cell_list
(
 uint16* position_no_ptr,
 uint16 arfcn
);

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_REPORT_CRITERIA

DESCRIPTION

  This function processes the report criteria in the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_report_criteria
(
  tdsrrc_InterRATReportCriteria* reportCriteria_ptr,
  tdsl1_inter_sys_meas_ctrl_struct_type*   inter_s_ptr,
  tdsrpt_crit_enum_type* mcm_rpt_crit_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm
(
  tdsrrc_MeasurementControl_r3_IEs* rrc_mcm_ptr
,uint8 meas_ctrl_tr_id
,   tdsrrc_DL_DCCH_Message* dcch_msg_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_UPDATE_INTER_RAT_CELL_LIST

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If Cell list is invalid
  SUCCESS: If Cell list is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_cell_list_mcm
(
  tdsrrc_InterRATCellInfoList* mcm_cell_list_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_EVENT_CRITERIA

DESCRIPTION

  This function processes the event criteria received in a Measurement Control Message
    and stores in the format that is sent to L1

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If event criteria is invalid
  SUCCESS: If event criteria is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_event_criteria
(
  tdsrrc_InterRATReportingCriteria* rrc_event_criteria_ptr,
  tdsl1_inter_sys_report_crit_struct_type* l1_event_criteria_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_CHECK_INTER_RAT_CELL_ORDER_MCM

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If cell list is in order
  SUCCESS: If cell list is not in order

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_check_inter_rat_cell_order_mcm
(
  tdsrrc_NewInterRATCellList   *local_inter_rat_cell_list_ptr
);

/*===========================================================================

FUNCTION   RRCIRAT_FILL_REMOVED_INTER_RAT_CELLS

DESCRIPTION

  This function processes the removed cell list received in a Measurement Control Message


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If removed cell list is valid
  SUCCESS: If removed cell list is invalid

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_fill_removed_inter_rat_cells
(
  tdsrrc_RemovedInterRATCellList* rrc_remove_cells_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_FIND_CELL_ID

DESCRIPTION

  This function processes the inter rat cell id


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If cell id is valid
  SUCCESS: If cell id is invalid

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_find_cell_id
(
 uint16* intra_freq_cell_id_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_REMOVE_CELL_ID

DESCRIPTION

  This function removes the inter rat cell id


DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_remove_cell_id
(
 uint16* inter_rat_cell_id_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_UPDATE_INTER_RAT_CELL_LIST_MCM

DESCRIPTION

  This function updates the inter rat cell list received in a Meas Ctrl Msg


DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If cell list is valid
  SUCCESS: If cell list is invalid

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_update_inter_rat_cell_list_mcm
(
 uint32* position_no_ptr,
 uint16* cell_id_ptr,  /* L1 value */
 uint16* cell_id_to_remove_ptr,  /* Determines if a cell has to be removed */
 uint16 arfcn
);


/*===========================================================================

FUNCTION   RRCIRAT_INITIALIZE_CELL_LIST

DESCRIPTION

  This function initializes the inter rat cell list


DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_initialize_cell_list(void);


/*===========================================================================

FUNCTION   tdsrrcirat_initialize_local_neighbor_list

DESCRIPTION

  This function initializes the local neighbor list


DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_initialize_local_neighbor_list(void);


/*===========================================================================

FUNCTION   RRCIRAT_SEND_MEAS_REPORT

DESCRIPTION

  This function sends the Measurement Report to UTRAN in CELL_DCH only

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_SET_INTER_SYS_EVENT_ID

DESCRIPTION

  This function sets event id in L1 structure

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_set_inter_sys_event_id
(
  tdsl1_inter_sys_event_enum_type* l1_event_type_ptr,
  tdsrrc_EventIDInterRAT* rrc_event_id_ptr
);

/*===========================================================================

FUNCTION   RRCIRAT_VALIDATE_EVENT_RESULTS

DESCRIPTION

  This function validates the event results sent by L1

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If event resuulst are valid
  SUCCESS: If event results are invalid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_validate_event_results
(
  tdsl1_inter_sys_event_results* l1_event_results_ptr
);

/*===========================================================================

FUNCTION   RRCIRAT_CHECK_CELL_ABSENCE_IN_CELL_LIST

DESCRIPTION

  This function checks if the cells are absent in CELL_INFO_LIST

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If CELL_INFO_LIST is empty
  SUCCESS: If CELL_INFO_LIST is not empty

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_check_cell_absence_in_cell_list
(
  void
);


/*===========================================================================

FUNCTION   RRCIRAT_VALIDATE_MEASURED_RESULTS

DESCRIPTION

  This function validates the measured results sent by L1

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If measured results are valid
  SUCCESS: If measured results are invalid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_validate_measured_results
(
  tdsl1_inter_sys_meas_result_struct_type* meas_results_ptr
);

/*===========================================================================

FUNCTION   RRCIRAT_INIT_SIB11_PARAMS

DESCRIPTION

  This function initializes SIB11 params for Inter-RAT Meas

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If measured results are valid
  SUCCESS: If measured results are invalid.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_init_sib11_params
(
  tdsl1_meas_sib_parm_struct_type* l1_sib11_inter_rat_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_CONVERT_RSSI_VALUE

DESCRIPTION

  This function converst the RSSI value sent by L1 into a format expected by UTRAN.
    Maximum converted value is 63 and minimum converted value is 0

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcirat_convert_rssi_value
(
  int16* gsm_carrier_rssi_ptr,
  uint8* rssi_converted_value_ptr
);


/*===========================================================================

FUNCTION   RRCIRAT_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

  Handles initialization of MEASUREMNT_IDENTIY variable
  for Iner RAT Measurements.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcirat_initialize_meas_id_list
(
 void
);


/*===========================================================================

FUNCTION  RRCIRAT_FILL_HCS_INFO

DESCRIPTION

 This function fills the overall HCS info for an Inter RAT Cell.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill overall HCS info correctly
 SUCCESS: If overall HCS info is filled correctly

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_fill_hcs_info
(
  tdsrrc_InterRATMeasurementSysInfo* sib_inter_rat_ptr,
  tdsl1_meas_sib_parm_struct_type* l1_sib_ptr
  ,tdsrrc_SysInfoType11_v590ext_IEs_newInterRATCellInfoList_v590ext * rel5_ext_ptr
);







/*===========================================================================

FUNCTION   RRCIFREQ_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

  Handles initialization of MEASUREMNT_IDENTIY variable
  for Inter Freq Measurements.

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcifreq_initialize_meas_id_list
(
 void
);


/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_EVENT_CRITERIA

DESCRIPTION

  This function processes the event criteria received in a Measurement Control Message
    and stores in the format that is sent to L1

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If event criteria is invalid
  SUCCESS: If event criteria is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_event_criteria
(
  tdsrrc_InterFreqReportingCriteria* rrc_event_criteria_ptr,
  tdsl1_inter_freq_report_crit_struct_type* l1_event_criteria_ptr
 ,tdsrrc_MeasurementControl_v590ext_IEs* rel5_ext_ptr
);


/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM

DESCRIPTION

  This function processes the Measurement Control Message for
    Inter-Freq Measurements

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm
(
  tdsrrc_MeasurementControl_r3_IEs* rrc_mcm_ptr
,tdsrrc_DL_DCCH_Message* dcch_msg_ptr
,uint8 meas_ctrl_tr_id
);



/*===========================================================================

FUNCTION   RRCIFREQ_SEND_MEAS_REPORT

DESCRIPTION

    This function sends the measurement report. First it validates the event
    and measured results depending upon the report type and it ignores the L1
    report if reported values are incorrect. The function also appends
    additional measurements if any and also updates the Inter freq meas
    id list if last periodic report is being sent

DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcifreq_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_ADDITIONAL_MEAS

DESCRIPTION

  This function contains the logic of checking if additional measurements
  need to be appended and also appends additional measurements to measured results.

DEPENDENCIES

  The calling function should provide rpt_ptr as NULL if additional results
  do not exist at all.

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_process_additional_measured_results
(
 uint16* primary_meas_id_ptr,
 tdsrrcmeas_layer_e_type primary_meas_type,
 tdsrrc_MeasurementReport* rpt_ptr
);


/*===========================================================================

FUNCTION   RRCTVM_APPEND_TRAFFIC_VOL_MEAS

DESCRIPTION

  This function appends Traffic Volume Measurements as additional measurements
  to any other Measurement Report

DEPENDENCIES

  The called function appends the element at the end of
  the list by allocating memory

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

boolean rrctvm_append_traffic_vol_meas
(
 uint16* additional_meas_id_ptr,
 tdsrrc_MeasurementReport* rpt_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_FIND_ADDITIONAL_MEAS_TYPE

DESCRIPTION

  This function checks if additional measurements exists in RRC database. If it
  exists, it returns its type else returns unknown Measurement which essentially
  implies that the additional measurement does not exist


DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None

===========================================================================*/

tdsrrcmeas_layer_e_type tdsrrcmeas_find_additional_meas_type
(
 uint16* additional_meas_id_ptr,
 tdsrpt_crit_enum_type* rpt_crit_ptr,
 tdsmeas_validity_enum_type* addtl_meas_validity_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_APPEND_INTER_RAT_RESULTS

DESCRIPTION

  This function appends Inter-RAT Measured Results to any report.
  It first checks if additional measured results have already been appended
  the measurement report. If yes, then it increments the pointer to point
  to the next available space in additional measured result list


DEPENDENCIES

  The calling function must have initialized the additional measured
  results pointer

RETURN VALUE

  True
  False

SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrcmeas_append_inter_rat_results
(
 uint8* meas_id_ptr,
 tdsl1_inter_sys_meas_result_struct_type * l1_meas_results
);

void tdsrrcmeas_append_inter_rat_results_ptr
(
 tdsrrc_MeasuredResults_LCR_r4* measured_results_ptr,
 tdsl1_inter_sys_meas_result_struct_type * l1_meas_results
);


/*===========================================================================

FUNCTION   RRCMEAS_APPEND_INTRA_FREQ_RESULTS

DESCRIPTION

  This function appends Intra-Freq Measured Results to any report.
  It first checks if additional measured results have already been appended
  the measurement report. If yes, then it increments the pointer to point
  to the next available space in additional measured result list


DEPENDENCIES

  The calling function must have initialized the additional measured
  results pointer

RETURN VALUE

  True
  False

SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrcmeas_append_intra_freq_results
(
 uint8* meas_id_ptr,
 tdsl1_meas_rpt_struct_type * l1_rpt
);


/*===========================================================================

FUNCTION   RRCTVM_APPEND_TRAFFIC_VOL_MEAS

DESCRIPTION

  This function appends Traffic Volume Measurements as additional measurements
  to any other Measurement Report

DEPENDENCIES

  The called function appends the element at the end of
  the list by allocating memory. The calling function must set
  the first element of the list to NULL if the list is empty

RETURN VALUE

  True
  False

SIDE EFFECTS

  None

===========================================================================*/

boolean tdsrrctvm_append_traffic_vol_results
(
 uint8* meas_id_ptr,
 tdsmac_ul_traffic_meas_ind_type * additional_tvm
);


void  tdsrrctvm_append_traffic_vol_results_ptr
(
   tdsrrc_MeasuredResults_LCR_r4  * measured_results_ptr,
   tdsmac_ul_traffic_meas_ind_type * additional_tvm
);

/*===========================================================================

FUNCTION   RRCMEAS_CONFIG_INTRA_FREQ_DB

DESCRIPTION

  Sets Intra Freq Reporting Criteria in RRC Measurement Database
  from Measurement Control Message after the validation of
  the message has been completed

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

boolean tdsrrcmeas_config_intra_freq_db
(
 tdsl1_meas_ctrl_parm_struct_type* l1_mcm_ptr,
 tdsmeas_validity_enum_type m_validity
);


/*===========================================================================

FUNCTION   RRCMEAS_DELETE_MEAS

DESCRIPTION

  Removes Intra Freq Meas Information in RRC Measurement Database.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrcmeas_delete_meas(uint16* meas_id_ptr);

/*===========================================================================

FUNCTION   RRCMEAS_SET_ADDITIONAL_MEAS_IN_DB

DESCRIPTION

  This function sets Additional Measurements in RRC database for the measurement
  type. This function is only called if it has been determined previously that
  the primary measurement exists and the new additional measurement does not conflict
  with the primary measurement validity and reporting criteria


DEPENDENCIES

  None.

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrcmeas_set_additional_meas_in_db
(
 tdsrrcmeas_layer_e_type* meas_type_ptr,
 tdsrrc_AdditionalMeasurementID_List * additional_list_ptr,
 uint32 tdsmeas_id
);


/*===========================================================================

FUNCTION   RRCMEAS_VALIDATE_ADDITIONAL_MEAS

DESCRIPTION

  This function checks if additional measurements exists in RRC database and have the
  same validity as the primary measurement. If it exists, it returns its type else
  returns unknown Measurement which essentially implies that the additional
  measurement does not exist. The function also returns the reporting criteria as per
  the measurement type. The reporting criteria is used by the calling function to
  determine if UTRAN is trying to append an event triggered meas. The function also
  sends Measurement Control Failure Message in case of error condition


DEPENDENCIES

  None.

RETURN VALUE

  False if invalid. Meas Ctrl Failure already sent.

SIDE EFFECTS

  None

===========================================================================*/

boolean tdsrrcmeas_validate_additional_meas
(
  tdsrrc_AdditionalMeasurementID_List * additional_list_ptr,
  uint32 tr_id,
  tdsrrcmeas_layer_e_type* primary_meas_type_ptr,
  uint32 mid
);


/*===========================================================================

FUNCTION   RRCTVM_PROCESS_MCM_IDENTITY

DESCRIPTION

  Set traffic volume meas identity variable from SIB information.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrctvm_process_mcm_identity
(
uint16* meas_id_ptr
);


/*===========================================================================

FUNCTION   RRCTVM_VALIDATE_ADDITIONAL_MEAS

DESCRIPTION

  This function checks if additional measurements exists in RRC database and have the
  same validity as the primary measurement. If it
  exists, it returns its type else returns unknown Measurement which essentially
  implies that the additional measurement does not exist. The function also returns
  the reporting criteria as per the measurement type. The reporting criteria is used by
  the calling function to determine if UTRAN is trying to append an event triggered meas


DEPENDENCIES

  None.

RETURN VALUE

  False if invalid. Meas Ctrl Failure already sent.

SIDE EFFECTS

  None

===========================================================================*/

boolean tdsrrctvm_validate_additional_meas
(
 tdsmeas_validity_enum_type* p_meas_validity,
 tdsrrc_AdditionalMeasurementID_List * additional_list_ptr,
 uint8 mcm_tr_id,
 uint32 mid
);


/*===========================================================================

FUNCTION   RRCTVM_CONFIG_TVM_DB

DESCRIPTION

  Sets Traffic Volume Meas Information in RRC Measurement Database.

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

boolean tdsrrctvm_config_tvm_db
(
 tdsmac_ul_traffic_meas_config_type* mac_mcm_ptr,
 tdsmeas_validity_enum_type* p_meas_validity
);


/*===========================================================================

FUNCTION   RRCTVM_SET_REPORTING_CRITERIA

DESCRIPTION

    This function sets reporting criteria in tdstvm_meas_id_list from SIB's


DEPENDENCIES

    None

RETURN VALUE

    None

SIDE EFFECTS

    None

===========================================================================*/

void tdsrrctvm_set_reporting_criteria
(
  uint16* meas_id_ptr,
  tdsrrc_TrafficVolumeReportCriteriaSysInfo* sib_rpt_crit_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_DELETE_ADDTL_MEAS

DESCRIPTION

    This function deletes any additional measurement that may be present
  in any of the MEASUREMENT_IDENTITY list at RRC level. The primary
  measurement identity has already been deleted


DEPENDENCIES

    None

RETURN VALUE

    None

SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcmeas_delete_addtl_meas
(
  uint16* prim_meas_id_ptr
);


/*===========================================================================

FUNCTION  RRCMEAS_FILL_HCS_INFO

DESCRIPTION

 This function fills the overall HCS info for an Intra Freq Cell.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill overall HCS info correctly
 SUCCESS: If overall HCS info is filled correctly

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_hcs_info
(
  tdsrrc_MeasurementControlSysInfo_LCR_r4_ext* sib_meas_ctrl_ptr
  ,tdsrrc_SysInfoType11_v590ext_IEs *sib11_rel5_ext_ptr
);


/*===========================================================================

FUNCTION  RRCMEAS_FILL_HCS_RSCP_CELL_INFO

DESCRIPTION

 This function fills in Cell Info for a HCS cell when
 Cell Selection Reselection Quality Measure is RSCP.
 The function processes Cell Info in ASN1 format and
 translates into the L1 RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill cell info correctly
 SUCCESS: If Cell Info is filled correctly

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_hcs_rscp_cell_info
(
    tdsrrc_CellInfoSI_HCS_RSCP_LCR_r4*sib_cell_info_ptr,
    tdsl1_cell_info_struct_type* l1_cell_info_ptr
  ,tdsrrc_CellSelectReselectInfo_v590ext * rel5_ext_ptr
  );


/*===========================================================================

FUNCTION   RRCMEAS_FILL_HCS_ECN0_CELL_INFO

DESCRIPTION

 This function fills in Cell Info for a HCS cell when
 Cell Selection Reselection Quality Measure is ECN0.
 The function processes Cell Info in ASN1 format and
 translates into the L1 RRC Interface format.

DEPENDENCIES

 Pointer to tdsrrc_CellInfoSI_ECN0 must be valid.

RETURN VALUE

 FAILURE: If passed pointer is NULL.
 SUCCESS: If able to analyze and fill Cell Info.

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_hcs_ecn0_cell_info
  (
  tdsrrc_CellInfoSI_HCS_ECN0_LCR_r4*sib_cell_info_ptr,
  tdsl1_cell_info_struct_type *l1_cell_info_ptr
  ,tdsrrc_CellSelectReselectInfo_v590ext * rel5_ext_ptr
  );



/*===========================================================================

FUNCTION  RRCIFREQ_VALIDATE_FREQ

DESCRIPTION

 This function validates the uplink and downlink carrier frequency.


DEPENDENCIES

 None

RETURN VALUE

 True if validation successful
 False if validation fails

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcifreq_validate_freq
(
 tdsl1_freq_info_struct_type* freq_info_ptr
);


/*===========================================================================

FUNCTION  RRCIFREQ_VALIDATE_MEAS_REPORT

DESCRIPTION

 This function validates the measurement report.

DEPENDENCIES

 None

RETURN VALUE

 True if validation successful
 False if validation fails

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcifreq_validate_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
);


/*===========================================================================

FUNCTION  RRCIFREQ_SET_INTER_FREQ_EVENT_ID

DESCRIPTION

 This function sets the event id as per ASN1 format from what L1 specified
 in the measurement report

DEPENDENCIES

 None

RETURN VALUE

 True if validation successful
 False if validation fails

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcifreq_set_inter_freq_event_id
(
 tdsl1_inter_freq_event_enum_type* event_id_ptr,
 tdsrrc_EventIDInterFreq* rrc_inter_freq_event_id_ptr
);

/*===========================================================================

FUNCTION   IFREQ_READ_SIB11

DESCRIPTION

  This function reads Inter Freq Cell List from SIB 11 and
  stores it in inter_freq_cell_id_list. This Inter Freq
  Cell List is used for cell reselection. When UTRAN sends a meas
  in connected DCH mode, this cell list is overwritten.

DEPENDENCIES

  -

RETURN VALUE
    Success if parameters successfully read
  Failure otherwise


SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_read_sib11
(
 tdsl1_inter_freq_cell_list_struct_type* rrc_inter_freq_sib_ptr
);


/*===========================================================================

FUNCTION   RRCIFREQ_UPDATE_INTER_FREQ_CELL_LIST

DESCRIPTION

  Updates the inter freq cell list with new values of inter freq
  cell id. This is done on receving SIB 11 system info. The calling fn gives
  position_no_ptr indicating where to store the cpi. If it
  points to TDSAVAILABLE_CELL_POSITION, then store cpi in the first
  available position in tdsinter_freq_cell_list and and mark that
  position as occupied. If position_no_ptr points to some other
  value then store cpi at the position given by that value

DEPENDENCIES

  None

RETURN VALUE

  FAILURE if inter freq cell list cannot be updated due to incorrect
  value of the inter freq cell id
  SUCCESS if inter freq cell list can be successfully updated

SIDE EFFECTS

  None
===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_update_inter_freq_cell_list
(
 uint32* position_no_ptr,
 uint16* cpi_ptr,
 uint16* dl_freq_ptr,
 boolean* sec_freq_ind_incl,
 boolean* sec_freq_ind
);


/*===========================================================================

FUNCTION   RRCIFREQ_INITIALIZE_CELL_LIST

DESCRIPTION

  This function initializes the inter freq cell list

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
===========================================================================*/

void tdsrrcifreq_initialize_cell_list
(
 void
);




/*===========================================================================

FUNCTION   RRCIFREQ_UPDATE_INTER_FREQ_CELL_LIST_MCM

DESCRIPTION

 Invoked from the cell list processing fn specified by MCM
 The calling fn specifies the position, scrambling code
 and the downlink freq. The fn checks if there is
 already a valid scr code and dl freq at that position.
 If yes, then it stores them in psc_to_remove_ptr and
 dl_freq_to_remove_ptr and updates the inter freq cell list
 with the new scr code and dl freq and does not change
 the position. If there was no cell at that position, then
 it stores the dl freq and scr code and markls that position
 in the inter freq cell list as occupied.

DEPENDENCIES

 None.

RETURN VALUE

 FAILURE: If unable to update the inter freq cell list.
 SUCCESS: If able to update the inter freq cell list.

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcifreq_update_inter_freq_cell_list_mcm
(
 uint32* position_no_ptr,
 uint16* cpi_ptr,
 uint16* dl_freq_ptr,
 uint16* cpi_to_remove_ptr,
 uint16* dl_freq_to_remove_ptr, 
 boolean *sec_freq_ind_incl,
 boolean *sec_freq_ind
);



/*===========================================================================

FUNCTION   RRCIFREQ_FIND_PSC_FREQ

DESCRIPTION

 Finds the primary scrambling code of the inter freq cell id. The primary
 scrambling code is returned back in psc_ptr provided. If not found,
 then cpi_ptr contains TDSINVALID_CPI

DEPENDENCIES

 None

RETURN VALUE

 TDSCPI_ABSENT if cpi absent
 TDSCPI_PRESENT if cpi present

SIDE EFFECTS

 None
===========================================================================*/

tdscpi_e_type tdsrrcifreq_find_cpi_freq
(
 uint32 inter_freq_cell_id,
 uint16* cpi_ptr,
 uint16* dl_freq_ptr, 
 boolean *sec_freq_ind_incl, 
 boolean *sec_freq_ind 
);


/*===========================================================================

FUNCTION   RRCIFREQ_REMOVE_CELLID

DESCRIPTION

 This function removes the cell id in inter freq cell list and marks
 the posiiton as available. It also invalidates the scr code
 and the dl freq to DEFAULT values.

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcifreq_remove_cellid
(
 uint32 inter_freq_cell_id
);


/*===========================================================================

FUNCTION   RRCIFREQ_FILL_RPT_QUAN

DESCRIPTION

 This function fills the reporting quantity in L1 structure from the ASN1 format

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcifreq_fill_rpt_quan
(
 tdsrrc_InterFreqReportingQuantity* mcm_rpt_quan_ptr,
 tdsl1_inter_freq_rpt_quan_struct_type* l1_rpt_quan_ptr
);


/*===========================================================================

FUNCTION   RRCIFREQ_SEND_L1_CMD

DESCRIPTION

 This function sends the L1 command from SIB's only

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcifreq_send_l1_cmd
(
 tdsl1_meas_ctrl_enum_type*  meas_choice_ptr,
 tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib_ptr
);


/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MEAS_QUANTITY

DESCRIPTION

 This function processes the measurement quantity and converst it into L1 format.

DEPENDENCIES

 None

RETURN VALUE

 Success if meas quantity is valid
 Failure if meas quantity is invalid

SIDE EFFECTS

 None
===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcifreq_process_meas_quantity
(
  tdsrrc_InterFreqMeasQuantity* meas_quan_ptr,
  tdsl1_inter_freq_meas_quan_struct_type* l1_meas_quan_ptr
);


/*===========================================================================

FUNCTION   RRCIFREQ_CONFIG_INTER_FREQ_DB

DESCRIPTION

 This function is called after the the validation of Measurement Control
 Message is successful and then Inter Freq Meas Id list needs to be updated.
 It stores the meas id, reporting criteria and validity

DEPENDENCIES

 None

RETURN VALUE

 Success if inter freq meas id list is successfully updated
 Failure otherwise

SIDE EFFECTS

 None
===========================================================================*/

boolean tdsrrcifreq_config_inter_freq_db
(
 tdsl1_meas_ctrl_parm_struct_type* l1_mcm_ptr,
 tdsrpt_crit_enum_type* rpt_crit_ptr,
 tdsmeas_validity_enum_type m_validity
);


uecomdef_status_e_type tdsrrcifreq_fill_hcs_info
(
  tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib_ptr
  ,tdsrrc_SysInfoType12_v590ext_IEs_newInterFrequencyCellInfoList_v590ext * rel5_ext_ptr
);



/*===========================================================================

FUNCTION   RRCUEIM_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

 This function initializes the measurement identity list of
 UE Internal Measurements

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcueim_initialize_meas_id_list
(
 void
);


/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM

DESCRIPTION

 This function processes the Measurement Control Message

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm
(
  tdsrrc_MeasurementControl_r3_IEs* rrc_mcm_ptr
 ,uint8 meas_ctrl_tr_id
);


/*===========================================================================

FUNCTION   RRCUEIM_SEND_MEAS_REPORT

DESCRIPTION

 This function sends the Measurement Report after validation

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcueim_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
);


/*===========================================================================

FUNCTION   RRCUEIM_SET_EVENT_ID

DESCRIPTION

 This function sets the event ID in ASN1 format

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcueim_set_event_id
(
 tdsl1_internal_meas_event_enum_type* l1_event_type_ptr,
 tdsrrc_UE_InternalEventResults* rrc_event_id_ptr
);


/*===========================================================================

FUNCTION   RRCUEIM_VALIDATE_MEASURED_RESULTS

DESCRIPTION

 This function validates the measured results

DEPENDENCIES

 None

RETURN VALUE

 Success if validation successful
 Failure Otherwise

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_validate_measured_results
(
 tdsl1_ue_internal_meas_rpt_struct_type* meas_results_ptr
);


/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_EVENT_CRITERIA

DESCRIPTION

 This function processes the event criteria specified in Measurement
 Control Message

DEPENDENCIES

 None

RETURN VALUE

 Success if event parameters are valid
 Failure Otherwise

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_event_criteria
(
 tdsrrc_UE_InternalReportingCriteria* rrc_event_criteria_ptr,
 tdsl1_internal_meas_rpt_crit_struct_type* l1_event_criteria_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_APPEND_INTERNAL_MEAS_RESULTS

DESCRIPTION

    This function appends Internal Measured Results to any report.
    It first checks if additional measured results have already been appended
    the measurement report. If yes, then it increments the pointer to point
    to the next available space in additional measured result list


DEPENDENCIES

    The calling function must have initialized the additional measured
    results pointer

RETURN VALUE

    True if UE Internal Measured Results successfully appended
    False Otherwise

SIDE EFFECTS

    None

===========================================================================*/

boolean tdsrrcmeas_append_internal_meas_results
(
 uint8* meas_id_ptr,
 tdsl1_ue_internal_meas_rpt_struct_type * ue_internal_meas
);

void tdsrrcmeas_append_internal_meas_results_ptr
(
 tdsrrc_MeasuredResults_LCR_r4  * measured_results_ptr,
 tdsl1_ue_internal_meas_rpt_struct_type * ue_internal_meas
);

/*===========================================================================

FUNCTION   RRCMEAS_APPEND_INTER_FREQ_MEAS_RESULTS

DESCRIPTION

    This function appends Inter-Freq Measured Results to any report.
    It first checks if additional measured results have already been appended
    the measurement report. If yes, then it increments the pointer to point
    to the next space in additional measured result list and sets the result
    in that location. Additional Measurement Identity passed to
  this function has already been validated

DEPENDENCIES

    The calling function must have initialized the additional measured
    results pointer

RETURN VALUE

    True if results successfully appended
    False otherwise

SIDE EFFECTS

    None

===========================================================================*/
boolean tdsrrcmeas_append_inter_freq_meas_results
(
 uint8* meas_id_ptr,
 tdsl1_meas_rpt_struct_type * l1_rpt
);

void tdsrrcmeas_append_inter_freq_meas_results_ptr
(
 tdsrrc_MeasuredResults_LCR_r4  * measured_results_ptr,
 tdsl1_meas_rpt_struct_type * l1_rpt
);



/*===========================================================================

FUNCTION   RRCMEAS_CHECK_MCM_FOR_SETUP_MODIFY_ADDITIONAL

DESCRIPTION

  This function checks if the existing measurement already exists
  in the measurement database. If yes, then it deletes the
  additional measurement associated with it. Also for a modify
  it checks if UTRAN tries to add more than the maximum no of
  allowed measurements, then it rejects the configuration

DEPENDENCIES

  None

RETURN VALUE

  True/False

SIDE EFFECTS

  None
===========================================================================*/

boolean tdsrrcmeas_check_mcm_for_setup_modify_additional
(
 boolean addtl_meas_incl,
 boolean mcm_setup,
 tdsrrcmeas_layer_e_type primary_meas_type,
 uint32 mid,
 tdsrrc_AdditionalMeasurementID_List * local_addtl_meas_list_ptr
);


#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)


/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability
(
  const tdsrrc_UE_Positioning_ReportingQuantity *rpt_quant_ptr
);


/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY_R5

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message for RRC R5.

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability_r5
(
  const tdsrrc_UE_Positioning_ReportingQuantity_r4 *rpt_quan_ptr
);

/*===========================================================================

FUNCTION    RRCMEAS_PROCESS_POS_MCM

DESCRIPTION

 Processes Measurement Control Message for any Positioning Measurement

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_process_pos_mcm
(
  void* msg_ptr,
  tdspos_info_struct_type* pos_info_ptr
  ,uint8 meas_ctrl_tr_id
);


/*===========================================================================

FUNCTION    RRCGPS_PROCESS_MCM

DESCRIPTION

 Processes GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm
(
  void* pos_msg_ptr,
  tdspos_info_struct_type* pos_info_ptr
  ,uint8 meas_ctrl_tr_id
);


/*===========================================================================

FUNCTION    RRCGPS_SEND_LSM_CMD

DESCRIPTION

 Sends command to LSM to start Measurements

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_send_lsm_cmd
(
  tdsrrc_meas_params_struct_type* gps_meas_params
);


/*===========================================================================

FUNCTION    RRCGPS_SEND_MEAS_REPORT

DESCRIPTION

 Sends GPS Measurement Report to UTRAN

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_send_meas_report
(
  tdsrrc_cmd_type *cmd_ptr
);


/*===========================================================================

FUNCTION    RRCGPS_SET_MEAS_RESULTS

DESCRIPTION

 Sets Measured Results received from LSM in ASN1 format

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_set_meas_results
(
  tdsrrc_gps_measured_results_struct_type* lsm_gps_meas_results_ptr,
  tdsrrc_UE_Positioning_GPS_MeasurementResults* rrc_gps_meas_results_ptr
);


/*===========================================================================

FUNCTION    RRCGPS_SET_POS_ERROR

DESCRIPTION

 Sets Positioning Error received from LSM in ASN1 format

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_set_pos_error
(
  tdsrrc_positioning_error_struct_type* lsm_gps_meas_ptr,
  tdsrrc_UE_Positioning_Error* rrc_gps_pos_error_ptr
);


/*===========================================================================

FUNCTION    RRCGPS_SET_POS_ESTIMATE

DESCRIPTION

 Sets Positioning Estimate received from LSM in ASN1 format

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_set_pos_estimate
(
  tdsrrc_positioning_estimate_struct_type* lsm_pos_est_ptr,
  tdsrrc_UE_Positioning_PositionEstimateInfo *rrc_gps_pos_est_ptr
);



/*===========================================================================

FUNCTION    RRCGPS_INITIALIZE_POS_MEAS_ID_LIST

DESCRIPTION

 Initializes Positioning Measurement Identity List

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcmeas_initialize_pos_meas_id_list
(
  void
);

/*===========================================================================

FUNCTION    RRCGPS_PROCESS_ASD_MSG

DESCRIPTION

 Processes Assistance Data Delievery Message received from UTRAN

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcgps_process_asd_msg
(
  tdsrrc_cmd_type* cmd_ptr
);


/*===========================================================================

FUNCTION    RRCGPS_DISPATCH_LSM_COMMANDS

DESCRIPTION

 Dispatch LSM Command

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcdispatch_lsm_commands
(
  void
);


/*===========================================================================

FUNCTION    RRCGPS_VALIDATE_MEAS_RESULTS

DESCRIPTION

 Validates Measured Results received from LSM

DEPENDENCIES

 None.

RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcgps_validate_meas_results
(
  tdsrrc_gps_measured_results_struct_type* lsm_gps_meas_results_ptr
);


/*===========================================================================

FUNCTION    RRCGPS_VALIDATE_POS_ESTIMATE

DESCRIPTION

 Validates Positioning Estimate received from LSM

DEPENDENCIES

 None.

RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcgps_validate_pos_estimate
(
  tdsrrc_positioning_estimate_struct_type* lsm_pos_est_ptr
);


/*===========================================================================

FUNCTION    RRCGPS_VALIDATE_POS_ERROR

DESCRIPTION

 Validates Positioning Error received from LSM

DEPENDENCIES

 None.

RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcgps_validate_pos_error
(
  tdsrrc_positioning_error_struct_type* lsm_gps_meas_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_CONFIG_POS_DB

DESCRIPTION

 Sets Positioning Meas Information in RRC Measurement Database.

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

boolean tdsrrcmeas_config_pos_db
(
 tdspos_info_struct_type* pos_info_ptr,
 tdsmeas_validity_enum_type* p_meas_validity,
 tdsrpt_crit_enum_type* pos_rpt_crit,
 boolean modify_meas_params_incl /* Only applicable for MODIFY */
);

#endif


/*===========================================================================

FUNCTION    RRCMEAS_SEND_RRC_STATUS_MSG

DESCRIPTION

 Sends RRC Status Message to UTRAN

DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcmeas_send_rrc_status_msg
(
 uint8* transaction_id_ptr,
 uint8* error_choice_ptr
);


/*===========================================================================

  FUNCTION     RRCMEAS_GET_FACH_MEAS_INFO

DESCRIPTION

  Gets FACH Meas Occasion Info from SIB11

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  Trashing Measurement Report may result in Soft Hand Off not working properly

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_get_fach_meas_info
(
 void
);

/*===========================================================================

FUNCTION tdsrrcmeas_get_rach_rpt_quantity

DESCRIPTION

 Gets RACH reporting measurement quantity

DEPENDENCIES

 This fn is only called when RACH criteria for the new cell is supported

RETURN VALUE

 Success if RACH quantity read is supported
 Failure if RACH quantity read is not supported

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_get_rach_rpt_quantity
(
  tdsl1_intra_freq_rach_rpt_quan_struct_type* meas_quantity
);


/*===========================================================================

FUNCTION  RRCIRAT_DELETE_DUP_NON_VERIFIED_CELLS

DESCRIPTION

 This function deletes duplicate non verified cells from Meas Report.

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcirat_delete_dup_non_verified_cells
(
 tdsl1_inter_sys_meas_result_struct_type* meas_results_ptr
);


/*===========================================================================

FUNCTION  RRCTVM_PROCESS_SIB_CHANGE_NOTIFICATION

DESCRIPTION

 This function processes SIB change notification to modify TVM

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrctvm_process_sib_change_notification
(
  tdsrrc_sib_change_mask_type sib_change_mask
);



/*===========================================================================

FUNCTION tdsrrcmeas_check_if_cpi_present_in_aset

DESCRIPTION

 Checks if CPI is present in ASET when UE transitions from Idle-->DCH
 or from FACH-->DCH

DEPENDENCIES



RETURN VALUE

 Success if CPI present in ASET
 Failure if CPI not present in ASET

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcmeas_check_if_cpi_present_in_aset
(
  void
);


/*===========================================================================

FUNCTION tdsrrcmeas_update_meas_after_state_trans

DESCRIPTION

 Updates in all meas id variable that the specific meas cannot be reported

DEPENDENCIES


RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcmeas_update_meas_after_state_trans
(
  void
);


/*===========================================================================

FUNCTION tdsrrcmeas_check_if_rpt_can_be_txted

DESCRIPTION

 Updates in all meas id variable that the specific meas cannot be reported

DEPENDENCIES


RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcmeas_check_if_rpt_can_be_txted
(
  uint16 m_id
);


/*===========================================================================

FUNCTION tdsrrcmeas_check_if_meas_reconfig_reqd

DESCRIPTION

 Get the meas id from SIB11 of the new cell. Then compare
 if this meas id matches with any of the meas id which was
 owned by a MCM. If TRUE, then return FALSE indicating that
 complete measurement reconfig not required. Else return TRUE
 indicating that complete meas reconfig required. The fn also checks
 if at meas db is empty. In that case it returns true in meas_db_empty_ptr

DEPENDENCIES


RETURN VALUE

 True/False

SIDE EFFECTS

 None

===========================================================================*/

boolean tdsrrcmeas_check_if_meas_reconfig_reqd
(
  uint16* m_id_ptr,
  boolean* meas_db_empty_ptr
);

#if defined(FEATURE_CGPS)
/*===========================================================================

FUNCTION tdsrrcmeas_set_cell_plmn_info

DESCRIPTION

 Sets the PLMN id, Cell id ,CPID, Freq of the current serving cell in the
 global structure tdsplmn_cell_info

DEPENDENCIES
 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcmeas_set_cell_plmn_info
(
  void
);
#endif /* FEATURE_MMGPS || FEATURE_CGPS */


#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION   RRCGPS_PROCESS_DCH_TO_FACH_PCH

DESCRIPTION

  DCH->FACH and DCH->PCH transition


DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS

===========================================================================*/

void tdsrrcgps_process_dch_to_fach_pch
(
 void
);


/*===========================================================================

FUNCTION   RRCGPS_PROCESS_FACH_TO_DCH

DESCRIPTION

  DCH->FACH transition


DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS



===========================================================================*/

void tdsrrcgps_process_fach_to_dch
(
 void
);

/*===========================================================================

FUNCTION   RRCGPS_PROCESS_FACH_TO_PCH

DESCRIPTION

  FACH->PCH transition


DEPENDENCIES

  None

RETURN VALUE

  None.

SIDE EFFECTS



===========================================================================*/

void tdsrrcgps_process_fach_to_pch
(
 void
);
#endif
/*===========================================================================

FUNCTION tdsrrcirat_read_sib12

DESCRIPTION

 Caller must check the tdssib12_present before calling this fn.
 This function processes SIB12 IEs and build the information
 to be sent down to L1 and updates the cell info list for
 inter rat measurements.

DEPENDENCIES

 SIB12 must have been received by the camping cell.

RETURN VALUE

 SUCCESS/FAIILURE

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_read_sib12
(
tdsrrc_SysInfoType12 *sib12_ptr,
tdsl1_meas_sib_parm_struct_type* l1_inter_rat_sib_ptr
);

/*===========================================================================

FUNCTION tdsrrcirat_process_remove_cell_list

DESCRIPTION

 This function processes the remove cell list of SIB12 and updates the inter
 rat cell list

DEPENDENCIES

 SIB12 must have been received by the camping cell.

RETURN VALUE
 None

SIDE EFFECTS

 None

===========================================================================*/
void tdsrrcirat_process_remove_cell_list
(
tdsrrc_RemovedInterRATCellList *removedInterRATCellList,
uint16 *num_removed_cells
);

/*===========================================================================

FUNCTION tdsrrcirat_build_composite_cell_list_from_SIB11_SIB12

DESCRIPTION

 This function builds a composite cell info list from SIB11 and SIB12 that is
 then sent down to L1

DEPENDENCIES

 SIB12 must have been received by the camping cell.

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcirat_build_composite_cell_list_from_SIB11_SIB12
(
tdsrrc_RemovedInterRATCellList *removedInterRATCellList,
tdsl1_inter_sys_cell_list_struct_type* sib11_cell_list_ptr,
tdsl1_inter_sys_cell_list_struct_type* sib12_cell_list_ptr,
uint16 num_removed_cells
);

/*===========================================================================

FUNCTION   copy_sib11_inter_freq_cell_list_to_sib12

DESCRIPTION

    This function copies the inter freq cell list from SIB 11 nto the SIB12
    inter freq cell list ptr

DEPENDENCIES

    None

RETURN VALUE
   None


SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcifreq_copy_sib11_inter_freq_cell_list_to_sib12
(
tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib11_ptr,
tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib12_ptr
);

/*===========================================================================

FUNCTION   tdsrrcifreq_build_composite_cell_list

DESCRIPTION

    This function builds the composite inter freq cell info list from SIB 11
    and SIB12.

DEPENDENCIES

    None

RETURN VALUE
   None


SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_build_composite_cell_list
(
tdsrrc_RemovedInterFreqCellList *removedInterFreqCellList,
tdsl1_inter_freq_cell_list_struct_type* sib11_cell_list_ptr,
tdsl1_inter_freq_cell_list_struct_type* sib12_cell_list_ptr,
uint32 *num_rmv_cells
);

/*===========================================================================

FUNCTION    RRCMEAS_CLEAR_SIB12

DESCRIPTION

 Initialises SIB12 stored variable, sib12_rach_report_crit
DEPENDENCIES

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcmeas_clear_sib12
(
tdsl1_meas_sib_parm_struct_type *rrcmeas_sib12_ptr
);

/*===========================================================================

FUNCTION   IFREQ_READ_SIB12

DESCRIPTION

    This function reads Inter Freq Cell List from SIB 12 and
    stores it in tdsinter_freq_meas_id_list. This Inter Freq
    Cell List is used for cell reselection. When UTRAN sends a meas
    in connected DCH mode, this cell list is overwritten.

DEPENDENCIES

    This function assumes that SIB12 is present and that the sib12_ptr
    != NULL

RETURN VALUE
   None


SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_read_sib12
(
 tdsrrc_SysInfoType12 *sib12_ptr,
 tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib11_ptr,
 tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib12_ptr
);

/*===========================================================================

FUNCTION    RRCMEAS_INITIALIZE_TRANSACTION_ID_LIST

DESCRIPTION

 Initialises transaction id list

 None.

RETURN VALUE

 void

SIDE EFFECTS

 None

===========================================================================*/

void tdsrrcmeas_initialize_transaction_id_list
(
 void
);



/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO

DESCRIPTION

    Process traffic volume information in a Meas
    Ctrl Message. Also sends a Meas Ctrl Failure
    Message in case of an error


DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r5
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
);

/*===========================================================================

FUNCTION   RRCMEAS_RETRIEVE_MEAS_OWNER_R5

DESCRIPTION

 Finds the owner of measurement identity given in a MCM Rel 5 Msg

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

tdsrrcmeas_layer_e_type tdsrrcmeas_retrieve_meas_owner_r5
(
tdsrrc_MeasurementCommand_r4* mcm_ptr,
uint32 tdsmeas_id
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
,tdspos_info_struct_type* pos_info_ptr
#endif
);

/*===========================================================================

FUNCTION   tdsrrcmeas_process_mcm_r5

DESCRIPTION

    This function processed the Release 5 Meas Ctrl Message

DEPENDENCIES

    None

RETURN VALUE
   Void


SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcmeas_process_mcm_r5
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION  RRCMEAS_PROCESS_MCM_INTRA_F_CELL_INFO_R5

DESCRIPTION

 This function processes the Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 The function processes Cell Info in ASN1 format and translates
 into the L1RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill mcm cell info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_process_mcm_intra_f_cell_info_r5
(
tdsrrc_IntraFreqCellInfoList_r4* intra_freq_cell_info_list_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_f_mcm_ptr
);

/*===========================================================================

FUNCTION  RRCMEAS_FILL_COMMON_MCM_INTRA_F_INFO_R5

DESCRIPTION

 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 The function processes Cell Info in ASN1 format and translates
 into the L1RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_fill_common_mcm_intra_f_info_r5
(
tdsrrc_IntraFrequencyMeasurement_r4* intra_f_meas_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_f_mcm_ptr,
tdsrrc_MeasurementControl_v590ext_IEs* mcm_ext_ptr
);

/*===========================================================================

FUNCTION  RRCMEAS_FILL_MCM_CELL_INFO_R5

DESCRIPTION

 This function fills in Cell Info received from a R5
 Measurement Control Message. The function processes
 Cell Info in ASN1 format and translates into the L1
 RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill cell info correctly.
 SUCCESS: If cell info is filled correctly

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_mcm_cell_info_r5
(
tdsrrc_CellInfo_r4* mcm_cell_info_ptr,
tdsl1_cell_info_struct_type* l1_cell_info_ptr
);

/*===========================================================================

FUNCTION     RRCMEAS_FILL_INTRA_FREQ_EVENT_CRITERIA_R5

DESCRIPTION

 This function fills in intra-freq event criteria for a Non-HCS cell when
 Cell Selection Reselection Quality Measure is RCSP or ECNO for R5 Message
 The function processes intra-ftreq event criteria Info in ASN1 format
 and translates into the L1 RRC Interface format.

DEPENDENCIES

 The passed pointers must be valid.

RETURN VALUE

 No of events.

SIDE EFFECTS

 None

===========================================================================*/

uint16 tdsrrcmeas_fill_intra_freq_event_criteria_r5
(
tdsrrc_IntraFreqReportingCriteria_r4  *rrc_intra_freq_reporting_criteria_ptr,
tdsl1_intra_freq_event_crit_struct_type *l1_intra_freq_event_crit_ptr,
tdsrrc_MeasurementControl_v590ext_IEs* mcm_ext_ptr
);

/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_INTRAFREQ_R5

DESCRIPTION

 Processed Intra Freq Meas in Rel5 Meas Ctrl Message

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/
uecomdef_status_e_type tdsrrcmeas_process_intrafreq_r5
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
);

/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R5

DESCRIPTION

 This function processes the Measurement Control Message rel 5

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r5
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCUEIM_FILL_MCM_INFO_R5

DESCRIPTION

 This function fills the Measurement Control information into the L1 structure

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_fill_mcm_info_r5
(
  tdsrrc_UE_InternalMeasurement_r4 *ue_InternalMeasurement_ptr,
  tdsl1_ue_internal_meas_ctrl_struct_type *ue_internal_ptr,
  uint8 tdstransaction_id,
  tdsrpt_crit_enum_type *mcm_rpt_crit_ptr
);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R5

DESCRIPTION

    This function processes the release 5 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r5
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION  RRCIFREQ_FILL_INTER_FREQ_INFO_R5

DESCRIPTION

 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 Also updates the Inter Freq Cell Info List for rel 5

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcifreq_fill_inter_freq_info_r5
(
 tdsrrc_InterFreqCellInfoList_r4* interFreqCellInfoList_ptr,
 tdsl1_meas_ctrl_parm_struct_type* l1_meas_ptr,
 tdsrrc_NewInterFreqCellList_v7b0ext *newInterFreqCellList_ptr
);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R5

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r5
(
 tdsrrc_InterFreqReportCriteria_r4* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr,
  tdsrrc_MeasurementControl_v590ext_IEs* mcm_590_ext_ptr
);
/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R7

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r7
(
 tdsrrc_InterFreqReportCriteria_r7* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr
);

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R5

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r5
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_CELL_LIST_MCM_R5

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If Cell list is invalid
  SUCCESS: If Cell list is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_cell_list_mcm_r5
(
  tdsrrc_InterRATCellInfoList_r4* mcm_cell_list_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
);


/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R5

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 5

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r5
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_POS_MCM_R5

DESCRIPTION

 This function processes the Positioning Measurement Control Message and
 determines if it is a A-GPS or Cell Id or OTDOA Message. Depending upon that
 it configures either LSM or L1 with measurement parameters

DEPENDENCIES

 None

RETURN VALUE

 FAILURE:
 SUCCESS:

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_process_pos_mcm_r5
(
  void* msg_ptr,
  tdspos_info_struct_type* pos_info_ptr,
  uint8 meas_ctrl_tr_id
);

/*===========================================================================

FUNCTION   RRCGPS_FILL_MCM_R5_INFO

DESCRIPTION

    This function fill the GPS structure for
    A-GPS Measurements.

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_fill_mcm_info_r5
(
  tdspos_info_struct_type* pos_info_ptr,
  tdsrrc_UE_Positioning_Measurement_r4 *up_Measurement_ptr,
  tdsrrc_meas_ctrl_struct_type *meas_ctrl_params_ptr,
  uint8 tdstransaction_id,
  boolean modify_meas_params_incl
);

/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R5

DESCRIPTION

    This function processes the rel 5 Measurement Control Message for
    A-GPS Measurements.

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm_r5
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
);
#endif





/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm
(
  tdsrrc_MeasurementControl_r3_IEs* rrc_mcm_ptr
  ,uint8 meas_ctrl_tr_id
);

/*==========================================================================
 FUNCTION   RRCQM_FILL_MCM_INFO

DESCRIPTION

  This function fills the L1 structure for Quality Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_fill_mcm_info
(
  tdsrrc_QualityMeasurement *qualityMeasurement_ptr,
  tdsl1_qual_meas_ctrl_struct_type  *qual_ptr,
  uint8 meas_ctrl_tr_id,
  tdsrpt_crit_enum_type *mcm_rpt_crit_ptr
);

/*==========================================================================
 FUNCTION   RRCQM_INITIALIZE_MEAS_ID_LIST

DESCRIPTION

  This function initializes the measurement identity list for
  Quality Measurements

DEPENDENCIES

    None.

RETURN VALUE

    None

SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcqm_initialize_meas_id_list
(
 void
);

/*===========================================================================

FUNCTION   RRCQM_SEND_MEAS_REPORT

DESCRIPTION

 This function sends the Quality Measurement Report after validation

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

void tdsrrcqm_send_meas_report
(
 tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
);

/*===========================================================================

FUNCTION tdsrrcmeas_check_meas_id_used_for_different_meas_type

DESCRIPTION

 The function checks if the new meas id is already used for a different meas type
 and if so releases the measurement from the primary measurement database, from
 the additional measurement dtabase and send a release command to L1 to release
 the measurement.

DEPENDENCIES
 None

RETURN VALUE

 None

SIDE EFFECTS

 None

===========================================================================*/
void tdsrrcmeas_check_meas_id_used_for_different_meas_type
(
  tdsrrcmeas_layer_e_type meas_owner,
  uint16 tdsmeas_id
);

/*===========================================================================

FUNCTION   tdsrrcirat_add_irat_cell_info_ind

DESCRIPTION

  This function set the irat cell info ind in meas report.


DEPENDENCIES

  None

RETURN VALUE

  none

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcirat_add_irat_cell_info_ind
(
  tdsrrc_MeasurementReport* rpt_ptr
);

/*===========================================================================

FUNCTION   tdsrrcmeas_process_mcm_r6

DESCRIPTION

    This function processed the Release 5 Meas Ctrl Message

DEPENDENCIES

    None

RETURN VALUE
   Void


SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcmeas_process_mcm_r6
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCMEAS_RETRIEVE_MEAS_OWNER_R6

DESCRIPTION

 Finds the owner of measurement identity given in a MCM Rel 6 Msg

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

tdsrrcmeas_layer_e_type tdsrrcmeas_retrieve_meas_owner_r6
(
tdsrrc_MeasurementCommand_r6* mcm_ptr,
uint32 tdsmeas_id
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
,tdspos_info_struct_type* pos_info_ptr
#endif
);

/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_INTRAFREQ_R6

DESCRIPTION

 Processed Intra Freq Meas in Rel6 Meas Ctrl Message

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/
uecomdef_status_e_type tdsrrcmeas_process_intrafreq_r6
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION  RRCMEAS_FILL_COMMON_MCM_INTRA_F_INFO_R6

DESCRIPTION

 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 The function processes Cell Info in ASN1 format and translates
 into the L1RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_fill_common_mcm_intra_f_info_r6
(
tdsrrc_IntraFrequencyMeasurement_r6* intra_f_meas_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_f_mcm_ptr
);

/*===========================================================================

FUNCTION     RRCMEAS_FILL_INTRA_FREQ_EVENT_CRITERIA_R6

DESCRIPTION

 This function fills in intra-freq event criteria for a Non-HCS cell when
 Cell Selection Reselection Quality Measure is RCSP or ECNO for R5 Message
 The function processes intra-ftreq event criteria Info in ASN1 format
 and translates into the L1 RRC Interface format.

DEPENDENCIES

 The passed pointers must be valid.

RETURN VALUE

 No of events.

SIDE EFFECTS

 None

===========================================================================*/

uint16 tdsrrcmeas_fill_intra_freq_event_criteria_r6
(
tdsrrc_IntraFreqReportingCriteria_r6  *rrc_intra_freq_reporting_criteria_ptr,
tdsl1_intra_freq_event_crit_struct_type *l1_intra_freq_event_crit_ptr

);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R6

DESCRIPTION

    This function processes the release 6 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r6
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R6

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r6
(
 tdsrrc_InterFreqReportCriteria_r6* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr
);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_EVENT_CRITERIA_R6

DESCRIPTION

    This function processes the event criteria received in a Measurement Control Message
  and stores in the format that is sent to L1

DEPENDENCIES

    None

RETURN VALUE

    FAILURE: If event criteria is invalid
    SUCCESS: If event criteria is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_event_criteria_r6
(
  tdsrrc_InterFreqReportingCriteria_r6* rrc_event_criteria_ptr,
  tdsl1_inter_freq_report_crit_struct_type* l1_event_criteria_ptr
);

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R6

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r6
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_CELL_LIST_MCM_R6

DESCRIPTION

  This function processes the cell list received in a Measurement Control Message

DEPENDENCIES

  None

RETURN VALUE

  FAILURE: If Cell list is invalid
  SUCCESS: If Cell list is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_cell_list_mcm_r6
(
  tdsrrc_InterRATCellInfoList_r6* mcm_cell_list_ptr,
  tdsl1_inter_sys_cell_list_struct_type* l1_cell_list_ptr
);

/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R6

DESCRIPTION

    Process traffic volume information in a Meas
    Ctrl Message R6. Also sends a Meas Ctrl Failure
    Message in case of an error


DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r6
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
);

/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R6

DESCRIPTION

 This function processes the Measurement Control Message rel 6

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r6
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);



#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R6

DESCRIPTION

    This function processes the rel 6 Measurement Control Message for
    A-GPS Measurements.

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm_r6
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
);

/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_POS_MCM_R6

DESCRIPTION

 This function processes the Positioning Measurement Control Message and
 determines if it is a A-GPS or Cell Id or OTDOA Message. Depending upon that
 it configures either LSM or L1 with measurement parameters

DEPENDENCIES

 None

RETURN VALUE

 FAILURE:
 SUCCESS:

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_process_pos_mcm_r6
(
  void* msg_ptr,
  tdspos_info_struct_type* pos_info_ptr,
  uint8 meas_ctrl_tr_id
);
#endif /* FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP_TDSCDMA */

/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R6

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 6

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r6
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*==========================================================================
 FUNCTION   tdsrrcifreq_free_memory_allocated_for_reporting

DESCRIPTION

  This function takes care of deallocating cell list which was allocated by L1 when it is 
  sending a inter frequency measurement indication to RRC
DEPENDENCIES

    None
RETURN VALUE

    None
SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_free_memory_allocated_for_reporting
(
  tdsl1_meas_rpt_struct_type* l1_meas_report_ptr
);

#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
/*===========================================================================

FUNCTION   RRCIFREQ_READ_RACH_RPT_INFO_SIB11

DESCRIPTION

    This function reads inter frequency rach repoting info from
    sib11. This function is called when the reading of inter frequency
    cell list from sib11 is successful and the rach reporting info is present
    in the sib11

DEPENDENCIES

    -

RETURN VALUE
FAILUE if rach repoting info reading fails
SUCCESS if rach reporting info rading is successful


SIDE EFFECTS

    None

===========================================================================*/
uecomdef_status_e_type tdsrrcifreq_read_rach_rpt_info_sib11(tdsrrc_InterFreqRACHReportingInfo *interFreqRACHReportingInfo,tdsl1_inter_freq_rach_rpt_type * l1_inter_freq_rach_rpt);

/*===========================================================================

FUNCTION   RRCIFREQ_READ_RACH_RPT_INFO_SIB12

DESCRIPTION

    This function reads inter frequency rach repoting info from
    sib12. This function is called when the reading of inter frequency
    cell list from sib12 is successful and the rach reporting info is present
    in the sib12

DEPENDENCIES

    -

RETURN VALUE
FAILUE if rach repoting info reading fails
SUCCESS if rach reporting info rading is successful


SIDE EFFECTS

    None

===========================================================================*/
uecomdef_status_e_type tdsrrcifreq_read_rach_rpt_info_sib12(tdsrrc_InterFreqRACHReportingInfo *interFreqRACHReportingInfo,tdsl1_inter_freq_rach_rpt_type * l1_inter_freq_rach_rpt);

/*===========================================================================

FUNCTION   RRCIFREQ_POPULATE_RACH_RPT-INFO

DESCRIPTION

    This function populates the meas_ptr with the rach reporting info 
    which is stored in tdsrrcifreq_inter_freq_rach_rpt
    This is called from rrcmeas when it is sending interfrequency cell list as
    state transition parameters to L1

DEPENDENCIES


RETURN VALUE
None

SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_populate_rach_rpt_info(tdsl1_meas_parm_type * meas_ptr);

/*===========================================================================

FUNCTION RRCIFREQ_APPEND_MEAS_RPT_ON_RACH

DESCRIPTION

 Appends the measured results to RACH messages if
 requested by the UTRAN. First, it checks if the
 meas report can be appended to the measge. If true,
 then it invokes a tdsl1mcmd_process_immediate_interf_meas_req to get
 intra-freq meas reports from L1.

DEPENDENCIES

 RACH reporting must have been activated by UTRAN in sib11 or sib12

RETURN VALUE

 FAILURE if RACH report not appended
 SUCCESS if RACH report appended


SIDE EFFECTS

 None

===========================================================================*/
uecomdef_status_e_type tdsrrcifreq_append_meas_rpt_on_rach(tdsrrc_MeasuredResultsOnRACHinterFreq * rrcifreq_rach_report_ptr, tdsrrc_proc_e_type proc_id);

/*===========================================================================

FUNCTION RRCIFREQ_GET_CELLID

DESCRIPTION

  This function returns the cell id for a given frequency and CPID

DEPENDENCIES



RETURN VALUE

Cellid if the cell is found
otherwise TDSL1_MAX_CELL_MEAS

SIDE EFFECTS

 None

===========================================================================*/
uint16 tdsrrcifreq_get_cellid(uint16 frequency,uint16 cpi);

/*===========================================================================

FUNCTION RRCIFREQ_ADJUST_CCCH_MESSAGE_LENGTH

DESCRIPTION

  This function removes some cells from the interfrequency measurements cell list 
  to adjust the message length

DEPENDENCIES



RETURN VALUE

SUCCESS if the adjustment if size is successful
FAILURE otherwise

SIDE EFFECTS

 None

===========================================================================*/
uecomdef_status_e_type tdsrrcifreq_adjust_ccch_message_length(void *msg_ptr,uint32 pdu_size);
#endif

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING

/*===========================================================================

FUNCTION RRCLBT_PROCEDURE_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  RRC GPS  procedure. This function is called by the
  dispatcher whenever an event is received for this module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrcgps_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION  

DESCRIPTION

  This function initializes the RRC GPS procedure.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcgps_init_procedure( void );
/*===========================================================================

FUNCTION  tdsrrcgps_set_psc_for_cell_id_req

DESCRIPTION

  This function updates the cpid for which cell id is requested
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcgps_set_cpid_for_cell_id_req
 (
   uint8 cpid
 );
/*===========================================================================

FUNCTION  tdsrrcgps_get_psc_for_cell_id_req

DESCRIPTION

  This function returns the cpid for which cell id is requested
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
uint8 tdsrrcgps_get_cpid_for_cell_id_req
(
 void
);

/*===========================================================================

FUNCTION  tdsrrcgps_is_gps_event_active

DESCRIPTION

  This function will return status of rrcgps event if active.
    
DEPENDENCIES

  None.
 
RETURN VALUE

TRUE : if GPS event is active,
FALSE : Otherwise

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcgps_is_gps_event_active
(
  void
);

#endif/*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
/*===========================================================================

FUNCTION tdsrrcmeas_read_sib11bis

DESCRIPTION

  Reads SIB11bis from the current active serving cell for all the measurements.
  If SIB11bis information is incorrect, meas setup command is not send to L1
  and UE does not do any measurements..

DEPENDENCIES

  SIB must have been stored by the SIB procedure

RETURN VALUE

  Success if SIB11bis has been correctly read
  Failure if SIB11bis has not been correctly read

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_read_sib11bis
(
  boolean sib18_present,
  tdsrrc_PLMNsOfIntraFreqCellsList * tmp_rrc_PLMNsOfIntraFreqCellsList_ptr
);

/*===========================================================================

FUNCTION tdsrrcifreq_read_sib11bis

DESCRIPTION

  Reads SIB11bis from the current active serving cell for all the measurements.
  If SIB11bis information is incorrect, meas setup command is not send to L1
  and UE does not do any measurements..

DEPENDENCIES

  SIB must have been stored by the SIB procedure

RETURN VALUE

  Success if SIB11bis has been correctly read
  Failure if SIB11bis has not been correctly read

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_read_sib11bis
(
  tdsl1_inter_freq_cell_list_struct_type* l1_inter_freq_sib_ptr,
  boolean sib18_present,
  tdsrrc_PLMNsOfInterFreqCellsList *tmp_rrc_PLMNsOfInterFreqCellsList_ptr
);

/*===========================================================================

FUNCTION tdsrrcirat_read_sib11bis

DESCRIPTION

  Reads SIB11bis from the current active serving cell for all the measurements.
  If SIB11bis information is incorrect, meas setup command is not send to L1
  and UE does not do any measurements..

DEPENDENCIES

  SIB must have been stored by the SIB procedure

RETURN VALUE

  Success if SIB11bis has been correctly read
  Failure if SIB11bis has not been correctly read

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_read_sib11bis
(
  tdsl1_meas_sib_parm_struct_type* l1_inter_rat_sib_ptr,
  boolean sib18_present,
  tdsrrc_PLMNsOfInterRATCellsList tmp_rrc_PLMNsOfInterRATCellsList_ptr
);
#endif /*FEATURE_TDSCDMA_SIB11BIS_SUPPORT*/

/*===========================================================================

FUNCTION  tdsrrcmeas_get_int_trans_index_for_trans_id

DESCRIPTION

  This function will return internal transaction index given the trans_id of the MCM and updates the current idx in accepted
  transactions as latest
    
DEPENDENCIES

  None.
 
RETURN VALUE

Internal transaction index

SIDE EFFECTS

  None.

===========================================================================*/
uint8 tdsrrcmeas_get_int_trans_index_for_trans_id
(
  uint8 tdstransaction_id
);


/*===========================================================================

FUNCTION   tdsrrcmeas_process_mcm_r7

DESCRIPTION

    This function processed the Release 7 Meas Ctrl Message

DEPENDENCIES

    None

RETURN VALUE
   Void


SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcmeas_process_mcm_r7
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCMEAS_RETRIEVE_MEAS_OWNER_R7

DESCRIPTION

 Finds the owner of measurement identity given in a MCM Rel 7 Msg

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/

tdsrrcmeas_layer_e_type tdsrrcmeas_retrieve_meas_owner_r7
(
tdsrrc_MeasurementCommand_r7* mcm_ptr,
uint32 tdsmeas_id
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
,tdspos_info_struct_type* pos_info_ptr
#endif
);

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R7

DESCRIPTION

    This function processes the rel 7 Measurement Control Message for
    A-GPS Measurements.

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm_r7
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
);

/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_POS_MCM_R7

DESCRIPTION

 This function processes the Positioning Measurement Control Message and
 determines if it is a A-GPS or Cell Id or OTDOA Message. Depending upon that
 it configures either LSM or L1 with measurement parameters

DEPENDENCIES

 None

RETURN VALUE

 FAILURE:
 SUCCESS:

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_process_pos_mcm_r7
(
  void* msg_ptr,
  tdspos_info_struct_type* pos_info_ptr,
  uint8 meas_ctrl_tr_id
);

/*===========================================================================

FUNCTION   RRCGPS_FILL_MCM_R7_INFO

DESCRIPTION

    This function fill the GPS structure for 
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_fill_mcm_info_r7
(
  tdspos_info_struct_type* pos_info_ptr,                       
  tdsrrc_UE_Positioning_Measurement_r7 *up_Measurement_ptr,
  tdsrrc_meas_ctrl_struct_type *meas_ctrl_params_ptr,
  uint8 tdstransaction_id,
  boolean modify_meas_params_incl
);

/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability_r7
(
  const tdsrrc_UE_Positioning_ReportingQuantity_r7 *rpt_quan_ptr
);

#ifdef FEATURE_CGPS_UMTS_CP_TDSCDMA

/*===========================================================================

FUNCTION
  RRCGPS_CONVERT_ASSISTANCE_DATA

DESCRIPTION
  This is a function takes a tdsrrc_UE_Positioning_GPS_AssistanceData structure 
  filled in by the oss_decode() function and returns a rrc_assistance_data
  structure. 

  The ASN.1 structure makes frequent use of linked lists. When these are encountered
  the data is copied into the appropriate array in the RRC structure.

PARAMETERS
  asn1_ptr - Pointer to ASN.1 data structure output from the OSS decoder
  rrc_ptr - Pointer to the RRC structure to be filled in

RETURN VALUE
  TRUE if conversion succeeded, FALSE if an error occurred.

===========================================================================*/
boolean tdsrrcgps_convert_assistance_data_r7( 
  tdsrrc_UE_Positioning_GPS_AssistanceData_r7* asn1_ptr,
  tdsrrc_assistance_data_struct_type* rrc_ptr 
);
  #endif
  
#endif /* FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP_TDSCDMA */

/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_INTRAFREQ_R7

DESCRIPTION

 Processed Intra Freq Meas in Rel7 Meas Ctrl Message

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/
uecomdef_status_e_type tdsrrcmeas_process_intrafreq_r7
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R7

DESCRIPTION

    This function processes the release 7 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r7
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R7

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R7. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r7
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
);

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R7

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r7
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R7

DESCRIPTION

 This function processes the Measurement Control Message rel 7

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r7
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R7

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 7

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r7
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION  RRCMEAS_FILL_COMMON_MCM_INTRA_F_INFO_R7

DESCRIPTION

 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 The function processes Cell Info in ASN1 format and translates
 into the L1RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_fill_common_mcm_intra_f_info_r7
(
tdsrrc_IntraFrequencyMeasurement_r7* intra_f_meas_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_f_mcm_ptr
);

/*===========================================================================

FUNCTION     RRCMEAS_FILL_INTRA_FREQ_EVENT_CRITERIA_R7

DESCRIPTION

 This function fills in intra-freq event criteria for a Non-HCS cell when
 Cell Selection Reselection Quality Measure is RCSP or ECNO for R7 Message
 The function processes intra-ftreq event criteria Info in ASN1 format
 and translates into the L1 RRC Interface format.

DEPENDENCIES

 The passed pointers must be valid.

RETURN VALUE

 No of events.

SIDE EFFECTS

 None

===========================================================================*/

uint16 tdsrrcmeas_fill_intra_freq_event_criteria_r7
(
tdsrrc_IntraFreqReportingCriteria_r7  *rrc_intra_freq_reporting_criteria_ptr,
tdsl1_intra_freq_event_crit_struct_type *l1_intra_freq_event_crit_ptr
);




#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================

FUNCTION   tdsrrcmeas_process_mcm_r8

DESCRIPTION

    This function processed the Release 8 Meas Ctrl Message

DEPENDENCIES

    None

RETURN VALUE
   Void


SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcmeas_process_mcm_r8
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 meas_ctrl_tr_id
);

/*===========================================================================
FUNCTION   RRCMEAS_RETRIEVE_MEAS_OWNER_R8

DESCRIPTION

 Finds the owner of measurement identity given in a MCM Rel 7 Msg

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/
tdsrrcmeas_layer_e_type tdsrrcmeas_retrieve_meas_owner_r8
(
tdsrrc_MeasurementCommand_r8* mcm_ptr,
uint32 tdsmeas_id
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
,tdspos_info_struct_type* pos_info_ptr
#endif
);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R8

DESCRIPTION

    This function processes the release 7 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r8
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================
FUNCTION   RRCMEAS_PROCESS_INTRAFREQ_R8

DESCRIPTION

 Processed Intra Freq Meas in Rel8 Meas Ctrl Message

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/
uecomdef_status_e_type tdsrrcmeas_process_intrafreq_r8
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R8

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R8. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r8
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
);

/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R8

DESCRIPTION 

 This function processes the Measurement Control Message rel 8

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r8
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_POS_MCM_R8

DESCRIPTION

 This function processes the Positioning Measurement Control Message and
 determines if it is a A-GPS or Cell Id or OTDOA Message. Depending upon that
 it configures either LSM or L1 with measurement parameters

DEPENDENCIES

 None

RETURN VALUE

 FAILURE:
 SUCCESS:

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_process_pos_mcm_r8
(
  void* msg_ptr,
  tdspos_info_struct_type* pos_info_ptr,
  uint8 meas_ctrl_tr_id
);

/*===========================================================================

FUNCTION   RRCGPS_PROCESS_MCM_R7

DESCRIPTION

    This function processes the rel 7 Measurement Control Message for
    A-GPS Measurements.

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_process_mcm_r8
(
  void* pos_mcm_ptr,
  tdspos_info_struct_type* p_info_ptr
  ,uint8 meas_ctrl_tr_id
);

/*===========================================================================

FUNCTION   RRCGPS_FILL_MCM_R8_INFO

DESCRIPTION

    This function fill the GPS structure for 
    A-GPS Measurements. 

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_fill_mcm_info_r8
(
  tdspos_info_struct_type* pos_info_ptr,                       
  tdsrrc_UE_Positioning_Measurement_r8 *up_Measurement_ptr,
  tdsrrc_meas_ctrl_struct_type *meas_ctrl_params_ptr,
  uint8 tdstransaction_id,
  boolean modify_meas_params_incl
);

/*===========================================================================

FUNCTION    RRCGPS_VERIFY_POSITIONING_CAPABILITY

DESCRIPTION

 Checks positioning capability to determine if UE could process
 A-GPS Measurement Control Message

DEPENDENCIES

 None.

RETURN VALUE

 Success/Failure

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcgps_verify_positioning_capability_r8
(
  const tdsrrc_UE_Positioning_ReportingQuantity_r8 *rpt_quan_ptr
);
#endif /*FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP_TDSCDMA*/

/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R8

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r8
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R8

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 8

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r8
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================
FUNCTION  tdsrrcifreq_fill_inter_freq_info_r8

DESCRIPTION
 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 Also updates the Inter Freq Cell Info List for rel 8

DEPENDENCIES
 None

RETURN VALUE
 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS
 None
===========================================================================*/
tdsrrcmeas_status_e_type tdsrrcifreq_fill_inter_freq_info_r8
(
 tdsrrc_InterFreqCellInfoList_r8* interFreqCellInfoList_ptr,
 tdsl1_meas_ctrl_parm_struct_type* l1_meas_ptr
);

#endif /*FEATURE_TDSCDMA_REL8*/
#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================

FUNCTION   tdsrrcmeas_process_mcm_r9

DESCRIPTION

    This function processed the Release 9 Meas Ctrl Message

DEPENDENCIES

    None

RETURN VALUE
   Void


SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcmeas_process_mcm_r9
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 meas_ctrl_tr_id
);
/*===========================================================================
FUNCTION   RRCMEAS_RETRIEVE_MEAS_OWNER_R9

DESCRIPTION

 Finds the owner of measurement identity given in a MCM Rel 9 Msg

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/
tdsrrcmeas_layer_e_type tdsrrcmeas_retrieve_meas_owner_r9
(
tdsrrc_MeasurementCommand_r9* mcm_ptr,
uint32 tdsmeas_id
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
  ,tdspos_info_struct_type* pos_info_ptr
#endif

);
/*===========================================================================
FUNCTION   RRCMEAS_PROCESS_INTRAFREQ_R9

DESCRIPTION

 Processed Intra Freq Meas in Rel8 Meas Ctrl Message

DEPENDENCIES

 None

RETURN VALUE

 None

SIDE EFFECTS

 None
===========================================================================*/
uecomdef_status_e_type tdsrrcmeas_process_intrafreq_r9
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);
/*===========================================================================

FUNCTION  RRCMEAS_PROCESS_MCM_INTRA_F_CELL_INFO_R9

DESCRIPTION

 This function processes the Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 The function processes Cell Info in ASN1 format and translates
 into the L1RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill mcm cell info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_process_mcm_intra_f_cell_info_r9
(
tdsrrc_IntraFreqCellInfoList_r9* intra_freq_cell_info_list_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_f_mcm_ptr
);


/*===========================================================================

FUNCTION  RRCMEAS_FILL_MCM_CELL_INFO_R9

DESCRIPTION

 This function fills in Cell Info received from a R5
 Measurement Control Message. The function processes
 Cell Info in ASN1 format and translates into the L1
 RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill cell info correctly.
 SUCCESS: If cell info is filled correctly

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_fill_mcm_cell_info_r9
(
tdsrrc_CellInfo_r9* mcm_cell_info_ptr,
tdsl1_cell_info_struct_type* l1_cell_info_ptr
);
/*===========================================================================

FUNCTION     RRCMEAS_FILL_INTRA_FREQ_EVENT_CRITERIA_R9

DESCRIPTION

 This function fills in intra-freq event criteria for a Non-HCS cell when
 Cell Selection Reselection Quality Measure is RCSP or ECNO for R9 Message
 The function processes intra-ftreq event criteria Info in ASN1 format
 and translates into the L1 RRC Interface format.

DEPENDENCIES

 The passed pointers must be valid.

RETURN VALUE

 No of events.

SIDE EFFECTS

 None

===========================================================================*/

uint16 tdsrrcmeas_fill_intra_freq_event_criteria_r9
(
tdsrrc_IntraFreqReportingCriteria_r9  *rrc_intra_freq_reporting_criteria_ptr,
tdsl1_intra_freq_event_crit_struct_type *l1_intra_freq_event_crit_ptr
);
/*===========================================================================

FUNCTION  RRCMEAS_FILL_COMMON_MCM_INTRA_F_INFO_R9

DESCRIPTION

 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 The function processes Cell Info in ASN1 format and translates
 into the L1RRC Interface format.

DEPENDENCIES

 None

RETURN VALUE

 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS

 None

===========================================================================*/

tdsrrcmeas_status_e_type tdsrrcmeas_fill_common_mcm_intra_f_info_r9
(
tdsrrc_IntraFrequencyMeasurement_r9* intra_f_meas_ptr,
tdsl1_intra_freq_meas_ctrl_struct_type* l1_intra_f_mcm_ptr
);
/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_MCM_R9

DESCRIPTION

    This function processes the release 9 Measurement Control Message for
  Inter-Freq Measurements

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_mcm_r9
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*===========================================================================

FUNCTION   RRCIFREQ_PROCESS_REPORTING_CRITERIA_R9

DESCRIPTION

    This function process the reporting criteria specified
  in Measurement Control Message

DEPENDENCIES

    None

RETURN VALUE

    Success or Failure depending upon processing of reporting criteria

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcifreq_process_reporting_criteria_r9
(
 tdsrrc_InterFreqReportCriteria_r9* ifreq_rpt_criteria_ptr,
 tdsl1_inter_freq_meas_ctrl_struct_type* l1_rpt_params_ptr
);
/*===========================================================================
FUNCTION  tdsrrcifreq_fill_inter_freq_info_r9

DESCRIPTION
 This function fills in common Cell Info for meas setup and
 meas modify command type in a Measurement Control Message.
 Also updates the Inter Freq Cell Info List for rel 9

DEPENDENCIES
 None

RETURN VALUE
 FAILURE: If unable to fill common mcm info correctly
 SUCCESS: If common mcm info filled correctly

SIDE EFFECTS
 None
===========================================================================*/
tdsrrcmeas_status_e_type tdsrrcifreq_fill_inter_freq_info_r9
(
 tdsrrc_InterFreqCellInfoList_r9* interFreqCellInfoList_ptr,
 tdsl1_meas_ctrl_parm_struct_type* l1_meas_ptr
);
/*===========================================================================

FUNCTION   RRCIRAT_PROCESS_MCM_R9

DESCRIPTION

  This function processes the Measurement Control Message

DEPENDENCIES

  None.

RETURN VALUE

  FAILURE: If Meas Ctrl is invalid
  SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrcirat_process_mcm_r9
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);
/*===========================================================================

FUNCTION     RRCTVM_PROCESS_TRAFFIC_VOL_INFO_R9

DESCRIPTION  

    Process traffic volume information in a Meas
    Ctrl Message R9. Also sends a Meas Ctrl Failure
    Message in case of an error
    
                    
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/
tdsrrctvm_status_e_type tdsrrctvm_process_traffic_vol_info_r9
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tid
);
/*===========================================================================

FUNCTION   RRCUEIM_PROCESS_MCM_R9

DESCRIPTION 

 This function processes the Measurement Control Message rel 9

DEPENDENCIES

 None

RETURN VALUE

 Success if message successfully processed
 Failure otherwise 

SIDE EFFECTS

 None
===========================================================================*/

uecomdef_status_e_type tdsrrcueim_process_mcm_r9
(
  tdsrrc_cmd_type* cmd_ptr,
  uint8 tdstransaction_id
);

/*==========================================================================
 FUNCTION   RRCQM_PROCESS_MCM_R9

DESCRIPTION

  This function processes the Measurement Control Message for
  Quality Measurements for rel 9

DEPENDENCIES

    None.

RETURN VALUE

    FAILURE: If Meas Ctrl is invalid
    SUCCESS: If Meas Ctrl is valid.

SIDE EFFECTS

    None

===========================================================================*/

uecomdef_status_e_type tdsrrcqm_process_mcm_r9
(
  tdsrrc_cmd_type *cmd_ptr,
  uint8 tdstransaction_id
);
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION   RRCMEAS_PROCESS_POS_MCM_R9

DESCRIPTION

 This function processes the Positioning Measurement Control Message and
 determines if it is a A-GPS or Cell Id or OTDOA Message. Depending upon that
 it configures either LSM or L1 with measurement parameters

DEPENDENCIES

 None

RETURN VALUE

 FAILURE:
 SUCCESS:

SIDE EFFECTS

 None

===========================================================================*/

uecomdef_status_e_type tdsrrcmeas_process_pos_mcm_r9
(
  void* msg_ptr,
  tdspos_info_struct_type* pos_info_ptr,
  uint8 meas_ctrl_tr_id
);

#endif

#endif /* FEATURE_TDSCDMA_REL9 */

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY

#define RRC_MAX_UTRA_DED_PRI_LAYERS 32
#define RRC_MAX_EUTRA_DED_PRI_LAYERS 32
#define RRC_MAX_GSM_DED_PRI_LAYERS 128

typedef struct
{
  uint32 frequency;/*ARFCN/URAFCN/EARFCN*/
  uint8 priority;
  geran_grr_band_ind_e band_ind;
}tdsrrc_frequency_layer_priority_info_type;

typedef struct
{
  /*If the dedicated priority information is valid or not*/
  boolean is_valid;

  /*
    TRUE if the variables are updated and DED_PRI_CHANGE_IND is not yet processed 
    FALSE otherwise
  */
  boolean dedicated_priority_info_crit_sec;

  /*PLMN in which the dedicated priorties are received*/
  rrc_plmn_identity_type plmn_id;
/*validity Timer value in minutes, 0 if timer is not present*/
  uint64 validity_timer; 

  uint8 num_utra_layers;
  tdsrrc_frequency_layer_priority_info_type utra_priority_layers[RRC_MAX_UTRA_DED_PRI_LAYERS];

  uint8 num_gsm_layers;
  tdsrrc_frequency_layer_priority_info_type gsm_priority_layers[RRC_MAX_GSM_DED_PRI_LAYERS];

  uint8 num_eutra_layers;
  tdsrrc_frequency_layer_priority_info_type eutra_priority_layers[RRC_MAX_EUTRA_DED_PRI_LAYERS];

  uint32 num_utra_tdd_freq;         /*!< Number of UTRA-TDD frequencies */
  lte_irat_reselection_priority_s utra_tdd_freq_list[LTE_IRAT_MAX_UTRAN_FREQ];
  uint32 num_hrdp_band_class;
  lte_irat_cdma_reselection_priority_s cdma_hrpd_list[LTE_IRAT_MAX_CDMA_BAND_CLASS];
  uint32 num_1x_band_class;
  lte_irat_cdma_reselection_priority_s cdma_1x_list[LTE_IRAT_MAX_CDMA_BAND_CLASS];
}tdsrrc_dedicated_priority_info_type;
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
typedef struct
{
  uint32 earfcn;
}tdsrrcirat_eutra_freq_type;

#define TDSRRCMEAS_GET_EUTRA_MEAS_BANDWIDTH(bandwidth)\
    ((bandwidth == tdsrrc_EUTRA_MeasurementBandwidth_mbw6) ? TDSL1_MBW6 :\
      ((bandwidth == tdsrrc_EUTRA_MeasurementBandwidth_mbw15) ? TDSL1_MBW15 :\
        ((bandwidth == tdsrrc_EUTRA_MeasurementBandwidth_mbw25) ? TDSL1_MBW25: \
          ((bandwidth == tdsrrc_EUTRA_MeasurementBandwidth_mbw50) ? TDSL1_MBW50: \
            (bandwidth == tdsrrc_EUTRA_MeasurementBandwidth_mbw75) ? TDSL1_MBW75 : TDSL1_MBW100\
            )\
          )\
        )\
      )\


#endif

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
/*===========================================================================

FUNCTION   tdsrrcmeas_configure_l1_with_priority_info_eutra_cell_list

DESCRIPTION 
  This function uses stored dedicated priority info and SIB19 to configure WL1 with priority info and E-UTRA cell list

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_configure_l1_with_priority_info_eutra_cell_list
(
  void
);

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
/*===========================================================================

FUNCTION   tdsrrcmeas_handle_get_dedicated_pri_req

DESCRIPTION 
  This function handles the get dedicated priorities req from inter RAT

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_handle_get_dedicated_pri_req
(
  tdsrrc_cmd_type* cmd_ptr
);
#endif

#ifdef FEATURE_TDSCDMA_TO_LTE
/*===========================================================================

FUNCTION   tdsrrcirat_initialize_eutra_freq_list

DESCRIPTION 
  This function initializes EUTRA frequency list stored

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcirat_initialize_eutra_freq_list
(
  void
);
/*===========================================================================

FUNCTION   tdsrrcirat_update_eutra_freq_list

DESCRIPTION 
  This function updates stored EUTRA frequency with EUTRA freq received in the argument

DEPENDENCIES
  None

RETURN VALUE
  SUCCESS : If EUTRA frequency is updated in the DB
  FAILURE   : Otherwise

SIDE EFFECTS
  None

===========================================================================*/
uecomdef_status_e_type tdsrrcirat_update_eutra_freq_list
(
  uint32 earfcn
);
#endif
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_DEPRIORITIZATION
boolean tdsrrc_is_earfcn_in_deprio_list
(
  uint32 earfcn
);
#endif
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) 
/*===========================================================================

FUNCTION   tdsrrcirat_is_meas_id_eutra_meas

DESCRIPTION
  This function returns TRUE if the meas id is EUTRA meas id

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the meas id is found in DB and is EUTRA measurement
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrcirat_is_meas_id_eutra_meas
(
  uint16 meas_id
);

#endif

/*===========================================================================

FUNCTION   tdsrrcmeas_trash_and_send_meas_failure

DESCRIPTION
  This function send MCF for all active meas_ctrl msgs

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcmeas_trash_and_send_meas_failure(void);

#ifdef FEATURE_GSM_QUADBAND
sys_band_T tdsrrcirat_get_gsm_band(tdsl1_gsm_cell_info_type);
#endif

#ifdef FEATURE_TDSCDMA_REL9
/*===========================================================================

FUNCTION   tdsrrcmeas_check_all_statusFlag_deact_in_DMO_config

DESCRIPTION 
  This is a fault-tolerance protection function when R9 MCM received by UE and
  DMO feature is disabled.
  Return TRUE if all statusFlag in DMO config are deactivate. False otherwise.
   
DEPENDENCIES
  None

RETURN VALUE
  Boolean
s
SIDE EFFECTS
  None

===========================================================================*/

boolean tdsrrcmeas_check_all_statusFlag_deact_in_DMO_config
(
  tdsrrc_CellDCHMeasOccasionInfo_TDD128_r9* dmo_ptr
);
#endif

#endif /* RRCMEASI_H */
