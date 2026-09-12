/*!
  @file
  tcxomgr.h

  @brief
  TCXOMgr external main header file. 
 
*/

/*===========================================================================

  Copyright (c) 2016 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document
  are confidential and proprietary information of Qualcomm Technologies
  Incorporated and all rights therein are expressly reserved.  By accepting
  this material the recipient agrees that this material and the information
  contained therein are held in confidence and in trust and will not be used,
  copied, reproduced in whole or in part, nor its contents revealed in any
  manner to others without the express written permission of Qualcomm
  Technologies Incorporated.

===========================================================================*/

/*============================================================================

$Header: //components/rel/mcs.mpss/5.2/api/tcxomgr.h#3 $

============================================================================*/

#ifndef TCXOMGR_H
#define TCXOMGR_H


/*============================================================================

                           INCLUDE FILES FOR MODULE

============================================================================*/

#include "rex.h"
#include "qw.h"
#include "timetick.h"

/*============================================================================

                        TCXO Manager client interface

============================================================================*/

/* Maximum number of frequency entries per client in AFC log packet.  */
#define TCXOMGR_MAX_FREQ_ENTRIES 8

#define TCXOMGR_MAX_NUM_CARRIERS 3
  /* The maximum number of carriers */

  
/* therm definitions, therm bit position for therms should be according
   to the sequence the therm enum is defined in 'tcxomgr_adc_client.h' */
#define TCXOMGR_NO_THERM_MASK           (uint32) 0x00000000
#define TCXOMGR_NONDEFER_MASK           (uint32) 0x80000000
/* All therms excluding non-defer mask */
#define TCXOMGR_ALL_THERM_MASK          (uint32) (0xFFFFFFFF & (~TCXOMGR_NONDEFER_MASK))
#define TCXOMGR_PMIC_THERM_MASK         (uint32) 0x1
#define TCXOMGR_XO_ACCURATE_THERM_MASK  (uint32) 0x2
#define TCXOMGR_XO_COARSE_THERM_MASK    (uint32) 0x4
#define TCXOMGR_PA_THERM_MASK           (uint32) 0x8
#define TCXOMGR_MSM_THERM_MASK          (uint32) 0x10

/* therm masks for GPS use */
#define TCXOMGR_GPS_ON_THERM_MASK       TCXOMGR_PMIC_THERM_MASK | TCXOMGR_XO_ACCURATE_THERM_MASK
#define TCXOMGR_GPS_OFF_THERM_MASK      TCXOMGR_XO_COARSE_THERM_MASK
#define TCXOMGR_GPS_APQ_THERM_MASK      TCXOMGR_MSM_THERM_MASK


/* Means "tcxomgr_temp_type" temperature is invalid or unavailable. */
#define TCXOMGR_INVALID_TEMP      ((tcxomgr_temp_type)0x80000000)

/*set specific bit(s) of a 32 bit unsigned flag using mask*/
#define  SET_BIT_MASK(flag, mask) (flag |= (mask)) 

/*set specific bit of a 32 bit unsigned flag using bit position*/
#define  SET_BIT_POS(flag, pos) (flag |= ((uint32)(0x1<<pos))) 

/*clear specific bit(s) of a 32 bit unsigned flag using mask*/
#define  CLR_BIT_MASK(flag, mask) ( flag &= (~mask)) 

/*clear specific bit of a 32 bit unsigned flag using bit position*/
#define  CLR_BIT_POS(flag, pos) ( flag &= ( ~((uint32)(0x1<<pos)) )) 

/*check specific bit(s) of a 32 bit unsigned flag using mask*/
#define  CHECK_BIT_MASK(flag, mask) ( (flag) & (mask)) 

/*check specific bit of a 32 bit unsigned flag using bit position*/
#define  CHECK_BIT_POS(flag, pos) ( (flag) & ((uint32)(0x1<<pos))) 

/*---------------------------------------------------------------------------
  Simple scalar types
---------------------------------------------------------------------------*/

/* VCTCXO value type
   Sensitivity: 2^-20 PPM per LSB */
typedef int32                     tcxomgr_vco_type;


/* Rotator value type
   Sensitivity: 2^-10 PPM per LSB */
typedef int32                     tcxomgr_rotator_type;

/* PPM value type
   Sensitivity: 2^-10 PPM per LSB */
typedef int32                     tcxomgr_ppm_type;

/* High Resolution PPM value type
   Sensitivity: 2^-12 PPM per LSB */
typedef int32                     tcxomgr_hr_ppm_type;

/* Frequency value type
   Sensitivity: 1 Hz per LSB */
typedef int32                     tcxomgr_hz_type;

/* Temperature value type
   Sensitivity: 2^-10 deg C per LSB */
typedef int32                     tcxomgr_temp_type;

/* Search space for WWANs
   Sensitivity: PPM  */
typedef uint8                     tcxomgr_uncertainty_type;

/* Search space for GPS
   Sensitivity: PPB  */
typedef uint32                    tcxomgr_ppb_uncertainty_type;


/* Derivative of the FT curve at a given temp
   Sensitivity: HRPPB/C */
typedef uint32                    tcxomgr_slope_uncertainity_type;


typedef struct
{
  tcxomgr_slope_uncertainity_type  wwan_slope_uncertainty; /* WWAN Coef */
  tcxomgr_slope_uncertainity_type  gps_slope_uncertainty;  /* GPS Coef  */

}tcxomgr_ext_slope_uncertainty_type;


typedef struct
{
  tcxomgr_temp_type          xo;                 /* XTAL temp
                                                    (see below for units)    */
  tcxomgr_temp_type          pmic;               /* 2^-10 C, PMIC temp       */
  tcxomgr_temp_type          pa;                 /* 1 C,     PA temp         */
} tcxomgr_temp_set_type;

/* XO temp is in units of 2^-10 degC */
#define TCXOMGR_XO_TEMP_TO_DEGC( temp )  ( (temp) >> 10 )

/*---------------------------------------------------------------------------
  Rpush Request Priority mode enum
---------------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_RPUSH_MODE_LOW,
  TCXOMGR_RPUSH_MODE_HIGH,
  TCXOMGR_RPUSH_MODE_OFF
}tcxomgr_rpush_req_mode_enum;

/*---------------------------------------------------------------------------
  Client ID's
---------------------------------------------------------------------------*/

/* The number of clients must not exceed the number of bits in
   tcxomgr.rpush_mask (32). If it does, rpush_mask's size will need
   to be increased */
typedef enum
{
  TCXOMGR_CLIENT_NONE,
  TCXOMGR_CLIENT_GPS,
  TCXOMGR_CLIENT_BT,
  TCXOMGR_CLIENT_MEDIA_FLO,
  TCXOMGR_CLIENT_GSM,
  TCXOMGR_CLIENT_WCDMA,
  TCXOMGR_CLIENT_CDMA_1X,
  TCXOMGR_CLIENT_CDMA_HDR,
  TCXOMGR_CLIENT_PPM,
  TCXOMGR_CLIENT_TV_OUT,
  TCXOMGR_CLIENT_WLAN,
  TCXOMGR_CLIENT_UBM_L1,
  TCXOMGR_CLIENT_FREQ_EST,
  TCXOMGR_CLIENT_LTE,
  TCXOMGR_CLIENT_TDSCDMA,
  TCXOMGR_CLIENT_IFC,
  TCXOMGR_NUM_CLIENTS
} tcxomgr_client_id_type;
/*~ SENTINEL tcxomgr_client_id_type.TCXOMGR_NUM_CLIENTS */

/*---------------------------------------------------------------------------
  Priorities
---------------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_NO_PRIORITY,
  TCXOMGR_DEFAULT_PRIORITY,
  TCXOMGR_UBM_L1,
  TCXOMGR_1X_OL,
  TCXOMGR_HDR_OL,
  TCXOMGR_1X_CL,
  TCXOMGR_HDR_CL,
  TCXOMGR_GSM,
  TCXOMGR_WCDMA,
  TCXOMGR_LTE,
  TCXOMGR_TDSCDMA,
  TCXOMGR_HIGHEST_PRIORITY,
  TCXOMGR_MAX_PRIORITY
} tcxomgr_priority_type;

/*---------------------------------------------------------------------------
  Source Type
---------------------------------------------------------------------------*/

/* List of possible vco sources */
typedef enum
{
  TCXOMGR_SOURCE_NONE,           /* no control of TCXO */
  TCXOMGR_SOURCE_OVERRIDE,       /* control of TCXO using override reg */
  TCXOMGR_SOURCE_1X_DSP,         /* control of TCXO using 1X DSP */
  TCXOMGR_SOURCE_HDR_DSP,        /* control of TCXO using HDR DSP */
  TCXOMGR_SOURCE_UMTS_DSP,       /* control of TCXO using UMTS DSP */
  TCXOMGR_SOURCE_POWER_OFF,      /* turn off TCXO */
  TCXOMGR_MAX_SOURCE
} tcxomgr_source_type;

/*---------------------------------------------------------------------------
  Restriction Type
---------------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_RESTRICTION_NONE,    /* TCXO not restricted, anything goes */
  TCXOMGR_RESTRICTION_FAST,    /* TCXO restr to fast closed loop ctrl */
  TCXOMGR_RESTRICTION_SLOW,    /* TCXO restr to slow closed loop ctrl */
  TCXOMGR_RESTRICTION_SRL,     /* TCXO restr to SRL closed loop ctrl */
  TCXOMGR_RESTRICTION_FROZEN,  /* TCXO restr to be frozen */
  TCXOMGR_RESTRICTION_XO,      /* TCXO restr irrelevant, XO target (frozen) */
  TCXOMGR_MAX_RESTRICTION
} tcxomgr_restriction_type;


/*---------------------------------------------------------------------------
  State Type (used in rotator updates)
---------------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_STATE_IDLE,
  TCXOMGR_STATE_TRAFFIC,
  TCXOMGR_STATE_NO_INFO,
  TCXOMGR_NUM_STATES
} tcxomgr_client_state_type;
/*~ SENTINEL tcxomgr_client_state_type.TCXOMGR_NUM_STATES */

/*----------------------------------------------------------------------*
  TCXOmgr frequency tracking modes in AFC log packet
 *----------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_VCO_ONLY_TRK_MODE,
  TCXOMGR_FIXED_BW_RVAFC_TRK_MODE,
  TCXOMGR_VAR_BW_RVAFC_TRK_MODE,
  TCXOMGR_ROT_ONLY_TRK_MODE
} tcxomgr_log_freq_track_mode_enum_type;

/*----------------------------------------------------------------------*
  TCXOmgr gain modes in AFC log packet
 *----------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_ACQ_GAIN_MODE,
  TCXOMGR_TRK_GAIN_MODE
} tcxomgr_log_gain_mode_enum_type;

/*----------------------------------------------------------------------*
  TCXOmgr TRK_LO polarity
 *----------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_TRK_LO_POLARITY_NORMAL,
  TCXOMGR_TRK_LO_POLARITY_INVERTED
} tcxomgr_trk_lo_enum_type;

/*----------------------------------------------------------------------*
  TCXOmgr TRK_LO polarity return type
 *----------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_TRK_LO_POLARITY_SUCCESS,    /* Polarity & override value updated */
  TCXOMGR_TRK_LO_POLARITY_INVALID,    /* Polarity is invalid  */
  TCXOMGR_TRK_LO_PDM_VALUE_UNCHANGED  /* Polarity updated, value unchanged */
} tcxomgr_trk_lo_return_enum_type;

/*----------------------------------------------------------------------*
  TCXOmgr Access Stratum Identifiers (AS_ID)

  If a client does not have an associated AS_ID, it should use
  TCXOMGR_AS_ID_NONE as AS_ID.
 *----------------------------------------------------------------------*/

typedef enum
{
  TCXOMGR_AS_ID_NONE = -1,
  TCXOMGR_AS_ID_1,
  TCXOMGR_AS_ID_2,
  TCXOMGR_AS_ID_3,
  TCXOMGR_AS_ID_MAX
} tcxomgr_as_id_enum_type;

/*----------------------------------------------------------------------*
  TCXOmgr Client Info used to identify a particular client.
 *----------------------------------------------------------------------*/

typedef struct
{
  /* Client Tech Identifier */
  tcxomgr_client_id_type     id;

  /* Access Stratum Identifier (AS_ID) of the client */
  tcxomgr_as_id_enum_type    as_id;

} tcxomgr_client_info_struct_type;

/*----------------------------------------------------------------------*
  TCXOmgr Structure used by TCXOMGR to report Rpush to clients registering
  for Rpush callback.
 *----------------------------------------------------------------------*/

typedef struct
{
  tcxomgr_client_info_struct_type client_info;
  tcxomgr_vco_type                vco_val;
  tcxomgr_rotator_type            rot_val;
  uint32                          num_samples;
  timetick_type                   begin_acq_time;
  timetick_type                   end_acq_time;
  int32                           temp;
  tcxomgr_client_state_type       state;
} tcxomgr_rpush_cb_data_type;

/*----------------------------------------------------------------------*
  TCXOmgr Callback function type for Rpush reporting.
 *----------------------------------------------------------------------*/

typedef void (*tcxomgr_rpush_cb_func_type)
(
  tcxomgr_rpush_cb_data_type      rpush_data
);

/*----------------------------------------------------------------------*
  TCXOmgr structure used as input to tcxomgr_register_rpush_cb
 *----------------------------------------------------------------------*/

typedef struct
{
  /* Information about the client registering for Rpush */
  tcxomgr_client_info_struct_type client_info;

  /* Function pointer to be used for callback */
  tcxomgr_rpush_cb_func_type      func_ptr;

} tcxomgr_rpush_cb_registration_type;

/*----------------------------------------------------------------------*
  TCXOmgr structure used to denote the Rpush mode information.
 *----------------------------------------------------------------------*/
typedef struct
{
  tcxomgr_rpush_req_mode_enum     mode;

} tcxomgr_rpush_mode_cb_data_type;

/*----------------------------------------------------------------------*
  Callback function type for Rpush Mode reporting.
 *----------------------------------------------------------------------*/

typedef void (*tcxomgr_rpush_mode_cb_func_type)
(
  tcxomgr_rpush_mode_cb_data_type    mode_info
);

/*----------------------------------------------------------------------*
  TCXOmgr structure used as input to tcxomgr_register_rpush_mode_cb
 *----------------------------------------------------------------------*/

typedef struct
{
  /* Information about the client registering for Rpush */
  tcxomgr_client_info_struct_type client_info;

  /* Function pointer to be used for callback */
  tcxomgr_rpush_mode_cb_func_type func_ptr;

} tcxomgr_rpush_mode_cb_registration_type;

/*----------------------------------------------------------------------*
  TCXOmgr structure to be used by clients to report Rpush to TCXOMGR.
 *----------------------------------------------------------------------*/
typedef struct
{
  tcxomgr_client_info_struct_type client_info;
  tcxomgr_rotator_type            rot_val;
  uint32                          num_samples;
  timetick_type                   begin_acq_time;
  timetick_type                   end_acq_time;
  tcxomgr_client_state_type       state;
} tcxomgr_rpush_data_type;

/*---------------------------------------------------------------------------
  Frequency Entries (AFC logging)
---------------------------------------------------------------------------*/

typedef PACKED struct
{
  /* vco accumulator value */
  uint16   vco_acc;

  /* rotator accumulator value */
  uint16   rot_acc;

  /* delta time */
  uint16   delta_time;

  /* signal strength indicator */
  uint16   signal_strength;

} tcxomgr_log_entries_struct_type;

/*---------------------------------------------------------------------------
  TCXO Manager Crystal Types
---------------------------------------------------------------------------*/

typedef enum
{
  XO_CRYSTAL,
  TCXO_CRYSTAL,
  VCTCXO_CRYSTAL,
  NO_CRYSTAL
} tcxomgr_crystal_type;

/*----------------------------------------------------------------------*
  TCXOmgr temp read subscribers enum 
  NOTE: Any addition/change within this enum also affects the temperature
  subscription log packet (0x1755). We should make sure that the log
  packet is in sync with the enum definition.
 *----------------------------------------------------------------------*/
typedef enum 
{
  /* NOTE: For GPS disabled build flavors, TCXOMGR re-uses client GPS to 
     register for temperature reads. That will ensure that its reading 
     the temperature, even if GPS frequency estimation is not registering
     for the same. */
  TCXOMGR_TEMP_READ_CLIENT_GPS,
  TCXOMGR_TEMP_READ_CLIENT_IFC,
  TCXOMGR_TEMP_READ_CLIENT_RESERVED,
  /* chipxn compensation is made last subscriber to make its call back
     function called at last. We make this because the cb function does
     more things than just copying the temp readings and takes time */
  TCXOMGR_TEMP_READ_CLIENT_CHIPXN_COMP,
  TCXOMGR_TEMP_READ_CLIENT_MAX
}tcxomgr_temp_read_subscriber_enum;

/*---------------------------------------------------------------------------
  Header Type (AFC logging)
---------------------------------------------------------------------------*/

typedef PACKED struct
{
  /* Client that is logging the AFC log */
  uint8    logging_client_id;

  /* Client that is controlling the VcTcxo */
  uint8    ctrl_client_id;

  /* Reserved field */
  uint8    reserved1;

  /* system time of first entry */
  qword    timestamp;

  /* frequency tracking mode - uses  tcxomgr_log_freq_track_mode_enum_type */
  uint8    afc_mode;

  /* frequency loop gain - uses tcxomgr_log_gain_mode_enum_type */
  uint8    afc_gain;

  /* SRL indicator */
  uint8    srl_indicator;

  /* Finger cross mode */
  uint8    cross_mode;

  /* rf band class */
  uint8    band;

  /* Reserved field */
  uint8   reserved;

  /* rf cdma channel */
  uint16   chan;

  /* Carrier frequency */
  uint32   carrier_freq;

  /* vco slope */
  uint16   vco_slope;

  /* Thermistor reading degC */
  uint16   therm;

  /* number of AFC entries */
  uint32   num_freq_entry;

} tcxomgr_log_header_struct_type;

/*---------------------------------------------------------------------------
  AFC log structure
---------------------------------------------------------------------------*/

typedef PACKED struct
{
  tcxomgr_log_header_struct_type      afc_header_info;

  tcxomgr_log_entries_struct_type     entries[TCXOMGR_MAX_FREQ_ENTRIES];

} tcxomgr_log_struct_type;

/*---------------------------------------------------------------------------
  Multi-carrier Frequency Entries (AFC logging)
---------------------------------------------------------------------------*/

typedef PACKED struct
{
  /* vco accumulator value */
  uint16   vco_acc;

  /* delta time */
  uint16   delta_time;

  /* signal strength indicator */
  uint16   signal_strength;

  /* rotator accumulator value */
  uint16   rot_acc[TCXOMGR_MAX_NUM_CARRIERS];

} tcxomgr_mc_log_entries_struct_type;

/*---------------------------------------------------------------------------
  Carrier Entries (AFC logging)
---------------------------------------------------------------------------*/

typedef PACKED struct
{
  /* rf band class */
  uint8    band;

  /* Reserved field */
  uint8    reserved1;

  /* rf cdma channel */
  uint16   chan;

  /* Carrier frequency */
  uint32   carrier_freq;

} tcxomgr_mc_log_carrier_struct_type;

typedef PACKED struct
{
  tcxomgr_temp_type          xo;                 /* 2^-10 C, XTAL temp       */
  tcxomgr_temp_type          pmic;               /* 2^-10 C, PMIC temp       */
  tcxomgr_temp_type          pa;                 /* 1 C,     PA temp         */
} tcxomgr_mc_log_temp_set_type;

/*---------------------------------------------------------------------------
  Header Type (AFC logging)
---------------------------------------------------------------------------*/

typedef PACKED struct
{

  /* Client that is logging the AFC log */
  uint8    logging_client_id;

  /* Client that is controlling the VcTcxo */
  uint8    ctrl_client_id;

  /* Carrier mask */
  uint8    carrier_mask;

  /* system time of first entry */
  qword    timestamp;

  /* frequency tracking mode - uses  tcxomgr_log_freq_track_mode_enum_type */
  uint8    afc_mode;

  /* frequency loop gain - uses tcxomgr_log_gain_mode_enum_type */
  uint8    afc_gain;

  /* SRL indicator */
  uint8    srl_indicator;

  /* Finger cross mode */
  uint8    cross_mode;

  /* vco slope */
  uint16   vco_slope;

  /* Thermistor reading degC */
  tcxomgr_mc_log_temp_set_type  therm;

  /* number of AFC entries */
  uint32   num_freq_entry;

} tcxomgr_mc_log_header_struct_type;

/*---------------------------------------------------------------------------
  MC AFC log structure
---------------------------------------------------------------------------*/

typedef PACKED struct
{
  tcxomgr_mc_log_header_struct_type      afc_header_info;

  /* carrier info */
  tcxomgr_mc_log_carrier_struct_type     carrier[TCXOMGR_MAX_NUM_CARRIERS];

  tcxomgr_mc_log_entries_struct_type     entries[TCXOMGR_MAX_FREQ_ENTRIES];

} tcxomgr_mc_log_struct_type;

/*---------------------------------------------------------------------------
  Client struct
---------------------------------------------------------------------------*/

typedef struct
{
  tcxomgr_client_id_type            id;
  tcxomgr_priority_type             priority;
  char                              name[10];
  tcxomgr_source_type               source;
  tcxomgr_restriction_type          restriction;
  boolean                           resp_req;
  tcxomgr_vco_type                (*vco_cb)        (void);
  void                            (*grant_cb)      (tcxomgr_source_type,
                                                    tcxomgr_restriction_type,
                                                    tcxomgr_vco_type);
  void                            (*change_cb)     (tcxomgr_source_type,
                                                    tcxomgr_restriction_type);
  void                            (*release_cb)    (void);
  void                            (*deny_cb)       (void);
  void                            (*notify_cb)     (tcxomgr_restriction_type);
  void                            (*rpush_flag_cb) (boolean);
  void                            (*rpush_cb)      (tcxomgr_client_id_type,
                                                    tcxomgr_vco_type,
                                                    tcxomgr_rotator_type,
                                                    uint32,
                                                    timetick_type,
                                                    timetick_type,
                                                    int32,
                                                    tcxomgr_client_state_type);
  tcxomgr_rotator_type            (*rot_cb)        (void);
} tcxomgr_client_type;

/*---------------------------------------------------------------------------
  Freq/Temp information Accumulator indicator values
---------------------------------------------------------------------------*/
  /* Enum is order from worst to best uncertianty
     NOTE: the *_NOT_CAL, *_FAC1, *_FT#, *_FAILED enum values apply when
           Factory Calibration of the Freq Temp table/curve is performed */

typedef enum
{
  TCXOMGR_FT_QUAL_FAILED,     /* Fac Cal Failed, phone should have been
                                  failed in the Factory
                                  56 PPM mode                                */
  TCXOMGR_FT_QUAL_NOT_CAL,    /* Factory Calibration has not been performed
                                  56 PPM mode                                */
  TCXOMGR_FT_QUAL_FAC1,       /* Intermediate step of Factory Calibration
                                  56 PPM mode                                */
  TCXOMGR_VCO_DEFAULT,        /* Default value was returned
                                  5 PPM mode                                 */
  TCXOMGR_FT_QUAL_DEFAULT = TCXOMGR_VCO_DEFAULT,

  TCXOMGR_FT_QUAL_WIDE_BINS,  /* Value from Calibrated Curve at extreme temp
                                    so wider bins can be used for acq        */

  TCXOMGR_FT_QUAL_COARSE,     /* Coarse Cal has been completed               */

  TCXOMGR_FT_QUAL_IFC,        /* IFC computed C1                             */

  TCXOMGR_FT_QUAL_FINE,       /* Fine Cal has been completed                 */

  TCXOMGR_FT_QUAL_FT1,        /* Value from calibrated curve with default C0
                                  5 PPM mode                                 */
  TCXOMGR_VCO_OLD_RGS,        /* Value from a not-so-recent Good System
                                  3 PPM mode                                 */
  TCXOMGR_FT_QUAL_OLD_RGS = TCXOMGR_VCO_OLD_RGS,

  TCXOMGR_VCO_INTERPOLATED,   /* Value was interpolated from adj value(s)
                                  3 PPM mode                                 */
  TCXOMGR_FT_QUAL_INTERP = TCXOMGR_VCO_INTERPOLATED,

  TCXOMGR_FT_QUAL_FT2,        /* Value from calibrated curve with known C0
                                  3 PPM mode - c0 state                      */
  TCXOMGR_VCO_EXACT,          /* Value for temp was found in freq/temp table
                                  3 PPM mode                                 */
  TCXOMGR_FT_QUAL_EXACT = TCXOMGR_VCO_EXACT,

  TCXOMGR_VCO_RGS,            /* Value was from the Recent Good System
                                  3 PPM mode                                 */
  TCXOMGR_FT_QUAL_RGS = TCXOMGR_VCO_RGS,

  TCXOMGR_VCO_RGS_RECENT,     /*  RGS, but less than 30 secs old             */

  TCXOMGR_VCO_LAST            /* last value in the enum                      */
}
tcxomgr_vco_indicator_type;

/*----------------------------------------------------------------------*
  Tcxomgr RGS fields type
 *----------------------------------------------------------------------*/
typedef enum
{
  TCXOMGR_RGS_FIELD_ID     = 1,
  TCXOMGR_RGS_FIELD_VCO    = 2,
  TCXOMGR_RGS_FIELD_ROT    = 4,
  TCXOMGR_RGS_FIELD_STATE  = 8,
  TCXOMGR_RGS_FIELD_TIME   = 16,
  TCXOMGR_RGS_FIELD_AS_ID  = 32,
}tcxomgr_rgs_fields_populated;

/*---------------------------------------------------------------------------
  Freq/Temp information struct
---------------------------------------------------------------------------*/

typedef struct
{
  /* VCO Type for WWANs */
  tcxomgr_vco_indicator_type      vco_type;

  /* VCO value for WWANs on VCTCXO target */
  tcxomgr_vco_type                vco_value;

  /* Rotator value for WWANs  */
  tcxomgr_rotator_type            rot_value;

  /* Frequency uncertainity based on VCO type and current temp */
  tcxomgr_uncertainty_type        freq_uncertainty;

  /* Frequency uncertainity based on VCO type and current temp */
  tcxomgr_ppb_uncertainty_type    freq_uncertainty_ppb;

  /* VCO type derived for GPS */
  tcxomgr_vco_indicator_type      gps_vco_type;

  /* Rotator value for GPS  */
  tcxomgr_rotator_type            gps_rot_value;

  /* Frequency uncertainity for GPS based on VCO type and current temp */
  tcxomgr_uncertainty_type        gps_freq_uncertainty;

  /* Frequency uncertainity for GPS based on VCO type and current temp */
  tcxomgr_ppb_uncertainty_type    gps_freq_uncertainty_ppb;

  /* Technology that provided the frequency error if the type is set to RGS
     Tech is set to TCXOMGR_CLIENT_NONE if estimates are from FT curve */
  tcxomgr_client_id_type          id;

  /* Access Stratum Identifier (AS_ID). */
  tcxomgr_as_id_enum_type         as_id;

  /* Temperature difference from last RGS. Value will be 0 (zero) is vco_type
     is not TCXOMGR_VCO_RGS or TCXOMGR_VCO_OLD_RGS */
  int32                           temp_diff;

  /* Time difference from last RGS. Value will be 0 (zero) is vco_type
     is not TCXOMGR_VCO_RGS or TCXOMGR_VCO_OLD_RGS */
  uint32                          time_diff;

   /* The rgs_offset field denots the signed uncertainty due to difference between
     the RGS and the FT curve and the temperature change since the RGS was reported.

     Note: This value will be populated only when field calibration was done
     atleast once and when the vco type is TCXOMGR_VCO_RGS. If TCXOMGR does not have
     confidence on the FT curve coefficients or if vco type is not TCXOMGR_VCO_RGS,
     the value of rgs_offset will be set to 0 (Zero).*/
  tcxomgr_rotator_type            rgs_offset;

  /* VCO Type from the FT curve, not taking any RGS into account */
  tcxomgr_vco_indicator_type      ft_vco_type;

  /* Rotator value from the FT curve, not taking any RGS into account */
  tcxomgr_rotator_type            ft_rot_value;

  /* Frequency uncertainity based on VCO type and current temp from the FT
   * curve, not taking any RGS into account */
  tcxomgr_ppb_uncertainty_type    ft_freq_uncertainty_ppb;

} tcxomgr_vco_info_type;


/*---------------------------------------------------------------------------
  Rotator/VCO information struct
---------------------------------------------------------------------------*/
typedef struct
{
  /* Which of the fields are filled by L1s */
  uint32                     valid_fields;

  /* client doing the update */
  tcxomgr_client_id_type     id;

  /* VCTCXO accumulator value (TRK_LO_ADJ PDM value) */
  tcxomgr_vco_type           vco_val;

  /* Rotator accumulator (in 2^-10 ppm units) */
  tcxomgr_rotator_type       rot_val;

  /* is RGS from traffic ? */
  tcxomgr_client_state_type  state;

  /* System time           */
  uint64                     time;

  /* Access Stratum Identifier (AS_ID). */
  tcxomgr_as_id_enum_type    as_id;

} tcxomgr_rgs_info_type;


/*---------------------------------------------------------------------------
  Recent Good System Values
---------------------------------------------------------------------------*/

typedef struct
{
  tcxomgr_client_id_type      system;
  int16                       temp_diff;
  uint32                      age_secs;
  tcxomgr_vco_type            vco;
  tcxomgr_rotator_type        rot;
}
tcxomgr_rgs_vals_struct_type;

/*============================================================================

                            MACROS AND DEFINES

============================================================================*/

/* If the age in the RGS is invalid, set it to this value */
#define TCXOMGR_INVALID_RGS_AGE  (0xFFFFFFFFUL)

/* Full (1)   PPM units to 2^-10 PPM units conversion */
#define PPM_FULL_TO_PPM( x )          ( (x) << 10 )

/* Half (1/2) PPM units to 2^-10 PPM units conversion */
#define PPM_HALF_TO_PPM( x )          ( (x) << 9 )

/* Full (1)   PPM units to 2^-12 High Resolution PPM units conversion */
#define PPM_FULL_TO_HR_PPM( x )       ( (x) << 12 )

/* Half (1/2) PPM units to 2^-12 High Resolution PPM units conversion */
#define PPM_HALF_TO_HR_PPM( x )       ( (x) << 11 )

/* 2^-10 PPM units to and from 2^-12 High Resolution PPM units */
#define PPM_PPM_TO_HR_PPM( x )        ( (x) << 2 )
#define PPM_HR_PPM_TO_PPM( x )        ( (x) >> 2 )
/* function pointer for subscribing to temp reads
   arguments:
    temp structure for temp data 
    temp_channel- enum representing what temp channels were read/valid
         bit 0- pmic
         bit 1- xo accurate
         bit 2- xo coarse
         bit 3- pa therm
         bit 4- msm therm*/

typedef void (*tcxomgr_temp_read_subscribe_cb) 
(
 tcxomgr_temp_set_type  temp,
 uint32                 temp_channel
);


/*============================================================================

                            FUNCTION DECLARATIONS

============================================================================*/

extern void tcxomgr_taskinit( void );

extern void tcxomgr_get_vco_slope
(
  uint16 *vco_slope,

  uint8 *vco_range
);

extern tcxomgr_vco_info_type tcxomgr_get_stored_vco (void);

extern tcxomgr_vco_info_type tcxomgr_get_client_rgs (tcxomgr_client_info_struct_type client_info);

extern tcxomgr_vco_info_type tcxomgr_get_subscription_rgs (tcxomgr_as_id_enum_type as_id);

extern void tcxomgr_reset_client_rgs_values( tcxomgr_client_info_struct_type client_info );

extern tcxomgr_vco_info_type tcxomgr_get_stored_vco_aged(void);

extern tcxomgr_ext_slope_uncertainty_type tcxomgr_get_slope_uncertainity(void);

extern void tcxomgr_get_rgs_values
(
  tcxomgr_rgs_vals_struct_type *rgs
);

extern tcxomgr_vco_indicator_type tcxomgr_get_freq_err_temp
(
  tcxomgr_vco_type *vco 
);

extern tcxomgr_vco_type tcxomgr_get_default_freq_err(void);

extern tcxomgr_vco_indicator_type tcxomgr_get_uncertainty (void);

extern tcxomgr_vco_type tcxomgr_get_current_vco (void);

extern tcxomgr_rotator_type tcxomgr_get_current_rot (void);

extern void tcxomgr_update_rgs_values
(
  tcxomgr_rgs_info_type *rgs
);

extern void tcxomgr_reset_rgs_and_temp_table
(
  tcxomgr_client_id_type  client_id
);

extern void tcxomgr_set_priority
(
  tcxomgr_client_id_type    id,
  tcxomgr_priority_type     priority
);

extern void tcxomgr_change_source
(
  tcxomgr_client_id_type  id,
  tcxomgr_source_type     source
);

extern void tcxomgr_set_source
(
  tcxomgr_client_id_type  id,
  tcxomgr_source_type     source
);

extern void tcxomgr_set_callbacks
(
  tcxomgr_client_id_type  id,
  tcxomgr_vco_type (*vco_cb)    (void),
  void             (*grant_cb)  (tcxomgr_source_type, tcxomgr_restriction_type,
                                 tcxomgr_vco_type),
  void             (*change_cb) (tcxomgr_source_type, tcxomgr_restriction_type),
  void             (*release_cb)(void),
  void             (*deny_cb)   (void),
  void             (*notify_cb) (tcxomgr_restriction_type),
  void             (*rpush_flag_cb) (boolean),
  void             (*rpush_cb)      (tcxomgr_client_id_type,
                                     tcxomgr_vco_type,
                                     tcxomgr_rotator_type,
                                     uint32,
                                     timetick_type,
                                     timetick_type,
                                     int32,
                                     tcxomgr_client_state_type),
  tcxomgr_rotator_type (*rot_cb) (void)
);

extern void tcxomgr_set_notify_callback
(
  tcxomgr_client_id_type  id,
  void (*notify_cb) (tcxomgr_restriction_type)
);

extern void tcxomgr_rpush_set_flag
(
  tcxomgr_client_id_type    id,
  boolean                   rpush_flag
);

extern boolean tcxomgr_rpush_get_flag( void );

extern uint32 tcxomgr_rpush_get_mask( void );

extern void tcxomgr_rpush
(
  tcxomgr_client_id_type    id,
  tcxomgr_rotator_type      rot_val,
  uint32                    num_samples,
  timetick_type             begin_acq_time,
  timetick_type             end_acq_time,
  tcxomgr_client_state_type state
);

extern void tcxomgr_ack
(
  tcxomgr_client_id_type    id,
  tcxomgr_source_type       source,
  tcxomgr_restriction_type  restriction
);

extern void tcxomgr_release
(
  tcxomgr_client_id_type    id,
  tcxomgr_vco_type          curr_vco_val
);

extern void tcxomgr_report
(
  tcxomgr_client_id_type    id,
  tcxomgr_source_type       source,
  tcxomgr_restriction_type  restriction
);

extern void tcxomgr_set_restriction
(
  tcxomgr_client_id_type    id,
  tcxomgr_restriction_type  restriction
);

extern tcxomgr_restriction_type tcxomgr_get_restriction
(
  tcxomgr_client_id_type    id
);

extern void tcxomgr_flush_client_log
(
  tcxomgr_log_struct_type     *afc_log_ptr
);

extern void tcxomgr_log_entry_updated
(
  tcxomgr_log_struct_type     *afc_log_ptr
);

extern void tcxomgr_mc_flush_client_log
(
  tcxomgr_mc_log_struct_type     *afc_log_ptr
);

extern void tcxomgr_mc_log_entry_updated
(
  tcxomgr_mc_log_struct_type     *afc_log_ptr
);

extern tcxomgr_ppm_type tcxomgr_ft_eval
(
  tcxomgr_temp_set_type temp
);

extern tcxomgr_ppm_type tcxomgr_ft_eval_gps
(
  tcxomgr_temp_set_type temp
);

extern tcxomgr_ppm_type tcxomgr_ft_curr( void );

extern tcxomgr_ppm_type tcxomgr_ft_curr_blocking( void );

extern boolean tcxomgr_ft_valid( void );

boolean tcxomgr_get_slope_err
(
  int32 *slope,
  int32 *outer_accum,
  timetick_type *timestamp
);

void tcxomgr_get_time_and_temp
(
  uint32 *time,
  tcxomgr_temp_set_type *temp
);

extern tcxomgr_trk_lo_return_enum_type tcxomgr_set_polarity
(
  tcxomgr_trk_lo_enum_type    polarity
);

extern void tcxomgr_subscribe_temp_read
(
   tcxomgr_temp_read_subscriber_enum  client_id,
   uint32                             temp_channel,
   uint16                             read_interval,
   tcxomgr_temp_read_subscribe_cb     cb_func
);

extern boolean tcxomgr_subscribe_temp_read_threshold
(
  tcxomgr_temp_read_subscriber_enum   client_id,
  uint32                              temp_channel,
  uint16                              read_threshold,
  tcxomgr_temp_read_subscribe_cb      cb_func
);

extern boolean tcxomgr_temp_threshold_set_addn_channels
(
  tcxomgr_temp_read_subscriber_enum   client_id, 
  uint32                              addn_channels
);

extern tcxomgr_crystal_type tcxomgr_get_crystal_type(void);

extern tcxomgr_rpush_req_mode_enum tcxomgr_get_rpush_req_mode (void);

extern void tcxomgr_register_rpush_cb
(
  tcxomgr_rpush_cb_registration_type reg_data
);

extern void tcxomgr_register_rpush_mode_cb
(
  tcxomgr_rpush_mode_cb_registration_type reg_data
);

extern void tcxomgr_rotator_push
(
  tcxomgr_rpush_data_type rpush_data
);

#endif /* !TCXOMGR_H */
