#ifndef _GTS_LOG_H_
#define _GTS_LOG_H_

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     Global Time Services Log Module

GENERAL DESCRIPTION
This file is the main file of Global Time Service Log module

Copyright (c) 2014 - 2015 Qualcomm Atheros, Inc.
Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc. 
Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

$Header:
$DateTime:
$Author:

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*--------------------------------------------------------------------------
 * Include Files
 *-----------------------------------------------------------------------*/

#include "cgps_api.h"
#include "gnss_common.h"
#include "gts_api.h"
#include "gts_drsync.h"

/*--------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -----------------------------------------------------------------------*/

#define LOG_GNSS_GTS_INPUT_REPORT_VERSION   9
#define LOG_GNSS_GTS_TIME_UPDATE_VERSION    4
#define LOG_GNSS_GTS_DB_DUMP_VERSION        9
#define LOG_GNSS_GTS_EVENTS_VERSION         6
#define LOG_GNSS_GTS_QTIME_CALIB_VERSION    4
#define LOG_GNSS_GTS_DB_DUMP_SUBVERSION     1
#define LOG_GNSS_GTS_INPUT_REPORT_SUBVERSION 1
#define LOG_GNSS_GTS_TIME_UPDATE_SUBVERSION 1
#define LOG_GNSS_GTS_QTIME_CALIB_SUBVERSION 1

/* LOG_GNSS_GTS_EVENTS_SUB_ID */
#define LOG_GNSS_GTS_EVENTS_LATCH_DB_LOG    1
#define LOG_GNSS_GTS_EVENTS_CDMA_VSTMR_LOG  2
#define LOG_GNSS_GTS_EVENTS_GPS_FCOUNT_LOG  3
#define LOG_GNSS_GTS_EVENTS_QTIME_LOG       4
#define LOG_GNSS_GTS_EVENTS_LTE_VSTMR_LOG   5
#define LOG_GNSS_GTS_EVENTS_COMP_TIME_LOG   6 //unused
#define LOG_GNSS_GTS_EVENTS_TIME_TICK_LOG   7
#define LOG_GNSS_GTS_EVENTS_GET_TIME        9
#define LOG_GNSS_GTS_EVENTS_DRSYNC_LOG      10
#define LOG_GNSS_GTS_EVENTS_OFFSET_LOG      11
#define LOG_GNSS_GTS_EVENTS_TICK_PROC_LOG   12
#define LOG_GNSS_GTS_EVENTS_CONV_NS_LOG     13
#define LOG_GNSS_GTS_EVENTS_DEL_TIME_LOG    14
#define LOG_GNSS_GTS_EVENTS_SS_UPDATE_LOG   15

#define LOG_GNSS_GTS_EVENTS_CONV_NS_LOG_SUB_VERSION     4
#define LOG_GNSS_GTS_EVENTS_DRSYNC_LOG_LOG_SUB_VERSION     2
#define LOG_GNSS_GTS_EVENTS_GET_TIME_LOG_SUB_VERSION     2
#define LOG_GNSS_GTS_EVENTS_COMP_TIME_LOG_SUB_VERSION     2

/* Time Transfer DM Log Structure */
#define GTS_TIME_TRANSFER_INFO_LOG_VERSION  57

#define GTS_TTR_PARAMS_LOG_CDMA_RTC_LEGACY          (0U<<0)
#define GTS_TTR_PARAMS_LOG_CDMA_RTC_STMR_BASED      (1U<<0)
#define GTS_TTR_PARAMS_LOG_GNSS_FREQ_OFFSET_NORMAL  (0U<<1)

#define GTS_MAX_TIME_TRANSFER_SAMPLES 1

#define GTS_CM_MAX_NO_STACKS_LOG  2

/* To control event logging */
#define GTS_DSBL_LOG_LATCH_DB       (1 << LOG_GNSS_GTS_EVENTS_LATCH_DB_LOG) 
#define GTS_DSBL_LOG_CDMA_VSTMR     (1 << LOG_GNSS_GTS_EVENTS_CDMA_VSTMR_LOG)
#define GTS_DSBL_LOG_FC             (1 << LOG_GNSS_GTS_EVENTS_GPS_FCOUNT_LOG)
#define GTS_DSBL_LOG_QTIME          (1 << LOG_GNSS_GTS_EVENTS_QTIME_LOG)
#define GTS_DSBL_LOG_LTE_VSTMR      (1 << LOG_GNSS_GTS_EVENTS_LTE_VSTMR_LOG)
#define GTS_DSBL_LOG_COMP_TIME      (1 << LOG_GNSS_GTS_EVENTS_COMP_TIME_LOG) //unused
#define GTS_DSBL_LOG_TIME_TICK_REQ  (1 << LOG_GNSS_GTS_EVENTS_TIME_TICK_LOG)
#define GTS_DSBL_LOG_SCLK           (1 << LOG_GNSS_GTS_EVENTS_SCLK_FCOUNT_LOG)
#define GTS_DSBL_LOG_GET_GTS_TIME   (1 << LOG_GNSS_GTS_EVENTS_GET_TIME)
#define GTS_DSBL_LOG_PPS            (1 << LOG_GNSS_GTS_EVENTS_DRSYNC_LOG)
#define GTS_DSBL_LOG_OFFSET         (1 << LOG_GNSS_GTS_EVENTS_OFFSET_LOG)
#define GTS_DSBL_LOG_TICK_PROC      (1 << LOG_GNSS_GTS_EVENTS_TICK_PROC_LOG)
#define GTS_DSBL_LOG_CONV_NS        (1 << LOG_GNSS_GTS_EVENTS_CONV_NS_LOG)
#define GTS_DSBL_LOG_DEL_TIME       (1 << LOG_GNSS_GTS_EVENTS_DEL_TIME_LOG)
#define GTS_DSBL_LOG_SS_UPDATE      (1 << LOG_GNSS_GTS_EVENTS_SS_UPDATE_LOG)

/*--------------------------------------------------------------------------
 * Type Declarations
 *-----------------------------------------------------------------------*/

typedef enum
{
  GTS_TTR_SRC_CDMA,
  GTS_TTR_SRC_LTE_SIB,
  GTS_TTR_SRC_LTE_FTA
}
gts_TtrSource;

typedef enum
{
  GPS_RTC_CLK_RUNNING_BETWEEN_LOGS,
  GPS_RTC_CLK_STOPPED_BETWEEN_LOGS
}
gts_TtrClkStatusBtwLogsType;

typedef enum
{
  /* All time elements are logged as it is */
  GTS_DBDUMP_RAW_TIME,
  /* All time elements are propagated to PE GPS Fix time */
  GTS_DBDUMP_PROP_FIX_TIME,
  /* All time elements are propagated to current Qtime */
  GTS_DBDUMP_CURRENT_TIME
} gts_DbDumpType;

/* enum structure for different clock rates */
typedef enum
{
  GTS_GCx20,
  GTS_GCx32,
  GTS_GCx40,
  GTS_GCx80,
  GTS_INVALID_RATE
} 
gts_BpClkRateType;

typedef enum
{
  GTS_GET_TIME,
  GTS_GET_TIME_USTMR,
  GTS_GET_TIME_FCOUNT, //Not used
  GTS_GET_TIME_OFFSET,
  GTS_GET_TIME_EXT, //Not used
  GTS_GET_TIME_FOR_SRC
} gts_GetTimeType;

typedef enum
{
  GTS_ERR_NONE,
  GTS_ERR_NULL_PTR,
  GTS_ERR_NO_FCOUNT,
  GTS_ERR_NO_LATCH,
  GTS_ERR_NO_TIME_AVL,
  GTS_ERR_MAX_SRC,
  GTS_ERR_FCQT_FAIL,
  GTS_ERR_NO_QTIME,
  GTS_ERR_BLACKLISTED
} gts_ErrorType;

typedef enum
{
  GTS_QTIME_CALIB_LOG_SRC_SV_FIX,
  GTS_QTIME_CALIB_LOG_SRC_MC_REQ
} gts_QtimeCalibLogSrcType;

/* Log Packet Definition */
#ifdef T_WINNT
#error code not present
#endif

/* Time Transfer sets of CDMA+GNSS RTCs latched at the same time structure */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* CDMA RTC count value (in CDMA Chipx8) */
  uint32  q_CdmaRtc;

  /* CDMA RTC phase value (in CDMA Chipx2^35) */
  uint32  q_CdmaRtcPhase;

  /* GNSS RTC value (in GPS Chipx80) */
  uint32  q_GnssRtc;

  /* GNSS Phase value (in sub-GPS Chipx80) */
  uint32  q_GnssPhase;

  /* GNSS Sample Count status */
  uint32  q_SampleCount;    

  /* USTMR Latch (in 19.2Mhz units ) */
  uint32  q_Ustmr;
}
gts_CdmaTimeLatchElemType;

typedef struct
{
  uint8 u_NumElems;
  uint64 t_Qtime;
  DBL d_LatchUnc;
  gts_CdmaTimeLatchElemType z_LatchedData[GTS_MAX_TIME_TRANSFER_SAMPLES];
}
gts_CdmaTimeLatchType;

/* LTE Time Transfer sets of O_STMR+GNSS RTCs latched at the same time structure */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* O_STMR count value (in CDMA Chipx8) */
  uint32  q_Ostmr;

  /* O_STMR phase value (in CDMA Chipx2^35) */
  uint32  q_OstmrPhase;

  /* GNSS RTC value (in GPS Chipx80) */
  uint32  q_GnssRtc;

  /* GNSS Phase value (in sub-GPS Chipx80) */
  uint32  q_GnssPhase;

  /* GNSS Sample Count status */
  uint32  q_SampleCount;  

  /* USTMR Latch (in 19.2Mhz units ) */
  uint32  q_Ustmr;  
}
gts_LteTimeLatchElemType;

typedef struct
{
  uint8 u_NumElems;
  uint64 t_Qtime;  
  DBL d_LatchUnc;
  gts_LteTimeLatchElemType z_LatchedData[GTS_MAX_TIME_TRANSFER_SAMPLES];
}
gts_LteTimeLatchType;

/* LTE timing advance structure */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Validity of timing advance data */
  boolean b_TaValid;

  /* Timing advance (Rx-Tx diff) in Ts units */
  S16 x_TaData;

  /* Serving cell physical ID. Range ~ 0 to 503 */
  U16 w_ServPhyCellId;
}
gts_TtrLogTaDataType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* LTE Time Transfer Report content */
  /* Request ID of the LTE Time Transfer request that triggered this report */
  U8   u_ReqID;

  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  U8 u_SubInfo;

  /* Report status. Of Type 'gts_LteTimeStatusType' */
  U8 e_RptStatus;

  /* Physical cell ID of the LTE serving cell [Range ~ 0 to 503] */
  U16  w_CellId;

  /* This field specifies the E-UTRA carrier frequency of the LTE serving cell 
   * based on lte_earfcn_t */
  U32  w_Earfcn;

  /* SIB8 "cdma-EUTRA-Synchronisation" info:
     TRUE if LTE NW is synchronized. Otherwise FALSE. */
  boolean b_Sib8Valid;

  /* The CDMA2000 system time corresponding to the SFN
     boundary at or after the ending boundary of the
     SI-Window in which this Time Transfer was taken. The size
     is 52 bits and the unit is [CDMA2000 chips]
  */
  U64  t_Sib8CdmaSystemTime;

  /* SIB16 System Time info*/
  boolean b_Sib16Valid;

  /* The Coordinated universal system time corresponding to the SFN
                              boundary at or after the ending boundary of the
     SI-Window in which this Time Transfer was taken. The size
     is 34 bits and the unit is [1 ms]
  */
  U64  t_Sib16GPSTime;

  /* Number of leap seconds between UTC time and GPS time */
  U16  w_LeapSeconds;

  /* LTE SFN system time of the subframe in which TTr was acquired
     in the unit of subframe */
  U16  w_Sfn;

  /* O-STMR value at the boundary of the subframe in which TTr was acquired */
  U32  q_OstmrSfBoundary;

  /* Best estimate of the current LTE system time uncertainty */
  U32  q_SysTimeUncEst;

  /* Current value of the slow clock counter */
  U64  t_Qtime;

  /* LPM RTC value latched at the same time as the first latched O_STMR/GNSS RTC
     set (in 19.2MHz untis) */
  U32  q_LpmRtcLatched;

  /* Universal STMR value latched at the same time as the first latched
     O_STMR/GNSS RTC set (in 19.2MHz untis) */
  U32  q_UnivStmrLatched;

  /* O_STMR control register read at the same time as the first latched
     O_STMR/GNSS RTC set (in Ts/2^30 units) */
  U32  q_OstmrFcw;

  /* Mark if the next two fields are valid or not */
  boolean b_OstmrVsrcValid;

  /* OSTMR From FW */
  U32 q_OstmrFromFW[2];

  /* VSRC From FW */
  U32 q_VsrcFromFW[2];

  /* Difference between O_STMR and VSRC STMR (in TBD units) */
  U32  q_OstmrVsrcStmrDiff;

  /* Number of sets of latched RTCs/STMRs per LTE Time Transfer Data report.
     u_NumTTSamples should be <= GTS_MAX_TIME_TRANSFER_SAMPLES
     u_NumTTSamples == 0 presents error condition
   */
  U8   u_NumTTSamples;

  /* Timing advance data to predict TTr uncertainity */
  gts_TtrLogTaDataType z_TaData;

  /* Sets of O_STMR + GNSS RTCs latched at the same time */
  gts_LteTimeLatchElemType z_LatchedData[ GTS_MAX_TIME_TRANSFER_SAMPLES];
} gts_LteTtrDmLog;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* LTE Time Transfer Report content */
  /* Request ID of the LTE Time Transfer request that triggered this report */
  U8   u_ReqID;
  U64  t_Qtime;

  boolean b_RefTimeUncPresent;
  U8   u_RefTimeUnc; /* reference time unc */

  boolean u_GpsWeekIsValid;    /* TRUE if the week number specified is valid */
  U16  w_GpsWeek;           /* Unambiguous GPS week */
  U32  q_GpsMsec;           /* GPS milliseconds of the week */
  FLT  f_GpsTimeUnc;        /* Time uncertainty (msec) */

  U16  w_SecsFromFrameStructStart;
  U32  q_FracSecsFromFrameStructStart;
  boolean b_FrameDriftPresent;
  U8   u_FrameDrift;
  FLT     f_ClkTimeBiasMs;
} gts_TtrLteLppFtaDmLog;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* 1x system time in qword format */
  U64  t_SystemTime;
  /* Current value of the slow clock counter */
  U64  t_Qtime;
  /* CDMA RTC value latched at the last Sync80 tick (in CDMA Chipx8) */
  U32  q_CdmaRtcSync80;
  /* CDMA RTC phase value latched at the last Sync80 tick (in CDMA Chipx8) */
  U32  q_CdmaRtcSync80Phase;
  /* Current 1x Ref PN offset */
  U16  w_RefPN;
  /* Current 1x serving frequency band associated to Ref PN */
  U8   u_Band;
  /* Current 1x serving frequency channel associated to Ref PN */
  U16  w_Channel;
  /* Current master finger position, where master fingers is
   * assigned to Ref PN Offset */
  S32   l_MasterFingerPosition;
  /* Maximum Antenna Range */
  U32  q_Mar;
  /* Best estimate of the current 1x system time uncertainty */
  U32  q_SysTimeUncEst;
  /* LPM RTC value latched at the same time as CDMA RTC */
  U32  q_LpmRtcLatched;
  /* BP1 Clock Rate - in packed structure this is U8 - gts_TtrBpRateType*/
  U8 e_Bp1_Rate;
  /* Variable to know if RTC clk had been stopped in-between logs - in packed structure this is U8 */
  U8 e_RtcClkStatusBtwLogs;
  /* Serving Band Class TCAL value */
  S16   w_TimeCalValue;
  /* Universal STMR value latched at the same time as the first latched
     CDMA/GNSS RTC set (in 19.2MHz untis) */
  U32  q_UnivStmrLatched;
  /* CDMA RTC STMR control register read at the same time as the first latched
     CDMA/GNSS RTC set (in CDMA Chipx2^35 units) */
  U32  q_CdmaRtcFcw;
  /* Number of sets of latched RTCs per 1x Time Transfer Data report */
  U8   u_NumTTSamples;
  /* Sets of CDMA+GNSS RTCs latched at the same time */
  gts_CdmaTimeLatchElemType z_LatchedData[ GTS_MAX_TIME_TRANSFER_SAMPLES ];
  /* Calculated and reported GNSS RTC value at Sync80 instance (in GPS Chipx1280) */
  U32  q_GnssRtcReported;
  /* Various Time Transfer parameters */
  U32 q_TimeTransferParams;
  /* Variable portion of CDMA RF Front End propagation delay (in CDMA Chipx8192
     units). This delay is measured as the difference between CDMA RTC and
     VSRC STMR. */
  U32  q_CdmaRfFeVariableDelayCCx8192;

} gts_CdmaTtrDmLog;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /*lint -e754 : local structure member not referenced */
  log_hdr_type z_Hdr; /* Standard DM header */
  U8  u_Version;   /* Version number for the log */

  /* Type of Time Transfer information in this log */
  U8 e_TTSource;  /* gts_TtrSource */

  union
  {
    gts_CdmaTtrDmLog z_Cdma;
    gts_LteTtrDmLog z_Lte;
    gts_TtrLteLppFtaDmLog z_LteLppFta;
  }z_Ttr;

} gts_TtrDmLog;

/* Common Time Transfer Data Structure */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Indicate if the latch was successful */
  U8 b_Latch;
  /* GPS RTC value latched at the same time as CDMA RTC (in GPS Chipx32) */
  U32  q_GnssRtc;
  /* GNSS RTC sample and chip counts @ Time transfer latch */
  U32  q_SampleCount;
  /* GNSS RTC Phase count @ Time transfer latch */
  U32  q_GnssPhase;
  /* Universal STMR value latched at the same time as the first latched
     CDMA/GNSS RTC set (in 19.2MHz untis) */
  U32  q_Ustmr;
} gts_LatchLogType;

/* Reference Time structure for Timetags */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Ref Time */
  U8  b_QtimeValid;
  /* Ref Time Validity */
  U64 t_Qtime;
  U32 f_QtimeBias;
} gts_QtimeLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  U8 b_Valid;
  U8 b_Used;
  gts_LatchLogType z_Latch;
  gts_QtimeLogType z_Qtime;
  U32 q_FCount;
} gts_FCLatchLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  U8 v_FixValid;     /* TRUE if fix is valid, FALSE if invalid */
  U16 w_GpsWeek;       /* GPS week number */
  U32 q_GpsTimeMs;     /* GPS time in week (milliseconds) */
  U32 q_RefFcount;     /* Receiver's ms counter value. */
  U64 t_TimeTickMsec;  /* Receiver's time tick ms counter value */
  U64 d_PosLla[3];        /* LLA position (rad, m) */
  U32 f_Punc3dMeters;     /* Uncertainty of position (m) */
  U64 f_PuncLatLonMeters[2];  /** Latitude, longitude uncertianty (m), 
                                  68% conf. within +/- this 1-sigma value */
  U32 f_PuncVertMeters;         /* Vertical uncertainty (m) */
  U32 f_ErrorEllipse[3];  /** Unc angle (deg), unc a (m), unc b (m)
                              39% conf. within this one-sigma ellipse */
  U8 e_HoriRelIndicator; /*horizontal reliability*/
  U8 e_VertRelIndicator; /*vertical reliability*/
  U64 d_VelEcef[3];       /* Velocity in ECEF (m/s) */
  U64 d_VelEnu[3];        /* Velocity in ENU (m/s) */
  U64 f_HeadingRad;       /* Heading angle, [0,2*pi) (rad). Referenced to North
                            (0 rad) and positive clock-wise. This is the "best" estimate
                            of heading and uses sensor data (if available and enabled).
                            See also: f_GnssHeadingRad below */
  U64 f_HeadingUncRad;    /* Heading uncertainty in Radians */
  U64 f_Vunc3dMps;              /* Velocity uncertainty (m/s) */
  U64 f_VuncEastNorthMps[2];    /* East, North uncertianty (m/s) */
  U64 f_VuncVertMps;      /* Vertical uncertainty (m/s) */
} gts_PosVelLogType; /* Pos Vel data to GTS */


#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  gts_PosVelLogType z_PosVelLog;

  /* Latched Information */
  gts_LatchLogType z_LatchInfoLog;
  gts_QtimeLogType z_QtimeInfoLog;
} gts_PosEstMsgLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  U32  q_VsrcFromFW[2];
  U32  q_VsrcPhFromFW[2];
  U32  q_OstmrFromFW[2];
  U32  q_OstmrPhFromFW[2];
  U64  t_RefDlTime;
} gts_VsrcInfoLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  U32  q_Freq;    /* DL frequency - Same as lte_earfcn_t*/
  U16  w_CellId;  /* Physical Cell ID */

  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  U8 u_SubInfo;

  /* SIB8 TRUE: LTE NW is synchronized and supports 1x. FALSE: LTE NW is NOT synchronized */
  U8 b_Sib8Valid;
  /* The CDMA2000 system time corresponding to the SFN boundary at or after the ending boundary of the SI-Window in which this Time Transfer was taken. The size is 52 bits and the unit is [CDMA2000 chips] */
  U64  t_Sib8CdmaSystemTime;

  U8 b_Sib16Valid; /* SIB16 System Time info*/
  U64  t_Sib16GpsMsecs; /* The Abs GPS time at the SFN boundary in of 10 msecs */
  U16  w_LeapSeconds; /* Number of leap seconds between UTC time and GPS time */

  U16  w_Sfn;             /* System Frame Number at which data was collected */
  U32  q_OStmrSfBoundary;  /* OSTMR at the boundary of the subframe */

  /* Latch Information */
  U32  q_UstmrLatched;   /* USTMR Latched in 19.2 Mhz Units */
  U32  q_OstmrLatched;   /* OSTMR Latched in 30.2 Mhz Units (Ts) */
  U32  q_OstmrPhLatched; /* OSTMR Latched in 30.2 Mhz Units (sub Ts)*/

  /* VSRC Information for PCC */
  /* Mark if the next field is valid or not */
  U8 b_OstmrVsrcValid;
  gts_VsrcInfoLogType z_RefVsrcInfo;
  gts_VsrcInfoLogType z_CurrVsrcInfo;

  /* Reference Time (QTIME) */
  gts_QtimeLogType  z_QtimeInfo;
} gts_LteTimeInfoLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  uint16 w_Sfn; /* Closest NR5G sub-frame number corresponding to below USTMR/NR5G STMR pair */
  uint32 q_Ustmr; /* USTMR in 19.2 MHz Units */
  uint32 q_Nr5gStmr; /* NR5G STMR in 30.2 MHz Units (Ts) */
  uint32 q_Nr5gStmrPh; /* NR5G STMR phase in 30.2 MHz Units (sub-Ts) scaled-up by 2^32 */
} gts_Nr5gDlInfoLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  uint8 u_SubInfo;

  uint32 q_Earfcn; /* DL frequency of primary serving cell */ 
  uint16 w_CellId; /* Physical Cell ID of primary serving cell */

  /* NR5G timing/latch info captured at current instance (current paging cycle) */
  gts_Nr5gDlInfoLogType z_CurrLatch;

  /* Reference Time (QTIME). This should be either same as current latched USTMR value (lower 24bits of Qtimer
   * will be the USTMR); or it should be captured as close to the current latched USTMR value as possible */
  gts_QtimeLogType z_QtimeInfo;

  /* Optional fields for SIB9 */
  boolean b_Sib9Valid; /* SIB9 System Time validity */
  uint64 t_Sib9GpsMsecs; /* Absolute GPS time at previous System frame boundary */
  uint16 w_LeapSeconds; /* Number of leap seconds between UTC time and GPS time */
  gts_Nr5gDlInfoLogType z_SysFrBdry; /* Nr5G STMR, SFN and USTMR corresponding to system frame bdry of SIB9 system time */
  boolean b_TaValid; /* Timing advance validity */
  int32 l_TimingAdvance; /* Timing advance value */

  /* Optional field for base station type */
  uint8 e_BsType; /* Same as enum 'gts_Nr5GBaseStationType' */

  /* Reserved fields */
  uint32 q_Reserved[GTS_NR5G_RESERVED_LEN];
} gts_Nr5gTimeInfoLogType;

/* ARFCN type, includes band if quadband is supported */
typedef enum
{
  GTS_SYS_BAND_EGSM_900,
  GTS_SYS_BAND_PGSM_900,
  GTS_SYS_BAND_PCS_1900,
  GTS_SYS_BAND_DCS_1800,
  GTS_SYS_BAND_CELL_850,
  GTS_INVALID_BAND,
  GTS_SYS_BAND_NONE = 0xFF  
} gts_GsmBandLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  U16 w_BandNum;
  U8 e_BandType; // type 'gts_GsmBandLogType'
} gts_GsmArfcnLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  U8 u_SubInfo;

  /* Cell Information. Base Station Identification code and freq */
  U8 u_Bsic;
  gts_GsmArfcnLogType z_Arfcn;

  /* Time Information */
  /* Frame Number 0..(26L*51L*2048L-1)  ~4.615ms frames */
  U32 q_FrameNum;
  /* Time Slot Number 0..7 */
  U8 u_TimeSlot;
  /* QSymbol Count (sub fn) */
  U32 q_QSymCnt;

  /* GSM State */
  U8 u_GL1State;

  /* Additional Drift in the time in QSym Units (if any) */
  S8 b_DriftQSym;

  /* Reference Time (QTIME) */
  gts_QtimeLogType  z_QtimeInfo;
} gts_GsmTimeInfoLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  U8 u_SubInfo;

  /* Cell Information */
  /* DL frequency and Primary scrambling code */
  U16  w_Freq;
  U16  w_PSC;

  /* Time Information */
  /* 10.3.6.75  System Frame Number  0..4095 10ms frames */
  U16  w_SysFn;

  /* 0..307200 Chipx8 per frame */
  U32  q_CX8Num;

  /* 24-bit XO value (USTMR) at SFN/Cx8 boundary */
  U64 t_UstmrAtCx8;

  /* GSM State */
  U8 u_WL1State;

  /* Reference Time (QTIME) */
  gts_QtimeLogType  z_QtimeInfo;
} gts_WcdmaTimeInfoLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Subscription Information – For MultiSim Support */
  U8 u_SubInfo;
  /* Cell Information */
  /* DL frequency. */
  U16  w_Freq;

  /* Cell Param ID */
  U16  w_CellId;

  /* Time Information */
  /* 64-bits value in TDS chipx8 units at status dump, clocked @ 10.24 MHz */
  U64  q_TdsTime;

  /* 30-bits value in TDS sub chipx8 units at status dump */
  U32  q_TdsPhase;

  /* USTMR at the Status Dump, Clocked at 19.2 Mhz */
  U32 q_Ustmr;

  /* TDS L1 State 0 – Paging, 1- Traffic*/
  U8 u_TDSL1State;

  /* Reference Time (QTIME) */
  gts_QtimeLogType  z_QtimeInfo;
} gts_TdsTimeInfoLogType;

/* GTS Freq Est Structure */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  U64 t_PrevQtime;  /* Time at the last temp Update */
  U64 t_CurrQtime; /* Time at the current temp Update */
  S32  l_FreqEst;   /* Freq Est based on Temperature at Prev and Curr Time */
  U32  f_FreqEstUncMs; /* Time Uncertainity estimate based on temp table */
  U8  u_FtCalState;  /* FT Calibration state */
  U8  e_FestSrc; /* enum of type 'gts_FestSrcType' */
} gts_FreqEstInfoLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* 1x system time in qword format */
  qword   t_SystemTime;

  /* CDMA RTC value latched at the last Sync80 tick (in CDMA Chipx8) */
  U32  q_CdmaRtcSync80;

  /* Current 1x Ref PN offset */
  U16  w_RefPN;

  /* Current 1x serving frequency band associated to Ref PN */
  U8   u_Band;

  /* Current 1x serving frequency channel associated to Ref PN */
  U16  w_Channel;

  /* Current master finger position, where master fingers is
   * assigned to Ref PN Offset */
  S32   l_MasterFingerPosition;

  /* Maximum Antenna Range */
  U32  q_Mar;

  /* Best estimate of the current 1x system time uncertainty */
  U32  q_SystemTimeUncertaintyEstimate;

  /* CDMA RTC STMR control register read at the same time as the first latched
     CDMA/GNSS RTC set (in CDMA Chipx2^35 units) */
  U32  q_CdmaRtcFcw;

  /* Difference between CDMA RTC and VSRC STMR (in CDMA Chipx8192 units) */
  U32  q_CdmaRtcVsrcStmrDiff;
}gts_CdmaInfoLogType;

/* GTS Time Input Log Type */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef union PACKED_POST
#else
typedef PACKED union PACKED_POST
#endif
{
  gts_PosEstMsgLogType z_PosEstMsgLog;
  gts_LteTimeInfoLogType z_LteTimeInfoLog;
  gts_GsmTimeInfoLogType z_GsmTimeInfoLog;
  gts_WcdmaTimeInfoLogType z_WcdmaTimeInfoLog;
  gts_TdsTimeInfoLogType z_TdsTimeInfoLog;
  gts_FreqEstInfoLogType z_FreqEstInfoLog;
  gts_CdmaInfoLogType    z_CdmaInfoLog;
  gts_Nr5gTimeInfoLogType z_Nr5gTimeInfoLog;
} gts_ModuleInfoLogType;

/* GTS Input Information Log Packet */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  U8 u_Version;

  /* Sub-Version number for the log */
  U8 u_SubVersion;

  U8 e_TimeModuleSource; // of enum type 'gts_TimeSrcType'
  gts_ModuleInfoLogType z_TimeInputLog;
} gts_TimeInputLogType;

/* GTS Time Update Log Packet */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* If the time below is valid or not */
  U8 b_Valid;

  /* Time Source */
  U8 e_TimeSrc;

  /* Absolute GPS Time related information */
  U16 w_GpsWeek;     /* GPS week number at reference tick [weeks] */
  U32 q_GpsMsec;     /* GPS msec at reference tick [msecs] */
  U32 f_ClkTimeBias; /* Clock bias [msecs]. Constraint to >-0.5 and <0.5 */
  U64 d_ClkTimeUncMs; /* 1 sided max time bias uncertainty [msecs] */

  /* Reference Time (QTIME) */
  gts_QtimeLogType  z_QtimeLogInfo;

  /* GPS Clock Specific Fields */
  U8  b_FCountValid; /* Mark if FCount below is valid */
  U32 q_GpsFCount;    /* GPS FCount at the Time Estimate */

  /* Freq Error Information */
  uint8 b_FreqEstValid;  /* Frequency Error Estimate is valid */
  int32 x_FreqEstErrPpb;   /* Freq Error Estimate in PPB (2^-10 PPM) */
  int32 x_FreqEstErrUncPpb;   /* Freq Error Estimate Unc in PPB (2^-10 PPM) */

  /* Velocity Information */
  uint8 b_VelValid;
  U32 f_VelocityMs;      /* Velocity of the device in meter/sec */

  /* Leap Second Info */
  uint8 b_LeapSecValid;
  uint8 u_LeapSecond;      /* Leap Second value in Seconds */
  
  /* Boolean field to indicate if the abs time was propagated (at the source).
   * Currently used only by PACE source to indicate if PACE time was propagated, or
   * computed using new QDR and <= 2 SV input */
  uint8 b_IsPropagated;

  /* XO frequency estimation source of enum type 'gts_FestSrcType' */
  uint8 e_FestSrc;

  /* Flag to indicate to GPS MC to force-use the time */
  uint8 b_ForceFlag;

  /* Enum to indicate constellation/format type. Same as enum 'gts_TimeFormatType' */
  uint8 e_TimeFormat;
}gts_TimeLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  U8 u_Version;
  
  U8 u_SubVersion;
  
  U8 e_ModSrc;
  gts_TimeLogType z_PrevTime;
  gts_TimeLogType z_CurrTime;
}gts_TimeUpdateLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Source Module */
  U8  e_SrcMod;
  /* Source Module Time */
  gts_TimeLogType z_GpsTime;

  /* Time Difference with GPS Ref Time */
  U8  b_TimeDiffValid;
  U64 d_TimeDiffMs;
}gts_DbTimeLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  U8 u_Version;

  /* Sub-Version number for the log */
  U8 u_SubVersion;

  /* 0x0 - Raw Time Dump
     0x1 - Time Dump propagated to recent GPS Fix */
  U8 u_Flags;
  /* Reference Time from GPS Fix */
  gts_TimeLogType z_GpsRefTime;
  gts_DbTimeLogType z_ModSrcTime[GTS_TIME_SRC_MAX_SRC];
}gts_DbLogType;

/* Time transfer register latches sent by CC/NavRx to GTS */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  U32 q_GnssRtc;
  U32 q_GnssRtcPhase;
  U32 q_Ustmr;
  U32 q_RawUstmr;
  U32 q_SampleSkip;
}
gts_TtrLatchRegSetLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard DM header */
  log_hdr_type z_Hdr;

  /* Version number for the log */
  U8 u_Version;
 
  /* SubLog Id Type */
  U8 u_SubLogIdType;    

  /* SubLog Id version */
  U8 u_SubLogIdVersion;
} gts_EventsLogHdrType;

/* GTS Events Register DB Log Packet */
#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;

  /* Log GTS Qtime */
  gts_QtimeLogType z_Qtime;

  /* Other Log Packet Fields for Reg Db */
  U8 u_LatchType;
  U16 w_Cnt;
  gts_TtrLatchRegSetLogType z_Elem[GTS_TTR_LATCH_REG_API_MAX_SIZE];

  /* Information from the stored DB */
  U16 w_ValidCntDb;

  /* Latch Stats */
  U32 q_NumTtrLatchSets;
  U32 q_NumTtrLatchErrorsPerSet;
  U32 q_NumTtrLatchErrorsTotal;  
} gts_TtrRegDbLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;
  
  /* USTMR  */
  U32 q_Ustmr;  

  /* CDMA RTC */
  U32 q_CdmaRtc;

  /* CDMA Phase */
  U32 q_CdmaPhase;
} gts_CdmaVstmrLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;
  
  /* USTMR */
  U32 q_Ustmr;  

  /* OSTMR */
  U32 q_Ostmr;

  /* OSTMR Phase */
  U32 q_OstmrPhase;
} gts_LteVstmrLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;
  
  /* GPS FCount is valid ? */
  U8 b_Valid;  

  /* GPS FCount */
  U32 q_GpsFCount;
} gts_GpsFCountLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;
  
  /* Qtime valid */
  U8 b_Valid;  

  /* Qtime */
  U64 t_Qtime;

  /* Qtime Bias*/
  FLT f_QtimeBias;
} gts_QtimeEventLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;
  
  /* Information is valid or not */
  U8 b_Valid;

  /* GPS FCount */
  U32 q_GpsFCount;

  /* Time Tick Sub Ms (in Ms) */
  U32 f_TimeTickSubMs;

  /* Unc of Sub Ms */
  U32 f_TimeTickUncMs;

  /* Time Tick (in Ms) */
  U64 t_TimeTickMs; 

  /* QTime Information */
  gts_QtimeLogType z_Qtime;
}gts_TimeTickInfoLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;

  /* Type of Get Time */
  U8 u_GetTimeType;

  /* Error Code */
  U8 u_ErrorCode;

  /* Information is valid or not */
  gts_TimeLogType z_GtsTime;

  /* USTMR information */
  U32 q_Ustmr;

  /* Offset Information */
  U64 d_Offset;
}gts_GetGtsTimeLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;

  /* GTS Time Used */
  gts_TimeLogType z_GtsTime;

  /* USTMR used for calculating Assert */
  U32 q_Ustmr;

  /* USTMR at which the DRSync pulse was asserted */
  U32 q_ATUstmr;

  /* USTMR at which the DRSync pulse was deasserted */
  U32 q_DATUstmr;

  /* Time diff for the next GPS sec boundary. DBL value stored in uint64 */
  U64 d_AssertTimeMsec;

  /* PPS and ISR Counters */
  U32 q_PpsCount; 
  U32 q_ISRCount;

  /* Qtime at which ISR is triggered */
  U64 t_IsrQtime;
}gts_DRSyncLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;

}gts_TickProcLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;

  /*Error recovery flag to blacklist the source */
  boolean b_ErrorRecovery;

}gts_DelTimeLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
	boolean b_ServCellChanged;

	int8 b_SysMode;  /* Same as ENUM sys_sys_mode_e_type */

	int8 b_SrvStatus;/* same as ENUM sys_srv_status_e_type */
	
}gts_SysStatusLogType;


#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;

  int8  b_SubId;
  
  U8 u_NumStacks;

  gts_SysStatusLogType z_SysStatus[GTS_CM_MAX_NO_STACKS_LOG];

}gts_SSUpdateInfoLogType;


#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /*lint -e754 : local structure member not referenced */

  log_hdr_type z_hdr;             /* Standard DM header */

  /* Version of log packet.Set to 1 for GTS QTime Calib */
  U8 u_Version;

  U8 u_SubVersion;
  
  /* This flag indicates whether it is calibrated or uncalibrated */
  U8 e_CalibState;

  /* Event which set or invalidate calibration */
  U8 e_CalibStateReason;

  /* This flag indivates whether calibration is based on MC request or Position fix */
  U8 e_QtimeCalibLogSrcType;
  
  /* To store current offset which can be used later */
  U64 d_PrevFCOffset;
  
  /* Store the Frequency biases for next update */
  U64 d_QtimeFreqBiasM1;
  U64 d_QtimeFreqBiasM2;
  
  /* Store previous freq bias to compute Fcount offset when
     receiver wake up from DPO sleep */
  U64 d_ApproxClkFreqBias;
  
  /* Store latest clock frequency bias estimate */
  U64 d_ClkFreqBias;
  
  /* Store latest clock frequency bias Unc */
  U64 d_ClkFreqBiasUnc;
  
  /* GTS time associated with QTime latch after position fix */
  gts_TimeLogType z_GtsTime1;
  gts_TimeLogType z_GtsTime2;
  
  /* Store latched QTime and Fcount at perticular Tick and wake-up instant */
  gts_FCLatchLogType z_TicLatch;
  gts_FCLatchLogType z_WakLatch;
} gts_QtimeCalibLogType;

#if defined (T_APQ8064) || defined (FEATURE_GNSS_SA)
typedef struct PACKED_POST
#else
typedef PACKED struct PACKED_POST
#endif
{
  /* Standard Header for GTS Events */
  gts_EventsLogHdrType z_Hdr;

  /* GTS Time Used */
  gts_TimeLogType z_GtsTime;

  /* Status */
  U8 u_Status;
  
  /* Computed Ns */
  U64 t_GpsTimeNs;
  U32 q_UncNs;

  int32 x_FreqEstErrPpb;   /* Freq Error Estimate in PPB (2^-10 PPM) */
  int32 x_FreqEstErrUncPpb;   /* Freq Error Estimate Unc in PPB (2^-10 PPM) */
  boolean b_FreqEstValid;  /* Frequency Error Estimate is valid */
  /* FEST source of enum type 'gts_FestSrcType' - defined in 'gts_api.h' */
  U8 e_FestSrc;

  /* Velocity Information */
  boolean b_VelValid;
  U32 f_VelocityMs; /* Velocity of the device in meter/sec in floating point */
} gts_ConvNsApiLogType;

#ifdef T_WINNT
#error code not present
#endif

/*--------------------------------------------------------------------------
 * Function Prototype Declarations
 *-----------------------------------------------------------------------*/

/*
 ******************************************************************************
 * Function gts_LogCdmaVstmrValues
 *
 * Description:
 *
 *  Generate the Time update log packet which provides the new time and the old
 *  time for the particular Time Src
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogCdmaVstmrValues( U32 q_CdmaRtc,U32 q_CdmaPhase,U32 q_Ustmr );

/*
 ******************************************************************************
 * Function gts_LogVstmrValues
 *
 * Description:
 *
 *  Generate the Time update log packet which provides the new time and the old
 *  time for the particular Time Src
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogLteVstmrValues( U32 q_Ostmr,U32 q_OstmrPhase,U32 q_Ustmr );

/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_CdmaTtrLog
 *
 * Description:
 *
 *  This function logs the CDMA Time transfer data
 *
 * Parameters:
 *
 *  pz_CdmaTimeInfo - CDMA Time Info Structure
 *  pz_CdmaLatchDb - CDMA Latch Database
 *  f_GpsRtcGCx80 - Reported GPS RTC
 *  w_TimeCalValue - TCAL value
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_CdmaTtrLog( const gts_CdmaTimeInfoType *pz_CdmaTimeInfo,
                     const gts_CdmaTimeLatchType *pz_CdmaLatchDb,
                     FLT f_GpsRtcGCx80, uint16 w_TimeCalValue );

/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_LteTtrLog
 *
 * Description:
 *
 *  This function logs the LTE Time transfer data
 *
 * Parameters:
 *
 *  pz_CdmaTimeInfo - CDMA Time Info Structure
 *  pz_CdmaLatchDb - CDMA Latch Database
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LteTtrLog( const gts_LteTimeInfoType *pz_LteTimeInfo,
                    const gts_LteTimeLatchType *pz_LteLatchDb );


/*
 ******************************************************************************
 * Function gts_LogQtimeCalib
 *
 * Description:
 *
 *  This function logs the USTMR calibration parameters when SV/PE makes
 *  GPS fix or when MC request for USTMR calibration source
 *
 * Parameters:
 *
 *  e_CalibSrcLog - Enum for calibration Source (MC request or Position fix)
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogQtimeCalib( const gts_QtimeCalibLogSrcType e_CalibSrcLog );


/*
 ******************************************************************************
 *
 * Function:
 *
 *   gts_LteFtaLog
 *
 * Description:
 *
 *  This function logs the LTE FTA (fine time assistance) data
 *
 * Parameters:
 *
 *  pz_CdmaTimeInfo - CDMA Time Info Structure
 *  pz_CdmaLatchDb - CDMA Latch Database
 *
 * Return value:
 *
 *  None
 *
  ******************************************************************************
*/
void gts_LteFtaLog( const gts_LteTimeInfoType *pz_Rpt,
                    const gnss_FTA_Information *pz_Fta,
                    const gnss_FTA_ReferenceTimeForOneCell *p_CellFta,
                    const gts_TimeType *pz_TimeEst,
                    const gts_QtimeType *pz_FtaQtime );

/*
 ******************************************************************************
 * Function gts_LogGpsFCount
 *
 * Description:
 *
 *  Generate the GPS FCount Event Log packet. This will help in Playback 
 *  processing of logs
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogGpsFCount( boolean b_Valid,U32 q_GpsFCount );

/*
 ******************************************************************************
 * Function gts_LogTimeUpdate
 *
 * Description:
 *
 *  Generate the Time update log packet which provides the new time and the old
 *  time for the particular Time Src
 *
 * Parameters:
 *
 *  pz_PrevTime - Previous Time estimate from DB
 *  pz_CurrTime - Current computed Time estimate
 *  e_ModSrc    - Module Source that is injecting the Time
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogTimeUpdate( const gts_TimeType* pz_PrevTime,
                        const gts_TimeType* pz_CurrTime,
                        const gts_TimeSrcType e_ModSrc );

/*
 ******************************************************************************
 * Function gts_LogPosTimeEstMsg
 *
 * Description:
 *
 *  This function logs Position Estimate Message from PE
 *
 * Parameters:
 *
 *  pz_PosTime - PE Position Estimate Report Message
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogPosTimeEstMsg( const gts_PosEstMsgType* pz_PosTime );

/*
 ******************************************************************************
 * Function gts_LogFreqEstMsg
 *
 * Description:
 *
 *  This function logs Freq Estimate Message from FEST
 *
 * Parameters:
 *
 *  pz_FreqEstInfo - Freq Estimate Message
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogFreqEstMsg( const gts_FreqEstInfoType* pz_FreqEstInfo );

/*
 ******************************************************************************
 * Function gts_LogLteTimeMsg
 *
 * Description:
 *
 *  This function logs Time Info msg from LTE ML1 into GTS Input Log packet
 *
 * Parameters:
 *
 *  pz_LteTime - LTE Time info msg from LTE ML1
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogLteTimeMsg( const gts_LteTimeInfoType* pz_LteTime );

/*
 ******************************************************************************
 * Function gts_LogNr5gTimeMsg
 *
 * Description:
 *
 *  This function logs Time Info msg from NR5G into GTS Input Log packet
 *
 * Parameters:
 *
 *  pz_Nr5gTime - NR5G Time Information
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogNr5gTimeMsg( const gts_Nr5gTimeInfoType* pz_Nr5gTime );

/*
 ******************************************************************************
 * Function gts_CopyGtsTimeInfo
 *
 * Description:
 *
 *  Function copies the Time info to the packed log Time info structure
 *
 * Parameters:
 *
 *  pz_Log - GTS Time info Log Pointer
 *  pz_Time - GTS Time info pointer
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_CopyGtsTimeInfo( gts_TimeLogType* pz_Log, const gts_TimeType* pz_Time );

/*
 ******************************************************************************
 * Function gts_LogTimeDb
 *
 * Description:
 *
 *  This function logs the database either in Raw format or Time comparison
 *  format where it compares with Reference time or in Current Time format,
 *  where all the time is estiamted current time for all sources and logged
 *
 * Parameters:
 *
 *  e_DumpType - Type of Log DB Dump (Raw,Prop,Current)
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogTimeDb( gts_DbDumpType e_DumpType );

/*
 ******************************************************************************
 * Function gts_LogGpsFCount
 *
 * Description:
 *
 *  Generate the GPS FCount Event Log packet. This will help in Playback 
 *  processing of logs
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogGpsFCount( boolean b_Valid,U32 q_GpsFCount );

/*
 ******************************************************************************
 * Function gts_LogTimeTickReq
 *
 * Description:
 *
 *  Generate the TimeTickReq. This will help in Playback 
 *  processing of logs
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogTimeTickReq(const gts_TimeTickInfoType* pz_TimeTickInfo);

/*
 ******************************************************************************
 * Function gts_LogGsmTimeMsg
 *
 * Description:
 *
 *  This function logs Time Info msg from GSM into GTS Input Log packet
 *
 * Parameters:
 *
 *  pz_GsmTime - GSM Time info msg from GSM module
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogGsmTimeMsg( const gts_GsmTimeInfoType* pz_GsmTime );

/*
 ******************************************************************************
 * Function gts_LogWcdmaTimeMsg
 *
 * Description:
 *
 *  This function logs Time Info msg from WCDMA ML1 into GTS Input Log packet
 *
 * Parameters:
 *
 *  pz_WcdmaTime - WCDMA Time info msg from WCDMA module
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogWcdmaTimeMsg( const gts_WcdmaTimeInfoType* pz_WcdmaTime );

/*
 ******************************************************************************
 * Function gts_LogTdsTimeMsg
 *
 * Description:
 *
 *  This function logs Time Info msg from TDS into GTS Input Log packet
 *
 * Parameters:
 *
 *  pz_TdsTime - TDS Time info msg from TDS module
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogTdsTimeMsg( const gts_TdscdmaTimeInfoType* pz_TdsTime );

/*
 ******************************************************************************
 * Function gts_LogLatchDb
 *
 * Description:
 *
 *  Log the GPS RTC/USTMR latches sent by CC
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogLatchDb( const gts_TtrRegLatchInfoType* pz_TtrLatch );

/*
 ******************************************************************************
 * Function gts_LogQtime
 *
 * Description:
 *
 *  Generate the Qtime Event Log packet. This will help in Playback 
 *  processing of logs
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogQtime( gts_QtimeType* pz_Qtime );

/*
 ******************************************************************************
 * Function gts_LogGetGtsTime
 *
 * Description:
 *
 *  Generate the Get GTS Time Event Log packet. This will help in Playback 
 *  processing of logs
 *
 * Parameters:
 *
 *  Log Type, GTS structure, Offset, Error type
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogGetGtsTime( gts_GetTimeType e_GetTimeType,
                        gts_TimeType* pz_GtsTime,
                        DBL d_Offset,
                        gts_ErrorType e_Err );

/*
 ******************************************************************************
 * Function gts_LogDRSyncPulseTime
 *
 * Description:
 *
 *  Generate the Log Packet when DRSync pulse was programmed
 *
 * Parameters:
 *
 *  p_DrSyncParams - DR sync params
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogDRSyncPulseTime( const gts_DRSyncParamType* p_DrSyncParams );

/*
 ******************************************************************************
 * Function gts_LogTickProc
 *
 * Description:
 *
 *  Generate the Log Packet when Offset request was recieved
 *
 * Parameters:
 *
 *  None
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogTickProc();

/*
 ******************************************************************************
 * Function gts_LogConvNsApi
 *
 * Description:
 *
 *  Generate the Log Packet when Convert NS API is being used
 *
 * Parameters:
 *
 *  pz_GtsTime - GTS Time to be converted to NS
 *  t_TimeNs - Time in Ns value computed by SW
 *  q_UncNs  - Time Unc in Ns computed by SW
 *  e_Status - Status of the API (return value)
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_LogConvNsApi(const gts_TimeType* pz_GtsTime, U64 t_TimeNs, 
                      U32 q_UncNs, U8 e_Status);

/*
 ******************************************************************************
 * Function gts_LogDelTime
 *
 * Description:
 *
 *  Generate the Log Packet when time delete is received from MCF
 *
 * Parameters:
 *
 *  pz_Deltime - pointer to delete time structure 
 *
 * Dependencies:
 *F
 *  NoneF
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/

void gts_LogDelTime(const gts_DelTimeType *pz_Deltime);

/*
******************************************************************************
* Function gts_LogSSChangeInfo
*
* Description:
*
*  Generate the Log Packet when Serving cell info is received from SM
*
* Parameters:
*
*  pz_SSInfo - pointer to serving cell info
*
* Dependencies:
*
*  None
*
* Return value:
*
*  None
*
******************************************************************************
*/
void gts_LogSSChangeInfo(const gts_SSInfoType *pz_SSInfo);

#endif //#ifndef _GTS_LOG_H_
