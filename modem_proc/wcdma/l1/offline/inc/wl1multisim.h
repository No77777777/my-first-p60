#ifndef WL1MULTISIM_H
#define WL1MULTISIM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            WCDMA L1 MULTISIM

GENERAL DESCRIPTION:
********************
  This file contains all the source code to support multisim. This file
  contains the code to support QTA, LTA, G2W TA and WRM.

EXTERNALIZED FUNCTIONS:
***********************
  

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2014 by Qualcomm Technologies, Inc. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1multisim.h#7 $
$DateTime: 2020/03/13 02:22:32 $
$Author: pwbldsvc $

when       who     what
--------   ---     ----------------------------------------------------------
02/19/20   am      CR2623733:L+W QTA, send FW STOP instead of FW SLEEP cmd.
01/10/20   ac      Mini dump framework checkin
11/25/19   ta      Ignore SCCH order for CPC-DRX operation 4ms before start QTA to FW 
11/20/19   as      Changed CM_INV max priority inversion time to to suport sib peridiocity 1024
06/14/18   pb      Return reserve/req duration in wl1_ds_get_entity_proc() API 
01/18/17   pb      Support state information update to TRM
11/27/17   th      Made changes to convert page demod inv to cm inv for high priority qta
08/15/16   pvr     Feature APS Changes for 1xSRLTE+W
08/16/17   ss     Changes for LTA timeline optimization
08/23/17   th/nc   MACRO to return value for no lock timer
09/09/16   kr      Backing out CR1048579
09/06/16   kr      handle gts fail case in meas only gap due to PICH nearby
07/20/16   ad      Made changes to update the duty cycle related variables when W gets the grant.
06/08/16   pg      Changes to pend the RnN when cell trans is in progress and 
                   unpend it at the end of processing
06/04/16   pg      Mods to fix race condtion with QTA ASYNC INFO commands and WRM
                   requests from RRC
03/29/16   pvr     Mods to log QDTA timings correctly in 0x41B6
03/10/16   ad      Changes to map page demod inversion also to high priority qta
02/08/16   gm      mods to update max priority inversion timer.
01/13/15   ad      Changes to pend the RnN when IFHHO is in progress and unpend it at the 
                   end of cphy_setup
01/05/16   pg      Mods to reset the prio inversion timer after Resume Sucess/Fail.
12/28/15   ap      Allow every alternate measurement QTA that is skipped due to collision with RX burst.
12/11/15   kr      When BCCH and CTCH collide with RRC has already Inverted BCCH priority
                   use CTCH INV for this cases instead of CTCH
11/26/15   pg      To save the DS status variables while moving to FTM Mode
                   and restore later while going to Online.
11/05/15   ad      Mods to log the sub reason from pending proc
11/06/15   sad     RACH vs QTA changes
10/22/15   abs     Mods to support ACQ in DR-DSDS mode
10/20/15   pvr     Protect winning ta client info inside QDTA gap.
10/15/15   ar      Add macro to force reset DS DB
10/12/15   skk     Added a timer to declare OOS in idle mode if lock is not available for 12sec.
10/07/15   rs      Don't run OOS algo for meas only wakeups 
10/07/15   rs      CTCH lock management for DSDS
09/29/15   pvr     DBDC QCTA changes
09/25/15   pg      To use internal Band information in RRC request and notify
                   for lock function if passed band information is not valid.
09/28/15   pvr     Add WL1_MULTISIM_MUTEX_LOCK
09/25/15   abs     Updated max inversion timer logic
09/01/15   sad     Update wl1_get_qta_start_cfn to return bool to indicate 
                   if the returned cfn is valid or not
08/25/15   sad     RACH timer to block QTA
08/25/15   us      Updates for including rfm_path_type
08/21/15   ar      Fix issues related to priority inversion
08/21/15   pvr     Add QDTA support for 0x41b6 log packet
08/19/15   sad     DBDC + Rx/Tx split feature check-in
08/12/15   ar      Add macro to query if max inv timer is running
08/13/15   gm      AS_ID cleanup.
08/12/15   ar      Add parameter to sib priority toggle api
08/11/15   rsr     Added Support to ensure that FW is put to Sleep, prior 
                   to Entry of Traffic State. 
08/04/15   vs      Changes related to the InterF and IRAT wrm activities.
07/27/15   abs     Corrected logic for cphy_band_req_timeout_timer handling
07/22/15   us      Compilation error fixes
07/21/15   pg      Mods to fix the unlock cancel while FACH QTA timer is running
07/23/15   abs     Changes to pend Ext cmd's/RnN's when in MEAS only gap
06/30/15   sks     Changes to initiate BPLMN process from full sleep without L1 req for lock.
07/17/15   pvr     QDTA changes 
07/16/15   ar      TRM-U Phase 3 changes
06/30/15   sks     Do not do a sleep abort for timetag request in any state.
07/02/15   ap      Initial check-in for EFACH+MSIM
06/30/15   tsk     FR 26237: DSDS with Rel.8 E-FACH HSRACH + MSIM Changes.
06/30/15   vs      Moving the macro RRC_UNLOCK_FACH_QTA_OVERHEAD_TIMER_IN_MS from wl1multisim.c file for extern purpose.
06/30/15   sks     Changes to initiate BPLMN process from full sleep without L1 req for lock.
06/25/15   pg      Mods for RoTA Optimizations to stop building PDU during 
                   RoTA gap.
06/23/15   skk     Mods to change DL WEAK IND for no lock timer during cell selection based on QXDM cmd. 
06/24/15   gm      Mods to return the lock release status from alt client.
06/24/15   abs     Mods to clear SBCH proc from ds_proc_mask if SBCH drop
                   is included in the CPHY_SETUP_REQ for PCH_SETUP
06/20/15   ac      Remove unneeded RX_TX_SPLIT featurization for BO
06/15/15   ar      Add prototype to return as_id for a give TRM client
06/12/15   da      TA Featurization for CR 846021: Changes to abort all searches in QTA gap 
                   if the winning client is 1x
06/04/15   gm      Added a function which returns the winning client id.
06/02/15   ar      Define macro for getting the pended proc in DS db
06/02/15   sks     Push CTCH_CRITICAL to the bottom of ds_proc_info as it has no attributes.
05/30/15   ar      Changes to support search abort for 1x QTAs
05/26/15   gsk     Added API to check if QTA is force reserve
05/25/15   skk     Change TRM priority if BCCH wakesup before sleep after PICH wakeup. 
05/19/15   sad     QTA cleanup code for idle tech
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
05/19/15   ar      Change the prototype for priority inversion to match the new function
05/19/15   sad     Changes to support modify band returning new device
                   for Rx/ Tx split
05/12/15   rs      Triggering FW sleep before handling ext cmd in early RF shutdown cases
05/01/15   kr      FR 26325: CB/BMC support for W/G/L+G MSIM DSDS
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
02/27/15   ap     Initial check-in for WL1 QSH framework 
05/01/15   skk     Decrease lock req timeout time for ACQ steps during split acq to 3sec
03/26/15   sad     update to 0x41B6 log packet
04/27/15   skk     Increase the no-lock timer during Cell Selection to 3sec. 
03/24/15   skk     Trigger DL WEAK IND if L1 doesnt get lock for 2 sec during cell selectoin. 
04/20/15   dm      Acq priority inversion for TRM unification.
04/01/15   sad     QTA cleanup code
03/16/15   gm      mods to remove WL1_DS_IS_TRM_SIG_PENDING_ON_SLEEP() check.
02/24/15   ar      In CELL_FACH state, send resume failure immediately if TA duration
                   is larger than OOS duration.
01/29/14   pr      WL1M global reorg for WplusW
01/28/15   pr      Global reorg for W + W.
01/23/15   sad     Global re-org for W+W
01/19/15   sks     Global re-org for W+W
11/13/14   jhl     Move wl1_get_rem_time_in_ms out of dual_sim definition to fix compiler error
10/10/14   gm      Mods to request for first QTA with force_reserve after moving to FACH state. 
09/23/14   pv      Support for TRM swapping of chains for call setup in DR mode.
                   Added API to handle pending call setup.
09/23/14   gm      Cleanup QTA related timers in case of winning client changes 
                   with UNLOCK_REQUIRED event.
09/18/14   gm      mods to remove global variables which are never used.
09/17/14   ac      Changed WSRCH_NBREJ and QTA_ROTA NVs to use WNV API
09/15/14   pg      FR 21174 : QTA allowed during re-config in DCH changes
09/10/14   dm      Set default QBTA+ROTA NV to 5.
08/20/14   sd      Fixed DSDS Medium and Low compiler warnings in WCDMA.
08/18/14   skk     Request for lock for CELL_SELECTION_REQ in G2W TA mode as lock can be released as a part of SIB sleep.
08/12/14   sd      Fixed DSDS Medium and Low compiler warnings in WCDMA.
07/24/14   skk     Handle pending LOCK_ACQ_SIG if go to sleep didnt get triggered because of not having enough time.
07/18/14   vs      Added support for HS multisim support.
07/18/14   gm      Changes for ROTA feature
07/18/14   gm      added a function wl1_query_qta_start to be used by RxD module.
07/02/14   gm      Fixes related to DR DSDS bring up from multisim side.
06/19/14   dm      DR-DSDS first cut.
05/06/14   as      Moving WRM to WL1TRM.
04/27/14   jkb     Add check for Feature dual sim with feature qta
04/24/14   as      Moving wrm code to wl1trm.
04/18/14   as      Initial version created
===========================================================================*/

/*===========================================================================

                INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"
#include "rfm.h"
#include "l1const.h"

#ifdef FEATURE_DUAL_SIM
#include "task.h"
#include "msg.h"
#include "trm.h"
#include "l1task_v.h"
#include "rrccmd_v.h"
#include "wl1drxentity.h"
#include "wl1trm.h"
#include "srchgsmdrv.h"
#include "wl1sleep.h"
#include "wl1cxm.h"
#ifdef FEATURE_QSH_MDUMP
#include "wl1_qsh_ext.h"
#endif
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

#ifdef FEATURE_QTA
/* 30 ms are sufficient to freeze all activities in FACH state . Also, as RRC
** files are zipped by qzip, we may face some delay in processing the commands
** and releasing the chain for QTA which may lead to missed page on other sub.
** So assuming a maximum delay of 10 ms, overhead time will be 30+10= 40 ms*/
#define RRC_UNLOCK_FACH_QTA_OVERHEAD_TIMER_IN_MS 40
#endif /* FEATURE_QTA */

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)

/* Different Quick Tuneaway states in WL1:
**   WL1_QTA_STATE_NOT_ACTIVE  - Default state, no QTA active.
**   WL1_QTA_STATE_READY_FOR_QTA   - All prepration needed for QTA to 
                                   proceed are concluded
**   WL1_QTA_STATE_WFW_START_QTA_SENT - Start QTA sent to WFW.  Waiting for response.
**   WL1_QTA_STATE_ACTIVE      - QTA in progress.  GSM is active.
**   WL1_QTA_STATE_WFW_STOP_QTA_SENT    -  QTA stopped, Informed to firmware and L1 will wait
                                           for the command Done Ind. from WFW 
**   WL1_QTA_STATE_QTA_STOPPED  - Intermediate state for acting on Back to back Qta
                                  if present or restoring L1 in state (just before qta)
                                  like unfreezing CM etc. 
**  WL1_QTA_STATE_QTA_CLEANUP   - Intermediate State for handling back to back qta
                                  in case triggered or not (expiry of tuneaway timer) 
*/
typedef enum
{
  WL1_QTA_STATE_NOT_ACTIVE,
  WL1_QTA_STATE_READY_FOR_QTA,
  WL1_QTA_STATE_WFW_START_QTA_SENT,
  WL1_QTA_STATE_ACTIVE,
  WL1_QTA_STATE_WFW_STOP_QTA_SENT,
  WL1_QTA_STATE_QTA_STOPPED,
  WL1_QTA_STATE_QTA_CLEANUP,
  WL1_QTA_STATE_MAX
} wl1_qta_state_enum_type;
#endif

/* Different Quick Diversity Tuneaway states in WL1:
**   WL1_QDTA_STATE_NOT_ACTIVE  - Default state, no QDTA active.
**   WL1_QDTA_STATE_READY_FOR_QDTA   - All preparation needed for QDTA to 
                                   proceed are concluded
**   WL1_QDTA_STATE_WFW_START_QDTA_SENT - Start QDTA sent to WFW.  Waiting for response.
**   WL1_QDTA_STATE_ACTIVE      - QDTA in progress.  GSM is active.
**   WL1_QDTA_STATE_WFW_STOP_QDTA_SENT    -  QDTA stopped, Informed to firmware and L1 will wait
                                           for the command Done Ind. from WFW 
**   WL1_QDTA_STATE_QDTA_STOPPED  - Restore all states in L1 just like before QDTA and inform 
                                    external modules if needed   
*/
typedef enum
{
  WL1_QDTA_STATE_NOT_ACTIVE,
  WL1_QDTA_STATE_READY_FOR_QDTA,
  WL1_QDTA_STATE_WFW_START_QDTA_SENT,
  WL1_QDTA_STATE_ACTIVE,
  WL1_QDTA_STATE_WFW_STOP_QDTA_SENT,
  WL1_QDTA_STATE_QDTA_STOPPED,
  WL1_QDTA_STATE_MAX
} wl1_qdta_state_enum_type;

/* standby mode enums*/
typedef enum {
WL1_SINGLE_STANDBY = 0,
WL1_DUAL_STANDBY
#ifdef FEATURE_TRIPLE_SIM
, WL1_TRIPLE_STANDBY
#endif
} wl1_standby_enum_type;

/* device mode enums*/
typedef enum {
WL1_SINGLE_ACTIVE = 0,
WL1_DUAL_ACTIVE
} wl1_device_mode_enum_type;

/*WL1 dual sim procedures*/
typedef enum {
  WL1_DS_PROC_START=0,
  WL1_DS_PROC_SUSPEND=1,
  WL1_DS_PROC_RESUME=2,
  WL1_DS_PROC_IDLE=3,
  WL1_DS_PROC_STOP=4,
  WL1_DS_PROC_DEACTIVATE=5,
  WL1_DS_PROC_FS=6,
  WL1_DS_PROC_ACQ=7,
  WL1_DS_PROC_SETUP_SBCH=8,
  WL1_DS_PROC_SETUP_NBCH=9,
  WL1_DS_PROC_SETUP_PCH=10,
  WL1_DS_PROC_SETUP_CTCH_FACH=11,
  WL1_DS_PROC_SETUP_DTCH_FACH=12,
  WL1_DS_PROC_SETUP_DCH=13,
  WL1_DS_PROC_PAGING=14,
  WL1_DS_PROC_CELL_RESEL_START=15,
  WL1_DS_PROC_CELL_TRANS_REQ=16,
  WL1_DS_PROC_BPLMN=17,
  WL1_DS_PROC_BPLMN_BCCH_DROP=18,
  WL1_DS_PROC_DRX=19,
  WL1_DS_PROC_GEN_CPHY_SETUP_REQ =20,
  WL1_DS_PROC_SPLIT_SRCH = 21,
  WL1_DS_PROC_CELL_SELECTION_REQ=22,
  WL1_DS_PROC_BCCH =23,
  WL1_DS_PROC_CTCH=24,
  WL1_DS_PROC_CTCH_INV=25,
  WL1_DS_PROC_CTCH_CRITICAL=26,
  WL1_DS_PROC_MEAS=27,
  WL1_DS_PROC_MEAS_IRAT=28,
  WL1_DS_PROC_MAX=29
}wl1_ds_proc_enum_type;

/*WL1 dual sim paging specific information structure*/
typedef struct {
  boolean lock_succ;
  
  boolean cell_trans_req_stat;

  boolean wl1_in_paging_wakeup;
} wl1_ds_paging_spec_info_struct_type;

/*Structure to back up the DS Status during online->FTM->Online mode transitions*/
typedef struct {

  /*Flag to indicate if WL1 has enterend into FTM mode*/
  boolean wl1_is_reset_done_during_ftm_mode; 

  /*True: UE is in dual sim mode; False: UE is in single sim mode*/
  boolean wl1_in_ds_mode;

  /* standby mode: TRUE for Dual standby, FALSE for single standby */
  wl1_standby_enum_type wl1_standby_mode;

  /* device mode: TRUE for dual active , FALSE for single active*/
  wl1_device_mode_enum_type wl1_device_mode;
}wl1_ds_status_struct_type;

/*Structure to hold WL1 dual sim information */
typedef struct {

  /*True: UE is in dual sim mode; False: UE is in single sim mode*/
  boolean wl1_in_ds_mode;

  /* standby mode: TRUE for Dual standby, FALSE for single standby */
  wl1_standby_enum_type wl1_standby_mode;

#ifdef FEATURE_TRIPLE_SIM
  /* Tells whether it is */
  rrc_multi_sim_mode_type wl1_multi_sim_mode;
#endif

  /* device mode: TRUE for dual active , FALSE for single active*/
  wl1_device_mode_enum_type wl1_device_mode;

  /*WL1 procedures that are holding TRM lock*/
  uint32 wl1_ds_proc_mask;

  /*Last WL1 procedure that requested for TRM lock*/
  wl1_ds_proc_enum_type ds_curr_proc;

  /*Last WL1 procedure that requested for TRM lock*/
  wl1_ds_proc_enum_type ds_pend_proc;

  /*Last cphy_setup_cmd procedure that requested for TRM lock*/
  wl1_ds_proc_enum_type last_cphy_setup_proc;

  /*Indicate whether cmd is pending waiting for the sleep proc to be completed*/
  boolean trm_lock_sig_pending_on_sleep;

  /* Flag to indicate Ext cmd processing with no TRM lock */
  boolean process_ext_cmd_without_lock;

  /* to store the information of req_n_not callback.*/
  trm_req_n_not_return_data wl1_last_req_and_not_data; 
}wl1_ds_db_struct_type;

/*Structure to hold WL1 dual sim statistics*/
typedef struct {
  uint32 lock_req_and_not_cnt;

  uint32 lock_req_and_not_succ_cnt;

  uint32 lock_req_and_not_fail_cnt;

  uint32 lock_req_cnt;

  uint32 lock_req_succ_cnt;

  uint32 lock_req_fail_cnt;

  uint32 lock_res_cnt;
} wl1_ds_overall_stat_struct_type;

typedef void WL1_DS_PROC_CB_PTR(void);

#ifdef FEATURE_WCDMA_EFACH_MSIM
typedef void WL1_DS_HS_DPCCH_SETUP_CB_FUNC_TYPE(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_EFACH_MSIM */

typedef void (* wrm_srch_lock_grant_cb_type) (boolean, wsub_id_e_type wsub_id);

/*Structure to hold WL1 dual sim information on a per procedure basis*/
typedef struct {
  /*Process the RRC command even if lock failed,
  but without mdsp and RF*/
  boolean can_lock_fail;

  wrm_activity_enum_type activity;

  /* How long the resource will be needed for (in milli secs) */
  uint32 lock_req_dur_in_ms;

  /* Callback to notify client when resource is granted */
  WL1_DS_PROC_CB_PTR *lock_grant_callback;

  uint32 lock_req_timeout_in_ms;

  uint32 trm_lock_proc_fail_cnt;

  uint32 trm_lock_proc_succ_cnt;

  l1_cmd_enum_type rrc_to_l1_cmd;

  rrc_cmd_e_type l1_to_rrc_cnf;
}wl1_ds_proc_info_struct_type;

/* =========================================================== */
/* Log packet definitions that reports the Tuneaway properties  */
/* =========================================================== */

/* Stores the types of Tune-Aways */
typedef enum
{
  /* Quick Tuneaway flag */
  WL1TRM_TUNEAWAY_LOG_QTA = 1,

  /* Long Tuneaway flag */
  WL1TRM_TUNEAWAY_LOG_LTA,

  /* QDTA flag */
  WL1TRM_TUNEAWAY_LOG_QDTA
} wl1trm_tuneaway_log_type;

/* Flags to the wl1trm_tuneaway_log_set_time function to record
** specific timeticks for Tuneaways */
typedef enum
{
  /* Flag that records the timetick when CPHY_SUSPEND_WCDMA_MODE_REQ is received */
  SET_TUNEAWAY_SUSPEND_START_TIMETICK,

  /* Flag that clears the timetick of suspend, used in case the FACH QTA is cancelled. */
  CLEAR_TUNEAWAY_SUSPEND_START_TIMETICK,

  /* Flag that records the timetic when RRC_CPHY_SUSPEND_WCDMA_MODE_CNF is sent */
  SET_TUNEAWAY_SUSPEND_END_TIMETICK,

  /* Flag that records the timetick when RESUME_WCDMA_MODE_REQ is received */
  SET_TUNEAWAY_RESUME_START_TIMETICK,

  /* Flag that records the timetick when CPHY_SETUP_CNF is sent */
  SET_TUNEAWAY_SETUP_END_TIMETICK
}wl1trm_tuneaway_log_set_time_type;

/* WL1TRM_TUNEAWAY_LOG_SUBMIT command type that contains the WL1TRM_TUNEAWAY_LOG_PKT
** log packet data */
typedef struct
{
  /* Command header for l1m command queue */
  l1_local_cmd_hdr_type   hdr;

  /* Log packet data associated with the command */
  wl1trm_tuneaway_log_data_struct_type log_data;

} wl1trm_tuneaway_log_submit_cmd_type;

typedef struct
{
  #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
  wl1_qta_state_enum_type  wl1_qta_state;

  /*Variable to track ciphering key restoring status after QTA */
  boolean restore_ul_ciph_keys_after_qta;

  boolean wl1_back_to_back_qta;

  /* Time in sclk stored when a winning client tries WRM unlock */
  uint32  wl1_unlock_by_time_in_sclk;

  /*Variable to differentiate UNLOCK_BY request coming for CA primary alone or both primary devices*/
  wrm_resource_enum_type unlock_by_primary_resource;

  #ifdef FEATURE_TRIPLE_SIM
  trm_client_enum_t wl1_back_to_back_qta_client_id;
  #endif

  #ifdef FEATURE_DUAL_SIM_WCDMA_TUNEAWAY
  /*Stores winning client info for primary device*/
  trm_client_info_t winning_ta_client;
  /*Stores winning client info for secondary device*/
  trm_client_info_t secondary_winning_ta_client;
  #endif /* FEATURE_DUAL_SIM_WCDMA_TUNEAWAY */
  #endif /* FEATURE_DUAL_SIM && FEATURE_QTA */
  
  wl1_qdta_state_enum_type  wl1_qdta_state;

  uint32  wl1_secondary_unlock_by_time_in_sclk;  

  trm_time_t trm_reserve_time_sclk_val;
  
  /* Variables to hold the last trm band req grant info*/
  l1_freq_scan_band_enum_type wl1_last_granted_band;

  trm_frequency_type_t wl1_last_granted_trm_freq_info;

  /*TRM lock acquisition success/failure statistics when in Dual SIM mode*/
  wl1_ds_overall_stat_struct_type wl1_ds_overall_stat;

  #ifdef FEATURE_WCDMA_TCXOMGR30
  uint32 wl1_already_has_tcxo;
  #endif

  /* Indicates Client's TuneAway prefence when reserving for a chain 
   * if client doesn't have preference it should be set to TRM_TUNEAWAY_NONE*/
    trm_tuneaway_enum_type    ta_info;

  /* Indicates Client's Restriction info that is causing current Unlock Callback to client 
      For Example, 
      If both RF and FW restrictions are present: 
          restrict_info =  ( 1 << TRM_RF_RESTRICTION  ) | ( 1 << TRM_FW_RESTRICTION  ); 
      If only RF restrictions is valid: 
          restrict_info =  ( 1 << TRM_RF_RESTRICTION  ) ; 
  */
  uint32                    restrict_info;

  /*Indicates if the last QTA gap opened was empty */
  boolean empty_qta_gap;
  /*Indicates if the last QDTA gap opened was empty */
  boolean empty_qdta_gap;
  /*Pend external command processing when QDTA is enabled*/
  boolean qdta_flag_pend_ext_cmd_processing;

  /* Indicates whether we have received an unlock by request for qta */
  boolean wl1_unlock_by_received_for_qta;
} wl1_multisim_w2x_ta_db_type;

typedef struct
{
  /* Operation Start/End QTA*/
  trm_state_oper_enum_type operation;

  /* Information regarding the Source client (X tech) in X->Y QTA*/
  trm_client_enum_t source_client;

  /* START_QTA state - RF Source (X tech) tech data for Script Building in 
     END_QTA State - This will be set to NULL */
  rfm_meas_common_param_type* rf_params;

  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
  /* Array of path allocated for both Primary ([0]) & Secondary ([1]) 
     If path is not allocated, device and sig_path will be marked as RFM_INVALID_DEVICE 
     & RFM_SIG_PATH_INVALID respectively*/
  rfm_path_type path_allocated[TRM_NUM_PRI_DIV_CLIENTS_PER_TECH];
  #else
  /* Array of devices allocated for both Primary ([0]) & Secondary ([1]) 
      If device is not allocated, it will be marked as RFM_INVALID_DEVICE*/
  rfm_device_enum_type dev_allocated[TRM_NUM_PRI_DIV_CLIENTS_PER_TECH];
  #endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED)*/

}wrm_qta_event_callback_data_type;

typedef struct
{
  /* QTA info passed by TRM */
  wrm_qta_event_callback_data_type qta_data;

  /* Flag to identify if an ext command
  ** was pended while in x2w qta gap*/
  boolean ext_cmd_pended_for_x2w_qta_gap;
}wl1_multisim_x2w_ta_db_type;

/* Multi-Sim Configuration of UE: SS/DSDA/DSDS/TSTS*/
typedef enum
{
  /* Single SIM */
  WL1_MULTI_SIM_SS,
    /* Dual Sim Dual active */
  WL1_MULTI_SIM_DSDA,
  /* Dual Sim Dual standby */
  WL1_MULTI_SIM_DSDS,
  /* Triple Sim Triple standby */
  WL1_MULTI_SIM_TSTS,
  /* Invalid Sim Config */
  INVALID_CONFIG
}wl1_ds_multi_sim_config_enum_type;

typedef struct
{
  #if defined(FEATURE_DUAL_SIM_WCDMA_TUNEAWAY) && defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
  /* How much before actual QTA gap must we disable equalizer.  Value MUST BE
  ** LESS THAN "wl1_qta_start_to_qta_gap_usec" above. */
  uint32  wl1_qta_wfw_start_to_qta_gap_usec;

  /* QTA start timestamp */
  uint32  wl1_qta_start_time_in_sclk;

 /* Latest RoTA Gap Start & End sclks*/
  uint32  wl1_rota_start_sclks;
  uint32  wl1_rota_end_sclks;

  /* Latest RoTA Gap Start & End subfn*/
  uint16  wl1_rota_start_subfn;
  uint16  wl1_rota_end_subfn;

   /* QTA start CFN & SlotNumber  */
  uint16  wl1_qta_start_cfn;
  uint16  wl1_qta_start_slot_num;

  /* Timetick values at beginning and end of QTA. */
  timetick_type start_qta_timetick, stop_qta_timetick, qta_duration_timetick;
  
  /* This will be set to TRUE if we started FRM timer*/
  boolean wl1_fach_reservation_timer_on;

  uint8 wl1_frames_after_qta_stop;

  /*QTA RF startup command Params*/
  mcalwcdma_cm_irat_rf_buf_ctrl_struct_type* qta_startup_cmd_params_ptr;

  #endif  /* #if defined(FEATURE_DUAL_SIM_WCDMA_TUNEAWAY) 
                    && defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA) */

  /* QDTA start timestamp */
  uint32  wl1_qdta_start_time_in_sclk;
  
  uint32  wl1_qdta_wfw_start_to_qdta_gap_usec;
  
  /* QDTA start CFN & SlotNumber  */
  uint16  wl1_qdta_start_cfn;
  uint16  wl1_qdta_start_slot_num;

  /* Timetick values at beginning and end of QDTA. */
  timetick_type start_qdta_timetick, stop_qdta_timetick;

  /* This flag indicates if a call to dl_cell_check_tddet_status is pending. If this flag
   is set, dl_cell_check_tddet_status should be called after the QTA gap is closed.*/
  boolean wl1_qta_dl_tddet_pending;

  /* This flag is enabled when wl1 is ready to perform LTA and is set in wl1_ok_for_tuneaway function
  ** before the LTA starts */
  boolean tuneaway_lta_underway_flag;

  /* This flag is enabled when wl1 is ready to perform QTA and is set in wl1_ok_for_qta function
  ** before the QTA starts */
  boolean tuneaway_qta_underway_flag;

  /* This flag is enabled when wl1 is ready to perform QDTA and is set in start_qdta function
  ** before the QDTA starts */
  boolean tuneaway_qdta_underway_flag;

  uint16 tuneaway_duration;

  /* This is the tuneaway duration returned by RRC and copied and kept locally. This is specific to L1 and is
     used by all L1 modules like searcher, DL etc to get the tuneaway duration */
  uint16 wl1_tuneaway_duration;

  /*Whether re-downloading FW is enabled in WL1*/
  uint8 wl1_ds_re_download_fw;

}wl1_multisim_internal_db_type;

typedef struct
{
  #ifdef FEATURE_DUAL_SIM_WCDMA_TUNEAWAY
  boolean print_full_tuneaway_config;

  /* Run-time debug flag, whether or not to read W2G tuneaway data from EFS/NV. */
  boolean wl1_w2g_tuneaway_read_nv;

  wl1_multisim_internal_db_type wl1_multisim_internal_db;

  /* To time different states of inter-RAT tuneaway. */
  timer_type wl1_tuneaway_timer;

  /* To ignore SCCH order processing in QTA*/
  timer_type wl1_ignore_scch_during_qta_timer;

  #ifdef FEATURE_QTA
  /* Debug timer for QTA feature. */
  timer_type wl1_qta_debug_timer;
  #endif
  #endif

  #ifdef FEATURE_QTA
  uint8 wcdma_qta_rota_nv;
  #endif

  /* The structure variable that stores all the required data for WL1TRM_TUNEAWAY_LOG_PKT
   * log packet during QTA or LTA */ 
  wl1trm_tuneaway_log_data_struct_type wl1trm_tuneaway_log_data;

  /* Timer to block QTA until response is received for RACH or timer expires */
  rex_timer_type wl1_block_qta_until_rach_done_timer;

  timer_type wl1_ignore_cpc_scch_during_qta_timer;

}wl1multisim_int_cntrl_params_type;

typedef struct
{

  /*Information about current WL1 Dual SIM procedure*/
  wl1_ds_db_struct_type wl1_ds_db;

  /* Database to store info when W is the target tech */
  wl1_multisim_x2w_ta_db_type wl1_multisim_x2w_ta_db;

  /* Database to store info when W is the source tech */
  wl1_multisim_w2x_ta_db_type wl1_multisim_extern_db;

  /*Paging specific Information when in Dual SIM mode*/
  wl1_ds_paging_spec_info_struct_type wl1_ds_paging_spec_info;

  /*To save the ext_cmd_ptr's of active proc's*/
  l1_ext_cmd_type* ds_proc_ext_cmd_ptr[WL1_DS_PROC_MAX];

  /*To indicate if we are in a MEAS only gap*/
  boolean meas_only_gap_in_progress;

  /*To check if external command need to be pended in meas only gap*/
  boolean ext_cmd_pended_for_meas_only_gap;

  /*To check if Max inversion timer needs to be adjusted based on alt_sub state
  **If alt_sub_in_data Max inversion timer should be WL1_DS_ALT_SUB_DATA_MAX_INV_PRIORITY_DURATION_TIME_MS
  **else if alt_sub_in_idle Max inversion timer should be WL1_DS_ALT_SUB_IDLE_MAX_INV_PRIORITY_DURATION_TIME_MS*/
  boolean re_eval_timer_for_alt_sub_in_data;
  /*Used to indicate whether RRC indicated to bump up duty cycle or not*/
  boolean update_acq_duty_cycle;
  /*Timer to start and wait until we invert the requested priority with TRM (if applicable)*/
  rex_timer_type priority_inversion_timer;

  /*Timer to start once we are using inverted priority and revert to normal priority
   *upon expiry */
  rex_timer_type max_inversion_timer;

  /*To indicate if priority inversion has started or not.*/
  boolean priority_inv_in_progress;

  /*Activity for which we are doing priority inversion currently*/
  wrm_activity_enum_type inversion_activity;

  /*To configure WCDMA TA types*/
  wl1_wcdma_ta_config_type wl1_wcdma_ta_config_nv;
 #ifdef FEATURE_WCDMA_EFACH_MSIM
  WL1_DS_HS_DPCCH_SETUP_CB_FUNC_TYPE *wl1_ds_hs_dpcch_setup_cb_ptr;
 #endif /* FEATURE_WCDMA_EFACH_MSIM */

  /*Timer to keep track of modify_band req sent out to TRM for primary */
  rex_timer_type cphy_band_req_timeout_timer;

  /*Timer to keep track of modify_band req sent out to TRM for CA */
  rex_timer_type cphy_band_req_for_ca_timeout_timer;

  /*Timer to keep track of req_n_not sent out to TRM for CA */
  rex_timer_type cphy_carrier_aggr_resource_timeout_timer;

  /*wl1_multisim critical section*/
  rex_crit_sect_type wl1_multisim_mutex_lock;

  /*Used to trigger DL_WEAK_IND/OOS during cell selection and Inter F resel*/
  rex_timer_type wl1_no_lock_timer;
  
  /*Will be set when the no lock timer expires and OOS is triggered while going to sleep.*/
  boolean wl1_declare_oos;

  /* Used to indicate the duration of low priority inversion timer */
  uint16 alt_sub_data_low_prio_inv_timer; 

  /* Used to indicate the duration of maximum priority inversion timer */
  uint16 alt_sub_data_max_prio_inv_timer;

  /*To indicate if we have either
    1) cphy in progress for ifhho
    2) Cell Transition in Progress
  */
  boolean ext_cmd_in_progress;

  /*To check if RnN is pended since either 
    1) Cphy in progress for IFHHO
    2) Cell Transition is in Progress
  */
  boolean req_n_not_pended_for_ext_cmd_in_progress;

}wl1multisim_ext_cntrl_params_type;

extern wl1multisim_ext_cntrl_params_type wl1multisim_ext_cntrl_params[WCDMA_NUM_SUBS];

extern wl1_ds_status_struct_type wl1_ds_status_backup_db[WCDMA_NUM_SUBS];

extern wl1_ds_proc_info_struct_type wl1_ds_proc_info[WL1_DS_PROC_MAX];

extern const wrm_res_rsn_for_activity_type wrm_res_rsn_for_activity[WRM_MAX_ACTIVITIES];

/*MACROS for Critical Section*/
#define WL1_MULTISIM_MUTEX_LOCK() REX_ISR_LOCK(&wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_mutex_lock)
#define WL1_MULTISIM_MUTEX_FREE() REX_ISR_UNLOCK(&wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_mutex_lock)

#define WL1_IN_DS_MODE() (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_in_ds_mode)

#define WL1_IN_DUAL_ACTIVE_MODE() \
        ( \
         ( (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_device_mode == WL1_DUAL_ACTIVE) && (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_standby_mode == WL1_DUAL_STANDBY) ) \
         ? TRUE : FALSE \
        )

#define WL1_SAVE_DS_MODE_STATUS_FOR_FTM() \
       do{                    \
        wl1_ds_status_backup_db[wsub_id].wl1_in_ds_mode =  wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_in_ds_mode;   \
        wl1_ds_status_backup_db[wsub_id].wl1_device_mode = wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_device_mode;  \
        wl1_ds_status_backup_db[wsub_id].wl1_standby_mode = wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_standby_mode;\
       }while(0)

#define WL1_RESTORE_DS_MODE_STATUS_FOR_ONLINE() \
       do{                    \
        wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_in_ds_mode = wl1_ds_status_backup_db[wsub_id].wl1_in_ds_mode;    \
        wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_device_mode= wl1_ds_status_backup_db[wsub_id].wl1_device_mode;   \
        wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_standby_mode = wl1_ds_status_backup_db[wsub_id].wl1_standby_mode;\
       }while(0)

#define WL1_FORCE_RESET_DS_MODE() \
       do{                    \
        wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_in_ds_mode = 0;  \
        wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_device_mode = WL1_SINGLE_ACTIVE; \
        wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_standby_mode = WL1_SINGLE_STANDBY; \
       }while(0)

#define WL1_NOT_ACTIVE() ((WL1_GET_L1M_STATE() == L1M_DEACTIVE) || (WL1_GET_L1M_STATE() == L1M_DEEP_SLEEP) || (WL1_GET_L1M_STATE() == L1M_STOPPED))

#define WRM_WL1_RESERVATION_TIME_SCLK_INVALID 0xFFFFFFFF

#define WL1_INTRA_F_SRCH_DUR (M_BY_N_MS_IN_SC(60,1))
#define WL1_INTER_F_SRCH_DUR (M_BY_N_MS_IN_SC(80,1))
#define WL1_INTER_RAT_SRCH_DUR (M_BY_N_MS_IN_SC(100,1))
#define WL1_LTE_SRCH_DUR (M_BY_N_MS_IN_SC(40,1))


#define WL1_DS_NUM_PEND_EXT_CMD 3

#define WL1_DS_INVALID_CMD_IDX 0xFF

/*Macro to check whether GL1 is allowing Tuneaway.*/
#define WRM_G2W_TA_MODE_ACTIVE(sub_id) wl1multisim_determine_g2w_ta_mode(sub_id)

#define WRM_W2X_QDTA_STATE(wsub_id) (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_extern_db.wl1_qdta_state)

#define WL1_GET_TUNEAWAY_TYPE(wsub_id) (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_extern_db.ta_info)

/*Unit test definitions*/
#if 0
#define TRM_LAST_RESOURCE 1
#define TRM_CHANNEL_MAINTENANCE 1
#define TRM_NUM_REASONS 1
#endif

/*If TRM lock request fails consecutively for this threshold count, then OOS
is declared to RRC*/
#define WL1_DS_PAG_TRM_LOCK_OOS_THRESH (8)

#define WL1_DS_IDLE_NO_LOCK_TIMEOUT_MS     12000

/*Macros to check whethe a procedure is valid and of particular type*/
#define WL1_DS_IS_PROC_VALID(ds_proc) (ds_proc < WL1_DS_PROC_MAX)

/* Macro to find the activity for given proc */
#define WL1_DS_GET_ACTIVITY_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].activity)

/*Macros to handle statistics corresponding to a procedure*/
#define WL1_DS_INC_FAIL_CNT_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].trm_lock_proc_fail_cnt++)

#define WL1_DS_INC_SUCC_CNT_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].trm_lock_proc_succ_cnt++)

#define WL1_DS_INC_OVERALL_FAIL_CNT() (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_extern_db.wl1_ds_overall_stat.lock_req_fail_cnt++)

#define WL1_DS_INC_OVERALL_SUCC_CNT() (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_extern_db.wl1_ds_overall_stat.lock_req_succ_cnt++)

#define WL1_DS_IS_CMD_PTR_VALID(ds_proc) (wl1multisim_ext_cntrl_params[wsub_id].ds_proc_ext_cmd_ptr[ds_proc] != NULL)

/*Macros to get various fields of a procedure*/
#define WL1_DS_CAN_LOCK_FAIL_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].can_lock_fail)

#define WL1_DS_GET_RESOURCE_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].resource_needed)

#define WL1_DS_GET_DUR_IN_MS(ds_proc) (wl1_ds_proc_info[ds_proc].lock_req_dur_in_ms)

#define WL1_DS_GET_DUR_IN_SCLK(ds_proc) (M_BY_N_MS_IN_SC(WL1_DS_GET_DUR_IN_MS(ds_proc),1))

#define WL1_DS_GET_LOCK_RSN_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].lock_reason)

#define WL1_DS_GET_CB_PTR_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].lock_grant_callback)

#define WL1_DS_GET_LOCK_REQ_TIMEOUT_FOR_PROC(ds_proc) (wl1_ds_proc_info[ds_proc].lock_req_timeout_in_ms)

#define WL1_DS_GET_PENDED_PROC() (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.ds_pend_proc)

#define WL1_DS_SET_PENDED_PROC(proc, wsub_id) (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.ds_pend_proc = proc)

#define WL1_DS_IS_PEND_PROC_VALID(wsub_id) (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.ds_pend_proc != WL1_DS_PROC_MAX)

#define WL1_DS_SET_LOCK_REQ_TIMEOUT_FOR_PROC(ds_proc,msec) \
  (wl1_ds_proc_info[ds_proc].lock_req_timeout_in_ms = msec)

#define WL1_DEFAULT_GRANT_CB (wl1_ds_grant_cb)

/*Macros to handle WL1 dual sim procedure bit mask*/
#define WL1_DS_PROC_BMSK(ds_proc) ((uint32)(1 << ((uint32)ds_proc)))

#define WL1_DS_RESET_DS_PROC_BMASK() \
(wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_ds_proc_mask = 0)

#define WL1_DS_ADD_PROC_TO_DS_BMASK(ds_proc) \
  (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_ds_proc_mask |= WL1_DS_PROC_BMSK(ds_proc))

#define WL1_DS_REM_PROC_FROM_DS_BMASK(ds_proc) \
  (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_ds_proc_mask &= (~(WL1_DS_PROC_BMSK(ds_proc))))


/*Macros to check whether any dual sim procedure is active in WL1*/
#define WL1_IN_DS_WITH_ANY_LOCK() (WL1_IN_DS_MODE() && ((WRM_CLI_STATE_IS_IN_LOCK(WCDMA_TRM, WRM_PRIMARY_CLIENT_L1, wsub_id)) || (wl1idle_ta_mgr_lock_rel_offline_activity_pending(wsub_id))))

#define WL1_IN_DS_WITH_NO_LOCK() (WL1_IN_DS_MODE() && ((!WRM_CLI_STATE_IS_IN_LOCK(WCDMA_TRM, WRM_PRIMARY_CLIENT_L1, wsub_id)) && (!wl1idle_ta_mgr_lock_rel_offline_activity_pending(wsub_id))))

#define WL1_DS_IS_ANY_PROC_ACTIVE() (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_ds_proc_mask != 0)

#define WL1_DS_IS_PROC_ACTIVE(ds_proc) \
  ((wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_ds_proc_mask & WL1_DS_PROC_BMSK(ds_proc)) != 0)
 
#define WL1_IS_ANY_OTHER_PROC_ACTIVE(ds_proc) \
  ((wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.wl1_ds_proc_mask & (~(WL1_DS_PROC_BMSK(ds_proc)))) != 0)

#define WL1_IN_DS_WITH_PAG_LOCK() (WL1_IN_DS_MODE() && (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_paging_spec_info.lock_succ))

#define WL1_IN_DS_WITH_NO_PAG_LOCK() (WL1_IN_DS_MODE() && (!wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_paging_spec_info.lock_succ))

#define WL1_IS_CELL_TRANS_REQ_SUCC() (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_paging_spec_info.cell_trans_req_stat)

#define WL1_IS_IN_PCH_SLEEP_DS_NO_LOCK() ((WL1_IN_DS_MODE()) && \
  (WL1_GET_L1M_STATE() == L1M_PCH_SLEEP) && \
  (WL1_IN_DS_WITH_NO_LOCK()))

#define WL1_IS_IN_PCH_SLEEP_DS_LOCK() ((WL1_IN_DS_MODE()) && \
  (WL1_GET_L1M_STATE() == L1M_PCH_SLEEP) && \
  (WL1_IN_DS_WITH_ANY_LOCK()))

#define WL1_DS_IS_CMD_IDX_INVALID(cmd_idx) \
  (cmd_idx == WL1_DS_INVALID_CMD_IDX)

#define WL1_DS_IS_TRM_SIG_PENDING_ON_SLEEP() \
  (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_db.trm_lock_sig_pending_on_sleep == TRUE)

#define WL1_DS_IS_WL1_IN_PAGING_WAKEUP() \
  (wl1multisim_ext_cntrl_params[wsub_id].wl1_ds_paging_spec_info.wl1_in_paging_wakeup == TRUE)

/* Priority inversion time employed when requesting for lock for split acq*/
#define WRM_L1_SPLIT_ACQ_PRIORITY_INVERSION_TIME_MS 160

/* Priority inversion time employed when requesting for lock for acq if alternate sub is in data call*/
#define WRM_ALT_SUB_DATA_ACQ_PRIO_INV_TIME_MS 2000

/* Priority inversion time employed with high duty cycle when requesting for lock for acq if alternate 
   sub is in data call*/
#define WRM_ALT_SUB_DATA_HIGH_DUTY_CYCLE_ACQ_PRIO_INV_TIME_MS 1000

/* Priority inversion time employed when requesting for lock for acq if alternate sub is in OOS/IDLE*/
#define WRM_ALT_SUB_IDLE_ACQ_PRIO_INV_TIME_MS 1000

/* Max duration for which WCDMA can hold lock using inverted ACQ priority when alternate sub is in IDLE/OOS*/
#define WL1_DS_ALT_SUB_IDLE_MAX_INV_PRIORITY_DURATION_TIME_MS  1000

/* Max duration for which WCDMA can hold lock using inverted ACQ priority when alternate sub is in data call*/
#define WL1_DS_ALT_SUB_DATA_MAX_INV_PRIORITY_DURATION_TIME_MS  200

/* Max duration for which WCDMA can hold lock using inverted ACQ priority when alternate sub is in data call.
   This will be used when we need lock for more than 200ms with high priority */
#define WL1_DS_ALT_SUB_DATA_HIGH_DUTY_CYCLE_MAX_INV_PRIORITY_DURATION_TIME_MS  250

/* Max duration for which WCDMA can hold lock with CM_INV priority.*/
/*Changed the value from 8 sec to 20.58(100ms extra buffer time added to 20.48)*/
#define WL1_DS_MAX_CM_INV_DURATION_TIME_MS  20580

/* Timeout timer value for band change request during CPHY */
#define WRM_BAND_REQ_TIMEOUT_TIMER_IN_MS 200

/* How much time before actual QDTA gap must we notify WFW about start of QDTA*/
#define WFW_START_TO_QDTA_GAP_TIME_IN_USEC 6000;

/* If timer expires we fail CPHY to RRC with no resource reason */
#define WRM_CARR_AGGR_REQ_TIMEOUT_TIMER_IN_MS 100

#define WL1_DS_CLR_LOCK_ACQ_SIGS(wsub_id)\
  {(void) rex_clr_sigs(wcdma_l1_get_tcb(wsub_id), WL1_TRM_LOCK_ACQ_SIG);\
   (void) rex_clr_sigs(wcdma_l1_get_tcb(wsub_id), WL1_TRM_LOCK_FAILED_SIG);}

#define WL1_DS_START_PRIO_INV_TMR(timer, wsub_id) \
  ((void)rex_set_timer(&wl1multisim_ext_cntrl_params[wsub_id].priority_inversion_timer,timer))
  
/*To clear all priority inversion variables.*/
#define WL1_DS_RESET_PRIORITY_INV_VARS(resource, wsub_id) \
do { \
     if(resource == WCDMA_TRM) \
     {  \
       (void) rex_clr_timer(&wl1multisim_ext_cntrl_params[wsub_id].priority_inversion_timer); \
       wl1multisim_ext_cntrl_params[wsub_id].priority_inv_in_progress = FALSE; \
       wl1multisim_ext_cntrl_params[wsub_id].re_eval_timer_for_alt_sub_in_data = FALSE; \
       (void) rex_clr_timer(&wl1multisim_ext_cntrl_params[wsub_id].max_inversion_timer); \
     } \
}while (0)

/* This timer is run once WCDMA has started using Inverted priority and until this time
     all activities use this inverted prio. Once the timer expires, we will switch back to normal priority*/
#define WL1_DS_START_MAX_INVERT_PRIO_TIME(resource, timer, wsub_id) \
do {\
     if(resource == WCDMA_TRM)\
     {\
       (void)rex_set_timer(&wl1multisim_ext_cntrl_params[wsub_id].max_inversion_timer,\
         timer); \
     } \
}while (0)

/*Macro to check if the max inversion timer is running.*/
#define WL1_DS_IS_MAX_INV_TIMER_RUNNING(wsub_id) \
  (rex_get_timer(&wl1multisim_ext_cntrl_params[wsub_id].max_inversion_timer) != 0)

/*Reset the max priority inversion variables. This will be called once the max inversion timer expires and
    we shouldn't be using the inversion priority anymore.*/
#define WL1_DS_RESET_MAX_PRIO_INV_TMR(resource, wsub_id) \
do { \
     if(resource == WCDMA_TRM) \
     {  \
       (void) rex_clr_timer(&wl1multisim_ext_cntrl_params[wsub_id].max_inversion_timer); \
     } \
}while (0)

/*Macro to reset the priority inversion timer incase it was running*/
#define WL1_DS_RESET_PRIO_INV_TMR(resource, wsub_id) \
do { \
    if(resource == WCDMA_TRM) \
    {  \
      (void) rex_clr_timer(&wl1multisim_ext_cntrl_params[wsub_id].priority_inversion_timer); \
    } \
}while (0)

#define WL1_DS_IS_PRIO_INV_IN_PROG(wsub_id) \
  (wl1multisim_ext_cntrl_params[wsub_id].priority_inv_in_progress == TRUE)


#ifdef FEATURE_QTA
/* MACRO to set/reset ext command pended in x2w qta gap flag */
#define WL1_X2W_QTA_SET_EXT_CMD_PENDING(value) \
  (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_x2w_ta_db.ext_cmd_pended_for_x2w_qta_gap = value)

/* MACRO to query if ext command was pended in x2w qta gap */
#define WL1_X2W_QTA_GET_EXT_CMD_PENDING() \
  (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_x2w_ta_db.ext_cmd_pended_for_x2w_qta_gap)
#endif

/*Macro to get the client that has requested for tuneaway*/
#define WL1_DS_GET_WINNING_TA_CLIENT() (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_extern_db.winning_ta_client.client_id)

/*Macro to get reason for which Idle tech has requested the TA gap*/
#define WL1_DS_GET_WINNING_TA_CLIENT_REASON() (wl1multisim_ext_cntrl_params[wsub_id].wl1_multisim_extern_db.winning_ta_client.reason)

/* MACRO to set/reset ext command pended in Meas only gap */
#define WL1_MEAS_ONLY_GAP_SET_EXT_CMD_PENDING(value) \
  (wl1multisim_ext_cntrl_params[wsub_id].ext_cmd_pended_for_meas_only_gap = value)

/* MACRO to query if ext command was pended in Meas only gap */
#define WL1_MEAS_ONLY_GAP_IS_EXT_CMD_PENDING() \
  (wl1multisim_ext_cntrl_params[wsub_id].ext_cmd_pended_for_meas_only_gap)


/*MACRO to set/reset MEAS only gap flag*/
#define WL1_DS_SET_MEAS_ONLY_GAP_VARS(value) \
  (wl1multisim_ext_cntrl_params[wsub_id].meas_only_gap_in_progress = value)

/* MACRO to query if we are in MEAS only gap*/
#define WL1_DS_GET_MEAS_ONLY_GAP_VARS() \
  (wl1multisim_ext_cntrl_params[wsub_id].meas_only_gap_in_progress)

/*MACRO to set/reset in case 
  1) IFHHO in Progress
  2) Cell Reselection in Progress 
     flag*/
#define WL1_DS_SET_EXT_CMD_IN_PROGRESS_VAR(value) \
  (wl1multisim_ext_cntrl_params[wsub_id].ext_cmd_in_progress = value)

/*MACRO to Get Value in case 
  1) IFHHO in Progress
  2) Cell Reselection in Progress 
     flag*/
#define WL1_DS_GET_EXT_CMD_IN_PROGRESS_VAR() \
  (wl1multisim_ext_cntrl_params[wsub_id].ext_cmd_in_progress)

/*MACRO to set/reset req_n_not_pended_for_ext_cmd_in_progress*/
#define WL1_DS_SET_REQ_N_NOT_PENDED_FOR_EXT_CMD_IN_PROGRESS_VAR(value) \
  (wl1multisim_ext_cntrl_params[wsub_id].req_n_not_pended_for_ext_cmd_in_progress = value)

/* MACRO to query value of req_n_not_pended_for_ext_cmd_in_progress*/
#define WL1_DS_GET_REQ_N_NOT_PENDED_FOR_EXT_CMD_IN_PROGRESS_VAR() \
  (wl1multisim_ext_cntrl_params[wsub_id].req_n_not_pended_for_ext_cmd_in_progress)

/* Is frequency a valid value*/
#define IS_CHAN_VALID(chan) ((chan != NO_FREQUENCY) && (chan != 0))

#define WL1_DS_CLR_NO_LOCK_TIMER() \
do{ \
    (void) rex_clr_timer(&wl1multisim_ext_cntrl_params[wsub_id].wl1_no_lock_timer); \
    (void) rex_clr_sigs(wcdma_l1_get_tcb(wsub_id), L1_NO_LOCK_TIMER_SIG); \
    wl1multisim_ext_cntrl_params[wsub_id].wl1_declare_oos = FALSE; \
  }while(0)

/*Macro to get the value for no lock timer*/
#define WL1_GET_NO_LOCK_TIMER(sub_id) (rex_get_timer(&wl1multisim_ext_cntrl_params[sub_id].wl1_no_lock_timer))

/*===========================================================================

                FUNCTION DECLARATIONS FOR MODULE

=========================================================================*/
extern boolean wl1idle_ta_mgr_lock_rel_offline_activity_pending( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_INIT

DESCRIPTION
  This function initialized WL1 dual sim related globals. This function must be
  once after power-on.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_init( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_MSIM_TEST_FRAMEWORK
/*===================================================================================
FUNCTION     WL1_DS_FILL_AND_UPDATE_ELS_INFO

DESCRIPTION 
   This will update Extended LTE Signalling (ELS) state information to TRM.
   There are two types:
   a) PERIODIC GAP 
      - used to decode PAGE
   b) APERIODIC GAP
      - used to decode CTCH/BCH
      - during MEAS_GAP/OOS
      - during RnN

DEPENDENCIES
  None.

PARAMETERS
  gap_type  - PERIODIC/APERIODIC GAP
  els_activity - WL1 ELS procedure for which lock is held or requested
  wrm_activity - WRM activity for which lock is held or requested
  gap_start - time for which idle tech has requested a lock in sclks
  duration  - idle tech ELS duration scheduled in sclks
  wsub_id   - wsub id

RETURN VALUE
  TRUE  - update ELS state information is successful.
  FALSE - update ELS state information failed.

======================================================================================*/
boolean wl1_ds_fill_and_update_els_info(trm_els_gap_enum_t gap_type,
                         trm_els_activity_enum_t els_activity,
                         wrm_activity_enum_type wrm_activity,
                         wrm_time_type gap_start,
                         wrm_duration_type duration,
                         wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION     WL1_DS_REQ_AND_NOT_LOCK_LOCK_FOR_PROC

DESCRIPTION
  This function requests for lock from WRM and returns success/failure to the
  caller.

DEPENDENCIES
  None.

PARAMETERS
  ds_proc:.the procedure for which WL1 needs the TRM lock

RETURN VALUE
  TRUE: lock acquisition successful
  FALSE: lock acquisition failed

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_req_and_ntfy_lock_for_proc(wl1_ds_proc_enum_type ds_proc, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_REQ_LOCK_FOR_PROC

DESCRIPTION
  This function requests lock for WL1 paging.

DEPENDENCIES
  None.

PARAMETERS
  ds_proc:.the procedure for which WL1 needs the TRM lock

RETURN VALUE
  boolean:True - Chain is granted
  False - else

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_ds_req_lock_for_proc(
  wl1_ds_proc_enum_type ds_proc, wrm_freq_input_type band_info, wrm_duration_type duration, wrm_activity_enum_type activity, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_GRANT_CB

DESCRIPTION
  This function is called by WRM with lock status for WL1.

DEPENDENCIES
  None.

PARAMETERS
  trm_client_enum_t: The client which needs the RF resource
  event: The RF resource which is being granted
  tag: Anonymous payload passed in wrm_request_and_notify()

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_grant_cb(boolean grant, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_FIND_PROC_FROM_SETUP_CMD

DESCRIPTION
  This function is called with the cphy_setup_cmd pointeto find whether any dual
  sim procedure is associated with this command.

DEPENDENCIES
  This function does not acquire lock for the associated procedure. The caller 
  must call wl1_ds_req_and_not_lock_lock_for_proc() to acquire lock based
  on the procedure returned by this function.

PARAMETERS
  ext_cmd_ptr: cphy_setup_cmd pointer sent by RRC

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern wl1_ds_proc_enum_type wl1_ds_find_proc_from_setup_cmd(l1_ext_cmd_type *ext_cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_FIND_PROC_FROM_EXT_CMD

DESCRIPTION
  This function is called with the command pointer associated with any
  command from RRC. It finds whether any dual sim procedure is associated
  with this command.

DEPENDENCIES
  This function does not acquire lock for the associated procedure. The caller 
  must call wl1_ds_req_and_not_lock_lock_for_proc() to acquire lock based
  on the procedure returned by this function.

PARAMETERS
  ext_cmd_ptr: Command pointer associated with the command from upper layers

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern wl1_ds_proc_enum_type wl1_ds_find_proc_from_ext_cmd(l1_ext_cmd_type *ext_cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_FIND_PROC_FROM_SETUP_CNF

DESCRIPTION
  This function is called with the command pointer associated with any confirm
  sent from L1 to RRC. It finds whether any dual sim procedure is associated
  with this command.

DEPENDENCIES
  This function does not release/acquire lock for the associated procedure. The caller 
  must call wl1_ds_req_and_not_lock_lock_for_proc() or wrm_release() to
  acquire/release the lock.

PARAMETERS
  cnf_to_rrc_ptr: Confirm message sent by L1 to RRC.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern wl1_ds_proc_enum_type wl1_ds_find_proc_from_ext_cnf(rrc_cmd_type *cnf_to_rrc_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_IS_LOCK_NEEDED_FOR_EXT_CMD

DESCRIPTION
  This function is called with the command pointer associated with any
  command from RRC. It finds whether any dual sim procedure is associated
  with this command.

DEPENDENCIES
  This function does not acquire lock for the associated procedure. The caller 
  must call wl1_ds_req_and_not_lock_lock_for_proc() to acquire lock based
  on the procedure returned by this function.

PARAMETERS
  ext_cmd_ptr: Command pointer associated with the command from upper layers

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern wl1_ds_proc_enum_type wl1_ds_is_lock_needed_for_ext_cmd(l1_ext_cmd_type *ext_cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_LOCK_ACQ_SUCC

DESCRIPTION
  This function handles TRM lock acquistion success for various procedures.
  It re-initializes mdsp/RF and increments any counters to track
  success.

DEPENDENCIES
  This function should not re-init mdsp/RF for paging procedure.

PARAMETERS
  ds_proc: Current dual sim procedure

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_hdle_lock_acq_succ(wl1_ds_proc_enum_type ds_proc, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_CNF_SENT_TO_RRC

DESCRIPTION
  This function handles confirmations sent to RRC from dual_sim/trm_lock perspective.
  It releases the TRM lock if it is no longer needed and resets flags.

DEPENDENCIES
  None

PARAMETERS
  ds_proc: Current dual sim procedure

RETURN VALUE
  TRUE - send conf to RRC.
  FALSE - don't send the conf to RRC yet.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_ds_hdle_cnf_sent_to_rrc(rrc_cmd_type *cnf_to_rrc_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_LOCK_FAILURE_FOR_PAGING

DESCRIPTION
  This function is called when lock acquisition has failed for paging procedure.
  This function sends OOS indication to RRC if the consecutive paging lock
  acquisition failure count is greater than the preset threshold.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_hdle_lock_failure_for_paging(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_CHK_N_RESET_PCH_OOS_CRIT

DESCRIPTION
  This function checks if OOS ind is about to be sent by DSDS module to RRC,
  and in that case resets the PCH OOS timer to avoid sending a second OOS
  indication.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_chk_n_reset_pch_oos_crit( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_CHECK_FOR_OOS

DESCRIPTION
  This function sends OOS indication to RRC if the consecutive paging lock
  acquisition failure count is greater than the preset threshold.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_check_for_oos( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_POST_CELL_TRANS_CNF_CB

DESCRIPTION
  This function is called at the end of cell transition. It resets any paging
  related statistics.

DEPENDENCIES
  None

PARAMETERS
  status: the status of cell transition

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_post_cell_trans_cnf_cb(boolean status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_PRE_CELL_TRANS_REQ_CB

DESCRIPTION
  This function is called upon receiving the cell transition command.
  It sets the status based on the cell reselection status from RRC

DEPENDENCIES
  None

PARAMETERS
  status: the status of cell transition
  Reselection type: L1_ASET_UPD_CELL_RESELECTION_FAILURE - Treated as Failure
  Any other reselection type is treared as Success

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_pre_cell_trans_req_cb(boolean status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_LOCK_ACQ_FAIL

DESCRIPTION
  This function handles TRM lock acquistion failure for various procedures.
  It sends failure confirmation to RRC and increments any counters to track
  failure.

DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr: Command pointer associated with the command from upper layers
  ds_proc: Current dual sim procedure

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_hdle_lock_acq_fail(wl1_ds_proc_enum_type ds_proc, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_PREP_AFTER_LOCK_ACQ

DESCRIPTION
  This function is called when lock acquisition is successful for a procedure
  and WL1 needs to be prepare mdsp/rf. This does not handle lock acquisition
  case for Paging wakeups.

DEPENDENCIES
  None

PARAMETERS
  ds_proc: the procedure for which WL1 needs to be prepared

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_prep_after_lock_acq(wl1_ds_proc_enum_type ds_proc, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_ENTER_SLEEP

DESCRIPTION
  This function must be called when WL1 is about to start sleeping. This function must be called
  from SLEEP_START_INT.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_enter_sleep( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_EXIT_SLEEP

DESCRIPTION
  This function conducts the dual sim procedure needed when WL1 exits sleep.
  This function must be called from micro wakeup ISR context.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  TRUE: Lock acquistion for paging is successful
  FALSE: Lock acquistion for paging failed

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_ds_exit_sleep( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_CELL_RESEL_START

DESCRIPTION
  This function must be called at the start of cell reselection. This function will ensure that WL1 holds
  the TRM lock during the entire duration of cell reselection.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_hdle_cell_resel_start( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_PRE_MEAS_START_IND

DESCRIPTION
 This function requests for TRM lock extension for a specified duration of
 trm_duration_t slpclk units

DEPENDENCIES
  This function must be called before srchcr starts any measurements in PCH_SLEEP state.

PARAMETERS
  duration - time in slpclks for which lock is requested.

RETURN VALUE
  True - if lock extension succeeded
  False - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_ds_pre_meas_start_ind(trm_duration_t duration);

/*===========================================================================
FUNCTION     WL1_DS_SET_PAGING_LOCK_STATUS

DESCRIPTION
  This function sets the paging lock status with the passed value.

DEPENDENCIES
  This function must be called at the earliest after wakeup.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_set_paging_lock_status(boolean lock_status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_STATUS_CHG_IND

DESCRIPTION
  This function processes the Dual SIM status Indication from RRC. If UE is in
  dual sim mode, then WL1-RRC interface and sleep/wakeup procedures are
  impacted. This includes extra time needed for wakeup, mdsp/RF preparation
  for every wakeup and checking for TRM lock to process RRC request/indication.
  If UE is in single sim mode, then WL1 does not need TRM lock for every
  procedure and wakeup time can be reduced accordingly.

DEPENDENCIES
  WL1 can accept dual sim status indication only in do-nothing states.

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_hdle_status_chg_ind(l1_ds_status_change_ind_type *ds_cmd_ptr, wsub_id_e_type wsub_id);

extern void wl1_ds_hdle_sig(rex_sigs_type ds_sig, wsub_id_e_type wsub_id);

extern void wl1_ds_proc_pich_done( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_BPLMN_START

DESCRIPTION
  This function handles the start of BPLMN search by changing the current
  DS procedure mask from Paging to BPLMN

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_hdle_bplmn_start( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_BPLMN_STOP

DESCRIPTION
  This function handles the stop of BPLMN search by changing the current
  DS procedure mask from BPLMN to Paging

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_hdle_bplmn_stop( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_IND_DRX_CMD

DESCRIPTION
  This function handles the sets/resets any variable in wl1_ds upon the reception
  of a drx command from RRC.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_ind_drx_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_OK_TO_PROC_EXT_CMD_WITHOUT_LOCK

DESCRIPTION
  This function says whether an external command can be processed without
  TRM lock and mdsp/RF in pch_sleep state.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  TRUE: if external command can be processed without TRM lock
  FALSE: if external command cannot be processed without TRM lock

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_ds_ok_to_proc_ext_cmd_in_sleep(l1_ext_cmd_type *ext_cmd_ptr,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_L1M_WOKEN_UP

DESCRIPTION
  This function clears the WL1 in paging wakeup flag as L1M is moving to PCH state.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_l1m_woken_up( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION enchs_override_cqi

DESCRIPTION
  This function sets a flag as well as the CQI override value to the MDSP. 
  mDSP reads the flag at the R99 frame boundary and if set, overrides the 
  CQI with the value written by L1.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void enchs_override_cqi(boolean flag, uint8 val, wsub_id_e_type wsub_id);

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
/*==========================================================================

FUNCTION     wl1_query_qta_start

DESCRIPTION
  This function is written to be used by RxD module to freeze RxD before the
  start of QTA. 

  WL1 subsystems call this function to determine the state of Quick Tuneaway
  (QTA).  QTA is a mode where WCDMA has frozen its state for a brief period,
  ~30 msec, to allow GSM to check its paging occasion.  So during QTA, WL1
  subsystems should forgo significant activities, like mDSP accesses, RF
  accesses, etc., but continue whatever processing will allow them to start
  back up again quickly in the future.

  WL1 subsystems must call this function periodically in order to determine
  current and future QTA status.  And once QTA goes active, WL1 subsystems
  must call this function periodically to determine when QTA has ended.
  At that point, WL1 subsystems should resume normal processing.

DEPENDENCIES
  None.

PARAMETERS
  uint16 *next_qta_cfn  - Return CFN of future QTA gap to caller.  Set to
                          MAX_CFN_COUNT if no known QTA gap pending.
  uint16 *next_qta_slot - Return slot of future QTA gap.  Value undefined
                          if no known QTA gap pending.
RETURN VALUE
 None.
===========================================================================*/
extern void wl1_query_qta_start(uint16 *next_qta_cfn, 
                                      uint16 *next_qta_slot, 
                                        wsub_id_e_type);
/*==========================================================================

FUNCTION     wl1_set_reserve_fail_count_for_first_page

DESCRIPTION
 As G might have missed some pages during transition to FACH state(those missed pages we cant count), 
 request first QTA with force_reserve. So setting the reserve_fail_count to 2.

DEPENDENCIES
 None.

PARAMETERS
 None.

RETURN VALUE
 None.
===========================================================================*/
extern void wl1_set_reserve_fail_count_for_first_page(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_IS_NEXT_TUNEAWAY_PAGE_DECODE

DESCRIPTION
  This function is called by RRC to determine whether the next W2G tuneaway
  is PAGE Decode by GSM Sub or not.  It calls into TRM to check GSM's intentions
  for the next tuneaway.  If GSM's intentions are long, like cell reselection, 
  then return FALSE.  If GSM's intentions are short, like only check paging 
  occasion, then do quick tuneaway.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - Next Tuneaway is short Tuneaway.
  FALSE - Next Tuneaway could be long Tuneaway.
===========================================================================*/
extern boolean wrm_is_next_tuneaway_page_decode( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_IS_NEXT_TUNEAWAY_CHANNEL_MAINTENANCE_INV

DESCRIPTION
  This function is called by RRC to determine whether the next tuneaway
  is channel maintenance inversion or not

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - Next Tuneaway is channel maintenance inversion reason
  FALSE - Next Tuneaway is non channel maintenance inversion reason
===========================================================================*/
boolean wrm_is_next_tuneaway_channel_maintenance_inv(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_IS_NEXT_TUNEAWAY_QTA

DESCRIPTION
  This function is called by RRC to determine whether the next W2G tuneaway
  should be legacy variety or quick variety (QTA).  It calls into TRM to
  check GSM's intentions for the next tuneaway.  If GSM's intentions are long,
  like cell reselection, then do legacy tuneaway.  If GSM's intentions are
  short, like only check paging occasion, then do quick tuneaway.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - Use QTA for next tuneaway.
  FALSE - Use legacy for next tuneaway.
===========================================================================*/
extern boolean wrm_is_next_tuneaway_qta( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wrm_winning_client_for_tune_away

DESCRIPTION
  This function is called by RRC to determine the winning client for tune away.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
     Winning client id.
===========================================================================*/
extern trm_client_enum_t wrm_winning_client_for_tune_away(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_IS_FACH_RESERVATION_TIMER_ON

DESCRIPTION
  Returns wl1_fach_reservation_timer_on

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - Fach reservation timer is running
  FALSE - Fach reservation timer is not running
===========================================================================*/
extern boolean wl1_is_fach_reservation_timer_on( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_QXTA_TIMER_EXPIRED_CMD

DESCRIPTION
  This function makes decision to call wl1_qta_timer_expired_cmd or wl1_qdta_timer_expired_cmd
  based on current ta_info.

DEPENDENCIES
  None.

PARAMETERS
  wsub_id_e_type wsub_id

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_qxta_timer_expired_cmd( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_QTA_TIMER_EXPIRED_CMD

DESCRIPTION
  local_wl1multisim_cntrl->wl1_tuneaway_timer expired during quick tuneaway procedure, which has
  different steps and different times between steps.  This is local command
  handler invoked for WL1_QTA_TIMER_EXPIRED_CMD.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_qta_timer_expired_cmd(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_QDTA_TIMER_EXPIRED_CMD

DESCRIPTION
  wl1multisim_ext_cntrl_params[wsub_id].wl1_secondary_timer expired during quick div tuneaway procedure, 
  which has different steps and different times between steps. This is QDTA state's handler.

DEPENDENCIES
  None.

PARAMETERS
  wsub_id : Subsrciption Id

RETURN VALUE
  None.
===========================================================================*/

extern void wl1_qdta_timer_expired_cmd(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_QTA_UNLOCK_CANCEL_CMD

DESCRIPTION
  This is local command handler is invoked for WL1_QTA_UNLOCK_CANCEL_CMD.


DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_qta_unlock_cancel_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_CPHY_START_QTA_REQ_CMD

DESCRIPTION
  This function is an indication from RRC that we are to start W2G quick
  tuneaway process.

DEPENDENCIES
  None.

PARAMETERS
  l1_start_qta_cmd_type *start_qta_cmd - Unused.

RETURN VALUE
  None.
===========================================================================*/
extern void l1m_cphy_start_qta_req_cmd(l1_start_qta_cmd_type *start_qta_cmd, wsub_id_e_type wsub_id);

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_QTA)
/*===========================================================================
FUNCTION     wl1_get_qta_start_cfn

DESCRIPTION
  Returns the QTA start CFN.value.and whether the value is valid or not

DEPENDENCIES
  None.

PARAMETERS
  ref_time: unlock_by_timer value in sclk
  *cfn: Updates the cfn value in this pointer
  wsub_id: subscription id

RETURN VALUE
  Boolean TRUE: If cfn is valid
          FALSE: cfn is invalid
===========================================================================*/
extern boolean wl1_get_qta_start_cfn(uint32 ref_time, uint8 *cfn, wsub_id_e_type wsub_id);
#endif /* FEATURE_DUAL_SIM && FEATURE_QTA */

/*===========================================================================
FUNCTION     WL1_START_QDTA

DESCRIPTION
  This function invokes QDTA procedure.

DEPENDENCIES
  None.

PARAMETERS
  wsub_id_e_type wsub_id

RETURN VALUE
  None.
===========================================================================*/

extern void wl1_start_qdta(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     L1M_CPHY_STOP_QTA_REQ_CMD

DESCRIPTION
  This function is an indication from RRC that we are to end W2G quick
  tuneaway process, meaning resume WCDMA mode.

DEPENDENCIES
  None.

PARAMETERS
  l1_stop_qta_cmd_type *stop_qta_cmd - Unused.

RETURN VALUE
  None.
===========================================================================*/
extern void l1m_cphy_stop_qta_req_cmd(l1_stop_qta_cmd_type *stop_qta_cmd, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_STOP_QDTA_GRANT_CALLBACK

DESCRIPTION
  This function marks end of QDTA gap. 

DEPENDENCIES
  None.

PARAMETERS
  boolean grant 
  wsub_id_e_type wsub_id

RETURN VALUE
  None.
===========================================================================*/

extern void wl1_stop_qdta_grant_callback(boolean grant, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_stop_qcta_grant_callback

DESCRIPTION
  This function marks end of DBDC QTA gap on CA device. 

DEPENDENCIES
  None.

PARAMETERS
  boolean grant 
  wsub_id_e_type wsub_id

RETURN VALUE
  None.
===========================================================================*/

extern void wl1_stop_qcta_grant_callback(boolean grant, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_ok_for_qdta

DESCRIPTION
  Function to check whether wl1 can allow QDTA or not. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean - TRUE if QDTA request approved.  FALSE if not.
===========================================================================*/
extern boolean wl1_ok_for_qdta( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_QTA_START_QTA

DESCRIPTION
  This function is invoked by QXDM debug command to simulate getting start
  QTA command from RRC.

  This is what QXDM command looks like to simulate QTA from RRC:

  send_data 75 4 47 0 0x1E 0x00

  The last 2 values define the QTA duration in msec.  For example, the above
  sets QTA duration of 30 msec = 0x001E.  Swap the bytes of the hex value to
  use and enter on the command line separated by spaces.

DEPENDENCIES
  None.

PARAMETERS
  uint16 qta_duration_msec - How long simulated QTA should last, i.e. when
                             to send stop QTA.
RETURN VALUE
  None.
===========================================================================*/
extern void wl1_qta_start_qta(uint16 qta_duration_msec, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_QTA_COMMAND_DONE_ISR

DESCRIPTION
  This function handles the QTA START and STOP done indicatores. from WFW

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_qta_command_done_isr( wsub_id_e_type wsub_id);

/*=============================================================================

FUNCTION  eq_drop_eq_for_qta

DESCRIPTION:

 QTA manager calls this function 12ms before QTA enabled all the time without 
 knowledge of either EQ enabled or not If EQ has already enabled EQ controller 
 sends EQ disable command to FW,Deregister the call back with triage,demod and 
 CM.If EQ is not enabled, no need to do anything.
 
DEPENDENCIES:

None

PARAMETERS:

None

RETURN VALUES:

None
=============================================================================*/
extern void  eq_drop_eq_for_qta(void);

/*=============================================================================
FUNCTION void  eq_enable_eq_after_qta
DESCRIPTION:

 QTA manager calls this function after QTA gap completed.EQ controller validates
 weather CM and TD are on or not.If both of them are off  EQ controller sends 
 EQ enable command to FW and register the call back functions with triage,demod
 and CM.otherwise it is in EQ disable state only.

 DEPENDENCIES:
 
 CM and TD both should be off.
 
 PARAMETERS:
 
 None

 RETURN VALUES:
 
 None
=============================================================================*/
extern void  eq_enable_eq_after_qta(void);

/*===========================================================================

FUNCTION  wl1_pend_ext_cmd_processing_for_qdta

DESCRIPTION
  This function pends external commands processing during QDTA

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

extern void wl1_pend_ext_cmd_processing_for_qdta(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wl1_is_ext_cmd_processing_allowed_during_qdta

DESCRIPTION
  This function checks to pends/allow external command processing during QDTA

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_is_ext_cmd_processing_allowed_during_qdta(l1_ext_cmd_type *ext_cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_IS_QDTA_ACTIVE

DESCRIPTION  This function checks if QDTA is active or not...

DEPENDENCIES
  None.

RETURN VALUE  Boolean.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_is_qdta_active( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wl1_is_qcta

DESCRIPTION
  This function checks if DBDC QTA is for CA carrier alone or not...

DEPENDENCIES
  None.

RETURN VALUE
  Boolean

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_is_qcta(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WL1_IS_QTA_ACTIVE

DESCRIPTION  This function checks if QTA is active or not...

DEPENDENCIES
  None.

RETURN VALUE  Boolean.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_is_qta_active( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_trm_query_trm_multisim_config_info

DESCRIPTION
  This function is to query trm about the multi-sim configuration. To print UE is configured for SS/DSDS/TSTS/DSDA

DEPENDENCIES
  Assumed to be called after HS has indicated to start logging to DECHS

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern wl1_ds_multi_sim_config_enum_type wl1_ds_query_trm_multisim_config_info(wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION  WL1_IN_QTA_GAP

DESCRIPTION
  This function checks if QTA is active or not...

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_in_qta_gap( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  WL1_IS_WFW_IN_QTA_GAP

DESCRIPTION
  This function checks if WFW is in QTA or  not...

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_is_wfw_in_qta_gap( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  WL1_IN_QTA_CLEANUP

DESCRIPTION
  This function checks if QTA just ended and CRCs reported are valid.

DEPENDENCIES
  None.

RETURN VALUE
  True - if QTA just ended and CRCs reported are not valid.
  else False.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_in_qta_cleanup( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION  WL1_IN_QTA_CLEANUP_BCH

DESCRIPTION
  This function checks if QTA just ended and BCH CRCs can be processed.

DEPENDENCIES
  None.

RETURN VALUE
  True - if QTA ended in the last 20 ms and BCH CRC reports are not to be 
  processed.
  else False.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_in_qta_cleanup_for_bch( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  WL1_IN_FRAME_AFTER_QTA

DESCRIPTION
  This function checks if this is the first frame after QTA.

DEPENDENCIES
  None.

RETURN VALUE
  True - if QTA ended in the previous frame.
  else False.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_in_frame_after_qta( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION  WL1_PREPARING_FOR_QTA

DESCRIPTION
  This function checks if there is a QTA preparation going or not..

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_preparing_for_qta( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  WL1_QTA_CLEANUP

DESCRIPTION
  This function cleanup QTA related timers and variables if set.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_qta_cleanup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_QTA_UNLOCK_CANCEL_CB

DESCRIPTION
  Post a local command to handle QTA unlock cancel event.  
  Local command should help mitigate any concurrency
  problems with timer and task running in parallel.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_qta_unlock_cancel_cb( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  l1m_gsm_rf_nv_init_wait_handler

DESCRIPTION
  This function is callbacek function used for GSM RF NV init to wait
  on the NV init done signal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void l1m_gsm_rf_nv_init_wait_handler(rex_sigs_type sig);

/*===========================================================================

FUNCTION SRCHDCH_FREEZE_SRCH

DESCRIPTION 
    This function is used to freeze the search HB event and make note of the 
    time stamp which is used later to evaluate the elapsed time while resuming.

DEPENDENCIES
  
RETURN VALUE
    None.

SIDE EFFECTS
    Ignore the HB event, so srch_ops_ctl_parms count will not be updated 
    until resumed.
===========================================================================*/
extern void srchdch_freeze_srch(void);

/*===========================================================================

FUNCTION SRCHDCH_UNFREEZE_SRCH

DESCRIPTION 
    This function is used to unfreeze the suspended HB event,calculate the elasped 
    time and increment the srch_ops_ctl_parms like RSSI,ASET SEARCH etc. 
    accordingly.

DEPENDENCIES
  
RETURN VALUE
    None.

SIDE EFFECTS
   The value of srch_ops_ctl_parms will be update according to the elapsed time.
===========================================================================*/
extern void srchdch_unfreeze_srch(void);

/*===========================================================================

FUNCTION     wl1_waitfor_mdsp_download_complete

DESCRIPTION
  This function will wait for mdsp download completion on a TIMEOUT sig.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean.

SIDE EFFECTS
  None.

===========================================================================*/
extern void wl1_waitfor_mdsp_download_complete(void);

/*===========================================================================
FUNCTION     wl1_query_qta_activity

DESCRIPTION
  WL1 subsystems call this function to determine the state of Quick Tuneaway
  (QTA).  QTA is a mode where WCDMA has frozen its state for a brief period,
  ~30 msec, to allow GSM to check its paging occasion.  So during QTA, WL1
  subsystems should forgo significant activities, like mDSP accesses, RF
  accesses, etc., but continue whatever processing will allow them to start
  back up again quickly in the future.

  WL1 subsystems must call this function periodically in order to determine
  current and future QTA status.  And once QTA goes active, WL1 subsystems
  must call this function periodically to determine when QTA has ended.
  At that point, WL1 subsystems should resume normal processing.

DEPENDENCIES
  None.

PARAMETERS
  uint16 *next_qta_cfn  - Return CFN of future QTA gap to caller.  Set to
                          MAX_CFN_COUNT if no known QTA gap pending.
  uint8  *next_qta_slot - Return slot of future QTA gap.  Value undefined
                          if no known QTA gap pending.
RETURN VALUE
  TRUE  - WL1 is currently in the middle of a QTA gap.
  FALSE - WL1 is currently NOT in the middle of a QTA gap.
===========================================================================*/
extern boolean wl1_query_qta_activity(uint16 *, uint16 *, wsub_id_e_type wsub_id);
                               
/*===========================================================================

FUNCTION  WL1_QTA_CLEANUP_INIT

DESCRIPTION
  This function initializes the variables needed for QTA cleanup.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_qta_cleanup_init( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  WL1_QTA_CLEANUP_RESET

DESCRIPTION
  This function resets the variables used for QTA cleanup.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_qta_cleanup_reset( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     WL1_QTA_SKIP_OLPC

DESCRIPTION

  This fucntion is called every frame to check if OLPC need to skipped in QTA gap

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  Boolean value
     TRUE: SKIP olpc in current frame
     FALSE: Don't skip olpc in current frame
===========================================================================*/
extern boolean wl1_qta_skip_olpc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_QTA_UPDATE_FRAME_CNT_FOR_CLEANUP

DESCRIPTION
  This function updates frame count variables after QTA stop 
  to know if we are in max TTI gap after QTA

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_qta_update_frame_cnt_for_cleanup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_PREPARE_L1_FOR_FACH_QTA

DESCRIPTION
  This function is called by wrm to prepare WL1 for next W2G tuneaway if in FACH
  state. This function may make reservations with FRM well in advance to indicate
  the QTA in future. It may restart a timer to make these reservations at proper time
  in case time to qta is greater than WL1_QTA_FRM_ADVANCE_RESERV_TIME.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_prepare_l1_for_fach_qta( wsub_id_e_type wsub_id);

#endif  /* #ifdef FEATURE_QTA */

/*===========================================================================
FUNCTION     WL1_DS_TRM_RESERVE_OPS_GTS_IND

DESCRIPTION
  This function must be called when WL1 receives a GO TO SLEEP IND from RRC. 
  It must be called from l1m_process_ext_cmd() only.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_trm_reserve_ops_gts_ind( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_QTA_SET_DL_TDDET_PENDING

DESCRIPTION
  This function sets the flag that indicates a call to dl_cell_check_tddet_status
  is pending, and should be completed after the QTA gap is closed.

DEPENDENCIES
  None

PARAMETERS
  boolean flag - Sets the internal flag to the value provided.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_qta_set_dl_tddet_pending(boolean flag, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1TRM_TUNEAWAY_LOG_SET_TIME

DESCRIPTION
  This function provides a way to store the suspend start, suspend stop, resume
  start and the setup confirmation timeticks into the wl1trm_tuneaway_log_packet's
  data structure.

DEPENDENCIES
  None.
 
PARAMETERS 
  wl1trm_tuneaway_log_set_time_type time_type - Flag to indicate the event.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1trm_tuneaway_log_set_time(wl1trm_tuneaway_log_set_time_type time_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1TRM_TUNEAWAY_LOG_UPDATE_FACH_QTA_REASON_BITMASK

DESCRIPTION
  This function updates the search reason bitmask with the search that was cancelled
  due to impending QTA or search which cancels the QTA.

DEPENDENCIES
  None.

PARAMETERS 
  uint8 add_new_reason - The active or blocked client that needs to be added to the
  tuneaway log packet.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1trm_tuneaway_log_update_fach_qta_reason_bitmask(uint8 add_new_reason, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_ds_hdle_cell_decfg_done_cb

DESCRIPTION
  This is  a callback function after the cell de registration is done.

DEPENDENCIES
  None

PARAMETERS
  None  
  
RETURN VALUE  
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_ds_hdle_cell_decfg_done_cb( wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM_WCDMA_TUNEAWAY

/*===========================================================================
FUNCTION     wl1_read_w2g_tuneaway_config_from_nv

DESCRIPTION
  This function reads W2G tuneaway configuration items from EFS/NV one time
  at WCDMA start-up.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_read_w2g_tuneaway_config_from_nv( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_cqi_zero_timeout_cmd

DESCRIPTION
  Local command handler for L1_CQI_ZERO_TIMEOUT_CMD.  If RRC never suspends
  WL1 for tuneaway for some reason, we want to reset CQI from zero back to
  normal.  In theory this timeout should never happen, but handle just in case.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_cqi_zero_timeout_cmd(void);

/*===========================================================================
FUNCTION     wl1_start_cqi_zero_cmd

DESCRIPTION
  Local command handler for L1_START_CQI_ZERO_CMD.  It is time to override
  CQI to zero in preparation for inter-RAT tuneaway in the near future.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_start_cqi_zero_cmd(void);

/*===========================================================================
FUNCTION     wl1_prepare_for_tuneaway

DESCRIPTION
  This function is called to tell WL1 to prepare for a future inter-RAT
  tuneaway, and to ask WL1 for permission to perform that future tuneaway.

DEPENDENCIES
  None.

PARAMETERS
  uint32 unlock_by_sclk - Future sleep clock timetick value when RRC intends
                          to SUSPEND WL1 and start tuneaway.
RETURN VALUE
  boolean - TRUE if tuneaway request approved.  FALSE if not.
===========================================================================*/
extern boolean wl1_prepare_for_tuneaway(uint32 unlock_by_sclk);

/*===========================================================================
FUNCTION     wl1_return_tuneaway_duration

DESCRIPTION
  This function saves the duration of tuneaway filled by RRC.
  RRC fills the duration into the global tuneaway_duration but might reset it if they start another 
  Suspend WCDMA procedure. So save the value into internal L1 variable and keep.

DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_read_tuneaway_duration_from_rrc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_return_tuneaway_duration

DESCRIPTION
  This function returns duration of tuneaway, in frames to the caller.

DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
Tuneaway duration, in frames.
===========================================================================*/
extern uint16 wl1_return_tuneaway_duration( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1_reset_tuneaway_duration

DESCRIPTION
  This function resets duration of tuneaway to zero.

DEPENDENCIES
  None.

PARAMETERS

RETURN VALUE
  None.
===========================================================================*/
extern void wl1_reset_tuneaway_duration( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION  RRC_IMMEDIATE_PREEMPTION_CB

DESCRIPTION

  This is a call back function to be called in case of pre-emption.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.
===========================================================================*/
extern boolean rrc_immediate_preemption_cb 
(
  sys_modem_as_id_e_type wrrc_as_id,
  /* The client which is being informed of an event */
  trm_client_enum_t               client,

  /* The event being sent to the client */
  trm_unlock_event_enum_t         event,

  /* Sclk timestamp for TRM_UNLOCK_BY */
  uint32                          unlock_by_sclk
);

#endif  /* FEATURE_DUAL_SIM_WCDMA_TUNEAWAY */

/*===========================================================================
FUNCTION  WRM_DRX_EXT_CMD_PROC_RESUME

DESCRIPTION
 Signals WL1 to resume external cmd processing, once DRX is in the correct
 state.

DEPENDENCIES
  To be used only in G2W TA mode.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_drx_ext_cmd_proc_resume(void);

extern void wrm_drx_ext_cmd_pend_notf(void);

/*===========================================================================
FUNCTION     WRM_G2W_TA_RESERVE_TRM_LOCK

DESCRIPTION
  This function is used by WCDMA Idle scheduler to reserve TRM lock in G2W TA
  mode.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_g2w_ta_reserve_trm_lock(uint32 trm_reserve_sclk);

/*===========================================================================
FUNCTION     WRM_SWITCH_G2W_TA_REG_MODE_PROC

DESCRIPTION
  This function is used by WCDMA to switch WRM from G2W TA mode to default
  DSDS mode

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wrm_switch_g2w_ta_reg_mode_proc(void);

/*===========================================================================
FUNCTION     WL1_DS_GET_SUBREASON_FROM_PEND_PROC

DESCRIPTION
  This function is used to know the sub reason from the pending command
  
DEPENDENCIES
  None.

PARAMETERS


RETURN VALUE
sys_proc_type_e_type - sub reason

SIDE EFFECTS
  None.
===========================================================================*/
extern sys_proc_type_e_type wl1_ds_get_sub_reason_from_pend_proc(
  wsub_id_e_type wsub_id
);

/*===========================================================================
FUNCTION     WL1_DS_HDLE_PRIORITY_INVERSION

DESCRIPTION
  This function is used to handle the priority inversion algorithm. The function
takes the activity and decides if priority inversion needs to be performed. If so, 
it starts the timer and returns the priority (TRM reason) appropriately.
  
DEPENDENCIES
  None.

PARAMETERS
  wrm_primary_client_enum_type    wrm_client_id     -- WL1TRM client requesting for lock.
  wrm_activity_enum_type req_activity -- WRM activity for which 
    inversion is requested

RETURN VALUE
  trm_reason_enum_t -- TRM reason to be used in the lock request to TRM

SIDE EFFECTS
  None.
===========================================================================*/
extern trm_reason_enum_t wl1_ds_hdle_priority_inversion(
  /* The client which needs the RF resource */
  wrm_primary_client_enum_type wrm_client_id,
  wrm_activity_enum_type req_activity,
  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_POST_PRIO_INV_DUR_TIMER_EXPIRED_CMD
 
DESCRIPTION
  This function posts local command to handle prio_inv_timer expiry
 
DEPENDENCIES
  None.
 
PARAMETERS
  Current WCDMA cb_data
 
RETURN VALUE
  None

===========================================================================*/
extern void wl1_ds_post_prio_inv_dur_timer_expired_cmd(timer_cb_data_type cb_data);
/*===========================================================================

FUNCTION     WL1_DS_PRIORITY_INV_TMR_EXPIRED_CB

DESCRIPTION 
  This will do a req_notify with ACQ_HIGH priority after priority
  inversion timer for acq expires. 

DEPENDENCIES
  None.

PARAMETERS 
  wrm_client_id: client which called this API.
  wsub_id: Subsctiption ID.

RETURN VALUE
  Boolean to indicate whether alternate client released the lock or not.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_handle_prio_inv_dur_timer_expired_cmd(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_MAX_INV_DUR_TIMER_EXPIRED_CB

DESCRIPTION 
  This function is called when the timer for Max Allowed Invert duration 
  timer expires.

DEPENDENCIES
  None.

PARAMETERS 
  uint32 unused.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_max_inv_dur_timer_expired_cb(timer_cb_data_type cb_data);


extern boolean wrm_relinquish_alt_client_lock_for_split_acq
(
  /* The client which is attempting to gain complete control of TRM lock */
  wrm_primary_client_enum_type wrm_client_id
, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_TRM_REQ_SEC_CHAIN

DESCRIPTION
  This is a wrapper function that requests for secondary antenna

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_trm_req_sec_chain(void);

/*===========================================================================
FUNCTION     WL1_DS_HANDLE_TRM_PENDING_SIG

DESCRIPTION
  This function must be called to handle the lock_acq_sig which was
  pended upon getting a TRM grant when WL1 is in paging wakeup.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void wl1_ds_handle_trm_pending_sig( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_DS_HANDLE_PENDING_CALL_SETUP

DESCRIPTION
  This function checks if call setup is pending from RRC and puts RF/FW to
  sleep and releases the TRM lock for the chain we are holding. This will 
  facilitate handling of pending RRC cmd in WRM and getting the right chain
  for TX.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_handle_pending_call_setup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_qta_rota_nv_read

DESCRIPTION
  Creates nv and reads it to find whether TX ia allowed or not.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_qta_rota_nv_read( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_wcdma_ta_config_nv_read

DESCRIPTION
  Reads NV to configure all TA types.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_wcdma_ta_config_nv_read( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION  wl1multisim_ext_cntrl_params_init

DESCRIPTION
  This function initialises external parameters

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1multisim_ext_cntrl_params_init( void );

/*===========================================================================

FUNCTION  wl1multisim_int_cntrl_params_init

DESCRIPTION
  This function initialises internal parameters

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1multisim_int_cntrl_params_init( wsub_id_e_type wsub_id );

/*=========================================================================
FUNCTION    WL1_DS_FAIL_RESUME_ON_LONG_TUNEAWAY

DESCRIPTION
    This function is called upon resume to check if the tuneaway duration is 
    1. Greater than OOS duration in FACH state
    2. DCH state is not handled currently here as RRC will trigger RLF after 
       ACQ during RESUME if T313+N313 out-of-sync's occured

    For now we support only CELL_FACH state and this function will indicate 
    to fail resume if #1 above is TRUE

PARAMETERS
  wsub_id_e_type wsub_id - Subscription ID.

DEPENDENCIES
  None

RETURN VALUE
  Boolean
  True -- Tuneaway duration > OOS duration (CELL_FACH state)
  False -- Otherwise
=========================================================================*/
extern boolean wl1_ds_fail_resume_on_long_tuneaway(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  WL1_GET_REM_TIME_IN_MS

DESCRIPTION
  This function converts the remaining Sclks to ms..

DEPENDENCIES
  None.

RETURN VALUE
  int 64, reamining Sclks time in ms

SIDE EFFECTS
  None.
===========================================================================*/
extern int64 wl1_get_rem_time_in_ms(uint32 rem_sclk, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_EFACH_MSIM
/*===========================================================================
FUNCTION     WL1_DS_UPDATE_HS_DPCCH_SETUP_CB

DESCRIPTION
  This function is called by HS when HS_DPCCH setup has to be
  delayed until after QTA.

DEPENDENCIES
  None.

PARAMETERS
  cb_ptr: cb_ptr to be called to continue HS_DPCCH setup.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_ds_update_hs_dpcch_setup_cb(WL1_DS_HS_DPCCH_SETUP_CB_FUNC_TYPE *cb_ptr, wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_EFACH_MSIM */

/*=========================================================================
FUNCTION    wl1_ds_get_ctch_proc

DESCRIPTION
 This function is called upon when reserving/requesting lock for CTCH
 this will return the PROC to be used which will identify the LOCK priority

PARAMETERS
 sub id

DEPENDENCIES
 None
RETURN �VALUE
wl1_ds_proc_enum_type
  WL1_DS_PROC_CTCH_CRITICAL
  WL1_DS_PROC_CTCH
=========================================================================*/
extern wl1_ds_proc_enum_type wl1_ds_get_ctch_proc(wsub_id_e_type wsub_id);
/*=========================================================================
FUNCTION    wl1_ds_get_entity_proc

DESCRIPTION
 This function is called upon when reserving/requesting lock for BCH/CTCH
 this will return the PROC to be used which will identify the LOCK priority

PARAMETERS
handle ,sub id
 duration (in sclks) returned by comparing entity collisions

DEPENDENCIES
 None
RETURN �VALUE
wl1_ds_proc_enum_type
  WL1_DS_PROC_CTCH_CRITICAL
  WL1_DS_PROC_CTCH
  WL1_DS_PROC_CTCH_LOW
  WL1_DS_PROC_PAGING
  WL1_DS_PROC_BCCH
=========================================================================*/
extern wl1_ds_proc_enum_type  wl1_ds_get_entity_proc(drx_entity_handle_type handle_waking_up,  wrm_duration_type *duration, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1TRM_UPDATE_TUNEAWAY_LOG_DATA

DESCRIPTION
  This function copies current tuneaway data into the log pointer

DEPENDENCIES
  None.

PARAMETERS
  WCDMA_CXM_LOG_PACKET_type *log_ptr - log_ptr to fill in
  wsub_id_e_type wsub_id - Subscription ID

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1trm_update_tuneaway_log_data(WCDMA_CXM_LOG_PACKET_type *log_ptr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1TRM_TUNEAWAY_LOG_DATA_INIT

DESCRIPTION
  This function initializes the content of wl1trm_tuneaway_log_data if log_data_init = TRUE
  else it will decide whether to init the db at the next call

DEPENDENCIES
  None.

PARAMETERS
  boolean log_data_init.- initialise the db or test the condition to initialise the db
  wsub_id_e_type wsub_id = Subscription ID

RETURN VALUE
  boolean value which specifies if we are supposed to init the tuneaway db

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1trm_tuneaway_log_data_init(boolean log_data_init,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1TRM_LOG_TUNEAWAY_TYPE

DESCRIPTION
  This function returns the tuneaway type for the current ta

DEPENDENCIES
  None.

PARAMETERS
  wsub_id_e_type wsub_id = Subscription ID

RETURN VALUE
  wl1trm_tuneaway_log_type.

SIDE EFFECTS
  None.
===========================================================================*/
extern wl1trm_tuneaway_log_type wl1trm_tuneaway_type(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wl1multisim_determine_g2w_ta_mode

DESCRIPTION
  This function is used by WCDMA to determine if GERAN currently supports
  G2W TA

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean: To indicate if we are in g2w ta mode

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1multisim_determine_g2w_ta_mode(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wl1_multisim_x2w_qta_cb_handler

DESCRIPTION
  This function converts the remaining Sclks to ms..

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void wl1_multisim_x2w_qta_cb_handler(trm_qta_event_callback_data* qta_data, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wl1_multisim_x2w_qta_cmd_handler

DESCRIPTION
  This function handles local context operatoins for QTA ASYNC INFO 
  command from TRM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void wl1_multisim_x2w_qta_cmd_handler(trm_qta_event_callback_data* qta_data, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wl1_in_x2w_qta_gap

DESCRIPTION
  This function checks if QTA is active or not...

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean wl1_in_x2w_qta_gap(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wl1_multisim_get_rf_params_for_qta

DESCRIPTION
  This function returns the pointer to source tech assigned memory which stores RF param info for QTA

DEPENDENCIES
  None.

RETURN VALUE
  rfm_meas_common_param_type: Returns the pointer to mem allocated for QTA tune

SIDE EFFECTS
  None.
===========================================================================*/
extern rfm_meas_common_param_type* wl1_multisim_get_rf_params_for_qta(wsub_id_e_type wsub_id);

#else /* else for FEATURE_DUAL_SIM */
#define WL1_IN_DS_MODE() (FALSE)
#define WL1_IN_DUAL_ACTIVE_MODE() (FALSE)
#endif /* FEATURE_DUAL_SIM */

#ifdef FEATURE_WCDMA_QSH_DUMP
/*=========================================================================
FUNCTION    wl1_msim_dump_cb

DESCRIPTION
    This function will be called by QSH for dumping out global variables in case of a FATAL

PARAMETERS
   debug_dump_ptr: Pointer to memory location in which to dump the data
DEPENDENCIES
  None

RETURN VALUE
  
=========================================================================*/
void wl1_msim_dump_cb(void* );

#endif /* FEATURE_WCDMA_QSH_DUMP*/

#ifndef FEATURE_WCDMA_TABASCO_MODEM 
/*=========================================================================
FUNCTION    WL1_DS_IS_SRCH_ABORT_FOR_QTA_NEEDED

DESCRIPTION
  This function will be called to decide if searcher should be suspended 
  QTA gaps

PARAMETERS

DEPENDENCIES
  None

RETURN VALUE
Boolean - TRUE : Abort searches as QTA for 1x is in progress
          FALSE: Otherwise
  
=========================================================================*/
boolean wl1_ds_is_srch_abort_for_qta_needed(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_TABASCO_MODEM  */

/*=========================================================================
FUNCTION    WL1_DS_TOGGLE_PRIO_INV_FOR_SIB_READ

DESCRIPTION
  Function is called by RRC to ask L1 to start/stop priority inversion for a SIB read. For now
  this function is called during SIB read by RRC to increase from CHANNEL_MAINT
  CHANNEL_MAINT_INV after the SIB wait timer expiry.

PARAMETERS
  wsub_id - SUB Id of this current instance

DEPENDENCIES
  None

RETURN VALUE
  None  
=========================================================================*/
extern void wl1_ds_toggle_prio_inv_for_sib_read(boolean enable, wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    wrm_cancel_modify_band

DESCRIPTION
    This function will inform TRM that we want to cancel the current modify_band_request

PARAMETERS
   sub_id

DEPENDENCIES
  None

RETURN VALUE
   None  
=========================================================================*/
extern void wrm_cancel_modify_band(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    wl1_multisim_is_sleep_reqd

DESCRIPTION
    This function is determine, if Sleep is reqd. between Page to traffic 
    Transition.

PARAMETERS
     Sub Id.

DEPENDENCIES
     None

RETURN VALUE
     Boolean  
=========================================================================*/
extern boolean wl1_multisim_is_fw_sleep_reqd_for_image_trans(wsub_id_e_type wsub_id);

/*===========================================================================
 FUNCTION  WL1_GET_CURRENT_CHAN

 DESCRIPTION
   get the default band and channel info.
 
 DEPENDENCIES
   None

 RETURN VALUE
   uint16  

 SIDE EFFECTS
   None
===========================================================================*/
extern uint16 wl1_get_current_chan(wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    WL1_CAN_CURR_ACTIVITY_CONTINUE

DESCRIPTION
   This function tries to check if the current entity can continue with its
   ongoing activity.
   It tries to extend TRM lock with the same or updated activity/duration and
   alos checks if there is any WRM pending command to be handled

PARAMETERS
     Entity handle and SUB ID

DEPENDENCIES
     None

RETURN VALUE
     Boolean
     TRUE - can continue with the current activity
     FALSE - Stop current activity from continuing 
=========================================================================*/
extern boolean wl1_can_curr_activity_continue(drx_entity_handle_type handle,wsub_id_e_type wsub_id);

/*=========================================================================
FUNCTION    wl1_ds_is_acq_dr_in_prog

DESCRIPTION
    This function return if acq_in_dr is in progress or not by checking
    if sub which is querying for this info is IN_LOCK with either
    TRM_ACQUISITION_CELL_SELECT or TRM_ACQUISITION_CELL_SELECT_INV
    which are the ACQ in DR reasons

PARAMETERS
   wsub_id

DEPENDENCIES
  None

RETURN VALUE
   boolean: If acq in DR is in progress
=========================================================================*/
extern boolean wl1_ds_is_acq_dr_in_prog(wsub_id_e_type wsub_id);

/*===========================================================================
 FUNCTION  wl1_srch_fach_unreserve_frame_for_qta

 DESCRIPTION
   Unreserve with FRM if we have a QTA reservation
 
 DEPENDENCIES
   None

 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/
extern void wl1_srch_fach_unreserve_frame_for_qta(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WRM_IS_NEXT_TUNEAWAY_FOR_MEASUREMENT

DESCRIPTION
  This function is called  to determine whether the next tuneaway
  is for measurement or not.  It calls into TRM to check the other sub's intentions
  for the next tuneaway. If it is for measurement, return TRUE else return FALSE.

DEPENDENCIES
  None.

PARAMETERS
  wsub_id

RETURN VALUE
  TRUE  - Next Tuneaway is for measurement.
  FALSE - Next Tuneaway is not for measurement.
===========================================================================*/
extern boolean wrm_is_next_tuneaway_for_measurement(wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     WRM_SUSPEND_OPS_FOR_EXT_CMD_IN_PROGRESS
 
DESCRIPTION
  This function is called when for Ext Cmd in Progress
  Currently Used for only:
  1) IFHHO in progress
  2) Cell Transition is in Progress.
 
DEPENDENCIES
  None.
 
PARAMETERS
  Current WCDMA SUB ID
 
RETURN VALUE
  None
===========================================================================*/
extern void wrm_suspend_ops_for_ext_cmd_in_progress(wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     WRM_RESUME_OPS_AFTER_EXT_CMD_DONE
 
DESCRIPTION
  This function is called when cphy is ended
  Currently Used for only:
  1) IFHHO in progress
  2) Cell Transition is in Progress.
 
DEPENDENCIES
  None.
 
PARAMETERS
  Current WCDMA SUB ID
 
RETURN VALUE
  None
===========================================================================*/
extern void wrm_resume_ops_after_ext_cmd_done(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_preparing_for_lta_in_given_num_frames

DESCRIPTION
  This function returns TRUE if we are getting CPHY_SUSPEND_REQ with 
  in the given number of frames
  
DEPENDENCIES
 NONE

RETURN VALUE
 TRUE - if CPHY_SUSPEND_REQ is with in given number of frames 
 FALSE - otherwise

SIDE EFFECTS
  None
===========================================================================*/

extern boolean wl1_preparing_for_lta_in_given_num_frames(uint8 num_frames , wsub_id_e_type wsub_id );
/*===========================================================================
FUNCTION     WL1_MULTISIM_GET_RF_PATH_IN_X2W_QTA_GAP

DESCRIPTION
  When we got QTA callback from TRM for WCDMA operation earlier, we stored 
  the value of the target primary and diversity devices to be returned by this
  function now.

DEPENDENCIES
  None.

PARAMETERS
  wrm_resource_enum_type: Resource Type
  wsub_id_e_type: WCDMA Subscription ID

RETURN VALUE
  rfm_path_type - RF path in use by WCDMA operation.

SIDE EFFECTS
  None.
===========================================================================*/
#ifdef FEATURE_WCDMA_RF_USES_RFM_CONTEXT_INTERFACE
extern rfm_context_type wl1_multisim_get_rf_path_in_x2w_qta_gap( wrm_resource_enum_type resource, wsub_id_e_type wsub_id);
#else
extern rfm_path_type wl1_multisim_get_rf_path_in_x2w_qta_gap( wrm_resource_enum_type resource, wsub_id_e_type wsub_id);
#endif /*FEATURE_WCDMA_RF_USES_RFM_CONTEXT_INTERFACE*/
/*===========================================================================

FUNCTION  wl1_is_rfm_path_valid_in_x2w_gap

DESCRIPTION
  Function to find out if the current device/sig_path for a particular resource is valid or not
  inside x2w gap

DEPENDENCIES

RETURN VALUE
  Boolean : TRUE if valid, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/

boolean wl1_is_rfm_path_valid_in_x2w_gap(wrm_resource_enum_type resource, wsub_id_e_type wsub_id);

extern rfm_device_enum_type wl1_multisim_get_rf_device_in_x2w_qta_gap( wrm_resource_enum_type resource, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wrm_is_aps_disabled

DESCRIPTION
  API to check if APS is disabled

DEPENDENCIES
  None

RETURN VALUE
  TRUE : if APS is disabled
  FALSE : Otherwise


SIDE EFFECTS
  None
=========================================================================*/
boolean wrm_is_aps_disabled(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION  wl1_multisim_qta_get_source_client_is_lte

DESCRIPTION
  This function returns if LTE is the source client for this QTA

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - L+W QTA
  FALSE - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean wl1_multisim_qta_get_source_client_is_lte(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION	wl1_qsh_mdump_collect_dsds
  
DESCRIPTION
  API to collect the dumps for all DSDS intrl and ext cntrl params

  DEPENDENCIES
  None.

  PARAMETERS
  wsub_id - Subscription ID

  RETURN VALUE
  None

  SIDE EFFECTS
  None.
  ===========================================================================*/
  QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_dsds(wsub_id_e_type wsub_id);
  #endif
#endif /* WL1MULTISIM_H */
