#ifndef LOC_LTE_OTDOA_LOG_H
#define LOC_LTE_OTDOA_LOG_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      Location LTE OTDOA Log Module Header

GENERAL DESCRIPTION
This file contains Location LTE OTDOA logging header.

===========================================================================
 
  Copyright (c) 2017 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================

Version Control

$Header: //components/rel/gnss8.mpss/11.1/gnss/wwanme/lte_otdoa/5_0/src/loc_lte_otdoa_log.h#1 $
$DateTime: 2019/10/24 01:30:36 $
$Author: pwbldsvc $

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*--------------------------------------------------------------------------
 * Include Files
 *-----------------------------------------------------------------------*/
#include "gnss_common.h"
#include "loc_lte_otdoa_common_defs.h"
#include "loc_lte_otdoa_control.h"
#include "loc_lte_otdoa_measproc.h"
#include "loc_lte_otdoa_gnss_api.h"
#include "loc_lte_otdoa_api.h"

/*--------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *-----------------------------------------------------------------------*/

/* 12-bit CER vectors will be packed into 32-bit words */
#define LOC_LTE_OTDOA_PACKED_CER                      (0U<<0)

/* 12-bit CER vectors will occupy full 32-bit word */
#define LOC_LTE_OTDOA_UNPACKED_CER                    (1U<<0)

/* Log types for AssitData log */
#define LLOC_AD_FROM_LOC_SERVER       0
#define LLOC_AD_UPDATE_TO_ML1         1
#define LLOC_AD_TO_ML1_AFTER_SERV_CELL_UPDATE 2
#define LLOC_AD_TO_ML1_PRS_SCANNER            3
#define LLOC_AD_TO_ML1_AFTER_MAX_SRCH_SIZE    4
#define LLOC_AD_TO_ML1_PARTIAL_UPDATE         5

/* Mask to log LTE supported bands in AD log packet */
#define LLOC_AD_LOG_LTE_BANDS_INFO_VALID  (1<<0)

/* For raw CER log packet, log the VSRC/OSTMR info only for PCC and SCC0/SCC1 carriers.
 * This is less than the actual number of LLOC_MAX_LTE_CARRIERS which is 5 */
#define LLOC_MAX_LTE_CARRIERS_LOGGING   3

/* log packet version IDs */
#define LLOC_PRS_TOA_LOG_VERSION              16
#define LLOC_PRS_RSTD_LOG_VERSION             7
#define LLOC_PRS_ASSIST_DATA_LOG_VERSION      9
#define LOC_LTE_OTDOA_PRS_OCCASION_MEAS_LOG_VERSION     16

/* Todo/To do/Note: TDP is using OTDOA DEBUG log packet with version 'WWANME_TDP_FW_PROFILE_LOG_VERSION 0'.
 * To differentiate with TDP, use version '51' for OTDOA events */
#define LLOC_OTDOA_EVENTS_LOG_VERSION         51
#define LLOC_SM_START_EVENT_LOG_SUBVER          1
#define LLOC_SM_STOP_EVENT_LOG_SUBVER           1
#define LLOC_SM_TIMER_UPDATE_EVENT_LOG_SUBVER   1
#define LLOC_SM_REQ_RPT_EVENT_LOG_SUBVER        1
#define LLOC_SM_OTA_RESET_EVENT_LOG_SUBVER      1
#define LLOC_ML1_SCELL_UPDATE_EVENT_LOG_SUBVER  1
#define LLOC_ML1_MEAS_AVAIL_EVENT_LOG_SUBVER    1
#define LLOC_ML1_ERROR_EVENT_LOG_SUBVER         1
#define LLOC_OTDOA_RSTD_RPT_EVENT_LOG_SUBVER    1
#define LLOC_OTDOA_DIAG_EVENT_LOG_SUBVER        1
#define LLOC_OTDOA_STATS_EVENT_LOG_SUBVER       1
#define LLOC_SM_FEATURE_STATUS_EVENT_LOG_SUBVER       1

/* Bit masks for logging. Use 3 MSBs to indicate modem type */
#define LLOC_NIKEL_MODEM                       (0<<29)
#define LLOC_DIME_MODEM                        (1<<29)
#define LLOC_BOLT_MODEM                        (2<<29)
#define LLOC_TRITON_MODEM                      (3<<29)
#define LLOC_TABASCO_MODEM                     (4<<29)
#define LLOC_THOR_MODEM                        (5<<29)
#define LLOC_ATLAS_MODEM                       (6<<29)
#define LLOC_JOLOKIA_MODEM                     (7<<29)
#define LLOC_WIN_ADJ_VALID                     (1U<<28)
#define LLOC_WIN_ADJ_INVALID                   (0U<<28)
#define LLOC_MAX_PEAK_SNR_VALID                (1U<<27)
#define LLOC_NC_MEASUREMENT_LOG                (1U<<26)
#define LLOC_MEAS_STATUS_VALID                 (1U<<25)
#define LLOC_SIGNAL_STRENGTH_VALID             (1U<<24)
#define LLOC_PETL_VALID                        (1U<<23)
#define LLOC_PETL_INVALID                      (0U<<23)
#define LLOC_REFTIME_VALID                     (1U<<20)
#define LLOC_VSRC_OSTMR_DIFF_VALID             (1U<<18)
#define LLOC_RSTD_EARLY_RPT                    (1U<<0)
#define LLOC_TOA_ADJ_MIXED_BW_VALID            (1U<<17)
#define LLOC_REF_TOA_FOR_RSTD_VALID            (1U<<1)

/* Data length for logging DIAG events */
#define LLOC_DIAG_LOG_EVENT_DATA_LEN    20

/* Enum to identify OTDOA events */
typedef enum
{
  LLOC_SM_OTDOA_START_EVENT,
  LLOC_SM_OTDOA_STOP_EVENT,
  LLOC_SM_TIMER_UPDATE_EVENT,
  LLOC_SM_REQUEST_RPT_EVENT,
  LLOC_SM_OTA_RESET_EVENT,
  LLOC_ML1_SCELL_UPDATE_EVENT,
  LLOC_ML1_PRS_MEAS_AVAIL_EVENT,
  LLOC_ML1_ERROR_EVENT,
  LLOC_OTDOA_RSTD_RPT_TIMEOUT_EVENT,
  LLOC_OTDOA_DIAG_EVENT,
  LLOC_OTDOA_STATS_EVENT,
  LLOC_SM_FEATURE_STATUS_EVENT,
}
lloc_LogEventsEnumType;

 /*--------------------------------------------------------------------------
 * Type Declarations
 *-----------------------------------------------------------------------*/

#ifdef T_WINNT
#error code not present
#endif /* T_WINNT */

/* Structure to send OSTMR and VSRC values. We have array of [2] for Rx0 and Rx1
 * antennas */
typedef PACKED struct PACKED_POST
{
  boolean  b_Valid;
  uint16  w_Sfn;
  uint64  t_VsrcFromFW[2];
  uint32  q_OstmrFromFW[2];
  uint32  q_OstmrPhFromFW[2];
  uint64  t_RefDlTime;
}
llomp_OstmrVsrcInfoTypeLog;

/* Structure to store parameters for FFT residual signal detection. Valid only for TA */
typedef PACKED struct PACKED_POST
{
  FLT f_FaThresUpdate;
  uint32 q_NoiseFloor;
  FLT f_Sigma40;
  FLT f_Sigma60;
}
lloc_FftResidualSignalDetectionLogType;

/* log packet for the outputs of EAP module */
typedef PACKED struct PACKED_POST
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  uint8 u_Version;

  /* Mask for reserved fields */
  uint32 q_Mask;

  /* Time of arrival of the earliest arriving path (EAP). Units of Ts */
  int64 t_EapToa;

  /* Physical cell ID of cell used in PRS measurement Range ~ 0 to 503*/
  uint16  w_PhyCellId;

  /* Sub-sample interpolated peak location. Units of Ts/256 */
  uint8 u_FractionalTs;

  /* A metric for measurement uncertainty based on Cramer-Rao lower
   * bound calculation, but modified by the "double-hump" uncertainty for
   * search windows that are off by more than the cyclic prefix width from nominal.
   * Units of Ts */
  FLT f_MeasurementUnc;

  /* the maximum CER (Channel Energy Response) value for the
   * measured cell across multiple hypothesis */
  FLT f_MaxCer;

  /* the CER value corresponding to the TOA of the EAP (earliest arriving path) */
  FLT f_EapCer;

  /* Cell type - ref. or ngbr or serving cell */
  uint8 e_CellType;

  /* System frame number associated with measurement of cell. Range ~ 0 to 1023 */
  uint16 w_Sfn;

  /* Offset from EAP to center of search window. May be positive or negative.
   * Units of Ts. */
  int32 l_WinCenterAndToaDiff;

  /* Offset from Max. CER to center of search window. May be positive or negative.
   * Units of Ts. */
  int32 l_WinCenterAndMaxCerDiff;

  /* Search window size. Max. value 2047 (Ts units) */
  uint16 w_SrchWinSize;

  /* measured signal to noise ratio (using EAP CER). Unitless */
  uint16 w_EapSnr;

  /* FW reported signal to signal+noise ratio */
  uint32 q_AvgSSNRx0;
  uint32 q_AvgSSNRx1;

  /* Peak position offset from SFN boundary (this includes PeakIndex including all corrections) */
  int16 x_PeakOffsetFromSfn;

  /* Accumulated TTL Adjustment */
  int16 x_TtlAdj;

  /* LTE system BW in RBs */
  uint8 u_LteSysBw;

  /* Accumulated COM (center of mass) Adjustment (Fixed Arithmetic Q6 format) */
  int32 l_ComAdj;

  /* Accumulated MSTMR adjustment, in Ts units, Q0 format. */
  int64 t_MstmrAdj;

  /* EARFCN of the cell - based on uint32 */
  uint32 w_Earfcn;

  /* PRS occasion seq number and Session counter */
  uint32 q_OccasionSeqNum;
  uint32 q_LppSessionCtr;

  /* NC related */
  uint8 u_NCLevel;
  uint8 e_PrsOccasionType;
  uint8 e_SignalStrength;
  uint8 e_MeasState;

  /* Enum of type 'loc_lte_otdoa_PrsMeasProcErrorType' */
  uint8 e_ToaErrorStatus;

  /* MaxPeakSnr */
  uint16 w_MaxPeakSnr;

  /* PRS Gain Bias */
  int8 b_PrsGainBias;

  /* Noise floor associated with the meas. */
  FLT f_NoiseFloor;

  /* Flag to indicate if the meas. was done when a stronger offending
   * cell (same mod-6 value) was muted, and also the PCI of the offending
   * cell */
  boolean b_MeasWhenOffendingCellMuted;
  uint16 w_OffendingCellPci;
  uint32 w_OffendingCellEarfcn;

  uint8 u_ML1SchedFlags;
  FLT f_GlobalSNRdB;

  /* Carrier used for the measuremnt of this cell. It could be PCC, SCC1 or SCC2.
   * Range: 0 to 'LTE_LL1_CARRIER_COUNT-1'. As of now, 'LTE_LL1_CARRIER_COUNT' is defined as '3'
   * in 'modem_proc\fw_lte\api\intf_common.h' */
  uint8 u_Carrier; 

  /* PRS frequency meas. type. Info about whether the current cell was an intra-frequency meas.,
   * or inter-frequency meas. Of type 'loc_lte_otdoa_FreqMeasInfoType' */
  uint8 e_MeasType;

  /* OSTMR-VSRC info at reference instance (when RF was turned ON) */
  llomp_OstmrVsrcInfoTypeLog z_RefVsrcInfo;

  /* OSTMR-VSRC info at start of a PRS occasion */
  llomp_OstmrVsrcInfoTypeLog z_PrsOccVsrcInfo;

  /* Num hypothesis and best hyp */
  uint8 u_NumHypPerCell;
  uint8 u_BestHypIdx;

  /* DL ref time and PeakIndex within window (in Ts units) */
  int64 t_DlRefTime;
  FLT f_PeakIndex;

  /* Data for FFT residual signal detection. Valid only for TA modem */
  lloc_FftResidualSignalDetectionLogType z_FftResidualSigDet[LOC_LTE_OTDOA_MAX_HYPO_PER_CELL];

  /* Reserved fields */
  uint16 w_Reserved[LLOC_RESERVED_FIELDS_CNT];

  /* Threshold Limit for weighting Rx0 and Rx1 used in Q31 Format */ 
  uint32 q_ThresSbySplusN; 

  /* Single Rx Ratio in Q8 format*/ 
  uint8 u_SingleRxRatio;

  /* False alarm threshold used for detecting the cell. Scaled by 10000 */
  uint32 q_FaTh;
}
lloc_PrsToaMeasLog;

/* global cell ID definition */
typedef PACKED struct PACKED_POST
{
  uint8 u_Mcc[3];
  uint8 u_Mnc[3];
  uint32 q_CellIdentity;
  uint8 u_NumOfMncDigits;
  uint8 u_NumOfCellIdBits;
}
lloc_LogGlobalCellIdType;

/* Meas. quality definition. All fields are encoded.  */
typedef PACKED struct PACKED_POST
{
  /* Bit-mapped validity.
   * Bit 0 represents validity of u_NumSamples */
  uint8 u_Valid;

  /* error resolution. Range ~ 0 to 3.
   * This field specifies the resolution R used in 'u_ErrorValue' field.
   * The encoding on two bits is as follows:
   * '00'      5  meters
   * '01'      10 meters
   * '10'      20 meters
   * '11'      30 meters.
   */
  uint8 u_ErrorResolution;

  /* error value. Range ~ 0 to 31.
   * This field specifies the target device’s best estimate of the uncertainty
   * of the OTDOA (or TOA) measurement.
   * The encoding on five bits is as follows:
   *   '00000' 0     to   (R*1-1) meters
   *   '00001' R*1   to   (R*2-1) meters
   *   '00010' R*2   to   (R*3-1) meters
   *   .... (and so on)
   *   '11111' R*31   meters or more;
   * where R is the resolution defined by error-Resolution field.
   * Eg., R=20m corresponds to 0-19m, 20-39,..., 620+ m.
   */
  uint8 u_ErrorValue;

  /* num. of samples used for estimation. Range ~ 0 to 7
   * If the error-Value field provides the sample uncertainty of the OTDOA (or TOA) measurement,
   * this field specifies how many measurements have been used by the target device to determine
   * this (i.e., sample size). Following 3 bit encoding is used:
   *   ‘000’   Not the baseline metric
   *   '001'   5-9
   *   '010'   10-14
   *   '011'   15-24
   *   '100'   25-34
   *   '101'   35-44
   *   '110'   45-54
   *   '111'   55 or more.
   * In case of the value ‘000’, the error-Value field contains the target device’s best estimate
   * of the uncertainty of the OTDOA (or TOA) measurement not based on the baseline metric.
   * E.g., other measurements such as signal-to-noise-ratio or signal strength can be utilized to
   * estimate the error-Value. If this field is absent, the value of this field is ‘000’.
   */
  uint8 u_ErrorNumSamples;
}
lloc_LogMeasQualityType;

/* RSTD info for cells */
typedef PACKED struct PACKED_POST
{
  /* Bit-mapped validity.
   * Bit 0 represents validity of this cell meas.
   * Bit 1 represents validity of q_SFN
   * Bit 2 represents validity of z_GlobalCellId
   * Bit 3 represents validity of w_Earfcn
   * Bit 4 represents validity of w_Rstd
   * Bit 5 represents validity of z_MeasQual */
  uint32 u_Valid;

  /* This field specifies the subframe number of the RSTD reference cell containing the
   * starting subframe of the PRS occasion during which the most recent neighbour cell
   * RSTD measurement was performed. Range ~ 0 to 1023 */
  uint16 w_Sfn;

  /* physical cell ID  Range ~ 0 to 503 */
  uint16 w_PhyCellId;

  /* This field specifies the ECGI, the globally unique identity of a cell in E-UTRA,
   * of the measured cell */
  lloc_LogGlobalCellIdType z_GlobalCellId;

  /* This field specifies the E-UTRA carrier frequency of the measured cell.
   * based on uint32 */
  uint32 w_Earfcn;

  /* RSTD value for neighbor cells. Ts units. Range ~ 0 to 12711 */
  uint16 w_Rstd;

  /* TDOA value with decimal precision */
  FLT f_TdoaTs;

  /* Time of arrival of the earliest arriving path (EAP). Units of Ts */
  int64 t_EapToa;

  /* measured signal to noise ratio. Unitless */
  uint16 w_EapSnr;
  uint16 w_MaxPeakSnr;

  /* Meas quality (uncertainity.) */
  lloc_LogMeasQualityType z_MeasQuality;

  /* PRS occasion seq number and Session counter */
  uint32 q_OccasionSeqNum;

  /* NC related */
  uint8 e_SignalStrength;
  uint8 e_MeasState;

  /* Reserved */
  uint16 w_Reserved[LLOC_RESERVED_FIELDS_CNT];
}
lloc_LogRstdInfoType;

/* RSTD meas. log packet */
typedef PACKED struct PACKED_POST
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  uint8 u_Version;

  /* Number of elements in measurement neighbor list Range 1 to 24 */
  uint8 u_NumNeighborMeasurements;

  /* reference cell meas. */
  lloc_LogRstdInfoType z_RefCellMeasurements;

  /* neighbor cell meas. */
  lloc_LogRstdInfoType z_NgbrCellMeasurements[LOC_LTE_OTDOA_MAX_NGBR_RSTD_RPT];

  /* error status */
  uint8 e_ErrorInfo;

  /* time-to-fix */
  uint32 q_Ttf;

  /* Config data */
  uint32 q_ConfigData;

  /* Session counter */
  uint32 q_LppSessionCtr;

  /* Reserved */
  uint16 w_Reserved[LLOC_RESERVED_FIELDS_CNT];
}
lloc_PrsRstdMeasLog;

/* log packet structure for storing hypothesis parameters */
typedef PACKED struct PACKED_POST
{
  /* Symbol offset index */
  int8 b_SymbolOffsetIndex;

  /* Prune window size */
  uint16 w_PruneWinSize;

  /* Prune window start index */
  uint16 w_PruneWinStartIndex;
}
lloc_LogHypoParamsType;

/* log packet structure for assitance data per cell */
typedef PACKED struct PACKED_POST
{
  /* physical cell ID  Range ~ 0 to 503 */
  uint16 w_PhyCellId;

  /* This field specifies the E-UTRA carrier frequency of the RSTD reference cell.
   * based on uint32 */
  uint32 w_Earfcn;

  /* Antenna port config */
  uint8 e_AntennaPortConfig;

  /* PRS bandwidth */
  uint8 e_PrsBandwidth;

  /* cyclic prefix length */
  uint8 e_CpLength;

  /* Configuration index */
  uint16 w_PrsConfigIndex;

  /* Number of downlink subframes */
  uint8 e_NumDlFrames;

  /* Muting pattern */
  /* index will be either 2, 4, 8 or 16. This represents the number of
   * bits valid in muting pattern */
  boolean b_MutingValid;
  uint8 e_MutingIndex;
  uint16 w_MutingPattern;

  /* expected RSTD */
  uint16 w_ExpectedRstd;

  /* exptected RSTD unc. */
  uint16 w_ExpectedRstdUnc;

  /* Slot number and Subframe offset (for inter-freq) */
  uint8 u_SlotNumOffset;
  uint16 w_PrsSubframeOffset;

  /* number of hypothesis */
  uint8 b_NumHyp;

  /* hypothesis parameters */
  lloc_LogHypoParamsType z_Hyp[ LOC_LTE_OTDOA_MAX_HYPO_PER_CELL ];

  /* Num of consecutive meas. to be done */
  uint8 u_NumOfConsecutiveMeas;

  /* Global cell ID */
  lloc_LogGlobalCellIdType z_GlobalCellId;

  /* Is strong enough to cause x-corr */
  boolean b_IsOffendingCell;

  /* PRS Gain Bias */
  int8 b_PrsGainBias;

  /* Is PRS enough to cause x-corr  */
  uint8 b_IsPrsIcOffendingCell;

  /* SNR associated with PRS IC*/
  FLT f_GlobalSNRdB;

  /* Reserved */
  uint16 w_Reserved[4];
}
lloc_LogAssistDataPerCell;

/* log packet for logging assistance data sent to ML1 */
typedef PACKED struct PACKED_POST
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  uint8 u_Version;

  /* Number of unique EARFCNs present in assitance data. Max 3 */
  uint16 w_NumFreqElems;

  /* Number of elements in measurement neighbor list Range 1 to 72 */
  uint8 u_NumNgbrs;

  /* session info sent by SM layer */
  uint8 e_OpMode;
  uint16 e_SessType;

  /* assistance data for reference cell */
  lloc_LogAssistDataPerCell z_RefCell;

  /* assistance data for ngbrs */
  lloc_LogAssistDataPerCell z_NgbrCell[ LOC_LTE_OTDOA_MAX_NGBR_CELLS ];

  /* Report type */
  uint8 e_LogType;

  /* Session counter */
  uint32 q_LppSessionCtr;

  /* Reserved */
  uint32 q_Mask;
  uint16 w_Reserved[LLOC_RESERVED_FIELDS_CNT];
}
lloc_PrsAssistDataLog;

/* cell-specific PRS Meas descriptor */
typedef PACKED struct PACKED_POST
{
  /* Physical Cell ID corresponding to the CER vector.
   * Range: 0~503
   */
  uint16  w_CerCellId;

  /* Symbol Tap Offset */
  int8 b_SymbolOffset;

  /* Start Tap Index in Ts [0-2048] */
  uint16 w_WinStartIdx;

  /* PRS Search Window Prune Size in Ts [0-2048]*/
  uint16 w_WinPruneSize;

  /* S/S+N Ratio reported from FW for this Hypothesis in Q31 format */
  /* Chain 0 */
  uint32 q_AvgSSNRx0;
  /* Chain 1*/
  uint32 q_AvgSSNRx1;

  /* Threshold Limit for weighting Rx0 and Rx1 used in Q31 Format */ 
  uint32 q_ThresSbySplusN; 

  /* Single Rx Ratio in Q8 format*/ 
  uint8 u_SingleRxRatio;

  /* The number of the prs subframe per occasion used by FW */
  uint8 e_NumDlFrames;

  /* Cell Carrier frequency - based on uint32 */
  uint32 w_Earfcn;

  /* PRS Gain Bias */
  int8 b_PrsGainBias;

  /* Flag to indicate if the meas. was done when a stronger offending
   * cell (same mod-6 value) was muted, and also the PCI of the offending
   * cell */
  boolean b_MeasWhenOffendingCellMuted;
  uint16 w_OffendingCellPci;
  uint32 w_OffendingCellEarfcn;

  uint8 u_ML1SchedFlags;

  /* PRS BW */
  uint8 u_PrsBw;
}
loc_lte_otdoa_PrsCellDescLog;

/* occasion-specific PRS Meas descriptor */
typedef PACKED struct PACKED_POST
{
  /* LTE downlink timeStamp aligned to frame boundary of PRS Occasion,
   * time read from 32-bit VSRC sample counter.
   * (alternatively 64-bit DL Link counter)
   */
  int64  r_DlTimestamp;

  /* Accumulated TTL Adjustment */
  int16  x_TtlAdj;

  /* cell ID of the serving sector in this PRS occasion */
  uint16  w_ServCellId;

  /* PRS Meas Result Sequence Number. This will be used
   * by GNSS SW to check if the data they read have been
   * overwritten or not.
   */
  uint32 q_OccasionSeqNum;

  /* The number of hypothesis in this PRS occasion, including other cells in this PRS occasion */
  uint8 u_NumHypInOccasion;

  /* Header containing relevant info for PRS measurement vector */
  loc_lte_otdoa_PrsCellDescLog z_CellInfo;

  /* LTE system BW in RBs */
  uint8 u_LteSysBw;

  /* Accumulated COM (center of mass) Adjustment (Fixed Arithmetic Q6 format) */
  int32 l_ComAdj;

  /* Accumulated MSTMR adjustment, in Ts units, Q0 format. */
  int64 t_MstmrAdj;

  /* SFN of the measured PRS occasion */
  uint16 w_Sfn;

  /* NC related info */
  uint8 u_NCLevel;
  uint8 e_MeasState;
  uint8 e_PrsOccasionType;

  /* Carrier used for the measuremnt of this cell. It could be PCC, SCC1 or SCC2.
   * Range: 0 to 'LTE_LL1_CARRIER_COUNT-1'. As of now, 'LTE_LL1_CARRIER_COUNT' is defined as '3'
   * in 'modem_proc\fw_lte\api\intf_common.h' */
  uint8 u_Carrier; 

  /* PRS frequency meas. type. Info about whether the current cell was an intra-frequency meas.,
   * or inter-frequency meas. Of type 'loc_lte_otdoa_FreqMeasInfoType' */
  uint8 e_MeasType;

  /* OSTMR-VSRC info at reference instance (when RF was turned ON). We have array of 3, for
   * each carrier. I.e. PCC, SCC0, SCC1 */
  llomp_OstmrVsrcInfoTypeLog z_RefVsrcInfo[LLOC_MAX_LTE_CARRIERS_LOGGING];

  /* OSTMR-VSRC info at start of a PRS occasion. We have array of 3, for
   * each carrier. I.e. PCC, SCC0, SCC1*/
  llomp_OstmrVsrcInfoTypeLog z_PrsOccVsrcInfo[LLOC_MAX_LTE_CARRIERS_LOGGING];
}
loc_lte_otdoa_PrsMeasOccasionDescLog;

/* PRS occasion meas. log packet */
typedef PACKED struct PACKED_POST
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  uint8 u_Version;

  /* Hypothesis seq num of this cell and Num of hyp for this cell */
  uint8 u_HypothesesSeqNum;
  uint8 u_NumHypForCell;

  /* configuration vector */
  uint32 q_ConfigData;

  /* PRS Occasion Meas Descriptor */
  loc_lte_otdoa_PrsMeasOccasionDescLog z_OccasionInfo;

  /* Session counter */
  uint32 q_LppSessionCtr;

  /* Reserved fields */
  uint16 w_Reserved[LLOC_RESERVED_FIELDS_CNT];

  /* Individual Cell Data */
  uint8 u_RawCer[1];
}
loc_lte_otdoa_PrsOccasionMeasLog;

/* Log packet header for logging OTDOA events */
typedef PACKED struct PACKED_POST
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  uint8 u_Version;

  /* Sub-Version number for the event */
  uint8 u_EventSubVer;

  /* Event type. Same as enum type 'lloc_LogEventsEnumType' */
  uint8 e_EventType;
  
  /* OTDOA state. Enum type 'lloc_StateType' */
  uint8 e_State;

  /* OTDOA LPP session number */
  uint32 q_LppSessNum;
} lloc_EventsLogHdrType;

/* Log packet definition for SM start command */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* LTE Subscription ID as defined in structure 'sys_modem_as_id_e_type' in
   * file 'modem_proc\mmcp\api\public\sys.h' */
  int8 b_SubId;

  /* OTDOA op mode and session type */
  uint8 e_OpMode;
  uint16 e_SessType;
  
  /* NV config */
  uint8 b_NonCohEnable;
  uint8 b_PrsIcEnable;
  uint8 b_InflateSrchWinSizeConfig;
} lloc_EventsLogSmStartType;

/* Log packet definition for SM stop command */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* LTE Subscription ID as defined in structure 'sys_modem_as_id_e_type' in
   * file 'modem_proc\mmcp\api\public\sys.h' */
  int8 b_SubId;

  uint8 u_Reason;
  uint16 e_SessType;
} lloc_EventsLogSmStopType;

/* Log packet definition for SM timer update */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* Timeout in milli seconds */
  uint32 q_Timeout;
  uint16 e_SessType;
} lloc_EventsLogSmUpdateTimeoutType;

/* Log packet definition for SM request report command */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  uint16 e_SessType;
  boolean b_IsEarlyRptReq;
} lloc_EventsLogSmReqRptType;

/* Log packet definition for SM OTA reset command */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* LTE Subscription ID as defined in structure 'sys_modem_as_id_e_type' in
   * file 'modem_proc\mmcp\api\public\sys.h' */
  int8 b_SubId;
} lloc_EventsLogSmOtaResetType;

/* Log packet definition for ML1 update scell info event */
typedef PACKED struct PACKED_POST
{
  /* Carrier info */
  boolean b_CarrierInfoValid; //TRUE for PCC. TRUE for SCC if CA is activated/configured
  
  /* Physical cell id */
  uint16 w_PhyCellId;

  /* E-ARFCN */
  uint32 w_Earfcn;

  /* PRS bandwidth in RBs */
  uint8 u_Bandwidth;

  /* Cyclic prefix. Enum of type 'loc_lte_otdoa_CpModeType' */
  uint8 e_CpMode;

  /* BW of serving cell in enum. Type 'loc_lte_otdoa_PrsBandwidthType' */
  uint8 e_ServingBw;

  /* Num of Tx antennas for serving cell. Type 'lloc_PrsAntPortConfigType' */
  uint8 e_NumTxAnt;

  /* Is MBSFN configuered for the carrier */
  boolean b_MbsfnInfoValid;
  
  /* Parameters for TDD carrier */
  boolean b_IsTddCarrier;
  /* Of type 'lte_l1_tdd_ul_dl_cfg_index_e' defined in 'modem_proc/lte/api/lte_l1_types.h' */
  uint8 u_TddConfig; 
  /* Of type 'lte_l1_tdd_special_subframe_pattern_e' defined in 'modem_proc/lte/api/lte_l1_types.h' */
  uint8 u_TddSpecialSfNum;
} lloc_LogScellInfoPerCellType;

typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* Enum to indicate when this info is passed to OTDOA SW by ML1.
   * Type 'loc_lte_otdoa_ServingCellContextType' */
  uint8 e_Context;

  /* Info for each carrier */
  lloc_LogScellInfoPerCellType z_Scell[LLOC_MAX_LTE_CARRIERS];
} lloc_EventsLogMl1UpdateScellType;

/* Log packet definition for ML1 PRS meas avail event */
typedef PACKED struct PACKED_POST
{
  /* Physical cell id */
  uint16 w_PhyCellId;

  /* number of hypothesis reqd. for this particular cell */
  uint8 u_NumHypPerCell;

  /* Hypothesis sequence number. Range - 1 to 'u_NumHypPerCell' */
  uint8 u_HypSeqNum;

  /* PRS BW used to measure this cell. Type 'loc_lte_otdoa_PrsBandwidthType' */
  uint8 e_PrsBw;

  /* Bitmask to specify different scheduling criteria used */
  uint8 u_SchedulingCriteriaBitMask;

  /* Carrier used for the measuremnt of this cell. It could be PCC, SCC1 or SCC2.
   * Range: 0 to 'LTE_LL1_CARRIER_COUNT-1'. As of now, 'LTE_LL1_CARRIER_COUNT' is defined as '3'
   * in 'modem_proc\fw_lte\api\intf_common.h' */
  uint8 u_Carrier; 

  /* PRS frequency meas. type. Based on enum 'loc_lte_otdoa_FreqMeasInfoType'.
   * Info about whether the current cell was an intra-frequency meas., or inter-frequency meas. */
  uint8 e_MeasType;

  /* LTE system bandwidth of the serving cell (it could be the sys. BW of the PCC or the SCC1/SCC2 cell
   * based on the carrier used for PRS measurement). Type: 'loc_lte_otdoa_PrsBandwidthType' */
  uint8 e_LteSysBw;
} lloc_LogMl1MeasAvailEventPerHypLogType;

typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* Sequence number of the data block */
  uint32 q_OccasionSeqNum;

  /* Total number of hypothesis searched in this PRS occasion */
  uint8 u_NumHyp;

  /* System Frame Number (SFN) of this PRS occasion */
  uint16 w_Sfn;

  /* Info of a strong offending cell, if it was muted in a PRS occasion */
  boolean b_OffendValid;
  uint16 w_OffendPci;
  uint32 w_OffendEarfcn;

  /* Hypothesis info */
  lloc_LogMl1MeasAvailEventPerHypLogType z_Hyp[LOC_LTE_OTDOA_MAX_HYPO_SRCH_PER_BATCH];
} lloc_EventsLogMl1PrsMeasAvailType;

/* Log packet definition for ML1 error event */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* enum of type 'loc_lte_otdoa_PrsErrorInfoType' */
  uint8 e_Error;
} lloc_EventsLogMl1ErrorType;

/* Log packet definition for OTDOA RSTD report timeout event */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  uint32 q_Reserved;
} lloc_EventsLogRstdRptTimeoutType;

/* structure for DIAG CMD event */
typedef PACKED struct PACKED_POST
{
  /* Command code */
  uint8 u_CmdCode;

  /* Subcommand code (overload of u_SubsysId) */
  uint8 u_GpsControl;

  /* Length of data payload (overload of w_SubsysCmdCode) */
  uint16 w_DataLength;

  /* CGPS Command Code (overload of u_CgpsCmdCode) */
  uint8 u_GpsSubcmd;

  /* OTDOA sub cmd data */
  uint8 u_CmdData[LLOC_DIAG_LOG_EVENT_DATA_LEN];
} lloc_DiagCmdForLoggingType;

/* Log packet definition for OTDOA DIAG cmd event */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  /* OTDOA sub cmd data */
  lloc_DiagCmdForLoggingType z_Cmd;
} lloc_EventsLogDiagCmdType;

/* Log packet definition for OTDOA stats event */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  uint32 q_Reserved;
} lloc_EventsLogOtdoaStatsType;

/* Log packet definition for SM OTDOA feature status */
typedef PACKED struct PACKED_POST
{
  /* Standard Header for OTDOA Events */
  lloc_EventsLogHdrType z_EventsHdr;

  uint8 b_Enable;
} lloc_EventsLogSmFeatureStatusType;

#ifdef T_WINNT
#error code not present
#endif /* T_WINNT */

/* Data strcuture used for logging PRS hypothesis data */
typedef struct
{
  const loc_lte_otdoa_measproc_MeasResultsAvailInfoType *p_MeasResults;
  boolean b_NcInterimLog;
  uint8 u_NcLevel;
  int32 q_LppSessionCtr;
  uint32 *p_CerVector;
} llomp_DataForHypLoggingType;

 /*--------------------------------------------------------------------------
 * Function Declarations
 *-----------------------------------------------------------------------*/
void lloc_LogSmStartOtdoaEvent( const loc_lte_otdoa_SmStartCmdInfoType * p_SmStartCmdInfo );
void lloc_LogSmStopOtdoaEvent( const loc_lte_otdoa_StopSessCmdType * p_SmStartCmdInfo );
void lloc_LogSmTimerUpdateEvent( const loc_lte_otdoa_UpdateTimoutInfoType * p_SmTimerUpdateInfo );
void lloc_LogSmReqRptEvent( const loc_lte_otdoa_ReqMeasRptCmdType * p_SmReqRptInfo );
void lloc_LogSmOtaResetEvent( const loc_lte_otdoa_OtaResetCmdType * p_SmOtaResetInfo );
void lloc_LogMl1ScellUpdateEvent( const loc_lte_otdoa_ServingCellInfoType *p_ScellInfo );
void lloc_LogMl1ErrorEvent( const loc_lte_otdoa_MeasErrorInfoType * p_Ml1Error );
void lloc_LogRstdRptTimeoutEvent( void );
void lloc_LogOtdoaDiagEvent( const lloc_DiagCmdForLoggingType * p_DiagCmd );
void lloc_LogOtdoaStatsEvent( void );
void lloc_LogSmFeatureStatus( const loc_lte_otdoa_FeatureStatusType *p_FeatureInfo );
void lloc_LogPrsToaMeas(const lloc_ToaDbPerCellType *p_ToaDbPerCell );
void lloc_LogRstdMeasRpt( const lloc_OtdoaMeasRptDbType *p_OtdoaMeasRptDb, const boolean b_IsEarlyRpt );
void lloc_LogAidingData( uint8 u_LogType, const lloc_PrsAidingDataDbType *p_PrsAidingDataDb );
void llomp_LogPrsDataForHypothesis
(
  const llomp_DataForHypLoggingType *p_HypData
);
#endif /* #ifndef LOC_LTE_OTDOA_LOG_H */
