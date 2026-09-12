#ifndef TLE_LOG_H
#define TLE_LOG_H
/*==============================================================================

                  Terrestrial Location Engine Log

GENERAL DESCRIPTION
   This file contains constants, structures and functions prototypes necessary
   for logging and debugging TLE.  This includes F3 message definitions, logs,
   and events.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

==============================================================================*/


/*==============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
  
Version Control

  $Header: //components/rel/gnss8.mpss/11.1/gnss/tle/1_0/common/inc/tle_log.h#1 $ 
  $DateTime: 2019/10/24 01:30:36 $ 
  $Author: pwbldsvc $ 

when          who     what, where, why
--------      ---     ----------------------------------------------------------

==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "gps_variation.h"
#include "err.h"
#include "msg.h"
#include "tle_common.h"
#include "msg_mask.h"
#include "tlm_api.h"
#include "tle_asn1_util.h"

/*==============================================================================

                    DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/

/*** BEGIN: Packed structures for logging purposes. ***/


/*==============================================================================

                                  CONSTANTS

==============================================================================*/

#define C_TLE_LOG_GTP_CONFIG_MASK_EULA_ENABLE            (0x01)
#define C_TLE_LOG_GTP_CONFIG_MASK_GLOBAL_UPLOAD_ENABLE   (0x02)

#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_UPLD_CONFIG_PRESENT           (0x0001)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_UPLD_SCREEN_PRESENT           (0x0002)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_UPLD_MODE_PRESENT             (0x0004)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_UPLD_FREQ_PRESENT             (0x0008)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_MAX_SCAN_PER_CELL_PRESENT     (0x0010)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_MAX_GRID_DEG_PRESENT          (0x0020)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_MIN_SCAN_INTERVAL_SEC_PRESENT (0x0040)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_PUNC_PRESENT                  (0x0080)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_SCAN_SELECT_PARAM_PRESENT     (0x0100)
#define C_TLE_LOG_GTP_CONFIG_TDP_MASK_SCAN_SELECT_METHOD_PRESENT    (0x0200)

#define C_TLE_LOG_GTP_TDP_CLIENT_UPLD_MASK_UPLD_CONTENT_ALL            (0x01)
#define C_TLE_LOG_GTP_TDP_CLIENT_UPLD_MASK_UPLD_CONTROL_ENABLED        (0x02)
#define C_TLE_LOG_GTP_TDP_CLIENT_UPLD_MASK_UPLD_SCREEN_ENABLED         (0x04)
#define C_TLE_LOG_GTP_TDP_CLIENT_UPLD_MASK_UPLD_MODE_AP                (0x08)
#define C_TLE_LOG_GTP_TDP_CLIENT_UPLD_MASK_TDP_UPLD_CONTROL_ENABLED    (0x10)

#define C_TLE_LOG_TDP_NUM_MAX_TDP_MEAS    (24)
#define C_TLE_HOME_CARRIER_ID_GWL  (1)
#define C_TLE_HOME_CARRIER_ID_CDMA (2)
#define TLE_TDP_LOG_OPERATION_MODE_BINARY (0)
#define TLE_TDP_LOG_OPERATION_MODE_ASN1   (1)

#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_GPS_TIME_PRESENT    (0x01)
#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_POS_REL_PRESENT     (0x02)
#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_ALT_PRESENT         (0x04)
#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_VERT_UNC_PRESENT    (0x08)
#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_CONFIDENCE_PRESENT  (0x10)
#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_VEL_PRESENT         (0x20)
#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_VEL_UNC_PRESENT     (0x40)
#define C_TLE_LOG_TDP_LOCATION_INFO_MASK_VEL_V2_PRESENT      (0x80)

#define C_TLE_LOG_TDP_SERVING_CELL_CONFIG_SIB8_VALID         (0x01)
#define C_TLE_LOG_TDP_SERVING_CELL_CONFIG_SIB16_VALID        (0x02)

#define C_TLE_LOG_TDP_CRS_BW_6           (0)
#define C_TLE_LOG_TDP_CRS_BW_15          (1)
#define C_TLE_LOG_TDP_CRS_BW_25          (2)
#define C_TLE_LOG_TDP_CRS_BW_50          (3)
#define C_TLE_LOG_TDP_CRS_BW_75          (4)
#define C_TLE_LOG_TDP_CRS_BW_100         (5)
#define C_TLE_LOG_TDP_CRS_BW_UNKNOWN     (6)

#define C_TLE_LOG_TDP_SERVING_CELL_CONFIG_CYCLIC_PREFIX_NORMAL   (0)
#define C_TLE_LOG_TDP_SERVING_CELL_CONFIG_CYCLIC_PREFIX_EXTENDED (1)

#define C_TLE_LOG_TDP_SERVING_CELL_CONFIG_ANTENNA_PORTS_1_OR_2   (0)
#define C_TLE_LOG_TDP_SERVING_CELL_CONFIG_ANTENNA_PORTS_4        (1)

#define C_TLE_LOG_TDP_LTE_CELL_ID_MASK_MCC_VALID        (0x01)
#define C_TLE_LOG_TDP_LTE_CELL_ID_MASK_MNC_VALID        (0x02)
#define C_TLE_LOG_TDP_LTE_CELL_ID_MASK_TAC_VALID        (0x04)
#define C_TLE_LOG_TDP_LTE_CELL_ID_MASK_CELL_ID_VALID    (0x08)

#define C_TLE_LOG_TDP_UPLOAD_MEAS_MASK_SIGNAL_POWER_VALID    (0x01)
#define C_TLE_LOG_TDP_UPLOAD_MEAS_MASK_SIGNAL_QUALITY_VALID  (0x02)
#define C_TLE_LOG_TDP_UPLOAD_MEAS_MASK_RX_TX_TIME_DIFF_VALID (0x04)
#define C_TLE_LOG_TDP_UPLOAD_MEAS_MASK_TOT_MS_VALID          (0x08)
#define C_TLE_LOG_TDP_UPLOAD_MEAS_MASK_SERVING_CELL          (0x10)

#define C_TLE_LOG_TDP_UPLOAD_MEAS_RANGING_MEASTYPE_PRS       (0x10)
#define C_TLE_LOG_TDP_UPLOAD_MEAS_RANGING_MEASTYPE_OPCRS     (0x10)
#define C_TLE_LOG_TDP_UPLOAD_MEAS_RANGING_MEASTYPE_UKCRS     (0x10)

/*==============================================================================

                                    MACROS

==============================================================================*/
/* TLE Modules */
#define TLE_TLM_MODULE 0
#define TLE_XTM_MODULE 1


void tle_printf(  const char* file, uint32 line, const char *p_Format, uint32 q_Data1, 
                  uint32 q_Data2, uint32 q_Data3 );

#ifdef FEATURE_TLE_SMART

#define TLE_MSG_LOW(a,b,c,d)  tle_printf(__FILENAME__, __LINE__, a,b,c,d)
#define TLE_MSG_MED(a,b,c,d)  tle_printf(__FILENAME__, __LINE__, a,b,c,d)
#define TLE_MSG_HIGH(a,b,c,d) tle_printf(__FILENAME__, __LINE__, a,b,c,d)
#define TLE_MSG_ERR(a,b,c,d)  tle_printf(__FILENAME__, __LINE__, a,b,c,d)

#define TLM_MSG_ERR(a,b,c,d)   tle_printf(__FILENAME__,__LINE__,a,b,c,d)
#define TLM_MSG_FATAL(a,b,c,d) tle_printf(__FILENAME__,__LINE__,a,b,c,d)

#define TLM_MSG_LOW(a,b,c,d)     tle_printf(__FILENAME__,__LINE__,"=TLM TASK= " a,b,c,d)
#define TLM_MSG_MED(a,b,c,d)     tle_printf(__FILENAME__,__LINE__,"=TLM TASK= " a,b,c,d)
#define TLM_MSG_HIGH(a,b,c,d)    tle_printf(__FILENAME__,__LINE__,"=TLM TASK= " a,b,c,d)
#define TLM_MM_MSG_LOW(a,b,c,d)  tle_printf(__FILENAME__,__LINE__,"=TLM TASK-MM= " a,b,c,d)
#define TLM_MM_MSG_MED(a,b,c,d)  tle_printf(__FILENAME__,__LINE__,"=TLM TASK-MM= " a,b,c,d)
#define TLM_MM_MSG_HIGH(a,b,c,d) tle_printf(__FILENAME__,__LINE__,"=TLM TASK-MM= " a,b,c,d)
#define TLM_MM_MSG_ERR(a,b,c,d)  tle_printf(__FILENAME__,__LINE__,"=TLM TASK-MM= " a,b,c,d)

/* XTM F3 Messages */
#define XTM_MSG_LOW(a,b,c,d)   tle_printf(__FILENAME__,__LINE__,"=XTM TASK= " a,b,c,d)
#define XTM_MSG_MED(a,b,c,d)   tle_printf(__FILENAME__,__LINE__,"=XTM TASK= " a,b,c,d)
#define XTM_MSG_HIGH(a,b,c,d)  tle_printf(__FILENAME__,__LINE__,"=XTM TASK= " a,b,c,d)
#define XTM_MSG_ERR(a,b,c,d)   tle_printf(__FILENAME__,__LINE__,"=XTM TASK= " a,b,c,d)

#else 
#define TLE_MSG_LOW(a,b,c,d)  MSG_3(MSG_SSID_TLE, MSG_LEGACY_LOW, a,b,c,d)
#define TLE_MSG_MED(a,b,c,d)  MSG_3(MSG_SSID_TLE, MSG_LEGACY_MED, a,b,c,d)
#define TLE_MSG_HIGH(a,b,c,d) MSG_3(MSG_SSID_TLE, MSG_LEGACY_HIGH, a,b,c,d)
#define TLE_MSG_ERR(a,b,c,d)  MSG_3(MSG_SSID_TLE, MSG_LEGACY_ERROR, a,b,c,d)

#define TLM_MSG_LOW(a,b,c,d)  MSG_3(MSG_SSID_TLE_TLM, MSG_LEGACY_LOW, a,b,c,d)
#define TLM_MSG_MED(a,b,c,d)  MSG_3(MSG_SSID_TLE_TLM, MSG_LEGACY_MED, a,b,c,d)
#define TLM_MSG_HIGH(a,b,c,d) MSG_3(MSG_SSID_TLE_TLM, MSG_LEGACY_HIGH, a,b,c,d)
#define TLM_MSG_ERR(a,b,c,d)  MSG_3(MSG_SSID_TLE_TLM, MSG_LEGACY_ERROR, a,b,c,d)
#define TLM_MM_MSG_LOW(a,b,c,d)  MSG_3(MSG_SSID_TLE_TLM_MM, MSG_LEGACY_LOW, a,b,c,d)
#define TLM_MM_MSG_MED(a,b,c,d)  MSG_3(MSG_SSID_TLE_TLM_MM, MSG_LEGACY_MED, a,b,c,d)
#define TLM_MM_MSG_HIGH(a,b,c,d) MSG_3(MSG_SSID_TLE_TLM_MM, MSG_LEGACY_HIGH, a,b,c,d)
#define TLM_MM_MSG_ERR(a,b,c,d)  MSG_3(MSG_SSID_TLE_TLM_MM, MSG_LEGACY_ERROR, a,b,c,d)

/* XTM F3 Messages */
#define XTM_MSG_LOW(a,b,c,d)  MSG_3(MSG_SSID_TLE_XTM, MSG_LEGACY_LOW, a,b,c,d)
#define XTM_MSG_MED(a,b,c,d)  MSG_3(MSG_SSID_TLE_XTM, MSG_LEGACY_MED, a,b,c,d)
#define XTM_MSG_HIGH(a,b,c,d) MSG_3(MSG_SSID_TLE_XTM, MSG_LEGACY_HIGH, a,b,c,d)
#define XTM_MSG_ERR(a,b,c,d)  MSG_3(MSG_SSID_TLE_XTM, MSG_LEGACY_ERROR, a,b,c,d)

#endif /* FEATURE_TLE_SMART */
/*==============================================================================

                          STRUCTURES AND ENUMERATIONS

==============================================================================*/
#ifdef FEATURE_CGPS_PLAYBACK
#pragma pack(push)
#pragma pack(1)
#endif

typedef PACKED struct PACKED_POST
{
  uint8         u_Type;                         /* XTRA-T Server IP address Type Used 
                                                   0 - IPV4 , 1 - URL, 2 - IPV6                        */
  char          cz_Url[XTT_IP_ADDRESS_SIZE];    /* XTRA-T Server URL Address. Null terminated.         */
  char          cz_Uri[XTT_IP_ADDRESS_SIZE];    /* XTRA-T Server URI Address. Null terminated.         */
  uint32        q_V4;                           /* XTRA-T Server IPv4 Address                          */
  uint16        w_V6[XTT_IP_ADDRESS_V6_SIZE];   /* XTRA-T Server IPv6 Address                          */
  uint16        w_Port;                         /* XTRA-T Server IP Port Number                        */  
} xtt_PackedIPAddressType;

typedef PACKED struct PACKED_POST
{
  uint32 q_GpsMsec;    /* GPS msec*/
  uint16 w_GpsWeek;    /* GPS week number */
} xtt_PackedGpsTimeType;

typedef PACKED struct PACKED_POST
{
  xtt_PackedGpsTimeType z_GpsTime;
  int32           l_TimeTagTimeBias;  /* unit of nano seconds */
  uint32          q_TimeTagTimeUncNsec; /* unit of nano seconds */
} xtt_PackedTimeTagCommonType;

typedef PACKED struct PACKED_POST
{
  xtt_PackedTimeTagCommonType z_TimeTagCommon;
  uint32                     q_Fn;          /* Frame Number ~4.615ms frames  */
  uint32                     q_SymbolCount; /* QSymbol Count (sub fn).*/
  uint8                      u_TimeSlot;    /* Time Slot Number 0..7 */
} xtt_PackedTimeTagGsmType;

typedef PACKED struct PACKED_POST
{
  uint64                     t_TdsTime;    /* 64-bits value in TDS chipx8 units at status dump, clocked @ 10.24 MHz */
  uint32                     q_TdsPhase;   /* 30-bits value in TDS sub chipx8 units at status dump */
} xtt_PackedTimeTagTdsValueType;

typedef PACKED struct PACKED_POST
{
  uint16                     w_SFN;           /* System Frame Number 0..4095 - 10 ms frames */
  uint32                     q_PnPosChipx8;   /* . 0..307200 Chipx8 per frame */
} xtt_PackedTimeTagWcdmaValueType;

typedef PACKED union PACKED_POST
{
  xtt_PackedTimeTagWcdmaValueType z_TimeTagWcdmaValue;
  xtt_PackedTimeTagTdsValueType z_TimeTagTdsValue;
} xtt_PackedTimeTagValueType;

typedef PACKED struct PACKED_POST
{
  xtt_PackedTimeTagCommonType z_TimeTagCommon;
  uint8 u_Flags; /* Currently LSB used to mark TDS time-tags */
  xtt_PackedTimeTagValueType z_TimeTag;
} xtt_PackedTimeTagWcdmaType;

typedef PACKED struct PACKED_POST
{
  xtt_PackedTimeTagCommonType z_TimeTagCommon;
} xtt_PackedTimeTagLteType;

typedef PACKED union PACKED_POST
{
  xtt_PackedTimeTagGsmType   z_TimeTagGsm;
  xtt_PackedTimeTagWcdmaType z_TimeTagWcdma;
  xtt_PackedTimeTagLteType   z_TimeTagLte;
} xtt_PackedTimeTagType;

typedef PACKED struct PACKED_POST 
{
  uint16 w_SuccessSelfCellDbCount;   /* No. of Successful Lookups, Self-learned CellDB */
  uint16 w_SuccessSelfSnDbCount;     /* No. of Successful Lookups, Self-learned SnDB */
  uint16 w_SuccessDownldCellDbCount; /* No. of Successful Lookups, Downloaded CELLDB */
  uint16 w_SuccessDownldSnDbCount;   /* No. of Successful Lookups, Downloaded SnDB */
  uint16 w_FailCellDbCount;          /* No. of Failed Lookups, Downloaded CellDB */
  uint16 w_FailSnDbCount;            /* No. of Failed Lookups, Downloaded SnDB */
  uint16 w_AddedSelfCellDbCount;     /* No. Added, Self-learned CellDB */
  uint16 w_AddedSelfSnDbCount;       /* No. Added, Self-learned SnDB */
  uint16 w_AddedDownldCellDbCount;   /* No. Added, Downloaded CellDB*/
  uint16 w_AddedDownldSnDbCount;     /* No. Added, Downloaded SnDB */
  uint16 w_DeletedSelfCellDbCount;   /* No. Deleted, Self-learned CellDB*/
  uint16 w_DeletedSelfSnDbCount;     /* No. Deleted, Self-learned SnDB */
  uint16 w_DeletedDownldCellDbCount; /* No. Deleted, Downloaded CellDB */
  uint16 w_DeletedDownldSnDbCount;   /* No. Deleted, Downloaded SnDB */
  uint16 w_UpdatedSelfCellDbCount;   /* No. Updated, Self-learned CellDB */
  uint16 w_UpdatedSelfSnDbCount;     /* No. Updated, Self-learned SnDB */
  uint16 w_UpdatedDownldCellDbCount; /* No. Updated, Downloaded CellDB */
  uint16 w_UpdatedDownldSnDbCount;   /* No. Updated, Downloaded SnDB */
  uint16 w_InUseSelfCellDbCount;     /* Currently in use, Self-learned CellDB */
  uint16 w_InUseSelfSnDbCount;       /* Currently in use, Self-learned SnDB */
  uint16 w_InUseDownldCellDbCount;   /* Currently in use, Downloaded CellDB */
  uint16 w_InUseDownldSnDbCount;     /* Currently in use, Downloaded SnDB */
  uint16 w_ArrCellsValidXta[C_TLE_RAT_MAX];
  uint16 w_ArrRegionsValidXta[C_TLE_RAT_MAX];
  uint16 w_ArrCellsFailedDload[C_TLE_RAT_MAX];
  uint16 w_CellsMissMatchedL3_WCDMA;
  uint16 w_CellsMissMatchedL3_LTE;
  uint16 w_CellsWithTdpData;
  uint16 w_NumofTdpScanlists;
  uint16 w_ArrCellsInTmpPartition[C_TLE_RAT_MAX];
  uint16 w_ArrRegionsWithCellsInTmpPartition[C_TLE_RAT_MAX];
  uint16 w_ArrCellsInTouch[C_TLE_RAT_MAX];
  uint16 w_ArrRegionsInTouch[C_TLE_RAT_MAX];
} xtt_PackedOpsPerfCounterType;

typedef PACKED struct PACKED_POST 
{
  xtt_PackedGpsTimeType         z_StartTime;
  xtt_PackedOpsPerfCounterType  z_Data;   /**/
  uint8                   u_MeasId; /* Identifier for the following measurement 
                                       or measurement list.
                                         0x00 - 0xC0	Individual measurements
                                         0xC1 - 0xFF	Measurement lists 

                                     Note: The only supported ID is C_TLM_OPS_MEASURMENT_ID */
} xtt_PackedOpsMeasurementType;

/*
   log.h says LOG_RECORD_DEFINE has been depricated.  log_hdr_type should be
   placed at the top of the structure and not accessed directly
*/
typedef PACKED struct PACKED_POST  
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  /* Flags field.  Not currently used. */
  uint8 u_Flags;

  /* The record being logged */
  xtt_PackedOpsMeasurementType z_OpsMeas;
} xtt_OpsMeasDMLogRecordType;

typedef PACKED struct PACKED_POST 
{
  int32  l_Latitude;   /* Latitude  [-90 , 90]  x (1-2^(-31)) degrees */
  int32  l_Longitude;  /* Longitude [-180 , 180]x (1-2^(-31)) degrees */

  uint16  w_Altitude;  /* Altitude of the best position estimate with the
                          lowest PosUnc for this cell. Height above the WGS84
                          reference ellipsoid, in units of 1 meter, in the 
                          range from  500 m to 15,883 m, where the binary 
                          value of the field conveys the height plus 500 m*/

  uint8  u_PosUncK;     /* PosUnc = 10 x ((1+0.1)^K - 1) [0, 1800Km] */
  uint8  u_AltUncK;     /* AltUnc = 45 x ((1+0.025)^K - 1) [0, 990m] */
} xtt_PackedLastKnownPosType;

typedef PACKED struct PACKED_POST 
{
  uint16 w_Mcc;          /* Layer 1 region ID */
  uint16 w_Sid;          /* Layer 2 region ID */
  uint16 w_Nid;          /* Layer 3 region ID */
  uint16 w_BsId;         /* Layer 4 region ID */
  uint16 w_Pn;           /* PN (9 bits) 0 to 511 */
  uint8  u_BandClass;    /* CDMA band class (5 bits) */ 
  uint16 w_Freq;          /* CDMA channel number in the band class */
  uint8  u_TimeZoneInfo;  /* Bit 0 (TZ_INCLUDED):
                                 0: No time zone information included
                                 1: Time zone information included
 
                             Bit 1 (DST_IND): Valid only if TZ_INCLUDED = 1
                                 0: Local time zone not on daylight savings time
                                 1: Local time zone on daylight savings time
 
                             Bits 2-6 (TIME_ZONE) : Valid only if TZ_INCLUDED = 1
                                  LTM_OFFSET value: 30 minute offset from UTC as a
                                                    twos complement value */
}xtt_PackedCellCdmaType;


typedef PACKED struct PACKED_POST 
{
  uint16 w_Mcc;     /* Layer 1 region ID */
  uint16 w_Mnc;     /* Layer 2 region ID */
  uint16 w_Lac;     /* Layer 3 region ID */
  uint16 w_Arfcn;   /* Absolute Radio Frequency Channel Number(0..1023)*/
  uint16 w_CellID;  /* Cell Identity - Integer (0..65535)*/
  uint8  u_BsIc;    /* Base Station Identity Code - Integer (0..63)*/
} xtt_PackedCellGsmType;


typedef PACKED struct PACKED_POST 
{
  uint16 w_Mcc;     /* Layer 1 region ID */
  uint16 w_Mnc;     /* Layer 2 region ID */
  uint16 w_Lac;     /* Layer 3 region ID: Location Area Code - Integer (1..65534) */
  uint32 q_UcId;    /* Layer 4 region ID: Cell Identity - Integer (0..268435455) */
  uint16 w_Freq;    /* UARFCN - Integer (0..16383)*/
  uint16 w_Psc;     /* Primary Synchronization Code - Integer (0..511)*/
} xtt_PackedCellWcdmaType;


typedef PACKED struct PACKED_POST 
{
  uint16 w_Mcc;     /* Layer 1 region ID */
  uint16 w_Mnc;     /* Layer 2 region ID */
  uint16 w_Tac;     /* Layer 3 region ID: Location Area Code - Integer (1..65534) */
  uint32 q_G_CI;    /* Layer 4 region ID: Cell Identity - Integer (0..268435455) */
  uint16 w_EARFCN;  /* Downlink center frequency - Integer (0..39649)*/
  uint16 w_PLID;    /* Physical cell ID - Integer (0..503)*/
} xtt_PackedCellLteType;


typedef PACKED struct PACKED_POST 
{
  xtt_PackedCellCdmaType  z_CellInfo;      
  int32             l_BsLatitude;  /* Base Station latitude  [-90 , 90]  x (1-2^(-31)) degrees */
  int32             l_BsLongitude; /* Base Station longitude [-180 , 180]x (1-2^(-31)) degrees */
} xtt_PackedCurrentCellCdmaType;  

typedef PACKED struct PACKED_POST 
{
  xtt_PackedCellGsmType z_CellInfo;
  uint16 w_RxLev;    /* Received Signal Level - Integer (0..63)*/
} xtt_PackedCurrentCellGsmType;


typedef PACKED struct PACKED_POST 
{
  xtt_PackedCellWcdmaType z_CellInfo; /* MRL can be added in this structure */ 
} xtt_PackedCurrentCellWcdmaType;

typedef PACKED struct PACKED_POST 
{
  xtt_PackedCellLteType z_CellInfo; /* MRL can be added in this structure */ 
} xtt_PackedCurrentCellLteType;

typedef PACKED union PACKED_POST
{
  xtt_PackedCurrentCellWcdmaType z_CellDataWcdma;
  xtt_PackedCurrentCellCdmaType  z_CellDataCdma;
  xtt_PackedCurrentCellGsmType   z_CellDataGsm;
  xtt_PackedCurrentCellLteType   z_CellDataLte;
} xtt_PackedCurrentCellType;

typedef PACKED struct PACKED_POST 
{
  xtt_PackedGpsTimeType  z_Gpstime; 
  uint16  w_Mcc;
  uint16  w_Mnc;
  uint8   u_AirInterfaceCapability;
  uint8   u_PositionCapability;
  uint8   u_CurAirInterfaceType;
  uint8   u_MobileDataAvailability; /* Bit 0 (LATEST_POSTITION available)
                                           0: Latest position is NOT valid
                                           1: Latest position is valid
                                       All other values reserved */
  xtt_PackedLastKnownPosType  z_Position;
  xtt_PackedCurrentCellType   z_CurCellData;
  uint8   u_DownloadPreference; /* Almanac download preference.
                                       Bit0  (BSA request)
                                          0  No BSA downloading.
                                          1  BSA downloading requested. (default)
                                       Bit1 (SNA request)
                                          0  No SNA downloading.
                                          1  SNA downloading requested. (default) */
  uint32  q_MaxDownloadBsa;
  uint32  q_MaxDownloadSna;
  uint16  w_NumofPartitions;
  
} xtt_PackedMobileStatusHeaderType;

typedef PACKED struct PACKED_POST
{ 
  uint16 w_Mcc;
  uint16 w_Mnc;
} tle_PackedWcdmaGsmLteHomeCarrierIdType;
 
typedef PACKED struct PACKED_POST
{ 
  uint8 u_CdmaRealmLength;
  char  c_CdmaRealm[255];
} tle_PackedCdmaHomeCarrierIdType;
 
typedef PACKED union PACKED_POST
{
  tle_PackedWcdmaGsmLteHomeCarrierIdType z_GsmWcdma;
  tle_PackedCdmaHomeCarrierIdType        z_Cdma;
} tle_PackedHomeCarrierIdUnionType;

typedef PACKED struct PACKED_POST
{
  uint8  u_LocationInfoMask;       /* Bit 0: GPS Time is present
                                      Bit 1: Position Reliability is present
                                      Bit 2: Altitude is present
                                      Bit 3: Vertical uncertainty is present
                                      Bit 4: Confidence is present
                                      Bit 5: Velocity is present
                                      Bit 6: Velocity uncertainty is present
                                      Bit 7: Velocity V2 is present */

  int32  l_Latitude;              /* In units of 180/2^32 degree
                                     (-90..+90*(1-2^-31)) */
  int32  l_Longitude;             /* In units of 360/2^32 degree
                                     (-180..+180*(1-2^-31)) */
  uint32 q_PosReliability;        /* 0 = Not set
                                     1 = Very low
                                     2 = Low
                                     3 = Medium
                                     4 = High
                                     5 = Max */
  uint32 q_TdpGpsMS;
  uint16 w_TdpGpsWeek;
  int16  x_Altitude;              /* In units of meters */
  uint16 w_TdpPosSource;          /* Position source:
                                     0 = None
                                     1 = MS Based GNSS
                                     2 = PE provided
                                     3 = AFLT
                                     4 = OTDOA
                                     5 = Other external source
                                     6 = OTA
                                     7 = WIFI */
  uint16 w_HorizontalSpeed;       /* In units of 0.1 m/sec (0..1030) */
  uint16 w_HorSpeedUncertainty;   /* In units of 0.05 m/sec (0..640) */
  uint16 w_Heading;               /* In units of 360/2^10 degrees from North (0..1024) */
  uint8  u_HorUncertainty;        /* K (0..127) */
  uint8  u_VertUncertainty;       /* K (0..127) */
  uint8  u_Confidence;            /* (0..100) */
  uint8  u_headingUncertainty;    /* In units of 180/2^6 degrees (0..64)Åä */
} tle_PackedTdpLocationInfoType;

typedef PACKED struct PACKED_POST
{
  xtt_PackedGpsTimeType  z_Gpstime; 
  uint16 w_ContentMask; /* Bit0: latestPositionPresent
                           Bit1: currentCellDataPresent 
                           Bit2: wifiDataPresent
                           Bit3: maxDlBsaSizePresent 
                           Bit4: maxDlSnaSizePresent
                           Bit5: maxDlWfaSizePresent
                           Bit6: localPartitionInfoPresent
                           Bit7: ocalFrequencyMaskInfoPresent
                           Bit8: uploadModeStatusPresent */

  uint8 u_HomeCarrierIdType; /* 0: Invalid
                                1: GSM/WCDMA/LTE
                                2: CDMA */
  tle_PackedHomeCarrierIdUnionType z_HomeCarrierId;
  uint16   w_AirInterfaceCapability; /* Bit map identifying supported air interface types:
                                        Bit 0: CDMA 1X
                                        Bit 1: GSM
                                        Bit 2: WCDMA
                                        Bit 3: WiFi 
                                        Bit 4: LTE 
                                        Bit 5: EVDO */
  uint16  w_PositionCapability;  /* Bit map identifying available positioning on the mobile device:
                                    Bit 0      GPS/GNSS
                                    Bit 1      CellID 
                                    Bit 2      AFLT
                                    Bit 3      OTDOA
                                    Bit 4      WiFi
                                    Bit 5      INS */

  tle_PackedTdpLocationInfoType     z_LatestPosition; 
   
  
  uint8   u_CurAirInterfaceType;  /* Air interface type of the current serving cell:
                                     0: Invalid
                                     1: CDMA 1X
                                     2: GSM
                                     3: WCDMA
                                     4 :LTE */

  xtt_PackedCurrentCellType   z_CurCellData;

  uint16   w_DownloadPreference; /* Bit map indicating Almanac and/or Location download requested:
                                    Bit 0 (BSA request)
                                        0:  No BSA downloading.
                                        1:  BSA downloading requested. 
                                    Bit 1 (SNA request)
                                        0:  No SNA downloading. 
                                        1:  SNA downloading requested.
                                    Bit 2 (Location request)
                                        0: No location download
                                        1:  location download enabled */
  uint32  q_MaxDownloadXtaSize;    /* Maximum number of bytes (maximum message size) that the mobile can 
                                      process in the server XTA response. 
                                      This applies only to the size of the WWAN download data.
                                      Suggested values are:
                                      70,000 bytes (typ)
                                      Min = 1,000 bytes, 
                                      Max for WWAN = 1,048,575 bytes */

  uint32  q_MaxDownloadBsa;         /* Maximum total number of BSA records (i.e., cells) downloadable 
                                       to mobile storage. 2,000 (typical), 8,000 (max) */

  uint32  q_MaxDownloadSna;         /* Maximum total number of SNA records (i.e. regions) downloadable 
                                       to mobile storage. 400 (typical), 2,000 (max) */

  uint8   u_UploadModeStatus;       /* 0 ñ Uploading CellDB using legacy binary
                                       1 ñ Uploading CellDB via AP using ASN.1 */

  uint16  w_NumofPartitions;
  
} tle_PackedAsn1MobileStatusHeaderType;

typedef PACKED union PACKED_POST
{
  xtt_PackedMobileStatusHeaderType     z_Binary;
  tle_PackedAsn1MobileStatusHeaderType z_Asn1;
} tle_PackedMobileStatusHeaderUnionType;
 
typedef PACKED struct PACKED_POST
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  /* Source field.  TLM / XTM. */
  uint8 u_Source;
  
  uint8 u_Type; /* 0: Binary
                   1: ASN.1 */
  /* The record being logged */
  tle_PackedMobileStatusHeaderUnionType z_MobileStatusHeader;
} xtt_MobileStatusHeaderDMLogRecordType;

typedef PACKED struct PACKED_POST 
{
  uint16  w_Mcc;
  uint16 w_Sid;          /* System ID */
  uint16 w_Nid;          /* Network ID */
  uint8  u_CdmaSeqNum;   /* CDMA sequence number to resolve BSID ambiguity */
} xtt_PackedPartitionBsaCdmaType;

typedef PACKED struct PACKED_POST 
{
  uint16  w_Mcc;          /* Mobile Country Code */  
  uint16  w_Mnc;          /* Mobile Network Code */
  uint16  w_Lac;          /* Location Area Code */
} xtt_PackedPartitionBsaGsmType;

typedef PACKED struct PACKED_POST 
{
  uint16 w_Mcc;          /* Mobile Country Code */  
  uint16 w_Mnc;          /* Mobile Network Code */
  uint16 w_Lac;          /* Location Area Code */
} xtt_PackedPartitionBsaWcdmaType;

typedef PACKED struct PACKED_POST 
{
  uint16 w_Mcc;          /* Mobile Country Code */  
  uint16 w_Mnc;          /* Mobile Network Code */
  uint16 w_Tac;          /* Tracking Area Code */
} xtt_PackedPartitionBsaLteType;

typedef PACKED union PACKED_POST 
{
  xtt_PackedPartitionBsaCdmaType  z_PartitionCdma;
  xtt_PackedPartitionBsaGsmType   z_PartitionGsm;
  xtt_PackedPartitionBsaWcdmaType z_PartitionWcdma;
  xtt_PackedPartitionBsaLteType   z_PartitionLte;
} xtt_PackedPartitionBsaType;

typedef PACKED struct PACKED_POST 
{
  xtt_PackedPartitionBsaType    z_BsaPartition;
  uint16                  w_CarrierId;
  uint8                   u_SubRegionId;       
} xtt_PackedPartitionIdBsaType;

typedef PACKED struct PACKED_POST 
{
  uint16  w_CarrierId;
  uint8   u_DivisionId; /* Index to divisions of carrier SNA data */
} xtt_PackedPartitionIdSnaType;

typedef PACKED union PACKED_POST
{
  xtt_PackedPartitionIdBsaType   z_PartitionBsa;
  xtt_PackedPartitionIdSnaType   z_PartitionSna;
} xtt_PackedPartitionIdType;

typedef PACKED struct PACKED_POST 
{
  uint8  u_PartitionType;         /* 0:SNA, 1:BSA */
  uint8  u_AirInterfaceType;      /* 0:CDMA, 1:GSM, 2:WCDMA, 3:LTE */
  xtt_PackedPartitionIdType   z_PartitionId;
  uint8                 u_PartitionRelVer;
} xtt_PackedPartitionType;

typedef PACKED struct PACKED_POST 
{
  uint8 u_Flag;            /* Bit0: 1 indicates last set*/   
  uint8 u_NumOfPartitions; /* Number of valid elements */
  xtt_PackedPartitionType  z_PartitionData[XTT_MOBILE_STATUS_MAX_PARTITION_NUM]; 


} xtt_PackedMobileStatusDataType;

typedef  PACKED struct PACKED_POST
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  /* Source field.  TLM / XTM. */
  uint8 u_Source;

  /* The record being logged */
  xtt_PackedMobileStatusDataType z_MobileStatusData;
} xtt_MobileStatusDataDMLogRecordType;

typedef PACKED struct PACKED_POST 
{
  uint8  u_DataUse; /* Bit 0: '0': non-participating network
                              '1': participating network
                     Bit 1 (XTA data available):
                        '0': XTA data is NOT available for download.
                        '1': XTA data is available for download.

                     Bits 2-4 (XTA management) Valid only if Bit 0 is set to 1
                          0: deletion
                          1: retention
                          2: conditional retention
                          3: replacement
                          4: insertion */

  uint8  u_ProximityOrder; /* Index based on the proximity of this XTA data 
                             partition to the serving cell or region.
                             The value of the index corresponds to the 
                             ordering of partitions in the partition list. The 
                             index starts from 0 with lower numbers indicating 
                             that the partition is closer to the serving cell 
                             or region. This field represents relevance of a 
                             partition to the serving cell or region. Value 
                             255 is reserved for partitions not included in 
                             the partition list*/


  uint8  u_AirInterfaceType;      /* 0:CDMA, 1:GSM, 2:WCDMA, 3:LTE */
  uint8  u_PartitionType;         /* 0:SNA, 1:BSA */
  uint8  u_GlobalCidStatus;   /* Defines the valid layers in each XTA record 
                                (only 2 LSB are used):
                                   0     INVALID
                                   1     Valid L1/L2/L3/L4 (BSA)
                                   2     L1/L2/L3
                                   3     L1/L2   */


  xtt_PackedPartitionIdType z_PartitionId;
  uint8  u_PartitionRelVer;
  uint16 w_DataTypeMask; /* Bit 0 (TERRAIN_HGT_INCLUDED)
                            Bit 1 (ANT_CENTER_INCLUDED)
                            Bit 2 (CDMA_SEQ_NUM_INCLUDED)
                            Bit 3 (CDMA_PILOT_PN_INCLUDED) */
  uint16  w_CreatWeek;      /* Partition creation week */
  uint32  q_CreatMsec;      /* Partition creation msec */
} xtt_PackedXtaDownldHeaderDataType;

typedef  PACKED struct PACKED_POST
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0, and 1 are supported */
  uint8 u_Version;

  /* Source field.  TLM / XTM. */
  uint8 u_Source;

  /* The record being logged */
  xtt_PackedXtaDownldHeaderDataType z_Data;
} xtt_DownloadHeaderDMLogRecordType;

typedef PACKED struct PACKED_POST
{
  uint8 u_UploadPreference; /* Bit 0 (Upload content)
                                   0  Upload only new information (default)
                                   1  Upload all
                               Bit 1-3  (Upload frequency) 
                                   0  Upload only in case of , max time elapsed, 
                                   1  Upload frequency unit - weeks
                                   2  Upload frequency unit days
                                   3  Upload frequency unit hours (for test purpose)
                                   4  Upload frequency unit - minutes (for test purpose)
                                   Values 5 - 7: reserved - treat as default.
                              Bit 4 (Upload control)
                                  0  Disable upload for subsequent transactions (default) 
                                  1  Enable upload for subsequent transactions
                                  Bit 5-7: reserved */

  uint8 u_UploadFreqUnits;   /* Number of units (weeks, days, hours, minutes) for Upload Frequency 
                                If Upload frequency is 'default', the Client will ignore this value */

  uint16 w_DownloadingFlag; /* Server response (flags) to mobile's XTA download request
                               Bit 0  (Serving CellID recognition)
                                   0  CellID is unrecognizable.
                                   1  CellID is recognized.
                               Bit 1 (Serving CellID membership) Valid only if Bit 0 is set to 1.
                                   0 CellID belongs to a non-participating network.
                                   1  CellID belongs to a participating network.
                               Bit 2 (Serving CellID corresponding XTA BSA partition availability) Valid only if Bit 0 and 1 are set to 1.
                                   0  CellID has no corresponding BSA partition.
                                   1  CellID has a corresponding BSA partition.
                               Bit 3 (Serving region recognition).
                                   0  Region ID is unrecognizable.
                                   1  Region ID is recognized.
                               Bit 4 (Serving region membership) Valid only if Bit 3 is set to 1.
                                   0  Region ID belongs to a non-participating network.
                                   1  Region ID belongs to a participating network.
                               Bit 5 (Serving region corresponding XTA-SNA availability) Valid only if Bit 3 is set to 1.*
                                   0  Serving region has no corresponding SNA.
                                   1  Serving region has a corresponding SNA.
                               Bit 6-15: reserved */
} xtt_PackedClientDownloadDataType;

typedef  PACKED struct PACKED_POST
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  /* The record being logged */
  xtt_PackedClientDownloadDataType z_Data;
} xtt_ClientDownloadDataDMLogRecordType;

typedef PACKED struct PACKED_POST
{
  uint8  u_ValidityFlag;  /* Bit 0 (Overall Response Validity)
                                 0: Invalid HTTP response
                                 1: Valid HTTP response
                             Bit 1 (StatusCode Validity)
                                 0: No StatusCode
                                 1: StatusCode valid
                             Bit 2 (ContentLength Validity)
                                 0: No ContentLength
                                 1: ContentLength valid
                           */
  uint16 w_StatusCode;     /* StatusCode of the HTTP response
                              Valid only if Bit 1 of u_ValidityFlag = 1
                            */
  uint32 q_ContentLength;  /* Value of the ContentLength HTTP header in the response
                              Valid only if Bit 2 of u_ValidityFlag = 1
                            */
} xtt_PackedHttpResponseStatusDataType;

typedef  PACKED struct PACKED_POST
{

  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  /* Source field.  TLM / XTM. */
  uint8 u_Source;

  /* The record being logged */
  xtt_PackedHttpResponseStatusDataType z_Data;

} xtt_HttpResponseStatusDMLogRecordType;

typedef PACKED struct PACKED_POST
{
  uint16 w_PresenceIndicator; /* Bit 0 (XT_REJECT presence)
                                     0: XT_REJECT not present
                                     1: XT_REJECT present
                                 Bit 1 (TIME_ASSISTANCE_DATA presence)
                                     0: TIME_ASSISTANCE_DATA not present
                                     1: TIME_ASSISTANCE_DATA present
                                 Bit 2 (CLIENT_DOWNLOAD_DATA presence)
                                     0: CLIENT_DOWNLOAD_DATA not present
                                     1: CLIENT_DOWNLOAD_DATA present
                                 Bit 3 (XTA_DOWNLOAD_DATA presence)
                                     0: XTA_DOWNLOAD_DATA not present
                                     1: XTA_DOWNLOAD_DATA present
                               */
  uint8  u_RejectMessageType; /* XT_MESSAGE_TYPE of the rejected request
                                 Valid only if Bit 0 of w_PresenceIndicator = 0
                               */
  uint16 w_RejectCauseCode;   /* cause code for why request was rejected
                                 Valid only if Bit 0 of w_PresenceIndicator = 0
                               */
} xtt_PackedXtaResponseStatusDataType;

typedef  PACKED struct PACKED_POST
{

  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  /* Source field.  TLM / XTM. */
  uint8 u_Source;

  /* The record being logged */
  xtt_PackedXtaResponseStatusDataType z_Data;

} xtt_XtaResponseStatusDMLogRecordType;

/*--------------------------------------------------------*/
/* Number of bytes to read from PDCOMM at a time          */
/*--------------------------------------------------------*/
#define XTT_PDCOMM_WRITE_SIZE (2500)   /* same as PDCOMM_WRITE_DATA_BUF_SIZE in pd_comms/pd_comms_tcp/src/pd_comms_tcp_data.h */
#define XTT_PDCOMM_READ_SIZE  (2000)   /* same as PDCOMM_READ_DATA_BUF_SIZE in pd_comms/pd_comms_tcp/src/pd_comms_tcp_data.h */

/*
   log.h says LOG_RECORD_DEFINE has been depricated.  log_hdr_type should be
   placed at the top of the structure and not accessed directly
*/
typedef PACKED struct PACKED_POST                        /* LOG_XTM_XTA_MSG_WRITE_C                             */
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type              z_Hdr;
  
  uint8                     u_PacketVersion;             /* Version number of the packet                        */
  xtt_PackedIPAddressType   s_IpAddr;                    /* IP address info                                     */
  
  uint64                    t_NumDataToWrite;            /* Total number of data remained to be written (bytes) */
  uint16                    w_NumDataWritten;            /* Number of data being written (bytes)                */
  uint8                     u_Data[XTT_PDCOMM_WRITE_SIZE];/* Data being written                                  */
} xtt_PackedXtaMsgWriteType;

/*
   log.h says LOG_RECORD_DEFINE has been depricated.  log_hdr_type should be
   placed at the top of the structure and not accessed directly
*/
typedef PACKED struct PACKED_POST                        /* LOG_XTM_XTA_MSG_READ_C                               */
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type              z_Hdr;
      
  uint8                     u_PacketVersion;             /* Version number of the packet                         */
  xtt_PackedIPAddressType   s_IpAddr;                    /* IP address info                                      */
  
  uint64                    t_NumDataToRead;             /* Number of Data remained to be read (bytes)           */
  uint16                    w_NumDataAvailableToRead;    /* Number of Data available for read (bytes)            */
  uint8                     u_Data[XTT_PDCOMM_READ_SIZE];/* Data available for read                              */
} xtt_PackedXtaMsgReadType;

typedef PACKED struct PACKED_POST
{
  uint16 w_Mcc;         /* Layer 1 ID: Mobile Country Code */
  uint16 w_Sid;         /* Layer 2 ID: System ID for CDMA. (0..32767) */
  uint16 w_Nid;         /* Layer 3 ID: Network ID for CDMA (0..65535) */
  uint16 w_BsId;        /* Layer 4 ID: Base Station ID for CDMA as provided in
                                       System Parameters Message (0..65535) */
} tlm_PackedCdmaIdType;

typedef PACKED struct PACKED_POST
{
  uint16 w_Mcc;     /* Layer 1 ID: Mobile Country Code */
  uint16 w_Mnc;     /* Layer 2 ID: Mobile Network Code */
  uint16 w_Lac;     /* Layer 3 ID: Location Area Code - Integer (1..65534) */
  uint16 w_CellId;  /* Layer 4 ID: Cell Identity - Integer (0..65535) */
} tlm_PackedGsmIdType;

typedef PACKED struct PACKED_POST
{
  uint16 w_Mcc;     /* Layer 1 ID: Mobile Country Code */
  uint16 w_Mnc;     /* Layer 2 ID: Mobile Network Code */
  uint16 w_Lac;     /* Layer 3 ID: Location Area Code - Integer (1..65534) */
  uint32 q_UcId;    /* Layer 4 ID: Cell Identity - Integer (0..268435455) */
} tlm_PackedWcdmaIdType;

typedef PACKED struct PACKED_POST
{
  uint16 w_Mcc;     /* Layer 1 ID: Mobile Country Code */
  uint16 w_Mnc;     /* Layer 2 ID: Mobile Network Code */
  uint16 w_Tac;     /* Layer 3 ID: Tracking Area Code. */
  uint32 q_CI;      /* Layer 4 ID: Cell Identity - Integer (0..268435455) */
} tlm_PackedLteIdType;

typedef PACKED union PACKED_POST
{
  tlm_PackedCdmaIdType  z_CdmaId;
  tlm_PackedGsmIdType   z_GsmId;
  tlm_PackedWcdmaIdType z_WcdmaId;
  tlm_PackedLteIdType   z_LteId;
} tlm_PackedCellularKeyUnionType;

typedef PACKED union PACKED_POST
{
  xtt_PackedGpsTimeType z_GpsTime;
  uint64                t_SlwClkMsec;
} tlm_PackedTimeStampUnionType;

typedef PACKED struct PACKED_POST
{
  uint8     u_TimeType;                   /* Should be populated with one of the
                                             values defined by
                                             db_TimeStampEnumType. */
  tlm_PackedTimeStampUnionType z_TimeStamp;
} tlm_PackedTimeStampType;

typedef PACKED struct PACKED_POST
{
  uint8                                     u_year;       /**< Year field. */
  uint8                                     u_month;      /**< Month field. */
  uint8                                     u_day;        /**< Day field. */
  uint8                                     u_hour;       /**< Hour field. */
  uint8                                     u_minute;     /**< Minute field. */
  uint8                                     u_second;     /**< Second field. */
  int32                                     l_TimeZone;  /** Time zone, as an offset from universal time
                                                            (i.e., the difference between local time and
                                                             universal time), in increments of 15 minutes.
                                                             The format for this type is specified in
                                                             3G TS 24.008 and 23.040. */
} tlm_PackedTimeAndTimezoneType;

typedef PACKED struct PACKED_POST
{
  /**< Whether the universal time is available. */
  uint8  u_UnivTimeAndTimeZoneAvail;
  /**< Universal Time Coordinated (UTC) time zone information. */
  tlm_PackedTimeAndTimezoneType             z_UnivTimeAndTimeZone;

  /**< Whether the timezone is available. */
  uint8                                   u_TimeZoneAvail;
  /**< Current time zone information. */
  int32                                   l_TimeZone;

  /**< Whether daylight saving information is available. */
  uint8                                   u_DaylightSavingAdjAvail;

  /**< Daylight saving adjustment. */
  int8                                    daylight_saving_adj;
} tlm_PackedTimeZoneInfoType;

typedef PACKED struct PACKED_POST
{
  uint8  u_SDataType;
  uint8  u_SysMode;
  uint16 w_Sid;
  /* The following types should match these types defined in services\cm\sys.h
   * sys_srch_win_type, sys_base_lat_type, sys_base_long_type, sys_base_id_type
   */
  uint8  u_SrchWinIndex; 
  /* neighbor set search window size table index               */
  /* 
     base station latitude [0.25sec and already sign extended].
     0 if not reported
  */
  int32  l_BaseLat;
  /* 
     base station longitude[0.25sec and already sign extended].
     0 if not reported
  */
  int32  l_BaseLong;
  uint16 w_BaseId;    /* base station ID */

  uint16 w_Nid;

  uint8  u_SubsId;    /* Subscription id (used for DS/DS operation) */
  uint64 t_ChangedFields;
  tlm_PackedTimeZoneInfoType z_TimeZoneInfo;
} tlm_PackedSSInfoType;

typedef PACKED struct PACKED_POST
{
  uint16 w_Pn;        /* Pilot PN as provided in System Parameters Message
                         (0-65535). */

  /* MBM_TODO: Can Freq and band class be consolidated? Freq only requires 11 bits
     and band class only requires 5.*/
  uint16 w_CdmaFreq;  /* CDMA channel number in the specified CDMA band
                         class. */
  uint8  u_BandClass; /* CDMA band class. */
} tlm_PackedVisitedCdmaCellType;

typedef PACKED struct PACKED_POST
{
  uint16 w_Arfcn;  /* Absolute Radio Frequency Channel Number  - Integer
                      (0..1023).*/
  uint8  u_Bsic;   /* Base Station Identity Code - Integer (0..63). */

  uint8 u_Band;    /* Stores the sys_band_T enum as a uint8. */
} tlm_PackedVisitedGsmCellType;

typedef PACKED struct PACKED_POST
{
  uint16 w_Freq;   /* UARFCN - Integer (0..16383). */
  uint16 w_Psc;    /* Primary Synchronization Code - Integer (0..511). For TD-SCDMA, Cell Parameters ID - Integer (0..127). */
} tlm_PackedVisitedWcdmaCellType;

typedef PACKED struct PACKED_POST
{
  uint16 w_PhysCellId;       /* Physical Cell ID - Integer (0..503). */
  uint16 w_TrackingAreaCode; /* Tracking Area Code - Integer (0..65535). */
  uint32 q_Freq;             /* DL Frequency - Integer (0..0XFFFFFFFF). */
} tlm_PackedVisitedLteCellType;

typedef PACKED union PACKED_POST
{
  tlm_PackedVisitedCdmaCellType  z_CdmaId;
  tlm_PackedVisitedGsmCellType   z_GsmId; 
  tlm_PackedVisitedWcdmaCellType z_WcdmaId;
  tlm_PackedVisitedLteCellType   z_LteId;
} tlm_PackedVisitedCellUnionType;

typedef PACKED struct PACKED_POST
{
  uint16 Arr_wCdmaFreq[C_TLE_MAX_NUM_OF_FREQ];
  uint8  u_NumOfValidFreq;
} tlm_PackedCdmaFreqListType;

typedef PACKED struct PACKED_POST
{
  uint8 u_StaleInfo;
   /* BS info is stale or not */
  uint8 u_BandClass;
    /* Band class of the base station */
  uint16 w_Channel;
    /* CDMA channel base station is using */
  uint16 w_BaseId;
    /* Base station identification */
  uint16 w_Sid;
    /* System Identification */
  uint16 w_Nid;
    /* Network Identification */
  uint16 w_PilotPn;
    /* Pilot PN */
  uint32 q_baseLat;
    /* Base Station Latitude */
  uint32 q_baseLong;
    /* Base Station Longitude */
} tlm_PackedBsInfoType;


typedef PACKED struct PACKED_POST
{
  tlm_PackedCdmaIdType z_Id;
  tlm_PackedCdmaFreqListType z_FreqList;

  tlm_PackedSSInfoType       z_SsInfo;
  tlm_PackedBsInfoType       z_BsInfo;  
  int8                       b_CdmaTz;
} tlm_PackedCdmaCellType;

typedef PACKED struct PACKED_POST
{
  uint16 Arr_wArfcn[C_TLE_MAX_NUM_OF_FREQ];
  uint8  u_NumOfValidFreq;
} tlm_PackedGsmFreqListType;

typedef PACKED struct PACKED_POST
{
  tlm_PackedGsmIdType z_Id;
  tlm_PackedGsmFreqListType z_FreqList;
  uint16 w_ArfcnNum;
  uint8  u_Bsic;   /* Base Station Identity Code - Integer (0..63). */
  uint8  u_Band;    /* Stores the sys_band_T enum as a uint8. */
  uint8  u_SubscrId;
} tlm_PackedGsmCellType;

typedef PACKED struct PACKED_POST
{
  uint32 Arr_qFreq[C_TLE_MAX_NUM_OF_FREQ];
  uint8  u_NumOfValidFreq;
} tlm_PackedLteFreqListType;

typedef PACKED struct PACKED_POST
{
  tlm_PackedLteIdType        z_Id;
  tlm_PackedLteFreqListType  z_FreqList;
  uint16 w_PhysCellId;       /* Physical Cell ID - Integer (0..503). */
  uint8  u_SubscrId;
  uint8  u_PreferCdma;
} tlm_PackedLteCellType;

typedef PACKED struct PACKED_POST
{
  uint16 Arr_wFreq[C_TLE_MAX_NUM_OF_FREQ];
  uint8  u_NumOfValidFreq;
} tlm_PackedWcdmaFreqListType;

typedef PACKED struct PACKED_POST
{
  tlm_PackedWcdmaIdType z_Id;
  tlm_PackedWcdmaFreqListType z_FreqList;
  uint16 w_Psc;    /* Primary Synchronization Code - Integer (0..511). */
  uint8 u_SubscrId;
} tlm_PackedWcdmaCellType;

typedef PACKED struct PACKED_POST
{
  tlm_PackedWcdmaCellType z_Cell[MAX_AS_IDS];
  uint8                   u_CellIdValid[MAX_AS_IDS];
  uint8                   u_TimerTrigerred;
} tlm_PackedWcdmaCellQueryInfoType;

typedef PACKED struct PACKED_POST 
{
  uint16 w_Mcc;     /* Layer 1 ID: Mobile Country Code */
  uint16 w_Mnc;     /* Layer 2 ID: Mobile Network Code */
  uint16 w_Lac;     /* Layer 3 ID: Location Area Code - Integer (1..65534) */
  uint32 q_UcId;    /* Layer 4 ID: Cell Identity - Integer (0..268435455) */
} tlm_PackedTdsIdType;  /* Also used for TD-SCDMA Cell ID */

typedef PACKED struct PACKED_POST
{
  uint16 Arr_wFreq[C_TLE_MAX_NUM_OF_FREQ];
  uint8  u_NumOfValidFreq;
} tlm_PackedTdsFreqListType;

typedef PACKED struct PACKED_POST
{
  tlm_PackedTdsIdType z_Id;
  tlm_PackedTdsFreqListType z_FreqList;
  uint8  u_cpid;     /* Cell parameters ID. */
  uint8 u_SubscrId;
} tlm_PackedTdsCellType;

typedef PACKED struct PACKED_POST  
{
  uint8 u_SubsId;
  tlm_PackedTimeZoneInfoType z_TimeZoneInfo;
} tlm_PackedTimeZoneMsgType;

typedef PACKED struct PACKED_POST
{
  int32  l_ClkTimeBiasNanoSec; /* Clock bias [Nanosecs]. */
  uint32 q_ClkTimeUncNanoSec;  /* 1 sided max time bias uncertainty [Nanosecs] */
  uint32 q_GpsMsec;     /* GPS msec at reference tick [msecs] */
  uint16 w_GpsWeek;     /* GPS week number at reference tick [weeks] */
  uint8  u_SourceOfTime; /* Source of the time */
} tlm_PackedTimeStructType;

typedef PACKED struct PACKED_POST 
{
   /* Frame Number 0..(26L*51L*2048L-1)  ~4.615ms frames */
   uint32     q_Fnum;
   /* QSymbol Count (sub fn) */
   uint32     q_SymbolCount;

   /* Time Slot Number 0..7 */
   uint8      u_TimeSlot;
} tlm_PackedGsmCellTimeType;

typedef PACKED struct PACKED_POST 
{
  /* Always Valid, when cell information is provided */
  tlm_PackedGsmCellType z_Cell;
  /* Always Valid, when cell information is provided */
  tlm_PackedGsmCellTimeType  z_CellTime;

} tlm_PackedGsmCellTimingType;

typedef PACKED struct PACKED_POST
{
  /* 10.3.6.75  System Frame Number  0..4095 10ms frames */
  uint16  w_FrameNum;

  /* 0..307200 Chipx8 per frame */
  uint32  q_PnPosChipx8;

} tlm_PackedWcdmaCellTimeType;

typedef PACKED struct PACKED_POST 
{
  /* Identifies cell by DL frequency and primary scrambling code. */
  tlm_PackedWcdmaCellType  z_Cell;

  /* WCDMA time snapshot of SFN and PN position. */
  tlm_PackedWcdmaCellTimeType   z_CellTime;
} tlm_PackedWcdmaCellTimingType;

typedef PACKED struct PACKED_POST 
{
  /* 64-bits, no rollover, chipx8 units, clocked @ 30.72MHz */
  uint64  t_LteRtc;

} tlm_PackedLteCellTimeType;

typedef PACKED struct PACKED_POST 
{
  /* Identifies cell by DL frequency and primary scrambling code. */
  tlm_PackedLteCellType   z_Cell;

  /* LTE time snapshot*/
  tlm_PackedLteCellTimeType   z_CellTime;
} tlm_PackedLteCellTimingType;

typedef PACKED struct PACKED_POST 
{
  /* TDSCDMA time snapshot of SubFN and PN position. */
  /* 64-bits value in TDS chipx8 units at status dump, clocked @ 10.24 MHz */
  uint64  t_TdsTime;

  /* 30-bits value in TDS sub chipx8 units at status dump */
  uint32  q_TdsPhase;
} tlm_PackedTDSCellTimingType;

typedef PACKED struct PACKED_POST 
{
  /* Identifies cell by DL frequency and cell parameters ID. */
  uint16 w_Freq;
  uint8  u_CellParamId;
} tlm_PackedTDScdmaCellInfoType;

typedef PACKED struct PACKED_POST 
{
  /* Identifies cell by DL frequency and cell parameters ID. */
  tlm_PackedTdsCellType      z_Cell;
  /* TDSCDMA time snapshot of SubFN and PN position. */
  tlm_PackedTDSCellTimingType   z_CellTime;
} tlm_PackedTDScdmaCellTimingType;

typedef PACKED union PACKED_POST
{
  tlm_PackedGsmCellTimingType   z_GsmCell;
  tlm_PackedWcdmaCellTimingType z_WcdmaCell;
  tlm_PackedLteCellTimingType   z_LteCell;
  tlm_PackedTDScdmaCellTimingType z_TDScdmaCell;    
} tlm_PackedTimeTagCellInfoType;

typedef PACKED struct PACKED_POST 
{
  tlm_PackedTimeStructType z_GpsTime; /* Week, Msec, Bias and Unc */
  tlm_PackedTimeTagCellInfoType z_TimeTagCell;

  /* Time-taick count at which the timetag database is valid */
  uint64 t_TimeTickCount;

  /* Timetick count at which the last timetag was requested */
  uint64 t_LastReqestTimeTickCount;

  /* 32-bit extended FCount value */
  uint32 q_FCount32;

  /* GPSRTC milliseconds from the TimeTag message. */
  uint32 q_GpsRtcMs;
  /* GPSRTC SubMs from the TimeTag message */
  uint32 q_SubMsCx80;

  /* USTMR count. Resolution of 19.2 MHz */
  uint32 q_UstmrCount;
  uint32 q_LpmCount;

  /* TRUE if this data base is valid */
  boolean u_Valid; 

  /* See enum above */
  uint8 u_TimeTagState;
  
  /* Time Inject request. If TRUE, inject time on completion of timetag */
  boolean u_TimeInjectReq;
  
  /* GPS time */
  boolean u_GpsTimeValid; /* Validity flag for GPS time */
  /* TRUE if the 32-bit FCount is valid */
  boolean u_FCount32Valid;

  uint8 u_ActiveRAT; 

  /* Cell Information */
  boolean u_CellInfoValid; /* Validity flag for Cell Info */
  boolean b_CellIdValid;
} tlm_PackedTimeTagStructType; 

typedef PACKED struct PACKED_POST 
{
  boolean b_configVaild; /* TRUE: Content is valid */

  uint16 w_TdpConfigMask; /* Bit0: w_TdpUploadConfigPresent,
                             Bit1: w_UploadScreeningManagementPresent,
                             Bit2: w_WwanUploadModePresent,
                             Bit3: w_FreqUnitsPresent,
                             Bit4: w_TdpMaxNumScanListPerCellPresent,
                             Bit5: w_TdpGridSizeDegPresent,
                             Bit6: w_TdpMinScanIntervalUploadSecPresent,
                             Bit7: w_TdpMaxPosUncUploadPresent,
                             Bit8: w_TdpScanSelectionParameterPresent,
                             Bit9: w_ScanSelectionMethodPresent */

  uint32 q_UploadFreqScale; /*  noTimer = 0,
                                weeks = 1,
                                days = 2,
                                hours = 3,
                                minutes = 4,
                                seconds = 5*/
  uint8  u_UploadFreqUnits;

  uint32 q_WwanDownloadFlag; /* { cell_id_recognized(0), 
                                  participating_network_cell(1), 
                                  bsa_available(2) ,
                                  region_recognized(3) , 
                                  participating_network_region(4) , 
                                  sna_available(5),
                                  level3RegionRecognized(6),
                                  mobileL3CoverageComplete(7),
                                }
                                The wwanDownloadFlag will be set to all ë0í if the client did 
                                not request BSA or SNA download in the */

  uint16 w_UploadMask; /*  Bit0: WwanUploadContent (0 ñ Upload only new information,
                                                    1 ñ Upload all) 

                           Bit1: WwanUploadControl (0 - Disable upload for subsequent transactions
                                                    1 - Enable upload for subsequent transactions)

                           Bit2: UploadScreening   (0 - Upload all data not explicitly prohibited
                                                    1 - Upload only data explicitly allowed)

                           Bit3: WwanUploadMode    (0 - Upload CellDB using legacy binary 
                                                    1 - Upload CellDB via AP using ASN.1 )

                           Bit4: TdpUploadControl  (0 - Disable TDP upload for subsequent transactions
                                                    1 - Enable TDP upload for subsequent transactions)

                                                    The following items are valid only if TDP uploads 
                                                    are enabled */

  uint32  q_TdpMinScanIntervalUploadSec; /* Recommended minimum time between scanlists for upload 
                                            in seconds*/
  uint32  q_TdpScanSelectionMethod;      /* {randomSelection(0), Ö }*/

  uint16  w_TdpMaxNumScanListPerCell;    /* Maximum number scanlists (instances of lteTdpUploadData) 
                                             that may be uploaded per serving cell record*/

  uint16  w_TdpGridSizeDeg;              /* Recommended spacing between scanlist collection points 
                                             in the unit of 0.00001 degree of latitude and longitude.*/

  uint16  w_TdpMaxPosUncUpload;          /* Maximum position uncertainty for scanlists for 
                                            uploads in meters*/

  uint16  w_TdpScanSelectionParameter;   /* A parameter for a cost function calculation, used 
                                            for selection among collected scans within a grid segment. */
} tle_PackedTdpClientDownloadData;


typedef PACKED struct PACKED_POST 
{
  uint32 q_ModemPseudoClientID; /* The existing PseudoClientID in binary GTP*/
  uint32 q_ClientType; /*  Client type as reported in binary protocol,
                           wwanClient(0), wlanClient(1), combinedClient(2), */

  uint32 q_GpsSecondsLastDownload; /* GPS second of last binary download */
  uint16 w_GpsWeekLastDownload;    /* GPS week of last binary download */
  uint16 w_ModemChipsetVersion;    /* Modem chipset version as reported in binary MP */
  uint16 w_ModemMajorSwVersion;     /* Modem major software version as reported in binary */
  uint16 w_ModemMinorSwVersion;     /* Modem minor software version as reported in binary */
  uint8 u_ModemAirIfCap;           /* Air interface capability as reported in binary */
  uint8 u_ModemPosCap;             /* Position capability as reported in binary */


  uint8  u_UploadFlag; /*  Bit0: EULA-UserSession ( 0 - User session Disabled
                                                    1 - User session Enaabled )

                           Bit1: GlobalUpload     ( 0 - Global upload not allowed
                                                    1 - Global upload enabled )   */
  uint8   u_FeatureControl;          /* OEM control */
  /* TLE_API_E_GTP_DISABLED = 0, TLE_API_E_GTP_BASIC, TLE_API_E_GTP_POWER_DISABLED,
     TLE_API_E_GTP_PREMIUM, TLE_API_E_GTP_PREMIUM_TDP, TLE_API_E_GTP_MAX */

  char  sz_ModemOemId[C_TLE_PROT_MAX_OEM_ID_LENGTH]; /* OEM ID as stored in XTM and reported 
                                                        in binary mobile status data*/
  char   sz_ModemModelId[C_TLE_PROT_MAX_MODEL_ID_LENGTH]; /* Modem model ID as reported in binary */
  char   sz_ModemSwVersion[C_TLE_PROT_MAX_ASN1_SW_VERSION_LENGTH]; /* Modem software version */
  uint8  u_Asn1ProtMajorRel;
  uint8  u_Asn1ProtMinorRel;
  uint8  u_Asn1ProtPointRel;
  uint16 w_HomeMcc;
  uint16 w_HomeMnc;
  uint8 u_CdmaRealmLen;
  char sz_CdmaRealm[C_TLE_PROT_MAX_REALM_LENGTH];
  tle_PackedTdpClientDownloadData z_TdpClientData;
} tle_PackedClientInformationType;

typedef PACKED struct PACKED_POST 
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  /* Source field.  TLM / XTM. */
  uint8 u_Source;

  /* The record being logged */
  tle_PackedClientInformationType z_Data;

} tle_ClientInformationDMLogRecordType;

typedef PACKED struct PACKED_POST
{
  uint8 u_ServingCellConfigMask;  /* Bit 0: Sib8 is valid
                                     Bit 1: Sib16 is valid */

  uint8 u_CellSystemBw; /* 0 = 1.4Mhz bandwidth
                           1 = 3Mhz bandwidth
                           2 = 5Mhz bandwidth
                           3 = 10Mhz bandwidth
                           4 = 15Mhz bandwidth
                           5 = 20Mhz bandwidth
                           6 = Unknown bandwidth */

  uint8 u_CyclicPrefix; /* 0 = Normal
                           1 = Extended */

  uint8 u_AntennaPorts; /* 0 = 1 or 2 transmit antennas
                           1 = 4 transmit antennas */

  uint64  t_Sib8CdmaSystemTime; /* The CDMA2000 system time corresponding to 
                                   the SFN boundary at or after the ending 
                                   boundary of the SI-Window in which this Time 
                                   Transfer was taken. The size is 52 bits and 
                                   the unit is [CDMA2000 chips] */

  uint64  t_Sib16FullGpsMsecs; /* The Full gps time at the SFN boundary in
                                   units of msecs */

} tdp_PackedServingCellConfigType;

typedef PACKED struct PACKED_POST
{
  uint8 u_CellIdMask;   /* Bit 0: MCC is valid
                           Bit 1: MNC is valid
                           Bit 2: TAC is valid
                           Bit 3: Cell ID is valid */
  uint16 w_PhyId;       /* Physical Id 0-503 */
  uint32 q_Earfcn;      /* Downlink center frequency - Integer (0..39649) */
  uint16 w_Mcc;         /* Layer 1 region ID */
  uint16 w_Mnc;         /* Layer 2 region ID */
  uint16 w_Tac;         /* Layer 3 region ID */
  uint32 q_CellId;      /* Layer 4 region ID */
} tdp_PackedLteCellIdStructType;

typedef PACKED struct PACKED_POST
{
  uint8 u_MeasMask;   /* Bit 0: Signal Power is valid
                         Bit 1: Signal Quality is valid
                         Bit 2: Rx-Tx Time difference is valid
                         Bit 3: Time of transmission is valid
                         Bit 4: This is a serving cell */


   uint8 u_SignalPower; /* Range (-140 dBm to -44 dBm) with 1 dBm 
                           resolution. Refer to GTP the ICD. */

   uint8 u_SignalQuality; /* Reference signal received quality.
                           For PRS measurements, the signalQuality will 
                           be RSRQ as defined in LPP. Range (-19.5 dB 
                           to -3dB with 0.5 dB resolution. The same 
                           values and encoding will be used for CRS 
                           measurements. Refer to GTP the ICD.*/

   uint16 w_RxTxTimeDiff; /* Rx-Tx time difference, as defined in 
                            LPP. Range (2 TS to 20472 TS). The same 
                            values and encoding will be used for PRS 
                            and CRS measurements. Refer to GTP the ICD.*/

   uint8 b_RangingMeasType; /* Ranging Measurement Type:
                               0 = PRS
                               1 = OPCRS
                               2 = UKCRS */

   uint8 b_RangingMeasBW; /* LTE DL bandwidth of the measurement:
                             0 = 1.4Mhz bandwidth
                             1 = 3Mhz bandwidth
                             2 = 5Mhz bandwidth
                             3 = 10Mhz bandwidth
                             4 = 15Mhz bandwidth
                             5 = 20Mhz bandwidth
                             6 = Unknown bandwidth */

   uint8 u_FineClockSyncFlag; /* 0 = FALSE
                                 1 = TRUE */
 
   uint16 w_TotMs; /* Offset of the observed signalís time of 
                   transmission from start of LTE frame structure in 
                   units of ms. Value is calculated as: (System Frame 
                   Number) * 10 +  sub_frame_offset_within_frame.
                   Required to be present for the serving cell.                
                   Optionally present for neighboring cells. When
                   present for neighboring cells, the value shall apply 
                   to the neighborís */

   int16 x_rToaMs; /* TOA measurements are relative to the GPS time in
                    the position estimate for this lteTdpUploadData
                    instance. Ms component of rToa Range (~ -32.7 sec
                    to 32.7 sec ) */   

   int32 l_rToaNs; /* TOA measurements are relative to the GPS time in 
                    the position estimate for this lteTdpUploadData                
                    instance. Ns component of rToa Range (-0.5 ms to 
                    0.5 ms) */

   uint8 u_rToaMeasUncUnPropK; /* K-value encoded measurement uncertainty, 
                              computed based on signal strength, multipath, 
                              etc.*/

   uint8 u_rToaClockUncPropK; /* K-value encoded clock Uncertainty. If the 
                             fineClockSyncFlag is set to 1, then this  
                             field represents the clock uncertainty wrt 
                             the GPS time of fix. Else, this represents 
                             the relative clock uncertainty growth from 
                             the first measurement of the block. */

} tdp_PackedUploadMeasStructType;

typedef PACKED struct PACKE_POST
{
  tdp_PackedLteCellIdStructType z_CellId;

  tdp_PackedUploadMeasStructType z_Meas;

} tdp_PackedMeasAndCellDataStructType;

typedef PACKED struct PACKED_POST 
{
  /* log header.  DO NOT ACCESS DIRECTLY! */
  log_hdr_type z_Hdr;

  /* Version of this packet.  Currently only 0 is supported */
  uint8 u_Version;

  uint8 u_Reason; /* 0 ñ Received and saved
                     1 ñ Received and dropped
                     2 ñ Uploaded
                     3 ñ Deleted */

  uint8 u_RejectReason; /* 0 ñ Not rejected
                           1 ñ Location fix is not sourced as GNSS
                           2 ñ Location fix is not fresh
                           3 ñ PUNC is too large
                           4 ñ Time elapsed is too short
                           5 ñ Database full
                           255 ñ Other/Unknown */

  tle_PackedTdpLocationInfoType z_LocationInfo;

  tdp_PackedServingCellConfigType z_ServingCellConfig;

  uint8 u_NumOfTdpMeas;

  tdp_PackedMeasAndCellDataStructType z_MeasAndCellData[C_TLE_LOG_TDP_NUM_MAX_TDP_MEAS];

} tle_TdpScanlistDMLogRecordType;

#ifdef FEATURE_CGPS_PLAYBACK
#pragma pack(pop)
#endif

typedef enum
{
  C_TLE_LOG_TDP_LOG_REASON_START = 0, 

  C_TLE_LOG_TDP_LOG_REASON_RECEIVED_AND_SAVED = C_TLE_LOG_TDP_LOG_REASON_START,
  C_TLE_LOG_TDP_LOG_REASON_RECEIVED_AND_REJECTED,
  C_TLE_LOG_TDP_LOG_REASON_UPLOADED,
  C_TLE_LOG_TDP_LOG_REASON_DELETED,

  C_TLE_LOG_TDP_LOG_REASON_END
} tle_TdpLogReasonType;

typedef enum
{
  C_TLE_LOG_TDP_REJECT_REASON_START = 0,

  C_TLE_LOG_TDP_REJECT_REASON_NOT_REJECTED = C_TLE_LOG_TDP_REJECT_REASON_START,
  C_TLE_LOG_TDP_REJECT_REASON_LOCATION_NOT_GNSS,
  C_TLE_LOG_TDP_REJECT_REASON_LOCATION_NOT_FRESH,
  C_TLE_LOG_TDP_REJECT_REASON_PUNC_TOO_LARGE,
  C_TLE_LOG_TDP_REJECT_REASON_TIME_ELAPSED_TOO_SHORT,
  C_TLE_LOG_TDP_REJECT_REASON_DATABASE_FULL,
  C_TLE_LOG_TDP_REJECT_REASON_TDP_DISABLED,
  C_TLE_LOG_TDP_REJECT_REASON_POS_REL_TOO_LOW,
  C_TLE_LOG_TDP_REJECT_REASON_NEW_COST_NOT_BETTER,
  C_TLE_LOG_TDP_REJECT_REASON_UPLOAD_IN_PROGRESS,
  C_TLE_LOG_TDP_REJECT_REASON_DATABASE_LOOKUP_FAILED,
  C_TLE_LOG_TDP_REJECT_REASON_DATABASE_DELETE_FAILED,
  C_TLE_LOG_TDP_REJECT_REASON_DATABASE_INSERT_FAILED,
  C_TLE_LOG_TDP_REJECT_REASON_MALFORMED_SCANLIST,
  C_TLE_LOG_TDP_REJECT_REASON_IDENTIFY_SERV_CELL_FAILED,
  C_TLE_LOG_TDP_REJECT_REASON_OTHER = 255,

  C_TLE_LOG_TDP_REJECT_REASON_END,
} tle_TdpLogRejectReasonType;

/* This struct is for gathering all the neccessary info before logging. */
typedef struct
{
  tle_TdpLogReasonType e_Reason;

  tle_TdpLogRejectReasonType e_RejectReason;

  boolean b_ServingCellConfigPresent;

  uint8 u_NumOfTdpMeas;

  const tle_TdpLocationInfoType * pz_LocationInfo;

  const tdp_ServingCellConfigType * pz_ServingCellConfig;

  const tdp_MeasAndCellDataStructType * pz_MeasAndCellData[C_TLE_LOG_TDP_NUM_MAX_TDP_MEAS];

} tle_TdpScanlistLogStructType;

/*==============================================================================

                      FUNCTION DECLARATIONS FOR MODULE

==============================================================================*/

/*===========================================================================

FUNCTION
  tle_LogGtpConfig

DESCRIPTION
  Logs GTP configuration data 

PARAMETERS
  cpz_GtpConfig - Points to the configuration to be logged.
  (input)
 
  u_Source  - The task creating this log (TLE_TLM_MODULE, or TLE_XTM_MODULE)

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - If the log is generated successfuly
  FALSE - Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogGtpConfig( const tle_ClientInformationType * cpz_GtpConfig,
                          uint8 u_Source);

/*===========================================================================

FUNCTION
  tle_LogOpsMeas

DESCRIPTION
  Logs the information for the given ops measurements.

PARAMETERS
  pz_OpsMeas         - The ops measurements to log.

  cu_Flags  - Flags to output in the log message.

  cpz_OpsStats - Points to the Ops Measurement statistics
  (input)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogOpsMeas( const xtt_OpsMeasurementType * const pz_OpsMeas,
                        const uint8 cu_Flags, const tle_OpsStatType *cpz_OpsStats);

/*===========================================================================

FUNCTION
  tle_PackXttGpsTime

DESCRIPTION
  This function packs an xtt_GpsTimeType struct.

PARAMETERS
  cpz_XttTime    - The gps time struct to pack.
  (input)

  pz_PackedTime  - The gps time struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackXttGpsTime(
  const xtt_GpsTimeType * const cpz_XttTime,
  xtt_PackedGpsTimeType * const pz_PackedTime);

/*===========================================================================

FUNCTION
  tle_PackTimeTagCommon

DESCRIPTION
  This function packs an xtt_TimeTagCommonType struct.

PARAMETERS
  cpz_XttTimeTagCommon    - The struct to pack.
  (input)

  pz_PackedTimeTagCommon  - The struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackTimeTagCommon(
  const xtt_TimeTagCommonType * const cpz_XttTimeTagCommon,
  xtt_PackedTimeTagCommonType * const pz_PackedTimeTagCommon);

/*===========================================================================

FUNCTION
  tle_PackXttTimeTagType

DESCRIPTION
  This function packs an xtt_TimeTagType struct.

PARAMETERS
  cpz_XttTimeTag    - The struct to pack.
  (input)
 
  ce_Rat            - Must be CDMA, WCDMA, or GSM.
  (input)

  pz_PackedTimeTag  - The struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackXttTimeTagType(
  const xtt_TimeTagType * const cpz_XttTimeTag,
  const cgps_ActiveRATEnumType ce_Rat,
  xtt_PackedTimeTagType * const pz_PackedTimeTag);


/*===========================================================================

FUNCTION
  tle_PackCurrentCellTypeType

DESCRIPTION
  This function packs an xtt_CurrentCellType struct.

PARAMETERS
  cpz_Cell    - The struct to pack.
  (input)
 
  ce_Rat            - Must be CDMA, WCDMA, or GSM.
  (input)
 
  pz_PackedCell  - The struct in packed format.
  (output)
 
  pw_Mcc (output) - If successful, points to the MCC of the cell
  pw_Mnc (output) - If successful, points to the MNC of the cell in UMTS systems,
                   0 for CDMA networks
DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackCurrentCellTypeType(
  const xtt_CurrentCellType * const cpz_Cell,
  const cgps_ActiveRATEnumType ce_Rat,
  xtt_PackedCurrentCellType * const pz_PackedCell, uint16 * pw_Mcc,
  uint16 * pw_Mnc);


/*===========================================================================

FUNCTION
  tle_PackMobileStatusHeaderType

DESCRIPTION
  This function packs an xtt_MobileStatusDataHeaderType struct.

PARAMETERS
  cpz_Header    - The struct to pack.
  (input)
 
  pz_PackedHeader  - The struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackMobileStatusHeaderType(
  const xtt_MobileStatusDataHeaderType * const cpz_Header,
  xtt_PackedMobileStatusHeaderType * const pz_PackedHeader);



/*===========================================================================

FUNCTION
  tle_PackMobileStatusDataType

DESCRIPTION
  This function packs an xtt_MobileStatusDataXtaType struct.

PARAMETERS
  cpz_Data    - The struct to pack.
  (input)
 
  pz_PackedData  - The struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackMobileStatusDataType(
  const xtt_MobileStatusDataXtaType * const cpz_Data,
  xtt_PackedMobileStatusDataType * const pz_PackedData);

/*===========================================================================

FUNCTION
  tle_PackPartitionType

DESCRIPTION
  This function packs an xtt_PartitionType struct.

PARAMETERS
  cpz_Partition       - The struct to pack.
  (input)
 
  pz_PackedPartition  - The struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackPartitionType(
  const xtt_PartitionType * const cpz_Partition,
  xtt_PackedPartitionType * const pz_PackedPartition);

/*===========================================================================

FUNCTION
  tle_PackPartitionIdBsaType

DESCRIPTION
  This function packs an xtt_PartitionIdBsaType struct.

PARAMETERS
  cpz_BsaPartition       - The struct to pack.
  (input)
 
  ce_Rat            - Must be CDMA, WCDMA, or GSM.
  (input)
 
  pz_PackedBsaPartition  - The struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackPartitionIdBsaType(
  const xtt_PartitionIdBsaType * const cpz_BsaPartition,
  const cgps_ActiveRATEnumType ce_Rat,
  xtt_PackedPartitionIdBsaType * const pz_PackedBsaPartition);

/*===========================================================================

FUNCTION
  tle_PackDownloadedHeaderType

DESCRIPTION
  This function packs an xtt_XtaDownldHeaderDataType struct.

PARAMETERS
  cpz_Header       - The struct to pack.
  (input)
 
  e_PartitionType - Partition type ( C_TLE_PARTITION_SNA, C_TLE_PARTITION_BSA )
  (input)
 
  pz_PackedHeader  - The struct in packed format.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackDownloadedHeaderType(
  const xtt_XtaDownldHeaderDataType * const cpz_Header,
  e_TlePartitionType e_PartitionType,
  xtt_PackedXtaDownldHeaderDataType * const pz_PackedHeader);


/*=============================================================================

  FUNCTION      tle_DebugPrint()

  DESCRIPTION 	This function outputs a generic debug message in real time

  DEPENDENCIES  None

  PARAMETERS
    buffer - message to be printed (output)

  RETURN VALUE  

  SIDE EFFECTS  

=============================================================================*/
void tle_DebugPrint(const char *buffer);

/*=============================================================================

  FUNCTION      tle_DebugPrintHigh()

  DESCRIPTION 	This function outputs a generic debug message in real time

  DEPENDENCIES  None

  PARAMETERS
    buffer - message to be printed (output)

  RETURN VALUE  

  SIDE EFFECTS  

=============================================================================*/
void tle_DebugPrintHigh(const char *buffer);

                      
/*===========================================================================

FUNCTION
  tle_LogMobleStatusHeader

DESCRIPTION
  Logs the mobile status header.

PARAMETERS
  cpz_Header - The ops measurements to log.

  cu_Source  - The module requesting the LOG. (TLE_XTM_MODULE, or TLE_TLM_MODULE)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogMobleStatusHeader(const xtt_MobileStatusDataHeaderType * const cpz_Header,
                                 const uint8 cu_Source);

/*===========================================================================

FUNCTION
  tle_LogMobileStatusData

DESCRIPTION
  Logs the mobile status data.

PARAMETERS
  cpz_Data   - The mobile status data to log.

  cu_Source  - The module requesting the LOG. (TLE_XTM_MODULE, or TLE_TLM_MODULE)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogMobileStatusData(const xtt_MobileStatusDataXtaType * const cpz_Data,
                                const uint8 cu_Source);

/*===========================================================================

FUNCTION
  tle_LogPartitionInfo

DESCRIPTION
  Logs the partition information during an download

PARAMETERS
  cpz_Header - The mobile status data to log.
 
  e_PartitionType - Partition type ( C_TLE_PARTITION_SNA, C_TLE_PARTITION_BSA )

  cu_Source  - The module requesting the LOG. (TLE_XTM_MODULE, or TLE_TLM_MODULE)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogPartitionInfo(const xtt_XtaDownldHeaderDataType * const cpz_Data,
                             e_TlePartitionType e_PartitionType,
                             const uint8 cu_Source);

/*===========================================================================

FUNCTION
  tle_PackClientDownloadData

DESCRIPTION
  This function packs an xtt_ClientDownloadDataType struct.

PARAMETERS
  cpz_ClientData       - The struct to pack.
  (input)

  pz_PackedClientData  - The packed struct.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackClientDownloadData(
  const xtt_ClientDownloadDataType * const cpz_ClientData,
  xtt_PackedClientDownloadDataType * const pz_PackedClientData);

/*===========================================================================

FUNCTION
  tle_LogClientDownloadData

DESCRIPTION
  Logs the client download data

PARAMETERS
  cpz_Data - The client download data to log.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogClientDownloadData(const xtt_ClientDownloadDataType * const cpz_Data);

/*===========================================================================

FUNCTION
  tle_PackHttpResponseStatus

DESCRIPTION
  This function packs an xtt_HttpResponseStatusDataType struct.

PARAMETERS
  cpz_ClientData       - Pointer to the non-packed HTTP response status struct.
  (input)

  pz_PackedClientData  - Pointer to the packed HTTP response status struct.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackHttpResponseStatus(
  const xtt_HttpResponseStatusDataType * const cpz_HttpRespStatus,
  xtt_PackedHttpResponseStatusDataType * const pz_PackedHttpRespStatus);

/*===========================================================================

FUNCTION
  tle_LogHttpResponseStatus

DESCRIPTION
  Logs the HTTP response status.

PARAMETERS
  cpz_Data - Pointer to the HTTP response status structure to log.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogHttpResponseStatus(const xtt_HttpResponseStatusDataType * const cpz_Data);

/*===========================================================================

FUNCTION
  tle_PackXtaResponseStatus

DESCRIPTION
  This function packs an xtt_XtaResponseStatusDataType struct.

PARAMETERS
  cpz_ClientData       - Pointer to the non-packed XTA response.
  (input)

  pz_PackedClientData  - Pointer to the packed XTA response.
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfully packed. 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_PackXtaResponseStatus(
  const xtt_XtaResponseStatusDataType * const cpz_XtaRespStatus,
  xtt_PackedXtaResponseStatusDataType * const pz_PackedXtaRespStatus);

/*===========================================================================

FUNCTION
  tle_LogXtaResponseStatus

DESCRIPTION
  Logs the XTA response status.

PARAMETERS
  cpz_Data - Pointer to the XTA response status structure to log.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogXtaResponseStatus(const xtt_XtaResponseStatusDataType * const cpz_Data);

/*===========================================================================

FUNCTION
  tle_LogXTAWrite

DESCRIPTION
  Logs the XTA Request Message during PDCOMM Write.

PARAMETERS
  cpz_Header - The XTA Request Message to log.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogXTAWrite(const xtt_XtaMsgWriteType * const cpz_Data);


/*===========================================================================

FUNCTION
  tle_LogXTARead

DESCRIPTION
  Logs the XTA Response Message during PDCOMM Read.

PARAMETERS
  cpz_Header - The XTA Response Message to log.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogXTARead(const xtt_XtaMsgReadType * const cpz_Data);                   

/*===========================================================================

FUNCTION
  tle_LogTdpScanlist

DESCRIPTION
  Logs the TDP scanlist

PARAMETERS 
  cpz_TdpScanListLog - The TDP scanlist log
  (input)

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - The Tdp scanlist is logged successfully
  FALSE - Otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogTdpScanlist(
   const tle_TdpScanlistLogStructType * const cpz_TdpScanListLog);

/*===========================================================================

FUNCTION
  tle_LogAsn1MobleStatusHeader

DESCRIPTION
  This function logs an ASN.1 mobile status header

PARAMETERS
  cpz_Asn1Header  - Points to the mobile staus header in ASN.1 format
  (input)

  cu_Source       - The module logging the mobile status header (TLM/XTM)
  (output)

DEPENDENCIES
  None

RETURN VALUE
  TRUE:         - Successfull 
  FALSE:        - Otherwise. 

SIDE EFFECTS
  None

===========================================================================*/
boolean tle_LogAsn1MobleStatusHeader(const tle_Asn1MobileStatusDataType *cpz_Asn1Header,
                                     const uint8 cu_Source);

#endif /* TLE_LOG_H */
