
/*===========================================================================
FILE: tdsgapmgr.h


GENERAL DESCRIPTION
  This file contains code responsible for gap management for TDSCDMA connected
  mode.


Copyright (c) 2010 by QUALCOMM Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/15/13   zl      Separate gap query APIs for UL and FW
10/15/12   zl      Create this file for T2L connected mode measurement
===========================================================================*/

#ifndef TDS_GAP_MANAGER_H
#define TDS_GAP_MANAGER_H

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "tdsirat.h"
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) || defined(FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT)
#include "tdsl1rrcif.h"
#include "tdsl1def.h"
#include "tdssrchltedrv.h"
#include "tdssrchmeas.h"
#include "tdsl1m_rcinit.h"
/*-------------------------------------------------------------------------*/
/*                        MACROS                                           */
/*-------------------------------------------------------------------------*/
/*Number of timeslots per sub-frame, including special ts. 
  TS7 is special timeslot. */
#define TDSGAPMGR_TS_NUM_PER_SUBFR    8
/*Number of subframes during which the DMO and Idle Interval 
  gaps will be calculated.*/
//#define TDSGAPMGR_FRAME_WIN_SIZE      5
/*Need 10 radio frames since UL needs to look forward for 80ms starting frome 
  N+2. */
#define TDSGAPMGR_FRAME_WIN_SIZE      10
#define TDSGAPMGR_SUBFR_WIN_SIZE      (2 * TDSGAPMGR_FRAME_WIN_SIZE)
/*Current we only support looking for DMO/Idle Interval gaps in 
  next 8 radio frames. */
#define TDSGAPMGR_LOOK_FORWARD_WIN_SIZE      8


/*To indicate whether 5ms or 10ms overhead is required for triggering LTE meas*/
#define TDSGAPMGR_LTE_TIMED_MEAS_OVERHEAD 10

#define TDSGAPMGR_INTLOCK()        REX_ISR_LOCK(&tdsgapmgr_crit_sect)
#define TDSGAPMGR_INTFREE()        REX_ISR_UNLOCK(&tdsgapmgr_crit_sect)

extern uint8 tdsgapmgr_cyclic_array_idx_lookup_table[TDSGAPMGR_SUBFR_WIN_SIZE][TDSGAPMGR_SUBFR_WIN_SIZE];
/*A Macro to calculate the actual index of a cyclic array*/ 
#define TDSGAPMGR_CAL_CYCLIC_ARRAY_IDX(baseidx, offset, maxLen) tdsgapmgr_cyclic_array_idx_lookup_table[baseidx][offset]

//#define TDSGAPMGR_CAL_CYCLIC_ARRAY_IDX(baseidx, offset, maxLen) (((baseidx)+(offset))%(maxLen))

/*This macro defines how far ahead TS update will be done. If it is 0, then at ts0 of each subframe, ts usage 
will be updated starting from this sub-frame. If it is defined as 1, then in the middle of each subframe, ts_usage 
will be updated starting from next sub-frame. */
#define TDSGAPMGR_TS_UPDATE_ADVANCE_SUBFN   0

/*For PDSCH*/
#define TDSGAPMGR_PDSCH_TS_USAGE_ALL_RX 0x0AA8 

//The time advance allowed for UL and othe modules to pick up gap pattern change. 
//consider TTI=40ms, and UL needs 20ms in advance. So, the offset is 6 radio frames. 
#define TDSGAPMGR_PATTERN_RECONFIG_OFFSET_SFN    6

/*-------------For Continuos Gaps------------*/
#define TDSGAPMGR_MAX_CONTINUOS_GAP_PATTERNS       4
#define TDSGAPMGR_CONTINUOS_GAP_INVALID_PATTERN_ID  0xFF
#define TDSGAPMGR_CONTINUOS_GAP_INVALID_TS_IDX      0xFF
#define TDSGAPMGR_CONTINUOS_GAP_INVALID_SUBSFN_NUM  0xFFFF

///For QTA gap pattern ID
#define DSDS_MAX_QTA_GAP_ID  2
#define GFRAME_GAP_PATTERN_ID 2
//this is additional 2 subfm at the end of 13G frame gap 
//make sure thiere is no overlap betweent the gap end and the multisim_cmd for QTA
#define TDSGAPMGR_IRAT_GAP_END_BUFFER     2


/*-------------------------------------------------------------------------*/
/*                        TYPE DEFINITIONS                                 */
/*-------------------------------------------------------------------------*/
#if defined(FEATURE_TDSCDMA_DMO_SUPPORT)
typedef struct
  {
    /* patternIdentifier INTEGER(0..maxMeasOccasionPattern-1), 0~4 */
    uint8                  pattern_sequence_identifier;

    /* activate, deactivate */
    tdsl1_occa_pattern_status_enum_type      status_flag;

    boolean                     measurement_purpose_incl;

    /* measurementPurpose BIT STRING (SIZE (5)) OPTIONAL */
    /* Bit 0 is for Inter-frequency measurement. */
    /* Bit 1 is for GSM carrier RSSI measurement. */
    /* Bit 2 is for Initial BSIC identification. */
    /* Bit 3 is for BSIC reconfirmation. */
    /* Bit 4 is for E-UTRA measurement. */
    /* The value 1 of a bit means that the measurement occasion pattern 
           sequence is applicable for the corresponding type of measurement.
           Bit 0 is the first/leftmost bit of the bit string. */
    uint8                       measurement_purpose;

    /* measurementOccasionPatternParameter MeasurementOccasionPatternParameter OPTIONAL */
    boolean                  occasion_pattern_seq_para_incl;
    tdsl1_occasion_pattern_seq_para_type  occasion_pattern_seq_para;
  } tdsgapmgr_dmo_pattern_struct;

/*This data structure contains both the DMO pattern and its configuration time. */
  typedef struct
  {
      //SFN when new configuration is received (to activate or to de-activate)
      uint16 config_sfn;           
      //Whether the current command to activate or de-activate is still pending or not.
      boolean config_pending; 
      //structure to contain each DMO pattern (defined in tdsl1rrcif.h). 
      tdsgapmgr_dmo_pattern_struct dmo_pattern;
  }tdsgapmgr_dmo_config_struct;
/*This is the data structure to store the Idle Interval configuration 
    used for Gap Identification. */
  typedef struct
  {
    /* patternIdentifier INTEGER(0..maxMeasOccasionPattern-1), 0~4 */
    //uint8                  pattern_sequence_identifier;

    /* measurementPurpose BIT STRING (SIZE (5)) OPTIONAL */
    /* Bit 0 is for Inter-frequency measurement. */
    /* Bit 1 is for GSM carrier RSSI measurement. */
    /* Bit 2 is for Initial BSIC identification. */
    /* Bit 3 is for BSIC reconfirmation. */
    /* Bit 4 is for E-UTRA measurement. */
    /* The value 1 of a bit means that the measurement occasion pattern 
           sequence is applicable for the corresponding type of measurement.
           Bit 0 is the first/leftmost bit of the bit string. */
    uint8                       measurement_purpose;
    
    /* DMO periodicity: 1 to 512 radio frames; 0 for not configured. */
    uint16                              periodicity;
  
    /* In frames. The measurement occasion position in the measurement period. 0~511 */
    uint16                             offset;
  
    /* The measurement occasion length in frames starting from the Offset. 1~512 */
    uint16                             m_length;
  
    /* Bitmap indicating which of the timeslot(s) is/are allocated for measurement. Bit string (7) */
     /* Bit 0 is the first/leftmost bit of the bit string.
        The value 0 of a bit means the corresponding timeslot is not used for measurement.
        The value 1 of a bit means the corresponding timeslot is used for measurement. */
     uint8                              timeslot_bitmap;
  } tdsgapmgr_dmo_pattern_for_gap_struct;
#endif

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT)
/*The data structure for Idle Interval Configuration*/
  typedef struct {
    boolean is_configed;    //Whether Idle Interval is configured or not
    uint16  meas_id[TDSL1_MAX_MEAS_ID];        //Associated measurement ID for this II
    uint16  config_sfn;     //SFN when new config is received (to set up or release)
    boolean config_pending;  //Whether the new config is still pending or not
    //Actual container for Idle Interval inforamtion, defined in tdsl1rrcif.h
    tdsl1_inter_sys_idle_interval_info_type idle_interval_info;
  } tdsgapmgr_idle_interval_config_struct; 

/*This is the data structure to store the Idle Interval config 
  used for Gap Identification. */
  typedef struct {
    uint8   periodicity;   //4: 40ms; 8: 80ms; Others: Not configured
    uint8   offset;        //offset of the Idle Interval within the period. 
  } tdsgapmgr_idle_interval_config_for_gap_struct;

#endif
  
/*
The purpose of opening a continuously gap. Currently only QTA gap is defined. 
*/
typedef enum
{
    QTA_GAP = 0,     //To open a gap for QTA
    GFRAME_GAP,      //To open a gap for 13 GSM frames
    T2L_FORCED_GAP,     //to open a gap for T2L in the case of DMO blocked by W QTA
    INVALID_GAP      //Other purposes are invalid. 
} tdsgapmgr_configured_gap_purpose_enum;

/*The current status of each continuous gap pattern*/
typedef enum
{
  CONTINUOS_GAP_INACTIVE = 0,
  CONTINUOS_GAP_STARTED  = 1,
  CONTINUOS_GAP_STOPPED  = 2, 
  CONTINUOS_GAP_CONFIGURED = 3
} tdsgapmgr_configured_gap_status_enum;

/*
The configured continuous gap pattern
*/
typedef struct
{
  //meta information
  tdsgapmgr_configured_gap_purpose_enum gap_purpose;
  uint8           gap_pattern_id;
  tdsgapmgr_configured_gap_status_enum  gap_status;
  boolean         config_pending;

  //gap start and end.
  uint16          gap_start_subfn;
  uint16          gap_end_subfn;
  uint8           gap_start_ts;
  uint8           gap_end_ts;  
}tdsgapmgr_continuous_gap_struct;
  
/*This enum indicate whether an idle timeslot is from NATURAL, DMO or IDLE Interval. 
  If the timeslot is a busy/TX/RX/UNknown one, here it is indicated as NONE. */
typedef enum
{
    TDSGAPMGR_IDLE_TS_NONE = 0x0000,              //not an idle timeslot
    TDSGAPMGR_IDLE_TS_DMO = 0x0001,               //DMO timeslot
    TDSGAPMGR_IDLE_TS_IDLEINTERVAL = 0x0002,      //Idle interval timeslot
    TDSGAPMGR_IDLE_TS_NATURAL = 0x0004,           //natural timeslot
    TDSGAPMGR_IDLE_TS_QTA = 0x0008,            //INVALID
    TDSGAPMGR_IDLE_TS_CONTINUOUS = 0x0010,
    TDSGAPMGR_IDLE_TS_DMO_AVAIL_L = 0x0100,
    TDSGAPMGR_IDLE_TS_DMO_AVAIL_T = 0x0200,
    TDSGAPMGR_IDLE_TS_DMO_AVAIL_G = 0x0400,    
    TDSGAPMGR_IDLE_TS_SCHED_L = 0x0800,          //TS is scheduled for LTE measurement
    TDSGAPMGR_IDLE_TS_SCHED_T = 0x1000,          //TS is scheduled for T2T measurement
    TDSGAPMGR_IDLE_TS_SCHED_G = 0x2000,          //TS is scheduled for T2G measurement
    TDSGAPMGR_IDLE_TS_WHOLE_SUBFM = 0x4000       //whole frame is DMO/Idle Interval gap
} tdsgapmgr_idle_ts_source_enum;

typedef enum
{
    TDSGAPMGR_DMO,               //DMO timeslot
    TDSGAPMGR_IDLEINTERVAL,      //Idle interval timeslot
    TDSGAPMGR_NATURAL,           //natural timeslot
    TDSGAPMGR_NONE,              //not an idle timeslot
    TDSGAPMGR_INVALID_SLOT_STATE  //INVALID
} tdsgapmgr_ts_usage_dmo_idleinterval_enum;

typedef struct 
{
    /*Is this an idle interval gap? If yes, all timeslots will be marked 
      for LTE measurement. */
    tdsgapmgr_ts_usage_dmo_idleinterval_enum dmo_idle_interval_flag;
    /*DMO pattern ID for this subframe*/
    uint8   dmo_pattern_id;
    /*timeslot marks for each timeslot in this radio frame. */
    tdsgapmgr_ts_usage_dmo_idleinterval_enum timeslots_per_frame[TDSGAPMGR_TS_NUM_PER_SUBFR*2];
} tdsgapmgr_ts_usage_dmoii_per_frame_struct;

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT)
extern tdsgapmgr_idle_interval_config_for_gap_struct tdsgapmgr_idle_interval_for_gap;
#endif

#define TDSGAPMGR_IDLE_INTERVAL_CONFIGURED() \
     ((tdsgapmgr_idle_interval_for_gap.periodicity==4) \
      ||(tdsgapmgr_idle_interval_for_gap.periodicity==8))


///For QTA gap pattern ID
#define DSDS_MAX_QTA_GAP_ID  2

typedef struct
{ 
  uint16 twait;
  /*twait timer*/
  uint16 timer_counter;
  /*the timer is expired*/
  boolean timer_expired;
  /*QTA gap id, can be 0 or 1*/
  uint8 qta_gap_id;  
  /*QTA start subfm*/
  uint16 qta_start;
} tdsgapmgr_cm_meas_gap_policy_struct_type;




/*-------------------------------------------------------------------------*/
/*                        GLOBAL VARIABLES                                 */
/*-------------------------------------------------------------------------*/
#if defined(FEATURE_TDSCDMA_DMO_SUPPORT)
tdsgapmgr_dmo_config_struct tdsgapmgr_dmo_config_db[TDSL1_MAX_OCCASION_PATTERN];
boolean                     tdsgapmgr_new_dmo_config[TDSL1_MAX_OCCASION_PATTERN];
/*DMO measurement purpose bitmapping over all the patterns*/
uint16                      tdsgapmgr_mp_mapping;
boolean                     tdsgapmgr_all_purpose_dmo;

#endif
/*Data structure to store the source of each idle ts for each sub-frame: this is a cyclic array. */
//uint8 tdsgapmgr_ts_usage_dmoii_subframes[TDSGAPMGR_SUBFR_WIN_SIZE][TDSGAPMGR_TS_NUM_PER_SUBFR];
/*Bit mapping in tdsgapmgr_idle_ts_source_subframes[][]:
       for each sub-frame: [0] is ts0; [1] is ts1 and [7] is special ts. 
 */
#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT)
tdsgapmgr_idle_interval_config_struct tdsgapmgr_idle_interval_config_db_restore;
#endif

uint16 tdsgapmgr_idle_ts_source_subframes[TDSGAPMGR_SUBFR_WIN_SIZE][TDSGAPMGR_TS_NUM_PER_SUBFR];
/*tdsgapmgr_idle_ts_source_subframes_baseidx                                                          :   ith element
    (tdsgapmgr_idle_ts_source_subframes_baseidx+k)%TDSGAPMGR_SUBFR_WIN_SIZE:   kth element */
uint8 tdsgapmgr_idle_ts_source_subframes_baseidx;

/*data structure to store combined gaps for 20 sub-frames (10 radio frames)*/
tdssrchmeas_ts_usage_state_enum_type 
  tdsgapmgr_ts_natural_dmo_idleinterval_subframes[TDSGAPMGR_SUBFR_WIN_SIZE][TDSGAPMGR_TS_NUM_PER_SUBFR];

uint8 tdsgapmgr_ts_natural_dmo_idleinterval_subframes_baseidx;

boolean tdsgapmgr_in_use;
extern boolean tdssrchmeas_heartbeat_is_in_DCH;

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) || defined(FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT)
  tdsgapmgr_cm_meas_gap_policy_struct_type tdsgapmgr_cm_meas_gap_policy;
#endif


/*-------------------------------------------------------------------------*/
/*                        FUNCTION PROTOTYPES                              */
/*-------------------------------------------------------------------------*/
/*===========================================================================

FUNCTION:  tdsgapmgr_init

DESCRIPTION:
  Initialize the internal data structues. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_init(void);

/*===========================================================================
FUNCTION        tdsgapmgr_cleanup

DESCRIPTION
  This function will clean up the gap manager due to TL1 moving out of DCH state.
  1) Clean all IDLE Interval and DMO configurations;
  2) Clean all measurement gaps which haven't been scheduled for measurement;
  3) send a flag to L1M if 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern boolean tdsgapmgr_cleanup(tdsl1m_state_enum_type next_state);

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT)
/*===========================================================================

FUNCTION:  tdsgapmgr_idleinterval_no_valid_meas_id

DESCRIPTION:
  This function checks if there is valid meas_id in the database.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: no meas id anymore, the idle interval config should be cleared.
  FASLE: still valid meas id available 

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsgapmgr_idleinterval_no_valid_meas_id(void);


/*===========================================================================

FUNCTION:  tdsgapmgr_idleinterval_remove_gap_config

DESCRIPTION:
  Remove the gap config from data base if no valid more meas_id.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_idleinterval_remove_gap_config(uint8 meas_id);


/*===========================================================================

FUNCTION:  tdsgapmgr_idleinterval_configured_for_meas_id

DESCRIPTION:
  This function checks whether Idle Interval is configured for
  one measurement ID.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsgapmgr_idleinterval_configured_for_meas_id(uint16 meas_id);


/*===========================================================================

FUNCTION:  tdsgapmgr_update_idleinterval_config

DESCRIPTION:
  This function belongs to the DMO/Idle Interval Handling component.
 
  It will update the Idle Interval config db based on current configuration.
  1) if to set up Idle Interval from nothing:
     new idle interval configuration is effective immediately.
  2) If to change Idle Interval configuration,
     or to remove Idle Interval configuration:
     new configuration will be pending for 2 radio frames.
 
  To remove an existing Idle Interval configuration, the k value in
  new_config should be a value which is not 2 or 3. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_update_idleinterval_config(
   tdsl1_inter_sys_idle_interval_info_type new_config, 
   uint16 meas_id);

/*===========================================================================

FUNCTION:  tdsgapmgr_update_idleinterval_for_gap

DESCRIPTION:
  This function allows the DMO/Idle Interval handling to update the Idle
  Interval configuration for gap identification. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_update_idleinterval_for_gap(tdsgapmgr_idle_interval_config_for_gap_struct new_config);

/*===========================================================================

FUNCTION:  tdsgapmgr_set_idle_interval_gap

DESCRIPTION:
  This function set idle interval gap to the radio frame at index. 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_set_idle_interval_gap(uint8 index);


/*===========================================================================

FUNCTION:  tdsgapmgr_remove_idle_interval_gap

DESCRIPTION:
  This function remove the subframes index*2 and index*2+1 of the Idle Interval
  bits. 
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_remove_idle_interval_in_ts_source(uint8 index);

/*===========================================================================

FUNCTION:  tdsgapmgr_idleinterval_config_cleanup

DESCRIPTION:
  This function cleans the Idle Interval configuration database.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_idleinterval_config_cleanup(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_idleinterval_config_for_gap_cleanup

DESCRIPTION:
  This function cleans the Idle Interval configuration for gap generation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_idleinterval_config_for_gap_cleanup(void);



#endif

#if defined(FEATURE_TDSCDMA_DMO_SUPPORT)
/*===========================================================================

FUNCTION:  tdsgapmgr_dmo_configed

DESCRIPTION:
  This function checks whether DMO is configured.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsgapmgr_dmo_configed(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_new_dmo_configed

DESCRIPTION:
  This function checks whether new DMO is configured.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsgapmgr_new_dmo_configed(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_update_dmo_for_gap

DESCRIPTION:
  This function allows the DMO Interval handling to update the Idle
  Interval configuration for gap identification. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_update_dmo_for_gap(tdsgapmgr_dmo_pattern_for_gap_struct new_config, uint8 ptn_id);

/*===========================================================================

FUNCTION:  tdsgapmgr_check_n_apply_dmo_config

DESCRIPTION:
  This function belongs to the DMO Handling component.

  This function will check whether the Idle Interval configuration is changed or not. If yes, it will 
  apply the pending DMO configuration at configured_sfn+2.  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_check_n_apply_dmo_config(uint8 ptn_id);

/*===========================================================================

FUNCTION:  tdsgapmgr_set_dmo_gap

DESCRIPTION:
  This function set dmo gap to the radio frame at index. 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/     
void tdsgapmgr_set_dmo_gap(uint8 sfn_idx, uint8 ptn_id, uint8* dmo_ts_mapping, uint16 mp_mapping);

/*===========================================================================

FUNCTION:  tdsgapmgr_remove_dmo_gap

DESCRIPTION:
  This function remove the subframes index*2 and index*2+1 of the dmo
  bits. 
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_remove_dmo_gap(uint8 sfn_idx);


/*===========================================================================

FUNCTION:  tdsgapmgr_identify_DMO_gaps

DESCRIPTION:
  This function identified the DMO gaps in the next few radio frames.  
 
  This function will move forward 1 radio frame and update DMO gaps
  based on current DMO configuration.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_identify_dmo_gaps(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_dmo_config_cleanup

DESCRIPTION:
  This function cleans the DMO configuration.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_dmo_config_cleanup(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_dmo_config_for_gap_cleanup

DESCRIPTION:
  This function cleans the DMO configuration for gap generation.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_dmo_config_for_gap_cleanup(void);

#endif

/*===========================================================================
FUNCTION  TDSGAPMGR_SCHED_GAP_TIME_SLOT_USAGE_UPDATE

DESCRIPTION:
  This function computes and updates the time slot usage in the CM gap for LTE  

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_scheduled_gap_time_slot_usage_update(
     uint8 subfm_start_idx, uint8 gap_len, uint8 ts_start_idx, uint8 ts_end_idx,
     tdssrchmeas_ts_usage_state_enum_type ts_usage, 
     tdsgapmgr_idle_ts_source_enum ts_source_bitmask_update,
     tdsgapmgr_idle_ts_source_enum ts_source_bitmask_cleanup);


/*===========================================================================
FUNCTION:  tdsgapmgr_query_dmo_idleinterval_gaps_by_subfn

DESCRIPTION:
  This function queries the timeslot mapping of combined DMO/Idle Interval gaps
  for the sub-frame indicated by sub_frame. 
 
  The return value is one uint8 value, in which each bit indicates a timeslot. 

  | X | X | X | X | X | X | X | X |   
   ts6 ts5 ts4 ts3 ts2 ts1 spt ts0     
      subframe                           
 
      spt:   special slot between ts0 and ts1.
      X:0:   not a DMO or Idle Interval timeslot
      X:1:   a DMO or Idle Interval timeslot
 
  Assumptions: 1) sub_frame should be larger than current sub-frame number. 
               2) sub_frame should be less than current sub-frame number + TDSGAPMGR_SUBFR_WIN_SIZE;              
 
DEPENDENCIES
  None.

RETURN VALUE
  time slot mapping 

SIDE EFFECTS
  None
===========================================================================*/
uint8 tdsgapmgr_query_dmo_idleinterval_gaps_by_subfn(
  //sub-frame number to be queried
  uint16 sub_frame
);



/*===========================================================================

FUNCTION:  tdsgapmgr_identify_idleinterval_gaps

DESCRIPTION:
  This function identified the Idle Interval gaps in the next few radio frames.  
 
  This function will move forward 1 radio frame and update Idle Interval gaps
  based on Idle Interval configuration.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_identify_idleinterval_gaps(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_combine_natural_ts_with_dmo_idleinterval_n_subfrm

DESCRIPTION:
  This function will combine the natural timeslots usage for next 10 subframes
  based on DMO/Idle Interval gap information.
 
  This function is called after getting natural timeslots and getting
  DMO/Idle Interval timeslots.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_combine_natural_ts_with_dmo_idleinterval_n_subfrm(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_combine_each_subframe

DESCRIPTION:
  This function will combine the natural timeslots usage for next 20 subframes
  based on DMO/Idle Interval gap information.
 
  This function is called after getting natural timeslots and getting
  DMO/Idle Interval timeslots.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_combine_each_subframe(
   uint8 dmo_idleinterval_subfrm_idx,
   uint8 combined_subfrm_idx);

/*===========================================================================
FUNCTION:  tdsgapmgr_query_idleinterval_gaps

DESCRIPTION:
  This function is an API function for DL/UL to query Idle Interval gaps in
  up to 8 consecutive radio frames. Within 8 consecutive radio frames,
  there will be up to 2 Idle Interval gaps.
 
  Assumptions: 1) start_sfn should be no smaller than current SFN;
               2) [start_sfn, (start_sfn+len-1)] should fall into
                  [current SFN, current SFN+9];
               3) len should be between 0 and 8;
               4) gap1 and gap2 are two pointers to store the SFN of
                    first and second Idle Interval gaps.
               5) This API will be used when in the first stage
                  when only Idle Interval is configured.
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_query_idleinterval_gaps(
 /*start_sfn*/
 uint16 start_sfn, 
 /*len should be between 0 and 8*/
 uint8 len, 
 /*Number of gaps within start_sfn and (start_sfn+len-1)*/
 uint8 *num_gaps,
 /*SFN of first idle interval gap; if num_gaps = 0, ignore this parameter*/
 uint16 *gap1, 
 /*SFN of second idle interval gap; if num_gaps<2, ignore this parameter*/
 uint16 *gap2);

/*===========================================================================
FUNCTION:  tdsgapmgr_query_dmo_idleinterval_gaps

DESCRIPTION:
  This function queries the timeslot mapping of combined DMO/Idle Interval gaps
  for the radio frames from start_sfn to start_sfn+len-1.
 
  For each radio frame, one uint16 element in the array ts_mapping[] will
  show whether each timeslot is configured as a DMO/Idle Interval or not.
  | X | X | X | X | X | X | X | X |   | X | X | X | X | X | X | X | X |
   ts6 ts5 ts4 ts3 ts2 ts1 spt ts0     ts6 ts5 ts4 ts3 ts2 ts1 spt ts0
      subframe1                           subframe0
 
      spt:   special slot between ts0 and ts1.
      X:0:   not a DMO or Idle Interval timeslot
      X:1:   a DMO or Idle Interval timeslot
 
  Assumptions: 1) start_sfn should be no smaller than current SFN;
               2) [start_sfn, (start_sfn+len-1)] should fall into
                  [current SFN, current SFN+9];
               3) len should be between 1 and 8;
               4) ts_mapping[] should have the same size as len;
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_query_dmo_idleinterval_gaps(
  //start radio frame
  uint16 start_sfn,   
  //length of radio frames (1 -- 8)
  uint8 len,
  //the array to store the timeslot mapping (size should be the same as len)      
  uint16 ts_mapping[]  
  );


/*===========================================================================

FUNCTION:  tdsgapmgr_chan_update_event_handler

DESCRIPTION:
  This function is called at ts0 boundary of each sub-frame. 
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_chan_update_event_handler(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_tfw_channel_update_handler

DESCRIPTION:
  This function will be registered in T-SEQ. It will be invoked at ts0 of
  the second sub-frame within a radio frame.
 
  At radio frame N, this function will query the DMO/Idle Interval gaps in radio
  frame N+1. If any timeslots in frame N+1 are configured as DMO/Idle Interval
  timeslots, this function will send a command to TFW.
 
  Otherwise, it will not send this command to TFW. 
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_tfw_channel_update_handler(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_send_timeslot_mask_to_TFW

DESCRIPTION:
  This function will send the timeslot mask for one SFN to TFW. 
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_send_timeslot_mask_to_TFW(uint16 gap_mask, uint16 activation_sfn);

/*===========================================================================
FUNCTION        tdsgapmgr_check_any_new_channel_config

DESCRIPTION     This function checks whether there is any new channel 
                configuration.


DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern boolean tdsgapmgr_check_any_new_channel_config(void); 

/*===========================================================================
FUNCTION        tdsgapmgr_get_ts_usage_dpchul

DESCRIPTION     This function identifies the timeslot usage based on DPCH UL 
                configuration.
                new_chan_config == TRUE: recalculate each sub-frame;
                Otherwise:               only calculate the last 2 sub-frames;

                Assumption: This function will be called before any other chann
                            update function is called. 
DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void tdsgapmgr_get_ts_usage_dpchul(boolean new_chan_config, uint16 start_subfn);

/*===========================================================================
FUNCTION        tdsgapmgr_get_ts_usage_dpchdl

DESCRIPTION     This function identifies the timeslot usage based on DPCH DL 
                configuration.
                new_chan_config == TRUE: recalculate each sub-frame;
                Otherwise:               only calculate the last sub-frame;

                Assumption: This function will be called after DPCH UL is
                            updated.  
DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void tdsgapmgr_get_ts_usage_dpchdl(boolean new_chan_config, uint16 start_subfn); 

/*===========================================================================
FUNCTION        tdsgapmgr_subframe_not_scheduled

DESCRIPTION     This function check whether no measurement is scheduled on one 
                subframe. 

DEPENDENCIES    

RETURN VALUE    TRUE: no measurement is scheduled for this sub-frame;

SIDE EFFECTS    
===========================================================================*/
extern boolean tdsgapmgr_subframe_not_scheduled(uint8 subframe_idx);

/*===========================================================================
FUNCTION        tdsgapmgr_convert_slotusage_bitmap_into_enum

DESCRIPTION     This function converts the timeslot usage for N subframes into 
                enums. 

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void tdsgapmgr_convert_slotusage_bitmap_into_enum(boolean new_chan_config); 

/*===========================================================================
FUNCTION        tdsgapmgr_get_ts_usage_uppch_fpach

DESCRIPTION     This function identifies the timeslot marked by uppch and fpach. 
                UpPCH and FPACH configurations are only effective for a certain
                period. 


DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void tdsgapmgr_get_ts_usage_uppch_fpach(boolean new_chan_config, uint16 start_subfn); 

/*===========================================================================
FUNCTION        tdsgapmgr_get_slotusage_in_N_subframes

DESCRIPTION     This function identifies the natural idle timeslots in N (20) 
                sub-frames.


DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void tdsgapmgr_get_ts_usage_prach(boolean new_chan_config, uint16 start_subfn);

/*===========================================================================
FUNCTION        tdsgapmgr_get_slotusage_in_N_subframes

DESCRIPTION     This function identifies the natural idle timeslots in N (20) 
                sub-frames.


DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
extern void tdsgapmgr_get_slotusage_in_N_subframes(void); 


/*===========================================================================
FUNCTION        tdsgapmgr_natural_ts_cleanup

DESCRIPTION     This function cleans the natural timeslots from channel 
                activities. 


DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
void tdsgapmgr_natural_ts_cleanup(void);

/*===========================================================================
FUNCTION        tdsgapmgr_gap_source_cleanup

DESCRIPTION     This function cleans the gap source information.

DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
void tdsgapmgr_gap_source_cleanup(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_check_erucch_config

DESCRIPTION:
   This function will check for the new E-RUCCH configuration and update the E-RUCCH configuration. 
   
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_check_erucch_config(
uint16  start_sfn);

/*===========================================================================

FUNCTION:  tdsgapmgr_find_dmo_with_biggest_gap

DESCRIPTION:
  This function finds the DMO configuration with biggest DMO gap size and return
  it. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_find_dmo_with_biggest_lte_gap(uint16 *p_periodicity,uint16 * p_m_length);

/*===========================================================================
FUNCTION        tdsgapmgr_erucch_overide_dmo_idleinterval

DESCRIPTION     This function overrides the DMO/Idle Interval timeslots with the E-RUCCH timeslot usage. 


DEPENDENCIES    

RETURN VALUE    None

SIDE EFFECTS    
===========================================================================*/
void tdsgapmgr_erucch_overide_dmo_idleinterval
(
//sub-frame offset
uint16 offset, 
//bitmap of the timeslot usage of PRACH/UpPCH/FPACH 
uint16 bit_map
);

/*===========================================================================
FUNCTION:  tdsgapmgr_idleinterval_dmo_active

DESCRIPTION:
 This function will return true if any of below conditions are met:
    1) Idle Interval config pending;
    2) Idle Interval is active;
    3) DMO config is pending;
    4) DMO config is active; 
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsgapmgr_idleinterval_dmo_active(void);
/*---------------------------------------Continous Gaps---------------------------*/

/*===========================================================================

FUNCTION:  tdsgapmgr_find_vacant_continuous_gap_pattern

DESCRIPTION:
  This function will check find the idx to the first vacant gap pattern data structure.  

DEPENDENCIES
  None.

RETURN VALUE
  idx of the first vacant gap pattern data structure.  

SIDE EFFECTS
  None
===========================================================================*/
uint8 tdsgapmgr_find_vacant_continuous_gap_pattern(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_start_configured_gap_pattern

DESCRIPTION:
  This function will start one continuous gap pattern for a specific purpose.
  activate_subsfn: The sub-frame number when the gap starts.
  start_ts_idx:    the first timeslot of the first sub-frame of the gap.
                   0: ts0
                   1: ts1
                   ..
                   6: ts6
  gap_pattern_id:  ID of this particular gap pattern
  gap_purpose:     Purpose of the gap pattern (Now only QTA is supported)

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:   gap pattern successfully started.
  FALSE:  gap pattern cannot be started. 

SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsgapmgr_start_configured_gap_pattern(
   uint16 activate_subsfn, 
   uint8 start_ts_idx, 
   uint8 gap_pattern_id,
   tdsgapmgr_configured_gap_purpose_enum gap_purpose 
);

/*===========================================================================

FUNCTION:  tdsgapmgr_stop_configured_gap_pattern

DESCRIPTION:
  This function will stop one continuous gap pattern previously configured
  for a specific purpose.
 
  activate_subsfn: The sub-frame number when the gap ends.
  end_ts_idx:      the last timeslot of the last sub-frame of the gap.
                   0: ts0
                   1: ts1
                   ..
                   6: ts6
  gap_pattern_id:  ID of this particular gap pattern to be stopped
  gap_purpose:     Purpose of the gap pattern (Now only QTA is supported)

DEPENDENCIES
  None.

RETURN VALUE
  TRUE:   gap pattern successfully stopped.
  FALSE:  gap pattern cannot be stopped. 

SIDE EFFECTS
  None
===========================================================================*/
extern boolean tdsgapmgr_stop_configured_gap_pattern(
   uint16 activate_subsfn, 
   uint8 end_ts_idx, 
   uint8 gap_pattern_id,
   tdsgapmgr_configured_gap_purpose_enum gap_purpose 
);

/*===========================================================================
FUNCTION:  tdsgapmgr_any_active_continuous_gap_pattern

DESCRIPTION:
  This function checks whether there is any Continuous Gap Pattern which is not IN-ACTIVE. 
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsgapmgr_any_active_continuous_gap_pattern(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_new_continuous_gap_pattern

DESCRIPTION:
  This function checks whether there is any new Continuous Gap Pattern or not. . 
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsgapmgr_new_continuous_gap_pattern(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_identify_continuous_gaps_per_pattern

DESCRIPTION:
  This function identifies each configured continous gaps. 
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_set_continuous_gap_subframe(
  uint8 sub_fn_idx, 
  uint8 start_ts_idx,
  uint8 end_ts_idx, 
  uint16 ts_gap_type);

/*===========================================================================
FUNCTION        tdsgapmgr_reset_continuous_gap_patterns

DESCRIPTION
  This function resets the Continuous Gap Patterns

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void tdsgapmgr_reset_continuous_gap_patterns(void);

/*===========================================================================

FUNCTION:  tdsgapmgr_identify_continuous_gaps_per_pattern

DESCRIPTION:
  This function identifies each configured continous gaps. 
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_identify_continuous_gaps_per_pattern(
uint8 pattern_idx,
boolean start_over);

/*===========================================================================

FUNCTION:  tdsgapmgr_identify_configured_continuous_gaps

DESCRIPTION:
  This function identifies each configured continous gaps. 
    
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_identify_configured_continuous_gaps(void);

/*======================================================================
FUNCTION tdsgapmgr_check_gap_config_conflicts

DESCRIPTION
    This function checks whether the start time and end time of one Continous Gap conflicts with another type of 
    Continuous Gap or not.
     
RETURN VALUE
    TRUE: conflicting;
    FALSE: not conflicting;

=======================================================================*/
boolean	tdsgapmgr_check_gap_config_conflicts(
uint16 gap_type1_start_subfn, 
uint16 gap_type1_end_subfn, 
tdsgapmgr_configured_gap_purpose_enum gap_purpose_type2
);

/*======================================================================
FUNCTION tdsgapmgr_registered_client_gap_start_conflict

DESCRIPTION
    This function check whether there is a QTA gap start at the current sfn.
     
RETURN VALUE
    True
    False

=======================================================================*/
boolean tdsgapmgr_registered_client_gap_start_conflict(uint16 offset);

/*#ifdef FEATURE_TDSCDMA_DSDS_QTA*/
/*======================================================================
FUNCTION tdsgapmgr_check_n_set_qta_gaps

DESCRIPTION
    This function check whether QTA gap can be configured in Gap Manager or not. If so, QTA gap start will be 
    registered in Gap Manager.
    QTA gap cannot be opened if 1) 
     
RETURN VALUE
    void

=======================================================================*/
extern boolean tdsgapmgr_check_n_set_qta_gaps(int64    ms_remain_for_qta);

/*======================================================================
FUNCTION tdsgapmgr_increment_qta_gap_pattern_id

DESCRIPTION
    This function increases and rounds the QTA gap ID. 
     
RETURN VALUE
    void

=======================================================================*/
extern void tdsgapmgr_increment_qta_gap_pattern_id(void);

/*======================================================================
FUNCTION tdsgapmgr_get_qta_gap_pattern_id

DESCRIPTION
    This function increases and rounds the QTA gap ID. 
     
RETURN VALUE
    void

=======================================================================*/
extern uint8 tdsgapmgr_get_qta_gap_pattern_id(void);

/*===========================================================================
FUNCTION:  tdsgapmgr_query_dmo_idleinterval_gaps_by_subfn_exclude_qta

DESCRIPTION:
  This function queries the timeslot mapping of combined DMO/Idle Interval gaps
  for the sub-frame indicated by sub_frame. If QTA is on for this sub-frame, 
  DMO or idle Interval information will be reset. 
 
  The return value is one uint8 value, in which each bit indicates a timeslot. 

  | X | X | X | X | X | X | X | X |   
   ts6 ts5 ts4 ts3 ts2 ts1 spt ts0     
      subframe                           
 
      spt:   special slot between ts0 and ts1.
      X:0:   not a DMO or Idle Interval timeslot
      X:1:   a DMO or Idle Interval timeslot
 
  Assumptions: 1) sub_frame should be larger than current sub-frame number. 
                        2) sub_frame should be less than current sub-frame number + TDSGAPMGR_SUBFR_WIN_SIZE;              
 
DEPENDENCIES
  None.

RETURN VALUE
  time slot mapping 

SIDE EFFECTS
  None
===========================================================================*/
extern uint8 tdsgapmgr_query_dmo_idleinterval_gaps_by_subfn_exclude_qta
(
  //sub-frame number to be queried
  uint16 sub_frame
);


/*===========================================================================
FUNCTION:  tdsgapmgr_query_dmo_idleinterval_gaps_exclude_qta

DESCRIPTION:
  This function queries the timeslot mapping of combined DMO/Idle Interval gaps
  for the radio frames from start_sfn to start_sfn+len-1. 

  If one timeslot is marked as QTA, the DMO/Idle Interval information will be reset. 
 
  For each radio frame, one uint16 element in the array ts_mapping[] will
  show whether each timeslot is configured as a DMO/Idle Interval or not.
  | X | X | X | X | X | X | X | X |   | X | X | X | X | X | X | X | X |
   ts6 ts5 ts4 ts3 ts2 ts1 spt ts0     ts6 ts5 ts4 ts3 ts2 ts1 spt ts0
      subframe1                           subframe0
 
      spt:   special slot between ts0 and ts1.
      X:0:   not a DMO or Idle Interval timeslot
      X:1:   a DMO or Idle Interval timeslot
 
  Assumptions: 1) start_sfn should be no smaller than current SFN;
               2) [start_sfn, (start_sfn+len-1)] should fall into
                  [current SFN, current SFN+9];
               3) len should be between 1 and 8;
               4) ts_mapping points to an array which at least is size len;
               5) ts_mapping[0] indicates radio frame start_sfn;
               6) ts_mapping[len-1] indicates radio frame (start_sfn+len-1);
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdsgapmgr_query_dmo_idleinterval_gaps_exclude_qta(
  //start radio frame
  uint16 start_sfn,   
  //length of radio frames (1 -- 8)
  uint8 len,
  //the pointer to an array to store the timeslot mapping 
  //(size should be the same as len)      
  uint16 *ts_mapping  
  );

/*#endif*/

#if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT) || defined(FEATURE_TDSCDMA_60MS_GAP_FOR_BSIC_OPT)
/*===========================================================================
FUNCTION  TDSGAPMGR_CM_MEAS_CLIENT_GAP_POLICYMGR

DESCRIPTION:
  This function judges T/G/L CM measurement priority

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_cm_meas_gap_policy_mgr(void);

/*===========================================================================
FUNCTION  tdssrchlte_cm_meas_cm_meas_gap_policy_update_timer

DESCRIPTION:
  This function will update the timer of twait for the gap policy, in unit of 5ms.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void tdsgapmgr_cm_meas_gap_policy_update_timer(uint16 delta);


#endif
#ifdef FEATURE_TDSCDMA_L1_LOGGING_ENABLED
/*======================================================================
FUNCTION tdsgapmgr_log_eutra_idle_interval_config_update

DESCRIPTION
    This function will log the idle interval information configured by network. 
     
RETURN VALUE
    void

=======================================================================*/
void tdsgapmgr_log_eutra_idle_interval_config_update(void);

/*======================================================================
FUNCTION tdsgapmgr_log_eutra_dmo_config_update

DESCRIPTION
    This function will log the DMO information configured by network. 
     
RETURN VALUE
    void

=======================================================================*/
void tdsgapmgr_log_eutra_dmo_config_update(void);


#endif

#ifdef FEATURE_TDSCDMA_TO_LTE_FORCED_GAP
extern void tdsgapmgr_calculate_dmo_blocked_by_ta_count(void);
boolean tdsgapmgr_check_subfn_in_dmo_gap(uint16 cur_subfn);

void tdsgapmgr_reset_t2l_forced_gap(void);
#endif


#endif  //FEATURE_TDSCDMA_CM_LTE_MEASUREMENT

#endif /* TDS_GAP_MANAGER_H */

