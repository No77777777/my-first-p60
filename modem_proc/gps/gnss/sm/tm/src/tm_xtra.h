#ifndef TM_XTRA_H
#define TM_XTRA_H


/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               TM XTRA header file

GENERAL DESCRIPTION
  This file contains data-structure definitions for TM-XTRA module

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

  
  Copyright (c) 2005 - 2012 Qualcomm Technologies Incorporated.
  Qualcomm Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2016 Qualcomm Atheros, Inc.
  Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
  Copyright (c) 2013 - 2021, Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_xtra.h#10 $

when       who   what, where, why
--------   ---   ----------------------------------------------------------
08/12/21  anksha Add support for Qtang2 XTRA download
01/28/20  anksha Added Support for Navic Assist download
06/27/19  anksha Added Feature xtra dynamic url selection
05/11/17   sg    Added support for XTRA client 2.5 
10/21/15   jv    Added XTRA3.1
09/16/15   skm   Xtra Version check fix 
05/12/14   ah    Added support for Set XTRA Version Check
11/28/11   rh    Changed minimum download interval from 1h to 24h
11/21/11   rh    Changed default download interval from 24h to 48h
06/18/07   mr    File created
===========================================================================*/

#include "gps_variation.h"
#include "pdapi.h"
#include "msg.h"
#include "tm_prtl_iface.h"
#include "tm_pdapi_client.h"
#include "gnss_xtra_common_ship.h"
#include "lm_api.h"

#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
#include "sm_util.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
=============================================================================

   XTRA data structures and declarations

=============================================================================
===========================================================================*/

/* polynomial used for crc computation */
#define TM_XTRA_CRC_POLYNOMIAL 0x8DEB2831

/* Maximum download interval in hours */
#define TM_XTRA_MAX_DOWNLOAD_INTERVAL (24 * 7)
/* Minimum download interval in hours*/
#define TM_XTRA_MIN_DOWNLOAD_INTERVAL 24
/* Default download interval in hours */
#define TM_XTRA_DEFAULT_DOWNLOAD_INTERVAL 48

/* Maximum number of download attempts (10) */
#define TM_XTRA_MAX_NUM_DOWNLOAD_ATTEMPTS 10
/* Minimum number of download attempts (0) */
#define TM_XTRA_MIN_NUM_DOWNLOAD_ATTEMPTS 0
/* Default number of download attempts (3) */
#define TM_XTRA_DEFAULT_NUM_DOWNLOAD_ATTEMPTS 3

/* Maximum time between retry attempts in mins */
#define TM_XTRA_MAX_TIME_BETWEEN_ATTEMPTS  120
/* Minimun time between retry attempts in mins */
#define TM_XTRA_MIN_TIME_BETWEEN_ATTEMPTS  1
/* Minimun time between retry attempts in mins */
#define TM_XTRA_DEFAULT_TIME_BETWEEN_ATTEMPTS  10

/* Maximum time info uncertainty in ms */
#define  TM_XTRA_MAX_TIME_INFO_DELAY_THRESH  10000
/* Minmum time info uncertainty in ms */
#define  TM_XTRA_MIN_TIME_INFO_DELAY_THRESH  100
/* Default time info uncertainty in ms */
#define  TM_XTRA_DEFAULT_TIME_INFO_DELAY_THRESH  1000

/* Maximum time info uncertainty in ms */
#define  TM_XTRA_MAX_TIME_INFO_UNC_THRESH  30000
/* Minmum time info uncertainty in ms */
#define  TM_XTRA_MIN_TIME_INFO_UNC_THRESH  100
/* Default time info uncertainty in ms */
#define  TM_XTRA_DEFAULT_TIME_INFO_UNC_THRESH  10000

/* Maximum server URL byte length */
#define TM_XTRA_MAX_SERVER_URL_SIZE  128
/* Max number of URLs */
#define TM_XTRA_MAX_NUMBER_URLS  3

/* Maxium part file size in bytes */
#define TM_XTRA_MAX_FILE_PART_SIZE (10 * 1024)

/* Maxium XTRA.bin file size in bytes */
#ifdef FEATURE_GNSS_XTRA3 

#define TM_XTRA_MAX_FILE_SIZE      (130 * 1024)

#elif defined(FEATURE_GNSS_XTRA2)

#define TM_XTRA_MAX_FILE_SIZE      (80 * 1024)

#else /* FEATURE_GNSS_XTRA2 */

#define TM_XTRA_MAX_FILE_SIZE      (45 * 1024)

#endif /* FEATURE_GNSS_XTRA2 */

/* NULL file size in bytes */
#define TM_XTRA_NULL_FILE_SIZE   0

/* Converstion for HOUR to MSEC */
#define TM_XTRA_MSEC_IN_HOUR (3600 * 1000)

/* Converstion for MIN to MSEC */
#define TM_XTRA_MSEC_IN_MIN (60 * 1000)

/*SECONDS in a week*/
#define TM_XTRA_GPS_SECONDS_IN_WEEK 604800.0
// Minimum GPS week that can be injected into LSM when the force flag is not set.
#define TM_XTRA_MIN_ACCEPTABLE_GPS_WEEK_FOR_INJECTION   1446

#define TM_XTRA_MSG_LOW( str, a, b, c )       MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "=TM XTRA= " str, a, b, c )
#define TM_XTRA_MSG_MED( str, a, b, c )       MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "=TM XTRA= " str, a, b, c )
#define TM_XTRA_MSG_HIGH( str, a, b, c )      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "=TM XTRA= " str, a, b, c )
#define TM_XTRA_MSG_ERROR( str, a, b, c )     MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "=TM XTRA= " str, a, b, c )

/* Timer value for auto download check */
#define TM_XTRA_AUTO_DLOAD_CHECK_TIME    TM_XTRA_MSEC_IN_HOUR
/* Timer value for recovery and data download retry */
#define TM_XTRA_RETRY_TIME               (15*1000)
/* Time out on last retry, in seconds  */
#define TM_XTRA_LAST_RETRY_TIMEOUT       (12*3600)

/* Initial value for retyr attempts */
#define TM_XTRA_INIT_RETRY_ATTEMPTS  0
/* randomize XTRA auto download over the last 30 minutes */
#define TM_XTRA_AUTO_DLOAD_RAND_MSECS    (TM_XTRA_MSEC_IN_HOUR/2)

/* randomize XTRA on-demand download over the last 60 minutes */
#define TM_XTRA_ONDEMAND_DLOAD_RAND_MSECS    (TM_XTRA_MSEC_IN_HOUR)
/* 1/RATE is the probablity that XTRA download will be requested early for per fix check */
#define TM_XTRA_ONDEMAND_DLOAD_RAND_RATE    (60)

/* Scaling factor to convert secs to msec */
#define TM_XTRA_SEC_TO_MSEC 1000

/* Conversion for MIN to SEC */
#define TM_XTRA_SEC_IN_MIN (60)

/*Minimum Time uncertainty for injected XTRA time*/
#define TM_XTRA_MIN_INJ_TIME_UNC  3500.0

typedef gnss_XtraVersionEnumType tm_xtra_version_e_type;

#define TM_XTRA_VERSION_UNKNOWN     XTRA_VERSION_UNKNOWN
#define TM_XTRA_VERSION_1           XTRA_VERSION_1
#define TM_XTRA_VERSION_2           XTRA_VERSION_2
#define TM_XTRA_VERSION_3           XTRA_VERSION_3
#define TM_XTRA_VERSION_MAX         XTRA_INVALID

#define TM_XTRA_BIT_SET(val, bit_no) (((val) >> (bit_no)) & 1)
#define TM_XTRA_BITS_SWAP(val, bit_pos1, bit_pos2) ((((val) & (1 << (bit_pos1))) >> (bit_pos1)) ^ (((val) & (1 << (bit_pos2))) >> (bit_pos2))) ? (((val)^(1<<(bit_pos1)))^(1<<(bit_pos2))):(val);

#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
#define TM_XTRA_SHIFT_RIGHT(val,num_bits) ((val) = (val)>>(num_bits))
#endif

/* For Gps Ephemeris*/
#define TM_XTRA_EPH_TYPE_GPS 1
/* For GLO Ephemeris*/
#define TM_XTRA_EPH_TYPE_GLO 2
/* For BDS Ephemeris*/
#define TM_XTRA_EPH_TYPE_BDS 3
/* For GAL Ephemeris*/
#define TM_XTRA_EPH_TYPE_GAL 4
/* For QZSS Ephemeris*/
#define TM_XTRA_EPH_TYPE_QZSS 5
/* For NAVIC Ephemeris*/
#define TM_XTRA_EPH_TYPE_NAVIC 6
/* Gps Eph Validity is only for (4*60*60*1000 = 14400000) 4 hours from the TOA*/
#define TM_XTRA_GPS_EPH_TIMEOUT 14400000
/* Glo Eph Validity is only for (45*60*1000 = 2700000) 45 mins from the TOA*/
#define TM_XTRA_GLO_EPH_TIMEOUT 2700000
/* BDS Eph Validity is only for (60*60*1000 = 3600000) 1 hour from the TOA*/
#define TM_XTRA_BDS_EPH_TIMEOUT 3600000
/* GAL Eph Validity is only for (3*60*60*1000 = 3600000) 3 hour from the TOA*/
#define TM_XTRA_GAL_EPH_TIMEOUT (10800000)
/* QZSS Eph Validity is only for (4*60*60*1000 = 14400000) 4 hours from the TOA*/
#define TM_XTRA_QZSS_EPH_TIMEOUT 14400000
/* NAVIC Eph Validity is only for (6*60*60*1000 = 21600000) 6 hours from the TOA*/
#define TM_XTRA_NAVIC_EPH_TIMEOUT 21600000
/* For Gps/QZSS CNAV ephemeris */
#define TM_XTRA_MIN_CNAV_EPH_ACCURACY_INDEX (-16)
#define TM_XTRA_MAX_CNAV_EPH_ACCURACY_INDEX (15)

/* Convert GPS time in weeks and msecs to full msec */
#define TM_XTRA_CONVERT_GPS_FULL_MS(week_num, msec ) \
              ( (double)(week_num) * (double)WEEK_MSECS + (double)(msec) )

/* Convert hours to msec */
#define TM_XTRA_CONVERT_HOURS_TO_MS(hours) \
             ((hours) * TM_XTRA_MSEC_IN_HOUR)

// Number of seconds between January 1, 1970 and January 6, 1980
#define DELTA_UTC_TO_GPS_SECONDS        (3657 * 24 * 60 * 60)

#define TM_XTRA_HANDSHAKE_CONFIG_INFO_DONE (1 << 0)
#define TM_XTRA_HANDSHAKE_CONFIG_INFO_NAVIC_ASSIST_SUPPORT (1 << 1)


#define TM_XTRA3_SERVER_FILENAME_SIZE_MAX           (20)
#ifndef FEATURE_QTANG2
#define TM_XTRA3_SERVER_MIN_ACCEPTED_FILENAME_SIZE  (10)  /* xtra3g.bin can be min constellation file possible for Dynamic url selection  */
#else
#define TM_XTRA3_SERVER_MIN_ACCEPTED_FILENAME_SIZE  (14)  /* xtra3grbej.bin min file for qtang2 */
#endif

#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
#define TM_XTRA3_MAX_SERVER_ENTRIES                 (25)
#define TM_XTRA3_BDS_B2A_BIT_POS                    (12)
#define TM_XTRA3_BDS_B1I_BIT_POS                    (2)
#define TM_XTRA_DO_BIT_CLEAR(val, bit_no) ((val) &= (~(1<<bit_no)))
#define TM_XTRA_DO_BIT_SET(val, bit_no)   ((val) |= ((1<<bit_no)))

#endif

/*TDM validity age of 48hrs */
#define TM_XTRA3_TDM_XTRA_VALIDITY_AGE   (48) /*48 hrs validity age for TDM XTRA*/

/* START:IMPORTANT NOTE:-This XTRA FILE version is MODEM PL/Brancg Dependent
   It is STRICTLY ADVISABLE to Check with DEV TEAM members 
   before Integrating this Part of Code to New PL/Branch.
*/
#define TM_XTRA3_FILE_VERSION_MAJOR    3  /*Legacy XTRA file Major version*/
#define TM_XTRA3_FILE_VERSION_MINOR    3  /*Legacy XTRA file Minor version*/
#define TM_PRECISE_XTRA3_FILE_VERSION_MAJOR    3  /*Precise XTRA file Major version*/
#define TM_PRECISE_XTRA3_FILE_VERSION_MINOR    6  /*Precise XTRA file Minor version*/

/*
** XTRA Module State information.
*/
typedef enum{
  TM_XTRA_STATE_IDLE,
  TM_XTRA_STATE_CLIENT_REQ,
  TM_XTRA_STATE_WAIT_DATA,
  TM_XTRA_STATE_INJECT_DATA,
  TM_XTRA_STATE_MAX,   /* For internal use */
} tm_xtra_state_e_type;

/* Enum for XTRA module timers */
typedef enum
{
  TM_XTRA_TIMER_ID_START = TM_XTRA_TIMER_ID_RANGE,
  TM_XTRA_TIMER_ID_AUTO_DLOAD,
  TM_XTRA_TIMER_ID_DLOAD_REQ_RETRY,
  TM_XTRA_TIMER_ID_NAVIC_ASSIST_RAND_DLOAD
} tm_xtra_timer_id_e_type;

typedef gps_TimeChkSourceType tm_xtra_data_check_source_e_type;

/* Enum for data validity check result
*/
typedef enum{
  TM_XTRA_DATA_CHECK_ERROR = -1,
  TM_XTRA_DATA_CHECK_INVALID_TIME,
  TM_XTRA_DATA_CHECK_SUCCESS
} tm_xtra_data_check_result_e_type;

/* Enum for the XTRA event type
*/
typedef enum
{
  TM_XTRA_EVENT_TYPE_NONE = 0,

  TM_XTRA_EVENT_TYPE_STATUS,
  TM_XTRA_EVENT_TYPE_REQ_DATA_DOWNLOAD,
  TM_XTRA_EVENT_TYPE_REQ_TIME,
  TM_XTRA_EVENT_TYPE_REQ_QUERY_DATA_VALID,
  TX_XTRA_EVENT_TYPE_CFG_INFO,
  TM_XTRA_EVENT_TYPE_FILENAME_UPDATE,
  TM_XTRA_EVENT_TYPE_REQ_NAVIC_ASSIST_DOWNLOAD,
  TM_XTRA_EVENT_TYPE_CFG_STATUS,
  TM_XTRA_EVENT_TYPE_MAX
} tm_xtra_event_e_type;

#ifdef FEATURE_XTRA3_DYN_URL_SELECTION

/*
For Creating the File Mask below config is used
GPS   - BIT 0
GLO   - BIT 1
BDS   - BIT 2 BDS B2A - BIT 12
GAL   - BIT 3
QZSS  - BIT 4
NAVIC - BIT 5
*/

typedef enum
{
   TM_XTRA3_FILE_MASK_G        = 0x0001,
   TM_XTRA3_FILE_MASK_R        = 0x0002,
   TM_XTRA3_FILE_MASK_C        = 0x0004,
   TM_XTRA3_FILE_MASK_GR       = 0x0003,
   TM_XTRA3_FILE_MASK_GC       = 0x0005,   
   TM_XTRA3_FILE_MASK_GRC      = 0x0007,
   TM_XTRA3_FILE_MASK_E        = 0x0008,
   TM_XTRA3_FILE_MASK_GE       = 0x0009,
   TM_XTRA3_FILE_MASK_J        = 0x0010,
   TM_XTRA3_FILE_MASK_GJ       = 0x0011,
   TM_XTRA3_FILE_MASK_GRJ      = 0x0013,
   TM_XTRA3_FILE_MASK_GCJ      = 0x0015,
   TM_XTRA3_FILE_MASK_GRCJ     = 0x0017,   
   TM_XTRA3_FILE_MASK_GREJ     = 0x001B,
   TM_XTRA3_FILE_MASK_GRCEJ    = 0x001F,
   TM_XTRA3_FILE_MASK_I        = 0x0020,
   TM_XTRA3_FILE_MASK_GEIJ     = 0x0039,
   TM_XTRA3_FILE_MASK_GRCEIJ   = 0x003F,
   TM_XTRA3_FILE_MASK_B        = 0x1000,//Making BIT12 For B2a case 
   TM_XTRA3_FILE_MASK_GRBE     = 0x100B,//Making BIT12 For B2a case   
   TM_XTRA3_FILE_MASK_GRBEJ    = 0x101B,//Making BIT12 For B2a case
   TM_XTRA3_FILE_MASK_GRBEJI   = 0x103B,//Making BIT12 For B2a case
}tm_xtra3_server_file_name_mask_e_type;


typedef enum
{
   TM_XTRA3_PRECEDENCE_ORDER_GPS      = 0,
   TM_XTRA3_PRECEDENCE_ORDER_GLO      = 1,
   TM_XTRA3_PRECEDENCE_ORDER_BDS      = 2,
   TM_XTRA3_PRECEDENCE_ORDER_GAL      = 3,
   TM_XTRA3_PRECEDENCE_ORDER_QZSS     = 4,
   TM_XTRA3_PRECEDENCE_ORDER_NAVIC    = 5,
   TM_XTRA3_PRECEDENCE_ORDER_MAX_CONS
}tm_xtra3_gnss_constellation_precedence_order_e_type;

typedef enum
{
   TM_XTRA3_DYN_URL_SEL_FILENAME_FOUND            = 0,
   TM_XTRA3_DYN_URL_SEL_FILENAME_NOT_FOUND        = 1,
   TM_XTRA3_DYN_URL_SEL_FILENAME_DEFAULT_SELECTED = 2,
   TM_XTRA3_DYN_URL_SEL_DB_NOT_INIT               = 3,
   TM_XTRA3_DYN_URL_SEL_DB_CORRUPT                = 4,
   TM_XTRA3_DYN_URL_CACHE_CORRUPT                 = 5,
   TM_XTRA3_DYN_URL_PARAMETER_ERROR               = 6,
   TM_XTRA3_DYN_URL_SEL_STATUS_MAX                = 0xFF
}tm_xtra3_dyn_url_selection_status_e_type;


typedef enum
{
   TM_XTRA3_DYN_URL_SEL_AT_XTRA_INIT,
   TM_XTRA3_DYN_URL_SEL_AT_XTRA_NORMAL
}tm_xtra3_dyn_url_selection_stage_e_type;
   
#endif
/*enum to define the TDM enablement state on XTRA*/
typedef enum
{
  TM_XTRA3_TDM_XTRA_STATE_NONE,
  TM_XTRA3_TDM_XTRA_STATE_ENABLED,    /*Tdm enabled stated read from band5 config*/
  TM_XTRA3_TDM_XTRA_STATE_DISABLED,   /*Tdm disbaled state read from band 5 config */
  TM_XTRA3_TDM_XTRA_STATE_SUSPENDED,  /*Tdm suspended due to override by control record for xtra*/
  TM_XTRA3_TDM_XTRA_STATE_MAX
}tm_xtra3_tdm_xtra_state_e_type;

typedef enum
{
  TM_XTRA3_PRECISE_XTRA_STATUS_NOT_KNOWN = 0,
  TM_XTRA3_PRECISE_XTRA_STATUS_DISABLED  = 1,
  TM_XTRA3_PRECISE_XTRA_STATUS_ENABLED   = 2,
  TM_XTRA3_PRECISE_XTRA_STATUS_MAX = 0xFF
}tm_xtra3_preciseXtra_enablement_status_e_type;

typedef enum
{ 
  TM_XTRA3_UIM_CARD_0 = 0,
  TM_XTRA3_UIM_CARD_1,
  TM_XTRA3_UIM_CARD_2,
  TM_XTRA3_UIM_CARD_MAX
}tm_xtra3_uim_card_slot_e_type;
/* Struct for status payload
*/
typedef struct
{
  pdsm_xtra_status_e_type    status;
  uint32                     value;
} tm_xtra_status_payload_s_type;

/* Struct for data download payload
*/
typedef struct
{
  boolean    force_data_dload;
} tm_xtra_data_dload_payload_s_type;

/* Struct for time info download payload
*/
typedef struct
{
  boolean    force_time_info_dload;
} tm_xtra_time_dload_payload_s_type;

/* Struct for data query payload
*/
typedef struct
{
  pdsm_xtra_status_e_type    status;
} tm_xtra_data_query_payload_s_type;
/** config info indication **/
typedef struct
{
  pdsm_xtra_status_e_type    status;
} tm_xtra_cfg_info_payload_s_type;

/* Union for the XTRA event payload
*/
typedef union
{
  tm_xtra_status_payload_s_type       status_payload;
  tm_xtra_data_dload_payload_s_type   data_dload_payload;
  tm_xtra_time_dload_payload_s_type   time_dload_payload;
  tm_xtra_data_query_payload_s_type   data_query_payload;
  tm_xtra_cfg_info_payload_s_type     cfg_info_payload;
}tm_xtra_event_payload_u_type;


/*
** TM XTRA session state information
*/
typedef struct
{

  /*===========================================*/
  /* Temp buffer to store the XTRA data file   */
  /*===========================================*/

  /* Memory buffer to store XTRA.BIN data from XTRA Client */
#ifdef FEATURE_CGPS_XTRA_DYNAMIC_ALLOC
  uint8 *xtraTempBuf;
#else
  uint8 xtraTempBuf[TM_XTRA_MAX_FILE_SIZE];
#endif

  uint32 q_XtraDataLen;

  /* GPS time from MGP */
  gps_TimeStructType  gps_time;

  /*========================================*/
  /* NV Settings cached here after power up */
  /*========================================*/

  /* Enable/Disable gpsOneXTRA feature */
  boolean                                          xtra_enabled;

  /* Number of unsuccessful download attempts before aborting (xtra client) */
  uint8                                            num_download_attempts;
  /* Number of minutes between download attempts */
  uint8                                            time_between_attempts;

  /* Enable/disable automatic periodic downloads of gpsOneXTRA files */
  boolean                                          auto_download_enabled;
  /* Number of hours between automatic downloads */
  uint16                                           download_interval;

  /* Primary gpsOneXTRA Server URL */
  uint8                                            primary_data_server_url[TM_XTRA_MAX_SERVER_URL_SIZE];
  /* Secondary gpsOneXTRA Server URL */
  uint8                                            secondary_data_server_url[TM_XTRA_MAX_SERVER_URL_SIZE];
  /* Tertiary gpsOneXTRA Server URL */
  uint8                                            tertiary_data_server_url[TM_XTRA_MAX_SERVER_URL_SIZE];

  /* TDM state variable */
  tm_xtra3_tdm_xtra_state_e_type                   e_tdmEnabled;

  /* Enable/disable time information feature */
  boolean                                          time_info_enabled;

  /* one-way delay failover threshold */
  uint32                                           time_info_oneway_delay_thresh;
  /* time uncertainty threshold */
  uint32                                           time_info_uncertainty_thresh;

  /* Primary SNTP server URL */
  uint8                                            primary_sntp_server_url[TM_XTRA_MAX_SERVER_URL_SIZE];
  /* Secondary SNTP server URL */
  uint8                                            secondary_sntp_server_url[TM_XTRA_MAX_SERVER_URL_SIZE];
  /* Tertiary SNTP server URL */
  uint8                                            tertiary_sntp_server_url[TM_XTRA_MAX_SERVER_URL_SIZE];

  /*========================================*/
  /* TM-XTRA module session information     */
  /*========================================*/
  /* data/time injection handler state */
  tm_xtra_state_e_type                             e_state;

  /* Number of dload request retry attempts on network error */
  uint8                                            retry_attempts;

  /* Number of dload request retry attempts on file error */
  uint8                                            dl_attempts;
  /* Continue blocking                                     */
  boolean                                          continue_blocking;

  /* Source of Time Information request */
  tm_xtra_data_check_source_e_type                 time_info_req_src;

  /* XTRA Version Check configured through QMI-LOC */
  pdsm_pa_xtra_ver_check_e_type                    version_check;

  /* Enablement Status for precise Xtra configured based on IPC from PE*/
  tm_xtra3_preciseXtra_enablement_status_e_type    preciseXtraEnableStatus;

  /*Xtra Client support of Precise Xtra downloads*/
  boolean                                          preciseXtraEnabledClient;

  /*========================================*/
  /* TM-XTRA module Timers                  */
  /*========================================*/
  os_TimerMsgType                 *auto_dload_timer;
  os_TimerMsgType                 *retry_dload_timer;
  qword                           time_of_last_download;
  uint32                          time_of_throttle_end;
  /* last key index used in XTRA3 signature verification */
  uint16 xtra3_last_key_index;
  /* UAX3 configuration */
  uint32 uax3_config;

} tm_xtra_state_type;


/*------------------------- Global Variables ------------------------------------*/
#ifdef FEATURE_CGPS_XTRA_DYNAMIC_ALLOC
extern tm_xtra_state_type*  p_xtra_state_info_ptr;  /* TM XTRA state information */
#else
extern tm_xtra_state_type  z_xtra_state_info;  /* TM XTRA state information */
#endif /*FEATURE_CGPS_XTRA_DYNAMIC_ALLOC*/


#ifdef FEATURE_XTRA3_DYN_URL_SELECTION

typedef struct
{
   uint16  w_FileMask;
   boolean b_IsNonMultibandCapable;
   uint8   u_XtraFileName[TM_XTRA3_SERVER_FILENAME_SIZE_MAX];
}tm_xtra3_server_file_store_s_type;


typedef struct
{
   uint8   u_NumFiles;
   boolean b_IsDbReady;
   tm_xtra3_server_file_store_s_type *z_XtraServerFileStore;
}tm_xtra3_server_file_name_database_s_type;

typedef struct
{

 boolean  b_IsMultiBandEnabled;
 boolean  b_IsDefaultUrlSelected;
 sm_ReportEventStructType   z_ActiveQualifiedConfigMaskBand1;
 sm_ReportEventStructType   z_ActiveQualifiedConfigMaskBand5;
 uint8    u_LastSelectedFileName[TM_XTRA3_SERVER_FILENAME_SIZE_MAX+1];
}tm_xtra3_dyn_url_selection_cache_s_type;

#endif
typedef struct
{
  uint8 u_asId;
  uint16  w_simMcc;
  boolean b_subcriptionReady;
}tm_xtra3_uim_data_s_type;


/*===========================================================================
=============================================================================

   Extern Functions

=============================================================================
===========================================================================*/

/*===========================================================================
FUNCTION tm_xtra_init_state_ptr

DESCRIPTION
Dynamically allocate memory for the XTRA state pointer

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
Memory allocation status
===========================================================================*/
extern boolean tm_xtra_init_state_ptr(void);

/*===========================================================================
FUNCTION tm_xtra_init

DESCRIPTION
Initialize the XTRA module

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern void tm_xtra_init( void );

/*===========================================================================
FUNCTION tm_xtra_handle_core_abort_recover

DESCRIPTION
Handle the abort and recover request from TM-Core.  Only certain abort reasons
should end TM-XTRA module processing.  For example, E911 call origination
or TM-Core task stop.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern void tm_xtra_handle_core_abort_recover
(
  tm_sess_stop_reason_e_type abort_reason
);

/*===========================================================================

FUNCTION tm_xtra_read_nv

DESCRIPTION
  This function reads NV items to set TM-XTRA configuration info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tm_xtra_read_nv( void );

/*===========================================================================
FUNCTION tm_xtra_reset_data_buffer

DESCRIPTION
This functions reset XTRA data download buffer if SET_DATA command fails.
It'll reset XTRA's state to IDLE as well

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
None
===========================================================================*/
extern void tm_xtra_reset_data_buffer( void );

/*===========================================================================
FUNCTION tm_xtra_abort_and_recover

DESCRIPTION
Initialize the XTRA module

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern void tm_xtra_abort_and_recover
(
  pdsm_xtra_status_e_type end_status
);


/*===========================================================================
FUNCTION tm_xtra_get_state_ptr

DESCRIPTION
Returns the global XTRA state info

INPUT PARAMS
tm_xtra_state_type* - reference to the global TM_XTRA state pointer

RETURN VALUE
void
===========================================================================*/
GPS_INLINE tm_xtra_state_type* tm_xtra_get_state_ptr(void)
{
#ifdef FEATURE_CGPS_XTRA_DYNAMIC_ALLOC
  return p_xtra_state_info_ptr;
#else
  return &z_xtra_state_info;
#endif /*FEATURE_CGPS_XTRA_DYNAMIC_ALLOC*/
}


/*===========================================================================

FUNCTION   tm_xtra_update_sess_state

DESCRIPTION
  This function is used by TM-XTRA module to update the current session state.

RETURN VALUE
  TRUE - State transition is successful
  FALSE - otherwise
============================================================================*/
extern boolean tm_xtra_update_sess_state
(
  tm_xtra_state_e_type new_state
);


/*===========================================================================

FUNCTION tm_xtra_handle_timer_expiry

DESCRIPTION
  This function is called by TM-core to inform XTRA module about timer
  expiry event.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tm_xtra_handle_timer_expiry
(
  const os_TimerExpiryType *timer_data
);


/*===========================================================================
FUNCTION tm_xtra_check_set_data_para

DESCRIPTION
Check the input parameters for XTRA set data client request.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern pdsm_xtra_cmd_err_e_type
tm_xtra_check_set_data_para
(
  const pdsm_xtra_cmd_s_type *req_ptr
);


/*===========================================================================
FUNCTION tm_xtra_check_initiate_download_para

DESCRIPTION
Check the input parameters for XTRA check init download parameters.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern pdsm_xtra_cmd_err_e_type
tm_xtra_check_initiate_download_para
(
  const pdsm_xtra_cmd_s_type *req_ptr
);


/*===========================================================================
FUNCTION tm_xtra_check_auto_download_para

DESCRIPTION
Check the input parameters for XTRA check auto download parameters.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern pdsm_xtra_cmd_err_e_type
tm_xtra_check_auto_download_para
(
  const pdsm_xtra_cmd_s_type *req_ptr
);

/*===========================================================================
FUNCTION tm_xtra_check_query_data_validity_para

DESCRIPTION
Check the input parameters for XTRA check data validy request parameters.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern pdsm_xtra_cmd_err_e_type
tm_xtra_check_query_data_validity_para
(
  const pdsm_xtra_cmd_s_type *req_ptr
);

/*===========================================================================
FUNCTION tm_xtra_check_query_data_validity_para

DESCRIPTION
Check the input parameters for XTRA check data validy request parameters.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern pdsm_xtra_cmd_err_e_type
tm_xtra_check_time_info_inject_para
(
  const pdsm_xtra_cmd_s_type *req_ptr
);

/*===========================================================================
FUNCTION tm_xtra_check_xtra_config_info_para

DESCRIPTION
Check the input parameters for query Xtra config info request .

INPUT PARAMS
pdsm_xtra_cmd_s_type *req_ptr : input xtra request pointer

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern pdsm_xtra_cmd_err_e_type
tm_xtra_check_xtra_config_info_para
(
  const pdsm_xtra_cmd_s_type *req_ptr
);

/*===========================================================================
FUNCTION tm_xtra_process_set_data_cmd

DESCRIPTION
Process the set data command request from XTRA client

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_process_set_data_cmd
(
  pdsm_xtra_cmd_s_type *cmd_ptr
);


/*===========================================================================
FUNCTION tm_xtra_process_initiate_download_cmd

DESCRIPTION
Process the initiate download command request from XTRA client

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_process_initiate_download_cmd
(
  pdsm_xtra_cmd_s_type *cmd_ptr
);


/*===========================================================================
FUNCTION tm_xtra_process_set_auto_download_cmd

DESCRIPTION
Process the set auto download command request from XTRA client

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_process_set_auto_download_cmd
(
  pdsm_xtra_cmd_s_type *cmd_ptr
);


/*===========================================================================
FUNCTION tm_xtra_process_query_data_validity_cmd

DESCRIPTION
Process the query data validity command request from XTRA client

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_process_query_data_validity_cmd
(
  pdsm_xtra_cmd_s_type *cmd_ptr
);

/*===========================================================================
FUNCTION tm_xtra_process_and_inject_data

DESCRIPTION
This function processes a data buffer containing the contents of an XTRA file.
It parses the assistance data and injects it into PE.

INPUT PARAMS
uint8 *pu_RawXtraData, 
uint32 q_length,
tm_xtra_version_e_type *pe_XtraVersion,
uint32 *pq_uDecodedMask

OUTPUT PARAMS

RETURN VALUE
boolean
===========================================================================*/
extern pdsm_xtra_status_e_type tm_xtra_process_and_inject_data
(
  uint8 *pu_RawXtraData, 
  uint32 q_length,
  tm_xtra_version_e_type *pe_XtraVersion,
  uint32 *pq_uDecodedMask
);

/*===========================================================================
FUNCTION tm_xtra_process_inject_time_info

DESCRIPTION
Process the inject time info command request from XTRA client

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_process_inject_time_info
(
  pdsm_xtra_cmd_s_type  *cmd_ptr
);

/*===========================================================================
FUNCTION tm_xtra_process_query_xtra_cfg_info_cmd

DESCRIPTION
Process the query xtra config info command request from XTRA client

INPUT PARAMS
pdsm_xtra_cmd_s_type  *cmd_ptr : Input xtra cmd .

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_process_query_xtra_cfg_info_cmd
(
  pdsm_xtra_cmd_s_type  *cmd_ptr
);

/*===========================================================================
=============================================================================

   TM-XTRA: PD API Event Handlers

=============================================================================
===========================================================================*/

/*===========================================================================
FUNCTION tm_xtra_trigger_event

DESCRIPTION
Send the specified XTRA event to the client

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_trigger_event
(
  tm_xtra_event_e_type          xtra_event,
  tm_xtra_event_payload_u_type  event_payload
);


/*===========================================================================
=============================================================================

   TM-XTRA: Data Validity Check Handler

=============================================================================
===========================================================================*/

/*===========================================================================
FUNCTION tm_xtra_data_check_handler

DESCRIPTION
Check the data validity based on the source. Before this function is called,
the global almanac data must be locked with gnss_LockXtraAndGetVersion()
and subsequently unlocked with gnss_UnlockXtraData();.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
  TM_XTRA_DATA_CHECK_ERROR
  TM_XTRA_DATA_CHECK_INVALID_TIME
  TM_XTRA_DATA_CHECK_SUCCESS

===========================================================================*/
extern tm_xtra_data_check_result_e_type tm_xtra_data_check_handler
(
  tm_xtra_data_check_source_e_type     source
);
/*=============================================================================
 TM-XTRA: tm_xtra2_check_GPS_validity
   
=============================================================================
===========================================================================*/

/*===========================================================================
FUNCTION tm_xtra2_check_GPS_validity

DESCRIPTION
Check the XTRA2 GPS data validity. 

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
  boolean: TRUE for success (Data is still valid), 
           FALSE for failure

===========================================================================*/
boolean tm_xtra2_check_GPS_validity(void);


/*=============================================================================

   TM-XTRA: tm_xtra2_check_GLO_validity
   
=============================================================================
===========================================================================*/

/*===========================================================================
FUNCTION tm_xtra2_check_GLO_validity

DESCRIPTION
Check the XTRA2 GLO data validity. 

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
  boolean: TRUE for success (Data is still valid), 
           FALSE for failure

===========================================================================*/
boolean tm_xtra2_check_GLO_validity(void);


/*===========================================================================
FUNCTION tm_xtra_check_time_uncertainty

DESCRIPTION
Check the current time uncertainty and fire the time info request if
necessary

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
boolean - if time was requested from XTRA
===========================================================================*/
extern boolean tm_xtra_check_time_uncertainty
(
  void
);

/*===========================================================================
FUNCTION tm_xtra_data_valid

DESCRIPTION
Checks if the XTRA data currently saved is valid or not

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
boolean - if XTRA data is valid
===========================================================================*/
extern boolean tm_xtra_data_valid
(
  void
);

/*===========================================================================
FUNCTION tm_xtra_gps_time_report_handler

DESCRIPTION
Handle the GPS time info from MGPS

Starts the following processes:
1) gpsOneXTRA data or time from clients
2) automatic download file time check
3) GPS fix request to check data validity

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern void tm_xtra_gps_time_report_handler
(
  gps_TimeStructType                         *p_gps_time
);


/*===========================================================================
FUNCTION tm_xtra_data_validity_check

DESCRIPTION
Request MGP for GPS time, must wait for response from MGP before validating
the gpsOneXTRA data/time. Once the MGP provides the GPS time, TM-Core will
call tm_xtra_gps_time_report_handler().


INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
extern boolean tm_xtra_data_validity_check
(
  tm_xtra_data_check_source_e_type     source
);

/*===========================================================================
FUNCTION tm_xtra_set_xtra_time_and_validity

DESCRIPTION
Get XTRA time and duration validy info from Almanac correctiond data.
This is a static memory space shared by SM and MGP.


INPUT PARAMS
pdsm_xtra_download_info_s_type* - pointer to xtra_download info where
                                  XTRA time and validity duration needs
								  to be set.

OUTPUT PARAMS
None

RETURN VALUE
  boolean

===========================================================================*/
extern boolean tm_xtra_set_xtra_time_and_validity
(
  pdsm_xtra_download_info_s_type *p_download_info

);

/*===========================================================================
FUNCTION tm_xtra_start_process_data_info

DESCRIPTION
Start the data injection to MGP, first process the gpsOneXTRA
file

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
None
===========================================================================*/
extern void tm_xtra_start_process_data_info
(
  const tm_xtra_data_check_source_e_type     source
);

/*===========================================================================
FUNCTION tm_xtra_start_process_time_info

DESCRIPTION
Start the time info injection to MGP, first process the gpsOneXTRA
time info

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
None
===========================================================================*/
extern void tm_xtra_start_process_time_info
(
  const tm_xtra_data_check_source_e_type     source
);
/*===========================================================================
FUNCTION tm_xtra_start_process_query_cfg_info

DESCRIPTION
Trigger time req to send config resp.

INPUT PARAMS
tm_xtra_data_check_source_e_type     source : Time check source type

OUTPUT PARAMS
None

RETURN VALUE
None
===========================================================================*/
extern void tm_xtra_start_process_query_cfg_info
(
  const tm_xtra_data_check_source_e_type     source
);

/*===========================================================================
FUNCTION tm_xtra_start_process_auto_dload_check

DESCRIPTION
Check the automatic periodic download parameters to determine if a new
gpsOneXTRA data download is required.  Send data download request
event to client if necessary.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
None
===========================================================================*/
extern void tm_xtra_start_process_auto_dload_check
(
  const tm_xtra_data_check_source_e_type     source
);


/*===========================================================================
FUNCTION tm_xtra_start_process_per_fix_check

DESCRIPTION
Check the automatic periodic download parameters to determine if a new
gpsOneXTRA data download is required.  Send data download request
event to client if necessary.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
None
===========================================================================*/
extern void tm_xtra_start_process_per_fix_check
(
  const tm_xtra_data_check_source_e_type     source
);

/**
 * retrieves a pointer to an lm_info data struct. this is used
 * for injecting xtra data to LM
 *
 * @return lm_info_s_type*
 *
 * NOTE 1: Currently, a call to this function must be followed
 * by lm_info(). DO NOT call this function again without first
 * calling lm_info, otherwise, prior data may be over-written.
 *
 * NOTE 2: calls to tm_xtra_get_lm_info() MUST be paired with a
 * corresponding call to tm_xtra_free_lm_info() (after calling
 * lm_info).
 *  */

lm_info_s_type * tm_xtra_get_lm_info(void);

/**
 * frees pointer to an lm_info data struct obtained from
 * tm_xtra_get_lm_info() for injecting xtra data to LM.
 * Currently, this function is simply a place-holder
 *
 *
 * @return boolean. true on success, false otherwise */
boolean tm_xtra_free_lm_info(lm_info_s_type* p_Info);

/*===========================================================================
FUNCTION tm_xtra_check_eph_validity

DESCRIPTION
Given the current time and the time of applicability of xtra file, this
function will check if gps/glo Eph should be injected or not.

INPUT PARAMS
gps_TimeStructType currTime - Curr valid GPS time in the system
gps_TimeStructType xtraTime - Time of first fix interval of GPS/GLO in the xtra.bin file
uint32 type - Gps or Glo

OUTPUT PARAMS
None

RETURN VALUE
boolean - TRUE (eph should be injected)
		- FALSE (eph should not be injected)
===========================================================================*/
boolean tm_xtra_check_eph_validity
(
  gps_TimeStructType currTime,
  gps_TimeStructType xtraTime,
  uint32             type
);
/*===========================================================================
FUNCTION tm_xtra_inj_done

DESCRIPTION
This function will mark as an indication from CD of the completion of Xtra
injection. Once this message is received, we send a trigger out to the
client indication "Xtra injection completed" also stop the retry download
timer and change the state of the system to TM_XTRA_STATE_IDLE

INPUT PARAMS
uint32  q_DecodeMask


OUTPUT PARAMS
None

RETURN VALUE
None
===========================================================================*/
void tm_xtra_inj_done(uint32  q_DecodeMask);

/*===========================================================================
FUNCTION tm_xtra_invoke_xtrat

DESCRIPTION
This function:
  1- Verifies to see if it is ok to initaite an XTRA-t session
  2- If it is ok to start an XTRA-T session, it initaites a PUNC based
     XTRA-T session (If PUNC > 25 Km) XTRA-T session will be initiated

INPUT PARAMS
e_cmd_err - Status of xtra injection

OUTPUT PARAMS
None

RETURN VALUE
 TRUE  - If the request for XTRA-T session is done
 FALSE - Otherwise
===========================================================================*/
boolean tm_xtra_invoke_xtrat(pdsm_xtra_cmd_err_e_type e_cmd_err);


/*===========================================================================
FUNCTION tm_xtra_is_enable

DESCRIPTION
Check if the XTRA feature is enabled

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
XTRA command status
===========================================================================*/
boolean tm_xtra_is_enable(void);

/*===========================================================================

FUNCTION tm_xtra_update_version_check

DESCRIPTION
  This function is called by TM-PDAPI to inform XTRA module about XTRA version
  check type for verification purposes.

DEPENDENCIES

RETURN VALUE
  TRUE  - Version check is successful
  FALSE - otherwise

SIDE EFFECTS
  None

===========================================================================*/
extern boolean tm_xtra_update_version_check
(
  pdsm_pa_xtra_ver_check_e_type xtra_version_check
);

/*===========================================================================

FUNCTION tm_xtra_get_version_check

DESCRIPTION
  This function is called by TM-PDAPI to get XTRA version check enabled parameter
  for LBS.

DEPENDENCIES

RETURN VALUE
  pdsm_pa_xtra_ver_check_e_type
    PDSM_PA_XTRA_VERSION_CHECK_DISABLED, XTRA Version Check Disabled
    PDSM_PA_XTRA_VERSION_CHECK_AUTO,     XTRA Version Check Automatic 
    PDSM_PA_XTRA_VERSION_CHECK_XTRA2,    XTRA Version Check with XTRA2 format 
    PDSM_PA_XTRA_VERSION_CHECK_XTRA3,    XTRA Version Check with XTRA3 format
    PDSM_PA_XTRA_VERSION_CHECK_MAX,      XTRA feature Disabled 

SIDE EFFECTS
  None

===========================================================================*/
extern pdsm_pa_xtra_ver_check_e_type tm_xtra_get_version_check ();

/*===========================================================================
FUNCTION tx_xtra_convert_Gps_to_utc

DESCRIPTION
Convert GPS time (Gps week and tow in sec) to UTC .

INPUT PARAMS
uint16 w_gpsWeek : Gps Week number
uint32 q_gpsTowSec : Gps Tow in sec

OUTPUT PARAMS
None

RETURN VALUE 
UTC time in sec
===========================================================================*/
uint64 tx_xtra_convert_Gps_to_utc(uint16 gpsWeek, uint32 gpsTowSec);
#ifdef __cplusplus
}
#endif

/*===========================================================================
FUNCTION tm_xtra3_update_active_constellation_config_info

DESCRIPTION
Updates the current constellation config info into dyn url cache

INPUT PARAMS
sm_ReportEventStructType  p_ReportEvent : Qualified constellation config info from ME

OUTPUT PARAMS
None

RETURN VALUE 
None
===========================================================================*/

void tm_xtra3_update_active_constellation_config_info(const sm_ReportEventStructType* p_ReportEvent);

#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
/*===========================================================================
FUNCTION tm_xtra3_init_server_file_name_record

DESCRIPTION
Initializes the XTRA3 Server supported filenames table.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
None
===========================================================================*/
void tm_xtra3_init_server_file_name_record();

/*===========================================================================
FUNCTION tm_xtra3_get_server_file_name_table

DESCRIPTION
Provides the reference to the server filenames records.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
Pointer to server filename database table.
===========================================================================*/
tm_xtra3_server_file_name_database_s_type * tm_xtra3_get_server_file_name_table();

/*===========================================================================
FUNCTION tm_xtra3_get_active_constellation_mask

DESCRIPTION
Provides the current active gnss constellations.

INPUT PARAMS
uint32 *pu_Band1Config : Band1 Constellation Config 

OUTPUT PARAMS
None

RETURN VALUE 
Mask of active constellations bit set
===========================================================================*/
uint16 tm_xtra3_get_active_constellation_mask(uint32 *pu_Band1Config);


/*===========================================================================
FUNCTION tm_xtra3_get_dyn_url_working_data

DESCRIPTION
Provides the reference to the working data used in dyn url selection

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
Pointer to the stored Data .
===========================================================================*/
tm_xtra3_dyn_url_selection_cache_s_type *tm_xtra3_get_dyn_url_cached_data();

/*===========================================================================
FUNCTION tm_xtra3_get_dyn_url_filename

DESCRIPTION
Provides the dynamically selected filename from list of commercialized filenames available based
on constellation config.

INPUT PARAMS
uint16  w_ActiveCfg          : current active constellations
unint8 **pu_FileName         : Pointer to the selected filename string
boolean b_IsMultiBandEnabled : Is Multiband enabled or not 

OUTPUT PARAMS
None

RETURN VALUE 
STATUS :FILENAME FOUND/NOT FOUND
===========================================================================*/
tm_xtra3_dyn_url_selection_status_e_type tm_xtra3_get_dyn_url_filename(uint16 w_ActiveCfg, uint8 **pu_FileName, boolean b_IsMultiBandEnabled);

/*===========================================================================
FUNCTION tm_xtra3_get_preferred_constellation_config

DESCRIPTION
Provides the next set of preferred constellations 

INPUT PARAMS
uint16  w_ActiveCfg : current active constellations
uint8 *pu_SelectedCfgIndex : Selected constellation Config  index in table

OUTPUT PARAMS
None

RETURN VALUE 
TRUE :upon success
FALSE:upon failure
===========================================================================*/
boolean tm_xtra3_get_preferred_constellation_config(uint16 w_ActiveCfg,uint8 *pu_SelectedCfgIndex );

/*===========================================================================
FUNCTION tm_xtra3_determine_file_name

DESCRIPTION
Determines the Filename to be used dynamically

INPUT PARAMS
tm_xtra3_dyn_url_selection_stage_e_type  w_XtraStage : current stage of xtra process (INIT/NORMAL)

OUTPUT PARAMS
None

RETURN VALUE 
STATUS : FILENAME FOUND/ NOT FOUND
===========================================================================*/
tm_xtra3_dyn_url_selection_status_e_type tm_xtra3_determine_file_name(tm_xtra3_dyn_url_selection_stage_e_type w_XtraStage);

/*===========================================================================
FUNCTION tm_xtra_get_Xtra_file_name_update_event_info

DESCRIPTION
Fill the Download info parameters to provided updated xtra filename to xtraclients

INPUT PARAMS
pdsm_xtra_info_s_type              info_ptr: Xtra server filename url info structure
tm_xtra_data_dload_payload_s_type  payload_ptr: event status payload

OUTPUT PARAMS
None

RETURN VALUE 
XTRA command status
===========================================================================*/
boolean tm_xtra_get_Xtra_file_name_update_event_info(pdsm_xtra_info_s_type *info_ptr,tm_xtra_data_dload_payload_s_type  *payload_ptr);

#endif /* FEATURE_XTRA3_DYN_URL_SELECTION */

/*===========================================================================
FUNCTION tm_xtra3_navic_assist_handle_download_req

DESCRIPTION
Processes the navic assist download request recieved from MGP via LM.

INPUT PARAMS
sm_ReqXtraStructType  pz_xtraAssistReq : Navic assist download request from MGP

OUTPUT PARAMS
None

RETURN VALUE 
None
===========================================================================*/

void tm_xtra3_navic_assist_handle_download_req(const sm_ReqXtraStructType *pz_xtraAssistReq);

/*===========================================================================
FUNCTION tm_xtra3_navic_assist_allow_download

DESCRIPTION
Determines if navic assist download request recieved from MGP should be allowed
or not.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
boolean : true if navic assist download request is allowed.
          false if navic assist download request is not allowed
===========================================================================*/

boolean tm_xtra3_navic_assist_allow_download();

/*===========================================================================
FUNCTION tm_xtra3_navic_assist_cancel_download

DESCRIPTION
Processes the navic assist cancel download request recieved from PE

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
None
===========================================================================*/
void tm_xtra3_navic_assist_cancel_download();

/*===========================================================================
FUNCTION tm_xtra3_navic_assist_get_data_download_event_info

DESCRIPTION
Fill the download request event parameters to be delivered to the XTRA clients

INPUT PARAMS
pdsm_xtra_info_s_type  *info_ptr                - ptr to pdsm data structure to hold server info
tm_xtra_data_dload_payload_s_type *payload_ptr  - ptr to payload data structure

OUTPUT PARAMS
None

RETURN VALUE 
XTRA command status
===========================================================================*/
boolean tm_xtra3_navic_assist_get_data_download_event_info(
                                                           pdsm_xtra_info_s_type             *info_ptr,
                                                           tm_xtra_data_dload_payload_s_type *payload_ptr );
/*================================================================================
FUNCTION tm_xtra3_get_uim_data_record()

DESCRIPTION
Provides the reference to the cached Sim mcc's

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
tm_xtra3_cached_sim_mcc_s_type*  z_uimData  ptr to struct containing sim cards mcc
===================================================================================*/

tm_xtra3_uim_data_s_type* tm_xtra3_get_uim_data_record();

/*===========================================================================
FUNCTION tm_xtra3_get_sim_mcc

DESCRIPTION
Reads the SIM IMSI from MMGSDI

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE 
TRUE/FALSE if success/Failure
===========================================================================*/

boolean tm_xtra3_get_sim_mcc();
/*================================================================================
FUNCTION tm_xtra3_get_srv_sys_mcc_List()
DESCRIPTION
Provides the reference to the cached PLMN MCC for subs
INPUT PARAMS
None
OUTPUT PARAMS
None
RETURN VALUE 
uint16*  w_srvSysMcc  ptr to struct containing mcc
===================================================================================*/
uint16* tm_xtra3_get_srv_sys_mcc_List();

/*===========================================================================
FUNCTION tm_xtra3_update_precise_xtra_enablement_status

DESCRIPTION
Update the precise Xtra enablement status as enabled/disabled

INPUT PARAMS
boolean enabled      - precise xtra enabled/disabled.

OUTPUT PARAMS
None

RETURN VALUE 
None
===========================================================================*/
void tm_xtra3_update_precise_xtra_enablement_status(             boolean enabled);

#endif /* TM_XTRA_H */
