#ifndef DRXOFFLINEPROCMGR_H
#define DRXOFFLINEPROCMGR_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         L 1    D R X   OFFLINE PROCESSING  R E L A T E D    D E F I N I T I O N S

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


REFERENCES
  CDMA Mobile Station ASIC Specification sheet.

Copyright (c) 2001-2012 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when        who      what, where, why
--------    ---      ---------------------------------------------------------------- 
01/10/20   ac      Mini dump framework checkin
01/21/16    skk     Pass PI Start position to FW in a async variable in RLP cycles.
11/4/15     sks     Double instantiate sampserv client mutex.
10/15/15    skk     FR26881 : HRP - Half Window Reacq with PICH Decode initial check in.
10/2/15     sks     Adjust early wakeup threshold for parallel reacq.
09/16/15    sks     Compilation error fixes for FEATURE DUAL SIM undefined.
08/11/15    sks     Wait for all notifications for PICH drop to be posted before unfreezing SS.
08/05/15    sks     Unfreeze SS before sending demod cmd if SS was frozen but PICH cfg was not sent
02/27/15    ap      Initial check-in for WL1 QSH framework 
03/05/15    sad     wl1_rf_state change
03/09/15    sks     Mods for full sleep in G2W TA mode.
03/09/15    rs      Sleep timeline manager changes
02/28/15    rsr     W + W Phase 2 Checkin.
02/23/15    pv       WL1 SLEEP MODE MANAGER support.
01/19/15    sks      Global re-org for W+W
01/08/15    rs       Replaced SECOND_REACQ notf with UPD_MEAS_RULES_DONE notf to prevent early RF shutdown evaluation 
                     happening prematurely when RLP kicks in.
10/14/14    sks      Pull in early wakeup threshold by 5 BPGs for parallel reacq to avoid PI_SUM 1s. 
06/20/14    skk      FR19218: ReacqLessPICH(RLP) porting for JO.
07/03/14    sks      Changes for FCDC
06/30/14    sks      Wait for RF sleep to complete before calling FW sleep - temporarily needed to fix a long RF sleep time.
05/27/14    sks      Decrease the early wakeup threshold by 10 BPGs for seq reacq to avoid PI_SUM 1s.
04/03/14    sks      Adjust outer loop for QXT change.
03/06/14    gsk      Enhancing freeze status query API
03/04/14    sks      Bump up W priority with co-ex for reacq searches for non-true sleep.
01/23/14    rs       Simple opts: increasing early wakeup threshold by 18 bpgs and reducing the sample size 
                     by 2 bps. Also added support to disable these optimization by setting NV 5458 to 0x18.
01/22/14     sks      Set/reset the srch_wakeup status for DSDA priority at first_reacq_failed and waiting_on_srch_Awake respectively.
01/22/14    sks      Raise the priority of PICH decodes and Reacq searches with co-ex manager for DSDA
12/19/13    gsk      Creating a bmask for second reacq to trach pich entity drop
12/12/13    sks      Reset the offline_pich_in_progress flag at PICH drop.
04/04/13    pv       Mutex lock around the check/setting of pich entity srch delay event.  
02/14/13    pv       Extern for SCCPCH start cell time.
02/11/13    pv       API for getting RF div chain status.
02/11/13    pv       Changes for QXT fixes.
01/21/13    pv       Offline PICH changes.
11/26/12    gsk      Externing a function.
09/12/12    pv       Change the early wakeup threshold for seq/prll reacq.
08/29/12    pv       Timeline opts - added new notf events and externs.
08/27/12    pv       Increased the samples beyond PI bits by 2.5 BPGs for 
                     Offline PICH.
08/24/12    pv       Added extern declaration of drxofflineprocmgr_ext_cntrl_params[wsub_id].sampservmgrdb
08/01/12    pv       Externs and new mutexlock.
07/27/12    pv       Backing out previous checkin.
07/22/12    pv       New ENUMs and externs.
07/16/12    sr       Fixed compiler warnings 
07/17/12    pv       Warning fixes.
07/16/12    pv       Externed a function.
07/13/12    saik/pv  Enhance DRX notf mech and arbitration functionality
07/06/12    saik/pv  DRX offline proc mgr first version

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "srchzz.h"

#include "l1rrcif.h"
#include "drxcmd.h"
#include "tlm.h"

#include "wl1api.h"

#include "wl1drxmanager.h"
#include "rxdiv_v.h"


#include "mdspsync.h"
#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
#include "wl1trm.h"
#include "wl1cxm.h"
#endif

#if defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)
#include "wl1_qsh_ext.h"
#endif /* defined (FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC) */

/*===========================================================================
                        DATA DECLARATIONS
===========================================================================*/
/* globals */

 

#ifdef FEATURE_WCDMA_REACQLESS_PICH
/*No of samples to be captured after PI bits, If RLP collect only 1 as FW uses 3 Tap Ncp filter else use 2 as FW uses 5 Tap Ncpfilter */
#define OFFLINE_PICH_SAMPLES_NEEDED_PAST_PI_IN_CHIPS ((SRCHZZ_PERFORM_RLP_THIS_CYCLE() ? 1 : 2) * CHIP_PER_256_CHIP_UNITS)
#else
/* Leave 5 BPGs beyond PI bits.*/
#define OFFLINE_PICH_SAMPLES_NEEDED_PAST_PI_IN_CHIPS (5 * CHIP_PER_256_CHIP_UNITS)
#endif
/* For short QXT, the samples are worth 18.75 bpgs. Leaving a buffer of 0.25 BPG.*/

/* the adjustment has been made for G2W TA mode to wake up earlier by 24 BPGs = 1.6 ms. This is because everytime G call drops while asleep, we wakeup
   and see a PO overrun in that cycle because the RF wakeup time shoots up. I observed that the delta was on an average about 1.6 ms. We can evaluate if this is 
   really needed. */
#ifdef FEATURE_DUAL_SIM
#define OFFLINE_PICH_SAMPLES_SHORT_QXT_IN_BPGS (WL1_IN_G2W_TA_MODE() ? (45 - drxofflineprocmgr_ext_cntrl_params[wsub_id].dec_pich_samples_by) : (21 - drxofflineprocmgr_ext_cntrl_params[wsub_id].dec_pich_samples_by))
/* For long QXT, the samples are worth 31.75 bpgs. Leaving a buffer of 0.25 BPG.*/
#define OFFLINE_PICH_SAMPLES_LONG_QXT_IN_BPGS  (WL1_IN_G2W_TA_MODE() ? (59 - drxofflineprocmgr_ext_cntrl_params[wsub_id].dec_pich_samples_by) : (35 - drxofflineprocmgr_ext_cntrl_params[wsub_id].dec_pich_samples_by))
#else
#define OFFLINE_PICH_SAMPLES_SHORT_QXT_IN_BPGS (21 - drxofflineprocmgr_ext_cntrl_params[wsub_id].dec_pich_samples_by)
/* For long QXT, the samples are worth 31.75 bpgs. Leaving a buffer of 0.25 BPG.*/
#define OFFLINE_PICH_SAMPLES_LONG_QXT_IN_BPGS  (35 - drxofflineprocmgr_ext_cntrl_params[wsub_id].dec_pich_samples_by)

#endif

#define OFFLINE_PICH_LONG_TO_SHORT_QXT_DIFF (OFFLINE_PICH_SAMPLES_LONG_QXT_IN_BPGS - OFFLINE_PICH_SAMPLES_SHORT_QXT_IN_BPGS)

/* 45 - 30 = 15 BPG difference between short and long QXT for online PICH*/
#define ONLINE_PICH_LONG_TO_SHORT_QXT_DIFF 15

#define OFFLINE_PICH_SS_ACT_TIME_IMMEDIATE 0xFFFF

#define OFFLINE_PICH_MDSP_MAX_PI_BIT_END_PN 144

#ifdef FEATURE_WCDMA_REACQLESS_PICH
#define OFFLINE_PICH_EARLY_WKUP_THRESH_BPG_PRLL_REACQ (diag_masks_enabled() && diag_f3_trace_enabled_check()) ? 131 : 137
#define OFFLINE_PICH_EARLY_WKUP_THRESH_BPG_SEQ_REACQ  (diag_masks_enabled() && diag_f3_trace_enabled_check()) ? 88 : 94
#define OFFLINE_PICH_EARLY_WKUP_THRESH_BPG_PRLL_REACQ_RLP (diag_masks_enabled() && diag_f3_trace_enabled_check()) ? 133 : 139
#else
#define OFFLINE_PICH_EARLY_WKUP_THRESH_BPG_PRLL_REACQ (107 + drxofflineprocmgr_ext_cntrl_params[wsub_id].inc_early_wkup_thresh_by)
#define OFFLINE_PICH_EARLY_WKUP_THRESH_BPG_SEQ_REACQ  (65 + drxofflineprocmgr_ext_cntrl_params[wsub_id].inc_early_wkup_thresh_by)
#endif

#define DRX_PI_START_TIME_TO_FW() (drxofflineprocmgr_ext_cntrl_params[wsub_id].pich_start_time)

#define OFFLINE_PICH_WAKEUP_ADJ_FOR_PI_END_OF_FRAME  20

#define DRX_NUM_CYCL_OFFL_PICH_TRIG_THRESH 3

#define SAMPSERV_DRX_NOTIFICATIONS_PENDING_NONE  0x0
#define SAMPSERV_DRX_SS_FREEZE_PENDING_BMSK      0x1
#define SAMPSERV_DRX_QFT_DONE_PENDING_BMSK       0x2
#define SAMPSERV_DRX_PICH_DONE_PENDING_BMSK      0x4
#define SAMPSERV_DRX_FIRST_REACQ_DONE_PENDING_BMSK 0x8
#define SAMPSERV_DRX_SECOND_REACQ_DONE_PENDING_BMSK 0x10
#define SAMPSERV_DRX_UNFREEZE_FOR_NO_PICH_SETUP 0x20


#define SAMPSERV_MGR_MUTEXLOCK() REX_ISR_LOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].sampserv_mgr_lock)
#define SAMPSERV_MGR_MUTEXFREE() REX_ISR_UNLOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].sampserv_mgr_lock)

#define SAMPSERV_MGR_CLIENT_LOCK()   REX_ISR_LOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].sampserv_mgr_client_lock)
#define SAMPSERV_MGR_CLIENT_UNLOCK() REX_ISR_UNLOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].sampserv_mgr_client_lock)
#define SAMPSERV_MGR_RESET_OFFLINE_PICH_IN_PROGRESS() drxofflineprocmgr_ext_cntrl_params[wsub_id].sampservmgrdb.drx_offline_pich_in_progress = FALSE
#define SAMPSERV_MGR_SET_OFFLINE_PICH_IN_PROGRESS() drxofflineprocmgr_ext_cntrl_params[wsub_id].sampservmgrdb.drx_offline_pich_in_progress = TRUE
#define RF_DIV_SLEEP_MUTEXLOCK() REX_ISR_LOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].rf_div_sleep_lock)
#define RF_DIV_SLEEP_MUTEXFREE() REX_ISR_UNLOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].rf_div_sleep_lock)

#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
#define SAMPSERV_MGR_RESET_SRCH_WAKEUP_FAILURE() drxofflineprocmgr_ext_cntrl_params[wsub_id].sampservmgrdb.wkup_srch_failed = FALSE
#define SAMPSERV_MGR_SET_SRCH_WAKEUP_FAILURE() drxofflineprocmgr_ext_cntrl_params[wsub_id].sampservmgrdb.wkup_srch_failed = TRUE
#endif

#define RF_SLP_WKUP_MUTEXLOCK() REX_ISR_LOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].rf_slp_wkup_lock)
#define RF_SLP_WKUP_MUTEXFREE() REX_ISR_UNLOCK(&drxofflineprocmgr_ext_cntrl_params[wsub_id].rf_slp_wkup_lock)


/* Timer that expires 3.7 ms before the next entity waking up after
   go_to_sleep fails*/

/*===========================================================================
                        DATA STRUCTURES
===========================================================================*/
typedef enum
{
  SS_FREEZE_CMD,
  SS_UNFREEZE_CMD,
  SS_CMD_NONE,
  NUM_SS_CMDS
} drx_offline_pich_ss_cmd_type;

typedef enum
{
  SRCH_SSMGR_CLIENT,
  DRX_SSMGR_CLIENT,
  TRIAGE_SSMGR_CLIENT,
  NUM_SSMGR_CLIENTS
} sampserv_mgr_client_type;


typedef enum
{
  SAMPSERV_PENDING_FROZEN,
  SAMPSERV_FROZEN,
  SAMPSERV_PENDING_UNFROZEN,
  SAMPSERV_UNFROZEN,
  SAMPSERV_INIT,
  NUM_SAMPSERV_STATES  
}sampserv_resource_state;

typedef struct
{
  /* 0 = SS frozen, 1 = SS unfrozen */
  boolean ss_status;
  /* 0= RF in LPM, 1 = RF out of LPM */
  boolean  rf_status;
} sampserv_mgr_resource_status_type;

typedef enum
{
  DRX_OUTER_LOOP_ADJ_OFFLINE_PICH_MODE,  
  DRX_ONLINE_PICH_MODE,
  DRX_OFFLINE_PICH_MODE,
  DRX_OFFLINE_RLP_MODE,
  NUM_DRX_MODES
} drx_pich_processing_mode_type;

typedef void (*sampserv_mgr_local_cmd_post_cb_fn) (wsub_id_e_type wsub_id);
typedef void (*drx_ss_ops_handler_type) (void);

/*------------------------------------------------------------------------------------------- 
                    VARIABLE DECLARATIONS
--------------------------------------------------------------------------------------------*/

typedef enum
{
  /* No communication established with DRX or DRX-SSMGR link reset - 00*/
  DRX_CLEAR_ALL_NOTIFICATION,
  /* DRX cmd received from RRC - 01*/
  DRX_NOTIFY_STANDBY_MODE_ENTRY_EVENT,
  /* DRX aborted, abort procedure completed and ext cmd processing initiated -02*/
  DRX_NOTIFY_STANDBY_MODE_EXIT_EVENT,
  /* SW has sent the SS freeze cmd to MDSP  - SS state goes to pending FROZEN , RF is still UP - 03*/
  DRX_NOTIFY_OFFLINE_PICH_START_EVENT,
  /* AGC tuning is complete. - 04*/
  DRX_NOTIFY_AGC_DONE_EVENT,
  /* SS freeze has occurred in FW  - only for setting SS state to FROZEN, RF goes to LPM -05*/
  DRX_NOTIFY_SS_FREEZE_EVENT,
  /* SS state goest to UNFROZEN, RF out of LPM - all searches can be allowed following this event-6 */
  DRX_NOTIFY_PCH_SCCPCH_START_EVENT,
  /* PICH decode done generic event -07*/ 
  DRX_NOTIFY_PICH_DONE_EVENT,
  /* PICH decode done, no PICH ack, so all searches can be allowed -08*/
  DRX_NOTIFY_PICH_NACK_EVENT,
  /* QXT done event, triggered by QXT done interrupt from FW -09*/
  DRX_NOTIFY_QXT_DONE,
  /* DRX SW triggered SS unfreeze, and RF out of LPM -10*/
  DRX_NOTIFY_SS_UNFREEZE_RF_OUT_OF_LPM_CMD,
  /* Notify that sleep will be delayed. -12*/  
  DRX_NOTIFY_DELAY_PICH_TO_SLEEP_EVENT,
  /* This is the case when SS freeze cmd is not yet done when
  ** we want to drop PICH entity and deconfigure cells after
  ** PICH done isr. -13*/
  DRX_NOTIFY_DELAY_PICH_ENTITY_SRCH_TO_SLEEP,
  /* FW/RF sleep complete -14 */
  DRX_NOTIFY_GO_TO_SLEEP_EVENT,
  /* Update meas rules is done -15*/
  DRX_UPD_MEAS_RULES_DONE,
  /* First reacq srch done -16*/
  DRX_NOTIFY_FIRST_REACQ_DONE,
  /* DRX DL to sleep start -17*/
  DRX_NOTIFY_READY_TO_START_DRX_DL_SLEEP_EVENT,
  DRX_NUM_NOTIFY_EVENTS
}drx_ssmgr_notify_evt_type;

typedef void (*drx_notification_post_cb_type)(wsub_id_e_type wsub_id);

typedef struct
{
  drx_ssmgr_notify_evt_type drx_notification_event;
  drx_notification_post_cb_type drx_notification_post_cb;

  uint32 drx_offl_pich_st_ss_frz_frc_act_time;
  boolean drx_offl_pich_st_rf_in_lpm;
}sampserv_mgr_drx_intf_type;

typedef struct
{
  boolean status;
}sampserv_mgr_triage_intf_type;

typedef enum
{
  SRCH_SSMGR_START,
  SRCH_SSMGR_COMPLETE,
  SRCH_SSMGR_ABORT,
  NUM_SRCH_SSMGR_OPS
}ssmgr_srch_op_type;

typedef struct
{
/* Searcher should set this flag at every samp serv mgr communication */
boolean srch_intf_valid;
ssmgr_srch_op_type  ssmgr_srch_op;

/* Operation type:  SRCH_SSMGR_START */
/* The flags below should mirror the values that search intends to send
** to FW for the search operation
*/
boolean srchusefreezeflag;
uint16 srchssfreezeflag;
boolean srchstoprfflag;

/* Handlers */
void (*ss_ops_done_handler) (void);
void (*cg_agc_done_handler) (void);

/* This flag needs to be set if srch ensures that some other mechanism
** such as rfm_tune_to_chan(), will take care of bringing rf out
** of LPM. If flag is set, SS mgr will avoid bringing RF out of LPM
** although that is the pre-search operation requirement
*/
boolean override_rf_out_lpm_op; 

srchzz_substate_enum_type srch_op;

}sampserv_mgr_srch_intf_type;

typedef struct
{ 
  /* Client requesting operation */  
  sampserv_mgr_client_type sampserv_mgr_client;
  /* Searcher interface */
  sampserv_mgr_srch_intf_type sampserv_srch_intf;
  /* DRX interface */
  sampserv_mgr_drx_intf_type sampserv_drx_intf;
  /* Triage interface */
  sampserv_mgr_triage_intf_type sampserv_triage_intf;
} sampserv_mgr_intf_type;

extern boolean sampserv_mgr_notify(sampserv_mgr_intf_type* sampserv_mgr_notification, wsub_id_e_type wsub_id);

extern boolean sampserv_mgr_query_freeze_status(boolean check_for_pend_frozen, wsub_id_e_type wsub_id);

typedef enum
{
  SAMPSERV_SSMGR_FORCE_UNFREEZE,
  SAMPSERV_SSMGR_NO_ACTION,
  SAMPSERV_SSMGR_NO_ACTION_ONLINE_MODE,
  SAMPSERV_SSMGR_QXT_DONE_EVT_NOTIFY,
  SAMPSERV_SSMGR_QXT_DONE_EVT_NOTIFY_ONLINE_MODE,
  SAMPSERV_SSMGR_PICH_DONE_EVT_NOTIFY,
  SAMPSERV_SSMGR_PICH_DONE_EVT_NOTIFY_ONLINE_MODE,
  SAMPSERV_SSMGR_PICH_NACK_EVT_NOTIFY,
  SAMPSERV_MGR_NUM_CMD_ACTION,
  DRX_COEX_PRIORITY_LOW,
  DRX_COEX_PRIORITY_HIGH,
  SAMPSERV_SSMGR_NUM_ACTIONS
} sampserv_mgr_cmd_action_type;


typedef struct
{
 l1_local_cmd_hdr_type local_cmd_hdr;
 sampserv_mgr_client_type client;
 sampserv_mgr_cmd_action_type action;    
 sampserv_mgr_local_cmd_post_cb_fn post_cb;
}sampserv_mgr_cmd_type;


#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
typedef enum
{
  PRIORITY_LOW,
  PRIORITY_HIGH
}drx_priority_level_with_coexmgr;

#endif

extern void sampservmgr_process_local_cmd(sampserv_mgr_cmd_type* cmd_ptr, wsub_id_e_type wsub_id);

extern boolean sampserv_mgr_is_ss_frozen( wsub_id_e_type wsub_id);

extern void ssmgr_notify_rf_tune( sampserv_mgr_client_type client,uint16 freq, wsub_id_e_type wsub_id);

extern void sampserv_mgr_set_notification_pending(sampserv_mgr_client_type,uint32 bmsk, wsub_id_e_type wsub_id);

extern void sampserv_mgr_clear_notification_pending(sampserv_mgr_client_type,uint32 bmsk, wsub_id_e_type wsub_id);

extern uint32 sampserv_mgr_get_notifications_pending_for_pich_drop( wsub_id_e_type wsub_id);

extern void ssmgr_mdsp_rsp_timeout_handler( wsub_id_e_type wsub_id);

extern boolean drx_offline_pich_ss_freeze_cmd_sent( wsub_id_e_type wsub_id);

extern void ssmgr_mdsp_rsp_agc_tune_timeout_handler(void);

extern boolean sampserv_mgr_nv_enabled( wsub_id_e_type wsub_id);

extern void sampserv_mgr_notify_wakeup_isr_entry(void);

extern void sampservmgr_post_local_cmd(sampserv_mgr_client_type client,sampserv_mgr_local_cmd_post_cb_fn post_cb,sampserv_mgr_cmd_action_type action, wsub_id_e_type wsub_id);

extern void sampserv_mgr_notify_status_operational( wsub_id_e_type wsub_id);

extern boolean sampserv_mgr_query_ss_unfreeze_status( wsub_id_e_type wsub_id);

extern void drx_early_rf_shutdown_handler( wsub_id_e_type wsub_id);

extern void drx_init_sampserv_state( wsub_id_e_type wsub_id);

extern void drx_disable_other_opts( wsub_id_e_type wsub_id);

void drx_clr_early_rf_shutdown_bmsk(drx_ssmgr_notify_evt_type drx_ssmgr_notf_evt, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION drx_offline_pich_send_sample_server_freeze_cmd

DESCRIPTION:
  This function sends sample server freeze command to MDSP sync i\f.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void drx_offline_pich_send_sample_server_freeze_cmd
(
  /* Indicates whether to apply freeze/unfreeze action to chain 0 */
  boolean chain0,
  /* Indicates whether to apply freeze/unfreeze action to chain 1 */
  boolean chain1,
  /* Indicates the action to be taken with RTR after sample server cmd action time.*/
  boolean rfPwrUpdateFlag,
  /* Indicates the STMR Cx1 value for the action time for the freeze/unfreeze.*/
  uint32 actionTime
);

typedef void  (*srch_ss_ops_handler_type) (void);
typedef void  (*srch_rf_tune_done_handler_type) (void);

typedef struct
{
  /* Common data */
  sampserv_mgr_client_type curr_ssmgr_client;
  sampserv_mgr_client_type prev_ssmgr_client;
  boolean pending_rf_state_update;
  boolean pending_rf_state;
  boolean pending_ss_state_update;
  sampserv_resource_state pending_ss_state;
  sampserv_resource_state current_state;
  sampserv_resource_state prev_state;
  sampserv_resource_state pending_next_state;
  boolean ssmgr_operational;
  boolean ssmgr_deact_pending;
  boolean ssmgr_force_unfreeze_pending;
  boolean ssmgr_wait_for_agc_tune_done;
  uint32 ssmgr_mdsp_ops_pending_bmsk;

  /* DRX data */
  drx_ssmgr_notify_evt_type current_notify_event;
  drx_ssmgr_notify_evt_type prev_notify_event;
  drx_notification_post_cb_type drx_pich_entity_to_sleep_cb;
  drx_notification_post_cb_type drx_pich_entity_srch_to_sleep_cb;  
  boolean drx_pich_entity_to_sleep_cb_pending;
  boolean drx_pich_entity_srch_to_sleep_cb_pending;

  boolean drx_rf_mgr_ss_freeeze_evt_notify;

  uint32  drx_notifications_pending_for_pich_drop_bmsk;

  /* Flag gets set to TRUE at DRX_NOTIFY_OFFLINE_PICH_START_EVENT
  ** Flag get cleared (set to FALSE) at either DRX_NOTIFY_PICH_NACK_EVENT 
  ** or at DRX_NOTIFY_SCCPCH_START_EVENT
  */
  boolean drx_offline_pich_in_progress;
  /* Search data */
  boolean rfstopflag;
  boolean rf_state_upd_at_srch_done_reqd;
  boolean rf_state;
  sampserv_resource_state final_state;
  boolean ssrv_in_transition;
  boolean srch_in_progress;
  boolean override_rf_out_lpm_op;
  srchzz_substate_enum_type completed_srch_type;
  srchzz_substate_enum_type started_srch_type;
  srch_ss_ops_handler_type srch_ss_ops_handler_cb;
  srch_rf_tune_done_handler_type srch_rf_tune_done_handler_cb;

  sampserv_mgr_srch_intf_type prev_srch_intf_notf;
  #ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
  boolean wkup_srch_failed;
  #endif
}ssmgr_db_type;


typedef struct
{
  ssmgr_db_type sampservmgrdb;
  tlm_tfn_struct_type sccpch_start_from_pich_st;
  tlm_tfn_struct_type agc_tune_done;
  tlm_tfn_struct_type sccpch_start;
  tlm_tfn_struct_type pi_offset_from_pich;
  tlm_tfn_struct_type pich_start_wrc;

  uint32 inc_early_wkup_thresh_by;
  uint32 dec_pich_samples_by;
  uint32 frc_at_agc_done;
  uint32 ss_freeze_action_time;
  uint32 pich_start_time;
  boolean drx_offline_pich_ss_freeze_cmd_this_cycle;
  

  rex_timer_type ssmgr_mdsp_rsp_timeout_timer;
  /* Timer that expires 3.7 ms before the next entity waking up after
go_to_sleep fails*/
  rex_timer_type drx_entity_wbump_up_timer;

  rex_crit_sect_type sampserv_mgr_client_notf_lock;
  rex_crit_sect_type sampserv_mgr_lock;
  rex_crit_sect_type sampserv_mgr_mdsp_ops_lock;
  rex_crit_sect_type rf_slp_wkup_lock;
  rex_crit_sect_type sampserv_mgr_client_delay_pich_sleep_lock;
  rex_crit_sect_type drx_rf_mgr_lock; 
  rex_crit_sect_type rf_div_sleep_lock;
  rex_crit_sect_type   sampserv_mgr_client_lock;
}drxofflineprocmgr_ext_cntrl_params_type;

extern drxofflineprocmgr_ext_cntrl_params_type drxofflineprocmgr_ext_cntrl_params[WCDMA_NUM_SUBS];

extern boolean wl1_is_rf_awake(wsub_id_e_type wsub_id);

extern void drx_set_early_rf_shutdown_bmsk(drx_pich_processing_mode_type drx_pich_proc_mode, wsub_id_e_type wsub_id);

extern void drx_perform_offline_pich_eval
(
  /* Indicates whether to apply freeze/unfreeze action to chain 0 */
  boolean chain0,
  /* Indicates whether to apply freeze/unfreeze action to chain 1 */
  boolean chain1,
  /* Indicates the action to be taken with RTR after sample server cmd action time.*/
  boolean rfPwrUpdateFlag,
  /* Indicates the STMR Cx1 value for the action time for the freeze/unfreeze.*/
  uint32 actionTime
, wsub_id_e_type wsub_id);

extern void drx_disable_early_rf_shutdown( wsub_id_e_type wsub_id);

extern void drx_disable_cell_fing_decfg_opt( wsub_id_e_type wsub_id);

extern void drx_offline_pich_logging( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     DRX_IS_OFFLINE_PICH_IN_PROGRESS

DESCRIPTION
  Returns the status of the offline pich
  
PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  TRUE - When Offline PICH in progress
  FALSE - When PICH ACK/NACK is recevied

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean drx_is_offline_pich_in_progress( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_IS_RF_DIV_AWAKE

DESCRIPTION
  This is the wrapper function to get the RF state for div chain.
  
PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_is_rf_div_awake(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_SAMPSERV_LAST_SRCH_PERFORMED

DESCRIPTION
  Returns the last search that is performed through drxofflineprocmgr 
  
PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  Type of search that is performed

SIDE EFFECTS
  None.
===========================================================================*/
srchzz_substate_enum_type drx_sampserv_last_srch_performed( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW
/*===========================================================================
FUNCTION     WL1_DSDA_TOGGLE_PRIORITY

DESCRIPTION
  This is the wrapper function to inform co-ex manager about the change in W's priority level for RF usage.
   Event gets triggered to higher the priority of WFW_COEX_ACTIVITY_SEARCHER in order to make sure G blanks during entire duration of PICH decode�
   this will ensure that the samples captured by WFW are clean during the PICH decode.
   Event is triggered to lower the priority of WFW_COEX_ACTIVITY_SEARCHER after PICH has been decoded, in order to make sure G does not have to blank
�  unnecessarily while W is in sleep.
  
PARAMETERS
  wl1_cxm_evt_type evt_type

DEPENDENCIES
  None

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_coexmgr_toggle_priority(drx_priority_level_with_coexmgr level, boolean drx_priority_called_from_second_reacq_done, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_SET_TIMER_FOR_W_BUMPUP

DESCRIPTION
  Function called only in case go_to_sleep fails. A new timer is set that expires 3.7 ms
  before the next entity waking up. This gives UE enough time to Bump up W priority.
  
  3.5 slots for coex manager delay + 2 slots for reacq search samples =  5.5 slots
  (3.7 ms) before reacq search command is sent to FW
 
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None
===========================================================================*/
void drx_set_timer_for_w_bumpup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_BUMP_UP_PRIORITY_ON_TIMER_EX

DESCRIPTION
  Wrapper function that sets the W priority on timer expiry
 
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None
===========================================================================*/
extern void wl1_bump_up_priority_on_timer_ex(timer_cb_data_type cb_data);

#endif /* FEATURE_WCDMA_MULTISIM_COEXISTENCE_SW */
/*===========================================================================
FUNCTION     DRX_DISABLE_SIMPLE_OPTS_THRU_NV5458

DESCRIPTION
  Function used to disable simple optimizations if NV 5458 is set to 0x18
  Opts to be disabled are:
  1. decrease the samples by 2 bgps
  2. Increase the early wakeup threshold by 18 bpgs
 
PARAMETERS
  

DEPENDENCIES
  None

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.
===========================================================================*/
extern void drx_disable_simple_opts_thru_nv5458( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
extern void wl1_drxofflineproc_get_debug_info(wl1_drx_sleep_mini_debug_info_struct_type* obj);
#endif

/*===========================================================================
FUNCTION     DRX_SSMGR_ENSURE_RF_AWAKE_AT_SLEEP_FAILURE

DESCRIPTION
   This function makes sure sample server and RF is up after sleep failure.
 
PARAMETERS
  None.

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void drx_ssmgr_ensure_rf_awake_at_sleep_failure(wsub_id_e_type wsub_id);

extern void drxofflineprocmgr_ext_cntrl_params_init( wsub_id_e_type wsub_id);

extern void drxofflineprocmgr_int_cntrl_params_init( wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION wl1_qsh_mdump_collect_drxofflineprocmgr_params

DESCRIPTION
  Wrapper

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_drxofflineprocmgr_params(wsub_id_e_type wsub_id);
#endif
#endif 
