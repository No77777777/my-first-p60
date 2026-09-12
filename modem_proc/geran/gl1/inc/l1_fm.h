#ifndef L1_FM_H
#define L1_FM_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      L A Y E R  1  F R A M E  M A N A G E R

GENERAL DESCRIPTION
   This module contains procedures to support the Neighbor cell activity
   scheduling in Layer 1. Together these functions make up the Frame manager.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/l1_fm.h#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
23/01/18   rv        CR2019399: Define G2T IRAT as LTA activity
03/25/16   zf        CR995061: Check if activity reserved before starting ncell activity
09/09/16   sn        CR1062272 TRM RESERVE AT during IDLE to transfer 
16/05/16   db        CR1009490 Move CBCH priority handling from GRR to GL1
26/26/15   mko       CR941539: After wakeup cancel, add correct FN offset while calculating  gap to next activity
24/11/15   sp        CR924033 Apply RR requested CB priority inversion, only on next TC=0 occassion.
24/04/15   am        CR825716 Delay SCH scheduling after 4 TRM denials in idle mode 
30/09/15   cjl       CR914688 Abort blocked acquisition attempts when leaving idle
07/09/15   am        CR899527 Ensure GL1 wakeup and reserve a high priority task
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
12/08/15   hd        CR888214: Exchange the FM activity if SET_PRIORITY_IND has come to invert and a CBCH activity is already booked
15/07/15   fan/akh   CR857332 FR19329: WMGR support for rescheduling non-PCH activities
08/07/15   df        CR815683 Avoid error recovery when G2W scripts are built during wakeup 
03/06/15   cws       CR846011 Clear all irat fm activity when srch state goes to NULL 
20/05/15   aga       CR840984 NULL2X IRAT changes for G2W IRAT
19/05/15   sjw       CR840271: clear inactive CBCH FM entries when priority is changed
03/02/15   cjl       CR790433 Create separate TRM activity for WCDMA reconfirm when needed
30/03/15   mc        CR773788 : COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
19/02/15   nk        CR787609 Defer TS change if there's a pending command at GFW.
19/02/15   fm        CR791018 GL1 should check SIM mode for NCell SCH Rx before starting it
12/12/14   sk        CR763710 Ensure NBCCH is read when consistently colliding with serving PCH or other sub's PCH
10/12/14   sjv       CR769423 Ensure l1_idle_mode_isr returns TRUE only when it is safe to release TRM 
03/11/14   sjv       CR750056 IRAT activities need to be aborted/cleared when an update_list is received
01/11/14   df        CR748602 Rework and enable adaptive timeline
29/09/14   sk        CR743686: Ensure Frames for the Cb activity are cleared when new scheduling/skip req received
17/10/14   sk        CR743536 Ensure gap to CCCH is updated on rude wakeup before ISR runs
21/10/14   fan       CR734490 Rework the wake up recovery mechanism
12/09/14   fan       CR636420: Run Sleep commit after GSTMR ISR at wakeup
21/02/14   sp        CR635810: Don't release TRM lock for access when the lock is granted after request_and_notify(), improve CR/620698
08/04/14   cs        CR638933 Allow equal priority event to be booked when suspend is set
25/02/14   sk        CR614910 Update G BPLMN handling inline with DSDA-OMRD implementation
21/02/14   sk/smd    CR620698:Don't release TRM lock for access when the lock is granted after request_and_notify()
09/01/14   zf        CR598637: DRX only updates TRM booking if DRX activity is matching with the TRM booking
15/11/13   js        CR572966:Reselections to the Ncell are still failing as SUB2 is not able to decode all the SIs leading to SYS_INFO_TIMER expiry
11/20/13   nk        CR 549654 : Preventing scheduling of bcch state machine when frame manager has aborted that activity
15/10/13   js        CR508118: Send missed block ind if wakeup for CCCH cancelled.
01/10/13   sk        CR552259 CR sync from 1.0/1.1
29/08/13   us      CR519643 Avoid ncell acquisition or delay ncell scheduling when bcch schedule is within 48msec
15/08/13   sk        CR524039 GBTA bringup changes
18/09/13   us      CR519643 Avoid ncell acquisition or delay ncell scheduling when bcch schedule is within 48msec
29/07/13   sk        CR519663 Partial QBTA bringup changes
10/06/13   cs        TSTS Updates
29/10/12   ss        CR396846 : Process Read sys info request only if next BCCH read does not coincides with active G2X measurement
                     scheduled in frame manager
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
20/06/11   cs        Add in CBCH for DSDS using individual priority
29/11/10   og        Adding G2L support.
19/11/10   nt        DSDS Enhancement to SCE scheduling during Tuneaway
10/18/05   sv        Add function prototype to return gap to next ccch\sdcch block.
08/04/05   sv        Add new logging event.
05/23/05   sv        Remove function prototype l1_fm_update_ccch_gap.
04/11/05   sv        Added new function prototype that updates the CCCH gap
                     in Frame Manager.
02/05/05   bm        Increased FM_PRI_BACKGROUND_PLMN priority next to CCCH
01/26/05   sv        Add support for Frame Manager.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif
#include "geran_multi_sim.h"
#include "rr_l1.h"
#include "rr_l1_g.h"

/*****************************************************

 LOCAL DEFINES

******************************************************/
#define QTA_OVERHEAD  11  /*Rounding  QTA overhead 48 ms to 11 FRAMES */
#define NOT_A_FRAME_NUMBER  0xFFFFFF
#define MAX_GAP             ((2048*51*26)-1)

/* Stores the number of frames required for the abort activity are completion of required frames by frame manager */
#define FRAMES_REQUIRED_FM_ABORT 2
#ifdef FEATURE_QTA
#define FM_MAX_EXTENDED_QTA_GAP 37 /* ~170ms to cover the longest PCH + BCCH + Ext BCCH + Ext PCH scenario */
#endif /* FEATURE_QTA */

typedef enum {
   FM_PRI_RESERVED = 0,
   FM_PRI_CCCH,
   FM_PRI_BACKGROUND_PLMN,
   FM_PRI_NCELL_BCCH,
   FM_PRI_CBCH,
   FM_PRI_NCELL_ACQ,
   FM_PRI_NCELL_SCH,
   FM_PRI_WCDMA_RECONF,
   FM_PRI_WCDMA_IDSEARCH,

#ifdef  FEATURE_GSM_TO_LTE
   FM_PRI_LTE_IDSEARCH,
#endif

#ifdef FEATURE_GSM_TDS
   FM_PRI_TDS_ACQ, /*Low Chip Rate TD-SCDMA*/
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   FM_PRI_MSIM_CBCH,
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   FM_PRI_DL_TBF,
   FM_PRI_MAX
} l1_fm_priority_T;

typedef enum {
   FM_STATE_NULL = 0,
   FM_STATE_USING_FRAMES,
   FM_STATE_CB_PENDING
} l1_fm_state_T;

typedef enum {
   EVE_FM_TICK,
   EVE_START_TASK,
   EVE_GET_AVAIL_FRAMES,
   EVE_ABORT_CURR_TASK,
   EVE_ABORT_LP_TASK,
   EVE_CLEAR_FRAMES,
   EVE_SET_REQD_FRAMES
} fm_event_T;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
typedef enum {
   FM_ACTION_NONE,
   FM_ACTION_CLEAR,
   FM_ACTION_CLEAR_ACTIVE_TASK,
   FM_ACTION_SET,
   FM_ACTION_DELAYED_RELEASE_FOR_TUNEAWAY,
   FM_ACTION_CLEAR_AND_SET,
   FM_ACTION_DELAYED_RELEASE_FOR_BPLMN
} action_event_T;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

typedef struct {
   l1_fm_state_T state;
   uint32    required_frames;
   uint32    max_required_frames;
   uint32    reserved_frame_number;
   void (*abort_callback)(void*, gas_id_t gas_id );
   void*     abort_data;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   action_event_T action;
   uint16    fw_load_frames;
   boolean   allowed_but_trm_denial;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   uint32 ustmr_s_time; /* USTMR start time of the activity (OTA) */
   uint32 ustmr_e_time; /* USTMR end time of the activity (OTA) */

} l1_fm_table_element_T;

typedef struct {
   l1_fm_priority_T activity;
   uint32           fn;
   uint32           duration;
   uint32           ustmr_s_time;
   uint32           ustmr_e_time;
} l1_fm_next_activity_T;


/* Two tables now available for use, only actually used in a dual sim build */
typedef struct
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  uint32                suspend_trm_release;
  l1_fm_priority_T      suspend_trm_release_pri;
  uint32                trm_reserved_fn;
  l1_fm_priority_T      trm_reserved_pri;
  trm_reason_enum_t     trm_reserved_reason;
  uint32                trm_reserved_duration;
  boolean               is_irat_activity;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  l1_fm_priority_T      fm_active_pri;
  boolean               leaving_idle_mode;
  uint32                gap_to_ccch_activity;
  l1_fm_table_element_T l1_fm_table[FM_PRI_MAX];

  /* @@OG: Fix to CR127568 */
  boolean               count_to_max_frames[FM_PRI_MAX];
}l1_fm_data_t;
extern l1_fm_data_t  l1_fm_data[NUM_GERAN_DATA_SPACES];



extern l1_fm_priority_T l1_fm_current_activity[NUM_GERAN_DATA_SPACES];

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION l1_fm_find_next_task_to_reserve

DESCRIPTION
  This function looks through the frame manger table to find the next activity
  that will run of type
  FM_PRI_CCCH
  FM_PRI_NCELL_BCCH
  FM_PRI_CBCH
  FM_PRI_NCELL_SCH
  Then this will be booked into TRM

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_find_next_task_to_reserve_or_release(gas_id_t gas_id);

boolean l1_multi_sim_trm_manager( gas_id_t gas_id );

extern void l1_fm_set_suspend_trm_release( uint32 frames,
                                           l1_fm_priority_T suspend_release_event_pri,
                                           gas_id_t gas_id );

uint32 l1_fm_get_suspend_trm_release( gas_id_t gas_id );
trm_reason_enum_t l1_fm_map_pri_to_trm(l1_fm_priority_T pri, gas_id_t gas_id);
void l1_fm_clear_required_frames_after_pch_decode( l1_fm_priority_T pri, gas_id_t gas_id );
boolean l1_fm_check_reserved_trm_activity( gas_id_t gas_id, uint32 frames_required );

void gl1_ms_fm_update_booking_frames( int32 frame_adjust, gas_id_t gas_id );
l1_fm_priority_T l1_get_fm_trm_priority(gas_id_t gas_id );
uint32 l1_fm_get_trm_duration(gas_id_t gas_id );
uint32 l1_fm_get_trm_reserved_fn( gas_id_t gas_id);
boolean l1_is_fm_trm_priority_g2t(gas_id_t gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
boolean l1_fm_any_activity_running( gas_id_t gas_id );
#endif
void l1_fm_set_activity_running(l1_fm_priority_T pri, gas_id_t gas_id);

#ifdef FEATURE_GPRS_GBTA
boolean l1_fm_check_sch_activity_next(gas_id_t gas_id);
#endif  /* FEATURE_GPRS_GBTA */

void l1_fm_reset_pri_res(gas_id_t gas_id);
boolean l1_fm_are_bplmn_frames_available( 
                                    uint32 frames_required,
                                    void ( *abort_callback )( void*, gas_id_t gas_id ),
                                    void* abort_data, gas_id_t gas_id );
boolean l1_fm_allowed_but_blocked_by_trm( l1_fm_priority_T pri, gas_id_t gas_id );
boolean l1_fm_task_in_conflict(l1_fm_priority_T task, uint32 task_fn, gas_id_t gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
l1_fm_priority_T l1_fm_get_active_pri(gas_id_t gas_id);

extern boolean l1_drx_wakeup_cancel_for_CCCH(uint32 reserved_frame_num,gas_id_t gas_id);
extern void l1_drx_set_gap_to_CCCH_on_rude_wakeup( gas_id_t gas_id );

void l1_fm_init( gas_id_t gas_id );
void l1_fm_reset( gas_id_t gas_id );
void l1_fm_tick( uint32 gap_to_next_ccch_activity, gas_id_t gas_id );
void l1_fm_set_leaving_idle_mode( boolean leaving_idle_mode, gas_id_t gas_id );
boolean l1_fm_leaving_idle_mode( gas_id_t gas_id );
boolean l1_fm_prioritise_nbcch_over_ccch(gas_id_t gas_id);

void l1_fm_set_required_frame_number(l1_fm_priority_T pri, uint32 frame_no, uint32 frames_reqd, gas_id_t gas_id);
void l1_fm_clear_required_frames(l1_fm_priority_T pri, gas_id_t gas_id);
void l1_fm_extend_required_frames(l1_fm_priority_T pri, uint32 frames_required, gas_id_t gas_id);

boolean l1_fm_are_frames_available( l1_fm_priority_T pri,
                                    uint32 frames_required,
                                    void ( *abort_callback )( void*, gas_id_t gas_id ),
                                    void* abort_data, gas_id_t gas_id );

uint32 l1_fm_get_available_frames(l1_fm_priority_T pri, uint32 current_frame_number, gas_id_t gas_id);
uint32 l1_fm_get_ccch_gap( gas_id_t gas_id );
#ifdef FRAME_MANAGER_LOGGING
void l1_fm_log_event(l1_fm_priority_T pri, fm_event_T event, gas_id_t gas_id);
#endif
l1_fm_priority_T l1_fm_get_running_activity( gas_id_t gas_id );

l1_fm_priority_T l1_fm_get_active_CBCH_pri( gas_id_t gas_id );
l1_fm_priority_T l1_fm_get_pending_CBCH_pri( gas_id_t gas_id );


extern void l1_clear_inactive_cbch_entries(gas_id_t gas_id);
uint32 l1_fm_get_required_frames_to_complete(l1_fm_priority_T pri,gas_id_t gas_id);
#ifdef FEATURE_GSM_COEX_SW_CXM
l1_fm_priority_T l1_fm_get_next_activity(l1_fm_next_activity_T* next_activity, gas_id_t gas_id);
boolean l1_fm_is_next_activity_IRAT(gas_id_t gas_id);
#endif

uint32 l1_fm_wmgr_rebook_activity(gas_id_t gas_id);
boolean l1_fm_wmgr_cancel_activity(gas_id_t gas_id);
l1_fm_priority_T l1_fm_get_activity_before_fn(l1_fm_priority_T cancelled_activity_pri, uint32 start_fn, gas_id_t gas_id);
l1_fm_priority_T l1_fm_get_other_activity(l1_fm_priority_T pri, gas_id_t gas_id);
void l1_fm_set_CBCH_pri(boolean high_prio, gas_id_t gas_id); 
l1_fm_priority_T l1_fm_updated_CBCH_pri(gas_id_t gas_id);

/* set next activity to start atleast after 4 frames */
#define L1_FM_WMGR_CANCEL_NEXT_ACTIVITY_MIN_GAP   4

/*Add FN offset while calclating the gap to next activity after wakeup cancel*/
#define L1_FM_WMGR_CANCEL_CALC_GAP_TO_NEXT_ACTIVITY_FN_OFFSET   2

l1_fm_state_T l1_fm_get_fm_table_state( gas_id_t gas_id,  l1_fm_priority_T pri);
void l1_fm_clear_irat_activity(gas_id_t gas_id);
boolean l1_fm_is_fn_ccch(uint32 frame_fn, gas_id_t gas_id);

void l1_fm_exchange_cbch_priority(gas_id_t gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) 
void l1_fm_toggle_CBCH_pri(gas_id_t gas_id);
#endif
sys_proc_type_e_type l1_fm_get_reserve_sub_reason(gas_id_t gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

l1_fm_priority_T l1_fm_is_activity_reserved(l1_fm_priority_T pri, gas_id_t gas_id);

#endif /* L1_FM_H */

