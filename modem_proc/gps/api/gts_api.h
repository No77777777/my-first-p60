#ifndef GTS_API_H
#define GTS_API_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     Global Time Services Header File

GENERAL DESCRIPTION
This file contains API definitions between Global Time Service module and
other modules outside GNSS.

  Copyright (c) 2014 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2019 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

Version Control

$Header: //components/rel/gnss8.mpss/11.1/api/gts_api.h#4 $
$DateTime: 2020/07/29 21:33:35 $
$Author: pwbldsvc $

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*--------------------------------------------------------------------------
 * Include Files
 *-----------------------------------------------------------------------*/

#include "comdef.h"
#include "sys_type.h"
#include "lte_as.h"
#include "qw.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*--------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -----------------------------------------------------------------------*/
#define GTS_NR5G_RESERVED_LEN   4

/*--------------------------------------------------------------------------
 * Type Declarations
 *-----------------------------------------------------------------------*/

/* GTS time constellation/format type. Similar to 'me_GnssTypeVal' enum type in
 * 'modem_proc/gps/gnss/mgp/me/gen9/inc/mgp_me_api.h' */
typedef enum
{
  GTS_TIME_GPS,
  GTS_TIME_GLONASS,
  GTS_TIME_BDS,
  GTS_TIME_GALILEO,
  GTS_TIME_NAVIC,
  GTS_TIME_MAX
} gts_TimeFormatType;;

/* FEST source type */
typedef enum
{
  GTS_FEST_SRC_FT, //XO FEST based on FT curve
  GTS_FEST_SRC_PE_FIX, //XO FEST based on last PE fix
  GTS_FEST_SRC_SV_MEAS, //XO FEST based on SV speed measurements (doppler)
  GTS_FEST_SRC_PACE, //XO FEST based on PACE
  GTS_FEST_SRC_MAX
} gts_FestSrcType;

typedef enum  
{
  GTS_CLIENT_INVALID,
  GTS_CLIENT_GNSS_ME,
  GTS_CLIENT_V2X_L1,
  GTS_CLIENT_MAX
} gts_ClientType;

typedef enum
{
  GTS_PULSE_CLIENT_INVALID,
  GTS_PULSE_CLIENT_GNSS_ME,
  GTS_PULSE_CLIENT_5G,
  GTS_PULSE_CLIENT_TEST_DEBUG,
  GTS_PULSE_CLIENT_MAX = GTS_PULSE_CLIENT_TEST_DEBUG
} gts_PulseClientType;

typedef enum
{
 GTS_GL1_STATE_OTHER          = 0,
 GTS_GL1_STATE_IDLE           = 30,
 GTS_GL1_STATE_GPRS_IDLE      = 31,
 GTS_GL1_STATE_DEDICATED      = 50,
 GTS_GL1_STATE_TRANSFER       = 51,
 GTS_GL1_STATE_DTM            = 52,
 GTS_GL1_STATE_MULTI_SIM_IDLE = 61
} gts_gl1_state_type;

/* Max Time Source elements.
 * These are also used to reference the DB array */
typedef enum
{
  GTS_TIME_SRC_GNSS_PE, /* 0 */
  GTS_TIME_SRC_GNSS_ME, /* 1 */ //GPS time from PE clock_put to ME
  GTS_TIME_SRC_LTE_ML1_SUB1, /* 2 */
  GTS_TIME_SRC_FREQEST_ME, /* 3 */ //ME time propogation using XO Freq Estimation based on last PE position fix
  GTS_TIME_SRC_GSM_L1_SUB1, /* 4 */
  GTS_TIME_SRC_GSM_L1_SUB2, /* 5 */
  GTS_TIME_SRC_GSM_L1_SUB3, /* 6 */
  GTS_TIME_SRC_WCDMA_L1_SUB1, /* 7 */
  GTS_TIME_SRC_WCDMA_L1_SUB2, /* 8 */
  GTS_TIME_SRC_TDSCDMA_L1, /* 9 */
  GTS_TIME_SRC_CDMA_M1X, /* 10 */
  GTS_TIME_SRC_LTE_ML1_TTR_SUB1, /* 11 */
  GTS_TIME_SRC_SLW_CLK_TTR, /* 12 */
  GTS_TIME_SRC_LTE_ML1_SUB2, /* 13 */
  GTS_TIME_SRC_LTE_ML1_TTR_SUB2, /* 14 */
  GTS_TIME_SRC_CORE_SVC,         /* 15 */
  GTS_TIME_SRC_PMIC_RTC,       /* 16 */
  GTS_TIME_SRC_QTIME_CALIB,      /* 17 */
  GTS_TIME_SRC_EXT_INJ_XTRA,    /* 18 */ //External time injection from XTRA source
  GTS_TIME_SRC_PACE,      /* 19 */
  GTS_TIME_SRC_FREQEST_PACE, /* 20 */ //GTS_TIME_SRC_PACE time propogation using XO Freq Estimation based on last PE position fix
  GTS_TIME_SRC_EXT_INJ_REF, /* 21 */ //External time injection from Network/Reference source
  GTS_TIME_SRC_NR5G, /* 22 */
  GTS_TIME_SRC_ME_SV_DECODE, /* 23 */ //GPS time from ME SV decode
  GTS_TIME_SRC_MAX_SRC
} gts_TimeSrcType;

/* Reference Time structure for Timetags */
typedef struct 
{
  /* Qtime is Valid */
  boolean b_QtimeValid;
  /* Qtime or 64 bit Dal Time @ Time Estimate in units of 19.2 Mhz */
  uint64  t_Qtime;
  /* Bias if applicable in Qtime units */
  float   f_QtimeBias;
  /* Uncertainty of Qtime estimate in ms, w.r.t GPS RTC */
  float   f_QtimeUncMs;
} gts_QtimeType;

typedef enum 
{
  GTS_LTE_TIME_RPT_PERIODIC,
  GTS_LTE_TIME_REQUESTED_VALID,
  GTS_LTE_TIME_REQUESTED_NOSIB,
  GTS_LTE_TIME_REQUESTED_ABORT,
  GTS_LTE_TIME_REQUESTED_ERROR
}
gts_LteTimeStatusType;

typedef enum 
{
  GTS_TRUE_TIME_SUCCESS,
  GTS_TRUE_TIME_DELAY_NOT_AVAILABLE,
  GTS_TRUE_TIME_INPUT_INVALID,
  GTS_TRUE_TIME_OTHER
}
gts_TrueTimeStatusType;

/* LTE timing advance structure */
typedef struct
{
  /* Validity of timing advance data */
  boolean b_TaValid;

  /* Timing advance (Rx-Tx diff) in Ts units */
  int16 x_TaData;
  
  /* Serving cell physical ID. Range ~ 0 to 503 */
  uint16 w_ServPhyCellId;
}
gts_TaDataType;

typedef struct
{
  uint32  q_Ustmr[2];
  uint32  q_VsrcFromFW[2]; 
  uint32  q_VsrcPhFromFW[2]; 
  uint32  q_OstmrFromFW[2];
  uint32  q_OstmrPhFromFW[2];
  uint64  t_RefDlTime;
} gts_VsrcInfoType;

typedef struct 
{
  /* Request ID of the LTE Time Transfer request that triggered this report */
  uint8   u_ReqID;

  /* Report status - gts_LteTimeStatusType */
  uint8 e_RptStatus;

  uint32  q_Freq;    /* DL frequency - Same as lte_earfcn_t*/ 
  uint16  w_CellId;  /* Physical Cell ID */

  /* SIB8 TRUE: LTE NW is synchronized and supports 1x. FALSE: LTE NW is NOT synchronized */
  boolean b_Sib8Valid;   
  /* The CDMA2000 system time corresponding to the SFN boundary at or after the ending boundary of the SI-Window in which this Time Transfer was taken. The size is 52 bits and the unit is [CDMA2000 chips] */
  uint64  t_Sib8CdmaSystemTime; 
  
  boolean b_Sib16Valid; /* SIB16 System Time info*/
  uint64  t_Sib16GpsMsecs; /* The Abs GPS time at the SFN boundary in of 10 msecs */
  uint16  w_LeapSeconds; /* Number of leap seconds between UTC time and GPS time */
  
  uint16  w_Sfn;             /* System Frame Number at which data was collected */
  uint32  q_OStmrSfBoundary;  /* OSTMR at the boundary of the subframe */

  /* Latch Information */
  uint32  q_UstmrLatched;   /* USTMR Latched in 19.2 Mhz Units */
  uint32  q_OstmrLatched;   /* OSTMR Latched in 30.2 Mhz Units ( Ts ) */
  uint32  q_OstmrPhLatched; /* OSTMR Latched in 30.2 Mhz Units ( sub Ts )*/

  /* Best estimate of the current LTE system time uncertainty */
  uint32  q_SysTimeUncEst;

  /* VSRC Information for PCC */
  /* Mark if the next field is valid or not */
  boolean b_OstmrVsrcValid; 
  gts_VsrcInfoType z_RefVsrcInfo;  
  gts_VsrcInfoType z_CurrVsrcInfo;

  /* Timing advance data to predict SIB8 TTr uncertainity */
  gts_TaDataType z_TaData;

  /* Reference Time ( QTIME ) */
  gts_QtimeType  z_QtimeInfo;

  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  uint8 u_SubInfo;
}gts_LteTimeInfoType; 

/* Timetag structure for NR 5G */
typedef enum
{
  GTS_NR5G_BS_UNKNOWN = 0,
  GTS_NR5G_BS_WIDE_AREA,
  GTS_NR5G_BS_MEDIUM_RANGE,
  GTS_NR5G_BS_LOCAL_AREA
} gts_Nr5GBaseStationType;

typedef struct
{
  uint16 w_Sfn; /* Closest NR5G sub-frame number corresponding to below USTMR/NR5G STMR pair */
  uint32 q_Ustmr; /* USTMR in 19.2 MHz Units */
  uint32 q_Nr5gStmr; /* NR5G STMR in 30.2 MHz Units (Ts) */
  uint32 q_Nr5gStmrPh; /* NR5G STMR phase in 30.2 MHz Units (sub-Ts) scaled-up by 2^32 */
} gts_Nr5gDlInfoType;

typedef struct 
{
  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  uint8 u_SubInfo;

  uint32 q_Earfcn; /* DL frequency of primary serving cell */ 
  uint16 w_CellId; /* Physical Cell ID of primary serving cell */

  /* NR5G timing/latch info captured at current instance (current paging cycle) */
  gts_Nr5gDlInfoType z_CurrLatch;

  /* Reference Time (QTIME). This should be either same as current latched USTMR value (lower 24bits of Qtimer
   * will be the USTMR); or it should be captured as close to the current latched USTMR value as possible */
  gts_QtimeType z_QtimeInfo;

  /* Optional fields for SIB9 */
  boolean b_Sib9Valid; /* SIB9 System Time validity */
  uint64 t_Sib9GpsMsecs; /* Absolute GPS time at previous System frame boundary */
  uint16 w_LeapSeconds; /* Number of leap seconds between UTC time and GPS time */
  gts_Nr5gDlInfoType z_SysFrBdry; /* Nr5G STMR, SFN and USTMR corresponding to system frame bdry of SIB9 system time */
  boolean b_TaValid; /* Timing advance validity */
  int32 l_TimingAdvance; /* Timing advance value */

  /* Optional field for base station type */
  uint8 e_BsType; /* Same as enum 'gts_Nr5GBaseStationType' */

  /* Reserved fields */
  uint32 q_Reserved[GTS_NR5G_RESERVED_LEN];
} gts_Nr5gTimeInfoType;

typedef struct
{
  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  uint8 u_SubInfo;
  /* Cell Information */
  /* Base Station Identification code */
  uint8 u_BSIC;
  /* Control Channel Frequency */
  ARFCN_T z_Arfcn;

  /* Time Information */
  /* Frame Number 0..(26L*51L*2048L-1)  ~4.615ms frames */
  uint32 q_FrameNum;
  /* Time Slot Number 0..7 */
  uint8 u_TimeSlot;
  /* QSymbol Count (sub fn) */
  uint32 q_QSymCnt;

  /* GSM State. Of type 'gts_gl1_state_type' */
  uint8 u_GL1State;

  /* Additional Drift in the time in QSym Units (if any) */
  int8 b_DriftQSym;

  /* Reference Time ( QTIME ) */
  gts_QtimeType  z_QtimeInfo;
} gts_GsmTimeInfoType;


typedef struct
{
  /* Subscription Information – For MultiSim Support.
   * Type - 'sys_modem_as_id_e_type' as defined in 'mmcp\api\public\sys.h' */
  uint8 u_SubInfo;

  /* Cell Information */
  /* DL frequency and Primary scrambling code */
  uint16  w_Freq;
  uint16  w_PSC;

  /* Time Information */
  /* 10.3.6.75  System Frame Number  0..4095 10ms frames */
  uint16  w_SysFn;

  /* 0..307200 Chipx8 per frame */
  uint32  q_CX8Num;

  /* 24-bit XO value (USTMR) at SFN/Cx8 boundary */
  uint32 q_UstmrAtCx8;

  /* WCDMA State */
  uint8 u_WL1State;

  /* Reference Time ( QTIME ) */
  gts_QtimeType  z_QtimeInfo;
}gts_WcdmaTimeInfoType;


typedef struct
{
  /* Subscription Information – For MultiSim Support */
  uint8 u_SubInfo;
  /* Cell Information */
  /* DL frequency. */
  uint16  w_Freq;

  /* Cell Param ID */
  uint16  w_CellId;

  /* Time Information */
  /* 64-bits value in TDS chipx8 units at status dump, clocked @ 10.24 MHz */
  uint64  q_TdsTime;

  /* 30-bits value in TDS sub chipx8 units at status dump */
  uint32  q_TdsPhase;

  /* USTMR Value at status dump, clocked @ 19.2 Mhz */
  uint32 q_Ustmr;

  /* TDS L1 State 0 – Paging, 1- Traffic*/
  uint8 u_TDSL1State;

  /* Reference Time ( QTIME ) */
  gts_QtimeType  z_QtimeInfo;
}  gts_TdscdmaTimeInfoType;


/* Time Transfer Data Structure */
typedef struct
{
  /* 1x system time in qword format */
  qword   t_SystemTime;

  /* Current value of the slow clock counter */
  uint64  t_Qtime;

  /* CDMA RTC value latched at the last Sync80 tick (in CDMA Chipx8) */
  uint32  q_CdmaRtcSync80;

  /* Current 1x Ref PN offset */
  uint16  w_RefPN;

  /* Current 1x serving frequency band associated to Ref PN */
  uint8   u_Band;

  /* Current 1x serving frequency channel associated to Ref PN */
  uint16  w_Channel;

  /* Current master finger position, where master fingers is
   * assigned to Ref PN Offset */
  int32   l_MasterFingerPosition;

  /* Maximum Antenna Range */
  uint32  q_Mar;

  /* Best estimate of the current 1x system time uncertainty */
  uint32  q_SystemTimeUncertaintyEstimate;

  /* CDMA RTC value latched at the same time as GPS RTC (in CDMA Chipx32) */
  uint32  q_CdmaRtcLatched;

  /* CDMA RTC phase value latched at the last Sync80 tick (in CDMA Chipx2^35) */
  uint32  q_CdmaRtcSync80Phase;

  /* Universal STMR value latched at the same time as the first latched
     CDMA/GNSS RTC set (in 19.2MHz untis) */
  uint32  q_UnivStmrLatched;

  /* CDMA RTC STMR control register read at the same time as the first latched
     CDMA/GNSS RTC set (in CDMA Chipx2^35 units) */
  uint32  q_CdmaRtcFcw;

  /* Difference between CDMA RTC and VSRC STMR (in CDMA Chipx8192 units) */
  uint32  q_CdmaRtcVsrcStmrDiff;

  /* Reference Time ( QTIME ) */
  gts_QtimeType  z_QtimeInfo;  
}
gts_CdmaTimeInfoType;

typedef struct
{
  /* If the time below is valid or not */
  boolean b_Valid;

  /* Time Source */
  uint8 e_TimeSrc; //of type 'gts_TimeSrcType'

  /* Absolute GPS Time related information */
  uint16  w_GpsWeek;     /* GPS week number at reference tick [weeks] */
  uint32  q_GpsMsec;     /* GPS msec at reference tick [msecs] */
  float   f_ClkTimeBias; /* Clock bias [msecs]. Constraint to >-0.5 and <0.5 */
  float   f_ClkTimeUncMs; /* 1 sided max time bias uncertainty [msecs]in float*/
  double  d_ClkTimeUncMs;/* Tunc in double version is used for math/calculation
                         to maintain precision of the value when tunc is large*/

  /* Reference Time ( QTIME ) */
  gts_QtimeType  z_QtimeInfo;
  
  /* Freq Error Information */
  boolean b_FreqEstValid;  /* Frequency Error Estimate is valid */
  int32 x_FreqEstErrPpb;   /* Freq Error Estimate in PPB (2^-10 PPM) */
  int32 x_FreqEstErrUncPpb;   /* Freq Error Estimate Unc in PPB (2^-10 PPM) */

  /* Velocity Information */
  boolean b_VelValid;
  float f_VelocityMs;      /* Velocity of the device in meter/sec */

  /* Leap Second Info */
  boolean b_LeapSecValid;
  uint8 u_LeapSecond;      /* Leap Second value in Seconds */
  
  /* GPS Clock Specific Fields */
  boolean b_FCountValid; /* Mark if FCount below is valid */
  uint32  q_GpsFCount;    /* GPS FCount at the Time Estimate */
  boolean b_HighPriority; /* Mark if the time was after a time refresh */

  /* XO frequency estimation source of enum type 'gts_FestSrcType' */
  uint8 e_FestSrc;

  /* Flag to indicate to GPS MC to force-use the time */
  boolean b_ForceFlag;

  /* Enum to indicate constellation/format type. Same as enum 'gts_TimeFormatType' */
  uint8 e_TimeFormat;
} gts_TimeType;

/* Callback Function for recieving Time updates */
typedef void ( * gts_TimeEstCbType )( const gts_TimeType * );

/* Request Time Info structure */
typedef struct
{
  uint8 e_ClientInfo; //of type 'gts_ClientType'
  boolean b_ContUpdates;
  boolean b_HighPriority;
} gts_ReqTimeInfoType;

/* Enums for selecting GPIO number for pulses */
typedef enum
{
  GTS_SM8150_PULSE_GPIO_NUM_60 = 60, //default config for SM8150
	 GTS_SM8150_PULSE_GPIO_NUM_76 = 76,
 	GTS_SM8150_PULSE_GPIO_NUM_77 = 77,
 	GTS_SM8150_PULSE_GPIO_NUM_81 = 81,
 	GTS_SM8150_PULSE_GPIO_NUM_82 = 82,
 	GTS_SDX24_PULSE_GPIO_NUM_42 = 42,
 	GTS_9x55_PULSE_GPIO_NUM_39 = 39,
  GTS_SDX55_PULSE_GPIO_NUM_32 = 32, //default config for SDX55
	 GTS_SDX55_PULSE_GPIO_NUM_31 = 31,
 	GTS_SDX55_PULSE_GPIO_NUM_76 = 76,
} gts_PulseGpioNumType;

/*--------------------------------------------------------------------------
 * Function Declarations
 *-----------------------------------------------------------------------*/
/*
 ******************************************************************************
 * Function gts_ReqGpsTime
 *
 * Description:
 *
 *  This function requests for the latest time from GTS module 
 *
 * Parameters:
 *
 *  pz_ReqTimeInfo - Pointer related to requesting time info like client info
 *  and continous updates
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  TRUE if the request was sent successfully, FALSE otherwise
 *
 ******************************************************************************
*/
boolean gts_ReqGpsTime( gts_ReqTimeInfoType* pz_ReqTimeInfo );

/*
 ******************************************************************************
 * Function gts_Nr5gTimeUpdate
 *
 * Description:
 *
 *  This function sends NR5G Time update IPC to GTS. It will be invoked by NR5G
 *  ML1 to send periodic time update information to GTS 
 *
 * Parameters:
 *
 *  pz_Nr5gTime - Pointer to the NR 5G Time structure 
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
void gts_Nr5gTimeUpdate( const gts_Nr5gTimeInfoType* pz_Nr5gTime );

/*
 ******************************************************************************
 * Function gts_LteTimeUpdate
 *
 * Description:
 *
 *  This function sends current LTE Time to GTS module using IPC
 *
 * Parameters:
 *
 *  pz_LteTime - Pointer to the LTE Time structure 
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

void gts_LteTimeUpdate( const gts_LteTimeInfoType* pz_LteTime );

/*
 ******************************************************************************
 * Function gts_GsmTimeUpdate
 *
 * Description:
 *
 *  This function sends current GSM Time to GTS module using IPC
 *
 * Parameters:
 *
 *  pz_GsmTime - Pointer to the GSM Time structure 
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

void gts_GsmTimeUpdate( const gts_GsmTimeInfoType* pz_GsmTime );

/*
 ******************************************************************************
 * Function gts_WcdmaTimeUpdate
 *
 * Description:
 *
 *  This function sends current WCDMA Time to GTS module using IPC
 *
 * Parameters:
 *
 *  pz_WcdmaTime - Pointer to the WCDMA Time structure 
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

void gts_WcdmaTimeUpdate( const gts_WcdmaTimeInfoType* pz_WcdmaTime );

/*
 ******************************************************************************
 * Function gts_TdscdmaTimeUpdate
 *
 * Description:
 *
 *  This function sends current TDS Time to GTS module using IPC
 *
 * Parameters:
 *
 *  pz_TdscdmaTime - Pointer to the TDS Time structure 
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

void gts_TdscdmaTimeUpdate( const gts_TdscdmaTimeInfoType* pz_TdscdmaTime );

/*
 ******************************************************************************
 * Function gts_RegForGtsTime
 *
 * Description:
 *
 *  This function allows clients to register a callback function to GTS for
 *  sending Time updates
 *
 * Parameters:
 *
 *  e_ClientInfo - Information of which client is registering
 *  p_CbFunction - Callback function which will be invoked by GTS to send
 *  time information to the client
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  TRUE if registration was successful, FALSE otherwise
 *
 ******************************************************************************
*/
boolean gts_RegForGtsTime( gts_ClientType e_ClientInfo, gts_TimeEstCbType p_CbFunction ); 

/*
 ******************************************************************************
 * gts_GetQtime
 *
 * Function description:
 *
 * This function returns the Qtime @ the calling instant
 *
 * Parameters:
 *  pz_Qtime - Qtime structure ( Validity,Time and Bias ) is updated
 *
 * Return value:
 *  None, but callers should check pz_Qtime->b_QtimeValid which will be TRUE 
 *  if the get time was successful, FALSE otherwise
 *
 ******************************************************************************
*/
void gts_GetQtime ( gts_QtimeType* pz_Qtime );

/*
 ******************************************************************************
 * Function gts_LteTtrUpdate
 *
 * Description:
 *
 *  This function sends current LTE Time to GTS module using IPC
 *
 * Parameters:
 *
 *  p_TTRpt - Pointer to the LTE Time structure 
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

void gts_LteTtrUpdate( gts_LteTimeInfoType * p_TTRpt );

/*
 ******************************************************************************
 * Function gts_CdmaTtrUpdate
 *
 * Description:
 *
 *  This function sends current CDMA Time to GTS module using IPC
 *
 * Parameters:
 *
 *  p_TTRpt - Pointer to the CDMA Time structure 
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
void gts_CdmaTtrUpdate( gts_CdmaTimeInfoType * p_TTRpt );

/*
 ******************************************************************************
 * Function gts_ConvertGtsTimeToNs
 *
 * Description:
 *
 *  This function takes GTS Time as input and provides the output in nanoseconds
 *
 * Parameters:
 *
 *  p_GtsTime - Pointer to the GTS Time structure 
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  GPS Time in Ns units (0 if there is any error)
 *
 ******************************************************************************
*/
uint64 gts_ConvertGtsTimeToNs( const gts_TimeType * p_GtsTime );

/*
 ******************************************************************************
 * Function gts_ConvGtsTimeToTrueNs
 *
 * Description:
 *
 *  This function calculates the True Time in Ns using the GTS Time that was 
 *  provided. True time is the GPS Time at the Antenna port. So the function
 *  corrects for the patch Delay between the Antenna port and Sample Server
 *
 * Parameters:
 *
 *  pz_GtsTime - Pointer to the GTS Time structure that has to be converted
 *  p_GpsTimeNs - GPS Time in Ns (since Jan 6, 1980)
 *  p_GpsTimeUncNs - Unc/Error Associated with the above Time in Ns
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  GTS_TRUE_TIME_SUCCESS - True time was successfully computed
 *  GTS_TRUE_TIME_DELAY_NOT_AVAILABLE - Time was converted, but delay was not
 *  available
 *  GTS_TRUE_TIME_INPUT_INVALID - GTS Time provided as input had some error
 *  GTS_TRUE_TIME_OTHER - Unknown error
 *
 ******************************************************************************
*/
gts_TrueTimeStatusType gts_ConvGtsTimeToTrueNs( const gts_TimeType * pz_GtsTime, 
                                                uint64 *p_TimeNs, 
                                                uint32 *p_TimeUncNs );

/*
 ******************************************************************************
 * Function gts_GetTime
 *
 * Description:
 *
 *  Get the current GTS/GPS Time in Wk, Ms, Bias and UNC format. GTS Time is the 
 *  Numpber of Millisec from Jan 6,1980. The Time is mapped to the current 
 *  QTimer which is the XO Counter Value. In cases where GTS Time is not available
 *  just the QTimer filed in the structre will be updated and the function will
 *  return FALSE indicating GTS Time is valid or not
 *
 * Parameters:
 *
 *  pz_GtsTime - Pointer to the GTS Time Estimate
 *
 * Dependencies:
 *
 *  None
 *
 * Return value:
 *
 *  TRUE if Time was filled in correctly, FALSE otherwise
 *
 ******************************************************************************
*/
boolean gts_GetTime( gts_TimeType* pz_GtsTime );

/*
 ******************************************************************************
 *
 * Function
 *   gts_TimeDiffMsec
 *
 * Description:
 *  Calculate difference between two GTS Time values ( T2-T1 ) either using GTS
 *  time or if it is not valid then using QTime. Although the resolution is ms,  
 *  accuracy depends on various factors. So if the time diff is more than 5 min,
 *  then it can have an error of upto 1 ms
 *
 * Parameters:
 *  pz_T2 - GTS Time at Instance 2
 *  pz_T1 - GTS Time at Instance 1
 *  px_Diff - Difference value in Ms is updated into this pointer
 *
 * Return value:
 *  TRUE if the calculation was successful, FALSE otherwise
 *
  ******************************************************************************
*/
boolean gts_TimeDiffMsec( const gts_TimeType* pz_T2,
                          const gts_TimeType* pz_T1,
                          int32* px_Diff);

/*
 ******************************************************************************
 * Function:
 *
 *   gts_RegisterPulse
 *
 * Description:
 *
 *  Function for external clients to register/de-register for the GNSS PPS hardware usage
 *
 * Parameters:
 *
 *  u_ClientId - Client id to use
 *  b_Enable - Enable or Disable Pulse for the client
 *
 * Return value:
 *
 *  TRUE if registration was successful, false otherwise
 *
 ******************************************************************************
*/
boolean gts_RegisterPulse( const gts_PulseClientType u_ClientId, const boolean b_Enable );

/*
 ******************************************************************************
 * Function:
 *
 *   gts_ProgramPulse
 *
 * Description:
 *
 *  Function for external clients to program a pulse on the GPIO with USTMR at which it 
 *  should be asserted and USTMR at which it has to be deasserted
 *
 * Parameters:
 *
 *  u_ClientId - External client ID
 *  q_AssertCnt - USTMR at which the GPIO will be pulled up
 *  q_DeAssertCnt - USTMR at which the GPIO will be pulled down
 *  b_Enable - Whether to program pulse or disable the programming
 *
 * Return value:
 *
 *  TRUE if programming was successful, false otherwise
 *
 ******************************************************************************
*/
boolean gts_ProgramPulse( const gts_PulseClientType u_ClientId,
                          const uint32 q_AssertCnt, const uint32 q_DeAssertCnt,
                          const boolean b_Enable );

/*
 ******************************************************************************
 * Function:
 *
 *   gts_ReconfigPulseGpio
 *
 * Description:
 *
 *  Function for external clients to reconfigure GPIO option for
 *  pulse
 *
 * Parameters:
 *
 *  w_GpioOption - GPIO Configure option number. 
 *      			Same as enum type 'gts_PulseGpioNumType'
 *
 * Return value:
 *
 *  None
 *
 ******************************************************************************
*/
void gts_ReconfigPulseGpio( const uint16 w_GpioOption );                          

/*
 ******************************************************************************
 *
 * Function
 *   gts_InjectAbsTime
 *
 * Description:
 *  API used by external modules to inject absolute time (in GPS time format) into GTS.
 *  The modules could be external time injection from XTRA/NTP, PMIC based propogation etc.
 *
 * Parameters:
 *  p_AbsTime - GTS Time from other module
 *
 * Return value:
 *  TRUE if IPC send was sucessful, FALSE otherwise
 *
 ******************************************************************************
*/
boolean gts_InjectAbsTime( gts_TimeType *p_AbsTime );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GTS_API_H */
