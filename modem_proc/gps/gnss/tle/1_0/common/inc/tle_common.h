#ifndef TLE_COMMON_H
#define TLE_COMMON_H
/*==============================================================================

                  Terrestrial Location Engine Common

GENERAL DESCRIPTION
   This file contains constants, structures and common utility functions prototypes
   necessary for both XTM and TLM.


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

  Copyright (c) 2010 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2014 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
  Copyright (c) 2015 - 2017 Qualcomm Technologies, Inc. 
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


when          who     what, where, why
--------      ---     ----------------------------------------------------------

==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/

#include "gps_common.h"
#include "tdp_common.h"
#include "tle_api.h"

/*==============================================================================

                    DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/

/*==============================================================================

                                  CONSTANTS

==============================================================================*/
/* TDP related contants */
#define C_TLE_ASN1_PROTOCOL_VER_MAJOR_RELEASE (3)
#define C_TLE_ASN1_PROTOCOL_VER_MINOR_RELEASE (2)
#define C_TLE_ASN1_PROTOCOL_VER_POINT_RELEASE (0)

#define C_TLE_NUM_OF_CELLS_IN_TDP_ARRAY      (5)
#define C_TLE_MAX_NUM_OF_SCANLIST_PER_CELL   (50)

#define C_TLE_AIR_INTERFACE_CAPABILITY_MASK_CDMA  (0x01) 
#define C_TLE_AIR_INTERFACE_CAPABILITY_MASK_GSM   (0x02) 
#define C_TLE_AIR_INTERFACE_CAPABILITY_MASK_WCDMA (0x04) 
#define C_TLE_AIR_INTERFACE_CAPABILITY_MASK_WiFi  (0x08)
#define C_TLE_AIR_INTERFACE_CAPABILITY_MASK_LTE   (0x10)
#define C_TLE_AIR_INTERFACE_CAPABILITY_MASK_EVDO  (0x20)    

#define C_TLE_MAX_MCC_MNC_BCD_VALUE (999)   /* Maximum value for MCC/MNC */
#define C_TLE_INVALID_L3_ID         (0xFFFF)

#define XTM_UDM_XTS_TIMEOUT         (20*1000)   /* 20 secs in ms units */

#define C_TLE_PROT_MAX_OEM_ID_LENGTH           (25)
#define C_TLE_PROT_MAX_MODEL_ID_LENGTH         (25)
#define C_TLE_PROT_MAX_SW_VERSION_LENGTH       (25)
#define C_TLE_PROT_MAX_ASN1_SW_VERSION_LENGTH  (255)
#define C_TLE_PROT_MAX_REALM_LENGTH            (255)

#define XTT_IP_ADDRESS_SIZE    (50)
#define XTT_IP_ADDRESS_V6_SIZE (20)

#define C_TLE_RAT_CDMA  0
#define C_TLE_RAT_GSM   1
#define C_TLE_RAT_WCDMA 2
#define C_TLE_RAT_LTE   3
#define C_TLE_RAT_MAX   (C_TLE_RAT_LTE + 1)

/* TLM flags masks */
#define C_TLE_MASK_W_ENTRY_MARK_TDS        0x01 /* Used to mark TDS bit in TLM flag */
#define C_TLE_MASK_W_ENTRY_IS_TDS          0xFE /* Used to check TDS bit in TLM flag */

#define C_TLE_USE_DELETE                     0
#define C_TLE_USE_RETENTION                  1
#define C_TLE_USE_CONDITIONAL_RETENTION      2
#define C_TLE_USE_REPLACE                    3
#define C_TLE_USE_INSERT                     4

#define C_TLE_USE_PARTICIPATE_MASK        0x01
#define C_TLE_USE_AVAILABLITY_MASK        0x02
#define C_TLE_USE_MANAGEMENT_MASK         0x1C

#define C_TLE_MAX_NUM_OF_FREQ                32


/* MOBILE_DATA_AVAILABILITY */
#define C_TLE_POS_AVAILABLITY_MASK_POS_VALID 0x01

#define C_TLE_LAST_UPLD_BSA_MSG (1U<<0)    /* Flag to indicate the last BSA Record in an upload */

#define C_TLE_LAST_UPLD_SNA_MSG (1U<<1)    /* Flag to indicate the last SNA Record in an upload */


/* Bit masks to be used with the time stamp type field in 
   xtt_XtaUpldCommonRecordType. */
#define XTT_TIME_TYPE_LOCAL  0x01  /* Bit 0. */
#define XTT_TIME_TYPE_GPS    0x02  /* Bit 1. */

#define XTT_GLOBAL_CID_INVALID    0
#define XTT_GLOBAL_CID_L1L2L3L4   1
#define XTT_GLOBAL_CID_L1L2L3     2
#define XTT_GLOBAL_CID_L1L2       3

/* These min/max values can be found in ASN.1 ICD */
#define C_TLE_TDP_MIN_ALTITUDE                  (-500)       /* in units of meters */
#define C_TLE_TDP_MAX_ALTITUDE                  (15883)
#define C_TLE_TDP_MIN_HOR_SPEED                 (0)          /* in units of 0.1 m/sec */
#define C_TLE_TDP_MAX_HOR_SPEED                 (1030)
#define C_TLE_TDP_MIN_HOR_SPEED_UNCERTAINTY     (0)          /* in units of 0.05 m/sec */
#define C_TLE_TDP_MAX_HOR_SPEED_UNCERTAINTY     (640)
#define C_TLE_TDP_MIN_HEADING                   (0)          /* in units of 360/2^10 from North */
#define C_TLE_TDP_MAX_HEADING                   (TWO_TO_10)
#define C_TLE_TDP_MIN_HEADING_UNCERTAINTY       (0)          /* in units of 180/2^6 */
#define C_TLE_TDP_MAX_HEADING_UNCERTAINTY       (TWO_TO_6)
#define C_TLE_TDP_MIN_HOR_UNCERTAINTY           (0)          /* K(0..127) */
#define C_TLE_TDP_MAX_HOR_UNCERTAINTY           (127)
#define C_TLE_TDP_MIN_VERT_UNCERTAINTY          (0)          /* K(0..127) */
#define C_TLE_TDP_MAX_VERT_UNCERTAINTY          (127)
#define C_TLE_TDP_MIN_CONFIDENCE                (0)          /* Percentage */
#define C_TLE_TDP_MAX_CONFIDENCE                (100)

#define C_TLE_NUM_MAX_TDP_MEAS                  (1)  /* Max # of TDP meas in a TDP Upload meas block */
/*==============================================================================

                                    MACROS

==============================================================================*/

/* Swap bytes in 16 bit value.  */
#define tle_bswap_16(x) ((((x) & 0xFF00) >> 8) | (((x) & 0x00FF) <<  8))

/* Swap bytes in 32 bit value.  */
#define tle_bswap_32(x) \
     ((((x) & 0xFF000000) >> 24) | (((x) & 0x00FF0000) >>  8) |     \
      (((x) & 0x0000FF00) <<  8) | (((x) & 0x000000FF) << 24))

#define C_TLE_RECORD_TYPE_BSA  (1)
#define C_TLE_RECORD_TYPE_SNA  (0)
#define C_TLE_BCD_FILLER 0x0FFF
/* CLIENT_DOWNLOADED_DATA */
#define C_TLE_MASK_UPLOAD_PREF_ALL          (0x01)
#define C_TLE_MASK_UPLOAD_PREF_FREQ_SCALE   (0x0E)
#define C_TLE_MASK_UPLOAD_PREF_UPLD_CTRL    (0x10)

/* C_TLE_VALID_ flags define the validity of data in xtt_MeasUploadCellDbRecordType 
   structure. */
#define C_TLE_VALID_GPSTIME  (1U<<0)   /* Position GPS time is valid */
#define C_TLE_VALID_TIMETAG  (1U<<1)   /* Time tag is valid */
#define C_TLE_VALID_FTA      (1U<<2)   /* FTA is valid */


#define C_TLE_NOT_LAST_XTA_MSG  0
#define C_TLE_LAST_XTA_MSG      (1U<<0) /* Flag to indicate the last XTA Record
                                           in download */

#define C_TLE_MAX_XT_SESS_RETRY_WO_SERV_BSA (3)

#define XTT_BPOOL_SIZE  BPOOL_SIZE_05 /*   1024 */

/* The following macro defines the number of XTA records that can be packed
within an IPC. For 1024 bytes IPC, 4 entries can be used.  
In general:
XTT_MAX_GEN_UPLOAD_NUM = 
       (XTT_BPOOL_SIZE - 18)/sizeof(xtt_XtaUpldGenericRecordType) */

#define XTT_MAX_GEN_UPLOAD_NUM 4 

/* The following macro defines the number of XTA records that can be packed
within an IPC. For 1024 bytes IPC, 60 XTA entries can be used.  
In general:
XTT_MOBILE_STATUS_MAX_PARTITION_NUM = (XTT_BPOOL_SIZE - 18)/sizeof(xtt_PartitionType) */

#define XTT_MOBILE_STATUS_MAX_PARTITION_NUM 60 

/* The following macro defines the number of XTA records that can be packed
within an IPC. For 1024 bytes IPC, 10 Generic entries can be used.  
In general:
XTT_MAX_DOWNLOAD_NUM = 
       (XTT_BPOOL_SIZE - 18)/sizeof(xtt_XtaDownldGenericRecordType) */

#define XTT_MAX_GEN_DOWNLOAD_NUM 10

/*==============================================================================

                          STRUCTURES AND ENUMERATIONS

==============================================================================*/
typedef enum
{
  C_TLE_CONFIDENCE_68,
  C_TLE_CONFIDENCE_99
} tle_eConfidenceLevelType;

typedef struct
{
  FLT   f_HorSpeedUncScale;
  FLT   f_HeadingUncScale;
  FLT   f_AltUncScale;
  FLT   f_PosUncScale;
  FLT   f_HeadingScale;
  FLT   f_HorSpeedScale;
} tle_ConfidenceScaleFactorType;

typedef enum
{
  C_TLE_CONN_TYPE_E_NONE  = 0,
  C_TLE_CONN_TYPE_E_DN_ONLY,       /* download only call flow */
  C_TLE_CONN_TYPE_E_UP_ONLY,       /* upload only call flow */
  C_TLE_CONN_TYPE_E_DNUP,          /* upload with free download call flow */
  C_TLE_CONN_TYPE_MAX,
  C_TLE_CONN_TYPE_SIZE = 0xFF  /* to fix enum size as int8 */
} tle_conn_e_type;

typedef enum
{
  C_TLE_PARTITION_INVALID = -1,
  C_TLE_PARTITION_SNA = 0,
  C_TLE_PARTITION_BSA = 1,
  C_TLE_PARTITION_END 
} e_TlePartitionType;


typedef struct 
{
  uint16  w_CarrierId;
  uint8   u_DivisionId; /* Index to divisions of carrier SNA data */
} xtt_PartitionIdSnaType;


typedef struct 
{
  uint16  w_Mcc;
  uint16 w_Sid;          /* System ID */
  uint16 w_Nid;          /* Network ID */
  uint8  u_CdmaSeqNum;   /* CDMA sequence number to resolve BSID ambiguity */
} xtt_PartitionBsaCdmaType;


typedef struct 
{
  uint16  w_Mcc;          /* Mobile Country Code */  
  uint16  w_Mnc;          /* Mobile Network Code */
  uint16  w_Lac;          /* Location Area Code */
} xtt_PartitionBsaGsmType;


typedef struct 
{
  uint16 w_Mcc;          /* Mobile Country Code */  
  uint16 w_Mnc;          /* Mobile Network Code */
  uint16 w_Lac;          /* Location Area Code */
} xtt_PartitionBsaWcdmaType;

typedef struct 
{
  uint16 w_Mcc;          /* Mobile Country Code */  
  uint16 w_Mnc;          /* Mobile Network Code */  
  uint16 w_Tac;          /* Tracking Area Code */
} xtt_PartitionBsaLteType;


typedef union
{
  xtt_PartitionBsaCdmaType  z_PartitionCdma;
  xtt_PartitionBsaGsmType   z_PartitionGsm;
  xtt_PartitionBsaWcdmaType z_PartitionWcdma;
  xtt_PartitionBsaLteType   z_PartitionLte;
} xtt_PartitionBsaType;


typedef struct 
{
  xtt_PartitionBsaType    z_BsaPartition;
  uint16                  w_CarrierId;
  uint8                   u_SubRegionId;       
} xtt_PartitionIdBsaType;


typedef union
{
  xtt_PartitionIdBsaType   z_PartitionBsa;
  xtt_PartitionIdSnaType   z_PartitionSna;
} xtt_PartitionIdType;

typedef struct 
{
  uint8                 u_PartitionType;         /* 0:SNA, 1:BSA */
  uint8                 u_AirInterfaceType;      /* 0:CDMA, 1:GSM, 2:WCDMA, 3:LTE */
  xtt_PartitionIdType   z_PartitionId;
  uint8                 u_PartitionRelVer;
} xtt_PartitionType;


/****************** Upload Data Structures ***********************************/

typedef struct 
{
  uint32 q_GpsMsec;    /* GPS msec*/
  uint16 w_GpsWeek;    /* GPS week number */
} xtt_GpsTimeType;

/* Use this structure as opposed to xtt_GpsTimeType when TUnc is needed */
typedef struct
{
  xtt_GpsTimeType z_GpsTime;
  FLT             f_ClkTimeUncMs; /* 1 sided max time uncertainty [msecs] */
} xtt_GpsTimeStructType;

typedef struct 
{
  uint16 w_Year;
  uint8  u_Month;
  uint8  u_Day;
  uint8  u_Hour;
  uint8  u_Minute;
  uint8  u_Seconds;
  uint8  u_TimeZone[3];  
} xtt_LocalTimeType;

typedef union
{
  xtt_LocalTimeType z_TimeStampLocal;
  xtt_GpsTimeType   z_TimeStampGps; 
} xtt_TimeStampType;

typedef struct 
{
  DBL    d_Lat;
  DBL    d_Long;
  DBL    d_Alt;
  int32  l_Latitude;   /* Latitude  [-90 , 90]  x (1-2^(-31)) degrees */
  int32  l_Longitude;  /* Longitude [-180 , 180]x (1-2^(-31)) degrees */

  uint16  w_Altitude;  /* Altitude of the best position estimate with the
                          lowest PosUnc for this cell. Height above the WGS84
                          reference ellipsoid, in units of 1 meter, in the 
                          range from  500 m to 15,883 m, where the binary 
                          value of the field conveys the height plus 500 m*/

  uint8  u_PosUncK;     /* PosUnc = 10 x ((1+0.1)^K - 1) [0, 1800Km] */
  uint8  u_AltUncK;     /* AltUnc = 45 x ((1+0.025)^K - 1) [0, 990m] */
} xtt_LastKnownPosType;

typedef struct 
{
  uint16 w_Sid;        /* System ID for CDMA.  */
  uint16 w_Nid;        /* Network ID for CDMA. */
} xtt_SndbKeyCdmaType;


typedef struct 
{
  uint16 w_MCC;
  uint16 w_MNC;
  uint16 w_LAC;        /* Location area code. */
} xtt_SndbKeyUmtsType;

typedef struct 
{
  uint16 w_MCC;
  uint16 w_MNC;
  uint16 w_TAC;        /* Tracking area code. */
} xtt_SndbKeyLteType;

typedef union 
{
  xtt_SndbKeyUmtsType z_KeyUmts;
  xtt_SndbKeyCdmaType z_KeyCdma;
  xtt_SndbKeyLteType  z_KeyLte;
} xtt_SndbKeyType;

typedef struct 
{
  xtt_LastKnownPosType z_LastPos;
  xtt_TimeStampType    z_TimeStamp;   

  xtt_GpsTimeType      z_PosGpsTime;
  uint8                u_ActiveRat;   /* 0: CDMA, 1: GSM, 2:WCDMA, 3:LTE */
  uint8                u_TimeStampType; /*Bit 1:Local time, Bit 2:GPS time */
  uint8                u_PosSrc; /* 0:None, 1:MS-Based, 2:PDE, 3:AFLT,           
                                    4:OTDOA, 5:Other external, 6:OTA */
  
  int32                l_MaxLat;
  int32                l_MinLat;
  int32                l_MaxLong;
  int32                l_MinLong;
  uint8                u_NumFixes; /* The number of fixes used to calculate
                                      the max/min, lat/long, in the range of 
                                      0-255. Zero represents that the number
                                      of fixes is larger than 255 */
} xtt_XtaUpldCommonRecordType;

typedef struct 
{
  xtt_GpsTimeType z_GpsTime;
  int32           l_TimeTagTimeBias;  /* unit of nano seconds */
  uint32          q_TimeTagTimeUncNsec; /* unit of nano seconds */
} xtt_TimeTagCommonType;


typedef struct
{
  xtt_TimeTagCommonType z_TimeTagCommon;
  uint32                q_Fn;          /* Frame Number ~4.615ms frames  */
  uint32                q_SymbolCount; /* QSymbol Count (sub fn).*/
  uint8                 u_TimeSlot;    /* Time Slot Number 0..7 */
} xtt_TimeTagGsmType;

typedef struct
{
  uint16        w_SFN;            /* System Frame Number 0..4095 - 10 ms frames */
  uint32        q_PnPosChipx8;    /* . 0..307200 Chipx8 per frame */
} xtt_TimeTagWcdmaValueType;

typedef struct
{
  uint64        t_TdsTime;    /* 64-bits value in TDS chipx8 units at status dump, clocked @ 10.24 MHz */
  uint32        q_TdsPhase;   /* 30-bits value in TDS sub chipx8 units at status dump */
} xtt_TimeTagTdsValueType;

typedef union
{
  xtt_TimeTagWcdmaValueType z_TimeTagWcdmaValue;
  xtt_TimeTagTdsValueType   z_TimeTagTdsValue;
} xtt_TimeTagValueType;

typedef struct
{
  xtt_TimeTagCommonType z_TimeTagCommon;
  xtt_TimeTagValueType z_TimeTag;
  uint8 u_Flags; /* Currently LSB used to mark TDS time-tags */
} xtt_TimeTagWcdmaType;

typedef struct
{
  xtt_TimeTagCommonType z_TimeTagCommon;
  uint64                t_LteRtc;        /* 64-bits, no rollover, chipx8 units,
                                            clocked @ 30.72MHz */
} xtt_TimeTagLteType;

typedef union
{
  xtt_TimeTagGsmType   z_TimeTagGsm;
  xtt_TimeTagWcdmaType z_TimeTagWcdma;
  xtt_TimeTagLteType   z_TimeTagLte;
} xtt_TimeTagType;


typedef struct 
{
  xtt_LastKnownPosType z_FtaPos;
  uint8                u_TimeRelUnc;
  uint8                u_TimingAdv;
} xtt_FtaPosType;


typedef struct
{
  xtt_FtaPosType  z_Pos;
  xtt_GpsTimeType z_GpsTime;  

  uint32  q_FrameNo;     /* BTS Reference Frame number during which the
                            location estimate was measured. The time of 
                            the GSM Reference Frame boundary is as
                            observed by MS, without timing Advance
                            compensation */

  uint16  w_BcchCarrier;  /* The absolute RF channel number of BCCH of the 
                          particular BTS corresponding to the GPS_TOW */

  uint16  w_CellId;       /* Cell ID at the time the location estimate is 
                             obtained.*/

  uint16  w_GpsTowSubMSec; /* only for MS-Based A-GPS. In units of 100ns the 
                              sub-millisecond part of the GPS time of 
                              measurement. This field, together with the 
                              GPS_TOW, provides a more accurate time stamp of 
                              the location estimate for MS-Based A-GPS. The 
                              precise GPS time of measurements [in msec] is 
                              equal to GPS_TOW + 0.0001*GPS_TOW_SUBMS. The MS 
                              shall only report this field when it is 
                             confident that any msec ambiguities is avoided. 
                             Otherwise, this field shall be set to '0'. */

  uint8   u_BsIc;        /* The BSIC of the BTS corresponding to GPS_TOW */ 
} xtt_FtaGsmType;

typedef struct
{
  uint64          t_UeGpsTiming;   /* GPS Time of Week in units of 1/16 UMTS chips */
  xtt_FtaPosType  z_Pos;
  uint32          q_CellId;        /* Cell ID at the time the location estimate  
                              is obtained */

  uint16  w_ReferenceSFN;     /* The SFN for which the location is valid; 
                                 at which the UE timing of cell frame is 
                                 captured */
                                 
  uint16  w_PrimaryCPichInfo; /* The reference cell for the GPS_TOW - SFN
                                 relationship [Pripmary scrambling code]*/
  
  uint16  w_GpsWeek;
} xtt_FtaWcdmaType;


typedef struct 
{
  xtt_FtaWcdmaType  z_FtaWcdma;  
  xtt_FtaGsmType    z_FtaGsm;
} xtt_FtaType;

typedef struct 
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
}xtt_CellCdmaType;

typedef struct 
{
  uint16 w_Mcc;     /* Layer 1 region ID */
  uint16 w_Mnc;     /* Layer 2 region ID */
  uint16 w_Lac;     /* Layer 3 region ID */
  uint16 w_Arfcn;   /* Absolute Radio Frequency Channel Number(0..1023)*/
  uint16 w_CellID;  /* Cell Identity - Integer (0..65535)*/
  uint8  u_BsIc;    /* Base Station Identity Code - Integer (0..63)*/
} xtt_CellGsmType;


typedef struct 
{
  uint16 w_Mcc;     /* Layer 1 region ID */
  uint16 w_Mnc;     /* Layer 2 region ID */
  uint16 w_Lac;     /* Layer 3 region ID: Location Area Code - Integer (1..65534) */
  uint32 q_UcId;    /* Layer 4 region ID: Cell Identity - Integer (0..268435455) */
  uint16 w_Freq;    /* UARFCN - Integer (0..16383)*/
  uint16 w_Psc;     /* Primary Synchronization Code - Integer (0..511) For TD-SCDMA Integer (0..127) */
} xtt_CellWcdmaType;

typedef struct 
{
  uint16 w_Mcc;     /* Layer 1 region ID */
  uint16 w_Mnc;     /* Layer 2 region ID */
  uint16 w_Tac;     /* Layer 3 region ID:  Integer (0..65535)
                       TAC = 0 means not available
                       TAC = 65534 means TAC is deleted */
  uint16 w_EARFCN;  /* Downlink center frequency - Integer (0..39649)*/
  uint16 w_PLID;    /* Physical cell ID - Integer (0..503)*/
  uint32 q_G_CI;    /* Layer 4 global cell ID from SIB type 1: Integer (0..268435455) or 2^28 - 1 */
} xtt_CellLteType;

typedef union
{
  xtt_CellWcdmaType z_CellInfoWcdma;
  xtt_CellCdmaType  z_CellInfoCdma;
  xtt_CellGsmType   z_CellInfoGsm;
  xtt_CellLteType   z_CellInfoLte;
} xtt_CellType;

typedef struct 
{
  xtt_XtaUpldCommonRecordType  z_CommonRecord;
  xtt_FtaType               z_FtaInfo;
  xtt_TimeTagType           z_TimeTagInfo;
  xtt_CellType              z_CellIdentification;

  uint8         u_PosConflict; /* 1:There is an inconsistency between position 
                          estimate and MCC (GMS/WCDMA) or SID ( CDMA) */
    
  uint8 u_Flag; /* Bit0: 1 indicates that GPS time is valid 
                   Bit1: 1 indicates that time tag included
                   Bit2: 1 indicates that FTA is included for this cell*/
} xtt_XtaUpldCellDbRecordType;


typedef struct 
{
  xtt_XtaUpldCommonRecordType  z_CommonRecord;
  xtt_SndbKeyType              z_SnDbKey;
} xtt_XtaUpldSnDbRecordType;


typedef union 
{
  xtt_XtaUpldCellDbRecordType z_CellRecord;
  xtt_XtaUpldSnDbRecordType   z_SnDbRecord;
} xtt_XtaUpldRecordType;

typedef struct 
{
  xtt_XtaUpldRecordType z_Record;
  uint8                 u_RecordType; /* 0:SNA, 1:BSA */
} xtt_XtaUpldGenericRecordType;

typedef struct 
{
  xtt_XtaUpldGenericRecordType  z_CellRecord[XTT_MAX_GEN_UPLOAD_NUM];  
  uint16 w_RecordCount; /* Number of cells in CellDB being transmitted
                     '0': no record is   present */

  uint8 u_Flag;        /* Bit0: 1 indicates last BSA set (C_TLM_LAST_UPLD_BSA_MSG)
                          Bit1: 1 indicates last SNA set (C_TLM_LAST_UPLD_SNA_MSG) */   
} xtt_XtaUpldDataType;


/* The data structure which contains the wwanClientDownloadData
   received from Application processor (AP) through ASN.1 server */
typedef struct 
{
  boolean b_Vaild; /* TRUE: Content is valid */
  struct 
  {
    uint16 w_TdpUploadConfigPresent : 1;
    uint16 w_UploadScreeningManagementPresent : 1;
    uint16 w_WwanUploadModePresent : 1;
    uint16 w_FreqUnitsPresent : 1;

    uint16 w_TdpMaxNumScanListPerCellPresent : 1;
    uint16 w_TdpGridSizeDegPresent : 1;
    uint16 w_TdpMinScanIntervalUploadSecPresent : 1;
    uint16 w_TdpMaxPosUncUploadPresent : 1;
    uint16 w_TdpScanSelectionParameterPresent : 1;
    uint16 w_ScanSelectionMethodPresent : 1;
  } z_GtpTdpConfigMask;

  boolean b_WwanUploadContent; /* FALSE – Upload only new information (default)
                                   TRUE  – Upload all */

  uint32 q_UploadFreqScale; /*  noTimer = 0,
                                weeks = 1,
                                days = 2,
                                hours = 3,
                                minutes = 4,
                                seconds = 5*/
  uint8   u_UploadFreqUnits;
  boolean b_WwanUploadControl; /* FALSE – Disable upload for subsequent transactions 
                                  TRUE  – Enable upload for subsequent transactions (default) */

  boolean b_UploadScreeningManagement; /* FALSE – Upload all data not explicitly prohibited (default)
                                          TRUE  – Upload only data explicitly allowed */

  boolean b_WwanUploadMode; /* FALSE – Upload CellDB using legacy binary
                               TRUE  – Upload CellDB via AP using ASN.1 (default setting) */

  uint32 q_WwanDownloadFlag; /* { cell_id_recognized(0), 
                                  participating_network_cell(1), 
                                  bsa_available(2) ,
                                  region_recognized(3) , 
                                  participating_network_region(4) , 
                                  sna_available(5),
                                  level3RegionRecognized(6),
                                  mobileL3CoverageComplete(7),
                                }
                                The wwanDownloadFlag will be set to all ‘0’ if the client did 
                                not request BSA or SNA download in the */

  boolean b_TdpUploadControl;  /* FALSE – Disable TDP upload for subsequent transactions 
                                  TRUE  – Enable TDP upload for subsequent transactions (default)
                                  Note 1: The following items are valid only if TDP uploads 
                                          are enabled. */

  uint16  w_TdpMaxNumScanListPerCell;     /* Maximum number scanlists (instances of lteTdpUploadData) 
                                             that may be uploaded per serving cell record*/

  uint16  w_TdpGridSizeDeg;               /* Recommended spacing between scanlist collection points 
                                             in the unit of 0.00001 degree of latitude and longitude.*/

  uint32  q_TdpMinScanIntervalUploadSec; /* Recommended minimum time between scanlists for upload 
                                            in seconds*/

  uint16  w_TdpMaxPosUncUpload;         /* Maximum position uncertainty for scanlists for 
                                           uploads in meters*/

  uint16  w_TdpScanSelectionParameter; /* A parameter for a cost function calculation, used 
                                          for selection among collected scans within a grid segment. */

  uint32  q_TdpScanSelectionMethod; /* {randomSelection(0), … }*/

} tle_TdpClientDownloadData;

typedef struct
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
} xtt_ClientDownloadDataType;

/* This structure is used for sending GTP configuration (MP and AP) to TLM
   All the relevant data are available in XTM and needs to be sent to TLM */

typedef struct 
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
  boolean b_UserSessionEnabled;    /* User session enabled as received through EULA in XTM */
  uint8   u_FeatureControl;          /* OEM control */
  /* TLE_API_E_GTP_DISABLED = 0, TLE_API_E_GTP_BASIC, TLE_API_E_GTP_POWER_DISABLED,
     TLE_API_E_GTP_PREMIUM, TLE_API_E_GTP_MAX */

  boolean b_GlobalUploadEnabled;   /* Global upload enable/disable, a received as part of 
                                      clientdownloadeddata in binary downloads */
  char  sz_ModemOemId[C_TLE_PROT_MAX_OEM_ID_LENGTH]; /* OEM ID as stored in XTM and reported 
                                                        in binary mobile status data*/
  char  sz_ModemModelId[C_TLE_PROT_MAX_MODEL_ID_LENGTH]; /* Modem model ID as reported in binary */
  char  sz_ModemSwVersion[C_TLE_PROT_MAX_ASN1_SW_VERSION_LENGTH]; /* Modem software version */
  boolean b_Valid;
  uint16  w_ProtocolVersion;

  uint16    w_HomeMcc;       /* Home MCC of mobile carrier (SS/SS) or */
                              /* designated data subscription (DS/DS)  */
                              
  uint16    w_HomeMnc;       /* Home MNC of mobile carrier (SS/SS) or */
                              /* designated data subscription (DS/DS)  */

  /* following fields valid if ucCarrierIdType = 1 */
  uint8     u_RealmLength;   /* Length of the Realm field in bytes */
  char      sz_Realm[C_TLE_PROT_MAX_REALM_LENGTH]; /* CDMA Realm */

  tle_TdpClientDownloadData z_TdpClientData;
  xtt_ClientDownloadDataType z_ClientDownloadedData;
  boolean b_BinaryUploadScreening; /* TRUE : Turn on upload screening in binary mode
                                      FALSE: Turn off upload screening in binary mode */
} tle_ClientInformationType;

typedef struct
{
  int32  l_Latitude;              /* In units of 180/2^32 degree
                                     (-90..+90*(1-2^-31)) */
  int32  l_Longitude;             /* In units of 360/2^32 degree
                                     (-180..+180*(1-2^-31)) */
  uint32 q_PosReliability;        /* (0..5) */
  uint32 q_TdpGpsMS;
  uint16 w_TdpGpsWeek;
  int16  x_Altitude;              /* In units of meters */
  uint16 w_TdpPosSource;
  uint16 w_HorizontalSpeed;       /* In units of 0.1 m/sec (0..1030) */
  uint16 w_HorSpeedUncertainty;   /* In units of 0.05 m/sec (0..640) */
  uint16 w_Heading;               /* In units of 360/2^10 degrees from North (0..1024) */
  uint8  u_HorUncertainty;        /* K (0..127) */
  uint8  u_VertUncertainty;       /* K (0..127) */
  uint8  u_Confidence;            /* (0..100) */
  uint8  u_headingUncertainty;    /* In units of 180/2^6 degrees (0..64)Š */

  struct 
  {
     uint8 u_GpsTimePresent : 1;
     uint8 u_PosReliabilityPresent : 1;
     uint8 u_AltitudePresent : 1;
     uint8 u_VertUncertaintyPresent : 1;
     uint8 u_ConfidencePresent : 1;
     uint8 u_VelocityPresent : 1;
     uint8 u_VelocityUncertaintyPresent : 1;
     uint8 u_VelocityV2Present : 1;
  } z_PositionMask;  /* Note: All bits are used, if need more flags we need to change this to uint16 */
} tle_TdpLocationInfoType;

typedef struct
{
  tle_TdpLocationInfoType z_LocationInfo;

  uint16 w_NumOfMeasAndCellData;
  tdp_MeasAndCellDataStructType z_MeasAndCellData[C_TLE_NUM_MAX_TDP_MEAS];
} tle_TdpLteUploadDataType;

typedef struct
{
  tdp_ServingCellConfigType z_ServingCellConfig;
  uint32 l_ScanSelectionMethod;

  uint16 w_NumOfLteUploadData;
  tle_TdpLteUploadDataType z_LteUploadData[C_TLE_MAX_NUM_OF_SCANLIST_PER_CELL];
} tle_TdpUploadDataType;

typedef struct
{
  boolean b_TdpUploadDataPresent;
  uint32  q_LteExtendedErfcn;
  xtt_XtaUpldCellDbRecordType z_CellDbUploadData;
  tle_TdpUploadDataType z_TdpUploadData;
} tle_TdpCellDbUploadData;

typedef struct 
{
  xtt_CellCdmaType  z_CellInfo;      
  int32             l_BsLatitude;  /* Base Station latitude  [-90 , 90]  x (1-2^(-31)) degrees */
  int32             l_BsLongitude; /* Base Station longitude [-180 , 180]x (1-2^(-31)) degrees */
} xtt_CurrentCellCdmaType;  


typedef struct 
{
  xtt_CellGsmType z_CellInfo;
  uint16 w_RxLev;    /* Received Signal Level - Integer (0..63)*/
} xtt_CurrentCellGsmType;


typedef struct 
{
  xtt_CellWcdmaType z_CellInfo; /* MRL can be added in this structure */ 
} xtt_CurrentCellWcdmaType;

typedef struct 
{
  xtt_CellLteType z_CellInfo; 
} xtt_CurrentCellLteType;

typedef union
{
  xtt_CurrentCellWcdmaType z_CellDataWcdma;
  xtt_CurrentCellCdmaType  z_CellDataCdma;
  xtt_CurrentCellGsmType   z_CellDataGsm;
  xtt_CurrentCellLteType   z_CellDataLte;
} xtt_CurrentCellType;

/****************** Mobile Status Data structures ****************************/

/* Structure for the MobileStatusData. Mobile status data is divided into
Two parts. Part 1 (xtt_MobileStatusDataHeaderType ) is passed in a single
IPC message. Part 2 (xtt_MobileStatusDataXtaType) can consist of multiple IPC
Messages, each can contain up to 60 XTA data (xtt_PartitionType)*/
typedef struct 
{
  xtt_CurrentCellType   z_CurCellData;
  uint8                 u_MobileDataAvailability; /* Bit 0 (LATEST_POSTITION available)
                                                         0:	Latest position is NOT valid
                                                         1:	Latest position is valid
                                                     All other values reserved */
  xtt_LastKnownPosType  z_LatestPosition;
  xtt_GpsTimeType       z_Gpstime; 
  uint32                q_MaxDownloadBsa;        /* Maximum total number of BSA
                                                    records (cells) downloadable
                                                    to mobile storage. Valid only if
                                                    BSA downloading is requested in
                                                    u_DownloadPreference */

  uint32                q_MaxDownloadSna;       /* Maximum total number of SNA
                                                    records (regions) downloadable
                                                    to mobile storage. Valid only if
                                                    SNA downloading is requested in
                                                    u_DownloadPreference */
  uint16                w_NumofPartitions;
  uint8                 u_CurAirInterfaceType; /* 0: CDMA
                                                  1: GSM
                                                  2: WCDMA
                                                  3: LTE */
  uint8                 u_DownloadPreference; /* Almanac download preference.
                                                 Bit0  (BSA request)
                                                    0  No BSA downloading.
                                                    1  BSA downloading requested. (default)
                                                 Bit1 (SNA request)
                                                    0  No SNA downloading.
                                                    1  SNA downloading requested. (default) */
} xtt_MobileStatusDataHeaderType;


typedef struct 
{
  xtt_PartitionType  z_PartitionData[XTT_MOBILE_STATUS_MAX_PARTITION_NUM]; /* To be packed in 
                                                            one  IPC*/
  uint8        u_Num;           /* Number of valid elements */
  uint8        u_Flag;          /* Bit0: 1 indicates last set*/   
} xtt_MobileStatusDataXtaType;

typedef struct 
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
} xtt_OpsPerfCounterType;

typedef struct
{
  xtt_GpsTimeType         z_StartTime;
  xtt_OpsPerfCounterType  z_Data;   /**/
  uint8                   u_MeasId; /* Identifier for the following measurement 
                                       or measurement list.
                                         0x00 - 0xC0	Individual measurements
                                         0xC1 - 0xFF	Measurement lists 

                                     Note: The only supported ID is C_TLM_OPS_MEASURMENT_ID */
} xtt_OpsMeasurementType;

typedef struct
{
  uint8                     u_Type;                         /* XTRA-T Server IP address Type Used 
                                                               0 - IPV4 , 1 - URL, 2 - IPV6                        */
  char                      cz_Url[XTT_IP_ADDRESS_SIZE];    /* XTRA-T Server URL Address. Null terminated.         */
  char                      cz_Uri[XTT_IP_ADDRESS_SIZE];    /* XTRA-T Server URI Address. Null terminated.         */
  uint32                    q_V4;                           /* XTRA-T Server IPv4 Address                          */
  uint16                    w_V6[XTT_IP_ADDRESS_V6_SIZE];   /* XTRA-T Server IPv6 Address                          */
  uint16                    w_Port;                         /* XTRA-T Server IP Port Number                        */  
} xtt_IPAddressType;

typedef struct
{
  xtt_IPAddressType         s_IpAddr;                    /* IP address info                                     */
  
  uint64                    t_NumDataToWrite;            /* Total number of data remained to be written (bytes) */
  uint16                    w_NumDataWritten;            /* Number of data being written (bytes)                */
  void *                    p_Data;                      /* Data being written                                  */
} xtt_XtaMsgWriteType;

typedef struct 
{
  xtt_PartitionIdType z_PartitionId;
  xtt_GpsTimeType     z_Gpstime;
  uint16              w_DataTypeMask; /* Bit 0 (TERRAIN_HGT_INCLUDED)
                                         Bit 1 (ANT_CENTER_INCLUDED)
                                         Bit 2 (CDMA_SEQ_NUM_INCLUDED)
                                         Bit 3 (CDMA_PILOT_PN_INCLUDED) */

  uint8               u_AirInterfaceType;      /* 0:CDMA, 1:GSM, 2:WCDMA 3:LTE */
  uint8               u_PartitionRelVer;
  uint8               u_DataUse; 
                  /* Bit 0: 
                        '0': non-participating network
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
                          
  uint8 u_PartitionOrder; /* Index based on the proximity of this XTA data 
                             partition to the serving cell or region.
                             The value of the index corresponds to the 
                             ordering of partitions in the partition list. The 
                             index starts from 0 with lower numbers indicating 
                             that the partition is closer to the serving cell 
                             or region. This field represents relevance of a 
                             partition to the serving cell or region. Value 
                             255 is reserved for partitions not included in 
                             the partition list*/

  uint8 u_GlobalCidStatus;   /* Defines the valid layers in each XTA record 
                                (only 2 LSB are used):
                                   0     INVALID
                                   1     Valid L1/L2/L3/L4 (BSA)
                                   2     L1/L2/L3
                                   3     L1/L2   */
  uint16 w_RecordsInPartition;
} xtt_XtaDownldHeaderDataType;


typedef struct 
{
  xtt_XtaDownldHeaderDataType z_XtaHeader;

  uint16 w_L1Id;
  uint16 w_L2Id;
  uint16 w_L3Id;
  uint16 w_L2CoverageRadius;
  uint16 w_L3CoverageRadius;
  int16  x_RefLat;
  int16  x_RefLon;
  int16  x_L3LatOffset;
  int16  x_L3LonOffset;
  uint16 w_L2MeanTerrainHight;
  uint16 w_L3MeanTerrainHight;
  uint8  u_L2StdTerrainHight;
  uint8  u_L3StdTerrainHight;
  uint8  u_L2Status; /* Bits 0:2 - Confidence level
                       0:Carrier BSA only, 1:high, 2:medium, 3:low*/

  uint8  u_L3Status;  /* Bits 0:2 - Confidence level
                         0:Carrier BSA only, 1:high, 2:medium, 3:low*/
} xtt_XtaDownldSnDbRecordType;


typedef struct 
{
  xtt_XtaDownldSnDbRecordType z_SndbRecord;
  uint32 q_L4Id;
  
  uint16 w_L4CoverageRadius;
  int16  x_L4LatOffset;
  int16  x_L4LonOffset;
  int16  x_AntCenterLatOffset;
  int16  x_AntCenterLonOffset;
  int16  x_AntCenterHight;
  uint16 w_L4MeanTerrainHight;
  uint8  u_L4StdTerrainHight;
//  uint16 w_CdmaPilotPn;       /* Pilot PN to resolve BSID ambiguity */
  uint8  u_CdmaSeqNum;        /* CDMA sequence number to resolve BSID ambiguity */
  uint16 w_PhyId;
  uint8  u_L4Status;          /* Bits 0:2 - Confidence level
                                 0:Carrier BSA only, 1:high, 2:medium, 3:low

                                 Bits 4:7 - Reserved

                                 Bits 3:5 - Estimation age. Valid only if Bit0:2 is not 0
                                            (carrier BSA only)
                                            0: <= 1 day
                                            1: > 1 day and <= 1 week
                                            2: > 1 week   and <= 2 weeks
                                            3: > 2 weeks  and <= 1 month
                                            4: > 1 month  and <= 4 months
                                            5: > 4 months and <= 1 year
                                            6: > 1 year
                                            7: Reserved

                                  Bit 6: 
                                      0: No antenna center information
                                      1: Antenna center information included

                                  Bit 7: Reserved   */
} xtt_XtaDownldCellDbRecordType;

typedef union 
{
  xtt_XtaDownldCellDbRecordType z_CellRecord;
  xtt_XtaDownldSnDbRecordType   z_SndbRecord;
} xtt_XtaDownldRecordType;

typedef struct 
{
  xtt_XtaDownldRecordType z_Record;
  uint8                   u_PartitionType;    /* 0:SNA, 1:BSA */
} xtt_XtaDownldGenericRecordType;


typedef struct 
{
  xtt_XtaDownldGenericRecordType z_XtaRecord[XTT_MAX_GEN_DOWNLOAD_NUM];

  uint8 u_RecordCount;        /* 0: no data is present. */
  uint8 u_Flag;             /* Bit0: 1 indicates last set*/   
} xtt_XtaDownldDataType;

/* LOG_XTM_HTTP_RESPONSE_STATUS_C */
typedef struct
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
} xtt_HttpResponseStatusDataType;

/* LOG_XTM_XTA_RESPONSE_STATUS_C */
typedef struct
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
} xtt_XtaResponseStatusDataType;

typedef struct
{
  xtt_IPAddressType         s_IpAddr;                    /* IP address info                                      */

  uint64                    t_NumDataToRead;             /* Number of Data remained to be read (bytes)           */
  uint16                    w_NumDataAvailableToRead;    /* Number of Data available for read (bytes)            */
  void *                    p_Data;                      /* Data available for read                              */
} xtt_XtaMsgReadType;

typedef enum
{
  C_TDP_SESSION_SRC_PERIODIC_TIMER,
  C_TDP_SESSION_SRC_PERIODIC_FORCED,
  C_TDP_SESSION_SRC_MEMORY_FULL,
  C_TDP_SESSION_SRC_INITIAL_UPLOAD,
  C_TDP_SESSION_SRC_END
} e_TleTdpSessionSrcType;

typedef struct
{
  uint16 w_ArrMatchedDlData[C_TLE_RAT_MAX];
  uint16 w_ArrCellsWithXta[C_TLE_RAT_MAX];
  uint16 w_ArrRegionsWithXta[C_TLE_RAT_MAX];
  uint16 w_ArrCellsMisMatchedL3[C_TLE_RAT_MAX];
  uint16 w_ArrCellsInTmpPartition[C_TLE_RAT_MAX];
  uint16 w_ArrRegionsWithCellInTmpPartition[C_TLE_RAT_MAX];
  uint16 w_ArrCellsInTouch[C_TLE_RAT_MAX];
  uint16 w_ArrRegionsInTouch[C_TLE_RAT_MAX];
  uint16 w_CellsWithTdpData;
  uint16 w_TdpScanListCount;
  uint16 w_RatCount;
} tle_OpsStatType;

/*==============================================================================

                      FUNCTION DECLARATIONS FOR MODULE

==============================================================================*/
                         
/*===========================================================================
FUNCTION
  tle_MapNumToRat

DESCRIPTION
  A helper function to map an integer to a valid known Radio Access
  Technolgoy (RAT)
  
PARAMETERS
   u_num - An integer representing RAT (input)
 
DEPENDENCIES
   None

RETURN VALUE
   cgps_ActiveRATEnumType 

SIDE EFFECTS
   None

===========================================================================*/
cgps_ActiveRATEnumType tle_MapNumToRat(uint8 u_num);

/*===========================================================================
FUNCTION
  tle_MapPartition

DESCRIPTION
  A helper function to map an integer to a valid known partition type.
  
PARAMETERS
   u_partition - An integer representing partition type (input)
 
DEPENDENCIES
   None

RETURN VALUE
   e_TlePartitionType 

SIDE EFFECTS
   None

===========================================================================*/
e_TlePartitionType tle_MapPartition(uint8 u_partition);


/*=============================================================================

  FUNCTION      tle_Bin2BCD

  DESCRIPTION
    This function converts a binary number in range 0-999 into a 4 bytes
    BCD number. The unused digits are filled with "1111"

  Example: The decimal number of 485 (0x01E5) will be converted to "0x485F"
  
  DEPENDENCIES

  PARAMETERS
    w_bin     : The binary value to be converted to BCD format (input)
    p_wBcd    : If successful, points to the encoded BCD value (output)
    
  RETURN VALUE
    TRUE : If the conversion is successful
    FALSE: Otherwise

  SIDE EFFECTS

=============================================================================*/
boolean tle_Bin2BCD(uint16 w_bin, uint16 * p_wBcd);

/*==============================================================================
FUNCTION
  tle_ReportPartition

DESCRIPTION
  Reports partition information through F3 messages.
  
PARAMETERS
  cpz_partition  - The partition information to be reported (input)

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
   None

==============================================================================*/
void tle_ReportPartition(const xtt_PartitionType * const cpz_partition);

/*===========================================================================
FUNCTION tle_GetEndianArchitecture

DESCRIPTION
  This function returns the endianness of the modem processor architecture

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  C_TLE_BIG_ENDIAN   : If MP has a big endian architecture
  C_TLE_LITTLE_ENDIAN: If MP has a little endian architecture

SIDE EFFECTS
  None.

===========================================================================*/
uint8 tle_GetEndianArchitecture(void);

/*===========================================================================
FUNCTION 
  tle_ConvInputPos2TdpLocationInfo

DESCRIPTION
  This function converts a location record from TLE input position format to
  TDP location information format.

PARAMETERS
 
  cpz_InputPos  - Points to the TLE input position structure
  (input)
 
  pz_TdpLocInfo - Points to the TDP location info structure
  (output)
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - Convert succeed.
  FALSE - Convert failed.

SIDE EFFECTS
  None.

===========================================================================*/
boolean tle_ConvInputPos2TdpLocationInfo(
   const tle_InputPosStructType *const cpz_InputPos,
   tle_TdpLocationInfoType *const pz_TdpLocInfo);


#endif /* TLE_COMMON_H */
