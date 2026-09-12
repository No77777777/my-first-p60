#ifndef WL1IDLETAMGR_H
#define WL1IDLETAMGR_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               WL1 IDLE MODE TUNEAWAY MANAGER

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2000-2007 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1idletamgr.h#3 $ 
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/10/20   ac      Mini dump framework checkin
10/12/15   skk     Added a timer to declare OOS in idle mode if lock is not available for 12sec.
07/15/15   pv      Pass the SUB ID for DR query API.  
04/21/15   bj      Split ACQ changes for W+1x. 
05/29/15   pv      Remove API to wakeup SPLIT SRCH entity in G2W TA mode. 
                   Check for NO LOCK state to allow SPLIT SRCH wakeup.
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
02/27/15   ap      Initial check-in for WL1 QSH framework 
02/20/15   skk     Trigger DL WEAK IND if L1 doesnt get lock for 2 sec during cell selectoin. 
04/20/15   dm      Acq priority inversion for TRM unification.
03/09/15   sks     Mods for full sleep in G2W TA mode.
02/28/15   rsr     W + W Phase 2 Checkin.
02/24/15   pv      DRX/WSLEEP code reorg for W+W
02/06/15   rs      Code changes for requesting lock with new proc during BCH decode
01/19/15   sks     Global re-org for W+W
11/25/14   skk     Set sleep time for BCCH in no lock cases following a geometric progression.
12/02/14   ar      Modify the return type for lock req API
10/21/14   kr/skk  DSDS:call rf tune to multichain in gts fail case
09/10/14   skk     If sleep time is invalid in inter f resel, make it 4.5ms.
08/20/14   skk     Ignore pended cmd if UE is paged when G is in data call.
07/28/14    skk     Handle pended ext cmd by setting ext_cmd_sig again.
04/07/14    rs     API to clear wl1idletamgr_ext_cntrl_params[wsub_id].wl1idle_ta_mgr_wrm_rf_tune_needed flag 
05/06/14   as      Moving WRM to WL1TRM.
03/06/14   gsk     Ignore PICH wakeups during interf reselection
02/28/14   skk     Handle the lock_not_rel timer expiry in WL1 task context.
12/03/13   skk     Dont pend any ext cmd in G2W_TA_ENTER state.
12/16/13   as      Porting G2W TA from Triton to Dime. 
09/30/13   pv      Enable all G2W TA components
09/15/13   pv      G2W TA idle mode framework     
=============================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "wl1drxmanager.h"
#include "wl1drxentity.h"
#include "wl1multisim.h"



#ifdef FEATURE_DUAL_SIM

/*===========================================================================
                     ENUMS AND MACROS
===========================================================================*/
typedef enum
{
  SRCH_W,
  SRCH_GSM,
  SRCH_LTE
} srch_type;

typedef struct
{
  uint16 freq;    /* Will have valid value only for non-IRAT searches.*/
  srch_type srch_due_next_gap; 
} drx_next_srch_info_type;

/* This will be called by DRX for deciding on the next gap.*/
boolean srchzz_any_srches_due(drx_next_srch_info_type*);


/* DRX sleep modes
** DRX_SLEEP_REGULAR_MODE - normal mode, true sleep, FW/RF sleep 
                            MCPM call and sleepctl programming.
** DRX_SLEEP_G2W_TA_MODE  - G2W TA mode, no true sleep, no RF control.
*/
typedef enum
{
  DRX_SLEEP_REGULAR_MODE,
  DRX_SLEEP_TA_MODE,
  DRX_NUM_SLEEP_MODES
} drx_sleep_mode_type;

typedef enum
{
  /*Not in G2W TA mode.*/
  DRX_G2W_TA_INACTIVE,
  
  /*In G2W TA mode.*/
  DRX_G2W_TA_ACTIVE,
  
  /*During G2W TA TRM lock is released during offline NASTT 
    to allow other Tech to re establish its data call.*/
  DRX_G2W_TA_OFFLINE_ACTIVITY_PENDING,
  
  DRX_G2W_TA_NUM_STATES
} drx_g2w_ta_state_type;

/* ENUMs for various notifications that can be sent to WL1IDLE_TA_MGR.*/
typedef enum
{
  IDLE_TA_MGR_NO_RESOURCE_NOTF,
  IDLE_TA_MGR_EXT_CMD_PENDING_NOTF,
  IDLE_TA_MGR_STEP1_DONE_OFFLINE_NASTT_PENDING_NOTF,
  IDLE_TA_MGR_NUM_NOTF
} wl1idle_ta_mgr_notf_type;

typedef struct
{
   uint32 wrm_g2w_ta_l1_trm_resrv_sclk;
   boolean wrm_split_acq_offline_nastt_in_progress;
}wl1idle_ta_mgr_wrm_db_type;

/* G2W TA database structure.*/
typedef struct
{
   boolean lock_status;
   uint32  num_g2w_ta_gaps;
   uint32  total_g2w_ta_gaps;
   wl1idle_ta_mgr_wrm_db_type wrm_db;

} wl1idle_ta_mgr_db_type;


typedef struct
{
  wl1idle_ta_mgr_db_type wl1_g2w_ta_db;

  boolean wl1idle_ta_mgr_wrm_rf_tune_needed;

  drx_sleep_mode_type drx_sleep_mode;

  drx_g2w_ta_state_type drx_g2w_ta_state;
  
}wl1idletamgr_ext_cntrl_params_type;

extern wl1idletamgr_ext_cntrl_params_type wl1idletamgr_ext_cntrl_params[WCDMA_NUM_SUBS];

typedef enum
{
  WL1IDLE_TA_MGR_EXT_CMD_NOT_APPLICABLE,
  WL1IDLE_TA_MGR_EXT_CMD_LOCK_WITH_GPRS_LTA_G2W_TA_DRX_END, // DRX abort condition in PCH_SLEEP
  WL1IDLE_TA_MGR_EXT_CMD_LOCK_NOT_REQD_G2W_TA_CONT, // Do not abort in PCH_SLEEP condition
  WL1IDLE_TA_MGR_EXT_CMD_LOCK_WITH_GPRS_LTA,
  WL1IDLE_TA_MGR_EXT_CMD_LOCK,
  WL1IDLE_TA_MGR_EXT_CMD_LOCK_NOT_REQD,
  WL1IDLE_TA_MGR_EXT_CMD_LOCK_NOT_REQD_G2W_TA_DRX_END, // DRX abort condition in PCH_SLEEP
  WL1IDLE_TA_MGR_EXT_CMD_LOCK_NOT_REQD_G2W_TA_END,
  WL1IDLE_TA_MGR_EXT_CMD_PROC_DEFAULT,
  NUM_MAX_WL1IDLE_TA_MGR_EXT_CMD_CATEGORY
} wl1idle_ta_mgr_ext_cmd_category_enum_type;


/* Macro to check if WL1 is operating in G2W TA mode.*/
#define WL1_IN_G2W_TA_MODE() ((WL1_IN_DS_MODE()) && \
                              (wl1idletamgr_ext_cntrl_params[wsub_id].drx_sleep_mode == DRX_SLEEP_TA_MODE) && \
                              (wl1idletamgr_ext_cntrl_params[wsub_id].drx_g2w_ta_state != DRX_G2W_TA_INACTIVE)\
                              )

/* Macro to check if Split search mode is allowed if we are in G2W TA mode.*/
#define WL1IDLE_TA_MGR_SPLIT_SRCH_ALLOWED() (!wl1trm_is_ue_in_dr_mode(wsub_id) && \
                                             WL1_IN_G2W_TA_MODE()          \
                                            )

/* Macro that updates if we got lock in G2W TA mode.*/
#define WL1_G2W_TA_SET_LOCK_STATUS()	\
do { 									\
  wl1idletamgr_ext_cntrl_params[wsub_id].wl1_g2w_ta_db.lock_status = TRUE; 	\
} while(0)   


/* Macro that updates if we did not get lock in G2W TA mode.*/
#define WL1_G2W_TA_SET_NO_LOCK_STATUS()	\
do { 									\
  wl1idletamgr_ext_cntrl_params[wsub_id].wl1_g2w_ta_db.lock_status = FALSE; 	\
} while(0)

/* Macro to check if we are in G2W TA mode with no lock.*/
#define WL1_IN_G2W_TA_MODE_WITH_NO_LOCK() (WL1_IN_G2W_TA_MODE() && (wl1idletamgr_ext_cntrl_params[wsub_id].wl1_g2w_ta_db.lock_status == FALSE))

/* Macro that returns num of G2W TA gaps.*/
#define WL1_IDLE_TA_MGR_INC_NUM_TA_GAPS() \
do {\
	wl1idletamgr_ext_cntrl_params[wsub_id].wl1_g2w_ta_db.num_g2w_ta_gaps ++;\
} while (0)

/* Macro to reset no.of G2W TA gaps.*/
#define WL1_IDLE_TA_MGR_RESET_NUM_TA_GAPS() \
do {\
	wl1idletamgr_ext_cntrl_params[wsub_id].wl1_g2w_ta_db.num_g2w_ta_gaps = 0;\
} while (0)

/* Macro to increment total no.of G2W TA gaps.*/
#define WL1_IDLE_TA_MGR_INC_TOTAL_TA_GAPS() \
do {\
	wl1idletamgr_ext_cntrl_params[wsub_id].wl1_g2w_ta_db.total_g2w_ta_gaps ++;\
} while (0)

/* Macro to get the no.of G2W TA gaps.*/
#define WL1_IDLE_TA_MGR_GET_NUM_TA_GAPS() (wl1idletamgr_ext_cntrl_params[wsub_id].wl1_g2w_ta_db.num_g2w_ta_gaps)

/* Macro to check if lock was released and Offline activity happening.*/
#define WL1_IDLE_TA_MGR_OFFLINE_ACTIVITY_PENDING() (wl1idletamgr_ext_cntrl_params[wsub_id].drx_g2w_ta_state == DRX_G2W_TA_OFFLINE_ACTIVITY_PENDING)

/*=========================================================================== 
                              FUNCTION EXTERNS 
===========================================================================*/

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_CHK_G2W_TA_MODE_ENTRY

DESCRIPTION
  This function queries WL1TRM if G2W TA mode entry is needed.

DEPENDENCIES
  None.

PARAMETERS
  None.
 
RETURN VALUE
  boolean - TRUE if entry is needed, FALSE otherwise.
===========================================================================*/
extern boolean wl1idle_ta_mgr_chk_g2w_ta_mode_enter( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_CHK_G2W_TA_MODE_EXIT

DESCRIPTION
  This function queries WL1TRM if G2W TA mode exit is needed.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean - TRUE if exit is needed, FALSE otherwise.
===========================================================================*/
extern boolean wl1idle_ta_mgr_chk_g2w_ta_mode_exit( wsub_id_e_type wsub_id);

extern void wl1idle_ta_mgr_check_schedule(drx_entity_handle_type entity_woken_up);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_G2W_TA_CONFIG_RF_FW_AT_WAKEUP

DESCRIPTION
  This function brings up FW and RF at the start of the gap in G2W TA mode.

DEPENDENCIES
  None.
 
PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_g2w_ta_config_rf_fw_at_wakeup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_MODIFY

DESCRIPTION
  This function is used by other modules to notify WL1IDLE_TA_MGR.
 
DEPENDENCIES
  None.

PARAMETERS
  Type of notification, DRX ENTITY handle.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_notify(wl1idle_ta_mgr_notf_type notf,drx_entity_handle_type handle, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_PREP_FOR_EXT_CMD_HANDLE

DESCRIPTION
  This function is called by WRM whenever G2W TA mode active and we get an
  external cmd.
 
DEPENDENCIES
  None.

PARAMETERS
  The WRM ext cmd category.

RETURN VALUE
  Boolean - TRUE if need to pend WRM to handle ext cmd, FALSE otherwise.
===========================================================================*/
extern boolean wl1idle_ta_mgr_prep_for_ext_cmd_handle(wl1idle_ta_mgr_ext_cmd_category_enum_type wrm_g2w_ta_mode_curr_ext_cmd_category, wsub_id_e_type wsub_id);

extern boolean wl1idle_ta_mgr_allow_sleep_all_entities_asleep(void);


extern void wl1idle_ta_mgr_set_curr_rf_freq( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_SET_RF_WARMUP_STATUS

DESCRIPTION
  This function sets the RF warmup status in G2W TA mode.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Boolean - TRUE if sleep can be allowed, FALSE otherwise.
===========================================================================*/
extern void wl1idle_ta_mgr_set_rf_warmup_status( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_CLR_RF_WARMUP_STATUS

DESCRIPTION
  This function clears the RF warmup status.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Boolean - TRUE if sleep can be allowed, FALSE otherwise.
===========================================================================*/
extern void wl1idle_ta_mgr_clr_rf_warmup_status( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_ADJ_PICH_WAKEUP_TIME

DESCRIPTION
  This function adjusts PICH wakeup time if needed in G2W TA mode.
 
DEPENDENCIES
  None.

PARAMETERS
  Pointer to the target time for wakeup.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_adj_pich_wakeup_time(int32 *target_time, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_REL_RESOURCES_POST_OFFLINE_SRCH

DESCRIPTION
  This function takes all necessary actions post an Offline search.
  Puts FW to sleep and SUSPEND and resets variables.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_rel_resources_post_offline_srch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_REL_RESOURCES_FOR_OFFLINE_SRCH

DESCRIPTION
  This function puts RF to sleep and releases the TRM lock since only Offline
  search is pending in this gap.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_rel_resources_pre_offline_srch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_WAIT_FOR_FW_RSP

DESCRIPTION
  This function posts a local cmd when cell/fing decfg is done after ACQ success.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_wait_for_fw_rsp(void);

extern drx_g2w_ta_state_type get_g2w_ta_state( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_G2W_TA_WRM_RESERVE_PENDING

DESCRIPTION
  This function returns if a TRM reservation for lock is pending within the specified
  number of frames.(1 frame = 10ms).

DEPENDENCIES
  None.

PARAMETERS
  uint32 reserve_chk_frames

RETURN VALUE
  Boolean.
===========================================================================*/
extern boolean wl1idle_ta_mgr_g2w_ta_wrm_reserve_pending(uint32 reserve_time_chk_frames, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_LOCK_NOT_REL_TMR_EXPIRED_CB

DESCRIPTION
  This is the callback for the timer expiry when we keep lock for long time,
  specified by WL1IDLE_TA_MGR_NO_LOCK_REL_TIMER_VAL_MS.

DEPENDENCIES
  None.

PARAMETERS
  uint32 but unused.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_lock_not_rel_tmr_expired_cb(timer_cb_data_type cb_data);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_LOCK_NOT_REL_TMR_EXPIRED_CB_HANDLER

DESCRIPTION
  This function is called in as a part of local cmd handling of
   the lock_not_rel_timer expiry

DEPENDENCIES
  None.

PARAMETERS
None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_lock_not_rel_tmr_expired_cb_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_G2W_TA_REL

DESCRIPTION
  This function is called from drx_rel() whenever we are moving out
  of PCH_SLEEP. 

DEPENDENCIES
  None.

PARAMETERS
None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_g2w_ta_rel( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1IDLE_TA_MGR_RELEASE_TRM_SPLIT_ACQ

DESCRIPTION
  This function is used by searcher to release TRM lock in split acq mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_release_trm_split_acq(wrm_primary_client_enum_type wrm_client_id,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1IDLE_TA_MGR_COMPLETE_SPLIT_ACQ_OFFLINE_NASTT_OPS

DESCRIPTION
  This function is used by WL1 to notify WRM that offline NASTT in split acq mode
  is complete.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_complete_split_acq_offline_nastt_ops( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1IDLE_TA_MGR_RELEASE_TRM

DESCRIPTION
  This function is used by WL1 idle mode scheduler to release lock for G2W TA

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_release_trm(wrm_primary_client_enum_type wrm_client_id, wsub_id_e_type wsub_id);
extern void wl1idletamgr_ext_cntrl_params_init( wsub_id_e_type wsub_id);
extern void wl1idletamgr_int_cntrl_params_init( wsub_id_e_type wsub_id);
extern void wl1idleta_globl_params_init(void);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_SET_G2W_TA_STATE

DESCRIPTION
  This function will perform all activities needed to enter the G2W TA mode.
  Update the data structures and internal databases.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void set_g2w_ta_state(drx_g2w_ta_state_type newState, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_REGISTER_SPLIT_SEARCH

DESCRIPTION
   This function registers split_srch in DSDS mode. This will be called 
    from drx_cmd.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1idle_ta_mgr_register_split_search(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_DEREGISTER_SPLIT_SRCH

DESCRIPTION
   This function deregisters split_srch in DSDS mode. This will be called 
    from drx_cmd.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/

extern void wl1idle_ta_mgr_deregister_split_search(wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
extern void wl1_idletamgr_get_debug_info(wl1_drx_sleep_mini_debug_info_struct_type* obj);
#endif

/*===========================================================================
FUNCTION    WL1IDLE_TA_MGR_SET_G2W_TA_STATE

DESCRIPTION
  This function will perform all activities needed to enter the G2W TA mode.
  Update the data structures and internal databases.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
drx_g2w_ta_state_type get_g2w_ta_state(wsub_id_e_type wsub_id);
#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_wl1idletamgr_params

DESCRIPTION
  Wrapper

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_wl1idletamgr_params(wsub_id_e_type wsub_id);
#endif
#endif /* FEATURE_DUAL_SIM */

#endif /* WL1IDLETAMGR_H */