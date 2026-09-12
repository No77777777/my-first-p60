
/******************************************************************************
  @file: loc_api_2.h
  @brief: Location API header file, Major verion 2

  DESCRIPTION
    Qualcommm Location API header 2.0

  INITIALIZATION AND SEQUENCING REQUIREMENTS
    N/A

  -----------------------------------------------------------------------------
  Copyright (c) 2009-2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Copyright (c) 2013-2015 QUALCOMM Atheros, Inc.
  All Rights Reserved. 
  QUALCOMM Proprietary and Confidential.
  Copyright (c) 2015-2025 Qualcomm Technologies, Inc. 
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
******************************************************************************/

/*=====================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   ---      -------------------------------------------------------
06/18/24   bpk	   FR99502:Incorporating latest location fix in emergency INVITE
01/24/19   nath    Added support for ME metrics
03/29/18   sum     Added support for Unpropagated fix reporting 
03/31/17   yh       loc_ioctl clean up
10/05/16   yh       QMI_LOC_SECURE_GET_AVAILABLE_POSITION_REQ_V02 clean up
05/19/16   yh       LocMW refactoring
01/11/16   yh       Loc_Wifi cleanup
10/05/15   yh       QZSS
06/11/15   sj       LB 2.0 Integration
05/18/15   jv       Added deletion bits for SBAS eph & alm.
09/05/14   ssu     GM LOWI Integration
08/21/14   jv       Added support for 16-bit SVID  
04/15/14   yh       XTRA version check added  
03/10/14   ah       Added support for Set Premium Services Cfg & Get Avail WWAN Pos
01/24/14   ah       Added GLO support for Get/Set NMEA Type Requests in Loc MW
02/25/13   ssu      Adding a new WWAN type used for AGNSS Emergency connections. 
07/14/11   ly       Initial version

$Header: //components/rel/gnss8.mpss/11.1/gnss/loc_mw/inc/loc_api_2.h#3 $
$DateTime: 2025/06/19 05:09:19 $
======================================================================*/

#ifndef LOC_API_2_H
#define LOC_API_2_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "comdef.h"    /* Definition for basic types and macros */
#include "pdapi.h"
#include "location_service_v02.h"

/*=============================================================================
 *
 *                             DATA DECLARATION
 *
 *============================================================================*/


/******************************************************************************
 *  Constants and configuration
 *****************************************************************************/
#define LOC_API_MAJOR_VERSION_NUMBER   2
#define LOC_API_MINOR_VERSION_NUMBER   4

/* Return value for loc_open in case of failure. */
#define LOC_CLIENT_HANDLE_INVALID     -1

/* Return value of loc api calls for loc_close, loc_start_fix, loc_stop_fix 
 * These are also the status for the ioctl callback */
#define LOC_API_SUCCESS                     (0)
#define LOC_API_GENERAL_FAILURE             (1)
#define LOC_API_UNSUPPORTED                 (2)
#define LOC_API_INVALID_HANDLE              (4)
#define LOC_API_INVALID_PARAMETER           (5)
#define LOC_API_ENGINE_BUSY                 (6)
#define LOC_API_PHONE_OFFLINE               (7)
#define LOC_API_TIMEOUT                     (8)
#define LOC_API_XTRA_VERSION_CHECK_FAILURE  (9)
#define LOC_API_GNSS_DISABLED               (10)

/* This specific value is used for sending indication on which error status
 * is custom specific to the API, instead of generic ones like above */
#define LOC_API_CUSTOM_ERROR_STATUS         (0xFFFFFFFF)

/******************************************************************************
 *  Misc global data
 *****************************************************************************/
/* Client handle that will be returned to LocAPI Clients */
typedef int32 loc_client_handle_type;

/******************************************************************************
 *  Location callback event masks
 *****************************************************************************/
typedef uint64 locClnt_EventMaskType;
#define LOC_EVENT_PARSED_POSITION_REPORT             0x00000001ULL /* Position report */
#define LOC_EVENT_SATELLITE_REPORT                   0x00000002ULL /* Satellite in view report */
#define LOC_EVENT_NMEA_1HZ_REPORT                    0x00000004ULL /* NMEA report at 1HZ rate */
#define LOC_EVENT_NMEA_POSITION_REPORT               0x00000008ULL /* NMEA report at position report rate */
#define LOC_EVENT_NI_NOTIFY_VERIFY_REQUEST           0x00000010ULL /* NI notification/verification request */
#define LOC_EVENT_ASSISTANCE_DATA_REQUEST            0x00000020ULL /* Assistance data, eg: time, predicted orbits request */
#define LOC_EVENT_LOCATION_SERVER_REQUEST            0x00000040ULL /* Request for location server */
#define LOC_EVENT_IOCTL_REPORT                       0x00000080ULL /* Deprecated after LocMW refactory*/
#define LOC_EVENT_STATUS_REPORT                      0x00000100ULL /* Misc status report: eg, engine state */
#define LOC_EVENT_WPS_NEEDED_REQUEST                 0x00000200ULL /* Request to start/stop Wifi fixes */
#define LOC_EVENT_SENSOR_STREAMING_READY_STATUS      0x00000400ULL /* Request to listen to sensor Streaming Status */
#define LOC_EVENT_TIME_SYNC_REQ                      0x00000800ULL /* Request to listen for Time Sync Requests */
#define LOC_EVENT_SPI_STREAMING_REPORT               0x00001000ULL /* Request to listen for SPI Streaming reports */
#define LOC_EVENT_GEOFENCE_BATCH_BREACH_REPORT       0x00002000ULL /* Request for geofence batch breach reports */
#define LOC_EVENT_GNSS_MEASUREMENT_REPORT            0x01000000ULL
#define LOC_EVENT_SV_POLY_REPORT                     0x02000000ULL
#define LOC_EVENT_INJECT_TIME_REQUEST                0x08000000ULL
#define LOC_EVENT_INJECT_XTRA_DATA_REQUEST           0x10000000ULL
#define LOC_EVENT_FIX_SESSION_STATE_REPORT           0x20000000ULL
#define LOC_EVENT_MASK_BATCHING_STATUS               0x40000000ULL
#define LOC_EVENT_MASK_BATCH_FULL_NOTIFICATION       0x80000000ULL
#define LOC_EVENT_MASK_LIVE_BATCHED_POSITION_REPORT  0x100000000ULL
#define LOC_EVENT_PARSED_UNPROPAGATED_POSITION_REPORT 0x200000000ULL
#define LOC_EVENT_MASK_BAND_MEASUREMENT_METRICS      0x400000000ULL
#define LOC_EVENT_GNSS_NHZ_MEASUREMENT_REPORT        0x800000000ULL
#define LOC_EVENT_QUERY_XTRA_INFO_STATUS             0x1000000000ULL
#define LOC_EVENT_MASK_DBH_POSITION                  0x100000000000ULL

/* Reserved event for internal development, not exposed to open source */
#define LOC_EVENT_RESERVED                           0x8000000000000000ULL

/*=============================================================================
 *
 *           Location data types for API and callback functions
 *
 *============================================================================*/

/*  Conversions of Accuracy Levels from QMI-Loc Levels to Meters */
#define LOC_ACCURACY_HIGH      (50)
#define LOC_ACCURACY_MED       (500)
#define LOC_ACCURACY_LOW       (1000)
#define LOC_DEFAULT_ACCURACY   LOC_ACCURACY_LOW  /* Accuracy is LOW by default */

/******************************************************************************
 *
 *           Union of all location-event request and callback data
 *
 *****************************************************************************/
typedef enum
{
  LOC_CMD_TYPE_PD_SCHEDULE,
  LOC_CMD_TYPE_REPORT_POSITION,
  LOC_CMD_TYPE_NOTIFY_CLIENT,
  LOC_CMD_TYPE_INJECT_XTRA_DATA,
  LOC_CMD_TYPE_SILENTLY_DENY_NI_REQUEST,
  LOC_CMD_TYPE_INIT_QMI_LOC
}LocMW_LocCmdEnumT;

typedef struct
{
  loc_client_handle_type           l_ClientHandle;
  LocMW_LocCmdEnumT                e_CmdType;
  locClnt_EventMaskType            t_EventType;

  // data must be allocated with loc_calloc and freed by loc_free
  // if not freed and set to NULL explicitly by the receiver,
  // locMW_HandleLocCmd would call loc_free for it
  void *p_QmiEventIndMsg;
} LocMW_LocCmdStructT;

/* -----------------------------------------------------------------------*//**
@brief 
 Structure with information that is required for loc_MW handled indication
*/ /* ------------------------------------------------------------------------*/
typedef struct
{
  loc_client_handle_type l_ClientHandle;
  uint32                 q_QmiIndMsgId;

  // QMI_LOC Data must be allocated with loc_calloc and freed by loc_free
  // if not freed and set to NULL explicitly by the receiver,
  // locQmi_ProcessInd would call loc_free for it
  void *p_QmiIndMsg;
  uint32                 q_LocApiStatus;
}locMW_QmiLocIndMsgStructT;

/* Structure to define the Secure Get Location report Client  parameters */
typedef struct
{
  /* client which requested the best available  position from specified source */
  loc_client_handle_type     l_ClientHandle;

  /* variable to serialize the available  position request,
     current implementation does not support concurrent requests */
  boolean                    v_InProgress;

  /* transaction id for the best available  position from specified source request */
  uint32                     q_PosTxnID;

  /* variable to check if the Nonce provided in best available  position from specified source is valid */
  boolean                    v_PosIsNonceValid;

  /* Security Nonce for the available position request */
  uint64                     t_PosNonce;

  /* variable to check if the Data mode of the Get best Available position  */
  qmiLocSecureLocDataModeEnumT_v02  e_PosDataMode;

  /* variable for Position estimate Source to  Get  Available position  */
  qmiLocPositionSourceEnumT_v02    e_PosDataSource;

}locPd_GetSecureAvailPosType;

/* -----------------------------------------------------------------------*//**
@brief 
Request Message; Used by the control point to get the best available
                   position estimate from the location engine. 
*/ /* ------------------------------------------------------------------------*/
typedef struct {

  /* Mandatory */
  /*  Transaction ID */
  uint32 transactionId;
  /**<   Identifies the transaction. The transaction ID
       is returned in the Secure Get Best Available Position indication. */

  /* Optional */
  /*  Security Nonce */
  uint8 nonce_valid;  /**< Must be set to true if nonce is being passed */
  uint64 nonce;
  /**<   Identifies the Secure transaction. The nonce value
       is returned in the Secure Get Best Available Position indication. */

  /* Optional */
  /*  Position Source */
  uint8 locPositionSource_valid;  /**< Must be set to true if locPositionSource is being passed */
  qmiLocPositionSourceEnumT_v02 locPositionSource;
  /**<   Position Source.
 Valid values: \n
      - eQMI_LOC_POS_SRC_GNSS (1) --  GNSS Position
 */

  /* Optional */
  /*  Position Report Data mode */
  uint8 locPositionRptDataMode_valid;  /**< Must be set to true if locPositionRptDataMode is being passed */
  qmiLocSecureLocDataModeEnumT_v02 locPositionRptDataMode;
  /**<   Position Report Data Mode.
 If this TLV is not sent, Position report defaults Encryption turned ON.
 Valid values: \n
      - eQMI_LOC_SECURE_LOC_DATA_ENCRYPTED (1) --  Data shall be encrypted 
      - eQMI_LOC_SECURE_LOC_DATA_UNENCRYPTED (2) --  Data shall be un-encrypted 
 */

  /* Optional */
  /*  Secured position report data propagation */
  uint8 locPositionRptDataTimePropogation_valid;  /**< Must be set to true if locPositionRptDataTimePropogation is being passed */
  qmiLocPositionRptPropagationEnumT_v02 locPositionRptDataTimePropogation;
  /**<   Secured position report data propagation
 If this TLV is not sent, Position report is default propagated to current UTC time.
   - Parameter Type : int32 \n
   - Parameter Valid values: \n
    - eQMI_LOC_POS_REPORT_PROPAGATED_TO_CURRENT_UTC (1)   --  Position reports are Propagated to current UTC. \n
  */
}qmiLocGetAvailablePositionReqParamMsgT_v02;

/* -----------------------------------------------------------------------*//**
@brief 
Indication Message; Used by the control point to get the best available
                   position estimate from the location engine. 

*/ /* ------------------------------------------------------------------------*/

typedef struct {

  /* Mandatory */
  /*  Get Best Available Position Status */
  qmiLocStatusEnumT_v02 status;
  /**<   Status of the Get Best Available Position request.

 Valid values: \n
      - eQMI_LOC_SUCCESS (0) --  Request was completed successfully \n 
      - eQMI_LOC_GENERAL_FAILURE (1) --  Request failed because of a general failure \n 
      - eQMI_LOC_UNSUPPORTED (2) --  Request failed because it is not supported \n 
      - eQMI_LOC_INVALID_PARAMETER (3) --  Request failed because it contained invalid parameters \n 
      - eQMI_LOC_ENGINE_BUSY (4) --  Request failed because the engine is busy \n 
      - eQMI_LOC_PHONE_OFFLINE (5) --  Request failed because the phone is offline \n 
      - eQMI_LOC_TIMEOUT (6) --  Request failed because it timed out \n 
      - eQMI_LOC_CONFIG_NOT_SUPPORTED (7) --  Request failed because an undefined configuration was requested \n 
      - eQMI_LOC_INSUFFICIENT_MEMORY (8) --  Request failed because the engine could not allocate sufficient memory for the request \n 
      - eQMI_LOC_MAX_GEOFENCE_PROGRAMMED (9) --  Request failed because the maximum number of Geofences are already programmed \n 
      - eQMI_LOC_XTRA_VERSION_CHECK_FAILURE (10) --  Location service failed because of an XTRA version-based file format check failure  */


  /* Optional */
  /*  Transaction ID */
  uint8 transactionId_valid;  /**< Must be set to true if transactionId is being passed */
  uint32 transactionId;
  /**<   Transaction ID that was specified in the Get Best
       Available Position request. This parameter will
       always be present if the status field is set to
       SUCCESS. */

  /* Optional */
  /*  Latitude */
  uint8 latitude_valid;  /**< Must be set to true if latitude is being passed */
  double latitude;
  /**<   Latitude (specified in WGS84 datum).
       \begin{itemize1}
       \item    Type: Floating point
       \item    Units: Degrees
       \item     Range: -90.0 to 90.0      \begin{itemize1}
         \item    Positive values indicate northern latitude
         \item    Negative values indicate southern latitude
       \vspace{-0.18in} \end{itemize1} \end{itemize1} */

  /* Optional */
  /*   Longitude */
  uint8 longitude_valid;  /**< Must be set to true if longitude is being passed */
  double longitude;
  /**<   Longitude (specified in WGS84 datum).
       \begin{itemize1}
       \item    Type: Floating point
       \item    Units: Degrees
       \item    Range: -180.0 to 180.0     \begin{itemize1}
         \item    Positive values indicate eastern longitude
         \item    Negative values indicate western longitude
       \vspace{-0.18in} \end{itemize1} \end{itemize1} */

  /* Optional */
  /*   Circular Horizontal Position Uncertainty */
  uint8 horUncCircular_valid;  /**< Must be set to true if horUncCircular is being passed */
  float horUncCircular;
  /**<   Horizontal position uncertainty (circular).\n
       - Units: Meters */

  /* Optional */
  /*  Altitude With Respect to Ellipsoid */
  uint8 altitudeWrtEllipsoid_valid;  /**< Must be set to true if altitudeWrtEllipsoid is being passed */
  float altitudeWrtEllipsoid;
  /**<   Altitude with respect to the WGS84 ellipsoid.\n
       - Units: Meters \n
       - Range: -500 to 15883 */

  /* Optional */
  /*  Vertical Uncertainty */
  uint8 vertUnc_valid;  /**< Must be set to true if vertUnc is being passed */
  float vertUnc;
  /**<   Vertical uncertainty.\n
       - Units: Meters */

  /* Optional */
  /*  UTC Timestamp */
  uint8 timestampUtc_valid;  /**< Must be set to true if timestampUtc is being passed */
  uint64 timestampUtc;
  /**<   UTC timestamp.
       \begin{itemize1}
       \item Units: Milliseconds since Jan. 1, 1970
       \vspace{-0.18in} \end{itemize1} */

  /* Optional */
  /*  Time Uncertainty */
  uint8 timeUnc_valid;  /**< Must be set to true if timeUnc is being passed */
  float timeUnc;
  /**<   Time uncertainty. \n
       - Units: Milliseconds */

  /* Optional */
  /*  Horizontal Elliptical Uncertainty Semi-Minor Axis */
  uint8 horUncEllipseSemiMinor_valid;  /**< Must be set to true if horUncEllipseSemiMinor is being passed */
  float horUncEllipseSemiMinor;
  /**<   Semi-minor axis of horizontal elliptical uncertainty. \n
       - Units: Meters */

  /* Optional */
  /*  Horizontal Elliptical Uncertainty Semi-Major Axis */
  uint8 horUncEllipseSemiMajor_valid;  /**< Must be set to true if horUncEllipseSemiMajor is being passed */
  float horUncEllipseSemiMajor;
  /**<   Semi-major axis of horizontal elliptical uncertainty. \n
       - Units: Meters */

  /* Optional */
  /*  Horizontal Elliptical Uncertainty Azimuth */
  uint8 horUncEllipseOrientAzimuth_valid;  /**< Must be set to true if horUncEllipseOrientAzimuth is being passed */
  float horUncEllipseOrientAzimuth;
  /**<   Elliptical horizontal uncertainty azimuth of orientation. \n
       - Units: Decimal degrees \n
       - Range: 0 to 180 */

  /* Optional */
  /*  Horizontal Circular Confidence */
  uint8 horCircularConfidence_valid;  /**< Must be set to true if horCircularConfidence is being passed */
  uint8 horCircularConfidence;
  /**<   Horizontal circular uncertainty confidence. \n
       - Units: Percent \n
       - Range: 0 to 99 */

  /* Optional */
  /*  Horizontal Elliptical Confidence */
  uint8 horEllipticalConfidence_valid;  /**< Must be set to true if horEllipticalConfidence is being passed */
  uint8 horEllipticalConfidence;
  /**<   Horizontal elliptical uncertainty confidence. \n
       - Units: Percent \n
       - Range: 0 to 99 */

  /* Optional */
  /*  Horizontal Reliability */
  uint8 horReliability_valid;  /**< Must be set to true if horReliability is being passed */
  qmiLocReliabilityEnumT_v02 horReliability;
  /**<   Specifies the reliability of the horizontal position.

 Valid values: \n
      - eQMI_LOC_RELIABILITY_NOT_SET (0) --  Location reliability is not set 
      - eQMI_LOC_RELIABILITY_VERY_LOW (1) --  Location reliability is very low; use it at your own risk 
      - eQMI_LOC_RELIABILITY_LOW (2) --  Location reliability is low; little or no cross-checking is possible 
      - eQMI_LOC_RELIABILITY_MEDIUM (3) --  Location reliability is medium; limited cross-check passed  
      - eQMI_LOC_RELIABILITY_HIGH (4) --  Location reliability is high; strong cross-check passed 
 */

  /* Optional */
  /*  Horizontal Speed */
  uint8 horSpeed_valid;  /**< Must be set to true if horSpeed is being passed */
  float horSpeed;
  /**<   Horizontal speed. \n
       - Units: Meters/second */

  /* Optional */
  /*  Horizontal Speed Uncertainty */
  uint8 horSpeedUnc_valid;  /**< Must be set to true if horSpeedUnc is being passed */
  float horSpeedUnc;
  /**<   Horizontal speed uncertainty. \n
       - Units: Meters/second */

  /* Optional */
  /*  Altitude With Respect to Sea Level */
  uint8 altitudeWrtMeanSeaLevel_valid;  /**< Must be set to true if altitudeWrtMeanSeaLevel is being passed */
  float altitudeWrtMeanSeaLevel;
  /**<   Altitude with respect to mean sea level. \n
       - Units: Meters */

  /* Optional */
  /*  Vertical Confidence */
  uint8 vertConfidence_valid;  /**< Must be set to true if vertConfidence is being passed */
  uint8 vertConfidence;
  /**<   Vertical uncertainty confidence. \n
       - Units: Percent \n
       - Range: 0 to 99 */

  /* Optional */
  /*  Vertical Reliability */
  uint8 vertReliability_valid;  /**< Must be set to true if vertReliability is being passed */
  qmiLocReliabilityEnumT_v02 vertReliability;
  /**<   Specifies the reliability of the vertical position.

 Valid values: \n
      - eQMI_LOC_RELIABILITY_NOT_SET (0) --  Location reliability is not set 
      - eQMI_LOC_RELIABILITY_VERY_LOW (1) --  Location reliability is very low; use it at your own risk 
      - eQMI_LOC_RELIABILITY_LOW (2) --  Location reliability is low; little or no cross-checking is possible 
      - eQMI_LOC_RELIABILITY_MEDIUM (3) --  Location reliability is medium; limited cross-check passed  
      - eQMI_LOC_RELIABILITY_HIGH (4) --  Location reliability is high; strong cross-check passed 
 */

  /* Optional */
  /*  Vertical Speed */
  uint8 vertSpeed_valid;  /**< Must be set to true if vertSpeed is being passed */
  float vertSpeed;
  /**<   Vertical speed. \n
         - Units: Meters/second */

  /* Optional */
  /*  Vertical Speed Uncertainty */
  uint8 vertSpeedUnc_valid;  /**< Must be set to true if vertSpeedUnc is being passed */
  float vertSpeedUnc;
  /**<   Vertical speed uncertainty. \n
       - Units: Meters/second */

  /* Optional */
  /*  Heading */
  uint8 heading_valid;  /**< Must be set to true if heading is being passed */
  float heading;
  /**<   Heading. \n
         - Units: Degrees \n
         - Range: 0 to 359.999  */

  /* Optional */
  /*  Heading Uncertainty */
  uint8 headingUnc_valid;  /**< Must be set to true if headingUnc is being passed */
  float headingUnc;
  /**<   Heading uncertainty. \n
       - Type: Floating point \n
       - Range: 0 to 359.999 */

  /* Optional */
  /*  Magnetic Deviation */
  uint8 magneticDeviation_valid;  /**< Must be set to true if magneticDeviation is being passed */
  float magneticDeviation;
  /**<   Difference between the bearing to true north and the bearing shown
      on a magnetic compass. The deviation is positive when the magnetic
      north is east of true north. */

  /* Optional */
  /*  Technology Used Mask */
  uint8 technologyMask_valid;  /**< Must be set to true if technologyMask is being passed */
  qmiLocPosTechMaskT_v02 technologyMask;
  /**<   Technology used in computing this fix.
 Valid bitmasks: \n
      - QMI_LOC_POS_TECH_MASK_SATELLITE (0x00000001) --  Satellites were used to generate the fix 
      - QMI_LOC_POS_TECH_MASK_CELLID (0x00000002) --  Cell towers were used to generate the fix 
      - QMI_LOC_POS_TECH_MASK_WIFI (0x00000004) --  Wi-Fi access points were used to generate the fix 
      - QMI_LOC_POS_TECH_MASK_SENSORS (0x00000008) --  Sensors were used to generate the fix 
      - QMI_LOC_POS_TECH_MASK_REFERENCE_LOCATION (0x00000010) --  Reference Location was used to generate the fix 
      - QMI_LOC_POS_TECH_MASK_INJECTED_COARSE_POSITION (0x00000020) --  Coarse position injected into the location engine was used to
        generate the fix 
      - QMI_LOC_POS_TECH_MASK_AFLT (0x00000040) --  AFLT was used to generate the fix 
      - QMI_LOC_POS_TECH_MASK_HYBRID (0x00000080) --  GNSS and network-provided measurements were used to
        generate the fix 
 */

  /* Optional */
  /*  Dilution of Precision */
  uint8 DOP_valid;  /**< Must be set to true if DOP is being passed */
  qmiLocDOPStructT_v02 DOP;

  /* Optional */
  /*  Dilution of Precision */
  uint8 extDOP_valid;  /**< Must be set to true if extDOP is being passed */
  qmiLocExtDOPStructT_v02 extDOP;

  /* Optional */
  /*  GPS Time */
  uint8 gpsTime_valid;  /**< Must be set to true if gpsTime is being passed */
  qmiLocGPSTimeStructT_v02 gpsTime;

  /* Optional */
  /*  Time Source */
  uint8 timeSrc_valid;  /**< Must be set to true if timeSrc is being passed */
  qmiLocTimeSourceEnumT_v02 timeSrc;
  /**<   Time source.
 Valid values: \n
      - eQMI_LOC_TIME_SRC_INVALID (0) --  Invalid time. 
      - eQMI_LOC_TIME_SRC_NETWORK_TIME_TRANSFER (1) --  Time is set by the 1X system 
      - eQMI_LOC_TIME_SRC_NETWORK_TIME_TAGGING (2) --  Time is set by WCDMA/GSM time tagging (i.e.,
       associating network time with GPS time) 
      - eQMI_LOC_TIME_SRC_EXTERNAL_INPUT (3) --  Time is set by an external injection 
      - eQMI_LOC_TIME_SRC_TOW_DECODE (4) --  Time is set after decoding over-the-air GPS navigation data
       from one GPS satellite 
      - eQMI_LOC_TIME_SRC_TOW_CONFIRMED (5) --  Time is set after decoding over-the-air GPS navigation data
       from multiple satellites 
      - eQMI_LOC_TIME_SRC_TOW_AND_WEEK_CONFIRMED (6) --  Both time of the week and the GPS week number are known 
      - eQMI_LOC_TIME_SRC_NAV_SOLUTION (7) --  Time is set by the position engine after the fix is obtained 
      - eQMI_LOC_TIME_SRC_SOLVE_FOR_TIME (8) --  Time is set by the position engine after performing SFT;
       this is done when the clock time uncertainty is large 
      - eQMI_LOC_TIME_SRC_GLO_TOW_DECODE (9) --  Time is set after decoding GLO satellites 
      - eQMI_LOC_TIME_SRC_TIME_TRANSFORM (10) --  Time is set after transforming the GPS to GLO time 
      - eQMI_LOC_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING (11) --  Time is set by the sleep time tag provided by the WCDMA network 
      - eQMI_LOC_TIME_SRC_GSM_SLEEP_TIME_TAGGING (12) --  Time is set by the sleep time tag provided by the GSM network 
      - eQMI_LOC_TIME_SRC_UNKNOWN (13) --  Source of the time is unknown 
      - eQMI_LOC_TIME_SRC_SYSTEM_TIMETICK (14) --  Time is derived from the system clock (better known as the slow clock);
       GNSS time is maintained irrespective of the GNSS receiver state 
      - eQMI_LOC_TIME_SRC_QZSS_TOW_DECODE (15) --  Time is set after decoding QZSS satellites 
      - eQMI_LOC_TIME_SRC_BDS_TOW_DECODE (16) --  Time is set after decoding BDS satellites 
 */

  /* Optional */
  /*  Sensor Data Usage */
  uint8 sensorDataUsage_valid;  /**< Must be set to true if sensorDataUsage is being passed */
  qmiLocSensorUsageIndicatorStructT_v02 sensorDataUsage;

  /* Optional */
  /*  SVs Used to Calculate the Fix */
  uint8 gnssSvUsedList_valid;  /**< Must be set to true if gnssSvUsedList is being passed */
  uint32 gnssSvUsedList_len;  /**< Must be set to # of elements in gnssSvUsedList */
  uint16 gnssSvUsedList[QMI_LOC_MAX_SV_USED_LIST_LENGTH_V02];
  /**<   Each entry in the list contains the SV ID of a satellite
       used for calculating this position report. The following
       information is associated with each SV ID: \n
       Range: \n
       - For GPS:     1 to 32 \n
       - For SBAS:    33 to 64 \n
       - For GLONASS: 65 to 96 \n
       - For QZSS:    193 to 197 \n
       - For BDS:     201 to 237
    */

  /* Optional */
  /*  NONCE */
  uint8 nonce_valid;  /**< Must be set to true if nonce is being passed */
  uint64 nonce;

  /* Optional */
  /* variable to check  the Data mode of the Get Available Secure position report  */
  qmiLocSecureLocDataModeEnumT_v02  e_PosDataMode;
}qmiLocGetAvailablePositionIndParamMsgT_v02;

/******************************************************************************
 *  Location event callback function type
 *****************************************************************************/
typedef boolean(loc_event_cb_f_type)
   (
    // The loc client for this event, only client registered for this event will be called
    loc_client_handle_type     l_ClientHandle,

    locClnt_EventMaskType      t_EventType,

    const void *p_EventPayload
    /*~ PARAM loc_event_payload POINTER DISC loc_event */
    );
/*~ CALLBACK loc_event_cb_f_type */

/*===========================================================================
 *
 *                          FUNCTION DECLARATION
 *
 *==========================================================================*/

/*===========================================================================
FUNCTION loc_open2

DESCRIPTION
  This function connects a location client to the location engine.
  If connection is successful, it will return a handle which the location client
  should use for future location operations. If connection fails, the return
  value will be LOC_CLIENT_HANDLE_INVALID.

  This is different from loc_open2, as it does not require the event-
  registration mask.

PARAMETERS
  event_callback:          a function pointer which will be invoked by the GPS
                           server when an event of interest happens.

DEPENDENCIES

RETURN VALUE
  -1 (LOC_CLIENT_HANDLE_INVALID) if connection fails.
  Otherwise, a number >= 0.

SIDE EFFECTS

===========================================================================*/

loc_client_handle_type loc_open2
(
   loc_event_cb_f_type    *event_callback
   );
/*~ FUNCTION loc_open
     RELEASE_FUNC loc_close(_RESULT_) */

/*===========================================================================
FUNCTION loc_close

DESCRIPTION
  This function discconect the client from the location engine.

PARAMETERS
  handle:  the handle returned by loc_open

DEPENDENCIES

RETURN VALUE
  On success: it returns 0 (LOC_API_SUCCESS).
  On failure, it will return one-zero error code.

SIDE EFFECTS

===========================================================================*/
extern int32 loc_close
(
   loc_client_handle_type handle
   );
/*~ FUNCTION loc_close */

/*===========================================================================
FUNCTION loc_start_fix

DESCRIPTION
  This function starts positioning process. If client is currently requesting
  fixes, this function will be silently ignored and LOC_API_SUCCESS will be
  returned

  When position is available, position result will be passed to client via
  the callback funtion pointer provided in loc_open.

PARAMETERS
  handle:  the handle returned by loc_open

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern int32 loc_start_fix
(
   loc_client_handle_type handle
   );
/*~ FUNCTION loc_start_fix  */

/*===========================================================================
FUNCTION loc_stop_fix

DESCRIPTION
  This function stops positioning process for this client. If client is not
  currently requesting fixes, it will return LOC_API_SUCCESS.

PARAMETERS
  handle:  the handle returned by loc_open

DEPENDENCIES

RETURN VALUE
  On success: it returns 0 (LOC_API_SUCCESS).
  On failure, it will return one-zero error code.

SIDE EFFECTS

===========================================================================*/
extern int32 loc_stop_fix
(
   loc_client_handle_type handle
   );
/*~ FUNCTION loc_stop_fix */

#ifdef __cplusplus
}
#endif

#endif /* LOC_API_2_H */


