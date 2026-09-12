#ifndef __WWCOEX_ACTION_IFACE_H__
#define __WWCOEX_ACTION_IFACE_H__
/*!
  @file wwcoex_fw_iface.h

  @brief
   APIs exposed by the CXM for WWAN-WWAN COEX for the inner loop (FW/G Arbitrator)

*/

/*=============================================================================

  Copyright (c) 2013-2015 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

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

=============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcs.mpss/5.2/api/wwcoex_action_iface.h#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
07/10/15   rj      Added support for Wwancoex-FW CRAT mode info
05/27/15   rj      Enable QXTA feature flag for FW
05/04/15   sg      QXTA support
05/01/15   jm      RFFE Bus Contention Solution (coex & wmgr)
10/30/14   jm      Interface change to support 4-bit action
08/30/14   sg      Support for spur mitigation
08/29/14   jm      Change SL TL sim to use FreqId Pointer
08/18/14   sg      Slot level timeline simulation support
08/12/14   jm      Slot level timeline support
02/05/14   jm      Added new ACTION for slot level backoff
11/21/13   jm      Added DDR Freq as desense type
05/05/13   ag      Moved init function
04/06/13   ag      WWAN Coex Mgr interface version 1

=============================================================================*/
/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
#include "msgr.h"

/* Keep this below all the includes */
#ifdef __cplusplus
extern "C" {
#endif

#define MCS_CXM_QXTA
	
/*=============================================================================

                       COMMON TYPES AND DEFINES

=============================================================================*/
#define WWCOEX_MAX_REQUESTING_IDS       8
#define WWCOEX_ACTION_BIT_SIZE          4
#define WWCOEX_ACTION_BITWISE_MASK      0xF

#define WWCOEX_MAX_CONCURRENT_SPURS     6
#define WWCOEX_INVALID_SPUR_HANDLE      0
/* Featurize legacy code because FW CXM depends on this */
#ifdef MCS_CXM_QXTA
#define WWCOEX_MAX_CARRIER_PER_CHANNEL  2
#else
#define WWCOEX_MAX_CARRIER_PER_CHANNEL  3
#endif

#define WWCOEX_CHNL_BIT_MASK            1
#define WWCOEX_CHNL_BIT_SIZE            1
#define WWCOEX_CAR_PER_CHNL_BIT_SIZE    2
#define WWCOEX_CAR_PER_CHNL_BIT_MASK    3
#define WWCOEX_CHNL_ID_BIT_SIZE         3
#define WWCOEX_CHNL_ID_BIT_MASK         7


/*! Maximum number of entries supported in slot timing info list. 
    4 frame look-ahead * 8 slots per frame */
#define CXM_SIM_MAX_NUM_TIMING_SLOTS 32

/*! To be used by tech to indicate unknown/invalid time for filling
    USTMR fields */
#define CXM_SIM_INVALID_USTMR_TIME 0xFFFFFFFF

/*! Maximum number of GSM frames supported in the slot timing info list. */
#define CXM_SIM_MAX_GSM_LOOK_AHEAD_FRAMES 4

/*! Maximum number of GSM specific conditional metrics */
#define CXM_SIM_MAX_GSM_METRIC_TYPES 3


/*! Enum for activity types - DL/UL */
typedef enum
{
  ACTIVITY_INVALID,
  ACTIVITY_RX,
  ACTIVITY_TX
}wwan_coex_activity_type;

/*! @brief Enum for identifying type of interference or desense issue */
typedef enum
{
  NO_DESENSE,
  NOISE_DESENSE,
  HARMONIC_DESENSE,
  BLOCKING_DESENSE,
  IM_TYPE1_DESENSE,
  IM_TYPE2_DESENSE,
  SPURIOUS_EMISSIONS,
  DDR_FREQ_DESENSE,
  IM_DESENSE_GPS,
  CONCURR_RESTRICTION,
  MAX_DESENSE_TYPES
}wwan_coex_desense_type;

/*! @FW tech IDs 
    This order should not be changed without corresponding 
    FW changes. This is alligned to FW tech IDs */
typedef enum {
  /*! No technology */
  CXM_FW_NOTECH,
  /*! GERAN SUB 0 */
  CXM_FW_GSM,
  /*! C2K */
  CXM_FW_1X,
  /*! HDR/DO */
  CXM_FW_DO,
  /*! WCDMA SUB 0 */
  CXM_FW_WCDMA,
  /*! LTE */
  CXM_FW_LTE,
  /*! TDSCDMA */
  CXM_FW_TDSCDMA,
  /*! GERAN SUB 1 */
  CXM_FW_GSM_G1,
  /*! GERAN SUB 2 */
  CXM_FW_GSM_G2,
  /*! NOT USED */
  CXM_FW_RF, 
  /*! WCDMA SUB 1 */
  CXM_FW_WCDMA_W1,
  /*! NOT USED*/
  CXM_FW_INTERNAL,
  /*! Last technology */
  CXM_FW_RAT_MAX
} cxmfw_rat_type;

#ifdef MCS_CXM_QXTA

/*! @brief Enum for Mitigation Action for WWAN coex
    This should be in the order of severity. FW-CXM depends on ordering 
    of this enum */
typedef enum
{ 
  /*! Action is None i.e there is no desense */
  ACTION_NONE,

  /*! Action is subframe level backoff at the FW conflict check level 
      i.e desense is expected */
  ACTION_BACKOFF,

    /*! Not used by WWAN CXM */
  ACTION_BLANK_WLAN,

  /*! Action is Blank i.e desense is expected */
  ACTION_BLANK,

  /*! Action is Unknown i.e either power or frequency information is 
    unavailable to accurately determine desense action */
  ACTION_UNKNOWN,

  /*! Action is Spur i.e the victim tech should apply a 
      notch to mitigate the spur */
  ACTION_SPUR,

  /*! Action spur is Unknown i.e frequency information is 
     unavailable to accurately determine desense action */
  ACTION_SPUR_UNKNOWN,

  /*! Not used by WWAN CXM */
  ACTION_ASDIV,

  /*! Action is channel conflict i.e only one tech can proceed */
  ACTION_FILTER_CONFLICT,
  
  /*! Action is channel conflict i.e only one tech can proceed */
  ACTION_CHANNEL_CONFLICT,

  /*! Action can only be 4 bits max */
  ACTION_MAX = ACTION_CHANNEL_CONFLICT
}cxm_action_type;

#else
/*! @brief Enum for Mitigation Action for WWAN coex
    This should be in the order of severity. FW-CXM depends on ordering 
    of this enum */
typedef enum
{ 
  /*! Action is None i.e there is no desense */
  ACTION_NONE,

  /*! Action is subframe level backoff at the FW conflict check level 
      i.e desense is expected */
  ACTION_BACKOFF,

  /*! Action is Blank i.e desense is expected */
  ACTION_BLANK,

  /*! Action is Unknown i.e either power or frequency information is 
    unavailable to accurately determine desense action */
  ACTION_UNKNOWN,

  /*! Action is Spur i.e the victim tech should apply a 
      notch to mitigate the spur */
  ACTION_SPUR,

 /*! Action spur is Unknown i.e frequency information is 
     unavailable to accurately determine desense action */
  ACTION_SPUR_UNKNOWN,

  /*! Action can only be 4 bits max */
  ACTION_MAX = ACTION_SPUR_UNKNOWN
}cxm_action_type;
#endif



/* MCS provides information, and common FW will infer the CRAT state
   for shared VUSS loading.
     Instance 0 tells Single Tech mode
     Instance 1 tells Concurrent RAT Mode
   ! @brief MCS Instance state enum */
typedef enum {

  /*! Instance 0 GSM */
  CXM_FW_INSTANCE0_GSM =0,

  /*! Instance 0 1X */
  CXM_FW_INSTANCE0_1X =1,

  /*! Instance 0 HDR */
  CXM_FW_INSTANCE0_HDR =2,

  /*! Instance 0 WCDMA */
  CXM_FW_INSTANCE0_WCDMA =3,

  /*! Instance 0 LTE */
  CXM_FW_INSTANCE0_LTE =4,

  /*! Instance 0 TDS */
  CXM_FW_INSTANCE0_TDS =5,

  /*! Instance 1 GSM */
  CXM_FW_INSTANCE1_GSM =6,

  /*! Instance 1 1X */
  CXM_FW_INSTANCE1_1X =7,

  /*! Instance 1 WCDMA */
  CXM_FW_INSTANCE1_WCDMA =8,

  /*! MAX Enums */
  CXM_FW_INSTANCE_MAX

} cxm_fw_crat_mode_e;

/*! @brief Concurrent RAT info
     Instance 0 tells Single Tech mode
     Instance 1 tells Concurrent RAT Mode
     */
typedef union 
{
  struct
  {
    uint32 instance0_gsm   : 1;
    uint32 instance0_1x    : 1;
    uint32 instance0_hdr   : 1;
    uint32 instance0_wcdma : 1;
    uint32 instance0_lte   : 1;
    uint32 instance0_tds   : 1;
    uint32 instance1_gsm   : 1;
    uint32 instance1_1x    : 1;
    uint32 instance1_wcdma : 1;
  }crat_info;

  uint32 dummy;
}cxm_fw_crat_mode_t;


/*! @brief Action Table for querying action per freqId pair */
typedef struct 
{
  /* FreqId of the tech that is requesting for a conflict check */
  uint32 requesting_freqid;

  /* FreqId of the tech that is conflicting with the requesting tech */
  uint32 conflicting_freqid;

  /* Mitigation action to be taken by the requesting tech if its low priority */
  cxm_action_type action;

}cxm_action_table_s;

/*! @brief Struct for providing desense results to FW CXM */
typedef struct
{
  /* FreqId of the requesting techs... */
  uint16 *requesting_ids;
  uint16 num_requesting_ids;

  /* FreqId of the conflicting techs... */
  uint16 *conflicting_ids;
  uint16 num_conflicting_ids;

  /* Action Mask... */
  uint32 actions;
  
  /* Activity Types ... */
  wwan_coex_activity_type requesting_activity;
  wwan_coex_activity_type conflicting_activity;
}cxm_action_query_s;

/*! @brief Struct for providing spur results to FW CXM */
typedef struct
{
  uint16 *requesting_ids;                       /* Requesting IDs. */
  uint16 *conflicting_ids;                      /* Conflicting IDs. */
  uint32 action;                                /* Action needed to mitigate spur. 4 bits for action */
  uint16 handle;                                /* Handle for notch settings. Single handle for each request */
  uint8  *requesting_ch_id;                     /* Channel ID list for requesting freq IDs */
  uint8  num_requesting_ch;                     /* Number of channels */
  uint8  num_carr_per_requesting_ch;            /* Number of carrier on each channel */
  uint8  *conflicting_ch_id;                    /* Channel ID list for conflicting freq IDs */
  uint8  num_conflicting_ch;                    /* Number of channels */
  uint8  num_carr_per_conflicting_ch;           /* Number of carrier on each channel */
  wwan_coex_activity_type requesting_activity;  /* Requesting activity. */
  wwan_coex_activity_type conflicting_activity; /* Conflicting activity. */
}cxm_spur_query_s;


/*! @brief Struct for providing results to FW CXM 
    Almost all of the fields are packed vectors 
    The same structure is used for desense, channel and filter conflict */
typedef struct
{
  /* Requesting Freq IDs for a channel pair 
     diversity does not have a unique freq ID */
  uint16 *req_ids;

  /* Conflicting Freq IDs for a channel pair 
     diversity does not have a unique freq ID */
  uint16 *conf_ids;

  /* Action is a packed field. CXM stores the result in this field
     Action is stored per carrier per chnl and uses 4 bits each */
  uint64 action;
  
  /* Requesting Channel IDs is a packed field
     Channel IDs are stored using 3 bits for each channel */
  uint32 req_ch_id;

  /* Number of requesting channels is a bitmask
     Bit is set to indicate each channel that is requested */
  uint8  num_req_ch;

  /* Number of carrier per req channel is a packed field
     This stores the number of carrier for a channel pair */
  uint8  num_carr_per_req_ch;            

  /* Conflicting Channel IDs is a packed field
     Channel IDs are stored using 3 bits for each channel */
  uint32 conf_ch_id;

  /* Number of conflicting channels is a bitmask
     Bit is set to indicate each channel that is conflicting */
  uint8  num_conf_ch;

  /* Number of carrier per conf channel is a packed field
     This stores the number of carrier for a channel pair */
  uint8  num_carr_per_conf_ch;

  /* Requesting activity */
  wwan_coex_activity_type req_act;

  /* Conflicting activity */
  wwan_coex_activity_type conf_act;

  /* Requesting 
FW tech Id */
  uint8  req_tech;

  /* Conflicting FW tech Id */
  uint8  conf_tech;
}cxm_conflict_check_s;


/*! @brief Struct for providing sour details to tech FW 
    This struct is returned when tech FW queries MCS using the spur handle */
typedef struct
{
  int32   spur_freq_offset;      /* Spur offset from the victim LO frequency. Units: Hz      */
  uint16  spur_freq_uncertainty; /* Uncertainty in the calculated spur frequency. Units: PPB */
  uint16  victim_freq_id;        /* Freq ID  on the vitim tech                               */
  int16   spur_level;            /* Spur level. Units: dbm*10                                */
  uint8   notch_depth;           /* Notch deptch from RFC spread sheet                       */
  uint8   spur_type;             /* Spur type from RFC spread sheet                          */
}cxm_spur_info_s;

/*! @brief Struct for storing the highest desense result */
typedef struct
{
  /* FreqId array of the requesting techs... */
  uint32 *requesting_ids;

  /* Action array for the corresponding requesting ids */
  cxm_action_type *actions;

  uint32 num_requesting_ids;
}cxm_highest_action_query_s;

/*! @brief CXM WWAN Tech Identifier Type. This structure lines up with QMI tech enum */
typedef enum
{
  CXM_SIM_TECH_DFLT_INVLD = -1, /*!< Default/Invalid tech ID */
  CXM_SIM_TECH_LTE        =  0, /*!< LTE */
  CXM_SIM_TECH_TDSCDMA,         /*!< TD-SCDMA */
  CXM_SIM_TECH_GSM1,            /*!< GSM on Subscription_1 */
  CXM_SIM_TECH_ONEX,            /*!< 1X */
  CXM_SIM_TECH_HDR,             /*!< HDR */
  CXM_SIM_TECH_WCDMA,           /*!< WCDMA */
  CXM_SIM_TECH_GSM2,            /*!< GSM on Subscription_2 */
  CXM_SIM_TECH_GSM3,            /*!< GSM on Subscription_3 */
  CXM_SIM_TECH_MAX              /*!< Max tech ID supported */
}cxm_sim_tech_type;

/*! @brief Tech link (band_channel) direction [UL/DL or both] */
typedef enum
{
  CXM_SIM_LNK_DRCTN_INVLD,     /*!< Default/Invalid link direction */
  CXM_SIM_LNK_DRCTN_UL,        /*!< Up-Link info-set */
  CXM_SIM_LNK_DRCTN_DL,        /*!< Down-Link info-set */
  CXM_SIM_LNK_DRCTN_UL_AND_DL, /*!< Up-Link and Down-Link info-set */
  CXM_SIM_LNK_DRCTN_MAX        /*!< Max value supported */
}cxm_sim_tech_link_direction;

typedef uint32 cxm_sim_tech_link_type;

typedef enum 
{
  CXM_SIM_SLOT_PRIO_LOW,   /*!< Low prio slot, used by WLAN */  
  CXM_SIM_SLOT_PRIO_HIGH   /*!< High prio slot, used by WLAN */
}cxm_sim_slot_prio_e;

/*! @brief Timing Slot Info */
typedef struct {
  /*! POINTER to Frequency ID -- This is different to cxm_timing_slot_entry */
  uint16* freqid; 

  /*! Frequency (kHz) */
  uint32 frequency;
  
  /*!< Tech operating bandwidth in Hz */
  uint32 bandwidth;

  /*!< Tech operating band (sys_band_class_e_type) */
  uint32 band;

  /*! Type of activity being registered, Uplink/Downlink - Both is considered invalid */ 
  cxm_sim_tech_link_direction direction;

  /*! Link Type of slot (Normal/Diversity/PowerMonitor) */
  cxm_sim_tech_link_type link_type;

  /*! Start time (in USTMR units) of the activity in the next slot (unpadded)*/ 
  uint32 start_time;

  /*! End time (in USTMR units) of the activity in the next slot (unpadded)*/ 
  uint32 end_time; 

  /*! Micro priority which maps to ACTIVITY_TIER */ 
  uint32 micro_prio;

  /*! Relative priority of activity */
  cxm_sim_slot_prio_e wlan_prio;

  /*! Power for slot (dBm * 10) */ 
  int32 pwr;
  
  /*! Bitwise mask based off of cxm_tl_conflict_type; to be filled by MCS only
      Example: 0b1010 =
      (1 << CXM_TL_CONFLICT_DESENSE_RX) | (1 << CXM_TL_CONFLICT_CHANNEL_TX) */
  uint16 conflict_mask;

}cxm_sim_timing_slot_entry; 


typedef struct {

  /*! DTx state indicator */
  boolean in_dtx_state;
  
  /*! Time (in USTMR units) where DTx state toggles within the
      four frame reporting. Macro CXM_SIM_INVALID_USTMR_TIME indicates
      all activity is in same state */
  uint32 dtx_switch_time;
  
  /*! DTx time metric:
      - Current time elapsed (in ms) in DTx state if indicator is TRUE
      - Current time elapse (in ms) out of DTx if indicator is FALSE */
  uint32 dtx_time;  
  
}cxm_sim_dtx_state_info;

/*! @brief Enum for GSM call type */
typedef enum
{
  /*! GSM is in no call */
  GSM_SIM_CALL_NONE,
  
  /*! GSM call is NOT voice-only */
  GSM_SIM_CALL_NOT_VOICE_ONLY,
  
  /*! CS Voice call with TCH half-rate codec & sub-channel 0 */
  GSM_SIM_CALL_VOICE_TCH_HALF_RATE_SUB_0,
  
  /*! CS Voice call with TCH half-rate codec & sub-channel 1 */
  GSM_SIM_CALL_VOICE_TCH_HALF_RATE_SUB_1,
  
  /*! CS Voice call with TCH full-rate codec */
  GSM_SIM_CALL_VOICE_TCH_FULL_RATE,
  
  /*! CS Voice call with AMR half-rate codec & sub-channel 0 */
  GSM_SIM_CALL_VOICE_AMR_HALF_RATE_SUB_0,
  
  /*! CS Voice call with AMR half-rate codec & sub-channel 1 */
  GSM_SIM_CALL_VOICE_AMR_HALF_RATE_SUB_1,  
  
  /*! CS Voice call with AMR full-rate codec */
  GSM_SIM_CALL_VOICE_AMR_FULL_RATE,
  
  /*! CS Signalling Call */
  GSM_SIM_CALL_CS_SDDCH,
  
  /*! Dual Transfer Mode (Voice+Data) with half-rate codec & sub-channel 0 */
  GSM_SIM_CALL_DTM_HALF_RATE_SUB_0,
  
  /*! Dual Transfer Mode (Voice+Data) with half-rate codec & sub-channel 1 */
  GSM_SIM_CALL_DTM_HALF_RATE_SUB_1,
  
  /*! Dual Transfer Mode (Voice+Data) with full-rate codec */
  GSM_SIM_CALL_DTM_FULL_RATE,
  
  /*! Packet Switched call */
  GSM_SIM_CALL_PS
  
}wwcoex_sim_gsm_call_type;

/*! @brief GSM specific metrics for slot timeline info */
typedef struct
{  

  /*! GSM Call type */
  wwcoex_sim_gsm_call_type call_type;
  
  /*! Start frame boundary (in USTMR units) of each
      frame in the message  */ 
  uint32 frame_start_time[CXM_SIM_MAX_GSM_LOOK_AHEAD_FRAMES];   
  
  /*! Start time (in USTMR units) of a new block, which indicates
      any activity reported after this time is NOT accurate.
      Macro CXM_SIM_INVALID_USTMR_TIME indicates all activity comes
      from same block */ 
  uint32 new_block_time;
    
  /*! Bit-wise enable validity indicator for GSM specific
      conditional metrics (defined below).  */
  uint8 metrics_valid_mask;
  
  /*! GSM Specific Conditional Metrics.
  
    Position 0: Max sustainable conecutive GSM UL DTx-ed frames after
    current GSM frame, measured in time (ms)

    Position 1: Max cluster of GSM UL DTx-ed frames with 1 intermittent
    Tx burst (SACCH) counted after current GSM frame,
    measured in time (ms)

    Position 2: Max cluster of GSM UL DTx-ed frames with 2 intermittent
    Tx bursts (SACCH) counted after current GSM frame,
    measured in time (ms)
  */
  uint32 cond_metics[CXM_SIM_MAX_GSM_METRIC_TYPES];

}cxm_sim_gsm_metric_info_s;

/*! @brief Union for all TECH specific metrics for timeline info */
typedef union
{
  cxm_sim_gsm_metric_info_s gsm_metrics; /*!< GSM specific parameters */
  
}cxm_sim_tech_specific_metrics_u;

/*! @brief Message for Slot Level Timing Indication sent to MCS */ 
typedef struct { 

  msgr_hdr_struct_type msg_hdr; 

  /*! Technology identifier */ 
  cxm_sim_tech_type tech_id; 

  /*!  Number of activity slots in list */ 
  uint8 num_entries;

  /*! List of slots - DTx activity should not be included */
  cxm_sim_timing_slot_entry slot_list[CXM_SIM_MAX_NUM_TIMING_SLOTS];
  
  /*! Information of UL DTx */
  cxm_sim_dtx_state_info dtx_info;
   
  /*! Tech specific metrics */
  cxm_sim_tech_specific_metrics_u tech_metrics;

}cxm_sim_timing_info_s;

/* UMID used by FW SIM to send GSM TL information to MCS */
enum
{
  SET_SIM_SLOT_ACT_TL_ID      = 0x7C,
};

enum
{
  /*! Tech Tx/Rx slot level activity indication (L1 to MCS) */
  MSGR_DEFINE_UMID( MCS, CXM, IND, SET_SIM_SLOT_ACTIVITY_TL,
                    SET_SIM_SLOT_ACT_TL_ID, cxm_sim_timing_info_s),
};

/*=======================================================================
CALLBACK wwcoexSubsModeCallback

DESCRIPTION
  Callback function type that WwanCoex uses to indicate FW regarding SubsMode change
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
typedef void (*wwcoexSubsModeCallback)(cxm_fw_crat_mode_t  mode_info);

/*=============================================================================

                        Function Declarations

=============================================================================*/

/*============================================================================

FUNCTION CXM_QUERY_ACTION

DESCRIPTION
  Query the mitigation action corresponding to the two techs freq combination.
  
DEPENDENCIES
  None

RETURN VALUE
  FALSE for incorrect arguments like NULL pointer, 0 entries in the list, 
  incorrect freqId
  TRUE otherwise

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_query_action
(
  cxm_action_query_s* query_st
);

/*============================================================================

FUNCTION CXM_QUERY_HIGHEST_ACTION

DESCRIPTION
  Query the highest mitigation action for one freqId
  
DEPENDENCIES
  None

RETURN VALUE
  FALSE for incorrect arguments like NULL pointer, 0 entries in the list, 
  incorrect freqId
  TRUE otherwise

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_query_highest_action
(
  cxm_highest_action_query_s* query_st
);

/*===========================================================================
FUNCTION wwcoex_init_tables

DESCRIPTION
  This API will init the wwcoex conflict tables.
  num_standby_stacks to be 2 for Dual Sim. this is number of concurrent standby 
  stacks.
  num_active_stacks to be 1 for DSDS and 2 for DSDA... this is number of 
  concurrent active stacks.

DEPENDENCIES 
  None

RETURN VALUE  
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void wwcoex_init_tables
(
  uint32 num_standby_stacks,
  uint32 num_active_stacks
);

/*===========================================================================
FUNCTION wwcoex_check_task_state

DESCRIPTION
  This API will inform the caller whether lmtsmgr task is up

DEPENDENCIES 
  None

RETURN VALUE  
  TRUE  - if the lmtsmgr task is up
  FALSE - if the lmtsmgr task is not running

SIDE EFFECTS
  None
  
===========================================================================*/
boolean wwcoex_check_task_state(void);

/*===========================================================================
FUNCTION wwcoex_set_task_state

DESCRIPTION
  This API will set lmtsmgr task up flag

DEPENDENCIES 
  None

RETURN VALUE  
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void wwcoex_set_task_state(boolean task_state);


/*===========================================================================
FUNCTION wwcoex_set_rf_chain_holders

DESCRIPTION
  This API will the RF chain holders

DEPENDENCIES 
  None

RETURN VALUE  
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void wwcoex_set_rf_chain_holders
(
  cxm_fw_crat_mode_t  *rf_crat_mode
);


/*===========================================================================
FUNCTION wwcoex_init_spur_sim

DESCRIPTION
  This API will init the spur simulator with preloaded values

DEPENDENCIES 
  None

RETURN VALUE  
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void wwcoex_init_spur_sim
(
  uint32 sim_input
);

/*===========================================================================
FUNCTION wwcoex_init_chnl_fltr_sim

DESCRIPTION
  This API will init the channel and filter conflict

DEPENDENCIES 
  None

RETURN VALUE  
  None

SIDE EFFECTS
  None
  
===========================================================================*/
void wwcoex_init_chnl_fltr_sim
(
  uint32 sim_input,
  uint8  data_tech,
  uint8  idle_tech
);

/*============================================================================

FUNCTION CXM_QUERY_SPUR_ACTION

DESCRIPTION
  Query the mitigation action corresponding to the two techs freq combination.
  
DEPENDENCIES
  None

RETURN VALUE
  FALSE for incorrect arguments like NULL pointer, 0 entries in the list, 
  incorrect freqId
  TRUE otherwise

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_query_spur_action
(
  cxm_spur_query_s* query_st
);

/*============================================================================

FUNCTION CXM_QUERY_SPUR_INFO_FROM_HANDLE

DESCRIPTION
  Query the spur setting corresponding to a handle.
  This API is called from Tech FW to MCS to get spur informaton
  
DEPENDENCIES
  None

RETURN VALUE
  FALSE for incorrect arguments like NULL pointer or invalid handle
  TRUE otherwise

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_get_spur_info_from_handle
(
  uint16               spur_handle,
  cxm_spur_info_s      *spur_info,
  uint32               *num_spurs
  
);

/*============================================================================

FUNCTION CXM_QUERY_CHANNEL_CONFLICT

DESCRIPTION
  Query if there are any channel conflict between the given freq IDs.
  This API will be used in QTA and QCTA.
  
DEPENDENCIES
  None

RETURN VALUE
  FALSE for incorrect arguments like NULL pointer, 0 entries in the list, 
  incorrect freqId
  TRUE otherwise

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_query_channel_conflict
(
  cxm_conflict_check_s * query_st
);


/*============================================================================

FUNCTION CXM_QUERY_FILTER_CONFLICT

DESCRIPTION
  Query if there is any filter conflict between the given freq IDs.
  This API will be used in QDTA.
  
DEPENDENCIES
  None

RETURN VALUE
  FALSE for incorrect arguments like NULL pointer, 0 entries in the list, 
  incorrect freqId
  TRUE otherwise

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_query_filter_conflict
(
  cxm_conflict_check_s* query_st
);


/*============================================================================

FUNCTION CXM_QUERY_DESENSE_ACTION

DESCRIPTION
  Query if there is any desense between the frequency combinations.
  
DEPENDENCIES
  None

RETURN VALUE
  FALSE for incorrect arguments like NULL pointer, 0 entries in the list, 
  incorrect freqId
  TRUE otherwise

SIDE EFFECTS
  None

============================================================================*/
boolean cxm_query_desense_action
(
  cxm_conflict_check_s* query_st
);

/*============================================================================

FUNCTION WWCOEX_SET_CRAT_MODE

DESCRIPTION
  Updates CRAT Mode to get what system is capable of
  
DEPENDENCIES
  None

Input VALUE
  Returns BitMask for Sub1+Sub2 techs

  Single SIM Capability
    L only  = (crat_info.instance0_lte = 0x1) 
    1X only = (crat_info.instance0_1x = 0x1) 
    W only  = (crat_info.instance0_wcdma = 0x1) 
    SLTE (L+1X) = ((crat_info.instance0_lte = 0x1) | (crat_info.instance1_1x = 0x1))
    SRLTE (L+1X QTA) = ((crat_info.instance0_lte = 0x1) | (crat_info.instance1_1x = 0x1))
  
  Multi SIM Capability
    L+W = ((crat_info.instance0_lte = 0x1) | (crat_info.instance1_wcdma = 0x1))
    L+1X = ((crat_info.instance0_lte = 0x1) | (crat_info.instance1_1x = 0x1))

============================================================================*/
void wwcoex_set_crat_mode( cxm_fw_crat_mode_t crat_mode );

/*============================================================================

FUNCTION CXM_UPDATE_SUBS_STATE

DESCRIPTION
  This function calls FW Callback which was registered by FW to get Mode change updates
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

============================================================================*/
void cxm_update_subs_state( void );

/*============================================================================

FUNCTION CXM_REGISTER_MODE_UPDATE_CALLBACK

DESCRIPTION
  Register CallBack with CXM for getting Mode Updates 
  
DEPENDENCIES
  None

RETURN VALUE
None

SIDE EFFECTS
  None

============================================================================*/
void cxm_register_mode_update_callback( wwcoexSubsModeCallback reg_cb );

/*============================================================================

FUNCTION CXM_QUERY_CRAT_MODE

DESCRIPTION
  Query CRAT Mode to get what system is capable of
  
DEPENDENCIES
  None

RETURN VALUE
  Returns BitMask for Sub1+Sub2 techs

  Single SIM Capability
    L only = ((crat_info.instance0_none = 0x0) | (crat_info.instance0_lte = 0x1) | (crat_info.instance1_none = 0x1))
    1X only = ((crat_info.instance0_none = 0x0) | (crat_info.instance0_1x = 0x1) | (crat_info.instance1_none = 0x1))
    W only = ((crat_info.instance0_none = 0x0) | (crat_info.instance0_wcdma = 0x1) | (crat_info.instance1_none = 0x1))
    SLTE (L+1X) = ((crat_info.instance0_none = 0x0) | (crat_info.instance0_lte = 0x1) | 
                             (crat_info.instance1_none = 0x0) |(crat_info.instance1_1x = 0x1))
  
  Multi SIM Capability
    L+W = ((crat_info.instance0_none = 0x0) | (crat_info.instance0_lte = 0x1) | 
                             (crat_info.instance1_none = 0x0) |(crat_info.instance1_wcdma = 0x1))
    L+1X = ((crat_info.instance0_none = 0x0) | (crat_info.instance0_lte = 0x1) | 
                             (crat_info.instance1_none = 0x0) |(crat_info.instance1_1x = 0x1))

SIDE EFFECTS
  None

============================================================================*/
cxm_fw_crat_mode_t cxm_query_crat_mode( void );


#ifdef __cplusplus
}
#endif

#endif /* __WWCOEX_ACTION_IFACE_H__ */
