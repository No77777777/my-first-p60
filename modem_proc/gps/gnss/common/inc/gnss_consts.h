/*=============================================================================

                        Session Manager API Header File

GENERAL DESCRIPTION
  gnss_consts.h contains system wide GNSS constants.
 
 
EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  

Copyright (c) 2005 - 2012 Qualcomm Technologies Incorporated.
Qualcomm Confidential and Proprietary. All Rights Reserved.
Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
Qualcomm Atheros Confidential and Proprietary. All Rights Reserved. 
Copyright (c) 2013 - 2024 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

=============================================================================*/


/*=============================================================================

                           EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/common/inc/gnss_consts.h#5 $ 
  $DateTime: 2024/07/25 22:07:08 $ 
  $Author: pwbldsvc $ 


when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/09/15    am     Removed N_MAX_LUCKY_SV
02/23/09   wjm     Initial version

=============================================================================*/

#ifndef GNSS_CONSTS_H
#define GNSS_CONSTS_H

#include "customer.h"
#include "comdef.h"
#include "gps_consts.h"


/* GPS Constants */
#define N_GPS_SV              N_SV  /* Number of available GPS satellites */
#define N_ACTIVE_GPS_CHAN     16  /* Total number of active GPS channels */
#define GPS_LOWEST_PRN        1   /* Lowest GPS PRN number for internal use */
#define GPS_L1_BW_HZ          (2046000.0) /* GPS L1 bandwidth in Hz */
#define N_MIN_GPS_SV_PRN      1    /* Minimum PRN */
#define N_MAX_GPS_SV_PRN      32   /* Maximum PRN */
#define XTRA3_GPS_FINE_CORR_BYTES_PER_FIT (37)

/* Glonass Constants */
#define N_GLO_SV                  24    /* Number of available Glonass 
                                          satellites */
#define N_MIN_GLO_SV_PRN          1   /* Minimum PRN */
#define N_MAX_GLO_SV_PRN          24   /* Maximum PRN */
#define XTRA3_GLO_FINE_CORR_BYTES_PER_FIT (36)

#define  GLO_SV_FREQ_OFFSET 8   /* conversion factor form Glo Frequency to Slot ID */

#define GLO_LOWEST_FREQ_NUMBER    (-7)  /* Lowest valid Glonass frequency 
                                          number */
#define GLO_HIGHEST_FREQ_NUMBER   6     /* Highest valid Glonass frequency 
                                          number */
/* Total number of valid Glonass frequencies */
#define N_GLO_FREQ                (GLO_HIGHEST_FREQ_NUMBER-GLO_LOWEST_FREQ_NUMBER+1)
#define N_ACTIVE_GLO_CHAN         14    /* Total number of active Glonass 
                                          channels */
#define GLO_SLOTID_OFFSET         64    /* Offset between Glonass slot ID and 
                                          the Glonass SV ID used in MGP 
                                          code. */
#define GLO_SLOTID_UNKNOWN        255   /* Glonass slot ID is currently 
                                          invalid */
#define GLO_INVALID_FREQ          (GLO_HIGHEST_FREQ_NUMBER + 1) 
#define GLO_LOWEST_ID             (GLO_SLOTID_OFFSET + 1)
#define GLO_L1_BW_HZ              (8437500.0) /* GLO L1 bandwidth in Hz */

/* Gen8b Lite has a reduced number of GLO HW channels */
#define C_MAX_GLO_CHANNELS_8  (8)
#define C_MAX_GLO_CHANNELS_12 (12)

/* This should match the similar named entries in mgp_me_api.h */
#define C_MGP_SRCH_MODE_L1_CX2_20_PDI    3
#define C_MGP_SRCH_MODE_R1_CX2_20_PDI   12
#define C_MGP_SRCH_MODE_B1_CX2_20_PDI   24
#define C_MGP_SRCH_MODE_E1_CX4_20_PDI   30

/* BDS-related constants */
#define N_BDS_SV_TOTAL          37 /* Number of Total BDS satellites defined in ICD*/
#define N_BDS_SV                N_BDS_SV_TOTAL /* Number of available BDS satellites */
#define N_MIN_BDS_SV_PRN        1    /* Minimum PRN */
#define N_MAX_BDS_SV_PRN        37   /* Maximum PRN */
#define XTRA3_BDS_FINE_CORR_BYTES_PER_FIT (42)

/* BDS SV ID offset for internal use */
#define BDS_SV_ID_OFFSET        200
#define BDS_LOWEST_ID           (BDS_SV_ID_OFFSET + 1)
#define C_LAST_BDS_ID           (BDS_SV_ID_OFFSET + N_BDS_SV_TOTAL)

#define N_ACTIVE_BDS_CHAN       (24)

/* NavIC internal const*/
#define NAVIC_SV_ID_OFFSET      (400)
#define NAVIC_LOWEST_ID         (NAVIC_SV_ID_OFFSET + 1)
#define GPS_NAVIC_WEEKS         (1024)

#define BDS_L1_BW_HZ            (4092000.0) /* BDS L1 bandwidth in Hz */

#define N_ACTIVE_QZSS_SBAS_CHAN   16 /*!< Number of active QZSS + SBAS channels */
#define QZSS_SV_ID_OFFSET        192
#define QZSSL1S_SV_ID_OFFSET     128
#define SBAS_SV_ID_OFFSET         87
#define XTRA3_QZSS_FINE_CORR_BYTES_PER_FIT (46)

/* GAL-related constants */
#define N_GAL_SV                36  /* Number of available GAL satellites */
#define N_MIN_GAL_SV_PRN        1   /* Minimum PRN */
#define N_MAX_GAL_SV_PRN        36   /* Maximum PRN */
#define XTRA3_GAL_FINE_CORR_BYTES_PER_FIT (43)

#define GAL_SV_ID_OFFSET        300
#define N_ACTIVE_GAL_CHAN       16  /* Number of active GAL channels */
#define GAL_LOWEST_ID           (GAL_SV_ID_OFFSET + 1)
#define GAL_L1_BW_HZ            (4092000.0) /* GAL L1 bandwidth in Hz */

/* NavIC -related constants */
#define NAVIC_N5_BW_HZ            (2046000.0) /* Navic L5 bandwidth in Hz */
#define XTRA3_NAVIC_FINE_CORR_BYTES_PER_FIT (44)

/* GNSS Constants */
#define N_MAX_VISIBLE_GNSS_SV  36 
#define GNSS_SVID_INVALID     0   /* SVID is unknown/invalid. Ref. Callisto_Svid_Mapping, Table 2. */ 

#define N_MAX_GNSS_SV         MAX(MAX(MAX(MAX(N_GPS_SV + N_QZSS_SV, N_GLO_SV), N_BDS_SV), N_GAL_SV),N_NAVIC_SV) /*!< Max number of SVs of a GNSS system */
#define N_ALL_GNSS_SV         (N_GPS_SV + N_GLO_SV + N_BDS_SV + N_GAL_SV + N_QZSS_SV + N_NAVIC_SV)  /* Total number of SVs of GNSS systems */
#define N_ALL_GNSS_MB_MEAS  (N_GPS_SV * 3 + N_GLO_SV * 2 + N_BDS_SV * 2 + N_GAL_SV * 2 + N_QZSS_SV * 3 + N_NAVIC_SV)

#define R_EARTH      6378.1363 /* Radius of earth [Km] */
#define H_I          350       /* Maximum electron density/altitude of ionosphere
                                  [Km]*/
/* Number of TDP channels */
#define N_ACTIVE_TDP_CHAN       1

/* TDP offset for internal use. Physical IDs in the range 0-503 will
be converted to 1000-1503 for internal use within PE */
#define TDP_PHY_ID_OFFSET       1000

/* Total number of active channels including GPS LBW and Glonass */
#define N_ACTIVE_GNSS_CHAN    (N_ACTIVE_GPS_CHAN + N_ACTIVE_GLO_CHAN + N_ACTIVE_BDS_CHAN +  N_ACTIVE_GAL_CHAN + N_QZSS_SV + N_ACTIVE_NAVIC_CHAN)

#define N_ACTIVE_MEAS_CHAN    (N_ACTIVE_GNSS_CHAN + N_ACTIVE_TDP_CHAN)

/* Total number of multiband measurement channels, 
   legacy + GPS L2, GLO G2, BDS B2, QZSS L2 and 
   (GPS L5, GAL E5A, QZSS L5, 14 channels for all L5 bands)*/
#define N_ACTIVE_MB_MEAS_CHAN (N_ACTIVE_GNSS_CHAN + \
N_ACTIVE_GPS_CHAN + N_ACTIVE_GLO_CHAN + N_ACTIVE_BDS_CHAN + N_QZSS_SV + 14)

#define MAX_ACTIVE_GNSS_CHAN  (MAX(MAX(MAX(N_ACTIVE_GPS_CHAN, N_ACTIVE_GLO_CHAN), N_ACTIVE_BDS_CHAN), N_ACTIVE_GAL_CHAN))

/* Max number of GPS + GLO SVs */
#define N_CPE_MAX_SV          (N_GPS_SV + N_GLO_FREQ)

/* Note: Total number of active channels for NHz Measurements is the same as 1Hz
** since we are using the same meas block structure. For multiband NHz, since
** only one NHz PRR measurement will be selected for one SV, the size of total NHz
** measurements on mbm_MbmNHzMeasBlkType will be N_ACTIVE_GNSS_CHAN. That is,
** one active_chan for each constellation, GPS+GLO+BDS+GAL+QZSS+NAVIC. */

/* Max number of GPS + GLO SVs */
#define N_CPE_MAX_SV          (N_GPS_SV + N_GLO_FREQ)

/* The range of BDS PRNs supported */ 
#define C_FIRST_BDS_D2_SV_PRN 1 
#define C_LAST_BDS_D2_SV_PRN 5
 
/* Number of potential BDS D2 satellites */ 
#define N_BDS_D2_SV (C_LAST_BDS_D2_SV_PRN - C_FIRST_BDS_D2_SV_PRN + 1)
 


/* GLO Signal Conversions */
#define GPS_UTC_OFFSET        (18U)             /* The leap second offset on
                                                  Jan-2017 */
#define GLO_UTC_SECS          (3U*3600U)        /* # of secs GLONASS is 
                                                  ahead of UTC (3 hrs) */
#define GLO_UTC_MSECS         (GLO_UTC_SECS*1000U)  /* # of msecs GLONASS is 
                                                  ahead of UTC (3 hrs) */
#define GLO_GPS_DAYS          (5839U)           /* # of days the GPS time 0 
                                                  preceeds GLONASS time 0 */
#define FOURYEAR_DAYS         (1461U)           /* # of days in 4-year cycle*/

#define MIN_SECS              (60U)             /* # of secs in one minute */
#define HOUR_MINS             (60U)             /* # of minutes in one hour */
#define HOUR_SECS             (3600U)           /* # of secs in one hour*/
#define DAY_HOURS             (24U)             /* # of hours in day */
#define DAY_SECS              (24U*HOUR_SECS)   /* # of secs in day */
#define DAY_MSECS             (DAY_SECS*1000U)  /* # of msecs in day */
#define FOURYEAR_SECS         (FOURYEAR_DAYS*DAY_SECS)   /* # of SEC in 4-year cycle*/
#define LEAPYEAR_DAYS         366U               /* # days in a leap year */
#define YEAR_DAYS             365U               /* # days in a year */
#define GPS_MINUTES_IN_WEEK       ((86400.0 * 7)/60)
#define GPS_SECONDS_IN_DAY        86400.0
#define GPS_SECONDS_IN_HOUR       3600.0
#define GPS_SECONDS_IN_MINUTE     60.0
#define DAYS_IN_ONE_WEEK          7
#define HOURS_IN_WEEK             (7 * 24)
#define NUM_DAYS_FRST_SIX_MTHS           181
#define NUM_DAYS_FRST_SIX_MTHS_LEAPYR    182
#define NUM_DAYS_LAST_SIX_MTHS           184
#define SECS_TO_MSECS             1000
#define SECS_TO_NSECS			  (1e9)
#define NSECS_TO_SECS               (1.0e-9)
#define MSECS_TO_SECS             0.001
#define MICRO_SEC_IN_MILLI_SEC    1000
#define CM_IN_METER               100
#define MSECS_TO_NSECS      (1e6)
#define NSECS_TO_MSECS       (0.000001)

/* Number of milliseconds between January 1, 1970 and January 6, 1980
  (3657 * 24 * 60 * 60 * 1000) */
#define GPS_TO_UTC_DELTA_MSECS       (315964800000ULL)

#define GPS_L2_FREQ_HZ (1200.0 * CA_FREQ) 
#define GPS_L5_FREQ_HZ (1150.0 * CA_FREQ) 
#define NAVIC_FREQ_HZ  (1150.0 * CA_FREQ) 
#define GAL_E5B_FREQ_HZ (1180.0 * CA_FREQ)

#define GLO_CA_CHIPS_MSEC  511   /* # of C/A chips per msec */
#define GLO_CA_PERIOD_MSEC 1     /* chip period in milliseconds */
#define GLO_CA_FREQ           (1000.0 * GLO_CA_CHIPS_MSEC)
#define DAY_BITS   (DAY_SECS*50U)               /* # of bits in day */
#define DAY_TKCOUNT (DAY_SECS/30U)              /* # of tk count in a day */
#define GLO_FRAME_SEC (30U)                     /* # of seconds in a Glonass 
                                                  frame */
#define GLO_L1_FREQ_HZ 1602000000              /* Glonass L1 in Hz used by RF 
                                                   driver during 
                                                   downconversion */
#define GLO_L2_FREQ_HZ 1246000000              /* Glonass L2 in Hz used by RF 
                                                   driver during 
                                                   downconversion */

#define C_GLO_STR_DATA_LEN          (100)   /*One Glo string can have 100 bits*/
#define C_TIMEMARK_MS               (300) /*Timemark duration is 300ms*/


#define C_MAX_GLO_SAT_SPEED_UNC C_MAX_SAT_SPEED_UNC

/*  Acquire & Track Glo svs in the following L1 Freq range -7,+6 
 *  Glo Based frquency @ -7 : 1598.0625. Frequency offset between 
 *  Glo svs is 562.5 Khz.
 */
#define	GLO_BASE_FREQ     (FLT) 1598.0625  /* Glo Freq -7 : 1598.0625 Mhz */
#define GLO_BASE_G2_FREQ  (FLT) 1242.9375  /* Glo Freq -7 : 1242.9375 Mhz */

#define	GLO_FREQ_OFFSET     (FLT) .5625  /*  .5625 Mhz */
#define GLO_FREQ_OFFSET_G2  (FLT) .4375  /*  .4375 MHz */

/*** GLO frequency double precision version definitions ***/
#define GLO_L1_FREQ_DBL(GLO_FREQ_INDEX)  ( ( 1598.0625  +  \
                                           ( (GLO_FREQ_INDEX - 1 ) * 0.5625  ) ) * 1000000.0 )
#define GLO_L1_WAVELENGTH_DBL(GLO_FREQ_INDEX) ( LIGHT_SEC / GLO_L1_FREQ_DBL(GLO_FREQ_INDEX) )

#define GLO_G2_FREQ_DBL(GLO_FREQ_INDEX)  ( ( 1242.9375 +  \
                                       ( (GLO_FREQ_INDEX - 1 ) * 0.4375 ) )    * 1000000.0 )
#define GLO_G2_WAVELENGTH_DBL(GLO_FREQ_INDEX) (FLT) ( LIGHT_SEC / GLO_G2_FREQ_DBL(GLO_FREQ_INDEX) )

/* Meters/Sec to L1 Hz and vice versa */
#define GLO_C_MsToL1Hz_DBL(GLO_FREQ_INDEX)	(1.0/GLO_L1_WAVELENGTH_DBL(GLO_FREQ_INDEX))
#define GLO_C_L1HzToMs_DBL(GLO_FREQ_INDEX) 	(GLO_L1_WAVELENGTH_DBL(GLO_FREQ_INDEX))

#define GLO_C_MpsToG2Hz_DBL(GLO_FREQ_INDEX) (FLT) (1.0/GLO_G2_WAVELENGTH_DBL(GLO_FREQ_INDEX))
#define GLO_C_G2HzToMps_DBL(GLO_FREQ_INDEX) (FLT) (GLO_G2_WAVELENGTH_DBL(GLO_FREQ_INDEX))
/*** End of double precision verion definitions ***/

#define GLO_L1_FREQ(GLO_FREQ_INDEX)  ( (FLT) ( GLO_BASE_FREQ +  \
                                               ( (FLT)(GLO_FREQ_INDEX - 1 ) * (FLT) GLO_FREQ_OFFSET ) ) \
                                                * 1000000.0 )
#define GLO_L1_WAVELENGTH(GLO_FREQ_INDEX) (FLT) ( LIGHT_SEC / GLO_L1_FREQ(GLO_FREQ_INDEX) )

#define GLO_G2_FREQ(GLO_FREQ_INDEX)  ( (FLT) ( GLO_BASE_G2_FREQ +  \
                                               ( (FLT)(GLO_FREQ_INDEX - 1 ) * (FLT) GLO_FREQ_OFFSET_G2 ) ) \
                                                * 1000000.0 )
#define GLO_G2_WAVELENGTH(GLO_FREQ_INDEX) (FLT) ( LIGHT_SEC / GLO_G2_FREQ(GLO_FREQ_INDEX) )

/* Meters/Sec to L1 Hz and vice versa */
#define GLO_C_MsToL1Hz(GLO_FREQ_INDEX)  (FLT) (1.0/GLO_L1_WAVELENGTH(GLO_FREQ_INDEX))
#define GLO_C_L1HzToMs(GLO_FREQ_INDEX)  (FLT) (GLO_L1_WAVELENGTH(GLO_FREQ_INDEX))

#define GLO_C_MpsToG2Hz(GLO_FREQ_INDEX) (FLT) (1.0/GLO_G2_WAVELENGTH(GLO_FREQ_INDEX))
#define GLO_C_G2HzToMps(GLO_FREQ_INDEX) (FLT) (GLO_G2_WAVELENGTH(GLO_FREQ_INDEX))

/* BDS Signal Conversions */
#define BDS_CA_CHIPS_MSEC      2046  /* # of C/A chips per msec */
#define BDS_CA_PERIOD_MSEC     1     /* chip period in milliseconds */
#define BDS_CA_FREQ           (1000.0 * BDS_CA_CHIPS_MSEC)
#define B1_FREQ               (763.0 * BDS_CA_FREQ)
#define B1C_FREQ              (770.0 * BDS_CA_FREQ) /* 1575.42 MHz */
#define B2_FREQ               (590.0 * BDS_CA_FREQ)
#define B2A_FREQ              (575.0 * BDS_CA_FREQ)
#define B2_FREQ_HZ             B2_FREQ
#define B2A_FREQ_HZ           (575.0 * BDS_CA_FREQ)
#define B2B_FREQ              (590.0 * BDS_CA_FREQ) /* 1207.14 MHz */

#define B1_WAVELENGTH           (LIGHT_SEC / B1_FREQ)
#define B2_WAVELENGTH           ( LIGHT_SEC /B2_FREQ )

#define BDS_B2A_CA_PERIOD_MSEC  1    /* PRN period in milliseconds */
#define B2A_WAVELENGTH          ( LIGHT_SEC /B2A_FREQ_HZ )

/* Meters/Sec to B1 Hz and vice versa */
#define C_MsToB1Hz      (FLT)(1.0/B1_WAVELENGTH)
#define C_B1HzToMs      (FLT)(B1_WAVELENGTH)

#define C_MsToB2Hz      (FLT)(1.0/B2_WAVELENGTH)
#define C_B2HzToMs      (FLT)(B2_WAVELENGTH)

#define C_MsToB2aHz     (FLT)(1.0/B2A_WAVELENGTH)
#define C_B2aHzToMs     (FLT)(B2A_WAVELENGTH)

#define C_MAX_BDS_SAT_SPEED_UNC C_MAX_SAT_SPEED_UNC

#define GPS_BDS_OFFSET_WEEK_SECS  14  /* difference in secs-in-a week between BDS and GPS
                                        GPS_BDS_OFFSET_WEEK_SECS = GPS_week_secs - BDS_week_secs */
#define GPS_BDS_DAYS              (9492U) /* number of days that GPS is ahead of BDS */
#define GPS_BDS_WEEKS             (GPS_BDS_DAYS/DAYS_IN_ONE_WEEK) /* number of weeks that
                                        GPS is ahead of BDS */
#define BDS_GPS_ADVANCE_SECS      (GPS_BDS_WEEKS * WEEK_SECS) +  GPS_BDS_OFFSET_WEEK_SECS
                                  /* number of secs that GPS is ahead of BDS */
#define GPS_GAL_WEEKS  (1024) /* number of weeks that GPS is ahead of GAL */

/* Galileo Signal */
#define GAL_CA_CHIPS_MSEC   1023 /* # of PRN chips per msec */
#define GAL_CA_PERIOD_MSEC  4    /* PRN period in milliseconds */
#define GAL_E5A_CA_PERIOD_MSEC  1    /* PRN period in milliseconds */

/*Navic Signal */
#define NAVIC_CA_CHIPS_MSEC 1023 /* # of PRN chips per msec */
#define NAVIC_CA_PERIOD_MSEC 1 /* PRN period in milliseconds */
#define C_NAVIC_GROUPDELAY_CX1CX2 (0.0)
#define NAVIC_CA_FREQ           (1000.0 * NAVIC_CA_CHIPS_MSEC)
#define NAVIC_FREQ               (1150.0 * NAVIC_CA_FREQ)


/* The range of NAVIC GEO PRNs supported */ //hm_todo
#define C_FIRST_NAVIC_GEO_SV_PRN 0 
#define C_LAST_NAVIC_GEO_SV_PRN 0

#define C_MAX_GAL_SAT_SPEED_UNC C_MAX_SAT_SPEED_UNC /*!< Line of sight, 1 sided m/sec */

/* The ICD-GPS-200 document provides these earth constants to convert from
 * the ECEF coordiante system into the WGS-84 lat, long, alt system.
*/
#define C_WGS84_A   6378137.000   /* Semi-major axis (m). (to equator)*/
#define C_WGS84_B   6356752.314   /* Semi-minor axis (m). (to poles)  */

/* Earth's obliquity and flatness terms. */
#define C_WGS84_F   (C_WGS84_A - C_WGS84_B) / C_WGS84_A
#define C_WGS84_1MF (1.0 - C_WGS84_F)
#define C_WGS84_R2  ((C_WGS84_B * C_WGS84_B) / (C_WGS84_A * C_WGS84_A))
#define C_WGS84_E2  (1.0 - (C_WGS84_B * C_WGS84_B) / (C_WGS84_A * C_WGS84_A))

/* World Geodetic Survey, 1984 - Earth's rotation rate (rad/sec). */		
/* Used by both cd and nf */
#define C_WGS84_ROTATION           7.2921151467e-5
#define C_CGCS2000_EARTH_ROTATION  7.292115e-5  /* defined for BDS */

/* mu, earth's gravitational constant, */
#define C_SQRT_EARTH_GRAV_CON         ((DBL)1.996498184321739e7)
#define C_CGCS200_SQRT_EARTH_GRAV_CON ((DBL)1.996498038566530e7) /* defined for BDS and GAL */

/* GPS relativistic clock correction constant. */
#define C_GPS_F_RELATIVITY       ((DBL) -4.442807633e-10)

/* some PZ90 Earth reference ellipsoide constants (PZ-90.11) */
#define C_PZ90A            ((DBL)6378136.0)   /* the semi-major axis in meters*/
#define C_PZ90B            ((DBL)6356751.3618)  /* Semi-minor axis (m). (to poles) */
#define C_PZ90MIU          ((DBL)398600.4418e+009) /* Gravitational constant in
                                                     meters^3 / second^2 */
#define C_PZ90OMEGADOTE    (DBL)7.292115e-5 /* Earth rotation rate, rad/s */
#define C_PZ90C20          ((DBL)-1.08262575e-3) /* 2nd zonal harmonic of
                                                     geopotential J20 */

/* GAL constants */
#define C_GAL_NOMINAL_SQRTA_METERS  5440.588203494177338011974 /* Nominal semi-major axis according to GAL ICD 29600 km */
#define C_GAL_NOMINAL_I0            0.977384381116825 /* Nominal inclination according to GAL ICD 56 degrees */
#define GRAVITY_MPSS_SQ             9.80665*9.80665 // accel due to gravity at sea level squared

/* default leap second pre and post guard period */
#define  C_LEAPSEC_PRE_GUARD_TIME_SEC     (3)  /* SEC*/
#define  C_LEAPSEC_POST_GUARD_TIME_SEC    (3)  /* SEC*/
#define  C_LEAPSEC_UTC_UNKNOWN_PRE_GUARD_TIME_SEC     (17)  /* SEC*/
#define  C_LEAPSEC_UTC_UNKNOWN_POST_GUARD_TIME_SEC    (19)  /* SEC*/

/*NavIC constants*/
#define N_NAVIC_SV            (C_LAST_NAVIC_SV_PRN - C_FIRST_NAVIC_SV_PRN + 1)
#define N_ACTIVE_NAVIC_CHAN    N_NAVIC_SV
#define C_MIN_NAVIC_WEEK      (1005)
#define C_MAX_NAVIC_WEEK      (32768) /*maps to 26 Aug 2627*/
#define C_FIRST_NAVIC_AUTONAV_IODEC       (76)
#define C_LAST_NAVIC_AUTONAV_IODEC        (159)
#define C_FIRST_NAVIC_FREQUENT_UPDATE_IODEC (160)
#define C_NAVIC_NUM_SRC_TIME_CONV         (3) /*NPLI currently not supported in XTRA*/

typedef enum
{
  MGP_GNSS_TYPE_GPS = 0,
  MGP_GNSS_TYPE_GLO,
  MGP_GNSS_TYPE_BDS,
  MGP_GNSS_TYPE_GAL,
  MGP_GNSS_TYPE_NAVIC,
  MGP_GNSS_TYPE_TDP,
  MGP_GNSS_TYPE_SBAS,
  MGP_GNSS_TYPE_QZSS,
  MGP_GNSS_TYPE_ALL = 255
} mgp_GnssTypeVal;
typedef uint8 mgp_GnssType;

typedef enum
{
  GNSS_DTM_WGS84 = 0,
  GNSS_DTM_PZ9011,
  /* Please insert new datum here */
  GNSS_DTM_MAX
} gnss_DatumEnumType;

typedef enum
{
  C_MGP_GNSS_SIG_TYPE_GPS_L1CA, /* GPS including QZSS */
  C_MGP_GNSS_SIG_TYPE_GPS_L2C, /* GPS including QZSS */
  C_MGP_GNSS_SIG_TYPE_GPS_L5, /* GPS including QZSS */
  C_MGP_GNSS_SIG_TYPE_GLO_G1,
  C_MGP_GNSS_SIG_TYPE_GLO_G2,
  C_MGP_GNSS_SIG_TYPE_BDS_B1I,
  C_MGP_GNSS_SIG_TYPE_BDS_B1C,
  C_MGP_GNSS_SIG_TYPE_BDS_B2A,
  C_MGP_GNSS_SIG_TYPE_GAL_E1,
  C_MGP_GNSS_SIG_TYPE_GAL_E5A,
  C_MGP_GNSS_SIG_TYPE_GAL_E5B,
  C_MGP_GNSS_SIG_TYPE_NAVIC_L5CA,
  C_MGP_GNSS_SIG_TYPE_LAST_VAL = C_MGP_GNSS_SIG_TYPE_NAVIC_L5CA,
  C_MGP_GNSS_SIG_TYPE_MAX = 255
} mgp_GnssSigTypeEnumVal;
typedef uint8 mgp_GnssSigTypeEnumType;

#endif /* GNSS_CONSTS_H */


